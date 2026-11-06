// Pause menu: the journal window (map, crystal stars, badge and recipe logs, tattle log).

#include "version.h"
#include "win/win_main.h"
#include "evt/evt_story.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "driver/swdrv.h"
#include "manager/filemgr.h"
#include "memory.h"
#include <string.h>
#include "battle/battle_monosiri.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "icon_ids.h"
#include "sdk/texPalette.h"
#include "mario/mario_pouch.h"
#include "data/item_data.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/gx.h>
#include "win/win_root.h"
#include "eff/eff_starstone.h"
#include "mario/mario.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "driver/camdrv.h"
#include "sfx_name_word.h"
#include "manager/fontmgr.h"
#include "eff/eff_magic1_n64.h"
#include "driver/msgdrv.h"
#include "libc_decls.h"
#include "win/win_log.h"
#include "driver/mapdrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_save_flags.h"

typedef struct TubuDt {
    s16 x;
    s16 y;
    u8 big;
    u8 type;
    u16 progress;
    const char* map;
} TubuDt;

typedef struct MonosiriWork {
    s32 state;          // 0x0
    s32 id;             // 0x4
    FileEntry* winTpl;   // 0x8
    FileEntry* monoDat;   // 0xC
    f32 curX;           // 0x10
    f32 curY;           // 0x14
    f32 x;              // 0x18
    f32 y;              // 0x1C
    s32 pose1;          // 0x20
    s32 pose2;          // 0x24
    smartEntry* smart;  // 0x28
    s32 alpha;          // 0x2C
} MonosiriWork;

static void capture(CameraId camera, void* param);
static void monosiri_disp(CameraId camera, void* param);

/**
 * Dots of the Journal map: position on the map, big (a place) or small (the path between), the chapter switch
 * that can also reveal it, the story progress that reveals it, and the area for its name and help text.
 * The 93 dots cover every area of the map roughly in story order,
 * from Rogueport and Petal Meadows to the Moon, the X-Naut Fortress and the Palace of Shadow.
 * The map page draws them, and the cursor names the nearest big one.
 */
#if VERSION >= VERSION_JP
TubuDt tubuDt[] = {
    {14, -34, 1, 0, 3, "gor"},
    {-4, -31, 0, 2, 12, "gor"},
    {-20, -31, 0, 2, 12, "gor"},
    {-33, -38, 1, 2, 12, "tik"},
    {118, 30, 1, 0, 23, "hei"},
    {134, 22, 0, 0, 26, "hei"},
    {150, 14, 0, 0, 26, "hei"},
    {176, 9, 0, 0, 26, "hei"},
    {200, 6, 1, 0, 26, "nok"},
    {136, 39, 0, 3, 35, "hei"},
    {152, 48, 0, 3, 35, "hei"},
    {165, 53, 0, 3, 35, "hei"},
    {165, 68, 1, 3, 35, "gon"},
    {0, 36, 1, 0, 71, "win"},
    {0, 56, 0, 0, 74, "win"},
    {0, 75, 1, 0, 74, "mri"},
    {16, 44, 0, 0, 81, "win"},
    {27, 53, 0, 0, 81, "win"},
    {42, 65, 1, 0, 81, "hou"},
    {30, -17, 0, 0, 127, "gor"},
    {36, 0, 0, 0, 127, "gor"},
    {46, 15, 0, 0, 127, "gor"},
    {55, 32, 0, 0, 127, "gor"},
    {74, 45, 0, 0, 127, "gor"},
    {83, 61, 0, 0, 127, "gor"},
    {83, 75, 0, 0, 127, "gor"},
    {83, 93, 0, 0, 127, "gor"},
    {72, 101, 0, 0, 127, "gor"},
    {71, 114, 0, 0, 127, "gor"},
    {71, 132, 0, 0, 127, "gor"},
    {80, 141, 0, 0, 127, "gor"},
    {97, 150, 1, 0, 127, "tou"},
    {-181, -90, 1, 0, 178, "usu"},
    {-163, -90, 0, 4, 184, "usu"},
    {-145, -90, 0, 4, 184, "usu"},
    {-135, -77, 0, 4, 184, "usu"},
    {-143, -63, 0, 4, 184, "usu"},
    {-159, -63, 0, 4, 184, "usu"},
    {-176, -63, 0, 4, 184, "usu"},
    {-191, -63, 0, 4, 184, "usu"},
    {-206, -63, 0, 4, 184, "usu"},
    {-228, -63, 1, 4, 184, "gra"},
    {-228, -44, 0, 0, 193, "gra"},
    {-216, -31, 0, 0, 193, "gra"},
    {-198, -27, 0, 0, 193, "gra"},
    {-181, -29, 0, 0, 193, "gra"},
    {-160, -33, 0, 0, 193, "gra"},
    {-139, -33, 1, 0, 193, "jin"},
    {13, -56, 0, 0, 232, "gor"},
    {13, -70, 0, 0, 232, "gor"},
    {12, -84, 0, 0, 232, "gor"},
    {-3, -94, 0, 0, 232, "gor"},
    {-14, -104, 0, 0, 232, "gor"},
    {-31, -115, 0, 0, 232, "gor"},
    {-45, -128, 0, 0, 232, "gor"},
    {-48, -143, 0, 0, 232, "gor"},
    {-38, -158, 0, 0, 232, "gor"},
    {-11, -163, 1, 0, 232, "muj"},
    {7, -163, 0, 0, 243, "muj"},
    {21, -163, 0, 0, 243, "muj"},
    {41, -163, 1, 0, 243, "dou"},
    {36, -34, 0, 0, 310, "gor"},
    {53, -27, 0, 0, 310, "gor"},
    {69, -12, 0, 0, 310, "gor"},
    {85, 1, 0, 0, 310, "gor"},
    {109, 7, 0, 0, 310, "gor"},
    {132, 4, 0, 0, 310, "gor"},
    {149, -8, 0, 0, 310, "gor"},
    {158, -25, 1, 0, 310, "eki"},
    {143, -40, 0, 0, 334, "eki"},
    {133, -52, 0, 0, 334, "eki"},
    {132, -66, 0, 0, 334, "eki"},
    {132, -82, 0, 0, 334, "eki"},
    {155, -90, 1, 0, 334, "pik"},
    {175, -90, 0, 0, 336, "pik"},
    {193, -91, 0, 0, 336, "pik"},
    {211, -91, 1, 0, 336, "sin"},
    {-167, 17, 1, 0, 353, "bom"},
    {-167, 37, 0, 0, 358, "bom"},
    {-167, 50, 0, 0, 358, "bom"},
    {-167, 64, 0, 0, 358, "bom"},
    {-167, 82, 0, 0, 358, "bom"},
    {-167, 99, 0, 0, 358, "bom"},
    {-167, 116, 0, 0, 358, "bom"},
    {-167, 133, 0, 0, 358, "bom"},
    {-167, 151, 1, 0, 358, "moo"},
    {-171, 169, 0, 0, 361, "moo"},
    {-185, 177, 0, 0, 361, "moo"},
    {-200, 171, 0, 0, 361, "moo"},
    {-209, 159, 1, 0, 361, "aji"},
    {-33, -50, 0, 0, 382, "gor"},
    {-33, -62, 0, 0, 382, "gor"},
    {-33, -74, 1, 0, 382, "las"},
};
#else
static f32 unusedSignTbl[16] = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
#endif

s32 stone_tbl[] = {ITEM_DIAMOND_STAR, ITEM_EMERALD_STAR, ITEM_GOLD_STAR, ITEM_RUBY_STAR, ITEM_SAPPHIRE_STAR, ITEM_GARNET_STAR, ITEM_CRYSTAL_STAR};

/**
 * Adds a Journal entry button (0 map, 1 Crystal Stars, 2 badge log, 3 recipe log,
 * 4 Tattle log) with its help text, and re-spaces all buttons 45 units apart around the centre.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void logEntry(WindowWork* w, s32 id) {
    static const char* msg_help[] = {
        "msg_menu_kiroku_map",
        "msg_menu_kiroku_stone",
        "msg_menu_kiroku_badge",
        "msg_menu_kiroku_ryori",
        "msg_menu_kiroku_mono",
    };
    WinLogMenu* m = &w->log.menu[w->log.count];
    s32 i;
    m->id = id;
    m->timer = 0;
    m->state = 0;
    m->msg = msg_help[id];
    w->log.count++;
    for (i = 0; i < w->log.count; i++) {
        w->log.menu[i].x = 0.0f;
        w->log.menu[i].tx = 0.0f;
        w->log.menu[i].ty = w->log.menu[i].y = 45.0f * w->log.count / 2.0f - 45.0f * i;
    }
}

#if VERSION <= VERSION_DEMO_JP
#define TUBU_TYPE(t) 0
#define TUBU_SEQ_361 360
TubuDt tubuDt[] = {
    {14, -34, 1, 0, 3, "gor"},
    {-4, -31, 0, TUBU_TYPE(2), 12, "gor"},
    {-20, -31, 0, TUBU_TYPE(2), 12, "gor"},
    {-33, -38, 1, TUBU_TYPE(2), 12, "tik"},
    {118, 30, 1, 0, 23, "hei"},
    {134, 22, 0, 0, 26, "hei"},
    {150, 14, 0, 0, 26, "hei"},
    {176, 9, 0, 0, 26, "hei"},
    {200, 6, 1, 0, 26, "nok"},
    {136, 39, 0, TUBU_TYPE(3), 35, "hei"},
    {152, 48, 0, TUBU_TYPE(3), 35, "hei"},
    {165, 53, 0, TUBU_TYPE(3), 35, "hei"},
    {165, 68, 1, TUBU_TYPE(3), 35, "gon"},
    {0, 36, 1, 0, 71, "win"},
    {0, 56, 0, 0, 74, "win"},
    {0, 75, 1, 0, 74, "mri"},
    {16, 44, 0, 0, 81, "win"},
    {27, 53, 0, 0, 81, "win"},
    {42, 65, 1, 0, 81, "hou"},
    {30, -17, 0, 0, 127, "gor"},
    {36, 0, 0, 0, 127, "gor"},
    {46, 15, 0, 0, 127, "gor"},
    {55, 32, 0, 0, 127, "gor"},
    {74, 45, 0, 0, 127, "gor"},
    {83, 61, 0, 0, 127, "gor"},
    {83, 75, 0, 0, 127, "gor"},
    {83, 93, 0, 0, 127, "gor"},
    {72, 101, 0, 0, 127, "gor"},
    {71, 114, 0, 0, 127, "gor"},
    {71, 132, 0, 0, 127, "gor"},
    {80, 141, 0, 0, 127, "gor"},
    {97, 150, 1, 0, 127, "tou"},
    {-181, -90, 1, 0, 178, "usu"},
    {-163, -90, 0, TUBU_TYPE(4), 184, "usu"},
    {-145, -90, 0, TUBU_TYPE(4), 184, "usu"},
    {-135, -77, 0, TUBU_TYPE(4), 184, "usu"},
    {-143, -63, 0, TUBU_TYPE(4), 184, "usu"},
    {-159, -63, 0, TUBU_TYPE(4), 184, "usu"},
    {-176, -63, 0, TUBU_TYPE(4), 184, "usu"},
    {-191, -63, 0, TUBU_TYPE(4), 184, "usu"},
    {-206, -63, 0, TUBU_TYPE(4), 184, "usu"},
    {-228, -63, 1, TUBU_TYPE(4), 184, "gra"},
    {-228, -44, 0, 0, 193, "gra"},
    {-216, -31, 0, 0, 193, "gra"},
    {-198, -27, 0, 0, 193, "gra"},
    {-181, -29, 0, 0, 193, "gra"},
    {-160, -33, 0, 0, 193, "gra"},
    {-139, -33, 1, 0, 193, "jin"},
    {13, -56, 0, 0, 232, "gor"},
    {13, -70, 0, 0, 232, "gor"},
    {12, -84, 0, 0, 232, "gor"},
    {-3, -94, 0, 0, 232, "gor"},
    {-14, -104, 0, 0, 232, "gor"},
    {-31, -115, 0, 0, 232, "gor"},
    {-45, -128, 0, 0, 232, "gor"},
    {-48, -143, 0, 0, 232, "gor"},
    {-38, -158, 0, 0, 232, "gor"},
    {-11, -163, 1, 0, 232, "muj"},
    {7, -163, 0, 0, 243, "muj"},
    {21, -163, 0, 0, 243, "muj"},
    {41, -163, 1, 0, 243, "dou"},
    {36, -34, 0, 0, 310, "gor"},
    {53, -27, 0, 0, 310, "gor"},
    {69, -12, 0, 0, 310, "gor"},
    {85, 1, 0, 0, 310, "gor"},
    {109, 7, 0, 0, 310, "gor"},
    {132, 4, 0, 0, 310, "gor"},
    {149, -8, 0, 0, 310, "gor"},
    {158, -25, 1, 0, 310, "eki"},
    {143, -40, 0, 0, 334, "eki"},
    {133, -52, 0, 0, 334, "eki"},
    {132, -66, 0, 0, 334, "eki"},
    {132, -82, 0, 0, 334, "eki"},
    {155, -90, 1, 0, 334, "pik"},
    {175, -90, 0, 0, 336, "pik"},
    {193, -91, 0, 0, 336, "pik"},
    {211, -91, 1, 0, 336, "sin"},
    {-167, 17, 1, 0, 353, "bom"},
    {-167, 37, 0, 0, 358, "bom"},
    {-167, 50, 0, 0, 358, "bom"},
    {-167, 64, 0, 0, 358, "bom"},
    {-167, 82, 0, 0, 358, "bom"},
    {-167, 99, 0, 0, 358, "bom"},
    {-167, 116, 0, 0, 358, "bom"},
    {-167, 133, 0, 0, 358, "bom"},
    {-167, 151, 1, 0, 358, "moo"},
    {-171, 169, 0, 0, TUBU_SEQ_361, "moo"},
    {-185, 177, 0, 0, TUBU_SEQ_361, "moo"},
    {-200, 171, 0, 0, TUBU_SEQ_361, "moo"},
    {-209, 159, 1, 0, TUBU_SEQ_361, "aji"},
    {-33, -50, 0, 0, 382, "gor"},
    {-33, -62, 0, 0, 382, "gor"},
    {-33, -74, 1, 0, 382, "las"},
};
#endif

/**
 * Sets up the Journal when the pause menu opens:
 * adds the entries unlocked by story progress (map always; Tattle log once Goombella joins;
 * Crystal Stars at Frankly's; badge log after the badge tutorial;
 * recipe log once unlocked), spawns the seven Crystal Star effects (as many lit, in table order, as Mario owns; the rest dark),
 * finds Mario's spot on the world map for the current area, and builds the sorted badge,
 * recipe and Tattle lists with their found counts.
 */
#if VERSION <= VERSION_DEMO_JP
void winLogInit(WindowWork* w) {
    s32 j;
    s32 i;
    u32 k;
    EffectEntry* e;
    const char* area;
    TattleEntry* t;
#endif

/**
 * Sets up the Journal when the pause menu opens:
 * adds the entries unlocked by story progress (map always; Tattle log once Goombella joins;
 * Crystal Stars at Frankly's; badge log after the badge tutorial;
 * recipe log once unlocked), spawns the seven Crystal Star effects (as many lit, in table order, as Mario owns; the rest dark),
 * finds Mario's spot on the world map for the current area, and builds the sorted badge,
 * recipe and Tattle lists with their found counts.
 */
#if VERSION >= VERSION_JP
void winLogInit(WindowWork* w) {
    u32 k;
    const char* area;
    s32 i;
    s32 j;
    TattleEntry* t;
    EffectEntry* e;
#endif

    WinSortEnt tmp;

    w->log.state = 0;
    w->log.cursor = 0;
    w->log.count = 0;
    logEntry(w, 0);
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= STG0_16_FRANKLYS_HOUSE) logEntry(w, 1);
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= STG0_07_GOOMBELLA_JOINS) logEntry(w, 4);
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= STG0_17_BADGE_TUTORIAL) logEntry(w, 2);
    if (swGet(GSWF_ID(GSWF_GOR_ZESS_T_DISH_RECEIVED))) logEntry(w, 3);
    w->log.stoneNum = 0;
    for (i = 0; i < 7; i++) {
        if (pouchCheckItem(stone_tbl[i])) {
            w->log.stoneNum++;
        }
    }
    for (i = 0; i < 7; i++) {
        if (i < w->log.stoneNum) {
            e = effStarStoneEntry(i, 0.0f, -1000.0f, 0.0f, 2.0f);
            w->log.eff[i] = e;
        } else {
            e = effStarStoneEntry(i + 7, 0.0f, -1000.0f, 0.0f, 2.0f);
            w->log.eff[i] = e;
            ((EffStarStoneWork*)e->userdata)->r = 0x20;
            ((EffStarStoneWork*)e->userdata)->g = 0x20;
            ((EffStarStoneWork*)e->userdata)->b = 0x20;
        }
        ((EffStarStoneWork*)e->userdata)->cam = CAMERA_2D;
        ((EffStarStoneWork*)e->userdata)->flags &= ~STAR_STONE_FLAG_SHINE_SFX;
#if VERSION >= VERSION_JP
        ((EffStarStoneWork*)e->userdata)->flags |= STAR_STONE_FLAG_EXTERNAL_DRAW;
#endif
    }

    w->mapRes = NULL;
    w->log.pose = -1;
    w->log.mapSel = -1;
    w->log.mapIdx = 0;
    w->log.mapX = w->log.mapY = 0.0f;
    w->log.mapWinTgtX = w->log.mapWinX = 0.0f;
    w->log.mapWinTgtY = w->log.mapWinY = -480.0f;
    w->log.scrollX = w->log.scrollY = 0.0f;

    area = gp->currentAreaName;
    if (!strcmp(gp->currentMapName, "pik_02") || !strcmp(gp->currentMapName, "pik_03")) area = "sin";
    if (!strcmp(gp->currentMapName, "win_04") || !strcmp(gp->currentMapName, "win_05")) area = "hou";
    if (!strncmp(gp->currentMapName, "rsh", 3)) {
        if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG6_27_RIVERSIDE_STATION) {
            area = "gor";
        } else if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG6_49_POSHLEY_HEIGHTS || swGet(GSWF_ID(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE))) {
            area = "eki";
        } else if (swGet(GSWF_ID(GSWF_HOM_TRAIN_AT_POSHLEY))) {
            area = "pik";
        } else {
            switch (swByteGet(GSW_ID(GSW_HOM_TRAIN_LEG))) {
            case 0:
                area = "gor";
                break;
            case 1:
                area = "eki";
                break;
            case 2:
                area = "pik";
                break;
            case 3:
                area = "gor";
                break;
            }
        }
    }
#if VERSION <= VERSION_DEMO_JP
    if (!strncmp(gp->currentMapName, "hom", 3))
        area = "eki";
    if (!strncmp(gp->currentMapName, "jon", 3))
        area = "tik";
    if (!strcmp(gp->currentMapName, "gra_06"))
        area = "jin";
    {
        for (k = 0; k < 93; k++) {
            if (!area || !strcmp(area, tubuDt[k].map))
                break;
        }
        if (k < 93)
            w->log.mapIdx = k;
    }
#else
    if (!strncmp(gp->currentMapName, "hom", 3)) area = "eki";
    if (!strncmp(gp->currentMapName, "jon", 3)) area = "tik";
    if (!strcmp(gp->currentMapName, "gra_06")) area = "jin";
    if (!strcmp(gp->currentMapName, "muj_10") || !strcmp(gp->currentMapName, "muj_11") || !strcmp(gp->currentMapName, "muj_12")) area = "dou";

    for (k = 0; k < sizeof(tubuDt) / sizeof(TubuDt); k++) {
        if (!area || !strcmp(area, tubuDt[k].map)) break;
    }
    if (k < sizeof(tubuDt) / sizeof(TubuDt)) w->log.mapIdx = k;
#endif
    w->log.mapX = 304.0f + tubuDt[w->log.mapIdx].x;
    w->log.mapY = 240.0f + tubuDt[w->log.mapIdx].y;
    {
        s32 dx = w->log.mapX + w->log.scrollX - 230.0f;
        s32 dy = w->log.mapY + w->log.scrollY - 120.0f;
        w->log.scrollX -= dx;
        w->log.scrollY -= dy;
    }
    if (w->log.scrollX > 0.0f) w->log.scrollX = 0.0f;
    if (w->log.scrollY > 0.0f) w->log.scrollY = 0.0f;
    if (w->log.scrollX < -148.0f) w->log.scrollX = -148.0f;
    if (w->log.scrollY < -240.0f) w->log.scrollY = -240.0f;
    w->log.stoneSel = 0;
    w->log.stoneAngle = w->log.stoneAngleTgt = 51.42857f * w->log.stoneSel;
    w->log.stoneWinTgtX = w->log.stoneWinX = 0.0f;
    w->log.stoneWinTgtY = w->log.stoneWinY = -480.0f;
    w->log.stoneAlphaTgt = w->log.stoneAlpha = 0.0f;

    w->log.badge.num = 0;
#if VERSION >= VERSION_US
    w->log.badge.page = 0;
#endif
    for (i = BADGE_MIN; i < BADGE_MAX; i++) {
        if (itemDataTable[i].type_sort_order != -1) {
            w->log.badgeIds[w->log.badge.num++] = i - BADGE_MIN;
#if VERSION >= VERSION_US
            if (swGet(i - BADGE_MIN + GSWF_BADGE_FOUND_BASE)) {
                w->log.badge.page++;
            }
#endif
        }
    }
    qqsort(w->log.badgeIds, w->log.badge.num, 2, compare_func5_2);
    w->log.badge.cursor = 0;
    w->log.badge.x = w->log.badge.tx = 320.0f;
    w->log.badge.y = w->log.badge.ty = -240.0f;
    w->log.badge.idx = 0;
    w->log.badge.scroll = w->log.badge.scrollTgt = 0.0f;
    w->log.badge.iconX = w->log.badge.iconTx = -200.0f;
    w->log.badge.iconY = w->log.badge.iconTy = -300.0f;

    w->log.recipe.num = 0;
#if VERSION >= VERSION_US
    w->log.recipe.page = 0;
#endif
    for (i = ITEM_SHROOM_FRY; i < ITEM_MAX; i++) {
        if (itemDataTable[i].type_sort_order != -1) {
            w->log.recipeIds[w->log.recipe.num++] = i - ITEM_SHROOM_FRY;
#if VERSION >= VERSION_US
            if (swGet(i - ITEM_SHROOM_FRY + GSWF_RECIPE_FOUND_BASE)) {
                w->log.recipe.page++;
            }
#endif
        }
    }
    qqsort(w->log.recipeIds, w->log.recipe.num, 2, compare_func6_2);
    w->log.recipe.cursor = 0;
    w->log.recipe.x = w->log.recipe.tx = 320.0f;
    w->log.recipe.y = w->log.recipe.ty = -240.0f;
    w->log.recipe.idx = 0;
    w->log.recipe.scroll = w->log.recipe.scrollTgt = 0.0f;
    w->log.recipe.iconX = w->log.recipe.iconTx = -200.0f;
    w->log.recipe.iconY = w->log.recipe.iconTy = -300.0f;

    w->log.enemyNum = 0;
    for (i = 1; i < 0xD8; i++) {
        t = battleGetUnitMonosiriPtr(i);
        if (!t->tattleLogTag) continue;
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG8_07_BELDAM_CONFRONTATION) {
            if (i == UNIT_BELDAM_CHAPTER8 || i == UNIT_MARILYN_CHAPTER8 || i == UNIT_DOOPLISS_CHAPTER8) continue;
        } else {
            if (i == UNIT_BELDAM_CHAPTER2 || i == UNIT_MARILYN_CHAPTER2 || i == UNIT_DOOPLISS_CHAPTER4_FIGHT1) continue;
        }
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG8_15_GRODUS) {
            if (i == UNIT_BOWSER_CHAPTER8) continue;
        } else {
            if (i == UNIT_BOWSER_CHAPTER3) continue;
        }
        w->log.enemy[w->log.enemyNum].kind = enemy_monoshiri_sort_table[i];
        w->log.enemy[w->log.enemyNum++].id = i;
    }
    for (i = 0; i < w->log.enemyNum - 1; i++) {
        for (j = i + 1; j < w->log.enemyNum; j++) {
            if (w->log.enemy[i].kind > w->log.enemy[j].kind) {
                tmp = w->log.enemy[i];
                w->log.enemy[i] = w->log.enemy[j];
                w->log.enemy[j] = tmp;
            }
        }
    }
    w->log.enemySel = 0;
    w->log.enemyX = w->log.enemyTx = 0.0f;
    w->log.enemyY = w->log.enemyTy = 0.0f;
    w->log.enemyPage = 0;
    w->log.enemyScroll = w->log.enemyScrollTgt = 0.0f;
    w->log.enemyKnown = 0;
    for (i = 1; i < 0xD8; i++) {
        t = battleGetUnitMonosiriPtr(i);
        if (!t->tattleLogTag) continue;
        if (i == UNIT_BELDAM_CHAPTER2 || i == UNIT_MARILYN_CHAPTER2 || i == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || i == UNIT_BOWSER_CHAPTER3) continue;
        if (swGet(i + GSWF_TATTLE_BASE)) {
            w->log.enemyKnown++;
        }
    }
}

/**
 * Run from win_root.c each time the Journal comes to the front:
 * hides the badge and recipe panels, restores their page scroll, and shows the selected entry's help text.
 */
void winLogInit2(WindowWork* w) {
    w->helpTgtX = -128.0f;
    w->helpTgtY = 8.0f + (45.0f * w->log.count / 2.0f - 45.0f * w->log.cursor);
    w->log.badge.x = w->log.badge.tx = 320.0f;
    w->log.badge.y = w->log.badge.ty = -240.0f;
    w->log.badge.scroll = w->log.badge.scrollTgt = w->log.badge.idx * 0xE8;
    w->log.badge.iconX = w->log.badge.iconTx = -200.0f;
    w->log.badge.iconY = w->log.badge.iconTy = -300.0f;
    w->log.recipe.x = w->log.recipe.tx = 320.0f;
    w->log.recipe.y = w->log.recipe.ty = -240.0f;
    w->log.recipe.scroll = w->log.recipe.scrollTgt = w->log.recipe.idx * 0xE8;
    w->log.recipe.iconX = w->log.recipe.iconTx = -200.0f;
    w->log.recipe.iconY = w->log.recipe.iconTy = -300.0f;
    winMsgEntry(w, 0, w->log.menu[w->log.cursor].msg, 0);
}

/** Frees the Journal's map texture, the Mario map marker model and the seven Crystal Star effects. */
void winLogExit(WindowWork* w) {
    s32 i;
    if (w->mapRes) {
        fileFree(w->mapRes);
        w->mapRes = NULL;
    }
    if (w->log.pose != -1) {
        animPoseRelease(w->log.pose);
        w->log.pose = -1;
    }
    for (i = 0; i < 7; i++) {
        effSoftDelete(w->log.eff[i]);
    }
}

/**
 * TRUE if a map route dot of this type has been revealed early by its flag:
 * 1 Boggly Woods, 2 the Rogueport Underground route, 3 Hooktail Castle, 4 Twilight Trail.
 * Type 0 dots depend only on story progress.
 */
static inline BOOL tubuCheck(u8 type) {
    switch (type) {
    case 0:
        return FALSE;
    case 1:
        if (swGet(GSWF_ID(GSWF_WIN_04_ENTERED))) return TRUE;
        break;
    case 2:
        if (swGet(GSWF_ID(GSWF_TIK_01_VISITED))) return TRUE;
        break;
    case 3:
        if (swGet(GSWF_ID(GSWF_GON_00_VISITED))) return TRUE;
        break;
    case 4:
        if (swGet(GSWF_ID(GSWF_GRA_00_ENTERED))) return TRUE;
        break;
    }
    return FALSE;
}


/**
 * Journal input.
 * State 0 picks an entry; 10 is the world map (load the map TPL, move the cursor with the stick or D-pad,
 * snap to and name the nearest place); 11 turns the Crystal Star carousel;
 * 12 and 13 browse the badge and recipe logs (7x4 grid pages);
 * 14 the Tattle log, where A opens an entry (21/22); 1000/1001 run the Z sort menu.
 * Returns -1 on B at the top level, -2 on Start, else 0.
 */
s32 winLogMain(WindowWork* w) {
    static char name[0x40];
    static char help[0x40];
    s32 i;
    s32 sel;
    TubuDt* t;
    s32 pages;
    s32 old;
    u32 k;
    const char* anim;
    const char* model;
    f32 min;
    f32 x;
    f32 y;
    f32 bx;
    f32 by;
    f32 dx;
    f32 dy;
    s32 ix;
    s32 iy;

    switch (w->log.state) {
    case 0:
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            return -1;
        }
        if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        if (w->dirRep & DEMO_STICK_DOWN) {
            old = w->log.cursor;
            w->log.cursor++;
            if (w->log.cursor >= w->log.count) w->log.cursor = 0;
            if (old != w->log.cursor) psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_UP) {
            old = w->log.cursor;
            w->log.cursor--;
            if (w->log.cursor < 0) w->log.cursor = w->log.count - 1;
            if (old != w->log.cursor) psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonTrg & PAD_BUTTON_A) {
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 30;
            }
            w->log.menu[w->log.cursor].state = 10;
            w->log.state = w->log.menu[w->log.cursor].id + 10;
            switch (w->log.menu[w->log.cursor].id) {
            case 0:
                w->log.mapWinTgtY = 20.0f;
                break;
            case 1:
                w->log.stoneWinTgtY = 20.0f;
                w->log.stoneAlphaTgt = 255.0f;
                break;
            case 2:
                w->log.badge.tx = -130.0f;
                w->log.badge.ty = 120.0f;
                w->log.badge.iconTx = -200.0f;
                w->log.badge.iconTy = 60.0f;
                break;
            case 3:
                w->log.recipe.tx = -130.0f;
                w->log.recipe.ty = 120.0f;
                w->log.recipe.iconTx = -200.0f;
                w->log.recipe.iconTy = 60.0f;
                break;
            case 4:
                break;
            }
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
        }
        w->helpTgtX = -128.0f;
        w->helpTgtY = 8.0f + (45.0f * w->log.count / 2.0f - 45.0f * w->log.cursor);
        winMsgEntry(w, 0, w->log.menu[w->log.cursor].msg, 0);
        break;
    case 10:
#if VERSION <= VERSION_DEMO_JP
        if (fileAsyncf(FILE_TYPE_TPL, NULL, WIN_DIR "jp/%s", getMarioStDvdRoot(), winGetMapTplName()) == 0U)
                break;
        if (!w->mapRes) {
            w->mapRes = fileAllocf(FILE_TYPE_TPL, WIN_DIR "jp/%s", getMarioStDvdRoot(), winGetMapTplName());
#else
#if VERSION >= VERSION_US
        if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/%s/%s", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us", winGetMapTplName()) == 0U) break;
#else
        if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/jp/%s", getMarioStDvdRoot(), winGetMapTplName()) == 0U) break;
#endif
        if (!w->mapRes) {
#if VERSION >= VERSION_US
            w->mapRes = fileAllocf(FILE_TYPE_TPL, "%s/w/%s/%s", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us", winGetMapTplName());
#else
            w->mapRes = fileAllocf(FILE_TYPE_TPL, "%s/w/jp/%s", getMarioStDvdRoot(), winGetMapTplName());
#endif
#endif
        }
        if (w->log.pose == -1) {
            if (w->flags & WINDOW_FLAG_8BIT) {
                anim = "W_1";
                w->log.pose = animPoseEntry("d_mario", 0);
                switch (marioGetColor()) {
                case 0:
                    anim = "W_1";
                    break;
                case 1:
                    anim = "W_15";
                    break;
                case 2:
                    anim = "W_17";
                    break;
                case 3:
                    anim = "W_18";
                    break;
                }
                animPoseSetAnim(w->log.pose, anim, 1);
            } else {
                model = "a_mario";
                switch (marioGetColor()) {
                case 0:
                    model = "a_mario";
                    break;
                case 1:
                    model = "a_mario_l";
                    break;
                case 2:
                    model = "a_mario_w";
                    break;
                case 3:
                    model = "a_mario_wl";
                    break;
                }
                w->log.pose = animPoseEntry(model, 0);
                animPoseSetAnim(w->log.pose, "M_W_1", 1);
            }
            animPoseSetMaterialFlagOn(w->log.pose, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        }
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 40;
            }
            w->log.menu[w->log.cursor].state = 20;
            w->log.state = 0;
            w->log.mapWinTgtY = -480.0f;
            fileFree(w->mapRes);
            w->mapRes = NULL;
            if (w->log.pose != -1) animPoseRelease(w->log.pose);
            w->log.pose = -1;
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        min = 10000.0f;
        x = -230.0f + w->log.scrollX + w->log.mapX;
        y = -120.0f + w->log.scrollY + w->log.mapY;
        bx = 74.0f + w->log.scrollX;
        by = 120.0f + w->log.scrollY;
#if VERSION <= VERSION_DEMO_JP
        for (k = 0; k < 0x5D; k++) {
            t = &tubuDt[k];
            if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < t->progress)
                break;
            {
#else
        for (k = 0; k < sizeof(tubuDt) / sizeof(TubuDt); k++) {
            t = &tubuDt[k];
            if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= t->progress || tubuCheck(t->type)) {
#endif
                if (t->big) {
                    f32 d = distABf(bx + t->x, by + t->y, x, y);
                    if (min > d) {
                        min = d;
                        sel = k;
                    }
                }
            }
        }
        if (min < 50.0f) {
            sprintf(name, "menu_monosiri_%s", tubuDt[sel].map);
            sprintf(help, "menu_map_%s", tubuDt[sel].map);
            if (w->log.mapSel != sel) psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
            w->log.mapSel = sel;
            winMsgEntry(w, 0, help, name);
        } else {
            w->log.mapSel = -1;
            winMsgEntry(w, 0, "msg_menu_kiroku_map", 0);
        }
        dx = keyGetStickX(0) / 8.0f;
        dy = keyGetStickY(0) / 8.0f;
        if (dx == 0.0f && dy == 0.0f) {
            if (keyGetButton(0) & PAD_BUTTON_LEFT) {
                dx = -8.0f;
            } else if (keyGetButton(0) & PAD_BUTTON_RIGHT) {
                dx = 8.0f;
            }
            if (keyGetButton(0) & PAD_BUTTON_UP) {
                dy = 8.0f;
            } else if (keyGetButton(0) & PAD_BUTTON_DOWN) {
                dy = -8.0f;
            }
        }
        if (dx == 0.0f && dy == 0.0f && w->log.mapSel != -1) {
            dx = 304.0f + tubuDt[w->log.mapSel].x - w->log.mapX;
            dy = 240.0f + tubuDt[w->log.mapSel].y - w->log.mapY;
            dx *= 0.2f;
            dy *= 0.2f;
        }
        w->log.mapX += dx;
        w->log.mapY += dy;
        if (w->log.mapX < 16.0f) w->log.mapX = 16.0f;
        if (w->log.mapY < 8.0f) w->log.mapY = 8.0f;
        if (w->log.mapX >= 604.0f) w->log.mapX = 604.0f;
        if (w->log.mapY >= 468.0f) w->log.mapY = 468.0f;
        ix = w->log.mapX + w->log.scrollX - 230.0f;
        iy = w->log.mapY + w->log.scrollY - 120.0f;
        if (ix >= 166.0f) w->log.scrollX -= ix - 166.0f;
        if (ix <= -150.0f) w->log.scrollX -= 150.0f + ix;
        if (iy >= 80.0f) w->log.scrollY -= iy - 80.0f;
        if (iy <= -80.0f) w->log.scrollY -= 80.0f + iy;
        if (w->log.scrollX > 0.0f) w->log.scrollX = 0.0f;
        if (w->log.scrollY > 0.0f) w->log.scrollY = 0.0f;
        if (w->log.scrollX < -148.0f) w->log.scrollX = -148.0f;
        if (w->log.scrollY < -240.0f) w->log.scrollY = -240.0f;
        w->helpTgtX = -230.0f + w->log.scrollX + w->log.mapX;
        w->helpTgtY = -120.0f + w->log.scrollY + w->log.mapY;
        w->helpTgtX -= 24.0f;
        w->helpTgtY += 20.0f;
        break;
    case 11:
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 40;
            }
            w->log.menu[w->log.cursor].state = 20;
            w->log.state = 0;
            w->log.stoneWinTgtY = -480.0f;
            w->log.stoneAlphaTgt = 0.0f;
        } else if (w->dirRep & DEMO_STICK_LEFT) {
            w->log.stoneSel--;
            while (w->log.stoneSel < 0) w->log.stoneSel += 7;
            w->log.stoneAngleTgt = 51.42857f * w->log.stoneSel;
            while (w->log.stoneAngle < w->log.stoneAngleTgt) w->log.stoneAngle += 360.0f;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SSTONE_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_RIGHT) {
            w->log.stoneSel++;
            while (w->log.stoneSel > 6) w->log.stoneSel -= 7;
            w->log.stoneAngleTgt = 51.42857f * w->log.stoneSel;
            while (w->log.stoneAngle > w->log.stoneAngleTgt) w->log.stoneAngle -= 360.0f;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_SSTONE_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        w->helpTgtY = w->helpTgtX = -50.0f;
        if (w->log.stoneSel < w->log.stoneNum) {
            winMsgEntry(w, -1, itemDataTable[stone_tbl[w->log.stoneSel]].description, itemDataTable[stone_tbl[w->log.stoneSel]].name);
        } else {
            winMsgEntry(w, -1, "msg_menu_stone_none_help", "msg_menu_stone_none_name");
        }
        break;
    case 12:
        pages = (w->log.badge.num + 27) / 28;
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 40;
            }
            w->log.menu[w->log.cursor].state = 20;
            w->log.state = 0;
            w->log.badge.tx = 330.0f;
            w->log.badge.ty = -250.0f;
            w->log.badge.iconTx = -200.0f;
            w->log.badge.iconTy = -300.0f;
        } else if (w->buttonRep & PAD_TRIGGER_L) {
            w->log.badge.idx--;
            if (w->log.badge.idx < 0) w->log.badge.idx = 0;
            w->log.badge.cursor = w->log.badge.idx * 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonRep & PAD_TRIGGER_R) {
            w->log.badge.idx++;
            if (w->log.badge.idx >= pages) w->log.badge.idx = pages - 1;
            w->log.badge.cursor = w->log.badge.idx * 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_RIGHT) {
            if (w->log.badge.cursor % 7 < 6 && w->log.badge.cursor + 1 < w->log.badge.num) w->log.badge.cursor++;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_LEFT) {
            if (w->log.badge.cursor % 7 > 0) w->log.badge.cursor--;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_UP) {
            if ((s32)(w->log.badge.cursor - 7) >= 0 && w->log.badge.cursor - 7 < w->log.badge.num) {
                w->log.badge.cursor -= 7;
            }
            w->log.badge.idx = w->log.badge.cursor / 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_DOWN) {
            if ((s32)(w->log.badge.cursor + 7) >= 0 && w->log.badge.cursor + 7 < w->log.badge.num) {
                w->log.badge.cursor += 7;
            } else if (w->log.badge.cursor + (7 - w->log.badge.cursor % 7) < w->log.badge.num) {
                w->log.badge.cursor = w->log.badge.num - 1;
            }
            w->log.badge.idx = w->log.badge.cursor / 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonTrg & PAD_TRIGGER_Z) {
            winSortEntry(w, 4, -310.0f, 150.0f);
            w->savedState = w->log.state;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
            w->log.state = 1000;
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        w->helpTgtX = w->log.badge.cursor % 28 % 7 * 56 - 140;
        w->helpTgtY = 90 - w->log.badge.cursor % 28 / 7 * 56;
        if (swGet(w->log.badgeIds[w->log.badge.cursor] + GSWF_BADGE_FOUND_BASE)) {
            winMsgEntry(w, -2, itemDataTable[w->log.badgeIds[w->log.badge.cursor] + BADGE_MIN].menu_description, itemDataTable[w->log.badgeIds[w->log.badge.cursor] + BADGE_MIN].name);
        } else {
            winMsgEntry(w, -2, "msg_menu_stone_none_help", "msg_menu_stone_none_name");
        }
        break;
    case 13:
        pages = (w->log.recipe.num + 27) / 28;
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 40;
            }
            w->log.menu[w->log.cursor].state = 20;
            w->log.state = 0;
            w->log.recipe.tx = 330.0f;
            w->log.recipe.ty = -250.0f;
            w->log.recipe.iconTx = -200.0f;
            w->log.recipe.iconTy = -300.0f;
        } else if (w->buttonRep & PAD_TRIGGER_L) {
            w->log.recipe.idx--;
            if (w->log.recipe.idx < 0) w->log.recipe.idx = 0;
            w->log.recipe.cursor = w->log.recipe.idx * 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonRep & PAD_TRIGGER_R) {
            w->log.recipe.idx++;
            if (w->log.recipe.idx >= pages) w->log.recipe.idx = pages - 1;
            w->log.recipe.cursor = w->log.recipe.idx * 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_RIGHT) {
            if (w->log.recipe.cursor % 7 < 6 && w->log.recipe.cursor + 1 < w->log.recipe.num) w->log.recipe.cursor++;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_LEFT) {
            if (w->log.recipe.cursor % 7 > 0) w->log.recipe.cursor--;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_UP) {
            if ((s32)(w->log.recipe.cursor - 7) >= 0 && w->log.recipe.cursor - 7 < w->log.recipe.num) {
                w->log.recipe.cursor -= 7;
            }
            w->log.recipe.idx = w->log.recipe.cursor / 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_DOWN) {
            if ((s32)(w->log.recipe.cursor + 7) >= 0 && w->log.recipe.cursor + 7 < w->log.recipe.num) {
                w->log.recipe.cursor += 7;
            } else if (w->log.recipe.cursor + (7 - w->log.recipe.cursor % 7) < w->log.recipe.num) {
                w->log.recipe.cursor = w->log.recipe.num - 1;
            }
            w->log.recipe.idx = w->log.recipe.cursor / 28;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonTrg & PAD_TRIGGER_Z) {
            winSortEntry(w, 5, -310.0f, 150.0f);
            w->savedState = w->log.state;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
            w->log.state = 1000;
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        w->helpTgtX = w->log.recipe.cursor % 28 % 7 * 56 - 140;
        w->helpTgtY = 90 - w->log.recipe.cursor % 28 / 7 * 56;
        if (swGet(w->log.recipeIds[w->log.recipe.cursor] + GSWF_RECIPE_FOUND_BASE)) {
            winMsgEntry(w, -2, itemDataTable[w->log.recipeIds[w->log.recipe.cursor] + ITEM_SHROOM_FRY].menu_description, itemDataTable[w->log.recipeIds[w->log.recipe.cursor] + ITEM_SHROOM_FRY].name);
        } else {
            winMsgEntry(w, -2, "msg_menu_stone_none_help", "msg_menu_stone_none_name");
        }
        break;
    case 14:
        pages = (w->log.enemyNum + 15) / 16;
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            for (i = 0; i < w->log.count; i++) {
                w->log.menu[i].state = 40;
            }
            w->log.menu[w->log.cursor].state = 20;
            w->log.state = 0;
            w->log.enemyTx = 320.0f;
            w->log.enemyTy = -240.0f;
        } else if (w->buttonRep & PAD_TRIGGER_L) {
            w->log.enemyPage--;
            if (w->log.enemyPage < 0) w->log.enemyPage = 0;
            w->log.enemySel = w->log.enemyPage * 16;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonRep & PAD_TRIGGER_R) {
            w->log.enemyPage++;
            if (w->log.enemyPage >= pages) w->log.enemyPage = pages - 1;
            w->log.enemySel = w->log.enemyPage * 16;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
            if (w->log.enemySel % 16 < 8) {
                w->log.enemySel += 8;
            } else {
                w->log.enemySel -= 8;
            }
            if (w->log.enemySel >= w->log.enemyNum) w->log.enemySel = w->log.enemyNum - 1;
            if (w->log.enemySel < 0) w->log.enemySel = 0;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_UP) {
            if (w->log.enemySel % 8 == 0) {
                w->log.enemySel -= 9;
            } else {
                w->log.enemySel -= 1;
            }
            if (w->log.enemySel < 0) w->log.enemySel = 0;
            w->log.enemyPage = w->log.enemySel / 16;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->dirRep & DEMO_STICK_DOWN) {
            if (w->log.enemySel % 8 == 7) {
                w->log.enemySel += 9;
            } else {
                w->log.enemySel += 1;
            }
            if (w->log.enemySel >= w->log.enemyNum) w->log.enemySel = w->log.enemyNum - 1;
            w->log.enemyPage = w->log.enemySel / 16;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_TABI_MOVE1 | PSND_SFX_BYPASS_MUTE));
        } else if (w->buttonTrg & PAD_BUTTON_A) {
            if (swGet(w->log.enemy[w->log.enemySel].id + GSWF_TATTLE_BASE)) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                w->log.state = 21;
                w->log.mono = monosiriInit(w->log.enemy[w->log.enemySel].id, -60.0f, 150.0f);
            }
        } else if (w->buttonTrg & PAD_TRIGGER_Z) {
            winSortEntry(w, 6, -310.0f, 150.0f);
            w->savedState = w->log.state;
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
            w->log.state = 1001;
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        w->helpTgtX = w->log.enemySel % 16 / 8 * 216 - 180;
        w->helpTgtY = 106 - w->log.enemySel % 16 % 8 * 24;
        if (swGet(w->log.enemy[w->log.enemySel].id + GSWF_TATTLE_BASE)) {
            winMsgEntry(w, 0, battleGetUnitMonosiriPtr(w->log.enemy[w->log.enemySel].id)->tattleLogTag, 0);
        } else {
            winMsgEntry(w, 0, "msg_menu_monosiri_enemy", 0);
        }
        break;
    case 21:
        if (w->buttonTrg & PAD_BUTTON_B) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            w->log.state = 22;
            monosiriClose(w->log.mono);
        } else if (w->buttonTrg & PAD_BUTTON_START) {
            monosiriExit(w->log.mono);
            return -2;
        } else {
            monosiriMain(w->log.mono);
        }
        break;
    case 22:
        if (monosiriCloseWait(w->log.mono)) {
            w->log.state = 14;
            monosiriExit(w->log.mono);
        } else {
            monosiriMain(w->log.mono);
        }
        break;
    case 1000:
    case 1001:
        if (!winSortWait(w)) {
            w->log.state = w->savedState;
        }
        if (w->buttonTrg & PAD_BUTTON_START) {
            return -2;
        }
        break;
    }
    return 0;
}


/**
 * Every frame: runs the entry buttons' slide animations (chosen one to the top-left, the rest off screen,
 * and back) and eases the map, Crystal Star, badge and recipe panels toward their targets.
 */
void winLogMain2(WindowWork* w) {
    s32 i;

    for (i = 0; i < w->log.count; i++) {
        switch (w->log.menu[i].state) {
        case 10:
            w->log.menu[i].tx = 0.0f;
            w->log.menu[i].ty = 140.0f;
            w->log.menu[i].timer = 0;
            w->log.menu[i].state++;
            break;
        case 11:
            if (++w->log.menu[i].timer > 8) {
                w->log.menu[i].tx = -160.0f;
                w->log.menu[i].ty = 140.0f;
                w->log.menu[i].timer = 0;
                w->log.menu[i].state++;
            }
            break;
        case 20:
            w->log.menu[i].tx = 0.0f;
            w->log.menu[i].ty = 140.0f;
            w->log.menu[i].timer = 0;
            w->log.menu[i].state++;
            break;
        case 21:
            if (++w->log.menu[i].timer > 8) {
                w->log.menu[i].tx = 0.0f;
                w->log.menu[i].ty = 45.0f * w->log.count / 2.0f - 45.0f * i;
                w->log.menu[i].timer = 0;
                w->log.menu[i].state++;
            }
            break;
        case 30:
            w->log.menu[i].tx = 0.0f;
            w->log.menu[i].ty = -160.0f;
            w->log.menu[i].timer = 0;
            w->log.menu[i].state++;
            break;
        case 0:
        case 41:
            break;
        case 40:
            w->log.menu[i].tx = 0.0f;
            w->log.menu[i].ty = 45.0f * w->log.count / 2.0f - 45.0f * i;
            w->log.menu[i].timer = 0;
            w->log.menu[i].state++;
            break;
        }
        w->log.menu[i].x += (w->log.menu[i].tx - w->log.menu[i].x) / FPS_EASE(8.0f);
        w->log.menu[i].y += (w->log.menu[i].ty - w->log.menu[i].y) / FPS_EASE(8.0f);
    }
    w->log.mapWinX += (w->log.mapWinTgtX - w->log.mapWinX) / FPS_EASE_D(8.0f, 8.0);
    w->log.mapWinY += (w->log.mapWinTgtY - w->log.mapWinY) / FPS_EASE_D(8.0f, 8.0);
    w->log.stoneWinX += (w->log.stoneWinTgtX - w->log.stoneWinX) / FPS_EASE_D(8.0f, 8.0);
    w->log.stoneWinY += (w->log.stoneWinTgtY - w->log.stoneWinY) / FPS_EASE_D(8.0f, 8.0);
    w->log.stoneAlpha += (w->log.stoneAlphaTgt - w->log.stoneAlpha) / FPS_EASE_D(8.0f, 8.0);
    w->log.stoneAngle += (w->log.stoneAngleTgt - w->log.stoneAngle) / FPS_EASE(4.0f);
    w->log.badge.scrollTgt = w->log.badge.idx * 0xE8;
    w->log.badge.scroll += (w->log.badge.scrollTgt - w->log.badge.scroll) / FPS_EASE(6.0f);
    w->log.badge.x += (w->log.badge.tx - w->log.badge.x) / FPS_EASE(6.0f);
    w->log.badge.y += (w->log.badge.ty - w->log.badge.y) / FPS_EASE(6.0f);
    w->log.badge.iconX += (w->log.badge.iconTx - w->log.badge.iconX) / FPS_EASE(6.0f);
    w->log.badge.iconY += (w->log.badge.iconTy - w->log.badge.iconY) / FPS_EASE(6.0f);
    w->log.recipe.scrollTgt = w->log.recipe.idx * 0xE8;
    w->log.recipe.scroll += (w->log.recipe.scrollTgt - w->log.recipe.scroll) / FPS_EASE(6.0f);
    w->log.recipe.x += (w->log.recipe.tx - w->log.recipe.x) / FPS_EASE(6.0f);
    w->log.recipe.y += (w->log.recipe.ty - w->log.recipe.y) / FPS_EASE(6.0f);
    w->log.recipe.iconX += (w->log.recipe.iconTx - w->log.recipe.iconX) / FPS_EASE(6.0f);
    w->log.recipe.iconY += (w->log.recipe.iconTy - w->log.recipe.iconY) / FPS_EASE(6.0f);
}

static void mapGX(WindowWork* w, f32 x, f32 y);
static void monoshiriGX(WindowWork* w, f32 x, f32 y);


/**
 * Draws the Journal (menu slot idx): the map panel,
 * the Crystal Star carousel (positions the star effects with shadows),
 * the badge and recipe boxes with the selected icon (and found counts from the US release on),
 * the Tattle log list, and the entry buttons.
 */
void winLogDisp(CameraId cam, void* param, s32 idx) {
    static const GXColor kW = {0xFF, 0xFF, 0xFF, 0xFF};
    WindowWork* w = param;
    GXColor c3;
    GXColor c2;
    Mtx m1;
    Mtx m2;
    f32 x;
    f32 y;
    s32 i;
    EffStarStoneWork* wk;
    c2 = kW;
    c3 = kW;
    x = w->menu[idx].x;
    y = w->menu[idx].y;

    winBgGX(w, 4, x, y);
    if (y + w->log.mapWinY > -480.0f) {
        mapGX(w, x + w->log.mapWinX, y + w->log.mapWinY);
    }
    if (y + w->log.stoneWinY > -480.0f) {
        winTexInit(*w->res->data);
        winTexSet(0x24, (Vec){x + w->log.stoneWinX, y + w->log.stoneWinY, 0.0f}, (Vec){60.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, w->log.stoneAlpha});
        for (i = 0; i < 7; i++) {
            Vec v = {0.0f, 0.0f, 170.0f};
            PSMTXRotRad(m1, 'x', 0.43633232f);
            PSMTXRotRad(m2, 'y', 0.017453292f * (51.42857f * i - w->log.stoneAngle));
            PSMTXMultVec(m2, &v, &v);
            PSMTXMultVec(m1, &v, &v);
#if VERSION >= VERSION_JP
            winKageGX(w, v.x + (x + w->log.stoneWinX), (v.y + (y + w->log.stoneWinY)) - 30.0f, v.z, 1.0f, (GXColor){0x00, 0x00, 0x00, 0x80});
#endif
            // FAKE: wk is reloaded before each position store; one load changes the object
            wk = w->log.eff[i]->userdata;
            wk->position.x = v.x + (x + w->log.stoneWinX);
            wk = w->log.eff[i]->userdata;
            wk->position.y = 20.0f + (v.y + (y + w->log.stoneWinY));
            wk = w->log.eff[i]->userdata;
            wk->position.z = v.z;
#if VERSION <= VERSION_DEMO_JP
            winKageGX(w, v.x + (x + w->log.stoneWinX), (v.y + (y + w->log.stoneWinY)) - 30.0f, v.z, 1.0f, (GXColor){0x00, 0x00, 0x00, 0x80});
#else
        }
        for (i = 0; i < 7; i++) {
            effStarStoneDraw(CAMERA_2D, w->log.eff[i]);
#endif
        }
    }
    if (w->log.badge.x < 320.0f && w->log.badge.y > -240.0f) {
        winHakoGX(w, 0, x + w->log.badge.x, y + w->log.badge.y);
#if VERSION >= VERSION_US
        {
            GXColor black;
            char buf[0x100];
            u16 width;

            black = (GXColor){0x00, 0x00, 0x00, 0xFF};
            winKirinukiGX(w, 0, (x + w->log.badge.iconX) - 50.0f, (y + w->log.badge.iconY) - 120.0f, 100.0f, 30.0f);
            sprintf(buf, gp->language != LANGUAGE_JP ? "%d/%d" : "%d\201^%d", w->log.badge.page, w->log.badge.num);
            width = 0.75f * FontGetMessageWidth(buf);
            winFontInit();
            winFontSet((Vec){(f32)((100 - width) / 2) + ((x + w->log.badge.iconX) - 50.0f), (y + w->log.badge.iconY) - 128.0f, 0.0f},
                       (Vec){0.75f, 0.75f, 0.75f}, black, buf);
        }
#endif
        winTexInit(*w->res->data);
        winTexSet(0x95, (Vec){x + w->log.badge.iconX, y + w->log.badge.iconY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c2);
        if (swGet(w->log.badgeIds[w->log.badge.cursor] + GSWF_BADGE_FOUND_BASE)) {
            winIconInit();
            winIconSet(itemDataTable[w->log.badgeIds[w->log.badge.cursor] + BADGE_MIN].icon_id, (Vec){x + w->log.badge.iconX, (y + w->log.badge.iconY) - 4.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c2);
        }
    }
    if (w->log.recipe.x < 320.0f && w->log.recipe.y > -240.0f) {
        winHakoGX(w, 1, x + w->log.recipe.x, y + w->log.recipe.y);
#if VERSION >= VERSION_US
        {
            GXColor black;
            char buf[0x100];
            u16 width;

            black = (GXColor){0x00, 0x00, 0x00, 0xFF};
            winKirinukiGX(w, 0, (x + w->log.recipe.iconX) - 50.0f, (y + w->log.recipe.iconY) - 120.0f, 100.0f, 30.0f);
            sprintf(buf, gp->language != LANGUAGE_JP ? "%d/%d" : "%d\201^%d", w->log.recipe.page, w->log.recipe.num);
            width = 0.75f * FontGetMessageWidth(buf);
            winFontInit();
            winFontSet((Vec){(f32)((100 - width) / 2) + ((x + w->log.recipe.iconX) - 50.0f), (y + w->log.recipe.iconY) - 128.0f, 0.0f},
                       (Vec){0.75f, 0.75f, 0.75f}, black, buf);
        }
#endif
        winTexInit(*w->res->data);
        winTexSet(0x96, (Vec){x + w->log.recipe.iconX, y + w->log.recipe.iconY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c2);
        if (swGet(w->log.recipeIds[w->log.recipe.cursor] + GSWF_RECIPE_FOUND_BASE)) {
            winIconInit();
            winIconSet(itemDataTable[w->log.recipeIds[w->log.recipe.cursor] + ITEM_SHROOM_FRY].icon_id, (Vec){x + w->log.recipe.iconX, (y + w->log.recipe.iconY) - 10.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c2);
        }
    }
    if (w->log.state == 14 || (u32)(w->log.state - 20) <= 2 || w->log.state == 1001) {
        monoshiriGX(w, x - 170.0f, 130.0f + y);
    }
    for (i = 0; i < w->log.count; i++) {
        if (i == w->log.cursor) {
            winTexInit(*w->res->data);
            winTexSet(0xB5, (Vec){x + w->log.menu[i].x, y + w->log.menu[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x00, 0x00, 0x00, 0x80});
            winTexInit_x2(*w->res->data);
            winTexSet_x2(w->log.menu[i].id + 0x1F, 0xB5, (Vec){(x + w->log.menu[i].x) - 8.0f, 8.0f + (y + w->log.menu[i].y), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, c3);
        } else {
            winTexInit_x2(*w->res->data);
            winTexSet_x2(w->log.menu[i].id + 0x1F, 0xB5, (Vec){x + w->log.menu[i].x, y + w->log.menu[i].y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0x80, 0x80, 0x80, 0xFF});
        }
    }
}

/**
 * Returns the world map texture for the story so far:
 * map0.tpl to map7.tpl, one step per chapter, each switching at that chapter's visit to Frankly's house.
 * Preloaded by winMain and loaded by the Journal map.
 */
const char* winGetMapTplName(void) {
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG0_16_FRANKLYS_HOUSE) return "map0.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG1_31_FRANKLYS_HOUSE_STG_1) return "map1.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG2_41_FRANKLYS_HOUSE_STG_2) return "map2.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG3_51_FRANKLYS_HOUSE_STG_3) return "map3.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG4_31_FRANKLYS_HOUSE_STG_4) return "map4.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG5_61_FRANKLYS_HOUSE_STG_5) return "map5.tpl";
    if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < STG6_71_FRANKLYS_HOUSE_STG_6) return "map6.tpl";
    return "map7.tpl";
}

/** Local fabs on the __fabs intrinsic, used for the map arrows' bobbing offset. */
static inline double fabs(double x) { return __fabs(x); }


/**
 * Draws the world map panel: the scrolled map texture,
 * the route dots revealed so far (the hovered place blinks), the current chapter's goal icon,
 * Mario's marker at his location, the frame, and bobbing arrows on the sides that can still scroll.
 */
static void mapGX(WindowWork* w, f32 x, f32 y) {
    static f32 pos_tbl[7][2] = {
        {165.0f, 68.0f}, {0.0f, 75.0f}, {97.0f, 150.0f}, {-139.0f, -33.0f},
        {41.0f, -163.0f}, {211.0f, -91.0f}, {-209.0f, 159.0f},
    };
    GXColor c2;
    GXColor c0;

    // FAKE: c2 and c0 are never read; GXColor white = {0xFF, 0xFF, 0xFF, 0xFF} alone changes the object
    GXColor white = (c2 = (c0 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF}));
    s32 tex;
    Mtx m;
    f32 top;
    f32 mx;
    f32 my;
    f32 s;
    f32 o;
    f32 scale;
    TubuDt* t;
    s32 i;
    s32 id;
    s32 k;
    s32 sw;
    AnimData* data;

    mx = 74.0f + (x + w->log.scrollX);
    my = 120.0f + (y + w->log.scrollY);
    if (w->mapRes) {
        top = 120.0f + y;
        GXSetScissor(((304.0f + x) - 230.0f) - 2.0f, (240.0f - top) - 2.0f, 0x1D0, 0xF4);
        winTexInit(*(TEXPalette**)w->mapRes->data);
        winTexSet(0, (Vec){mx, my, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        winTexInit(*w->res->data);
        for (i = 0; i < sizeof(tubuDt) / sizeof(TubuDt); i++) {
            t = &tubuDt[i];
#if VERSION <= VERSION_DEMO_JP
            if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) < t->progress)
                break;
            {
#else
            if (swByteGet(GSW_ID(GSW_STORY_PROGRESS)) >= t->progress || tubuCheck(t->type)) {
#endif
                if (w->log.mapSel == i) {
                    // FAKE: the texture id goes through a spare temporary tex in a comma expression; ((gp->retraceCount >> 4) & 1) ? 0xB9 : 0xBA changes the object
                    winTexSet((tex = 0xBA, ((gp->retraceCount >> 4) & 1) ? tex = 0xB9 : 0, tex), (Vec){mx + t->x, my + t->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
                } else {
                    winTexSet(t->big ? 0xBA : 0xB8, (Vec){mx + t->x, my + t->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
                }
            }
        }
        sw = swByteGet(GSW_ID(GSW_STORY_PROGRESS));
        id = -1;
        if (sw >= STG0_16_FRANKLYS_HOUSE && sw < STG1_16_HOOKTAIL) {
            id = ICON_SAC_DEKA;
            k = 0;
        } else if (sw >= STG1_31_FRANKLYS_HOUSE_STG_1 && sw < STG2_25_AFTER_THE_BOSS_BATTLE) {
            id = ICON_SAC_BAKUGAME;
            k = 1;
        } else if (sw >= STG2_41_FRANKLYS_HOUSE_STG_2 && sw < STG3_32_BOSS_BATTLE) {
#if VERSION <= VERSION_DEMO_JP
            id = ICON_SAC_SUKI;
            k = 2;
        } else if (sw >= STG3_51_FRANKLYS_HOUSE_STG_3 && sw < STG4_15_03_THE_RUBY_STAR) {
            id = ICON_SAC_MUKI;
            k = 3;
        } else if (sw >= STG4_31_FRANKLYS_HOUSE_STG_4 && sw < STG5_43_X_NAUTS_RETREAT) {
            id = ICON_SAC_ZUBASTAR;
            k = 4;
        } else if (sw >= STG5_61_FRANKLYS_HOUSE_STG_5 && sw < STG6_53_THE_REAL_GARNET_STAR) {
            id = ICON_SAC_SCISSOR;
            k = 5;
        } else if (sw >= STG6_71_FRANKLYS_HOUSE_STG_6 && sw < STG7_20_LORD_CRUMP_DEFEATED) {
            id = ICON_SAC_GENKI1;
#else
            id = ICON_SAC_MUKI;
            k = 2;
        } else if (sw >= STG3_51_FRANKLYS_HOUSE_STG_3 && sw < STG4_15_03_THE_RUBY_STAR) {
            id = ICON_SAC_SCISSOR;
            k = 3;
        } else if (sw >= STG4_31_FRANKLYS_HOUSE_STG_4 && sw < STG5_43_X_NAUTS_RETREAT) {
            id = ICON_SAC_GENKI1;
            k = 4;
        } else if (sw >= STG5_61_FRANKLYS_HOUSE_STG_5 && sw < STG6_53_THE_REAL_GARNET_STAR) {
            id = ICON_SAC_SUKI;
            k = 5;
        } else if (sw >= STG6_71_FRANKLYS_HOUSE_STG_6 && sw < STG7_20_LORD_CRUMP_DEFEATED) {
            id = ICON_SAC_ZUBASTAR;
#endif
            k = 6;
        }
        if (id != -1) {
            winIconInit();
            winIconSet(id, (Vec){mx + pos_tbl[k][0], 45.0f + (my + pos_tbl[k][1]), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        }
        if (w->log.pose != -1) {
            // BUG: reads the loop length of the Mario page's pose (mario.poseId), not of the map marker's own pose
            // (log.pose), so the marker's local time wraps at another animation's length.
            animPoseGetAnimPosePtr(w->mario.poseId);
            data = animPoseGetAnimDataPtr(w->mario.poseId);
            scale = 1.5f;
            if (w->flags & WINDOW_FLAG_8BIT) {
                scale = 3.0f;
            }
            PSMTXTrans(m, mx + tubuDt[w->log.mapIdx].x, my + tubuDt[w->log.mapIdx].y, 0.0f);
            animPoseSetLocalTime(w->log.pose, gp->retraceCount % (s32)data->loopInfo->end);
            animPoseMain(w->log.pose);
            animPoseDrawMtx(w->log.pose, m, 1, 0.0f, scale);
            animPoseDrawMtx(w->log.pose, m, 2, 0.0f, scale);
            animPoseDrawMtx(w->log.pose, m, 3, 0.0f, scale);
        }
        GXSetScissor(0, 0, 0x260, 0x1E0);
        winTexInit(*w->res->data);
        winTexSet(0xBF, (Vec){(x - 230.0f) - 4.0f, y, 0.0f}, (Vec){1.0f, 32.0f, 1.0f}, white);
        winTexSet(0xBF, (Vec){4.0f + (230.0f + x), y, 0.0f}, (Vec){1.0f, 32.0f, 1.0f}, white);
        winTexSet(0xBF, (Vec){x, 4.0f + top, 0.0f}, (Vec){59.5f, 1.0f, 1.0f}, white);
        winTexSet(0xBF, (Vec){x, (y - 120.0f) - 4.0f, 0.0f}, (Vec){59.5f, 1.0f, 1.0f}, white);
        s = fabs(2.0f * (f32)sin((f32)gp->retraceCount / 8.0f));
        winTexInit(*w->res->data);
        o = -240.0f == w->log.scrollY ? 0.0f : s;
        winTexSet(0xBD, (Vec){x, top + o, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        o = -148.0f == w->log.scrollX ? 0.0f : s;
        winTexInit(*w->res->data);
        winTexSet(0xBC, (Vec){(220.0f + x) + o, y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        o = 0.0f == w->log.scrollY ? 0.0f : s;
        winTexInit(*w->res->data);
        winTexSet(0xBE, (Vec){x, (y - 120.0f) - o, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        o = 0.0f == w->log.scrollX ? 0.0f : s;
        winTexInit(*w->res->data);
        winTexSet(0xBB, (Vec){(x - 220.0f) - o, y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    }
}


/**
 * Draws the Tattle log page: the known/total count,
 * 16 entries in two columns with their log numbers (unknown enemies as "???",
 * from JP on, Doopliss as "?" until his name is guessed), page arrows and the Z sort hint.
 */
static void monoshiriGX(WindowWork* w, f32 x, f32 y) {
    GXColor k1;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor k2;
    char buf[256];
    char num[32];
    s32 i;
    s32 id;
    u16 tw;
    const char* msg;
    const char* name;
    TattleEntry* te;
    f32 fw;
    f32 ix;

    k1 = (GXColor){0x00, 0x00, 0x00, 0xFF};
    k2 = k1;
    winBookGX(w, 1, x, y);
    winKirinukiGX(w, 0, x - 110.0f, y - 160.0f, 100.0f, 50.0f);
    msg = msgSearch("msg_menu_monosiri_kazu");
    tw = 0.75f * FontGetMessageWidth(msg);
#if VERSION >= VERSION_US
    if (tw > 80.0f) {
        tw = tw * (80.0f / tw);
    }
#endif
    winFontInit();
#if VERSION >= VERSION_US
    winFontSetWidth((Vec){(x - 108.0f) + (100 - tw) / 2, y - 166.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k1, 80.0f, msg);
    sprintf(buf, gp->language != LANGUAGE_JP ? "%d/%d" : "%d\201^%d", w->log.enemyKnown, w->log.enemyNum);
#else
    winFontSet((Vec){(x - 108.0f) + (100 - tw) / 2, y - 166.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k1, msg);
    sprintf(buf, "%d\201^%d", w->log.enemyKnown, w->log.enemyNum);
#endif
    tw = 0.75f * FontGetMessageWidth(buf);
    winFontSet((Vec){(x - 108.0f) + (100 - tw) / 2, y - 184.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k1, buf);
    winFontInit();
    for (i = 0; i < 16; i++) {
#if VERSION <= VERSION_DEMO_JP
        s32 id;
        id = w->log.enemy[w->log.enemyPage * 16 + i].id;
        te = battleGetUnitMonosiriPtr(id);
        if (w->log.enemyPage * 16 + i >= w->log.enemyNum)
            break;
        if (swGet(id + GSWF_TATTLE_BASE)) {
            {
#else
        id = w->log.enemy[w->log.enemyPage * 16 + i].id;
        te = battleGetUnitMonosiriPtr(id);
        if (w->log.enemyPage * 16 + i >= w->log.enemyNum) break;
        if (swGet(id + GSWF_TATTLE_BASE)) {
            if (id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || id == UNIT_DOOPLISS_CHAPTER8) {
                if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
                    name = msgSearch("btl_un_hatena");
                } else {
                    name = msgSearch("btl_un_rampell");
                }
            } else {
#endif
                name = msgSearch(te->enemyTag);
            }
        } else {
#if VERSION >= VERSION_US
            name = gp->language != LANGUAGE_JP ? "???" : "\201H\201H\201H";
#else
            name = "\201H\201H\201H";
#endif
        }
#if VERSION >= VERSION_US
        sprintf(num, gp->language != LANGUAGE_JP ? "%d. " : "%d\201D", enemy_monoshiri_sort_table[id]);
        winFontSetWidth((Vec){((24.0f + x) - 5.0f) + (i / 8) * 216, (y - 16.0f) - (i % 8) * 24, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k2, 40.0f, num);
        winFontSetWidth((Vec){(56.0f + x) + (i / 8) * 216, (y - 16.0f) - (i % 8) * 24, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k2, 145.0f, name);
#else
        sprintf(num, "%d\201D", enemy_monoshiri_sort_table[id]);
        winFontSet((Vec){((24.0f + x) - 5.0f) + (i / 8) * 216, (y - 16.0f) - (i % 8) * 24, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k2, num);
        winFontSet((Vec){(56.0f + x) + (i / 8) * 216, (y - 16.0f) - (i % 8) * 24, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, k2, name);
#endif
    }
    if (w->log.enemyPage > 0) {
        winTexInit(*w->res->data);
        winTexSet(0x17, (Vec){440.0f + x, 18.0f + y, 0.0f}, (Vec){-1.0f, -1.0f, -1.0f}, white);
        winIconInit();
        winIconSet(ICON_TRIGGER_L, (Vec){440.0f + x, y, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    }
    if (w->log.enemyPage < (w->log.enemyNum + 15) / 16 - 1) {
        winTexInit(*w->res->data);
        winTexSet(0x17, (Vec){440.0f + x, (y - 15.0f) - 240.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
        winIconInit();
        winIconSet(ICON_TRIGGER_R, (Vec){440.0f + x, y - 240.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    }
#if VERSION <= VERSION_DEMO_JP
    {
        const char* msg;
        msg = msgSearch("msg_menu_sort_narabikae");
        fw = 0.7f * FontGetMessageWidth(msg);
        winFontInit();
        ix = x + (420.0f - fw);
        winFontSet((Vec){ix, y - 210.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, k1, msg);
        winIconInit();
        winIconSet(ICON_TRIGGER_Z_ALT, (Vec){ix - 30.0f, y - 220.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    }
#else
    msg = msgSearch("msg_menu_sort_narabikae");
    fw = 0.7f * FontGetMessageWidth(msg);
    winFontInit();
    ix = x + (420.0f - fw);
    winFontSet((Vec){ix, y - 210.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, k1, msg);
    winIconInit();
    winIconSet(ICON_TRIGGER_Z_ALT, (Vec){ix - 30.0f, y - 220.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
#endif
}

MonosiriWork* monosiriInit(s32 id, f32 x, f32 y) {
    MonosiriWork* m = _mapAlloc(sizeof(MonosiriWork));
    memset(m, 0, sizeof(MonosiriWork));
    m->state = 0;
    m->id = id;
    m->x = x;
    m->y = y;
    m->curX = 500.0f;
    m->curY = -300.0f;
#if VERSION >= VERSION_JP
    m->alpha = 0;
    m->pose1 = -1;
    m->pose2 = -1;
#endif
    return m;
}

/**
 * Per-frame Tattle card update: loads win.tpl and mono.dat (per-enemy framing), allocates the portrait buffer,
 * loads the enemy's model (plus Smorg's extra part model) and fades it in,
 * registers capture and the card's draw, and slides the card toward its target (off screen once closing).
 */
void monosiriMain(MonosiriWork* m) {
    s32 group = gp->inBattle ? 1 : 0;
#if VERSION <= VERSION_DEMO_JP
    const char* lang[2] = {"jp", "us"};
    GXTexObj tex;
    TattleEntry* t;
    if (!m)
        return;
    switch (m->state) {
        case 0:
            if (fileAsyncf(FILE_TYPE_TPL, NULL, WIN_DIR "%s/win.tpl", getMarioStDvdRoot(), lang[gp->language]) == 0U)
                break;
            if (fileAsyncf(FILE_TYPE_RAW, NULL, WIN_DIR "jp/mono.dat", getMarioStDvdRoot()) == 0U)
            break;
            m->winTpl = fileAllocf(FILE_TYPE_TPL, WIN_DIR "%s/win.tpl", getMarioStDvdRoot(), lang[gp->language]);
            m->monoDat = fileAllocf(FILE_TYPE_RAW, WIN_DIR "jp/mono.dat", getMarioStDvdRoot());
            m->state++;
            m->alpha = 0;
            m->pose1 = -1;
            m->pose2 = -1;
#else
    const char* lang[VERSION_PICK(2, 6)] = {
        "jp", "us",
#if VERSION >= VERSION_US
        "ge", "fr", "sp", "it"
#endif
    };

    GXTexObj tex;
    TattleEntry* t;
    if (!m) return;
    switch (m->state) {
    case 0:
        if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/%s/win.tpl", getMarioStDvdRoot(), lang[gp->language]) == 0U) break;
#if VERSION >= VERSION_US
        if (fileAsyncf(FILE_TYPE_RAW, NULL, "%s/w/%s/mono.dat", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us") == 0U) break;
#else
        if (fileAsyncf(FILE_TYPE_RAW, NULL, "%s/w/jp/mono.dat", getMarioStDvdRoot()) == 0U) break;
#endif
        m->winTpl = fileAllocf(FILE_TYPE_TPL, "%s/w/%s/win.tpl", getMarioStDvdRoot(), lang[gp->language]);
#if VERSION >= VERSION_US
        m->monoDat = fileAllocf(FILE_TYPE_RAW, "%s/w/%s/mono.dat", getMarioStDvdRoot(), gp->language == LANGUAGE_JP ? "jp" : "us");
#else
        m->monoDat = fileAllocf(FILE_TYPE_RAW, "%s/w/jp/mono.dat", getMarioStDvdRoot());
#endif
        m->state++;
#endif
        TEXGetGXTexObjFromPalette(*(TEXPalette**)m->winTpl->data, &tex, 0x37);
        m->smart = smartAlloc(GXGetTexBufferSize(GXGetTexObjWidth(&tex), GXGetTexObjHeight(&tex), GX_TF_RGB5A3, GX_FALSE, 0), SMART_GROUP_PERMANENT);
        break;
    case 1:
        t = battleGetUnitMonosiriPtr(m->id);
        if (!animGroupBaseAsync(t->modelName, group, NULL)) break;
        if (m->id == UNIT_SMORG && !animGroupBaseAsync("c_n_moa_a", group, NULL)) break;
        m->alpha += 20;
        if (m->alpha > 255) m->alpha = 255;
        if (m->pose1 == -1) {
            if (m->id == UNIT_SMORG) {
                m->pose1 = animPoseEntry(t->modelName, group);
                animPoseSetAnim(m->pose1, t->poseName, 1);
                animPoseWorldPositionEvalOn(m->pose1);
                animPoseMain(m->pose1);
                m->pose2 = animPoseEntry("c_n_moa_a", group);
                animPoseSetAnim(m->pose2, "S_1", 1);
                animPoseMain(m->pose2);
            } else {
                m->pose1 = animPoseEntry(t->modelName, group);
                animPoseSetAnim(m->pose1, t->poseName, 1);
                animPoseMain(m->pose1);
            }
            dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_CUSTOM_FIRST, capture, m);
        }
        break;
    case 2:
        m->x = 600.0f;
        m->y = -400.0f;
        break;
    }
    m->curX += (m->x - m->curX) / FPS_EASE(6.0f);
    m->curY += (m->y - m->curY) / FPS_EASE(6.0f);
    if (m->state > 0) {
        dispEntry(CAMERA_2D, 0.0f, DISP_MODE_OPAQUE, monosiri_disp, m);
    }
}

/** Starts the Tattle card sliding off screen. */
void monosiriClose(MonosiriWork* m) {
    if (m) {
        m->state = 2;
    }
}

/** TRUE once the closing Tattle card is off screen (or there is no card). */
BOOL monosiriCloseWait(MonosiriWork* m) {
    if (!m) return TRUE;
    if (m->curX > 500.0f && m->curY < -300.0f) return TRUE;
    return FALSE;
}

/** Frees a Tattle card: its files, models, portrait buffer and the work itself. */
void monosiriExit(MonosiriWork* m) {
    if (m) {
        if (m->winTpl) {
            fileFree(m->winTpl);
#if VERSION >= VERSION_JP
            m->winTpl = NULL;
#endif
        }
        if (m->monoDat) {
            fileFree(m->monoDat);
#if VERSION >= VERSION_JP
            m->monoDat = NULL;
#endif
        }
        if (m->pose1 != -1) {
            animPoseRelease(m->pose1);
#if VERSION >= VERSION_JP
            m->pose1 = -1;
#endif
        }
        if (m->pose2 != -1) {
            animPoseRelease(m->pose2);
#if VERSION >= VERSION_JP
            m->pose2 = -1;
#endif
        }
#if VERSION >= VERSION_JP
        if (m->smart) {
            smartFree(m->smart);
            m->smart = NULL;
        }
#else
        smartFree(m->smart);
#endif
        _mapFree(m);
    }
}

typedef struct MonoDat {
    s32 id;
    f32 x;
    f32 y;
    f32 scale;
} MonoDat;

/* header of the monosiri position file (monoDat): entry table pointer, then its size in bytes */
typedef struct MonoDatFile {
    MonoDat* entries; // 0x0
    u32 size;         // 0x4
} MonoDatFile;

/**
 * Renders the Tattle card's enemy portrait off screen:
 * frames the model's bounding box (adjusted by its mono.dat entry), draws it,
 * and copies the result into the card's RGB5A3 texture buffer.
 * For Smorg (unit 0x6B) it also draws 500 small parts scattered along the body's chains.
 */
static void capture(CameraId camera, void* param) {
    MonosiriWork* m = param;
    AnimData* data;
    Vec min;
    Vec max;
    CameraEntry bak;
    Mtx44 pm;
    Mtx view;
    Mtx t;
    Mtx sm;
    Mtx t2;
    GXTexObj tex;
    f32 vp[6];
    f32 proj[7];
    Vec v2;
    Vec v1;
    u16 bb0, bb1, bb2, bb3;
    u32 sx, sy, sw, sh;
    f32 cx;
    f32 cy;
    f32 dist;
    f32 tx, ty, s;
    f32 f;
    u16 tw;
    u16 th;
    s32 i;
    MonoDat* e;
    s32 n;
    Vec* gpos;
    s32* cnt;
    s32* idx;
    Vec* a;
    Vec* b;
    s32 k;

    data = animPoseGetAnimDataPtr(m->pose1);
    min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    cx = (max.x + min.x) / 2.0f;
    cy = (max.y + min.y) / 2.0f;
    GXGetViewportv(vp);
    GXGetProjectionv(proj);
    GXGetScissor(&sx, &sy, &sw, &sh);
    TEXGetGXTexObjFromPalette(*(TEXPalette**)m->winTpl->data, &tex, 0x37);
    tw = GXGetTexObjWidth(&tex);
    th = GXGetTexObjHeight(&tex);
    GXSetViewport(0.0f, 0.0f, tw, th, 0.0f, 1.0f);
    GXSetScissor(1, 1, tw - 2, th - 2);
    C_MTXPerspective(pm, 25.0f, 1.2666667f, 1.0f, 1000.0f);
    GXSetProjection(pm, GX_PERSPECTIVE);
    if ((max.x - min.x) / tw > (max.y - min.y) / th) {
        dist = (max.x - cx) / (f32)tan(0.21816615760326385);
    } else {
        dist = ((608.0f * (max.y - cy)) / 480.0f) / (f32)tan(0.21816615760326385);
    }
    C_MTXLookAt(view, &(Vec){cx, cy, dist}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){cx, cy, 0.0f});
    n = ((MonoDatFile*)m->monoDat->data)->size / sizeof(MonoDat);
    e = ((MonoDatFile*)m->monoDat->data)->entries;
    for (i = 0; i < n; i++, e++) {
        if (e->id == m->id) break;
    }
    if (i < n) {
        tx = e->x;
        ty = e->y;
        s = e->scale;
    } else {
        ty = tx = 0.0f;
        s = 1.0f;
    }
    PSMTXTrans(t, tx, ty, 0.0f);
    PSMTXScale(sm, s, s, s);
    PSMTXConcat(t, sm, t);
    PSMTXConcat(view, t, view);
    sysWaitDrawSync();
    GXClearBoundingBox();
    bak = *camGetPtr(camera);
    PSMTXCopy(view, camGetCurPtr()->view);
    if (animPoseGetVivianType(m->pose1)) {
        animPoseVivianMain(m->pose1, &(Vec){0.0f, 0.0f, 0.0f});
    }
    animPoseSetMaterialFlagOn(m->pose1, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    animPosePeraOff(m->pose1);
    PSMTXIdentity(t);
    animPoseDrawMtx(m->pose1, t, 1, 0.0f, 1.0f);
    animPoseDrawMtx(m->pose1, t, 2, 0.0f, 1.0f);
    animPoseDrawMtx(m->pose1, t, 3, 0.0f, 1.0f);
    animPosePeraOn(m->pose1);
    if (m->id == UNIT_SMORG) {
        gpos = (Vec*)animPoseGetAnimPosePtr(m->pose1)->worldPositions->address;
        cnt = smartAlloc(g_moamoa_chain_num * 4, SMART_GROUP_TEMP)->address;
        idx = smartAlloc(2000, SMART_GROUP_TEMP)->address;
        memset(cnt, 0, g_moamoa_chain_num * 4);

        // FAKE: reversed loop compare; i < 500 changes the object
        for (i = 0; 500 > i; i++) {
            idx[i] = cnt[i % g_moamoa_chain_num]++;
        }
        animPoseSetMaterialFlagOn(m->pose2, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
        animPosePeraOff(m->pose2);
        for (i = 0; i < 500; i++) {
            k = i % g_moamoa_chain_num;
            f = (f32)idx[i] / (f32)cnt[k];
            a = &gpos[animPoseGetGroupIdx(m->pose1, g_moamoa_chain_tbl[k].parent)];
            b = &gpos[animPoseGetGroupIdx(m->pose1, g_moamoa_chain_tbl[k].child)];
            PSVECScale(a, &v1, f);
            PSVECScale(b, &v2, 1.0f - f);
            PSVECAdd(&v1, &v2, &v1);
            PSMTXTrans(t2, v1.x + (5 - irand(10)), v1.y + (5 - irand(10)), v1.z + (5 - irand(10)));
            animPoseDrawMtx(m->pose2, t2, 1, 0.0f, 1.0f);
            animPoseDrawMtx(m->pose2, t2, 2, 0.0f, 1.0f);
            animPoseDrawMtx(m->pose2, t2, 3, 0.0f, 1.0f);
        }
        animPosePeraOn(m->pose2);
    }
    PSMTXCopy(bak.view, camGetCurPtr()->view);
    sysWaitDrawSync();
    GXReadBoundingBox(&bb0, &bb1, &bb2, &bb3);
    GXSetTexCopySrc(0, 0, tw, th);
    GXSetTexCopyDst(tw, th, GX_TF_RGB5A3, GX_FALSE);
#if VERSION >= VERSION_US
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
#endif
    GXCopyTex(m->smart->address, GX_TRUE);
    GXPixModeSync();
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(proj);
    GXSetScissor(sx, sy, sw, sh);
}


/**
 * Draws the Tattle card: the half-book page, the captured portrait with a soft edge and fade-in alpha,
 * the enemy's log number and name (Doopliss as "?" until his name is guessed, not in the early demos),
 * and where it is found.
 */
static void monosiri_disp(CameraId camera, void* param) {
    MonosiriWork* m = param;
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    char buf[256];
    Mtx t1;
    Mtx sc;
    Mtx t2;
    GXTexObj tex;
    f32 x;
    f32 y;
    f32 s;
    f32 w;
    s32 mtx;
    s32 i;
    s32 id;
    TattleEntry* te;
    u16 tw;
    u16 th;
#if VERSION <= VERSION_DEMO_JP
    s32 d;
    GXColor c0;
    GXColor c;

    x = m->curX;
    y = m->curY;
    {
#else
    x = m->curX;
    y = m->curY;
    if (m->winTpl) {
#endif
        winHalfBookGX(*(TEXPalette**)m->winTpl->data, 0, x, y);
        TEXGetGXTexObjFromPalette(*(TEXPalette**)m->winTpl->data, &tex, 0x37);
        tw = GXGetTexObjWidth(&tex);
        th = GXGetTexObjHeight(&tex);
        TEXGetGXTexObjFromPalette(*(TEXPalette**)m->winTpl->data, &tex, 0x37);
        GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXInitTexObj(&tex, m->smart->address, tw, th, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP1);
        GXSetNumTexGens(8);
        for (i = 0; i < 8; i++) {
            mtx = GX_TEXMTX0 + i * 3;
            s = 1.0f - 0.04f * (7 - i);
            PSMTXTrans(t1, 0.5f, 0.5f, 0.0f);
            PSMTXScale(sc, s, s, s);
            PSMTXTrans(t2, -0.5f, -0.5f, 0.0f);
            PSMTXConcat(t1, sc, t1);
            PSMTXConcat(t1, t2, t1);
            GXLoadTexMtxImm(t1, mtx, GX_MTX2x4);
            GXSetTexCoordGen2(i, GX_TG_MTX2x4, GX_TG_TEX0, mtx, GX_FALSE, GX_PTIDENTITY);
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){0xCF, 0xBC, 0x9E, 0xFF});
        GXSetTevColor(GX_TEVREG1, (GXColor){0x00, 0x00, 0x00, m->alpha});
        GXSetNumTevStages(12);
        for (i = 0; i < 8; i++) {
            GXSetTevOrder(i, i, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorOp(i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(i, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(i, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, i == 0 ? GX_CA_ZERO : GX_CA_APREV);
            GXSetTevKAlphaSel(i, GX_TEV_KASEL_1_8);
        }
        GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD7, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE8, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE8, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE8, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE8, GX_CA_APREV, GX_CA_ZERO, GX_CA_TEXA, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE9, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE9, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE9, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE9, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD7, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE10, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE10, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE10, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE10, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD7, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE11, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE11, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE11, GX_CC_TEXC, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE11, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            f32 x1 = 32.0f + x;
            f32 y1 = y - 8.0f;
            f32 qw = 216.0f;
            GXPosition3f32(x1, y1, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(x1, y1 - 160.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
            GXPosition3f32(x1 + qw, y1 - 160.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(x1 + qw, y1, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
        }
        id = m->id;
        te = battleGetUnitMonosiriPtr(id);
#if VERSION <= VERSION_DEMO_JP
        sprintf(buf, "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch(te->enemyTag));
#else
        if (id == UNIT_DOOPLISS_CHAPTER4_FIGHT1 || id == UNIT_DOOPLISS_CHAPTER8) {
            if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG4_14_NAME_GUESSING) {
#if VERSION >= VERSION_US
                sprintf(buf, gp->language != LANGUAGE_JP ? "%d. %s" : "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch("btl_un_hatena"));
            } else {
                sprintf(buf, gp->language != LANGUAGE_JP ? "%d. %s" : "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch("btl_un_rampell"));
            }
        } else {
            sprintf(buf, gp->language != LANGUAGE_JP ? "%d. %s" : "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch(te->enemyTag));
#else
                sprintf(buf, "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch("btl_un_hatena"));
            } else {
                sprintf(buf, "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch("btl_un_rampell"));
            }
        } else {
            sprintf(buf, "%d\201D%s", enemy_monoshiri_sort_table[id], msgSearch(te->enemyTag));
#endif
        }
#endif
        w = 0.75f * FontGetMessageWidth(buf);
        winFontInit();
        winFontSet((Vec){(16.0f + x) + (248.0f - w) / 2.0f, y - 180.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, black, buf);
#if VERSION <= VERSION_DEMO_JP
        sprintf(buf, "%s\201@%s", msgSearch("menu_monosiri_derutokoro"), msgSearch(te->locationTag));
        w = 0.75f * FontGetMessageWidth(buf);
        winFontSet((Vec){(16.0f + x) + (248.0f - w) / 2.0f, y - 210.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, black, buf);
#else
#if VERSION >= VERSION_US
        sprintf(buf, gp->language != LANGUAGE_JP ? "%s %s" : "%s\201@%s", msgSearch("menu_monosiri_derutokoro"), msgSearch(te->locationTag));
#else
        sprintf(buf, "%s\201@%s", msgSearch("menu_monosiri_derutokoro"), msgSearch(te->locationTag));
#endif
        w = 0.75f * FontGetMessageWidth(buf);
        winFontSet((Vec){1.0f + ((16.0f + x) + (248.0f - w) / 2.0f), y - 210.0f, 0.0f}, (Vec){0.75f, 0.75f, 0.75f}, black, buf);
#endif
    }
}

