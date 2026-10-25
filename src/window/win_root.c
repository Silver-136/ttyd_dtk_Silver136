#include "win/win_main.h"
#include "battle/battle_monosiri.h"
#include "data/item_data.h"
#include "data/mail_data.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>
#include "win/win_item.h"
#include "libc_decls.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "win/win_mario.h"
#include "win/win_party.h"

extern ItemData itemDataTable[];
void qqsort(void* base, u32 n, u32 size, s32 (*compare)(const void*, const void*));

typedef struct WinSortData {
    const char* name;
    void (*func)(WindowWork* wp);
} WinSortData;

void winMakeSkipList(WindowWork* wp);
void winTexSet_x2(u32 a, u32 b, Vec translate, Vec scale, GXColor color);
void winBadgeDisp(s32 camId, WindowWork* wp, s32 n);
void winLogDisp(s32 camId, WindowWork* wp, s32 n);
void winMailDisp(WindowWork* wp);
void winMsgDisp(WindowWork* wp, f32 x, f32 y);
void winSortGX(WindowWork* wp);
void winMakeEquipList(WindowWork* wp);

void sort_1_1_func(WindowWork* wp);
void sort_1_2_func(WindowWork* wp);
void sort_2_1_func(WindowWork* wp);
void sort_2_2_func(WindowWork* wp);
void sort_3_1_func(WindowWork* wp);
void sort_3_2_func(WindowWork* wp);
void sort_3_3_func(WindowWork* wp);
void sort_4_1_func(WindowWork* wp);
void sort_4_2_func(WindowWork* wp);
void sort_4_3_func(WindowWork* wp);
void sort_5_1_func(WindowWork* wp);
void sort_5_2_func(WindowWork* wp);
void sort_5_3_func(WindowWork* wp);
void sort_6_1_func(WindowWork* wp);
void sort_6_2_func(WindowWork* wp);
void sort_7_1_func(WindowWork* wp);
void sort_7_2_func(WindowWork* wp);
void sort_7_3_func(WindowWork* wp);
void sort_8_1_func(WindowWork* wp);
void sort_8_2_func(WindowWork* wp);

u8 enemy_monoshiri_sort_table[] = {
    0x00, 0x01, 0x02, 0x03, 0x2F, 0x30, 0x94, 0x2A, 0x62, 0x95, 0x96, 0x12, 0x24, 0x24, 0x0A, 0x0B,
    0x32, 0x14, 0x4D, 0x54, 0x33, 0x32, 0x15, 0x63, 0x43, 0x38, 0x4F, 0x3E, 0x6B, 0x6E, 0x6F, 0x79,
    0x7A, 0x7B, 0x75, 0x99, 0x01, 0x0C, 0x0D, 0x36, 0x1B, 0x1D, 0x4E, 0x56, 0x2C, 0x2D, 0x23, 0x0E,
    0x0F, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27, 0x2B, 0x18, 0x19, 0x1A, 0x5A, 0x13, 0x50, 0x51, 0x29,
    0x66, 0x67, 0x04, 0x05, 0x06, 0x3C, 0x3D, 0x52, 0x1F, 0x20, 0x40, 0x47, 0x48, 0x68, 0x9A, 0x9B,
    0x9C, 0x9D, 0x9E, 0x9F, 0x4A, 0x4B, 0x34, 0x35, 0x39, 0x21, 0x5B, 0x5C, 0x57, 0x69, 0xA0, 0xA1,
    0xA2, 0xA3, 0xA4, 0x77, 0x6B, 0x6B, 0x6B, 0x44, 0x37, 0x22, 0x49, 0x6A, 0xA5, 0xA6, 0xA7, 0xA8,
    0x45, 0x3A, 0x53, 0x70, 0x71, 0x72, 0x73, 0x6C, 0x6D, 0x76, 0xA9, 0x41, 0x4C, 0x5D, 0x5E, 0x59,
    0x5F, 0x5F, 0x16, 0x17, 0x64, 0x79, 0x7A, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68, 0x68,
    0x29, 0x28, 0x78, 0x74, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x07, 0x08, 0x09, 0x10, 0x11, 0x2E, 0x1C,
    0x1E, 0x60, 0x3B, 0x31, 0x3F, 0x55, 0x46, 0x42, 0x58, 0x61, 0x61, 0x65, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
};

#include "driver/dispdrv.h"
#include "driver/swdrv.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "evt/evt_lottery.h"
#include "icon_ids.h"
#include "driver/animdrv.h"

void winMarioInit();
void winItemInit();
void winBadgeInit();
void winLogInit();
void winMarioInit2();
void winItemInit2();
void winBadgeInit2();
void winLogInit2();
s32 winMarioMain();
s32 winItemMain();
s32 winBadgeMain();
s32 winLogMain();
void winMarioMain2();
void winItemMain2();
void winBadgeMain2();
void winLogMain2();
void winMarioExit();
void winItemExit();
void winBadgeExit();
void winLogExit();
void winRootBgDrawDisp();
void winRootBgCaptureDisp();
double fabs(double);

#define WIN_ROOT_ADD_TAB(n)                       \
    {                                             \
        WinTab* t = &w->tab[w->tabNum];       \
        t->id = n;                                \
        t->state = 0;                             \
        t->x = t->y = 1000.0f;                    \
        t->msg = msg_help[n];                     \
        w->tabNum++;                              \
    }

void winBgMain(WindowWork* wp);
void winMsgMain(WindowWork* wp);
void winSortMain(WindowWork* wp);

#ifdef VERSION_G8ME01
#define WR_FRAMES(n) (gp->framerate * (n) / 60)
#define WR_FSTEP(n) ((n) * gp->framerate / 60.0f)
#else
#define WR_FRAMES(n) (n)
#define WR_FSTEP(n) (n)
#endif

static inline void winRootTabMain(WindowWork* w) {
    s32 i;

    for (i = 0; i < 5; i++) {
        switch (w->tab[i].state) {
        case 0:
            w->tab[i].x = i * 105 - 205;
            w->tab[i].y = 300.0f;
            w->tab[i].timer = i << 2;
            w->tab[i].state++;
            break;
        case 1:
        case 11:
            if (--w->tab[i].timer < 0) {
                w->tab[i].timer = 0;
                w->tab[i].state++;
            }
            break;
        case 2:
            w->tab[i].y = intplGetValue(11, 300.0f, 185.0f, w->tab[i].timer++, WR_FRAMES(10));
            if (w->tab[i].timer > WR_FRAMES(10)) {
                w->tab[i].state++;
            }
            break;
        case 10:
            w->tab[i].timer = (4 - i) * 4;
            w->tab[i].state++;
            break;
        case 12:
            w->tab[i].y = intplGetValue(11, 180.0f, 300.0f, w->tab[i].timer++, WR_FRAMES(10));
            if (w->tab[i].timer > WR_FRAMES(10)) {
                w->tab[i].state++;
            }
            break;
        case 13:
            break;
        }
        }
}

s32 winRootMain(WindowWork* w) {
    static char* msg_help[] = {
        "msg_menu_mario", "msg_menu_nakama", "msg_menu_mochimono", "msg_menu_badge", "msg_menu_kiroku",
    };
    s32 x;
    s32 i;
    s32 ret;

    switch (w->active) {
    case 0:
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1 | PSND_SFX_BYPASS_MUTE));
        w->tabNum = 0;
        WIN_ROOT_ADD_TAB(0);
        if (swByteGet(0) >= 7 || partyChkJoin(PARTNER_GOOMBELLA)) {
            WIN_ROOT_ADD_TAB(1);
        }
        WIN_ROOT_ADD_TAB(2);
        if (swByteGet(0) >= 20 || pouchGetHaveBadgeCnt()) {
            WIN_ROOT_ADD_TAB(3);
        }
        WIN_ROOT_ADD_TAB(4);
        for (i = 0; i < w->tabNum; i++) {
            if (w->tab[i].id == w->menuSel) {
                w->tabSel = i;
            }
        }
        for (i = 0; i < 5; i++) {
            w->menu[i].state = -1;
            w->menu[i].x = w->menu[i].y = 1000.0f;
            w->menu[i].id = -1;
        }
        w->menu[0].x = w->menu[0].y = 1000.0f;
        w->menu[0].state = 0;
        w->menu[0].id = w->tab[w->tabSel].id;
        w->msgState = 0;
        w->msgOfsX = w->msgOfsY = 1000.0f;
        w->msg = NULL;
        w->msgName = 0;
        w->lastMsg = NULL;
        winMarioInit(w);
        winPartyInit(w);
        winItemInit(w);
        winBadgeInit(w);
        winLogInit(w);
        w->helpX = w->helpTgtX = w->tabSel * 105 - 270;
        w->helpY = 300.0f;
        w->helpTgtY = 185.0f;
        w->sortPosX = w->sortTgtX = -1000.0f;
        w->sortPosY = w->sortTgtY = -1000.0f;
        winMsgEntry(w, 0, w->tab[w->tabSel].msg, 0);
        w->active = 10;
        switch (w->menuSel) {
        case 1:
            winPartyInit2(w);
            w->active = 12;
            w->flags |= 0x4000;
            break;
        case 2:
            winItemInit2(w);
            w->active = 13;
            w->flags |= 0x4000;
            break;
        case 3:
            winBadgeInit2(w);
            w->active = 14;
            w->flags |= 0x4000;
            break;
        case 4:
            winLogInit2(w);
            w->active = 15;
            w->flags |= 0x4000;
            break;
        }
        if (gp->flags & 0x1000) {
            w->tabNum = 0;
            WIN_ROOT_ADD_TAB(1);
            winPartyInit2(w);
            w->active = 12;
            w->tabSel = 0;
        }
        w->texBuf = smartAlloc(GXGetTexBufferSize(gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_FALSE, 0), 1);
        w->texBufValid = 0;
        dispEntry(CAMERA_2D, 1000.0f, 0, (DispCallback)winRootBgCaptureDisp, w);
        break;
    case 10:
        if (w->buttonTrg & (PAD_BUTTON_B | PAD_BUTTON_START)) {
            if (w->menu[0].state == 100) {
                w->active = 20;
            }
        } else if (w->buttonTrg & PAD_BUTTON_A) {
            if (w->tab[w->tabSel].id == 1 && !w->party.count) {
                break;
            }
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
            w->active = w->tab[w->tabSel].id + 11;
            switch (w->tab[w->tabSel].id) {
            case 0:
                winMarioInit2(w);
                break;
            case 1:
                winPartyInit2(w);
                break;
            case 2:
                winItemInit2(w);
                break;
            case 3:
                winBadgeInit2(w);
                break;
            case 4:
                winLogInit2(w);
                break;
            }
#ifdef VERSION_G8ME01
        } else if (w->dirRep & DEMO_STICK_LEFT && w->menu[0].state == 100) {
#else
        } else if (w->dirRep & DEMO_STICK_LEFT) {
#endif
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SLIDE2 | PSND_SFX_BYPASS_MUTE));
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SLIDE1 | PSND_SFX_BYPASS_MUTE));
            w->tabSel--;
            if (w->tabSel < 0) {
                w->tabSel = w->tabNum - 1;
            }
            for (i = 4; i > 0; i--) {
                w->menu[i] = w->menu[i - 1];
            }
            w->menu[0].x = w->menu[0].y = 1000.0f;
            w->menu[0].state = 30;
            w->menu[0].id = w->tab[w->tabSel].id;
            w->helpTgtX = w->tabSel * 105 - 270;
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, 0);
#ifdef VERSION_G8ME01
        } else if (w->dirRep & DEMO_STICK_RIGHT && w->menu[0].state == 100) {
#else
        } else if (w->dirRep & DEMO_STICK_RIGHT) {
#endif
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SLIDE2 | PSND_SFX_BYPASS_MUTE));
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SLIDE1 | PSND_SFX_BYPASS_MUTE));
            w->tabSel++;
            if (w->tabSel >= w->tabNum) {
                w->tabSel = 0;
            }
            for (i = 4; i > 0; i--) {
                w->menu[i] = w->menu[i - 1];
            }
            w->menu[0].x = w->menu[0].y = 1000.0f;
            w->menu[0].state = 20;
            w->menu[0].id = w->tab[w->tabSel].id;
            w->helpTgtX = w->tabSel * 105 - 270;
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, 0);
        }
        break;
    case 100:
#ifdef VERSION_G8ME01
        if (w->menu[0].state == 100) {
#else
        if (w->menu[w->tabSel].state == 100) {
#endif
            w->active = 10;
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        switch (w->active) {
        case 11:
            ret = winMarioMain(w);
            break;
        case 12:
            ret = winPartyMain(w);
            break;
        case 13:
            ret = winItemMain(w);
            break;
        case 14:
            ret = winBadgeMain(w);
            break;
        case 15:
            ret = winLogMain(w);
            break;
        }
        if ((gp->flags & 0x1000) && ret == -1) {
            ret = -2;
        }
        if (ret == -1) {
            w->helpTgtX = w->tabSel * 105 - 270;
            w->helpTgtY = 185.0f;
            w->active = 10;
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, 0);
            w->flags &= ~0x4000;
        } else if (ret == -2) {
            w->active = 20;
            w->flags &= ~0x4000;
        }
        break;
    case 20:
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1 | PSND_SFX_BYPASS_MUTE));
        for (i = 0; i < w->tabNum; i++) {
            w->tab[i].state = 10;
        }
        for (i = 0; i < 5; i++) {
            w->menu[i].state = 10;
        }
        w->msgState = 10;
        N_mapDispOn();
        camDispOn(CAMERA_3D);
        w->active++;
        break;
    default:
        if (w->texBuf) {
            smartFree(w->texBuf);
            w->texBuf = NULL;
            w->texBufValid = 0;
        }
        w->active++;
        if (w->active > 50) {
            winMarioExit(w);
            winPartyExit(w);
            winItemExit(w);
            winBadgeExit(w);
            winLogExit(w);
            if (w->party.field_0x19C && w->party.cursor != w->party.field_0x1FC) {
                return -2;
            }
            if (w->item.useItem) {
                return -3;
            }
            return -1;
        }
        break;
    }

    winRootTabMain(w);

    winBgMain(w);
    winMsgMain(w);
    winMarioMain2(w);
    winPartyMain2(w);
    winItemMain2(w);
    winBadgeMain2(w);
    winLogMain2(w);
    winSortMain(w);

    if (w->active < 20) {
        if (w->flags & 0x4000) {
            w->helpX += (w->helpTgtX - w->helpX) / WR_FSTEP(4.0f);
            w->helpY += (w->helpTgtY - w->helpY) / WR_FSTEP(4.0f);
        } else {
            w->helpX += (w->helpTgtX - w->helpX) / WR_FSTEP(4.0f);
            w->helpY += ((w->helpTgtY + (w->tab[w->tabSel].y - 185.0f)) - w->helpY) / WR_FSTEP(4.0f);
        }
    } else if (w->helpY > 180.0f) {
        w->helpX += (w->helpTgtX - w->helpX) / WR_FSTEP(4.0f);
        w->helpY += ((w->helpTgtY + (w->tab[w->tabSel].y - 185.0f)) - w->helpY) / WR_FSTEP(4.0f);
    } else {
        w->helpX += ((w->helpTgtX + w->menu[0].x) - w->helpX) / WR_FSTEP(4.0f);
        w->helpY += (w->helpTgtY - w->helpY) / WR_FSTEP(4.0f);
    }
    w->helpX += __fabs(2.0f * (f32)sin(gp->retraceCount / 8.0f));
    w->sortPosX += (w->sortTgtX - w->sortPosX) / WR_FSTEP(4.0f);
    w->sortPosY += (w->sortTgtY - w->sortPosY) / WR_FSTEP(4.0f);
    dispEntry(CAMERA_2D, 1000.0f, 0, (DispCallback)winRootBgDrawDisp, w);
    return 0;
}

void winRootStrippedPool(f32* f) {
    f[0] = 185.0f;
    f[1] = 1000.0f;
}

static inline void winTabDisp(WindowWork* wp);

void winRootDisp(s32 camId, WindowWork* wp) {
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 i;
    s32 state;

    if (wp->active) {
        for (i = 4; i >= 0; i--) {
            state = wp->menu[i].state;
            if (state == -1) {
                continue;
            }
            if (i > 0 && wp->menu[i - 1].state == 100) {
                continue;
            }
            if (i > 0 && (u32)(state - 10) <= 1) {
                continue;
            }
            switch (wp->menu[i].id) {
                case 0:
                    winMarioDisp(camId, wp, i);
                    break;
                case 1:
                    winPartyDisp(camId, wp, i);
                    break;
                case 2:
                    winItemDisp(camId, wp, i);
                    break;
                case 3:
                    winBadgeDisp(camId, wp, i);
                    break;
                case 4:
                    winLogDisp(camId, wp, i);
                    break;
            }
        }
        winTabDisp(wp);
        winMailDisp(wp);
        winMsgDisp(wp, wp->msgOfsX, wp->msgOfsY);
        winSortGX(wp);
        if (!(wp->flags & 0x8000)) {
            winTexInit((TEXPalette*)*wp->res->data);
            winTexSet(0, (Vec){2.0f + wp->helpX, wp->helpY - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x00, 0x00, 0x00, 0x80});
            winTexSet(0, (Vec){wp->helpX, wp->helpY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        }
    }
}

static inline void winTabDisp(WindowWork* wp) {
    s32 i;

    for (i = 0; i < wp->tabNum; i++) {
        GXColor c;
        if (wp->tabSel == i) {
            c = (GXColor){0xFF, 0xFF, 0xFF, 0xFE};
        } else {
            c = (GXColor){0x80, 0x80, 0x80, 0xFE};
        }
        winTexInit_x2((TEXPalette*)*wp->res->data);
        winTexSet_x2(wp->tab[i].id + 1, 0xB2, (Vec){wp->tab[i].x, wp->tab[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c);
    }
}

void winBgMain(WindowWork* wp) {

    s32 i;

    for (i = 0; i < 5; i++) {
        switch (wp->menu[i].state) {
            case 0:
                wp->menu[i].x = 800.0f;
                wp->menu[i].y = 0.0f;
                wp->menu[i].timer = 0;
                wp->menu[i].state++;
                break;
            case 1:
                wp->menu[i].x = intplGetValue(11, 800.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(16));
                if (wp->menu[i].timer > WR_FRAMES(16)) {
                    wp->menu[i].state = 100;
                }
                break;
            case 10:
                wp->menu[i].y = 0.0f;
                wp->menu[i].timer = 0;
                wp->menu[i].state++;
                break;
            case 11:
                wp->menu[i].x = intplGetValue(11, 0.0f, 800.0f, wp->menu[i].timer++, WR_FRAMES(16));
                if (wp->menu[i].timer > WR_FRAMES(16)) {
                    wp->menu[i].state = -1;
                }
                break;
            case 20:
                wp->menu[i].y = 0.0f;
                wp->menu[i].timer = 0;
                wp->menu[i].state++;
                break;
            case 21:
                wp->menu[i].x = intplGetValue(11, 640.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(24));
                if (wp->menu[i].timer > WR_FRAMES(24)) {
                    wp->menu[i].state = 100;
                }
                break;
            case 30:
                wp->menu[i].y = 0.0f;
                wp->menu[i].timer = 0;
                wp->menu[i].state++;
                break;
            case 31:
                wp->menu[i].x = intplGetValue(11, -640.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(24));
                if (wp->menu[i].timer > WR_FRAMES(24)) {
                    wp->menu[i].state = 100;
                }
                break;
            case 40:
                wp->menu[i].y = 0.0f;
                wp->menu[i].timer = 0;
                wp->menu[i].state++;
                break;
            case 41:
                wp->menu[i].x = intplGetValue(11, 0.0f, -640.0f, wp->menu[i].timer++, WR_FRAMES(24));
                if (wp->menu[i].timer > WR_FRAMES(24)) {
                    wp->menu[i].state = -1;
                }
                break;
            case -1:
            case 100:
                break;
        }
    }
}

void winBgGX(WindowWork* wp, s32 type, f32 x, f32 y) {
    Mtx m;
    GXTexObj obj;
    f32 sy;

    switch (type) {
    case 0:
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xC8, 0x66, 0x60, 0xFF });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0xE1, 0xA5, 0xA1, 0xFF });
        GXSetTevColor(GX_TEVREG2, (GXColor){ 0xF0, 0xCD, 0xCB, 0xFF });
        break;
    case 1:
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xC8, 0x6B, 0x96, 0xFF });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0xDB, 0xAB, 0xC6, 0xFF });
        GXSetTevColor(GX_TEVREG2, (GXColor){ 0xEC, 0xD1, 0xE1, 0xFF });
        break;
    case 2:
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xBB, 0x9C, 0x21, 0xFF });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0xD7, 0xCD, 0x76, 0xFF });
        GXSetTevColor(GX_TEVREG2, (GXColor){ 0xEA, 0xE5, 0xAD, 0xFF });
        break;
    case 3:
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0x2F, 0xA5, 0x30, 0xFF });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0xA2, 0xD2, 0x6E, 0xFF });
        GXSetTevColor(GX_TEVREG2, (GXColor){ 0xCB, 0xE7, 0xA7, 0xFF });
        break;
    case 4:
        GXSetTevColor(GX_TEVREG0, (GXColor){ 0x28, 0x7E, 0xC5, 0xFF });
        GXSetTevColor(GX_TEVREG1, (GXColor){ 0x78, 0xB3, 0xD6, 0xFF });
        GXSetTevColor(GX_TEVREG2, (GXColor){ 0xAF, 0xD6, 0xEA, 0xFF });
        break;
    }
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    PSMTXTrans(m, x, 18.0f + y, 0.0f);
    PSMTXConcat(camGetPtr((CameraId)8)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, 7);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    sy = 320.0f / GXGetTexObjHeight(&obj);
    PSMTXScale(m, 570.0f / GXGetTexObjWidth(&obj), sy, 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-285.0f, 160.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(-285.0f, -160.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.0f, -160.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.0f, 160.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, 160.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(0.0f, -160.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(285.0f, -160.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(285.0f, 160.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, 6);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    sy = 288.0f / GXGetTexObjHeight(&obj);
    PSMTXScale(m, 570.0f / GXGetTexObjWidth(&obj), sy, 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C2, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A2);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-285.0f, 144.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(-285.0f, -144.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(285.0f, -144.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(285.0f, 144.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winMsgMain(WindowWork* wp) {
    switch (wp->msgState) {
        case 0:
            wp->msgOfsX = -800.0f;
            wp->msgOfsY = -170.0f;
            wp->msgTimer = 0;
            wp->msgState++;
            break;
        case 1:
            wp->msgOfsX = intplGetValue(11, -800.0f, 0.0f, wp->msgTimer++, WR_FRAMES(16));
            if (wp->msgTimer > WR_FRAMES(16)) {
                wp->msgState++;
            }
            break;
        case 2:
#ifdef VERSION_G8ME01
            if (wp->item.state == 202) {
                break;
            }
#endif
            if (keyGetDirTrg(0) & DEMO_SUBSTICK_UP) {
                wp->msgLine -= 2;
                if (wp->msgLine < 0) {
                    wp->msgLine = 0;
                } else {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE2 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (keyGetDirTrg(0) & DEMO_SUBSTICK_DOWN) {
                if (wp->msgLine / 2 < (wp->msgLines + 1) / 2 - 1) {
                    wp->msgLine += 2;
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE2 | PSND_SFX_BYPASS_MUTE));
                }
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            wp->msgTimer = 0;
            wp->msgState++;
            break;
        case 11:
            wp->msgOfsX = intplGetValue(11, 0.0f, -800.0f, wp->msgTimer++, WR_FRAMES(16));
            if (wp->msgTimer > WR_FRAMES(16)) {
                wp->msgState++;
            }
            break;
        case 12:
            break;
    }
}

void windowDispGX_Waku_col(u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve);
void winIconInit(void);
void winIconSet(u32 id, Vec translate, Vec scale, GXColor color);
void winFontInit(void);
void winFontSet(Vec translate, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetEdge(Vec translate, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetLabel(Vec translate, Vec scale, GXColor color, const char* msg);

#ifdef VERSION_G8ME01
void winFontSetEdgeWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...);
#endif
void winMsgDisp(WindowWork* wp, f32 x, f32 y) {
    GXColor white = { 0xFF, 0xFF, 0xFF, 0xFF };
    GXColor black = { 0x00, 0x00, 0x00, 0xFF };
    GXColor waku = { 0xFF, 0xFF, 0xFF, 0xFF };
    char buf[1024];
    f32 ofs;
    f32 bx;
    f32 by;
    f32 ny;
    f32 ix;
    f32 fw;
    const char* m;
    s32 n;

    bx = (10.0f + (x - 250.0f)) - 10.0f;
    by = 40.0f + y;
    windowDispGX_Waku_col(0, waku, bx, by, 500.0f, 80.0f, 20.0f);
    if (wp->msgItem != 0 && wp->msgItem > 0) {
        winTexInit((TEXPalette*)*wp->res->data);
        winTexSet(0x25, (Vec){ bx, by, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
        winIconInit();
        winIconSet(itemDataTable[wp->msgItem].icon_id, (Vec){ bx, by, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
    }
    if (wp->msgName != NULL) {
        if (wp->msgItem == -2) {
            ofs = -130.0f;
        } else {
            ofs = 0.0f;
        }
        m = msgSearch(wp->msgName);
        fw = FontGetMessageWidth(m);
        winNameGX(wp, 1, (x - 120.0f) + ofs, ny = 8.0f + (16.0f + by), 240.0f, 32.0f);
#ifdef VERSION_G8ME01
        // US: long names are squeezed to 210 wide
        if (fw > 210.0f) {
            fw = 210.0f;
        }
        winFontInit();
        winFontSetEdgeWidth((Vec){ ofs + ((x - 120.0f) + (240.0f - fw) / 2.0f), ny - 4.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white, 210.0f, m);
#else
        winFontInit();
        winFontSetEdge((Vec){ ofs + ((x - 120.0f) + (240.0f - fw) / 2.0f), ny - 4.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white, m);
#endif
    }
    if (wp->msg != NULL) {
        GXSetScissor(0, (u32)(240.0f - (by -= 14.0f)), 0x260, 0x38);
        if (strcmp(wp->msg, "msg_lottery_number") == 0) {
            n = lotteryGetPtr()->number;
            sprintf(buf, msgSearch(wp->msg), n);
            winFontInit();
            winFontSet((Vec){ (40.0f + (x - 250.0f)) - 10.0f, by + (f32)(wp->msgLine * 28), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, black, buf);
        } else {
            winFontInit();
            winFontSetLabel((Vec){ (40.0f + (x - 250.0f)) - 10.0f, by + (f32)(wp->msgLine * 28), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, black, wp->msg);
        }
        GXSetScissor(0, 0, 0x260, 0x1E0);
    }
#ifdef VERSION_G8ME01
    if (wp->item.state != 202 && wp->msgLines > 2) {
#else
    if (wp->msgLines > 2) {
#endif
        winIconInit();
        if (wp->msgLine == 0) {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                winIconSet(ICON_1B6, (Vec){ (250.0f + x) - 10.0f, (40.0f + (y - 40.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        } else if (wp->msgLine / 2 == (wp->msgLines + 1) / 2 - 1) {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                winIconSet(ICON_1B5, (Vec){ (250.0f + x) - 10.0f, 24.0f + (40.0f + (y - 40.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        } else {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                ix = (250.0f + x) - 10.0f;
                winIconSet(ICON_1B5, (Vec){ ix, 24.0f + (by = 40.0f + (y - 40.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                winIconSet(ICON_1B6, (Vec){ ix, by - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        }
        winIconSet(ICON_070, (Vec){ (250.0f + x) - 10.0f, 40.0f + (y - 40.0f), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    }
}

void winMsgEntry(WindowWork* wp, s32 item, const char* msg, const char* name) {
    static char _tmp[0x40];
    BOOL changed;
    u16 lines;

    changed = FALSE;
    if (msg) {
        if (wp->lastMsg != msg) {
            changed = TRUE;
        } else if (strcmp(_tmp, msg)) {
            changed = TRUE;
        }
        if (changed) {
            wp->msgItem = item;
            wp->msg = msg;
            wp->msgName = name;
            FontGetMessageWidthLine(msgSearch(wp->msg), &lines);
            wp->msgLine = 0;
            wp->msgLines = lines + 1;
            wp->lastMsg = msg;
            strncpy(_tmp, msg, 60);
        }
    }
}

static inline void winRootTexQuad(WindowWork* wp, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;
    f32 sy;
    double aw;
    double ah;

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    ah = __fabs(h);
    aw = __fabs(w);
    PSMTXScale(m, (f32)aw / GXGetTexObjWidth(&obj), (f32)ah / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

#pragma inline_max_total_size(100000)
void winKirinukiGX(WindowWork* wp, s32 type, f32 x, f32 y, f32 w, f32 h) {
#ifndef VERSION_G8ME01
    f32 w74 = 74.0f;
    f32 w66 = 66.0f;
#endif
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
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
    GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
    GXSetCurrentMtx(0);
    if (type == 0) {
        winRootTexQuad(wp, 0xB, x, y, 8, 8);
        winRootTexQuad(wp, 0x9, x + 8, y, w - 8 - 8, 8);
        winRootTexQuad(wp, 0xD, x + w - 8, y, 8, 8);
        winRootTexQuad(wp, 0xA, x, y - 8, 8, h - 8 - 8);
        winRootTexQuad(wp, 0x8, x + 8, y - 8, w - 8, h - 8 - 8);
        winRootTexQuad(wp, 0xC, x, y - (h - 8), 8, 8);
        winRootTexQuad(wp, 0x8, x + 8, y - (h - 8), w - 8 - 8, 8);
        winRootTexQuad(wp, 0xE, x + w - 8, y - (h - 8), 8, 8);
    } else {
#ifdef VERSION_G8ME01
        // US: the tab under the frame is sized to the "remaining" label
        f32 nk = 0.5f * FontGetMessageWidth(msgSearch("msg_menu_badge_nokori")) + 60.0f;

#endif
        winRootTexQuad(wp, 0xB, x, y, 8, 8);
        winRootTexQuad(wp, 0x9, x + 8, y, w - 8 - 8, 8);
        winRootTexQuad(wp, 0xD, x + w - 8, y, 8, 8);
        winRootTexQuad(wp, 0xA, x, y - 8, 8, h - 8 - 8);
        winRootTexQuad(wp, 0x8, x + 8, y - 8, w - 8, h - 8 - 8);
        winRootTexQuad(wp, 0xC, x, y - (h - 8), 8, 8);
        winRootTexQuad(wp, 0x8, x + 8, y - (h - 8), w - 8, 8);
#ifdef VERSION_G8ME01
        winRootTexQuad(wp, 0xF, x + (w - nk), y - h, 16, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h, nk - 16, 8);
        winRootTexQuad(wp, 0xA, x + (w - nk) + 8, y - h - 8, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h - 8, nk - 16, 8);
        winRootTexQuad(wp, 0xC, x + (w - nk) + 8, y - h - 16, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h - 16, nk - 16 - 8, 8);
        winRootTexQuad(wp, 0xE, x + w - 8, y - h - 16, 8, 8);
#else
        winRootTexQuad(wp, 0xF, x + (w - 90), y - h, 16, 8);
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h, w74, 8);
        winRootTexQuad(wp, 0xA, x + (w - 90) + 8, y - h - 8, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h - 8, w74, 8);
        winRootTexQuad(wp, 0xC, x + (w - 90) + 8, y - h - 16, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h - 16, w66, 8);
        winRootTexQuad(wp, 0xE, x + w - 8, y - h - 16, 8, 8);
#endif
    }
}

static inline double fabs_d(double x) { return __fabs(x); }
static inline float fabsf_(float x) { return (float)fabs_d((double)x); }
#define NAME_QUAD(x0, y0, x1, y1)              \
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);          \
    GXPosition3f32(x0, y0, 0.0f);              \
    GXTexCoord2f32(0.0f, 0.0f);                \
    GXPosition3f32(x0, y1, 0.0f);              \
    GXTexCoord2f32(0.0f, 1.0f);                \
    GXPosition3f32(x1, y1, 0.0f);              \
    GXTexCoord2f32(1.0f, 1.0f);                \
    GXPosition3f32(x1, y0, 0.0f);              \
    GXTexCoord2f32(1.0f, 0.0f)

#define NAME_TEX(obj, id, mtx, tw, th)                                                        \
    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, id);                                   \
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1); \
    GXLoadTexObj(&obj, GX_TEXMAP0);                                                           \
    GXSetNumTexGens(1);                                                                       \
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY); \
    MTXScale(mtx, fabsf_(tw) / (f32)GXGetTexObjWidth(&obj), fabsf_(th) / (f32)GXGetTexObjHeight(&obj), 1.0f); \
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4)

static inline void nameQuad(f32 x0, f32 y0, f32 qw, f32 qh) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x0, y0, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x0, y0 - qh, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x0 + qw, y0 - qh, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x0 + qw, y0, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winNameGX(WindowWork* wp, s32 type, f32 x, f32 y, f32 w, f32 h) {
    Mtx m0a;
    Mtx m0b;
    Mtx m1a;
    Mtx m1b;
    Mtx m2a;
    Mtx m2b;
    GXTexObj t0a;
    GXTexObj t0b;
    GXTexObj t1a;
    GXTexObj t1b;
    GXTexObj t2a;
    GXTexObj t2b;

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
    GXSetCurrentMtx(0);

    if (type == 0) {
        NAME_TEX(t0a, 0x13, m0a, w - 16.0f, 32.0f);
        nameQuad(x, y, w - 16.0f, 32.0f);
        NAME_TEX(t0b, 0x14, m0b, 16.0f, 32.0f);
        
        nameQuad((x + w) - 16.0f, y, 16.0f, 32.0f);
    } else if (type == 1) {
        NAME_TEX(t1a, 0x15, m1a, w - 16.0f, 32.0f);
        nameQuad(x, y, w - 16.0f, 32.0f);
        NAME_TEX(t1b, 0x16, m1b, 16.0f, 32.0f);
        
        nameQuad((x + w) - 16.0f, y, 16.0f, 32.0f);
    } else if (type == 2) {
        NAME_TEX(t2a, 0x78, m2a, w - 8.0f, 24.0f);
        nameQuad(x, y, w - 8.0f, 24.0f);
        NAME_TEX(t2b, 0x79, m2b, 8.0f, 24.0f);
        
        nameQuad((x + w) - 8.0f, y, 8.0f, 24.0f);
    }
}

void winFontInit(void);
void winFontSetLabel(Vec pos, Vec scale, GXColor color, const char* msg);
void* L_msgGetWork(void);

static inline void winBookTex(WindowWork* w, s32 id, f32 x, f32 y, f32 width, f32 height) {
    GXTexObj obj;
    Mtx m;
    f64 aw;
    f64 ah;

    TEXGetGXTexObjFromPalette((TEXPalette*)*w->res->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    ah = __fabs(height);
    aw = __fabs(width);
    PSMTXScale(m, (f32)aw / GXGetTexObjWidth(&obj), (f32)ah / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - height, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + width, y - height, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + width, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winBookGX(WindowWork* w, s32 a, f32 x, f32 y) {
    f32 off;
    s32 i;

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    if (a == 0) {
        off = 0.0f;
    } else {
        off = 32.0f;
    }

    winBookTex(w, 0x33, 8.0f + x, y - 8.0f, 432.0f, 256.0f - off);
    winBookTex(w, 0x28, x, y, 16.0f, 8.0f);
    winBookTex(w, 0x2C, (16.0f + x), y, 184.0f, 8.0f);
    winBookTex(w, 0x26, (184.0f + (16.0f + x)), y, 48.0f, 16.0f);
    winBookTex(w, 0x2D, (48.0f + (184.0f + (16.0f + x))), y, 184.0f, 8.0f);
    winBookTex(w, 0x2A, (184.0f + (48.0f + (184.0f + (16.0f + x)))), y, 16.0f, 8.0f);
    winBookTex(w, 0x30, x, y - 8.0f, 8.0f, 256.0f - off);
    winBookTex(w, 0x32, 8.0f + (184.0f + (16.0f + x)), y - 16.0f, 16.0f, 248.0f - off);
    winBookTex(w, 0x31, 8.0f + (184.0f + (48.0f + (184.0f + (16.0f + x)))), y - 8.0f, 8.0f, 256.0f - off);
    winBookTex(w, 0x29, x, y - 8.0f - 256.0f + off, 16.0f, 16.0f);
    winBookTex(w, 0x2E, (16.0f + x), y - 8.0f - 256.0f + off, 184.0f, 16.0f);
    winBookTex(w, 0x27, (184.0f + (16.0f + x)), y - 8.0f - 256.0f + off, 48.0f, 24.0f);
    winBookTex(w, 0x2F, (48.0f + (184.0f + (16.0f + x))), y - 8.0f - 256.0f + off, 184.0f, 16.0f);
    winBookTex(w, 0x2B, (184.0f + (48.0f + (184.0f + (16.0f + x)))), y - 8.0f - 256.0f + off, 16.0f, 16.0f);

    if (a == 1) {
        for (i = 0; i < 8; i++) {
            winBookTex(w, 0x34, (16.0f + x) + (i % 2) * 216, y - 32.0f - (i / 2) * 48, 192.0f, 32.0f);
        }
    }

    if (a == 0) {
        s32 pose;
        AnimData* anim;
        f32 lx;

        winFontInit();
        lx = 20.0f + x;
        winFontSetLabel((Vec){lx, y - 14.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0xFF},
                        luigi_book_list[w->item.bookIdx][w->item.bookPage * 2]);
        winFontSetLabel((Vec){220.0f + lx, y - 14.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0xFF},
                        luigi_book_list[w->item.bookIdx][w->item.bookPage * 2 + 1]);
        pose = ((MessageWork*)L_msgGetWork())->poseId;
        anim = animPoseGetAnimDataPtr(pose);
        if (w->item.bookPage < w->item.bookPages - 1) {
            animPoseSetAnim(pose, "HOS_C_1", 0);
        } else {
            animPoseSetAnim(pose, "HOS_E_1", 0);
        }
        animPoseSetLocalTime(pose, gp->retraceCount % (s32)anim->loopInfo[2]);
        animPoseMain(pose);
        animPoseDraw(pose, 425.0f + x, y - 275.0f, 0.0f, 180.0f, 5.0f, 2);
    }
}

static inline void winHalfBookQuad(TEXPalette* tpl, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;
    f64 aw;
    f64 ah;

    TEXGetGXTexObjFromPalette(tpl, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    ah = __fabs(h);
    aw = __fabs(w);
    MTXScale(m, (f32)aw / (f32)GXGetTexObjWidth(&obj), (f32)ah / (f32)GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winHalfBookGX(TEXPalette* tpl, s32 a, f32 x, f32 y) {
    s32 i;
    Mtx m;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x00, 0x00, 0x00, 0x80 });
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetZCompLoc(GX_TRUE);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
            MTXTrans(m, 8.0f, -8.0f, 0.0f);
            MTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
            GXLoadPosMtxImm(m, 0);
            GXSetCurrentMtx(0);
        } else {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(GX_FALSE);
            GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
            GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
            GXSetCurrentMtx(0);
        }
        winHalfBookQuad(tpl, 0x33, x + 8.0f, y - 8.0f, 320.0f, 248.0f);
        winHalfBookQuad(tpl, 0x28, x, y, 16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x35, x + 16.0f, y, 248.0f, 8.0f);
        winHalfBookQuad(tpl, 0x26, x + 16.0f + 248.0f, y, 48.0f, 16.0f);
        winHalfBookQuad(tpl, 0x3B, x + 16.0f + 248.0f + 48.0f, y, 24.0f, 16.0f);
        winHalfBookQuad(tpl, 0x30, x, y - 8.0f, 8.0f, 248.0f);
        winHalfBookQuad(tpl, 0x32, x + 16.0f + 248.0f + 8.0f, y - 16.0f, 16.0f, 240.0f);
        winHalfBookQuad(tpl, 0x36, x + 16.0f, y - 200.0f, 248.0f, 40.0f);
        winHalfBookQuad(tpl, 0x3A, x, y - 256.0f, 16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x38, x + 32.0f, y - 256.0f, -16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x38, x + 32.0f, y - 256.0f, 240.0f, 8.0f);
        winHalfBookQuad(tpl, 0x39, x + 272.0f, y - 256.0f, 16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x38, x + 288.0f, y - 256.0f, 16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x38, x + 320.0f, y - 256.0f, -16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x38, x + 320.0f, y - 256.0f, 16.0f, 8.0f);
        winHalfBookQuad(tpl, 0x3C, x + 328.0f, y - 16.0f, 8.0f, 240.0f);
    }
}

void winMailGX(WindowWork* wp, f32 x, f32 y) {
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
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
    GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
    GXSetCurrentMtx(0);
    winRootTexQuad(wp, 0x4D, x, y, 32, 24);
    winRootTexQuad(wp, 0x63, x + 32, y, 344, 24);
    winRootTexQuad(wp, 0x64, x + 200, y, 16, 24);
    winRootTexQuad(wp, 0x55, x + 32, y - 24, 344, 8);
    winRootTexQuad(wp, 0x4E, x + 32 + 72 + 40 + 48 + 16 + 112 + 40 + 16, y, 40, 24);
    winRootTexQuad(wp, 0x51, x, y - 24, 16, 248);
    winRootTexQuad(wp, 0x55, x + 16, y - 24, 16, 248);
    winRootTexQuad(wp, 0x55, x + 16 + 16 + 72 + 8 + 248 + 16, y - 24, 16, 248);
    winRootTexQuad(wp, 0x51, x + 16 + 16 + 72 + 8 + 248 + 16 + 16, y - 24, 16, 248);
    winRootTexQuad(wp, 0x52, x + 16 + 16 + 72 + 8 + 248 + 16 + 16 + 16, y - 24, 8, 248);
    winRootTexQuad(wp, 0x5C, x + 32, y - 32, 344, 240);
    winRootTexQuad(wp, 0x55, x + 32, y - 248, 344, 24);
    winRootTexQuad(wp, 0x4F, x, y - 272, 32, 32);
    winRootTexQuad(wp, 0x53, x + 32, y - 272, 344, 32);
    winRootTexQuad(wp, 0x50, x + 32 + 344, y - 272, 40, 32);
    winRootTexQuad(wp, 0x54, x + 16, y - 24 - 8, 8, -8);
    winRootTexQuad(wp, 0x54, x + 384 + 8, y - 24 - 8, -8, -8);
    winRootTexQuad(wp, 0x54, x + 16, y - 264, 8, 8);
    winRootTexQuad(wp, 0x54, x + 384 + 8, y - 264, -8, 8);
    GXSetScissor(x + 304 + 32, 240 - y + 32 + 1, 344, 200);
}

void winIconInit(void);
void winIconSet(u32 id, Vec translate, Vec scale, GXColor color);
void winFontSet(Vec translate, Vec scale, GXColor color, const char* fmt, ...);
BOOL swGet(s32 id);

void winHakoGX(WindowWork* w, s32 a, f32 x, f32 y) {
    u32 c0;
    u32 c1;
    f32 scroll;
    s32 ofs;
    s32 j;
    s32 i;
    s32 py;
    s32 k;
    s32 idx;
    s32 id;
    f32 yy;
    const char* msg;
    f32 mw;
    f32 fx;
    s32 npages;
    s32 p;
    s32 page;
    s32 num;

    if (a == 0) {
        scroll = w->log.badge.scroll;
        num = w->log.badge.num;
        page = w->log.badge.idx;
    } else {
        scroll = w->log.recipe.scroll;
        num = w->log.recipe.num;
        page = w->log.recipe.idx;
    }

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetScissor(304.0f + x, 240.0f - y, 400, 228);

    ofs = a * 13;
    for (p = 0; p < (num + 27) / 28; p++) {
        py = p * 232;
        if (scroll + (y - j * 56) - py > 400.0f || scroll + (y - j * 56) - py < -400.0f) {
            continue;
        }
        if ((ofs != 0 && p == 2) || (ofs != 1 && p == 3)) {
            winBookTex(w, (a * 13 + 0x7B), x, y + scroll - p * 232, 56.0f, 56.0f);
            winBookTex(w, (a * 13 + 0x7D), x, y - 56.0f + scroll - p * 232, 8.0f, 8.0f);
            winBookTex(w, (a * 13 + 0x7E), 8.0f + x, y - 56.0f + scroll - p * 232, 48.0f, 8.0f);
            winBookTex(w, (a * 13 + 0x85), 56.0f + x, y + scroll - p * 232, 8.0f, 8.0f);
            winBookTex(w, (a * 13 + 0x84), 56.0f + x, y - 8.0f + scroll - p * 232, 8.0f, 48.0f);
            winBookTex(w, (a * 13 + 0x81), 56.0f + x, y - 56.0f + scroll - p * 232, 8.0f, 8.0f);
            break;
        }
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 4; j++) {
                winBookTex(w, (a * 13 + 0x7B) + ((i + j * 7) & 1), x + i * 56, scroll + (y - j * 56) - py, 56.0f, 56.0f);
            }
        }
        c0 = (a * 13 + 0x7D);
        c1 = (a * 13 + 0x7E);
        for (i = 0; i < 7; i++) {
            if (i == 0) {
                id = c0;
            } else if (i & 1) {
                id = (a * 13 + 0x7F);
            } else {
                id = (a * 13 + 0x86);
            }
            winBookTex(w, id, x + i * 56, y - 224.0f + scroll - py, 8.0f, 8.0f);
            if ((i & 1) == 0) {
                id = c1;
            } else {
                id = (a * 13 + 0x80);
            }
            winBookTex(w, id, 8.0f + (x + i * 56), y - 224.0f + scroll - py, 48.0f, 8.0f);
        }
        c0 = (a * 13 + 0x85);
        c1 = (a * 13 + 0x84);
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                id = c0;
            } else if (i & 1) {
                id = (a * 13 + 0x83);
            } else {
                id = (a * 13 + 0x87);
            }
            winBookTex(w, id, 392.0f + x, scroll + (y - i * 56) - py, 8.0f, 8.0f);
            if ((i & 1) == 0) {
                id = c1;
            } else {
                id = (a * 13 + 0x82);
            }
            winBookTex(w, id, 392.0f + x, scroll + (y - i * 56 - 8.0f) - py, 8.0f, 48.0f);
        }
        winBookTex(w, (a * 13 + 0x81), 392.0f + x, y - 224.0f + scroll - py, 8.0f, 8.0f);
    }

    npages = (num + 27) / 28;
    winTexInit((TEXPalette*)*w->res->data);
    for (p = 0; p < npages; p++) {
        if (scroll + (y - j * 56) - p * 232 > 400.0f || scroll + (y - j * 56) - p * 232 < -400.0f) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 7; i++) {
                s32 n = p * 28 + j * 7 + i;
                if (n >= num) {
                    continue;
                }
                yy = scroll + (y - j * 56) - p * 232 - 28.0f;
                if (ofs == 0) {
                    if (swGet(w->log.badgeIds[n] + 0x80)) {
                        continue;
                    }
                } else {
                    if (swGet(w->log.recipeIds[n] + 0x41)) {
                        continue;
                    }
                }
                winTexSet(0x97, (Vec){4.0f + (24.0f + (x + i * 56)), yy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                          (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
        }
    }

    winIconInit();
    for (p = 0; p < npages; p++) {
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 7; i++) {
                s32 n = p * 28 + j * 7 + i;
                s32 icon;
                if (n >= num) {
                    continue;
                }
                yy = scroll + (y - j * 56) - p * 232 - 28.0f;
                if (ofs == 0) {
                    if (!swGet(w->log.badgeIds[n] + 0x80)) {
                        continue;
                    }
                    icon = itemDataTable[w->log.badgeIds[n] + 0xF0].icon_id;
                } else {
                    if (!swGet(w->log.recipeIds[n] + 0x41)) {
                        continue;
                    }
                    icon = itemDataTable[w->log.recipeIds[n] + 0xB3].icon_id;
                }
                winIconSet(icon, (Vec){4.0f + (24.0f + (x + i * 56)), yy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                           (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
        }
    }

    GXSetScissor(0, 0, 608, 480);
    if (page > 0) {
        winTexInit((TEXPalette*)*w->res->data);
        fx = 395.0f + x;
        winTexSet(0x17, (Vec){fx, 15.0f + y, 0.0f}, (Vec){-1.0f, -1.0f, -1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        winIconInit();
        winIconSet(ICON_07E, (Vec){fx, y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }
    if (num > page * 28 + 28) {
        winTexInit((TEXPalette*)*w->res->data);
        fx = 395.0f + x;
        winTexSet(0x17, (Vec){fx, y - 235.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        winIconInit();
        winIconSet(ICON_080, (Vec){fx, y - 220.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }

    msg = msgSearch("msg_menu_sort_narabikae");
    mw = 0.7f * FontGetMessageWidth(msg);
    winFontInit();
    fx = x + (380.0f - mw);
    winFontSet((Vec){fx, y - 228.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0x00, 0x00, 0x00, 0xFF}, msg);
    winIconInit();
    winIconSet(ICON_217, (Vec){fx - 30.0f, y - 237.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

static inline void winWazaQuad(WindowWork* wp, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;
    f64 aw;
    f64 ah;

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    ah = __fabs(h);
    aw = __fabs(w);
    MTXScale(m, (f32)aw / (f32)GXGetTexObjWidth(&obj), (f32)ah / (f32)GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(m, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winWazaGX(WindowWork* wp, s32 a, f32 x, f32 y, f32 w, f32 h) {
    f32 w1;

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
    GXSetCurrentMtx(0);

    w1 = 0.75f * w;
    winWazaQuad(wp, 0x98, x, y, w1, 32.0f);
    winWazaQuad(wp, 0x9A, x + w1, y, w - w1 - 16.0f, 32.0f);
    winWazaQuad(wp, 0x9B, x + (w - 16.0f), y, 16.0f, 32.0f);
    winWazaQuad(wp, 0xA2, x, y - 32.0f, w1, h - 32.0f - 16.0f);
    winWazaQuad(wp, 0x9E, x + w1, y - 32.0f, w - w1 - 8.0f, h - 32.0f - 16.0f);
    winWazaQuad(wp, 0xA1, x + (w - 8.0f), y - 32.0f, 8.0f, h - 32.0f - 16.0f);
    winWazaQuad(wp, 0x9E, x + w1, y - (h - 16.0f), w - w1 - 8.0f, 8.0f);
    winWazaQuad(wp, 0xA1, x + (w - 8.0f), y - (h - 16.0f), 8.0f, 8.0f);
    winWazaQuad(wp, 0x9C, x, y - (h - 16.0f), w1, 16.0f);
    winWazaQuad(wp, 0x9F, x + w1, y - (h - 8.0f), w - w1 - 16.0f, 8.0f);
    winWazaQuad(wp, 0x9D, x + (w - 16.0f), y - (h - 16.0f), 16.0f, 16.0f);
}

WinSortData sort_1[] = {
    {"msg_menu_sort_aiueo", sort_1_1_func},
    {"msg_menu_sort_kind", sort_1_2_func},
    {NULL, NULL},
};
WinSortData sort_2[] = {
    {"msg_menu_sort_aiueo", sort_2_1_func},
    {"msg_menu_sort_kind", sort_2_2_func},
    {NULL, NULL},
};
WinSortData sort_3[] = {
    {"msg_menu_sort_aiueo", sort_3_1_func},
    {"msg_menu_sort_kind", sort_3_2_func},
    {"msg_menu_sort_bp", sort_3_3_func},
    {NULL, NULL},
};
WinSortData sort_4[] = {
    {"msg_menu_sort_aiueo", sort_4_1_func},
    {"msg_menu_sort_kind", sort_4_2_func},
    {"msg_menu_sort_bp", sort_4_3_func},
    {NULL, NULL},
};
WinSortData sort_5[] = {
    {"msg_menu_sort_aiueo", sort_5_1_func},
    {"msg_menu_sort_kind", sort_5_2_func},
    {"msg_menu_sort_bp", sort_5_3_func},
    {NULL, NULL},
};
WinSortData sort_6[] = {
    {"msg_menu_sort_aiueo", sort_6_1_func},
    {"msg_menu_sort_kind", sort_6_2_func},
    {NULL, NULL},
};
WinSortData sort_7[] = {
    {"msg_menu_sort_number", sort_7_1_func},
    {"msg_menu_sort_place", sort_7_2_func},
    {"msg_menu_sort_aiueo", sort_7_3_func},
    {NULL, NULL},
};
WinSortData sort_8[] = {
    {"msg_menu_sort_aiueo", sort_8_1_func},
    {"msg_menu_sort_number", sort_8_2_func},
    {NULL, NULL},
};
WinSortData* sort_data[] = {sort_1, sort_2, sort_3, sort_4, sort_5, sort_6, sort_7, sort_8};

void sort_1_1_func(WindowWork* wp) {
    s32 sel = wp->item.cursor[0];
    s32 mode;

    pouchGetHaveItemCnt();
    pouchHaveItem(sel);
    mode = 7;
    if (!wp->sortRev) {
        mode = 0;
    }
    pouchSortItem(mode);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[0] = 0;
    wp->item.page[0] = wp->item.cursor[0] / 10;
}

void sort_1_2_func(WindowWork* wp) {
    s32 sel = wp->item.cursor[0];

    pouchGetHaveItemCnt();
    pouchHaveItem(sel);
    pouchSortItem(!wp->sortRev ? 1 : 8);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[0] = 0;
    wp->item.page[0] = wp->item.cursor[0] / 10;
}

void sort_2_1_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 2 : 9);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[1] = 0;
    wp->item.page[1] = wp->item.cursor[1] / 10;
}

void sort_2_2_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 3 : 10);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[1] = 0;
    wp->item.page[1] = wp->item.cursor[1] / 10;
}

void sort_3_1_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
    pouchSortItem(!wp->sortRev ? 4 : 11);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

void sort_3_2_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
    pouchSortItem(!wp->sortRev ? 5 : 12);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

void sort_3_3_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
    pouchSortItem(!wp->sortRev ? 6 : 13);
    pouchSortItem(!wp->sortRev ? 6 : 13);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

s32 N_compare_func4_1(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = ((WinBadgeEnt*)a)->id;
    s16 ib = ((WinBadgeEnt*)b)->id;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
}

s32 N_compare_func4_1_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = ((WinBadgeEnt*)a)->id;
    s16 ib = ((WinBadgeEnt*)b)->id;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
}

s32 N_compare_func4_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)((WinBadgeEnt*)a)->id].type_sort_order;
    s16 ib = itemDataTable[(s16)((WinBadgeEnt*)b)->id].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

s32 N_compare_func4_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)((WinBadgeEnt*)a)->id].type_sort_order;
    s16 ib = itemDataTable[(s16)((WinBadgeEnt*)b)->id].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

void sort_4_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->badge.list, wp->badge.count, 12, N_compare_func4_1);
    } else {
        qqsort(wp->badge.list, wp->badge.count, 12, N_compare_func4_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}

void sort_4_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->badge.list, wp->badge.count, 12, N_compare_func4_2);
    } else {
        qqsort(wp->badge.list, wp->badge.count, 12, N_compare_func4_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}

void sort_4_3_func(WindowWork* wp) {
    ItemData* db;
    ItemData* da;
    WinBadgeEnt tmp;
    s32 i;
    BOOL swap;
    s32 j;

    if (!wp->sortRev) {
        for (i = 0; i < wp->badge.count - 1; i++) {
            for (j = i + 1; j < wp->badge.count; j++) {
                da = &itemDataTable[(s16)wp->badge.list[i].id];
                db = &itemDataTable[(s16)wp->badge.list[j].id];
                swap = FALSE;

                if (da->bp_cost > db->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == db->bp_cost) {
                    if (da->type_sort_order > db->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
                    tmp = wp->badge.list[i];
                    wp->badge.list[i] = wp->badge.list[j];
                    wp->badge.list[j] = tmp;
                }
            }
        }
    } else {
        for (i = 0; i < wp->badge.count - 1; i++) {
            for (j = i + 1; j < wp->badge.count; j++) {
                da = &itemDataTable[(s16)wp->badge.list[i].id];
                db = &itemDataTable[(s16)wp->badge.list[j].id];
                swap = FALSE;

                if (da->bp_cost < (&itemDataTable[(s16)wp->badge.list[j].id])->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == (&itemDataTable[(s16)wp->badge.list[j].id])->bp_cost) {
                    if (da->type_sort_order < (&itemDataTable[(s16)wp->badge.list[j].id])->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
                    tmp = wp->badge.list[i];
                    wp->badge.list[i] = wp->badge.list[j];
                    wp->badge.list[j] = tmp;
                }
            }
        }
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}

s32 compare_func5_1(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + 0xF0;
    s16 ib = *(s16*)b + 0xF0;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
}

s32 compare_func5_1_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + 0xF0;
    s16 ib = *(s16*)b + 0xF0;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
}

s32 compare_func5_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + 0xF0)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + 0xF0)].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

s32 compare_func5_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + 0xF0)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + 0xF0)].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

void sort_5_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.badgeIds, wp->log.badge.num, 2, compare_func5_1);
    } else {
        qqsort(wp->log.badgeIds, wp->log.badge.num, 2, compare_func5_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.badge.cursor = 0;
    wp->log.badge.idx = wp->log.badge.cursor / 28;
}

void sort_5_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.badgeIds, wp->log.badge.num, 2, compare_func5_2);
    } else {
        qqsort(wp->log.badgeIds, wp->log.badge.num, 2, compare_func5_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.badge.cursor = 0;
    wp->log.badge.idx = wp->log.badge.cursor / 28;
}

void sort_5_3_func(WindowWork* wp) {

    if (!wp->sortRev) {
        ItemData* db;
        ItemData* da;
        s16 a;
        s16 b;
        s32 i;
        s32 j;
        s32 swap;
        for (i = 0; i < wp->log.badge.num - 1; ++i) {
            for (j = i + 1; j < wp->log.badge.num; ++j) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                da = &itemDataTable[(s16)(a + 0xF0)];
                db = &itemDataTable[(s16)(b + 0xF0)];
                swap = FALSE;

                if (da->bp_cost > db->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == db->bp_cost) {
                    if (da->type_sort_order > db->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
                    wp->log.badgeIds[i] = b;
                    wp->log.badgeIds[j] = a;
                }
            }
        }
    } else {
        ItemData* db;
        ItemData* da;
        s16 a;
        s16 b;
        s32 i;
        s32 swap;
        s32 j;
        for (i = 0; i < wp->log.badge.num - 1; ++i) {
            for (j = i + 1; j < wp->log.badge.num; ++j) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                da = &itemDataTable[(s16)(a + 0xF0)];
                db = &itemDataTable[(s16)(b + 0xF0)];
                swap = FALSE;

                if (da->bp_cost < db->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == db->bp_cost) {
                    if (da->type_sort_order < db->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
                    wp->log.badgeIds[i] = b;
                    wp->log.badgeIds[j] = a;
                }
            }
        }
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.badge.cursor = 0;
    wp->log.badge.idx = wp->log.badge.cursor / 28;
}

s32 compare_func6_1(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + 0xB3;
    s16 ib = *(s16*)b + 0xB3;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
}

s32 compare_func6_1_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + 0xB3;
    s16 ib = *(s16*)b + 0xB3;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
}

s32 compare_func6_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + 0xB3)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + 0xB3)].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

s32 compare_func6_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + 0xB3)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + 0xB3)].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

void sort_6_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, 2, compare_func6_1);
    } else {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, 2, compare_func6_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.recipe.cursor = 0;
    wp->log.recipe.idx = wp->log.recipe.cursor / 28;
}

void sort_6_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, 2, compare_func6_2);
    } else {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, 2, compare_func6_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.recipe.cursor = 0;
    wp->log.recipe.idx = wp->log.recipe.cursor / 28;
}

s32 compare_func1(const void* a, const void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    } else if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func1_r(const void* a, const void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    } else if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

void sort_7_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

static inline u32 getMonosiriPlace(const char* place) {
    static char* tbl[] = {
        "nara", "gor", "tik", "shiga", "hei", "nok", "stn", "gon", "win", "hou", "mri", "tou", "usu",
        "gra", "jin", "muj", "dou", "hom", "eki", "rsh", "pik", "sin", "bom", "moo", "aji", "las",
    };
    char buf[64];
    u32 i;

    for (i = 0; i < 26; i++) {
        sprintf(buf, "menu_monosiri_%s", tbl[i]);
        if (!strcmp(buf, place)) {
            break;
        }
    }
    if (i >= 26) {
        i = 0;
    }
    return i;
}

s32 compare_func2(const void* a, const void* b) {
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);
    s32 ia = getMonosiriPlace(ta->locationTag);
    s32 ib = getMonosiriPlace(tb->locationTag);

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

s32 compare_func2_r(const void* a, const void* b) {
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);
    s32 ia = getMonosiriPlace(ta->locationTag);
    s32 ib = getMonosiriPlace(tb->locationTag);

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

void sort_7_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1);
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func2);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1_r);
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

s32 compare_func3(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);

    pouchHiraToKata(msgSearch(ta->enemyTag), buf1);
    pouchHiraToKata(msgSearch(tb->enemyTag), buf2);
    if (((WinSortEnt*)a)->id == 0x4D || ((WinSortEnt*)a)->id == 0x87) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 210) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf1);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf1);
        }
    }
    if (((WinSortEnt*)b)->id == 0x4D || ((WinSortEnt*)b)->id == 0x87) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 210) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf2);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf2);
        }
    }
    return strcmp(buf1, buf2);
}

s32 compare_func3_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);

    pouchHiraToKata(msgSearch(ta->enemyTag), buf1);
    pouchHiraToKata(msgSearch(tb->enemyTag), buf2);
    if (((WinSortEnt*)a)->id == 0x4D || ((WinSortEnt*)a)->id == 0x87) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 210) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf1);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf1);
        }
    }
    if (((WinSortEnt*)b)->id == 0x4D || ((WinSortEnt*)b)->id == 0x87) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 210) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf2);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf2);
        }
    }
    return strcmp(buf2, buf1);
}

void sort_7_3_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1);
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func3);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func1_r);
        qqsort(wp->log.enemy, wp->log.enemyNum, 2, compare_func3_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

s32 compare_func4(const void* a, const void* b) {
    MailData* ma = mailGetPtr(((WinSortEnt*)a)->id);
    MailData* mb = mailGetPtr(((WinSortEnt*)b)->id);
    const char* sa = msgSearch(ma->name);
    const char* sb = msgSearch(mb->name);

    return strcmp(sa, sb);
}

s32 compare_func4_r(const void* a, const void* b) {
    MailData* ma = mailGetPtr(((WinSortEnt*)a)->id);
    MailData* mb = mailGetPtr(((WinSortEnt*)b)->id);
    const char* sa = msgSearch(ma->name);
    const char* sb = msgSearch(mb->name);

    return strcmp(sb, sa);
}

void sort_8_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->item.mail, wp->item.mailNum, 2, compare_func4);
    } else {
        qqsort(wp->item.mail, wp->item.mailNum, 2, compare_func4_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->item.mailSel = 0;
    wp->item.mailPage = wp->item.mailSel / 5;
}

s32 compare_func5(const void* a, const void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    } else if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5_r(const void* a, const void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    } else if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

void sort_8_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->item.mail, wp->item.mailNum, 2, compare_func5);
    } else {
        qqsort(wp->item.mail, wp->item.mailNum, 2, compare_func5_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->item.mailSel = 0;
    wp->item.mailPage = wp->item.mailSel / 5;
}

void winSortMain(WindowWork* wp) {
    u32 trg = wp->buttonTrg;
    u32 rep = wp->dirRep;
    const char* msg;

    if (!wp->sortActive) {
        trg = 0;
        rep = 0;
    }
#ifdef VERSION_G8ME01
    // US: the 0x1000 button closes the sort menu first, without a sound
    if (trg & 0x1000) {
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    } else
#endif
    if (trg & 0x200) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    } else if (rep & 0x1000) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        wp->sortSel--;
        if (wp->sortSel < 0) {
            wp->sortSel = wp->sortNum - 1;
        }
    } else if (rep & 0x2000) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        wp->sortSel++;
        if (wp->sortSel >= wp->sortNum) {
            wp->sortSel = 0;
        }
    } else if (trg & 0x100) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
        sort_data[wp->sortType][wp->sortSel].func(wp);
    }
#ifndef VERSION_G8ME01
    else if (trg & 0x1000) {
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    }
#endif
    if (wp->sortActive) {
        wp->helpTgtX = 30.0f + wp->sortTgtX;
        wp->helpTgtY = (wp->sortTgtY - 40.0f) - (f32)(wp->sortSel * 40);
        winMsgEntry(wp, 0, sort_data[wp->sortType][wp->sortSel].name, 0);
    }
    wp->sortPosX += (wp->sortTgtX - wp->sortPosX) / 8.0f;
    wp->sortPosY += (wp->sortTgtY - wp->sortPosY) / 8.0f;
}

static inline double winSortFabs(double x) {
    return __fabs(x);
}

static inline void winSortTex(WindowWork* wp, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, GX_FALSE, 0x7D);
    PSMTXScale(m, (f32)winSortFabs(w) / (f32)GXGetTexObjWidth(&obj), (f32)winSortFabs(h) / (f32)GXGetTexObjHeight(&obj),
               1.0f);
    GXLoadTexMtxImm(m, 0x1E, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winSortGX(WindowWork* wp) {
    f32 x = wp->sortPosX;
    f32 y = wp->sortPosY;
    s32 i;
    f32 w = 128.0f;
    f32 w2 = 136.0f;
    f32 yy;
    u32 kind;

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
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
    GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
    GXSetCurrentMtx(0);

    if (wp->sortType == 7) {
        GXSetScissor((u32)(16.0f + (16.0f + (304.0f + wp->item.mailX))),
                     (u32)(40.0f + (240.0f - wp->item.mailY)), 0x200, 0xC0);
    }

    winSortTex(wp, 0xA5, x, y, w, 24.0f);
    winSortTex(wp, 0xA3, x + w, y, 24.0f, 24.0f);
    for (i = 0; i < wp->sortNum; i++) {
        winSortTex(wp, 0xA6, 8.0f + (x + 128.0f), (y - 24.0f) - (f32)(i * 40), 16.0f, 40.0f);
    }
    winSortTex(wp, 0xA4, x + w, (y - 24.0f) - (f32)(wp->sortNum * 40), 24.0f, 24.0f);
    winSortTex(wp, 0xA7, x, (y - 24.0f) - (f32)(wp->sortNum * 40), w, 24.0f);
    winSortTex(wp, 0xA8, x, y - 24.0f, w2, wp->sortNum * 40);

    for (i = 0; i < wp->sortNum; i++) {
        WinSortData* sd = sort_data[wp->sortType];
        if (strcmp(sd[i].name, "msg_menu_sort_aiueo") == 0) {
            kind = 0xA9;
        } else if (strcmp(sd[i].name, "msg_menu_sort_kind") == 0) {
            kind = 0xAC;
        } else if (strcmp(sd[i].name, "msg_menu_sort_bp") == 0) {
            kind = 0xAA;
        } else if (strcmp(sd[i].name, "msg_menu_sort_number") == 0) {
            kind = 0xAD;
        } else if (strcmp(sd[i].name, "msg_menu_sort_place") == 0) {
            kind = 0xAE;
        } else {
            kind = 0xAA;
        }
        if (i == wp->sortSel) {
            winTexInit((TEXPalette*)*wp->res->data);
            yy = (y - 20.0f) - (f32)(i * 40) - 16.0f - 8.0f;
            winTexSet(0xB6, (Vec){ 16.0f + (3.0f + (64.0f + x)), yy, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f },
                      (GXColor){ 0x00, 0x00, 0x00, 0x80 });
            winTexInit_x2((TEXPalette*)*wp->res->data);
            winTexSet_x2(kind, 0xB6, (Vec){ 4.0f + (16.0f + (64.0f + x)) - 6.0f, 6.0f + yy, 0.0f },
                         (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        } else {
            winTexInit_x2((TEXPalette*)*wp->res->data);
            winTexSet_x2(kind, 0xB6,
                         (Vec){ 4.0f + (16.0f + (64.0f + x)), (y - 20.0f) - (f32)(i * 40) - 16.0f - 8.0f, 0.0f },
                         (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0x80, 0x80, 0x80, 0xFF });
        }
    }

    if (wp->sortType == 7) {
        GXSetScissor(0, 0, 0x260, 0x1E0);
    }
}

void winSortEntry(WindowWork* wp, s32 type, f32 x, f32 y) {
    WinSortData* sd = sort_data[type];
    s32 n;

    n = 0;
    while (sd->name != NULL) {
        sd++;
        n++;
    }
    wp->sortType = type;
    wp->sortNum = n;
    wp->sortSel = 0;
    wp->sortRev = 0;
    wp->sortPosX = -1000.0f;
    wp->sortTgtX = x;
    wp->sortPosY = y;
    wp->sortTgtY = y;
    wp->sortActive = 1;
}

BOOL winSortWait(WindowWork* wp) {
    return wp->sortActive != 0;
}

void winKageGX(WindowWork* wp, GXColor color, f32 x, f32 y, f32 z, f32 s) {
    Mtx out;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    GXTexObj obj;
    f32 sy;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, 0x7A);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, x, y, z);
    sy = s * (2.0f * GXGetTexObjHeight(&obj));
    PSMTXScale(scale, s * (2.0f * GXGetTexObjWidth(&obj)), sy, 1.0f);
    PSMTXRotRad(rot, 'x', -1.3962634f);
    PSMTXConcat(trans, rot, out);
    PSMTXConcat(out, scale, out);
    PSMTXConcat(camGetPtr((CameraId)8)->view, out, out);
    GXLoadPosMtxImm(out, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void winZClear(void) {
    Mtx mtx;

    PSMTXTrans(mtx, 0.0f, 0.0f, -1000.0f);
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetColorUpdate(GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-500.0f, 300.0f, 0.0f);
    GXPosition3f32(0.0f, 300.0f, 0.0f);
    GXPosition3f32(0.0f, -100.0f, 0.0f);
    GXPosition3f32(-500.0f, -100.0f, 0.0f);
    GXSetColorUpdate(GX_TRUE);
}

#ifdef VERSION_G8ME01
void winFontSetWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...);
#endif
void winMailDisp(WindowWork* wp) {
    s32 i;
    s32 id;
    s32 kind;
    GXColor col;
    MailData* mail;

    if (wp->item.mailX < 320.0f && wp->item.mailY > -240.0f) {
        winMailGX(wp, wp->item.mailX, wp->item.mailY);
        if (wp->item.state != 0xCA) {
            for (i = 0; i < wp->item.mailNum; i++) {
                GXColor white;

                id = wp->item.mail[i].id;
                kind = wp->item.mail[i].kind;
                mail = mailGetPtr(id);
                {
                GXColor black = { 0x00, 0x00, 0x00, 0xFF };
                if (mail->flags & 1) {
                    col = (GXColor){ 0xD7, 0xAC, 0xBC, 0xFF };
                } else if (mail->flags & 2) {
                    col = (GXColor){ 0xD3, 0xD7, 0xAC, 0xFF };
                } else if (mail->flags & 4) {
                    col = (GXColor){ 0xAC, 0xC2, 0xD7, 0xFF };
                } else {
                    col = (GXColor){ 0xE1, 0xE1, 0xE1, 0xFF };
                }
                winTexInit((TEXPalette*)*wp->res->data);
                winTexSet(0x77, (Vec){ 4.0f + (208.0f + wp->item.mailX), wp->item.mailScroll + ((wp->item.mailY - 56.0f) - (f32)(i * 40)), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, col);
                white = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
                winTexSet((u32)((s32)pouchCheckMail(id) == 2 ? (void*)0x5E : (void*)0x5D), (Vec){ 72.0f + wp->item.mailX, wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                winTexSet(0x65 + (kind + 1) / 10, (Vec){ 2.0f + (88.0f + wp->item.mailX), wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                winTexSet(0x65 + (kind + 1) % 10, (Vec){ 2.0f + (104.0f + wp->item.mailX), wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                winFontInit();
#ifdef VERSION_G8ME01
                winFontSetWidth((Vec){ 120.0f + wp->item.mailX, wp->item.mailScroll + (8.0f + ((wp->item.mailY - 56.0f) - (f32)(i * 40))), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, 230.0f, msgSearch(mail->name));
#else
                winFontSetLabel((Vec){ 120.0f + wp->item.mailX, wp->item.mailScroll + (8.0f + ((wp->item.mailY - 56.0f) - (f32)(i * 40))), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, mail->name);
#endif
                }
            }
            GXSetScissor(0, 0, 0x260, 0x1E0);
        } else {
            s32 kind2 = wp->item.mail[wp->item.mailSel].kind;
            GXColor white = { 0xFF, 0xFF, 0xFF, 0xFF };
            GXColor black = { 0x00, 0x00, 0x00, 0xFF };
            GXColor col;

            mail = mailGetPtr(wp->item.mail[wp->item.mailSel].id);
            if (mail->flags & 1) {
                col = (GXColor){ 0xD7, 0xAC, 0xBC, 0xFF };
            } else if (mail->flags & 2) {
                col = (GXColor){ 0xD3, 0xD7, 0xAC, 0xFF };
            } else if (mail->flags & 4) {
                col = (GXColor){ 0xAC, 0xC2, 0xD7, 0xFF };
            } else {
                col = (GXColor){ 0xE1, 0xE1, 0xE1, 0xFF };
            }
            winTexInit((TEXPalette*)*wp->res->data);
            winTexSet(0x77, (Vec){ 4.0f + (208.0f + wp->item.mailX), wp->item.mailY - 56.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, col);
            winTexSet(0x5E, (Vec){ 72.0f + wp->item.mailX, wp->item.mailY - 56.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
            winTexSet(0x65 + (kind2 + 1) / 10, (Vec){ 2.0f + (88.0f + wp->item.mailX), (wp->item.mailY - 56.0f) - 3.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
            winTexSet(0x65 + (kind2 + 1) % 10, (Vec){ 2.0f + (104.0f + wp->item.mailX), (wp->item.mailY - 56.0f) - 3.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
            winFontInit();
#ifdef VERSION_G8ME01
            winFontSetWidth((Vec){ 120.0f + wp->item.mailX, 8.0f + (wp->item.mailY - 56.0f), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, 230.0f, msgSearch(mail->name));
#else
            winFontSetLabel((Vec){ 120.0f + wp->item.mailX, 8.0f + (wp->item.mailY - 56.0f), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, mail->name);
#endif
            GXSetScissor((u32)(32.0f + (304.0f + wp->item.mailX)), (u32)(72.0f + (240.0f - wp->item.mailY)), 0x158, 0xB1);
            winTexInit((TEXPalette*)*wp->res->data);
            winTexSet(0x77, (Vec){ 4.0f + (208.0f + wp->item.mailX), (wp->item.mailY - 56.0f) - 80.0f, 0.0f }, (Vec){ 1.0f, 7.5f, 1.0f }, col);
            winFontInit();
#ifdef VERSION_G8ME01
            winFontSetWidth((Vec){ 80.0f + wp->item.mailX, (wp->item.mailY - 80.0f) + (f32)(wp->item.mailLine * 21), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, (GXColor){ 0x00, 0x00, 0x00, 0xFF }, 280.0f, msgSearch(mail->body));
#else
            winFontSetLabel((Vec){ 80.0f + wp->item.mailX, (wp->item.mailY - 80.0f) + (f32)(wp->item.mailLine * 21), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, (GXColor){ 0x00, 0x00, 0x00, 0xFF }, mail->body);
#endif
            if (mail->flags & 0x1000) {
                winTexInit((TEXPalette*)*wp->mailRes->data);
                winTexSet(mail->icon, (Vec){ 210.0f + wp->item.mailX, 7.0f + ((wp->item.mailY - (f32)((wp->item.mailLines - 1) * 21)) + (f32)(wp->item.mailLine * 21)), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
            GXSetScissor(0, 0, 0x260, 0x1E0);
            if (wp->item.mailLines > 8) {
                winIconInit();
                if (wp->item.mailLine == 0) {
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_1B6, (Vec){ 360.0f + wp->item.mailX, (20.0f + (wp->item.mailY - 230.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
                } else if (wp->item.mailLine + 6 == wp->item.mailLines) {
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_1B5, (Vec){ 360.0f + wp->item.mailX, 24.0f + (20.0f + (wp->item.mailY - 230.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
                } else {
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_1B5, (Vec){ 360.0f + wp->item.mailX, 24.0f + (20.0f + (wp->item.mailY - 230.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                        winIconSet(ICON_1B6, (Vec){ 360.0f + wp->item.mailX, (20.0f + (wp->item.mailY - 230.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
                }
                winIconSet(ICON_070, (Vec){ 360.0f + wp->item.mailX, 20.0f + (wp->item.mailY - 230.0f), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        }
    }
}

void winRootBgCaptureDisp(s32 unused, WindowWork* bg) {
    GXSetTexCopySrc(0, 0, gp->fbWidth, gp->efbHeight);
    GXSetTexCopyDst(gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(bg->texBuf->address, GX_FALSE);
    GXTexModeSync();
    bg->texBufValid = 1;
    N_mapDispOff();
    camDispOff((CameraId)4);
}

void winRootBgDrawDisp(s32 unused, WindowWork* bg) {
    GXTexObj obj;
    f32 w;
    f32 h;

    if (bg->texBufValid) {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
        GXSetCurrentMtx(0);
        GXInitTexObj(&obj, bg->texBuf->address, gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXLoadTexObj(&obj, GX_TEXMAP0);
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        w = gp->fbWidth;
        h = gp->efbHeight;
        GXPosition3f32(-w / 2.0f, -h / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXPosition3f32(w / 2.0f, -h / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(w / 2.0f, h / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(-w / 2.0f, h / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
    }
}
