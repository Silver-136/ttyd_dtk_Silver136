#include "effect/eff_toiki.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effToikiEntry(s32 type, s32 arg1, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effToikiMain(void);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Toiki_80428228;
    extern f32 float_0_80428200;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_0p5_80428230;
    extern f32 float_255_80428234;
    extern f32 float_2p5_80428238;

    void* effect;
    void* work;
    f32 angle;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = &str_Toiki_80428228;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effToikiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = float_0p5_80428230;
    *(f32*)((s32)work + 0x20) = float_0_80428200;
    *(s32*)((s32)work + 0x30) = arg1;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x28) = float_255_80428234;
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x10) = float_2p5_80428238 * (f32)cos(angle);
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x14) = float_2p5_80428238 * (f32)sin(angle);
    *(f32*)((s32)work + 0x18) = float_0_80428200;
    *(f32*)((s32)work + 0x24) = float_0_80428200;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
