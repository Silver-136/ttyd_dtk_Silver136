#include "action/ac_crane_timing.h"

void battleAcDelete_CraneTiming(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

void battleAcDisp_CraneTiming(void) {
}

s32 battleAcResult_CraneTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

