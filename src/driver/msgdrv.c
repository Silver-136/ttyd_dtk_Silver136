#include "driver/msgdrv.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "driver/npcdrv.h"
#include "driver/windowdrv.h"
#include "manager/dvdmgr.h"
#include "manager/fontmgr.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include <stdlib.h>
#include "icon_ids.h"
#include "libc_decls.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "battle/battle_unit.h"
#include "driver/mapdrv.h"
const char R_no_messages_JP[] = "<col ff0000ff>\203\201\203b\203Z\201[\203W\202\252\202\240\202\350\202\334\202\271\202\361</col>\n<key>";
#ifdef VERSION_G8ME01
const char R_no_messages_US[] = "<col ff0000ff>No messages.[English]<col>\012<key>";
const char R_no_messages_DE[] = "<col ff0000ff>No messages.[German]<col>\012<key>";
const char R_no_messages_FR[] = "<col ff0000ff>No messages.[French]<col>\012<key>";
const char R_no_messages_ES[] = "<col ff0000ff>No messages.[Spanish]<col>\012<key>";
const char R_no_messages_IT[] = "<col ff0000ff>No messages.[Italian]<col>\012<key>";
const char R_no_messages_DU[] = "<col ff0000ff>No messages.[Dutch]<col>\012<key>";
#else
const char R_no_messages_US[] = "<col ff0000ff>No messages.<col>\n<key>";
#endif

// target .bss/.sbss pad (uninitialised objects are emitted in reverse order, so this sits last)

#define WIN_STATE(w) (*(u16*)(w)->state)


// MsgWinData.callback holds a function pointer in an s32 (msgWindow_Entry takes the callback as s32)
#define MSG_CB(d) (*(void (**)(s32, smartEntry*))&(d)->callback)
// A message window's MsgWinData lives in its userdata block. A macro, not an inline function: as a function the object changes (2026-10-27).
#define WIN_DATA(w) ((MsgWinData*)(w)->userdata->address)
void msgWindow_Disp(CameraId camId, void* param);
void msgWindow_Main(WindowEntry* window);
void msgWindow_Clear_Main(WindowEntry* window);
void msgWindow_Delete(WindowEntry* window);
void selectWindow_Main(WindowEntry* window);

s32 msgMain(smartEntry* smart);
void selectWindow_Disp(CameraId camId, void* param);


typedef struct MsgLookupEntry {
    u32 startaddr; //0x0
    u32 endaddr; //0x4
} MsgLookupEntry;

typedef struct MsgIcon {
    const char* name; //0x0
    s32 value; //0x4
} MsgIcon;

//.data
MsgIcon msgIcon[] = {
    {"PAD_A", ICON_BUTTON_A},
    {"PAD_B", ICON_BUTTON_B},
    {"STICK_LEFT", ICON_STICK_LEFT},
    {"STICK", ICON_STICK_NEUTRAL},
    {"AC_ON", ICON_AC_ON},
    {"BUTTON_L", ICON_TRIGGER_L},
    {"BUTTON_R", ICON_TRIGGER_R},
    {"HM", ICON_HP},
    {"PAD_X", ICON_BUTTON_X},
    {"PAD_X_ON", ICON_BUTTON_X_PRESSED},
    {"PAD_Y", ICON_BUTTON_Y},
    {"PAD_Y_ON", ICON_BUTTON_Y_PRESSED},
    {"PAD_Z_OFF", ICON_TRIGGER_Z},
    {"PAD_ST_OFF", ICON_BUTTON_START},
    {"black_key", ICON_ITEM_BLACK_KEY},
    {"ANM_PAD_A", ICON_BUTTON_A_ANIM},
    {"ANM_STICK_RIGHT", ICON_STICK_RIGHT_ANIM},
    {"ANM_PAD_START", ICON_BUTTON_START_ANIM}
};

s32 msgWindowOpen_SE_Data[] = {
    0x16, 0x17, 0x1A, 0x1B, 0x20, 0x21, 0x1C, 0x1D,
    0x20, 0x21, 0x1C, 0x1D, 0x18, 0x19, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x1E, 0x1F, -1, -1
};

//.sbss
MessageWork msgWork;
BOOL msgSEflag; //msgSEInit and related functions are unused
void* msgSortDvdData;
u16 g5_active_s772;

//.sdata
MessageWork* msgw = &msgWork;
#ifdef VERSION_G8ME01
const char* ErrMessage[7] = {R_no_messages_JP, R_no_messages_US, R_no_messages_DE, R_no_messages_FR,
                             R_no_messages_ES, R_no_messages_IT, R_no_messages_DU};
#else
const char* ErrMessage[2] = {R_no_messages_JP, R_no_messages_US};
#endif
const char* paperPoseTbl[6] = {
    "PM_Y_1B", "PM_Y_2B", "PM_F_1B",
    "PM_F_3B", "PM_C_1B", "PM_C_2B"
};
#ifdef VERSION_G8ME01
const char* language_dir[7] = {"msg/JP", "msg/US", "msg/DE", "msg/FR", "msg/SP", "msg/IT", "msg/US"};
#else
const char* language_dir[2] = {"msg", "emsg"};
#endif

//local prototypes
int msg_compare(const void* a, const void* b);
void msgDispKeyWait_render(s32 wXluStage);

void msgInit(void) {
    msgw->poseId = animPoseEntry("hoshi_2", 2);
    animPoseSetAnim(msgw->poseId, "HOS_C_1", TRUE);
    animPoseSetMaterialFlagOn(msgw->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    animPoseSetMaterialFlagOff(msgw->poseId, MAP_OBJ_FLAG_COLOR);
    animPoseSetGXFunc(msgw->poseId, msgDispKeyWait_render, 0);
    msgw->entries[0].address = NULL;
    msgw->entries[0].lookup = NULL;
    msgw->entries[1].address = NULL;
    msgw->entries[1].lookup = NULL;
    msgLoad("global", 1);
}

void msgLoad(const char* filename, s32 index) {
    MessageEntry* entry;
    const char* str;
    MsgLookupEntry* lookup;
    const char* msgptr;
    int i;
#ifdef VERSION_G8ME01
    s32 msgcount;
    DVDEntry* dvd;
    s32 max;
    s32 len;
#else
    DVDEntry* dvd;
    s32 msgcount;
#endif
    u32 ofs;
    char filepath[64];
    char dvdpath[128];

    entry = &msgw->entries[index];
#ifdef VERSION_G8ME01
    // US: the global message buffer is allocated once, sized for the largest message file of all languages
    if (!entry->address && index == 1) {
        max = 0;
        for (i = 0; i <= 6; i++) {
            sprintf(filepath, "./%s/%s.txt", language_dir[i], filename);
            sprintf(dvdpath, "%s/%s", getMarioStDvdRoot(), filepath);
            dvd = DVDMgrOpen(dvdpath, 2, 0);
            if (dvd) {
                len = DVDMgrGetLength(dvd);
                DVDMgrClose(dvd);
                if (max < len) {
                    max = len;
                }
            }
        }
        entry->address = __memAlloc(HEAP_DEFAULT, OSRoundUp32B(max));
    }
    entry->length = 0;
    sprintf(filepath, "./%s/%s.txt", language_dir[gp->language], filename);
    sprintf(dvdpath, "%s/%s", getMarioStDvdRoot(), filepath);
    dvd = DVDMgrOpen(dvdpath, 2, 0);
    if (dvd) {
        entry->length = DVDMgrGetLength(dvd);
        switch (index) {
            case 0:
                entry->address = _mapAlloc(OSRoundUp32B(entry->length));
                break;
            case 1:
                break;
        }
        DVDMgrRead(dvd, entry->address, OSRoundUp32B(entry->length), 0);
        DVDMgrClose(dvd);
    }
#else
    sprintf(filepath, "./%s/%s.txt", language_dir[gp->language], filename);

    if (entry->address) {
        if (index == 1) {
            __memFree(HEAP_DEFAULT, entry->address);
        }
        entry->address = NULL;
    }
    entry->length = 0;

    if (!entry->address) {
        sprintf(dvdpath, "%s/%s", getMarioStDvdRoot(), filepath);
        dvd = DVDMgrOpen(dvdpath, 2, 0);
        if (dvd) {
            entry->length = DVDMgrGetLength(dvd);
            switch (index) {
                case 0:
                    entry->address = _mapAlloc(OSRoundUp32B(entry->length));
                    break;
                case 1:
                    entry->address = __memAlloc(HEAP_DEFAULT, OSRoundUp32B(entry->length));
                    break;
            }
            DVDMgrRead(dvd, entry->address, OSRoundUp32B(entry->length), 0);
            DVDMgrClose(dvd);
        }
    }
#endif

    if (!entry->address) {
        entry->length = 0;
        entry->count = 0;
    } else {
        msgptr = entry->address;
        msgcount = 0;
        while (*(s8*)msgptr) {
            msgptr += strlen(msgptr) + 1;
            msgcount++;
        }
        entry->count = msgcount / 2; //tag, message
    }

    if (entry->lookup) {
        smartFree(entry->lookup);
    }
    if (!entry->count) {
        entry->lookup = NULL;
    } else {
        ofs = 0;
        entry->lookup = smartAlloc(entry->count * sizeof(MsgLookupEntry), 0);
        str = entry->address;
        lookup = entry->lookup->address;
        for (i = 0; i < entry->count; i++, lookup++) {
            lookup->startaddr = ofs;
            ofs += strlen(str) + 1;
            str += strlen(str) + 1;
            lookup->endaddr = ofs;
            ofs += strlen(str) + 1;
            str += strlen(str) + 1;
        }
        msgSortDvdData = entry->address;
        qsort(entry->lookup->address, entry->count, sizeof(MsgLookupEntry), msg_compare);
    }
}

static inline void msgAddPage(smartEntry* smart, s32 y) {
    MsgWinData* d = smart->address;
    if (!(d->flags & MSG_WIN_FLAG_NO_PAGE_LOG) && d->pageCount < 110) {
        if (d->pageCount == 0 || d->pageY[d->pageCount - 1] != y) {
            d->pageY[d->pageCount++] = y;
        }
    }
}

static inline void msgCheckSkip(smartEntry* smart, MsgWinData* data) {
    s32 btn, trgv;
    if (!(data->flags & MSG_WIN_FLAG_NO_SKIP)) {
        btn = 0x100;
        if (((MsgWinData*)smart->address)->flags & MSG_WIN_FLAG_KEY_X) {
            btn = 0x500;
        }
        trgv = keyGetButtonTrg(0);
        if (trgv & (btn | 0x200)) {
            data->flags |= MSG_WIN_FLAG_FAST;
        }
    }
}

static inline void msgVoiceSE(smartEntry* smart) {
    MsgWinData* d = smart->address;
    if (d->window->type != 1 && !msgSEflag) {
        psndSFXOnVol(SFX_NAME_WORD(PSND_SFX_MESSAGE1_1 | PSND_SFX_BYPASS_MUTE), d->talkVolume);
        msgSEflag = 1;
    }
}

s32 msgMain(smartEntry* smart) {
    MsgWinData* data = smart->address;
    MsgChar* entry;
    s32 i;
    s32 ms;
    NpcEntry* npc;

    msgSEflag = 0;
    if (data->flags & MSG_WIN_FLAG_START) {
        if (MSG_CB(data)) {
            MSG_CB(data)(0, smart);
        }
        data->flags &= ~MSG_WIN_FLAG_START;
        msgAddPage(smart, 0);
    }
    if (data->scrollTarget > data->scrollPos) {
        data->scrollPos += 0.36f * OSTicksToMilliseconds(gp->renderTime - data->lastUpdateTime);
        msgCheckSkip(smart, data);
        if (!(data->flags & MSG_WIN_FLAG_SMALL) && (data->flags & MSG_WIN_FLAG_FAST)) {
            data->scrollPos += 0.36f * OSTicksToMilliseconds(gp->renderTime - data->lastUpdateTime);
        }
        if (data->scrollTarget < data->scrollPos) {
            data->scrollPos = data->scrollTarget;
        }
        data->lastUpdateTime = gp->renderTime;
        return 3;
    }
    switch (data->pageState) {
    case 1:
        return 2;
    case 2:
        data->pageAdvanced = 1;
        data->curPage++;
        data->pageState = 0;
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_MESSAGE_WAIT1 | PSND_SFX_BYPASS_MUTE));
        data->playTime += 100;
        if (MSG_CB(data)) {
            MSG_CB(data)(3, smart);
        }
        break;
    default:
        ms = OSTicksToMilliseconds(gp->renderTime - data->lastUpdateTime);
        msgCheckSkip(smart, data);
        if (!(data->flags & MSG_WIN_FLAG_SMALL) && (data->flags & MSG_WIN_FLAG_FAST)) {
            ms *= 100;
        }
        data->playTime += ms;
        break;
    }
    data->lastUpdateTime = gp->renderTime;
    entry = &data->entries[data->nextEntry];
    for (i = data->nextEntry; i < VERSION_PICK(0x500, 0xA00); i++, entry++) {
        if (entry->time > data->playTime) {
            break;
        }
        switch (entry->glyph) {
        case 0xFFFF:
            if (MSG_CB(data)) {
                MSG_CB(data)(1, smart);
            }
            data->nextEntry = i;
            return 0;
        case 0xFFFE:
            if (entry->x == data->curPage) {
                data->pageState = 1;
                data->playTime = entry->time;
                data->flags &= ~MSG_WIN_FLAG_FAST;
                if (MSG_CB(data)) {
                    MSG_CB(data)(2, smart);
                }
            }
            break;
        case 0xFFFD:
            data->scrollTarget -= entry->y;
            data->playTime = entry->time;
            msgAddPage(smart, data->scrollTarget);
            break;
        case 0xFFFC:
            data->nextEntry = i + 1;
            data->playTime = entry->time;
            data->flags &= ~MSG_WIN_FLAG_FAST;
            return 1;
        case 0xFFFB:
            if (MSG_CB(data)) {
                MSG_CB(data)(4, smart);
            }
            break;
        case 0xFFFA:
            data->talkSfxGroup = entry->value;
            break;
        case 0xFFF9:
            data->talkVolume = entry->value;
            break;
        case 0xFFF5:
            if (entry->value) {
                data->flags |= MSG_WIN_FLAG_NO_SKIP;
                data->flags &= ~MSG_WIN_FLAG_FAST;
                data->playTime = entry->time;
            } else {
                data->flags &= ~MSG_WIN_FLAG_NO_SKIP;
            }
            break;
        case 0xFFF3:
            npc = entry->npc;
            if (!npc) {
                npc = npcNameToPtr(data->speakerName);
            }
            animPoseSetPaperAnimGroup(npc->poseId, "p_b_st", 1);
            animPosePaperPeraOn(npc->poseId);
            animPoseSetPaperAnim(npc->poseId, paperPoseTbl[entry->time], 1);
            break;
        case 0xFFF2:
            npc = entry->npc;
            if (data->pageState) {
                i--;
                break;
            }
            if (!npc) {
                npc = npcNameToPtr(data->speakerName);
            }
            animPoseSetPaperAnimGroup(npc->poseId, NULL, 1);
            break;
        case 0xFFF4:
        case 0xFFF8:
            break;
        default:
            if (MSG_CB(data)) {
                MSG_CB(data)(5, smart);
            }
            if (entry->value & MSG_CHAR_VOICE) {
                msgVoiceSE(smart);
            } else if (entry->value & MSG_CHAR_TALK) {
                if (((MsgWinData*)smart->address)->window->type != 3) {
                    ((MsgWinData*)smart->address)->talkTimer = 15;
                }
                msgVoiceSE(smart);
            }
            break;
        }
    }
    data->nextEntry = i;
    if (data->pageState) {
        return 2;
    }
    return 3;
}

#ifdef VERSION_G8ME01

// US: <scaleX> letters (MSG_CHAR_SCALE_X) are stretched on x only
// A macro, not an inline function: as a function the US constant order changes.
#define MSG_DRAW_SCALE(c)                                \
    if ((c)->value & MSG_CHAR_SCALE_X) {                 \
        FontDrawScaleVec((Vec){(c)->scale, 1.0f, 1.0f}); \
    }                                                    \
    else {                                               \
        FontDrawScale((c)->scale);                       \
    }
#endif

BOOL msgDisp(smartEntry* smart, u8 alpha, f32 x, f32 y);
BOOL msgDisp(smartEntry* smart, u8 alpha, f32 x, f32 y) {
    s32 al;
    s32 hide;
    s32 w;
    MsgWinData* data = smart->address;
    MsgChar* c = data->entries;
    s32 i;
    BOOL ret = FALSE;
    u8 a;
    f32 sx, sy, hx, hy, t;
    s32 page, py, d;
    GXColor col, col2, col4;
    Mtx m;
    Mtx m2;
    Vec pos;

    FontDrawStart_alpha(alpha);
    al = alpha;
    for (i = 0; i < data->nextEntry; i++, c++) {
        switch (c->glyph) {
            case 0xFFFF:
                return FALSE;
            case 0xFFFE:
                if (data->curPage == c->x) {
                    ret = TRUE;
                }
                break;
            case 0xFFF7:
                FontDrawColor(c->color);
                break;
            case 0xFFF6:
                if (c->value) {
                    FontDrawNoise();
                } else {
                    FontDrawNoiseOff();
                }
                break;
            case 0xFFF4:
                col = FontGetDrawColor();
                PSMTXIdentity(m);
                if (c->rot >= 90.0f && c->rot <= 270.0f) {
                    PSMTXScaleApply(m, m, 1.0f, 1.0f, -1.0f);
                }
                PSMTXTransApply(m, m, x + c->x, c->y + (y + data->scrollPos) - 40.0f, 0.0f);
                animPoseMain((s32)c->value);
                animPoseDrawMtx((s32)c->value, m, 2, c->rot, c->scale);
                FontDrawStart_alpha(alpha);
                FontDrawColor(col);
                break;
            case 0xFFF5:
            case 0xFFF9:
            case 0xFFFA:
            case 0xFFFB:
            case 0xFFFC:
            case 0xFFFD:
                break;
            default:
                hide = 0;
                a = 0xFF;
                sx = x + c->x;
                sy = c->y + (y + data->scrollPos);
                if (data->flags & MSG_WIN_FLAG_REVIEW) {
                    page = data->page;
                    py = data->pageY[page];
                    if (data->scrollPos == py) {
                        if (c->y + data->pageY[page + 1] <= 0) {
                            break;
                        }
                        hide = 1;
                    } else if (data->scrollPos < py) {
                        if (c->y + py > 0) {
                            hide = 1;
                        } else {
                            d = py - data->scrollPos;
                            if (d > 100) {
                                d = 100;
                            }
                            a = (100 - d) * 255 / 100;
                            if (page != data->pageCount - 1) {
                                if (c->y + data->pageY[page + 1] <= 0) {
                                    break;
                                }
                                hide = 1;
                            } else {
                                col2 = FontGetDrawColor();
                                col2.a = col2.a * a / 255;
                                GXSetTevColor(GX_TEVREG0, col2);
                            }
                        }
                    } else {
                        if (c->y + data->pageY[page + 1] > 0) {
                            hide = 1;
                        } else {
                            d = data->scrollPos - py;
                            if (d > 100) {
                                d = 100;
                            }
                            a = d * 255 / 100;
                            if (page != data->pageCount - 2) {
                                if (c->y + data->pageY[page + 2] <= 0) {
                                    break;
                                }
                                hide = 1;
                            } else {
                                col2 = FontGetDrawColor();
                                col2.a = col2.a * a / 255;
                                GXSetTevColor(GX_TEVREG0, col2);
                            }
                        }
                    }
                }
                if (c->glyph == 0xFFF8) {
                    col4 = FontGetDrawColor();
#ifdef VERSION_G8ME01
                    if ((((data->scrollPos + c->y) - 40) + c->iconY) > 0) {
                        break;
                    }
#endif
                    pos.x = 20.0f + (x + c->x);
                    pos.y = c->iconY + (c->y + (y + data->scrollPos) - 40.0f);
                    pos.z = 0.0f;
                    iconDispGxAlpha(&pos, 0x10, (u16)c->value, a * al / 255, c->scale);
                    FontDrawStart_alpha(alpha);
                    FontDrawColor(col4);
                    break;
                }
                if ((f32)(c->y + data->scrollPos) >= 32.0f * c->scale - 10.0f) {
                    break;
                }
                if (hide) {
                    GXColor gray = {0x50, 0x50, 0x50, 0xFF};
                    switch (data->window->type) {
                        case 3:
                        case 8:
                        case 9: {
                            GXColor gray2 = {0x6E, 0x6E, 0x6E, 0xFF};
                            gray = gray2;
                            break;
                        }
                    }
                    gray.a = a;
                    GXSetTevColor(GX_TEVREG0, gray);
#ifdef VERSION_G8ME01
                    MSG_DRAW_SCALE(c);
#else
                    FontDrawScale(c->scale);
#endif
                    FontDrawCode(sx, sy, c->glyph);
                    FontDrawColor_();
                } else if (c->value & MSG_CHAR_DYNAMIC) {
                    w = (u16)(kanjiGetWidth(c->glyph) / 2);
#ifdef VERSION_G8ME01
                    if (c->value & MSG_CHAR_SHAKE) {
                        sx += irand(10000) % 3;
                        sy += irand(10000) % 3;
                    }
                    hx = sx;
                    hy = sy;
#else
                    hx = sx;
                    hy = sy;
                    if (c->value & MSG_CHAR_SHAKE) {
                        hx = sx + irand(10000) % 3;
                        hy = sy + irand(10000) % 3;
                    }
#endif
                    PSMTXTrans(m2, -w, 12.0f, 0.0f);
                    PSMTXScaleApply(m2, m2, c->rot, c->rot, c->rot);
                    PSMTXTransApply(m2, m2, (u32)w, (-12.0f), 0.0f);
                    PSMTXScaleApply(m2, m2, c->scale, c->scale, c->scale);
                    PSMTXTransApply(m2, m2, hx, hy, 0.0f);
                    FontDrawCodeMtx(m2, c->glyph);
                    c->rot = 1.0f + 0.5f * (c->rot - 1.0f);
                } else if (c->value & MSG_CHAR_SHAKE) {
#ifdef VERSION_G8ME01
                    sx += irand(10000) % 3;
                    sy += irand(10000) % 3;
                    MSG_DRAW_SCALE(c);
                    FontDrawCode(sx, sy, c->glyph);
#else
                    hy = sx + irand(10000) % 3;
                    hx = sy + irand(10000) % 3;
                    FontDrawScale(c->scale);
                    FontDrawCode(hy, hx, c->glyph);
#endif
                } else if (c->value & MSG_CHAR_WAVE) {
#ifdef VERSION_G8ME01
                    t = 0.01f * OSTicksToMilliseconds(gp->renderTime - data->openTime) - 0.02f * sx;
                    sx += 3.2f * (f32)cos(t);
                    sy -= 3.2f * (f32)sin(t);
                    MSG_DRAW_SCALE(c);
                    FontDrawCode(sx, sy, c->glyph);
#else
                    t = 0.01f * OSTicksToMilliseconds(gp->renderTime - data->openTime) - 0.02f * sx;
                    hx = cos(t);
                    hy = sin(t);
                    FontDrawScale(c->scale);
                    FontDrawCode(sx + 3.2f * hx, sy - 3.2f * hy, c->glyph);
#endif
                } else if (c->value & MSG_CHAR_SEL_ROW(data->sel)) {
                    f32 hx2;
                    t = 0.01f * OSTicksToMilliseconds(gp->renderTime - data->openTime) - 0.02f * sx;
#ifdef VERSION_G8ME01
                    hx2 = sx + 3.2f * (f32)cos(t);
                    sy -= 3.2f * (f32)sin(t);
                    hy = sy;
                    MSG_DRAW_SCALE(c);
#else
                    hx2 = sx + 3.2f * (f32)cos(t);
                    hy = sy - 3.2f * (f32)sin(t);
                    FontDrawScale(c->scale);
#endif
                    {
                        GXColor shadow = {0x00, 0x00, 0x00, 0x50};
                        FontDrawColor(shadow);
                    }
                    FontDrawCode(hx2, hy, c->glyph);
                    FontDrawEdge();
                    FontDrawRainbowColor();
                    {
                        GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
                        FontDrawColor(white);
                    }
                    FontDrawCode(hx2 - 4.0f, 4.0f + hy, c->glyph);
                    FontDrawEdgeOff();
                    FontDrawRainbowColorOff();
                    FontDrawColorIDX(0);
                } else {
#ifdef VERSION_G8ME01
                    MSG_DRAW_SCALE(c);
#else
                    FontDrawScale(c->scale);
#endif
                    FontDrawCode(sx, sy, c->glyph);
                }
                break;
        }
    }
    return ret;
}

void msgDispKeyWait_render(s32 wXluStage) {
    AnimationWork* wp = animGetPtr();

    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    wp->currDispMode = wXluStage;
}

#ifdef VERSION_G8ME01
u16 hankakuSearch(s32 c);
#else
u16 hankakuSearch(char c);
#endif

/* FAKE: opt_lifetimes off keeps total as one web, so the rubyW conversion int shares the loop-1 accumulator register
   as in the target. The webs the target does split are split by hand: glyphD, wwD, wwR and kR (declared first so they take
   the numbers of the split temps). See FAKES.md */
#pragma opt_lifetimes off
void msgAnalize(smartEntry* smart, char* msg) {
    u16 glyphD;
    s32 wwD;
    s32 wwR;
    s32 kR;
    s16 topY;
    s16 savedPage;
    s16 savedScroll;

    MsgWinData* data;
    /* FAKE: nn, fp, fx, ys and yx are one-element arrays (IRO scalarizes them into @ temps). nn is the ruby index copy
       of i. fp/fx both hold flags & MSG_CHAR_DYNAMIC and ys/yx both hold y for the second ruby loop; a dead guard at the loop top
       redefines fp and ys (never runs), and fx/yx are re-copied from fp/ys before time is cleared. Same values; the fx copy
       merges only in the second colouring round and the y pair fixes the preheader order, which gives the target
       register order (FAKES.md) */
    s32 nn[1];
    /* FAKE: zz, z3 and z4 are write-only scratch; the kR init reads them back as 0 + 1 + 2 - 3 = 0. Their three constant
       loads only exist before register allocation and fill scheduler slots, so the loop-2 counter init lands after the
       rubyW truncation (FAKES.md) */
    s32 zz[1];
    s32 z3[1];
    s32 z4[1];
    u32 fp[1];
    u32 fx[1];
    s32 ys[1];
    s32 yx[1];
    WindowEntry* window;
    MsgChar* entry;
    s32 check;
    u16 glyph;
    s16 x;
    s16 y; // s16 in the original: extsh before every int->float use (0x20b0, 0x2b98) and after <pos> (0x23b0)
    s16 rubyX;
    s32 time;
    u32 flags;
    char tag[0x80];
    char args[0x80];
    char name[0x20];
    char anim1[0x20];
    char anim2[0x20];
    char paper[0x20];
    char paper2[0x20];
    s32 step;
#ifdef VERSION_G8ME01
    Vec scale; // US: x and y scale apart (<scaleX>)
    u32 maxW; // US: widest line so far, becomes the window width
#else
    f32 scale;
#endif
    f32 fv;
    s32 posX;
    int posY;
    s32 waitTime;
    s32 iconId;
    s32 iconOffX;
    s32 iconY;
    MsgWinData* data2;
    s32 iconAdvance;
    f32 iconScale;
    s32 animX;
    int animY;
    s32 animFlip;
    f32 animScale;
    s32 paperTime;
    s16 winX;
    s16 winY;
    s16 winHeight;
    s16 winWidth;
    s16 sel0;
    s16 sel1;
    u16 iw;
    u16 ih;
    f32 maxScale;
    char* next;
    BOOL isHM;
    s32 i;
    s32 j;
    s32 k;
    s32 half;
    s32 total;
    s32 ww;

    data = smart->address;
    window = data->window;
    entry = &data->entries[data->charCount];
    flags = 0;
    maxScale = 0.0f;
#ifdef VERSION_G8ME01
    {
        Vec one = {1.0f, 1.0f, 1.0f};
        scale = one;
    }
    maxW = 70;
#else
    scale = 1.0f;
#endif
    if (window->type < 0) {
        window->type = 0;
    }
    time = data->parseTime;
    topY = 0;
    savedPage = data->parsePage;
    x = data->parseX;
    savedScroll = data->parseScroll;
    y = data->parseY;
    step = data->parseSpeed;
    switch (window->type) {
    case 3:
    case 8:
        switch (window->type) {
        case 3: {
            static const GXColor c = {0xDC, 0xC8, 0xDC, 0xFF};
            entry->value = *(( u32*)(&c) );
            break;
        }
        case 8: {
            static const GXColor c = {0x00, 0xD9, 0x14, 0xFF};
            entry->value = *(( u32*)(&c) );
            break;
        }
        }
        entry->glyph = 0xFFF7;
        entry->time = time;
        entry++;
        data->charCount++;
        break;
    default:
        break;
    }

    while (TRUE) {
        check = *(( u8*)msg) ;
#ifdef VERSION_G8ME01
        if (_ismbblead((s8)check)) {
#else
        if (_ismbblead((u8)check)) {
#endif
            glyph = kanjiSearch(*(( u16*)msg) );
#ifdef VERSION_G8ME01
            ww = scale.x * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) );
            entry->glyph = glyph;
            entry->x = x;
            entry->y = y - (s16)(10.0f * scale.y);
            entry->scale = scale.x;
#else
            ww = scale * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) );
            entry->glyph = glyph;
            entry->x = x;
            entry->y = y - (s16)(10.0f * scale);
            entry->scale = scale;
#endif
            entry->value = flags;
            if (( *( (u16*)msg )) == 0x8145) {
                entry->value |= MSG_CHAR_VOICE;
            } else if ((( *( (u16*)msg )) != 0x8140 ) && (( *( (u16*)msg )) != 0x8151) ) {
                entry->value |= MSG_CHAR_TALK;
            }
            if (( flags & MSG_CHAR_DYNAMIC) != 0 ) {
                entry->rot = fv;
            }
#ifdef VERSION_G8ME01
            if (maxScale < scale.y) {
                maxScale = scale.y;
            }
#else
            if (maxScale < scale) {
                maxScale = scale;
            }
#endif
            entry->time = time;
            x += ww;
            msg += 2;
            time += step;
            entry++;
            data->charCount++;
            continue;
        }
        switch ((s8)check) {
        case 0:
            entry->glyph = 0xFFFF;
            entry->time = time;
#ifdef VERSION_G8ME01
            if (data->selWidth == 0) {
                if (maxW < x) {
                    maxW = x;
                }
                data->selWidth = maxW + 80;
            }
#endif
            data->parseTime = time;
            data->parseX = x;
            data->parseY = y;
            data->parsePage = savedPage;
            data->parseScroll = savedScroll;
            return;
        case '\n':
            entry->glyph = 0xFFFB;
            entry->time = time;
#ifndef VERSION_G8ME01
            x = 0;
#endif
            if (0.0f == maxScale) {
                maxScale = 1.0f;
            }
#ifdef VERSION_G8ME01
            if (data->flags & MSG_WIN_FLAG_SMALL) {
                // US: tighter line spacing
                y -= 0.7f * (32.0f * maxScale);
                savedScroll -= 0.7f * (32.0f * maxScale);
            } else {
                y -= 32.0f * maxScale;
                savedScroll -= 32.0f * maxScale;
            }
#else
            y -= 32.0f * maxScale;
            savedScroll -= 32.0f * maxScale;
#endif
            msg++;
            entry++;
            time += step;
            data->charCount++;
            maxScale = 0.0f;
            if (data->flags & MSG_WIN_FLAG_SELECT) {
                flags &= ~MSG_CHAR_SEL_ROW_MASK;
                flags |= MSG_CHAR_SEL_ROW(data->numSel);
                data->numSel++;
#ifdef VERSION_G8ME01
                if (data->selWidth == 0) {
                    if (maxW < x) {
                        maxW = x;
                    }
                }
#endif
            }
#ifdef VERSION_G8ME01
            x = 0;
#endif
            continue;
        case '<':
            next = msgGetCommand(msg + 1, tag, args);
            msg = next;
            /* FAKE: dead int guard (COLOURING W): msg is redefined in the arm while next is still live
               (the later newline test on next), so msg/next interfere and the target copy mr r21,r15 survives */
            {
                s32 gz = 0;
                if (gz != 0) {
                    msg++;
                }
            }
            if (( strcmp(tag, "k") == 0 ) || ( strcmp(tag, "key") == 0) ) {
                entry->glyph = 0xFFFE;
                entry->x = savedPage;
                entry->time = time;
                time += 100;
                entry++;
                data->charCount++;
                savedPage++;
            } else if (strcmp(tag, "keyxon") == 0) {
                data->flags |= MSG_WIN_FLAG_KEY_X;
            } else if (( strcmp(tag, "p") == 0 ) || ( strcmp(tag, "scrl_auto") == 0) ) {
                if (savedScroll != 0) {
                    entry->glyph = 0xFFFD;
                    entry->y = savedScroll - 3;
                    topY = y - 6;
                    y -= 3;
                    entry->time = time;
                    time += 0x32;
                    entry++;
                    savedScroll = 0;
                    data->charCount++;
                }
            } else if (( strcmp(tag, "o") == 0 ) || ( strcmp(tag, "once_stop") == 0) ) {
                entry->glyph = 0xFFFC;
                entry->time = time;
                time += 0x32;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "pos") == 0) {
                sscanf(args, "%d%d", &posX, &posY);
                x = posX;
                if (posY != 999) {
                    y = topY - posY;
#ifdef VERSION_G8ME01
                    savedScroll = -posY;
                }
#else
                }
                savedScroll = -posY;
#endif
#ifdef VERSION_G8ME01
            } else if (strcmp(tag, "scale") == 0) {
                sscanf(args, "%f", &scale.x);
                flags &= ~MSG_CHAR_SCALE_X;
                scale.y = scale.x;
            } else if (strcmp(tag, "scaleX") == 0) {
                sscanf(args, "%f", &scale.x);
                flags |= MSG_CHAR_SCALE_X;
            } else if (strcmp(tag, "/scale") == 0) {
                Vec one = {1.0f, 1.0f, 1.0f};
                flags &= ~MSG_CHAR_SCALE_X;
                scale = one;
#else
            } else if (strcmp(tag, "scale") == 0) {
                sscanf(args, "%f", &scale);
            } else if (strcmp(tag, "/scale") == 0) {
                scale = 1.0f;
#endif
            } else if (strcmp(tag, "wait") == 0) {
                sscanf(args, "%d", &waitTime);
                time += waitTime;
            } else if (strcmp(tag, "speed") == 0) {
                sscanf(args, "%d", &step);
            } else if (strcmp(tag, "/speed") == 0) {
                step = data->parseSpeed;
            } else if (strcmp(tag, "dynamic") == 0) {
                sscanf(args, "%f", &fv);
                flags |= MSG_CHAR_DYNAMIC;
            } else if (strcmp(tag, "/dynamic") == 0) {
                flags &= ~MSG_CHAR_DYNAMIC;
            } else if (strcmp(tag, "shake") == 0) {
                flags |= MSG_CHAR_SHAKE;
            } else if (strcmp(tag, "/shake") == 0) {
                flags &= ~MSG_CHAR_SHAKE;
            } else if (strcmp(tag, "wave") == 0) {
                flags |= MSG_CHAR_WAVE;
            } else if (strcmp(tag, "/wave") == 0) {
                flags &= ~MSG_CHAR_WAVE;
            } else if (strcmp(tag, "col") == 0) {
                sscanf(args, "%x", &entry->value);
                entry->glyph = 0xFFF7;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "/col") == 0) {
                entry->value = 0xFF;
                entry->glyph = 0xFFF7;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "noise") == 0) {
                entry->value = 1;
                entry->glyph = 0xFFF6;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "/noise") == 0) {
                entry->value = 0;
                entry->glyph = 0xFFF6;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "dkey") == 0) {
                entry->value = 1;
                entry->glyph = 0xFFF5;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "/dkey") == 0) {
                entry->value = 0;
                entry->glyph = 0xFFF5;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "plate") == 0) {
                window->type = 2;
                step = 0;
                data->parseSpeed = 0;
            } else if (strcmp(tag, "system") == 0) {
                static const GXColor c = {0xDC, 0xC8, 0xDC, 0xFF};
                window->type = 3;
                entry->value = *(( u32*)(&c) );
                entry->glyph = 0xFFF7;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "tec") == 0) {
                static const GXColor c = {0x00, 0xD9, 0x14, 0xFF};
                window->type = 8;
                entry->value = *(( u32*)(&c) );
                entry->glyph = 0xFFF7;
                entry->time = time;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "majo") == 0) {
                window->type = 9;
            } else if (strcmp(tag, "diary") == 0) {
                window->type = 7;
            } else if (strcmp(tag, "housou") == 0) {
                window->type = 10;
            } else if (strcmp(tag, "boss") == 0) {
                window->type = 6;
            } else if (strcmp(tag, "kanban") == 0) {
                window->type = 4;
                step = 0;
                data->parseSpeed = 0;
            } else if (strcmp(tag, "small") == 0) {
                window->type = 1;
                data->flags |= MSG_WIN_FLAG_SMALL;
            } else if (strcmp(tag, "clear") == 0) {
                window->type = 11;
            } else if (strcmp(tag, "wpos") == 0) {
                sscanf(args, "%hd%hd%hd%hd", &winX, &winY, &winHeight, &winWidth);
                window->flags |= WINDOW_FLAG_WPOS;
                window->x = winX;
                window->y = winY;
                window->height = winHeight;
                window->width = winWidth;
            } else if (strcmp(tag, "select") == 0) {
                window->type = 5;
                data->flags |= MSG_WIN_FLAG_SELECT;
                sscanf(args, "%hd%hd%hd%hd%hd", &sel0, &sel1, &data->selWidth, &data->selY);
                flags |= MSG_CHAR_SEL_ROW(0);
                data->defSel = sel0;
                data->cancel = sel1;
                data->numSel = 1;
                step = 0;
            } else if (strcmp(tag, "se") == 0) {
                sscanf(args, "%d", &entry->value);
                entry->glyph = 0xFFFA;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "vol") == 0) {
                sscanf(args, "%d", &entry->value);
                entry->glyph = 0xFFF9;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "nowinse") == 0) {
                window->flags |= WINDOW_FLAG_NOWINSE;
            } else if (strcmp(tag, "icon") == 0) {
                isHM = FALSE;
                if ((( (s8) args [0] ) >= '0') && (((s8) args [0]) <= '9') ) {
                    sscanf(args, "%d%f%d%d%d", &iconId, &iconScale, &iconOffX, &iconY, &iconAdvance);
                } else {
                    sscanf(args, "%s%f%d%d%d", name, &iconScale, &iconOffX, &iconY, &iconAdvance);
                    if (strcmp(name, "HM") == 0) {
                        isHM = TRUE;
                    }
                    iconId = msgIconStr2ID(name);
                    if (iconId < 0) {
                        iconId = 0;
                    }
                }
                if (!isHM) {
                    entry->value = iconId;
                    iconGetWidthHight(&iw, &ih, (u16)iconId);
                    entry->glyph = 0xFFF8;
                    entry->x = (x + (( 0.5f * iw ) * iconScale) ) + iconOffX;
#ifndef VERSION_G8ME01
                    x += 0x28;
#endif
                    entry->y = y - ( ih * iconScale) ;
                    entry->iconY = iconY;
                    entry->scale = iconScale;
                    entry->time = time;
                    entry++;
#ifdef VERSION_G8ME01
                    x += iconAdvance + 0x28;
#endif
                    time += step;
                    data->charCount++;
                } else {
#ifdef VERSION_G8ME01
                    if ((u32)gp->language == LANGUAGE_JP) {
                        glyph = kanjiSearch(0x94B5);
                    } else {
                        glyph = hankakuSearch(-48);
                    }
                    iw = scale.x * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) );
                    entry->glyph = glyph;
                    entry->x = x;
                    entry->y = y - (s16)(10.0f * scale.y);
                    entry->scale = scale.x;
#else
                    glyph = kanjiSearch(0x94B5);
                    iw = scale * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) );
                    entry->glyph = glyph;
                    entry->x = x;
                    entry->y = y - (s16)(10.0f * scale);
                    entry->scale = scale;
#endif
                    entry->value = flags;
                    entry->value |= MSG_CHAR_TALK;
                    if (flags & MSG_CHAR_DYNAMIC) {

                        entry->rot = fv;
                    }
#ifdef VERSION_G8ME01
                    if (maxScale < scale.y) {
                        maxScale = scale.y;
                    }
#else
                    if (maxScale < scale) {
                        maxScale = scale;
                    }
#endif
                    entry->time = time;
                    entry++;
                    x += iw;
                    time += step;
                    data->charCount++;
                }
            } else if (strcmp(tag, "anim") == 0) {
                sscanf(args, "%s%s%d%d%f%d", anim1, anim2, &animX, &animY, &animScale, &animFlip);
                entry->value = animPoseEntry(anim1, 2);
                animPoseSetAnim(entry->value, anim2, 1);
                animPoseSetMaterialFlagOn(entry->value, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
                animPoseSetMaterialFlagOff(entry->value, MAP_OBJ_FLAG_COLOR);
                /* FAKE: one-trip loop (j is dead here); it enables the backend array-to-register pass that removes the zz/z3/z4 stores */
                for (j = 0; j < 1; j++) {
                    animPoseSetGXFunc(msgw->poseId, msgDispKeyWait_render, 0);
                }
                entry->glyph = 0xFFF4;
                entry->x = animX;
                entry->y = topY - animY;
                entry->scale = animScale;
                entry->rot = (animFlip == 0) ? ( 0.0f ) : ( 180.0f) ;
                entry++;
                time += step;
                data->charCount++;
            } else if (strcmp(tag, "paper") == 0) {
                sscanf(args, "%s%d", paper, &paperTime);
                entry->npc = (strcmp(paper, "me") != 0) ? npcNameToPtr(paper) : NULL;
                entry->glyph = 0xFFF3;
                entry->time = paperTime;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "/paper") == 0) {
                sscanf(args, "%s", paper2);
                entry->npc = (strcmp(paper2, "me") != 0) ? npcNameToPtr(paper2) : NULL;
                entry->glyph = 0xFFF2;
                entry++;
                data->charCount++;
            } else if (strcmp(tag, "r") == 0) {
                data2 = data;
                if (data2->window->type != 1) {
                    rubyX = x;
                }
            } else if (((( s8*)tag)[0] == 'R' ) || ( (((s8*)tag)[0] == '/' ) && (( (u32)((s8*)tag)[0] ) == 0x2F72) )) {
                i = 1;
                if (((s8*)tag)[0] != 'R') { i = 2;
                }
                if (data->window->type == 1) {  // data2 is only set by the "r" tag branch
                    continue;
                }
                half = ((s32)strlen(tag + i)) / 2;
                total = 0;
                j = i;
                // FAKE: (u32) index casts here and in the second loop stop the IR optimizer from strength-reducing tag[j]/tag[i]; the backend
                // does it instead and its pointer-IV init in the preheader keeps the dead j/i steps alive, as in the target (FAKES.md)
                for (k = 0; k < half; k++) {
                    glyph = kanjiSearch(*(u16*)&tag[(u32)j]);
#ifdef VERSION_G8ME01
                    total += (s32)(0.55f * (scale.y * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) )));
#else
                    total += (s32)(0.55f * (scale * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) )));
#endif
                    j += 2;
                }
#ifdef VERSION_G8ME01
                total = (u16)total - ( 0.55f * (4.0f * scale.x));
                rubyX = ((rubyX + x) - 4.0f * scale.x - (u16)total) / 2;
#else
                total = (u16)total - ( 0.55f * (4.0f * scale));
                rubyX = ((rubyX + x) - 4.0f * scale - (u16)total) / 2;
#endif
                /* second loop open-coded: kR is its counter and glyph its glyph (split webs, numbered as in the target) */
                yx[0] = y;
                ys[0] = y;
                fp[0] = flags & MSG_CHAR_DYNAMIC;
                fx[0] = flags & MSG_CHAR_DYNAMIC;
                nn[0] = i;
                *(int*)zz = 0;
                *(int*)z3 = 1;
                *(int*)z4 = 2;
                for (kR = zz[0] + z3[0] + z4[0] - 3; kR < half; kR++) {
                    {
                        s32 gz = 0;
                        if (gz != 0) {
                            fp[0] = fx[0];
                            ys[0] = yx[0] + ys[0];
                        }
                    }
                    glyph = kanjiSearch(*(u16*)&tag[(u32)nn[0]]);
#ifdef VERSION_G8ME01
                    wwR = 0.55f * (scale.x * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) ));
#else
                    wwR = 0.55f * (scale * (( f32)(kanjiGetWidth((u16)(glyph + 0)) + 4) ));
#endif
                    entry->glyph = glyph;
                    entry->x = rubyX;
                    entry->y = ys[0];
#ifdef VERSION_G8ME01
                    entry->scale = 0.55f * scale.x;
#else
                    entry->scale = 0.55f * scale;
#endif
                    entry->value = flags;
                    if (fp[0]) {
                        entry->rot = fv;
                    }
                    fx[0] = fp[0];
                    yx[0] = ys[0];
                    entry->time = 0;
                    rubyX += wwR;
                    entry++;
                    data->charCount++;
                    nn[0] += 2;
                }
                continue;
            }
            if (( *( (s8*)next )) == '\n') {
                msg++;
            }
            continue;
        default:
            if (((( (u32)gp->language ) == 0 ) && (( (s8)check ) >= '0' )) && (( (s8)check ) <= '9') ) {
                glyphD = kanjiSearch((u16)(check + 0x821F));
            } else {
                glyphD = hankakuSearch(check);
            }
#ifdef VERSION_G8ME01
            wwD = scale.x * (( f32)(kanjiGetWidth((u16)(glyphD + 0)) + 2) );
            entry->glyph = glyphD;
            entry->x = x;
            entry->y = y - (s16)(10.0f * scale.y);
            entry->scale = scale.x;
#else
            wwD = scale * (( f32)(kanjiGetWidth((u16)(glyphD + 0)) + 2) );
            entry->glyph = glyphD;
            entry->x = x;
            entry->y = y - (s16)(10.0f * scale);
            entry->scale = scale;
#endif
            entry->value = flags;
            if (( *( (s8*)msg )) != ' ') {
                entry->value |= MSG_CHAR_TALK;
            }
            if (flags & MSG_CHAR_DYNAMIC) {
                entry->rot = fv;
            }
#ifdef VERSION_G8ME01
            if (maxScale < scale.y) {
                maxScale = scale.y;
            }
#else
            if (maxScale < scale) {
                maxScale = scale;
            }
#endif
            entry->time = time;
            x += wwD;
            msg++;
            time += step;
            entry++;
            data->charCount++;  // data2 is only set on one branch (otherwise this writes through NULL + 0x38, as in the game)
            continue;
        }
    }
}
#pragma opt_lifetimes reset

MessageWork* L_msgGetWork(void) {
    return &msgWork;
}

int msg_compare(const void* a, const void* b) {
    s32 result = strcmp((char*)msgSortDvdData + *(u32*)a, (char*)msgSortDvdData + *(u32*)b);
    if (result > 0) {
        return 1;
    }
    if (result < 0) {
        return -1;
    }
    return 0;
}

const char* msgSearch(const char* tag) {
    MessageEntry* entry;
    MsgLookupEntry* lookup;
    smartEntry* smart;
    s32 i, max, j, index;
    s32 result;

    for (i = 0; i < 2; i++) { //check both entries
        entry = &msgw->entries[i];
        smart = entry->lookup;
        if (smart) {
            lookup = smart->address;
            j = 0;
            max = entry->count;
            while (max >= j) {
                index = (j + max) / 2; //tag, message
                if (index >= entry->count) {
                    break;
                }
                result = strcmp(tag, (const char*)entry->address + lookup[index].startaddr);
                if (!result) {
                    return (const char*)entry->address + lookup[index].endaddr;
                }
                if (result < 0) {
                    max = index - 1;
                } else {
                    j = index + 1;
                }
            }
        }
    }
    return ErrMessage[gp->language];
}

BOOL _ismbblead(u32 c) {
#ifdef VERSION_G8ME01
    if ((u32)gp->language != LANGUAGE_JP) {
        return FALSE;
    }
#endif
    c = (u8)c;
    if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) {
        return TRUE;
    }
    return FALSE;
}

char* msgGetCommand(char* msg, char* cmd, char* arg) {
    s32 i;
    BOOL inArg = FALSE;
    char* m = msg;
    char* c = cmd;
    char* a = arg;

    for (i = 0; i < 0x80; i++, m++) {
        switch ((s8)*m) {
        case '>':
            if (!inArg) {
                *c = 0;
            } else {
                *a = 0;
            }
            return m + 1;
        case ' ':
            if (!inArg) {
                *c = 0;
                inArg = TRUE;
            } else {
                *a++ = *m;
            }
            break;
        default:
            if (!inArg) {
                *c++ = *m;
            } else {
                *a++ = *m;
            }
            break;
        }
    }
    return NULL;
}

s32 msgIconStr2ID(const char* str) {
    u32 i;

    for (i = 0; i < 18; i++) {
        if (!strcmp(msgIcon[i].name, str)) {
            return msgIcon[i].value;
        }
    }
    return -1;
}

s32 msgWindow_Entry(const char* msg, s32 callback, u16 priority) {
    WindowEntry* window;
    char* copy;
    MsgWinData* data;
    smartEntry* smart;
    s32 id;

    id = windowEntry(priority);
    window = windowGetPointer(id);
    window->main = msgWindow_Main;
    window->delete = msgWindow_Delete;
    WIN_STATE(window) = 5;
    window->type = -1;
    window->flags |= WINDOW_FLAG_BUSY;
    copy = __memAlloc(HEAP_DEFAULT, strlen(msg));
    strcpy(copy, msg);
    smart = smartAlloc(sizeof(MsgWinData), 1);
    data = smart->address;
    memset(data, 0, sizeof(MsgWinData));
    data->window = window;
    data->flags = 3;
    data->callback = callback;
    data->lastUpdateTime = gp->renderTime;
    data->openTime = gp->renderTime;
    data->talkSfxGroup = 0;
    data->talkVolume = 0x7F;
    data->parseTime = 0;
    data->parseX = 0;
    data->parseY = 0;
    data->parsePage = 0;
    data->parseScroll = 0;
    data->parseSpeed = 25;
    data->pageCount = 0;
    msgAnalize(smart, copy);
    if (window->type == 1) {
        data->flags |= MSG_WIN_FLAG_NO_PAGE_LOG;
    }
    __memFree(HEAP_DEFAULT, copy);
    window->userdata = smart;
    data = window->userdata->address;
    data->talkTimer = 0;
    data->talkPoseSet = 0;
    switch (window->type) {
    case 11:
        window->main = msgWindow_Clear_Main;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        if (!(window->flags & WINDOW_FLAG_WPOS)) {
            window->x = -280.0f;
            window->y = 225.0f;
            window->height = 560.0f;
            window->width = 176.0f;
        }
        break;
    case 5:
        window->main = selectWindow_Main;
        window->x = (s16)(-WIN_DATA(window)->selWidth / 2);
        window->y = WIN_DATA(window)->selY;
        window->height = WIN_DATA(window)->selWidth;
        window->width = (s16)(WIN_DATA(window)->numSel * 36 + 40);
        WIN_DATA(window)->sel = WIN_DATA(window)->defSel;
        data->cursorY = window->y - (data->sel * 31 + 58);
        break;
    case 1:
#ifdef VERSION_G8ME01
        window->priority = priority + 10;
#else
        window->priority = 10;
#endif
        window->height = VERSION_PICK(0.7f, 0.9f) * GXGetTexObjWidth(getWakuTexObj(3));
        window->width = VERSION_PICK(0.7f, 0.9f) * GXGetTexObjHeight(getWakuTexObj(3));
        break;
    }
    return id;
}

#pragma optimize_for_size on
void msgWindow_Delete(WindowEntry* window) {
    MsgWinData* data;
    s32 i;

    if (window->userdata) {
        data = window->userdata->address;
        for (i = 0; i < data->charCount; i++) {
        }
        smartFree(window->userdata);
        window->userdata = NULL;
    }
}

#pragma optimize_for_size reset

void msgWindow_Add(char* msg, s32 id) {
    WindowEntry* window = windowGetPointer(id);
    smartEntry* smart = window->userdata;

    ((MsgWinData*)smart->address)->lastUpdateTime = gp->renderTime;
    msgAnalize(smart, msg);
    WIN_STATE(window) = 1;
    window->flags |= WINDOW_FLAG_BUSY;
}

void msgWindow_Continue(s32 id) {
    WindowEntry* window = windowGetPointer(id);
    MsgWinData* data;

    WIN_STATE(window) = 1;
    window->flags |= WINDOW_FLAG_BUSY;
    data = window->userdata->address;
    data->lastUpdateTime = gp->renderTime;
}

void msgWindow_Repeat(s32 id) {
    WindowEntry* window = windowGetPointer(id);
    MsgWinData* data;

    WIN_STATE(window) = 1;
    window->flags |= WINDOW_FLAG_BUSY;
    data = window->userdata->address;
    data->lastUpdateTime = gp->renderTime;
    data->openTime = gp->renderTime;
    data->playTime = 0;
    data->scrollTarget = 0;
    data->scrollPos = 0;
    data->nextEntry = 0;
}

void msgWindow_ForceClose(const char* name) {
    WindowEntry* window;
    MsgWinData* data;
    s32 i;

    for (i = 0; i < 7; i++) {
        window = windowGetPointer(i);
        if (window->flags && WIN_STATE(window) && window->userdata &&
            ((MsgWinData*)window->userdata->address)->tag &&
            !strcmp(((MsgWinData*)window->userdata->address)->tag, name)) {
            WIN_STATE(window) = 7;
            data = window->userdata->address;
            if (data->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                BtlUnit_ChangeStayAnim(data->unitId);
            } else {
                npcSetStayPose(data->speakerName);
            }
        }
    }
}

static inline BOOL msgWinPrevPage(WindowEntry* window) {
    MsgWinData* d = WIN_DATA(window);
    d->page--;
    if (d->page < 0) {
        d->page = 0;
        return FALSE;
    }
    return TRUE;
}

static inline void msgWinNextPage(WindowEntry* window) {
    MsgWinData* d = WIN_DATA(window);
    s32 max;
    d->page++;
    max = d->pageCount - 1;
    if (d->page > max) {
        d->page = max;
    }
}

#pragma opt_common_subs off
void msgWindow_Main(WindowEntry* window) {
    smartEntry* smart = window->userdata;
    MsgWinData* data = smart->address;
    s32 se;
    s32 target;
    u32 trg;

    if (data->talkTimer) {
        if (!data->talkPoseSet) {
            if (data->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                BtlUnit_ChangeTalkAnim(data->unitId);
            } else {
                npcSetTalkPose(data->speakerName);
            }
            data->talkPoseSet = 1;
        }
        data->talkTimer--;
        if (data->talkTimer == 0) {
            MsgWinData* d2 = smart->address;
            if (d2->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                BtlUnit_ChangeStayAnim(d2->unitId);
            } else {
                npcSetStayPose(d2->speakerName);
            }
            data->talkPoseSet = 0;
        }
    }

    switch (WIN_STATE(window)) {
    case 5:
        if (!(window->flags & WINDOW_FLAG_NOWINSE)) {
            se = msgWindowOpen_SE_Data[window->type * 2];
            if (se >= 0) {
                psndSFXOn(SFX_NAME_WORD(se | PSND_SFX_BYPASS_MUTE));
            }
        }
        window->timestamp = gp->renderTime;
        WIN_STATE(window) = 6;
    case 6:
        window->flags |= WINDOW_FLAG_BUSY;
        window->alpha = OSTicksToMilliseconds(gp->renderTime - window->timestamp) * 255 / 200;
        if (window->alpha > 255) {
            window->alpha = 255;
            WIN_STATE(window) = 1;
            data->openTime = gp->renderTime;
            data->lastUpdateTime = gp->renderTime;
        }
        break;
    case 7:
        if (!(window->flags & WINDOW_FLAG_NOWINSE)) {
            se = msgWindowOpen_SE_Data[window->type * 2 + 1];
            if (se >= 0) {
                psndSFXOn(SFX_NAME_WORD(se | PSND_SFX_BYPASS_MUTE));
            }
        }
        window->timestamp = gp->renderTime;
        WIN_STATE(window) = 8;
    case 8:
        if (window->alpha == 0) {
            windowDelete(window);
            return;
        }
        window->alpha = 255 - OSTicksToMilliseconds(gp->renderTime - window->timestamp) * 255 / 200;
        if (window->alpha <= 0) {
            window->alpha = 0;
        }
        break;
    case 1:
        switch (msgMain(window->userdata)) {
        case 0:
            WIN_STATE(window) = 7;
            if (WIN_DATA(window)->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                BtlUnit_ChangeStayAnim(WIN_DATA(window)->unitId);
            } else {
                npcSetStayPose(WIN_DATA(window)->speakerName);
            }
            break;
        case 1:
            WIN_STATE(window) = 2;
            window->flags &= ~WINDOW_FLAG_BUSY;
            if (WIN_DATA(window)->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                BtlUnit_ChangeStayAnim(WIN_DATA(window)->unitId);
            } else {
                npcSetStayPose(WIN_DATA(window)->speakerName);
            }
            break;
        case 2:
            WIN_STATE(window) = 3;
            break;
        case 3:
            break;
        }
        break;
    case 3:
        target = 0x100;
        if (WIN_DATA(window)->flags & MSG_WIN_FLAG_KEY_X) {
            target = 0x500;
        }
        trg = keyGetButtonTrg(0);
        if (trg & (target | 0x200)) {
            WIN_STATE(window) = 1;
            data->pageState = 2;
            if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                if (!(data->flags & MSG_WIN_FLAG_NO_SKIP)) {
                    data->flags |= MSG_WIN_FLAG_FAST;
                }
            }
        }
#ifdef VERSION_G8ME01
        else // US: one key per frame
#endif
        if (keyGetButtonTrg(0) & PAD_TRIGGER_Z) {
            data->page = data->pageCount - 1;
            if (msgWinPrevPage(window) != 0U) {
                WIN_STATE(window) = 9;
                window->timestamp = gp->renderTime;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MESSAGE_Z1 | PSND_SFX_BYPASS_MUTE));
                data->flags |= MSG_WIN_FLAG_REVIEW;
            }
        }
        break;
    case 9:
        target = data->pageY[data->page];
        if (data->scrollPos >= target) {
            data->scrollPos = data->scrollPos - 0.6f * OSTicksToMilliseconds(gp->renderTime - window->timestamp);
            window->timestamp = gp->renderTime;
            if (data->scrollPos <= target) {
                data->scrollPos = target;
                WIN_STATE(window) = 11;
            }
        }
        break;
    case 10:
        target = data->pageY[data->page];
        if (data->scrollPos <= target) {
            data->scrollPos = data->scrollPos + 0.6f * OSTicksToMilliseconds(gp->renderTime - window->timestamp);
            window->timestamp = gp->renderTime;
            if (data->scrollPos >= target) {
                data->scrollPos = target;
                if (data->page == data->pageCount - 1) {
                    WIN_STATE(window) = 3;
                    data->flags &= ~MSG_WIN_FLAG_REVIEW;
                } else {
                    WIN_STATE(window) = 11;
                }
            }
        }
        break;
    case 11:
        target = 0x100;
        if (WIN_DATA(window)->flags & MSG_WIN_FLAG_KEY_X) {
            target = 0x500;
        }
        trg = keyGetButtonTrg(0);
        if (trg & target) {
            WIN_STATE(window) = 10;
            window->timestamp = gp->renderTime;
            msgWinNextPage(window);
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_MESSAGE_Z1 | PSND_SFX_BYPASS_MUTE));
        }
#ifdef VERSION_G8ME01
        else // US: one key per frame
#endif
        if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
            WIN_STATE(window) = 10;
            window->timestamp = gp->renderTime;
            data->page = data->pageCount - 1;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_MESSAGE_Z1 | PSND_SFX_BYPASS_MUTE));
        }
#ifdef VERSION_G8ME01
        else // US: one key per frame
#endif
        if (keyGetButtonTrg(0) & PAD_TRIGGER_Z) {
            WIN_STATE(window) = 9;
            window->timestamp = gp->renderTime;
            if (msgWinPrevPage(window) != 0U) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MESSAGE_Z1 | PSND_SFX_BYPASS_MUTE));
            }
        }
        break;
    }
    dispEntry(CAMERA_2D, 400.0f - window->priority, 0, msgWindow_Disp, window);
}

#pragma opt_common_subs reset
void msgWindow_Clear_Main(WindowEntry* window) {
    MsgWinData* data = WIN_DATA(window);

    switch (WIN_STATE(window)) {
    case 5:
        WIN_STATE(window) = 1;
        window->timestamp = gp->renderTime;
        data->openTime = gp->renderTime;
        data->lastUpdateTime = gp->renderTime;
    case 1:
    case 6:
        window->flags |= WINDOW_FLAG_BUSY;
        if (window->alpha < 255) {
            window->alpha = OSTicksToMilliseconds(gp->renderTime - window->timestamp) * 255 / 200;
            if (window->alpha > 255) {
                window->alpha = 255;
            }
        }
        if (WIN_STATE(window) != 2) {
            switch (msgMain(window->userdata)) {
            case 0:
                WIN_STATE(window) = 7;
                window->timestamp = gp->renderTime;
                if (WIN_DATA(window)->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                    BtlUnit_ChangeStayAnim(WIN_DATA(window)->unitId);
                } else {
                    npcSetStayPose(WIN_DATA(window)->speakerName);
                }
                break;
            case 1:
                WIN_STATE(window) = 2;
                window->flags &= ~WINDOW_FLAG_BUSY;
                if (WIN_DATA(window)->flags & MSG_WIN_FLAG_BATTLE_UNIT) {
                    BtlUnit_ChangeStayAnim(WIN_DATA(window)->unitId);
                } else {
                    npcSetStayPose(WIN_DATA(window)->speakerName);
                }
                break;
            case 2:
                WIN_STATE(window) = 3;
                break;
            case 3:
                break;
            }
        }
        break;
    case 7:
        if (window->alpha == 0) {
            windowDelete(window);
            return;
        }
        window->alpha = 255 - OSTicksToMilliseconds(gp->renderTime - window->timestamp) * 255 / 200;
        if (window->alpha <= 0) {
            window->alpha = 0;
        }
        break;
    }
    dispEntry(CAMERA_2D, 400.0f - window->priority, 0, msgWindow_Disp, window);
}

void msgWindow_Disp(CameraId camId, void* param) {
    CameraEntry* camera = camGetCurPtr();
    BOOL ret = FALSE;
    WindowEntry* window = param;
    Vec pos;
    f32 w;
    f32 h;
    f32 ex;
    f32 ey;
    s32 sy;
    static u16 active;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});

    switch (window->type) {
    case 0:
    case 1:
    case 6:
    case 8:
    case 9:
        if (window->tailMode != 0) {
            if (window->tailMode & 0x10) {
                pos = window->speakerPos;
            } else {
                getScreenPoint(&window->speakerPos, &pos);
            }
            switch (window->type) {
            case 0:
            case 6:
            case 8:
            case 9:
                ex = intplGetValue(EASING_LINEAR, pos.x, ( window->height / 2.0f ) + window->x, window->alpha, 255);
                ey = intplGetValue(EASING_LINEAR, pos.y, window->y - ( window->width / 2.0f ), window->alpha, 255);
                w = intplGetValue(EASING_LINEAR, 10.0f, window->height, window->alpha, 255);
                h = intplGetValue(EASING_LINEAR, 10.0f, window->width, window->alpha, 255);
                windowDispGX_Message(window->type, window->tailMode, window->alpha, ex - ( w / 2.0f ), ey + ( h / 2.0f ), w, h, pos.x, pos.y);
                if (window->alpha == 255) {
                    GXSetScissor(camera->scissor[0], 240.0f - (window->y - 36.0f), camera->scissor[2], 100);
                    ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
                }
                break;
            case 1:
                window->x = pos.x - ( window->height / 2.0f) ;
                if (window->tailMode & 8) {
                    window->y = pos.y - 80.0f;
                } else {
                    window->y = 60.0f + pos.y;
                }
                ex = intplGetValue(EASING_LINEAR, pos.x, ( window->height / 2.0f ) + window->x, window->alpha, 255);
                ey = intplGetValue(EASING_LINEAR, pos.y, window->y - ( window->width / 2.0f ), window->alpha, 255);
                w = intplGetValue(EASING_LINEAR, 10.0f, window->height, window->alpha, 255);
                h = intplGetValue(EASING_LINEAR, 10.0f, window->width, window->alpha, 255);
                windowDispGX_Message(window->type, window->tailMode, window->alpha, ex - ( w / 2.0f ), ey + ( h / 2.0f ), w, h, pos.x, pos.y);
                if (window->alpha == 255) {
                    sy = 240 - ((( s32)window->y ) - 10);
                    if (sy < 0) { sy = 0;
                    }
                    if (sy > 480) { sy = 480;
                    }
                    GXSetScissor(camera->scissor[0], sy, camera->scissor[2], 0x7E);
                    msgDisp(window->userdata, window->alpha, VERSION_PICK(20.0f, 28.0f) + window->x, window->y - VERSION_PICK(7.0f, 6.0f));
                }
                break;
            }
        } else {
            windowDispGX_Message(window->type, 0, window->alpha, window->x, window->y, window->height, window->width, 0.0f, 0.0f);
            GXSetScissor(camera->scissor[0], ( 240.0f - (window->y - 36.0f) ) - 8.0f, camera->scissor[2], 0x7E);
            ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
        }
        break;
    case 2:
    case 4:
    case 10:
        windowDispGX_Kanban(window->type, window->alpha, window->x, window->y, window->height, window->width);
        GXSetScissor(camera->scissor[0], ( 240.0f - (window->y - 36.0f) ) - 2.0f, camera->scissor[2], 0x68);
        ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
        break;
    case 7:
        windowDispGX_Kanban(window->type, window->alpha, window->x, window->y, window->height, window->width);
        GXSetScissor(camera->scissor[0], ( 240.0f - (window->y - 36.0f) ) - 8.0f, camera->scissor[2], 0x7E);
        ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
        break;
    case 3:
        windowDispGX_System(window->type, window->alpha, window->x, window->y, window->height, window->width);
        GXSetScissor(camera->scissor[0], ( 240.0f - (window->y - 36.0f) ) - 8.0f, camera->scissor[2], window->width - 50.0f);
        ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
        break;
    case 11:
        GXSetScissor(camera->scissor[0], ( 240.0f - (window->y - 36.0f) ) - 8.0f, camera->scissor[2], window->width - 50.0f);
        ret = msgDisp(window->userdata, window->alpha, 62.0f + window->x, window->y - 36.0f);
        break;
    case 5:
        break;
    }

    GXSetScissor(camera->scissor[0], camera->scissor[1], camera->scissor[2], camera->scissor[3]);
    if (ret) {
        if (active == 0) { active = 1;
        }
        animPoseMain(msgw->poseId);
        animPoseDraw(msgw->poseId, ( window->x + window->height ) - 52.0f, 31.0f + (window->y - window->width), 0.0f, 180.0f, 5.0f, 2);
    }
}

void selectWindow_Main(WindowEntry* window) {
    MsgWinData* data = window->userdata->address;
    s8 prev;

    msgMain(window->userdata);
    switch (WIN_STATE(window)) {
    case 5:
        window->alpha += 25;
        if (window->alpha > 255) {
            window->alpha = 255;
            WIN_STATE(window) = 1;
        }
        break;
    case 1:
        prev = data->sel;
        if ((keyGetButtonTrg(0) & PAD_BUTTON_UP) || (keyGetDirTrg(0) & DEMO_STICK_UP)) {
            data->sel--;
        } else if ((keyGetButtonRep(0) & PAD_BUTTON_UP) || (keyGetDirRep(0) & DEMO_STICK_UP)) {
            if (data->sel) {
                data->sel--;
            }
        }
        if ((keyGetButtonTrg(0) & PAD_BUTTON_DOWN) || (keyGetDirTrg(0) & DEMO_STICK_DOWN)) {
            data->sel++;
        } else if ((keyGetButtonRep(0) & PAD_BUTTON_DOWN) || (keyGetDirRep(0) & DEMO_STICK_DOWN)) {
            if (data->sel < data->numSel - 1) {
                data->sel++;
            }
        }
        if (data->sel < 0) {
            data->sel = data->numSel - 1;
        }
        if (data->sel >= data->numSel) {
            data->sel = 0;
        }
        if (prev != data->sel) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
            switch (data->cancel) {
            case -1:
                break;
            case -2:
                data->sel = -1;
                WIN_STATE(window) = 7;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                break;
            default:
                data->sel = data->cancel;
                WIN_STATE(window) = 7;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                break;
            }
        } else if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            WIN_STATE(window) = 7;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
        }
        break;
    case 7:
        if (window->alpha == 0) {
            WIN_STATE(window) = 4;
            window->flags &= ~WINDOW_FLAG_BUSY;
            return;
        }
        window->alpha -= 25;
        if (window->alpha <= 0) {
            window->alpha = 0;
        }
        break;
    case 4:
        return;
    }
    dispEntry(CAMERA_2D, 400.0f - window->priority, 0, selectWindow_Disp, window);
}

void selectWindow_Disp(CameraId camId, void* param) {
    WindowEntry* window = param; //cast to correct type
    MsgWinData* data = window->userdata->address;
    Vec pos;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, window->alpha}, window->x, window->y, window->height, window->width, 30.0f);
    msgDisp(window->userdata, window->alpha, 40.0f + window->x, window->y - 25.0f);
    pos.x = 2.0f + window->x;
    data->cursorY += 0.3f * (window->y - (f32)(data->sel * 31 + 58) - data->cursorY);
    pos.y = data->cursorY;
    pos.z = 0.0f;
    iconDispGxAlpha(&pos, 0x14, ICON_YUBI_CURSOR, window->alpha, 1.0f);
}

