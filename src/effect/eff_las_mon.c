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
#pragma use_lmw_stmw off
void* effLasMonEntry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effLasMonMain(void* entry);
    extern char str_LasMon_80302ab8[];
    extern f32 float_1_804289c0;

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
    scale = float_1_804289c0;
    *(void**)((s32)entry + 0x10) = effLasMonMain;
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effLasMonMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 loop);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effLasMonDisp(s32 cameraId, void* entry);
    extern u32 vec3_80302a98[];
    extern char str_MOBJ_EFF_las_mon_80302aa4[];
    extern char str_A_1_804289bc;
    extern char str_S_1_804289c4;
    extern f32 float_1_804289c0;

    void* work;
    u32 dispPos[3];
    u32 pos[3];
    s32 rawLanguage;
    s32 language;

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302a98[0];
    pos[1] = vec3_80302a98[1];
    pos[2] = vec3_80302a98[2];
    *(f32*)&pos[0] = *(f32*)((s32)work + 4);
    *(f32*)&pos[1] = *(f32*)((s32)work + 8);
    *(f32*)&pos[2] = *(f32*)((s32)work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];
    rawLanguage = *(s32*)((s32)gp + 0x14);
    language = (s32)(((u32)(-rawLanguage | rawLanguage)) >> 0x1F);

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        *(s32*)((s32)work + 0x20) = 0x10;
    }
    if (*(s32*)((s32)work + 0x20) < 1000) {
        *(s32*)((s32)work + 0x20) -= 1;
    }
    if (*(s32*)((s32)work + 0x20) < 0x10) {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x20) << 4;
    }
    if (*(s32*)((s32)work + 0x20) < 0) {
        if (*(s32*)((s32)work + 0x14) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x14));
        }
        effDelete(entry);
        return;
    }

    if (!animGroupBaseAsync(str_MOBJ_EFF_las_mon_80302aa4, language, 0)) {
        return;
    }
    if (*(s32*)((s32)work + 0x14) == -1) {
        *(s32*)((s32)work + 0x14) = animPoseEntry(str_MOBJ_EFF_las_mon_80302aa4, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x14), &str_A_1_804289bc, 1);
        *(s32*)((s32)work + 0x18) = 0;
    }

    switch (*(s32*)((s32)work + 0x18)) {
        case 0:
            if (animPoseGetLoopTimes(*(s32*)((s32)work + 0x14)) >= float_1_804289c0) {
                animPoseSetAnim(*(s32*)((s32)work + 0x14), &str_S_1_804289c4, 1);
                *(s32*)((s32)work + 0x18) += 1;
            }
            break;
        case 1:
            break;
    }

    dispEntry(4, 2, effLasMonDisp, entry, dispCalcZ(dispPos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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
