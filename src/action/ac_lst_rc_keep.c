#include "action/ac_lst_rc_keep.h"

s32 battleAcResult_LstRcKeep(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_LstRcKeep(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1005;
}

