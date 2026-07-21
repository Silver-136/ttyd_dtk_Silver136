#include "effect/eff_scanning.h"
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
void effScanningMain(void* effect) {
    extern void* gp;
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern s32 animGroupBaseAsync(const char* name, s32 heap, s32 flags);
    extern s32 animPoseEntry(const char* name, s32 heap);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effScanningDisp(s32 cameraId, void* effect);
    extern char str_EFF_Scan_80300bb4[];
    extern char str_Z_1_804281f4[];
    extern f32 float_1_804281f8;

    void* work;
    f32 pos[3];
    s32 heap;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    heap = (*(s32*)((s32)gp + 0x14) != 0);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        if (*(s32*)((s32)work + 0x14) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x14));
        }
        effDelete(effect);
        return;
    }

    switch (*(s32*)((s32)work + 0x18)) {
        case 0:
            if (animGroupBaseAsync(str_EFF_Scan_80300bb4, heap, 0) == 0) {
                return;
            }
            *(s32*)((s32)work + 0x14) = animPoseEntry(str_EFF_Scan_80300bb4, heap);
            animPoseSetAnim(*(s32*)((s32)work + 0x14), str_Z_1_804281f4, 1);
            *(s32*)((s32)work + 0x18) += 1;
            break;
        case 1:
            if (animPoseGetLoopTimes(*(s32*)((s32)work + 0x14)) > float_1_804281f8) {
                animPoseRelease(*(s32*)((s32)work + 0x14));
                effDelete(effect);
                return;
            }
            break;
    }

    dispEntry(4, 2, effScanningDisp, dispCalcZ(pos), effect);
}

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

