#include "effect/n64/eff_pokopi_thunder_n64.h"


u8 effPokopiThunderDisp(void) {
    return 0;
}


u8 effPokopiThunderN64Entry(void) {
    return 0;
}


void effPokopiThunderMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiThunderDisp(void);
    extern const Vec3 vec3_802fbd28;
    extern f32 float_0p5_80425dd4;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbd28;
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
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    type = *(s32*)work;
    frame = *(s32*)(work + 0x2C);
    if (type == 2) {
        if (frame >= 0x28) {
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
        }
    } else if (frame >= 0xB) {
        *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
    }
    dispEntry(*(s32*)(work + 0x118), 2, effPokopiThunderDisp, effect, dispCalcZ(&dispPos));
}

