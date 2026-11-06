// Pause menu root: the tabs, help text, sort menu and shared window frames, and the sort functions for every list.

#include "version.h"
#include "driver/windowdrv.h"
#include "evt/evt_story.h"
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
#include "win/win_root.h"


typedef struct WinSortData {
    const char* name;
    void (*func)(WindowWork* wp);
} WinSortData;

// FAKE: private s32/WindowWork* view of winLogDisp (win_log.c: CameraId, void*); the definition's prototype changes
// the object (register allocation in winRootDisp), with or without winRootDisp's camId as CameraId.
void winLogDisp(s32 camId, WindowWork* wp, s32 n);
static void winMailDisp(WindowWork* wp);
static void winMsgDisp(WindowWork* wp, f32 x, f32 y);
static void winSortGX(WindowWork* wp);

static void sort_1_1_func(WindowWork* wp);
static void sort_1_2_func(WindowWork* wp);
static void sort_2_1_func(WindowWork* wp);
static void sort_2_2_func(WindowWork* wp);
static void sort_3_1_func(WindowWork* wp);
static void sort_3_2_func(WindowWork* wp);
static void sort_3_3_func(WindowWork* wp);
static void sort_4_1_func(WindowWork* wp);
static void sort_4_2_func(WindowWork* wp);
static void sort_4_3_func(WindowWork* wp);
static void sort_5_1_func(WindowWork* wp);
static void sort_5_2_func(WindowWork* wp);
static void sort_5_3_func(WindowWork* wp);
static void sort_6_1_func(WindowWork* wp);
static void sort_6_2_func(WindowWork* wp);
static void sort_7_1_func(WindowWork* wp);
static void sort_7_2_func(WindowWork* wp);
static void sort_7_3_func(WindowWork* wp);
static void sort_8_1_func(WindowWork* wp);
static void sort_8_2_func(WindowWork* wp);

#if VERSION >= VERSION_JP
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
#endif

#include "driver/dispdrv.h"
#include "driver/swdrv.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "evt/evt_lottery.h"
#include "icon_ids.h"
#include "driver/animdrv.h"

// FAKE: old-style declarations, so both can be called with w though they take no parameters (win_mario.c,
// win_badge.c); the two calls without the argument change the object.
void winMarioMain2();
void winBadgeExit();
void winRootBgDrawDisp(CameraId unused, void* param);
void winRootBgCaptureDisp(CameraId unused, void* param);

// Macro, not inline: as static inline winRootAddTab(w, id, msg) the object changes.
#define WIN_ROOT_ADD_TAB(n)             \
    {                                   \
        WinTab* t = &w->tab[w->tabNum]; \
        t->id = n;                      \
        t->state = 0;                   \
        t->x = t->y = 1000.0f;          \
        t->msg = msg_help[n];           \
        w->tabNum++;                    \
    }

static void winBgMain(WindowWork* wp);
static void winMsgMain(WindowWork* wp);
static void winSortMain(WindowWork* wp);

#if VERSION >= VERSION_US
#define WR_FRAMES(n) (gp->framerate * (n) / 60)
#define WR_FSTEP(n) ((n) * gp->framerate / 60.0f)
#else
#define WR_FRAMES(n) (n)
#define WR_FSTEP(n) (n)
#endif

/**
 * Animates the five tab labels at the top of the menu each frame:
 * on open they drop in one by one (4 frames apart) from y 300 to 185 over 10 frames;
 * states 10-13 (set when the menu closes) slide them back up, rightmost first.
 */
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
            w->tab[i].y = intplGetValue(EASING_SINE_IN_OUT, 300.0f, 185.0f, w->tab[i].timer++, WR_FRAMES(10));
            if (w->tab[i].timer > WR_FRAMES(10)) {
                w->tab[i].state++;
            }
            break;
        case 10:
            w->tab[i].timer = (4 - i) * 4;
            w->tab[i].state++;
            break;
        case 12:
            w->tab[i].y = intplGetValue(EASING_SINE_IN_OUT, 180.0f, 300.0f, w->tab[i].timer++, WR_FRAMES(10));
            if (w->tab[i].timer > WR_FRAMES(10)) {
                w->tab[i].state++;
            }
            break;
        case 13:
            break;
        }
    }
}

/**
 * Pause menu state machine, run each frame by winMain (win_main.c).
 * On open it builds the tabs (Mario, Party once Goombella joined, Items,
 * Badges once the badge tutorial is past or a badge is owned, Journal),
 * inits every page and can jump straight into the page in menuSel;
 * then L/R stick switches tabs, A enters the tab's own Main, B/Start closes.
 * Returns 0 while open, then -1 closed, -2 a partner switch was picked, -3 an item was picked to use.
 * The kiosk demo opens straight into Party; from JP on the game frame is captured as the menu's background
 * (texBuf).
 */
s32 winRootMain(WindowWork* w) {
    static char* msg_help[] = {
        "msg_menu_mario", "msg_menu_nakama", "msg_menu_mochimono", "msg_menu_badge", "msg_menu_kiroku",
    };
    s32 i;
    s32 ret;

    switch (w->active) {
    case 0:
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1 | PSND_SFX_BYPASS_MUTE));
        w->tabNum = 0;
        WIN_ROOT_ADD_TAB(0);
        if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= STG0_07_GOOMBELLA_JOINS || partyChkJoin(PARTNER_GOOMBELLA)) {
            WIN_ROOT_ADD_TAB(1);
        }
        WIN_ROOT_ADD_TAB(2);
        if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= STG0_17_BADGE_TUTORIAL || pouchGetHaveBadgeCnt()) {
            WIN_ROOT_ADD_TAB(3);
        }
        WIN_ROOT_ADD_TAB(4);
#if VERSION <= VERSION_DEMO_JP
        {
            s32 j;
            for (j = 0; j < w->tabNum; j++) {
                if (w->tab[j].id == w->menuSel) {
                    w->tabSel = j;
                }
            }
        }
#else
        for (i = 0; i < w->tabNum; i++) {
            if (w->tab[i].id == w->menuSel) {
                w->tabSel = i;
            }
        }
#endif
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
        w->msgName = NULL;
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
        winMsgEntry(w, 0, w->tab[w->tabSel].msg, NULL);
        w->active = 10;
        switch (w->menuSel) {
        case 1:
            winPartyInit2(w);
            w->active = 12;
            w->flags |= WINDOW_FLAG_DIRECT_OPEN;
            break;
        case 2:
            winItemInit2(w);
            w->active = 13;
            w->flags |= WINDOW_FLAG_DIRECT_OPEN;
            break;
        case 3:
            winBadgeInit2(w);
            w->active = 14;
            w->flags |= WINDOW_FLAG_DIRECT_OPEN;
            break;
        case 4:
            winLogInit2(w);
            w->active = 15;
            w->flags |= WINDOW_FLAG_DIRECT_OPEN;
            break;
        }
        if (gp->flags & GW_FLAG_E3) {
            w->tabNum = 0;
            WIN_ROOT_ADD_TAB(1);
            winPartyInit2(w);
            w->active = 12;
            w->tabSel = 0;
        }
#if VERSION >= VERSION_JP
        w->texBuf = smartAlloc(GXGetTexBufferSize(gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_FALSE, 0), SMART_GROUP_MAP);
        w->texBufValid = 0;
        dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, winRootBgCaptureDisp, w);
#endif
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
#if VERSION >= VERSION_US
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
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, NULL);
#if VERSION >= VERSION_US
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
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, NULL);
        }
        break;
    case 100:
#if VERSION >= VERSION_US
        if (w->menu[0].state == 100) {
#else
        // BUG: tests the menu slot at the tab index, but menu[0] is always the current tab's entry; US and later
        // test menu[0].
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
        if ((gp->flags & GW_FLAG_E3) && ret == -1) {
            ret = -2;
        }
        if (ret == -1) {
            w->helpTgtX = w->tabSel * 105 - 270;
            w->helpTgtY = 185.0f;
            w->active = 10;
            winMsgEntry(w, 0, w->tab[w->tabSel].msg, NULL);
            w->flags &= ~WINDOW_FLAG_DIRECT_OPEN;
        } else if (ret == -2) {
            w->active = 20;
            w->flags &= ~WINDOW_FLAG_DIRECT_OPEN;
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
#if VERSION >= VERSION_JP
        mapDispOn();
        camDispOn(CAMERA_3D);
#endif
        w->active++;
        break;
    default:
#if VERSION >= VERSION_JP
        if (w->texBuf) {
            smartFree(w->texBuf);
            w->texBuf = NULL;
            w->texBufValid = 0;
        }
#endif
        w->active++;
        if (w->active > 50) {
            winMarioExit(w);
            winPartyExit(w);
            winItemExit(w);
            winBadgeExit(w);
            winLogExit(w);
            if (w->party.confirmed && w->party.cursor != w->party.startCursor) {
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
        if (w->flags & WINDOW_FLAG_DIRECT_OPEN) {
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
#if VERSION >= VERSION_JP
    dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, winRootBgDrawDisp, w);
#endif
    return 0;
}

// FAKE: stand-in that nothing calls; it only places 185.0f and 1000.0f (180.0f in the early demos) in this object's constant pool.
// Removing it changes the object.
void winRootStrippedPool(f32* f) {
    f[0] = 185.0f;
#if VERSION <= VERSION_DEMO_JP
    f[1] = 180.0f;
#else
    f[1] = 1000.0f;
#endif
}

static void winTabDisp(WindowWork* wp);

/**
 * Draws the whole pause menu for winDisp (win_main.c):
 * the open pages back to front (menu slots 4..0, skipping ones covered by a fully slid-in page or sliding out),
 * then the tabs, mail window, help box, sort menu and the bobbing cursor hand with its shadow.
 * From JP on the hand is hidden while a book is open (flags 0x8000).
 */
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

            // FAKE: hand-written unsigned range test for the closing states 10 and 11;
            // state == 10 || state == 11 changes the object
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
#if VERSION <= VERSION_DEMO_JP
        {
#else
        if (!(wp->flags & WINDOW_FLAG_BOOK_OPEN)) {
#endif
            winTexInit((TEXPalette*)*wp->res->data);
            winTexSet(0, (Vec){2.0f + wp->helpX, wp->helpY - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x00, 0x00, 0x00, 0x80});
            winTexSet(0, (Vec){wp->helpX, wp->helpY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        }
    }
}

/** Draws the tab labels at their animated positions; the selected tab is white, the others grey. */
static void winTabDisp(WindowWork* wp) {
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

/**
 * Slides each open page (menu[0..4]) per frame:
 * 0/1 in from the right on open (16 frames), 10/11 out on close,
 * 20/21 and 30/31 in from the right or left on a tab change (24 frames), 40/41 out to the left.
 * 100 = settled, -1 = off.
 */
static void winBgMain(WindowWork* wp) {
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
                wp->menu[i].x = intplGetValue(EASING_SINE_IN_OUT, 800.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(16));
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
                wp->menu[i].x = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 800.0f, wp->menu[i].timer++, WR_FRAMES(16));
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
                wp->menu[i].x = intplGetValue(EASING_SINE_IN_OUT, 640.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(24));
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
                wp->menu[i].x = intplGetValue(EASING_SINE_IN_OUT, -640.0f, 0.0f, wp->menu[i].timer++, WR_FRAMES(24));
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
                wp->menu[i].x = intplGetValue(EASING_SINE_IN_OUT, 0.0f, -640.0f, wp->menu[i].timer++, WR_FRAMES(24));
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

/**
 * Draws a tab page's 570x320 background panel at (x, y), tinted per tab:
 * type 0 Mario (red), 1 Party (pink), 2 Items (yellow), 3 Badges (green), 4 Journal (blue).
 * Texture 7 is the panel shape, texture 6 the pattern on it.
 */
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
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

/**
 * Per-frame logic of the help box at the bottom of the menu:
 * slides it in on open (16 frames) and out on close (state 10),
 * and scrolls its text two lines per C-stick up/down with a sound.
 * US and later freeze the scrolling while a letter is being read (item.state 202).
 */
static void winMsgMain(WindowWork* wp) {
    switch (wp->msgState) {
        case 0:
            wp->msgOfsX = -800.0f;
            wp->msgOfsY = -170.0f;
            wp->msgTimer = 0;
            wp->msgState++;
            break;
        case 1:
            wp->msgOfsX = intplGetValue(EASING_SINE_IN_OUT, -800.0f, 0.0f, wp->msgTimer++, WR_FRAMES(16));
            if (wp->msgTimer > WR_FRAMES(16)) {
                wp->msgState++;
            }
            break;
        case 2:
#if VERSION >= VERSION_US
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
            wp->msgOfsX = intplGetValue(EASING_SINE_IN_OUT, 0.0f, -800.0f, wp->msgTimer++, WR_FRAMES(16));
            if (wp->msgTimer > WR_FRAMES(16)) {
                wp->msgState++;
            }
            break;
        case 12:
            break;
    }
}

/**
 * Draws the help box at offset (x, y): the frame, the item icon when msgItem > 0,
 * the title plate when msgName is set (shifted left for msgItem -2;
 * US and later squeeze titles over 210 wide), the text scrolled by msgLine,
 * and blinking scroll arrows when it has more than two lines.
 * "msg_lottery_number" gets the Lottery Pick number filled in.
 */
static void winMsgDisp(WindowWork* wp, f32 x, f32 y) {
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

    // FAKE: the != 0 test is redundant; wp->msgItem > 0 alone changes the object
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
#if VERSION >= VERSION_US
        // US and later: long names are squeezed to 210 wide
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
#if VERSION >= VERSION_US
    if (wp->item.state != 202 && wp->msgLines > 2) {
#else
    if (wp->msgLines > 2) {
#endif
        winIconInit();
        if (wp->msgLine == 0) {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                winIconSet(ICON_ARROW_DOWN, (Vec){ (250.0f + x) - 10.0f, (40.0f + (y - 40.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        } else if (wp->msgLine / 2 == (wp->msgLines + 1) / 2 - 1) {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                winIconSet(ICON_ARROW_UP, (Vec){ (250.0f + x) - 10.0f, 24.0f + (40.0f + (y - 40.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        } else {
            if ((gp->retraceCount & 0x1F) < 0x14) {
                ix = (250.0f + x) - 10.0f;
                winIconSet(ICON_ARROW_UP, (Vec){ ix, 24.0f + (by = 40.0f + (y - 40.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                winIconSet(ICON_ARROW_DOWN, (Vec){ ix, by - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        }
        winIconSet(ICON_SUBSTICK_NEUTRAL, (Vec){ (250.0f + x) - 10.0f, 40.0f + (y - 40.0f), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    }
}

/**
 * Sets the help box text, called by every page as the cursor moves:
 * msg is the message tag, item the item whose icon is shown (0 none, -1/-2 a titled box), name the title tag.
 * Only a new tag takes effect: it counts the lines and scrolls back to the top. A NULL msg is ignored.
 */
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

/** Draws win.tpl texture id stretched over a w x h quad with its top-left at (x, y); the caller sets up GX first. */
static inline void winRootTexQuad(WindowWork* wp, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;
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

/**
 * Draws a "kirinuki" (cut-out) inset panel of w x h at (x, y) from 8-pixel corner and edge pieces,
 * used for the boxes inside the pages.
 * Type 1 adds a tab under the bottom-right edge for the Badges page's remaining-BP label;
 * US and later size that tab to the "msg_menu_badge_nokori" text instead of a fixed 90.
 */
void winKirinukiGX(WindowWork* wp, s32 type, f32 x, f32 y, f32 w, f32 h) {
#if VERSION < VERSION_US
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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
#if VERSION >= VERSION_US
        // US and later: the tab under the frame is sized to the "remaining" label
        f32 nk = 0.5f * FontGetMessageWidth(msgSearch("msg_menu_badge_nokori")) + 60.0f;
#endif
        winRootTexQuad(wp, 0xB, x, y, 8, 8);
        winRootTexQuad(wp, 0x9, x + 8, y, w - 8 - 8, 8);
        winRootTexQuad(wp, 0xD, x + w - 8, y, 8, 8);
        winRootTexQuad(wp, 0xA, x, y - 8, 8, h - 8 - 8);
        winRootTexQuad(wp, 0x8, x + 8, y - 8, w - 8, h - 8 - 8);
        winRootTexQuad(wp, 0xC, x, y - (h - 8), 8, 8);
        winRootTexQuad(wp, 0x8, x + 8, y - (h - 8), w - 8, 8);
#if VERSION >= VERSION_US
        winRootTexQuad(wp, 0xF, x + (w - nk), y - h, 16, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h, nk - 16, 8);
        winRootTexQuad(wp, 0xA, x + (w - nk) + 8, y - h - 8, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h - 8, nk - 16, 8);
        winRootTexQuad(wp, 0xC, x + (w - nk) + 8, y - h - 16, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - nk) + 16, y - h - 16, nk - 16 - 8, 8);
#else
        winRootTexQuad(wp, 0xF, x + (w - 90), y - h, 16, 8);

        // FAKE: 74.0f and 66.0f held in locals (JP and the early demos); the literals change the object
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h, w74, 8);
        winRootTexQuad(wp, 0xA, x + (w - 90) + 8, y - h - 8, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h - 8, w74, 8);
        winRootTexQuad(wp, 0xC, x + (w - 90) + 8, y - h - 16, 8, 8);
        winRootTexQuad(wp, 0x8, x + (w - 90) + 16, y - h - 16, w66, 8);
#endif
        winRootTexQuad(wp, 0xE, x + w - 8, y - h - 16, 8, 8);
    }
}

/** Absolute value in double precision, for nameTex below. */
static inline double fabs_d(double x) { return __fabs(x); }

/** Float absolute value that goes through fabs_d (double), for nameTex below. */
static inline float fabsf_(float x) { return (float)fabs_d((double)x); }

static inline void nameTex(WindowWork* wp, GXTexObj* obj, s32 id, Mtx mtx, f32 tw, f32 th) {
    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, obj, id);
    GXInitTexObjLOD(obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    MTXScale(mtx, fabsf_(tw) / (f32)GXGetTexObjWidth(obj), fabsf_(th) / (f32)GXGetTexObjHeight(obj), 1.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
}

/** Emits one qw x qh textured quad with its top-left at (x0, y0) for winNameGX, using the texture nameTex loaded. */
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

/**
 * Draws a name plate w wide at (x, y): a stretched body plus a rounded right end.
 * Type 0 is the Party page's partner name (32 high), 1 the help box title (32 high),
 * 2 the 24-high plate on Mario's page. h is not used.
 */
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    if (type == 0) {
        nameTex(wp, &t0a, 0x13, m0a, w - 16.0f, 32.0f);
        nameQuad(x, y, w - 16.0f, 32.0f);
        nameTex(wp, &t0b, 0x14, m0b, 16.0f, 32.0f);

        nameQuad((x + w) - 16.0f, y, 16.0f, 32.0f);
    } else if (type == 1) {
        nameTex(wp, &t1a, 0x15, m1a, w - 16.0f, 32.0f);
        nameQuad(x, y, w - 16.0f, 32.0f);
        nameTex(wp, &t1b, 0x16, m1b, 16.0f, 32.0f);

        nameQuad((x + w) - 16.0f, y, 16.0f, 32.0f);
    } else if (type == 2) {
        nameTex(wp, &t2a, 0x78, m2a, w - 8.0f, 24.0f);
        nameQuad(x, y, w - 8.0f, 24.0f);
        nameTex(wp, &t2b, 0x79, m2b, 8.0f, 24.0f);

        nameQuad((x + w) - 8.0f, y, 8.0f, 24.0f);
    }
}


/**
 * Draws an open two-page book at (x, y).
 * a == 0 is a Super Luigi book read from the Items page:
 * the two current pages of luigi_book_list text and, from JP on,
 * an animated page-turn prompt (more pages or the end).
 * a == 1 is the Journal's Tattle Log book, 32 shorter, with eight entry slots in two columns.
 */
void winBookGX(WindowWork* w, s32 type, f32 x, f32 y) {
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

    if (type == 0) {
        off = 0.0f;
    } else {
        off = 32.0f;
    }

    winRootTexQuad(w, 0x33, 8.0f + x, y - 8.0f, 432.0f, 256.0f - off);
    winRootTexQuad(w, 0x28, x, y, 16.0f, 8.0f);
    winRootTexQuad(w, 0x2C, (16.0f + x), y, 184.0f, 8.0f);
    winRootTexQuad(w, 0x26, (184.0f + (16.0f + x)), y, 48.0f, 16.0f);
    winRootTexQuad(w, 0x2D, (48.0f + (184.0f + (16.0f + x))), y, 184.0f, 8.0f);
    winRootTexQuad(w, 0x2A, (184.0f + (48.0f + (184.0f + (16.0f + x)))), y, 16.0f, 8.0f);
    winRootTexQuad(w, 0x30, x, y - 8.0f, 8.0f, 256.0f - off);
    winRootTexQuad(w, 0x32, 8.0f + (184.0f + (16.0f + x)), y - 16.0f, 16.0f, 248.0f - off);
    winRootTexQuad(w, 0x31, 8.0f + (184.0f + (48.0f + (184.0f + (16.0f + x)))), y - 8.0f, 8.0f, 256.0f - off);
    winRootTexQuad(w, 0x29, x, y - 8.0f - 256.0f + off, 16.0f, 16.0f);
    winRootTexQuad(w, 0x2E, (16.0f + x), y - 8.0f - 256.0f + off, 184.0f, 16.0f);
    winRootTexQuad(w, 0x27, (184.0f + (16.0f + x)), y - 8.0f - 256.0f + off, 48.0f, 24.0f);
    winRootTexQuad(w, 0x2F, (48.0f + (184.0f + (16.0f + x))), y - 8.0f - 256.0f + off, 184.0f, 16.0f);
    winRootTexQuad(w, 0x2B, (184.0f + (48.0f + (184.0f + (16.0f + x)))), y - 8.0f - 256.0f + off, 16.0f, 16.0f);

    if (type == 1) {
        for (i = 0; i < 8; i++) {
            winRootTexQuad(w, 0x34, (16.0f + x) + (i % 2) * 216, y - 32.0f - (i / 2) * 48, 192.0f, 32.0f);
        }
    }

    if (type == 0) {
        s32 pose;
        AnimData* anim;
        f32 lx;

        winFontInit();
        lx = 20.0f + x;
        winFontSetLabel((Vec){lx, y - 14.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0xFF},
                        luigi_book_list[w->item.bookIdx][w->item.bookPage * 2]);
        winFontSetLabel((Vec){220.0f + lx, y - 14.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0xFF},
                        luigi_book_list[w->item.bookIdx][w->item.bookPage * 2 + 1]);
#if VERSION >= VERSION_JP
                        pose = ((MessageWork*)msgGetWork())->poseId;
                        anim = animPoseGetAnimDataPtr(pose);
                        if (w->item.bookPage < w->item.bookPages - 1) {
                            animPoseSetAnim(pose, "HOS_C_1", 0);
                        } else {
                            animPoseSetAnim(pose, "HOS_E_1", 0);
                        }
                        animPoseSetLocalTime(pose, gp->retraceCount % (s32)anim->loopInfo->end);
                        animPoseMain(pose);
                        animPoseDraw(pose, 425.0f + x, y - 275.0f, 0.0f, 180.0f, 5.0f, 2);
#endif
    }
}

/** Draws texture id of the given TPL stretched over a w x h quad at (x, y), for winHalfBookGX. */
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

/**
 * Draws a single-page book frame at (x, y) from the given TPL,
 * first as a half-transparent black shadow offset by (8, -8), then in full colour.
 * Used by the Tattle card display in win_log.c (monosiri_disp). a is not used.
 */
void winHalfBookGX(TEXPalette* tpl, s32 a, f32 x, f32 y) {
    s32 i;
    Mtx m;

    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
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
            GXLoadPosMtxImm(m, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
        } else {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(GX_FALSE);
            GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
            GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
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

/**
 * Draws the mail window's frame at (x, y) for winMailDisp,
 * then leaves the scissor clipped to the letter list area (winMailDisp restores it).
 */
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

/**
 * Draws a Journal collection grid ("hako" = box) at (x, y): a == 0 the badge log, 1 the recipe log.
 * Pages of 7x4 cells scroll vertically; found badges (GSWF_BADGE_FOUND_BASE) and cooked recipes show their icon,
 * the rest a blank mark. Adds page-up/down arrows and the "msg_menu_sort_narabikae" sort hint.
 */
void winHakoGX(WindowWork* w, s32 type, f32 x, f32 y) {
    f32 scroll;
    s32 ofs;
    s32 j;
    s32 i;
    s32 py;
    s32 id;
    f32 yy;
    const char* msg;
    f32 mw;
    f32 fx;
    s32 npages;
    s32 p;
    s32 page;
    s32 num;

    if (type == 0) {
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

    ofs = type * 13;
    for (p = 0; p < (num + 27) / 28; p++) {
        py = p * 232;

        // BUG: j is read before any loop sets it: uninitialized on the first page, afterwards whatever
        // the tile loop below left (4).
        if (scroll + (y - j * 56) - py > 400.0f || scroll + (y - j * 56) - py < -400.0f) {
            continue;
        }

        // ofs is type * 13 (0 or 13 in the two normal modes), so ofs != 1 always holds and page 3 always takes this branch.
        if ((ofs != 0 && p == 2) || (ofs != 1 && p == 3)) {
            winRootTexQuad(w, (type * 13 + 0x7B), x, y + scroll - p * 232, 56.0f, 56.0f);
            winRootTexQuad(w, (type * 13 + 0x7D), x, y - 56.0f + scroll - p * 232, 8.0f, 8.0f);
            winRootTexQuad(w, (type * 13 + 0x7E), 8.0f + x, y - 56.0f + scroll - p * 232, 48.0f, 8.0f);
            winRootTexQuad(w, (type * 13 + 0x85), 56.0f + x, y + scroll - p * 232, 8.0f, 8.0f);
            winRootTexQuad(w, (type * 13 + 0x84), 56.0f + x, y - 8.0f + scroll - p * 232, 8.0f, 48.0f);
            winRootTexQuad(w, (type * 13 + 0x81), 56.0f + x, y - 56.0f + scroll - p * 232, 8.0f, 8.0f);
            break;
        }
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 4; j++) {
                winRootTexQuad(w, (type * 13 + 0x7B) + ((i + j * 7) & 1), x + i * 56, scroll + (y - j * 56) - py, 56.0f, 56.0f);
            }
        }
        for (i = 0; i < 7; i++) {
            if (i == 0) {
                id = (type * 13 + 0x7D);
            } else if (i & 1) {
                id = (type * 13 + 0x7F);
            } else {
                id = (type * 13 + 0x86);
            }
            winRootTexQuad(w, id, x + i * 56, y - 224.0f + scroll - py, 8.0f, 8.0f);
            if ((i & 1) == 0) {
                id = (type * 13 + 0x7E);
            } else {
                id = (type * 13 + 0x80);
            }
            winRootTexQuad(w, id, 8.0f + (x + i * 56), y - 224.0f + scroll - py, 48.0f, 8.0f);
        }
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                id = (type * 13 + 0x85);
            } else if (i & 1) {
                id = (type * 13 + 0x83);
            } else {
                id = (type * 13 + 0x87);
            }
            winRootTexQuad(w, id, 392.0f + x, scroll + (y - i * 56) - py, 8.0f, 8.0f);
            if ((i & 1) == 0) {
                id = (type * 13 + 0x84);
            } else {
                id = (type * 13 + 0x82);
            }
            winRootTexQuad(w, id, 392.0f + x, scroll + (y - i * 56 - 8.0f) - py, 8.0f, 48.0f);
        }
        winRootTexQuad(w, (type * 13 + 0x81), 392.0f + x, y - 224.0f + scroll - py, 8.0f, 8.0f);
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
                    if (swGet(w->log.badgeIds[n] + GSWF_BADGE_FOUND_BASE)) {
                        continue;
                    }
                } else {
                    if (swGet(w->log.recipeIds[n] + GSWF_RECIPE_FOUND_BASE)) {
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
                    if (!swGet(w->log.badgeIds[n] + GSWF_BADGE_FOUND_BASE)) {
                        continue;
                    }
                    icon = itemDataTable[w->log.badgeIds[n] + BADGE_MIN].icon_id;
                } else {
                    if (!swGet(w->log.recipeIds[n] + GSWF_RECIPE_FOUND_BASE)) {
                        continue;
                    }
                    icon = itemDataTable[w->log.recipeIds[n] + ITEM_SHROOM_FRY].icon_id;
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
        winIconSet(ICON_TRIGGER_L, (Vec){fx, y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }
    if (num > page * 28 + 28) {
        winTexInit((TEXPalette*)*w->res->data);
        fx = 395.0f + x;
        winTexSet(0x17, (Vec){fx, y - 235.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        winIconInit();
        winIconSet(ICON_TRIGGER_R, (Vec){fx, y - 220.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }

    msg = msgSearch("msg_menu_sort_narabikae");
    mw = 0.7f * FontGetMessageWidth(msg);
    winFontInit();
    fx = x + (380.0f - mw);
    winFontSet((Vec){fx, y - 228.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0x00, 0x00, 0x00, 0xFF}, msg);
    winIconInit();
    winIconSet(ICON_TRIGGER_Z_ALT, (Vec){fx - 30.0f, y - 237.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

/** Draws win.tpl texture id stretched over a w x h quad at (x, y), for winWazaGX. */
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

/**
 * Draws the w x h panel behind a move list ("waza" = moves) at (x, y), with a 32-high header strip;
 * used on Mario's and the Party page. a is not used.
 */
void winWazaGX(WindowWork* wp, s32 a, f32 x, f32 y, f32 w, f32 h) {
    f32 w1;

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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

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

/** Sort options (Z) of the Items list: by name or by type. */
WinSortData sort_1[] = {
    {"msg_menu_sort_aiueo", sort_1_1_func},
    {"msg_menu_sort_kind", sort_1_2_func},
    {NULL, NULL},
};

/** Sort options of the Important Items list: by name or by type. */
WinSortData sort_2[] = {
    {"msg_menu_sort_aiueo", sort_2_1_func},
    {"msg_menu_sort_kind", sort_2_2_func},
    {NULL, NULL},
};

/** Sort options of the full badge list: by name, by type or by BP cost. */
WinSortData sort_3[] = {
    {"msg_menu_sort_aiueo", sort_3_1_func},
    {"msg_menu_sort_kind", sort_3_2_func},
    {"msg_menu_sort_bp", sort_3_3_func},
    {NULL, NULL},
};

/** Sort options of the equipped badge list: by name, by type or by BP cost. */
WinSortData sort_4[] = {
    {"msg_menu_sort_aiueo", sort_4_1_func},
    {"msg_menu_sort_kind", sort_4_2_func},
    {"msg_menu_sort_bp", sort_4_3_func},
    {NULL, NULL},
};

/** Sort options of the Journal's badge log: by name, by type or by BP cost. */
WinSortData sort_5[] = {
    {"msg_menu_sort_aiueo", sort_5_1_func},
    {"msg_menu_sort_kind", sort_5_2_func},
    {"msg_menu_sort_bp", sort_5_3_func},
    {NULL, NULL},
};

/** Sort options of the Journal's recipe log: by name or by type. */
WinSortData sort_6[] = {
    {"msg_menu_sort_aiueo", sort_6_1_func},
    {"msg_menu_sort_kind", sort_6_2_func},
    {NULL, NULL},
};
#if VERSION <= VERSION_DEMO_JP
u8 enemy_monoshiri_sort_table[] = {
    0x00, 0x01, 0x02, 0x03, 0x2F, 0x30, 0x94, 0x2A, 0x62, 0x95, 0x96, 0x12, 0x24, 0x97, 0x0A, 0x0B,
    0x32, 0x14, 0x4D, 0x54, 0x33, 0x98, 0x15, 0x63, 0x43, 0x38, 0x4F, 0x3E, 0x6B, 0x6E, 0x6F, 0x79,
    0x7A, 0x7B, 0x75, 0x99, 0x7D, 0x0C, 0x0D, 0x36, 0x1B, 0x1D, 0x4E, 0x56, 0x2C, 0x2D, 0x23, 0x0E,
    0x0F, 0x25, 0x7E, 0x26, 0x7F, 0x27, 0x80, 0x2B, 0x18, 0x19, 0x1A, 0x5A, 0x13, 0x50, 0x51, 0x29,
    0x66, 0x67, 0x04, 0x05, 0x06, 0x3C, 0x3D, 0x52, 0x1F, 0x20, 0x40, 0x47, 0x48, 0x68, 0x9A, 0x9B,
    0x9C, 0x9D, 0x9E, 0x9F, 0x4A, 0x4B, 0x34, 0x35, 0x39, 0x21, 0x5B, 0x5C, 0x57, 0x69, 0xA0, 0xA1,
    0xA2, 0xA3, 0xA4, 0x76, 0x81, 0x82, 0x83, 0x44, 0x37, 0x22, 0x49, 0x6A, 0xA5, 0xA6, 0xA7, 0xA8,
    0x45, 0x3A, 0x53, 0x70, 0x71, 0x72, 0x73, 0x6C, 0x6D, 0x76, 0xA9, 0x41, 0x4C, 0x5D, 0x5E, 0x59,
    0x5F, 0x84, 0x16, 0x17, 0x64, 0x79, 0x7A, 0x68, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93,
    0x29, 0x28, 0x78, 0x74, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x07, 0x08, 0x09, 0x10, 0x11, 0x2E, 0x1C,
    0x1E, 0x60, 0x3B, 0x31, 0x3F, 0x55, 0x46, 0x42, 0x58, 0x61, 0x8B, 0x65, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
};
#else

/** Sort options of the Tattle Log: by number, by where the enemy appears most, or by name. */
WinSortData sort_7[] = {
    {"msg_menu_sort_number", sort_7_1_func},
    {"msg_menu_sort_place", sort_7_2_func},
    {"msg_menu_sort_aiueo", sort_7_3_func},
    {NULL, NULL},
};

/** Sort options of the mail list: by name or by number. */
WinSortData sort_8[] = {
    {"msg_menu_sort_aiueo", sort_8_1_func},
    {"msg_menu_sort_number", sort_8_2_func},
    {NULL, NULL},
};
WinSortData* sort_data[] = {sort_1, sort_2, sort_3, sort_4, sort_5, sort_6, sort_7, sort_8};
#endif

/** Items list by name (pouchSortItem 0, reversed 7), then rebuilds the Important Items list (winMakeSkipList). */
static void sort_1_1_func(WindowWork* wp) {
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

/** Items list by type (pouchSortItem 1, reversed 8). */
static void sort_1_2_func(WindowWork* wp) {
    s32 sel = wp->item.cursor[0];

    pouchGetHaveItemCnt();
    pouchHaveItem(sel);
    pouchSortItem(!wp->sortRev ? 1 : 8);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[0] = 0;
    wp->item.page[0] = wp->item.cursor[0] / 10;
}

/** Important items list by name (pouchSortItem 2, reversed 9). */
static void sort_2_1_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 2 : 9);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[1] = 0;
    wp->item.page[1] = wp->item.cursor[1] / 10;
}

/** Important items list by type (pouchSortItem 3, reversed 10). */
static void sort_2_2_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 3 : 10);
    wp->sortRev = 1 - wp->sortRev;
    winMakeSkipList(wp);
    wp->item.cursor[1] = 0;
    wp->item.page[1] = wp->item.cursor[1] / 10;
}

/** Badges page, all badges by name (pouchSortItem 4, reversed 11), then rebuilds the equipped list. */
static void sort_3_1_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
    pouchSortItem(!wp->sortRev ? 4 : 11);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

/** All badges by type (pouchSortItem 5, reversed 12). */
static void sort_3_2_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
    pouchSortItem(!wp->sortRev ? 5 : 12);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

/**
 * All badges by BP cost (pouchSortItem 6, reversed 13).
 * The early demos sort by name first; JP and later just repeat the BP sort.
 */
static void sort_3_3_func(WindowWork* wp) {
    s32 sel = wp->badge.sel[0];

    pouchGetHaveBadgeCnt();
    pouchHaveBadge(sel);
#if VERSION <= VERSION_DEMO_JP
    pouchSortItem(!wp->sortRev ? 4 : 11);
#else
    pouchSortItem(!wp->sortRev ? 6 : 13);
#endif
    pouchSortItem(!wp->sortRev ? 6 : 13);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
    wp->badge.sel[0] = 0;
    wp->badge.page[0] = wp->badge.sel[0] / 8;
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos sort the equipped list through the pouch, like the owned list. */
static void sort_4_1_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 4 : 11);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
#else

/**
 * qqsort compare for the equipped badge list (WinBadgeEnt) by name;
 * names go through pouchHiraToKata so hiragana and katakana sort together.
 */
// Descriptive name (not in the symbol maps).
s32 compare_func4_1(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = ((WinBadgeEnt*)a)->id;
    s16 ib = ((WinBadgeEnt*)b)->id;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
}

/** Reverse of compare_func4_1. */
// Descriptive name (not in the symbol maps).
s32 compare_func4_1_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    s16 ia = ((WinBadgeEnt*)a)->id;
    s16 ib = ((WinBadgeEnt*)b)->id;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
}

/** qqsort compare for the equipped badge list by the item table's type_sort_order. */
// Descriptive name (not in the symbol maps).
s32 compare_func4_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)((WinBadgeEnt*)a)->id].type_sort_order;
    s16 ib = itemDataTable[(s16)((WinBadgeEnt*)b)->id].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

/** Reverse of compare_func4_2. */
// Descriptive name (not in the symbol maps).
s32 compare_func4_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)((WinBadgeEnt*)a)->id].type_sort_order;
    s16 ib = itemDataTable[(s16)((WinBadgeEnt*)b)->id].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

/** Sorts the equipped badge list by name through compare_func4_1 (or its reverse). */
static void sort_4_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->badge.list, wp->badge.count, sizeof(WinBadgeEnt), compare_func4_1);
    } else {
        qqsort(wp->badge.list, wp->badge.count, sizeof(WinBadgeEnt), compare_func4_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
#endif
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}

#if VERSION <= VERSION_DEMO_JP

/** Equipped badges by type, through the pouch (modes 5/12). */
static void sort_4_2_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 5 : 12);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
#else

/** Sorts the equipped badge list by type through compare_func4_2 (or its reverse). */
static void sort_4_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->badge.list, wp->badge.count, sizeof(WinBadgeEnt), compare_func4_2);
    } else {
        qqsort(wp->badge.list, wp->badge.count, sizeof(WinBadgeEnt), compare_func4_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
#endif
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}

#if VERSION <= VERSION_DEMO_JP

/** Equipped badges by name, then by BP cost, through the pouch. */
static void sort_4_3_func(WindowWork* wp) {
    pouchSortItem(!wp->sortRev ? 4 : 11);
    pouchSortItem(!wp->sortRev ? 6 : 13);
    wp->sortRev = 1 - wp->sortRev;
    winMakeEquipList(wp);
#else

/** Sorts the equipped badge list by BP cost, breaking ties by type_sort_order (or reversing both). */
static void sort_4_3_func(WindowWork* wp) {
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

                // FAKE: re-indexes itemDataTable instead of using db (same pointer); db->... changes the object
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
#endif
    wp->badge.sel[1] = 0;
    wp->badge.page[1] = wp->badge.sel[1] / 8;
}


/** Journal badge log by name, JP and later: names normalized through pouchHiraToKata first. */
static s32 compare_func5_1(const void* a, const void* b) {
#if VERSION <= VERSION_DEMO_JP
    const s16* ea = a;
    const s16* eb = b;
    s16 ia = *ea + BADGE_MIN;
    s16 ib = *eb + BADGE_MIN;
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));

    if (r > 0) {
        return 1;
    } else if (r < 0) {
        return -1;
    }
    return 0;
#else
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + BADGE_MIN;
    s16 ib = *(s16*)b + BADGE_MIN;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
#endif
}

/** Reverse of compare_func5_1. */
static s32 compare_func5_1_r(const void* a, const void* b) {
#if VERSION <= VERSION_DEMO_JP
    const s16* ea = a;
    const s16* eb = b;
    s16 ia = *ea + BADGE_MIN;
    s16 ib = *eb + BADGE_MIN;
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));

    if (r < 0) {
        return 1;
    } else if (r > 0) {
        return -1;
    }
    return 0;
#else
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + BADGE_MIN;
    s16 ib = *(s16*)b + BADGE_MIN;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
#endif
}

/** qqsort compare for the Journal badge log by type_sort_order. */
s32 compare_func5_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + BADGE_MIN)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + BADGE_MIN)].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

/** Reverse of compare_func5_2. */
static s32 compare_func5_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + BADGE_MIN)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + BADGE_MIN)].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

/** Journal badge log (log.badgeIds) by name. */
static void sort_5_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_1);
    } else {
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.badge.cursor = 0;
    wp->log.badge.idx = wp->log.badge.cursor / 28;
}

/** Journal badge log by type. */
static void sort_5_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_2);
    } else {
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.badge.cursor = 0;
    wp->log.badge.idx = wp->log.badge.cursor / 28;
}

/**
 * Journal badge log by BP cost.
 * The early demos sort by name first, then bubble by BP cost only; their first press puts the highest cost first.
 */
static void sort_5_3_func(WindowWork* wp) {
    if (!wp->sortRev) {
        ItemData* db;
        ItemData* da;
        s16 a;
        s16 b;
        s32 i;
        s32 j;
#if VERSION <= VERSION_DEMO_JP
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_1);
        for (i = 0; i < wp->log.badge.num - 1; i++) {
            for (j = i + 1; j < wp->log.badge.num; j++) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                if (itemDataTable[a + BADGE_MIN].bp_cost < itemDataTable[b + BADGE_MIN].bp_cost) {
#else
        s32 swap;
        for (i = 0; i < wp->log.badge.num - 1; ++i) {
            for (j = i + 1; j < wp->log.badge.num; ++j) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                da = &itemDataTable[(s16)(a + BADGE_MIN)];
                db = &itemDataTable[(s16)(b + BADGE_MIN)];
                swap = FALSE;

                if (da->bp_cost > db->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == db->bp_cost) {
                    if (da->type_sort_order > db->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
#endif
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
#if VERSION <= VERSION_DEMO_JP
        s32 j;
        qqsort(wp->log.badgeIds, wp->log.badge.num, sizeof(s16), compare_func5_1_r);
        for (i = 0; i < wp->log.badge.num - 1; i++) {
            for (j = i + 1; j < wp->log.badge.num; j++) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                if (itemDataTable[a + BADGE_MIN].bp_cost > itemDataTable[b + BADGE_MIN].bp_cost) {
#else
        s32 swap;
        s32 j;
        for (i = 0; i < wp->log.badge.num - 1; ++i) {
            for (j = i + 1; j < wp->log.badge.num; ++j) {
                a = wp->log.badgeIds[i];
                b = wp->log.badgeIds[j];
                da = &itemDataTable[(s16)(a + BADGE_MIN)];
                db = &itemDataTable[(s16)(b + BADGE_MIN)];
                swap = FALSE;

                if (da->bp_cost < db->bp_cost) {
                    swap = TRUE;
                } else if (da->bp_cost == db->bp_cost) {
                    if (da->type_sort_order < db->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
#endif
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

/** Journal recipe log by name, JP and later: names normalized through pouchHiraToKata first. */
static s32 compare_func6_1(const void* a, const void* b) {
#if VERSION <= VERSION_DEMO_JP
    const s16* ea = a;
    const s16* eb = b;
    s16 ia = *ea + ITEM_SHROOM_FRY;
    s16 ib = *eb + ITEM_SHROOM_FRY;
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));

    if (r > 0) {
        return 1;
    } else if (r < 0) {
        return -1;
    }
    return 0;
#else
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + ITEM_SHROOM_FRY;
    s16 ib = *(s16*)b + ITEM_SHROOM_FRY;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf1, buf2);
#endif
}

/** Reverse of compare_func6_1. */
static s32 compare_func6_1_r(const void* a, const void* b) {
#if VERSION <= VERSION_DEMO_JP
    const s16* ea = a;
    const s16* eb = b;
    s16 ia = *ea + ITEM_SHROOM_FRY;
    s16 ib = *eb + ITEM_SHROOM_FRY;
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));

    if (r < 0) {
        return 1;
    } else if (r > 0) {
        return -1;
    }
    return 0;
#else
    char buf1[128];
    char buf2[128];
    s16 ia = *(s16*)a + ITEM_SHROOM_FRY;
    s16 ib = *(s16*)b + ITEM_SHROOM_FRY;

    pouchHiraToKata(msgSearch(itemDataTable[ia].name), buf1);
    pouchHiraToKata(msgSearch(itemDataTable[ib].name), buf2);
    return strcmp(buf2, buf1);
#endif
}

/** qqsort compare for the Journal recipe log by type_sort_order. */
s32 compare_func6_2(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + ITEM_SHROOM_FRY)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + ITEM_SHROOM_FRY)].type_sort_order;

    if (ia > ib) {
        return 1;
    } else if (ia < ib) {
        return -1;
    }
    return 0;
}

/** Reverse of compare_func6_2. */
static s32 compare_func6_2_r(const void* a, const void* b) {
    s16 ia = itemDataTable[(s16)(*(s16*)a + ITEM_SHROOM_FRY)].type_sort_order;
    s16 ib = itemDataTable[(s16)(*(s16*)b + ITEM_SHROOM_FRY)].type_sort_order;

    if (ia < ib) {
        return 1;
    } else if (ia > ib) {
        return -1;
    }
    return 0;
}

/** Journal recipe log (log.recipeIds) by name. */
static void sort_6_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, sizeof(s16), compare_func6_1);
    } else {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, sizeof(s16), compare_func6_1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.recipe.cursor = 0;
    wp->log.recipe.idx = wp->log.recipe.cursor / 28;
}

/** Journal recipe log by type. */
static void sort_6_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, sizeof(s16), compare_func6_2);
    } else {
        qqsort(wp->log.recipeIds, wp->log.recipe.num, sizeof(s16), compare_func6_2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.recipe.cursor = 0;
    wp->log.recipe.idx = wp->log.recipe.cursor / 28;
}

/** qqsort compare for the Tattle Log (WinSortEnt) by its log number (kind, from enemy_monoshiri_sort_table). */
static s32 compare_func1(const void* a, const void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    } else if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

/** Reverse of compare_func1. */
static s32 compare_func1_r(const void* a, const void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    } else if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

/** Tattle Log by log number. */
static void sort_7_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

/**
 * Returns the place-sort rank of a tattle entry's location tag ("menu_monosiri_<area>"):
 * its index in tbl, or 0 if unknown. JP and later move the Excess Express (rsh) after the hom and eki maps.
 */
static inline u32 getMonosiriPlace(const char* place) {
    static char* tbl[] = {
        "nara", "gor", "tik", "shiga", "hei", "nok", "stn", "gon", "win", "hou", "mri", "tou", "usu",
#if VERSION <= VERSION_DEMO_JP
        "gra", "jin", "muj", "dou", "rsh", "hom", "eki", "pik", "sin", "bom", "moo", "aji", "las",
#else
        "gra", "jin", "muj", "dou", "hom", "eki", "rsh", "pik", "sin", "bom", "moo", "aji", "las",
#endif
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

#if VERSION <= VERSION_DEMO_JP
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
#endif

/** qqsort compare for the Tattle Log by where the enemy is found (getMonosiriPlace of its locationTag). */
static s32 compare_func2(const void* a, const void* b) {
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

/** Reverse of compare_func2. */
static s32 compare_func2_r(const void* a, const void* b) {
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

/** Tattle Log by place: sorts by number first, then by place. */
static void sort_7_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1);
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func2);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1_r);
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func2_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

/** Tattle Log by name, JP and later: normalized, with Doopliss's entries under his current name. */
#if VERSION >= VERSION_JP
static s32 compare_func3(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);

    pouchHiraToKata(msgSearch(ta->enemyTag), buf1);
    pouchHiraToKata(msgSearch(tb->enemyTag), buf2);
    if (((WinSortEnt*)a)->id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || ((WinSortEnt*)a)->id == UNIT_DOOPLISS_CHAPTER8) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf1);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf1);
        }
    }
    if (((WinSortEnt*)b)->id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || ((WinSortEnt*)b)->id == UNIT_DOOPLISS_CHAPTER8) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf2);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf2);
        }
    }
    return strcmp(buf1, buf2);
}
#endif

#if VERSION >= VERSION_JP

/** Reverse of compare_func3 (JP and later). */
static s32 compare_func3_r(const void* a, const void* b) {
    char buf1[128];
    char buf2[128];
    TattleEntry* ta = battleGetUnitMonosiriPtr(((WinSortEnt*)a)->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(((WinSortEnt*)b)->id);

    pouchHiraToKata(msgSearch(ta->enemyTag), buf1);
    pouchHiraToKata(msgSearch(tb->enemyTag), buf2);
    if (((WinSortEnt*)a)->id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || ((WinSortEnt*)a)->id == UNIT_DOOPLISS_CHAPTER8) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf1);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf1);
        }
    }
    if (((WinSortEnt*)b)->id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || ((WinSortEnt*)b)->id == UNIT_DOOPLISS_CHAPTER8) {
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
            pouchHiraToKata(msgSearch("btl_un_hatena"), buf2);
        } else {
            pouchHiraToKata(msgSearch("btl_un_rampell"), buf2);
        }
    }
    return strcmp(buf2, buf1);
}
#endif

#if VERSION <= VERSION_DEMO_JP

/** Early-demo Tattle Log name comparison: compares the entries' enemyTag messages directly. */
static s32 compare_func3(const void* a, const void* b) {
    const WinSortEnt* ea = a;
    const WinSortEnt* eb = b;
    TattleEntry* ta = battleGetUnitMonosiriPtr(ea->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(eb->id);
    const char* sa = msgSearch(ta->enemyTag);
    const char* sb = msgSearch(tb->enemyTag);

    return strcmp(sa, sb);
}

/** Reverse of compare_func3 (early demos). */
static s32 compare_func3_r(const void* a, const void* b) {
    const WinSortEnt* ea = a;
    const WinSortEnt* eb = b;
    TattleEntry* ta = battleGetUnitMonosiriPtr(ea->id);
    TattleEntry* tb = battleGetUnitMonosiriPtr(eb->id);
    const char* sa = msgSearch(ta->enemyTag);
    const char* sb = msgSearch(tb->enemyTag);

    return strcmp(sb, sa);
}
#endif

/** Tattle Log by name (by number first, then by name). */
static void sort_7_3_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1);
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func3);
    } else {
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func1_r);
        qqsort(wp->log.enemy, wp->log.enemyNum, sizeof(WinSortEnt), compare_func3_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->log.enemySel = 0;
    wp->log.enemyPage = wp->log.enemySel / 16;
}

/** qqsort compare for the mail list by letter title (MailData name). */
static s32 compare_func4(const void* a, const void* b) {
    MailData* ma = mailGetPtr(((WinSortEnt*)a)->id);
    MailData* mb = mailGetPtr(((WinSortEnt*)b)->id);
    const char* sa = msgSearch(ma->name);
    const char* sb = msgSearch(mb->name);

    return strcmp(sa, sb);
}

/** Reverse of compare_func4. */
static s32 compare_func4_r(const void* a, const void* b) {
    MailData* ma = mailGetPtr(((WinSortEnt*)a)->id);
    MailData* mb = mailGetPtr(((WinSortEnt*)b)->id);
    const char* sa = msgSearch(ma->name);
    const char* sb = msgSearch(mb->name);

    return strcmp(sb, sa);
}

/** Mail list (item.mail) by title. */
static void sort_8_1_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->item.mail, wp->item.mailNum, sizeof(WinSortEnt), compare_func4);
    } else {
        qqsort(wp->item.mail, wp->item.mailNum, sizeof(WinSortEnt), compare_func4_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->item.mailSel = 0;
    wp->item.mailPage = wp->item.mailSel / 5;
}

/** qqsort compare for the mail list by letter number (kind, the order the letters arrived in). */
static s32 compare_func5(const void* a, const void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    } else if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

/** Reverse of compare_func5. */
static s32 compare_func5_r(const void* a, const void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    } else if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

/** Mail list by letter number. */
static void sort_8_2_func(WindowWork* wp) {
    if (!wp->sortRev) {
        qqsort(wp->item.mail, wp->item.mailNum, sizeof(WinSortEnt), compare_func5);
    } else {
        qqsort(wp->item.mail, wp->item.mailNum, sizeof(WinSortEnt), compare_func5_r);
    }
    wp->sortRev = 1 - wp->sortRev;
    wp->item.mailSel = 0;
    wp->item.mailPage = wp->item.mailSel / 5;
}

/**
 * Per-frame input for the sort menu while it is open (sortActive):
 * up/down picks an option, A runs its sort, B closes with a sound,
 * Start closes silently (US and later check Start first).
 * The PAL demo and PAL clear sortRev whenever the cursor moves.
 * While open it points the cursor hand at the option and shows the option's help text.
 */
static void winSortMain(WindowWork* wp) {
    u32 trg = wp->buttonTrg;
    u32 rep = wp->dirRep;

    if (!wp->sortActive) {
        trg = 0;
        rep = 0;
    }
#if VERSION >= VERSION_US
    // US and later: Start is checked before B, the stick and A, so it closes the sort menu even when one of them is pressed too
    if (trg & PAD_BUTTON_START) {
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    } else
#endif
    if (trg & PAD_BUTTON_B) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    } else if (rep & DEMO_STICK_UP) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
#if VERSION >= VERSION_DEMO_EU
        wp->sortRev = 0;
#endif
        wp->sortSel--;
        if (wp->sortSel < 0) {
            wp->sortSel = wp->sortNum - 1;
        }
    } else if (rep & DEMO_STICK_DOWN) {
#if VERSION >= VERSION_DEMO_EU
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        wp->sortRev = 0;
#else
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
#endif
        wp->sortSel++;
        if (wp->sortSel >= wp->sortNum) {
            wp->sortSel = 0;
        }
    } else if (trg & PAD_BUTTON_A) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
        sort_data[wp->sortType][wp->sortSel].func(wp);
    }
#if VERSION < VERSION_US
    else if (trg & PAD_BUTTON_START) {
        wp->sortActive = 0;
        wp->sortTgtX = -600.0f;
    }
#endif
    if (wp->sortActive) {
        wp->helpTgtX = 30.0f + wp->sortTgtX;
        wp->helpTgtY = (wp->sortTgtY - 40.0f) - (f32)(wp->sortSel * 40);
        winMsgEntry(wp, 0, sort_data[wp->sortType][wp->sortSel].name, NULL);
    }
    wp->sortPosX += (wp->sortTgtX - wp->sortPosX) / 8.0f;
    wp->sortPosY += (wp->sortTgtY - wp->sortPosY) / 8.0f;
}

/** Draws win.tpl texture id stretched over a w x h quad at (x, y), for the sort menu panel. */
static inline void winSortTex(WindowWork* wp, s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx m;

    TEXGetGXTexObjFromPalette((TEXPalette*)*wp->res->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(m, (f32)fabs_d(w) / (f32)GXGetTexObjWidth(&obj), (f32)fabs_d(h) / (f32)GXGetTexObjHeight(&obj),
               1.0f);
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

/**
 * Draws the sort menu panel at sortPosX/Y with one 40-high row per option,
 * each showing its label texture (chosen from the option's message tag);
 * the selected row is lit with a shadow, the rest grey. The mail sort is clipped to the mail window.
 */
static void winSortGX(WindowWork* wp) {
    f32 x = wp->sortPosX;
    f32 y = wp->sortPosY;
    s32 i;

    // FAKE: constant locals w and w2 for the sort window sizes; the literals change the object
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

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

/**
 * Opens the sort menu (Z on the Items, Badges and Journal pages and in the mail list) with sort_data[type]'s
 * options, sliding in from the left to (x, y), first option selected and not reversed.
 */
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

/** TRUE while the sort menu is open; the pages hold their own input until it closes. */
BOOL winSortWait(WindowWork* wp) {
    return wp->sortActive != 0;
}

/**
 * Draws a round drop shadow ("kage") in color at (x, y, z),
 * scaled by s and tilted 80 degrees back so it lies flat,
 * under the figures shown in the menu (Mario, partners, the Journal's Crystal Stars).
 */
void winKageGX(WindowWork* wp, f32 x, f32 y, f32 z, f32 s, GXColor color) {
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
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, out, out);
    GXLoadPosMtxImm(out, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

/**
 * Resets the depth buffer over the left part of the screen by drawing an invisible quad at z -1000 with depth
 * writes on and colour writes off, so the 3D Mario or partner drawn next on the Mario
 * and Party pages is not hidden.
 */
void winZClear(void) {
    Mtx mtx;

    PSMTXTrans(mtx, 0.0f, 0.0f, -1000.0f);
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

/**
 * Draws the mail window opened from the Items page while it is on screen.
 * As a list: one row per letter with a sender colour (MailData flags 1/2/4), an opened/unopened envelope,
 * its two-digit number and title.
 * While a letter is read (item.state 202): its header, the body scrolled by mailLine,
 * its picture (flags 0x1000) and scroll arrows.
 */
static void winMailDisp(WindowWork* wp) {
    s32 i;
    s32 id;
    s32 kind;
    GXColor col;
    MailData* mail;

    if (wp->item.mailX < 320.0f && wp->item.mailY > -240.0f) {
        winMailGX(wp, wp->item.mailX, wp->item.mailY);
        if (wp->item.state != 202) {
            for (i = 0; i < wp->item.mailNum; i++) {
                GXColor white;

                id = wp->item.mail[i].id;
                kind = wp->item.mail[i].kind;
                mail = mailGetPtr(id);

                // FAKE: GXColor black lives in its own bare block; declaring it in the loop body changes the object
                {
                    GXColor black = { 0x00, 0x00, 0x00, 0xFF };
                    if (mail->flags & MAIL_FLAG_PEACH) {
                        col = (GXColor){ 0xD7, 0xAC, 0xBC, 0xFF };
                    } else if (mail->flags & MAIL_FLAG_TROUBLE) {
                        col = (GXColor){ 0xD3, 0xD7, 0xAC, 0xFF };
                    } else if (mail->flags & MAIL_FLAG_INFO) {
                        col = (GXColor){ 0xAC, 0xC2, 0xD7, 0xFF };
                    } else {
                        col = (GXColor){ 0xE1, 0xE1, 0xE1, 0xFF };
                    }
                    winTexInit((TEXPalette*)*wp->res->data);
                    winTexSet(0x77, (Vec){ 4.0f + (208.0f + wp->item.mailX), wp->item.mailScroll + ((wp->item.mailY - 56.0f) - (f32)(i * 40)), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, col);
                    white = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };

                    // FAKE: the texture id goes through void* casts; pouchCheckMail(id) == 2 ? 0x5E : 0x5D changes the object
                    winTexSet((u32)(pouchCheckMail(id) == 2 ? (void*)0x5E : (void*)0x5D), (Vec){ 72.0f + wp->item.mailX, wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                    winTexSet(0x65 + (kind + 1) / 10, (Vec){ 2.0f + (88.0f + wp->item.mailX), wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                    winTexSet(0x65 + (kind + 1) % 10, (Vec){ 2.0f + (104.0f + wp->item.mailX), wp->item.mailScroll + (((wp->item.mailY - 56.0f) - (f32)(i * 40)) - 3.0f), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, white);
                    winFontInit();
#if VERSION >= VERSION_US
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
            if (mail->flags & MAIL_FLAG_PEACH) {
                col = (GXColor){ 0xD7, 0xAC, 0xBC, 0xFF };
            } else if (mail->flags & MAIL_FLAG_TROUBLE) {
                col = (GXColor){ 0xD3, 0xD7, 0xAC, 0xFF };
            } else if (mail->flags & MAIL_FLAG_INFO) {
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
#if VERSION <= VERSION_DEMO_JP
            winFontSetLabel((Vec){120.0f + wp->item.mailX, 8.0f + (wp->item.mailY - 56.0f), 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, black, mail->name);
            GXSetScissor((u32)(32.0f + (304.0f + wp->item.mailX)), (u32)(72.0f + (240.0f - wp->item.mailY)), 0x158, 0xAE);
#else
#if VERSION >= VERSION_US
            winFontSetWidth((Vec){ 120.0f + wp->item.mailX, 8.0f + (wp->item.mailY - 56.0f), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, 230.0f, msgSearch(mail->name));
#else
            winFontSetLabel((Vec){ 120.0f + wp->item.mailX, 8.0f + (wp->item.mailY - 56.0f), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, black, mail->name);
#endif
            GXSetScissor((u32)(32.0f + (304.0f + wp->item.mailX)), (u32)(72.0f + (240.0f - wp->item.mailY)), 0x158, 0xB1);
#endif
            winTexInit((TEXPalette*)*wp->res->data);
            winTexSet(0x77, (Vec){ 4.0f + (208.0f + wp->item.mailX), (wp->item.mailY - 56.0f) - 80.0f, 0.0f }, (Vec){ 1.0f, 7.5f, 1.0f }, col);
            winFontInit();
#if VERSION <= VERSION_DEMO_JP
            winFontSetLabel((Vec){80.0f + wp->item.mailX, (wp->item.mailY - 80.0f) + (f32)(wp->item.mailLine * 21), 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, (GXColor){0x00, 0x00, 0x00, 0xFF}, mail->body);
            if (mail->flags & MAIL_FLAG_PICTURE) {
                winTexInit((TEXPalette*)*wp->mailRes->data);
                winTexSet(mail->icon, (Vec){210.0f + wp->item.mailX, (wp->item.mailY - (f32)((wp->item.mailLines - 3) * 21)) + (f32)(wp->item.mailLine * 21), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
            GXSetScissor(0, 0, 0x260, 0x1E0);
            {
#else
#if VERSION >= VERSION_US
            winFontSetWidth((Vec){ 80.0f + wp->item.mailX, (wp->item.mailY - 80.0f) + (f32)(wp->item.mailLine * 21), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, (GXColor){ 0x00, 0x00, 0x00, 0xFF }, 280.0f, msgSearch(mail->body));
#else
            winFontSetLabel((Vec){ 80.0f + wp->item.mailX, (wp->item.mailY - 80.0f) + (f32)(wp->item.mailLine * 21), 0.0f }, (Vec){ 0.75f, 0.75f, 0.75f }, (GXColor){ 0x00, 0x00, 0x00, 0xFF }, mail->body);
#endif
            if (mail->flags & MAIL_FLAG_PICTURE) {
                winTexInit((TEXPalette*)*wp->mailRes->data);
                winTexSet(mail->icon, (Vec){ 210.0f + wp->item.mailX, 7.0f + ((wp->item.mailY - (f32)((wp->item.mailLines - 1) * 21)) + (f32)(wp->item.mailLine * 21)), 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
            GXSetScissor(0, 0, 0x260, 0x1E0);
            if (wp->item.mailLines > 8) {
#endif
                winIconInit();
                if (wp->item.mailLine == 0) {
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_ARROW_DOWN, (Vec){ 360.0f + wp->item.mailX, (20.0f + (wp->item.mailY - 230.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
#if VERSION <= VERSION_DEMO_JP
                } else if (wp->item.mailLine + 8 == wp->item.mailLines) {
#else
                } else if (wp->item.mailLine + 6 == wp->item.mailLines) {
#endif
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_ARROW_UP, (Vec){ 360.0f + wp->item.mailX, 24.0f + (20.0f + (wp->item.mailY - 230.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
                } else {
                    if ((gp->retraceCount & 0x1F) < 0x14) {
                        winIconSet(ICON_ARROW_UP, (Vec){ 360.0f + wp->item.mailX, 24.0f + (20.0f + (wp->item.mailY - 230.0f)), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                        winIconSet(ICON_ARROW_DOWN, (Vec){ 360.0f + wp->item.mailX, (20.0f + (wp->item.mailY - 230.0f)) - 24.0f, 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                    }
                }
                winIconSet(ICON_SUBSTICK_NEUTRAL, (Vec){ 360.0f + wp->item.mailX, 20.0f + (wp->item.mailY - 230.0f), 0.0f }, (Vec){ 0.6f, 0.6f, 0.6f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
            }
        }
    }
}

// Descriptive name (not in the symbol maps); placeholder unk_8014d5dc, JP 0x8014D5DC. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
void winRootBgCaptureDisp(CameraId unused, void* param) {
    WindowWork* bg = param;
    GXSetTexCopySrc(0, 0, gp->fbWidth, gp->efbHeight);
    GXSetTexCopyDst(gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(bg->texBuf->address, GX_FALSE);
#if VERSION >= VERSION_DEMO_EU
    GXPixModeSync();
    DCFlushRange(bg->texBuf->address, GXGetTexBufferSize(gp->fbWidth, gp->efbHeight, GX_TF_RGB565, GX_FALSE, 0));
    GXInvalidateTexAll();
#else
    GXTexModeSync();
#endif
    bg->texBufValid = 1;
    mapDispOff();
    camDispOff(CAMERA_3D);
}
#endif

/**
 * Draws the frame captured by winRootBgCaptureDisp full screen as the menu's backdrop,
 * queued each frame by winRootMain, so the field can stay switched off while the menu is open.
 */
// Descriptive name (not in the symbol maps); placeholder unk_8014d338, JP 0x8014D338. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
void winRootBgDrawDisp(CameraId unused, void* param) {
    WindowWork* bg = param;
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
#if VERSION >= VERSION_DEMO_EU
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
#else
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
#endif
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
#if VERSION >= VERSION_DEMO_EU
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
#else
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
#endif
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
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
#endif
