// Action command: rotate the control stick in circles (ac_stick_rotate); more rotations, stronger result.

#include "battle/ac/ac_table.h"
#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/icondrv.h"
#include "system.h"
#include <stdlib.h>
#include <string.h>

// BattleActions.params slots this command reads (btlevtcmd_AcSetParamAll)
typedef enum StickRotateParam {
    STICK_ROTATE_PARAM_MAX_COUNT = 0,
    STICK_ROTATE_PARAM_START_TIME = 1,
    STICK_ROTATE_PARAM_BUTTON = 2,
    STICK_ROTATE_PARAM_NEED_COUNT = 3,
    STICK_ROTATE_PARAM_STEPS_A = 4,
    STICK_ROTATE_PARAM_STEPS_B = 5,
    STICK_ROTATE_PARAM_TIMEOUT = 6,
} StickRotateParam;

typedef struct StickRotateWork {
    s8 dir;       // 0x0
    s8 prevDir;   // 0x1
    s8 expectDir; // 0x2, the direction the next step must reach (also drawn as the stick icon)
    s8 nextDir;   // 0x3, the direction after that
    s8 roll;      // 0x4
    s8 stepFrames; // 0x5, frames since the last step
    s8 iconDir;   // 0x6
    u8 iconTimer; // 0x7
    u16 steps;    // 0x8, direction changes so far; 8 = one full turn
    u8 pad_A[2];
    s32 timer;    // 0xC
    u16 fastSteps; // 0x10, steps made sooner than STICK_ROTATE_PARAM_STEPS_A frames
    u8 pad_12[2];
    s32 endTimer; // 0x14
} StickRotateWork;



static void actionCommandDisp(f32 x, f32 y);
void actionCommandDisp2(f32 x, f32 y);

/**
 * Reads the Control Stick as one of eight directions, 1 up then clockwise to 8 up-left, or 0 when centred.
 * Returns -1 when the command needs a button held (params[2]) and it is not.
 */
s32 _GetInputDir(void) {
    u32 button = _battleWorkPointer->actionCommands.params[STICK_ROTATE_PARAM_BUTTON]; // pad mask

    if (button != 0 && !BattlePadCheckNow(button)) {
        return -1;
    }
    if (BattlePadCheckNow(BATTLE_PAD_STICK_UP)) {
        if (BattlePadCheckNow(BATTLE_PAD_STICK_RIGHT)) {
            return 2;
        }
        if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT)) {
            return 8;
        }
        return 1;
    }
    if (BattlePadCheckNow(BATTLE_PAD_STICK_DOWN)) {
        if (BattlePadCheckNow(BATTLE_PAD_STICK_RIGHT)) {
            return 4;
        }
        return (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) != 0U) + 5;
    }
    if (BattlePadCheckNow(BATTLE_PAD_STICK_RIGHT)) {
        return 3;
    }
    return BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) != 0U ? 7 : 0;
}

/**
 * Which way the stick turned since last frame: +1 clockwise, -1 anticlockwise, 0 when centred, jumping or new;
 * an unchanged direction keeps the current roll.
 */
s32 _CalcRollDir(void) {
    StickRotateWork* w = (StickRotateWork*)_battleWorkPointer->actionCommands.work;
    s8 cur = w->dir;
    s8 prev = w->prevDir;
    s32 d;
    if (cur <= 0) {
        return 0;
    }
    if (prev <= 0) {
        return 0;
    }
    if (cur == prev) {
        return (u8)w->roll;
    }
    if (cur == 1 && prev == 8) {
        return 1;
    }
    if (prev == 1 && cur == 8) {
        return -1;
    }
    d = (s8)(cur - prev);
    if (abs(d) > 1) {
        return 0;
    }
    return d;
}

// Ends the command: clears AC_RESULT_RUNNING (AC_RESULT_SUCCESS stays as it is) and moves to the end state 1002
static inline void stickRotateFail(BattleWork* work) {
    work->actionCommands.result &= ~AC_RESULT_RUNNING;
    work->actionCommands.acState = 1002;
}

/**
 * Per-frame driver of action command 9, which no battle script in the tree sets up.
 * Start rotating the stick (holding button params[2], if set) within 120 frames;
 * every 8 steps is one rotation, counted in outputParams[0] up to params[0].
 * After params[1] frames, or on letting go of the button, it succeeds with params[3] rotations or more.
 * acFlags bit 0 demands a steady one-way turn, each step within params[4]..params[5] frames;
 * bit 1 a fixed direction.
 */
s32 battleAcMain_StickRotate(BattleWork* work) {
    StickRotateWork* sw;
    BattleACDispParams* disp;
    BattleActions* ac = &work->actionCommands;
    BOOL restart;
    sw = (StickRotateWork*)ac->work;
    disp = &work->actionCommands.disp;

    do {
        restart = FALSE;
        switch (ac->acState) {
            case 0:
                ac->result = AC_RESULT_RUNNING;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                sw->expectDir = 0;
                sw->nextDir = 0;
                sw->iconDir = 0;
                sw->iconTimer = 0;
                sw->fastSteps = 0;
                sw->stepFrames = 0;
                ac->acState = 99;
                break;
            case 99:
                break;
            case 100:
                sw->timer = 120;
                ac->acState = 1000;
                sw->roll = 0;
                sw->steps = 0;
                ac->outputParams[0] = 0;
                if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
                    sw->expectDir = 3;
                    sw->nextDir = 2;
                }
            case 1000:
                if ((ac->setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (ac->acUnit->tokenFlags & kConfusedAction)) {
                    if (ac->setupFlags & AC_SETUP_FLAG_RETRIES) {
                        ac->setupParam--;
                        if (ac->setupParam >= 0) {
                            break;
                        }
                    }
                    // irand(100) is never negative: a confused auto-play always fails
                    if (irand(100) >= 0) {
                        ac->result = 0;
                    } else {
                        ac->result = AC_RESULT_SUCCESS;
                        ac->resultCount++;
                    }
                    return 0;
                }
                sw->dir = _GetInputDir();
                if (sw->dir == -1 || (work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON] == 0 && sw->dir <= 0)) {
                    if (ac->countdown <= 0) {
                        if (--sw->timer > 0) {
                            break;
                        }
                        ac->result = 0;
                        restart = TRUE;
                        ac->acState = 1002;
                        break;
                    }
                }
                ac->acState = 1001;
                sw->prevDir = 0;
                ac->outputParams[0]++;
                sw->timer = work->actionCommands.params[STICK_ROTATE_PARAM_START_TIME];
                if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
                    sw->roll = -1;
                    sw->prevDir = 4;
                }
                break;
            case 1001: {
                s32 r;
                if (ac->countdown > 0) {
                    ac->countdown--;
                    ac->result = AC_RESULT_SUCCESS;
                    restart = TRUE;
                    ac->acState = 1002;
                    ac->outputParams[0] = work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT];
                }
                sw->timer--;
                sw->stepFrames++;
                if (work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON] != 0 && !BattlePadCheckNow(work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON])) {
                    sw->timer = 0;
                }
                if (sw->timer < 0) {
                    if ((s32)ac->outputParams[0] >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT] && !BattlePadCheckNow(work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON])) {
                        ac->result |= AC_RESULT_SUCCESS;
                    }
                    ac->result &= ~AC_RESULT_RUNNING;
                    restart = TRUE;
                    ac->acState = 1002;
                    break;
                }
                sw->dir = _GetInputDir();
                if (sw->dir == -1) {
                    if ((s32)ac->outputParams[0] >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT]) {
                        ac->result |= AC_RESULT_SUCCESS;
                    }
                    ac->result &= ~AC_RESULT_RUNNING;
                    restart = TRUE;
                    ac->acState = 1002;
                    break;
                }
                if (sw->dir == 0) {
                    sw->roll = 0;
                    if ((work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_STRICT_UNUSED) && sw->prevDir != 0) {
                        stickRotateFail(work);
                        restart = TRUE;
                        break;
                    }
                } else if (sw->prevDir != 0) {
                    r = _CalcRollDir();
                    if ((s8)r != 0) {
                        if (sw->roll == 0) {
                            s8 t;
                            sw->roll = r;
                            t = sw->dir + sw->roll;
                            if (t >= 9) {
                                t = 1;
                            }
                            if (t <= 0) {
                                t = 8;
                            }
                            sw->expectDir = sw->dir;
                            sw->nextDir = t;
                        } else if ((s8)sw->roll != (s8)r) {
                            stickRotateFail(work);
                            restart = TRUE;
                            break;
                        }
                        if (sw->prevDir != sw->dir) {
                            sw->steps++;
                            ac->outputParams[0] = (sw->steps + 7) / 8;
                            if ((s32)ac->outputParams[0] > work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT]) {
                                ac->outputParams[0] = work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT];
                            }
                            if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_STRICT_UNUSED) {
                                if (sw->steps > 1) {
                                    s8 st = sw->stepFrames;
                                    if (st > work->actionCommands.params[STICK_ROTATE_PARAM_STEPS_B]) {
                                        stickRotateFail(work);
                                        restart = TRUE;
                                        break;
                                    } else if (st < work->actionCommands.params[STICK_ROTATE_PARAM_STEPS_A]) {
                                        sw->fastSteps++;
                                        if (sw->fastSteps > work->actionCommands.params[STICK_ROTATE_PARAM_TIMEOUT]) {
                                            stickRotateFail(work);
                                            restart = TRUE;
                                            break;
                                        }
                                    }
                                }
                                sw->stepFrames = 0;
                                if (sw->roll != 0 && (s8)sw->roll != (s8)r) {
                                    stickRotateFail(work);
                                    restart = TRUE;
                                    break;
                                }
                                if (sw->dir != sw->expectDir) {
                                    stickRotateFail(work);
                                    restart = TRUE;
                                    break;
                                } else {
                                    s8 t = sw->nextDir + sw->roll;
                                    if (t >= 9) {
                                        t = 1;
                                    }
                                    if (t <= 0) {
                                        t = 8;
                                    }
                                    sw->prevDir = sw->dir;
                                    sw->expectDir = sw->nextDir;
                                    sw->nextDir = t;
                                    if (sw->steps >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT] * 8) {
                                        ac->result |= AC_RESULT_SUCCESS;
                                        stickRotateFail(work);
                                        restart = TRUE;
                                        break;
                                    }
                                }
                            }
                        }
                    } else if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
                        sw->roll = 0;
                        // FAKE: GUIDED tested a second time (always true here); without the second test the object changes
                        if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
                            stickRotateFail(work);
                            restart = TRUE;
                            break;
                        }
                    } else if (sw->roll != 0) {
                        sw->roll = 0;
                    }
                }
                sw->prevDir = sw->dir;
                break;
            }
            case 1002:
                ac->acState = 1003;
                break;
            case 1003:
                break;
            case 1004:
                sw->endTimer = 60;
                disp->slideTimer = 60;
                ac->acState = 1005;
            case 1005:
                sw->endTimer--;
                if (sw->endTimer <= 0) {
                    ac->acState = 1006;
                }
                break;
            case 1006:
                ac->maincb = NULL;
                ac->dispcb = NULL;
                ac->resultcb = NULL;
                ac->deletecb = NULL;
                break;
        }
    } while (restart);
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_StickRotate(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the stick icon in over 20 frames and out over 40 after the command is stopped,
 * drawn by actionCommandDisp with acFlags bit 1 and actionCommandDisp2 otherwise.
 */
void battleAcDisp_StickRotate(CameraId cameraId, void* param) {
    BattleACDispParams* disp = &((BattleActions*)&((BattleWork*)param)->actionCommands)->disp;
    void (*fn)(f32, f32);
// FAKE: `work` is a macro for the cast parameter, not a local: a `BattleWork* work = param;` local (before or after
// disp) changes the object in both versions. #undef at the end of the function.
// FAKE: the no-op (BattleActions*) cast on the acFlags test below, as on disp above.
#define work ((BattleWork*)param)

    if (((BattleActions*)&work->actionCommands)->acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
        fn = actionCommandDisp;
    } else {
        fn = actionCommandDisp2;
    }
    switch (work->actionCommands.acState) {
        case 99:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            fn(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 100:
        case 1000:
        case 1001:
        case 1002:
        case 1003:
            disp->iconX = 30.0f;
            fn(disp->iconX, disp->iconY);
            break;
        case 1004:
        case 1005:
        case 1006:
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            if (disp->slideTimer <= 40) {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, disp->slideTimer, 40);
            } else {
                disp->iconX = 30.0f;
            }
            fn(disp->iconX, disp->iconY);
            break;
    }
#undef work
}

// no automatic inlining for _get_icon_id, actionCommandDisp, actionCommandDisp2, as in the original (the object differs without it)
#pragma auto_inline off
/** Icon of the stick tilted in direction 1-8 (see _GetInputDir), or centred for anything else. */
u16 _get_icon_id(s8 dir) {
    switch (dir) {
        case 1:
            return ICON_STICK_UP;
        case 2:
            return ICON_STICK_UP_RIGHT;
        case 3:
            return ICON_STICK_RIGHT;
        case 4:
            return ICON_STICK_DOWN_RIGHT;
        case 5:
            return ICON_STICK_DOWN;
        case 6:
            return ICON_STICK_DOWN_LEFT;
        case 7:
            return ICON_STICK_LEFT;
        case 8:
            return ICON_STICK_UP_LEFT;
        default:
            return ICON_STICK_NEUTRAL;
    }
}

// Draws a stick icon at the command's icon position. A macro: as an inline function the object changes;
// the caller declares one Vec per call site.
#define DRAW_STICK_ICON(v, id)     \
    (v) = (Vec){0.0f, 0.0f, 0.0f}; \
    (v).x = -200.0f + x;           \
    (v).y = 70.0f + y;             \
    iconDispGx(&(v), ICON_FLAG_2D, (id), 1.0f)

/** Stick icon for the fixed-direction mode (acFlags bit 1): shows the direction to turn to next. */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    StickRotateWork* sw = (StickRotateWork*)ac->work;
    Vec v0, v1, v2, v3, v4, v5;

    switch (work->actionCommands.acState) {
        case 99:
        case 100:
            DRAW_STICK_ICON(v0, ICON_STICK_NEUTRAL);
            break;
        case 1000:
            DRAW_STICK_ICON(v1, _get_icon_id(sw->expectDir));
            break;
        case 1001:
            if (sw->stepFrames < ac->params[STICK_ROTATE_PARAM_STEPS_A]) {
                if (sw->steps <= 1) {
                    DRAW_STICK_ICON(v2, _get_icon_id(sw->expectDir));
                } else {
                    DRAW_STICK_ICON(v3, _get_icon_id(sw->prevDir));
                }
            } else {
                DRAW_STICK_ICON(v4, _get_icon_id(sw->expectDir));
            }
            break;
        case 1002:
        case 1003:
        case 1004:
        case 1005:
        case 1006:
            DRAW_STICK_ICON(v5, ICON_STICK_NEUTRAL);
            break;
    }
}

/**
 * Stick icon for the free mode: spins through the eight directions as a hint,
 * following the player's turn direction.
 */
void actionCommandDisp2(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    StickRotateWork* sw = (StickRotateWork*)ac->work;
    Vec v0, v1, v2;

    switch (work->actionCommands.acState) {
        case 99:
        case 100:
            DRAW_STICK_ICON(v0, ICON_STICK_NEUTRAL);
            break;
        case 1000:
        case 1001:
            sw->iconTimer++;
            if (sw->iconTimer >= (ac->params[STICK_ROTATE_PARAM_STEPS_A] + ac->params[STICK_ROTATE_PARAM_STEPS_B]) / 2) {
                s32 step;
                sw->iconTimer = 0;
                step = sw->roll;
                if (step == 0) {
                    step = -1;
                }
                if (sw->iconDir == 0) {
                    sw->iconDir = 1;
                } else {
                    sw->iconDir += step;
                    if (sw->iconDir >= 9) {
                        sw->iconDir = 1;
                    } else if (sw->iconDir <= 0) {
                        sw->iconDir = 8;
                    }
                }
            }
            DRAW_STICK_ICON(v1, _get_icon_id(sw->iconDir));
            break;
        case 1002:
        case 1003:
        case 1004:
        case 1005:
        case 1006:
            DRAW_STICK_ICON(v2, ICON_STICK_NEUTRAL);
            break;
    }
}

#pragma auto_inline reset
/** Called by btlevtcmd_StopAC: starts the slide-out and the 60-frame end wait. */
void battleAcDelete_StickRotate(BattleWork* work) {
    work->actionCommands.acState = 1004;
}
