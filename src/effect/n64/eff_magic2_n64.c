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


void effMagic2Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effMagic2Disp(s32, s32);
    extern f32 float_0p7_80425a48;
    extern f32 float_6p2832_80425a4c;
    extern f32 float_360_80425a50;
    extern f32 float_0p4_80425a54;
    extern f32 float_0p1_80425a58;
    extern f32 float_0p3_80425a5c;
    u8* work;
    Vec3 pos;
    s32 timer;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 0x10); pos.y = *(f32*)(work + 0x14); pos.z = *(f32*)(work + 0x18);
    *(s32*)(work + 0x28) -= 1;
    *(s32*)(work + 0x2C) += 1;
    timer = *(s32*)(work + 0x28);
    if (timer < 0) { effDelete(effect); return; }
    if (*(s32*)(work + 0x2C) < 11) *(s32*)(work + 0x24) = (*(s32*)(work + 0x2C) * 0xFF) / 10;
    if (timer < 6) *(s32*)(work + 0x24) = (timer * 0xFF) / 6;
    if (timer < 10 && *(s32*)work == 0) *(s32*)(work + 0x1C) = (s32)((f32)*(s32*)(work + 0x1C) * float_0p7_80425a48);
    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        f32 wave = (f32)sin((f64)(float_6p2832_80425a4c * (f32)(timer * 10) / float_360_80425a50));
        if (*(s32*)work == 1)
            *(s32*)(work + 0x1C) = (s32)(float_0p4_80425a54 * (((f32)*(s32*)(work + 0x20) * float_0p1_80425a58 * wave + (f32)*(s32*)(work + 0x20)) - (f32)*(s32*)(work + 0x1C)) + (f32)*(s32*)(work + 0x1C));
        else
            *(s32*)(work + 0x1C) = (s32)(float_0p3_80425a5c * (((f32)*(s32*)(work + 0x20) * float_0p1_80425a58 * wave + (f32)*(s32*)(work + 0x20)) - (f32)*(s32*)(work + 0x1C)) + (f32)*(s32*)(work + 0x1C));
        *(s32*)(work + 0x58) += *(s32*)(work + 0x5C);
        *(s32*)(work + 0x50) += *(s32*)(work + 0x54);
        *(s32*)(work + 0x30) += *(s32*)(work + 0x34);
        *(s32*)(work + 0x38) += *(s32*)(work + 0x3C);
        if (*(s32*)(work + 0x30) < 0) *(s32*)(work + 0x30) += 0x100;
        if (*(s32*)(work + 0x38) < 0) *(s32*)(work + 0x38) += 0x100;
        work += 0x60;
    }
    dispEntry(4, 2, effMagic2Disp, effect, dispCalcZ(&pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

