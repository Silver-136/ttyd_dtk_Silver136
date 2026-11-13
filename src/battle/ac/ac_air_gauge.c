// Action command: air gauge (ac_air_gauge); press A repeatedly to fill a gauge that constantly drains.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

typedef struct AirGaugeWork {
    s32 frame;      // 0x0, frames since input started
    f32 gauge;      // 0x4, 0..100, drained by _GetGaugeDecValue
    u32 presses;    // 0x8, A presses (1 = success when auto)
    u32 holdFrames; // 0xC, frames A held since the last press
    s32 showPressed; // 0x10, A icon drawn pressed (set by _GetGaugeDecValue)
    s32 endWait;    // 0x14, frames before the result
} AirGaugeWork;

// Hold keys for _GetGaugeDecValue, one table per params[7] level (the seven tables are identical, except _table_3
// in the early demos, which has the later keys): frame = frames
// A has been held, value = amount taken off the base drain (interpolated between keys), flag = A icon drawn
// pressed; frame -1 ends the table.

typedef struct GaugeKey {
    s32 frame;
    f32 value;
    s32 flag;
} GaugeKey;

#if VERSION <= VERSION_DEMO_JP
static GaugeKey _table_0[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_1[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_2[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
#else
static GaugeKey _table_0[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_1[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_2[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
#endif

/**
 * Gauge curve keys {frame, value, flag; -1 ends it} for params[7] level 3 (read by _GetGaugeDecValue);
 * the same in every version.
 */
static GaugeKey _table_3[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};

#if VERSION <= VERSION_DEMO_JP
static GaugeKey _table_4[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_5[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_6[5] = {{0, 0.0f, 1}, {10, 1.2f, 1}, {30, 0.7f, 1}, {50, 0.0f, 0}, {-1, -1.0f, 0}};
#else
static GaugeKey _table_4[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_5[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
static GaugeKey _table_6[5] = {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}};
#endif

static void actionCommandDisp(f32 x, f32 y);
static f32 _GetGaugeDecValue(void);

/**
 * Per-frame driver of action command 14, used by Flurrie's Gale Force.
 * While params[1] frames run, each A press lifts the gauge for a moment (see _GetGaugeDecValue) and it
 * otherwise drains.
 * Sets outputParams[0] to the level reached against thresholds params[3..6] (params[2] levels), [1] to A held,
 * [2] to the final gauge (0..100). Returns 0 when finished; a confused auto-play skips the input and always gets level 0.
 */
s32 battleAcMain_AirGauge(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    AirGaugeWork* pw = (AirGaugeWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (ac->acState) {
        case AC_STATE_INIT:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            pw->frame = 0;
            ac->outputParams[0] = 0;
            ac->outputParams[2] = 0;
            ac->outputParams[1] = 0;
            ac->acState = AC_STATE_WAIT_START;
            ac->result = AC_RESULT_RUNNING;
            pw->gauge = 0.0f;
            pw->presses = 0;
            pw->holdFrames = 0;
            pw->showPressed = 0;
            break;
        case AC_STATE_WAIT_START:
            break;
        case AC_STATE_START:
            if ((ac->setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (ac->acUnit->tokenFlags & kConfusedAction)) {
                // irand(100) is never negative: a confused auto-play always gets level 0; the random level below never runs
                if (irand(100) >= 0) {
                    ac->setupParam = ac->params[3] / 2;
                    ac->outputParams[0] = 0;
                    ac->outputParams[2] = 0;
                } else {
                    s32 v = irand(ac->params[2] - 1) + 1;
                    ac->outputParams[0] = v;
                    switch (v) {
                        case 1:
                            ac->setupParam = ac->params[3] - 1;
                            break;
                        case 2:
                            ac->setupParam = ac->params[4] - 1;
                            break;
                        case 3:
                            ac->setupParam = ac->params[5] - 1;
                            break;
                        case 4:
                        default:
                            ac->setupParam = ac->params[5] + 1;
                            break;
                    }
                    ac->outputParams[2] = 100;
                }
                ac->acState = AC_STATE_RUN + 1;
                break;
            }
            BattleAcGaugeSeInit();
            ac->acState = AC_STATE_RUN;
        case AC_STATE_RUN:
            pw->frame++;
            if (BattlePadCheckNow(PAD_BUTTON_A)) {
                ac->outputParams[1] = 1;
                if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
                    pw->presses++;
                    pw->holdFrames = 0;
                }
                pw->holdFrames++;
            } else {
                ac->outputParams[1] = 0;
                pw->holdFrames = 0;
            }
            pw->gauge -= _GetGaugeDecValue();
            if (ac->acUnit->badgesEquipped.AutoCommand) {
                pw->presses = 1;
                pw->gauge = 100.0f;
            }
            if (pw->gauge < 0.0f) {
                pw->gauge = 0.0f;
            }
            if (pw->gauge > 100.0f) {
                pw->gauge = 100.0f;
            }
            BattleAcGaugeSeUpdate(pw->gauge);
            if (pw->frame >= ac->params[1]) {
                if (ac->acUnit->badgesEquipped.AutoCommand) {
                    pw->presses = 1;
                    pw->gauge = 100.0f;
                }
                if (!pw->presses) {
                    ac->outputParams[0] = 0;
                    ac->outputParams[2] = 0;
                    ac->result = 0;
                    BattleAcGaugeSeDelete();
                    return 0;
                }
                ac->result = AC_RESULT_SUCCESS;
                ac->resultCount++;
                if (pw->gauge < (f32)ac->params[3]) {
                    ac->outputParams[0] = 1;
                } else if (ac->params[2] <= 2) {
                    ac->outputParams[0] = 2;
                } else if (pw->gauge < (f32)ac->params[4]) {
                    ac->outputParams[0] = 2;
                } else if (ac->params[2] <= 3) {
                    ac->outputParams[0] = 3;
                } else if (pw->gauge < (f32)ac->params[5]) {
                    ac->outputParams[0] = 3;
                } else if (ac->params[2] <= 4) {
                    ac->outputParams[0] = 4;
                } else if (pw->gauge < (f32)ac->params[6]) {
                    ac->outputParams[0] = 4;
                } else {
                    ac->outputParams[0] = 5;
                }
                ac->outputParams[2] = (s32)pw->gauge;
                ac->acState = AC_STATE_RUN + 2;
            }
            break;
        case AC_STATE_RUN + 1:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if (ac->outputParams[0] > 0) {
                    ac->result = AC_RESULT_SUCCESS;
                    ac->resultCount++;
                } else {
                    ac->result = 0;
                }
                return 0;
            }
            break;
        case AC_STATE_RUN + 2:
            BattleAcGaugeSeDelete();
            pw->endWait = 60;
            ac->acState = AC_STATE_RUN + 3;
        case AC_STATE_RUN + 3:
            pw->endWait--;
            if (pw->endWait <= 0) {
                ac->acState = AC_STATE_RUN + 4;
            }
            break;
        case AC_STATE_RUN + 4:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

/**
 * Per-frame gauge change: a drain rate chosen by difficulty params[7],
 * minus a lift that rises over the first 20 frames A is held after a press and fades out by frame 80,
 * so the gauge rises while A is held after a press and drains once A is released or held too long.
 * Also sets the flag that shows the A icon as pressed (the key's flag; 1 while A is not held or past the last key).
 * The early demos use a steeper, shorter lift curve and lower drain rates.
 */
static f32 _GetGaugeDecValue(void) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    AirGaugeWork* pw = (AirGaugeWork*)ac->work;
    GaugeKey* t;
    f32 base;

    switch (ac->params[7]) {
#if VERSION <= VERSION_DEMO_JP
        case 0:
            base = 0.0f;
            t = _table_0;
            break;
        case 1:
            base = 0.2f;
            t = _table_1;
            break;
        case 2:
            base = 0.3f;
            t = _table_2;
            break;
        case 3:
            base = 0.4f;
            t = _table_3;
            break;
        case 4:
            base = 0.45f;
            t = _table_4;
            break;
        case 5:
            base = 0.45f;
            t = _table_5;
            break;
#else
        case 0: base = 0.3f; t = _table_0; break;
        case 1: base = 0.33f; t = _table_1; break;
        case 2: base = 0.36f; t = _table_2; break;
        case 3: base = 0.4f; t = _table_3; break;
        case 4: base = 0.44f; t = _table_4; break;
        case 5: base = 0.47f; t = _table_5; break;
#endif
        case 6:
#if VERSION <= VERSION_DEMO_JP
        default:
            t = _table_6;
            base = 0.45f;
#else
        default: t = _table_6;
        base = 0.5f;
#endif
        break;
    }
    while (pw->holdFrames > t->frame) {
        t++;
        if (t->frame < 0) {
            break;
        }
    }
    if (t->frame <= 0) {
        pw->showPressed = 1;
        return base;
    }
    pw->showPressed = t->flag;
    base -= intplGetValue(EASING_LINEAR, t[-1].value, t->value, pw->holdFrames - t[-1].frame, t->frame - t[-1].frame);
    return base;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_AirGauge(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the gauge and A icon in from the left over 20 frames,
 * and once the command ends keeps them up for 40 frames before sliding them back out.
 */
void battleAcDisp_AirGauge(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case AC_STATE_START:
        case AC_STATE_RUN:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN + 2:
        case AC_STATE_RUN + 3:
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

/** Called by btlevtcmd_StopAC: jumps to the end state, which stops the gauge sound and slides the display out. */
void battleAcDelete_AirGauge(BattleWork* work) {
    work->actionCommands.acState = AC_STATE_RUN + 2;
}

/**
 * Draws the level gauge (its fill eased toward the real value by 0.01 a frame) and the A button icon,
 * drawn pressed while _GetGaugeDecValue's flag is set.
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    AirGaugeWork* pw;
    f32 ratio;

    camGetPtr(CAMERA_2D);
    pw = (AirGaugeWork*)ac->work;
    ratio = pw->gauge / 100.0f;
    if (disp->gaugeRatio <= ratio) {
        disp->gaugeRatio += 0.01f;
        if (disp->gaugeRatio > ratio) {
            disp->gaugeRatio = ratio;
        }
    } else {
        disp->gaugeRatio -= 0.01f;
        if (disp->gaugeRatio < ratio) {
            disp->gaugeRatio = ratio;
        }
    }
    BattleAcDrawGauge((s32)x, (s32)y, disp->gaugeRatio, 0xB2, ac->params[2], ac->params[3], ac->params[4], ac->params[5], 0);
    if (pw->showPressed) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 58.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_BUTTON_A_PRESSED, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 58.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_BUTTON_A, 1.0f);
    }
}
