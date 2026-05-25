#include "action/ac_power_gauge_lv2.h"

s32 battleAcResult_PowerGaugeLv2(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PowerGaugeLv2(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

