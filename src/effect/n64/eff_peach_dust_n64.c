#include "effect/n64/eff_peach_dust_n64.h"


u8 effPeachDustN64Entry(void) {
    return 0;
}


u8 effPeachDustDisp(int param_1, int param_2) {
    return 0;
}


void effPeachDustMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPeachDustDisp(s32, s32);
    extern const Vec3 vec3_802fbc68;
    extern u8 scale_data[];
    extern f32 float_0p01_80425ce8;
    extern f32 float_0p4_80425ce4;
    extern f32 float_0p1_80425cec;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 dead;
    s32 frame;
    s32 count;
    f32 scale;

    dead = 0;
    i = 0;
    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc68;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x1C) -= 1;
    *(s32*)(work + 0x20) += 1;

    frame = *(s32*)(work + 0x20);
    part = work + 0x2C;
    while (i < *(s32*)((s32)effect + 8) - 1) {
        *(s32*)(part + 0x28) = (frame + i + i) & 7;
        if (*(s32*)(part + 0x1C) < 0) {
            dead++;
        }
        *(s32*)(part + 0x1C) -= 1;
        count = *(s32*)(part + 0x1C);
        if (count > 0x1E || count < 0) {
            *(s32*)(part + 0x28) = -1;
        } else {
            *(s32*)(part + 0x20) += 1;
            if (*(s32*)(part + 0x20) - 1 < 0x19) {
                *(f32*)(part + 0x24) =
                    float_0p4_80425ce4 * (float_0p01_80425ce8 * (f32)scale_data[*(s32*)(part + 0x20) - 1]);
            } else {
                scale = *(f32*)(part + 0x24);
                *(f32*)(part + 0x24) = scale + float_0p1_80425cec * (float_0p1_80425cec - scale);
            }
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x10);
        }
        i++;
        part += 0x2C;
    }
    if (dead >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effPeachDustDisp, effect, dispCalcZ(&dispPos));
    }
}

