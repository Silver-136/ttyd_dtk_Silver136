#include "effect/n64/eff_kemuri3_n64.h"


u8 effKemuri3Disp(int param_1, int param_2) {
    return 0;
}


u8 effKemuri3Main(int* param_1) {
    return 0;
}


void* effKemuri3N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri3Main(void*);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri3N64_802fb20c[];
    extern f32 float_0p3_80425684;
    extern f32 float_0_80425668;
    extern f32 float_1_8042566c;
    extern f32 float_1p75_80425688;
    extern f32 float_neg0p08_8042568c;
    extern f32 float_neg3p9_80425690;
    extern f32 float_6p2832_80425694;
    extern f32 float_360_80425698;
    void* entry;
    u8* work;
    s32 i;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri3N64_802fb20c;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0x2A8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri3Main;

    for (i = 0; i < 5; i++) {
        *(s32*)work = 1;
        *(f32*)(work + 0x64) = float_0p3_80425684;
        *(f32*)(work + 0x68) = float_0_80425668;
        *(f32*)(work + 0x6C) = float_0_80425668;
        *(f32*)(work + 0xC) = x;
        *(f32*)(work + 0x10) = y;
        *(f32*)(work + 0x14) = z;
        *(f32*)(work + 0x18) = float_1_8042566c;
        *(f32*)(work + 0x1C) = float_1_8042566c;
        *(f32*)(work + 0x20) = float_1_8042566c;
        *(u8*)(work + 8) = 0xFF;
        *(s16*)(work + 6) = 0x3C;
        *(f32*)(work + 0x24) = float_0_80425668;
        *(f32*)(work + 0x28) = angle + (f32)(i * 0x48);
        *(f32*)(work + 0x2C) = float_0_80425668;
        *(f32*)(work + 0x70) = float_1p75_80425688;
        *(f32*)(work + 0x74) = float_neg0p08_8042568c;
        *(f32*)(work + 0x78) = float_0_80425668;
        *(f32*)(work + 0x7C) = float_0_80425668;
        *(f32*)(work + 0x60) = float_neg3p9_80425690;
        rad = float_6p2832_80425694 * reviseAngle(*(f32*)(work + 0x28)) / float_360_80425698;
        *(f32*)(work + 0x80) = (f32)sin(rad);
        *(f32*)(work + 0x84) = (f32)cos(rad);
        work += 0x88;
    }

    return entry;
}

