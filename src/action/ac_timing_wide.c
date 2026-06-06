#include "action/ac_timing_wide.h"

extern f32 float_30_80426874;
extern f32 float_neg300_80426878;

f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(f32 x, f32 y);

s32 battleAcResult_TimingWide(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_TimingWide(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EC;
}

void battleAcDisp_TimingWide(void* camera, void* obj) {
    register s32 work;
    s32 state;
    s32 timer;
    s32 enabled;
    s32 flags;

    work = (s32)obj;
    work += 0x1F20;
    state = *(s32*)((s32)obj + 0x1F58);
    if (state == 3) {
        goto display_only;
    }
    if (state >= 3) {
        goto state_high;
    }
    if (state >= 0) {
        goto intro;
    }
    return;

state_high:
    if (state >= 6) {
        return;
    }
    goto outro;

intro:
    if (*(s32*)((s32)obj + 0x1CC4) & 1) {
        *(f32*)(work + 0x14) = float_30_80426874;
    } else {
        *(f32*)(work + 0x14) = intplGetValue(4, 0x14 - *(s32*)(work + 0x20), 0x14, float_neg300_80426878, float_30_80426874);
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer > 0) {
        *(s32*)(work + 0x20) = timer - 1;
    }
    return;

display_only:
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    return;

outro:
    flags = *(s32*)((s32)obj + 0x1CC4);
    enabled = 1;
    if ((flags & 2) && (*(s32*)((s32)obj + 0x1CB8) & 2)) {
        enabled = 0;
    }
    if ((flags & 4) && !(*(s32*)((s32)obj + 0x1CB8) & 2)) {
        enabled = 0;
    }
    timer = *(s32*)(work + 0x20);
    if (timer >= 0x28) {
        if (enabled) {
            *(f32*)(work + 0x14) = intplGetValue(4, timer - 0x28, 0x14, float_30_80426874, float_neg300_80426878);
        } else {
            *(f32*)(work + 0x14) = float_30_80426874;
        }
    } else {
        *(f32*)(work + 0x14) = float_30_80426874;
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer < 0x3C) {
        *(s32*)(work + 0x20) = timer + 1;
    }
}

