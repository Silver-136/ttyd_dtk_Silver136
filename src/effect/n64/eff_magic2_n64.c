#include "effect/n64/eff_magic2_n64.h"


u8 effMagic2Disp(int param_1, int param_2) {
    return 0;
}


u8 magic2_4_dl(void) {
    return 0;
}


u8 magic2_2_line_dl(void) {
    return 0;
}


u8 effMagic2Main(int* param_1) {
    return 0;
}


void* effMagic2N64Entry(s32 type, s32 arg, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic2Main(void*);
    extern s32 rand(void);
    extern char str_Magic2N64_802fb460[];
    extern f32 float_10_80425a68;
    extern f32 float_4_80425a6c;
    extern f32 float_0_80425a34;
    extern f32 float_0p2_80425a70;
    void* entry;
    u8* work;
    s32 r;
    s32 angle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic2N64_802fb460;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x60);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic2Main;

    *(s32*)work = type;
    *(s32*)(work + 0x28) = arg;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 0x10) = x;
    *(f32*)(work + 0x14) = y;
    *(f32*)(work + 0x18) = z;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 0x40) = 0;
    *(s32*)(work + 0x4C) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x3C) = 0;
    *(s32*)(work + 0x38) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = float_10_80425a68;

    r = rand();
    angle = r - ((r / 0x168) * 0x168);
    *(f32*)(work + 0x58) = (f32)angle;
    *(f32*)(work + 0x5C) = float_4_80425a6c;
    *(f32*)(work + 0x50) = float_0_80425a34;
    *(f32*)(work + 0x54) = float_0_80425a34;

    if (type == 1) {
        *(s32*)(work + 0x3C) = 0x60;
        *(f32*)(work + 0x20) = scale;
    } else {
        *(s32*)(work + 0x44) = -2;
        *(s32*)(work + 0x4C) = -4;
        *(f32*)(work + 0x20) = float_0p2_80425a70 * scale;
    }
    return entry;
}

