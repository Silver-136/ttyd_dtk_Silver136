#include "effect/n64/eff_rappa_n64.h"


u8 effRappaDisp(void) {
    return 0;
}


u8 effRappaN64Entry(void) {
    return 0;
}


void effRappaMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRappaDisp(void);
    extern const Vec3 vec3_802fbdc8;
    extern f32 float_0p8_80425e6c;
    extern f32 float_0p9_80425e70;
    extern f32 float_10_80425e74;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbdc8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x34) += 1;
    timer = *(s32*)(work + 0x30);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 10) {
        *(s32*)(work + 0x2C) = (s32)((f32)*(s32*)(work + 0x2C) * float_0p8_80425e6c);
    }

    part = work + 0x38;
    if (timer < 10) {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= float_0p9_80425e70;
            *(f32*)(part + 0x24) += *(f32*)(part + 0x28) / float_10_80425e74;
        }
    } else {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= float_0p9_80425e70;
        }
    }
    dispEntry(4, 2, effRappaDisp, effect, dispCalcZ(&dispPos));
}

