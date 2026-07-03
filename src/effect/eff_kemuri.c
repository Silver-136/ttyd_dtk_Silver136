#include "effect/eff_kemuri.h"


u8 effKemuri2Entry(f32 x, f32 y, f32 z, f32 vx, f32 vz) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void* camGetPtr(s32 cameraId);
    extern f32 reviseAngle(f32 angle);
    extern u8 effKemuri2Main(void* effEntry);
    extern char str_Kemuri2_802f3720[];
    extern f32 float_1000_804229f8;
    extern f32 float_0_804229a4;
    extern f32 float_360_804229f4;
    extern f32 float_6p2832_804229fc;
    extern f32 float_5_80422a00;
    extern f32 float_1_80422998;
    extern f32 float_0p7_80422a04;
    extern f32 float_neg20_80422a08;
    extern double sin(double x);
    extern double cos(double x);
    void* entry;
    void* work;
    f32 sx;
    f32 sz;
    f32 angle;
    f32 camAngle;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri2_802f3720;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x8C);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, 0x8C);
    *(void**)((s32)entry + 0x10) = effKemuri2Main;

    sx = float_1000_804229f8 * vx;
    sz = float_1000_804229f8 * vz;
    angle = (float_360_804229f4 * angleABf(float_0_804229a4, float_0_804229a4, sx, sz)) / float_6p2832_804229fc;
    camAngle = *(f32*)((s32)camGetPtr(4) + 0x114);
    *(f32*)((s32)work + 0x78) = reviseAngle(((float_360_804229f4 * angleABf(float_0_804229a4, float_0_804229a4, sx, sz)) / float_6p2832_804229fc) + camAngle);
    *(s32*)((s32)work + 0x3C) = 0;
    rad = (float_6p2832_804229fc * (f32)(s32)angle) / float_360_804229f4;
    *(f32*)((s32)work + 0x30) = (f32)sin(rad);
    *(f32*)((s32)work + 0x34) = (f32)cos(rad);
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = float_5_80422a00 + z;
    *(f32*)((s32)work + 0x10) = float_1_80422998;
    *(f32*)((s32)work + 0x20) = float_0p7_80422a04;
    *(f32*)((s32)work + 0x64) = float_neg20_80422a08;
    *(f32*)((s32)work + 0x68) = float_0_804229a4;
    *(s32*)((s32)work + 0x6C) = 0xFF;
    *(f32*)((s32)work + 0x70) = float_1_80422998;
    *(f32*)((s32)work + 0x74) = float_1_80422998;
    return 0;
}

void* effKemuriEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_Kemuri_802f3728[];
    extern f32 float_0_804229a4;
    extern f32 float_1_80422998;
    extern f32 float_3_80422a0c;
    extern f32 float_neg3_80422a10;
    extern void* effKemuriMain(void* effEntry);
    void* entry;
    void* work;
    void* child;
    s32 count;
    s32 i;

    entry = effEntry();
    count = 1;
    if (kind == 0) {
        count = 3;
    } else if (kind == 1) {
        count = 2;
    } else if (kind == 2) {
        count = 2;
    }

    *(char**)((s32)entry + 0x14) = str_Kemuri_802f3728;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 0x8C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuriMain;

    *(s32*)((s32)work + 0x0) = kind;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x40) = 0;
    *(f32*)((s32)work + 0x44) = float_1_80422998;
    *(f32*)((s32)work + 0x48) = float_1_80422998;
    *(f32*)((s32)work + 0x74) = float_1_80422998;

    child = (void*)((s32)work + 0x8C);
    i = 1;
    while (i < *(s32*)((s32)entry + 0x8)) {
        *(f32*)((s32)child + 0x4) = float_0_804229a4;
        *(f32*)((s32)child + 0x8) = float_0_804229a4;
        *(f32*)((s32)child + 0xC) = float_0_804229a4;
        *(f32*)((s32)child + 0x44) = float_1_80422998;
        *(f32*)((s32)child + 0x48) = float_1_80422998;
        *(f32*)((s32)child + 0x74) = float_1_80422998;
        *(f32*)((s32)child + 0x58) = float_0_804229a4;
        *(f32*)((s32)child + 0x5C) = float_0_804229a4;
        *(s32*)((s32)child + 0x40) = 0;
        *(s32*)((s32)child + 0x80) = 0;
        *(s32*)((s32)child + 0x84) = 0;

        if (kind == 0) {
            if ((i & 1) != 0) {
                *(f32*)((s32)child + 0x4C) = float_3_80422a0c;
            } else {
                *(f32*)((s32)child + 0x4C) = float_neg3_80422a10;
            }
            *(s32*)((s32)child + 0x88) = 0x20;
        } else if (kind == 1) {
            *(f32*)((s32)child + 0x4C) = float_0_804229a4;
            *(s32*)((s32)child + 0x88) = 0x20;
        } else if (kind == 2) {
            *(f32*)((s32)child + 0x4C) = float_0_804229a4;
            *(s32*)((s32)child + 0x88) = 0x40;
        }

        i++;
        child = (void*)((s32)child + 0x8C);
    }

    return entry;
}

u8 effKemuri2Disp(s32 param_1, void* effEntry) {
    return 0;
}


u8 effKemuriDisp(s32 param_1, void* effEntry) {
    return 0;
}


u8 effKemuriMain(void* effEntry) {
    return 0;
}


u8 effKemuri2Main(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effKemuri2Disp(s32 cameraId, void* effEntry);
    extern LocalVec3 vec3_802f3704;
    extern f32 float_3p1416_804229c8;
    extern f32 float_30_804229cc;
    extern f32 float_0p9_804229d0;
    extern f32 float_0p95_804229d4;
    extern f32 float_0p8_804229d8;
    extern double sin(double x);
    extern double cos(double x);
    LocalVec3 dispPos;
    void* work;
    void* cur;
    s32 i;
    f32 wave;

    work = *(void**)((s32)effEntry + 0xC);
    dispPos = vec3_802f3704;
    dispPos.x = *(f32*)((s32)work + 0x4);
    dispPos.y = *(f32*)((s32)work + 0x8);
    dispPos.z = *(f32*)((s32)work + 0xC);

    wave = (f32)sin((float_3p1416_804229c8 * (f32)*(s32*)((s32)work + 0x3C)) / float_30_804229cc);
    *(s32*)((s32)work + 0x3C) += 1;
    if (*(s32*)((s32)work + 0x3C) > 30) {
        effDelete(effEntry);
        return 0;
    }

    cur = work;
    i = 0;
    while (i < *(s32*)((s32)effEntry + 0x8)) {
        *(f32*)((s32)cur + 0x4) += *(f32*)((s32)cur + 0x10) * *(f32*)((s32)cur + 0x30);
        *(f32*)((s32)cur + 0xC) += *(f32*)((s32)cur + 0x10) * *(f32*)((s32)cur + 0x34);
        *(f32*)((s32)cur + 0x20) *= float_0p9_804229d0;
        *(f32*)((s32)cur + 0x8) += *(f32*)((s32)cur + 0x20);
        *(f32*)((s32)cur + 0x64) *= float_0p95_804229d4;
        *(f32*)((s32)cur + 0x68) += *(f32*)((s32)cur + 0x64);
        *(f32*)((s32)cur + 0x70) = wave;
        if (*(s32*)((s32)cur + 0x3C) > 15) {
            *(s32*)((s32)cur + 0x6C) = (s32)((f32)*(s32*)((s32)cur + 0x6C) * float_0p8_804229d8);
        }
        i++;
        cur = (void*)((s32)cur + 0x8C);
    }
    dispEntry(4, 2, effKemuri2Disp, effEntry, dispCalcZ(&dispPos));
    return 0;
}

