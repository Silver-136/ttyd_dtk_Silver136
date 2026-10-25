#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

typedef struct TimingAWork {          // BattleActions.work for the TimingA command
    s32 pad_00; // never accessed
    u32 flags;  // 0x4, btlevtcmd_ac_timing_flag_onoff
    s32 endTimer;
} TimingAWork;

s32 battleAcMain_TimingA(BattleWork* work);
s32 battleAcTimingA_GetSuccessFrame(BattleWork* work);
