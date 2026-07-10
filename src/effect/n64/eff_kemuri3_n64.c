#include "effect/n64/eff_kemuri3_n64.h"


u8 effKemuri3Disp(int param_1, int param_2) {
    return 0;
}


void effKemuri3Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri3Disp(void);
    extern void PSMTXRotRad(f32 m[3][4], char axis, f32 rad);
    extern void PSMTXTrans(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern Vec3 vec3_802fb200[];
    extern f32 float_0p85_80425674;
    extern f32 float_0_80425668;
    extern f32 float_0p004_80425678;
    extern f32 float_1_8042566c;
    extern f32 float_neg100_8042567c;
    extern f32 float_deg2rad_80425680;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;
    f32 mScale[3][4];
    f32 mTrans[3][4];
    f32 mRotX[3][4];
    f32 mRotY[3][4];
    f32 mRotZ[3][4];
    s32 alive;
    s32 i;
    s16 timer;

    alive = 0;
    i = 0;
    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb200;
    pos = *base;
    pos.x = *(f32*)(work + 0xC);
    pos.y = *(f32*)(work + 0x10);
    pos.z = *(f32*)(work + 0x14);
    dispPos = pos;

    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)work != 0) {
            timer = *(s16*)(work + 6) - 1;
            *(s16*)(work + 6) = timer;
            if (timer <= 0) {
                *(s32*)work = 0;
            } else {
                alive = 1;
                *(f32*)(work + 0x60) *= float_0p85_80425674;
                *(f32*)(work + 0xC) += *(f32*)(work + 0x60) * *(f32*)(work + 0x80);
                *(f32*)(work + 0x14) += *(f32*)(work + 0x60) * *(f32*)(work + 0x84);
                *(f32*)(work + 0x78) += *(f32*)(work + 0x7C);
                *(f32*)(work + 0x74) += *(f32*)(work + 0x78);
                *(f32*)(work + 0x70) += *(f32*)(work + 0x74);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x70);
                if (*(f32*)(work + 0x70) < float_0_80425668) {
                    *(f32*)(work + 0x78) = float_0p004_80425678;
                }
                *(f32*)(work + 0x24) -= float_1_8042566c;
                if (*(f32*)(work + 0x24) < float_neg100_8042567c) {
                    *(f32*)(work + 0x24) = float_neg100_8042567c;
                }

                PSMTXRotRad(mRotX, 'x', float_deg2rad_80425680 * *(f32*)(work + 0x24));
                PSMTXRotRad(mRotY, 'y', float_deg2rad_80425680 * *(f32*)(work + 0x28));
                PSMTXRotRad(mRotZ, 'z', float_deg2rad_80425680 * *(f32*)(work + 0x2C));
                PSMTXTrans(mTrans, *(f32*)(work + 0xC), *(f32*)(work + 0x10), *(f32*)(work + 0x14));
                PSMTXScale(mScale, *(f32*)(work + 0x18), *(f32*)(work + 0x1C), *(f32*)(work + 0x20));
                PSMTXConcat(mRotX, mRotZ, mRotZ);
                PSMTXConcat(mRotY, mRotZ, mRotZ);
                PSMTXConcat(mRotZ, mScale, mScale);
                PSMTXConcat(mTrans, mScale, (f32 (*)[4])(work + 0x30));
            }
        }
        i++;
        work += 0x88;
    }

    if (alive != 0) {
        dispEntry(4, 2, effKemuri3Disp, effect, dispCalcZ(&dispPos));
    } else {
        effDelete(effect);
    }
}

void* effKemuri3N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri3Main(void*);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri3N64_802fb20c[];
    extern f32 float_0p3_80425684;
    extern f32 float_0_80425668;
    extern f32 float_1_8042566c;
    extern f32 float_1p75_80425688;
    extern f32 float_neg0p08_8042568c;
    extern f32 float_neg3p9_80425690;
    extern f32 float_6p2832_80425694;
    extern f32 float_360_80425698;
    void* entry;
    u8* work;
    s32 i;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri3N64_802fb20c;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0x2A8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri3Main;

    for (i = 0; i < 5; i++) {
        *(s32*)work = 1;
        *(f32*)(work + 0x64) = float_0p3_80425684;
        *(f32*)(work + 0x68) = float_0_80425668;
        *(f32*)(work + 0x6C) = float_0_80425668;
        *(f32*)(work + 0xC) = x;
        *(f32*)(work + 0x10) = y;
        *(f32*)(work + 0x14) = z;
        *(f32*)(work + 0x18) = float_1_8042566c;
        *(f32*)(work + 0x1C) = float_1_8042566c;
        *(f32*)(work + 0x20) = float_1_8042566c;
        *(u8*)(work + 8) = 0xFF;
        *(s16*)(work + 6) = 0x3C;
        *(f32*)(work + 0x24) = float_0_80425668;
        *(f32*)(work + 0x28) = angle + (f32)(i * 0x48);
        *(f32*)(work + 0x2C) = float_0_80425668;
        *(f32*)(work + 0x70) = float_1p75_80425688;
        *(f32*)(work + 0x74) = float_neg0p08_8042568c;
        *(f32*)(work + 0x78) = float_0_80425668;
        *(f32*)(work + 0x7C) = float_0_80425668;
        *(f32*)(work + 0x60) = float_neg3p9_80425690;
        rad = float_6p2832_80425694 * reviseAngle(*(f32*)(work + 0x28)) / float_360_80425698;
        *(f32*)(work + 0x80) = (f32)sin(rad);
        *(f32*)(work + 0x84) = (f32)cos(rad);
        work += 0x88;
    }

    return entry;
}

