#include "action/ac_stick_rotate.h"

s32 battleAcResult_StickRotate(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_StickRotate(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EC;
}

s32 _get_icon_id(s8 id) {
    switch (id) {
        case 0:
            return 0x84;
        case 1:
            return 0x85;
        case 2:
            return 0x82;
        case 3:
            return 0x7F;
        case 4:
            return 0x7E;
        case 5:
            return 0x7D;
        case 6:
            return 0x80;
        case 7:
            return 0x83;
        default:
            return 0x81;
    }
}

