#include "action/ac_repeatedly.h"

void BattleAcGaugeSeDelete(void* wp);

s32 battleAcResult_Repeatedly(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_Repeatedly(void* wp) {
    if (*(s32*)((s32)wp + 0x1CC8) == 10) {
        *(u32*)((s32)wp + 0xEF4) &= ~0x20000000;
    }
    BattleAcGaugeSeDelete(wp);
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}


s32 battleAcMain_Repeatedly(void* battleWork) {
    return 0;
}


void actionCommandDisp(f32 x, f32 y) {
}


void battleAcDisp_Repeatedly(s32 param_1, void* battleWork) {
}
