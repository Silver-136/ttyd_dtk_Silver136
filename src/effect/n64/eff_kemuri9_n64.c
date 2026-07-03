#include "effect/n64/eff_kemuri9_n64.h"


u8 effKemuri9Disp(void) {
    return 0;
}


void effKemuri9Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri9Disp(void);
    extern const Vec3 vec3_802fb2c0;
    extern f32 float_16_80425804;
    extern f32 float_0p1_80425810;
    extern f32 float_0p04_80425814;
    extern f32 float_0p9_80425818;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;
    s32 life;
    s32 nextFrame;
    s32 fadeIn;
    s32 fadeOut;
    s32 countInt;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb2c0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    frame = *(s32*)(work + 0x38);
    life = *(s32*)(work + 0x3C);
    nextFrame = frame + 1;
    fadeOut = (s32)((float_16_80425804 * (f32)frame) / (f32)life);
    fadeIn = (s32)((float_16_80425804 * (f32)nextFrame) / (f32)life);
    *(s32*)(work + 0x38) = nextFrame;
    if (fadeOut > 0xF) {
        fadeOut = 0xF;
    }
    if (fadeIn > 0xF) {
        fadeIn = 0xF;
    }
    *(s32*)(work + 0x30) = fadeIn;
    *(s32*)(work + 0x34) = fadeOut;
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return;
    }

    *(f32*)(work + 0x1C) += float_0p1_80425810 * (*(f32*)(work + 0x20) - *(f32*)(work + 0x1C));
    countInt = (s32)*(f32*)(work + 0x1C);
    part = work + 0x40;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x40) {
        *(f32*)(part + 0x14) += float_0p04_80425814;
        *(f32*)(part + 0x14) *= float_0p9_80425818;
        *(f32*)(part + 4) = (f32)countInt * *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) = (f32)countInt * *(f32*)(part + 0x18);
    }

    dispEntry(4, 2, effKemuri9Disp, effect, dispCalcZ(&dispPos));
}

void* effKemuri9N64Entry(s32 type, f32 x, f32 y, f32 z, s32 count, s32 life, f32 targetRadius, f32 angleOffset) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri9Main(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri9N64_802fb2d8[];
    extern f32 float_10_8042581c;
    extern f32 float_0_80425800;
    extern f32 float_6p2832_80425820;
    extern f32 float_360_80425824;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 angleDeg;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri9N64_802fb2d8;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x40);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri9Main;
    *(s32*)(work + 0x24) = life;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x2C) = life;
    *(s32*)(work + 0x30) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = float_10_8042581c;
    *(f32*)(work + 0x20) = targetRadius;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x3C) = life;
    *(s32*)(work + 0x38) = 0;

    part = work + 0x40;
    for (i = 1; i < count + 1; i++, part += 0x40) {
        *(f32*)(part + 4) = float_0_80425800;
        *(f32*)(part + 8) = float_0_80425800;
        angleDeg = ((i - 1) * 0x168) / count;
        *(f32*)(part + 0xC) = float_0_80425800;
        rad = (float_6p2832_80425820 * (angleOffset + (f32)angleDeg)) / float_360_80425824;
        *(f32*)(part + 0x10) = (f32)sin(rad);
        *(f32*)(part + 0x14) = float_0_80425800;
        *(f32*)(part + 0x18) = (f32)cos(rad);
    }
    return entry;
}

