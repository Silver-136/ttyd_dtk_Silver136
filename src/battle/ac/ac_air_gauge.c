#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

void BattleAcDrawGauge(s32 x, s32 y, s32 icon, f32 ratio, s32 a, s32 b, s32 c, s32 d, s32 e);

typedef struct AirGaugeWork {
    s32 frame;      // 0x0, frames since input started
    f32 gauge;      // 0x4, 0..100, drained by _GetGaugeDecValue
    u32 presses;    // 0x8, A presses (1 = success when auto)
    s32 holdFrames; // 0xC, frames A held since the last press
    s32 unk10;      // 0x10
    s32 endWait;    // 0x14, frames before the result
} AirGaugeWork;

// static data from inlined header functions

typedef struct GaugeKey {
    s32 frame;
    f32 value;
    s32 flag;
} GaugeKey;

#define GAUGE_TABLE {{0, 0.0f, 1}, {20, 1.0f, 1}, {40, 1.0f, 1}, {80, 0.0f, 0}, {-1, -1.0f, 0}}
GaugeKey _table_0[5] = GAUGE_TABLE;
GaugeKey _table_1[5] = GAUGE_TABLE;
GaugeKey _table_2[5] = GAUGE_TABLE;
GaugeKey _table_3[5] = GAUGE_TABLE;
GaugeKey _table_4[5] = GAUGE_TABLE;
GaugeKey _table_5[5] = GAUGE_TABLE;
GaugeKey _table_6[5] = GAUGE_TABLE;

typedef struct AirPw {
    s32 frame;
    f32 gauge;
    s32 hit;
    u32 hold;
    s32 flag;
} AirPw;

static void actionCommandDisp(f32 x, f32 y);
f32 _GetGaugeDecValue(void);

s32 battleAcMain_AirGauge(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    AirGaugeWork* pw = (AirGaugeWork*)ac->work;
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;

    switch (ac->mAcState) {
        case 0:
            memset(disp, 0, 0x2C);
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            pw->frame = 0;
            ac->outputParams[0] = 0;
            ac->outputParams[2] = 0;
            ac->outputParams[1] = 0;
            ac->mAcState = 99;
            ac->result = 1;
            pw->gauge = 0.0f;
            pw->presses = 0;
            pw->holdFrames = 0;
            pw->unk10 = 0;
            break;
        case 99:
            break;
        case 100:
            if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & 0x10)) {
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
                ac->mAcState = 1001;
                break;
            }
            BattleAcGaugeSeInit();
            ac->mAcState = 1000;
        case 1000:
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
            if (ac->mAcUnit->badgesEquipped.AutoCommand) {
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
                if (ac->mAcUnit->badgesEquipped.AutoCommand) {
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
                ac->result = 2;
                ac->mResultCount++;
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
                ac->mAcState = 1002;
            }
            break;
        case 1001:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if ((s32)ac->outputParams[0] > 0) {
                    ac->result = 2;
                    ac->mResultCount++;
                } else {
                    ac->result = 0;
                }
                return 0;
            }
            break;
        case 1002:
            BattleAcGaugeSeDelete();
            pw->endWait = 60;
            ac->mAcState = 1003;
        case 1003:
            pw->endWait--;
            if (pw->endWait <= 0) {
                ac->mAcState = 1004;
            }
            break;
        case 1004:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

f32 _GetGaugeDecValue(void) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    AirPw* pw = (AirPw*)ac->work;
    GaugeKey* t;
    f32 base;

    switch ((u32)ac->params[7]) {
        case 0: base = 0.3f; t = _table_0; break;
        case 1: base = 0.33f; t = _table_1; break;
        case 2: base = 0.36f; t = _table_2; break;
        case 3: base = 0.4f; t = _table_3; break;
        case 4: base = 0.44f; t = _table_4; break;
        case 5: base = 0.47f; t = _table_5; break;
        case 6:
        default: t = _table_6;
        base = 0.5f;
        break;
    }
    while (pw->hold > t->frame) {
        t++;
        if (t->frame < 0) {
            break;
        }
    }
    if (t->frame <= 0) {
        pw->flag = 1;
        return base;
    }
    pw->flag = t->flag;
    base -= intplGetValue(0, t[-1].value, t->value, pw->hold - t[-1].frame, t->frame - t[-1].frame);
    return base;
}

s32 battleAcResult_AirGauge(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_AirGauge(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleACDispParams* disp = (BattleACDispParams*)&work->actionCommands.disp;
    s32 i;
    switch (work->actionCommands.mAcState) {
        case 99:
            disp->iconX = intplGetValue(4, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case 100:
            i = 4;
        case 1000:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (AC_SLIDE_TIMER_AT(disp, i) > 0) {
                (AC_SLIDE_TIMER_AT(disp, i))--;
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

void battleAcDelete_AirGauge(BattleWork* work) {
    work->actionCommands.mAcState = 1002;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    AirPw* pw;
    f32 ratio;

    camGetPtr(CAMERA_2D);
    pw = (AirPw*)ac->work;
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
    BattleAcDrawGauge((s32)x, (s32)y, 0xB2, disp->gaugeRatio, ac->params[2], ac->params[3], ac->params[4],
                      ac->params[5], 0);
    if (pw->flag) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 58.0f + y;
        iconDispGx(&pos, 0x10, ICON_065, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 58.0f + y;
        iconDispGx(&pos, 0x10, ICON_064, 1.0f);
    }
}
