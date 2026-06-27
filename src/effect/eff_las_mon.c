#include "effect/eff_las_mon.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effLasMonMain(void* entry);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern char str_LasMon_80302ab8[];
extern u32 dat_804289b0;
extern f32 float_0_804289b4;
extern f32 float_10_804289b8;
extern f32 float_1_804289c0;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effLasMonDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x14);
    f32 scale;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x14), 0x40);
        tempColor = dat_804289b0;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x1C);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x14), &color);
        animPoseMain(*(s32*)((s32)work + 0x14));
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804289b4, 1, float_10_804289b8);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804289b4, 2, float_10_804289b8);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804289b4, 3, float_10_804289b8);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma use_lmw_stmw off
void* effLasMonEntry(s32 type, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;
    f32 scale;
    s32 zero;
    s32 minusOne;
    s32 alpha;
    s32 timer;

    *(char**)((s32)entry + 0x14) = str_LasMon_80302ab8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x24));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effLasMonMain;
    scale = float_1_804289c0;
    zero = 0;
    minusOne = -1;
    alpha = 0xFF;
    timer = 1000;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x18) = zero;
    *(s32*)((s32)work + 0x14) = minusOne;
    *(s32*)((s32)work + 0x1C) = alpha;
    *(s32*)((s32)work + 0x20) = timer;

    return entry;
}
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_las_mon 20260624_184929 */

/* stub-fill: effLasMonMain | prototype_only | source_prototype */
void effLasMonMain(void* entry) {
    return;
}
