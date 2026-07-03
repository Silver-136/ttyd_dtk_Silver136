#include "effect/n64/eff_sweat_n64.h"


u8 effSweatDisp(void) {
    return 0;
}


u8 effSweatN64Entry(void) {
    return 0;
}


void effSweatMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effSweatDisp(void);
    extern const Vec3 vec3_802fc144;
    extern f32 float_0p94_8042634c;
    extern f32 float_0p05_80426350;
    extern f32 float_neg0p1_80426354;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 dead;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc144;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    dead = 0;
    part = work + 0x34;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        if (*(s32*)(part + 0x2C) != 0) {
            *(s32*)(part + 0x2C) -= 1;
            continue;
        }
        *(f32*)(part + 4) += *(f32*)(part + 0x18);
        *(f32*)(part + 8) += *(f32*)(part + 0x1C);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x20);
        *(f32*)(part + 0x18) *= float_0p94_8042634c;
        *(f32*)(part + 0x1C) -= float_0p05_80426350;
        if (*(f32*)(part + 0x1C) < float_neg0p1_80426354) {
            *(f32*)(part + 0x1C) = float_neg0p1_80426354;
        }
        *(f32*)(part + 0x20) *= float_0p94_8042634c;
        if (*(s32*)(part + 0x28) < 8) {
            *(s32*)(part + 0x30) = (*(s32*)(part + 0x28) << 5) + 0x1F;
        }
        if (*(s32*)(part + 0x24) < 0x10) {
            *(s32*)(part + 0x30) = *(s32*)(part + 0x24) << 4;
        }
        if (*(s32*)(part + 0x30) > 0xFF) {
            *(s32*)(part + 0x30) = 0xFF;
        }
        if (*(s32*)(part + 0x30) < 0) {
            *(s32*)(part + 0x30) = 0;
        }
        *(s32*)(part + 0x28) += 1;
        *(s32*)(part + 0x24) -= 1;
        if (*(s32*)(part + 0x24) < 0) {
            dead++;
        }
    }
    if (dead >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effSweatDisp, effect, dispCalcZ(&dispPos));
    }
}

