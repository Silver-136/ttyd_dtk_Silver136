#include "icon_ids.h"
#include "battle/ac/ac_table.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>

typedef struct TimingWideWork {
    s32 button;   // 0x0
    s8 mode;      // 0x4
    s32 frame;    // 0x8
    s32 state;    // 0xC
    s32 endTimer; // 0x10
} TimingWideWork;

#define TW_WORK(ac) ((TimingWideWork*)(ac)->work)

static void actionCommandDisp(f32 x, f32 y);

s32 battleAcMain_TimingWide(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    TimingWideWork* tw = TW_WORK(ac);
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;
    s32 autoCmd = 0;
    s32 pressed;

    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        autoCmd = 1;
    }

    switch (ac->mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            tw->state = 0;
            tw->frame = 0;
            switch (ac->params[0]) {
                case 5:
                    tw->button = 0x100;
                    tw->mode = 1;
                    break;
                case 0:
                    tw->button = 0x40000;
                    tw->mode = 0;
                    break;
                case 1:
                    tw->button = 0x80000;
                    tw->mode = 0;
                    break;
                case 2:
                    tw->button = 0x40000;
                    tw->mode = -1;
                    break;
                case 3:
                    tw->button = 0x80000;
                    tw->mode = -1;
                    break;
                default:
                    tw->button = 0x40000;
                    tw->mode = 0;
                    break;
            }
            ac->outputParams[0] = 0;
            ac->outputParams[1] = 0;
            ac->outputParams[2] = 0;
            ac->mAcState = 99;
            ac->result = 1;
            break;
        case 100:
            if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    ac->setupParam = 0;
                    if (ac->params[2] > 0) {
                        ac->setupParam = irand(ac->params[2]);
                    }
                    if (ac->setupParam <= 0) {
                        ac->setupParam = ac->params[1] + 2;
                    }
                } else {
                    ac->setupParam = ac->params[2] + irand(ac->params[3] - ac->params[2]);
                }
                ac->mAcState = 0x3EA;
                tw->state = 2;
            } else {
                if (ac->params[0] == 4) {
                    ac->mAcState = 0x3E8;
                } else {
                    ac->mAcState = 0x3E9;
                }
                tw->state = 1;
            }
            break;
        case 0x3E8:
            if (BattlePadCheckTrigger(tw->button)) {
                ac->outputParams[1] = 1;
                ac->mAcState = 0x3E9;
            }
            break;
        case 0x3E9:
            tw->frame++;
            if (tw->frame > ac->params[1]) {
                ac->result = 0;
                ac->mAcState = 0x3EC;
                tw->state = 4;
                if ((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_HOLD_AFTER_RESULT) {
                    ac->mAcState = 0x3EB;
                    tw->state = 3;
                }
            } else {
                pressed = 0;
                switch (tw->mode) {
                    case 1:
                        if (BattlePadCheckTrigger(tw->button)) {
                            pressed = 1;
                        }
                        break;
                    case -1:
                        if (BattlePadCheckUp(tw->button)) {
                            pressed = 1;
                        }
                        break;
                    case 0:
                    default:
                        if (!BattlePadCheckNow(tw->button)) {
                            pressed = 1;
                        }
                        break;
                }
                if (tw->frame >= ac->params[2] && tw->frame < ac->params[3]) {
                    ac->outputParams[2] = 1;
                } else {
                    ac->outputParams[2] = 0;
                }
                if (autoCmd) {
                    if (ac->outputParams[2]) {
                        pressed = 1;
                    } else {
                        pressed = 0;
                    }
                }
                if (pressed) {
                    if (ac->outputParams[2]) {
                        ac->result = 2;
                        ac->mResultCount++;
                        ac->outputParams[0] = 1;
                    } else {
                        ac->result = 0;
                        ac->outputParams[0] = -1;
                    }
                    ac->mAcState = 0x3EC;
                    tw->state = 4;
                    if ((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_HOLD_AFTER_RESULT) {
                        ac->mAcState = 0x3EB;
                        tw->state = 3;
                    }
                }
            }
            break;
        case 0x3EA:
            tw->frame++;
            ac->setupParam--;
            if (tw->frame > ac->params[1]) {
                ac->result = 0;
                return 0;
            }
            if (ac->setupParam < 0) {
                if (tw->frame >= ac->params[2] && tw->frame < ac->params[3]) {
                    ac->result = 2;
                    ac->mResultCount++;
                    ac->outputParams[0] = 1;
                } else {
                    ac->result = 0;
                    ac->outputParams[0] = -1;
                }
                return 0;
            }
            break;
        case 0x3EB:
            break;
        case 0x3EC:
            tw->endTimer = 60;
            ac->mAcState = 0x3ED;
            tw->state = 5;
        case 0x3ED:
            tw->endTimer--;
            if (tw->endTimer <= 0) {
                ac->mAcState = 0x3EE;
                tw->state = 6;
            }
            break;
        case 0x3EE:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_TimingWide(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_TimingWide(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;
    s32 show;
    switch (TW_WORK(ac)->state) {
        case 0:
        case 1:
        case 2:
            if ((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_NO_SLIDE_IN) {
                disp->iconX = 30.0f;
            } else {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 3:
            actionCommandDisp(disp->iconX, disp->iconY);
            break;
        case 4:
        case 5:
            show = 1;
            if (((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_KEEP_ICON_ON_SUCCESS_UNUSED) && (ac->result & 2)) {
                show = 0;
            }
            if (((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_KEEP_ICON_ON_FAIL_UNUSED) && !(ac->result & 2)) {
                show = 0;
            }
            if (disp->slideTimer >= 40) {
                if (show) {
                    disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -300.0f, disp->slideTimer - 40, 20);
                } else {
                    disp->iconX = 30.0f;
                }
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

void battleAcDelete_TimingWide(BattleWork* work) {
    work->actionCommands.mAcState = 0x3EC;
}

#define ICON(id)                            \
    {                                       \
        Vec pos = {0.0f, 0.0f, 0.0f};       \
        pos.x = -200.0f + x;                \
        pos.y = 50.0f + y;                  \
        iconDispGx(&pos, 0x10, (id), 1.0f); \
    }
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    s32* frame = (s32*)&ac->disp;
    TimingWideWork* tw;
    u16 a;
    u16 b;
    camGetPtr(CAMERA_2D);
    tw = TW_WORK(ac);
    switch (tw->button) {
        case PAD_BUTTON_A:
            b = ICON_BUTTON_A_PRESSED;
            a = ICON_BUTTON_A;
            break;
        case PAD_BUTTON_B:
            b = ICON_BUTTON_B_PRESSED;
            a = ICON_BUTTON_B;
            break;
        case PAD_BUTTON_X:
            b = ICON_BUTTON_X_PRESSED;
            a = ICON_BUTTON_X;
            break;
        case PAD_BUTTON_Y:
            b = ICON_BUTTON_Y_PRESSED;
            a = ICON_BUTTON_Y;
            break;
        case BATTLE_PAD_STICK_LEFT:
            b = ICON_STICK_LEFT;
            a = ICON_STICK_NEUTRAL;
            break;
        case BATTLE_PAD_STICK_RIGHT:
            b = ICON_STICK_RIGHT;
            a = ICON_STICK_NEUTRAL;
            break;
        default:
            b = ICON_STICK_LEFT;
            a = ICON_STICK_NEUTRAL;
            break;
    }
    switch (tw->state) {
        case 0:
            if (tw->mode > 0) ICON(a) else ICON(b)
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            if (!((s32)ac->acFlags & AC_TIMING_WIDE_FLAG_SWAP_RESULT_ICON)) {
                if (tw->mode > 0) ICON(a) else ICON(b)
            } else {
                if (tw->mode > 0) ICON(b) else ICON(a)
            }
            break;
        case 1:
            if (tw->mode != -1) {
                if (tw->frame >= ac->params[2] && tw->frame < ac->params[3]) {
                    if (tw->mode > 0) ICON(b) else ICON(a)
                } else {
                    if (tw->mode > 0) ICON(a) else ICON(b)
                }
            } else {
                if (tw->frame >= ac->params[2]) {
                    ICON(a)
                } else {
                    ICON(b)
                }
            }
            break;
    }
    (*frame)++;
}
