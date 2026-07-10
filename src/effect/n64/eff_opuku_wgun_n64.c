#include "effect/n64/eff_opuku_wgun_n64.h"


u8 effOpukuWgunDisp(void) {
    return 0;
}


u8 effOpukuWgunN64Entry(void) {
    return 0;
}


void effOpukuWgunMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOpukuWgunDisp(void);
    extern f64 sin(f64);
    extern const Vec3 vec3_802fbbe8;
    extern f32 float_6p2832_80425bd8;
    extern f32 float_360_80425bdc;
    extern f32 float_180_80425bf4;
    extern f32 float_120_80425bf8;
    extern f32 float_255_80425bfc;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 duration;
    s32 i;
    s32 step;
    f32 t;
    f32 phase;
    s32 alpha;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbbe8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    type = *(s32*)work;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x2C) = 0x10;
    }
    if (*(s32*)(work + 0x2C) < 0x3E8) {
        *(s32*)(work + 0x2C) -= 1;
    }
    *(s32*)(work + 0x30) += 1;
    if (*(s32*)(work + 0x2C) < 0) {
        effDelete(effect);
        return;
    }

    duration = *(s32*)(work + 0x28);
    *(s32*)(work + 0x54) += 1;
    if (*(s32*)(work + 0x54) > 0xC) {
        *(s32*)(work + 0x54) = 0xC;
    }

    p = work;
    for (i = 0; i < 0xC; i++, p += 4) {
        if (*(s32*)(p + 0x178) != 0) {
            *(s32*)(p + 0x178) -= 1;
            if (*(s32*)(p + 0x178) != 0) {
                continue;
            }
        }

        step = *(s32*)(p + 0x1A8) + 1;
        *(s32*)(p + 0x1A8) = step;
        if (step >= duration) {
            step = duration;
        }

        t = (f32)step / (f32)duration;
        if (type == 0) {
            *(f32*)(p + 0xE8) = *(f32*)(work + 0x10) + t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10));
            *(f32*)(p + 0x118) = *(f32*)(work + 0x14) + t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14));
            *(f32*)(p + 0x148) = *(f32*)(work + 0x18) + t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
        } else {
            phase = (float_6p2832_80425bd8 * (float_180_80425bf4 * t)) / float_360_80425bdc;
            *(f32*)(p + 0xE8) = *(f32*)(work + 0x10) + t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10));
            alpha = (s32)(float_255_80425bfc - (float_255_80425bfc * t));
            *(f32*)(p + 0x118) = *(f32*)(work + 0x14) + t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14)) + float_120_80425bf8 * (f32)sin(phase);
            *(f32*)(p + 0x148) = *(f32*)(work + 0x18) + t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
            *(u8*)(work + 0x1D8 + i) = (u8)alpha;
        }
    }

    dispEntry(4, 2, effOpukuWgunDisp, effect, dispCalcZ(&dispPos));
}

