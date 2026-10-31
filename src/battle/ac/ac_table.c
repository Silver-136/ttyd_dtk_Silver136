// Table of all action commands: action-command ids and their handlers.

#include "battle/ac/ac_table.h"

#define AC_ENTRY(id, name) \
    { id, battleAcMain_##name, battleAcResult_##name, battleAcDisp_##name, battleAcDelete_##name }

/**
 * Every action command: its id (the type btlevtcmd_SetupAC passes to BattleActionCommandSetup,
 * which looks it up here) and its main, result, draw and delete handlers. Ends with id 0.
 */
ActionCommandEntry ActionCommandList[] = {
    AC_ENTRY(1, TimingA),
    AC_ENTRY(11, TimingWide),
    AC_ENTRY(3, StickKeepLeft),
    AC_ENTRY(4, LstRcKeep),
    AC_ENTRY(5, Repeatedly),
    AC_ENTRY(6, RepeatedlyLv),
    AC_ENTRY(7, Shot),
    AC_ENTRY(8, ShotTarget),
    AC_ENTRY(9, StickRotate),
    AC_ENTRY(10, ButtonDown),
    AC_ENTRY(12, PendulumCraneTiming),
    AC_ENTRY(13, CraneTiming),
    AC_ENTRY(14, AirGauge),
    AC_ENTRY(15, PowerGauge),
    AC_ENTRY(16, PowerGaugeLv),
    AC_ENTRY(17, PowerGaugeLv2),
    AC_ENTRY(18, SignalTiming),
    AC_ENTRY(19, GaugeTiming),
    AC_ENTRY(20, GaugeTiming2),
    AC_ENTRY(21, MonosiriTarget),
    {0, NULL, NULL, NULL, NULL},
};
