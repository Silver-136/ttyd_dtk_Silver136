#include "effect/eff_toiki.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effToikiEntry(s32 type, s32 arg1, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effToikiMain(void);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Toiki_80428228;
    extern f32 float_0_80428200;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_0p5_80428230;
    extern f32 float_255_80428234;
    extern f32 float_2p5_80428238;

    void* effect;
    void* work;
    f32 angle;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = &str_Toiki_80428228;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effToikiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = float_0p5_80428230;
    *(f32*)((s32)work + 0x20) = float_0_80428200;
    *(s32*)((s32)work + 0x30) = arg1;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x28) = float_255_80428234;
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x10) = float_2p5_80428238 * (f32)cos(angle);
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x14) = float_2p5_80428238 * (f32)sin(angle);
    *(f32*)((s32)work + 0x18) = float_0_80428200;
    *(f32*)((s32)work + 0x24) = float_0_80428200;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* stub-fill: effToikiMain | prototype_only | source_prototype */
void effToikiMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effToikiDisp(s32 cameraId, void* effect);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXMultVec(f32 mtx[3][4], f32* src, f32* dst);
    extern void* effMistEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale);
    extern f32 float_0p95_80428208;
    extern f32 float_1_8042820c;
    extern f32 float_4_80428210;
    extern f32 float_320_80428214;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_deg2rad_80428204;
    extern f32 float_0_80428200;
    extern f32 float_0p015_80428224;

    void* work;
    f32 pos[3];
    f32 mist[3];
    f32 mtx[3][4];
    s32 timer;
    s32 type;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    type = *(s32*)((s32)work + 0);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        *(s32*)((s32)work + 0x30) = 0x10;
    }
    if (*(s32*)((s32)work + 0x30) < 1000) {
        *(s32*)((s32)work + 0x30) -= 1;
    }
    if (*(s32*)((s32)work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0xC) += *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x10) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x14) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x18) *= float_0p95_80428208;

    if (*(s32*)((s32)work + 0x30) < 0x10) {
        *(f32*)((s32)work + 0x28) = (f32)(*(s32*)((s32)work + 0x30) << 4);
    }

    if (*(f32*)((s32)work + 0x24) > float_1_8042820c &&
        *(f32*)((s32)work + 0x24) < float_4_80428210) {
        timer = *(s32*)((s32)work + 0x30);
        if ((timer % 20) == 0) {
            f32 radians = ((float_200_8042821c + *(f32*)((s32)work + 0x20)) * float_6p2832_80428218) / float_360_80428220;
            f32 dist = float_320_80428214 * *(f32*)((s32)work + 0x1C);
            mist[0] = *(f32*)((s32)work + 4) + (dist * (f32)cos(radians));
            mist[1] = *(f32*)((s32)work + 8) + (dist * (f32)sin(radians));
            mist[2] = *(f32*)((s32)work + 0xC) + float_1_8042820c;
            PSMTXRotRad(mtx, 0x79, float_deg2rad_80428204 * -*(f32*)((s32)camGetPtr(4) + 0x114));
            PSMTXMultVec(mtx, mist, mist);
            effMistEntry(type + 1, 0x3C, mist[0], mist[1], mist[2], float_0_80428200);
        }
    }

    *(s32*)((s32)work + 0x2C) += 1;
    *(f32*)((s32)work + 0x24) += float_0p015_80428224;
    if (*(f32*)((s32)work + 0x24) > float_4_80428210) {
        *(f32*)((s32)work + 0x24) = float_4_80428210;
    }
    dispEntry(4, 2, effToikiDisp, dispCalcZ(pos), effect);
}

/* CHATGPT STUB FILL: main/effect/eff_toiki 20260624_184929 */

/* stub-fill: effToikiDisp | missing_definition | ghidra_signature */
void effToikiDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, u32*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void GXLoadTexMtxImm(f32[3][4], s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32, void*, s32);
    extern void GXCallDisplayList(void*, s32);
    extern f32 float_deg2rad_80428204;
    extern u32 color_tbl[];
    extern u8 queen_toiki_vertex_tbl[];
    extern u8 queen_toiki_normal_tbl[];
    extern u8 queen_toiki_color0_tbl[];
    extern u8 queen_toiki_texcoord0_tbl[];
    extern void* queen_toiki_dl_0_tbl[];
    extern u8 queen_toiki_dl_0_size_tbl[];

    s32* work = *(s32**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    u32 color = (color_tbl[work[0]] & 0xFFFFFF00) | ((s32)*(f32*)&work[10] & 0xFF);
    f32 size;
    s32 i;

    effGetTexObj(0x5C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetTevColor(1, &color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)&work[9], 0.0f, 0.0f);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);

    PSMTXTrans(trans, *(f32*)&work[1], *(f32*)&work[2], *(f32*)&work[3]);
    PSMTXRotRad(rot, 0x79, float_deg2rad_80428204 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 0x7A, float_deg2rad_80428204 * *(f32*)&work[8]);
    PSMTXConcat(trans, rot, trans);
    size = *(f32*)&work[7];
    PSMTXScale(scaleMtx, size, size, size);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x34), trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 6);
    GXSetArray(9, queen_toiki_vertex_tbl, 6);
    GXSetVtxDesc(0xA, 2);
    GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
    GXSetArray(0xA, queen_toiki_normal_tbl, 3);
    GXSetVtxDesc(0xB, 2);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetArray(0xB, queen_toiki_color0_tbl, 4);
    GXSetVtxDesc(0xD, 2);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0xE);
    GXSetArray(0xD, queen_toiki_texcoord0_tbl, 4);
    for (i = 0; i < 14; i++) {
        GXCallDisplayList(queen_toiki_dl_0_tbl[i], queen_toiki_dl_0_size_tbl[i] << 5);
    }
}

