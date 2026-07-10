#include "effect/eff_treasure_map.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTreasureMapEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effTreasureMapMain(void);
    extern const char str_Treasure_Map_802ff078[];
    extern f32 float_1_80427940;
    extern f32 float_0_80427934;

    void* entry = effEntry();
    void* work;

    *(const char**)((s32)entry + 0x14) = str_Treasure_Map_802ff078;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x2C);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effTreasureMapMain;
    *(u32*)entry |= 2;
    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x18) = 1000;
    *(s32*)((s32)work + 0x14) = 0xFF;
    *(f32*)((s32)work + 0x10) = float_1_80427940;
    *(f32*)((s32)work + 0x20) = float_0_80427934;
    *(s32*)((s32)work + 0x24) = -1;
    *(s32*)((s32)work + 0x28) = 4;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_treasure_map 20260624_184823 */

/* stub-fill: effTreasureMapDisp | missing_definition | ghidra_signature */
void effTreasureMapDisp(s32 cameraId, void* effect) {
    typedef f32 MtxLocal[3][4];

    extern void camGetPtr(s32 cameraId);
    extern void PSMTXTrans(MtxLocal mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(MtxLocal mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(MtxLocal mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(MtxLocal a, MtxLocal b, MtxLocal out);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, MtxLocal mtx, s32 mode, f32 rot, f32 scale);
    extern u32 dat_80427920;
    extern u32 dat_80427924;
    extern f32 float_1p1_80427928;
    extern f32 float_1p06_8042792c;
    extern f32 float_deg2rad_80427930;
    extern f32 float_0_80427934;
    extern f32 float_10_80427938;

    void* work = *(void**)((s32)effect + 0xC);
    MtxLocal trans;
    MtxLocal scale;
    MtxLocal rot;
    MtxLocal mtx;
    s32 i;

    camGetPtr(cameraId);
    if (*(s32*)((s32)work + 0x24) != -1) {
        for (i = 0; i < 2; i++) {
            f32 s = float_1p1_80427928 * *(f32*)((s32)work + 0x10);
            u32 color;
            u32 colorCopy;
            if (i == 0) {
                s *= float_1p06_8042792c;
                color = dat_80427920;
            } else {
                color = dat_80427924;
            }
            PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
            PSMTXScale(scale, s, s, s);
            PSMTXRotRad(rot, 0x79, float_deg2rad_80427930 * *(f32*)((s32)work + 0x20));
            PSMTXConcat(trans, scale, mtx);
            PSMTXConcat(mtx, rot, mtx);
            animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x24), 0x40);
            ((u8*)&color)[3] = *(u8*)((s32)work + 0x14);
            colorCopy = color;
            animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x24), &colorCopy);
            animPoseMain(*(s32*)((s32)work + 0x24));
            animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 1, float_0_80427934, float_10_80427938);
            animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 2, float_0_80427934, float_10_80427938);
            animPoseDrawMtx(*(s32*)((s32)work + 0x24), mtx, 3, float_0_80427934, float_10_80427938);
        }
    }
}

/* stub-fill: effTreasureMapMain | prototype_only | source_prototype */
void effTreasureMapMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* gp;
    extern s32 animGroupBaseAsync(void* name, s32 battle, s32 flags);
    extern s32 animPoseEntry(void* name, s32 battle);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effTreasureMapDisp(s32 cameraId, void* effect);
    extern VecLocal vec3_802ff058;
    extern char str_EFF_treasure_map_802ff064[];
    extern char str_Z_1_8042793c[];

    void* work = *(void**)((s32)effect + 0xC);
    VecLocal pos = vec3_802ff058;
    s32 flag = *(s32*)((s32)gp + 0x14);
    s32 inBattle = ((u32)(-flag) | (u32)flag) >> 31;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);

    if (*(s32*)((s32)work + 0x24) == -1) {
        if (animGroupBaseAsync(str_EFF_treasure_map_802ff064, inBattle, 0) == 0) {
            return;
        }
        *(s32*)((s32)work + 0x24) = animPoseEntry(str_EFF_treasure_map_802ff064, inBattle);
        animPoseSetAnim(*(s32*)((s32)work + 0x24), str_Z_1_8042793c, 1);
    }

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4U;
        *(s32*)((s32)work + 0x18) = 0x10;
    }
    if (*(s32*)((s32)work + 0x18) < 1000) {
        *(s32*)((s32)work + 0x18) -= 1;
    }
    if (*(s32*)((s32)work + 0x18) < 0x10) {
        *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x18) << 4;
    }

    if (*(s32*)((s32)work + 0x18) < 0) {
        if (*(s32*)((s32)work + 0x24) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x24));
        }
        effDelete(effect);
    } else {
        *(s32*)((s32)work + 0x1C) += 1;
        dispEntry(*(s32*)((s32)work + 0x28), 2, effTreasureMapDisp, effect, dispCalcZ(&pos));
    }
}

