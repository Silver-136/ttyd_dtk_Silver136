#include "effect/eff_sheep.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSheepDisp(s32 cameraId, void* effect) {
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_0_80427ea8;
    extern f32 float_10_80427eac;

    f32 mtx[3][4];
    f32 scaleMtx[3][4];
    f32 base[3][4];
    void* work;
    s32 i;
    void* part;

    work = *(void**)((s32)effect + 0xC);
    PSMTXTrans(base, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));

    i = 1;
    part = (void*)((s32)work + 0x34);
    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)((s32)part + 0x20) != -1) {
            PSMTXTrans(mtx, *(f32*)((s32)part + 4), *(f32*)((s32)part + 8), *(f32*)((s32)part + 0xC));
            PSMTXScale(scaleMtx, *(f32*)((s32)part + 0x10), *(f32*)((s32)part + 0x10), *(f32*)((s32)part + 0x10));
            PSMTXConcat(mtx, scaleMtx, mtx);
            PSMTXConcat(base, mtx, mtx);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 1, float_0_80427ea8, float_10_80427eac);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 2, float_0_80427ea8, float_10_80427eac);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 3, float_0_80427ea8, float_10_80427eac);
        }
        i++;
        part = (void*)((s32)part + 0x34);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
