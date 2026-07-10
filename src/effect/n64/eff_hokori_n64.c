#include "effect/n64/eff_hokori_n64.h"


u8 effHokoriDisp(int param_1, int param_2) {
    return 0;
}


u8 effHokoriN64Entry(void) {
    return 0;
}


void effHokoriMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effHokoriDisp(s32, s32);
    extern Vec3 vec3_802fb0d0[];
    extern f32 float_0p9_80425490;
    extern f32 float_0p005_80425494;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb0d0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x2C) -= 1;
    if (*(s32*)(work + 0x2C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) < 0x18) {
        *(s32*)(work + 0x18) = (s32)((f32)*(s32*)(work + 0x18) * float_0p9_80425490);
    }

    part = work + 0x48;
    if (*(s32*)work <= 1) {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x24) += float_0p005_80425494;
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    } else {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    }

    dispEntry(4, 2, effHokoriDisp, effect, dispCalcZ(&dispPos));
}

