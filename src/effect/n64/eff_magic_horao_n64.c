#include "effect/n64/eff_magic_horao_n64.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_MagicHoraoN64_802fb4d0[];
extern f32 float_0_80425ae8;
extern f32 float_1_80425af8;


u8 effMagicHoraoDisp(void) {
    return 0;
}


u8 effMagicHoraoMain(int* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
void* effMagicHoraoN64Entry(s32 type, f32 x, f32 y, f32 z, s32 param4, s32 param5, f32 param6, f32 param7, f32 param8, s32 param9) {
    void* entry;
    void* work;
    s32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_MagicHoraoN64_802fb4d0;
    *(s32*)((s32)entry + 0x8) = 1;

    work = __memAlloc(3, 0x48);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagicHoraoMain;

    *(s32*)((s32)work + 0x0) = type;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x28) = param8;
    *(f32*)((s32)work + 0x14) = param6;
    *(s32*)((s32)work + 0x1C) = param4;
    *(f32*)((s32)work + 0x18) = param7;
    *(s32*)((s32)work + 0x20) = param5;
    *(f32*)((s32)work + 0x34) = float_1_80425af8;

    zero = 0;
    *(s32*)((s32)work + 0x38) = rand() % 360;
    *(s32*)((s32)work + 0x40) = param9;
    *(s32*)((s32)work + 0x44) = zero;
    *(s32*)((s32)work + 0x3C) = param4;
    *(f32*)((s32)work + 0x24) = param6;
    *(f32*)((s32)work + 0x10) = float_0_80425ae8;
    *(s32*)((s32)work + 0x2C) = zero;

    return entry;
}
#pragma no_register_save_helpers off
