#include "action/ac_button_down.h"

s32 battleAcResult_ButtonDown(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_ButtonDown(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EB;
}

