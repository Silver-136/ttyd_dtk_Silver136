#include "action/ac_shot.h"

extern f32 float_neg200_80424840;
extern f32 float_0_80424828;
extern u32 dat_80424818;

void* memset(void* dst, int value, u32 size);

s32 battleAcResult_Shot(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_Shot(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

void _ac_disp_init(void* wp) {
    void* disp = (void*)((s32)wp + 0x1F20);
    f32 y;
    f32 zero;
    s8 alpha;

    memset(disp, 0, 0x2C);

    y = float_neg200_80424840;
    zero = float_0_80424828;

    *(f32*)((s32)disp + 0x14) = y;

    alpha = -0x10;

    *(f32*)((s32)disp + 0x18) = zero;
    *(s32*)((s32)disp + 0x20) = 0x14;
    *(u32*)((s32)disp + 0xC) = dat_80424818;
    *(u8*)((s32)disp + 0x10) = 0;
    *(s8*)((s32)disp + 0x11) = alpha;
    *(s8*)((s32)disp + 0x12) = alpha;
    *(u8*)((s32)disp + 0x13) = 0;
    *(f32*)((s32)disp + 0x28) = zero;
}
