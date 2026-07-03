#include "effect/n64/eff_kemuri10_n64.h"


u8 effKemuri10Disp(int param_1, int param_2) {
    return 0;
}


u8 effKemuri10N64Entry(void) {
    return 0;
}


void effKemuri10Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri10Disp(s32, s32);
    extern const Vec3 vec3_802fb2e8;
    extern f32 float_0p3_80425834;
    extern f32 float_0p8_80425838;
    extern f32 float_0p6_8042583c;
    extern f32 float_0p7_80425840;
    extern f32 float_1p05_80425844;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 camera;
    s32 i;
    s32 timer;
    s32 alpha;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb2e8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x5C) -= 1;
    *(s32*)(work + 0x60) += 1;
    timer = *(s32*)(work + 0x5C);
    camera = *(s32*)(work + 0x6C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x70) {
        if (timer > 5) {
            alpha = *(s32*)(work + 0x58);
            *(s32*)(work + 0x58) = (s32)((f32)alpha + (float_0p3_80425834 * (f32)(0xFF - alpha)));
            *(f32*)(work + 0x10) *= float_0p8_80425838;
            *(f32*)(work + 0x14) *= float_0p8_80425838;
            *(f32*)(work + 0x18) *= float_0p8_80425838;
        }
        if (timer < 5) {
            *(s32*)(work + 0x58) = (s32)((f32)*(s32*)(work + 0x58) * float_0p6_8042583c);
            *(f32*)(work + 0x10) *= float_0p7_80425840;
            *(f32*)(work + 0x14) *= float_0p7_80425840;
            *(f32*)(work + 0x18) *= float_0p7_80425840;
        }
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x4C) += *(f32*)(work + 0x50);
        *(f32*)(work + 0x54) *= float_1p05_80425844;
    }

    dispEntry(camera, 2, effKemuri10Disp, effect, dispCalcZ(&dispPos));
}

