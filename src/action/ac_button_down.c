#include "action/ac_button_down.h"

extern f32 float_neg300_8042685c;
extern f32 float_0_80426860;

f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(s32 type, f32 x, f32 y);

s32 battleAcResult_ButtonDown(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_ButtonDown(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EB;
}

void battleAcDisp_ButtonDown(void* camera, void* obj) {
    s32 state;
    s32 timer;
    f32 x;
    f32 y;

    state = *(s32*)((s32)obj + 0x1C9C);
    obj = (void*)((s32)obj + 0x1F20);
    if (state == 0x63) {
        goto active;
    }
    if (state >= 0x63) {
        goto inactive;
    }
    if (state == 0) {
        goto active;
    }
    goto inactive;

active:
    timer = *(s32*)((s32)obj + 0x20);
    if (timer > 0) {
        *(s32*)((s32)obj + 0x20) = timer - 1;
    }
    x = intplGetValue(4, 0x14 - *(s32*)((s32)obj + 0x20), 0x14, float_neg300_8042685c, float_0_80426860);
    y = float_0_80426860;
    goto disp;

inactive:
    y = float_0_80426860;
    x = y;

disp:
    actionCommandDisp(0, x, y);
}

