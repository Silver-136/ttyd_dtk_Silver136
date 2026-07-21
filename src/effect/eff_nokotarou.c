#include "effect/eff_nokotarou.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effNokotarouMain(void* entry);

extern char str_Nokotarou_80302b18[];
extern f32 float_0_80428a34;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effNokotarouEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field1C) {
    void* entry = effEntry();
    void* work;
    f32 zero;
    s32 alpha;

    *(char**)((s32)entry + 0x14) = str_Nokotarou_80302b18;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a34;
    *(void**)((s32)entry + 0x10) = effNokotarouMain;
    alpha = 0xFF;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = *(f32*)((s32)work + 4);
    *(f32*)((s32)work + 0x14) = *(f32*)((s32)work + 8);
    *(f32*)((s32)work + 0x18) = *(f32*)((s32)work + 0xC);
    *(f32*)((s32)work + 0x1C) = field1C;
    *(f32*)((s32)work + 0x20) = zero;
    *(s32*)((s32)work + 0x24) = alpha;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }
    *(s32*)((s32)work + 0x30) = -1;
    *(s32*)((s32)work + 0x34) = -1;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* stub-fill: effNokotarouMain | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effNokotarouMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 loop);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effNokotarouDisp(s32 cameraId, void* entry);
    extern u32 vec3_80302af8[];
    extern char str_MOBJ_EFF_nokotarou3_80302b04[];
    extern char str_S_1_80428a44;
    extern char str_A_1_80428a48;
    extern f32 float_1_80428a40;
    extern f32 float_0p03125_80428a4c;

    void* work;
    u32 dispPos[3];
    u32 pos[3];
    s32 rawLanguage;
    s32 language;
    f32 step;
    f32 cur;
    f32 one;

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302af8[0];
    pos[1] = vec3_80302af8[1];
    pos[2] = vec3_80302af8[2];
    *(f32*)&pos[0] = *(f32*)((s32)work + 0x10);
    *(f32*)&pos[1] = *(f32*)((s32)work + 0x14);
    *(f32*)&pos[2] = *(f32*)((s32)work + 0x18);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];
    rawLanguage = *(s32*)((s32)gp + 0x14);
    language = (s32)(((u32)(-rawLanguage | rawLanguage)) >> 0x1F);

    if (!animGroupBaseAsync(str_MOBJ_EFF_nokotarou3_80302b04, language, 0)) {
        return;
    }
    if (*(s32*)((s32)work + 0x30) == -1) {
        *(s32*)((s32)work + 0x30) = animPoseEntry(str_MOBJ_EFF_nokotarou3_80302b04, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x30), &str_S_1_80428a44, 1);
        *(s32*)((s32)work + 0x34) = animPoseEntry(str_MOBJ_EFF_nokotarou3_80302b04, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x34), &str_A_1_80428a48, 1);
    }
    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        *(s32*)((s32)work + 0x28) = 0x20;
    }
    if (*(s32*)((s32)work + 0x28) < 1000) {
        *(s32*)((s32)work + 0x28) -= 1;
    }
    if (*(s32*)((s32)work + 0x28) < 0) {
        if (*(s32*)((s32)work + 0x30) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x30));
        }
        if (*(s32*)((s32)work + 0x34) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x34));
        }
        effDelete(entry);
        return;
    }
    if (*(s32*)((s32)work + 0x28) < 0x20) {
        step = float_0p03125_80428a4c;
        *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x28) << 3;
        cur = *(f32*)((s32)work + 0x20);
        *(f32*)((s32)work + 0x20) = cur + (-cur * step);
    } else {
        one = float_1_80428a40;
        cur = *(f32*)((s32)work + 0x20);
        step = float_0p03125_80428a4c;
        *(f32*)((s32)work + 0x20) = cur + ((one - cur) * step);
    }
    dispEntry(4, 2, effNokotarouDisp, entry, dispCalcZ(dispPos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_nokotarou 20260624_185035 */

/* stub-fill: effNokotarouDisp | missing_definition | ghidra_signature */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effNokotarouDisp(s32 cameraId, void* entry) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void* camGetPtr(s32 cameraId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);
    extern u32 dat_80428a28;
    extern u32 dat_80428a2c;
    extern f32 float_deg2rad_80428a30;
    extern f32 float_0_80428a34;
    extern f32 float_2_80428a38;
    extern f32 float_100_80428a3c;
    extern f32 float_1_80428a40;

    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    f32 baseScale = *(f32*)((s32)work + 0x20) * *(f32*)((s32)work + 0x1C);
    void* cam;
    f32 angle;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;

    camGetPtr(cameraId);
    if (*(s32*)((s32)work + 0x30) != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14), *(f32*)((s32)work + 0x18));
        PSMTXScale(scaleMtx, baseScale, baseScale, baseScale);
        cam = camGetPtr(cameraId);
        angle = -*(f32*)((s32)cam + 0x114);
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428a30 * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x30), 0x40);
        tempColor = dat_80428a28;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x24);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x30), &color);
        animPoseMain(*(s32*)((s32)work + 0x30));
        animPoseDrawMtx(*(s32*)((s32)work + 0x30), trans, float_0_80428a34, 1, float_2_80428a38);
        animPoseDrawMtx(*(s32*)((s32)work + 0x30), trans, float_0_80428a34, 2, float_2_80428a38);
        animPoseDrawMtx(*(s32*)((s32)work + 0x30), trans, float_0_80428a34, 3, float_2_80428a38);
    }

    if (*(s32*)((s32)work + 0x34) != -1) {
        scaleX = (((*(f32*)((s32)work + 0x10) - *(f32*)((s32)work + 4)) / float_100_80428a3c) * *(f32*)((s32)work + 0x1C));
        scaleY = (float_1_80428a40 + ((*(f32*)((s32)work + 0x14) - *(f32*)((s32)work + 8)) / float_100_80428a3c)) * *(f32*)((s32)work + 0x1C);
        scaleZ = (float_1_80428a40 + ((*(f32*)((s32)work + 0x18) - *(f32*)((s32)work + 0xC)) / float_100_80428a3c)) * *(f32*)((s32)work + 0x1C);
        PSMTXTrans(trans, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14), *(f32*)((s32)work + 0x18));
        PSMTXScale(scaleMtx, scaleX, scaleY, scaleZ);
        cam = camGetPtr(cameraId);
        angle = -*(f32*)((s32)cam + 0x114);
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428a30 * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x34), 0x40);
        tempColor = dat_80428a2c;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x24);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x34), &color);
        animPoseMain(*(s32*)((s32)work + 0x34));
        animPoseDrawMtx(*(s32*)((s32)work + 0x34), trans, float_0_80428a34, 1, float_2_80428a38);
        animPoseDrawMtx(*(s32*)((s32)work + 0x34), trans, float_0_80428a34, 2, float_2_80428a38);
        animPoseDrawMtx(*(s32*)((s32)work + 0x34), trans, float_0_80428a34, 3, float_2_80428a38);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

