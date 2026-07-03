#include "effect/n64/eff_kemuri4_n64.h"


u8 effKemuri4Main(int* param_1) {
    return 0;
}


u8 effKemuri4Disp(int param_1, int param_2) {
    return 0;
}


void* effKemuri4N64Entry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 side) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri4Main(void*);
    extern s32 rand(void);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri4N64_802fb22c[];
    extern f32 float_0_804256a8;
    extern f32 float_1_804256b0;
    extern f32 float_5p4_804256b4;
    extern f32 float_0p5_804256b8;
    extern f32 float_neg0p07_804256bc;
    extern f32 float_0p003_804256c0;
    extern f32 float_6p2832_804256c4;
    extern f32 float_360_804256c8;
    void* entry;
    u8* work;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri4N64_802fb22c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x88);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri4Main;

    *(s32*)work = 1;
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x6C) = float_0_804256a8;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x18) = float_1_804256b0;
    *(f32*)(work + 0x1C) = float_1_804256b0;
    *(f32*)(work + 0x20) = float_1_804256b0;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x68) = float_1_804256b0;
    *(f32*)(work + 0x64) = float_5p4_804256b4;
    *(f32*)(work + 0x60) = side != float_0_804256a8 ? -10.0f : 10.0f;
    *(f32*)(work + 0x70) = float_0p5_804256b8;
    *(f32*)(work + 0x74) = float_neg0p07_804256bc;
    *(f32*)(work + 0x78) = float_0p003_804256c0;
    *(f32*)(work + 0x7C) = float_0_804256a8;
    *(s16*)(work + 6) = 0xF;
    *(f32*)(work + 0x28) = angle;
    *(f32*)(work + 0x24) = float_0_804256a8;
    *(f32*)(work + 0x2C) = float_0_804256a8;
    *(u8*)(work + 9) = (u8)(((rand() % 100) >> 4) & 1);
    rad = float_6p2832_804256c4 * reviseAngle(angle + (side != float_0_804256a8 ? -90.0f : 90.0f)) / float_360_804256c8;
    *(f32*)(work + 0x80) = (f32)sin(rad);
    *(f32*)(work + 0x84) = (f32)cos(rad);

    return entry;
}

