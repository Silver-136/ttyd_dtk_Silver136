#include "effect/n64/eff_glass_n64.h"


u8 effGlassDisp(int param_1, int param_2) {
    return 0;
}


u8 effGlassN64Entry(void) {
    return 0;
}


void effGlassMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effGlassDisp(s32, s32);
    extern Vec3 vec3_802fb020[];
    extern f32 float_0_8042540c;
    extern f32 float_1_80425410;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb020;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x10) << 4;
    }
    *(f32*)(work + 0x44) += *(f32*)(work + 0x48);
    if (*(f32*)(work + 0x44) < float_0_8042540c) {
        *(f32*)(work + 0x44) += float_1_80425410;
    }
    if (*(f32*)(work + 0x44) > float_1_80425410) {
        *(f32*)(work + 0x44) -= float_1_80425410;
    }
    *(f32*)(work + 0x50) += *(f32*)(work + 0x58);
    *(f32*)(work + 4) += *(f32*)(work + 0x4C);
    *(f32*)(work + 8) += *(f32*)(work + 0x50);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x54);
    *(f32*)(work + 0x3C) += *(f32*)(work + 0x40);
    dispEntry(4, 2, effGlassDisp, effect, dispCalcZ(&dispPos));
}

