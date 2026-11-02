// Declares the acrobat timing-window functions.

#pragma once

#include "battle/battle.h"

// BattleWork.stylishResult: BattleAcrobatMain
enum {
    STYLISH_RESULT_LATE = -2,
    STYLISH_RESULT_EARLY = -1,
    STYLISH_RESULT_NONE = 0,
    STYLISH_RESULT_NO_PRESS = 1,  // or no unit
    STYLISH_RESULT_SUCCESS = 2,
};

s32 BattleAcrobatStart(BattleWork* work, s32 unitId, s32 windowStartFrame,
                        s32 windowEndFrame, s32 endFrame, s32 earlyFrames);
s32 BattleAcrobatMain(BattleWork* work);
s32 BattleAcrobatGetResult(BattleWork* work, s32* result, s32* curFrame);