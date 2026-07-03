#include "effect/eff_uranoko.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effUranokoMain(void* entry);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern char str_Uranoko_80302a10[];
extern u32 dat_804288c0;
extern f32 float_deg2rad_804288c4;
extern f32 float_0_804288c8;
extern f32 float_6_804288cc;
extern f32 float_1_804288d4;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effUranokoDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x20);
    void* cam;
    f32 scale;
    f32 angle;
    f32 deg;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x10) * *(f32*)((s32)work + 0x14);
        PSMTXScale(scaleMtx, scale, scale, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_804288c4;
        PSMTXRotRad(rot, 0x79, deg * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x20), 0x40);
        tempColor = dat_804288c0;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x24);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x20), &color);
        animPoseMain(*(s32*)((s32)work + 0x20));
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_804288c8, 1, float_6_804288cc);
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_804288c8, 2, float_6_804288cc);
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_804288c8, 3, float_6_804288cc);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUranokoEntry(s32 type, s32 timer, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;
    void* callback;
    f32 scale1;
    f32 scale0;
    s32 zero;

    *(char**)((s32)entry + 0x14) = str_Uranoko_80302a10;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x28));
    callback = effUranokoMain;
    scale1 = float_1_804288d4;
    scale0 = float_0_804288c8;
    zero = 0;
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = callback;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = scale1;
    *(f32*)((s32)work + 0x10) = scale0;
    *(s32*)((s32)work + 0x18) = zero;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(s32*)((s32)work + 0x24) = 0xFF;
    *(s32*)((s32)work + 0x20) = -1;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_uranoko 20260624_184929 */

/* stub-fill: effUranokoMain | prototype_only | source_prototype */
void effUranokoMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, const char* animName, s32 loop);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void effUranokoDisp(s32 cameraId, void* entry);
    extern char str_MOBJ_EFF_uranoko_803029fc[];
    extern const char str_A_1_804288d0[];
    extern f32 float_1_804288d4;
    extern f32 float_0p125_804288d8;

    void* work;
    Vec pos;
    s32 language;

    work = *(void**)((s32)entry + 0xC);
    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    language = *(s32*)((s32)gp + 0x14) != 0;

    if (animGroupBaseAsync(str_MOBJ_EFF_uranoko_803029fc, language, 0)) {
        if (*(s32*)((s32)work + 0x20) == -1) {
            *(s32*)((s32)work + 0x20) = animPoseEntry(str_MOBJ_EFF_uranoko_803029fc, language);
            animPoseSetAnim(*(s32*)((s32)work + 0x20), str_A_1_804288d0, 1);
        }
        if (*(u32*)entry & 4) {
            *(u32*)entry &= ~4;
            *(s32*)((s32)work + 0x1C) = 0x10;
        }
        if (*(s32*)((s32)work + 0x1C) < 1000) {
            *(s32*)((s32)work + 0x1C) -= 1;
        }
        if (*(s32*)((s32)work + 0x1C) < 0x10) {
            *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x1C) << 4;
        }
        if (*(s32*)((s32)work + 0x1C) < 0) {
            if (*(s32*)((s32)work + 0x20) != -1) {
                animPoseRelease(*(s32*)((s32)work + 0x20));
            }
            effDelete(entry);
        } else {
            *(s32*)((s32)work + 0x18) += 1;
            *(f32*)((s32)work + 0x10) += (float_1_804288d4 - *(f32*)((s32)work + 0x10)) * float_0p125_804288d8;
            dispCalcZ(&pos);
            dispEntry(4, 2, effUranokoDisp, 0.0f, entry);
        }
    }
}

