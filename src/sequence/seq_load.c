#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include "seq/seq_load.h"
#include "sdk_decls.h"

#ifdef VERSION_G8ME01
// US version: all functions exact (loadDraw needs the two FAKE spellings marked inside it).
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
#include "win/win_mario.h"
typedef struct LoadWork {
    u16 flags;          // 0x0
    FileEntry* file;    // 0x4
    f32 x;              // 0x8
    f32 y;              // 0xC
    f32 tx;             // 0x10
    f32 ty;             // 0x14
    s32 unk18;          // 0x18
    s32 unk1C;          // 0x1C
    s32 slot;           // 0x20
    s32 unk24;          // 0x24
    s32 unk28;          // 0x28
    f32 alpha;          // 0x2C
    s32 unk30;          // 0x30
    s32 unk34;          // 0x34
    const char* msg;    // 0x38
    EventEntry* evt;    // 0x3C
    s32 unk40;          // 0x40
    s32 unk44;          // 0x44
    s32 unk48;          // 0x48
} LoadWork;

typedef struct WinDt {
    f32 x;      // 0x0
    f32 y;      // 0x4
    f32 ofsX;   // 0x8
    f32 ofsY;   // 0xC
    s32 unk10;  // 0x10
    s32 unk14;  // 0x14
    u8 next[4]; // 0x18
} WinDt;

extern void SoundSetOutputMode(s32 mode);
extern BOOL psndENVOn(u32 flags, const char* name);
extern BOOL psndBGMOff(u32 flags);
void nameEntOn(s32 type);
void nameEntOff(void);
const char* nameEntGetName(void);
BOOL nameEntWait(void);
BOOL nameEntPrepare(void);
BOOL nameEntIsCancel(void);
double sin(double);
double fmod(double, double);
static inline float fabsf(float x) { return (float)__fabs(x); }
void winTexInit_x2(TEXPalette* texture);
void winTexSet_x2(u32 id1, u32 id2, Vec translate, Vec scale, GXColor color);
void winTexSetRot(u32 id, Vec translate, Vec scale, GXColor color, f32 rot);
void nameMaskGX(f32 alpha);
void FontDrawScale(f32 scale);
void FontDrawString(f32 x, f32 y, const char* str);
void FontDrawEdge(void);
void FontDrawEdgeOff(void);
void FontDrawScaleVec(Vec scale);
const char* msgSearch(const char* key);
void loadMain(SequenceWork* seq);
void loadDraw(CameraId camera, void* param);
void continueGame(void);

__declspec(section ".data") f32 seq_load_negone_one_a[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_374[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 seq_load_one_negone_a[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_373[] = { 1.0f, -1.0f };
__declspec(section ".data") f32 seq_load_negone_one_b[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_360[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 seq_load_one_negone_b[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_359[] = { 1.0f, -1.0f };

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

__declspec(section ".data") static u32 gap_06_803DB834_bss;
static LoadWork work;
static LoadWork* wp = &work;
__declspec(section ".sdata2") const u32 seq_logo_zero_color[2];

static EvtWord evt_shake[12];

s32 evt_seq_load_keep_msg_window(EventEntry* evt, BOOL isFirstCall) {
    fadeReset(4);
    wp->unk48 = evt->printWindowId;
    return 2;
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

static inline BOOL loadStart(void);

void seq_loadMain(SequenceWork* seq) {
    switch (seq->state) {
        case 0:
            if (loadStart()) {
                seq->state++;
            }
            break;
        case 1:
            wp->evt = evtEntryType(evt_memcard_check, 0, 0, 0);
            if (wp->flags & 0x80) {
                wp->flags &= ~0x80;
                wp->evt->lwData[10] = 1;
                wp->evt->printWindowId = wp->unk48;
            }
            seq->state++;
            break;
        case 2:
            if (evtCheckID(wp->evt->eventId) == 0) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) != 1) {
                    seqSetSeq(SEQ_TITLE, NULL, NULL);
                } else {
                    wp->evt = NULL;
                    seq->state++;
                    wp->unk18 = 0;
                    wp->unk1C = cardGetCode();
                    wp->flags &= ~0x40;
                    if (OSGetSoundMode() == 0) {
                        SoundSetOutputMode(0);
                        gp->soundMode = 0;
                        wp->unk40 = 0;
                    } else {
                        SoundSetOutputMode(1);
                        if (gp->soundMode == 0) {
                            wp->unk40 = 1;
                        } else {
                            wp->unk40 = 2;
                        }
                    }
                    wp->unk44 = gp->rumbleEnabled == 0;
                    wp->unk24 = wp->unk40;
                    wp->unk28 = wp->unk44;
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

static inline BOOL loadStart(void) {
    const char* lang[] = {"jp", "us", "ge", "fr", "sp", "it"};
    if (fileAsyncf(4, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0U) {
        wp->file = fileAllocf(4, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]);
        return TRUE;
    }
    return FALSE;
}

#define LANG ((u32)gp->language)
#define SFX(id) psndSFXOn((const char*)(id))
#define SLOTFLAGS(i) (cardGetFilePtr()->saves[(i)].data.flags)

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
    if (wp->unk1C != code) {
        trg = 0;
        dir = 0;
    }
    wp->unk1C = code;
    for (i = 0; i < 4; i++) {
        flags = SLOTFLAGS(i);
        if (!(flags & 1) || (flags & 2)) {
            break;
        }
    }
    if (i < 4) {
        wp->flags &= ~0x10;
    } else {
        wp->flags |= 0x10;
    }
    wp->flags &= ~0x20;
    for (i = 0; i < 4; i++) {
        flags = SLOTFLAGS(i);
        if (!(flags & 1) && !(flags & 2)) {
            break;
        }
    }
    if (i >= 4) {
        wp->flags |= 0x20;
    }

    switch (wp->unk18) {
        case 0:
            wp->unk18++;
        case 1:
            wp->msg = msgSearch("msg_savefile_select");
            prev = wp->slot;
            if (dir & 0x1000) {
                wp->slot = win_dt[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt[prev].next[3];
            } else if (trg & 0x100) {
                flags = SLOTFLAGS(wp->slot);
                switch (wp->slot) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        if (flags & 2) {
                            SFX(0x13);
                        } else if (flags & 1) {
                            wp->unk18 = 0x1e;
                            SFX(0x12);
                        } else {
                            wp->unk18 = 0x14;
                            SFX(0x12);
                        }
                        break;
                    case 4:
                        if (!(wp->flags & 0x10)) {
                            for (i = 0; i < 4; i++) {
                                flags = SLOTFLAGS(i);
                                if (!(flags & 1) || (flags & 2)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(0x12);
                                wp->slot = 0;
                                wp->unk18 = 0x28;
                            } else {
                                SFX(0x13);
                            }
                        }
                        break;
                    case 5:
                        if (!(wp->flags & 0x20)) {
                            for (i = 0; i < 4; i++) {
                                flags = SLOTFLAGS(i);
                                if (!(flags & 2) && !(flags & 1)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(0x12);
                                wp->slot = 0;
                                wp->unk18 = 0x32;
                            } else {
                                SFX(0x13);
                            }
                        }
                        break;
                    case 6:
                        SFX(0x13);
                        wp->unk18 = 0xa;
                        break;
                    case 7:
                        SFX(0x12);
                        psndBGMOff(0x800);
                        wp->unk18 = 0x64;
                        break;
                    case 8:
                        SFX(0x12);
                        wp->unk18 = 0xc8;
                        break;
                }
            } else if (trg & 0x200) {
                SFX(0x13);
                wp->unk18 = 0xa;
            }
            if (prev != wp->slot) {
                if (wp->slot < 4) {
                    SFX(9);
                } else {
                    SFX(5);
                }
            }
            wp->tx = win_dt[wp->slot].x + win_dt[wp->slot].ofsX;
            wp->ty = win_dt[wp->slot].y + win_dt[wp->slot].ofsY;
            break;
        case 0xa:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->unk18++;
            break;
        case 0xb:
            if (fadeIsFinish()) {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
        case 0x14:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_start, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->unk18++;
            break;
        case 0x15:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~0x40;
                if (evtGetValue(NULL, (s32)0xFD050F80) == 1) {
                    wp->unk18++;
                } else {
                    wp->unk18 = 1;
                }
            }
            break;
        case 0x16:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            psndBGMOff_f_d(0x200, 2000, 0);
            wp->unk18++;
            break;
        case 0x17:
            if (fadeIsFinish()) {
                wp->flags |= 0x40;
                wp->evt = evtEntryType(evt_continue, 0, 0, 0);
                wp->unk18++;
            }
            break;
        case 0x18:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~0x40;
                continueGame();
            }
            break;
        case 0x1e:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->unk18++;
            break;
        case 0x1f:
            if (fadeIsFinish()) {
                wp->unk18++;
            }
            break;
        case 0x20:
            if (nameEntPrepare()) {
                nameEntOn(0);
                fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                wp->unk18++;
            }
            break;
        case 0x21:
            if (fadeIsFinish() && !nameEntWait()) {
                fadeEntry(0x18, 0, (GXColor){0, 0, 0, 255});
                if (!nameEntIsCancel()) {
                    psndBGMOff_f_d(0x200, 3000, 0);
                }
                wp->unk18++;
            }
            break;
        case 0x22:
            if (fadeIsFinish()) {
                nameEntOff();
                if (nameEntIsCancel()) {
                    fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                    wp->unk18 = 1;
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
                    swByteSet(0, 0);
                    if (code == 0) {
                        gp->saveSlot = wp->slot;
                        wp->evt = evtEntryType(evt_memcard_save_wrapper, 0, 0, 0);
                        wp->evt->lwData[0] = 0x17;
                        wp->unk18++;
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
            if (dir & 0x1000) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & 0x100) {
                if (prev < 4) {
                    flags = SLOTFLAGS(wp->slot);
                    if ((flags & 2) || !(flags & 1)) {
                        SFX(0x12);
                        wp->unk18++;
                    } else {
                        SFX(0x13);
                    }
                } else {
                    wp->slot = 4;
                    wp->unk18 = 1;
                    SFX(0x13);
                }
            } else if (trg & 0x200) {
                wp->slot = 4;
                wp->unk18 = 1;
                SFX(0x13);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x29:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_delete, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->unk18++;
            break;
        case 0x2a:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) == 0) {
                    wp->flags &= ~0x40;
                    wp->slot = 4;
                    wp->unk18 = 1;
                } else {
                    wp->flags |= 0x80;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x32:
            wp->msg = msgSearch("msg_savefile_copy_select");
            prev = wp->slot;
            if (dir & 0x1000) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & 0x100) {
                if (prev < 4) {
                    flags = SLOTFLAGS(wp->slot);
                    if (!(flags & 2) && !(flags & 1)) {
                        wp->unk18 = 0x33;
                        wp->unk30 = wp->slot;
                        wp->unk34 = (wp->unk30 + 1) % 4;
                        wp->slot = wp->unk34;
                        SFX(0x12);
                    }
                } else {
                    wp->slot = 5;
                    wp->unk18 = 1;
                    SFX(0x13);
                }
            } else if (trg & 0x200) {
                wp->slot = 5;
                wp->unk18 = 1;
                SFX(0x13);
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
            if (dir & 0x1000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[0];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x2000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[1];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x4000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[2];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x8000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[3];
                } while (wp->slot == wp->unk30);
            } else if (trg & 0x100) {
                wp->unk18 = 0x34;
                wp->unk34 = wp->slot;
                SFX(0x12);
            } else if (trg & 0x200) {
                wp->slot = wp->unk30;
                wp->unk18 = 0x32;
                SFX(0x13);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->unk34 = wp->slot;
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x34:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_copy, 0, 0, 0);
            wp->evt->lwData[0] = wp->unk30;
            wp->evt->lwData[1] = wp->unk34;
            wp->unk18++;
            break;
        case 0x35:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) == 0) {
                    wp->flags &= ~0x40;
                    wp->slot = 5;
                    wp->unk18 = 1;
                } else {
                    wp->flags |= 0x80;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x64:
            wp->flags |= 1;
            sound = wp->unk24;
            if (dir & 0x1000) {
                wp->unk24 = wp->unk24 - 1;
                SFX(5);
            } else if (dir & 0x2000) {
                wp->unk24 = wp->unk24 + 1;
                SFX(5);
            } else if (trg & 0x100) {
                switch (sound) {
                    case 0:
                        OSSetSoundMode(0);
                        gp->soundMode = 0;
                        break;
                    case 1:
                        OSSetSoundMode(1);
                        gp->soundMode = 0;
                        break;
                    case 2:
                        OSSetSoundMode(1);
                        gp->soundMode = 1;
                        break;
                }
                if (OSGetSoundMode() == 0) {
                    SoundSetOutputMode(0);
                } else {
                    SoundSetOutputMode(1);
                }
                SFX(0x12);
                psndBGMOn(0xa0, NULL);
                psndENVOff(0x200);
                wp->unk40 = wp->unk24;
                wp->flags &= ~1;
                wp->unk18 = 1;
                break;
            } else if (trg & 0x200) {
                SFX(0x13);
                psndBGMOn(0xa0, NULL);
                psndENVOff(0x200);
                wp->unk24 = wp->unk40;
                if (OSGetSoundMode() == 0) {
                    SoundSetOutputMode(0);
                } else {
                    SoundSetOutputMode(1);
                }
                wp->flags &= ~1;
                wp->unk18 = 1;
                break;
            }
            if (wp->unk24 >= 3) {
                wp->unk24 = 0;
            }
            if (wp->unk24 < 0) {
                wp->unk24 = 2;
            }
            if (wp->unk24 != sound) {
                switch (wp->unk24) {
                    case 0:
                        SoundSetOutputMode(0);
                        break;
                    case 1:
                        SoundSetOutputMode(1);
                        break;
                    case 2:
                        SoundSetOutputMode(2);
                        break;
                }
                psndENVOn(0x110, (const char*)0x75);
            }
            wp->tx = -74.0f;
            wp->ty = 4.0f + (50.0f - 50.0f * wp->unk24);
            break;
        case 0xc8:
            wp->flags |= 1;
            if (dir & 0x1000) {
                wp->unk28--;
                SFX(5);
            } else if (dir & 0x2000) {
                wp->unk28++;
                SFX(5);
            } else if (trg & 0x100) {
                gp->rumbleEnabled = wp->unk28 == 0;
                SFX(0x12);
                wp->unk44 = wp->unk28;
                wp->flags &= ~1;
                wp->unk18 = 1;
                if (gp->rumbleEnabled) {
                    evtEntryType(evt_shake, 0, 0, 0);
                }
            } else if (trg & 0x200) {
                SFX(0x13);
                wp->unk28 = wp->unk44;
                wp->flags &= ~1;
                wp->unk18 = 1;
            }
            if (wp->unk28 >= 2) {
                wp->unk28 = 0;
            }
            if (wp->unk28 < 0) {
                wp->unk28 = 1;
            }
            wp->tx = -74.0f;
            wp->ty = 4.0f + (30.0f - 50.0f * wp->unk28);
            break;
    }
    wp->x += (wp->tx - wp->x) / 4.0f;
    wp->y += (wp->ty - wp->y) / 4.0f;
    { f32 s = 2.0f * (f32)sin(gp->retraceCount / 8.0f); wp->x += __fabsf(s); }
    dispEntry(CAMERA_3D, 1000.0f, 0, loadDraw, NULL);
}

#define GXFIFO_F32 (*(volatile f32*)0xCC008000)

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
    GXFIFO_F32 = x0;
    GXFIFO_F32 = y0;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = x0;
    GXFIFO_F32 = y1;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 1.0f;
    GXFIFO_F32 = x1;
    GXFIFO_F32 = y1;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 1.0f;
    GXFIFO_F32 = 1.0f;
    GXFIFO_F32 = x1;
    GXFIFO_F32 = y0;
    GXFIFO_F32 = 0.0f;
    GXFIFO_F32 = 1.0f;
    GXFIFO_F32 = 0.0f;
}

void loadDraw(CameraId camera, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    GXColor white = {255, 255, 255, 255};
    GXColor gray = {160, 160, 160, 255};
    GXColor dark = {128, 128, 128, 128};
    f32 vp[6];
    f32 proj[7];
    CameraEntry saved;
    char buf[0x20];
    char buf2[0x100];
    Mtx m;
    Mtx s;
    f32 x;
    f32 y;
    f32 w;
    u64 sec;
    WinDt* win1[1]; // FAKE: one-element array, so IRO turns win1 into an @ temp: the address of win_dt is computed into
                    // win1 itself and loop 1 walks a copy of it (the addi r30 / mr r24,r30 direction of the target)
    s32 i;
    s32 k;

    if (wp->unk18 >= 0x20 && wp->unk18 < 0x28) {
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
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    loadDrawBg(0x35, -304.0f, 240.0f, -272.0f, -240.0f);
    loadDrawBg(0x34, -272.0f, 240.0f, 272.0f, -240.0f);
    loadDrawBg(0x36, 272.0f, 240.0f, 304.0f, -240.0f);

    win1[0] = win_dt;
    for (i = 0; i < 9; i++) {
        GXColor col = gray;
        if (i == wp->slot) {
            col = white;
        }
        if (wp->unk18 == 0x33 && (i == wp->unk30 || i == wp->unk34)) {
            col = white;
        }
        if (i == 4 && (wp->flags & 0x10)) {
            col = dark;
        }
        if (i == 5 && (wp->flags & 0x20)) {
            col = dark;
        }
        switch (win1[0][i].unk10) {
            case 10:
            case 11:
            case 12:
            case 13: {
                col.a = 0xfe;
                winTexInit(*wp->file->data);
                winTexSet(0xe, (Vec){win1[0][i].x, win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
                winTexInit(*wp->file->data);
                winTexSet(win1[0][i].unk10, (Vec){win1[0][i].x, 60.0f + win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
                break;
            }
            default: {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(win1[0][i].unk10, win1[0][i].unk14, (Vec){win1[0][i].x, win1[0][i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, col);
                break;
            }
        }
    }

    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, -250.0f, 200.0f, 500.0f, 40.0f, 20.0f);
    FontDrawStart();
    {
        const char* msg = wp->msg;
        FontDrawMessage(-FontGetMessageWidth(msg) / 2, 192, msg);
    }

    for (i = 0; i < 4; i++, win1[0]++) {
        CardSaveSlot* save = &cardGetFilePtr()->saves[i];
        u16 flags;
        OSTime time;
        u16 stars;
        s32 gsw0;
        char* name;
        s16 level;
        const char* str;
        s32 hours;
        s32 mins;
        flags = save->data.flags;
        level = save->data.pouch.level;
        time = save->data.global.retraceLocalTime;
        stars = save->data.pouch.mStarPowersObtained;
        gsw0 = save->data.global.mGSW0;
        name = (char*)save->data.global.playerName;

        FontDrawStart();
        FontDrawColor((GXColor){192, 192, 192, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){255, 255, 255, 255});
        }
        FontDrawScale(0.6f);
        FontDrawEdge();
        if (LANG == 0) {
            str = "\x83\x66\x81\x5B\x83\x5E";
        } else {
            str = msgSearch("msg_mcard_file");
        }
        sprintf(buf, "%s%s", str, winZenkakuStr(i + 1));
        FontDrawString(win1[0]->x - 100.0f, 84.0f + win1[0]->y, buf);
        FontDrawEdgeOff();
        FontDrawScale(1.0f);
        FontDrawColor((GXColor){32, 32, 32, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){0, 0, 0, 255});
        }
        if (flags & 2) {
            if (LANG == 0) {
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
            if (LANG == 0) {
                str = "\x83\x6A\x83\x85\x81\x5B\x83\x51\x81\x5B\x83\x80";
            } else {
                str = msgSearch("msg_mcard_new_game");
            }
            FontDrawString(win1[0]->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1[0]->y, str);
        } else if (gsw0 == 0) {
            if (LANG == 0) {
                str = "\x82\xCD\x82\xB6\x82\xDF\x82\xA9\x82\xE7";
            } else {
                str = msgSearch("msg_mcard_hajimekara");
            }
            FontDrawString(win1[0]->x - FontGetMessageWidth(str) / 2.0f, 12.0f + win1[0]->y, str);
            if (strlen(name) != 0) {
                FontDrawString(win1[0]->x - FontGetMessageWidth(name) / 2, 70.0f + win1[0]->y, name);
            }
        } else {
            FontGetMessageWidth(LANG != 0 ? "88" : "\x82\x57\x82\x57");
            sec = OSTicksToSeconds(time);
            if (sec > 359999) {
                hours = 99;
                mins = 59;
            } else {
                hours = (u64)sec / 3600;
                mins = ((u64)sec % 3600) / 60;
            }
            if (strlen(name) != 0) {
                FontDrawString(win1[0]->x - FontGetMessageWidth(name) / 2, 70.0f + win1[0]->y, name);
            }
            sprintf(buf2, LANG != 0 ? "%s %s" : "%s\x81\x40%s", msgSearch("msg_menu_mario_level"), winZenkakuStr(level));
            FontDrawString(win1[0]->x - FontGetMessageWidth(buf2) / 2.0f, 30.0f + win1[0]->y, buf2);
            sprintf(buf2, LANG != 0 ? "%s " : "%s\x81\x40", msgSearch("msg_menu_mario_num_playtime"));
            strcat(buf2, winZenkakuStrPad(hours, 2));
            strcat(buf2, " ");
            strcat(buf2, winZenkakuStrPad(mins, 2));
            w = FontGetMessageWidth(buf2);
            if (w > 200.0f) {
                FontDrawScaleVec((Vec){200.0f / w, 1.0f, 1.0f});
                w = 200.0f;
            }
            FontDrawString(win1[0]->x - w / 2.0f, win1[0]->y, buf2);
            FontDrawScaleVec((Vec){1.0f, 1.0f, 1.0f});
            w = FontGetMessageWidth(buf2);
            sprintf(buf2, LANG != 0 ? "%s " : "%s\x81\x40", msgSearch("msg_menu_mario_num_playtime"));
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
            PSMTXScale(s, 1.5f, 1.5f, 1.5f);
            PSMTXConcat(m, s, m);
            iconDispGxCol(m, 0, 0x208, (GXColor){0, 0, 0, 255});
            for (k = 0; k < 7; k++) {
                if (stars & (1 << (k + 1))) {
                    PSMTXTrans(m, (win1[0]->x - 106.0f) + k * 32, *(f32*)((u8*)win1[0] + offsetof(WinDt, y)) /* FAKE: win1->y read through a byte offset (same load) keeps &win1->y a late loop temp, as in the target's numbering */ - 52.0f, 0.0f);
                    iconDispGxCol(m, 0, (u16)stone_id[k], i == wp->slot ? white : gray);
                }
            }
        }
    }

    if (wp->flags & 1) {
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

    if (wp->unk18 >= 100 && wp->unk18 < 200) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x13, 0x1f, (Vec){0.0f, 100.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 3; i++) {
            if (i == wp->unk24) {
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

    if (wp->unk18 >= 200 && wp->unk18 < 300) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x14, 0x1f, (Vec){0.0f, 100.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 2; i++) {
            if (i == wp->unk28) {
                winTexInit(*wp->file->data);
                winTexSet(0x20, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x20, (Vec){-4.0f, 34 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x20, (Vec){0.0f, 30 - i * 50, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){255, 255, 255, 255});
            }
        }
    }

    if (wp->unk18 == 0x33 && wp->unk34 < 4) {
        f32 angle[4][4] = {
            {0.0f, -90.0f, -180.0f, -135.0f},
            {90.0f, 0.0f, 135.0f, 180.0f},
            {0.0f, -45.0f, 0.0f, -90.0f},
            {45.0f, 0.0f, 90.0f, 0.0f},
        };
        f32 cx;
        f32 cy;
        if (!(f32)fmod(angle[wp->unk30][wp->unk34], 90.0)) {
            f32 c0 = wp->unk30 % 2;
            f32 r0 = wp->unk30 / 2;
            f32 r1 = wp->unk34 / 2;
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
        winTexSetRot(0x1d, (Vec){2.0f + cx, cy - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 128}, angle[wp->unk30][wp->unk34]);
        winTexSetRot(0x1d, (Vec){cx, cy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white, angle[wp->unk30][wp->unk34]);
    }

    if (!(wp->flags & 0x40)) {
        winTexInit(*wp->file->data);
        winTexSet(0, (Vec){2.0f + wp->x, wp->y - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 128});
        winTexSet(0, (Vec){wp->x, wp->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }

    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
    *camGetCurPtr() = saved;
}

s32 stone_id[7] = {0x197 + 8, 0x198 + 8, 0x19A + 8, 0x19C + 8, 0x19D + 8, 0x199 + 8, 0x19B + 8};

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
    psndSetFlag(4);
    marioPartyEntry(gp->savePartyId[0]);
    marioPartyEntry(gp->savePartyId[1]);
    psndClearFlag(4);
    countDownSaveReStart();
    gp->flags |= 1;
    switch (swByteGet(4)) {
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
            swByteSet(0, 0);
            seqSetSeq(SEQ_MAP_CHANGE, "aaa_00", "prologue");
            break;
        case 24:
            gp->areaFadeInType = 0x3d;
            gp->areaFadeOutType = 0x38;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
    }
}
#else
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
#include "evt/evt_cmd.h"
#define EXT_LIT(sym, val) (val)

typedef struct LoadWork {
    u16 flags;          // 0x0
    FileEntry* file;    // 0x4
    f32 x;              // 0x8
    f32 y;              // 0xC
    f32 tx;             // 0x10
    f32 ty;             // 0x14
    s32 unk18;          // 0x18
    s32 unk1C;          // 0x1C
    s32 slot;           // 0x20
    s32 unk24;          // 0x24
    s32 unk28;          // 0x28
    f32 alpha;          // 0x2C
    s32 unk30;          // 0x30
    s32 unk34;          // 0x34
    const char* msg;    // 0x38
    EventEntry* evt;    // 0x3C
    s32 unk40;          // 0x40
    s32 unk44;          // 0x44
    s32 unk48;          // 0x48
} LoadWork;

typedef struct WinDt {
    f32 x;      // 0x0
    f32 y;      // 0x4
    f32 ofsX;   // 0x8
    f32 ofsY;   // 0xC
    s32 unk10;  // 0x10
    s32 unk14;  // 0x14
    u8 next[4]; // 0x18
} WinDt;

extern EvtWord evt_memcard_check[];
extern EvtWord evt_memcard_start[];
extern EvtWord evt_memcard_save[];
extern EvtWord evt_memcard_delete[];
extern EvtWord evt_memcard_copy[];
extern s32 evt_sub_rumble_onoff(EventEntry* evt, BOOL isFirstCall);
extern s32 evt_sub_animgroup_async(EventEntry* evt, BOOL isFirstCall);
extern void SoundSetOutputMode(s32 mode);
extern void OSSetSoundMode(u32 mode);
extern BOOL psndENVOn(u32 flags, const char* name);
extern BOOL psndBGMOff(u32 flags);
void nameEntOn(s32 type);
void nameEntOff(void);
const char* nameEntGetName(void);
BOOL nameEntWait(void);
BOOL nameEntPrepare(void);
BOOL nameEntIsCancel(void);
double sin(double);
double fmod(double, double);
static inline float fabsf(float x) { return (float)__fabs(x); }
void winTexInit_x2(TEXPalette* texture);
void winTexSet_x2(u32 id1, u32 id2, Vec translate, Vec scale, GXColor color);
void winTexSetRot(u32 id, Vec translate, Vec scale, GXColor color, f32 rot);
void nameMaskGX(f32 alpha);
const char* winZenkakuStrPad(s32 num, s32 digits);
void FontDrawScale(f32 scale);
void FontDrawString(f32 x, f32 y, const char* str);
void FontDrawEdge(void);
void FontDrawEdgeOff(void);
void loadMain(SequenceWork* seq);
void loadDraw(CameraId camera, void* param);
void continueGame(void);

__declspec(section ".data") f32 seq_load_negone_one_a[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_374[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 seq_load_one_negone_a[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_373[] = { 1.0f, -1.0f };
__declspec(section ".data") f32 seq_load_negone_one_b[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_360[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 seq_load_one_negone_b[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_359[] = { 1.0f, -1.0f };

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

__declspec(section ".data") static u32 gap_06_803DB834_bss;
static LoadWork work;
static LoadWork* wp = &work;

extern s32 stone_id[];
static EvtWord evt_shake[12];
extern EvtWord evt_continue[];

s32 evt_seq_load_keep_msg_window(EventEntry* evt, BOOL isFirstCall) {
    fadeReset(4);
    wp->unk48 = evt->printWindowId;
    return 2;
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

static inline BOOL loadStart(void) {
    if (fileAsyncf(4, NULL, "%s/w/jp/name.tpl\0\0\0", getMarioStDvdRoot()) != 0U) {
        wp->file = fileAllocf(4, "%s/w/jp/name.tpl\0\0\0", getMarioStDvdRoot());
        return TRUE;
    }
    return FALSE;
}

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);
void GXClearVtxDesc(void);
void GXGetProjectionv(f32* ptr);
u16 GXGetTexObjHeight(const GXTexObj* to);
u16 GXGetTexObjWidth(const GXTexObj* to);
void GXGetViewportv(f32* vp);
void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt,
    f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
    GXBool do_edge_lod, GXAnisotropy max_aniso);
void GXLoadPosMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type);
void GXLoadTexObj(GXTexObj* obj, GXTexMapID id);
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1);
void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn);
void GXSetCullMode(GXCullMode mode);
void GXSetCurrentMtx(u32 id);
void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color);
void GXSetNumChans(u8 nChans);
void GXSetNumTevStages(u8 nStages);
void GXSetNumTexGens(u8 nTexGens);
void GXSetProjection(const f32 mtx[4][4], GXProjectionType type);
void GXSetProjectionv(const f32* ptr);
void GXSetTevOp(GXTevStageID id, GXTevMode mode);
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color);
void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx, GXBool normalize, u32 pt_texmtx);
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
void GXSetZCompLoc(GXBool before_tex);
void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable);
u32 OSGetSoundMode(void);
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
int __div2i();
int __div2u();
int __mod2u();
int sprintf(char *s, const char *format, ...);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *s);
int winZenkakuStr();
void seq_loadMain(SequenceWork* seq) {
    switch (seq->state) {
        case 0:
            if (loadStart()) {
                seq->state++;
            }
            break;
        case 1:
            wp->evt = evtEntryType(evt_memcard_check, 0, 0, 0);
            if (wp->flags & 0x80) {
                wp->flags &= ~0x80;
                wp->evt->lwData[10] = 1;
                wp->evt->printWindowId = wp->unk48;
            }
            seq->state++;
            break;
        case 2:
            if (evtCheckID(wp->evt->eventId) == 0) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) != 1) {
                    seqSetSeq(SEQ_TITLE, NULL, NULL);
                } else {
                    wp->evt = NULL;
                    seq->state++;
                    wp->unk18 = 0;
                    wp->unk1C = cardGetCode();
                    wp->flags &= ~0x40;
                    if (OSGetSoundMode() == 0) {
                        SoundSetOutputMode(0);
                        gp->soundMode = 0;
                        wp->unk40 = 0;
                    } else {
                        SoundSetOutputMode(1);
                        if (gp->soundMode == 0) {
                            wp->unk40 = 1;
                        } else {
                            wp->unk40 = 2;
                        }
                    }
                    wp->unk44 = gp->rumbleEnabled == 0;
                    wp->unk24 = wp->unk40;
                    wp->unk28 = wp->unk44;
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

#define SFX(id) psndSFXOn((const char*)(id))
#define SLOTFLAGS(i) (cardGetFilePtr()->saves[(i)].data.flags)

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
    if (wp->unk1C != code) {
        trg = 0;
        dir = 0;
    }
    wp->unk1C = code;
    for (i = 0; i < 4; i++) {
        flags = SLOTFLAGS(i);
        if (!(flags & 1) || (flags & 2)) {
            break;
        }
    }
    if (i < 4) {
        wp->flags &= ~0x10;
    } else {
        wp->flags |= 0x10;
    }
    wp->flags &= ~0x20;
    for (i = 0; i < 4; i++) {
        flags = SLOTFLAGS(i);
        if (!(flags & 1) && !(flags & 2)) {
            break;
        }
    }
    if (i >= 4) {
        wp->flags |= 0x20;
    }

    switch (wp->unk18) {
        case 0:
            wp->unk18++;
        case 1:
            wp->msg = msgSearch("msg_savefile_select");
            prev = wp->slot;
            if (dir & 0x1000) {
                wp->slot = win_dt[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt[prev].next[3];
            } else if (trg & 0x100) {
                flags = SLOTFLAGS(wp->slot);
                switch (wp->slot) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        if (flags & 2) {
                            SFX(0x13);
                        } else if (flags & 1) {
                            wp->unk18 = 0x1e;
                            SFX(0x12);
                        } else {
                            wp->unk18 = 0x14;
                            SFX(0x12);
                        }
                        break;
                    case 4:
                        if (!(wp->flags & 0x10)) {
                            for (i = 0; i < 4; i++) {
                                flags = SLOTFLAGS(i);
                                if (!(flags & 1) || (flags & 2)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(0x12);
                                wp->slot = 0;
                                wp->unk18 = 0x28;
                            } else {
                                SFX(0x13);
                            }
                        }
                        break;
                    case 5:
                        if (!(wp->flags & 0x20)) {
                            for (i = 0; i < 4; i++) {
                                flags = SLOTFLAGS(i);
                                if (!(flags & 2) && !(flags & 1)) {
                                    break;
                                }
                            }
                            if (i < 4) {
                                SFX(0x12);
                                wp->slot = 0;
                                wp->unk18 = 0x32;
                            } else {
                                SFX(0x13);
                            }
                        }
                        break;
                    case 6:
                        SFX(0x13);
                        wp->unk18 = 0xa;
                        break;
                    case 7:
                        SFX(0x12);
                        psndBGMOff(0x800);
                        wp->unk18 = 0x64;
                        break;
                    case 8:
                        SFX(0x12);
                        wp->unk18 = 0xc8;
                        break;
                }
            } else if (trg & 0x200) {
                SFX(0x13);
                wp->unk18 = 0xa;
            }
            if (prev != wp->slot) {
                if (wp->slot < 4) {
                    SFX(9);
                } else {
                    SFX(5);
                }
            }
            wp->tx = win_dt[wp->slot].x + win_dt[wp->slot].ofsX;
            wp->ty = win_dt[wp->slot].y + win_dt[wp->slot].ofsY;
            break;
        case 0xa:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->unk18++;
            break;
        case 0xb:
            if (fadeIsFinish()) {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
        case 0x14:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_start, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->unk18++;
            break;
        case 0x15:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~0x40;
                if (evtGetValue(NULL, (s32)0xFD050F80) == 1) {
                    wp->unk18++;
                } else {
                    wp->unk18 = 1;
                }
            }
            break;
        case 0x16:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            psndBGMOff_f_d(0x200, 2000, 0);
            wp->unk18++;
            break;
        case 0x17:
            if (fadeIsFinish()) {
                wp->flags |= 0x40;
                wp->evt = evtEntryType(evt_continue, 0, 0, 0);
                wp->unk18++;
            }
            break;
        case 0x18:
            if (!evtCheckID(wp->evt->eventId)) {
                wp->flags &= ~0x40;
                continueGame();
            }
            break;
        case 0x1e:
            fadeEntry(0x1b, 0, (GXColor){0, 0, 0, 255});
            wp->unk18++;
            break;
        case 0x1f:
            if (fadeIsFinish()) {
                wp->unk18++;
            }
            break;
        case 0x20:
            if (nameEntPrepare()) {
                nameEntOn(0);
                fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                wp->unk18++;
            }
            break;
        case 0x21:
            if (fadeIsFinish() && !nameEntWait()) {
                fadeEntry(0x18, 0, (GXColor){0, 0, 0, 255});
                if (!nameEntIsCancel()) {
                    psndBGMOff_f_d(0x200, 3000, 0);
                }
                wp->unk18++;
            }
            break;
        case 0x22:
            if (fadeIsFinish()) {
                nameEntOff();
                if (nameEntIsCancel()) {
                    fadeEntry(0x1c, 0, (GXColor){0, 0, 0, 255});
                    wp->unk18 = 1;
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
                    swByteSet(0, 0);
                    if (code == 0) {
                        gp->saveSlot = wp->slot;
                        wp->evt = evtEntryType(evt_memcard_save, 0, 0, 0);
                        wp->evt->lwData[0] = 0x17;
                        wp->unk18++;
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
            if (dir & 0x1000) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & 0x100) {
                if (prev < 4) {
                    flags = SLOTFLAGS(wp->slot);
                    if ((flags & 2) || !(flags & 1)) {
                        SFX(0x12);
                        wp->unk18++;
                    } else {
                        SFX(0x13);
                    }
                } else {
                    wp->slot = 4;
                    wp->unk18 = 1;
                    SFX(0x13);
                }
            } else if (trg & 0x200) {
                wp->slot = 4;
                wp->unk18 = 1;
                SFX(0x13);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x29:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_delete, 0, 0, 0);
            wp->evt->lwData[0] = wp->slot;
            wp->unk18++;
            break;
        case 0x2a:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) == 0) {
                    wp->flags &= ~0x40;
                    wp->slot = 4;
                    wp->unk18 = 1;
                } else {
                    wp->flags |= 0x80;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x32:
            wp->msg = msgSearch("msg_savefile_copy_select");
            prev = wp->slot;
            if (dir & 0x1000) {
                wp->slot = win_dt2[prev].next[0];
            } else if (dir & 0x2000) {
                wp->slot = win_dt2[prev].next[1];
            } else if (dir & 0x4000) {
                wp->slot = win_dt2[prev].next[2];
            } else if (dir & 0x8000) {
                wp->slot = win_dt2[prev].next[3];
            } else if (trg & 0x100) {
                if (prev < 4) {
                    flags = SLOTFLAGS(wp->slot);
                    if (!(flags & 2) && !(flags & 1)) {
                        wp->unk18 = 0x33;
                        wp->unk30 = wp->slot;
                        wp->unk34 = (wp->unk30 + 1) % 4;
                        wp->slot = wp->unk34;
                        SFX(0x12);
                    }
                } else {
                    wp->slot = 5;
                    wp->unk18 = 1;
                    SFX(0x13);
                }
            } else if (trg & 0x200) {
                wp->slot = 5;
                wp->unk18 = 1;
                SFX(0x13);
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
            if (dir & 0x1000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[0];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x2000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[1];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x4000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[2];
                } while (wp->slot == wp->unk30);
            } else if (dir & 0x8000) {
                do {
                    wp->slot = win_dt3[wp->slot].next[3];
                } while (wp->slot == wp->unk30);
            } else if (trg & 0x100) {
                wp->unk18 = 0x34;
                wp->unk34 = wp->slot;
                SFX(0x12);
            } else if (trg & 0x200) {
                wp->slot = wp->unk30;
                wp->unk18 = 0x32;
                SFX(0x13);
            }
            if (prev != wp->slot && wp->slot < 4) {
                SFX(9);
            }
            wp->unk34 = wp->slot;
            wp->tx = win_dt2[wp->slot].x + win_dt2[wp->slot].ofsX;
            wp->ty = win_dt2[wp->slot].y + win_dt2[wp->slot].ofsY;
            break;
        case 0x34:
            wp->flags |= 0x40;
            wp->evt = evtEntryType(evt_memcard_copy, 0, 0, 0);
            wp->evt->lwData[0] = wp->unk30;
            wp->evt->lwData[1] = wp->unk34;
            wp->unk18++;
            break;
        case 0x35:
            if (!evtCheckID(wp->evt->eventId)) {
                if (evtGetValue(NULL, (s32)0xFAA2B580) == 0) {
                    wp->flags &= ~0x40;
                    wp->slot = 5;
                    wp->unk18 = 1;
                } else {
                    wp->flags |= 0x80;
                    seq->state = 1;
                    return;
                }
            }
            break;
        case 0x64:
            wp->flags |= 1;
            sound = wp->unk24;
            if (dir & 0x1000) {
                wp->unk24 = wp->unk24 - 1;
                SFX(5);
            } else if (dir & 0x2000) {
                wp->unk24 = wp->unk24 + 1;
                SFX(5);
            } else if (trg & 0x100) {
                switch (sound) {
                    case 0:
                        OSSetSoundMode(0);
                        gp->soundMode = 0;
                        break;
                    case 1:
                        OSSetSoundMode(1);
                        gp->soundMode = 0;
                        break;
                    case 2:
                        OSSetSoundMode(1);
                        gp->soundMode = 1;
                        break;
                }
                if (OSGetSoundMode() == 0) {
                    SoundSetOutputMode(0);
                } else {
                    SoundSetOutputMode(1);
                }
                SFX(0x12);
                psndBGMOn(0xa0, NULL);
                psndENVOff(0x200);
                wp->unk40 = wp->unk24;
                wp->flags &= ~1;
                wp->unk18 = 1;
                break;
            } else if (trg & 0x200) {
                SFX(0x13);
                psndBGMOn(0xa0, NULL);
                psndENVOff(0x200);
                wp->unk24 = wp->unk40;
                if (OSGetSoundMode() == 0) {
                    SoundSetOutputMode(0);
                } else {
                    SoundSetOutputMode(1);
                }
                wp->flags &= ~1;
                wp->unk18 = 1;
                break;
            }
            if (wp->unk24 >= 3) {
                wp->unk24 = 0;
            }
            if (wp->unk24 < 0) {
                wp->unk24 = 2;
            }
            if (wp->unk24 != sound) {
                switch (wp->unk24) {
                    case 0:
                        SoundSetOutputMode(0);
                        break;
                    case 1:
                        SoundSetOutputMode(1);
                        break;
                    case 2:
                        SoundSetOutputMode(2);
                        break;
                }
                psndENVOn(0x110, (const char*)0x75);
            }
            wp->tx = EXT_LIT(float_neg74_8041be74, -74.0f);
            wp->ty = EXT_LIT(float_4_8041be78, 4.0f) + (EXT_LIT(float_50_8041be6c, 50.0f) - 50.0f * wp->unk24);
            break;
        case 0xc8:
            wp->flags |= 1;
            if (dir & 0x1000) {
                wp->unk28--;
                SFX(5);
            } else if (dir & 0x2000) {
                wp->unk28++;
                SFX(5);
            } else if (trg & 0x100) {
                gp->rumbleEnabled = wp->unk28 == 0;
                SFX(0x12);
                wp->unk44 = wp->unk28;
                wp->flags &= ~1;
                wp->unk18 = 1;
                if (gp->rumbleEnabled) {
                    evtEntryType(evt_shake, 0, 0, 0);
                }
            } else if (trg & 0x200) {
                SFX(0x13);
                wp->unk28 = wp->unk44;
                wp->flags &= ~1;
                wp->unk18 = 1;
            }
            if (wp->unk28 >= 2) {
                wp->unk28 = 0;
            }
            if (wp->unk28 < 0) {
                wp->unk28 = 1;
            }
            wp->tx = EXT_LIT(float_neg74_8041be74, -74.0f);
            wp->ty = EXT_LIT(float_4_8041be78, 4.0f) + (EXT_LIT(float_30_8041be3c, 30.0f) - 50.0f * wp->unk28);
            break;
    }
    wp->x += (wp->tx - wp->x) / EXT_LIT(float_4_8041be78, 4.0f);
    wp->y += (wp->ty - wp->y) / EXT_LIT(float_4_8041be78, 4.0f);
    { f32 s = 2.0f * (f32)sin(gp->retraceCount / 8.0f); wp->x += __fabsf(s); }
    dispEntry(CAMERA_3D, EXT_LIT(float_1000_8041be84, 1000.0f), 0, loadDraw, NULL);
}

#define GXFIFO_F32 (*(volatile f32*)0xCC008000)

static inline void loadDrawBg(u32 id, f32 x0, f32 y0, f32 x1, f32 y1) {
    GXTexObj obj;
    Mtx m;
    f64 w;
    f64 h;

    TEXGetGXTexObjFromPalette(*wp->file->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    h = __fabs(y0 - y1);
    w = __fabs(x1 - x0);
    PSMTXScale(m, (f32)w / GXGetTexObjWidth(&obj), (f32)h / GXGetTexObjHeight(&obj), EXT_LIT(float_1_8041bde8, 1.0f));
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXFIFO_F32 = x0;
    GXFIFO_F32 = y0;
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = x0;
    GXFIFO_F32 = y1;
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_1_8041bde8, 1.0f);
    GXFIFO_F32 = x1;
    GXFIFO_F32 = y1;
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_1_8041bde8, 1.0f);
    GXFIFO_F32 = EXT_LIT(float_1_8041bde8, 1.0f);
    GXFIFO_F32 = x1;
    GXFIFO_F32 = y0;
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
    GXFIFO_F32 = EXT_LIT(float_1_8041bde8, 1.0f);
    GXFIFO_F32 = EXT_LIT(float_0_8041bde4, 0.0f);
}

void loadDraw(CameraId camera, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    GXColor white = {255, 255, 255, 255};
    GXColor gray = {160, 160, 160, 255};
    GXColor dark = {128, 128, 128, 128};
    f32 vp[6];
    f32 proj[7];
    CameraEntry saved;
    char buf[0x20];
    char buf2[0x100];
    Mtx m;
    Mtx s;
    f32 x;
    f32 y;
    f32 w;
    u64 sec;
    WinDt* win1;
    s32 i;
    s32 j;
    s32 k;

    if (wp->unk18 >= 0x20 && wp->unk18 < 0x28) {
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
    GXSetFog(GX_FOG_NONE, EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), (GXColor){255, 255, 255, 255});
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
    // FAKE: win1 reused as a cam alias before its win_dt web, so the win_dt web becomes a lifetime-split temp
    // (addi straight into win1's register, loop 1 iterates a copy) as in the target
    win1 = (WinDt*)cam;
    GXLoadPosMtxImm(((CameraEntry*)win1)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    loadDrawBg(0x35, EXT_LIT(float_neg304_8041bdec, -304.0f), EXT_LIT(float_240_8041bdf0, 240.0f), EXT_LIT(float_neg272_8041bdf8, -272.0f), EXT_LIT(float_neg240_8041bdf4, -240.0f));
    loadDrawBg(0x34, EXT_LIT(float_neg272_8041bdf8, -272.0f), EXT_LIT(float_240_8041bdf0, 240.0f), EXT_LIT(float_272_8041bdfc, 272.0f), EXT_LIT(float_neg240_8041bdf4, -240.0f));
    loadDrawBg(0x36, EXT_LIT(float_272_8041bdfc, 272.0f), EXT_LIT(float_240_8041bdf0, 240.0f), EXT_LIT(float_304_8041be00, 304.0f), EXT_LIT(float_neg240_8041bdf4, -240.0f));

    win1 = win_dt;
    for (i = 0; i < 9; i++) {
        GXColor col = gray;
        if (i == wp->slot) {
            col = white;
        }
        if (wp->unk18 == 0x33 && (i == wp->unk30 || i == wp->unk34)) {
            col = white;
        }
        if (i == 4 && (wp->flags & 0x10)) {
            col = dark;
        }
        if (i == 5 && (wp->flags & 0x20)) {
            col = dark;
        }
        switch (win1[i].unk10) {
            case 10:
            case 11:
            case 12:
            case 13: {
                col.a = 0xfe;
                winTexInit(*wp->file->data);
                winTexSet(0xe, (Vec){win1[i].x, win1[i].y, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, col);
                winTexInit(*wp->file->data);
                winTexSet(win1[i].unk10, (Vec){win1[i].x, EXT_LIT(float_60_8041be04, 60.0f) + win1[i].y, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, col);
                break;
            }
            default: {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(win1[i].unk10, win1[i].unk14, (Vec){win1[i].x, win1[i].y, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, col);
                break;
            }
        }
    }

    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, EXT_LIT(float_neg250_8041be08, -250.0f), EXT_LIT(float_200_8041be0c, 200.0f), EXT_LIT(float_500_8041be10, 500.0f), EXT_LIT(float_40_8041be14, 40.0f), EXT_LIT(float_20_8041be18, 20.0f));
    FontDrawStart();
    {
        const char* msg = wp->msg;
        FontDrawMessage(-FontGetMessageWidth(msg) / 2, 192, msg);
    }

    for (i = 0; i < 4; i++, win1++) {
        CardSaveSlot* save = &cardGetFilePtr()->saves[i];
        u16 flags = save->data.flags;
        s16 level = save->data.pouch.level;
        OSTime time = save->data.global.retraceLocalTime;
        u16 stars = save->data.pouch.mStarPowersObtained;
        s32 gsw0 = save->data.global.mGSW0;
        char* name = (char*)save->data.global.playerName;
        const char* str;
        s32 hours;
        s32 mins;

        FontDrawStart();
        FontDrawColor((GXColor){192, 192, 192, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){255, 255, 255, 255});
        }
        FontDrawScale(EXT_LIT(float_0p6_8041be1c, 0.6f));
        FontDrawEdge();
        sprintf(buf, "\203f\201[\203^%s", winZenkakuStr(i + 1));
        FontDrawString(win1->x - 100.0f, EXT_LIT(float_84_8041be24, 84.0f) + win1->y, buf);
        FontDrawEdgeOff();
        FontDrawScale(EXT_LIT(float_1_8041bde8, 1.0f));
        FontDrawColor((GXColor){32, 32, 32, 255});
        if (i == wp->slot) {
            FontDrawColor((GXColor){0, 0, 0, 255});
        }
        if (flags & 2) {
            str = "\202\261\202\355\202\352\202\304\202\242\202\334\202\267";
            FontDrawString(win1->x - FontGetMessageWidth(str) / EXT_LIT(float_2_8041be70, 2.0f), EXT_LIT(float_12_8041be2c, 12.0f) + win1->y, str);
        } else if (flags & 1) {
            str = "\203j\203\205\201[\203Q\201[\203\200";
            FontDrawString(win1->x - FontGetMessageWidth(str) / EXT_LIT(float_2_8041be70, 2.0f), EXT_LIT(float_12_8041be2c, 12.0f) + win1->y, str);
        } else if (gsw0 == 0) {
            str = "\202\315\202\266\202\337\202\251\202\347";
            FontDrawString(win1->x - FontGetMessageWidth(str) / EXT_LIT(float_2_8041be70, 2.0f), EXT_LIT(float_12_8041be2c, 12.0f) + win1->y, str);
            if (strlen(name) != 0) {
                FontDrawString(win1->x - FontGetMessageWidth(name) / 2, 70.0f + win1->y, name);
            }
        } else {
            FontGetMessageWidth("\x82\x57\x82\x57");
            sec = OSTicksToSeconds(time);
            if (sec > 359999) {
                hours = 99;
                mins = 59;
            } else {
                hours = (u64)sec / 3600;
                mins = ((u64)sec % 3600) / 60;
            }
            if (strlen(name) != 0) {
                FontDrawString(win1->x - FontGetMessageWidth(name) / 2, 70.0f + win1->y, name);
            }
            sprintf(buf2, "\203\214\203x\203\213\201@%s", winZenkakuStr(level));
            FontDrawString(win1->x - FontGetMessageWidth(buf2) / EXT_LIT(float_2_8041be70, 2.0f), EXT_LIT(float_30_8041be3c, 30.0f) + win1->y, buf2);
            strcpy(buf2, "\203v\203\214\203C\202\266\202\251\202\361\201@");
            strcat(buf2, winZenkakuStrPad(hours, 2));
            strcat(buf2, " ");
            strcat(buf2, winZenkakuStrPad(mins, 2));
            w = FontGetMessageWidth(buf2);
            FontDrawString(win1->x - w / EXT_LIT(float_2_8041be70, 2.0f), win1->y, buf2);
            strcpy(buf2, "\203v\203\214\203C\202\266\202\251\202\361\201@");
            strcat(buf2, winZenkakuStrPad(hours, 2));
            x = FontGetMessageWidth(buf2);
            PSMTXTrans(m, EXT_LIT(float_8_8041be44, 8.0f) + (x + (win1->x - w / EXT_LIT(float_2_8041be70, 2.0f))), EXT_LIT(float_1_8041bde8, 1.0f) + (win1->y - 22.0f), EXT_LIT(float_0_8041bde4, 0.0f));
            PSMTXScale(s, EXT_LIT(float_1p5_8041be4c, 1.5f), EXT_LIT(float_1p5_8041be4c, 1.5f), EXT_LIT(float_1p5_8041be4c, 1.5f));
            PSMTXConcat(m, s, m);
            iconDispGxCol(m, 0, 0x201, (GXColor){0, 0, 0, 255});
            // FAKE: y read through a byte offset so the loop-invariant &win1->y becomes a hoisted @ temp (target pop order)
            for (k = 0; k < 7; k++) {
                if (stars & (1 << (k + 1))) {
                    PSMTXTrans(m, (win1->x - 106.0f) + k * 32, *(f32*)((u8*)win1 + offsetof(WinDt, y)) - 52.0f, 0.0f);
                    iconDispGxCol(m, 0, (u16)stone_id[k], i == wp->slot ? white : gray);
                }
            }
        }
    }

    if (wp->flags & 1) {
        wp->alpha += 0.05f;
    } else {
        wp->alpha -= 0.05f;
    }
    if (wp->alpha < EXT_LIT(float_0_8041bde4, 0.0f)) {
        wp->alpha = 0.0f;
    }
    if (wp->alpha > EXT_LIT(float_1_8041bde8, 1.0f)) {
        wp->alpha = 1.0f;
    }
    nameMaskGX(wp->alpha);

    if (wp->unk18 >= 100 && wp->unk18 < 200) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x13, 0x1f, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_100_8041be20, 100.0f), EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 3; i++) {
            if (i == wp->unk24) {
                winTexInit(*wp->file->data);
                winTexSet(0x33, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), 50 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x15, 0x33, (Vec){-4.0f, 54 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x15, 0x33, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), 50 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
            }
        }
    }

    if (wp->unk18 >= 200 && wp->unk18 < 300) {
        winTexInit(*wp->file->data);
        winTexSet(0xf, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
        winTexInit_x2(*wp->file->data);
        winTexSet_x2(0x14, 0x1f, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), EXT_LIT(float_100_8041be20, 100.0f), EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
        for (i = 0; i < 2; i++) {
            if (i == wp->unk28) {
                winTexInit(*wp->file->data);
                winTexSet(0x33, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), 30 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){0, 0, 0, 64});
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x33, (Vec){-4.0f, 34 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
            } else {
                winTexInit_x2(*wp->file->data);
                winTexSet_x2(i + 0x18, 0x33, (Vec){EXT_LIT(float_0_8041bde4, 0.0f), 30 - i * 50, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){255, 255, 255, 255});
            }
        }
    }

    if (wp->unk18 == 0x33 && wp->unk34 < 4) {
        f32 angle[4][4] = {
            {0.0f, -90.0f, -180.0f, -135.0f},
            {90.0f, 0.0f, 135.0f, 180.0f},
            {0.0f, -45.0f, 0.0f, -90.0f},
            {45.0f, 0.0f, 90.0f, 0.0f},
        };
        f32 cx;
        f32 cy;
        if (!(f32)fmod(angle[wp->unk30][wp->unk34], 90.0)) {
            f32 c0 = wp->unk30 % 2;
            f32 r0 = wp->unk30 / 2;
            f32 r1 = wp->unk34 / 2;
            if (r0 == r1) {
                cx = 0.0f;
                cy = EXT_LIT(float_120_8041be5c, 120.0f) - 140.0f * r0;
            } else {
                cx = -150.0f + 300.0f * c0;
                cy = EXT_LIT(float_50_8041be6c, 50.0f);
            }
        } else {
            cx = 0.0f;
            cy = EXT_LIT(float_50_8041be6c, 50.0f);
        }
        cy -= EXT_LIT(float_40_8041be14, 40.0f);
        winTexInit(*wp->file->data);
        winTexSetRot(0x1d, (Vec){EXT_LIT(float_2_8041be70, 2.0f) + cx, cy - 2.0f, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){0, 0, 0, 128}, angle[wp->unk30][wp->unk34]);
        winTexSetRot(0x1d, (Vec){cx, cy, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, white, angle[wp->unk30][wp->unk34]);
    }

    if (!(wp->flags & 0x40)) {
        winTexInit(*wp->file->data);
        winTexSet(0, (Vec){EXT_LIT(float_2_8041be70, 2.0f) + wp->x, wp->y - 2.0f, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, (GXColor){0, 0, 0, 128});
        winTexSet(0, (Vec){wp->x, wp->y, EXT_LIT(float_0_8041bde4, 0.0f)}, (Vec){EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f), EXT_LIT(float_1_8041bde8, 1.0f)}, white);
    }

    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
    *camGetCurPtr() = saved;
}

s32 stone_id[7] = {0x197, 0x198, 0x19A, 0x19C, 0x19D, 0x199, 0x19B};

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
#pragma force_active on
#pragma force_active on
#pragma defer_codegen off
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
    psndSetFlag(4);
    marioPartyEntry(gp->savePartyId[0]);
    marioPartyEntry(gp->savePartyId[1]);
    psndClearFlag(4);
    countDownSaveReStart();
    gp->flags |= 1;
    switch (swByteGet(4)) {
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
            swByteSet(0, 0);
            seqSetSeq(SEQ_MAP_CHANGE, "aaa_00", "prologue");
            break;
        case 24:
            gp->areaFadeInType = 0x3d;
            gp->areaFadeOutType = 0x38;
            seqSetSeq(SEQ_MAP_CHANGE, "aji_10", 0);
            break;
    }
}
#pragma defer_codegen reset
#endif
