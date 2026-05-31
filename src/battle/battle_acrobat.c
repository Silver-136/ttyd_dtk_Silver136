#include "battle/battle_acrobat.h"


s32 BattleAcrobatMain(void* battleWork) {
    return 0;
}


void _accrobat_timing_icon_disp(s32 param_1, void* unit) {
}


s32 BattleAcrobatStart(void* battleWork, s32 unitId, s32 a, s32 b, s32 c, s32 d) {
    *(s32*)((s32)battleWork + 0x271C) = 0;
    *(s32*)((s32)battleWork + 0x2720) = a;
    *(s32*)((s32)battleWork + 0x2724) = b;
    *(s32*)((s32)battleWork + 0x2728) = c;
    *(s32*)((s32)battleWork + 0x272C) = unitId;
    *(s32*)((s32)battleWork + 0x2730) = 0;
    *(s32*)((s32)battleWork + 0x2734) = d;
    return 0;
}


s32 BattleAcrobatGetResult(void* battleWork, s32* result1, s32* result2) {
    *result1 = *(s32*)((s32)battleWork + 0x2730);
    *result2 = *(s32*)((s32)battleWork + 0x271C);
    return 2;
}
