#include "effect/eff_sandars.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSandarsDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_80428138;
    extern f32 float_0_80428148;
    extern f32 float_4_80428150;
    extern u32 dat_80428130;

    u32 evtColor;
    u32 color;
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 mtx[3][4];
    void* work;
    void* cam;
    f32 angle;
    f32 deg;

    work = *(void**)((s32)effect + 0xC);
    camGetPtr(cameraId);
    if (*(s32*)((s32)work + 0x1C) != -1) {
        PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        deg = float_deg2rad_80428138;
        PSMTXRotRad(rotMtx, 0x79, deg * -angle);
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(transMtx, scaleMtx, mtx);
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x28), *(f32*)((s32)work + 0x28), *(f32*)((s32)work + 0x28));
        PSMTXConcat(mtx, scaleMtx, mtx);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x1C), 0x40);
        color = dat_80428130;
        ((u8*)&color)[3] = *(s32*)((s32)work + 0x20);
        evtColor = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x1C), &evtColor);
        animPoseMain(*(s32*)((s32)work + 0x1C));
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 1, float_0_80428148, float_4_80428150);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 2, float_0_80428148, float_4_80428150);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 3, float_0_80428148, float_4_80428150);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
