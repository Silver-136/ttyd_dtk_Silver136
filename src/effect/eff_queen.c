#include "effect/eff_queen.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effQueenMain(void* entry);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern u32 dat_80428a50;
extern f32 float_deg2rad_80428a54;
extern f32 float_0_80428a58;
extern f32 float_10_80428a5c;
extern char str_Queen_80428a68;
extern f32 float_1_80428a70;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effQueenEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    void* work;
    void* callback;
    f32 zero;
    f32 one;
    s32 zeroInt;
    s32 minusOne;
    s32 alpha;

    *(char**)((s32)entry + 0x14) = &str_Queen_80428a68;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x2C));
    callback = effQueenMain;
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a58;
    *(void**)((s32)entry + 0x10) = callback;
    zeroInt = 0;
    minusOne = -1;
    alpha = 0xFF;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = scale;
    *(f32*)((s32)work + 0x10) = zero;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x20) = 1000;
    } else {
        *(s32*)((s32)work + 0x20) = timer;
    }
    *(s32*)((s32)work + 0x1C) = zeroInt;
    *(s32*)((s32)work + 0x18) = minusOne;
    *(s32*)((s32)work + 0x24) = zeroInt;
    *(s32*)((s32)work + 0x28) = alpha;
    one = float_1_80428a70;
    *(f32*)((s32)work + 0x10) = one;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_queen 20260624_185035 */

/* stub-fill: effQueenMain | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effQueenMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 loop);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effQueenDisp(s32 cameraId, void* entry);
    extern u32 vec3_80302b28[];
    extern char str_MOBJ_EFF_queen_torna_80302b34[];
    extern char str_S_1_80428a60;
    extern f32 float_0p8_80428a64;

    void* work;
    u32 dispPos[3];
    u32 pos[3];
    s32 rawLanguage;
    s32 language;
    s32 limit;

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302b28[0];
    pos[1] = vec3_80302b28[1];
    pos[2] = vec3_80302b28[2];
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
        *(s32*)((s32)work + 0x20) = 0x64;
    }
    if (!animGroupBaseAsync(str_MOBJ_EFF_queen_torna_80302b34, language, 0)) {
        return;
    }
    if (*(s32*)((s32)work + 0x18) == -1) {
        *(s32*)((s32)work + 0x18) = animPoseEntry(str_MOBJ_EFF_queen_torna_80302b34, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x18), &str_S_1_80428a60, 1);
    }
    if (*(s32*)((s32)work + 0x20) < 1000) {
        *(s32*)((s32)work + 0x20) -= 1;
    }
    if (*(s32*)((s32)work + 0x20) < 0) {
        if (*(s32*)((s32)work + 0x18) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x18));
        }
        effDelete(entry);
        return;
    }
    if (*(s32*)((s32)work + 0x20) < 0x64) {
        *(s32*)((s32)work + 0x24) -= 3;
    } else {
        *(s32*)((s32)work + 0x24) += 3;
    }
    limit = (s32)((f32)*(s32*)((s32)work + 0x28) * float_0p8_80428a64);
    if (*(s32*)((s32)work + 0x24) > limit) {
        *(s32*)((s32)work + 0x24) = limit;
    }
    if (*(s32*)((s32)work + 0x24) < 0) {
        *(s32*)((s32)work + 0x24) = 0;
    }
    dispEntry(4, 2, effQueenDisp, entry, dispCalcZ(dispPos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effQueenDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x18);
    void* cam;
    f32 scale;
    f32 scaleY;
    f32 angle;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scaleY = *(f32*)((s32)work + 0x14);
        scale = *(f32*)((s32)work + 0x10) * scaleY;
        PSMTXScale(scaleMtx, scale, scaleY, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428a54 * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x18), 0x40);
        tempColor = dat_80428a50;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x24);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x18), &color);
        animPoseMain(*(s32*)((s32)work + 0x18));
        animPoseDrawMtx(*(s32*)((s32)work + 0x18), trans, float_0_80428a58, 1, float_10_80428a5c);
        animPoseDrawMtx(*(s32*)((s32)work + 0x18), trans, float_0_80428a58, 2, float_10_80428a5c);
        animPoseDrawMtx(*(s32*)((s32)work + 0x18), trans, float_0_80428a58, 3, float_10_80428a5c);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

