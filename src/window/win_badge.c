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

extern ItemData itemDataTable[];

void winTexInit_x2(TEXPalette* tpl);
void winTexSet_x2(u32 a, u32 b, Vec translate, Vec scale, GXColor color);
void winIconInit(void);
void winIconGrayInit(void);
void winIconSet(u32 id, Vec translate, Vec scale, GXColor color);
void winFontInit(void);
void winFontSet(Vec translate, Vec scale, GXColor color, const char* fmt, ...);
void winFontSetR(Vec translate, Vec scale, GXColor color, const char* fmt, ...);
const char* winZenkakuStr(s32 n);

void winMakeEquipList(WindowWork* w) {
    s32 n = 0;
    s32 i;
    PouchData* pouch = pouchGetPtr();

    for (i = 0; i < 200; i++) {
        if (pouch->equippedBadges[i] != 0) {
            w->badge.list[n].equipped = 1;
            w->badge.list[n].idx = i;
            w->badge.list[n].id = pouch->equippedBadges[i];
            n++;
        }
    }
    w->badge.count = n;
}

void winBadgeInit(WindowWork* w) {
    w->badge.state = 0;
    w->badge.tab = 0;
    w->badge.sel[0] = 0;
    w->badge.page[0] = 0;
    w->badge.sel[1] = 0;
    w->badge.page[1] = 0;
    winMakeEquipList(w);
}

void winBadgeInit2(WindowWork* w) {
    w->helpTgtX = -270.0f;
    w->helpTgtY = 125 - w->badge.tab * 40;
    w->badge.scroll[0] = w->badge.scrollTgt[0] = 184.0f * w->badge.page[0];
    w->badge.scroll[1] = w->badge.scrollTgt[1] = 184.0f * w->badge.page[1];
    winMsgEntry(w, 0, w->badge.tab == 0 ? "msg_menu_badge_all" : "msg_menu_badge_equip", 0);
    winMakeEquipList(w);
}

void winBadgeExit(void) {
}

void mario_change(WindowWork* w, s32 unused) {
    s32 hasLeft = pouchEquipCheckBadge(0x147);
    s32 hasRight = pouchEquipCheckBadge(0x148);
    const char* model;
    const char* animL;
    const char* animR;

    if (w->flags & 0x2000) {
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
            animPoseSetMaterialFlagOn(w->mario.poseId, 0x1800);
        }
        if (w->log.pose != -1) {
            animPoseRelease(w->log.pose);
            w->log.pose = animPoseEntry("d_mario", 0);
            animPoseSetAnim(w->log.pose, animR, 1);
            animPoseSetMaterialFlagOn(w->log.pose, 0x1800);
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
            animPoseSetMaterialFlagOn(w->mario.poseId, 0x1800);
        }
        if (w->log.pose != -1) {
            animPoseRelease(w->log.pose);
            w->log.pose = animPoseEntry(model, 0);
            animPoseSetAnim(w->log.pose, "M_W_1", 1);
            animPoseSetMaterialFlagOn(w->log.pose, 0x1800);
        }
    }
}

s32 winBadgeMain(WindowWork* w) {
    PouchData* pouch = pouchGetPtr();
    s32 tab;
    s32 cnt;
    s32 pages;
    s32 mode;
    s32 m;

    switch (w->badge.state) {
    case 0:
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(0x20013));
            return -1;
        }
        if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        if (w->dirRep & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
            w->badge.tab = 1 - w->badge.tab;
            psndSFXOn(SFX_NAME_WORD(0x20005));
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
            psndSFXOn(SFX_NAME_WORD(0x20012));
            w->badge.state = 10;
            break;
        } else if (w->buttonTrg & PAD_TRIGGER_Z) {
            // FAKE: an untyped copy of w is loaded into r3 before m is chosen (a WindowWork* copy or w itself is propagated into the call)
            void* p = w;
            s32 m = 3;
            if (w->badge.tab == 0) {
                m = 2;
            }
            winSortEntry(p, m, -310.0f, 150.0f);
            w->savedState = w->badge.state;
            psndSFXOn(SFX_NAME_WORD(0x20012));
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
                    psndSFXOn(SFX_NAME_WORD(0x20039));
                    pouchUnEquipBadgeIndex(idx);
                } else if (pouch->availableBP >= itemDataTable[pouchHaveBadge(idx)].bp_cost) {
                    psndSFXOn(SFX_NAME_WORD(0x20038));
                    pouchEquipBadgeIndex(idx);
                } else {
                    psndSFXOn(SFX_NAME_WORD(0x20014));
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
                mario_change(w, pouchHaveBadge(idx));
            } else {
                // FAKE: the selection is read through w advanced by tab words (sel[0] of the shifted work); w->badge.sel[tab] swaps r27/r28, an s32* &w->badge.sel[tab] folds 0x3E4 into an addi + lwzx
                WindowWork* wt = (WindowWork*)((s32*)w + tab);
                s32 idx = w->badge.list[wt->badge.sel[0]].idx;
                if (w->badge.list[wt->badge.sel[0]].equipped) {
                    psndSFXOn(SFX_NAME_WORD(0x20039));
                    pouchUnEquipBadgeIndex(idx);
                    w->badge.list[wt->badge.sel[0]].equipped = 0;
                } else if (pouch->availableBP >= itemDataTable[pouchHaveBadge(idx)].bp_cost) {
                    psndSFXOn(SFX_NAME_WORD(0x20038));
                    pouchEquipBadgeIndex(idx);
                    w->badge.list[wt->badge.sel[0]].equipped = 1;
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
                mario_change(w, pouchHaveBadge(idx));
            }
        } else if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(0x20013));
            w->badge.state = 0;
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        } else if (w->buttonRep & PAD_TRIGGER_L) {
            w->badge.page[tab]--;
            if (w->badge.page[tab] < 0) {
                w->badge.page[tab] = 0;
            }
            w->badge.sel[tab] = w->badge.page[tab] * 8;
            psndSFXOn(SFX_NAME_WORD(0x20035));
        } else if (w->buttonRep & PAD_TRIGGER_R) {
            w->badge.page[tab]++;
            if (w->badge.page[tab] >= pages) {
                w->badge.page[tab] = pages - 1;
            }
            w->badge.sel[tab] = w->badge.page[tab] * 8;
            psndSFXOn(SFX_NAME_WORD(0x20035));
        } else if (w->dirRep & DEMO_STICK_UP) {
            w->badge.sel[tab]--;
            psndSFXOn(SFX_NAME_WORD(0x20035));
        } else if (w->dirRep & DEMO_STICK_DOWN) {
            w->badge.sel[tab]++;
            psndSFXOn(SFX_NAME_WORD(0x20035));
        } else if (w->buttonTrg & PAD_TRIGGER_Z) {
            // FAKE: an untyped copy of w is loaded into r3 before m is chosen (a WindowWork* copy or w itself is propagated into the call)
            void* p = w;
            m = 3;
            if (tab == 0) {
                m = 2;
            }
            winSortEntry(p, m, -310.0f, 150.0f);
            w->savedState = w->badge.state;
            psndSFXOn(SFX_NAME_WORD(0x20012));
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
            mode = pouchHaveBadge(w->badge.sel[tab]);
        } else {
            mode = w->badge.list[w->badge.sel[tab]].id;
        }
        winMsgEntry(w, mode, itemDataTable[mode].menu_description, 0);
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

void winBadgeMain2(WindowWork* w) {
    s32 i;
    for (i = 0; i < 2; i++) {
        w->badge.scrollTgt[i] = 184.0f * w->badge.page[i];
        w->badge.scroll[i] += (w->badge.scrollTgt[i] - w->badge.scroll[i]) / 4.0f;
    }
}

#ifdef VERSION_G8ME01
void winFontSetWidth(Vec translate, Vec scale, GXColor color, f32 width, const char* fmt, ...);
#endif
void badge_disp(WindowWork* w, f32 xo, f32 yo);

#define Q ((pouch->maximumBP + 9) / 10)

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
#ifdef VERSION_G8ME01
    f32 tw;
#endif

    winBgGX(w, 3, x, y);
    winKirinukiGX(w, 0, x - 100.0f, 20.0f + (120.0f + y), 365.0f, 210.0f);
    a = 105.0f + y;
    c = (x - 190.0f) - 4.0f;
    b = 4.0f + a;
    for (i = 0; i < 2; i++) {
        off = i * VERSION_PICK(40, 45);
        if (i == w->badge.tab) {
            winTexInit((TEXPalette*)*w->res->data);
            winTexSet(0xB4, (Vec){x - 190.0f, VERSION_PICK(20.0f, 25.0f) + (a - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x80});
            winTexInit_x2((TEXPalette*)*w->res->data);
            winTexSet_x2(i + 0x1A, 0xB4, (Vec){c, VERSION_PICK(20.0f, 25.0f) + (b - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        } else {
            winTexInit_x2((TEXPalette*)*w->res->data);
            winTexSet_x2(i + 0x1A, 0xB4, (Vec){x - 190.0f, VERSION_PICK(20.0f, 25.0f) + (a - off), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x80, 0x80, 0x80, 0xFF});
        }
    }
    winKirinukiGX(w, 1, x - 260.0f, yy = 20.0f + (40.0f + y), 150.0f, 158 - (10 - Q) * 13);
    winFontInit();
#ifdef VERSION_G8ME01
    {
        // US: BP label per language (indexed by gp->language)
        const char* bpLabel[7] = {"\202a\202o", "BP", "OP", "PB", "PM", "PT", "BP"};
        winFontSet((Vec){x - 220.0f, yy = yy - 4.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, bpLabel[gp->language]);
    }
#else
    winFontSet((Vec){x - 220.0f, yy = yy - 4.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "\202a\202o");
#endif
    winFontSetR((Vec){(x - 175.0f) - 18.0f, yy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "%s", winZenkakuStr(pouch->maximumBP));
#ifdef VERSION_G8ME01
    tw = FontGetMessageWidth(msgSearch("msg_menu_badge_nokori")) * 0.5f + 60.0f;
    winFontSet((Vec){4.0f + (16.0f + ((x - 260.0f) + (150.0f - tw))), ((20.0f + (y - 108.0f)) - 18.0f) + (10 - Q) * 13, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("msg_menu_badge_nokori"));
#else
    winFontSet((Vec){x - 180.0f, ((20.0f + (y - 108.0f)) - 18.0f) + (10 - Q) * 13, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black, msgSearch("msg_menu_badge_nokori"));
#endif
    winFontSetR((Vec){VERSION_PICK((x - 145.0f) - 16.0f, 4.0f + ((x - 145.0f) - 16.0f)), ((20.0f + (y - 102.0f)) - 18.0f) + (10 - Q) * 13, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, "%s", winZenkakuStr(pouch->availableBP));
    winTexInit((TEXPalette*)*w->res->data);
    d = 28.0f + y;
    for (i = 0; i < pouch->availableBP; i++) {
        winTexSet(0x1C, (Vec){(x - 242.0f) + (i % 10) * 13, (20.0f + (d - (i / 10) * 13)) - 18.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    for (i = pouch->availableBP; i < pouch->maximumBP; i++) {
        winTexSet(0x1D, (Vec){(x - 242.0f) + (i % 10) * 13, (20.0f + (d - (i / 10) * 13)) - 18.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
    badge_disp(w, x, y);
}

static inline s32 badgeTexA(s32 e) { s32 t = 0x1D; if (e) t = 0x1C; return t; }
static inline s32 badgeTexB(s32 e) { if (e) return 0x1C; return 0x1D; }
void badge_disp(WindowWork* w, f32 xo, f32 yo) {
    WindowWork* p0;
    ItemData* item;
    s8* cost;
    s32 j;
    GXColor colA = {255, 255, 255, 255};
    GXColor colB;
    GXColor colU;
    GXColor colD;
    char buf[64];
    s32 cnt;
    s32 rowOff;
    s32 i;
    s32 badge;
    s32 equipped;
    s32 gray;
    f32 scroll;
    f32 width;
    s32 tw;

    if (w->badge.tab == 0) {
        cnt = pouchGetHaveBadgeCnt();
    } else {
        cnt = w->badge.count;
    }
    GXSetScissor(0, 0x77, 0x260, 0xB8);
    p0 = w;
    // FAKE: p0 is a WindowWork* stepped one WinBadgeEnt at a time, so list[0] reads w->badge.list[i] with the 0x404 kept in the load displacement (w->badge.list[i] or a WinBadgeEnt* walk: 99.95-99.96%, induction order / base differ)
    for (i = 0, rowOff = 0; i < cnt; p0 = (WindowWork*)((WinBadgeEnt*)p0 + 1), rowOff += 23, i++) {
        if (w->badge.tab == 0) {
            badge = pouchHaveBadge(i);
        } else {
            badge = p0->badge.list[0].id;
        }
        scroll = w->badge.scroll[w->badge.tab];
        if (w->badge.tab == 0) {
            equipped = pouchEquipCheckBadgeIndex(i);
        } else {
            equipped = p0->badge.list[0].equipped;
        }
        if ((scroll + ((yo + 110.0f) - (f32)rowOff)) > 240.0f) {
            continue;
        }
        if ((scroll + ((yo + 110.0f) - (f32)rowOff)) < -240.0f) {
            break;
        }
        if (equipped) {
            winTexInit((TEXPalette*)*w->res->data);
            winTexSet(0x1E, (Vec){82.0f + xo, (scroll + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){44.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
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
        winIconSet(itemDataTable[badge].icon_id, (Vec){xo - 75.0f, (scroll + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){0.54f, 0.54f, 0.54f}, colA);
        item = &itemDataTable[badge];
        winFontInit();
#ifdef VERSION_G8ME01
        winFontSetWidth((Vec){xo - 55.0f, scroll + (12.0f + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, colB, 170.0f, msgSearch(item->name));
#else
        winFontSet((Vec){xo - 55.0f, scroll + (12.0f + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, colB, item->id);
#endif
        cost = &item->bp_cost;
        sprintf(buf, "%s", winZenkakuStr(*cost));
        winFontSet((Vec){(142.0f + xo) - (f32)(FontGetMessageWidth(buf) / 2), scroll + (12.0f + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, colB, buf);
        winTexInit((TEXPalette*)*w->res->data);
        for (j = 0; j < *cost; j++) {
            { s32 t; winTexSet((t = 0x1D, equipped ? t = 0x1C : 0, t), (Vec){(160.0f + xo) + j * 13, (scroll + ((yo + 110.0f) - (f32)rowOff)), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, colA); }
        }
    }
    GXSetScissor(0, 0, 0x260, 0x1E0);
    {
        f32 ax;
        colU = (GXColor){255, 255, 255, 255};
        colD = (GXColor){0, 0, 0, 255};

        if (w->badge.page[w->badge.tab] > 0) {
            winTexInit((TEXPalette*)*w->res->data);
            ax = 250.0f + xo;
            winTexSet(0x17, (Vec){ax, 20.0f + (18.0f + (110.0f + yo)), 0.0f}, (Vec){-1.0f, -1.0f, -1.0f}, colU);
            winIconInit();
            winIconSet(ICON_07E, (Vec){ax, 20.0f + (113.0f + yo), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, colU);
        }
        if (cnt > w->badge.page[w->badge.tab] * 8 + 8) {
            winTexInit((TEXPalette*)*w->res->data);
            ax = 250.0f + xo;
            winTexSet(0x17, (Vec){ax, 20.0f + ((yo - 90.0f) - 15.0f), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, colU);
            winIconInit();
            winIconSet(ICON_080, (Vec){ax, 20.0f + (yo - 90.0f), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, colU);
        }
        width = 0.7f * (f32)(u32)FontGetMessageWidth(msgSearch("msg_menu_sort_narabikae"));
        winFontInit();
        width = xo + (220.0f - width);
        winFontSet((Vec){width, 20.0f + ((yo - 92.0f) - 10.0f), 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, colD, msgSearch("msg_menu_sort_narabikae"));
        winIconInit();
        winIconSet(ICON_217, (Vec){width - 30.0f, 20.0f + ((yo - 100.0f) - 10.0f), 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, colU);
        winFontInit();
        winFontSet((Vec){120.0f + xo, 135.0f + yo, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, colD, msgSearch("msg_menu_badge_hituyou"));
    }
}
