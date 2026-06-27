#include "effect/eff_batten.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBattenDisp2(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_80427ee8;
    extern f32 float_0_80427eec;
    extern f32 float_2_80427ef0;
    extern u32 dat_80427ee4;

    u32 evtColor;
    u32 color;
    f32 mtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    void* work;
    void* cam;
    f32 deg;
    f32 angle;

    work = *(void**)((s32)effect + 0xC);
    if (*(s32*)((s32)work + 0x24) != -1) {
        PSMTXTrans(mtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_80427ee8;
        PSMTXRotRad(rotMtx, 0x79, deg * angle);
        PSMTXConcat(mtx, rotMtx, mtx);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXConcat(mtx, (f32(*)[4])((s32)work + 0x28), mtx);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x24), 0x40);
        color = dat_80427ee4;
        ((u8*)&color)[3] = *(u8*)((s32)work + 0x14);
        evtColor = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x24), &evtColor);
        animPoseMain(*(s32*)((s32)work + 0x24));
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 1, float_0_80427eec, float_2_80427ef0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 2, float_0_80427eec, float_2_80427ef0);
        animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 3, float_0_80427eec, float_2_80427ef0);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBattenEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effBattenMain(void);
    extern void PSMTXIdentity(f32 mtx[3][4]);
    extern const char str_Batten_80300398[];
    extern f32 float_0_80427eec;
    extern f32 float_3_80427f14;

    void* effect;
    void* work;
    void* child;
    s32 count;
    s32 i;
    s32 zero;
    s32 wait;
    f32 childScale;
    f32 zeroF;

    effect = effEntry();
    count = 1;
    if (type == 0) {
        count = 4;
    }
    *(const char**)((s32)effect + 0x14) = str_Batten_80300398;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x5C);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effBattenMain;
    *(u32*)effect |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(u8*)((s32)work + 0x14) = 0xFF;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(s32*)((s32)work + 0x24) = -1;
    PSMTXIdentity((f32(*)[4])((s32)work + 0x28));

    zeroF = float_0_80427eec;
    child = (void*)((s32)work + 0x5C);
    childScale = float_3_80427f14;
    i = 1;
    zero = 0;
    while (i < *(s32*)((s32)effect + 8)) {
        *(f32*)((s32)child + 4) = zeroF;
        wait = (i - 1) * 10;
        i++;
        *(f32*)((s32)child + 8) = zeroF;
        *(f32*)((s32)child + 0xC) = zeroF;
        *(u8*)((s32)child + 0x14) = zero;
        *(f32*)((s32)child + 0x10) = childScale;
        *(s32*)((s32)child + 0x18) = zero;
        *(s32*)((s32)child + 0x1C) = wait;
        child = (void*)((s32)child + 0x5C);
    }
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_batten 20260624_184929 */

/* stub-fill: effBattenDisp | missing_definition | ghidra_signature */
u8 effBattenDisp(void) {
    return 0;
}

/* stub-fill: effBattenMain | prototype_only | source_prototype */
void effBattenMain(void) {
    return;
}
