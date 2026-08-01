#include "action/ac_table.h"

typedef void (*ActionCommandCallback)(void);

typedef struct ActionCommandEntry {
    s32 id;
    ActionCommandCallback main;
    ActionCommandCallback result;
    ActionCommandCallback display;
    ActionCommandCallback destroy;
} ActionCommandEntry;

#define DECLARE_ACTION_COMMAND(name)              \
    extern void battleAcMain_##name(void);        \
    extern void battleAcResult_##name(void);      \
    extern void battleAcDisp_##name(void);        \
    extern void battleAcDelete_##name(void)

DECLARE_ACTION_COMMAND(TimingA);
DECLARE_ACTION_COMMAND(TimingWide);
DECLARE_ACTION_COMMAND(StickKeepLeft);
DECLARE_ACTION_COMMAND(LstRcKeep);
DECLARE_ACTION_COMMAND(Repeatedly);
DECLARE_ACTION_COMMAND(RepeatedlyLv);
DECLARE_ACTION_COMMAND(Shot);
DECLARE_ACTION_COMMAND(ShotTarget);
DECLARE_ACTION_COMMAND(StickRotate);
DECLARE_ACTION_COMMAND(ButtonDown);
DECLARE_ACTION_COMMAND(PendulumCraneTiming);
DECLARE_ACTION_COMMAND(CraneTiming);
DECLARE_ACTION_COMMAND(AirGauge);
DECLARE_ACTION_COMMAND(PowerGauge);
DECLARE_ACTION_COMMAND(PowerGaugeLv);
DECLARE_ACTION_COMMAND(PowerGaugeLv2);
DECLARE_ACTION_COMMAND(SignalTiming);
DECLARE_ACTION_COMMAND(GaugeTiming);
DECLARE_ACTION_COMMAND(GaugeTiming2);
DECLARE_ACTION_COMMAND(MonosiriTarget);

#undef DECLARE_ACTION_COMMAND

#define ACTION_COMMAND(idValue, name)                 \
    {                                                 \
        idValue, battleAcMain_##name,                 \
        battleAcResult_##name, battleAcDisp_##name,   \
        battleAcDelete_##name                         \
    }

ActionCommandEntry ActionCommandList[] = {
    ACTION_COMMAND(1, TimingA),
    ACTION_COMMAND(11, TimingWide),
    ACTION_COMMAND(3, StickKeepLeft),
    ACTION_COMMAND(4, LstRcKeep),
    ACTION_COMMAND(5, Repeatedly),
    ACTION_COMMAND(6, RepeatedlyLv),
    ACTION_COMMAND(7, Shot),
    ACTION_COMMAND(8, ShotTarget),
    ACTION_COMMAND(9, StickRotate),
    ACTION_COMMAND(10, ButtonDown),
    ACTION_COMMAND(12, PendulumCraneTiming),
    ACTION_COMMAND(13, CraneTiming),
    ACTION_COMMAND(14, AirGauge),
    ACTION_COMMAND(15, PowerGauge),
    ACTION_COMMAND(16, PowerGaugeLv),
    ACTION_COMMAND(17, PowerGaugeLv2),
    ACTION_COMMAND(18, SignalTiming),
    ACTION_COMMAND(19, GaugeTiming),
    ACTION_COMMAND(20, GaugeTiming2),
    ACTION_COMMAND(21, MonosiriTarget),
    { 0 },
};

#undef ACTION_COMMAND

__declspec(section ".data") const u32 gap_05_803644EC_data = 0;
