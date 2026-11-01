// Acrobat timing window: a short timed-press prompt and its result, used during Mario and partner attacks.

#include "version.h"
#include "battle/battle_acrobat.h"
#include "battle/battle_ac.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "icon_ids.h"

//local prototypes
void _accrobat_timing_icon_disp(CameraId camId, void* param);

/**
 * Arms a Stylish timing check for unitId (btlevtcmd_ACRStart's first call):
 * the A press counts inside frames windowStartFrame..windowEndFrame, the check gives up at endFrame,
 * and a press recorded in the earlyFrames frames before the start already counts as too early.
 * Clears the frame counter and result; always returns 0.
 */
s32 BattleAcrobatStart(BattleWork* work, s32 unitId, s32 windowStartFrame,
                        s32 windowEndFrame, s32 endFrame, s32 earlyFrames) {
    work->stylishCurFrame = 0;
    work->stylishWindowStart = windowStartFrame;
    work->stylishWindowEnd = windowEndFrame;
    work->stylishEndFrame = endFrame;
    work->stylishUnitId = unitId;
    work->stylishResult = 0;
    work->stylishEarlyFrames = earlyFrames;
    return 0;
}

/**
 * Runs one frame of a Stylish move check for work->stylishUnitId.
 * Sets work->stylishResult: 2 = A pressed inside the window,
 * -1 = pressed too early (or before the move started), -2 = too late, 1 = window passed or the unit is confused.
 * Timing Tutor draws the "!" icon during the window and Simplifier-style AutoCommand presses for the player.
 * Returns EVT_RETURN_BLOCK while still waiting, EVT_RETURN_YIELD once decided.
 */
s32 BattleAcrobatMain(BattleWork* work) {
    BattleUnit* unit;
    BOOL doSimplified;
    s32 i;

    unit = BattleGetUnitPtr(work, work->stylishUnitId);
    doSimplified = FALSE;
    work->stylishCurFrame++;
    if (!unit) {
        work->stylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    if (unit->tokenFlags & kConfusedAction) {
        work->stylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    if ((work->badgeEquippedFlags & BATTLE_BADGE_TIMING_TUTOR) && (work->stylishCurFrame > work->stylishWindowStart && work->stylishCurFrame <= work->stylishWindowEnd)) {
#if VERSION <= VERSION_DEMO_US
dispEntry(CAMERA_3D, 900.0f, 1, _accrobat_timing_icon_disp, unit);
#else
        Vec pos = { 0 };
        pos.x = unit->position.x;
        pos.y = unit->position.y;
        pos.z = 15.0f + unit->position.z;
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 1, _accrobat_timing_icon_disp, unit);
#endif
    }
    if (unit->badgesEquipped.AutoCommand && (work->stylishCurFrame > work->stylishWindowStart && work->stylishCurFrame <= work->stylishWindowEnd)) {
        doSimplified = TRUE;
    }
    if (work->stylishCurFrame <= 1 && work->stylishEarlyFrames > 0) {
        for (i = 0; i < work->stylishEarlyFrames; ++i) {
            if (BattlePadCheckRecordTrigger(i, PAD_BUTTON_A)) {
                work->stylishResult = -1;
                return EVT_RETURN_YIELD;
            }
        }
    }
    if (BattlePadCheckTrigger(PAD_BUTTON_A) || doSimplified) {
        if (work->stylishCurFrame <= work->stylishWindowStart) {
            work->stylishResult = -1;
            return EVT_RETURN_YIELD;
        }
        if (work->stylishCurFrame <= work->stylishWindowEnd) {
            work->stylishResult = 2;
            return EVT_RETURN_YIELD;
        }
        work->stylishResult = -2;
        return EVT_RETURN_YIELD;
    }
    if (work->stylishCurFrame >= work->stylishEndFrame) {
        work->stylishResult = 1;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/** Draws the Timing Tutor "!" icon just above and right of the unit's head. */
void _accrobat_timing_icon_disp(CameraId camId, void* param) {
    BattleUnit* unit = param;
    Vec pos;

    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += BtlUnit_GetHeight(unit) + 5;
    pos.x += 15.0f;
    iconDispGx(&pos, 0, ICON_STYLISH_TIMING, 0.67f);
}

/** Copies the last Stylish check result and the frame it was decided on to the script helper's outputs. */
s32 BattleAcrobatGetResult(BattleWork* work, s32* result, s32* curFrame) {
    *result = work->stylishResult;
    *curFrame = work->stylishCurFrame;
    return EVT_RETURN_DONE;
}
