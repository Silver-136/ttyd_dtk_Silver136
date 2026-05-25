#include "action/ac_stick_keep_left.h"

void battleAcDelete_StickKeepLeft(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3ED;
}

s32 battleAcResult_StickKeepLeft(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

