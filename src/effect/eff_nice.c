#include "effect/eff_nice.h"


u8 acrobatMain(int* param_1) {
    return 0;
}


u8 effAcrobatDisp(void) {
    return 0;
}


u8 effNiceMain(void) {
    return 0;
}


u8 effNiceEntry(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effNiceDisp(int cameraId, int entry) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
    extern void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void* camGetPtr(s32 cameraId);
    extern void* gp;
    extern f32 float_deg2rad_80422e88;
    extern f32 float_8_80422eb8;
    extern f32 float_0p7_80422ebc;
    extern f32 float_0_80422e94;
    extern f32 float_1_80422e9c;
    extern u8 animPoseDrawMtx();
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    void* work = *(void**)((s32)entry + 0xC);
    u32 renderMode;

    PSMTXTrans(trans,
               *(f32*)((s32)work + 4),
               *(f32*)((s32)work + 8),
               *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, -*(f32*)((s32)camGetPtr(cameraId) + 0x114) * float_deg2rad_80422e88);
    PSMTXScale(scale,
               *(f32*)((s32)work + 0x1C),
               *(f32*)((s32)work + 0x1C),
               *(f32*)((s32)work + 0x1C));
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXScale(scale, float_8_80422eb8, float_8_80422eb8, float_8_80422eb8);
    PSMTXConcat(mtx, scale, mtx);

    renderMode = *(s32*)((s32)gp + 0x16C);
    if ((renderMode == 1 && *(s32*)work == 3) ||
        (renderMode == 1 && *(s32*)work == 4) ||
        (renderMode == 5 && *(s32*)work == 6)) {
        PSMTXScale(scale, float_0p7_80422ebc, float_0p7_80422ebc, float_0p7_80422ebc);
        PSMTXConcat(mtx, scale, mtx);
    }

    animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 1, float_0_80422e94, float_1_80422e9c);
    animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 2, float_0_80422e94, float_1_80422e9c);
    return animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 3, float_0_80422e94, float_1_80422e9c);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 rendermodeFunc(s32 param_1) {
    return 0;
}


void effNiceAsync(s32 a) {
}
