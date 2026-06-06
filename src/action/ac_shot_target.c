#include "action/ac_shot_target.h"

extern f32 float_neg200_80427e74;
extern f32 float_0_80427e78;
extern u32 dat_80427e58;

void* memset(void* dst, int value, u32 size);

s32 battleAcResult_ShotTarget(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_ShotTarget(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1003;
}

void _ac_disp_init(void* wp) {
    void* disp = (void*)((s32)wp + 0x1F20);
    f32 y;
    f32 zero;
    s8 alpha;

    memset(disp, 0, 0x2C);
    y = float_neg200_80427e74;
    zero = float_0_80427e78;
    alpha = -0x10;
    *(f32*)((s32)disp + 0x14) = y;
    *(f32*)((s32)disp + 0x18) = zero;
    *(s32*)((s32)disp + 0x20) = 0x14;
    *(u32*)((s32)disp + 0xC) = dat_80427e58;
    *(u8*)((s32)disp + 0x10) = 0;
    *(u8*)((s32)disp + 0x11) = alpha;
    *(u8*)((s32)disp + 0x12) = alpha;
    *(u8*)((s32)disp + 0x13) = 0;
    *(f32*)((s32)disp + 0x28) = zero;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_ShotTarget(s32 unused, void* wp) {
    extern f32 float_30_80427e7c;
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern void actionCommandDisp(f32 x, f32 y);

    s32 state;
    s32 timer;

    state = *(s32*)((s32)wp + 0x1C9C);
    wp = (void*)((s32)wp + 0x1F20);
    if (state >= 1000) {
        goto ge_1000;
    }
    if (state >= 101) {
        return;
    }
    if (state >= 99) {
        goto disp_in;
    }
    return;

ge_1000:
    if (state >= 1005) {
        return;
    }
    if (state >= 1003) {
        goto disp_out;
    }
disp_in:
    timer = *(s32*)((s32)wp + 0x20);
    *(f32*)((s32)wp + 0x14) = intplGetValue(4, 0x14 - timer, float_neg200_80427e74, float_30_80427e7c, 0x14);
    actionCommandDisp(*(f32*)((s32)wp + 0x14), *(f32*)((s32)wp + 0x18));
    timer = *(s32*)((s32)wp + 0x20);
    if (timer > 0) {
        *(s32*)((s32)wp + 0x20) = timer - 1;
    }
    return;

disp_out:
    timer = *(s32*)((s32)wp + 0x20);
    if (timer >= 40) {
        *(f32*)((s32)wp + 0x14) = intplGetValue(4, timer - 40, float_30_80427e7c, float_neg200_80427e74, 0x14);
    } else {
        *(f32*)((s32)wp + 0x14) = float_30_80427e7c;
    }
    actionCommandDisp(*(f32*)((s32)wp + 0x14), *(f32*)((s32)wp + 0x18));
    timer = *(s32*)((s32)wp + 0x20);
    if (timer < 60) {
        *(s32*)((s32)wp + 0x20) = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void _ac_rumble_param_set(u32 type, s16* frame1, s16* frame2, f32* strength) {
    extern f32 float_25_80427e80;
    extern f32 float_neg1_80427e84;
    extern f32 float_0p6_80427e88;
    extern f32 float_0p25_80427e8c;
    extern f32 float_0p5_80427e90;
    extern f32 float_0p8_80427e94;
    extern f32 float_1p5_80427e98;
    extern f32 float_3_80427e9c;
    extern f32 float_2_80427e6c;
    extern s32 irand(s32 max);

    *frame1 = irand(0x78) + 0x78;
    *frame2 = *frame1 - 1;
    *strength = float_25_80427e80;
    if (irand(0x64) < 0x32) {
        *strength = float_neg1_80427e84 * *strength;
    }

    switch (type) {
        case 0:
            *strength = float_0p6_80427e88 * (float_0p25_80427e8c * *strength);
            break;
        case 1:
            *strength = float_0p6_80427e88 * (float_0p5_80427e90 * *strength);
            break;
        case 2:
            *strength = float_0p6_80427e88 * (float_0p8_80427e94 * *strength);
            break;
        case 3:
            *strength = float_0p6_80427e88 * *strength;
            break;
        case 4:
            *strength = float_0p6_80427e88 * (float_1p5_80427e98 * *strength);
            break;
        case 5:
            *strength = float_0p6_80427e88 * (float_2_80427e6c * *strength);
            break;
        case 6:
        default:
            *strength = float_0p6_80427e88 * (float_3_80427e9c * *strength);
            break;
    }
}
