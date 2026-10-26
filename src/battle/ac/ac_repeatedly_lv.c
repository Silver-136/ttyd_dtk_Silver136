#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>

s32 BattleACGetButtonIcon(s32 button, s32 pressed);
void BattleAcDrawGauge(f32 anim, s32 x, s32 y, u8 a, s32 n, s32 v0, s32 v1, s32 v2, s32 flags);

typedef struct RepeatedlyLvWork {
    u32 mask;    // 0x0
    u32 maskA;   // 0x4
    u32 maskB;   // 0x8
    u32 maxMask; // 0xC
    f32 rate;    // 0x10
    f32 gauge;   // 0x14
    s16 s18;     // 0x18
    s16 s1a;     // 0x1A
    s16 s1c;     // 0x1C
    s16 s1e;     // 0x1E
    s16 s20;     // 0x20
    s16 s22;     // 0x22
    u8 u24;      // 0x24
    s8 state;    // 0x25
    u8 u26;      // 0x26
    u8 u27;      // 0x27
    u8 pad[8];
    s32 endTimer; // 0x30
} RepeatedlyLvWork;

#define RL_IconDisp ((void (*)(Vec*, s16, s32, f32))iconDispGx)

static void _init_param(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    RepeatedlyLvWork* p = (RepeatedlyLvWork*)ac->work;
    s32 v;
    p->maxMask = 0;
    p->u27 = 1;
    switch (ac->params[0]) {
        case 2:
            p->mask = p->maskA = p->maskB = 0x200;
            break;
        case 3:
            p->mask = p->maskA = p->maskB = 0x400;
            break;
        case 4:
            p->mask = p->maskA = p->maskB = 0x800;
            break;
        case 5:
            p->mask = p->maskA = p->maskB = 0x20;
            break;
        case 8:
            p->maxMask = 0x100;
        case 7:
            p->u27 = 0;
        case 6:
            p->mask = p->maskA = p->maskB = 0x40000;
            break;
        case 11:
            p->mask = p->maskA = 0x100;
            p->maskB = 0x200;
            break;
        case 12:
            p->mask = p->maskA = 0x400;
            p->maskB = 0x800;
            break;
        case 13:
            p->mask = p->maskA = 0x10000;
            p->maskB = 0x20000;
            break;
        case 14:
            p->mask = p->maskA = 0x40000;
            p->maskB = 0x80000;
            break;
        case 15:
            p->mask = p->maskA = 0x40;
            p->maskB = 0x20;
            break;
        default:
            p->mask = p->maskA = p->maskB = 0x100;
            break;
    }
    p->s22 = 0;
    p->s1e = p->s20 = ac->params[1];
    p->s18 = 0;
    p->gauge = 0.0f;
    p->s1c = 0;
    p->u24 = ac->params[2];
    p->s1a = ac->params[3];
    p->rate = 0.01f * (f32)ac->params[4];
    p->u26 = ac->params[5];
    v = work->actionCommands.countdown;
    if (v > 0) {
        work->actionCommands.countdown = v - 1;
        p->state = 1;
    } else {
        p->state = 0;
    }
    ac->outputParams[2] = 0;
    ac->outputParams[0] = 0;
    ac->outputParams[1] = 0;
}

void battleAcDelete_RepeatedlyLv(BattleWork* work);

s32 battleAcMain_RepeatedlyLv(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    RepeatedlyLvWork* p = (RepeatedlyLvWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;
    s32 hit = 0;
    s32 autoCmd = 0;
    s32 r;
    s32 g;

    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        autoCmd = 1;
    }
    switch (ac->mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->color = (GXColor){255, 255, 255, 255};
            disp->colorStep[0] = 0;
            disp->colorStep[1] = -16;
            disp->colorStep[2] = -16;
            disp->colorStep[3] = 0;
            disp->gaugeRatio = 0.0f;
            ac->result = 1;
            _init_param(work);
            ac->mAcState = 99;
            break;
        case 99:
            break;
        case 100:
            if (!((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction))) {
                if (p->maxMask != 0 && !BattlePadCheckNow(p->maxMask) && !autoCmd) {
                    p->s22++;
                    if (p->s22 < 150) {
                        break;
                    }
                    ac->outputParams[2] = 0;
                    ac->result &= ~1;
                    ac->result |= 2;
                    ac->mAcState = 1002;
                    break;
                }
            }
            BattleAcGaugeSeInit();
            ac->outputParams[0] = 1;
            ac->mAcState = 1000;
        case 1000:
            if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                if (ac->setupFlags & 2) {
                    ac->setupParam--;
                    if (ac->setupParam >= 0) {
                        break;
                    }
                }
                ac->result &= ~1;
                ac->result |= 2;
                r = irand(100);
                p->s18 = 0;
                while (r < 0) {
                    r -= p->u26;
                    if (r < 0) {
                        break;
                    }
                    p->s18++;
                }
                ac->outputParams[2] = p->s18;
                ac->mResultCount++;
                battleAcDelete_RepeatedlyLv(work);
                return 0;
            }
            switch (ac->params[0]) {
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    if (p->s1c & 1) {
                        p->mask = p->maskB;
                    } else {
                        p->mask = p->maskA;
                    }
                    break;
            }
            if (p->u27) {
                if (ac->acFlags & 0x20) {
                    if (BattlePadCheckNow(p->mask)) {
                        hit = 1;
                        p->s1c++;
                    }
                } else {
                    if (BattlePadCheckTrigger(p->mask)) {
                        hit = 1;
                        p->s1c++;
                    }
                }
            } else {
                if (BattlePadCheckUp(p->mask)) {
                    hit = 1;
                    p->s1c++;
                }
            }
            if (autoCmd && !hit) {
                hit = 1;
                p->s1c++;
            }
            if (p->state == 0) {
                if (hit) {
                    p->gauge = 0.4f * (f32)p->s1a + p->gauge;
                }
                if (p->gauge >= 100.0f) {
                    p->gauge = 100.0f;
                    p->state = 1;
                    if (ac->acFlags & 0x20) {
                        p->state = -1;
                    }
                } else {
                    p->gauge = p->gauge - p->rate;
                    if (p->gauge <= 0.0f) {
                        p->gauge = 0.0f;
                    }
                }
            } else if (p->state == -1) {
                p->gauge = p->gauge - 1.5f * p->rate;
                if (p->gauge <= 0.0f) {
                    p->gauge = 0.0f;
                    p->state = 0;
                }
            }
            ac->outputParams[1] = (s32)p->gauge;
            g = (s32)p->gauge;
            if (p->u26 == 0) {
                p->u26 = 100;
            }
            p->s18 = 0;
            while (g > 0) {
                g -= p->u26;
                p->s18++;
                if (g <= 0) {
                    break;
                }
            }
            if (p->s18 == 0 && (ac->acFlags & 0x40)) {
                p->s18 = 1;
            }
            if (p->state && !(ac->acFlags & 1)) {
                p->s18++;
            }
            ac->outputParams[2] = p->s18;
            if (p->maxMask != 0 && !BattlePadCheckNow(p->maxMask)) {
                p->s20 = 0;
            }
            BattleAcGaugeSeUpdate(p->gauge);
            if (p->s20 > 0) {
                p->s20--;
                break;
            }
            ac->result &= ~1;
            if (p->gauge >= (f32)ac->params[6]) {
                ac->result |= 2;
            } else {
                ac->result &= ~2;
            }
            battleAcDelete_RepeatedlyLv(work);
            ac->mAcState = 1002;
            break;
        case 1002:
            p->endTimer = 60;
            ac->mAcState = 1003;
            break;
        case 1003:
            p->endTimer--;
            if (p->endTimer <= 0) {
                ac->mAcState = 1004;
            }
            break;
        case 1004:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            break;
    }
    return 1;
}

s32 battleAcResult_RepeatedlyLv(BattleWork* work) {
    return work->actionCommands.result;
}

static void actionCommandDisp(f32 x, f32 y);

void battleAcDisp_RepeatedlyLv(CameraId cameraId, BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;
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

void battleAcDelete_RepeatedlyLv(BattleWork* work) {
    work->actionCommands.mAcState = 1002;
    BattleAcGaugeSeDelete();
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    RepeatedlyLvWork* p = (RepeatedlyLvWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;
    s32 arr[4];
    s32 i, count, flags, phase;
    f32 target;
    camGetPtr(CAMERA_2D);
    {
    s32 numIcons;
    s32 pressedA, idleA, pressedB, idleB;
    s32 off0, off1;
    idleB = 0;
    pressedB = 0;
    numIcons = 1;
    ac->disp.blinkTimer++;
    off1 = 0;
    off0 = 0;
    pressedA = BattleACGetButtonIcon(p->mask, 1);
    idleA = BattleACGetButtonIcon(p->mask, 0);
    switch (ac->params[0]) {
        case 15:
            off0 = -10;
            off1 = 10;
        case 11:
        case 12:
            pressedA = BattleACGetButtonIcon(p->maskA, 1);
            idleA = BattleACGetButtonIcon(p->maskA, 0);
            pressedB = BattleACGetButtonIcon(p->maskB, 1);
            idleB = BattleACGetButtonIcon(p->maskB, 0);
            numIcons = 2;
            break;
        case 13:
            pressedA = ICON_STICK_UP;
            idleA = ICON_STICK_DOWN;
            break;
        case 14:
            pressedA = ICON_STICK_LEFT;
            idleA = ICON_STICK_RIGHT;
            break;
    }
    phase = ac->disp.blinkTimer / 7;
    if (numIcons == 1) {
        if ((phase & 1) == 1) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            RL_IconDisp(&pos, 0x10, idleA, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            RL_IconDisp(&pos, 0x10, pressedA, 1.0f);
        }
    } else {
        if ((phase & 1) == 1) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -225.0f + x + off0;
            pos.y = 70.0f + y;
            RL_IconDisp(&pos, 0x10, idleA, 1.0f);
            {
                Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = -175.0f + x + off1;
                pos2.y = 70.0f + y;
                RL_IconDisp(&pos2, 0x10, pressedB, 1.0f);
            }
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -225.0f + x + off0;
            pos.y = 70.0f + y;
            RL_IconDisp(&pos, 0x10, pressedA, 1.0f);
            {
                Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = -175.0f + x + off1;
                pos2.y = 70.0f + y;
                RL_IconDisp(&pos2, 0x10, idleB, 1.0f);
            }
        }
    }
    }
    target = p->gauge / 100.0f;
    if (disp->gaugeRatio <= target) {
        if (ac->acFlags & 8) {
            disp->gaugeRatio += 0.02f;
        } else {
            disp->gaugeRatio += 0.01f;
        }
        if (ac->acFlags & 0x10) {
            disp->gaugeRatio = target;
        }
        if (disp->gaugeRatio > target) {
            disp->gaugeRatio = target;
        }
    } else {
        if (ac->acFlags & 8) {
            disp->gaugeRatio -= 0.02f;
        } else {
            disp->gaugeRatio -= 0.01f;
        }
        if (ac->acFlags & 0x10) {
            disp->gaugeRatio = target;
        }
        if (disp->gaugeRatio < target) {
            disp->gaugeRatio = target;
        }
    }
    count = 0;
    flags = 0;
    if (p->state == 1 && disp->gaugeRatio >= 1.0f) {
        flags |= 1;
        disp->color.g += disp->colorStep[1];
        disp->color.b += disp->colorStep[2];
        if (disp->color.g <= 0x80 && disp->colorStep[1] < 0) {
            disp->colorStep[1] = disp->colorStep[2] = 0x10;
        }
        if (disp->color.g >= 0xF5 && disp->colorStep[1] > 0) {
            disp->colorStep[1] = disp->colorStep[2] = -0x10;
        }
    }
    if ((s8)p->state == -1) {
        flags |= 2;
    }
    for (i = 0; i < 4; i++) {
        arr[i] = 100;
    }
    for (i = 0; i < 4; i++, count++) {
        arr[i] = ac->gaugeParams[i];
        if (i != 0 && arr[i] - arr[i - 1] <= 0) {
            break;
        }
    }
    BattleAcDrawGauge(disp->gaugeRatio, (s32)x, (s32)y, p->u24, count, arr[0], arr[1], arr[2], flags);
    if (ac->acFlags & 2) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -288.0f + x + (f32)(ac->params[6] * 0xB0) / 100.0f;
        pos.y = 45.0f + y;
        RL_IconDisp(&pos, 0x10, ICON_096, 1.0f);
    }
    if (ac->acFlags & 4) {
        Mtx m3, m1, m2;
        s32 number = (s32)ac->outputParams[2] + ac->params[7];
        { f32 w = 108.0f; PSMTXTrans(m1, -200.0f + x + w, 25.0f + y, 0.0f); }
        PSMTXScale(m2, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(m1, m2, m3);
        iconNumberDispGx(m3, number, FALSE, (GXColor){255, 255, 255, 255});
    }
}
