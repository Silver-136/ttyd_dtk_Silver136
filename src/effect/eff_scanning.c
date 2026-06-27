#include "effect/eff_scanning.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effScanningDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_804281e8;
    extern f32 float_0_804281ec;
    extern f32 float_10_804281f0;

    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    void* work;
    void* cam;
    f32 angle;
    f32 deg;

    work = *(void**)((s32)effect + 0xC);
    if (*(s32*)((s32)work + 0x14) != -1) {
        PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_804281e8;
        PSMTXRotRad(rotMtx, 0x79, deg * angle);
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        animPoseMain(*(s32*)((s32)work + 0x14));
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 1, float_0_804281ec, float_10_804281f0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 2, float_0_804281ec, float_10_804281f0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 3, float_0_804281ec, float_10_804281f0);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effScanningEntry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effScanningMain(void);
    extern char str_Scanning_80300bc0[];
    extern f32 float_1_804281f8;

    void* effect;
    void* work;
    f32 one;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = str_Scanning_80300bc0;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x24);
    *(void**)((s32)effect + 0xC) = work;
    one = float_1_804281f8;
    *(void**)((s32)effect + 0x10) = effScanningMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = one;
    *(s32*)((s32)work + 0x14) = -1;
    *(s32*)((s32)work + 0x18) = 0;
    *(s32*)((s32)work + 0x1C) = 0;
    *(s32*)((s32)work + 0x20) = 0xFF;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_scanning 20260624_184929 */

/* stub-fill: effScanningMain | prototype_only | source_prototype */
void effScanningMain(void) {
    return;
}
