#include "action/ac_stick_keep_left.h"

void battleAcDelete_StickKeepLeft(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3ED;
}

s32 battleAcResult_StickKeepLeft(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}


void actionCommandDisp(s32 icon, f32 x, f32 y) {
    ;
}


s32 battleAcMain_StickKeepLeft(void* battleWork) {
    return 0;
}


void battleAcDisp_StickKeepLeft(s32 param_1, void* battleWork) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    void* data;
    void* disp;
    s32 state;
    s32 timer;

    data = (void*)((s32)battleWork + 0x1F4C);
    disp = (void*)((s32)battleWork + 0x1F20);
    state = *(s32*)((s32)battleWork + 0x1C9C);

    if (state >= 99 && state < 1001) {
        *(s32*)((s32)disp + 0x24) = -1;
        *(f32*)((s32)disp + 0x14) = intplGetValue(4, 20 - *(s32*)((s32)disp + 0x20), 20, -300.0f, 30.0f);
        actionCommandDisp(*(s32*)((s32)disp + 0x24), *(f32*)((s32)disp + 0x14), *(f32*)((s32)disp + 0x18));
        if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
            timer = *(s32*)((s32)disp + 0x20);
            if (timer > 0) {
                *(s32*)((s32)disp + 0x20) = timer - 1;
            }
        }
    } else if (state == 1001) {
        *(s32*)((s32)disp + 0x24) = *(s32*)((s32)data + 4) - 1;
        *(f32*)((s32)disp + 0x14) = intplGetValue(4, 20 - *(s32*)((s32)disp + 0x20), 20, -300.0f, 30.0f);
        actionCommandDisp(*(s32*)((s32)disp + 0x24), *(f32*)((s32)disp + 0x14), *(f32*)((s32)disp + 0x18));
        if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
            timer = *(s32*)((s32)disp + 0x20);
            if (timer > 0) {
                *(s32*)((s32)disp + 0x20) = timer - 1;
            }
        }
    } else if (state >= 1005 && state < 1007) {
        timer = *(s32*)((s32)disp + 0x20);
        if (timer >= 40) {
            *(f32*)((s32)disp + 0x14) = intplGetValue(4, timer - 40, 20, 30.0f, -300.0f);
        } else {
            *(f32*)((s32)disp + 0x14) = 30.0f;
        }
        actionCommandDisp(*(s32*)((s32)disp + 0x24), *(f32*)((s32)disp + 0x14), *(f32*)((s32)disp + 0x18));
        if (*(s32*)((s32)battleWork + 0x1CD4) == 0) {
            timer = *(s32*)((s32)disp + 0x20);
            if (timer < 60) {
                *(s32*)((s32)disp + 0x20) = timer + 1;
            }
        }
    } else if (state >= 1002 && state < 1005) {
        actionCommandDisp(*(s32*)((s32)disp + 0x24), 30.0f, 0.0f);
    }
}

