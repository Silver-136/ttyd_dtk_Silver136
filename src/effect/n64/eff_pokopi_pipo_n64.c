#include "effect/n64/eff_pokopi_pipo_n64.h"


u8 effPokopiPipoN64Entry(void) {
    return 0;
}


u8 effPokopiPipoDisp(void) {
    return 0;
}


void effPokopiPipoMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiPipoDisp(void);
    extern const Vec3 vec3_802fbcf8;
    extern u8 w_scale_data[];
    extern u8 h_scale_data[];
    extern f32 float_0p01_80425d78;
    extern f32 float_40_80425d7c;
    extern f32 float_neg0p2_80425d80;
    extern f32 float_neg1_80425d84;
    extern f32 float_neg4_80425d88;
    extern f32 float_0_80425d74;
    extern f32 float_14p286_80425d8c;
    extern f32 float_neg20_80425d90;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;
    s32 scaleIndex;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbcf8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x10;
    }
    if (*(s32*)(work + 0x28) < 0x3E8) {
        *(s32*)(work + 0x28) -= 1;
    }
    *(s32*)(work + 0x2C) += 1;
    frame = *(s32*)(work + 0x2C);
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }

    scaleIndex = *(s32*)(work + 0x60);
    if (scaleIndex >= 0 && scaleIndex < 7) {
        *(f32*)(work + 0x58) = float_0p01_80425d78 * (f32)w_scale_data[scaleIndex];
        *(f32*)(work + 0x5C) = float_0p01_80425d78 * (f32)h_scale_data[scaleIndex];
        *(f32*)(work + 0x50) -= float_40_80425d7c;
        *(s32*)(work + 0x60) += 1;
    } else {
        *(f32*)(work + 0x48) += float_neg0p2_80425d80;
        *(f32*)(work + 4) += *(f32*)(work + 0x44);
        *(f32*)(work + 8) += *(f32*)(work + 0x48);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x4C);
        *(f32*)(work + 0x50) += *(f32*)(work + 0x54);
    }

    if (frame - 1 == *(s32*)(work + 0x68)) {
        *(f32*)(work + 0x48) *= float_neg1_80425d84;
        *(f32*)(work + 0x54) = float_neg4_80425d88;
        *(s32*)(work + 0x60) = 0;
    } else if (*(f32*)(work + 0x48) < float_0_80425d74 && *(f32*)(work + 8) < float_14p286_80425d8c) {
        *(f32*)(work + 0x48) = *(f32*)(work + 0x48) - *(f32*)(work + 0x48);
        *(f32*)(work + 8) = float_14p286_80425d8c;
        *(f32*)(work + 0x54) = float_neg20_80425d90;
    }
    dispEntry(4, 2, effPokopiPipoDisp, effect, dispCalcZ(&dispPos));
}

