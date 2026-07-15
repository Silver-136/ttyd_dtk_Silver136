#include "effect/n64/eff_kemuri5_n64.h"


#pragma optimize_for_size off

void* effKemuri5N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKemuri5Main(void*);
    extern s32 rand(void);
    extern f64 reviseAngle(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri5N64_802fb24c[];
    extern f32 float_0_804256d8;
    extern f32 float_6p2832_804256e4;
    extern f32 float_360_804256e8;
    extern f32 float_1_80425704;
    extern f32 float_0p03_80425708;
    extern f32 float_1p7_8042570c;
    extern f32 float_0p5_80425710;
    extern f32 float_neg0p02_80425714;
    extern f32 float_5Eneg05_80425718;
    extern f32 float_neg3p9_8042571c;
    void* entry;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri5N64_802fb24c;
    *(s32*)((s32)entry + 8) = 8;
    part = __memAlloc(3, 0x260);
    *(u8**)((s32)entry + 0xC) = part;
    *(void**)((s32)entry + 0x10) = effKemuri5Main;
    for (i = 0; i < 8; i++) {
        *(s32*)part = 1;
        *(u8*)(part + 8) = 0xFF;
        *(s16*)(part + 6) = 0x1E;
        *(f32*)(part + 0xC) = x;
        *(f32*)(part + 0x10) = y;
        *(f32*)(part + 0x14) = z;
        *(f32*)(part + 0x18) = float_1_80425704;
        *(f32*)(part + 0x1C) = float_1_80425704;
        *(f32*)(part + 0x20) = float_1_80425704;
        *(f32*)(part + 0x24) = float_0p03_80425708 * (f32)(rand() % 11) + float_1_80425704;
        *(f32*)(part + 0x28) = float_0p03_80425708 * (f32)(rand() % 11) + float_1p7_8042570c;
        *(f32*)(part + 0x2C) = (f32)(rand() % 61);
        *(f32*)(part + 0x30) = float_neg3p9_8042571c;
        *(f32*)(part + 0x34) = float_0p5_80425710;
        *(f32*)(part + 0x38) = float_neg0p02_80425714;
        *(f32*)(part + 0x3C) = float_5Eneg05_80425718;
        *(f32*)(part + 0x40) = float_0_804256d8;
        *(f32*)(part + 0x44) = (f32)sin(reviseAngle((f64)(angle + i * 45)) * float_6p2832_804256e4 / float_360_804256e8);
        *(f32*)(part + 0x48) = (f32)cos(reviseAngle((f64)(angle + i * 45)) * float_6p2832_804256e4 / float_360_804256e8);
        part += 0x4C;
    }
    return entry;
}

#pragma optimize_for_size on

u8 effKemuri5Disp(int param_1, int param_2) {
    return 0;
}


void effKemuri5Main(void* effect) {
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
    extern void effKemuri5Disp(s32, s32);
    extern Vec3 vec3_802fb240;
    extern f32 float_12_804256e0;
    extern f32 float_6p2832_804256e4;
    extern f32 float_360_804256e8;
    extern f32 float_0p1_804256ec;
    extern f32 float_0p83_804256f0;
    extern f32 float_0p006_804256f4;
    extern f32 float_7p5_804256f8;
    extern f32 float_0p98_804256fc;
    extern f32 float_0p9_80425700;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 alive;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb240;
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
                *(f32*)(work + 0x2C) = reviseAngle(float_12_804256e0 + *(f32*)(work + 0x2C));
                *(f32*)(work + 0x18) = *(f32*)(work + 0x24) +
                    float_0p1_804256ec *
                    (f32)sin(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x1C) = *(f32*)(work + 0x28) +
                    float_0p1_804256ec *
                    (f32)cos(float_6p2832_804256e4 * *(f32*)(work + 0x2C) / float_360_804256e8);
                *(f32*)(work + 0x30) *= float_0p83_804256f0;
                *(f32*)(work + 0xC) += *(f32*)(work + 0x30) * *(f32*)(work + 0x44);
                *(f32*)(work + 0x14) += *(f32*)(work + 0x30) * *(f32*)(work + 0x48);
                *(f32*)(work + 0x38) += *(f32*)(work + 0x3C);
                *(f32*)(work + 0x34) += *(f32*)(work + 0x38);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x34);
                *(f32*)(work + 0x24) += float_0p006_804256f4 * (float_7p5_804256f8 - *(f32*)(work + 0x24));
                *(f32*)(work + 0x28) *= float_0p98_804256fc;
                if (*(s16*)(work + 6) < 10) {
                    *(f32*)(work + 0x28) *= float_0p9_80425700;
                }
                if (*(s16*)(work + 6) < 15) {
                    *(u8*)(work + 8) -= 0x10;
                }
            }
        }
        work += 0x4C;
    }

    if (alive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri5Disp, effect, dispCalcZ(&dispPos));
    }
}

