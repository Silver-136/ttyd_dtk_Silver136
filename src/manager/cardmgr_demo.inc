// cardmgr.c as the early demos have it (from the LLCOOLT tree's demo view): a different file layout, most helpers inlined.
// The helpers the JP demo map lists as UNUSED (memcard_*, check_sum, headerCheck, dataCheck, make_default, make_header,
// make_data, cardCopy2Buffer, format_main, delete_main) are plain functions whose calls are all inlined; their
// out-of-line copies have the map's sizes (check_sum 0xa8, headerCheck 0xf0, dataCheck 0x118, make_default 0x2f8,
// make_header 0x1c0, make_data 0x130, cardCopy2Buffer 0x264) and they sit at the map's slots. The map's numbers of
// the unit's jump tables and strings (@1229 to @2276) also give the inlining: each step between two of them counts
// the labels and temporaries of the functions compiled in between, and all but one step equal the map's.

//.sbss
static CardWork* wp;

// Set once a card has been opened; memcard_open then reads its serial number.
// The demo keeps it in bit 0x4000 (JP and later keep it in 0x2000 and use 0x4000 for the insert watch).
#define CARD_FLAG_SERIAL 0x4000

//.sdata
static const char* version = VERSION_PICK("009", "010");

// local prototypes
static u32 check_sum(u8* data, s32 size);
static void make_default(void);
static void make_header(void);
static void make_data(s32 slot);

static void memcard_probe(void); // the demo calls it, the later versions inline it
static void memcard_mount(void);
static void memcard_unmount(void);
static void memcard_check(void);
static void memcard_format(void);
static void memcard_delete(void);
static void memcard_create(void);
static void memcard_write(void* addr, s32 length, s32 offset);
static void memcard_read(void* addr, s32 length, s32 offset);
static void memcard_set_stat(void);
static void memcard_get_stat(void);
static void memcard_get_serial(void);
static void memcard_open(void);
static void memcard_close(void);

static void mountDetachCallback(s32 chan, s32 result);
static void mountAttachCallback(s32 chan, s32 result);
static void checkCallback(s32 chan, s32 result);
static void formatCallback(s32 chan, s32 result);
static void deleteCallback(s32 chan, s32 result);
static void createCallback(s32 chan, s32 result);
static void writeCallback(s32 chan, s32 result);
static void readCallback(s32 chan, s32 result);
static void statusCallback(s32 chan, s32 result);

static void write_main(void);
static void write_header_main(void);
static void read_main(void);
static void read_all_main(void);
static void create_main(void);
static void format_main(void);
static void delete_main(void);

static BOOL headerCheck(void* buf);
static BOOL dataCheck(void* buf);

/**
 * The in-memory copy of the save file: the header (comment, banner, icon, settings)
 * and the four save slots the file select screen lists.
 */
CardData* cardGetFilePtr(void) {
    return wp->data;
}

/**
 * Cancels any card access in flight and resets the in-memory save file to an empty header and four erased slots;
 * the memory card scripts call it before loading the file again.
 * The demo keeps its insert-watch flag across the reset.
 */
void cardBufReset(void) {
    s32 flags;
    s32 timeout;

    flags = wp->flags;
    if (wp->flags & CARD_FLAG_BUSY) {
        CARDCancel(&wp->info);
        for (timeout = 1000000; timeout > 0; timeout--) {
            if (!(wp->flags & CARD_FLAG_BUSY)) {
                break;
            }
        }
        wp->flags &= ~CARD_FLAG_BUSY;
    }

    make_default();

    wp->chan = -1;
    wp->fileNo = -1;
    wp->serialNo = 0;
    wp->op = CARD_OP_IDLE;
    wp->result = CARD_RESULT_NOCARD;
    wp->flags = 0;
    if (flags & CARD_FLAG_INSERT_WATCH) {
        wp->flags |= CARD_FLAG_INSERT_WATCH;
    }
}

/**
 * Boot-time setup: allocates the card work, the SDK work area,
 * the save file image and the read and write slot buffers,
 * builds an empty file image and starts the CARD library.
 */
void cardInit(void) {
    wp = __memAlloc(HEAP_DEFAULT, sizeof(CardWork));
    memset(wp, 0, sizeof(CardWork));
    wp->workArea = __memAlloc(HEAP_DEFAULT, CARD_WORKAREA_SIZE);
    memset(wp->workArea, 0, CARD_WORKAREA_SIZE);
    wp->data = __memAlloc(HEAP_DEFAULT, sizeof(CardData));
    memset(wp->data, 0, sizeof(CardData));
    wp->readBuf = __memAlloc(HEAP_DEFAULT, sizeof(CardSaveSlot));
    memset(wp->readBuf, 0, sizeof(CardSaveSlot));
    wp->writeBuf = __memAlloc(HEAP_DEFAULT, sizeof(CardSaveSlot));
    memset(wp->writeBuf, 0, sizeof(CardSaveSlot));

    make_default();

    wp->chan = -1;
    wp->fileNo = -1;
    wp->serialNo = 0;
    wp->op = CARD_OP_IDLE;
    wp->result = CARD_RESULT_NOCARD;
    wp->flags = 0;
    CARDInit();
}

/**
 * TRUE while a card operation is running or a card call is in flight (or busyFrames is nonzero; nothing in the
 * early demos sets it).
 * Scripts wait on it, and mariost.c waits for it before a reset so a save is never cut off.
 */
BOOL cardIsExec(void) {
    if (wp->flags & CARD_FLAG_EXEC) {
        return TRUE;
    }
    if (wp->flags & CARD_FLAG_BUSY) {
        return TRUE;
    }
    return wp->busyFrames != 0;
}

/** Result of the last card operation as a CARD_RESULT_* code (READY on success, NOCARD, BROKEN, INSSPACE, ...). */
s32 cardGetCode(void) {
    return wp->result;
}

/**
 * Advances the running card operation by one step each frame (from mariost.c):
 * write, header write, read, read all, create, format (no insert watch in the early demos).
 * Each step starts one SDK call and the next frame continues once its callback has stored the result.
 * Read (op 3) and delete (op 7) are started only by cardRead and cardDelete, which nothing calls.
 */
void cardMain(void) {
    switch (wp->op) {
        case CARD_OP_WRITE:
            write_main();
            break;

        case CARD_OP_WRITE_HEADER:
            write_header_main();
            break;

        case CARD_OP_READ:
            read_main();
            break;

        case CARD_OP_READ_ALL:
            read_all_main();
            break;

        case CARD_OP_CREATE:
            create_main();
            break;

        case CARD_OP_FORMAT:
            format_main();
            break;

        case CARD_OP_DELETE:
            delete_main();
            break;

        case CARD_OP_STOPPED:
        default:
            break;
    }
}

/**
 * Snapshots the running game into a save slot of the file image (the reverse of cardCopy2Main): records Mario's
 * position, both partners, the save time and count, then copies the global work, pouch, defeated-NPC records,
 * badge shop, Pianta Parlor and informant state and stamps the slot with the version, size and checksums.
 */
// Original name, size (0x264) and place (between cardMain and cardCopy2Main) from the JP demo map, which lists it as
// UNUSED: its only call, in cardWrite, is inlined. The compiler inlines a function of at most 30 statements, which
// the two partner lines fit as conditional expressions (as if/else blocks the function has 35 and stays a call).
void cardCopy2Buffer(s32 slot) {
    CardSaveSlot* save;
    MarioWork* mario;
    PartyEntry* party;

    save = &wp->data->saves[slot];
    mario = marioGetPtr();
    memset(save, 0, sizeof(CardSaveSlot));
    gp->savePlayerPos = mario->position;
    party = partyGetPtr(marioGetPartyId());
    gp->savePartyId[0] = (party != NULL) ? party->currentMemberId : mario->prevPartyMemberId[0];
    party = partyGetPtr(marioGetExtraPartyId());
    gp->savePartyId[1] = (party != NULL) ? party->currentMemberId : mario->prevPartyMemberId[1];
    gp->saveTime = OSGetTime();
    gp->saveCount++;
    gp->flags &= ~GW_FLAG_SAVE_LOADED;
    gp->flags &= ~GW_FLAG_NO_ENTRANCE;
    gp->resetType = 0;
    memcpy(&save->data.global, gp, sizeof(GlobalWork));
    memcpy(&save->data.pouch, pouchGetPtr(), sizeof(PouchData));
    memcpy(save->data.deadInfo, fbatGetPointer()->deadInfo, sizeof(save->data.deadInfo));
    memcpy(&save->data.badgeShop, bdsw, sizeof(BadgeShopWork));
    memcpy(&save->data.parlor, yuwp, sizeof(PiantaParlorWork));
    memcpy(&save->data.informant, &_jdt.readFlags, sizeof(_jdt.readFlags));
    save->data.flags &= ~CARD_SAVE_FLAG_ERASED;
    save->data.flags &= ~CARD_SAVE_FLAG_NO_VALID_COPY;
    strcpy(save->version, version);
    save->size = sizeof(save->data);
    save->checksum1 = 0;
    save->checksum2 = ~0u;
    save->checksum1 = check_sum((u8*)&save->data, sizeof(save->data));
    save->checksum2 = ~save->checksum1;
}

/**
 * Loads a save slot into the running game: copies its global work, pouch, defeated-NPC records, badge shop,
 * Pianta Parlor and informant state over the live ones,
 * keeping the console settings (sound mode, rumble, last slot, progressive scan) and runtime pointers.
 * Called by the file select's load.
 */
void cardCopy2Main(s32 slot) {
    CardSaveSlot* save;
    void* backup1;
    void* backup2;
    s32 backup4;
    s32 backup5;
    s32 backup6;
    u32 backup3;

    save = &wp->data->saves[slot];
    backup1 = gp->fbatData;
    backup2 = gp->mapAlloc;
    backup3 = (u32)(gp->flags & GW_FLAG_PROGRESSIVE);
    backup4 = gp->soundMode;
    backup5 = gp->rumbleEnabled;
    backup6 = gp->lastSlot;
    memcpy(gp, &save->data.global, sizeof(GlobalWork));
    gp->fbatData = backup1;
    memcpy(pouchGetPtr(), &save->data.pouch, sizeof(PouchData));
    memcpy(fbatGetPointer()->deadInfo, save->data.deadInfo, sizeof(NpcDeadInfo) * 64);
    memcpy(bdsw, &save->data.badgeShop, sizeof(BadgeShopWork));
    memcpy(yuwp, &save->data.parlor, sizeof(PiantaParlorWork));
    memcpy(&_jdt.readFlags, &save->data.informant, sizeof(InformantReadFlags));
    gp->resetType = 0;
    gp->retraceTime = OSGetTime();
    strcpy(gp->currentAreaName, "123");
    gp->relocationBase = NULL;
    gp->soundMode = backup4;
    gp->rumbleEnabled = backup5;
    gp->lastSlot = backup6;
    gp->mapAlloc = backup2;
    gp->debugFlag = 0;
    gp->debugBattle = 0;
    if (backup3 != 0) {
        gp->flags |= GW_FLAG_PROGRESSIVE;
    } else {
        gp->flags &= ~GW_FLAG_PROGRESSIVE;
    }
}

/** Starts erasing a save slot: writes an empty slot marked erased (flag 1) over both its copies on the card. */
void cardErase(s32 slotNo) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE;
        wp->currSlot = slotNo;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags |= CARD_FLAG_ERASE;
        make_data(-1);
    }
}

/** Starts copying save slot from into slot to (file select's Copy), writing both copies of the target on the card. */
void cardCopy(s32 from, s32 to) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE;
        wp->currSlot = to;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags |= CARD_FLAG_ERASE;
        memcpy(wp->writeBuf, &wp->data->saves[from], sizeof(CardSaveSlot));
    }
}

/** Saves the game into a slot: snapshots the running game into the slot of the file image and starts the write. */
void cardWrite(s32 slot) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE;
        wp->currSlot = slot;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags &= ~CARD_FLAG_ERASE;
        cardCopy2Buffer(slot);
    }
}

/**
 * Step machine for cardWrite/cardCopy/cardErase:
 * mounts and checks the card, rewrites a bad header, checks which of the slot's two copies is valid,
 * and writes the new data to the other copy first and then over the valid one, so a power cut never loses both.
 * Ends by closing the file and unmounting.
 */
static void write_main(void) {
    if (wp->result) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;
            case 1:
                memcard_mount();
                break;
            case 2:
                memcard_check();
                break;
            case 3:
                memcard_open();
                break;
            case 4:
                memcard_read(wp->readBuf, sizeof(CardMetadata), 0);
                break;
            case 5:
                if (!headerCheck(wp->readBuf)) {
                    make_header();
                    memcard_write(wp->data, sizeof(CardMetadata), 0);
                }
                break;
            case 6:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot))));
                break;
            case 7:
                wp->validSave1 = dataCheck(wp->readBuf);
                break;
            case 8:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardData) + (wp->currSlot * sizeof(CardSaveSlot))));
                break;
            case 9:
                wp->validSave2 = dataCheck(wp->readBuf);
                break;
            case 10:
                if (wp->validSave1 && !wp->validSave2) {
                    wp->writeFirst = 1;
                    wp->writeSecond = 0;
                } else {
                    wp->writeFirst = 0;
                    wp->writeSecond = 1;
                }
                if (wp->flags & CARD_FLAG_ERASE) {
                    memcard_write(wp->writeBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot)) + (wp->writeFirst * (sizeof(CardSaveSlot) * 4))));
                } else {
                    memcard_write(&wp->data->saves[wp->currSlot], sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot)) + (wp->writeFirst * (sizeof(CardSaveSlot) * 4))));
                }
                break;
            case 11:
                if (wp->flags & CARD_FLAG_ERASE) {
                    memcard_write(wp->writeBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot)) + (wp->writeSecond * (sizeof(CardSaveSlot) * 4))));
                } else {
                    memcard_write(&wp->data->saves[wp->currSlot], sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot)) + (wp->writeSecond * (sizeof(CardSaveSlot) * 4))));
                }
                break;
            case 12:
                memcard_get_serial();
                break;
            case 13:
                memcard_close();
                break;
            case 14:
                memcard_unmount();
                break;
            case 15:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags |= CARD_FLAG_SERIAL;
                wp->flags &= ~CARD_FLAG_EXEC;
                if (wp->flags & CARD_FLAG_ERASE) {
                    wp->flags &= ~CARD_FLAG_ERASE;
                    memcpy(&wp->data->saves[wp->currSlot], wp->writeBuf, sizeof(CardSaveSlot));
                }
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts rewriting just the save file header (comment, date and settings like sound mode and rumble). */
void cardWriteHeader(void) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE_HEADER;
        wp->currSlot = 0;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags &= ~CARD_FLAG_ERASE;
        make_header();
    }
}

/** Step machine for cardWriteHeader: mount, check, open, write the header block, close, unmount. */
static void write_header_main(void) {
    if (wp->result) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;
            case 1:
                memcard_mount();
                break;
            case 2:
                memcard_check();
                break;
            case 3:
                memcard_open();
                break;
            case 4:
                memcard_write(wp->data, sizeof(CardMetadata), 0);
                break;
            case 5:
                memcard_get_serial();
                break;
            case 6:
                memcard_close();
                break;
            case 7:
                memcard_unmount();
                break;
            case 8:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags |= CARD_FLAG_SERIAL;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts reading one save slot from the card (read_main, op 3). */
// Original name and size (0x68) from the JP demo map (UNUSED there: nothing calls it); the map lists it right before
// read_main. The body is cardReadAll's (the same size) with read_main's operation and the slot it reads.
void cardRead(s32 slot) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_READ;
        wp->opSlot = slot;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED | CARD_FLAG_HEADER_INVALID);
        wp->flags &= ~CARD_FLAG_SERIAL;
    }
}

/**
 * Step machine for reading one slot (opSlot): validates both copies
 * and loads the newer valid one into the file image, or marks the slot corrupt (flag 2) if neither is valid.
 * Its starter cardRead has no caller.
 */
static void read_main(void) {
    s32 saveType; // invalid = -1, main = 0, backup = 1

    if (wp->result) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;
            case 1:
                memcard_mount();
                break;
            case 2:
                memcard_check();
                break;
            case 3:
                memcard_open();
                break;
            case 4:
                memcard_read(wp->readBuf, sizeof(CardMetadata), 0);
                break;
            case 5:
                if (!headerCheck(wp->readBuf)) {
                    wp->flags |= CARD_FLAG_HEADER_INVALID;
                }
                break;
            case 6:
                memcard_read(wp->readBuf, 0x4000, (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardMetadata)));
                break;
            case 7:
                wp->validSave1 = dataCheck(wp->readBuf);
                wp->lastSaveTime1 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 8:
                memcard_read(wp->readBuf, 0x4000, (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardData)));
                break;
            case 9:
                wp->validSave2 = dataCheck(wp->readBuf);
                wp->lastSaveTime2 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 10:
                if (wp->validSave1 != 0 && wp->validSave2 != 0) {
                    if (!(wp->lastSaveTime1 <= wp->lastSaveTime2)) {
                        saveType = 0;
                    } else {
                        saveType = 1;
                    }
                } else if (wp->validSave1 == 0 && wp->validSave2 != 0) {
                    saveType = 1;
                } else if (wp->validSave1 != 0 && wp->validSave2 == 0) {
                    saveType = 0;
                } else {
                    saveType = -1;
                }
                if (saveType != -1) {
                    memcard_read(&wp->data->saves[wp->opSlot], sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->opSlot * sizeof(CardSaveSlot)) + (saveType * (sizeof(CardSaveSlot) * 4))));
                } else {
                    wp->data->saves[wp->opSlot].data.flags |= CARD_SAVE_FLAG_NO_VALID_COPY;
                }
                break;
            case 11:
                memcard_get_serial();
                break;
            case 12:
                memcard_close();
                break;
            case 13:
                memcard_unmount();
                break;
            case 14:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags |= CARD_FLAG_SERIAL;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts reading the whole save file from slot A: header and all four slots (file select start-up). */
void cardReadAll(void) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_READ_ALL;
        wp->opSlot = 0;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED | CARD_FLAG_HEADER_INVALID);
        wp->flags &= ~CARD_FLAG_SERIAL;
    }
}

/**
 * Step machine for cardReadAll: loads the header (rebuilding it if corrupt, CARD_FLAG_HEADER_INVALID)
 * and takes the sound mode, rumble and last slot settings from it,
 * then for each of the four slots picks the newer valid copy, or marks it corrupt.
 */
static void read_all_main(void) {
    s32 saveType; // invalid = -1, main = 0, backup = 1

    if (wp->result) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
        return;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;
            case 1:
                memcard_mount();
                break;
            case 2:
                memcard_check();
                break;
            case 3:
                memcard_open();
                break;
            case 4:
                memcard_read(wp->data, 0x2000, 0);
                break;
            case 5:
                if (!headerCheck(&wp->data->metadata)) {
                    make_header();
                    wp->flags |= CARD_FLAG_HEADER_INVALID;
                }
                gp->soundMode = wp->data->metadata.data.soundMode;
                gp->rumbleEnabled = wp->data->metadata.data.rumbleEnabled;
                gp->lastSlot = wp->data->metadata.data.lastSlot;
                break;
            case 6:
                memcard_read(wp->readBuf, 0x4000, (wp->opSlot << 0xE) + 0x2000);
                break;
            case 7:
                wp->validSave1 = dataCheck(wp->readBuf);
                wp->lastSaveTime1 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 8:
                memcard_read(wp->readBuf, 0x4000, (wp->opSlot << 0xE) + 0x12000);
                break;
            case 9:
                wp->validSave2 = dataCheck(wp->readBuf);
                wp->lastSaveTime2 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 10:
                if (wp->validSave1 != 0 && wp->validSave2 != 0) {
                    if (!(wp->lastSaveTime1 <= wp->lastSaveTime2)) {
                        saveType = 0;
                    } else {
                        saveType = 1;
                    }
                } else if (wp->validSave1 == 0 && wp->validSave2 != 0) {
                    saveType = 1;
                } else if (wp->validSave1 != 0 && wp->validSave2 == 0) {
                    saveType = 0;
                } else {
                    saveType = -1;
                }
                if (saveType != -1) {
                    memcard_read(&wp->data->saves[wp->opSlot], sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->opSlot * sizeof(CardSaveSlot)) + (saveType * (sizeof(CardSaveSlot) * 4))));
                } else {
                    wp->data->saves[wp->opSlot].data.flags |= CARD_SAVE_FLAG_NO_VALID_COPY;
                }
                wp->opSlot++;
                if (wp->opSlot < 4) {
                    wp->opStep = 3;
                }
                break;
            case 11:
                memcard_get_serial();
                break;
            case 12:
                memcard_close();
                break;
            case 13:
                memcard_unmount();
                break;
            case 14:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags |= CARD_FLAG_SERIAL;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts creating the save file on the card, replacing any old one (evt_memcard's create, from the file select). */
void cardCreate(void) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_CREATE;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED | CARD_FLAG_HEADER_INVALID);
        wp->flags &= ~CARD_FLAG_SERIAL;
        wp->flags &= ~CARD_FLAG_ERASE;
    }
}

/**
 * Step machine for cardCreate: deletes any old file, creates a 0x22000-byte file,
 * writes an empty header and slots, then sets the comment,
 * banner and animated icon in the card's directory entry.
 */
static void create_main(void) {
    if (wp->result) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;
            case 1:
                memcard_mount();
                break;
            case 2:
                memcard_check();
                break;
            case 3:
                memcard_delete();
                break;
            case 4:
                memcard_create();
                break;
            case 5:
                make_default();
                memcard_write(wp->data, 0x12000, 0);
                break;
            case 6:
                memcard_write(wp->data->saves, 0x10000, 0x12000);
                break;
            case 7:
                memcard_get_stat();
                break;
            case 8:
                CARDSetCommentAddress(&wp->status, 0);
                CARDSetIconAddress(&wp->status, 0x40);
                CARDSetBannerFormat(&wp->status, CARD_STAT_BANNER_RGB5A3);
                CARDSetIconAnim(&wp->status, CARD_STAT_ANIM_LOOP);
                CARDSetIconFormat(&wp->status, 0, CARD_STAT_ICON_C8);
                CARDSetIconSpeed(&wp->status, 0, CARD_STAT_SPEED_MIDDLE);
                CARDSetIconFormat(&wp->status, 1, CARD_STAT_ICON_NONE);
                CARDSetIconSpeed(&wp->status, 1, CARD_STAT_SPEED_END);
                break;
            case 9:
                memcard_set_stat();
                break;
            case 10:
                memcard_get_serial();
                break;
            case 11:
                memcard_close();
                break;
            case 12:
                memcard_unmount();
                break;
            case 13:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags |= CARD_FLAG_SERIAL;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts formatting the card in slot A (evt_memcard's format, offered by the file select for an unusable card). */
void cardFormat(void) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_FORMAT;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED | CARD_FLAG_HEADER_INVALID);
        wp->flags &= ~CARD_FLAG_SERIAL;
    }
}

/** Step machine for formatting: mount, format, unmount. */
static void format_main(void) {
    if (wp->result != CARD_RESULT_READY) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;

            case 1:
                memcard_mount();
                break;

            case 2:
                memcard_format();
                break;

            case 3:
                memcard_unmount();
                break;

            case 4:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Starts deleting the save file from the card (delete_main, op 7). */
// Original name and size (0x60) from the JP demo map (UNUSED there: nothing calls it); the map lists it right before
// delete_main. The body is cardFormat's (the same size) with delete_main's operation.
void cardDelete(void) {
    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_DELETE;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED | CARD_FLAG_HEADER_INVALID);
        wp->flags &= ~CARD_FLAG_SERIAL;
    }
}

/**
 * Step machine for deleting the save file: mount, check, delete, unmount.
 * Nothing starts it: cardDelete has no caller.
 */
static void delete_main(void) {
    if (wp->result != CARD_RESULT_READY) {
        wp->op = CARD_OP_STOPPED;
        wp->flags |= CARD_FLAG_FAILED;
        wp->flags &= ~CARD_FLAG_EXEC;
    } else if (!(wp->flags & CARD_FLAG_BUSY)) {
        switch (wp->opStep) {
            case 0:
                memcard_probe();
                break;

            case 1:
                memcard_mount();
                break;

            case 2:
                memcard_check();
                break;

            case 3:
                memcard_delete();
                break;

            case 4:
                memcard_unmount();
                break;

            case 5:
                wp->op = CARD_OP_IDLE;
                wp->flags |= CARD_FLAG_DONE;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/** Byte sum; the file header and each save slot store it and its complement as their checksums. */
static u32 check_sum(u8* data, s32 size) {
    s32 i;
    u32 sum;

    sum = 0;
    for (i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * TRUE if the file header at buf has matching checksums: the read buffer after a header read, or the file image
 * that cardReadAll reads straight into.
 */
static BOOL headerCheck(void* buf) {
    CardMetadata* metadata;

    metadata = buf;
    if (metadata->checksum2 != ~metadata->checksum1) {
        return FALSE;
    } else {
        if (metadata->checksum1 != check_sum((u8*)&metadata->data, sizeof(metadata->data))) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

/**
 * TRUE if the slot at buf (always the read buffer) has the right size, this release's version string and matching
 * checksums, so a save from a different version reads as corrupt.
 */
static BOOL dataCheck(void* buf) {
    CardSaveSlot* save;

    save = buf;
    if (save->size != sizeof(save->data)) {
        return FALSE;
    } else if (strcmp(save->version, version)) {
        return FALSE;
    } else if (save->checksum2 != ~save->checksum1) {
        return FALSE;
    } else {
        if (save->checksum1 != check_sum((u8*)&save->data, sizeof(save->data))) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

/** Builds an empty save file image: a fresh header and four erased slots. */
// Original name, size (0x2f8) and place from the JP demo map (UNUSED: its three calls are inlined).
static void make_default(void) {
    s32 i;

    make_header();
    for (i = 0; i < 4; i++) {
        make_data(i);
    }
}

/**
 * Builds the save file header: the game title (the Japanese one)
 * and today's date comment shown in the GameCube memory card screen, the banner and icon images, and the sound,
 * rumble and last slot settings, then its checksums.
 */
static void make_header(void) {
    CardMetadata* metadata;
    OSCalendarTime td;
    metadata = &cardGetFilePtr()->metadata;
    memset(metadata, 0, sizeof(metadata->data));
    strcpy(metadata->data.fileName, "ペーパーマリオＲＰＧ");
    OSTicksToCalendarTime(OSGetTime(), &td);
    sprintf(metadata->data.dateString, "%2d月%2d日のセーブデータです", td.mon + 1, td.mday);
    memcpy(metadata->data.bannerTex, _mariost_banner_tex, sizeof(metadata->data.bannerTex));
    memcpy(metadata->data.iconTex, _mariost_icon_tex, sizeof(metadata->data.iconTex));
    memcpy(metadata->data.iconTlut, _mariost_icon_tlut, sizeof(metadata->data.iconTlut));
    metadata->data.rumbleEnabled = gp->rumbleEnabled ? 1 : 0;
    metadata->data.soundMode = (u16)gp->soundMode;
    metadata->data.lastSlot = (u16)gp->lastSlot;
    metadata->checksum1 = 0;
    metadata->checksum2 = ~0u;
    metadata->checksum1 = check_sum((u8*)&metadata->data, sizeof(metadata->data));
    metadata->checksum2 = ~metadata->checksum1;
}

/**
 * Makes a slot an erased, checksummed empty save: one of the four slots of the file image, or with -1 the slot
 * staged for writing (cardErase).
 */
static void make_data(s32 slot) {
    CardSaveSlot* save;

    if (slot != -1) {
        save = &cardGetFilePtr()->saves[slot];
    } else {
        save = wp->writeBuf;
    }
    memset(save, 0, sizeof(save->data));
    save->data.flags |= CARD_SAVE_FLAG_ERASED;
    strcpy(save->version, version);
    save->size = sizeof(save->data);
    save->checksum1 = 0;
    save->checksum2 = ~0u;
    save->checksum1 = check_sum((u8*)&save->data, sizeof(save->data));
    save->checksum2 = ~save->checksum1;
}

// Probes slot A and records the result in the card work. The demo also tests a slot B result that it never
// probes. BUG: resultB is never set; the demo binary compares r31, a callee-saved register this function
// never writes, so the slot B branches test whatever the caller left there. The later versions inline it.
static void memcard_probe(void) {
    s32 memSize;
    s32 sectorSize;
    s32 result;
    // the declaration order sets the registers: the demo's out-of-line copy and the inlined copies need opposite orders
    s32 resultB;
    s32 timeout;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;
    timeout = 1000000;
    i = 0;
    while ((result = CARDProbeEx(0, &memSize, &sectorSize)) == CARD_RESULT_BUSY) {
        if (++i > timeout) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
        timeout = (u32)timeout; // FAKE: no-op self-assignment the target compiles to a register copy; see FAKES.md
    }

    if (result == CARD_RESULT_READY && sectorSize != 0x2000) {
        result = CARD_RESULT_WRONGDEVICE;
    }
    if (result == CARD_RESULT_READY) {
        wp->result = CARD_RESULT_READY;
        wp->chan = 0;
    } else if (resultB == CARD_RESULT_READY) {
        wp->result = CARD_RESULT_READY;
        wp->chan = 1;
    } else if (result == CARD_RESULT_WRONGDEVICE) {
        wp->result = CARD_RESULT_WRONGDEVICE;
        wp->chan = 0;
    } else if (resultB == CARD_RESULT_WRONGDEVICE) {
        wp->result = CARD_RESULT_WRONGDEVICE;
        wp->chan = 1;
    } else {
        wp->result = result;
        wp->chan = -1;
    }
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Card-removed callback for CARDMountAsync; does nothing. */
static void mountDetachCallback(s32 chan, s32 result) {
    ; // empty
}

/**
 * Mount completion: stores the result, treating BROKEN and ENCODING as success
 * so the following check step reports them.
 */
static void mountAttachCallback(s32 chan, s32 result) {
    wp->result = result;
    if (result == CARD_RESULT_BROKEN || result == CARD_RESULT_ENCODING) {
        wp->result = CARD_RESULT_READY;
    }
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Mounts the card in the chosen slot asynchronously, in fast mode. */
static void memcard_mount(void) {
    CARDSetFastMode(TRUE);
    wp->flags |= CARD_FLAG_BUSY;
    CARDMountAsync(wp->chan, wp->workArea, mountDetachCallback, mountAttachCallback);
}

/** Unmounts the card, retrying while busy. */
static void memcard_unmount(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;
    i = 0;
    while ((result = CARDUnmount(wp->chan)) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** CARDCheckAsync completion: stores the result and clears the in-flight flag. */
static void checkCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Checks the card's file system asynchronously. */
static void memcard_check(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDCheckAsync(wp->chan, checkCallback);
}

/** CARDFormatAsync completion: stores the result and clears the in-flight flag. */
static void formatCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Formats the card asynchronously. */
static void memcard_format(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDFormatAsync(wp->chan, formatCallback);
}

/** Delete completion for an asynchronous delete: a missing file counts as success. Nothing uses it. */
// Original name, size (0x28) and place from the JP demo map (UNUSED there: memcard_delete calls CARDDelete and waits).
// The body is the other callbacks' (0x1c) with memcard_delete's NOFILE test, as mountAttachCallback has its own.
static void deleteCallback(s32 chan, s32 result) {
    if (result == CARD_RESULT_NOFILE) {
        result = CARD_RESULT_READY;
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Deletes the save file, retrying while busy; a missing file counts as success. */
static void memcard_delete(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;

    i = 0;
    while ((result = CARDDelete(wp->chan, "mariost_save_file")) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }

    if (result == CARD_RESULT_NOFILE) {
        result = CARD_RESULT_READY;
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** CARDCreateAsync completion: stores the result and clears the in-flight flag. */
static void createCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Creates mariost_save_file with room for the header and the eight slot copies (0x22000 bytes). */
static void memcard_create(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDCreateAsync(wp->chan, "mariost_save_file", sizeof(CardData) + (sizeof(CardSaveSlot) * 4), &wp->info, createCallback);
}

/** CARDWriteAsync completion: stores the result and clears the in-flight flag. */
static void writeCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Writes length bytes at offset in the open save file asynchronously. */
static void memcard_write(void* addr, s32 length, s32 offset) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDWriteAsync(&wp->info, addr, length, offset, writeCallback);
}

/** CARDReadAsync completion: stores the result and clears the in-flight flag. */
static void readCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Reads length bytes at offset from the open save file asynchronously. */
static void memcard_read(void* addr, s32 length, s32 offset) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDReadAsync(&wp->info, addr, length, offset, readCallback);
}

/** CARDSetStatusAsync completion: stores the result and clears the in-flight flag. */
static void statusCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Writes the directory entry (comment, banner and icon settings) of the save file asynchronously. */
static void memcard_set_stat(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDSetStatusAsync(wp->chan, wp->info.fileNo, &wp->status, statusCallback);
}

/** Reads the save file's directory entry into wp->status, retrying while busy. */
static void memcard_get_stat(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;

    i = 0;
    while ((result = CARDGetStatus(wp->chan, wp->info.fileNo, &wp->status)) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Reads the card's serial number, which memcard_open later compares to spot a swapped card. */
static void memcard_get_serial(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;

    i = 0;
    while ((result = CARDGetSerialNo(wp->chan, &wp->serialNo)) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }

    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/**
 * Opens the save file.
 * If a card was opened before, it first refuses (NOPERM) a card with a different serial number.
 * Finds mariost_save_file in the directory; a file of the wrong layout gives NOFILE.
 * With no file it reports why one cannot be made (INSSPACE, NOENT) or NOFILE when there is room.
 */
static void memcard_open(void) {
    CARDStat stat;
    u64 serialNo;
    s32 byteNotUsed;
    s32 filesNotUsed;
    s32 i;
    s32 fileNo;
    s32 result;

    serialNo = 0;
    wp->flags |= CARD_FLAG_BUSY;
    if (wp->flags & CARD_FLAG_SERIAL) {
        i = 0;
        while ((result = CARDGetSerialNo(wp->chan, &serialNo)) == -1) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if (result != 0) {
            wp->result = result;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        if (serialNo != wp->serialNo) {
            wp->result = CARD_RESULT_NOPERM;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
    }
    for (fileNo = 0; fileNo < 127; fileNo++) {
        i = 0;
        while ((result = CARDGetStatus(wp->chan, fileNo, &stat)) == CARD_RESULT_BUSY) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if ((result != CARD_RESULT_NOFILE) && (result != CARD_RESULT_NOPERM)) {
            if (result == CARD_RESULT_READY) {
                if (!strcmp(stat.fileName, "mariost_save_file")) {
                    if ((stat.commentAddr == 0) && (stat.iconAddr == 0x40) && (stat.length == 0x22000)) {
                        wp->fileNo = fileNo;
                        break;
                    } else {
                        wp->fileNo = fileNo;
                        wp->result = CARD_RESULT_NOFILE;
                        wp->flags &= ~CARD_FLAG_BUSY;
                        return;
                    }
                }
            } else {
                if ((result == CARD_RESULT_FATAL_ERROR) || (result == CARD_RESULT_NOCARD)) {
                    wp->result = result;
                    wp->flags &= ~CARD_FLAG_BUSY;
                    return;
                }
            }
        }
    }
    if (fileNo >= 127) {
        i = 0;
        while ((result = CARDFreeBlocks(wp->chan, &byteNotUsed, &filesNotUsed)) == CARD_RESULT_BUSY) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if (result != 0) {
            wp->result = result;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        if (byteNotUsed < 0x22000) {
            wp->result = CARD_RESULT_INSSPACE;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        if (filesNotUsed < 1) {
            wp->result = CARD_RESULT_NOENT;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        wp->result = -4;
        wp->flags &= ~CARD_FLAG_BUSY;
        return;
    }
    i = 0;
    while ((result = CARDOpen(wp->chan, "mariost_save_file", &wp->info)) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Closes the save file, retrying while busy. */
static void memcard_close(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;

    i = 0;
    while ((result = CARDClose(&wp->info)) == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }

    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}
