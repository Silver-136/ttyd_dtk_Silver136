// Action command: power gauge with levels (ac_power_gauge_lv); the gauge fill is graded into success levels.

#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

typedef struct PowerGaugeWork {
    s32 counter;   // 0x0
    f32 gauge;     // 0x4
    s32 field_0x8; // 0x8
    s32 limit;     // 0xC
    s32 timer;     // 0x10
} PowerGaugeWork;



/**
 * Per-frame driver of action command 16, which no battle script in the tree sets up.
 * A gauge fills over 100 frames; holding exactly L and R together stops it,
 * succeeding at params[2] or above with outputParams[0] set to the level reached against thresholds params[3..5]
 * (params[1] levels). Fails if the gauge fills without the press.
 */
s32 battleAcMain_PowerGaugeLv(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* tw = (PowerGaugeWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.acState) {
        case AC_STATE_WAIT_START:
            break;
        case AC_STATE_INIT:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            tw->counter = 0;
            work->actionCommands.outputParams[0] = 0;
            work->actionCommands.acState = AC_STATE_WAIT_START;
            work->actionCommands.result = AC_RESULT_RUNNING;
            tw->limit = 100;
            tw->gauge = 0.0f;
            break;
        case AC_STATE_START:
            if ((work->actionCommands.setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (work->actionCommands.acUnit->tokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    work->actionCommands.setupParam = 30;
                    work->actionCommands.outputParams[0] = 0;
                } else {
                    work->actionCommands.outputParams[0] = irand(ac->params[1] - 1) + 1;
                    work->actionCommands.setupParam = 30;
                }
                work->actionCommands.acState = AC_STATE_RUN + 1;
                break;
            }
            work->actionCommands.acState = AC_STATE_RUN;
        case AC_STATE_RUN:
            tw->counter++;
            if (tw->counter > tw->limit) {
                work->actionCommands.outputParams[0] = 0;
                work->actionCommands.result = 0;
                tw->gauge = 101.0f;
                return 0;
            }
            tw->gauge = intplGetValue(EASING_CUBIC_IN, 0.0f, 100.0f, tw->counter, tw->limit);
            if (BattlePadGetNow() == (PAD_TRIGGER_L | PAD_TRIGGER_R)) {
                if (tw->gauge < (f32)ac->params[2]) {
                    work->actionCommands.result = 0;
                    work->actionCommands.result &= ~AC_RESULT_SUCCESS;
                } else {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.resultCount++;
                    if (ac->params[1] <= 1) {
                        work->actionCommands.outputParams[0] = 1;
                    } else if (tw->gauge < (f32)ac->params[3]) {
                        work->actionCommands.outputParams[0] = 1;
                    } else if (ac->params[1] <= 2) {
                        work->actionCommands.outputParams[0] = 2;
                    } else if (tw->gauge < (f32)ac->params[4]) {
                        work->actionCommands.outputParams[0] = 2;
                    } else if (ac->params[1] <= 3) {
                        work->actionCommands.outputParams[0] = 3;
                    } else if (tw->gauge < (f32)ac->params[5]) {
                        work->actionCommands.outputParams[0] = 3;
                    } else if (ac->params[1] <= 4) {
                        work->actionCommands.outputParams[0] = 4;
                    } else {
                        work->actionCommands.outputParams[0] = 5;
                    }
                }
                work->actionCommands.acState = AC_STATE_RUN + 2;
            }
            break;
        case AC_STATE_RUN + 1:
            work->actionCommands.setupParam--;
            if (work->actionCommands.setupParam < 0) {
                if (work->actionCommands.outputParams[0] > 0) {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.resultCount++;
                } else {
                    work->actionCommands.result = 0;
                }
                return 0;
            }
            break;
        case AC_STATE_RUN + 2:
            work->actionCommands.acState = AC_STATE_RUN + 3;
        case AC_STATE_RUN + 3:
            work->actionCommands.doneFlags |= AC_DONE_FLAG_FINISHED;
            tw->timer = 60;
            work->actionCommands.acState = AC_STATE_RUN + 4;
        case AC_STATE_RUN + 4:
            tw->timer--;
            if (tw->timer <= 0) {
                work->actionCommands.acState = AC_STATE_RUN + 5;
            }
            break;
        case AC_STATE_RUN + 5:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_PowerGaugeLv(BattleWork* work) {
    return work->actionCommands.result;
}


static void actionCommandDisp(f32 x, f32 y);

#pragma opt_propagation off
/**
 * Display callback: slides the gauge in from the left over 20 frames,
 * and once the command ends keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_PowerGaugeLv(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleACDispParams* disp = &work->actionCommands.disp;
    s32 i = 4;
    switch (work->actionCommands.acState) {
        case AC_STATE_WAIT_START:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - AC_SLIDE_TIMER_AT(disp, i), 20);
        case AC_STATE_START:
        case AC_STATE_RUN:
        case AC_STATE_RUN + 2:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (AC_SLIDE_TIMER_AT(disp, i) > 0) {
                (AC_SLIDE_TIMER_AT(disp, i))--;
            }
            break;
        case AC_STATE_RUN + 3:
        case AC_STATE_RUN + 4:
            if (AC_SLIDE_TIMER_AT(disp, i) >= 40) {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -300.0f, AC_SLIDE_TIMER_AT(disp, i) - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (AC_SLIDE_TIMER_AT(disp, i) < 60) {
                (AC_SLIDE_TIMER_AT(disp, i))++;
            }
            break;
    }
}

#pragma opt_propagation reset
/** Called by btlevtcmd_StopAC: unless the command already finished, starts the 60-frame end wait. */
void battleAcDelete_PowerGaugeLv(BattleWork* work) {
    if (work->actionCommands.doneFlags & AC_DONE_FLAG_FINISHED) {
        return;
    }
    work->actionCommands.acState = AC_STATE_RUN + 3;
}

/**
 * Draws the A icon frame, the level gauge and the L and R icons,
 * shown pressed while the gauge is in the success zone.
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    PowerGaugeWork* tw;
    f32 ratio;
    camGetPtr(CAMERA_2D);
    tw = (PowerGaugeWork*)ac->work;
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 25.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_GAUGE_FRAME, 1.0f);
    }
    ratio = tw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;
    BattleAcDrawGauge((s32)x, (s32)y, disp->gaugeRatio, 0xB2, work->actionCommands.params[1], work->actionCommands.params[2], work->actionCommands.params[3], work->actionCommands.params[4], 0);
    switch (work->actionCommands.acState) {
        case AC_STATE_WAIT_START:
        case AC_STATE_START:
        case AC_STATE_RUN + 2:
        case AC_STATE_RUN + 3:
        case AC_STATE_RUN + 4: {
            Vec pos = {0.0f, 0.0f, 0.0f};
            x = -220.0f + x;
            y = 70.0f + y;
            pos.x = x;
            pos.y = y;
            iconDispGx(&pos, ICON_FLAG_2D, ICON_TRIGGER_L, 1.0f);
            {
                Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = 56.0f + x;
                pos2.y = y;
                iconDispGx(&pos2, ICON_FLAG_2D, ICON_TRIGGER_R, 1.0f);
            }
            break;
        }
        case AC_STATE_RUN:
            if (tw->gauge >= (f32)work->actionCommands.params[2] && tw->gauge <= 100.0f) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                x = -220.0f + x;
                y = 70.0f + y;
                pos.x = x;
                pos.y = y;
                iconDispGx(&pos, ICON_FLAG_2D, ICON_TRIGGER_L_PRESSED, 1.0f);
                {
                    Vec pos2 = {0.0f, 0.0f, 0.0f};
                    pos2.x = 56.0f + x;
                    pos2.y = y;
                    iconDispGx(&pos2, ICON_FLAG_2D, ICON_TRIGGER_R_PRESSED, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                x = -220.0f + x;
                y = 70.0f + y;
                pos.x = x;
                pos.y = y;
                iconDispGx(&pos, ICON_FLAG_2D, ICON_TRIGGER_L, 1.0f);
                {
                    Vec pos2 = {0.0f, 0.0f, 0.0f};
                    pos2.x = 56.0f + x;
                    pos2.y = y;
                    iconDispGx(&pos2, ICON_FLAG_2D, ICON_TRIGGER_R, 1.0f);
                }
            }
            break;
    }
}
