#include "battle/battle_ac_help.h"

extern void* _battleWorkPointer;
void BattleAudience_WinSetActive(s32 active);

void BattleAcHelpInit(void) {
}

void BattleAcHelpSetDispType(s32 type) {
    *(s32*)((s32)_battleWorkPointer + 0x1900C) = type;
    if (type == 0) {
        BattleAudience_WinSetActive(1);
    } else {
        BattleAudience_WinSetActive(0);
    }
}

void BattleAcHelpSetHelp(s32 help) {
    *(s32*)((s32)_battleWorkPointer + 0x19008) = help;
}
