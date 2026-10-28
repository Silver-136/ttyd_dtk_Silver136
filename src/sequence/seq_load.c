#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include "seq/seq_load.h"
#include "sdk_decls.h"
#include "win/win_mario.h"
#include "sfx_name_word.h"
#include "nameent.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "libc_decls.h"
#include "sound.h"
#include "demo/DEMOPad.h"

#include "countdown.h"
#include "driver/fadedrv.h"
#include "driver/seqdrv.h"
#include "driver/swdrv.h"
#include "event.h"
#include "manager/cardmgr.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/filemgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/os.h>

#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include <string.h>
#include <stdio.h>
#include "driver/camdrv.h"
#include "manager/fontmgr.h"
#include "driver/windowdrv.h"
#include "driver/icondrv.h"
#include "win/win_main.h"
#include "sdk/texPalette.h"
#include "icon_ids.h"
#ifndef VERSION_G8ME01
#include "evt/evt_cmd.h"
#endif
// LoadWork.flags (file select)
typedef enum LoadWorkFlags {
    LOAD_FLAG_DIM = (1 << 0),          // 0x1
    LOAD_FLAG_NO_DELETE = (1 << 4),    // 0x10
    LOAD_FLAG_NO_COPY = (1 << 5),      // 0x20
    LOAD_FLAG_HIDE_CURSOR = (1 << 6),  // 0x40
    LOAD_FLAG_RECHECK = (1 << 7),      // 0x80
} LoadWorkFlags;

typedef struct LoadWork {
    u16 flags;         // 0x0, LoadWorkFlags
    FileEntry* file;   // 0x4
    f32 x;             // 0x8
    f32 y;             // 0xC
    f32 tx;            // 0x10
    f32 ty;            // 0x14
    s32 state;         // 0x18, state machine step
    s32 cardCode;      // 0x1C, last cardGetCode() result
    s32 slot;          // 0x20
    s32 soundModeSel;  // 0x24, sound mode being chosen (0 mono, 1 stereo, 2 surround)
    s32 rumbleOffSel;  // 0x28, rumble-off choice being edited
    f32 alpha;         // 0x2C
    s32 copyFrom;      // 0x30, copy: source slot
    s32 copyTo;        // 0x34, copy: destination slot
    const char* msg;   // 0x38
    EventEntry* evt;   // 0x3C
    s32 soundMode;     // 0x40, sound mode in effect
    s32 rumbleOff;     // 0x44, 1 = rumble disabled
    s32 printWindowId; // 0x48, saved evt->printWindowId
} LoadWork;

typedef struct WinDt {
    f32 x;      // 0x0
    f32 y;      // 0x4
    f32 ofsX;   // 0x8
    f32 ofsY;   // 0xC
    s32 tex;    // 0x10, window texture id (winTexSet)
    s32 tex2;   // 0x14, second texture (winTexSet_x2)
    u8 next[4]; // 0x18
} WinDt;

double sin(double);
void FontDrawString(f32 x, f32 y, const char* str);
void loadMain(SequenceWork* seq);
void loadDraw(CameraId camera, void* param);
void continueGame(void);

WinDt win_dt[9] = {
    {-140.0f, 75.0f, -120.0f, 60.0f, 13, 0, {7, 2, 1, 1}},
    {140.0f, 75.0f, -120.0f, 60.0f, 12, 0, {8, 3, 0, 0}},
    {-140.0f, -65.0f, -120.0f, 60.0f, 11, 0, {0, 4, 3, 3}},
    {140.0f, -65.0f, -120.0f, 60.0f, 10, 0, {1, 5, 2, 2}},
    {-200.0f, -150.0f, -80.0f, 0.0f, 16, 30, {2, 7, 6, 5}},
    {0.0f, -150.0f, -80.0f, 0.0f, 17, 30, {2, 7, 4, 6}},
    {200.0f, -150.0f, -80.0f, 0.0f, 18, 30, {3, 8, 5, 4}},
    {-130.0f, -190.0f, -100.0f, 0.0f, 19, 31, {4, 0, 8, 8}},
    {130.0f, -190.0f, -100.0f, 0.0f, 20, 31, {5, 1, 7, 7}},
};

WinDt win_dt2[7] = {
    {-140.0f, 75.0f, -120.0f, 60.0f, 13, 0, {6, 2, 1, 1}},
    {140.0f, 75.0f, -120.0f, 60.0f, 12, 0, {6, 3, 0, 0}},
    {-140.0f, -65.0f, -120.0f, 60.0f, 11, 0, {0, 6, 3, 3}},
    {140.0f, -65.0f, -120.0f, 60.0f, 10, 0, {1, 6, 2, 2}},
    {-200.0f, -150.0f, -80.0f, 0.0f, 16, 30, {2, 7, 6, 5}},
    {0.0f, -150.0f, -80.0f, 0.0f, 17, 30, {2, 7, 4, 6}},
    {200.0f, -150.0f, -80.0f, 0.0f, 18, 30, {3, 1, 6, 6}},
};

WinDt win_dt3[4] = {
    {-140.0f, 75.0f, -120.0f, 60.0f, 13, 0, {2, 2, 1, 1}},
    {140.0f, 75.0f, -120.0f, 60.0f, 12, 0, {3, 3, 0, 0}},
    {-140.0f, -65.0f, -120.0f, 60.0f, 11, 0, {0, 0, 3, 3}},
    {140.0f, -65.0f, -120.0f, 60.0f, 10, 0, {1, 1, 2, 2}},
};

static LoadWork work;
static LoadWork* wp = &work;
#ifdef VERSION_G8ME01
__declspec(section ".sdata2") const u32 seq_logo_zero_color[2];
#endif

static EvtWord evt_shake[12];

// Descriptive name (not in the symbol maps); placeholder unk_800f2398, JP 0x800F2398. See docs/NAME_PROVENANCE.md.
s32 evt_seq_load_keep_msg_window(EventEntry* evt, BOOL isFirstCall) {
    fadeReset(4);
    wp->printWindowId = evt->printWindowId;
    return EVT_RETURN_DONE;
}

void seq_loadInit(SequenceWork* seq) {
    memset(wp, 0, sizeof(LoadWork));
}

void seq_loadExit(SequenceWork* seq) {
    if (wp->file != NULL) {
        fileFree(wp->file);
    }
    wp->file = NULL;
}

#ifdef VERSION_G8ME01
static inline BOOL loadStart(void);

#else
static inline BOOL loadStart(void) {
    if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/jp/name.tpl", getMarioStDvdRoot()) != 0U) {
        wp->file = fileAllocf(FILE_TYPE_TPL, "%s/w/jp/name.tpl", getMarioStDvdRoot());
        return TRUE;
    }
    return FALSE;
}

#endif
void seq_loadMain(SequenceWork* seq) {
    switch (seq->state) {
        case 0:
            if (loadStart()) {
                seq->state++;
            }
            break;
        case 1:
            wp->evt = evtEntryType(evt_memcard_check, 0, 0, 0);
            if (wp->flags & LOAD_FLAG_RECHECK) {
                wp->flags &= ~LOAD_FLAG_RECHECK;
                wp->evt->lwData[10] = 1;
                wp->evt->printWindowId = wp->printWindowId;
            }
            seq->state++;
            break;
        case 2:
            if (evtCheckID(wp->evt->eventId) == 0) {
                if (evtGetValue(NULL, GF(0)) != 1) {
                    seqSetSeq(SEQ_TITLE, NULL, NULL);
                } else {
                    wp->evt = NULL;
                    seq->state++;
                    wp->state = 0;
                    wp->cardCode = cardGetCode();
                    wp->flags &= ~LOAD_FLAG_HIDE_CURSOR;
                    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
                        SoundSetOutputMode(SND_OUTPUTMODE_MONO);
                        gp->soundMode = 0;
                        wp->soundMode = 0;
                    } else {
                        SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
                        if (gp->soundMode == 0) {
                            wp->soundMode = 1;
                        } else {
                            wp->soundMode = 2;
                        }
                    }
                    wp->rumbleOff = gp->rumbleEnabled == 0;
                    wp->soundModeSel = wp->soundMode;
                    wp->rumbleOffSel = wp->rumbleOff;
                    wp->slot = gp->lastSlot;
                    wp->x = win_dt[wp->slot].x;
                    wp->y = win_dt[wp->slot].y;
                }
            }
            break;
        case 3: {
            GXColor c = {0, 0, 0, 255};
            fadeEntry(0x1c, 0, c);
            seq->state++;
            break;
        }
        case 4:
            loadMain(seq);
            break;
    }
}

#ifdef VERSION_G8ME01
static inline BOOL loadStart(void) {
    const char* lang[] = {"jp", "us", "ge", "fr", "sp", "it"};
    if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0U) {
        wp->file = fileAllocf(FILE_TYPE_TPL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]);
        return TRUE;
    }
    return FALSE;
}

#endif
#define SFX(id) psndSFXOn((const char*)(id))

void loadMain(SequenceWork* seq) {
    u32 trg;
    u32 dir;
    s32 code;
    s32 prev;
    s32 i;
    s32 sound;
    u16 flags;

    trg = keyGetButtonTrg(0);
    keyGetButtonRep(0);
    dir = keyGetDirRep(0);
    code = cardGetCode();
    if (cardIsExec() || !fadeIsFinish()) {
        trg = 0;
        dir = 0;
    }
    if (wp->cardCode != code) {
        trg = 0;
        dir = 0;
    }
    wp->cardCode = code;
    for (i = 0; i < 4; i++) {
        flags = cardGetFilePtr()->saves[i].data.flags;
        if (!(flags & 1) || (flags & 2)) {
            break;
        }
    }
    if (i < 4) {
        wp->flags &= ~LOAD_FLAG_NO_DELETE;
    } else {
        wp->flags |= LOAD_FLAG_NO_DELETE;
    }
    wp->flags &= ~LOAD_FLAG_NO_COPY;
    for (i = 0; i < 4; i++) {
        flags = cardGetFilePtr()->saves[i].data.flags;
        if (!(flags & 1) && !(flags & 2)) {
            break;
        }
    }
    if (i >= 4) {
        wp->flags |= LOAD_FLAG_NO_COPY;
    }

    switch (wp->state) {
        case 0:
            wp->state++;
        case 1:
            wp->msg = msgSearch("msg_savefile_select");
            prev = wp->slot;
            if (dir & DEMO_STICK_UP) {
                wp->slot = win_dt[prev].next[0];
            } else if (dir & DEMO_STICK_DOWN) {
                wp->slot = win_dt[prev].next[1];
            } else if (dir & DEMO_STICK_LEFT) {
                wp->slot = win_dt[prev].next[2];
            } else if (dir & DEMO_STICK_RIGHT) {
                wp->slot = win_dt[prev].next[3];
            } else if (trg & PAD_BUTTON_A) {
                flags = cardGetFilePtr()->saves[wp->slot].data.flags;
                switch (wp->slot) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        if (flags & 2) {
                            SFX(PSND_SFX_CURSOR_CANCEL1);
                        } else if (flags & 1) {
                            wp->state = 0x1e;
                            SFX(PSND_SFX_CURSOR_OK1);
                        } else {
                            wp->state = 0x14;
                            SFX(PSND_SFX_CURSOR_OK1);
                        }
                        break;
                    case 4:
                        if (!(wp->flags & LOAD_FLAG_NO_DELETE)) {
                            for (i = 0; i < 4; i++) {
                                flags = cardGetFilePtr()->saves[i].data.flags;
                                if (!(flags & 1) || (flags & 2)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(PSND_SFX_CURSOR_OK1);
                                wp->slot = 0;
                                wp->state = 0x28;
                            } else {
                                SFX(PSND_SFX_CURSOR_CANCEL1);
                            }
                        }
                        break;
                    case 5:
                        if (!(wp->flags & LOAD_FLAG_NO_COPY)) {
                            for (i = 0; i < 4; i++) {
                                flags = cardGetFilePtr()->saves[i].data.flags;
                                if (!(flags & 2) && !(flags & 1)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(PSND_SFX_CURSOR_OK1);
                                wp->slot = 0;
                                wp->state = 0x32;
                            } else {
                                SFX(PSND_SFX_CURSOR_CANCEL1);
                            }
                        }
                        break;
                    case 6:
                        SFX(PSND_SFX_CURSOR_CANCEL1);
                        wp->state = 0xa;
                        break;
                    case 7:
                        SFX(PSND_SFX_CURSOR_OK1);
                        psndBGMOff(PSND_SILENT);
                        wp->state = 0x64;
                        break;
                    case 8:
                        SFX(PSND_SFX_CURSOR_OK1);
                        wp->state = 0xc8;
                        break;
                }
            } else if (trg & PAD_BUTTON_B) {
                SFX(PSND_SFX_CURSOR_CANCEL1);
                wp->state = 0xa;
            }
            if (prev != wp->slot) {
                if (wp->slot < 4) {
                    SFX(9);
                } else {
                    SFX(PSND_SFX_CURSOR_MOVE1);
                }
            }
            wp->tx = win_dt[wp->slot].x + win_dt[wp->slot].ofsX;
            wp->ty = win_dt[wp->slot].y + win_dt[wp->slot].ofsY;
            break;
        case 0xa:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->state++;
            break;
        case 0xb:
            if (fadeIsFinish()) {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
        case 0x14:
            wp->flags |= LOAD_FLAG_HIDE_CURSOR;
            wp->evt = evtEntryType(evt_memcard_start, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->state++;
            break;
        case 0x15:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~LOAD_FLAG_HIDE_CURSOR;
                if (evtGetValue(NULL, GW(0)) == 1) {
                    wp->state++;
                } else {
                    wp->state = 1;
                }
            }
            break;
        case 0x16:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            psndBGMOff_f_d(PSND_FADEOUT, 2000, 0);
            wp->state++;
            break;
        case 0x17:
            if (fadeIsFinish()) {
                wp->flags |= LOAD_FLAG_HIDE_CURSOR;
                wp->evt = evtEntryType(evt_continue, 0, 0, 0);
                wp->state++;
            }
            break;
        case 0x18:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~LOAD_FLAG_HIDE_CURSOR;
                continueGame();
            }
            break;
        case 0x1e:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->state++;
            break;
        case 0x1f:
            if (fadeIsFinish()) {
                wp->state++;
            }
            break;
        case 0x20:
            if (nameEntPrepare()) {
                nameEntOn(0);
                fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                wp->state++;
            }
            break;
        case 0x21:
            if (fadeIsFinish() && !nameEntWait()) {
                fadeEntry(0x18, 0, (GXColor){0, 0, 0, 255});
                if (!nameEntIsCancel()) {
                    psndBGMOff_f_d(PSND_FADEOUT, 3000, 0);
                }
                wp->state++;
            }
            break;
        case 0x22:
            if (fadeIsFinish()) {
                nameEntOff();
                if (nameEntIsCancel()) {
                    fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                    wp->state = 1;
                } else {
                    strcpy((char*)gp->playerName, nameEntGetName());
                    gp->lastSlot = wp->slot;
                    stg0_00_init();
                    gp->saveSlot = wp->slot;
                    marioPartyKill();
                    {
                        MarioWork* m = marioGetPtr();
                        m->prevPartyMemberId[0] = 0;
                        m->prevPartyMemberId[1] = 0;
                    }
                    gp->areaFadeInType = 0;
                    gp->areaFadeOutType = 25;
                    swInit();
                    swByteSet(0, STG0_00_PROLOGUE);
                    if (code == 0) {
                        gp->saveSlot = wp->slot;
#ifdef VERSION_G8ME01
                        wp->evt = evtEntryType(evt_memcard_save_wrapper, 0, 0, 0);
#else
                        wp->evt = evtEntryType(evt_memcard_save, 0, 0, 0);
#endif
                        wp->evt->lwData[0] = 0x17;
                        wp->state++;
                    } else {
                        seqSetSeq(SEQ_MAP_CHANGE, "aaa_00", "prologue");
                    }
                }
            }
            break;
        case 0x23:
            if (!evtCheckID(wp->evt->eventId)) {
                seqSetSeq(SEQ_MAP_CHANGE, "aaa_00", "prologue");
            }
            break;
        case 0x28:
            wp->msg = msgSearch("msg_savefile_delete_select");
            prev = wp->slot;
            if (dir & DEMO_STICK_UP) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & DEMO_STICK_DOWN) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & DEMO_STICK_LEFT) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & DEMO_STICK_RIGHT) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & PAD_BUTTON_A) {
                if (prev < 4) {
                    flags = cardGetFilePtr()->saves[wp->slot].data.flags;
                    if ((flags & 2) || !(flags & 1)) {
                        SFX(PSND_SFX_CURSOR_OK1);
                        wp->state++;
                    } else {
                        SFX(PSND_SFX_CURSOR_CANCEL1);
                    }
                } else {
                    wp->slot = 4;
                    wp->state = 1;
                    SFX(PSND_SFX_CURSOR_CANCEL1);
                }
            } else if (trg & PAD_BUTTON_B) {
                wp->slot = 4;
                wp->state = 1;
                SFX(PSND_SFX_CURSOR_CANCEL1);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x29:
            wp->flags |= LOAD_FLAG_HIDE_CURSOR;
            wp->evt = evtEntryType(evt_memcard_delete, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->state++;
            break;
        case 0x2a:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, GF(0)) == 0) {
                    wp->flags &= ~LOAD_FLAG_HIDE_CURSOR;
                    wp->slot = 4;
                    wp->state = 1;
                } else {
                    wp->flags |= LOAD_FLAG_RECHECK;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x32:
            wp->msg = msgSearch("msg_savefile_copy_select");
            prev = wp->slot;
            if (dir & DEMO_STICK_UP) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & DEMO_STICK_DOWN) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & DEMO_STICK_LEFT) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & DEMO_STICK_RIGHT) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & PAD_BUTTON_A) {
                if (prev < 4) {
                    flags = cardGetFilePtr()->saves[wp->slot].data.flags;
                    if (!(flags & 2) && !(flags & 1)) {
                        wp->state = 0x33;
                        wp->copyFrom = wp->slot;
                        wp->copyTo = (wp->copyFrom + 1) % 4;
                        wp->slot = wp->copyTo;
                        SFX(PSND_SFX_CURSOR_OK1);
                    }
                } else {
                    wp->slot = 5;
                    wp->state = 1;
                    SFX(PSND_SFX_CURSOR_CANCEL1);
                }
            } else if (trg & PAD_BUTTON_B) {
                wp->slot = 5;
                wp->state = 1;
                SFX(PSND_SFX_CURSOR_CANCEL1);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x33:
            wp->msg = msgSearch("msg_savefile_copy_point");
            prev = wp->slot;
            if (dir & DEMO_STICK_UP) {
                do {
                    wp->slot = win_dt3[wp->slot].next[0];
                } while (wp->slot == wp->copyFrom);
            } else if (dir & DEMO_STICK_DOWN) {
                do {
                    wp->slot = win_dt3[wp->slot].next[1];
                } while (wp->slot == wp->copyFrom);
            } else if (dir & DEMO_STICK_LEFT) {
                do {
                    wp->slot = win_dt3[wp->slot].next[2];
                } while (wp->slot == wp->copyFrom);
            } else if (dir & DEMO_STICK_RIGHT) {
                do {
                    wp->slot = win_dt3[wp->slot].next[3];
                } while (wp->slot == wp->copyFrom);
            } else if (trg & PAD_BUTTON_A) {
                wp->state = 0x34;
                wp->copyTo = wp->slot;
                SFX(PSND_SFX_CURSOR_OK1);
            } else if (trg & PAD_BUTTON_B) {
                wp->slot = wp->copyFrom;
                wp->state = 0x32;
                SFX(PSND_SFX_CURSOR_CANCEL1);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->copyTo = wp->slot;
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x34:
            wp->flags |= LOAD_FLAG_HIDE_CURSOR;
            wp->evt = evtEntryType(evt_memcard_copy, 0, 0, 0);
            wp->evt->lwData[0] = wp->copyFrom;
            wp->evt->lwData[1] = wp->copyTo;
            wp->state++;
            break;
        case 0x35:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, GF(0)) == 0) {
                    wp->flags &= ~LOAD_FLAG_HIDE_CURSOR;
                    wp->slot = 5;
                    wp->state = 1;
                } else {
                    wp->flags |= LOAD_FLAG_RECHECK;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x64:
            wp->flags |= LOAD_FLAG_DIM;
            sound = wp->soundModeSel;
            if (dir & DEMO_STICK_UP) {
                wp->soundModeSel = wp->soundModeSel - 1;
                SFX(PSND_SFX_CURSOR_MOVE1);
            } else if (dir & DEMO_STICK_DOWN) {
                wp->soundModeSel = wp->soundModeSel + 1;
                SFX(PSND_SFX_CURSOR_MOVE1);
            } else if (trg & PAD_BUTTON_A) {
                switch (sound) {
                    case 0:
                        OSSetSoundMode(OS_SOUND_MODE_MONO);
                        gp->soundMode = 0;
                        break;
                    case 1:
                        OSSetSoundMode(OS_SOUND_MODE_STEREO);
                        gp->soundMode = 0;
                        break;
                    case 2:
                        OSSetSoundMode(OS_SOUND_MODE_STEREO);
                        gp->soundMode = 1;
                        break;
                }
                if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
                    SoundSetOutputMode(SND_OUTPUTMODE_MONO);
                } else {
                    SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
                }
                SFX(PSND_SFX_CURSOR_OK1);
                psndBGMOn(PSND_CONTINUE | PSND_UNSILENT, NULL);
                psndENVOff(PSND_FADEOUT);
                wp->soundMode = wp->soundModeSel;
                wp->flags &= ~LOAD_FLAG_DIM;
                wp->state = 1;
                break;
            } else if (trg & PAD_BUTTON_B) {
                SFX(PSND_SFX_CURSOR_CANCEL1);
                psndBGMOn(PSND_CONTINUE | PSND_UNSILENT, NULL);
                psndENVOff(PSND_FADEOUT);
                wp->soundModeSel = wp->soundMode;
                if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
                    SoundSetOutputMode(SND_OUTPUTMODE_MONO);
                } else {
                    SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
                }
                wp->flags &= ~LOAD_FLAG_DIM;
                wp->state = 1;
                break;
            }
            if (wp->soundModeSel >= 3) {
                wp->soundModeSel = 0;
            }
            if (wp->soundModeSel < 0) {
                wp->soundModeSel = 2;
            }
            if (wp->soundModeSel != sound) {
                switch (wp->soundModeSel) {
                    case 0:
                        SoundSetOutputMode(SND_OUTPUTMODE_MONO);
                        break;
                    case 1:
                        SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
                        break;
                    case 2:
                        SoundSetOutputMode(SND_OUTPUTMODE_SURROUND);
                        break;
                }
                psndENVOn(PSND_RESTART | PSND_FADEIN, (const char*)PSND_ENV_SYS_FL1);
            }
#ifdef VERSION_G8ME01
            wp->tx = -74.0f;
            wp->ty = 4.0f + (50.0f - 50.0f * wp->soundModeSel);
#else
            wp->tx = -74.0f;
            wp->ty = 4.0f + (50.0f - 50.0f * wp->soundModeSel);
#endif
            break;
        case 0xc8:
            wp->flags |= LOAD_FLAG_DIM;
            if (dir & DEMO_STICK_UP) {
                wp->rumbleOffSel--;
                SFX(PSND_SFX_CURSOR_MOVE1);
            } else if (dir & DEMO_STICK_DOWN) {
                wp->rumbleOffSel++;
                SFX(PSND_SFX_CURSOR_MOVE1);
            } else if (trg & PAD_BUTTON_A) {
                gp->rumbleEnabled = wp->rumbleOffSel == 0;
                SFX(PSND_SFX_CURSOR_OK1);
                wp->rumbleOff = wp->rumbleOffSel;
                wp->flags &= ~LOAD_FLAG_DIM;
                wp->state = 1;
                if (gp->rumbleEnabled) {
                    evtEntryType(evt_shake, 0, 0, 0);
                }
            } else if (trg & PAD_BUTTON_B) {
                SFX(PSND_SFX_CURSOR_CANCEL1);
                wp->rumbleOffSel = wp->rumbleOff;
                wp->flags &= ~LOAD_FLAG_DIM;
                wp->state = 1;
            }
            if (wp->rumbleOffSel >= 2) {
                wp->rumbleOffSel = 0;
            }
            if (wp->rumbleOffSel < 0) {
                wp->rumbleOffSel = 1;
            }
#ifdef VERSION_G8ME01
            wp->tx = -74.0f;
#else
            wp->tx = -74.0f;
#endif
            wp->ty = 4.0f + (30.0f - 50.0f * wp->rumbleOffSel);
            break;
    }
    wp->x += (wp->tx - wp->x) / 4.0f;
    wp->y += (wp->ty - wp->y) / 4.0f;
    {
        f32 s = 2.0f * (f32)sin(gp->retraceCount / 8.0f);
        wp->x += __fabsf(s);
    }
    dispEntry(CAMERA_3D, 1000.0f, 0, loadDraw, NULL);
}

static inline void loadDrawBg(u32 id, f32 x0, f32 y0, f32 x1, f32 y1) {
    GXTexObj obj;
    Mtx m;
    f64 w;
    f64 h;

    TEXGetGXTexObjFromPalette(*wp->file->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    h = __fabs(y0 - y1);
    w = __fabs(x1 - x0);
    PSMTXScale(m, (f32)w / GXGetTexObjWidth(&obj), (f32)h / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y0, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x0, y1, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x1, y1, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x1, y0, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void loadDraw(CameraId camera, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    GXColor white = {255, 255, 255, 255};
    GXColor gray = {160, 160, 160, 255};
    GXColor dark = {128, 128, 128, 128};
    f32 vp[GX_VIEWPORT_SZ];
    f32 proj[GX_PROJECTION_SZ];
    CameraEntry saved;
    char buf[0x20];
    char buf2[0x100];
    Mtx m;
    Mtx s;
    f32 x;
    f32 w;
    u64 sec;
#ifdef VERSION_G8ME01
    WinDt* win1[1]; // FAKE: one-element array, so IRO turns win1 into an @ temp: the address of win_dt is computed into
                    // win1 itself and loop 1 walks a copy of it (the addi r30 / mr r24,r30 direction of the target)
    s32 i;
#else
    WinDt* win1;
    s32 i;
    s32 j;
#endif
    s32 k;

    if (wp->state >= 0x20 && wp->state < 0x28) {
        return;
    }
    GXGetViewportv(vp);
    GXGetProjectionv(proj);
    GXSetProjection(cam->projection, cam->projType);
    saved = *camGetCurPtr();
    *camGetCurPtr() = *cam;
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){255, 255, 255, 255});
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
#ifdef VERSION_G8ME01
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    loadDrawBg(0x35, -304.0f, 240.0f, -272.0f, -240.0f);
    loadDrawBg(0x34, -272.0f, 240.0f, 272.0f, -240.0f);
    loadDrawBg(0x36, 272.0f, 240.0f, 304.0f, -240.0f);

    win1[0] = win_dt;
#else
    // FAKE: win1 reused as a cam alias before its win_dt web, so the win_dt web becomes a lifetime-split temp
    // (addi straight into win1's register, loop 1 iterates a copy) as in the target
    win1 = (WinDt*)cam;
    GXLoadPosMtxImm(((CameraEntry*)win1)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    loadDrawBg(0x35, -304.0f, 240.0f, -272.0f, -240.0f);
    loadDrawBg(0x34, -272.0f, 240.0f, 272.0f, -240.0f);
    loadDrawBg(0x36, 272.0f, 240.0f, 304.0f, -240.0f);

    win1 = win_dt;
#endif
    for (i = 0; i < 9; i++) {
        GXColor col = gray;
        if (i == wp->slot) {
            col = white;
        }
        if (wp->state == 0x33 && (i == wp->copyFrom || i == wp->copyTo)) {
            col = white;
        }
        if (i == 4 && (wp->flags & LOAD_FLAG_NO_DELETE)) {
            col = dark;
        }
        if (i == 5 && (wp->flags & LOAD_FLAG_NO_COPY)) {
            col = dark;
        }
#ifdef VERSION_G8ME01
        switch (win1[0][i].tex) {
#else
        switch (win1[i].tex) {
#endif
            case 10:
            case 11:
            case 12:
            case 13: {
                col.a = 0xfe;
                winTexInit(*wp->file->data);
#ifdef VERSION_G8ME01
                winTexSet(0xe, (Vec){win1[0][i].x, win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
                winTexInit(*wp->file->data);
                winTexSet(win1[0][i].tex, (Vec){win1[0][i].x, 60.0f + win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
#else
                winTexSet(0xe, (Vec){win1[i].x, win1[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
                winTexInit(*wp->file->data);
                winTexSet(win1[i].tex, (Vec){win1[i].x, 60.0f + win1[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
#endif
                break;
            }
            default: {
                winTexInit_x2(*wp->file->data);
#ifdef VERSION_G8ME01
                winTexSet_x2(win1[0][i].tex, win1[0][i].tex2, (Vec){win1[0][i].x, win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
#else
                winTexSet_x2(win1[i].tex, win1[i].tex2, (Vec){win1[i].x, win1[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
#endif
                break;
            }
        }
    }

#ifdef VERSION_G8ME01
    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, -250.0f, 200.0f, 500.0f, 40.0f, 20.0f);
#else
    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, -250.0f, 200.0f, 500.0f, 40.0f, 20.0f);
#endif
    FontDrawStart();
    {
        const char* msg = wp->msg;
        FontDrawMessage(-FontGetMessageWidth(msg) / 2, 192, msg);
    }

#ifdef VERSION_G8ME01
    for (i = 0; i < 4; i++, win1[0]++) {
        CardSaveSlot* save = &cardGetFilePtr()->saves[i];
        u16 flags;
        OSTime time;
        u16 stars;
        s32 gsw0;
        char* name;
        s16 level;
#else
    for (i = 0; i < 4; i++, win1++) {
        CardSaveSlot* save = &cardGetFilePtr()->saves[i];
        u16 flags = save->data.flags;
        s16 level = save->data.pouch.level;
        OSTime time = save->data.global.retraceLocalTime;
        u16 stars = save->data.pouch.mStarPowersObtained;
        s32 gsw0 = save->data.global.mGSW0;
        char* name = (char*)save->data.global.playerName;
#endif
        const char* str;
        s32 hours;
        s32 mins;
#ifdef VERSION_G8ME01
        flags = save->data.flags;
        level = save->data.pouch.level;
        time = save->data.global.retraceLocalTime;
        stars = save->data.pouch.mStarPowersObtained;
        gsw0 = save->data.global.mGSW0;
        name = (char*)save->data.global.playerName;
#endif

        FontDrawStart();
        FontDrawColor((GXColor){192, 192, 192, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){255, 255, 255, 255});
        }
        FontDrawScale(0.6f);
        FontDrawEdge();
#ifdef VERSION_G8ME01
        if ((u32)gp->language == LANGUAGE_JP) {
            str = "\x83\x66\x81\x5B\x83\x5E";
        } else {
            str = msgSearch("msg_mcard_file");
        }
        sprintf(buf, "%s%s", str, winZenkakuStr(i + 1));
        FontDrawString(win1[0]->x - 100.0f, 84.0f + win1[0]->y, buf);
#else
        sprintf(buf, "\203f\201[\203^%s", winZenkakuStr(i + 1));
        FontDrawString(win1->x - 100.0f, 84.0f + win1->y, buf);
#endif
        FontDrawEdgeOff();
        FontDrawScale(1.0f);
        FontDrawColor((GXColor){32, 32, 32, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){0, 0, 0, 255});
        }
        if (flags & 2) {
#ifdef VERSION_G8ME01
            if ((u32)gp->language == LANGUAGE_JP) {
                str = "\x82\xB1\x82\xED\x82\xEA\x82\xC4\x82\xA2\x82\xDC\x82\xB7";
            } else {
                str = msgSearch("msg_mcard_kowareteru");
            }
            w = FontGetMessageWidth(str);
            FontDrawStart();
            FontDrawMessage(win1[0]->x - w / 2.0f, 28.0f + win1[0]->y, str);
            str = msgSearch("msg_mcard_kowaretitle");
            FontDrawString(win1[0]->x - FontGetMessageWidth(str) / 2, 70.0f + win1[0]->y, str);
        } else if (flags & 1) {
            if ((u32)gp->language == LANGUAGE_JP) {
                str = "\x83\x6A\x83\x85\x81\x5B\x83\x51\x81\x5B\x83\x80";
            } else {
                str = msgSearch("msg_mcard_new_game");
            }
            FontDrawString(win1[0]->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1[0]->y, str);
        } else if (gsw0 == 0) {
            if ((u32)gp->language == LANGUAGE_JP) {
                str = "\x82\xCD\x82\xB6\x82\xDF\x82\xA9\x82\xE7";
            } else {
                str = msgSearch("msg_mcard_hajimekara");
            }
            FontDrawString(win1[0]->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1[0]->y, str);
            if (strlen(name) != 0) {
                FontDrawString(win1[0]->x - FontGetMessageWidth(name) / 2, 70.0f + win1[0]->y, name);
            }
        } else {
            FontGetMessageWidth((u32)gp->language != LANGUAGE_JP ? "88" : "\x82\x57\x82\x57");
#else
            str = "\202\261\202\355\202\352\202\304\202\242\202\334\202\267";
            FontDrawString(win1->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1->y, str);
        } else if (flags & 1) {
            str = "\203j\203\205\201[\203Q\201[\203\200";
            FontDrawString(win1->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1->y, str);
        } else if (gsw0 == 0) {
            str = "\202\315\202\266\202\337\202\251\202\347";
            FontDrawString(win1->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1->y, str);
            if (strlen(name) != 0) {
                FontDrawString(win1->x - FontGetMessageWidth(name) / 2, 70.0f + win1->y, name);
            }
        } else {
            FontGetMessageWidth("\x82\x57\x82\x57");
#endif
            sec = OSTicksToSeconds(time);
            if (sec > 359999) {
                hours = 99;
                mins = 59;
            } else {
                hours = (u64)sec / 3600;
                mins = ((u64)sec % 3600) / 60;
            }
            if (strlen(name) != 0) {
#ifdef VERSION_G8ME01
                FontDrawString(win1[0]->x - FontGetMessageWidth(name) / 2, 70.0f + win1[0]->y, name);
            }
            sprintf(buf2, (u32)gp->language != LANGUAGE_JP ? "%s %s" : "%s\x81\x40%s", msgSearch("msg_menu_mario_level"), winZenkakuStr(level));
            FontDrawString(win1[0]->x - FontGetMessageWidth(buf2) / 2.0f, 30.0f + win1[0]->y, buf2);
            sprintf(buf2, (u32)gp->language != LANGUAGE_JP ? "%s " : "%s\x81\x40", msgSearch("msg_menu_mario_num_playtime"));
#else
                FontDrawString(win1->x - FontGetMessageWidth(name) / 2, 70.0f + win1->y, name);
            }
            sprintf(buf2, "\203\214\203x\203\213\201@%s", winZenkakuStr(level));
            FontDrawString(win1->x - FontGetMessageWidth(buf2) / 2.0f, 30.0f + win1->y, buf2);
            strcpy(buf2, "\203v\203\214\203C\202\266\202\251\202\361\201@");
#endif
            strcat(buf2, winZenkakuStrPad(hours, 2));
            strcat(buf2, " ");
            strcat(buf2, winZenkakuStrPad(mins, 2));
            w = FontGetMessageWidth(buf2);
#ifdef VERSION_G8ME01
            if (w > 200.0f) {
                FontDrawScaleVec((Vec){200.0f / w, 1.0f, 1.0f});
                w = 200.0f;
            }
            FontDrawString(win1[0]->x - w / 2.0f, win1[0]->y, buf2);
            FontDrawScaleVec((Vec){1.0f, 1.0f, 1.0f});
            w = FontGetMessageWidth(buf2);
            sprintf(buf2, (u32)gp->language != LANGUAGE_JP ? "%s " : "%s\x81\x40", msgSearch("msg_menu_mario_num_playtime"));
            strcat(buf2, winZenkakuStrPad(hours, 2));
            x = FontGetMessageWidth(buf2);
            if (w > 200.0f) {
                x *= 200.0f / w;
                x += 1600.0f / w;
                w = 200.0f;
            } else {
                x += 8.0f;
            }
            PSMTXTrans(m, x + (win1[0]->x - w / 2.0f), 1.0f + (win1[0]->y - 22.0f), 0.0f);
#else
            FontDrawString(win1->x - w / 2.0f, win1->y, buf2);
            strcpy(buf2, "\203v\203\214\203C\202\266\202\251\202\361\201@");
            strcat(buf2, winZenkakuStrPad(hours, 2));
            x = FontGetMessageWidth(buf2);
            PSMTXTrans(m, 8.0f + (x + (win1->x - w / 2.0f)), 1.0f + (win1->y - 22.0f), 0.0f);
#endif
            PSMTXScale(s, 1.5f, 1.5f, 1.5f);
            PSMTXConcat(m, s, m);
            iconDispGxCol(m, 0, ICON_COUNTER_COLON, (GXColor){0, 0, 0, 255});
#ifdef VERSION_G8ME01
            for (k = 0; k < 7; k++) {
                if (stars & (1 << (k + 1))) {
                    PSMTXTrans(m, (win1[0]->x - 106.0f) + k * 32, *(f32*)((u8*)win1[0] + offsetof(WinDt, y)) /* FAKE: win1->y read through a byte offset (same load) keeps &win1->y a late loop temp, as in the target's numbering */ - 52.0f, 0.0f);
#else
            // FAKE: y read through a byte offset so the loop-invariant &win1->y becomes a hoisted @ temp (target pop order)
            for (k = 0; k < 7; k++) {
                if (stars & (1 << (k + 1))) {
                    PSMTXTrans(m, (win1->x - 106.0f) + k * 32, *(f32*)((u8*)win1 + offsetof(WinDt, y)) - 52.0f, 0.0f);
#endif
                    iconDispGxCol(m, 0, (u16)stone_id[k], i == wp->slot ? white : gray);
                }
            }
        }
    }

    if (wp->flags & LOAD_FLAG_DIM) {
        wp->alpha += 0.05f;
    } else {
        wp->alpha -= 0.05f;
    }
    if (wp->alpha < 0.0f) {
        wp->alpha = 0.0f;
    }
    if (wp->alpha > 1.0f) {
        wp->alpha = 1.0f;
    }
    nameMaskGX(wp->alpha);

    if (wp->state >= 100 && wp->state < 200) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x13, 0x1f, (Vec){0.0f, 100.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 3; i++) {
            if (i == wp->soundModeSel) {
                winTexInit(*wp->file->data);
                winTexSet(0x33, (Vec){0.0f, 50 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x15, 0x33, (Vec){-4.0f, 54 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x15, 0x33, (Vec){0.0f, 50 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            }
        }
    }

    if (wp->state >= 200 && wp->state < 300) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x14, 0x1f, (Vec){0.0f, 100.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 2; i++) {
            if (i == wp->rumbleOffSel) {
                winTexInit(*wp->file->data);
#ifdef VERSION_G8ME01
                winTexSet(0x20, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x20, (Vec){-4.0f, 34 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x20, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
#else
                winTexSet(0x33, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x33, (Vec){-4.0f, 34 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x33, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
#endif
            }
        }
    }

    if (wp->state == 0x33 && wp->copyTo < 4) {
        f32 angle[4][4] = {
            {0.0f, -90.0f, -180.0f, -135.0f},
            {90.0f, 0.0f, 135.0f, 180.0f},
            {0.0f, -45.0f, 0.0f, -90.0f},
            {45.0f, 0.0f, 90.0f, 0.0f},
        };
        f32 cx;
        f32 cy;
        if (!(f32)fmod(angle[wp->copyFrom][wp->copyTo], 90.0)) {
            f32 c0 = wp->copyFrom % 2;
            f32 r0 = wp->copyFrom / 2;
            f32 r1 = wp->copyTo / 2;
            if (r0 == r1) {
                cx = 0.0f;
                cy = 120.0f - 140.0f * r0;
            } else {
                cx = -150.0f + 300.0f * c0;
                cy = 50.0f;
            }
        } else {
            cx = 0.0f;
            cy = 50.0f;
        }
        cy -= 40.0f;
        winTexInit(*wp->file->data);
        winTexSetRot(0x1d, (Vec){2.0f + cx, cy - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 128}, angle[wp->copyFrom][wp->copyTo]);
        winTexSetRot(0x1d, (Vec){cx, cy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, angle[wp->copyFrom][wp->copyTo]);
    }

    if (!(wp->flags & LOAD_FLAG_HIDE_CURSOR)) {
        winTexInit(*wp->file->data);
        winTexSet(0, (Vec){2.0f + wp->x, wp->y - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 128});
        winTexSet(0, (Vec){wp->x, wp->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }

    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
    *camGetCurPtr() = saved;
}

#ifdef VERSION_G8ME01
s32 stone_id[7] = {0x197 + 8, 0x198 + 8, 0x19A + 8, 0x19C + 8, 0x19D + 8, 0x199 + 8, 0x19B + 8};
#else
s32 stone_id[7] = {0x197, 0x198, 0x19A, 0x19C, 0x19D, 0x199, 0x19B};
#endif

static EVT_BEGIN(evt_shake)
    CALL(evt_sub_rumble_onoff, 0, 0)
    WAIT_MS(150)
    CALL(evt_sub_rumble_onoff, 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_continue)
    CALL(evt_sub_animgroup_async, STRING("OFF_f_in_out_mario"))
    CALL(evt_sub_animgroup_async, STRING("OFF_f_in_out_kuppa"))
    CALL(evt_sub_animgroup_async, STRING("OFF_f_in_out_peach"))
    RETURN
EVT_END()
#ifndef VERSION_G8ME01
#pragma defer_codegen off
#endif
void continueGame(void) {
    cardCopy2Main(wp->slot);
    gp->saveSlot = wp->slot;
    gp->lastSlot = wp->slot;
    marioPartyKill();
    {
        MarioWork* m = marioGetPtr();
        m->prevPartyMemberId[0] = 0;
        m->prevPartyMemberId[1] = 0;
    }
    psndSetFlag(PSND_FLAG_NO_SFX);
    marioPartyEntry(gp->savePartyId[0]);
    marioPartyEntry(gp->savePartyId[1]);
    psndClearFlag(PSND_FLAG_NO_SFX);
    countDownSaveReStart();
    gp->flags |= GW_FLAG_SAVE_LOADED;
    switch (swByteGet(4)) { // GSW_SAVE_RESUME_POINT
        case 0:
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 28;
            seqSetSeq(SEQ_MAP_CHANGE, gp->currentMapName, 0);
            break;
        case 1:
            gp->areaFadeInType = 61;
            gp->areaFadeOutType = 56;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
        case 2:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "kpa_00", 0);
            break;
        case 3:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "nok_01", "nokotarou");
            break;
        case 4:
            gp->areaFadeInType = 61;
            gp->areaFadeOutType = 56;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
        case 5:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "hei_00", 0);
            break;
        case 6:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "mri_00", 0);
            break;
        case 7:
            gp->areaFadeInType = 61;
            gp->areaFadeOutType = 56;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
        case 8:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "mri_01", 0);
            break;
        case 9:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "tou_02", 0);
            break;
        case 10:
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 28;
            seqSetSeq(SEQ_MAP_CHANGE, "usu_00", 0);
            break;
        case 11:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "gor_04", 0);
            break;
        case 12:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "usu_00", 0);
            break;
        case 13:
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 28;
            seqSetSeq(SEQ_MAP_CHANGE, "muj_12", 0);
            break;
        case 14:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "usu_00", 0);
            break;
        case 15:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "muj_00", 0);
            break;
        case 16:
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 28;
            seqSetSeq(SEQ_MAP_CHANGE, "pik_03", "next");
            break;
        case 17:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "gor_01", 0);
            break;
        case 18:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "gor_04", "resha_return");
            break;
        case 19:
            gp->areaFadeInType = 62;
            gp->areaFadeOutType = 58;
            seqSetSeq(SEQ_MAP_CHANGE, "pik_01", 0);
            break;
        case 20:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_18", "e_bero");
            break;
        case 21:
            gp->areaFadeInType = 60;
            gp->areaFadeOutType = 54;
            seqSetSeq(SEQ_MAP_CHANGE, "gor_00", 0);
            break;
        case 22:
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 28;
            seqSetSeq(SEQ_MAP_CHANGE, "jin_04", 0);
            break;
        case 23:
            stg0_00_init();
            gp->saveSlot = wp->slot;
            marioPartyKill();
            {
                MarioWork* m = marioGetPtr();
                m->prevPartyMemberId[0] = 0;
                m->prevPartyMemberId[1] = 0;
            }
            gp->areaFadeInType = 0;
            gp->areaFadeOutType = 25;
            swInit();
            swByteSet(0, STG0_00_PROLOGUE);
            seqSetSeq(SEQ_MAP_CHANGE, "aaa_00", "prologue");
            break;
        case 24:
            gp->areaFadeInType = 61;
            gp->areaFadeOutType = 56;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
    }
}
#ifndef VERSION_G8ME01
#pragma defer_codegen reset
#endif
