// Status window: the field HUD (HP, FP, star power gauge, coins, star points) and its show/hide logic.

#include "version.h"
#include "icon_ids.h"
#include "statuswindow.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "driver/seqdrv.h"
#include "driver/swdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include <string.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "libc_decls.h"


typedef StatusValue SVal;

typedef StatusWindowWork SWork;

typedef struct alwaysDtEntry {
    s16 xpos;   // 0x0
    s16 ypos;   // 0x2
    u16 iconId; // 0x4
    f32 scale;  // 0x8
} alwaysDtEntry;

//.data
alwaysDtEntry alwaysDt[] = {
    { 108, 40, ICON_STATUS_HP_BANNER, 1.0f }, // red
    { 38, 40, ICON_STATUS_MARIO, 1.0f },
    { 124, 34, ICON_SLASH, 1.0f },
    { 288, 40, ICON_STATUS_FP_BANNER, 1.0f }, // orange
    { 220, 40, ICON_STATUS_FP, 1.0f },
    { 305, 34, ICON_SLASH, 1.0f },
    { 492, 40, ICON_STATUS_COIN_BANNER, 1.0f }, // green, behind the Star Points and coins
    { 400, 40, ICON_STATUS_STAR_POINTS, 1.0f },
    { 490, 40, ICON_STATUS_COIN, 1.0f },
    { 422, 34, ICON_TIMES, 1.0f },
    { 512, 34, ICON_TIMES, 1.0f },
    { 360, 76, ICON_STATUS_SP_BANNER, 1.0f }, // blue
    { 220, 80, ICON_STATUS_SP, 0.9f }
};

//.sdata
alwaysDtEntry partysDt[] = {
    { 108, 76, ICON_STATUS_HP_BANNER, 1.0f },
    { 38, 80, ICON_STATUS_GOOMBELLA, 1.0f },
    { 124, 70, ICON_SLASH, 1.0f },
};
u16 party_icon[8] = { ICON_STATUS_GOOMBELLA, ICON_STATUS_GOOMBELLA, ICON_STATUS_KOOPS, ICON_STATUS_BOBBERY, ICON_STATUS_YOSHI, ICON_STATUS_FLURRIE, ICON_STATUS_VIVIAN, ICON_STATUS_MS_MOWZ };
u16 gauge_back[8] = { ICON_SP_ORB_1, ICON_SP_ORB_2, ICON_SP_ORB_3, ICON_SP_ORB_4, ICON_SP_ORB_5, ICON_SP_ORB_6, ICON_SP_ORB_7, ICON_SP_ORB_8 };
u16 gauge_wakka[16] = { ICON_SP_ORB_PART_0, ICON_SP_ORB_PART_1, ICON_SP_ORB_PART_2, ICON_SP_ORB_PART_3, ICON_SP_ORB_PART_4, ICON_SP_ORB_PART_5, ICON_SP_ORB_PART_6, ICON_SP_ORB_PART_7,
                        ICON_SP_ORB_PART_8, ICON_SP_ORB_PART_9, ICON_SP_ORB_PART_10, ICON_SP_ORB_PART_11, ICON_SP_ORB_PART_12, ICON_SP_ORB_PART_13, ICON_SP_ORB_PART_14, ICON_SP_ORB_PART_15 };
#if VERSION >= VERSION_US
// US: pause-menu icons per language
s16 party_icon_tbl[6] = { 0x215, 0x22B, 0x22C, 0x22D, 0x22E, 0x22F };
s16 gear_icon_tbl[6] = { 0x216, 0x230, 0x231, 0x232, 0x233, 0x234 };
s16 badge_icon_tbl[6] = { 0x217, 0x235, 0x236, 0x235, 0x237, 0x238 };
s16 journal_icon_tbl[6] = { 0x218, 0x239, 0x239, 0x239, 0x23A, 0x23B };
#endif



//.sbss
static StatusWindowWork* wp;

void statusWinDisp(CameraId camId, void* param);
BOOL valueCheck(void);
void valueUpdate(void);

/**
 * Allocates the field HUD state at boot and sets its open
 * and closed positions (it slides down from above the screen), starting open.
 */
void statusWinInit(void) {
    wp = __memAlloc(HEAP_DEFAULT, sizeof(StatusWindowWork));
    memset(wp, 0, sizeof(StatusWindowWork));
    // FAKE: empty null check after the memset; removing it changes the object
    if (!wp) {
    }
    wp->speed = 1.0f;
    wp->openX = -304.0f;
    wp->openY = 208.0f;
    wp->closeX = -304.0f;
    wp->closeY = 334.0f;
    wp->idle = 0;
    wp->forceTimer = 0;
    wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
    wp->flags |= (STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
}

/** Resets the HUD's idle timers on a map change and turns a pending forced open/close into a normal open/close. */
void statusWinReInit(void) {
    wp->idle = 0;
    wp->forceTimer = 0;
    if (wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN) {
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_OPEN;
        wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
        wp->flags |= (STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
    } else if (wp->flags & STATUS_WINDOW_FLAG_OPEN) {
        wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
        wp->flags |= (STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
    }
    if (wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE) {
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_CLOSE;
        wp->flags |= (STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
        wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
    } else if (wp->flags & STATUS_WINDOW_FLAG_CLOSED) {
        wp->flags |= (STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
        wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
    }
}

/**
 * Per-frame HUD logic in the field: hidden when Mario is not himself (a transformation or another character)
 * and left alone in the Pit of 100 Trials ("yuu").
 * While Mario can move it opens after he stands still for 10 seconds, closes when he walks,
 * toggles with Z (held for 10 seconds, with a sound) and pops open for 2 seconds when a value changes.
 * Then starts and runs the slide animation (16 frames in, 28 out).
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Per-frame HUD logic in the field: hidden when Mario is not himself (a transformation or another character)
 * and left alone in the Pit of 100 Trials ("yuu").
 * While Mario can move it opens after he stands still for 10 seconds, closes when he walks,
 * toggles with Z (held for 10 seconds, with a sound) and pops open for 2 seconds when a value changes.
 * Then starts and runs the slide animation (16 frames in, 28 out).
 */
void statusWinMain(void) {
    MarioWork* mario;
    f32 inv;
    f32 one;
    f32 t;

    one = 1.0f;
    mario = marioGetPtr();
    if (mario->characterId) {
        wp->flags = STATUS_WINDOW_FLAG_CLOSED;
        wp->duration = one;
        wp->timer = one;
        wp->tEnd = 0.0f;
        wp->t = 0.0f;
        return;
    }
    if (!strcmp(gp->currentAreaName, "yuu")) {
        return;
    }
    switch (seqGetSeq()) {
        case SEQ_GAME:
        case SEQ_MAP_CHANGE:
            if ((marioChkCtrl() ? FALSE : TRUE) || (marioChkKey() ? FALSE : TRUE) || !(mario->flags & MARIO_FLAG_ACTIVE)) {
                if (statusWinCheck()) {
                    statusWinClose();
                }
                valueCheck();
                wp->idle = 0;
                wp->forceTimer = 0;
            } else {
                if (VECDistance(&wp->prevPos, &mario->position) < 1.0f) {
                    wp->idle++;
                    if (wp->idle > gp->framerate * 10) {
                        wp->idle = gp->framerate * 10;
                        if (!wp->forcedClose && !wp->forceTimer && !statusWinCheck()) {
                            statusWinOpen();
}
                    }
                } else {
                    wp->idle = 0;
                    wp->forcedClose = 0;
                    if (!wp->forceTimer && statusWinCheck()) {
                        statusWinClose();
                    }
                }
                if (mario->buttonTrg & PAD_TRIGGER_Z) {
                    if (!statusWinCheck()) {
                        statusWinOpen();
                        wp->forceTimer = gp->framerate * 10;
                        psndSFXOn((const char*)0x2A);
                        wp->flags |= STATUS_WINDOW_FLAG_MENU_ICONS;
                    } else {
                        statusWinClose();
                        wp->forceTimer = gp->framerate * 10;
                        wp->forcedClose = 1;
                        psndSFXOn((const char*)0x2B);
                    }
                }
                if (valueCheck()) {
                    wp->forceTimer = gp->framerate * 2;
                    if (!statusWinCheck()) {
                        statusWinOpen();
                    }
                }
            }
            if (wp->forceTimer) {
                wp->forceTimer--;
            }
            wp->prevPos = mario->position;
            if (wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN) {
                wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
            }
            if (wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE) {
                wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
            }
            break;
        case SEQ_BATTLE:
            break;
        default:
            if (statusWinCheck()) {
                statusWinClose();
            }
            break;
    }
    if (wp->flags & STATUS_WINDOW_FLAG_OPEN_REQ || wp->flags & STATUS_WINDOW_FLAG_CLOSE_REQ || wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN_REQ || wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ) {
        wp->tStart = wp->t;
        wp->tEnd = (wp->flags & (STATUS_WINDOW_FLAG_OPEN_REQ | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ)) ? 1.0f : 0.0f;
        wp->timer = 0.0f;
        wp->duration = (wp->flags & (STATUS_WINDOW_FLAG_OPEN_REQ | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ)) ? 16.0f / wp->speed : 28.0f / wp->speed;
        wp->flags &= ~STATUS_WINDOW_FLAG_OPEN_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_CLOSE_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_OPEN_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ;
    }
wp->timer += one;
if (wp->timer >= wp->duration) {
        wp->timer = wp->duration;
        wp->t = wp->tEnd;
        if (wp->t == 0.0f) {
            wp->flags &= ~STATUS_WINDOW_FLAG_MENU_ICONS;
        }
    } else {
        inv = (f32)cos(3.1415927f * wp->timer / wp->duration);
        wp->t = wp->tStart + (wp->tEnd - wp->tStart) * (one - inv) / 2.0f;
    }
    t = wp->t;
    inv = 1.0f - t;
    wp->x = wp->openX * t + wp->closeX * inv;
    wp->y = wp->openY * t + wp->closeY * inv;
    valueUpdate();
    if (!(wp->flags & STATUS_WINDOW_FLAG_DISP_OFF)) {
        dispEntry(CAMERA_2D, 500.0f, 0, statusWinDisp, NULL);
    }
}
#else
/**
 * Per-frame HUD logic in the field: hidden when Mario is not himself (a transformation or another character)
 * and left alone in the Pit of 100 Trials ("yuu").
 * While Mario can move it opens after he stands still for 10 seconds, closes when he walks,
 * toggles with Z (held for 10 seconds, with a sound) and pops open for 2 seconds when a value changes.
 * Then starts and runs the slide animation (16 frames in, 28 out).
 */
void statusWinMain(void) {
    MarioWork* mario;
    f32 inv;
    f32 one;
    f32 t;

    one = 1.0f;
    mario = marioGetPtr();
    if (mario->characterId) {
        wp->flags = STATUS_WINDOW_FLAG_CLOSED;
        wp->duration = one;
        wp->timer = one;
        wp->tEnd = 0.0f;
        wp->t = 0.0f;
        return;
    }
    if (!strcmp(gp->currentAreaName, "yuu")) {
        return;
    }
    switch (seqGetSeq()) {
        case SEQ_GAME:
        case SEQ_MAP_CHANGE:
            // FAKE: (x ? FALSE : TRUE) for !x; the plain negation changes the object
            if ((marioChkCtrl() ? FALSE : TRUE) || (marioChkKey() ? FALSE : TRUE) || !(mario->flags & MARIO_FLAG_ACTIVE)) {
                if (statusWinCheck()) {
                    statusWinClose();
                }
                valueCheck();
                wp->idle = 0;
                wp->forceTimer = 0;
            } else {
                if (VECDistance(&wp->prevPos, &mario->position) < 1.0f) {
                    wp->idle++;
                    if (wp->idle > gp->framerate * 10) {
                        wp->idle = gp->framerate * 10;
                        if (!wp->forcedClose && !wp->forceTimer && !statusWinCheck()) {
                            statusWinOpen();
                            wp->flags |= STATUS_WINDOW_FLAG_MENU_ICONS;
                        }
                    }
                } else {
                    wp->idle = 0;
                    wp->forcedClose = 0;
                    if (!wp->forceTimer && statusWinCheck()) {
                        statusWinClose();
                    }
                }
                if (mario->buttonTrg & PAD_TRIGGER_Z) {
                    if (!statusWinCheck()) {
                        statusWinOpen();
                        wp->forceTimer = gp->framerate * 10;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1));
                        wp->flags |= STATUS_WINDOW_FLAG_MENU_ICONS;
                    } else {
                        statusWinClose();
                        wp->forceTimer = gp->framerate * 10;
                        wp->forcedClose = 1;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1));
                    }
                }
                if (valueCheck()) {
                    wp->forceTimer = gp->framerate * 2;
                    if (!statusWinCheck()) {
                        statusWinOpen();
                    }
                }
            }
            if (wp->forceTimer) {
                wp->forceTimer--;
            }
            wp->prevPos = mario->position;
            if (wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN) {
                wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
            }
            if (wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE) {
                wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
            }
            break;
        case SEQ_BATTLE:
            break;
        default:
            if (statusWinCheck()) {
                statusWinClose();
            }
            break;
    }
    if (wp->flags & STATUS_WINDOW_FLAG_OPEN_REQ || wp->flags & STATUS_WINDOW_FLAG_CLOSE_REQ || wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN_REQ || wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ) {
        wp->tStart = wp->t;
        wp->tEnd = (wp->flags & (STATUS_WINDOW_FLAG_OPEN_REQ | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ)) ? 1.0f : 0.0f;
        wp->timer = 0.0f;
        wp->duration = (wp->flags & (STATUS_WINDOW_FLAG_OPEN_REQ | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ)) ? 16.0f / wp->speed : 28.0f / wp->speed;
        wp->flags &= ~STATUS_WINDOW_FLAG_OPEN_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_CLOSE_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_OPEN_REQ;
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ;
    }
#if VERSION >= VERSION_US
    wp->timer += 60.0f / gp->framerate;
#else
    wp->timer += one;
#endif
    if (wp->timer >= wp->duration) {
        wp->timer = wp->duration;
        wp->t = wp->tEnd;
        if (wp->t == 0.0f) {
            wp->flags &= ~STATUS_WINDOW_FLAG_MENU_ICONS;
        }
    } else {
        inv = (f32)cos(3.1415927f * wp->timer / wp->duration);
        wp->t = wp->tStart + (wp->tEnd - wp->tStart) * (one - inv) / 2.0f;
    }
    t = wp->t;
    inv = 1.0f - t;
    wp->x = wp->openX * t + wp->closeX * inv;
    wp->y = wp->openY * t + wp->closeY * inv;
    valueUpdate();
    if (!(wp->flags & STATUS_WINDOW_FLAG_DISP_OFF)) {
        dispEntry(CAMERA_2D, 500.0f, 0, statusWinDisp, NULL);
    }
}
#endif

// FAKE: iconDispGx called through a scale-first function pointer cast (the scale is in f1 either way), which
// evaluates the arguments in the retail order; the plain call changes the object.
#define iconDispGxScaleFirst ((void (*)(f32, Vec*, s16, u16))iconDispGx)

/**
 * Draws the Star Power gauge at (x, y): one 32-pixel slot per crystal star obtained,
 * full slots for each 100 of sp and the partly filled slot in 15 steps.
 */
void gaugeDisp(s32 sp, f32 x, f32 y) {
    s32 i;
    s32 max;
    s32 full;
    s32 part;

    max = 0;
    for (i = 0; i < sizeof(gauge_back) / sizeof(gauge_back[0]); i++) {
        if (pouchGetPtr()->starPowersObtained & (1 << i)) {
            max += 100;
        }
    }
    if (sp > max) {
        sp = max;
    }
    if (sp < 0) {
        sp = 0;
    }
    full = sp / 100;
    part = sp % 100;
    part = part * 15 / 99;
    if (sp % 100 && sp && !part) {
        part = 1;
    }
    if (part) {
        Vec pos;
        pos.x = x + (f32)(full * 32);
        pos.y = y;
        pos.z = 0.0f;
#if VERSION <= VERSION_DEMO_US
        iconDispGxScaleFirst(1.0f, &pos, 0x4, gauge_wakka[part]);
#else
        iconDispGxScaleFirst(1.0f, &pos, 0x10, gauge_wakka[part]);
#endif
    }
    for (i = 0; i < sizeof(gauge_back) / sizeof(gauge_back[0]); i++) {
        Vec pos2;
        if (!(pouchGetPtr()->starPowersObtained & (1 << i))) {
            break;
        }
        pos2.x = x + (f32)(i * 32);
        pos2.y = 12.0f + y;
        pos2.z = 0.0f;
#if VERSION <= VERSION_DEMO_US
        iconDispGxScaleFirst(1.0f, &pos2, 0x4, i < full ? gauge_back[i] : ICON_SP_ORB_EMPTY);
#else
        iconDispGxScaleFirst(1.0f, &pos2, 0x10, i < full ? gauge_back[i] : ICON_SP_ORB_EMPTY);
#endif
    }
}

static inline void numDisp(f32 x, f32 y, s32 value, BOOL small) {
    Mtx m;
    MTXTrans(m, x, y, 0.0f);
    iconNumberDispGx(m, value, small, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
}

/**
 * Draws the HUD: the HP, FP, Star Power and coin/Star Point banners with their icons and numbers,
 * Mario's and the partner's HP, and the gauge, blinking any value flagged by the stat blink functions.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Draws the HUD: the HP, FP, Star Power and coin/Star Point banners with their icons and numbers,
 * Mario's and the partner's HP, and the gauge, blinking any value flagged by the stat blink functions.
 */
static void statusWinDisp(CameraId camId, void* param) {
    alwaysDtEntry* e;
    s32 i;
    s32 blinkOn;
    s32 blink;
    f32 x, y, y2;
    Mtx m, s;
    Mtx m2, s2;

    x = wp->x;
    y = wp->y;
    blinkOn = 1;
    if (wp->blinkTimer > 0) {
        wp->blinkTimer--;
        blinkOn = (wp->blinkTimer % 16) >= 6;
    }

    e = alwaysDt;
    for (i = 0; i < sizeof(alwaysDt) / sizeof(alwaysDt[0]); i++, e++) {
        if (e->iconId == ICON_STATUS_SP_BANNER && !pouchGetPtr()->starPowersObtained) {
            break;
        }
        if (!blinkOn) {
            blink = wp->blink;
            if (blink & 1 && i == 1) {
                continue;
            }
            if (blink & 2 && i == 4) {
                continue;
            }
            if (blink & 0x10 && i == 8) {
                continue;
            }
            if (blink & 4 && i == 12) {
                continue;
            }
        }
        MTXTrans(m, x + (f32)e->xpos, y - (f32)e->ypos, 0.0f);
        MTXScale(s, e->scale, e->scale, e->scale);
        MTXConcat(m, s, m);
        iconDispGxCol(m, ICON_FLAG_2D, e->iconId, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }

    if (!(wp->blink & 1) || blinkOn) {
        numDisp(120.0f + x - 6.0f - 10.0f, 2.0f + (y - 38.0f), wp->cur.hp, 0);
    }
    numDisp(186.0f + x - 6.0f - 10.0f, 3.0f + (y - 38.0f), wp->cur.maxHp, 1);
    if (!(wp->blink & 2) || blinkOn) {
        numDisp(292.0f + x - 6.0f, 2.0f + (y - 38.0f), wp->cur.fp, 0);
    }
    numDisp(358.0f + x - 6.0f, 3.0f + (y - 38.0f), wp->cur.maxFp, 1);
    numDisp(462.0f + x, 2.0f + (y - 38.0f), wp->cur.starPoints, 0);
    if (!(wp->blink & 0x10) || blinkOn) {
        numDisp(572.0f + x, 2.0f + (y - 38.0f), wp->cur.coins, 0);
    }
    if (!(wp->blink & 4) || blinkOn) {
        gaugeDisp(wp->cur.ap, 260.0f + x, 4.0f + (y - 87.0f));
    }

    if (wp->cur.party) {
        partysDt[1].iconId = party_icon[wp->cur.party];
        if (wp->cur.party == PARTNER_YOSHI) {
            switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                case 0:
                    partysDt[1].iconId = ICON_STATUS_YOSHI;
                    break;
                case 1:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_1;
                    break;
                case 2:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_2;
                    break;
                case 3:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_3;
                    break;
                case 4:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_4;
                    break;
                case 5:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_5;
                    break;
                case 6:
                default:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_6;
                    break;
            }
        }
        e = partysDt;
        for (i = 0; i < sizeof(partysDt) / sizeof(partysDt[0]); i++, e++) {
            if (wp->blink & 8 && !blinkOn && i == 1) {
                continue;
            }
            MTXTrans(m, x + (f32)e->xpos, y - (f32)e->ypos, 0.0f);
            MTXScale(s, e->scale, e->scale, e->scale);
            MTXConcat(m, s, m);
            iconDispGxCol(m, ICON_FLAG_2D, e->iconId, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
        if (!(wp->blink & 8) || blinkOn) {
            numDisp(120.0f + x - 6.0f - 10.0f, 4.0f + (2.0f + (y - 78.0f)), wp->cur.partyHp, 0);
        }
        numDisp(186.0f + x - 6.0f - 10.0f, 4.0f + (2.0f + (y - 78.0f)), wp->cur.partyMaxHp, 1);
    }
}
#else
/**
 * Draws the HUD: the HP, FP, Star Power and coin/Star Point banners with their icons and numbers,
 * Mario's and the partner's HP, and the gauge, blinking any value flagged by the stat blink functions.
 */
void statusWinDisp(CameraId camId, void* param) {
    alwaysDtEntry* e;
    s32 i;
    s32 blinkOn;
    s32 blink;
    f32 x, y, y2;
    Mtx m, s;
    Mtx m2, s2;

    x = wp->x;
    y = wp->y;
    blinkOn = 1;
    if (wp->blinkTimer > 0) {
        wp->blinkTimer--;
        blinkOn = (wp->blinkTimer % 16) >= 6;
    }

    e = alwaysDt;
    for (i = 0; i < sizeof(alwaysDt) / sizeof(alwaysDt[0]); i++, e++) {
        if (e->iconId == ICON_STATUS_SP_BANNER && !pouchGetPtr()->starPowersObtained) {
            break;
        }
        if (!blinkOn) {
            blink = wp->blink;
            if (blink & 1 && i == 1) {
                continue;
            }
            if (blink & 2 && i == 4) {
                continue;
            }
            if (blink & 0x10 && i == 8) {
                continue;
            }
            if (blink & 4 && i == 12) {
                continue;
            }
        }
        MTXTrans(m, x + (f32)e->xpos, y - (f32)e->ypos, 0.0f);
        MTXScale(s, e->scale, e->scale, e->scale);
        MTXConcat(m, s, m);
        iconDispGxCol(m, ICON_FLAG_2D, e->iconId, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    }

    if (!(wp->blink & 1) || blinkOn) {
        numDisp(120.0f + x - 6.0f - 10.0f, 2.0f + (y - 38.0f), wp->cur.hp, 0);
    }
    numDisp(186.0f + x - 6.0f - 10.0f, 3.0f + (y - 38.0f), wp->cur.maxHp, 1);
    if (!(wp->blink & 2) || blinkOn) {
        numDisp(292.0f + x - 6.0f, 2.0f + (y - 38.0f), wp->cur.fp, 0);
    }
    numDisp(358.0f + x - 6.0f, 3.0f + (y - 38.0f), wp->cur.maxFp, 1);
    numDisp(462.0f + x, 2.0f + (y - 38.0f), wp->cur.starPoints, 0);
    if (!(wp->blink & 0x10) || blinkOn) {
        numDisp(572.0f + x, 2.0f + (y - 38.0f), wp->cur.coins, 0);
    }
    if (!(wp->blink & 4) || blinkOn) {
        gaugeDisp(wp->cur.ap, 260.0f + x, 4.0f + (y - 87.0f));
    }

    if (wp->cur.party) {
        partysDt[1].iconId = party_icon[wp->cur.party];
        if (wp->cur.party == PARTNER_YOSHI) {
            switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                case 0:
                    partysDt[1].iconId = ICON_STATUS_YOSHI;
                    break;
                case 1:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_1;
                    break;
                case 2:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_2;
                    break;
                case 3:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_3;
                    break;
                case 4:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_4;
                    break;
                case 5:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_5;
                    break;
                case 6:
                default:
                    partysDt[1].iconId = ICON_STATUS_YOSHI_6;
                    break;
            }
        }
        e = partysDt;
        for (i = 0; i < sizeof(partysDt) / sizeof(partysDt[0]); i++, e++) {
            if (wp->blink & 8 && !blinkOn && i == 1) {
                continue;
            }
            MTXTrans(m, x + (f32)e->xpos, y - (f32)e->ypos, 0.0f);
            MTXScale(s, e->scale, e->scale, e->scale);
            MTXConcat(m, s, m);
            iconDispGxCol(m, ICON_FLAG_2D, e->iconId, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        }
        if (!(wp->blink & 8) || blinkOn) {
            numDisp(120.0f + x - 6.0f - 10.0f, 4.0f + (2.0f + (y - 78.0f)), wp->cur.partyHp, 0);
        }
        numDisp(186.0f + x - 6.0f - 10.0f, 4.0f + (2.0f + (y - 78.0f)), wp->cur.partyMaxHp, 1);
    }

    if (wp->flags & STATUS_WINDOW_FLAG_MENU_ICONS && !gp->inBattle) {
        y2 = -180.0f + (wp->openY - wp->y);
        MTXTrans(m2, -190.0f, y2, 0.0f);
        MTXScale(s2, 0.75f, 0.75f, 0.75f);
        MTXConcat(m2, s2, m2);
        iconDispGxCol(m2, ICON_FLAG_2D, ICON_MENU_DPAD, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        if (swByteGet(0) >= STG0_07_GOOMBELLA_JOINS || partyChkJoin(PARTNER_GOOMBELLA)) { // GSW_STORY_PROGRESS
            MTXTrans(m2, -250.0f, 16.0f + y2, 0.0f);
            MTXScale(s2, 0.75f, 0.75f, 0.75f);
            MTXConcat(m2, s2, m2);
#if VERSION >= VERSION_US
            iconDispGxCol(m2, ICON_FLAG_2D, (u16)party_icon_tbl[gp->language], (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#else
            iconDispGxCol(m2, ICON_FLAG_2D, ICON_MENU_PARTY, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#endif
        }
        MTXTrans(m2, -190.0f, 50.0f + y2, 0.0f);
        MTXScale(s2, 0.75f, 0.75f, 0.75f);
        MTXConcat(m2, s2, m2);
#if VERSION >= VERSION_US
        iconDispGxCol(m2, ICON_FLAG_2D, (u16)gear_icon_tbl[gp->language], (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        if (swByteGet(0) >= STG0_17_BADGE_TUTORIAL || pouchGetHaveBadgeCnt()) { // GSW_STORY_PROGRESS
            f32 xoff[6] = { 0.0f, 10.0f, 3.0f, 8.0f, 8.0f, 0.0f };
            MTXTrans(m2, -130.0f + xoff[gp->language], 16.0f + y2, 0.0f);
            MTXScale(s2, 0.75f, 0.75f, 0.75f);
            MTXConcat(m2, s2, m2);
            iconDispGxCol(m2, ICON_FLAG_2D, (u16)badge_icon_tbl[gp->language], (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#else
        iconDispGxCol(m2, ICON_FLAG_2D, ICON_MENU_GEAR, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        if (swByteGet(0) >= STG0_17_BADGE_TUTORIAL || pouchGetHaveBadgeCnt()) { // GSW_STORY_PROGRESS
            MTXTrans(m2, -130.0f, 16.0f + y2, 0.0f);
            MTXScale(s2, 0.75f, 0.75f, 0.75f);
            MTXConcat(m2, s2, m2);
            iconDispGxCol(m2, ICON_FLAG_2D, ICON_MENU_BADGES, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#endif
        }
        MTXTrans(m2, -190.0f, y2 - 25.0f, 0.0f);
        MTXScale(s2, 0.75f, 0.75f, 0.75f);
        MTXConcat(m2, s2, m2);
#if VERSION >= VERSION_US
        iconDispGxCol(m2, ICON_FLAG_2D, (u16)journal_icon_tbl[gp->language], (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#else
        iconDispGxCol(m2, ICON_FLAG_2D, ICON_MENU_JOURNAL, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
#endif
    }
}
#endif

/**
 * Reads the values the HUD shows: from the pouch in the field (the partner from the party),
 * from Mario's and the partner's battle units in battle.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Reads the values the HUD shows: from the pouch in the field (the partner from the party),
 * from Mario's and the partner's battle units in battle.
 */
static void statusGetValue(StatusValue* values) {
#else
/**
 * Reads the values the HUD shows: from the pouch in the field (the partner from the party),
 * from Mario's and the partner's battle units in battle.
 */
void statusGetValue(StatusValue* values) {
#endif
    BattleWork* bw;
    BattleUnit* mario;
    BattleUnit* party;

    if (!gp->inBattle) {
        values->hp = pouchGetHP();
        values->maxHp = pouchGetMaxHP();
        values->fp = pouchGetFP();
        values->maxFp = pouchGetMaxFP();
        values->ap = pouchGetAP();
        values->maxAp = pouchGetMaxAP();
        values->starPoints = pouchGetStarPoint();
        values->coins = pouchGetCoin();
#if VERSION <= VERSION_DEMO_US
        if (!marioBgmodeChk()) {
            values->party = marioGetParty();
#else
        values->party = !marioBgmodeChk() ? (s16)marioGetParty() : marioGetPtr()->prevPartyMemberId[0];
#endif
        values->partyHp = pouchGetPtr()->partyData[values->party].currentHP;
        values->partyMaxHp = pouchGetPtr()->partyData[values->party].maximumHP;
#if VERSION <= VERSION_DEMO_US
    if (values->party) {
        wp->cur.lastParty = values->party;
    }
} else {
    values->party = wp->cur.lastParty;
    values->partyHp = pouchGetPtr()->partyData[values->party].currentHP;
    values->partyMaxHp = pouchGetPtr()->partyData[values->party].maximumHP;
}
#endif
    } else {
        bw = _battleWorkPointer;
        mario = BattleGetMarioPtr(bw);
        party = BattleGetPartyPtr(bw);
        values->hp = mario->currentHp;
        values->maxHp = mario->maxHp;
        values->fp = mario->currentFp;
        values->maxFp = mario->maxFp;
        values->ap = pouchGetAP();
        values->maxAp = pouchGetMaxAP();
        values->starPoints = pouchGetStarPoint();
        values->coins = pouchGetCoin();
        if (party) {
            values->party = BattleTransPartyId(party->currentKind);
            values->partyHp = party->currentHp;
            values->partyMaxHp = party->maxHp;
        } else {
            values->party = 0;
            values->partyHp = 0;
            values->partyMaxHp = 0;
        }
    }
}

/**
 * Returns TRUE when any shown value differs from what the HUD last displayed, and marks HP,
 * FP or Star Power changes for a blink.
 */
BOOL valueCheck(void) {
    StatusValue v;
    BOOL changed = FALSE;

    statusGetValue(&v);
    if (wp->cur.hp != v.hp) {
        changed = TRUE;
        wp->pendingBlink |= 1;
    }
    if (wp->cur.maxHp != v.maxHp) {
        changed = TRUE;
    }
    if (wp->cur.fp != v.fp) {
        changed = TRUE;
        wp->pendingBlink |= 2;
    }
    if (wp->cur.maxFp != v.maxFp) {
        changed = TRUE;
    }
    if (wp->cur.ap != v.ap) {
        changed = TRUE;
    }
    if (wp->cur.maxAp != v.maxAp) {
        changed = TRUE;
    }
    if (wp->cur.party != v.party) {
        wp->cur.party = v.party;
    } else if (wp->cur.party) {
        if (wp->cur.partyHp != v.partyHp) {
            changed = TRUE;
            wp->pendingBlink |= 8;
        }
        if (wp->cur.partyMaxHp != v.partyMaxHp) {
            changed = TRUE;
        }
    }
    if (wp->cur.starPoints != v.starPoints) {
        changed = TRUE;
    }
    if (wp->cur.coins != v.coins) {
        changed = TRUE;
        wp->pendingBlink |= 0x10;
    }
    if (!changed) {
        if (wp->pendingBlink != 0U) {
            wp->blink = wp->pendingBlink;
            wp->pendingBlink = 0;
            wp->blinkTimer = gp->framerate * 2;
        }
    } else {
        wp->blink = 0;
    }
    return changed;
}

/**
 * Step for the counting-number animation: a tenth of the remaining difference, at least 1 toward the target,
 * 0 when equal.
 */
static inline s32 calcStep(s32 target, s32 cur) {
    s32 d = target - cur;
    if (d) {
        d /= 10;
        if (!d) {
            d = -1;
            if (target > cur) {
                d = 1;
            }
        }
    }
    return d;
}

#if VERSION <= VERSION_DEMO_US
/**
 * Counts the displayed HUD numbers toward the real values every other frame while the window is open,
 * with the coin counting sound every 8th frame (6th in the demos).
 */
static void valueUpdate(void) {
#else
/**
 * Counts the displayed HUD numbers toward the real values every other frame while the window is open,
 * with the coin counting sound every 8th frame (6th in the demos).
 */
void valueUpdate(void) {
#endif
    static s32 count;
    StatusValue v;
    s32 d;

    statusGetValue(&v);
    count++;
    count %= 1200;
    if (count & 1) {
        return;
    }
    if (!statusWinCheck()) {
        return;
    }
    d = calcStep(v.coins, wp->cur.coins);
#if VERSION <= VERSION_DEMO_US
    if (d && !(count % 6)) {
#else
    if (d && !(count % 8)) {
#endif
        psndSFXOn("SFX_COUNT_COIN1");
    }
    wp->cur.hp += calcStep(v.hp, wp->cur.hp);
    wp->cur.maxHp += calcStep(v.maxHp, wp->cur.maxHp);
    wp->cur.fp += calcStep(v.fp, wp->cur.fp);
    wp->cur.maxFp += calcStep(v.maxFp, wp->cur.maxFp);
    wp->cur.ap += calcStep(v.ap, wp->cur.ap);
    wp->cur.maxAp += calcStep(v.maxAp, wp->cur.maxAp);
    wp->cur.starPoints += calcStep(v.starPoints, wp->cur.starPoints);
    wp->cur.coins += calcStep(v.coins, wp->cur.coins);
    wp->cur.party = v.party;
    if (wp->cur.party) {
        wp->cur.partyHp += calcStep(v.partyHp, wp->cur.partyHp);
        wp->cur.partyMaxHp += calcStep(v.partyMaxHp, wp->cur.partyMaxHp);
    }
    if (wp->cur.starPoints > 99) {
        wp->cur.starPoints = 99;
    }
    if (wp->cur.coins > 999) {
        wp->cur.coins = 999;
    }
}

/** Requests the HUD to slide in. */
void statusWinOpen(void) {
    wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
    wp->flags |= (STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
}

/** Requests the HUD to slide out. */
void statusWinClose(void) {
    wp->flags |= (STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
    wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
}

/** Slides the HUD in and pins it open regardless of Mario's movement, until statusWinForceOff. */
void statusWinForceOpen(void) {
    wp->flags &= ~(STATUS_WINDOW_FLAG_CLOSED | STATUS_WINDOW_FLAG_CLOSE_REQ);
    wp->flags &= ~(STATUS_WINDOW_FLAG_FORCE_CLOSE | STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ);
    wp->flags |= (STATUS_WINDOW_FLAG_FORCE_OPEN | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ);
}

/** Slides the HUD out and keeps it closed, until statusWinForceOff or statusWinForceCloseClear. */
void statusWinForceClose(void) {
    wp->flags &= ~(STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_OPEN_REQ);
    wp->flags &= ~(STATUS_WINDOW_FLAG_FORCE_OPEN | STATUS_WINDOW_FLAG_FORCE_OPEN_REQ);
    wp->flags |= (STATUS_WINDOW_FLAG_FORCE_CLOSE | STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ);
}

/** Drops a forced close without reopening the HUD. */
void statusWinForceCloseClear(void) {
    wp->flags &= ~(STATUS_WINDOW_FLAG_FORCE_CLOSE | STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ);
}

/** Ends a forced open or close, leaving the HUD in the matching normal state; called when message windows close. */
void statusWinForceOff(void) {
    if (wp->flags & STATUS_WINDOW_FLAG_FORCE_OPEN) {
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_OPEN;
        statusWinOpen();
    } else if (wp->flags & STATUS_WINDOW_FLAG_OPEN) {
        statusWinOpen();
    }
    if (wp->flags & STATUS_WINDOW_FLAG_FORCE_CLOSE) {
        wp->flags &= ~STATUS_WINDOW_FLAG_FORCE_CLOSE;
        statusWinClose();
    } else if (wp->flags & STATUS_WINDOW_FLAG_CLOSED) {
        statusWinClose();
    }
}

/** Lets the HUD draw again after statusWinDispOff. */
void statusWinDispOn(void) {
    wp->flags &= ~STATUS_WINDOW_FLAG_DISP_OFF;
}

/** Stops drawing the HUD without changing whether it is open. */
void statusWinDispOff(void) {
    wp->flags |= STATUS_WINDOW_FLAG_DISP_OFF;
}

/** Returns TRUE when the HUD is open or forced open. */
BOOL statusWinCheck(void) {
    return (wp->flags & (STATUS_WINDOW_FLAG_OPEN | STATUS_WINDOW_FLAG_FORCE_OPEN)) ? TRUE : FALSE;
}

/** Returns TRUE when a shown value changed since the last display (see valueCheck). */
BOOL statusWinCheckUpdate(void) {
    return valueCheck() != 0;
}

/** Snaps every displayed HUD value to the current one without the counting animation. */
void statusWinForceUpdate(void) {
    StatusValue v;

    statusGetValue(&v);
    wp->cur.hp = v.hp;
    wp->cur.maxHp = v.maxHp;
    wp->cur.fp = v.fp;
    wp->cur.maxFp = v.maxFp;
    wp->cur.ap = v.ap;
    wp->cur.maxAp = v.maxAp;
    wp->cur.starPoints = v.starPoints;
    wp->cur.coins = v.coins;
    wp->cur.party = v.party;
    if (wp->cur.party) {
        wp->cur.partyHp = v.partyHp;
        wp->cur.partyMaxHp = v.partyMaxHp;
    }
}

/** Snaps the displayed coin count to the current one. */
void statusWinForceUpdateCoin(void) {
    StatusValue v;

    statusGetValue(&v);
    wp->cur.coins = v.coins;
}

/** Returns the screen position of the Star Power gauge, where the audience's Star Power sparkles fly to. */
Vec statusGetApPos(void) {
    Vec pos;
    pos.x = wp->x + (f32)alwaysDt[12].xpos;
    pos.y = wp->y - (f32)alwaysDt[12].ypos;
    pos.z = 0.0f;
    return pos;
}

/** Makes the Star Power number blink for frames, replacing any other blink. */
void statusAPBlink(s32 frames) {
    wp->blink = 0;
    wp->blink |= 4;
    wp->blinkTimer = frames;
}

// Descriptive name (not in the symbol maps).
#if VERSION > VERSION_DEMO_US
void statusClearBlink(void) {
    wp->blink = 0;
}
#endif

/** Makes the FP number blink for frames. */
void statusFPBlink(s32 frames) {
    wp->blink |= 2;
    wp->blinkTimer = frames;
}

/** Makes Mario's HP number blink for frames. */
void statusMarioHPBlink(s32 frames) {
    wp->blink |= 1;
    wp->blinkTimer = frames;
}

/** Makes the partner's HP number blink for frames. */
void statusPartyHPBlink(s32 frames) {
    wp->blink |= 8;
    wp->blinkTimer = frames;
}

// FAKE: unreferenced (dead-stripped at link); it only sets the order of the float constants in the pool, and the
// object changes without it.
void statusWinEase(f32* f) {
    f[0] = 0.0f;
#if VERSION >= VERSION_US
    // US pool order: 0, 60 (framerate step), pi, 1, 0.5
    f[4] = 60.0f;
    f[2] = 3.1415927f;
    f[1] = 1.0f;
#else
    f[1] = 1.0f;
    f[2] = 3.1415927f;
#endif
    f[3] = 0.5f;
}
