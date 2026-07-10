#include "effect/n64/eff_rothammer_success_n64.h"


u8 effRotHammerSuccessDisp(void) {
    return 0;
}


void* effRotHammerSuccessN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, s32 lifetime) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRotHammerSuccessMain(void*);
    extern char str_RotHammerSuccessN64_802fbeac[];
    extern f32 float_0_80425f94;
    extern f32 float_6p2832_80425f98;
    extern f32 float_360_80425f9c;
    extern f32 float_8_80425fa0;
    extern f32 float_20_80425fa4;
    extern f32 float_1_80425fa8;
    extern double sin(double);
    extern double cos(double);
    void* entry;
    u8* work;
    u8* part;
    f32 zero;
    f32 twoPi;
    f32 fullCircle;
    f32 speed;
    f32 twenty;
    f32 one;
    f32 angle;
    s32 i;
    s32 limit;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_RotHammerSuccessN64_802fbeac;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x34);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRotHammerSuccessMain;
    zero = float_0_80425f94;
    twoPi = float_6p2832_80425f98;
    *(s32*)(work + 0x24) = lifetime;
    part = work + 0x34;
    fullCircle = float_360_80425f9c;
    limit = count + 1;
    *(s32*)(work + 0x28) = 0;
    i = 1;
    *(s32*)(work + 0x2C) = lifetime;
    speed = float_8_80425fa0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    twenty = float_20_80425fa4;
    *(f32*)(work + 8) = y;
    one = float_1_80425fa8;
    *(f32*)(work + 0xC) = z;
    for (; i < limit; i++, part += 0x34) {
        *(f32*)(part + 4) = zero;
        *(f32*)(part + 8) = zero;
        *(f32*)(part + 0xC) = zero;
        angle = ((f32)(((i - 1) * 360) / count) * twoPi) / fullCircle;
        *(f32*)(part + 0x10) = speed * (f32)sin(angle);
        *(f32*)(part + 0x14) = speed * (f32)cos(angle);
        *(f32*)(part + 0x18) = zero;
        *(f32*)(part + 0x1C) = zero;
        *(f32*)(part + 0x20) = twenty;
        *(f32*)(part + 0x30) = one;
    }
    return entry;
}

void effRotHammerSuccessMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRotHammerSuccessDisp(void);
    extern const Vec3 vec3_802fbea0;
    extern f32 float_0_80425f94;
    extern f32 float_0p8_80425f88;
    extern f32 float_0p7_80425f8c;
    extern f32 float_0p2_80425f90;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    f32 zero;
    f32 dampFast;
    f32 dampSlow;
    f32 fadeStep;
    s32 timer;
    s32 threshold;
    s32 fade;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbea0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    timer = *(s32*)(work + 0x24);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    threshold = *(s32*)(work + 0x2C);
    fade = timer << 1;
    zero = float_0_80425f94;
    dampFast = float_0p8_80425f88;
    dampSlow = float_0p7_80425f8c;
    fadeStep = float_0p2_80425f90;
    i = 1;
    part = work + 0x34;
    for (; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        if (timer > 5) {
            *(f32*)(part + 0x10) *= dampFast;
            *(f32*)(part + 0x14) *= dampFast;
            *(f32*)(part + 0x18) *= dampFast;
        }
        if (timer < 5) {
            *(f32*)(part + 0x20) *= dampFast;
            *(f32*)(part + 0x10) *= dampSlow;
            *(f32*)(part + 0x14) *= dampSlow;
            *(f32*)(part + 0x18) *= dampSlow;
        }
        if (fade < threshold) {
            *(f32*)(part + 0x30) += fadeStep * -*(f32*)(part + 0x30);
        }
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) = zero;
        *(f32*)(part + 0x1C) += *(f32*)(part + 0x20);
    }
    dispEntry(4, 2, effRotHammerSuccessDisp, effect, dispCalcZ(&dispPos));
}

