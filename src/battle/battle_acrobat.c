#include "battle/battle_acrobat.h"
#include "battle/battle_ac.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "icon_ids.h"

//local prototypes
void _accrobat_timing_icon_disp(CameraId camId, void* param);

s32 BattleAcrobatStart(BattleWork* work, s32 unitId, s32 windowStartFrame,
                        s32 windowEndFrame, s32 endFrame, s32 earlyFrames) {
    work->mStylishCurFrame = 0;
    work->mStylishWindowStart = windowStartFrame;
    work->mStylishWindowEnd = windowEndFrame;
    work->mStylishEndFrame = endFrame;
    work->mStylishUnitId = unitId;
    work->mStylishResult = 0;
    work->mStylishEarlyFrames = earlyFrames;
    return 0;
}

s32 BattleAcrobatMain(BattleWork* work) {
    BattleActions* manager;
    BattleUnit* unit;
    BOOL doSimplified;
    s32 i;

    manager = &work->actionCommands;
    unit = BattleGetUnitPtr(work, work->mStylishUnitId);
    doSimplified = FALSE;
    work->mStylishCurFrame++;
    if (!unit) {
        work->mStylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    if (unit->mTokenFlags & kConfusedAction) {
        work->mStylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    if ((work->badgeEquippedFlags & 4) && (work->mStylishCurFrame > work->mStylishWindowStart && work->mStylishCurFrame <= work->mStylishWindowEnd)) {
        Vec v11 = { 0 };
        v11.x = unit->mPosition.x;
        v11.y = unit->mPosition.y;
        v11.z = 15.0f + unit->mPosition.z;
        dispEntry(CAMERA_3D, dispCalcZ(&v11), 1, _accrobat_timing_icon_disp, unit);
    }
    if (unit->badgesEquipped.AutoCommand && (work->mStylishCurFrame > work->mStylishWindowStart && work->mStylishCurFrame <= work->mStylishWindowEnd)) {
        doSimplified = TRUE;
    }
    if (work->mStylishCurFrame <= 1 && work->mStylishEarlyFrames > 0) {
        for (i = 0; i < work->mStylishEarlyFrames; ++i) {
            if (BattlePadCheckRecordTrigger(i, 0x100)) {
                work->mStylishResult = -1;
                return EVT_RETURN_YIELD;
            }
        }
    }
    if (BattlePadCheckTrigger(0x100) || doSimplified) {
        if (work->mStylishCurFrame <= work->mStylishWindowStart) {
            work->mStylishResult = -1;
            return EVT_RETURN_YIELD;
        }
        if (work->mStylishCurFrame <= work->mStylishWindowEnd) {
            work->mStylishResult = 2;
            return EVT_RETURN_YIELD;
        }
        work->mStylishResult = -2;
        return EVT_RETURN_YIELD;
    }
    if (work->mStylishCurFrame >= work->mStylishEndFrame) {
        work->mStylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

void _accrobat_timing_icon_disp(CameraId camId, void* param) {
    BattleUnit* unit = param;
    Vec pos;

    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += BtlUnit_GetHeight(unit) + 5;
    pos.x += 15.0f;
    iconDispGx(&pos, 0, ICON_09A, 0.67f);
}

s32 BattleAcrobatGetResult(BattleWork* work, s32* result, s32* curFrame) {
    *result = work->mStylishResult;
    *curFrame = work->mStylishCurFrame;
    return EVT_RETURN_DONE;
}
