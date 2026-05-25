#include "action/ac_power_gauge.h"

s32 battleAcResult_PowerGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PowerGauge(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

