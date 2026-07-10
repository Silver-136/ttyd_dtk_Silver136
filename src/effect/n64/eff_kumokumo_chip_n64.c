#include "effect/n64/eff_kumokumo_chip_n64.h"


u8 effKumokumoChipN64Entry(void) {
    return 0;
}


u8 effKumokumoChipDisp(int param_1, int param_2) {
    return 0;
}


void effKumokumoChipMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKumokumoChipDisp(s32, s32);
    extern const Vec3 vec3_802fb378;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    s32 timer;
    s32 frame;
    s32 flags;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb378;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    flags = *(s32*)effect;
    type = *(s32*)work;
    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    timer = *(s32*)(work + 0x1C);
    if (timer < 0x3E8) {
        *(s32*)(work + 0x1C) = timer - 1;
    }
    *(s32*)(work + 0x20) += 1;
    timer = *(s32*)(work + 0x1C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x20);
    if (type == 0 || type == 2) {
        if (frame < 0x10) {
            *(s32*)(work + 0x30) = (frame << 4) + 0xF;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 0x10) {
            *(s32*)(work + 0x30) = timer << 4;
        }
    } else {
        if (frame < 4) {
            *(s32*)(work + 0x30) = (frame << 6) + 0x3F;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 4) {
            *(s32*)(work + 0x30) = timer << 6;
        }
    }

    *(f32*)(work + 4) += *(f32*)(work + 0x10);
    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
    dispEntry(4, 2, effKumokumoChipDisp, effect, dispCalcZ(&dispPos));
}

