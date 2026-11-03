/* "cardmgr" - Memory Card Manager
 * Handles all communication with save data and writing to memory cards.
 */
#include "version.h"
#include "manager/cardmgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "party.h"
#include <string.h>
#include <stddef.h>
#include "evt/evt_badgeshop.h"
#include "evt/evt_yuugijou.h"
#include "evt/evt_johoya.h"
#include "mario/mariost.h"
#include "libc_decls.h"

#define CARD_FILE_BYTES ((s32)(sizeof(CardData) + sizeof(CardSaveSlot) * 4))

#define CARD_REQUIRED_SECTOR_BYTES 0x2000

#define CARD_OP_IDLE 0
#define CARD_OP_WRITE 1
#define CARD_OP_WRITE_HEADER 2
#define CARD_OP_READ 3
#define CARD_OP_READ_ALL 4
#define CARD_OP_CREATE 5
#define CARD_OP_FORMAT 6
#define CARD_OP_DELETE 7
#define CARD_OP_STOPPED 8

//.rodata
#include "assets/_mariost_icon_tex.inc"
#include "assets/_mariost_icon_tlut.inc"
#include "assets/_mariost_banner_tex.inc"

#if VERSION <= VERSION_DEMO_US
#include "../src/manager/cardmgr_demo.inc"
#else
const char cardFileName[] = "mariost_save_file";
#if VERSION >= VERSION_US
#if VERSION == VERSION_EU
const char card_title_jp[0x18] = "ペーパーマリオＲＰＧ";
const char card_date_jp[0x20] = "%2d月%2d日のセーブデータです";
const char cardGameTitle[0xC] = "Paper Mario";
const char cardDateFormat[] = "%2d/%2d save data";
const char card_date_de[0x18] = "Spielstand vom: %2d/%2d";
const char card_date_fr[0x16] = "Sauvegarde du %2d/%2d";
const char card_date_es[0x1B] = "Datos guardados el %2d/%2d";
const char card_date_it[0x15] = "Salvataggio: %2d/%2d";
#else
const char cardGameTitle[] = "Paper Mario";
const char cardDateFormat[] = "%2d/%2d save data";
#endif
#else
const char cardGameTitle[] = "ペーパーマリオＲＰＧ";
const char cardDateFormat[0x20] = "%2d月%2d日のセーブデータです";
#endif

//.sbss
static CardWork* wp;

//.sdata
const char* version = VERSION_PICK("009", "010");

//local prototypes
static void check_sum(CardSaveSlot* save);
static void make_header(void);
static void make_data(void);

static void memcard_probe(void);
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
void memcard_open(void);
static void memcard_close(void);

void write_main(void);
void write_header_main(void);
void read_main(void);
void read_all_main(void);
void create_main(void);
static void format_main(void);
static void delete_main(void);

static BOOL headerCheck(void);
static inline BOOL headerCheck2(void);
static BOOL dataCheck(void);

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
    s32 timeout;

    // FAKE: extra volatile status read; removing it changes the object
    wp->flags;
    if (wp->flags & CARD_FLAG_BUSY) {
        CARDCancel(&wp->info);
        for (timeout = 1000000; timeout > 0; timeout--) {
            if (!(wp->flags & CARD_FLAG_BUSY)) {
                break;
            }
        }
        wp->flags &= ~CARD_FLAG_BUSY;
    }

    make_header();
    make_data();

    wp->chan = -1;
    wp->fileNo = -1;
    wp->serialNo = 0;
    wp->op = CARD_OP_IDLE;
    wp->result = CARD_RESULT_NOCARD;
    wp->flags = 0;
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

    make_header();
    make_data();

    wp->chan = -1;
    wp->fileNo = -1;
    wp->serialNo = 0;
    wp->op = CARD_OP_IDLE;
    wp->result = CARD_RESULT_NOCARD;
    wp->flags = 0;
    CARDInit();
}

// Descriptive name (not in the symbol maps); placeholder unk_800b0ed0, JP 0x800B0ED0. See docs/NAME_PROVENANCE.md.
/**
 * Starts watching slot A for a card being inserted or removed (memcard_watch_start);
 * also keeps cardIsExec busy for 10 frames while the probe settles. Not in the demo.
 */
void cardInsertWatchStart(void) {
    wp->prevInserted = wp->chan;
    wp->busyFrames = 10;
    wp->flags |= CARD_FLAG_INSERT_WATCH;
}

/** Stops the insert watch and clears its changed flag. */
// Descriptive name (not in the symbol maps); placeholder unk_800b0eac, JP 0x800B0EAC. See docs/NAME_PROVENANCE.md.
/** Stops the insert watch and clears its changed flag. */
void cardInsertWatchEnd(void) {
    wp->flags &= ~CARD_FLAG_INSERT_WATCH;
    wp->flags &= ~(CARD_FLAG_INSERT_CHANGED | CARD_FLAG_INSERT_WATCH);
}

/** TRUE once a card was inserted or removed in slot A since cardInsertWatchStart; the file select scripts poll it. */
// Descriptive name (not in the symbol maps); placeholder unk_800b0e80, JP 0x800B0E80. See docs/NAME_PROVENANCE.md.
/** TRUE once a card was inserted or removed in slot A since cardInsertWatchStart; the file select scripts poll it. */
BOOL cardInsertChanged(void) {
    if (wp->flags & CARD_FLAG_INSERT_WATCH && wp->flags & CARD_FLAG_INSERT_CHANGED) {
        return TRUE;
    }
    return FALSE;
}

/**
 * TRUE while a card operation is running or a card call is in flight (or the insert watch is settling).
 * Scripts wait on it, and mariost.c waits for it before a reset so a save is never cut off.
 */
BOOL cardIsExec(void) {
    if (wp == NULL) {
        return FALSE;
    }
    if (wp->flags & CARD_FLAG_EXEC) {
        return TRUE;
    }
    if (wp->flags & CARD_FLAG_BUSY) {
        return TRUE;
    }
    return wp->busyFrames != 0;
}

s32 cardGetCode(void) {
    return wp->result;
}

/**
 * Advances the running card operation by one step each frame (from mariost.c):
 * write, header write, read, read all, create, format; also samples slot A for the insert watch.
 * Each step starts one SDK call and the next frame continues once its callback has stored the result.
 * Delete (op 7) has no starter function.
 */
void cardMain(void) {
    s32 inserted;

    if (wp->flags & CARD_FLAG_INSERT_WATCH) {
        inserted = CARDProbe(0) ? 0 : -1; // -1 when no card answers in slot A, else 0
        if (wp->prevInserted != inserted) {
            wp->flags |= CARD_FLAG_INSERT_CHANGED;
        }
        wp->prevInserted = inserted;
        if (wp->busyFrames) {
            wp->busyFrames--;
        }
    }
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
            break;

        case CARD_OP_DELETE:
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
            break;

        case CARD_OP_STOPPED:
        default:
            break;
    }
}

/**
 * Loads a save slot into the running game: copies its global work, pouch, defeated-NPC records, badge shop,
 * Pianta Parlor and informant state over the live ones,
 * keeping the console settings (sound mode, rumble, last slot, progressive scan, and the language,
 * plus the 50/60 Hz choice on PAL) and runtime pointers. Called by the file select's load.
 */
void cardCopy2Main(s32 slot) {
    void* savedMapAlloc;
#if VERSION >= VERSION_US
    s32 savedSoundMode;
    CardSaveSlot* save;
    void* savedFbatData;
    s32 savedRumble;
    s32 savedLastSlot;
    u32 progressive;
    s32 language;
#else
    u32 progressive;
    CardSaveSlot* save;
    void* savedFbatData;
    s32 savedSoundMode;
    s32 savedRumble;
    s32 savedLastSlot;
#endif

#if VERSION == VERSION_EU
    s32 framerate;
#endif
    save = &wp->data->saves[slot];
    savedFbatData = gp->fbatData;
    savedMapAlloc = gp->mapAlloc;
    progressive = (u32)(gp->flags & GW_FLAG_PROGRESSIVE);
#if VERSION >= VERSION_US
    language = gp->language;
#if VERSION == VERSION_EU
    framerate = gp->framerate;
#endif
#endif
    savedSoundMode = gp->soundMode;
    savedRumble = gp->rumbleEnabled;
    savedLastSlot = gp->lastSlot;
    memcpy(gp, &save->data.global, sizeof(GlobalWork));
    gp->fbatData = savedFbatData;
    memcpy(pouchGetPtr(), &save->data.pouch, sizeof(PouchData));
    memcpy(fbatGetPointer()->deadInfo, save->data.deadInfo, sizeof(NpcDeadInfo) * 64);
    memcpy(bdsw, &save->data.badgeShop, sizeof(BadgeShopWork));
    memcpy(yuwp, &save->data.parlor, sizeof(PiantaParlorWork));
    memcpy(&_jdt.readFlags, &save->data.informant, sizeof(InformantUnknown));
    gp->resetType = 0;
    gp->resetButtonDown = 0;
#if VERSION >= VERSION_US
    gp->softResetWait = 0;
#endif
    gp->dvdError = 0;
    gp->retraceTime = OSGetTime();
    strcpy(gp->currentAreaName, "123");
    gp->relocationBase = NULL;
    gp->soundMode = savedSoundMode;
    gp->rumbleEnabled = savedRumble;
    gp->lastSlot = savedLastSlot;
    gp->mapAlloc = savedMapAlloc;
    gp->debugFlag = 0;
    gp->debugBattle = 0;
    if (progressive != 0) {
        gp->flags |= GW_FLAG_PROGRESSIVE;
    } else {
        gp->flags &= ~GW_FLAG_PROGRESSIVE;
    }
#if VERSION >= VERSION_US
    gp->language = language;
#if VERSION == VERSION_EU
    // PAL: loading a save keeps the current 50/60 Hz frame rate
    gp->framerate = framerate;
#endif
#endif
}

/** Starts erasing a save slot: writes an empty slot marked erased (flag 1) over both its copies on the card. */
void cardErase(s32 slotNo) {
    CardSaveSlot* save;

    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE;
        wp->currSlot = slotNo;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags |= CARD_FLAG_ERASE;
        save = wp->writeBuf;
        memset(save, 0, sizeof(save->data));
        save->data.flags |= CARD_SAVE_FLAG_ERASED;
        check_sum(save);
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

/**
 * Saves the game into a slot: records Mario's position, both partners, the save time and count,
 * then snapshots the global work, pouch, defeated-NPC records, badge shop, Pianta Parlor and informant state,
 * checksums it and starts the write.
 */
void cardWrite(s32 slot) {
    CardSaveSlot* save;
    MarioWork* mario;
    PartyEntry* party;
    s8 partyId;

    if (!(wp->flags & CARD_FLAG_EXEC)) {
        wp->op = CARD_OP_WRITE;
        wp->currSlot = slot;
        wp->opStep = 0;
        wp->result = CARD_RESULT_READY;
        wp->flags |= CARD_FLAG_EXEC;
        wp->flags &= ~(CARD_FLAG_DONE | CARD_FLAG_FAILED);
        wp->flags &= ~CARD_FLAG_ERASE;
        save = &wp->data->saves[slot];
        mario = marioGetPtr();
        memset(save, 0, sizeof(CardSaveSlot));
        gp->savePlayerPos = mario->position;
        party = partyGetPtr(marioGetPartyId());
        if (party != NULL) {
            partyId = party->currentMemberId;
        } else {
            partyId = mario->prevPartyMemberId[0];
        }
        gp->savePartyId[0] = partyId;
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != NULL) {
            partyId = party->currentMemberId;
        } else {
            partyId = mario->prevPartyMemberId[1];
        }
        gp->savePartyId[1] = partyId;
        gp->saveTime = OSGetTime();
        gp->saveCount++;
        gp->flags &= ~GW_FLAG_SAVE_LOADED;
        gp->flags &= ~GW_FLAG_NO_ENTRANCE;
        gp->resetType = 0;
#if VERSION >= VERSION_US
        gp->resetButtonDown = 0;
        gp->softResetWait = 0;
#endif
        gp->dvdError = 0;
        memcpy(&save->data.global, gp, sizeof(GlobalWork));
        memcpy(&save->data.pouch, pouchGetPtr(), sizeof(PouchData));
        memcpy(save->data.deadInfo, fbatGetPointer()->deadInfo, sizeof(save->data.deadInfo));
        memcpy(&save->data.badgeShop, bdsw, sizeof(BadgeShopWork));
        memcpy(&save->data.parlor, yuwp, sizeof(PiantaParlorWork));
        memcpy(&save->data.informant, &_jdt.readFlags, sizeof(_jdt.readFlags));
        save->data.flags &= ~CARD_SAVE_FLAG_ERASED;
        save->data.flags &= ~CARD_SAVE_FLAG_NO_VALID_COPY;
        check_sum(save);
    }
}

static void memcard_probe(void) {
    s32 memSize, sectorSize;
    s32 result;
    s32 i;
    s32 timeout;

    wp->flags |= CARD_FLAG_BUSY;
    timeout = 1000000;
    i = 0;
    while ((result = CARDProbeEx(0, &memSize, &sectorSize)) == CARD_RESULT_BUSY) {
        if (++i > timeout) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }

        // FAKE: self-assignment with no effect; without it the object changes
        timeout = (u32)timeout;
    }

    if (result == CARD_RESULT_READY && sectorSize != CARD_REQUIRED_SECTOR_BYTES) {
        result = CARD_RESULT_WRONGDEVICE;
    }
    if (result == CARD_RESULT_READY) {
        wp->result = CARD_RESULT_READY;
        wp->chan = 0;
    } else if (result == CARD_RESULT_WRONGDEVICE) {
        wp->result = CARD_RESULT_WRONGDEVICE;
        wp->chan = 0;
    } else {
        wp->result = result;
        wp->chan = -1;
    }
    wp->flags &= ~CARD_FLAG_BUSY;
}

/**
 * Step machine for cardWrite/cardCopy/cardErase:
 * mounts and checks the card, rewrites a bad header, checks which of the slot's two copies is valid,
 * and writes the new data to the other copy first and then over the valid one, so a power cut never loses both.
 * Ends by closing the file and unmounting.
 */
void write_main(void) {
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
                if (!headerCheck()) {
                    make_header();
                    memcard_write(wp->data, sizeof(CardMetadata), 0);
                }
                break;
            case 6:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (wp->currSlot * sizeof(CardSaveSlot))));
                break;
            case 7:
                wp->validSave1 = dataCheck();
                break;
            case 8:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)(sizeof(CardData) + (wp->currSlot * sizeof(CardSaveSlot))));
                break;
            case 9:
                wp->validSave2 = dataCheck();
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
                wp->flags |= CARD_FLAG_WRITTEN;
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
void write_header_main(void) {
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
                wp->flags |= CARD_FLAG_WRITTEN;
                wp->flags &= ~CARD_FLAG_EXEC;
                break;
        }
        wp->opStep++;
    } else if (CARDGetResultCode(wp->chan) != CARD_RESULT_BUSY && wp->result != CARDGetResultCode(wp->chan)) {
        wp->result = CARDGetResultCode(wp->chan);
        wp->flags &= ~CARD_FLAG_BUSY;
    }
}

/**
 * Step machine for reading one slot (opSlot): validates both copies
 * and loads the newer valid one into the file image, or marks the slot corrupt (flag 2) if neither is valid.
 * No starter in this file sets its op (3).
 */
void read_main(void) {
    s32 saveType; //invalid = -1, main = 0, backup = 1

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
                if (!headerCheck()) {
                    wp->flags |= CARD_FLAG_HEADER_INVALID;
                }
                break;
            case 6:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardMetadata)));
                break;
            case 7:
                wp->validSave1 = dataCheck();
                wp->lastSaveTime1 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 8:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardData)));
                break;
            case 9:
                wp->validSave2 = dataCheck();
                wp->lastSaveTime2 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 10:
                if (wp->validSave1 != 0 && wp->validSave2 != 0) {
                    if (wp->lastSaveTime1 > wp->lastSaveTime2) {
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
                wp->flags |= CARD_FLAG_WRITTEN;
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
        wp->flags &= ~CARD_FLAG_WRITTEN;
    }
}

void read_all_main(void) {
    s32 saveType; //invalid = -1, main = 0, backup = 1

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
                memcard_read(wp->data, sizeof(CardMetadata), 0);
                break;
            case 5:
                if (!headerCheck2()) {
                    make_header();
                    wp->flags |= CARD_FLAG_HEADER_INVALID;
                }
                gp->soundMode = wp->data->metadata.data.soundMode;
                gp->rumbleEnabled = wp->data->metadata.data.rumbleEnabled;
                gp->lastSlot = wp->data->metadata.data.lastSlot;
                break;
            case 6:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardMetadata)));
                break;
            case 7:
                wp->validSave1 = dataCheck();
                wp->lastSaveTime1 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 8:
                memcard_read(wp->readBuf, sizeof(CardSaveSlot), (s32)((wp->opSlot * sizeof(CardSaveSlot)) + sizeof(CardData)));
                break;
            case 9:
                wp->validSave2 = dataCheck();
                wp->lastSaveTime2 = ((CardSaveSlot*)wp->readBuf)->data.global.saveTime;
                break;
            case 10:
                if (wp->validSave1 != 0 && wp->validSave2 != 0) {
                    if (wp->lastSaveTime1 > wp->lastSaveTime2) {
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
                wp->flags |= CARD_FLAG_WRITTEN;
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
        wp->flags &= ~CARD_FLAG_WRITTEN;
        wp->flags &= ~CARD_FLAG_ERASE;
    }
}

/**
 * Step machine for cardCreate: deletes any old file, creates a 0x22000-byte file,
 * writes an empty header and slots, then sets the comment,
 * banner and animated icon in the card's directory entry.
 */
void create_main(void) {
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
                make_header();
                make_data();
                memcard_write(wp->data, sizeof(CardData), 0);
                break;
            case 6:
                memcard_write(wp->data->saves, sizeof(wp->data->saves), sizeof(CardData));
                break;
            case 7:
                memcard_get_stat();
                break;
            case 8:
                CARDSetCommentAddress(&wp->status, 0);
                CARDSetIconAddress(&wp->status, offsetof(CardMetadata, data.bannerTex));
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
                wp->flags |= CARD_FLAG_WRITTEN;
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
        wp->flags &= ~CARD_FLAG_WRITTEN;
    }
}

/** Step machine for formatting: mount, format, unmount. Unused: cardMain carries its own copy for op 6. */
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

/**
 * Step machine for deleting the save file: mount, check, delete, unmount.
 * Unused: cardMain carries its own copy (op 7).
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

/** Stamps a slot with the save version string and size and its checksums (byte sum and its complement). */
static void check_sum(CardSaveSlot* save) {
    s32 i;
    u8* temp;
    u32 checksum;

    strcpy(save->version, version);
    save->size = sizeof(save->data);
    save->checksum1 = (checksum = 0u);
    save->checksum2 = ~0u;

    temp = (u8*)&save->data;
    for (i = 0; i < (s32)sizeof(save->data); i++) {
        checksum += temp[i];
    }
    save->checksum1 = checksum;
    save->checksum2 = ~save->checksum1;
}

/** TRUE if the header just read into the read buffer has matching checksums. */
static BOOL headerCheck(void) {
    s32 i;
    s32 checksum;
    u8* temp;
    CardMetadata* metadata;

    metadata = wp->readBuf;
    if (metadata->checksum2 != ~metadata->checksum1) {
        return FALSE;
    } else {
        checksum = 0;
        temp = (u8*)&metadata->data;
        for (i = 0; i < (s32)sizeof(metadata->data); i++) {
            checksum += temp[i];
        }
        if (metadata->checksum1 != checksum) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

/** TRUE if the header in the file image has matching checksums (cardReadAll reads it straight there). */
static inline BOOL headerCheck2(void) {
    s32 i;
    s32 checksum;
    u8* temp;
    CardMetadata* metadata;

    metadata = &wp->data->metadata;
    if (metadata->checksum2 != ~metadata->checksum1) {
        return FALSE;
    } else {
        checksum = 0;
        temp = (u8*)&metadata->data;
        for (i = 0; i < (s32)sizeof(metadata->data); i++) {
            checksum += temp[i];
        }
        if (metadata->checksum1 != checksum) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

/**
 * TRUE if the slot in the read buffer has the right size, this release's version string and matching checksums,
 * so a save from a different version reads as corrupt.
 */
static BOOL dataCheck(void) {
    s32 i;
    u32 checksum;
    u8* temp;
    CardSaveSlot* save;

    save = wp->readBuf;
    if (save->size != sizeof(save->data)) {
        return FALSE;
    } else if (strcmp(save->version, version)) {
        return FALSE;
    } else if (save->checksum2 != ~save->checksum1) {
        return FALSE;
    } else {
        checksum = 0;
        temp = (u8*)&save->data;
        for (i = 0; i < (s32)sizeof(save->data); i++) {
            checksum += temp[i];
        }
        if (save->checksum1 != checksum) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

#if VERSION == VERSION_EU
/** Byte sum of the save file header (PAL: an inline; JP and US spell the loop out in make_header). */
static inline u32 header_sum(u8* data, s32 size) {
    s32 i;
    u32 sum;

    sum = 0;
    for (i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}
#endif

/** PAL: not inlined; the comment and its date order follow gp->language */
static void make_header(void) {
    CardMetadata* metadata;
#if VERSION != VERSION_EU
    s32 i;
    u8* temp;
    u32 checksum;
#endif
    OSCalendarTime td;

    metadata = &cardGetFilePtr()->metadata;
    memset(metadata, 0, sizeof(metadata->data));
#if VERSION == VERSION_EU
    OSTicksToCalendarTime(OSGetTime(), &td);
    switch (gp->language) {
        case 0:
            strcpy(metadata->data.fileName, card_title_jp);
            sprintf(metadata->data.dateString, card_date_jp, td.mon + 1, td.mday);
            break;
        case 1:
        case 6:
            strcpy(metadata->data.fileName, cardGameTitle);
            sprintf(metadata->data.dateString, cardDateFormat, td.mon + 1, td.mday);
            break;
        case 2:
            strcpy(metadata->data.fileName, cardGameTitle);
            sprintf(metadata->data.dateString, card_date_de, td.mday, td.mon + 1);
            break;
        case 3:
            strcpy(metadata->data.fileName, cardGameTitle);
            sprintf(metadata->data.dateString, card_date_fr, td.mday, td.mon + 1);
            break;
        case 4:
            strcpy(metadata->data.fileName, cardGameTitle);
            sprintf(metadata->data.dateString, card_date_es, td.mday, td.mon + 1);
            break;
        case 5:
            strcpy(metadata->data.fileName, cardGameTitle);
            sprintf(metadata->data.dateString, card_date_it, td.mday, td.mon + 1);
            break;
    }
#else
    strcpy(metadata->data.fileName, cardGameTitle);
    OSTicksToCalendarTime(OSGetTime(), &td);
    sprintf(metadata->data.dateString, cardDateFormat, td.mon + 1, td.mday);
#endif
    memcpy(metadata->data.bannerTex, _mariost_banner_tex, sizeof(metadata->data.bannerTex));
    memcpy(metadata->data.iconTex, _mariost_icon_tex, sizeof(metadata->data.iconTex));
    memcpy(metadata->data.iconTlut, _mariost_icon_tlut, sizeof(metadata->data.iconTlut));
    metadata->data.rumbleEnabled = (gp->rumbleEnabled != 0);
    metadata->data.soundMode = (u16)gp->soundMode;
    metadata->data.lastSlot = (u16)gp->lastSlot;
#if VERSION == VERSION_EU
    metadata->checksum1 = 0;
    metadata->checksum2 = ~0u;
    metadata->checksum1 = header_sum((u8*)&metadata->data, sizeof(metadata->data));
#else
    metadata->checksum1 = (checksum = 0u);
    metadata->checksum2 = ~0u;

    temp = (u8*)&metadata->data;
    for (i = 0; i < (s32)sizeof(metadata->data); i++) {
        checksum += temp[i];
    }
    metadata->checksum1 = checksum;
#endif
    metadata->checksum2 = ~metadata->checksum1;
}

/** Clears the four slots of the file image to erased, checksummed empty saves. */
static void make_data(void) {
    CardSaveSlot* save;
    s32 i;

    for (i = 0; i < 4; i++) {
        // FAKE: i is never -1 here; the plain save = &cardGetFilePtr()->saves[i] changes the object
        if (i != -1) {
            save = &cardGetFilePtr()->saves[i];
        } else {
            save = wp->writeBuf;
        }
        memset(save, 0, sizeof(save->data));
        save->data.flags |= CARD_SAVE_FLAG_ERASED;
        check_sum(save);
    }
}

/** Card-removed callback for CARDMountAsync; does nothing. */
void mountDetachCallback(s32 chan, s32 result) {
    ; //stubbed in retail
}

/**
 * Mount completion: stores the result, treating BROKEN and ENCODING as success
 * so the following check step reports them.
 */
void mountAttachCallback(s32 chan, s32 result) {
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
    while (result = CARDUnmount(wp->chan), result == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** CARDCheckAsync completion: stores the result and clears the in-flight flag. */
void checkCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Checks the card's file system asynchronously. */
static void memcard_check(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDCheckAsync(wp->chan, checkCallback);
}

/** CARDFormatAsync completion: stores the result and clears the in-flight flag. */
void formatCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Formats the card asynchronously. */
static void memcard_format(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDFormatAsync(wp->chan, formatCallback);
}

/** Deletes the save file, retrying while busy; a missing file counts as success. */
static void memcard_delete(void) {
    s32 result;
    s32 i;

    wp->flags |= CARD_FLAG_BUSY;

    i = 0;
    while (result = CARDDelete(wp->chan, cardFileName), result == CARD_RESULT_BUSY) {
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
void createCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Creates mariost_save_file with room for the header and the eight slot copies (0x22000 bytes). */
static void memcard_create(void) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDCreateAsync(wp->chan, cardFileName, CARD_FILE_BYTES, &wp->info, createCallback);
}

/** CARDWriteAsync completion: stores the result and clears the in-flight flag. */
void writeCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Writes length bytes at offset in the open save file asynchronously. */
static void memcard_write(void* addr, s32 length, s32 offset) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDWriteAsync(&wp->info, addr, length, offset, writeCallback);
}

/** CARDReadAsync completion: stores the result and clears the in-flight flag. */
void readCallback(s32 chan, s32 result) {
    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}

/** Reads length bytes at offset from the open save file asynchronously. */
static void memcard_read(void* addr, s32 length, s32 offset) {
    wp->flags |= CARD_FLAG_BUSY;
    CARDReadAsync(&wp->info, addr, length, offset, readCallback);
}

/** CARDSetStatusAsync completion: stores the result and clears the in-flight flag. */
void statusCallback(s32 chan, s32 result) {
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
    while (result = CARDGetStatus(wp->chan, wp->info.fileNo, &wp->status), result == CARD_RESULT_BUSY) {
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
    while (result = CARDGetSerialNo(wp->chan, &wp->serialNo), result == CARD_RESULT_BUSY) {
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
void memcard_open(void) {
    CARDStat stat;
    u64 serialNo;
    s32 byteNotUsed;
    s32 filesNotUsed;
    s32 i;
    s32 fileNo;
    s32 result;

    serialNo = 0;
    wp->flags |= CARD_FLAG_BUSY;
    if (wp->flags & CARD_FLAG_WRITTEN) {
        i = 0;
        while (result = CARDGetSerialNo(wp->chan, &serialNo), result == CARD_RESULT_BUSY) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if (result != CARD_RESULT_READY) {
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
    for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
        i = 0;
        while (result = CARDGetStatus(wp->chan, fileNo, &stat), result == CARD_RESULT_BUSY) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if ((result != CARD_RESULT_NOFILE) && (result != CARD_RESULT_NOPERM)) {
            if (result == CARD_RESULT_READY) {
                if (!strcmp(stat.fileName, cardFileName)) {
                    if ((stat.commentAddr == 0) && (stat.iconAddr == offsetof(CardMetadata, data.bannerTex)) && (stat.length == CARD_FILE_BYTES)) {
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
    if (fileNo >= CARD_MAX_FILE) {
        i = 0;
        while (result = CARDFreeBlocks(wp->chan, &byteNotUsed, &filesNotUsed), result == CARD_RESULT_BUSY) {
            if (++i > 1000000) {
                result = CARD_RESULT_FATAL_ERROR;
                break;
            }
        }
        if (result != CARD_RESULT_READY) {
            wp->result = result;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        if (byteNotUsed < CARD_FILE_BYTES) {
            wp->result = CARD_RESULT_INSSPACE;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        if (filesNotUsed < 1) {
            wp->result = CARD_RESULT_NOENT;
            wp->flags &= ~CARD_FLAG_BUSY;
            return;
        }
        wp->result = CARD_RESULT_NOFILE;
        wp->flags &= ~CARD_FLAG_BUSY;
        return;
    }
    i = 0;
    while (result = CARDOpen(wp->chan, cardFileName, &wp->info), result == CARD_RESULT_BUSY) {
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
    while (result = CARDClose(&wp->info), result == CARD_RESULT_BUSY) {
        if (++i > 1000000) {
            result = CARD_RESULT_FATAL_ERROR;
            break;
        }
    }

    wp->result = result;
    wp->flags &= ~CARD_FLAG_BUSY;
}
#endif
