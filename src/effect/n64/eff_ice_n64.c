#include "effect/n64/eff_ice_n64.h"


void effIceMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 effTblRandN64(s32, s32);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effIceDisp(s32, s32);
    extern Vec3 vec3_802fb150[];
    extern f32 float_0p3_80425524;
    extern f32 float_0p8_80425528;
    extern f32 float_6p2832_8042552c;
    extern f32 float_2_80425530;
    extern f32 float_360_80425534;
    extern f32 float_neg0p05_80425538;
    extern f32 float_0p2_8042553c;
    extern f32 float_0p92_80425540;
    extern f32 float_10_80425544;
    extern f32 float_0p5_80425548;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;
    s32 time;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb150;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x28) -= 1;
    *(s32*)(work + 0x2C) += 1;
    time = *(s32*)(work + 0x28);
    if (time < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) < 10) {
        *(s32*)(work + 0x24) = (s32)(float_0p3_80425524 * (255 - *(s32*)(work + 0x24)) + *(s32*)(work + 0x24));
    }
    if (time < 10) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p8_80425528);
    }

    part = work + 0x30;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        *(f32*)(part + 0x14) += float_neg0p05_80425538;
        *(f32*)(part + 0x10) += float_0p2_8042553c * (f32)sin((f64)((float_6p2832_8042552c * float_2_80425530 * *(f32*)(part + 0x18)) / float_360_80425534));
        *(f32*)(part + 0x14) += float_neg0p05_80425538;
        *(f32*)(part + 0x10) *= float_0p92_80425540;
        *(f32*)(part + 0x1C) += (f32)(effTblRandN64(50, time + i * 0x14) - 25);
        *(f32*)(part + 0x18) += float_10_80425544 * (f32)sin((f64)((float_6p2832_8042552c * *(f32*)(part + 0x1C)) / float_360_80425534));
        *(f32*)(part + 0x20) += float_10_80425544 * (f32)cos((f64)((float_6p2832_8042552c * *(f32*)(part + 0x1C) * float_0p5_80425548) / float_360_80425534));
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        part += 0x30;
    }
    dispEntry(4, 2, effIceDisp, effect, dispCalcZ(&dispPos));
}

u8 effIceDisp(int param_1, int param_2) {
    return 0;
}


#pragma optimize_for_size off

#pragma optimize_for_size off

void* effIceN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effIceMain(void*);
    extern s32 rand(void);
    extern char str_IceN64_802fb160[];
    extern f32 float_10_80425544;
    extern f32 float_0_8042551c;
    extern f32 float_2_80425530;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    f32 ten;
    f32 zero;
    f32 two;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_IceN64_802fb160;
    *(s32*)((s32)entry + 8) = 9;
    work = __memAlloc(3, 0x1B0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effIceMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 0x1E;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    ten = float_10_80425544;
    zero = float_0_8042551c;
    two = float_2_80425530;

    part = work + 0x30;
    for (i = 1; i < 9; i++) {
        *(f32*)(part + 4) = (f32)(rand() % 10 - 5);
        *(f32*)(part + 8) = (f32)(rand() % 10 + 5);
        *(f32*)(part + 0xC) = ten;
        *(f32*)(part + 0x18) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x20) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x1C) = (f32)((rand() % 100) / 10);
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = two;
        part += 0x30;
    }

    return entry;
}

#pragma optimize_for_size on

#pragma optimize_for_size on

