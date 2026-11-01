// Work area for the timing-A action command (ac_timing_a).

#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

typedef struct TimingAWork {          // BattleActions.work for the TimingA command
    s32 pad_00; // never accessed
    u32 flags;  // 0x4, btlevtcmd_ac_timing_flag_onoff
    s32 endTimer;
} TimingAWork;

// TimingAWork.flags (btlevtcmd_ac_timing_flag_onoff)
enum {
    AC_TIMING_A_ICON_PRESSED = 1,   // draw the pressed A icon
    AC_TIMING_A_WINDOW_BY_COUNT = 2, // battleAcTimingA_GetSuccessFrame takes the window from the table by resultCount
};

// outputParams[0]
enum {
    AC_TIMING_A_RESULT_TOO_EARLY = 1, // pressed too often or too early
    AC_TIMING_A_RESULT_NO_PRESS = 2,
    AC_TIMING_A_RESULT_SUCCESS = 3,
};

s32 battleAcMain_TimingA(BattleWork* work);
s32 battleAcTimingA_GetSuccessFrame(BattleWork* work);
