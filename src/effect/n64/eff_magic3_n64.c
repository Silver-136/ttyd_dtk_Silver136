#include "effect/n64/eff_magic3_n64.h"


u8 effMagic3N64Entry(void) {
    return 0;
}


u8 effMagic3Main(void) {
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic3Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern u32 dat_80425a78;
    extern f32 float_0p015625_80425a7c;
    extern f32 float_0p03125_80425a80;
    extern f32 float_0_80425a84;
    extern f32 float_deg2rad_80425a88;
    extern f32 float_1_80425a8c;
    extern u8 size32x16_tex64x32_vtx[];
    f32 trans[3][4];
    f32 scaleMtx[3][4];
    f32 rotY[3][4];
    f32 rotZ[3][4];
    u8 texObj[0x20];
    void* cam;
    u8* item;
    s32 i;
    u32 colorTmp;
    u32 color;
    f32 deg2rad;
    f32 camRot;

    cam = camGetPtr(cameraId);
    item = *(u8**)((s32)effect + 0xC);

    colorTmp = dat_80425a78;
    ((u8*)&colorTmp)[3] = *(s32*)(item + 0x24);
    color = colorTmp;
    GXSetTevColor(1, &color);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p015625_80425a7c, float_0p03125_80425a80, float_0_80425a84);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x1E, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x16_tex64x32_vtx);

    deg2rad = float_deg2rad_80425a88;
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, item += 0x30) {
        PSMTXTrans(trans, *(f32*)(item + 4), *(f32*)(item + 8), *(f32*)(item + 0xC));
        camRot = -*(f32*)((s32)camGetPtr(4) + 0x114);
        PSMTXRotRad(rotY, 0x79, deg2rad * camRot);
        PSMTXRotRad(rotZ, 0x7A, deg2rad * *(f32*)(item + 0x1C));
        PSMTXScale(scaleMtx, *(f32*)(item + 0x20), *(f32*)(item + 0x20), float_1_80425a8c);
        PSMTXConcat(trans, rotY, trans);
        PSMTXConcat(trans, rotZ, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat((void*)((s32)cam + 0x11C), trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

