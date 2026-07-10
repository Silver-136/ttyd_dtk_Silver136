#include "effect/n64/eff_kemuri6_n64.h"


u8 effKemuri6Disp(int param_1, int param_2) {
    return 0;
}


void* effKemuri6N64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri6Main(void*);
    extern s32 rand(void);
    extern char str_Kemuri6N64_802fb26c[];
    extern f32 float_0_80425728;
    extern f32 float_1_80425748;
    extern f32 float_1p4_8042574c;
    extern f32 float_0p03_80425750;
    extern f32 float_1p5_80425754;
    extern f32 float_2_80425758;
    extern f32 float_neg0p5_8042575c;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri6N64_802fb26c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x50);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri6Main;

    *(s32*)work = 1;
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x40) = float_0_80425728;
    *(f32*)(work + 0x44) = float_0_80425728;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x1C) = float_1_80425748;
    *(f32*)(work + 0x20) = float_1_80425748;
    *(f32*)(work + 0x24) = float_1_80425748;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x28) = float_1p4_8042574c + float_0p03_80425750 * (f32)(rand() % 11);
    *(f32*)(work + 0x2C) = float_1p5_80425754 + float_0p03_80425750 * (f32)(rand() % 11);
    *(f32*)(work + 0x30) = (f32)(rand() % 61);
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x38) = float_2_80425758;
    *(f32*)(work + 0x3C) = float_neg0p5_8042575c;
    *(s16*)(work + 6) = 0xF;
    *(f32*)(work + 0x18) = (f32)(rand() % 0x169);
    *(u8*)(work + 8) = 0xFF;

    return entry;
}

void effKemuri6Main(void* effect) {
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
    extern void effKemuri6Disp(s32, s32);
    extern void* marioGetPtr(void);
    extern Vec3 vec3_802fb260;
    extern f32 float_12_80425730;
    extern f32 float_6p2832_80425734;
    extern f32 float_360_80425738;
    extern f32 float_0p1_8042573c;
    extern f32 float_0p14_80425740;
    extern f32 float_0p94_80425744;
    extern f32 float_0_80425728;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 alive;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb260;
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
                *(f32*)(work + 0x30) = reviseAngle(float_12_80425730 + *(f32*)(work + 0x30));
                *(f32*)(work + 0x1C) = *(f32*)(work + 0x28) +
                    float_0p1_8042573c *
                    (f32)sin(float_6p2832_80425734 * *(f32*)(work + 0x30) / float_360_80425738);
                *(f32*)(work + 0x20) = *(f32*)(work + 0x2C) +
                    float_0p1_8042573c *
                    (f32)cos(float_6p2832_80425734 * *(f32*)(work + 0x30) / float_360_80425738);
                if (*(u16*)(work + 4) == 0 && *(s16*)(work + 6) > 4 && *(s16*)(work + 6) <= 5) {
                    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0) {
                        *(s16*)(work + 6) += 1;
                        return;
                    }
                }
                *(f32*)(work + 0x3C) += *(f32*)(work + 0x40);
                *(f32*)(work + 0x38) += *(f32*)(work + 0x3C);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x38);
                *(u8*)(work + 8) -= 0xF;
                *(f32*)(work + 0x28) += float_0p14_80425740;
                *(f32*)(work + 0x2C) *= float_0p94_80425744;
                if (*(f32*)(work + 0x38) < float_0_80425728) {
                    *(f32*)(work + 0x3C) = float_0_80425728;
                    *(f32*)(work + 0x38) = float_0_80425728;
                }
            }
        }
        work += 0x50;
    }

    if (alive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri6Disp, effect, dispCalcZ(&dispPos));
    }
}

