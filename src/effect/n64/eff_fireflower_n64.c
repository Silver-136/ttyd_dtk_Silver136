#include "effect/n64/eff_fireflower_n64.h"


u8 effFireFlowerDisp(void) {
    return 0;
}


u8 effFireFlowerMain(int* param_1) {
    return 0;
}


#pragma optimize_for_size off

void* effFireFlowerN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireFlowerMain(void*);
    extern s32 rand(void);
    extern char str_FireFlowerN64_802faf18[];
    extern f32 float_0_80425254;
    extern f32 float_neg10_80425278;
    extern f32 float_0p5_8042528c;
    extern f32 float_8_80425290;
    extern f32 float_0p05_80425294;
    extern f32 float_1p5_80425298;
    void* entry;
    u8* work;
    u8* part;
    f32 zero;
    f32 eight;
    f32 point05;
    f32 divScale;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireFlowerN64_802faf18;
    *(s32*)((s32)entry + 8) = 0x1F;
    work = __memAlloc(3, 0xA2C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireFlowerMain;

    if (type == 1) {
        *(s32*)(work + 0x50) = 1;
        type = 0;
    }
    *(s32*)(work + 4) = 0;
    *(s32*)work = type;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (timer <= 0) {
        timer = 1000;
    }
    *(s32*)(work + 0x40) = timer;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x3C) = 0xFF;
    *(f32*)(work + 0x30) = float_0p5_8042528c;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x38) = 0;
    zero = float_0_80425254;
    *(f32*)(work + 0x28) = zero;
    *(f32*)(work + 0x2C) = float_neg10_80425278;
    eight = float_8_80425290;
    point05 = float_0p05_80425294;
    divScale = float_1p5_80425298;

    part = work + 0x54;
    for (i = 1; i < 0x1F; i++) {
        *(s32*)part = 1;
        *(f32*)(part + 8) = zero;
        *(f32*)(part + 0xC) = zero;
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = eight;
        *(f32*)(part + 0x18) = zero;
        *(f32*)(part + 0x1C) = (f32)(rand() % 20 - 8);
        *(f32*)(part + 0x20) = point05 * (f32)(rand() % 10 - 5);
        *(f32*)(part + 0x24) = (f32)(rand() % 80 + 5);
        *(s32*)(part + 0x3C) = 0xFF;
        *(s32*)(part + 0x4C) = (i - 1) * 2;
        *(s32*)(part + 0x44) = 0xA;
        *(f32*)(part + 0x14) /= divScale;
        *(f32*)(part + 0x18) /= divScale;
        *(f32*)(part + 0x1C) /= divScale;
        part += 0x54;
    }

    return entry;
}

#pragma optimize_for_size on

