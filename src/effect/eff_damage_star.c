#include "effect/eff_damage_star.h"


u8 effDamageStarDisp(void) {
    return 0;
}


u8 effDamageStarEntry(void) {
    return 0;
}


u8 effDamageStarMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effCalcMayaAnim(void* anim);
    extern void effDeleteMayaAnim(void* anim);
    extern void effDelete(void* effect);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effDamageStarDisp(void);
    extern void* gpGlobals;
    extern LocalVec3 vec3_802f3a60;
    extern u8 scale_data[];
    extern f32 float_0_80422c80;
    extern f32 float_1_80422c98;
    extern f32 float_10_80422ca4;
    extern f32 float_0p5_80422cc4;
    extern f32 float_0p01_80422cc8;
    extern f32 float_720_80422ccc;
    extern f32 float_1p5_80422cd0;
    extern f32 float_0p1_80422cd4;
    extern f32 float_0p75_80422cd8;
    LocalVec3 dispPos;
    void* work;
    void* part;
    s32 delta;
    s32 timer;
    s32 i;
    f32 scale;

    work = *(void**)((s32)effEntry + 0xC);
    dispPos = vec3_802f3a60;
    dispPos.x = *(f32*)((s32)work + 0x4);
    dispPos.y = *(f32*)((s32)work + 0x8);
    dispPos.z = *(f32*)((s32)work + 0xC);

    effCalcMayaAnim(*(void**)((s32)work + 0x44));

    delta = 0;
    if (*(u32*)((s32)work + 0x40) < *(u32*)((s32)gpGlobals + 0x1C)) {
        delta = *(u32*)((s32)gpGlobals + 0x1C) - *(u32*)((s32)work + 0x40);
    }
    if (delta < 1) {
        delta = 1;
    }

    while (delta-- > 0) {
        if (*(s32*)((s32)work + 0x24) < 100) {
            *(s32*)((s32)work + 0x24) -= 1;
        }
        *(s32*)((s32)work + 0x28) += 1;

        if (*(s32*)((s32)work + 0x24) < 0) {
            effDeleteMayaAnim(*(void**)((s32)work + 0x44));
            effDelete(effEntry);
            return 0;
        }

        timer = *(s32*)((s32)work + 0x28);
        part = (void*)((s32)work + 0x90);
        for (i = 2; i < *(s32*)((s32)effEntry + 0x8); i++, part = (void*)((s32)part + 0x48)) {
            if (timer < 20) {
                scale = float_0p01_80422cc8 * (f32)scale_data[timer];
                *(f32*)((s32)part + 0x2C) = scale;
                *(f32*)((s32)part + 0x30) = scale;
                *(f32*)((s32)part + 0x34) = scale;
            } else if (timer < 40) {
                *(f32*)((s32)part + 0x1C) = intplGetValue(float_0_80422c80, float_720_80422ccc, 0xB, timer - 20, 19);
            } else if (timer < 50) {
                *(f32*)((s32)part + 0x2C) = intplGetValue(float_1_80422c98, float_1p5_80422cd0, 4, timer - 40, 9);
                *(f32*)((s32)part + 0x30) = intplGetValue(float_1_80422c98, float_0p5_80422cc4, 4, timer - 40, 9);
            } else if (timer < 60) {
                *(f32*)((s32)part + 0x2C) = intplGetValue(float_1p5_80422cd0, float_0p1_80422cd4, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x30) = intplGetValue(float_0p5_80422cc4, float_1_80422c98, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x8) = intplGetValue(*(f32*)((s32)part + 0x8), *(f32*)((s32)part + 0x8) + float_10_80422ca4, 1, timer - 50, 9);
            } else {
                *(f32*)((s32)part + 0x2C) = float_0_80422c80;
                *(f32*)((s32)part + 0x30) = float_0_80422c80;
                *(f32*)((s32)part + 0x34) = float_0_80422c80;
            }

            *(f32*)((s32)part + 0x10) *= float_0p75_80422cd8;
            *(f32*)((s32)part + 0x14) *= float_0p75_80422cd8;
            *(f32*)((s32)part + 0x18) *= float_0p75_80422cd8;
            *(f32*)((s32)part + 0x4) += *(f32*)((s32)part + 0x10);
            *(f32*)((s32)part + 0x8) += *(f32*)((s32)part + 0x14);
            *(f32*)((s32)part + 0xC) += *(f32*)((s32)part + 0x18);
        }
    }

    *(s32*)((s32)work + 0x40) = *(s32*)((s32)gpGlobals + 0x1C);
    dispEntry(4, 8, effDamageStarDisp, effEntry, dispCalcZ(&dispPos));
    return 0;
}

