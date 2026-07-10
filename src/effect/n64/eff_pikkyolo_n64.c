#include "effect/n64/eff_pikkyolo_n64.h"


u8 effPikkyoloN64Entry(void) {
    return 0;
}


u8 effPikkyoloDisp(void) {
    return 0;
}


void effPikkyoloMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPikkyoloDisp(void);
    extern const Vec3 vec3_802fbc98;
    extern f32 float_0p5_80425d0c;
    extern f32 float_0_80425d08;
    extern f32 float_neg0p7_80425d14;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 offset;
    f32 half;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc98;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 0x3E8) {
        *(s32*)(work + 0x1C) -= 1;
    }
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 8) {
        *(s16*)(work + 0x32) = *(s32*)(work + 0x1C) << 5;
    }

    half = float_0p5_80425d0c;
    zero = float_0_80425d08;
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x3C) {
        *(f32*)(work + 0x14) -= half;
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        if (*(f32*)(work + 8) < zero && *(f32*)(work + 0x14) < zero) {
            *(f32*)(work + 8) = zero;
            *(f32*)(work + 0x14) *= float_neg0p7_80425d14;
        }
        offset = (((i & 1) << 1) - 1) * 0xE;
        *(f32*)(work + 0x28) += (f32)offset;
    }
    dispEntry(4, 2, effPikkyoloDisp, effect, dispCalcZ(&dispPos));
}

