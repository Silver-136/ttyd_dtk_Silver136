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
    s8 iconA;     // 0x2
    s8 iconB;     // 0x3
    s8 roll;      // 0x4
    s8 stall;     // 0x5
    s8 iconDir;   // 0x6
    u8 iconTimer; // 0x7
    u16 count;    // 0x8
    u8 pad_A[2];
    s32 timer;    // 0xC
    u16 slow;     // 0x10
    u8 pad_12[2];
    s32 endTimer; // 0x14
} StickRotateWork;

// FAKE: the no-op (BattleActions*) casts here and on disp below keep the address add early (the plain field reschedules the disp functions)
#define AC_WORK(bw) ((StickRotateWork*)((BattleActions*)&(bw)->actionCommands)->work)


static void actionCommandDisp(f32 x, f32 y);
void actionCommandDisp2(f32 x, f32 y);
u16 _get_icon_id(s8 dir);

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
    work->actionCommands.mAcState = 1002;
}

s32 battleAcMain_StickRotate(BattleWork* work) {
    StickRotateWork* sw;
    BattleACDispParams* disp;
    BattleActions* ac = &work->actionCommands;
    BOOL restart;
    sw = (StickRotateWork*)ac->work;
    disp = &work->actionCommands.disp;

    do {
        restart = FALSE;
        switch (ac->mAcState) {
            case 0:
                ac->result = AC_RESULT_RUNNING;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                sw->iconA = 0;
                sw->iconB = 0;
                sw->iconDir = 0;
                sw->iconTimer = 0;
                sw->slow = 0;
                sw->stall = 0;
                ac->mAcState = 99;
                break;
            case 99:
                break;
            case 100:
                sw->timer = 120;
                ac->mAcState = 1000;
                sw->roll = 0;
                sw->count = 0;
                ac->outputParams[0] = 0;
                if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_GUIDED_UNUSED) {
                    sw->iconA = 3;
                    sw->iconB = 2;
                }
            case 1000:
                if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                    if (ac->setupFlags & 2) {
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
                        ac->mResultCount++;
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
                        ac->mAcState = 1002;
                        break;
                    }
                }
                ac->mAcState = 1001;
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
                    ac->mAcState = 1002;
                    ac->outputParams[0] = work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT];
                }
                sw->timer--;
                sw->stall++;
                if (work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON] != 0 && !BattlePadCheckNow(work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON])) {
                    sw->timer = 0;
                }
                if (sw->timer < 0) {
                    if ((s32)ac->outputParams[0] >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT] && !BattlePadCheckNow(work->actionCommands.params[STICK_ROTATE_PARAM_BUTTON])) {
                        ac->result |= AC_RESULT_SUCCESS;
                    }
                    ac->result &= ~AC_RESULT_RUNNING;
                    restart = TRUE;
                    ac->mAcState = 1002;
                    break;
                }
                sw->dir = _GetInputDir();
                if (sw->dir == -1) {
                    if ((s32)ac->outputParams[0] >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT]) {
                        ac->result |= AC_RESULT_SUCCESS;
                    }
                    ac->result &= ~AC_RESULT_RUNNING;
                    restart = TRUE;
                    ac->mAcState = 1002;
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
                            sw->iconA = sw->dir;
                            sw->iconB = t;
                        } else if ((s8)sw->roll != (s8)r) {
                            stickRotateFail(work);
                            restart = TRUE;
                            break;
                        }
                        if (sw->prevDir != sw->dir) {
                            sw->count++;
                            ac->outputParams[0] = (sw->count + 7) / 8;
                            if ((s32)ac->outputParams[0] > work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT]) {
                                ac->outputParams[0] = work->actionCommands.params[STICK_ROTATE_PARAM_MAX_COUNT];
                            }
                            if (work->actionCommands.acFlags & AC_STICK_ROTATE_FLAG_STRICT_UNUSED) {
                                if (sw->count > 1) {
                                    s8 st = sw->stall;
                                    if (st > work->actionCommands.params[STICK_ROTATE_PARAM_STEPS_B]) {
                                        stickRotateFail(work);
                                        restart = TRUE;
                                        break;
                                    } else if (st < work->actionCommands.params[STICK_ROTATE_PARAM_STEPS_A]) {
                                        sw->slow++;
                                        if (sw->slow > work->actionCommands.params[STICK_ROTATE_PARAM_TIMEOUT]) {
                                            stickRotateFail(work);
                                            restart = TRUE;
                                            break;
                                        }
                                    }
                                }
                                sw->stall = 0;
                                if (sw->roll != 0 && (s8)sw->roll != (s8)r) {
                                    stickRotateFail(work);
                                    restart = TRUE;
                                    break;
                                }
                                if (sw->dir != sw->iconA) {
                                    stickRotateFail(work);
                                    restart = TRUE;
                                    break;
                                } else {
                                    s8 t = sw->iconB + sw->roll;
                                    if (t >= 9) {
                                        t = 1;
                                    }
                                    if (t <= 0) {
                                        t = 8;
                                    }
                                    sw->prevDir = sw->dir;
                                    sw->iconA = sw->iconB;
                                    sw->iconB = t;
                                    if (sw->count >= work->actionCommands.params[STICK_ROTATE_PARAM_NEED_COUNT] * 8) {
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
                ac->mAcState = 1003;
                break;
            case 1003:
                break;
            case 1004:
                sw->endTimer = 60;
                disp->slideTimer = 60;
                ac->mAcState = 1005;
            case 1005:
                sw->endTimer--;
                if (sw->endTimer <= 0) {
                    ac->mAcState = 1006;
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

s32 battleAcResult_StickRotate(BattleWork* work) {
    return work->actionCommands.result;
}

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
    switch (work->actionCommands.mAcState) {
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

#pragma auto_inline off
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

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    StickRotateWork* sw = AC_WORK(work);
    Vec v0, v1, v2, v3, v4, v5;

    switch (work->actionCommands.mAcState) {
        case 99:
        case 100:
            DRAW_STICK_ICON(v0, ICON_STICK_NEUTRAL);
            break;
        case 1000:
            DRAW_STICK_ICON(v1, _get_icon_id(sw->iconA));
            break;
        case 1001:
            if (sw->stall < ac->params[STICK_ROTATE_PARAM_STEPS_A]) {
                if (sw->count <= 1) {
                    DRAW_STICK_ICON(v2, _get_icon_id(sw->iconA));
                } else {
                    DRAW_STICK_ICON(v3, _get_icon_id(sw->prevDir));
                }
            } else {
                DRAW_STICK_ICON(v4, _get_icon_id(sw->iconA));
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

void actionCommandDisp2(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    StickRotateWork* sw = AC_WORK(work);
    Vec v0, v1, v2;

    switch (work->actionCommands.mAcState) {
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
void battleAcDelete_StickRotate(BattleWork* work) {
    work->actionCommands.mAcState = 1004;
}
