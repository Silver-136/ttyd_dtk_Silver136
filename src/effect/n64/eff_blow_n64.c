#include "effect/n64/eff_blow_n64.h"


u8 effBlowN64Entry(void) {
    return 0;
}


u8 effBlowMain(int* param_1) {
    return 0;
}


void effBlowDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_0p03125_80424cfc;
    extern f32 float_0_80424d00;
    extern f32 float_deg2rad_80424d04;
    extern u32 dat_80424cf8;
    extern u8 size16x16_tex32x32_vtx[];
    u8* work;
    u8* item;
    void* cam;
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 i;

    cam = camGetPtr(param_1);
    work = *(u8**)((s32)param_2 + 0xC);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xA, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    effGetTexObjN64(0x34, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80424cfc, float_0p03125_80424cfc, float_0_80424d00);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    PSMTXTrans(baseMtx, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));

    item = work + 0x68;
    for (i = 1; i < *(s32*)((s32)param_2 + 8); i++, item += 0x68) {
        color = dat_80424cf8;
        ((u8*)&color)[3] = *(s32*)(item + 0x58);
        GXSetTevColor(1, &color);

        PSMTXTrans(transMtx, *(f32*)(item + 8), *(f32*)(item + 0xC), *(f32*)(item + 0x10));
        PSMTXRotRad(rotMtx, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114) * float_deg2rad_80424d04));
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXScale(scaleMtx, *(f32*)(item + 0x44), *(f32*)(item + 0x48), *(f32*)(item + 0x44));
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        PSMTXRotRad(rotMtx, 0x7A, *(f32*)(item + 0x4C) * float_deg2rad_80424d04);
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(baseMtx, transMtx, transMtx);
        PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

