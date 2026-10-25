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

extern ItemData itemDataTable[];
void winIconInit(void);
void winIconSet(u32 id, Vec translate, Vec scale, GXColor color);

typedef struct LinkDt {
    s16 x;              // 0x0
    s16 y;              // 0x2
    const char* pose0;  // 0x4
    const char* pose1;  // 0x8
    const char* pose2;  // 0xC
    u16 item;           // 0x10
    u16 f12;            // 0x12
    const char* msg;    // 0x14
    u8 links[4];        // 0x18
} LinkDt;


LinkDt linkDt[18] = {
    {-268, 142, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_lv"), {6, 1, 11, 9}},
    {-268, 118, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_star_r"), {0, 2, 11, 7}},
    {-272, 68, ("M_H_1"), NULL, ("R_1"), 9, 0, NULL, {1, 3, 11, 7}},
    {-242, 24, ("M_R_1"), NULL, ("R_1"), 6, 0, NULL, {2, 4, 11, 8}},
    {-270, -34, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_hp"), {3, 5, 13, 13}},
    {-270, -68, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_fp"), {4, 6, 13, 13}},
    {-270, -102, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_bp"), {5, 0, 13, 13}},
    {-100, 90, ("M_S_1"), ("M_S_1"), ("S_1"), 2, 0, ("msg_slit"), {9, 10, 2, 9}},
    {-100, 20, ("M_S_1"), ("M_S_1"), ("S_1"), 4, 0, ("msg_plane"), {10, 4, 3, 10}},
    {-50, 130, ("M_S_1"), ("M_S_1"), ("S_1"), 5, 0, ("msg_ship"), {0, 7, 0, 11}},
    {-50, 60, ("M_S_1"), ("M_S_1"), ("S_1"), 3, 0, ("msg_roll"), {7, 8, 8, 12}},
    {20, 106, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_star_power"), {17, 12, 9, 0}},
    {20, 70, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_waza"), {11, 13, 10, 0}},
    {3, 10, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_star_p"), {12, 14, 4, 4}},
    {3, -18, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_coin"), {13, 15, 4, 4}},
    {3, -46, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_kakera"), {14, 16, 4, 4}},
    {3, -74, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_scoin"), {15, 17, 4, 4}},
    {3, -102, ("M_S_1"), NULL, ("S_1"), 0, 0, ("msg_menu_mario_time"), {16, 11, 4, 4}},
};
const char* hammer_help[] = {NULL, ("msg_hammer_1"), ("msg_hammer_2"), ("msg_hammer_3")};
const char* boots_help[] = {NULL, ("msg_boots_1"), ("msg_boots_2"), ("msg_boots_3")};
const char* hammer_pose[] = {NULL, ("M_H_1"), ("M_H_4"), ("M_H_7")};

void winMarioInit(WindowWork* w) {
    PouchData* pouch;
    const char* name;
    s32 i;

    pouch = pouchGetPtr();
    if (w->flags & 0x2000) {
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
            case 0: name = "a_mario"; break;
            case 1: name = "a_mario_l"; break;
            case 2: name = "a_mario_w"; break;
            case 3: name = "a_mario_wl"; break;
        }
        w->mario.poseId = animPoseEntry(name, 0);
        animPoseSetAnim(w->mario.poseId, "M_S_1", 1);
    }
    animPoseSetMaterialFlagOn(w->mario.poseId, 0x1800);
    if (marioGetPtr()->flags & MARIO_FLAG_IS_SHADOW_MARIO) {
        animPoseSetMaterialFlagOn(w->mario.poseId, 0x0A000000);
    }
    w->mario.f18C = 0;
    w->mario.f190 = 0;
    w->mario.f194 = 0;
    w->mario.f198 = 0;
    for (i = 0; i < 8; i++) {
        if (pouch->mStarPowersObtained & (1 << i)) {
            w->mario.f198++;
        }
    }
}

void winMarioInit2(WindowWork* w) {
    w->marioSel = 0;
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
    w->mario.f18C = 0;
    w->mario.f190 = 0;
    w->mario.f194 = 0;
}

void winMarioExit(WindowWork* w) {
    animPoseRelease(w->mario.poseId);
}

#define COLOR_ANIM(var, base)                         \
    if (strcmp(base, "S_1") == 0) {                   \
        switch (marioGetColor()) {                    \
            case 0: var = "S_1"; break;               \
            case 1: var = "S_2"; break;               \
            case 2: var = "S_3"; break;               \
            case 3: var = "S_4"; break;               \
        }                                             \
    } else {                                          \
        switch (marioGetColor()) {                    \
            case 0: var = "R_1"; break;               \
            case 1: var = "R_15"; break;              \
            case 2: var = "R_17"; break;              \
            case 3: var = "R_18"; break;              \
        }                                             \
    }

#define NAV(n)                                                        \
    w->marioSel = linkDt[w->marioSel].links[n];                                 \
    p = &linkDt[w->marioSel];                                              \
    while (1) {                                                       \
        if (w->marioSel == 11 || w->marioSel == 12) {                           \
            if (w->mario.f198 != 0) break;                                  \
        } else if (p->item == 0 || pouchCheckItem(p->item) != 0) {    \
            break;                                                    \
        }                                                             \
        w->marioSel = p->links[n];                                         \
        p = &linkDt[w->marioSel];                                          \
    }                                                                 \
    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));

s32 winMarioMain(WindowWork* w) {
    const char* anims[2];
    LinkDt* lp;
    LinkDt* p;

    switch (w->mario.f190) {
        case 0:
            if (w->buttonTrg & PAD_BUTTON_A) {
                if (w->marioSel == 12) {
                    w->mario.f190 = 1;
                    w->mario.f194 = 0;
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                }
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                if (w->flags & 0x2000) {
                    COLOR_ANIM(anims[1], "S_1")
                } else {
                    anims[1] = "M_S_1";
                }
                animPoseSetAnim(w->mario.poseId, anims[1], 0);
                return -1;
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            } else if (w->dirRep & DEMO_STICK_UP) {
                NAV(0)
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                NAV(1)
            } else if (w->dirRep & DEMO_STICK_LEFT) {
                NAV(2)
            } else if (w->dirRep & DEMO_STICK_RIGHT) {
                NAV(3)
            }
            lp = &linkDt[w->marioSel];
            w->helpTgtX = lp->x;
            w->helpTgtY = lp->y;
            switch (w->marioSel) {
            case 2:
                if (w->flags & 0x2000) {
                    COLOR_ANIM(anims[0], lp->pose2)
                } else {
                    anims[0] = hammer_pose[pouchGetHammerLv()];
                }
                animPoseSetAnim(w->mario.poseId, anims[0], 0);
                if (w->mario.f18C == 0) {
                    if (animPoseGetLoopTimes(w->mario.poseId) >= 0.89f) {
                        w->mario.f18C = 30;
                    }
                } else {
                    w->mario.f18C--;
                }
                if (w->flags & 0x2000) {
                    w->mario.f18C = 0;
                }
                break;
            default:
                w->mario.f18C = 0;
                if (w->flags & 0x2000) {
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
                w->mario.f194--;
                if (w->mario.f194 < 0) {
                    w->mario.f194 = w->mario.f198 - 1;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->dirRep & DEMO_STICK_DOWN) {
                w->mario.f194++;
                if (w->mario.f194 >= w->mario.f198) {
                    w->mario.f194 = 0;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_BUTTON_B) {
                w->mario.f190 = 0;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            } else if (w->buttonTrg & PAD_BUTTON_START) {
                return -2;
            }
            w->helpTgtX = -280.0f;
            w->helpTgtY = 10.0f + (120.0f - 26.0f * w->mario.f194 - 12.0f);
            winMsgEntry(w, 0, superActionTable[w->mario.f194]->description, 0);
            break;
    }
    return 0;
}

void winMarioMain2(void) {
}

#ifdef VERSION_G8ME01
// US: plain ASCII digits
const char* winZenkakuStr(s32 n) {
    static char tmp2[0x100];
    sprintf(tmp2, "%d", n);
    return tmp2;
}

char winZenkakuStrPad_buf[0x100];

char* winZenkakuStrPad(s32 n, s32 digits) {
    char buf[0x100];
    sprintf(buf, "%%0%dd", digits);
    sprintf(winZenkakuStrPad_buf, buf, n);
    return winZenkakuStrPad_buf;
}

#else
const char* winZenkakuStr(s32 n) {
    const char* tbl[10] = {"\x82\x4f", "\x82\x50", "\x82\x51", "\x82\x52", "\x82\x53",
                           "\x82\x54", "\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58"};
    static char tmp2[0x100];
    char buf[0x100];
    u32 i;
    if (n < 0) {
        strcpy(tmp2, "\x81\x7c");
        n = -n;
    } else {
        strcpy(tmp2, "");
    }
    sprintf(buf, "%d", n);
    for (i = 0; i < strlen(buf); i++) {
        strcat(tmp2, tbl[(s8)buf[i] - '0']);
    }
    return tmp2;
}

char winZenkakuStrPad_tmp[0x100];

char* winZenkakuStrPad(s32 n, s32 digits) {
    const char* tbl[10] = {"\x82\x4f", "\x82\x50", "\x82\x51", "\x82\x52", "\x82\x53",
                           "\x82\x54", "\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58"};
    char buf[0x100];
    s32 i;
    if (n < 0) {
        strcpy(winZenkakuStrPad_tmp, "\x81\x7c");
        n = -n;
    } else {
        strcpy(winZenkakuStrPad_tmp, "");
    }
    sprintf(buf, "%d", n);
    for (i = 0; i < digits; i++) {
        if ((u32)i < (u32)(digits - strlen(buf))) {
            strcat(winZenkakuStrPad_tmp, "\x82\x4f");
        } else {
            strcat(winZenkakuStrPad_tmp, tbl[(s8)buf[i - (digits - strlen(buf))] - '0']);
        }
    }
    return winZenkakuStrPad_tmp;
}
#endif

static inline void fukiKey(WindowWork* w, u16 icon, f32 x, f32 y) {
    winTexInit(*w->res->data);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    winTexSet(0xB1, (Vec){x + 5.0f, y - 5.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0x80});
    winTexSet(0xB1, (Vec){x, y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, (GXColor){0xE1, 0xE1, 0xE1, 0xFE});
    winIconInit();
    winIconSet(icon, (Vec){x + 6.0f, y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

static inline void fukiTool(WindowWork* w, u16 icon, f32 x, f32 y) {
    winTexInit(*w->res->data);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    winTexSet(0xB1, (Vec){x + 5.0f, y - 5.0f, 0.0f}, (Vec){-0.8f, 0.8f, 0.8f}, (GXColor){0x00, 0x00, 0x00, 0x80});
    winTexSet(0xB1, (Vec){x, y, 0.0f}, (Vec){-0.8f, 0.8f, 0.8f}, (GXColor){0xE1, 0xE1, 0xE1, 0xFE});
    winIconInit();
    winIconSet(icon, (Vec){x - 6.0f, y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
}

void fukidashi(WindowWork* w, u32 idx, f32 x, f32 y) {
    switch (idx) {
        case 2:
            if (pouchGetHammerLv() > 0) {
                fukiTool(w, itemDataTable[pouchGetHammerLv() + 8].icon_id, x - 230.0f, y + 72.0f);
            }
            break;
        case 3:
            if (pouchGetJumpLv() > 0) {
                fukiTool(w, itemDataTable[pouchGetJumpLv() + 5].icon_id, x - 210.0f, y + 20.0f);
            }
            break;
        case 7:
            if (pouchCheckItem(ITEM_PAPER_CURSE)) {
                fukiKey(w, itemDataTable[2].icon_id, x - 80.0f, y + 90.0f);
            }
            break;
        case 8:
            if (pouchCheckItem(ITEM_PLANE_CURSE)) {
                fukiKey(w, itemDataTable[4].icon_id, x - 80.0f, y + 20.0f);
            }
            break;
        case 9:
            if (pouchCheckItem(ITEM_BOAT_CURSE)) {
                fukiKey(w, itemDataTable[5].icon_id, x - 30.0f, y + 130.0f);
            }
            break;
        case 10:
            if (pouchCheckItem(ITEM_TUBE_CURSE)) {
                fukiKey(w, itemDataTable[3].icon_id, x - 30.0f, y + 60.0f);
            }
            break;
    }
}

void winZClear(void);
void winFontInit(void);
void winFontSet(Vec pos, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetR(Vec pos, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetEdge(Vec pos, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetLabel(Vec pos, Vec scale, GXColor color, const char* msg);

#ifdef VERSION_G8ME01
void winFontSetWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...);
void winFontSetEdgeWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...);

// US: every float live range has its own variable (register order); rowC is the second value of rowB
void winMarioDisp(s32 camId, void* param, s32 idx) {
    f32 rowC;
    // FAKE: q1/q2 hold copies of row0/row1 for the dead guards after the f198 block (register order, no code)
    f32 q1[1];
    f32 q2[1];
    static s32 pri[6] = {3, 2, 8, 10, 7, 9};
    f32 rowL;
    f32 y2;
    f32 x2;
    f32 cx6;
    f32 cx5;
    f32 cx4;
    f32 cx3;
    f32 cx2;
    f32 nameW3;
    f32 rowB;
    f32 rowA;
    f32 row0;
    f32 row1;
    f32 row2;
    f32 nameW2;
    f32 left;
    f32 col7;
    f32 col6;
    f32 col5;
    f32 col4;
    f32 col3;
    f32 col2;
    f32 rowA3;
    f32 rowA2;
    f32 row0b;
    f32 rowB3;
    f32 rowB2;
    f32 row1b;
    f32 row2b;
    f32 x;
    f32 y;
    f32 cx;
    f32 col;
    f32 nameW;
    f32 numX;
    f32 row3;
    f32 rubyW;
    WindowWork* w;
    GXColor white;
    GXColor black;
    PouchData* pouch;
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
    winNameGX(w, 2, x - 265.0f, 154.0f + y, 190.0f, 24.0f);
    cx = 10.0f + (x - 265.0f);
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("name_mario"));
    cx += 0.8f * FontGetMessageWidth(msgSearch("name_mario"));
    cx += 20.0f;
    winIconInit();
    winIconSet(ICON_19E, (Vec){cx, 142.0f + y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    cx += 20.0f;
    winFontInit();
    winFontSetEdgeWidth((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, 45.0f, msgSearch("msg_menu_mario_level"));
    sprintf(buf, "%s", winZenkakuStr(pouch->level));
    nameW = FontGetMessageWidth(buf);
    if (nameW > 20.0f) {
        nameW = 20.0f;
    }
    winFontInit();
    winFontSetEdgeWidth((Vec){(190.0f + (x - 265.0f)) - nameW - 10.0f, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, 20.0f, buf);
    winTexInit(*w->res->data);
    winTexSet(0xB0, (Vec){x - 255.0f, 112.0f + y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSetWidth((Vec){x - 240.0f, 124.0f + y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black, 140.0f, msgSearch(BattleGetRankNameLabel(pouch->level)));

    for (i = 0; i < 6; i++) {
        if (w->marioSel != pri[i]) {
            fukidashi(w, pri[i], x, y);
        }
    }
    fukidashi(w, w->marioSel, x, y);

    winKirinukiGX(w, 0, x - 270.0f, y - 10.0f, 264.0f, 110.0f);
    col = 50.0f + (x - 270.0f);
    for (i = 0; i < 3; i++) {
        GXColor cols[3] = {{0xCB, 0x8C, 0xA2, 0xFE}, {0xCB, 0xBE, 0x89, 0xFE}, {0x94, 0xB9, 0xC7, 0xFE}};
        GXColor reds[3] = {{0xA7, 0x3C, 0x3C, 0xFF}, {0x37, 0x69, 0x1E, 0xFF}, {0x0F, 0x21, 0x7A, 0xFF}};
        const char* const N_stat_name_tbl[7][3] = {
            {"\x82\x67\x82\x6F", "\x82\x65\x82\x6F", "\x82\x61\x82\x6F"},
            {"HP", "FP", "BP"},
            {"KP", "BP", "OP"},
            {"PC", "PF", "PB"},
            {"PC", "PF", "PM"},
            {"PV", "PF", "PT"},
            {"HP", "FP", "BP"},
        };
        const char* ruby[3] = {"msg_menu_mario_ruby_hp", "msg_menu_mario_ruby_fp", "msg_menu_mario_ruby_bp"};

        rowL = 50.0f + (x - 270.0f) - 2.0f;
        nameW2 = FontGetMessageWidth(N_stat_name_tbl[gp->language][i]);
        rubyW = 0.5f * FontGetMessageWidth(msgSearch(ruby[i]));
        winTexInit(*w->res->data);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        winTexSet(0xAF, (Vec){rowL, (cx2 = (y - 38.0f) - (f32)(i * 34)) - 4.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, cols[i]);
        winFontInit();
        winFontSet((Vec){col - nameW2 / 2.0f, 12.0f + cx2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, N_stat_name_tbl[gp->language][i]);
        winFontSet((Vec){col - rubyW / 2.0f, 21.0f + cx2, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, reds[i], msgSearch(ruby[i]));
    }

    winIconInit();
    winIconSet(ICON_19F, (Vec){col2 = 110.0f + (x - 270.0f), row0 = 4.0f + (y - 38.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winIconSet(ICON_1A0, (Vec){col2, row1 = 4.0f + (rowB = (y - 38.0f) - 34.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winIconSet(ICON_1A1, (Vec){col2, 4.0f + (rowA = (y - 38.0f) - 68.0f), 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winFontInit();
    winFontSetR((Vec){col3 = 130.0f + (x - 270.0f), row2 = 4.0f + (12.0f + (y - 38.0f)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentHP));
    winFontSetR((Vec){col3, rowC = 4.0f + (12.0f + rowB), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentFP));
    winFontSetR((Vec){col3, 4.0f + (12.0f + rowA), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumBP));
    winTexInit(*w->res->data);
    col4 = 190.0f + (x - 270.0f);
    winTexSet(0x10, (Vec){col4, row0, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    // FAKE: copies read only by the dead guards below; they keep two coalesced FPR nodes live across rowC (register order)
    q1[0] = row0;
    q2[0] = row1;
    winTexSet(0x10, (Vec){col4, row1, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSet((Vec){col5 = 200.0f + (x - 270.0f), row2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumHP));
    winFontSet((Vec){col5, rowC, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumFP));

    if (w->mario.f198 > 0) {
        winKirinukiGX(w, 0, 10.0f + x, (20.0f + (130.0f + y)) - 20.0f, 260.0f, 80.0f);
        winIconInit();
        col6 = 10.0f + (40.0f + x);
        nameW3 = 106.0f + y;
        winIconSet(ICON_20F, (Vec){col6, nameW3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        cx3 = 70.0f + y;
        winIconSet(ICON_197, (Vec){col6, cx3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        winFontInit();
        winFontSetWidth((Vec){10.0f + (70.0f + x), 10.0f + cx3, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, 175.0f, msgSearch("msg_menu_mario_special"));
        winIconInit();
        cx4 = 10.0f + (80.0f + x);
        for (i = 0; i < w->mario.f198; i++) {
            if (i < pouch->currentSP / 100) {
                winIconSet(ICON_1B7 + i, (Vec){cx4 + (f32)(i * 22), nameW3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
            } else {
                winIconSet(ICON_1BF, (Vec){cx4 + (f32)(i * 22), nameW3, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
            }
        }
    }

    // FAKE: two dead int guards (gz is 0; folded after register allocation, no code): the first redefines q1/q2 so
    // they are not propagated, the second consumes them into y
    {
        s32 gz = 0;
        if (gz != 0) {
            q1[0] = -q1[0];
            q2[0] = -q2[0];
        }
    }
    {
        s32 gz = 0;
        if (gz != 0) {
            y = q1[0] + q2[0];
        }
    }
    left = 10.0f + x;
    winKirinukiGX(w, 0, left, 10.0f + (20.0f + y), 260.0f, 150.0f);
    winIconInit();
    rowB2 = 10.0f + (20.0f + x);
    winIconSet(ICON_18C, (Vec){rowB2, 10.0f + y, 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_18B, (Vec){rowB2, 10.0f + (y - 28.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_18D, (Vec){rowB2, 10.0f + (y - 56.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_102, (Vec){rowB2, 10.0f + (y - 84.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(ICON_1A2, (Vec){rowB2, 10.0f + (y - 112.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winFontInit();
    col7 = 10.0f + (35.0f + x);
    rowA2 = 12.0f + y;
    row0b = 10.0f + rowA2;
    winFontSetWidth((Vec){col7, row0b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 140.0f, msgSearch("msg_menu_mario_num_sp"));
    row1b = 10.0f + (rowA2 - 28.0f);
    winFontSetWidth((Vec){col7, row1b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 140.0f, msgSearch("msg_menu_mario_num_coin"));
    row2b = 10.0f + (rowA2 - 56.0f);
    winFontSetWidth((Vec){col7, row2b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 140.0f, msgSearch("msg_menu_mario_num_hoshi"));
    row3 = 10.0f + (rowA2 - 84.0f);
    winFontSetWidth((Vec){col7, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 140.0f, msgSearch("msg_menu_mario_num_super_coin"));
    rowA3 = 10.0f + (rowA2 - 112.0f);
    winFontSetWidth((Vec){col7, rowA3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, 125.0f, msgSearch("msg_menu_mario_num_playtime"));
    winFontSetR((Vec){numX = 2.0f + (8.0f + (205.0f + x)), row0b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mStarPoints));
    winFontSetR((Vec){numX, row1b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->coins));
    winFontSetR((Vec){numX, row2b, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mStarPieceCount));
    winFontSetR((Vec){numX, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mShineSpriteCount));

    tsecs = gp->retraceLocalTime / OS_TIMER_CLOCK;
    if (tsecs > 359999) {
        hour = 99;
        min = 59;
    } else {
        secs = tsecs;
        hour = (u64)secs / 3600;
        min = (u64)secs % 3600 / 60;
    }
    winFontSetR((Vec){2.0f + (2.0f + ((8.0f + (172.0f + x)) - 16.0f)), rowA3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(hour, 2));
    winFontSetR((Vec){numX, rowA3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(min, 2));

    winTexInit(*w->res->data);
    cx5 = 10.0f + (190.0f + x);
    for (i = 0; i < 4; i++) {
        winTexSet(0x11, (Vec){cx5, 10.0f + (y - (f32)(i * 28)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    rowB3 = 220.0f + x;
    winTexSet(0x12, (Vec){rowB3, (10.0f + (2.0f + (y - 112.0f))) - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);

    animPoseGetAnimPosePtr(w->mario.poseId);
    anim = animPoseGetAnimDataPtr(w->mario.poseId);
    if (w->mario.f18C == 0) {
        animPoseSetLocalTime(w->mario.poseId, (f32)(gp->retraceCount % (s32)anim->loopInfo[2]));
    }
    animPoseMain(w->mario.poseId);
    winKageGX(w, (GXColor){0x00, 0x00, 0x00, 0x80}, x - 150.0f, y, 0.0f, 1.0f);
    PSMTXTrans(mtx, x - 150.0f, y, 0.0f);
    winZClear();
    animPoseDrawMtx(w->mario.poseId, mtx, 2, 0.0f, 5.0f);

    if (w->mario.f190) {
        const char* msg = msgSearch("msg_menu_mario_sac_waza");
        winWazaGX(w, 0, x - 270.0f, 160.0f, 270.0f, 254.0f);
        winFontInit();
        winFontSetEdge((Vec){(x - 270.0f) + (f32)((240 - FontGetMessageWidth(msg)) / 2), 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msg);
        winFontSetEdge((Vec){rowB3 - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("btl_cost_disp_AP"));
        y2 = 10.0f + (left - 270.0f);
        x2 = (228.0f + x) - 270.0f;
        cx6 = 4.0f + ((17.0f + rowB3) - 270.0f);
        for (i = 0; i < w->mario.f198; i++) {
            char sbuf[0x20];

            winFontSetWidth((Vec){y2, (f32)(130 - i * 26), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, 180.0f, msgSearch(superActionTable[i]->name));
            // FAKE: the star power cost (s8 baseSpCost, never negative) is read unsigned (lbz with no extsb), as by the old u8 view of the move table
            sprintf(sbuf, "%s", winZenkakuStr((u8)superActionTable[i]->baseSpCost));
            winFontSet((Vec){x2 - (f32)(FontGetMessageWidth(sbuf) / 2), (f32)(130 - i * 26), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, sbuf);
            winFontSet((Vec){cx6, (f32)(120 - i * 26), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("btl_cost_disp_AP"));
        }
    }
}
#else
void winMarioDisp(s32 camId, void* param, s32 idx) {
    static s32 pri[6] = {3, 2, 8, 10, 7, 9};
    WindowWork* w;
    GXColor white;
    GXColor black;
    PouchData* pouch;
    f32 x;
    f32 y;
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
    s32 i;
    s64 secs;
    u64 tsecs;
    s32 hour;
    s32 min;
    AnimData* anim;
    Mtx mtx;
    char buf[0x100];

    w = param;
    // FAKE: dead store; makes y the first float referenced, so its later web (the f190 label x) is split before x's (register order)
    y = 0.0f;
    pouch = pouchGetPtr();
    white = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    black = (GXColor){0x00, 0x00, 0x00, 0xFF};
    x = w->menu[idx].x;
    y = w->menu[idx].y;
    winBgGX(w, 0, x, y);
    winNameGX(w, 2, x - 265.0f, 154.0f + y, 190.0f, 24.0f);
    cx = 10.0f + (x - 265.0f);
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("name_mario"));
    cx += 0.8f * FontGetMessageWidth(msgSearch("name_mario"));
    cx += 20.0f;
    winIconInit();
    winIconSet(0x19E, (Vec){cx, 142.0f + y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    cx += 20.0f;
    winFontInit();
    winFontSetEdge((Vec){cx, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("msg_menu_mario_level"));
    sprintf(buf, "%s", winZenkakuStr(pouch->level));
    winFontInit();
    winFontSetEdge((Vec){(190.0f + (x - 265.0f)) - 0.8f * FontGetMessageWidth(buf) - 10.0f, 152.0f + y, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, buf);
    winTexInit(*w->res->data);
    winTexSet(0xB0, (Vec){x - 255.0f, 112.0f + y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSet((Vec){x - 240.0f, 124.0f + y, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black, msgSearch(BattleGetRankNameLabel(pouch->level)));

    for (i = 0; i < 6; i++) {
        if (w->marioSel != pri[i]) {
            fukidashi(w, pri[i], x, y);
        }
    }
    fukidashi(w, w->marioSel, x, y);

    winKirinukiGX(w, 0, x - 270.0f, y - 10.0f, 264.0f, 110.0f);
    col = 50.0f + (x - 270.0f);
    for (i = 0; i < 3; i++) {
        GXColor cols[3] = {{0xCB, 0x8C, 0xA2, 0xFE}, {0xCB, 0xBE, 0x89, 0xFE}, {0x94, 0xB9, 0xC7, 0xFE}};
        GXColor reds[3] = {{0xA7, 0x3C, 0x3C, 0xFF}, {0x37, 0x69, 0x1E, 0xFF}, {0x0F, 0x21, 0x7A, 0xFF}};
        const char* const N_stat_name_tbl[3] = {"\x82\x67\x82\x6F", "\x82\x65\x82\x6F", "\x82\x61\x82\x6F"};
        const char* ruby[3] = {"msg_menu_mario_ruby_hp", "msg_menu_mario_ruby_fp", "msg_menu_mario_ruby_bp"};

        rowA = 50.0f + (x - 270.0f) - 2.0f;
        nameW = FontGetMessageWidth(N_stat_name_tbl[i]);
        rubyW = 0.5f * FontGetMessageWidth(msgSearch(ruby[i]));
        winTexInit(*w->res->data);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        winTexSet(0xAF, (Vec){rowA, (cx = (y - 38.0f) - (f32)(i * 34)) - 4.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, cols[i]);
        winFontInit();
        winFontSet((Vec){col - nameW / 2.0f, 12.0f + cx, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, N_stat_name_tbl[i]);
        winFontSet((Vec){col - rubyW / 2.0f, 21.0f + cx, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, reds[i], msgSearch(ruby[i]));
    }

    winIconInit();
    col = 110.0f + (x - 270.0f);
    row0 = 4.0f + (y - 38.0f);
    winIconSet(0x19F, (Vec){col, row0, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    rowB = (y - 38.0f) - 34.0f;
    row1 = 4.0f + rowB;
    winIconSet(0x1A0, (Vec){col, row1, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    rowA = (y - 38.0f) - 68.0f;
    winIconSet(0x1A1, (Vec){col, 4.0f + rowA, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    winFontInit();
    col = 130.0f + (x - 270.0f);
    row2 = 4.0f + (12.0f + (y - 38.0f));
    winFontSetR((Vec){col, row2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentHP));
    rowB = 4.0f + (12.0f + rowB);
    winFontSetR((Vec){col, rowB, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->currentFP));
    winFontSetR((Vec){col, 4.0f + (12.0f + rowA), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumBP));
    winTexInit(*w->res->data);
    col = 190.0f + (x - 270.0f);
    winTexSet(0x10, (Vec){col, row0, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winTexSet(0x10, (Vec){col, row1, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    col = 200.0f + (x - 270.0f);
    winFontSet((Vec){col, row2, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumHP));
    winFontSet((Vec){col, rowB, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, "%s", winZenkakuStr(pouch->maximumFP));

    if (w->mario.f198 > 0) {
        winKirinukiGX(w, 0, 10.0f + x, (20.0f + (130.0f + y)) - 20.0f, 260.0f, 80.0f);
        winIconInit();
        col = 10.0f + (40.0f + x);
        rowB = 106.0f + y;
        winIconSet(0x20F, (Vec){col, rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        cx = 70.0f + y;
        winIconSet(0x197, (Vec){col, cx, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        winFontInit();
        winFontSet((Vec){10.0f + (70.0f + x), 10.0f + cx, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, msgSearch("msg_menu_mario_special"));
        winIconInit();
        cx = 10.0f + (80.0f + x);
        for (i = 0; i < w->mario.f198; i++) {
            if (i < pouch->currentSP / 100) {
                winIconSet(0x1B7 + i, (Vec){cx + (f32)(i * 22), rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
            } else {
                winIconSet(0x1BF, (Vec){cx + (f32)(i * 22), rowB, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
            }
        }
    }

    left = 10.0f + x;
    winKirinukiGX(w, 0, left, 10.0f + (20.0f + y), 260.0f, 150.0f);
    winIconInit();
    rowB = 10.0f + (20.0f + x);
    winIconSet(0x18C, (Vec){rowB, 10.0f + y, 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(0x18B, (Vec){rowB, 10.0f + (y - 28.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(0x18D, (Vec){rowB, 10.0f + (y - 56.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(0x102, (Vec){rowB, 10.0f + (y - 84.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
    winIconSet(0x1A2, (Vec){rowB, 10.0f + (y - 112.0f), 0.0f}, (Vec){0.62f, 0.62f, 0.62f}, white);
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
    winFontSetR((Vec){numX, row0, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mStarPoints));
    winFontSetR((Vec){numX, row1, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->coins));
    winFontSetR((Vec){numX, row2, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mStarPieceCount));
    winFontSetR((Vec){numX, row3, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pouch->mShineSpriteCount));

    tsecs = gp->retraceLocalTime / OS_TIMER_CLOCK;
    if (tsecs > 359999) {
        hour = 99;
        min = 59;
    } else {
        secs = tsecs;
        hour = (u64)secs / 3600;
        min = (u64)secs % 3600 / 60;
    }
    winFontSetR((Vec){2.0f + (2.0f + ((8.0f + (172.0f + x)) - 16.0f)), rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(hour, 2));
    winFontSetR((Vec){numX, rowA, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStrPad(min, 2));

    winTexInit(*w->res->data);
    cx = 10.0f + (190.0f + x);
    for (i = 0; i < 4; i++) {
        winTexSet(0x11, (Vec){cx, 10.0f + (y - (f32)(i * 28)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    rowB = 220.0f + x;
    winTexSet(0x12, (Vec){rowB, (10.0f + (2.0f + (y - 112.0f))) - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);

    animPoseGetAnimPosePtr(w->mario.poseId);
    anim = animPoseGetAnimDataPtr(w->mario.poseId);
    if (w->mario.f18C == 0) {
        animPoseSetLocalTime(w->mario.poseId, (f32)(gp->retraceCount % (s32)anim->loopInfo[2]));
    }
    animPoseMain(w->mario.poseId);
    winKageGX(w, (GXColor){0x00, 0x00, 0x00, 0x80}, x - 150.0f, y, 0.0f, 1.0f);
    PSMTXTrans(mtx, x - 150.0f, y, 0.0f);
    winZClear();
    animPoseDrawMtx(w->mario.poseId, mtx, 2, 0.0f, 5.0f);

    if (w->mario.f190) {
        winWazaGX(w, 0, x - 270.0f, 160.0f, 270.0f, 254.0f);
        winFontInit();
        winFontSetEdge((Vec){(90.0f + x) - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("msg_menu_mario_sac_waza"));
        winFontSetEdge((Vec){rowB - 270.0f, 155.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, "\x82\x72\x82\x6F");
        y = 10.0f + (left - 270.0f);
        x = (228.0f + x) - 270.0f;
        cx = 4.0f + ((17.0f + rowB) - 270.0f);
        for (i = 0; i < w->mario.f198; i++) {
            char sbuf[0x20];

            winFontSetLabel((Vec){y, (f32)(130 - i * 26), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, superActionTable[i]->name);
            // FAKE: the star power cost (s8 baseSpCost, never negative) is read unsigned (lbz with no extsb), as by the old u8 view of the move table
            sprintf(sbuf, "%s", winZenkakuStr((u8)superActionTable[i]->baseSpCost));
            winFontSet((Vec){x - (f32)(FontGetMessageWidth(sbuf) / 2), (f32)(130 - i * 26), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black, sbuf);
            winFontSet((Vec){cx, (f32)(120 - i * 26), 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, "\x82\x72\x82\x6F");
        }
    }
}
#endif
