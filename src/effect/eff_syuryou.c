#include "effect/eff_syuryou.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effSyuryouMain(void* entry);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern char str_Syuryou_803029e4[];
extern u32 dat_804288a0;
extern f32 float_deg2rad_804288a4;
extern f32 float_0_804288a8;
extern f32 float_10_804288ac;
extern f32 float_1_804288b4;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSyuryouDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x14);
    void* cam;
    f32 scale;
    f32 angle;
    f32 deg;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_804288a4;
        PSMTXRotRad(rot, 0x79, deg * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x14), 0x40);
        tempColor = dat_804288a0;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x18);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x14), &color);
        animPoseMain(*(s32*)((s32)work + 0x14));
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804288a8, 1, float_10_804288ac);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804288a8, 2, float_10_804288ac);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), trans, float_0_804288a8, 3, float_10_804288ac);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSyuryouEntry(s32 type, s32 timer, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;

    *(char**)((s32)entry + 0x14) = str_Syuryou_803029e4;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x24));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSyuryouMain;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = float_1_804288b4;
    *(s32*)((s32)work + 0x18) = 0xFF;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(s32*)((s32)work + 0x18) = 0xFF;
    *(s32*)((s32)work + 0x14) = -1;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
