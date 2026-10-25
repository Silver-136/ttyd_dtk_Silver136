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

void BattleAcDrawGauge();

typedef struct RepeatDisp {
    s32 frame;        // 0x0
    s32 frame2;       // 0x4
    u8 field_0x8[4];  // 0x8
    GXColor color;    // 0xC
    s8 delta[4];      // 0x10
    f32 unk14;        // 0x14
    f32 unk18;        // 0x18
    f32 unk1C;        // 0x1C
    s32 timer;        // 0x20
    u8 field_0x24[4]; // 0x24
    f32 gauge;        // 0x28
} RepeatDisp;

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
    u8 iconParam;     // 0x2A
    s8 done;          // 0x2B
    u8 resetFlag;     // 0x2C
    u8 field_0x2D[3]; // 0x2D
    s32 endTimer;     // 0x30
} RepeatWork;


static void actionCommandDisp(f32 x, f32 y);
void battleAcDelete_Repeatedly(BattleWork* work);


static s32 _power_table[] = {55, 50, 45, 40, 35, 30, 25};

static void _init_param(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    RepeatWork* r = (RepeatWork*)ac->work;
    RepeatDisp* d = (RepeatDisp*)&ac->disp;
    ((RepeatWork*)work->actionCommands.work)->resetFlag = 0;
    ((RepeatWork*)work->actionCommands.work)->holdMask = 0;
    switch (work->actionCommands.params[0]) {
        case 10:
            work->flags |= BATTLE_FLAG_DISABLE_HRESET;
            break;
        case 8:
            r->holdMask = 0x100;
        case 6:
            r->btn0 = r->btn1 = r->btn2 = 0x40000;
            break;
        case 2:
            r->btn0 = r->btn1 = r->btn2 = 0x200;
            break;
        case 3:
            r->btn0 = r->btn1 = r->btn2 = 0x400;
            break;
        case 4:
            r->btn0 = r->btn1 = r->btn2 = 0x800;
            break;
        case 5:
            r->btn0 = r->btn1 = r->btn2 = 0x20;
            break;
        case 11:
            r->btn0 = r->btn1 = 0x100;
            r->btn2 = 0x200;
            break;
        case 16:
            r->btn0 = r->btn1 = 0x100;
            r->btn2 = 0x200;
            r->timer = irand(30) + 60;
            break;
        case 12:
            r->btn0 = r->btn1 = 0x400;
            r->btn2 = 0x800;
            break;
        default:
            r->btn0 = r->btn1 = r->btn2 = 0x100;
            break;
    }
    r->graceStart = r->grace = work->actionCommands.params[1];
    r->pos = work->actionCommands.params[2];
    r->vel = work->actionCommands.params[3] / 10.0f;
    if (r->vel) {
        r->pos = irand(100);
    }
    r->gauge = work->actionCommands.params[7];
    d->gauge = 0.01f * r->gauge;
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
    RepeatDisp* d = (RepeatDisp*)&ac->disp;
    s32 diff = BattleActionCommandGetDifficulty(work);
    s32 pressed;
    BOOL autoCmd = FALSE;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCmd = TRUE;
    }
    pressed = 0;
    switch (work->actionCommands.mAcState) {
        case 0:
            memset(d, 0, sizeof(RepeatDisp));
            d->unk14 = -300.0f;
            d->unk18 = 0.0f;
            d->timer = 20;
            d->color = (GXColor){255, 255, 255, 255};
            d->delta[0] = 0;
            d->delta[1] = -16;
            d->delta[2] = -16;
            d->delta[3] = 0;
            d->gauge = 0.0f;
            work->actionCommands.result = 1;
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
                        if (SIProbe(0) == 8) {
                            r->resetFlag = 0;
                            pressed = 1;
                            r->count++;
                        }
                    } else {
                        if (SIProbe(0) != 8 && SIProbe(0) != 0x80) {
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
            work->actionCommands.result &= ~1;
            if (r->gauge >= r->pos || r->done) {
                work->actionCommands.result |= 2;
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

void battleAcDisp_Repeatedly(CameraId cameraId, BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;
    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1000:
        case 1001:
            disp->iconX = intplGetValue(4, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1002:
        case 1003:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(4, 30.0f, -300.0f, disp->slideTimer - 40, 20);
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
    RepeatDisp* d;
    s32 odd;
    s32 i;
    u16 icon1, icon2;
    s32 flags;
    f32 target;
    BattleActions* acw = &_battleWorkPointer->actionCommands;
    r = (RepeatWork*)acw->work;
    d = (RepeatDisp*)acw->dispData;
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
        if ((d->frame / 7 & 1) == 1) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, 0x10, icon2, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, 0x10, icon1, 1.0f);
        }
    } else {
        odd = (d->frame / 7) & 1;
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
                    iconDispGx(&pos, 0x10, icon, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = (-200.0f + x) + (f32)(i * 40);
                pos.y = 70.0f + y;
                iconDispGx(&pos, 0x10, b, 0.5f);
            }
        }
    }
    target = r->gauge / 100.0f;
    if (d->gauge <= target) {
        d->gauge += VERSION_PICK(0.01f, 0.0125f);
        if (d->gauge > target) {
            d->gauge = target;
        }
    } else {
        d->gauge -= VERSION_PICK(0.01f, 0.0125f);
        if (d->gauge < target) {
            d->gauge = target;
        }
    }
    flags = 0;
    if ((s32)(u8)r->done == 1 && d->gauge >= 1.0f) {
        flags |= 1;
        ((u8*)&d->color)[1] += d->delta[1];
        ((u8*)&d->color)[2] += d->delta[2];
        if (((u8*)&d->color)[1] <= 0x80 && d->delta[1] < 0) {
            d->delta[1] = d->delta[2] = 0x10;
        }
        if (((u8*)&d->color)[1] >= 0xF5 && d->delta[1] > 0) {
            d->delta[1] = d->delta[2] = -0x10;
        }
    }
    if (r->done == -1) {
        flags |= 2;
    }
    BattleAcDrawGauge((s32)x, (s32)y, d->gauge, r->iconParam, 2,
                      (acw->acFlags & 2) ? 100 : (s32)r->pos, 100, 100, flags);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = (-288.0f + x) + (176.0f * r->pos) / 100.0f;
        pos.y = 45.0f + y;
        iconDispGx(&pos, 0x10, ICON_096, 1.0f);
    }
    d->frame++;
    d->frame2++;
}

