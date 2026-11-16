// Pause menu: the badge window (equip and unequip badges).

#include "version.h"
#include "win/win_main.h"
#include "data/item_data.h"
#include "driver/animdrv.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "mario/mario_pouch.h"
#include <dolphin/gx.h>
#include "mario/mariost.h"
#include "icon_ids.h"
#include "libc_decls.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "win/win_mario.h"
#include "driver/mapdrv.h"

/** Window texture ids of the BP pips: lit for available (or, in the list, equipped) points, dark for used ones. */
typedef enum WinBpTex {
    WIN_TEX_BP_LIT = 0x1C,
    WIN_TEX_BP_DARK = 0x1D
} WinBpTex;

/** Builds the "Equipped" tab's list from the pouch: one entry (slot index, badge id, equipped) per equipped badge. */
void winMakeEquipList(WindowWork* w) {
    s32 n = 0;
    s32 i;
    PouchData* pouch = pouchGetPtr();

    for (i = 0; i < MAX_BADGES; i++) {
        if (pouch->equippedBadges[i] != 0) {
            w->badge.list[n].equipped = 1;
            w->badge.list[n].idx = i;
            w->badge.list[n].id = pouch->equippedBadges[i];
            n++;
        }
    }
    w->badge.count = n;
}

/** Resets the Badges window when the pause menu opens: "All Badges" tab, cursors and pages at the top. */
void winBadgeInit(WindowWork* w) {
    w->badge.state = 0;
    w->badge.tab = 0;
    w->badge.sel[0] = 0;
    w->badge.page[0] = 0;
    w->badge.sel[1] = 0;
    w->badge.page[1] = 0;
    winMakeEquipList(w);
}

/**
 * Run from win_root.c each time the Badges window comes to the front:
 * snaps the list scroll to the saved pages, sets the help text for the current tab
 * and rebuilds the equipped list.
 */
void winBadgeInit2(WindowWork* w) {
    w->helpTgtX = -270.0f;
    w->helpTgtY = 125 - w->badge.tab * 40;
    w->badge.scroll[0] = w->badge.scrollTgt[0] = 184.0f * w->badge.page[0];
    w->badge.scroll[1] = w->badge.scrollTgt[1] = 184.0f * w->badge.page[1];
    winMsgEntry(w, 0, w->badge.tab == 0 ? "msg_menu_badge_all" : "msg_menu_badge_equip", 0);
    winMakeEquipList(w);
}

/** Closing hook for the Badges window; empty. */
void winBadgeExit(void) {
}

/**
 * After a badge change, reloads the menu's Mario models
 * so they show the L Emblem / W Emblem outfits (a_mario_l, a_mario_w, a_mario_wl),
 * or the matching 8-bit Mario animations when Mario is in 8-bit form.
 */
static void mario_change(WindowWork* w, s32 unused) {
    s32 hasLeft = pouchEquipCheckBadge(ITEM_L_EMBLEM);
    s32 hasRight = pouchEquipCheckBadge(ITEM_W_EMBLEM);
    const char* model;
    const char* animL;
    const char* animR;

    if (w->flags & WINDOW_FLAG_8BIT) {
        if (hasLeft && !hasRight) {
            animL = "S_2";
            animR = "W_15";
        } else if (!hasLeft && hasRight) {
            animL = "S_3";
            animR = "W_17";
        } else if (hasLeft && hasRight) {
            animL = "S_4";
            animR = "W_18";
        } else {
            animL = "S_1";
            animR = "W_1";
        }
        if (w->mario.poseId != -1) {
            animPoseRelease(w->mario.poseId);
            w->mario.poseId = animPoseEntry("d_mario", 0);
            animPoseSetAnim(w->mario.poseId, animL, 1);
            animPoseSetMaterialFlagOn(w->mario.poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        }
        if (w->log.pose != -1) {
            animPoseRelease(w->log.pose);
            w->log.pose = animPoseEntry("d_mario", 0);
            animPoseSetAnim(w->log.pose, animR, 1);
            animPoseSetMaterialFlagOn(w->log.pose, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        }
    } else {
        if (hasLeft && !hasRight) {
            model = "a_mario_l";
        } else if (!hasLeft && hasRight) {
            model = "a_mario_w";
        } else if (hasLeft && hasRight) {
            model = "a_mario_wl";
        } else {
            model = "a_mario";
        }
        if (w->mario.poseId != -1) {
            animPoseRelease(w->mario.poseId);
            w->mario.poseId = animPoseEntry(model, 0);
            animPoseSetAnim(w->mario.poseId, "M_S_1", 1);
            animPoseSetMaterialFlagOn(w->mario.poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        }
        if (w->log.pose != -1) {
            animPoseRelease(w->log.pose);
            w->log.pose = animPoseEntry(model, 0);
            animPoseSetAnim(w->log.pose, "M_W_1", 1);
            animPoseSetMaterialFlagOn(w->log.pose, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        }
    }
}

/**
 * Badges window input.
 * State 0 picks the "All Badges" / "Equipped" tab,
 * state 10 moves through the list (L/R page) and equips or removes the badge on A if BP allows,
 * state 1000 runs the Z sort menu. Returns -1 on B from the tab selection, -2 on Start (close the menu), else 0.
 */
s32 winBadgeMain(WindowWork* w) {
    PouchData* pouch = pouchGetPtr();
    s32 tab;
    s32 cnt;
    s32 pages;
    s32 itemId;

    switch (w->badge.state) {
        case 0:
            if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                return -1;
            }
            if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            if (w->dirRep & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                w->badge.tab = 1 - w->badge.tab;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
                if (w->badge.tab) {
                    winMakeEquipList(w);
                }
            } else if (w->buttonTrg & PAD_BUTTON_A) {
                if (w->badge.tab == 0 && !pouchGetHaveBadgeCnt()) {
                    break;
                }
                if (w->badge.tab == 1 && w->badge.count == 0) {
                    break;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->badge.state = 10;
                break;
            } else if (w->buttonTrg & PAD_TRIGGER_Z) {
                winSortEntry(w, w->badge.tab == 0 ? WIN_SORT_BADGES : WIN_SORT_EQUIPPED_BADGES, -310.0f, 150.0f);
                w->savedState = w->badge.state;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->badge.state = 1000;
            }
            w->helpTgtX = -270.0f;
            w->helpTgtY = VERSION_PICK(125, 130) - w->badge.tab * VERSION_PICK(40, 45);
            winMsgEntry(w, 0, w->badge.tab == 0 ? "msg_menu_badge_all" : "msg_menu_badge_equip", 0);
            break;
        case 10:
            if (w->badge.tab == 0) {
                cnt = pouchGetHaveBadgeCnt();
            } else {
                cnt = w->badge.count;
            }
            pages = (cnt + 7) / 8;
            tab = w->badge.tab;
            if (w->buttonTrg & PAD_BUTTON_A) {
                if (tab == 0) {
                    s32 idx = w->badge.sel[tab];
                    if (pouchEquipCheckBadgeIndex(idx)) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_BADGE_REMOVE1 | PSND_SFX_BYPASS_MUTE));
                        pouchUnEquipBadgeIndex(idx);
                    } else if (pouch->availableBP >= itemDataTable[pouchHaveBadge(idx)].bp_cost) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_BADGE_PUT1 | PSND_SFX_BYPASS_MUTE));
                        pouchEquipBadgeIndex(idx);
                    } else {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                    }
                    pouchReviseMarioParam();
                    pouchRevisePartyParam();
                    mario_change(w, pouchHaveBadge(idx));
                } else {
                    // FAKE: the selection is read through w advanced by tab words (sel[0] of the shifted work); w->badge.sel[tab] swaps r27/r28, an s32* &w->badge.sel[tab] folds 0x3E4 into an addi + lwzx
                    WindowWork* wt = (WindowWork*)((s32*)w + tab);
                    s32 idx = w->badge.list[wt->badge.sel[0]].idx;
                    if (w->badge.list[wt->badge.sel[0]].equipped) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_BADGE_REMOVE1 | PSND_SFX_BYPASS_MUTE));
                        pouchUnEquipBadgeIndex(idx);
                        w->badge.list[wt->badge.sel[0]].equipped = 0;
                    } else if (pouch->availableBP >= itemDataTable[pouchHaveBadge(idx)].bp_cost) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_BADGE_PUT1 | PSND_SFX_BYPASS_MUTE));
                        pouchEquipBadgeIndex(idx);
                        w->badge.list[wt->badge.sel[0]].equipped = 1;
                    }
                    pouchReviseMarioParam();
                    pouchRevisePartyParam();
                    mario_change(w, pouchHaveBadge(idx));
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                w->badge.state = 0;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            } else if (w->buttonRep & PAD_TRIGGER_L) {
                w->badge.page[tab]--;
                if (w->badge.page[tab] < 0) {
                    w->badge.page[tab] = 0;
                }
                w->badge.sel[tab] = w->badge.page[tab] * 8;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonRep & PAD_TRIGGER_R) {
                w->badge.page[tab]++;
                if (w->badge.page[tab] >= pages) {
                    w->badge.page[tab] = pages - 1;
                }
                w->badge.sel[tab] = w->badge.page[tab] * 8;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_UP) {
                w->badge.sel[tab]--;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                w->badge.sel[tab]++;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_TRIGGER_Z) {
                winSortEntry(w, tab == 0 ? WIN_SORT_BADGES : WIN_SORT_EQUIPPED_BADGES, -310.0f, 150.0f);
                w->savedState = w->badge.state;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->badge.state = 1000;
            }
            if (w->badge.sel[tab] < 0) {
                w->badge.sel[tab] = cnt - 1;
            }
            if (w->badge.sel[tab] >= cnt) {
                w->badge.sel[tab] = 0;
            }
            w->badge.page[tab] = w->badge.sel[tab] / 8;
            w->helpTgtX = -110.0f;
            w->helpTgtY = 110 - (w->badge.sel[tab] % 8) * 23;
            if (w->badge.tab == 0) {
                itemId = pouchHaveBadge(w->badge.sel[tab]);
            } else {
                itemId = w->badge.list[w->badge.sel[tab]].id;
            }
            winMsgEntry(w, itemId, itemDataTable[itemId].menu_description, 0);
            break;
        case 1000:
            if (!winSortWait(w)) {
                w->badge.state = w->savedState;
            }
            if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            break;
    }
    return 0;
}

/**
 * Every frame: eases each tab's list scroll a quarter of the way toward its current page (184 units per 8-row
 * page).
 */
void winBadgeMain2(WindowWork* w) {
    s32 i;
    for (i = 0; i < 2; i++) {
        w->badge.scrollTgt[i] = 184.0f * w->badge.page[i];
        w->badge.scroll[i] += (w->badge.scrollTgt[i] - w->badge.scroll[i]) / 4.0f;
    }
}

static void badge_disp(WindowWork* w, f32 xo, f32 yo);

/**
 * BP pips (textures WIN_TEX_BP_LIT / WIN_TEX_BP_DARK): in the BP panel one per BP point, ten to a row; in the
 * badge list one per point of a badge's cost. The BP panel is 28 + rows * BP_PIP_STEP high, where rows =
 * (maximumBP + 9) / BP_PIPS_PER_ROW is the number of pip rows Mario's maximum BP needs; written out it is
 * 158 - (BP_PIP_ROWS - rows) * BP_PIP_STEP, the full ten-row height (28 + 10 * 13 = 158) less the rows not needed.
 */
enum BadgeBpLayout {
    BP_PIP_STEP = 13,     // pip to pip, across and down
    BP_PIPS_PER_ROW = 10, // pips in one row of the BP panel
    BP_PIP_ROWS = 10,     // pip rows of the full-height BP panel
};

/**
 * Draws the Badges window (menu slot n): the two tabs,
 * the BP panel with total and remaining BP and a pip per BP point, then the badge list via badge_disp.
 * (x, y) is the window's position (w->menu[n]); screen coordinates have the origin at the centre and y growing
 * upwards. As in win_item.c, most y values here are written "20.0f + (...)", a 20-pixel lift added around the
 * rest; where it comes from is not known.
 */
void winBadgeDisp(s32 camId, void* param, s32 n) {
    WindowWork* w = param;
    GXColor white = {255, 255, 255, 255};
    GXColor black = {0, 0, 0, 255};
    s32 i;
    s32 off;
    PouchData* pouch = pouchGetPtr();
    f32 c;
    f32 b;
    f32 a;
    f32 yy;
    f32 d;
    f32 x = w->menu[n].x;
    f32 y = w->menu[n].y;
#if VERSION >= VERSION_US
    f32 tw;
#endif

    winBgGX(w, 3, x, y);
    // the inset list panel: top-left corner (x - 100, y + 140), 365 x 210
    winKirinukiGX(w, 0, x - 100.0f, 20.0f + (120.0f + y), 365.0f, 210.0f);
    // The two tabs (i = 0 All Badges, 1 Equipped) left of the panel, centred at x - 190, the first at y + 125
    // (JP) or y + 130 (US) and 40 (JP) or 45 (US) pixels apart. Texture 0xB4 is the tab shape; 0x1A + i the
    // tab's label, cut to that shape (winTexSet_x2). The open tab draws a half-transparent black copy of the shape
    // as a shadow, then the label in white 4 pixels up and to the left of it; the other tab draws its label in
    // gray. winBadgeMain state 0 points the hand cursor at the same y (helpTgtY).
    a = 105.0f + y;
    c = (x - 190.0f) - 4.0f;
    b = 4.0f + a;
    for (i = 0; i < 2; i++) {
        off = i * VERSION_PICK(40, 45);
        if (i == w->badge.tab) {
            winTexInit(*w->res->data);
            winTexSet(0xB4, (Vec){x - 190.0f, VERSION_PICK(20.0f, 25.0f) + (a - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                      (GXColor){0, 0, 0, 0x80});
            winTexInit_x2(*w->res->data);
            winTexSet_x2(i + 0x1A, 0xB4, (Vec){c, VERSION_PICK(20.0f, 25.0f) + (b - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                         white);
        } else {
            winTexInit_x2(*w->res->data);
            winTexSet_x2(i + 0x1A, 0xB4, (Vec){x - 190.0f, VERSION_PICK(20.0f, 25.0f) + (a - off), 0.0f},
                         (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x80, 0x80, 0x80, 0xFF});
        }
    }
    // The BP panel under the tabs: top-left corner (x - 260, y + 60), 150 wide, as high as the pip rows need
    // (BadgeBpLayout). Type 1 adds the tab under its bottom-right edge that holds the "remaining" label
    // (msg_menu_badge_nokori) and the remaining BP; their y follows the panel's bottom edge, BP_PIP_STEP per
    // pip row. Inside the panel: "BP" (scale 0.8) at x - 220 with Mario's maximum BP after it.
#if VERSION <= VERSION_DEMO_JP
    winKirinukiGX(w, 1, x - 260.0f, yy = 20.0f + (40.0f + y), 150.0f,
                  158 - (BP_PIP_ROWS - ((pouch->maximumBP + 9) / BP_PIPS_PER_ROW)) * BP_PIP_STEP);
    winFontInit();
    winFontSet((Vec){x - 220.0f, yy = yy - 4.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "ＢＰ");
    // early demos: the numbers are written left-aligned with "%d"
    winFontSet((Vec){x - 175.0f, yy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "%d", pouch->maximumBP);
    winFontSet((Vec){x - 180.0f,
                     ((20.0f + (y - 108.0f)) - 18.0f) +
                         (BP_PIP_ROWS - ((pouch->maximumBP + 9) / BP_PIPS_PER_ROW)) * BP_PIP_STEP,
                     0.0f},
               (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("msg_menu_badge_nokori"));
    winFontSet((Vec){x - 145.0f,
                     ((20.0f + (y - 102.0f)) - 18.0f) +
                         (BP_PIP_ROWS - ((pouch->maximumBP + 9) / BP_PIPS_PER_ROW)) * BP_PIP_STEP,
                     0.0f},
               (Vec){0.8f, 0.8f, 0.8f}, black, "%d", pouch->availableBP);
#else
    winKirinukiGX(w, 1, x - 260.0f, yy = 20.0f + (40.0f + y), 150.0f,
                  158 - (BP_PIP_ROWS - (pouch->maximumBP + 9) / BP_PIPS_PER_ROW) * BP_PIP_STEP);
    winFontInit();
#if VERSION >= VERSION_US
    {

        // US and later: BP label per language (indexed by gp->language)
        const char* bpLabel[7] = {"ＢＰ", "BP", "OP", "PB", "PM", "PT", "BP"};
        winFontSet((Vec){x - 220.0f, yy = yy - 4.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, bpLabel[gp->language]);
    }
#else
    winFontSet((Vec){x - 220.0f, yy = yy - 4.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "ＢＰ");
#endif
    // JP and later: the numbers are right-aligned (winFontSetR) and written by winZenkakuStr (full-width digits
    // in JP, ASCII in US and later); the maximum BP ends at x - 193
    winFontSetR((Vec){(x - 175.0f) - 18.0f, yy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "%s",
                winZenkakuStr(pouch->maximumBP));
#if VERSION >= VERSION_US
    // The "remaining" label starts 20 pixels into the tab under the panel (the tab is tw wide and ends at the
    // panel's right edge, as winKirinukiGX sizes it; 16 is its corner piece). JP's fixed x - 180 is the same
    // place for JP's fixed 90-pixel tab.
    tw = FontGetMessageWidth(msgSearch("msg_menu_badge_nokori")) * 0.5f + 60.0f;
    winFontSet((Vec){4.0f + (16.0f + ((x - 260.0f) + (150.0f - tw))),
                     ((20.0f + (y - 108.0f)) - 18.0f) +
                         (BP_PIP_ROWS - (pouch->maximumBP + 9) / BP_PIPS_PER_ROW) * BP_PIP_STEP,
                     0.0f},
               (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("msg_menu_badge_nokori"));
#else
    winFontSet((Vec){x - 180.0f,
                     ((20.0f + (y - 108.0f)) - 18.0f) +
                         (BP_PIP_ROWS - (pouch->maximumBP + 9) / BP_PIPS_PER_ROW) * BP_PIP_STEP,
                     0.0f},
               (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("msg_menu_badge_nokori"));
#endif
    // the remaining BP, ending at x - 161 (JP) or x - 157 (US and later)
    winFontSetR((Vec){VERSION_PICK((x - 145.0f) - 16.0f, 4.0f + ((x - 145.0f) - 16.0f)),
                      ((20.0f + (y - 102.0f)) - 18.0f) +
                          (BP_PIP_ROWS - (pouch->maximumBP + 9) / BP_PIPS_PER_ROW) * BP_PIP_STEP,
                      0.0f},
                (Vec){0.8f, 0.8f, 0.8f}, black, "%s", winZenkakuStr(pouch->availableBP));
#endif
    // One pip per BP point, BP_PIPS_PER_ROW to a row: pip i at (x - 242, y + 30) plus (i % 10) steps right and
    // (i / 10) steps down. The available BP are lit, the rest up to the maximum (used by equipped badges) dark.
    winTexInit(*w->res->data);
    d = 28.0f + y;
    for (i = 0; i < pouch->availableBP; i++) {
        winTexSet(WIN_TEX_BP_LIT,
                  (Vec){(x - 242.0f) + (i % BP_PIPS_PER_ROW) * BP_PIP_STEP,
                        (20.0f + (d - (i / BP_PIPS_PER_ROW) * BP_PIP_STEP)) - 18.0f,
                        0.0f},
                  (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    for (i = pouch->availableBP; i < pouch->maximumBP; i++) {
        winTexSet(WIN_TEX_BP_DARK,
                  (Vec){(x - 242.0f) + (i % BP_PIPS_PER_ROW) * BP_PIP_STEP,
                        (20.0f + (d - (i / BP_PIPS_PER_ROW) * BP_PIP_STEP)) - 18.0f,
                        0.0f},
                  (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    badge_disp(w, x, y);
}

/**
 * The badge list: one column, eight rows per page. winBadgeMain2 scrolls by 184.0f (8 * BADGE_LIST_ROW_H) per
 * page; winBadgeMain state 10 moves the hand cursor on the same rows (helpTgtX = -110, helpTgtY = 110 - row * 23).
 */
enum BadgeListLayout {
    BADGE_LIST_ROW_H = 23, // one row
    BADGE_LIST_PAGE = 8,   // badges per page
};

/**
 * Draws the scrolling badge list for the current tab:
 * equipped highlight, icon, name, BP cost and cost pips, plus the page arrows and the Z sort hint.
 * JP and later gray out badges Mario cannot afford.
 *
 * Row i (badge i of the tab's list) has its line at y = yo + 110 - i * BADGE_LIST_ROW_H, scrolled up by
 * w->badge.scroll[tab] (unlike win_item.c's rows, without the 20-pixel lift): the equipped highlight centred
 * at xo + 82, the icon at (xo - 75, y) (scale 0.54), then 12 higher the name at xo - 55 and the BP cost (early
 * demos at xo + 130, later around xo + 142), and the cost pips from xo + 160 at the icon's height, one
 * BP_PIP_STEP apart. The "BP needed" heading (msg_menu_badge_hituyou) sits over the cost column at
 * (xo + 120, yo + 135). A scissor box clips the list to screen lines 119 to 302 (of 480), one page of rows.
 */
static void badge_disp(WindowWork* w, f32 xo, f32 yo) {
    ItemData* item;
    s8* cost;
    s32 j;
#if VERSION <= VERSION_DEMO_JP
    GXColor colA;
#else
    GXColor colA = {255, 255, 255, 255};
#endif
    GXColor colB;
    GXColor colU;
    GXColor colD;
    char buf[64];
    s32 cnt;
    s32 i;
    s32 badge;
    s32 equipped;
    s32 gray;
    f32 scroll;
    f32 width;
#if VERSION <= VERSION_DEMO_JP
    colA = (GXColor){255, 255, 255, 255};
    colB = (GXColor){0, 0, 0, 255};
    colU = colA;
    colD = colB;
#endif
    if (w->badge.tab == 0) {
        cnt = pouchGetHaveBadgeCnt();
    } else {
        cnt = w->badge.count;
    }
    GXSetScissor(0, 0x77, 0x260, 0xB8);
    for (i = 0; i < cnt; i++) {
        if (w->badge.tab == 0) {
            badge = pouchHaveBadge(i);
        } else {
            badge = w->badge.list[i].id;
        }
        scroll = w->badge.scroll[w->badge.tab];
        if (w->badge.tab == 0) {
            equipped = pouchEquipCheckBadgeIndex(i);
        } else {
            equipped = w->badge.list[i].equipped;
        }
        // rows above the screen are skipped; the first row below it ends the list
        if ((scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))) > 240.0f) {
            continue;
        }
        if ((scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))) < -240.0f) {
            break;
        }
        // equipped: texture 0x1E stretched 44 times wide, centred at xo + 82, drawn first so it lies behind the row
        if (equipped) {
            winTexInit(*w->res->data);
#if VERSION <= VERSION_DEMO_JP
            winTexSet(0x1E, (Vec){82.0f + xo, (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                      (Vec){44.0f, 1.0f, 1.0f}, colA);
        }
        winIconInit();
        winIconSet(itemDataTable[badge].icon_id,
                   (Vec){xo - 75.0f, (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                   (Vec){0.54f, 0.54f, 0.54f}, colA);
        item = &itemDataTable[badge];
        winFontInit();
        // early demos: the name is item->id (the Japanese name) as it is, the cost a "%d" at xo + 130
        winFontSet((Vec){xo - 55.0f, scroll + (12.0f + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                   (Vec){0.9f, 0.9f, 0.9f}, colB, item->id);
        cost = &item->bp_cost;
        winFontSet((Vec){130.0f + xo, scroll + (12.0f + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                   (Vec){0.9f, 0.9f, 0.9f}, colB, "%d", *cost);
#else
            winTexSet(0x1E, (Vec){82.0f + xo, (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                      (Vec){44.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
        // a badge that is not equipped and costs more than the BP left: gray icon and gray (0x7F) text
        if (!equipped && itemDataTable[badge].bp_cost > pouchGetPtr()->availableBP) {
            colB = (GXColor){0x7F, 0x7F, 0x7F, 0xFF};
            gray = 1;
        } else {
            colB = (GXColor){0, 0, 0, 0xFF};
            gray = 0;
        }
        if (!gray) {
            winIconInit();
        } else {
            winIconGrayInit();
        }
        winIconSet(itemDataTable[badge].icon_id,
                   (Vec){xo - 75.0f, (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                   (Vec){0.54f, 0.54f, 0.54f}, colA);
        item = &itemDataTable[badge];
        winFontInit();
#if VERSION >= VERSION_US
        // the name, squeezed to at most 170 pixels wide
        winFontSetWidth((Vec){xo - 55.0f, scroll + (12.0f + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                        (Vec){0.9f, 0.9f, 0.9f}, colB, 170.0f, msgSearch(item->name));
#else
        // JP: the name is item->id (the Japanese name) as it is
        winFontSet((Vec){xo - 55.0f, scroll + (12.0f + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))), 0.0f},
                   (Vec){0.9f, 0.9f, 0.9f}, colB, item->id);
#endif
        // the cost (winZenkakuStr: full-width digits in JP, ASCII in US and later), starting half its unscaled
        // width left of xo + 142
        cost = &item->bp_cost;
        sprintf(buf, "%s", winZenkakuStr(*cost));
        winFontSet((Vec){(142.0f + xo) - (f32)(FontGetMessageWidth(buf) / 2),
                         scroll + (12.0f + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))),
                         0.0f},
                   (Vec){0.9f, 0.9f, 0.9f}, colB, buf);
#endif
        // one pip per point of the cost, lit when the badge is equipped
        winTexInit(*w->res->data);
        for (j = 0; j < *cost; j++) {
#if VERSION <= VERSION_DEMO_JP
            winTexSet(equipped ? WIN_TEX_BP_LIT : WIN_TEX_BP_DARK,
                      (Vec){(160.0f + xo) + j * BP_PIP_STEP,
                            (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))),
                            0.0f},
                      (Vec){1.0f, 1.0f, 1.0f}, colA);
#else
            winTexSet(equipped ? WIN_TEX_BP_LIT : WIN_TEX_BP_DARK,
                      (Vec){(160.0f + xo) + j * BP_PIP_STEP,
                            (scroll + ((yo + 110.0f) - (f32)(i * BADGE_LIST_ROW_H))),
                            0.0f},
                      (Vec){1.0f, 1.0f, 1.0f}, colA);
#endif
        }
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);
    {
        f32 arrowX;
#if VERSION >= VERSION_JP
        colU = (GXColor){255, 255, 255, 255};
        colD = (GXColor){0, 0, 0, 255};
#endif

        // Page arrows right of the panel at xo + 250 (texture 0x17; scale -1 turns it to point up): up arrow at
        // yo + 148 with the L button icon under it when there is a previous page, down arrow at yo - 85 with the
        // R button icon above it when there is a next page (the same places as in win_item.c)
        if (w->badge.page[w->badge.tab] > 0) {
            winTexInit(*w->res->data);
            arrowX = 250.0f + xo;
            winTexSet(0x17, (Vec){arrowX, 20.0f + (18.0f + (110.0f + yo)), 0.0f}, (Vec){-1.0f, -1.0f, -1.0f}, colU);
            winIconInit();
            winIconSet(ICON_TRIGGER_L, (Vec){arrowX, 20.0f + (113.0f + yo), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, colU);
        }
        if (cnt > w->badge.page[w->badge.tab] * BADGE_LIST_PAGE + BADGE_LIST_PAGE) {
            winTexInit(*w->res->data);
            arrowX = 250.0f + xo;
            winTexSet(0x17, (Vec){arrowX, 20.0f + ((yo - 90.0f) - 15.0f), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, colU);
            winIconInit();
            winIconSet(ICON_TRIGGER_R, (Vec){arrowX, 20.0f + (yo - 90.0f), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, colU);
        }
        // Sort hint under the panel: the "sort" text (scale 0.7) ends at xo + 220, the Z button icon 30 pixels
        // left of it (width holds the text's width, then its x)
        width = 0.7f * (f32)FontGetMessageWidth(msgSearch("msg_menu_sort_narabikae"));
        winFontInit();
        width = xo + (220.0f - width);
        winFontSet((Vec){width, 20.0f + ((yo - 92.0f) - 10.0f), 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, colD,
                   msgSearch("msg_menu_sort_narabikae"));
        winIconInit();
        winIconSet(ICON_TRIGGER_Z_ALT, (Vec){width - 30.0f, 20.0f + ((yo - 100.0f) - 10.0f), 0.0f},
                   (Vec){0.6f, 0.6f, 0.6f}, colU);
        // the cost column's "BP needed" heading (scale 0.5)
        winFontInit();
        winFontSet((Vec){120.0f + xo, 135.0f + yo, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, colD,
                   msgSearch("msg_menu_badge_hituyou"));
    }
}
