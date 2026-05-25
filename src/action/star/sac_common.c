#include "action/star/sac_common.h"

extern s32 cheerEnable;
extern s32 handBeatEnable;

void BattleAcHelpSetDispType(s32 type);

s32 sac_cheer_end(void) {
    cheerEnable = 0;
    return 2;
}

s32 sac_handbeat_end(void) {
    handBeatEnable = 0;
    return 2;
}

s32 sac_ac_help_off(void) {
    BattleAcHelpSetDispType(0);
    return 2;
}

s32 sac_ac_help_on(void) {
    BattleAcHelpSetDispType(2);
    return 2;
}
