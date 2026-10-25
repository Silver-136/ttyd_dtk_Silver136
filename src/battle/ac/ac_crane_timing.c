#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "system.h"
#include <string.h>
#include "battle/battle_sub.h"


typedef struct CraneTimingParams {
    u8 field_0x0[0x3C - 0x0]; // 0x0, BattleActions header
    s32 unitId;               // 0x3C
    s32 partsId;              // 0x40
    s32 startDelay;           // 0x44
    s32 randomRange;          // 0x48
    u8 field_0x4C[0x50 - 0x4C];
    s32 maxFrames;            // 0x50
} CraneTimingParams;

typedef struct CraneTimingWork {
    s32 button;     // 0x0
    s32 frame;      // 0x4
    Vec basePos;    // 0x8
    Vec pos;        // 0x14
    f32 speed;      // 0x20
    s32 delay;      // 0x24
    f32 range;      // 0x28
    s32 endTimer;   // 0x2C
} CraneTimingWork;

#define AC_PARAMS(work) ((CraneTimingParams*)&(work)->actionCommands)

s32 battleAcMain_CraneTiming(BattleWork* work) {
    CraneTimingWork* cw;
    BattleUnit* unit;
    BOOL autoCommand;
    BattleACDispParams* disp;
    BOOL inRange;
    BattleActions* ac;

    ac = &work->actionCommands;
    autoCommand = FALSE;
    cw = (CraneTimingWork*)ac->work;
    disp = (BattleACDispParams*)&ac->disp;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = TRUE;
    }
    unit = BattleGetUnitPtr(work, AC_PARAMS(work)->unitId);
    BtlUnit_GetPartsPtr(unit, AC_PARAMS(work)->partsId);

    switch (work->actionCommands.mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            cw->frame = 0;
            cw->delay = AC_PARAMS(work)->startDelay;
            cw->range = irand(AC_PARAMS(work)->randomRange);
            cw->button = 0x100;
            work->actionCommands.outputParams[0] = 0;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = 1;
            break;
        case 100:
            unit = BattleGetUnitPtr(work, AC_PARAMS(work)->unitId);
            BtlUnit_GetPartsPtr(unit, AC_PARAMS(work)->partsId);
            BtlUnit_GetPos(unit, &cw->pos.x, &cw->pos.y, &cw->pos.z);
            cw->basePos = cw->pos;
            cw->frame = 0;
            cw->speed = 1.5f;
            work->actionCommands.mAcState = 1000;
        case 1000:
            if (cw->delay != 0) {
                cw->delay--;
            }
            cw->frame++;
            cw->pos.x += cw->speed;
            cw->pos.y = 5.0f * sinfd(reviseAngle(cw->frame * 5)) + cw->basePos.y;
            if (cw->pos.x >= cw->basePos.x + cw->range && cw->speed > 0.0f) {
                cw->speed *= -1.0f;
                cw->range = irand(AC_PARAMS(work)->randomRange);
            }
            if (cw->pos.x <= cw->basePos.x - cw->range && cw->speed < 0.0f) {
                cw->speed *= -1.0f;
                cw->range = irand(AC_PARAMS(work)->randomRange);
            }
            if (cw->speed >= 0.0f) {
                unit->faceDirection = 1;
            } else {
                unit->faceDirection = -1;
            }
            BtlUnit_SetPos(unit, cw->pos.x, cw->pos.y, cw->pos.z);
            inRange = FALSE;
            if (autoCommand && __fabs(cw->basePos.x - cw->pos.x) < 5.0) {
                inRange = TRUE;
            }
            if ((BattlePadCheckTrigger(cw->button) && cw->delay <= 0) ||
                (inRange && cw->delay <= 0)) {
                work->actionCommands.outputParams[0] = 1;
                if (__fabs(cw->basePos.x - cw->pos.x) > 20.0) {
                    work->actionCommands.result = 0;
                } else {
                    work->actionCommands.result = 2;
                    work->actionCommands.mResultCount++;
                }
                work->actionCommands.mAcState = 1002;
                break;
            }
            if (cw->frame >= AC_PARAMS(work)->maxFrames) {
                work->actionCommands.result = 0;
                work->actionCommands.mAcState = 1002;
            }
            break;
        case 1002:
            cw->endTimer = 60;
            work->actionCommands.mAcState = 1003;
        case 1003:
            cw->endTimer--;
            if (cw->endTimer > 0) {
                break;
            }
            work->actionCommands.mAcState = 1004;
            break;
        case 1004:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_CraneTiming(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_CraneTiming(CameraId cameraId, void* param) {
}

void battleAcDelete_CraneTiming(BattleWork* work) {
    work->actionCommands.mAcState = 1002;
}
