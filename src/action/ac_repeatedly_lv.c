#include "action/ac_repeatedly_lv.h"

void BattleAcGaugeSeDelete(void* wp);

s32 battleAcResult_RepeatedlyLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_RepeatedlyLv(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
    BattleAcGaugeSeDelete(wp);
}


void actionCommandDisp(f32 x, f32 y) {
}


s32 battleAcMain_RepeatedlyLv(void* battleWork) {
    return 0;
}


u8 battleAcDisp_RepeatedlyLv(s32 param_1, int param_2) {
    return 0;
}
