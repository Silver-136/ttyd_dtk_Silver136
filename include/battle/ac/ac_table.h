#pragma once

#include "battle/battle.h"
#include <dolphin/types.h>

typedef struct ActionCommandEntry {
    s32 id;
    s32 (*main)(BattleWork* work);
    s32 (*result)(BattleWork* work);
    void (*disp)(CameraId camId, void* param);
    void (*delete)(BattleWork* work);
} ActionCommandEntry;

#define AC_DECLARE(name)                                   \
    s32 battleAcMain_##name(BattleWork* work);             \
    s32 battleAcResult_##name(BattleWork* work);           \
    void battleAcDisp_##name(CameraId camId, void* param); \
    void battleAcDelete_##name(BattleWork* work);

AC_DECLARE(TimingA)
AC_DECLARE(TimingWide)
AC_DECLARE(StickKeepLeft)
AC_DECLARE(LstRcKeep)
AC_DECLARE(Repeatedly)
AC_DECLARE(RepeatedlyLv)
AC_DECLARE(Shot)
AC_DECLARE(ShotTarget)
AC_DECLARE(StickRotate)
AC_DECLARE(ButtonDown)
AC_DECLARE(PendulumCraneTiming)
AC_DECLARE(CraneTiming)
AC_DECLARE(AirGauge)
AC_DECLARE(PowerGauge)
AC_DECLARE(PowerGaugeLv)
AC_DECLARE(PowerGaugeLv2)
AC_DECLARE(SignalTiming)
AC_DECLARE(GaugeTiming)
AC_DECLARE(GaugeTiming2)
AC_DECLARE(MonosiriTarget)

extern ActionCommandEntry ActionCommandList[];
