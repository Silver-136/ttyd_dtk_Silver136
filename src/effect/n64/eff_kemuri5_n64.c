#include "effect/n64/eff_kemuri5_n64.h"


u8 effKemuri5N64Entry(void) {
    return 0;
}


u8 effKemuri5Disp(int param_1, int param_2) {
    return 0;
}


void effKemuri5Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri5Disp(s32, s32);
    extern Vec3 vec3_802fb240;
    extern f32 float_12_804256e0;
    extern f32 float_6p2832_804256e4;
    extern f32 float_360_804256e8;
    extern f32 float_0p1_804256ec;
    extern f32 float_0p83_804256f0;
    extern f32 float_0p006_804256f4;
    extern f32 float_7p5_804256f8;
    extern f32 float_0p98_804256fc;
    extern f32 float_0p9_80425700;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 alive;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb240;
    pos.x = *(f32*)(work + 0xC);
    pos.y = *(f32*)(work + 0x10);
    pos.z = *(f32*)(work + 0x14);
    dispPos = pos;
    alive = 0;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        if (*(s32*)work != 0) {
            *(s16*)(work + 6) -= 1;
            if (*(s16*)(work + 6) <= 0) {
                *(s32*)work = 0;
            } else {
                alive = 1;
                *(f32*)(work + 0x2C) = reviseAngle(float_12_804256e0 + *(f32*)(work + 0x2C));
                *(f32*)(work + 0x18) = *(f32*)(work + 0x24) +
                    float_0p1_804256ec *
                    (f32)sin(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x1C) = *(f32*)(work + 0x28) +
                    float_0p1_804256ec *
                    (f32)cos(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x30) *= float_0p83_804256f0;
                *(f32*)(work + 0xC) += *(f32*)(work + 0x30) * *(f32*)(work + 0x44);
                *(f32*)(work + 0x14) += *(f32*)(work + 0x30) * *(f32*)(work + 0x48);
                *(f32*)(work + 0x38) += *(f32*)(work + 0x3C);
                *(f32*)(work + 0x34) += *(f32*)(work + 0x38);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x34);
                *(f32*)(work + 0x24) += float_0p006_804256f4 * (float_7p5_804256f8 - *(f32*)(work + 0x24));
                *(f32*)(work + 0x28) *= float_0p98_804256fc;
                if (*(s16*)(work + 6) < 10) {
                    *(f32*)(work + 0x28) *= float_0p9_80425700;
                }
                if (*(s16*)(work + 6) < 15) {
                    *(u8*)(work + 8) -= 0x10;
                }
            }
        }
        work += 0x4C;
    }

    if (alive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri5Disp, effect, dispCalcZ(&dispPos));
    }
}

