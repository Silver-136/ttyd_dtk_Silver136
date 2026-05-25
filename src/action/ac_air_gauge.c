#include "action/ac_air_gauge.h"

void battleAcDelete_AirGauge(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

s32 battleAcResult_AirGauge(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

