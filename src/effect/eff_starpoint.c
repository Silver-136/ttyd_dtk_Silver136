#include "effect/eff_starpoint.h"
void* effStarPointEntry(double x, double y, double z, s32 kind, s32 count) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 rand(void);
    extern const char str_StarPoint_802fe534[];
    extern void effStarPointMain(void);
    extern f64 double_to_int_802fe540;
    extern f32 float_0_80426f0c;
    extern f32 float_6p2832_80426f10;
    extern f32 float_360_80426f14;
    extern f32 float_0p00761_80426f18;
    extern f32 float_0p16605_80426f1c;
    extern f32 float_1_80426f20;
    extern f32 float_3p1416_80426f24;
    extern f32 float_1p5708_80426f28;
    extern f32 float_4p7124_80426f2c;
    extern f32 float_2_80426f30;
    extern f32 float_0p03705_80426f34;
    extern f32 float_0p4967_80426f38;
    extern f32 float_14_80426f3c;

    void* entry;
    s32* work;
    s32* child;
    s32 i;
    s32 nextDelay;
    s32 r;
    s32 q;
    s32 deg;
    f32 angle;
    f32 s;
    f32 c;
    f32 t;
    f32 t2;
    f32 zero;
    f32 pSinA;
    f32 pSinB;
    f32 one;
    f32 pi;
    f32 halfPi;
    f32 threeHalfPi;
    f32 two;
    f32 pCosA;
    f32 pCosB;
    f32 fourteen;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarPoint_802fe534;
    *(s32*)((s32)entry + 0x8) = count + 1;

    work = (s32*)__memAlloc(3, *(s32*)((s32)entry + 0x8) * 0x34);
    *(s32**)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = effStarPointMain;

    work[0] = kind;
    *(f32*)((s32)work + 0xC) = (f32)x;
    *(f32*)((s32)work + 0x10) = (f32)y;
    *(f32*)((s32)work + 0x14) = (f32)z;
    work[1] = 0x3C;
    work[2] = 0;

    zero = float_0_80426f0c;
    pSinA = float_0p00761_80426f18;
    pSinB = float_0p16605_80426f1c;
    one = float_1_80426f20;
    pi = float_3p1416_80426f24;
    halfPi = float_1p5708_80426f28;
    threeHalfPi = float_4p7124_80426f2c;
    two = float_2_80426f30;
    pCosA = float_0p03705_80426f34;
    pCosB = float_0p4967_80426f38;
    fourteen = float_14_80426f3c;

    child = (s32*)((s32)work + 0x34);
    nextDelay = 4;
    i = 1;

    while (i < *(s32*)((s32)entry + 0x8)) {
        r = rand();

        *(f32*)((s32)child + 0xC) = zero;
        *(f32*)((s32)child + 0x10) = zero;
        *(f32*)((s32)child + 0x14) = zero;

        q = r / 360;
        if (r < 0 && (r - q * 360) != 0) {
            q--;
        }
        deg = r - q * 360;

        angle = (float)deg;
        angle = (float_6p2832_80426f10 * angle) / float_360_80426f14;

        if (angle <= pi) {
            if (angle <= halfPi) {
                t = angle;
                s = ((pSinA * t * t - pSinB) * t * t + one) * t;
            } else {
                t = halfPi - (angle - halfPi);
                s = ((pSinA * t * t - pSinB) * t * t + one) * t;
            }
        } else if (angle < threeHalfPi) {
            t = angle - pi;
            s = -(((pSinA * t * t - pSinB) * t * t + one) * t);
        } else {
            t = halfPi - (angle - threeHalfPi);
            s = -(((pSinA * t * t - pSinB) * t * t + one) * t);
        }

        *(f32*)((s32)child + 0x18) = two * s;

        if (angle <= pi) {
            if (angle <= halfPi) {
                t = angle;
                t2 = t * t;
                c = (pCosA * t2 - pCosB) * t2 + one;
            } else {
                t = halfPi - (angle - halfPi);
                t2 = t * t;
                c = -((pCosA * t2 - pCosB) * t2 + one);
            }
        } else if (angle < threeHalfPi) {
            t = angle - pi;
            t2 = t * t;
            c = -((pCosA * t2 - pCosB) * t2 + one);
        } else {
            t = halfPi - (angle - threeHalfPi);
            t2 = t * t;
            c = (pCosA * t2 - pCosB) * t2 + one;
        }

        *(f32*)((s32)child + 0x1C) = two * c;
        *(f32*)((s32)child + 0x20) = fourteen;
        *(f32*)((s32)child + 0x24) = two;
        child[0xB] = 0;
        child[0xC] = nextDelay;

        nextDelay += 3;
        i++;
        child = (s32*)((s32)child + 0x34);
    }

    return entry;
}

void effStarPointMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern u32 vec3_802fe528[];
    extern f32 float_0_80426f0c;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(Vec* pos);
    extern void effStarPointDisp(void* camera, void* effect);
    extern void dispEntry(s32 cameraId, s32 order, void* dispFunc, void* effect, f32 z);

    s32* work;
    s32* child;
    Vec base;
    Vec pos;
    s32 i;
    s32 alpha;
    f32 zero;
    f32 zOrder;

    work = *(s32**)((s32)effect + 0xC);

    base.x = ((f32*)vec3_802fe528)[0];
    base.y = ((f32*)vec3_802fe528)[1];
    base.z = ((f32*)vec3_802fe528)[2];

    base.x = *(f32*)((s32)work + 0xC);
    base.y = *(f32*)((s32)work + 0x10);
    base.z = *(f32*)((s32)work + 0x14);

    pos.x = base.x;
    pos.y = base.y;
    pos.z = base.z;

    work[1] = work[1] - 1;
    work[2] = work[2] + 1;

    if (work[1] < 0) {
        effDelete(effect);
        return;
    }

    i = 1;
    child = (s32*)((s32)work + 0x34);
    zero = float_0_80426f0c;

    while (i < *(s32*)((s32)effect + 8)) {
        if (child[0xC] != 0) {
            child[0xC] = child[0xC] - 1;
            child[0xB] = 0;
        } else {
            *(f32*)((s32)child + 0x20) =
                *(f32*)((s32)child + 0x20) - *(f32*)((s32)child + 0x24);

            *(f32*)((s32)child + 0xC) =
                *(f32*)((s32)child + 0xC) + *(f32*)((s32)child + 0x18);

            *(f32*)((s32)child + 0x10) =
                *(f32*)((s32)child + 0x10) + *(f32*)((s32)child + 0x20);

            *(f32*)((s32)child + 0x14) =
                *(f32*)((s32)child + 0x14) + *(f32*)((s32)child + 0x1C);

            if (*(f32*)((s32)child + 0x20) < zero) {
                alpha = child[0xB] - 2;
                child[0xB] = alpha;
                if (child[0xB] < 0) {
                    child[0xB] = 0;
                }
            } else {
                child[0xB] = 0xFF;
            }
        }

        i++;
        child = (s32*)((s32)child + 0x34);
    }

    zOrder = dispCalcZ(&pos);
    dispEntry(4, 1, effStarPointDisp, effect, zOrder);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effStarPointDisp(void* camera, void* effect) {
    f32 base[12];
    f32 rot[12];
    f32 mtx[12];
    void* work;
    s32 i;

    extern f32 float_deg2rad_80426f08;
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(void* m, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void iconDispGx2(void* mtx, s32 icon, s32 color);

    work = *(void**)((s32)effect + 0xC);
    PSMTXTrans(base, *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14));
    PSMTXRotRad(rot, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114)) * float_deg2rad_80426f08);
    PSMTXConcat(base, rot, base);

    i = 1;
    work = (void*)((s32)work + 0x34);
    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)((s32)work + 0x2C) != 0) {
            PSMTXTrans(mtx, *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14));
            PSMTXConcat(base, mtx, mtx);
            iconDispGx2(mtx, 0, 0x194);
        }
        i++;
        work = (void*)((s32)work + 0x34);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
