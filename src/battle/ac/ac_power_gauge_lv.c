#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

#pragma optimize_for_size on

typedef struct PowerGaugeWork {
    s32 counter;   // 0x0
    f32 gauge;     // 0x4
    s32 field_0x8; // 0x8
    s32 limit;     // 0xC
    s32 timer;     // 0x10
} PowerGaugeWork;

#define PG_WORK(ac) ((PowerGaugeWork*)(ac)->work)

void BattleAcDrawGauge(s32 x, s32 y, s32 icon, f32 ratio, s32 a, s32 b, s32 c, s32 d, s32 e);

s32 battleAcMain_PowerGaugeLv(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* tw = PG_WORK(ac);
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.mAcState) {
        case 99:
            break;
        case 0:
            memset(disp, 0, 0x2C);
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            tw->counter = 0;
            work->actionCommands.outputParams[0] = 0;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = 1;
            tw->limit = 100;
            tw->gauge = 0.0f;
            break;
        case 100:
            if ((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    work->actionCommands.setupParam = 30;
                    work->actionCommands.outputParams[0] = 0;
                } else {
                    work->actionCommands.outputParams[0] = irand(ac->params[1] - 1) + 1;
                    work->actionCommands.setupParam = 30;
                }
                work->actionCommands.mAcState = 1001;
                break;
            }
            work->actionCommands.mAcState = 1000;
        case 1000:
            tw->counter++;
            if (tw->counter > tw->limit) {
                work->actionCommands.outputParams[0] = 0;
                work->actionCommands.result = 0;
                tw->gauge = 101.0f;
                return 0;
            }
            tw->gauge = intplGetValue(2, tw->counter, tw->limit, 0.0f, 100.0f);
            if (BattlePadGetNow() == 0x60) {
                if (tw->gauge < (f32)ac->params[2]) {
                    work->actionCommands.result = 0;
                    work->actionCommands.result &= ~2;
                } else {
                    work->actionCommands.result = 2;
                    work->actionCommands.mResultCount++;
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
                work->actionCommands.mAcState = 1002;
            }
            break;
        case 1001:
            work->actionCommands.setupParam--;
            if (work->actionCommands.setupParam < 0) {
                if ((s32)work->actionCommands.outputParams[0] > 0) {
                    work->actionCommands.result = 2;
                    work->actionCommands.mResultCount++;
                } else {
                    work->actionCommands.result = 0;
                }
                return 0;
            }
            break;
        case 1002:
            work->actionCommands.mAcState = 1003;
        case 1003:
            work->actionCommands.doneFlags |= 1;
            tw->timer = 60;
            work->actionCommands.mAcState = 1004;
        case 1004:
            tw->timer--;
            if (tw->timer <= 0) {
                work->actionCommands.mAcState = 1005;
            }
            break;
        case 1005:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_PowerGaugeLv(BattleWork* work) {
    return work->actionCommands.result;
}


static void actionCommandDisp(f32 x, f32 y);

#pragma opt_propagation off
void battleAcDisp_PowerGaugeLv(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleACDispParams* disp = (BattleACDispParams*)&work->actionCommands.disp;
    s32 i = 4;
    switch (work->actionCommands.mAcState) {
        case 99:
            disp->iconX = intplGetValue(4, 20 - AC_SLIDE_TIMER_AT(disp, i), 20, -300.0f, 30.0f);
        case 100:
        case 1000:
        case 1002:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (AC_SLIDE_TIMER_AT(disp, i) > 0) {
                (AC_SLIDE_TIMER_AT(disp, i))--;
            }
            break;
        case 1003:
        case 1004:
            if (AC_SLIDE_TIMER_AT(disp, i) >= 40) {
                disp->iconX = intplGetValue(4, AC_SLIDE_TIMER_AT(disp, i) - 40, 20, 30.0f, -300.0f);
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
void battleAcDelete_PowerGaugeLv(BattleWork* work) {
    if (work->actionCommands.doneFlags & 1) {
        return;
    }
    work->actionCommands.mAcState = 1003;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    PowerGaugeWork* tw;
    f32 ratio;
    camGetPtr(CAMERA_2D);
    tw = PG_WORK(ac);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 25.0f + y;
        iconDispGx(&pos, 0x10, ICON_08C, 1.0f);
    }
    ratio = tw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;
    BattleAcDrawGauge((s32)x, (s32)y, 0xB2, disp->gaugeRatio, work->actionCommands.params[1], work->actionCommands.params[2],
                                    work->actionCommands.params[3], work->actionCommands.params[4], 0);
    switch (work->actionCommands.mAcState) {
        case 99:
        case 100:
        case 1002:
        case 1003:
        case 1004: {
            Vec pos = {0.0f, 0.0f, 0.0f};
            x = -220.0f + x;
            y = 70.0f + y;
            pos.x = x;
            pos.y = y;
            iconDispGx(&pos, 0x10, ICON_07E, 1.0f);
            {
Vec pos2 = {0.0f, 0.0f, 0.0f};
            pos2.x = 56.0f + x;
            pos2.y = y;
            iconDispGx(&pos2, 0x10, ICON_080, 1.0f);
            }
            break;
        }
        case 1000:
            if (tw->gauge >= (f32)work->actionCommands.params[2 + 0] && tw->gauge <= 100.0f) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                    x = -220.0f + x;
                y = 70.0f + y;
                pos.x = x;
                pos.y = y;
                iconDispGx(&pos, 0x10, ICON_07F, 1.0f);
                {
Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = 56.0f + x;
                pos2.y = y;
                iconDispGx(&pos2, 0x10, ICON_081, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                    x = -220.0f + x;
                y = 70.0f + y;
                pos.x = x;
                pos.y = y;
                iconDispGx(&pos, 0x10, ICON_07E, 1.0f);
                {
Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = 56.0f + x;
                pos2.y = y;
                iconDispGx(&pos2, 0x10, ICON_080, 1.0f);
                }
            }
            break;
    }
}
