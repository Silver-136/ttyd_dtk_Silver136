#include "effect/eff_majinai.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMajinaiMain(void* entry);
void* mapGetWork(void);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern u32 dat_80428b10;
extern u32 dat_80428b1c;
extern f32 float_deg2rad_80428b20;
extern f32 float_0_80428b24;
extern f32 float_10_80428b28;
extern f32 float_1_80428b3c;
extern char str_Majinai_80302fa0[];

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMajinaiDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x1C);
    void* cam;
    f32 scale;
    f32 angle;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x10);
        PSMTXScale(scaleMtx, scale, scale, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428b20 * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x1C), 0x840);
        tempColor = dat_80428b1c;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x14);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x1C), &color);
        animPoseMain(*(s32*)((s32)work + 0x1C));
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 1, float_10_80428b28);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 2, float_10_80428b28);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), trans, float_0_80428b24, 3, float_10_80428b28);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMajinaiEntry(f32 x, f32 y, f32 z, s32 type) {
    s32 typeCopy;
    void* entry;
    void* work;
    void* callback;
    void* mapWork;
    f32 scale;
    s32 alpha;
    s32 zero;
    s32 minusOne;

    typeCopy = type;
    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Majinai_80302fa0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) << 5));
    callback = effMajinaiMain;
    *(void**)((s32)entry + 0xC) = work;
    scale = float_1_80428b3c;
    *(void**)((s32)entry + 0x10) = callback;
    alpha = 0xFF;
    zero = 0;
    minusOne = -1;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = typeCopy;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x14) = alpha;
    *(s32*)((s32)work + 0x18) = zero;
    *(s32*)((s32)work + 0x1C) = minusOne;
    mapWork = mapGetWork();
    *(u32*)((s32)mapWork + 0x1E) = dat_80428b10;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_majinai 20260624_185035 */

/* stub-fill: effMajinaiMain | prototype_only | source_prototype */
void effMajinaiMain(void* entry) {
    return;
}
