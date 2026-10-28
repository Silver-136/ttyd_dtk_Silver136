#include "battle/ac/ac_table.h"
#include "version.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>
#include "icon_ids.h"
#include "dolphin/si.h"


typedef struct RepeatWork {
    u32 btn0;         // 0x0
    u32 btn1;         // 0x4
    u32 btn2;         // 0x8
    u32 holdMask;     // 0xC
    f32 pos;          // 0x10
    f32 vel;          // 0x14
    f32 gauge;        // 0x18
    f32 decay;        // 0x1C
    u16 count;        // 0x20
    u16 timer;        // 0x22
    s16 power;        // 0x24
    s16 graceStart;   // 0x26, initial grace, never read
    s16 grace;        // 0x28
    u8 iconParam;     // 0x2A, gauge width: the third int argument of BattleAcDrawGauge (x, y, width, ...)
    s8 done;          // 0x2B
    u8 resetFlag;     // 0x2C
    u8 pad_2D[3];     // 0x2D, never accessed
    s32 endTimer;     // 0x30
} RepeatWork;


static void actionCommandDisp(f32 x, f32 y);
void battleAcDelete_Repeatedly(BattleWork* work);


static s32 _power_table[] = {55, 50, 45, 40, 35, 30, 25};

static void _init_param(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    RepeatWork* r = (RepeatWork*)ac->work;
    BattleACDispParams* d = &ac->disp;
    r->resetFlag = 0;
    r->holdMask = 0;
    switch (work->actionCommands.params[0]) {
        case 10:
            work->flags |= BATTLE_FLAG_DISABLE_HRESET;
            break;
        case 8:
            r->holdMask = PAD_BUTTON_A;
        case 6:
            r->btn0 = r->btn1 = r->btn2 = BATTLE_PAD_STICK_LEFT;
            break;
        case 2:
            r->btn0 = r->btn1 = r->btn2 = PAD_BUTTON_B;
            break;
        case 3:
            r->btn0 = r->btn1 = r->btn2 = PAD_BUTTON_X;
            break;
        case 4:
            r->btn0 = r->btn1 = r->btn2 = PAD_BUTTON_Y;
            break;
        case 5:
            r->btn0 = r->btn1 = r->btn2 = PAD_TRIGGER_R;
            break;
        case 11:
            r->btn0 = r->btn1 = PAD_BUTTON_A;
            r->btn2 = PAD_BUTTON_B;
            break;
        case 16:
            r->btn0 = r->btn1 = PAD_BUTTON_A;
            r->btn2 = PAD_BUTTON_B;
            r->timer = irand(30) + 60;
            break;
        case 12:
            r->btn0 = r->btn1 = PAD_BUTTON_X;
            r->btn2 = PAD_BUTTON_Y;
            break;
        default:
            r->btn0 = r->btn1 = r->btn2 = PAD_BUTTON_A;
            break;
    }
    r->graceStart = r->grace = work->actionCommands.params[1];
    r->pos = work->actionCommands.params[2];
    r->vel = work->actionCommands.params[3] / 10.0f;
    if (r->vel) {
        r->pos = irand(100);
    }
    r->gauge = work->actionCommands.params[7];
    d->gaugeRatio = 0.01f * r->gauge;
    r->count = 0;
    r->power = work->actionCommands.params[5];
    r->decay = 0.01f * work->actionCommands.params[6];
    r->iconParam = 0xB2;
    if (work->actionCommands.countdown > 0) {
        work->actionCommands.countdown--;
        r->done = 1;
    } else {
        r->done = 0;
    }
    work->actionCommands.outputParams[0] = 0;
}

s32 battleAcMain_Repeatedly(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    RepeatWork* r = (RepeatWork*)ac->work;
    BattleACDispParams* d = &ac->disp;
    s32 diff = BattleActionCommandGetDifficulty(work);
    s32 pressed;
    BOOL autoCmd = FALSE;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCmd = TRUE;
    }
    pressed = 0;
    switch (work->actionCommands.mAcState) {
        case 0:
            memset(d, 0, sizeof(BattleACDispParams));
            d->iconX = -300.0f;
            d->iconY = 0.0f;
            d->slideTimer = 20;
            d->color = (GXColor){255, 255, 255, 255};
            d->colorStep[0] = 0;
            d->colorStep[1] = -16;
            d->colorStep[2] = -16;
            d->colorStep[3] = 0;
            d->gaugeRatio = 0.0f;
            work->actionCommands.result = AC_RESULT_RUNNING;
            r->grace = 0;
            _init_param(work);
            work->actionCommands.mAcState = 99;
            break;
        case 99:
            break;
        case 100:
            if (!((work->actionCommands.setupFlags & 1) &&
                  (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                if (r->holdMask) {
                    if (!BattlePadCheckNow(r->holdMask)) {
                        break;
                    }
                }
            }
            BattleAcGaugeSeInit();
            work->actionCommands.mAcState = 1000;
            work->actionCommands.outputParams[0] = 1;
        case 1000:
            switch (work->actionCommands.params[0]) {
                case 10:
                    if (r->resetFlag) {
                        if (!OSGetResetButtonState()) {
                            r->resetFlag = 0;
                            pressed = 1;
                            r->count++;
                        }
                    } else {
                        if (OSGetResetButtonState()) {
                            r->resetFlag = 1;
                            pressed = 1;
                            r->count++;
                        }
                    }
                    break;
                case 9:
                    if (r->resetFlag) {
                        if (SIProbe(0) == SI_ERROR_NO_RESPONSE) {
                            r->resetFlag = 0;
                            pressed = 1;
                            r->count++;
                        }
                    } else {
                        if (SIProbe(0) != SI_ERROR_NO_RESPONSE && SIProbe(0) != SI_ERROR_BUSY) {
                            r->resetFlag = 1;
                            pressed = 1;
                            r->count++;
                        }
                    }
                    break;
                case 16:
                    r->timer--;
                    if (r->timer == 0) {
                        r->timer = irand(30) + 60;
                        if (r->btn0 == r->btn1) {
                            r->btn0 = r->btn2;
                        } else {
                            r->btn0 = r->btn1;
                        }
                    }
                    if (BattlePadCheckTrigger(r->btn0)) {
                        pressed = 1;
                        r->count++;
                    }
                    break;
                case 11:
                case 12:
                    if (r->count & 1) {
                        r->btn0 = r->btn2;
                    } else {
                        r->btn0 = r->btn1;
                    }
                default:
                    if (BattlePadCheckTrigger(r->btn0)) {
                        pressed = 1;
                        r->count++;
                    }
                    break;
            }
            if (autoCmd && !pressed) {
                pressed = 1;
                r->count++;
            }
            if (!r->done) {
                if (pressed) {
                    r->gauge += 0.01f * ((f32)_power_table[diff] * (f32)r->power);
                }
                r->gauge -= r->decay;
                if (r->gauge <= 0.0f) {
                    r->gauge = 0.0f;
                }
                if (r->gauge >= 100.0f) {
                    r->gauge = 100.0f;
                    r->done = 1;
                }
            }
            if (0.0f != r->vel) {
                r->pos += r->vel;
                if (r->pos <= 0.0f) {
                    r->pos = 0.0f;
                    r->vel *= -1.0f;
                }
                if (r->pos >= 100.0f) {
                    r->pos = 100.0f;
                    r->vel *= -1.0f;
                }
            }
            if (r->holdMask) {
                if (!BattlePadCheckNow(r->holdMask)) {
                    r->grace = 0;
                }
            }
            BattleAcGaugeSeUpdate(r->gauge);
            if (r->grace > 0) {
                r->grace--;
                break;
            }
            work->actionCommands.result &= ~AC_RESULT_RUNNING;
            if (r->gauge >= r->pos || r->done) {
                work->actionCommands.result |= AC_RESULT_SUCCESS;
            }
            battleAcDelete_Repeatedly(work);
            work->actionCommands.mAcState = 1002;
            break;
        case 1002:
            r->endTimer = 60;
            work->actionCommands.mAcState = 1003;
            break;
        case 1003:
            r->endTimer--;
            if (r->endTimer <= 0) {
                work->actionCommands.mAcState = 1004;
            }
            break;
        case 1004:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            break;
    }
    return 1;
}

s32 battleAcResult_Repeatedly(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_Repeatedly(CameraId cameraId, void* param) {
    BattleWork* work = param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1000:
        case 1001:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1002:
        case 1003:
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

void battleAcDelete_Repeatedly(BattleWork* work) {
    switch (work->actionCommands.params[0]) {
        case 10:
            work->flags &= ~BATTLE_FLAG_DISABLE_HRESET;
            break;
    }
    BattleAcGaugeSeDelete();
    work->actionCommands.mAcState = 1002;
}

static void actionCommandDisp(f32 x, f32 y) {
    RepeatWork* r;
    BattleACDispParams* d;
    s32 odd;
    s32 i;
    u16 icon1, icon2;
    s32 flags;
    f32 target;
    BattleActions* acw = &_battleWorkPointer->actionCommands;
    r = (RepeatWork*)acw->work;
    d = &acw->disp;
    camGetPtr(CAMERA_2D);
    if (acw->params[0] != 16) {
        switch (r->btn0) {
            case PAD_BUTTON_A:
                icon1 = ICON_BUTTON_A_PRESSED;
                icon2 = ICON_BUTTON_A;
                break;
            case PAD_BUTTON_B:
                icon1 = ICON_BUTTON_B_PRESSED;
                icon2 = ICON_BUTTON_B;
                break;
            case PAD_BUTTON_X:
                icon1 = ICON_BUTTON_X_PRESSED;
                icon2 = ICON_BUTTON_X;
                break;
            case PAD_BUTTON_Y:
                icon1 = ICON_BUTTON_Y_PRESSED;
                icon2 = ICON_BUTTON_Y;
                break;
            case BATTLE_PAD_STICK_LEFT:
            default:
                icon1 = ICON_STICK_LEFT;
                icon2 = ICON_STICK_NEUTRAL;
                break;
        }
        if ((d->blinkTimer / 7 & 1) == 1) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, ICON_FLAG_2D, icon2, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, ICON_FLAG_2D, icon1, 1.0f);
        }
    } else {
        odd = (d->blinkTimer / 7) & 1;
        for (i = 0; i < 2; i++) {
            BOOL match;
            u16 a, b, icon;
            if (i == 0) {
                a = ICON_BUTTON_A_PRESSED;
                b = ICON_BUTTON_A;
                if (r->btn0 == r->btn1) {
                    match = TRUE;
                } else {
                    match = FALSE;
                }
            } else {
                a = ICON_BUTTON_B_PRESSED;
                b = ICON_BUTTON_B;
                if (r->btn0 == r->btn2) {
                    match = TRUE;
                } else {
                    match = FALSE;
                }
            }
            if (match) {
                icon = a;
                if (odd == 1) {
                    icon = b;
                }
                {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (-200.0f + x) + (f32)(i * 40);
                    pos.y = 70.0f + y;
                    iconDispGx(&pos, ICON_FLAG_2D, icon, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = (-200.0f + x) + (f32)(i * 40);
                pos.y = 70.0f + y;
                iconDispGx(&pos, ICON_FLAG_2D, b, 0.5f);
            }
        }
    }
    target = r->gauge / 100.0f;
    if (d->gaugeRatio <= target) {
        d->gaugeRatio += VERSION_PICK(0.01f, 0.0125f);
        if (d->gaugeRatio > target) {
            d->gaugeRatio = target;
        }
    } else {
        d->gaugeRatio -= VERSION_PICK(0.01f, 0.0125f);
        if (d->gaugeRatio < target) {
            d->gaugeRatio = target;
        }
    }
    flags = 0;
    if (r->done == 1 && d->gaugeRatio >= 1.0f) {
        flags |= 1;
        d->color.g += d->colorStep[1];
        d->color.b += d->colorStep[2];
        if (d->color.g <= 0x80 && d->colorStep[1] < 0) {
            d->colorStep[1] = d->colorStep[2] = 0x10;
        }
        if (d->color.g >= 0xF5 && d->colorStep[1] > 0) {
            d->colorStep[1] = d->colorStep[2] = -0x10;
        }
    }
    if (r->done == -1) {
        flags |= 2;
    }
    BattleAcDrawGauge((s32)x, (s32)y, d->gaugeRatio, r->iconParam, 2, (acw->acFlags & AC_REPEATEDLY_FLAG_PLAIN_GAUGE) ? 100 : (s32)r->pos, 100, 100, flags);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = (-288.0f + x) + (176.0f * r->pos) / 100.0f;
        pos.y = 45.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_GAUGE_MARKER, 1.0f);
    }
    d->blinkTimer++;
    d->frame2++;
}

