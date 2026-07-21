#include "effect/eff_irekaeru.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effIrekaeruEntry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effIrekaeruMain(void);
    extern char str_Irekaeru_80300b50[];
    extern f32 float_1_804281ac;

    void* effect;
    void* work;
    f32 one;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = str_Irekaeru_80300b50;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x24);
    *(void**)((s32)effect + 0xC) = work;
    one = float_1_804281ac;
    *(void**)((s32)effect + 0x10) = effIrekaeruMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = one;
    *(s32*)((s32)work + 0x14) = -1;
    *(s32*)((s32)work + 0x18) = 0;
    *(s32*)((s32)work + 0x1C) = 0;
    *(s32*)((s32)work + 0x20) = 0;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* CHATGPT STUB FILL: main/effect/eff_irekaeru 20260624_184929 */

/* stub-fill: effIrekaeruMain | prototype_only | source_prototype */
void effIrekaeruMain(void* effect) {
    extern void* gp;
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern s32 animGroupBaseAsync(const char* name, s32 heap, s32 flags);
    extern s32 animPoseEntry(const char* name, s32 heap);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effIrekaeruDisp(s32 cameraId, void* effect);
    extern char* eff_irekae_str_tbl[];
    extern char str_Z_1_804281a8[];
    extern f32 float_1_804281ac;
    extern f32 float_0p75_804281b0;

    void* work;
    f32 pos[3];
    s32 heap;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    heap = (*(s32*)((s32)gp + 0x14) != 0);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        if (*(s32*)((s32)work + 0x14) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x14));
        }
        effDelete(effect);
        return;
    }

    switch (*(s32*)((s32)work + 0x18)) {
        case 0:
            if (animGroupBaseAsync(eff_irekae_str_tbl[*(s32*)((s32)gp + 0x16C)], heap, 0) == 0) {
                return;
            }
            *(s32*)((s32)work + 0x14) = animPoseEntry(eff_irekae_str_tbl[*(s32*)((s32)gp + 0x16C)], heap);
            animPoseSetAnim(*(s32*)((s32)work + 0x14), str_Z_1_804281a8, 1);
            *(s32*)((s32)work + 0x18) += 1;
            break;
        case 1:
            if (animPoseGetLoopTimes(*(s32*)((s32)work + 0x14)) > float_1_804281ac) {
                animPoseRelease(*(s32*)((s32)work + 0x14));
                effDelete(effect);
                return;
            }
            break;
    }

    if (animPoseGetLoopTimes(*(s32*)((s32)work + 0x14)) < float_0p75_804281b0) {
        *(s32*)((s32)work + 0x20) += 0x14;
        if (*(s32*)((s32)work + 0x20) > 0xFF) {
            *(s32*)((s32)work + 0x20) = 0xFF;
        }
    } else {
        *(s32*)((s32)work + 0x20) -= 0x14;
        if (*(s32*)((s32)work + 0x20) < 0) {
            *(s32*)((s32)work + 0x20) = 0;
        }
    }

    dispEntry(4, 2, effIrekaeruDisp, dispCalcZ(pos), effect);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effIrekaeruDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_8042819c;
    extern f32 float_0_804281a0;
    extern f32 float_2_804281a4;
    extern u32 dat_80428198;

    u32 evtColor;
    u32 color;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotMtx[3][4];
    void* work;
    void* cam;
    f32 angle;
    f32 deg;

    work = *(void**)((s32)effect + 0xC);
    if (*(s32*)((s32)work + 0x14) != -1) {
        PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_8042819c;
        PSMTXRotRad(rotMtx, 0x79, deg * angle);
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x14), 0x40);
        color = dat_80428198;
        ((u8*)&color)[3] = *(s32*)((s32)work + 0x20);
        evtColor = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x14), &evtColor);
        animPoseMain(*(s32*)((s32)work + 0x14));
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 1, float_0_804281a0, float_2_804281a4);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 2, float_0_804281a0, float_2_804281a4);
        animPoseDrawMtx(*(s32*)((s32)work + 0x14), transMtx, 3, float_0_804281a0, float_2_804281a4);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

