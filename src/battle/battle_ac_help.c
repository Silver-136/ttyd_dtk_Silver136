#include "battle/battle_ac_help.h"

extern void* _battleWorkPointer;
extern f32 float_498_804272b8;

void BattleAudience_WinSetActive(s32 active);
void _ac_help_disp(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);

void BattleAcHelpInit(void) {
}

void BattleAcHelpSetDispType(s32 type) {
    *(s32*)((s32)_battleWorkPointer + 0x1900C) = type;
    switch (type) {
        case 0:
            BattleAudience_WinSetActive(1);
            break;
        default:
            BattleAudience_WinSetActive(0);
            break;
    }
}

void BattleAcHelpSetHelp(s32 help) {
    *(s32*)((s32)_battleWorkPointer + 0x19008) = help;
}

void BattleAcHelpMain(void) {
    void* battleWork = _battleWorkPointer;
    void* acHelp = (void*)((s32)battleWork + 0x19008);

    if (*(u32*)acHelp != 0 && *(s32*)((s32)acHelp + 4) != 0 && !(*(u32*)((s32)battleWork + 0xEF4) & 2)) {
        dispEntry(8, 1, _ac_help_disp, 0, float_498_804272b8);
    }
}
