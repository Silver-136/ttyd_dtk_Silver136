#include "effect/n64/eff_water_damage_n64.h"


u8 effWaterDamageN64Entry(s64 marioPosX, s64 marioPosYWithOffset, s64 marioPosZ, s64 sizeScale, s32 param_5, s32 param_6) {
    return 0;
}


u8 effWaterDamageDisp(int param_1, int param_2) {
    return 0;
}


u8 effWaterDamageDisp2(int param_1, int param_2) {
    return 0;
}


void effWaterDamageMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effWaterDamageDisp(void);
    extern void effWaterDamageDisp2(void);
    extern const Vec3 vec3_802fc268;
    extern f32 float_0p98_804264e8;
    extern f32 float_neg0p8_804264ec;
    extern f32 float_neg0p1_804264f0;
    extern f32 float_1p2_804264f4;
    extern f32 float_0p9_804264f8;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 altDisp;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc268;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    type = *(s32*)work;
    altDisp = *(s32*)(work + 0x38);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 1000) {
        *(s32*)(work + 0x1C) -= 1;
    }
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 4) {
        *(s32*)(work + 0x30) = *(s32*)(work + 0x1C) << 6;
    }
    part = work + 0x3C;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x3C) {
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(s32*)(part + 0x30) = (s32)((f32)*(s32*)(part + 0x30) * float_0p98_804264e8);
        if (type == 2) {
            *(f32*)(part + 0x14) += float_neg0p8_804264ec;
        }
        *(f32*)(part + 0x14) += float_neg0p1_804264f0;
        *(f32*)(part + 0x10) *= float_0p98_804264e8;
        *(f32*)(part + 0x14) *= float_0p98_804264e8;
        *(f32*)(part + 0x18) *= float_0p98_804264e8;
        if (type == 3) {
            *(f32*)(part + 0x34) *= float_1p2_804264f4;
        } else {
            *(f32*)(part + 0x34) *= float_0p9_804264f8;
        }
    }
    if (altDisp == 0) {
        dispEntry(4, 8, effWaterDamageDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(4, 8, effWaterDamageDisp2, effect, dispCalcZ(&dispPos));
    }
}

