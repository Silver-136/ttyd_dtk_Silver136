#include "action/ac_timing_wide.h"


s32 battleAcResult_TimingWide(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_TimingWide(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EC;
}