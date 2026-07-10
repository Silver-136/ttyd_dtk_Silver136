#include "effect/n64/eff_flower_n64.h"


u8 effFlowerDisp(int param_1, int param_2) {
    return 0;
}


u8 effFlowerN64Entry(void) {
    return 0;
}


void effFlowerMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effFlowerDisp(s32, s32);
    extern Vec3 vec3_802fafb0[];
    extern f32 float_0p001_80425350;
    extern f32 float_10_80425354;
    extern f32 float_45_80425358;
    extern f32 float_1p4_8042535c;
    extern f32 float_1_80425360;
    extern f32 float_0p0007_80425364;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fafb0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)(work + 0x34) < 0x96) {
        *(f32*)(work + 0x10) -= float_0p001_80425350;
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x24) += *(f32*)(work + 0x2C);
        if (*(s32*)work == 0) {
            if (*(f32*)(work + 0x24) < float_10_80425354) {
                *(f32*)(work + 0x24) = float_10_80425354;
                *(f32*)(work + 0x2C) = -*(f32*)(work + 0x2C);
            }
            if (*(f32*)(work + 0x24) > float_45_80425358) {
                *(f32*)(work + 0x24) = float_45_80425358;
                *(f32*)(work + 0x2C) = -*(f32*)(work + 0x2C);
            }
        }
        *(f32*)(work + 0x28) += *(f32*)(work + 0x30);
        if (*(s32*)work == 0) {
            *(f32*)(work + 0x14) = float_1p4_8042535c;
        } else {
            *(f32*)(work + 0x14) = float_1_80425360;
        }
    } else {
        *(f32*)(work + 0x20) += float_0p0007_80425364;
        *(f32*)(work + 0x1C) += *(f32*)(work + 0x20);
        *(f32*)(work + 0x14) += *(f32*)(work + 0x1C);
        if (*(f32*)(work + 0x14) > float_1_80425360) {
            *(f32*)(work + 0x14) = float_1_80425360;
        }
    }

    if (*(s32*)work == 1) {
        *(s32*)(work + 0x40) += *(s32*)(work + 0x3C);
        if (*(s32*)(work + 0x40) >= 0x14) {
            *(s32*)(work + 0x40) = 0x14;
            *(s32*)(work + 0x3C) = -1;
        } else if (*(s32*)(work + 0x40) <= 0) {
            *(s32*)(work + 0x40) = 0;
            *(s32*)(work + 0x3C) = 1;
        }
    }

    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(s32*)(work + 0x34) -= 1;
    *(s32*)(work + 0x38) += 1;
    if (*(s32*)(work + 0x34) < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effFlowerDisp, effect, dispCalcZ(&dispPos));
    }
}

