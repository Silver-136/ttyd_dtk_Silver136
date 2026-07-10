#include "effect/n64/eff_thruhammer_n64.h"


u8 main_dl(int param_1, s32 param_2) {
    return 0;
}


u8 effThruHammerDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThruHammerN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThruHammerMain(void*);
    extern char str_ThruHammerN64_802fc180[];
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;

    entry = effEntry();
    if (type == 1) {
        goto count_one;
    }
    if (type >= 1) {
        goto count_one;
    }
    if (type >= 0) {
        goto count_four;
    }
    goto count_one;
count_four:
    count = 4;
    goto count_done;
count_one:
    count = 1;
count_done:
    *(char**)((s32)entry + 0x14) = str_ThruHammerN64_802fc180;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x58);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThruHammerMain;
    *(s32*)(work + 0x28) = 0x3C;
    *(s32*)(work + 0x2C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x38) = 0xFF;
    *(u8*)(work + 0x50) = 0;
    *(u8*)(work + 0x51) = 0xFF;
    *(u8*)(work + 0x52) = 0x7A;
    *(u8*)(work + 0x53) = 0xF0;
    *(u8*)(work + 0x54) = 0xFF;
    *(u8*)(work + 0x55) = 0xFA;
    part = work + 0x58;
    for (i = 1; i < count + 1; i++, part += 0x58) {
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void effThruHammerMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effThruHammerDisp(void);
    extern Vec3 vec3_802fc168[];
    extern f32 float_0_80426370;
    extern f32 float_0p8_8042637c;
    extern f32 float_6p2832_80426380;
    extern f32 float_360_80426384;
    extern f32 float_0p5_80426388;
    extern f32 float_32_8042638c;
    extern f32 float_10_80426390;
    extern f32 float_0p9_80426394;
    extern f32 float_0p2_80426398;
    extern f32 float_0p1_8042639c;
    extern f32 float_128_804263a0;
    extern double sin(double);
    extern double cos(double);
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 i;
    f32 angle;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc168;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x2C) += 1;
    *(s32*)(work + 0x28) -= 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) >= 8) {
        *(s32*)(work + 0x38) = (s32)((f32)*(s32*)(work + 0x38) * float_0p8_8042637c);
    }
    part = work + 0x58;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x58) {
        *(s32*)(part + 0x4C) += 1;
        if (*(s32*)(part + 0x4C) < 0) {
            continue;
        }
        if (*(s32*)(part + 0x4C) == 0) {
            *(f32*)(part + 4) = float_0_80426370;
            *(f32*)(part + 8) = float_0_80426370;
            *(f32*)(part + 0xC) = float_0_80426370;
            angle = (*(f32*)(part + 0x34) * float_6p2832_80426380) / float_360_80426384;
            *(f32*)(part + 0x10) = float_0p5_80426388 * -(f32)sin(angle);
            *(f32*)(part + 0x14) = float_0p5_80426388 * (f32)cos(angle);
            *(f32*)(part + 0x18) = float_0_80426370;
            *(f32*)(part + 0x3C) = float_32_8042638c;
            *(f32*)(part + 0x40) = float_32_8042638c;
            *(f32*)(part + 0x48) = float_32_8042638c;
            *(f32*)(part + 0x20) = float_10_80426390;
            *(f32*)(part + 0x1C) = float_0p5_80426388;
            *(f32*)(part + 0x24) = float_0p5_80426388;
            *(f32*)(part + 0x44) = float_32_8042638c;
        }
        *(f32*)(part + 0x3C) += *(f32*)(part + 0x40);
        *(f32*)(part + 0x44) += *(f32*)(part + 0x48);
        *(f32*)(part + 0x40) *= float_0p9_80426394;
        *(f32*)(part + 0x1C) += float_0p2_80426398;
        *(f32*)(part + 0x24) += float_0p2_80426398;
        *(f32*)(part + 0x20) += float_0p2_80426398 * (float_0p1_8042639c - *(f32*)(part + 0x20));
        if (*(f32*)(part + 0x3C) > float_128_804263a0) {
            *(f32*)(part + 0x3C) = float_128_804263a0;
        }
    }
    dispEntry(4, 2, effThruHammerDisp, effect, dispCalcZ(&dispPos));
}

