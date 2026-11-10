// Action command: power gauge (ac_power_gauge); a gauge charged with the given button (A, L or R).

#include "version.h"
#include "battle/ac/ac_table.h"
#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>

typedef struct PowerGaugeWork {
    s32 frame;     // 0x0
    f32 gauge;     // 0x4
    s32 frame2;    // 0x8
    s32 maxFrames; // 0xC
    s8 autoSuccess; // 0x10, result of the confused auto-play (1 = success)
    s8 phase;      // 0x11
    s8 mode;       // 0x12
    u8 pad13;      // 0x13
    u32 button;    // 0x14
    s32 endTimer;  // 0x18
} PowerGaugeWork;

static void actionCommandDisp(f32 x, f32 y);

/**
 * Per-frame driver of action command 15, used by Bobbery's Bomb, Yoshi's Gulp and Ms. Mowz's Kiss Thief.
 * A gauge fills over params[1] frames (curve params[3]) and stays full for params[2] more.
 * params[0] 0/1: press L+R or A while it is at params[4] or above;
 * 2/3: hold A or R (within 120 frames) to fill it and release at params[4] or above.
 * acFlags bit 0 fills only while held. Running out of time fails.
 */
s32 battleAcMain_PowerGauge(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* pw = (PowerGaugeWork*)ac->work;
    BOOL isAuto = FALSE;
    BattleACDispParams* disp = &ac->disp;

    if (ac->acUnit->badgesEquipped.AutoCommand) {
        isAuto = TRUE;
    }

    switch (ac->acState) {
        case AC_STATE_INIT:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            pw->frame = 0;
            pw->frame2 = 0;
            ac->outputParams[0] = 0;
            ac->acState = AC_STATE_WAIT_START;
            ac->result = AC_RESULT_RUNNING;
            pw->maxFrames = ac->params[1] + ac->params[2];
            pw->gauge = 0.0f;
            switch (ac->params[0]) {
                case 0:
                    pw->mode = 1;
                    pw->button = PAD_TRIGGER_L | PAD_TRIGGER_R;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
                case 1:
                    pw->mode = 1;
                    pw->button = PAD_BUTTON_A;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
                case 2:
                    pw->mode = 0;
                    pw->button = PAD_BUTTON_A;
                    pw->phase = 0;
                    break;
                case 3:
                    pw->mode = 0;
                    pw->button = PAD_TRIGGER_R;
                    pw->phase = 0;
                    break;
                default:
                    pw->mode = 1;
                    pw->button = PAD_BUTTON_A;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
            }
            break;
        case AC_STATE_WAIT_START:
            break;
        case AC_STATE_START:
            if ((ac->setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (ac->acUnit->tokenFlags & kConfusedAction)) {
                // irand(100) is never negative: a confused auto-play always fails
                if (irand(100) >= 0) {
                    ac->setupParam = 30;
                    pw->autoSuccess = 0;
                } else {
                    pw->autoSuccess = 1;
                    ac->setupParam = 30;
                }
                ac->outputParams[0] = 1;
                ac->acState = AC_STATE_RUN + 2;
                break;
            }
            if (pw->mode) {
                ac->acState = AC_STATE_RUN + 1;
            } else {
                ac->acState = AC_STATE_RUN;
            }
            if (pw->mode) {
                BattleAcGaugeSeInit();
            }
        case AC_STATE_RUN:
        case AC_STATE_RUN + 1:
            pw->frame++;
            switch (pw->phase) {
                case 0:
                    if (!pw->mode) {
                        if (pw->button == BattlePadGetNow() || isAuto) {
                            pw->phase = 1;
                            ac->acState = AC_STATE_RUN + 1;
                            pw->frame = 0;
                            ac->outputParams[0] = 1;
                            BattleAcGaugeSeInit();
                        }
                    }
                    break;
                case 1:
                    if (pw->mode) {
                        if (pw->button == BattlePadGetNow()) {
                            if (BattlePadCheckTrigger(pw->button)) {
                                pw->phase = 2;
                            }
                        }
                    } else {
                        if (!BattlePadCheckNow(pw->button)) {
                            if (BattlePadCheckUp(pw->button)) {
                                pw->phase = 2;
                            }
                        }
                    }
                    break;
            }
            if (!(ac->acFlags & AC_POWER_GAUGE_FLAG_FILL_WHILE_HELD)) {
                pw->frame2++;
            } else if (pw->phase == 1) {
                pw->frame2++;
            }
            if (ac->acState == AC_STATE_RUN) {
                if (pw->frame > 120) {
                    pw->frame = 0;
                    ac->result = 0;
                    pw->gauge = 0.0f;
                    ac->acState = AC_STATE_RUN + 3;
                    ac->outputParams[0] = 2;
                    break;
                }
            } else if (pw->frame > pw->maxFrames) {
                pw->frame = 0;
                ac->result = 0;
                pw->gauge = 101.0f;
                ac->acState = AC_STATE_RUN + 3;
                break;
            }
            if (pw->frame <= ac->params[1]) {
                pw->gauge = intplGetValue(ac->params[3], 0.0f, 100.0f, pw->frame2, ac->params[1]);
#if VERSION >= VERSION_US
                if (pw->gauge > 100.0f) {
                    pw->gauge = 100.0f;
                }
#endif
            } else {
                pw->gauge = 100.0f;
            }
            BattleAcGaugeSeUpdate(pw->gauge);
            if (isAuto) {
                if (pw->gauge >= (f32)ac->params[4]) {
                    pw->phase = 2;
                } else {
                    pw->phase = 1;
                }
            }
            if (pw->phase == 2) {
                if (pw->gauge < (f32)ac->params[4]) {
                    ac->result = 0;
                    ac->result &= ~AC_RESULT_SUCCESS;
                } else {
                    ac->result = AC_RESULT_SUCCESS;
                    ac->resultCount++;
                }
                ac->acState = AC_STATE_RUN + 3;
            }
            break;
        case AC_STATE_RUN + 2:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if (pw->autoSuccess) {
                    ac->result = AC_RESULT_SUCCESS;
                    ac->resultCount++;
                } else {
                    ac->result = 0;
                }
                ac->doneFlags |= AC_DONE_FLAG_FINISHED;
                pw->endTimer = 60;
                ac->acState = AC_STATE_RUN + 5;
                return 0;
            }
            break;
        case AC_STATE_RUN + 3:
            ac->acState = AC_STATE_RUN + 4;
        case AC_STATE_RUN + 4:
            ac->doneFlags |= AC_DONE_FLAG_FINISHED;
            BattleAcGaugeSeDelete();
            pw->endTimer = 60;
            ac->acState = AC_STATE_RUN + 5;
        case AC_STATE_RUN + 5:
            pw->endTimer--;
            if (pw->endTimer <= 0) {
                ac->acState = AC_STATE_RUN + 6;
            }
            break;
        case AC_STATE_RUN + 6:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_PowerGauge(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the gauge in from the left over 20 frames,
 * and once the command ends keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_PowerGauge(CameraId cameraId, void* param) {
    BattleWork* work = param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case AC_STATE_START:
        case AC_STATE_RUN:
        case AC_STATE_RUN + 1:
        case AC_STATE_RUN + 3:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN + 4:
        case AC_STATE_RUN + 5:
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
void battleAcDelete_PowerGauge(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & AC_DONE_FLAG_FINISHED)) {
        work->actionCommands.acState = AC_STATE_RUN + 4;
    }
}

/**
 * Draws the gauge with its params[4] threshold (acFlags bit 1 hides it), the threshold marker or,
 * with acFlags bit 2, a star that lights in the zone (bit 3 shifts it left), and the button icons,
 * flashed as a cue to press or release.
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* pw;
    u16 a, b, c, d;
    BattleACDispParams* disp = &ac->disp;
    f32 ratio;
    u32 flags;

    camGetPtr(CAMERA_2D);
    pw = (PowerGaugeWork*)ac->work;
    ratio = pw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;

    if (ac->acFlags & AC_POWER_GAUGE_FLAG_PLAIN_GAUGE) {
        BattleAcDrawGauge((s32)x, (s32)y, ratio, 0xB2, 1, 100, 100, 100, 0);
    } else {
        BattleAcDrawGauge((s32)x, (s32)y, ratio, 0xB2, 2, ac->params[4], 100, 100, 0);
    }

    flags = ac->acFlags;
    if (!(flags & AC_POWER_GAUGE_FLAG_SHOW_ZONE_LAMP)) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -288.0f + x + (f32)(ac->params[4] * 0xB0) / 100.0f;
        pos.y = 45.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_GAUGE_MARKER, 1.0f);
    } else {
        u16 icon;
        if (pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) {
            icon = ICON_AC_ON;
        } else {
            icon = ICON_AC_OFF;
        }
        if (flags & AC_POWER_GAUGE_FLAG_LAMP_SHIFT_LEFT) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = 120.0f + (-200.0f + x) - 12.0f;
            pos.y = 18.0f + y;
            iconDispGx(&pos, ICON_FLAG_2D, icon, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = 120.0f + (-200.0f + x);
            pos.y = 18.0f + y;
            iconDispGx(&pos, ICON_FLAG_2D, icon, 1.0f);
        }
    }

    // No default: a, b, c and d are set only for params[0] 0..3, although the main function accepts other values.
    switch (ac->params[0]) {
        case 0:
            a = ICON_TRIGGER_L_PRESSED;
            b = ICON_TRIGGER_L;
            c = ICON_TRIGGER_R_PRESSED;
            d = ICON_TRIGGER_R;
            break;
        case 1:
            a = BattleACGetButtonIcon(PAD_BUTTON_A, 1);
            b = BattleACGetButtonIcon(PAD_BUTTON_A, 0);
            c = 0;
            d = 0;
            break;
        case 2:
            a = BattleACGetButtonIcon(PAD_BUTTON_A, 1);
            b = BattleACGetButtonIcon(PAD_BUTTON_A, 0);
            c = 0;
            d = 0;
            break;
        case 3:
            a = BattleACGetButtonIcon(PAD_TRIGGER_R, 1);
            b = BattleACGetButtonIcon(PAD_TRIGGER_R, 0);
            c = 0;
            d = 0;
            break;
    }

    switch (ac->acState) {
        case AC_STATE_WAIT_START:
        case AC_STATE_START:
        case AC_STATE_RUN + 3:
        case AC_STATE_RUN + 4:
        case AC_STATE_RUN + 5:
            if ((u16)c) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                f32 px = -225.0f + x;
                f32 py = 80.0f + y;
                pos.x = px;
                pos.y = py;
                iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
                {
                    Vec pos2 = {0.0f, 0.0f, 0.0f};
                    pos2.x = 56.0f + px;
                    pos2.y = py;
                    iconDispGx(&pos2, ICON_FLAG_2D, d, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = 28.0f + (-225.0f + x);
                pos.y = 80.0f + y;
                iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
            }
            break;
        case AC_STATE_RUN:
        case AC_STATE_RUN + 1:
            if (pw->mode) {
                if (pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        iconDispGx(&pos, ICON_FLAG_2D, a, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            iconDispGx(&pos2, ICON_FLAG_2D, c, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        iconDispGx(&pos, ICON_FLAG_2D, a, 1.0f);
                    }
                } else {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            iconDispGx(&pos2, ICON_FLAG_2D, d, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
                    }
                }
            } else {
                if ((pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) || ac->outputParams[0] == 0) {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            iconDispGx(&pos2, ICON_FLAG_2D, d, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        iconDispGx(&pos, ICON_FLAG_2D, b, 1.0f);
                    }
                } else {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        iconDispGx(&pos, ICON_FLAG_2D, a, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            iconDispGx(&pos2, ICON_FLAG_2D, c, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        iconDispGx(&pos, ICON_FLAG_2D, a, 1.0f);
                    }
                }
            }
            break;
    }
}
