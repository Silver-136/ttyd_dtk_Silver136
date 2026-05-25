#include "action/ac_power_gauge_lv.h"

s32 battleAcResult_PowerGaugeLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_PowerGaugeLv(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

