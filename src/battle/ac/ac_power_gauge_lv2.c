// Action command: power gauge with level tables (ac_power_gauge_lv2); charged while the button is held.

#include "version.h"
#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>


typedef struct PGLv2Work {
    s32 timer;     // 0x0
    s32 maxFrames; // 0x4
    f32 gauge;     // 0x8
    s32 button;    // 0xC
    f32 rate;      // 0x10
    f32 dec;       // 0x14
    s32 full;      // 0x18
    s32 inZone;    // 0x1C
    s16 zones[3];  // 0x20
    u8 pad26[2];   // 0x26
    s32 endTimer;  // 0x28
} PGLv2Work;


/**
 * Lip Lock gauge speed by difficulty: how fast holding A fills the gauge (300 at the easiest down to 40),
 * and how fast holding during a wrong window drains it (100 rising to 130).
 */
static s32 _power_table[] = {300, 200, 150, 100, 80, 60, 40};
s32 _power_table2[] = {100, 100, 100, 100, 110, 120, 130};

static void actionCommandDisp(f32 x, f32 y);

/**
 * Per-frame driver of action command 17, used by Flurrie's Lip Lock.
 * For 7 seconds holding A fills the gauge (rate params[1], faster at easy difficulty) except during three random
 * 0.75-second windows near 1, 3 and 5 seconds, where holding drains it;
 * it also leaks params[2]/100 a frame and locks once full.
 * outputParams[0] is the level (gauge x params[3] / 100,
 * one more when full in JP and later unless acFlags bit 0); success needs params[4] or more.
 */
s32 battleAcMain_PowerGaugeLv2(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PGLv2Work* g = (PGLv2Work*)ac->work;
    BattleACDispParams* disp = &ac->disp;
    s32 diff = BattleActionCommandGetDifficulty(work);
    s32 flag = 0;
    s32 pressed;
    s32 i;
    s32 w;
    if (ac->acUnit->badgesEquipped.AutoCommand) {
        flag = 1;
    }
    switch (ac->acState) {
        case AC_STATE_INIT:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            disp->colorStep[0] = 0;
            disp->colorStep[1] = -16;
            disp->colorStep[2] = -16;
            disp->colorStep[3] = 0;
            g->timer = 0;
            ac->outputParams[0] = 0;
            ac->acState = AC_STATE_WAIT_START;
            ac->result = AC_RESULT_RUNNING;
            g->maxFrames = sysMsec2Frame(7000);
            g->gauge = 0.0f;
            g->rate = 0.01f * ac->params[1];
            g->dec = 0.01f * ac->params[2];
            g->full = 0;
            g->inZone = 0;
            g->button = PAD_BUTTON_A;
            sysMsec2Frame(1000);
            for (i = 0; i < 3; i++) {
                g->zones[i] = irand(sysMsec2Frame(250)) + (i * 2 + 1) * sysMsec2Frame(1000);
            }
            break;
        case AC_STATE_WAIT_START:
            break;
        case AC_STATE_START:
            if ((ac->setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (ac->acUnit->tokenFlags & kConfusedAction)) {
                // irand(100) is never negative: a confused auto-play always fails; the random level below never runs
                if (irand(100) >= 0) {
                    ac->setupParam = 30;
                    ac->outputParams[0] = 0;
                } else {
                    ac->outputParams[0] = irand(ac->params[3] - 1) + 1;
                    ac->setupParam = 30;
                }
                ac->acState = AC_STATE_RUN + 1;
                break;
            }
            BattleAcGaugeSeInit();
            ac->acState = AC_STATE_RUN;
        case AC_STATE_RUN:
            g->timer++;
            if (g->timer > g->maxFrames) {
                if (ac->outputParams[0] < ac->params[4]) {
                    ac->result = 0;
                    ac->result &= ~AC_RESULT_SUCCESS;
                } else {
                    ac->result = AC_RESULT_SUCCESS;
                    ac->resultCount++;
                }
                ac->acState = AC_STATE_RUN + 2;
                break;
            }
            pressed = BattlePadCheckNow(g->button) != 0;
            w = sysMsec2Frame(750);
            g->inZone = 1;
            for (i = 0; i < 3; i++) {
                s32 z = g->zones[i];
                if (g->timer >= z && g->timer < z + w) {
                    g->inZone = 0;
                    break;
                }
            }
            if (flag) {
                if (g->inZone) {
                    pressed = TRUE;
                } else {
                    pressed = FALSE;
                }
            }
            if (!g->full) {
                if (pressed) {
                    if (g->inZone) {
                        g->gauge += 0.01f * (g->rate * (f32)_power_table[diff]);
                    } else {
                        g->gauge -= 0.01f * (g->rate * (f32)_power_table2[diff]);
                    }
                }
                if (g->gauge >= 100.0f) {
                    g->gauge = 100.0f;
                    g->full = 1;
                } else {
                    g->gauge -= g->dec;
                }
                if (g->gauge <= 0.0f) {
                    g->gauge = 0.0f;
                }
            }
            BattleAcGaugeSeUpdate(g->gauge);
            ac->outputParams[0] = (s32)(g->gauge * (f32)ac->params[3] / 100.0f);
            if (ac->outputParams[0] >= ac->params[3]) {
                ac->outputParams[0] = ac->params[3] - 1;
#if VERSION >= VERSION_JP
            }
            if (!(ac->acFlags & AC_POWER_GAUGE_LV2_FLAG_NO_FULL_BONUS_UNUSED)) {
                if (g->gauge >= 100.0f) {
                    ac->outputParams[0]++;
                }
#endif
            }
            break;
        case AC_STATE_RUN + 1:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if (ac->outputParams[0] >= ac->params[4]) {
                    ac->result = AC_RESULT_SUCCESS;
                    ac->resultCount++;
                } else {
                    ac->result = 0;
                }
                return 0;
            }
            break;
        case AC_STATE_RUN + 2:
            ac->acState = AC_STATE_RUN + 3;
        case AC_STATE_RUN + 3:
            ac->doneFlags |= AC_DONE_FLAG_FINISHED;
            BattleAcGaugeSeDelete();
            g->endTimer = 60;
            ac->acState = AC_STATE_RUN + 4;
        case AC_STATE_RUN + 4:
            g->endTimer--;
            if (g->endTimer <= 0) {
                ac->acState = AC_STATE_RUN + 5;
            }
            break;
        case AC_STATE_RUN + 5:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_PowerGaugeLv2(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the gauge in from the left over 20 frames,
 * and once the command ends keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_PowerGaugeLv2(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case AC_STATE_START:
        case AC_STATE_RUN:
        case AC_STATE_RUN + 2:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN + 3:
        case AC_STATE_RUN + 4:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -300.0f, disp->slideTimer - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer < 60) {
                disp->slideTimer++;
            }
            break;
    }
}

/** Called by btlevtcmd_StopAC: unless the command already finished, stops the gauge sound and starts the end wait. */
void battleAcDelete_PowerGaugeLv2(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & AC_DONE_FLAG_FINISHED)) {
        work->actionCommands.acState = AC_STATE_RUN + 3;
    }
}

// iconDispGx called through a cast: its callers pass different argument types and no single prototype fits
// them all (docs/CLEANUP_BACKLOG.md, function-pointer casts).

/**
 * Draws the gauge with level marks from gaugeParams (pulsing red once full), the A icon and a star,
 * both lit while holding A is safe.
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    PGLv2Work* g = (PGLv2Work*)ac->work;
    f32 ratio;
    s32 full;
    s32 tmp[4];
    s32 cnt;
    s32 i;
    u16 pressedIcon;
    u16 icon;
    f32 px;
    f32 py;
    camGetPtr(CAMERA_2D);
    px = -200.0f + x;
    py = 25.0f + y;
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = px;
        pos.y = py;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_GAUGE_FRAME, 1.0f);
    }
    full = 0;
    ratio = g->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    ac->disp.gaugeRatio = ratio;
    if (g->full) {
        ac->disp.color.g += ac->disp.colorStep[1];
        ac->disp.color.b += ac->disp.colorStep[2];
        if (ac->disp.gaugeRatio >= 1.0f) {
            full |= 1;
        }
        if (ac->disp.color.g <= 0x80 && ac->disp.colorStep[1] < 0) {
            ac->disp.colorStep[1] = ac->disp.colorStep[2] = 0x10;
        }
        if (ac->disp.color.g >= 0xF5 && ac->disp.colorStep[1] > 0) {
            ac->disp.colorStep[1] = ac->disp.colorStep[2] = -0x10;
        }
    }
    cnt = 0;
    for (i = 0; i < 4; i++) {
        tmp[i] = 100;
    }
    for (i = 0; i < 4; i++, cnt++) {
        tmp[i] = ac->gaugeParams[i];
        if (i != 0 && !(tmp[i] - tmp[i - 1] > 0)) {
            break;
        }
    }
    BattleAcDrawGauge((s32)x, (s32)y, ac->disp.gaugeRatio, 0xB2, cnt, tmp[0], tmp[1], tmp[2], full);
    pressedIcon = BattleACGetButtonIcon(g->button, 1);
    icon = BattleACGetButtonIcon(g->button, 0);
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
        case AC_STATE_START:
        case AC_STATE_RUN + 2:
        case AC_STATE_RUN + 3:
            case AC_STATE_RUN + 4: {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = px;
                pos.y = 70.0f + y;
                iconDispGx(&pos, ICON_FLAG_2D, icon, 1.0f);
                break;
            }
        case AC_STATE_RUN:
            if (g->inZone) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = px;
                pos.y = 70.0f + y;
                iconDispGx(&pos, ICON_FLAG_2D, pressedIcon, 1.0f);
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = px;
                pos.y = 70.0f + y;
                iconDispGx(&pos, ICON_FLAG_2D, icon, 1.0f);
            }
            break;
    }
    {
        u16 id = ICON_AC_OFF;
        Vec pos;
        if (g->inZone) {
            id = ICON_AC_ON;
        }
        pos = (Vec){0.0f, 0.0f, 0.0f};
        pos.x = 120.0f + px;
        pos.y = py;
        iconDispGx(&pos, ICON_FLAG_2D, id, 1.0f);
    }
}
