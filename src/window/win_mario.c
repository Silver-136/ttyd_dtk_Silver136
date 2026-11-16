// Pause menu: the Mario window (stats, special moves, equipped gear).

#include "version.h"
#include "win/win_main.h"
#include "battle/battle_mario.h"
#include "icon_ids.h"
#include "data/item_data.h"
#include "driver/animdrv.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "pmario_sound.h"
#include <dolphin/gx.h>
#include <string.h>
#include "libc_decls.h"
#include "win/win_mario.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "battle/battle_seq_end.h"
#include "win/win_root.h"
#include "driver/mapdrv.h"

typedef struct LinkDt {
    s16 x;              // 0x0
    s16 y;              // 0x2
    const char* pose0;  // 0x4
    const char* pose1;  // 0x8
    const char* pose2;  // 0xC
    u16 item;           // 0x10
    const char* msg;    // 0x14
    u8 links[4];        // 0x18
} LinkDt;

/**
 * Entries of the pause menu's Mario screen: level, Star Rank, hammer, boots, HP, FP, BP,
 * the four curses, Star Power, special moves, Star Points, coins, Star Pieces,
 * Shine Sprites and play time. Each gives the cursor spot, Mario's poses, the item it needs,
 * its help text and the entry reached by up, down, left and right. winMarioMain moves through it.
 */
static LinkDt linkDt[18] = {
    {-268, 142, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_lv", {6, 1, 11, 9}},
    {-268, 118, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_star_r", {0, 2, 11, 7}},
    {-272, 68, "M_H_1", NULL, "R_1", ITEM_HAMMER, NULL, {1, 3, 11, 7}},
    {-242, 24, "M_R_1", NULL, "R_1", ITEM_BOOTS, NULL, {2, 4, 11, 8}},
    {-270, -34, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_hp", {3, 5, 13, 13}},
    {-270, -68, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_fp", {4, 6, 13, 13}},
    {-270, -102, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_bp", {5, 0, 13, 13}},
    {-100, 90, "M_S_1", "M_S_1", "S_1", ITEM_PAPER_CURSE, "msg_slit", {9, 10, 2, 9}},
    {-100, 20, "M_S_1", "M_S_1", "S_1", ITEM_PLANE_CURSE, "msg_plane", {10, 4, 3, 10}},
    {-50, 130, "M_S_1", "M_S_1", "S_1", ITEM_BOAT_CURSE, "msg_ship", {0, 7, 0, 11}},
    {-50, 60, "M_S_1", "M_S_1", "S_1", ITEM_TUBE_CURSE, "msg_roll", {7, 8, 8, 12}},
    {20, 106, "M_S_1", NULL, "S_1", 0, "msg_menu_star_power", {17, 12, 9, 0}},
    {20, 70, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_waza", {11, 13, 10, 0}},
    {3, 10, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_star_p", {12, 14, 4, 4}},
    {3, -18, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_coin", {13, 15, 4, 4}},
    {3, -46, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_kakera", {14, 16, 4, 4}},
    {3, -74, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_scoin", {15, 17, 4, 4}},
    {3, -102, "M_S_1", NULL, "S_1", 0, "msg_menu_mario_time", {16, 11, 4, 4}},
};
static const char* hammer_help[] = {NULL, "msg_hammer_1", "msg_hammer_2", "msg_hammer_3"};
static const char* boots_help[] = {NULL, "msg_boots_1", "msg_boots_2", "msg_boots_3"};
static const char* hammer_pose[] = {NULL, "M_H_1", "M_H_4", "M_H_7"};

/**
 * Sets up the Mario window when the pause menu opens:
 * loads Mario's model in his current outfit (emblem colors, or the 8-bit d_mario model in 8-bit form)
 * and counts the special moves (Star Powers) obtained.
 */
void winMarioInit(WindowWork* w) {
    PouchData* pouch;
    const char* name;
    s32 i;

    pouch = pouchGetPtr();
    if (w->flags & WINDOW_FLAG_8BIT) {
        name = "S_1";
        switch (marioGetColor()) {
            case 0:
                name = "S_1";
                break;
            case 1:
                name = "S_2";
                break;
            case 2:
                name = "S_3";
                break;
            case 3:
                name = "S_4";
                break;
        }
        w->mario.poseId = animPoseEntry("d_mario", 0);
        animPoseSetAnim(w->mario.poseId, name, 1);
    } else {
        name = "a_mario";
        switch (marioGetColor()) {
            case 0:
                name = "a_mario";
                break;
            case 1:
                name = "a_mario_l";
                break;
            case 2:
                name = "a_mario_w";
                break;
            case 3:
                name = "a_mario_wl";
                break;
        }
        w->mario.poseId = animPoseEntry(name, 0);
        animPoseSetAnim(w->mario.poseId, "M_S_1", 1);
    }
    animPoseSetMaterialFlagOn(w->mario.poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    if (marioGetPtr()->flags & MARIO_FLAG_IS_SHADOW_MARIO) {
        animPoseSetMaterialFlagOn(w->mario.poseId, MAP_OBJ_TEV_CALLBACK(10));
    }
#if VERSION >= VERSION_JP
    w->mario.hammerHold = 0;
#endif
    w->mario.mode = 0;
    w->mario.specialCursor = 0;
    w->mario.specialCount = 0;
    for (i = 0; i < 8; i++) {
        if (pouch->starPowersObtained & (1 << i)) {
            w->mario.specialCount++;
        }
    }
}

/**
 * Run from win_root.c each time the Mario window comes to the front:
 * cursor back to the Level entry, its help text, special-move list closed.
 */
void winMarioInit2(WindowWork* w) {
    w->marioSel = 0;

    // each target field is stored twice (both stores are in the retail code)
    w->helpTgtX = w->helpTgtX = linkDt[w->marioSel].x;
    w->helpTgtY = w->helpTgtY = linkDt[w->marioSel].y;
    switch (w->marioSel) {
        case 2:
            winMsgEntry(w, 0, hammer_help[pouchGetHammerLv()], 0);
            break;
        case 3:
            winMsgEntry(w, 0, boots_help[pouchGetJumpLv()], 0);
            break;
        default:
            winMsgEntry(w, 0, linkDt[w->marioSel].msg, 0);
            break;
    }
#if VERSION >= VERSION_JP
    w->mario.hammerHold = 0;
#endif
    w->mario.mode = 0;
    w->mario.specialCursor = 0;
}

/** Frees the Mario model loaded by winMarioInit. */
void winMarioExit(WindowWork* w) {
    animPoseRelease(w->mario.poseId);
}

#define COLOR_ANIM(var, base)            \
    if (strcmp(base, "S_1") == 0) {      \
        switch (marioGetColor()) {       \
            case 0: var = "S_1"; break;  \
            case 1: var = "S_2"; break;  \
            case 2: var = "S_3"; break;  \
            case 3: var = "S_4"; break;  \
        }                                \
    } else {                             \
        switch (marioGetColor()) {       \
            case 0: var = "R_1"; break;  \
            case 1: var = "R_15"; break; \
            case 2: var = "R_17"; break; \
            case 3: var = "R_18"; break; \
        }                                \
    }

#define MOVE_MARIO_CURSOR(n)                                       \
    w->marioSel = linkDt[w->marioSel].links[n];                    \
    p = &linkDt[w->marioSel];                                      \
    while (1) {                                                    \
        if (w->marioSel == 11 || w->marioSel == 12) {              \
            if (w->mario.specialCount != 0) break;                 \
        } else if (p->item == ITEM_NULL || pouchCheckItem(p->item) != 0) { \
            break;                                                 \
        }                                                          \
        w->marioSel = p->links[n];                                 \
        p = &linkDt[w->marioSel];                                  \
    }                                                              \
    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));

/**
 * Mario window input.
 * The D-pad moves between the entries in linkDt (skipping gear and curses Mario lacks),
 * sets the help text and poses Mario to match (hammer swing, boots, ...);
 * A on the special moves entry opens the list of Star Powers with their descriptions.
 * Returns -1 on B from the main entries, -2 on Start, else 0.
 */
s32 winMarioMain(WindowWork* w) {
    const char* anims[2];
    LinkDt* lp;
    LinkDt* p;

    switch (w->mario.mode) {
        case 0:
            if (w->buttonTrg & PAD_BUTTON_A) {
                if (w->marioSel == 12) {
                    w->mario.mode = 1;
                    w->mario.specialCursor = 0;
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
#if VERSION <= VERSION_DEMO_JP
                animPoseSetAnim(w->mario.poseId, (w->flags & WINDOW_FLAG_8BIT) ? "S_1" : "M_S_1", 0);
#else
                if (w->flags & WINDOW_FLAG_8BIT) {
                    COLOR_ANIM(anims[1], "S_1")
                } else {
                    anims[1] = "M_S_1";
                }
                animPoseSetAnim(w->mario.poseId, anims[1], 0);
#endif
                return -1;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            } else if (w->dirRep & DEMO_STICK_UP) {
                MOVE_MARIO_CURSOR(0)
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                MOVE_MARIO_CURSOR(1)
            } else if (w->dirRep & DEMO_STICK_LEFT) {
                MOVE_MARIO_CURSOR(2)
            } else if (w->dirRep & DEMO_STICK_RIGHT) {
                MOVE_MARIO_CURSOR(3)
            }
            lp = &linkDt[w->marioSel];
            w->helpTgtX = lp->x;
            w->helpTgtY = lp->y;
            switch (w->marioSel) {
                case 2:
                    if (w->flags & WINDOW_FLAG_8BIT) {
                        COLOR_ANIM(anims[0], lp->pose2)
                    } else {
                        anims[0] = hammer_pose[pouchGetHammerLv()];
                    }
                    animPoseSetAnim(w->mario.poseId, anims[0], 0);
#if VERSION >= VERSION_JP
                    if (w->mario.hammerHold == 0) {
                        if (animPoseGetLoopTimes(w->mario.poseId) >= 0.89f) {
                            w->mario.hammerHold = 30;
                        }
                    } else {
                        w->mario.hammerHold--;
                    }
                    if (w->flags & WINDOW_FLAG_8BIT) {
                        w->mario.hammerHold = 0;
                    }
#endif
                    break;
                default:
#if VERSION >= VERSION_JP
                    w->mario.hammerHold = 0;
#endif
                    if (w->flags & WINDOW_FLAG_8BIT) {
                        COLOR_ANIM(anims[0], lp->pose2)
                    } else {
                        anims[0] = lp->pose0;
                    }
                    animPoseSetAnim(w->mario.poseId, anims[0], 0);
                    break;
            }
            switch (w->marioSel) {
                case 2:
                    winMsgEntry(w, 0, hammer_help[pouchGetHammerLv()], 0);
                    break;
                case 3:
                    winMsgEntry(w, 0, boots_help[pouchGetJumpLv()], 0);
                    break;
                default:
                    winMsgEntry(w, 0, lp->msg, 0);
                    break;
            }
            break;
        case 1:
            if (w->dirRep & DEMO_STICK_UP) {
                w->mario.specialCursor--;
                if (w->mario.specialCursor < 0) {
                    w->mario.specialCursor = w->mario.specialCount - 1;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                w->mario.specialCursor++;
                if (w->mario.specialCursor >= w->mario.specialCount) {
                    w->mario.specialCursor = 0;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                w->mario.mode = 0;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            w->helpTgtX = -280.0f;
            w->helpTgtY = 10.0f + (120.0f - 26.0f * w->mario.specialCursor - 12.0f);
            winMsgEntry(w, 0, superActionTable[w->mario.specialCursor]->description, 0);
            break;
    }
    return 0;
}

/** Per-frame update hook for the Mario window; empty. */
void winMarioMain2(void) {
}

/** US and later use ASCII digits; JP and the early demos use full-width digits. */
const char* winZenkakuStr(s32 n) {
#if VERSION <= VERSION_DEMO_JP
    static char tmp2[0x100];
    const char* tbl[10] = {"０", "１", "２", "３", "４",
                           "５", "６", "７", "８", "９"};
    char buf[0x100];
    u32 i;
    sprintf(buf, "%d", n);
    strcpy(tmp2, "");
    for (i = 0; i < strlen(buf); i++) {
        strcat(tmp2, tbl[(s8)buf[i] - '0']);
    }
#elif VERSION >= VERSION_US
    static char tmp2[0x100];
    sprintf(tmp2, "%d", n);
#else
    const char* tbl[10] = {"０", "１", "２", "３", "４",
                           "５", "６", "７", "８", "９"};
    static char tmp2[0x100];
    char buf[0x100];
    u32 i;
    if (n < 0) {
        strcpy(tmp2, "－");
        n = -n;
    } else {
        strcpy(tmp2, "");
    }
    sprintf(buf, "%d", n);
    for (i = 0; i < strlen(buf); i++) {
        // the digit character is sign-extended before the index (the extsb is in the retail code)
        strcat(tmp2, tbl[(s8)buf[i] - '0']);
    }
#endif
    return tmp2;
}

/** Formats n zero-padded to digits places (the play time's hours and minutes); returns a shared static buffer. */
#if VERSION >= VERSION_US
char winZenkakuStrPad_buf[0x100];

// Descriptive name (not in the symbol maps); placeholder unk_8016af5c, JP 0x8016AF5C. See docs/NAME_PROVENANCE.md.
char* winZenkakuStrPad(s32 n, s32 digits) {
    char buf[0x100];
    sprintf(buf, "%%0%dd", digits);
    sprintf(winZenkakuStrPad_buf, buf, n);
    return winZenkakuStrPad_buf;
}
#else
#if VERSION >= VERSION_JP
char winZenkakuStrPad_tmp[0x100];
#endif

// Descriptive name (not in the symbol maps); placeholder unk_8016af5c, JP 0x8016AF5C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
char* winZenkakuStrPad(s32 n, s32 digits) {
    const char* tbl[10] = {"０", "１", "２", "３", "４",
                           "５", "６", "７", "８", "９"};
    char buf[0x100];
    s32 i;
    if (n < 0) {
        strcpy(winZenkakuStrPad_tmp, "－");
        n = -n;
    } else {
        strcpy(winZenkakuStrPad_tmp, "");
    }
    sprintf(buf, "%d", n);
    for (i = 0; i < digits; i++) {
        if ((u32)i < (u32)(digits - strlen(buf))) {
            strcat(winZenkakuStrPad_tmp, "０");
        } else {
            // the digit character is sign-extended before the index (the extsb is in the retail code)
            strcat(winZenkakuStrPad_tmp, tbl[(s8)buf[i - (digits - strlen(buf))] - '0']);
        }
    }
    return winZenkakuStrPad_tmp;
}
#endif
#endif

/** Draws a speech-bubble (fukidashi) with a curse's icon in it, plus a drop shadow. */
static inline void fukiKey(WindowWork* w, u16 icon, f32 x, f32 y) {
    winTexInit(*w->res->data);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    winTexSet(0xB1, (Vec){x + 5.0f, y - 5.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0x80});
    winTexSet(0xB1, (Vec){x, y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0xE1, 0xE1, 0xE1, 0xFE});
    winIconInit();
    winIconSet(icon, (Vec){x + 6.0f, y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

/** Mirrored speech bubble for the hammer or boots icon (the bubble texture flipped horizontally). */
static inline void fukiTool(WindowWork* w, u16 icon, f32 x, f32 y) {
    winTexInit(*w->res->data);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    winTexSet(0xB1, (Vec){x + 5.0f, y - 5.0f, 0.0f}, (Vec){-0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0x80});
    winTexSet(0xB1, (Vec){x, y, 0.0f}, (Vec){-0.8f, 0.8f, 0.8f}, (GXColor){0xE1, 0xE1, 0xE1, 0xFE});
    winIconInit();
    winIconSet(icon, (Vec){x - 6.0f, y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

/**
 * Draws the bubble for Mario window entry idx if Mario owns it:
 * current hammer (2), current boots (3), or the Paper, Plane, Boat and Tube curses (7-10).
 */
static void fukidashi(WindowWork* w, u32 idx, f32 x, f32 y) {
    switch (idx) {
        case 2:
            if (pouchGetHammerLv() > 0) {
                fukiTool(w, itemDataTable[pouchGetHammerLv() + (ITEM_HAMMER - 1)].icon_id, x - 230.0f, y + 72.0f);
            }
            break;
        case 3:
            if (pouchGetJumpLv() > 0) {
                fukiTool(w, itemDataTable[pouchGetJumpLv() + (ITEM_BOOTS - 1)].icon_id, x - 210.0f, y + 20.0f);
            }
            break;
        case 7:
            if (pouchCheckItem(ITEM_PAPER_CURSE)) {
                fukiKey(w, itemDataTable[ITEM_PAPER_CURSE].icon_id, x - 80.0f, y + 90.0f);
            }
            break;
        case 8:
            if (pouchCheckItem(ITEM_PLANE_CURSE)) {
                fukiKey(w, itemDataTable[ITEM_PLANE_CURSE].icon_id, x - 80.0f, y + 20.0f);
            }
            break;
        case 9:
            if (pouchCheckItem(ITEM_BOAT_CURSE)) {
                fukiKey(w, itemDataTable[ITEM_BOAT_CURSE].icon_id, x - 30.0f, y + 130.0f);
            }
            break;
        case 10:
            if (pouchCheckItem(ITEM_TUBE_CURSE)) {
                fukiKey(w, itemDataTable[ITEM_TUBE_CURSE].icon_id, x - 30.0f, y + 60.0f);
            }
            break;
    }
}

/**
 * Row spacing of the Mario window (both copies of winMarioDisp). Where a row offset is added as a float
 * (34.0f, 68.0f; 28.0f ... 112.0f) the code writes the multiples out.
 */
enum MarioWindowLayout {
    MARIO_STAT_ROW_H = 34,  // HP row to FP row to BP row
    MARIO_ORB_W = 22,       // one Star Power orb to the next
    MARIO_COUNT_ROW_H = 28, // one counter row (Star Points, coins, Star Pieces, Shine Sprites, play time)
    MARIO_MOVE_ROW_H = 26,  // one row of the special-move list (winMarioMain moves the hand cursor by 26 too)
};

#if VERSION >= VERSION_US
/**
 * Draws the Mario window (menu slot idx): name, level and rank, the gear and curse bubbles, HP/FP/BP,
 * the Star Power gauge, Star Points, coins, Star Pieces, Shine Sprites and play time (capped at 99:59),
 * Mario's model, and the special-move list when open.
 *
 * (x, y) is the window's position (w->menu[idx]); screen coordinates have the origin at the centre and y
 * growing upwards. Panels and plates are given by their top-left corner, textures and icons by their
 * centre, text by its start. Left half: name plate, rank medal, HP/FP/BP panel, Mario's model; right half:
 * the Star Power panel and the counters panel. The pre-US copy below draws the same layout.
 */
void winMarioDisp(s32 camId, void* param, s32 idx) {
    static s32 pri[6] = {3, 2, 8, 10, 7, 9};
    WindowWork* w = param;
    PouchData* pouch = pouchGetPtr();
    GXColor white;
    GXColor black;
    f32 cx;
    f32 nameW;
    char buf[0x100];
    s32 i;
    f32 iconX;
    f32 apX;
    f32 left;
    u64 secs;
    s32 hour;
    s32 min;
    AnimData* anim;
    Mtx mtx;
    f32 x;
    f32 y;

    white = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    black = (GXColor){0x00, 0x00, 0x00, 0xFF};
    x = w->menu[idx].x;
    y = w->menu[idx].y;
    winBgGX(w, 0, x, y);
    // Name plate (190 wide, top-left (x - 265, y + 154)): "Mario" in white outlined text at 0.8 scale, a Mario
    // icon 20 pixels after it, then "Lvl." (at most 45 pixels wide) 20 pixels further on
    winNameGX(w, 2, x - 265.0f, 154.0f + y, 190.0f, 24.0f);
    cx = 10.0f + (x - 265.0f);
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("name_mario"));
    cx += 0.8f * FontGetMessageWidth(msgSearch("name_mario"));
    cx += 20.0f;
    winIconInit();
    winIconSet(ICON_MARIO, (Vec){cx, 142.0f + y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    cx += 20.0f;
    winFontInit();
    winFontSetEdgeWidth((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, 45.0f,
                        msgSearch("msg_menu_mario_level"));
    // the level number near the plate's right end: it starts 10 + its unscaled width (at most 20) pixels
    // before the end and is squeezed to at most 20 pixels wide
    sprintf(buf, "%s", winZenkakuStr(pouch->level));
    nameW = FontGetMessageWidth(buf);
    if (nameW > 20.0f) {
        nameW = 20.0f;
    }
    winFontInit();
    winFontSetEdgeWidth((Vec){(190.0f + (x - 265.0f)) - nameW - 10.0f, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                        white, 20.0f, buf);
    // Star Rank: the medal (texture 0xB0) at (x - 255, y + 112), the rank's name to its right (at most 140 wide)
    winTexInit(*w->res->data);
    winTexSet(0xB0, (Vec){x - 255.0f, 112.0f + y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSetWidth((Vec){x - 240.0f, 124.0f + y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black, 140.0f,
                    msgSearch(BattleGetRankNameLabel(pouch->level)));

    // the gear and curse bubbles, the one under the cursor drawn last so it sits on top
    for (i = 0; i < 6; i++) {
        if (w->marioSel != pri[i]) {
            fukidashi(w, pri[i], x, y);
        }
    }
    fukidashi(w, w->marioSel, x, y);

    // HP/FP/BP panel (264 x 110, top-left (x - 270, y - 10)). Row i's text line is (y - 38) - i * MARIO_STAT_ROW_H.
    // Left column: a coloured plate, texture 0xAF (an oval) used only as an alpha mask (TEV colour = C0, the
    // colour passed to winTexSet; alpha 0xFE makes winTexSet blend): HP pink and FP yellow as in win_item.c,
    // BP light blue. On it the stat's name ("HP", "FP", "BP" in the game's language) centred on x - 220, and
    // above that its ruby ("Heart Pts." and so on) at half size in dark red, green or blue.
    winKirinukiGX(w, 0, x - 270.0f, y - 10.0f, 264.0f, 110.0f);
    {
        f32 nameW2;
        f32 rubyW;

        for (i = 0; i < 3; i++) {
            GXColor cols[3] = {{0xCB, 0x8C, 0xA2, 0xFE}, {0xCB, 0xBE, 0x89, 0xFE}, {0x94, 0xB9, 0xC7, 0xFE}};
            GXColor reds[3] = {{0xA7, 0x3C, 0x3C, 0xFF}, {0x37, 0x69, 0x1E, 0xFF}, {0x0F, 0x21, 0x7A, 0xFF}};
            // Descriptive name (not in the symbol maps).
            const char* const stat_name_tbl[7][3] = {
                {"ＨＰ", "ＦＰ", "ＢＰ"},
                {"HP", "FP", "BP"},
                {"KP", "BP", "OP"},
                {"PC", "PF", "PB"},
                {"PC", "PF", "PM"},
                {"PV", "PF", "PT"},
                {"HP", "FP", "BP"},
            };
            const char* ruby[3] = {"msg_menu_mario_ruby_hp", "msg_menu_mario_ruby_fp", "msg_menu_mario_ruby_bp"};

            nameW2 = FontGetMessageWidth(stat_name_tbl[gp->language][i]);
            rubyW = 0.5f * FontGetMessageWidth(msgSearch(ruby[i]));
            winTexInit(*w->res->data);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            winTexSet(0xAF, (Vec){(50.0f + (x - 270.0f)) - 2.0f, ((y - 38.0f) - i * MARIO_STAT_ROW_H) - 4.0f, 0.0f},
                      (Vec){1.0f, 1.0f, 1.0f}, cols[i]);
            winFontInit();
            winFontSet((Vec){(50.0f + (x - 270.0f)) - nameW2 / 2.0f, 12.0f + ((y - 38.0f) - i * MARIO_STAT_ROW_H), 0.0f},
                       (Vec){1.0f, 1.0f, 1.0f}, black, stat_name_tbl[gp->language][i]);
            winFontSet((Vec){(50.0f + (x - 270.0f)) - rubyW / 2.0f, 21.0f + ((y - 38.0f) - i * MARIO_STAT_ROW_H), 0.0f},
                       (Vec){0.5f, 0.5f, 0.5f}, reds[i], msgSearch(ruby[i]));
        }
    }

    // Right of the plates: the HP, FP and BP icons at x - 160; current HP, current FP and maximum BP
    // right-aligned to the width of "999" (winFontSetR) from x - 140; for HP and FP a "/" (texture 0x10)
    // centred at x - 80 and the maximum from x - 70. BP shows only the maximum.
    winIconInit();
    iconX = 110.0f + (x - 270.0f);
    winIconSet(ICON_HP, (Vec){iconX, 4.0f + (y - 38.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winIconSet(ICON_FP, (Vec){iconX, 4.0f + ((y - 38.0f) - 34.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winIconSet(ICON_BP, (Vec){iconX, 4.0f + ((y - 38.0f) - 68.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winFontInit();
    winFontSetR((Vec){130.0f + (x - 270.0f), 4.0f + (12.0f + (y - 38.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
                "%s", winZenkakuStr(pouch->currentHP));
    winFontSetR((Vec){130.0f + (x - 270.0f), 4.0f + (12.0f + ((y - 38.0f) - 34.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                black, "%s", winZenkakuStr(pouch->currentFP));
    winFontSetR((Vec){130.0f + (x - 270.0f), 4.0f + (12.0f + ((y - 38.0f) - 68.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                black, "%s", winZenkakuStr(pouch->maximumBP));
    winTexInit(*w->res->data);
    winTexSet(0x10, (Vec){190.0f + (x - 270.0f), 4.0f + (y - 38.0f), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winTexSet(0x10, (Vec){190.0f + (x - 270.0f), 4.0f + ((y - 38.0f) - 34.0f), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSet((Vec){200.0f + (x - 270.0f), 4.0f + (12.0f + (y - 38.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
               "%s", winZenkakuStr(pouch->maximumHP));
    winFontSet((Vec){200.0f + (x - 270.0f), 4.0f + (12.0f + ((y - 38.0f) - 34.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
               black, "%s", winZenkakuStr(pouch->maximumFP));

    // Star Power panel (260 x 80, top-left (x + 10, y + 130)), only once Mario has a special move. Top row
    // (y + 106, the Star Power entry): ICON_MENU_SP at x + 50, then one orb per special move from x + 90,
    // MARIO_ORB_W apart; orb i is lit (its own icon ICON_SP_ORB_1 + i) when the Star Power holds at least
    // i + 1 full units of 100, else empty. Second row (y + 70, the special moves entry): a special-move icon
    // (ICON_SAC_DEKA) and "Specials" (at most 175 wide).
    if (w->mario.specialCount > 0) {
        f32 spY;

        winKirinukiGX(w, 0, 10.0f + x, (20.0f + (130.0f + y)) - 20.0f, 260.0f, 80.0f);
        winIconInit();
        iconX = 10.0f + (40.0f + x);
        winIconSet(ICON_MENU_SP, (Vec){iconX, 106.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        spY = 70.0f + y;
        winIconSet(ICON_SAC_DEKA, (Vec){iconX, spY, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        winFontInit();
        winFontSetWidth((Vec){10.0f + (70.0f + x), 10.0f + spY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, 175.0f,
                        msgSearch("msg_menu_mario_special"));
        winIconInit();
        for (i = 0; i < w->mario.specialCount; i++) {
            if (i < pouch->currentSP / 100) {
                winIconSet(ICON_SP_ORB_1 + i, (Vec){(10.0f + (80.0f + x)) + i * MARIO_ORB_W, 106.0f + y, 0.0f},
                           (Vec){0.8f, 0.8f, 0.8f}, white);
            } else {
                winIconSet(ICON_SP_ORB_EMPTY, (Vec){(10.0f + (80.0f + x)) + i * MARIO_ORB_W, 106.0f + y, 0.0f},
                           (Vec){0.8f, 0.8f, 0.8f}, white);
            }
        }
    }

    // Counters panel (260 x 150, top-left (x + 10, y + 30)): Star Points, coins, Star Pieces, Shine Sprites and
    // play time, one row each MARIO_COUNT_ROW_H apart from y + 10 (icon centres; the text line is 12 higher).
    // Icon at x + 30, label from x + 45 (at most 140 wide, play time 125), value right-aligned to the width
    // of "999" from x + 215. The first four rows have a times sign (texture 0x11) at x + 200 between label and value;
    // play time is "hours:minutes" (texture 0x12 is the ":").
    left = 10.0f + x;
    winKirinukiGX(w, 0, left, 10.0f + (20.0f + y), 260.0f, 150.0f);
    winIconInit();
    iconX = 10.0f + (20.0f + x);
    winIconSet(ICON_STAR_POINT, (Vec){iconX, 10.0f + y, 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_COIN, (Vec){iconX, 10.0f + (y - 28.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_STAR_PIECE, (Vec){iconX, 10.0f + (y - 56.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_SHINE_SPRITE, (Vec){iconX, 10.0f + (y - 84.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_PLAY_TIME, (Vec){iconX, 10.0f + (y - 112.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winFontInit();
    winFontSetWidth((Vec){10.0f + (35.0f + x), 10.0f + (12.0f + y), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 140.0f,
                    msgSearch("msg_menu_mario_num_sp"));
    winFontSetWidth((Vec){10.0f + (35.0f + x), 10.0f + ((12.0f + y) - 28.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                    140.0f, msgSearch("msg_menu_mario_num_coin"));
    winFontSetWidth((Vec){10.0f + (35.0f + x), 10.0f + ((12.0f + y) - 56.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                    140.0f, msgSearch("msg_menu_mario_num_hoshi"));
    winFontSetWidth((Vec){10.0f + (35.0f + x), 10.0f + ((12.0f + y) - 84.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                    140.0f, msgSearch("msg_menu_mario_num_super_coin"));
    winFontSetWidth((Vec){10.0f + (35.0f + x), 10.0f + ((12.0f + y) - 112.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                    125.0f, msgSearch("msg_menu_mario_num_playtime"));
    winFontSetR((Vec){2.0f + (8.0f + (205.0f + x)), 10.0f + (12.0f + y), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                "%s", winZenkakuStr(pouch->starPoints));
    winFontSetR((Vec){2.0f + (8.0f + (205.0f + x)), 10.0f + ((12.0f + y) - 28.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                black, "%s", winZenkakuStr(pouch->coins));
    winFontSetR((Vec){2.0f + (8.0f + (205.0f + x)), 10.0f + ((12.0f + y) - 56.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                black, "%s", winZenkakuStr(pouch->starPieceCount));
    winFontSetR((Vec){2.0f + (8.0f + (205.0f + x)), 10.0f + ((12.0f + y) - 84.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                black, "%s", winZenkakuStr(pouch->shineSpriteCount));
    {
        // capped at 99:59 (from 100 hours on)
        u64 tsecs = OSTicksToSeconds(gp->playTime);
        if (tsecs > 359999) {
            hour = 99;
            min = 59;
        } else {
            secs = tsecs;
            hour = tsecs / 3600;
            secs %= 3600;
            min = secs / 60;
        }
    }
    // hours (two digits) right-aligned from x + 168, minutes (two digits) from x + 215, the ":" at x + 220
    winFontSetR((Vec){2.0f + (2.0f + ((8.0f + (172.0f + x)) - 16.0f)), 10.0f + ((12.0f + y) - 112.0f), 0.0f},
                (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(hour, 2));
    winFontSetR((Vec){2.0f + (8.0f + (205.0f + x)), 10.0f + ((12.0f + y) - 112.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f},
                black, "%s", winZenkakuStrPad(min, 2));
    winTexInit(*w->res->data);
    for (i = 0; i < 4; i++) {
        winTexSet(0x11, (Vec){10.0f + (190.0f + x), 10.0f + (y - i * MARIO_COUNT_ROW_H), 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                  white);
    }
    apX = 220.0f + x;
    winTexSet(0x12, (Vec){apX, (10.0f + (2.0f + (y - 112.0f))) - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);

    // Mario's model at (x - 150, y) on a round half-transparent shadow, drawn after a Z clear. The animation
    // time follows the frame counter, except while winMarioMain's hammerHold counts down (the hammer entry).
    animPoseGetAnimPosePtr(w->mario.poseId);
    anim = animPoseGetAnimDataPtr(w->mario.poseId);
    if (w->mario.hammerHold == 0) {
        animPoseSetLocalTime(w->mario.poseId, (f32)(gp->retraceCount % (s32)anim->loopInfo->end));
    }
    animPoseMain(w->mario.poseId);
    winKageGX(w, x - 150.0f, y, 0.0f, 1.0f, (GXColor){0x00, 0x00, 0x00, 0x80});
    PSMTXTrans(mtx, x - 150.0f, y, 0.0f);
    winZClear();
    animPoseDrawMtx(w->mario.poseId, mtx, 2, 0.0f, 5.0f);

    // The special-move list (winMarioMain mode 1, opened with A on the special moves entry): a 270 x 254 panel
    // with top-left (x - 270, 160) - its y does not follow the window - with "Moves" (white, outlined, 0.8)
    // centred by its unscaled width in the first 240 pixels and "SP" at x - 50 in the header strip. Row i
    // (text line 130 - i * MARIO_MOVE_ROW_H): the move's name from x - 250 (at most 180 wide), its cost centred
    // on x - 42 and a small "SP" at x - 29, 10 lower. The hand cursor points at (-280, 118 - 26 * cursor).
    if (w->mario.mode) {
        const char* msg = msgSearch("msg_menu_mario_sac_waza");
        char sbuf[0x20];

        winWazaGX(w, 0, x - 270.0f, 160.0f, 270.0f, 254.0f);
        winFontInit();
        winFontSetEdge((Vec){(x - 270.0f) + (240 - FontGetMessageWidth(msg)) / 2, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                       white, msg);
        winFontSetEdge((Vec){apX - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("btl_cost_disp_AP"));
        for (i = 0; i < w->mario.specialCount; i++) {
            winFontSetWidth((Vec){10.0f + (left - 270.0f), 130 - i * MARIO_MOVE_ROW_H, 0.0f}, (Vec){1.0f, 1.0f, 1.0f},
                            black, 180.0f, msgSearch(superActionTable[i]->name));
            // The star power cost (s8 baseSpCost, never negative) is read unsigned (the retail code has lbz with no extsb).
            sprintf(sbuf, "%s", winZenkakuStr((u8)superActionTable[i]->baseSpCost));
            winFontSet((Vec){((228.0f + x) - 270.0f) - FontGetMessageWidth(sbuf) / 2, 130 - i * MARIO_MOVE_ROW_H, 0.0f},
                       (Vec){1.0f, 1.0f, 1.0f}, black, sbuf);
            winFontSet((Vec){4.0f + ((17.0f + apX) - 270.0f), 120 - i * MARIO_MOVE_ROW_H, 0.0f}, (Vec){0.5f, 0.5f, 0.5f},
                       black, msgSearch("btl_cost_disp_AP"));
        }
    }
}
#else
/**
 * Pre-US winMarioDisp: the same screen without the translated stat names and width limits.
 * Draws the Mario window (menu slot idx): name, level and rank, the gear and curse bubbles, HP/FP/BP,
 * the Star Power gauge, Star Points, coins, Star Pieces, Shine Sprites and play time (capped at 99:59; the
 * early demos cap only the hours at 99), Mario's model, and the special-move list when open.
 *
 * Same layout and coordinates as the US copy above (see there), except: the level number ends 10 pixels
 * before the name plate's right end; the move list's "Moves" starts at x - 180 and its names have no width
 * limit; and the early demos print the counters, play time and move costs with "%d" in other places (noted
 * below). The f32 locals hold whatever row or column is used next, so their names are only rough.
 */
void winMarioDisp(s32 camId, void* param, s32 idx) {
    static s32 pri[6] = {3, 2, 8, 10, 7, 9};
    WindowWork* w;
    GXColor white;
    GXColor black;
    PouchData* pouch;
#if VERSION <= VERSION_DEMO_JP
    f32 left;
    f32 x;
    f32 y = 0.0f;
    f32 rowA;
    f32 rowB;
    f32 cx;
    f32 col;
    f32 row0;
    f32 row1;
    f32 row2;
    f32 nameW;
    f32 row3;
    f32 numX;
    f32 rubyW;
#else
    f32 x;
    f32 y = 0.0f;
    f32 rowA;
    f32 rowB;
    f32 cx;
    f32 col;
    f32 row0;
    f32 row1;
    f32 row2;
    f32 nameW;
    f32 left;
    f32 numX;
    f32 row3;
    f32 rubyW;
#endif
    s32 i;
    s64 secs;
    u64 tsecs;
    s32 hour;
    s32 min;
    AnimData* anim;
    Mtx mtx;
    char buf[0x100];

    w = param;
    pouch = pouchGetPtr();
    white = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    black = (GXColor){0x00, 0x00, 0x00, 0xFF};
    x = w->menu[idx].x;
    y = w->menu[idx].y;
    winBgGX(w, 0, x, y);
    // name plate: "Mario", a Mario icon, "Lvl." and the level number (ending 10 pixels before the plate's end)
    winNameGX(w, 2, x - 265.0f, 154.0f + y, 190.0f, 24.0f);
    cx = 10.0f + (x - 265.0f);
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("name_mario"));
    cx += 0.8f * FontGetMessageWidth(msgSearch("name_mario"));
    cx += 20.0f;
    winIconInit();
    winIconSet(ICON_MARIO, (Vec){cx, 142.0f + y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    cx += 20.0f;
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("msg_menu_mario_level"));
    sprintf(buf, "%s", winZenkakuStr(pouch->level));
    winFontInit();
    winFontSetEdge((Vec){(190.0f + (x - 265.0f)) - 0.8f * FontGetMessageWidth(buf) - 10.0f, 152.0f + y, 0.0f},
                   (Vec){0.8f, 0.8f, 0.8f}, white, buf);
    // Star Rank: the medal (texture 0xB0) and the rank's name
    winTexInit(*w->res->data);
    winTexSet(0xB0, (Vec){x - 255.0f, 112.0f + y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSet((Vec){x - 240.0f, 124.0f + y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black,
               msgSearch(BattleGetRankNameLabel(pouch->level)));

    // the gear and curse bubbles, the one under the cursor drawn last so it sits on top
    for (i = 0; i < 6; i++) {
        if (w->marioSel != pri[i]) {
            fukidashi(w, pri[i], x, y);
        }
    }
    fukidashi(w, w->marioSel, x, y);

    // HP/FP/BP panel: per row the coloured plate (texture 0xAF as an alpha mask), the stat's name (always
    // the full-width "ＨＰ", "ＦＰ", "ＢＰ") and its ruby above it at half size
    winKirinukiGX(w, 0, x - 270.0f, y - 10.0f, 264.0f, 110.0f);
    col = 50.0f + (x - 270.0f);
    for (i = 0; i < 3; i++) {
        GXColor cols[3] = {{0xCB, 0x8C, 0xA2, 0xFE}, {0xCB, 0xBE, 0x89, 0xFE}, {0x94, 0xB9, 0xC7, 0xFE}};
        GXColor reds[3] = {{0xA7, 0x3C, 0x3C, 0xFF}, {0x37, 0x69, 0x1E, 0xFF}, {0x0F, 0x21, 0x7A, 0xFF}};
        const char* const stat_name_tbl[3] = {"ＨＰ", "ＦＰ", "ＢＰ"};
        const char* ruby[3] = {"msg_menu_mario_ruby_hp", "msg_menu_mario_ruby_fp", "msg_menu_mario_ruby_bp"};

        rowA = 50.0f + (x - 270.0f) - 2.0f;
        nameW = FontGetMessageWidth(stat_name_tbl[i]);
        rubyW = 0.5f * FontGetMessageWidth(msgSearch(ruby[i]));
        winTexInit(*w->res->data);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        winTexSet(0xAF, (Vec){rowA, (cx = (y - 38.0f) - (f32)(i * MARIO_STAT_ROW_H)) - 4.0f, 0.0f},
                  (Vec){1.0f, 1.0f, 1.0f}, cols[i]);
        winFontInit();
        winFontSet((Vec){col - nameW / 2.0f, 12.0f + cx, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, stat_name_tbl[i]);
        winFontSet((Vec){col - rubyW / 2.0f, 21.0f + cx, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, reds[i], msgSearch(ruby[i]));
    }

    // the stat icons and values: current / maximum for HP and FP ("/" is texture 0x10), the maximum for BP
    winIconInit();
    col = 110.0f + (x - 270.0f);
    row0 = 4.0f + (y - 38.0f);
    winIconSet(ICON_HP, (Vec){col, row0, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    rowB = (y - 38.0f) - 34.0f;
    row1 = 4.0f + rowB;
    winIconSet(ICON_FP, (Vec){col, row1, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    rowA = (y - 38.0f) - 68.0f;
    winIconSet(ICON_BP, (Vec){col, 4.0f + rowA, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winFontInit();
    col = 130.0f + (x - 270.0f);
    row2 = 4.0f + (12.0f + (y - 38.0f));
    winFontSetR((Vec){col, row2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentHP));
    rowB = 4.0f + (12.0f + rowB);
    winFontSetR((Vec){col, rowB, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentFP));
    winFontSetR((Vec){col, 4.0f + (12.0f + rowA), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s",
                winZenkakuStr(pouch->maximumBP));
    winTexInit(*w->res->data);
    col = 190.0f + (x - 270.0f);
    winTexSet(0x10, (Vec){col, row0, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winTexSet(0x10, (Vec){col, row1, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    col = 200.0f + (x - 270.0f);
    winFontSet((Vec){col, row2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumHP));
    winFontSet((Vec){col, rowB, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumFP));

    // Star Power panel: ICON_MENU_SP and one orb per special move (lit for each full 100 Star Power),
    // then a special-move icon and "Specials"
    if (w->mario.specialCount > 0) {
        winKirinukiGX(w, 0, 10.0f + x, (20.0f + (130.0f + y)) - 20.0f, 260.0f, 80.0f);
        winIconInit();
        col = 10.0f + (40.0f + x);
        rowB = 106.0f + y;
        winIconSet(ICON_MENU_SP, (Vec){col, rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        cx = 70.0f + y;
        winIconSet(ICON_SAC_DEKA, (Vec){col, cx, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        winFontInit();
        winFontSet((Vec){10.0f + (70.0f + x), 10.0f + cx, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
                   msgSearch("msg_menu_mario_special"));
        winIconInit();
        cx = 10.0f + (80.0f + x);
        for (i = 0; i < w->mario.specialCount; i++) {
            if (i < pouch->currentSP / 100) {
                winIconSet(ICON_SP_ORB_1 + i, (Vec){cx + (f32)(i * MARIO_ORB_W), rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                           white);
            } else {
                winIconSet(ICON_SP_ORB_EMPTY, (Vec){cx + (f32)(i * MARIO_ORB_W), rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f},
                           white);
            }
        }
    }

    // Counters panel: Star Points, coins, Star Pieces, Shine Sprites and play time (icon, label, value); the
    // first four rows have the times sign (texture 0x11), play time the ":" (texture 0x12); then Mario himself
    left = 10.0f + x;
    winKirinukiGX(w, 0, left, 10.0f + (20.0f + y), 260.0f, 150.0f);
    winIconInit();
#if VERSION <= VERSION_DEMO_JP
    rowB = 10.0f + (20.0f + x);
    winIconSet(ICON_STAR_POINT, (Vec){rowB, 10.0f + y, 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_COIN, (Vec){rowB, 10.0f + (y - 28.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_STAR_PIECE, (Vec){rowB, 10.0f + (y - 56.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_SHINE_SPRITE, (Vec){rowB, 10.0f + (y - 84.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_PLAY_TIME, (Vec){rowB, 10.0f + (y - 112.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winFontInit();
    col = 10.0f + (35.0f + x);
    rowA = 12.0f + y;
    row0 = 10.0f + rowA;
    winFontSet((Vec){col, row0, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_sp"));
    row1 = 10.0f + (rowA - 28.0f);
    winFontSet((Vec){col, row1, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_coin"));
    row2 = 10.0f + (rowA - 56.0f);
    winFontSet((Vec){col, row2, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_hoshi"));
    row3 = 10.0f + (rowA - 84.0f);
    winFontSet((Vec){col, 10.0f + (rowA - 84.0f), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
               msgSearch("msg_menu_mario_num_super_coin"));
    rowA = 10.0f + (rowA - 112.0f);
    winFontSet((Vec){col, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_playtime"));
    // the values with "%d", right-aligned to the width of "９９９" from x + 213
    numX = 8.0f + (205.0f + x);
    winFontSetR((Vec){numX, row0, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouch->starPoints);
    winFontSetR((Vec){numX, row1, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouch->coins);
    winFontSetR((Vec){numX, row2, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouch->starPieceCount);
    winFontSetR((Vec){numX, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%d", pouch->shineSpriteCount);

    // play time: the hours capped at 99 (minutes keep counting), hours with "%d" from x + 162, minutes with
    // "%02d" from x + 216, the ":" at (x + 224, y - 100)
    secs = gp->playTime / OS_TIMER_CLOCK;
    tsecs = (u64)secs / 3600;
    min = (u64)secs % 3600 / 60;
    if (tsecs > 99) {
        tsecs = 99;
    }
    hour = tsecs;
    winFontSetR((Vec){(8.0f + (170.0f + x)) - 16.0f, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%d", hour);
    winFontSetR((Vec){(8.0f + (224.0f + x)) - 16.0f, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%02d", min);
    winTexInit(*w->res->data);
    cx = 10.0f + (190.0f + x);
    for (i = 0; i < 4; i++) {
        winTexSet(0x11, (Vec){cx, 10.0f + (y - (f32)(i * MARIO_COUNT_ROW_H)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    rowB = 220.0f + x;
    winTexSet(0x12, (Vec){4.0f + rowB, 10.0f + (2.0f + (y - 112.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    animPoseGetAnimPosePtr(w->mario.poseId);
    anim = animPoseGetAnimDataPtr(w->mario.poseId);
    animPoseSetLocalTime(w->mario.poseId, (f32)(gp->retraceCount % (s32)anim->loopInfo->end));
#else
    // JP: as in US (values with winZenkakuStr from x + 215, play time capped at 99:59, hours zero-padded)
    rowB = 10.0f + (20.0f + x);
    winIconSet(ICON_STAR_POINT, (Vec){rowB, 10.0f + y, 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_COIN, (Vec){rowB, 10.0f + (y - 28.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_STAR_PIECE, (Vec){rowB, 10.0f + (y - 56.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_ITEM_SHINE_SPRITE, (Vec){rowB, 10.0f + (y - 84.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_PLAY_TIME, (Vec){rowB, 10.0f + (y - 112.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winFontInit();
    col = 10.0f + (35.0f + x);
    rowA = 12.0f + y;
    row0 = 10.0f + rowA;
    winFontSet((Vec){col, row0, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_sp"));
    row1 = 10.0f + (rowA - 28.0f);
    winFontSet((Vec){col, row1, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_coin"));
    row2 = 10.0f + (rowA - 56.0f);
    winFontSet((Vec){col, row2, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_hoshi"));
    row3 = 10.0f + (rowA - 84.0f);
    winFontSet((Vec){col, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_super_coin"));
    rowA = 10.0f + (rowA - 112.0f);
    winFontSet((Vec){col, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, msgSearch("msg_menu_mario_num_playtime"));
    numX = 2.0f + (8.0f + (205.0f + x));
    winFontSetR((Vec){numX, row0, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->starPoints));
    winFontSetR((Vec){numX, row1, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->coins));
    winFontSetR((Vec){numX, row2, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->starPieceCount));
    winFontSetR((Vec){numX, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s",
                winZenkakuStr(pouch->shineSpriteCount));

    tsecs = OSTicksToSeconds(gp->playTime);
    if (tsecs > 359999) {
        hour = 99;
        min = 59;
    } else {
        secs = tsecs;
        hour = (u64)secs / 3600;
        min = (u64)secs % 3600 / 60;
    }
    winFontSetR((Vec){2.0f + (2.0f + ((8.0f + (172.0f + x)) - 16.0f)), rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black,
                "%s", winZenkakuStrPad(hour, 2));
    winFontSetR((Vec){numX, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(min, 2));

    winTexInit(*w->res->data);
    cx = 10.0f + (190.0f + x);
    for (i = 0; i < 4; i++) {
        winTexSet(0x11, (Vec){cx, 10.0f + (y - (f32)(i * MARIO_COUNT_ROW_H)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    rowB = 220.0f + x;
    winTexSet(0x12, (Vec){rowB, (10.0f + (2.0f + (y - 112.0f))) - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);

    // the model's animation time (JP: held while hammerHold counts down, as in US)
    animPoseGetAnimPosePtr(w->mario.poseId);
    anim = animPoseGetAnimDataPtr(w->mario.poseId);
    if (w->mario.hammerHold == 0) {
        animPoseSetLocalTime(w->mario.poseId, (f32)(gp->retraceCount % (s32)anim->loopInfo->end));
    }
#endif
    // Mario's model on its shadow, as in US
    animPoseMain(w->mario.poseId);
    winKageGX(w, x - 150.0f, y, 0.0f, 1.0f, (GXColor){0x00, 0x00, 0x00, 0x80});
    PSMTXTrans(mtx, x - 150.0f, y, 0.0f);
    winZClear();
    animPoseDrawMtx(w->mario.poseId, mtx, 2, 0.0f, 5.0f);

    // the special-move list: the panel and header as in US ("Moves" from x - 180, "ＳＰ"), then one row per move
    if (w->mario.mode) {
        // The star power cost (s8 baseSpCost, never negative) is read unsigned (the retail code has lbz with no extsb).
#if VERSION <= VERSION_DEMO_JP
        winWazaGX(w, 0, x - 270.0f, 160.0f, 270.0f, 254.0f);
        winFontInit();
        winFontSetEdge((Vec){(90.0f + x) - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white,
                       msgSearch("msg_menu_mario_sac_waza"));
        // rows: the name from x - 260, the cost with "%d" from x - 50 (under the header's "ＳＰ"), the small
        // "ＳＰ" at x - 33, 10 lower
        numX = rowB - 270.0f;
        winFontSetEdge((Vec){numX, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, "ＳＰ");
        x = left - 270.0f;
        cx = (17.0f + rowB) - 270.0f;
        for (i = 0; i < w->mario.specialCount; i++) {
            char sbuf[0x20];

            winFontSetLabel((Vec){x, (f32)(130 - i * MARIO_MOVE_ROW_H), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
                            superActionTable[i]->name);
            sprintf(sbuf, "%d", (u8)superActionTable[i]->baseSpCost);
            winFontSet((Vec){numX, (f32)(130 - i * MARIO_MOVE_ROW_H), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, sbuf);
            winFontSet((Vec){cx, (f32)(120 - i * MARIO_MOVE_ROW_H), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, "ＳＰ");
        }
#else
        winWazaGX(w, 0, x - 270.0f, 160.0f, 270.0f, 254.0f);
        winFontInit();
        winFontSetEdge((Vec){(90.0f + x) - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white,
                       msgSearch("msg_menu_mario_sac_waza"));
        winFontSetEdge((Vec){rowB - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, "ＳＰ");
        // rows as in US: the name from x - 250, the cost centred on x - 42, the small "ＳＰ" at x - 29, 10 lower
        y = 10.0f + (left - 270.0f);
        x = (228.0f + x) - 270.0f;
        cx = 4.0f + ((17.0f + rowB) - 270.0f);
        for (i = 0; i < w->mario.specialCount; i++) {
            char sbuf[0x20];

            winFontSetLabel((Vec){y, (f32)(130 - i * MARIO_MOVE_ROW_H), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black,
                            superActionTable[i]->name);
            sprintf(sbuf, "%s", winZenkakuStr((u8)superActionTable[i]->baseSpCost));
            winFontSet((Vec){x - (f32)(FontGetMessageWidth(sbuf) / 2), (f32)(130 - i * MARIO_MOVE_ROW_H), 0.0f},
                       (Vec){1.0f, 1.0f, 1.0f}, black, sbuf);
            winFontSet((Vec){cx, (f32)(120 - i * MARIO_MOVE_ROW_H), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, "ＳＰ");
        }
#endif
    }
}

#endif
