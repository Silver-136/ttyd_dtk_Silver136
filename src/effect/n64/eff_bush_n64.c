#include "effect/n64/eff_bush_n64.h"


void effBushMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* marioGetPtr(void);
    extern void effDelete(void* effect);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern s32 rand(void);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effBushDisp(s32 cameraId, void* effect);
    extern f64 double_to_int_802facb0;
    extern f32 float_0p3_80424ea4;
    extern f32 float_0p8_80424ea8;
    extern f32 float_6p2832_80424eac;
    extern f32 float_2_80424eb0;
    extern f32 float_360_80424eb4;
    extern f32 float_neg0p05_80424eb8;
    extern f32 float_0p2_80424ebc;
    extern f32 float_0p94_80424ec0;
    extern f32 float_0p95_80424ec4;
    extern f32 float_0p05_80424ec8;
    extern f32 float_0p92_80424ecc;
    extern f32 float_25_80424ed0;
    extern f32 float_10_80424ed4;
    extern f32 float_0p5_80424ed8;
    u8* work;
    u8* part;
    s32 type;
    s32 i;
    s32 r;
    LocalVec3 pos;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    type = *(s32*)work;

    if ((*(s32*)(work + 0x2C) < 5) || (*(s32*)(work + 0x2C) > 0xE) || (type == 1) ||
        (type != 0) || (*(u16*)((s32)marioGetPtr() + 0x10) != 0)) {
        (*(s32*)(work + 0x28))--;
        (*(s32*)(work + 0x2C))++;
        if (*(s32*)(work + 0x2C) > 0x4F1A0) {
            *(s32*)(work + 0x2C) = 0x100;
        }
        if (*(s32*)(work + 0x28) < 0) {
            effDelete(effect);
            return;
        }

        if (*(s32*)(work + 0x2C) < 10) {
            *(s32*)(work + 0x24) = (s32)(float_0p3_80424ea4 *
                (f32)(0xFF - *(s32*)(work + 0x24)) + (f32)*(s32*)(work + 0x24));
        }
        if (*(s32*)(work + 0x28) < 10) {
            *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p8_80424ea8);
        }

        part = work + 0x34;
        for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
            *(f32*)(part + 0x18) += float_neg0p05_80424eb8;
            *(f32*)(part + 0x14) += float_0p2_80424ebc *
                (f32)sin((float_6p2832_80424eac * float_2_80424eb0 * *(f32*)(part + 0x1C)) / float_360_80424eb4);
            if (type == 0) {
                *(f32*)(part + 0x14) *= float_0p94_80424ec0;
                *(f32*)(part + 0x18) *=
                    float_0p05_80424ec8 *
                    (f32)sin((float_6p2832_80424eac * *(f32*)(part + 0x1C)) / float_360_80424eb4) +
                    float_0p95_80424ec4;
            } else {
                *(f32*)(part + 0x18) += float_neg0p05_80424eb8;
                *(f32*)(part + 0x14) *= float_0p92_80424ecc;
            }
            r = rand();
            *(f32*)(part + 0x20) += (f32)(r % 50) - float_25_80424ed0;
            *(f32*)(part + 0x1C) += float_10_80424ed4 *
                (f32)sin((float_6p2832_80424eac * *(f32*)(part + 0x20)) / float_360_80424eb4);
            *(f32*)(part + 0x24) += float_10_80424ed4 *
                (f32)cos((float_6p2832_80424eac * *(f32*)(part + 0x20) * float_0p5_80424ed8) / float_360_80424eb4);
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
        }

        dispEntry(4, 2, effBushDisp, effect, dispCalcZ(&pos));
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBushDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
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
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 dat_80424e90;
    extern f32 float_deg2rad_80424e94;
    extern f32 float_4_80424e98;
    extern f32 float_1_80424e9c;
    extern f32 float_0_80424ea0;
    extern u8 DAT_8039e448[];
    extern u8 DAT_8039e452[];
    extern u8 DAT_8039e44e[];
    void* cam;
    void* cam3d;
    u8* work;
    u8* part;
    s32 i;
    u32 color;
    u8 texObj[0x20];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 baseMtx[3][4];
    f32 texMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);

    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424e94 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = dat_80424e90;
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_4_80424e98, float_1_80424e9c, float_0_80424ea0);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    effGetTexObjN64(0x2A, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xB, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 0, 5, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxDesc(0xB, 2);
    GXSetVtxDesc(0xD, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0xC);
    GXSetArray(9, DAT_8039e448, 0xE);
    GXSetArray(0xB, DAT_8039e452, 0xE);
    GXSetArray(0xD, DAT_8039e44e, 0xE);

    part = work + 0x34;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        PSMTXTrans(transMtx, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rotMtx, float_deg2rad_80424e94 * *(f32*)(part + 0x18), 'z');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXRotRad(rotMtx, float_deg2rad_80424e94 * *(f32*)(part + 0x20), 'y');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(baseMtx, transMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 0xC);
        tri2(0, 1, 2, 0, 0, 2, 3);
        tri2(6, 5, 4, 4, 7, 6, 4);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma optimize_for_size off

void* effBushN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBushMain(void);
    extern s32 rand(void);
    extern char str_BushN64_802facb8[];
    extern f64 double_to_int_802facb0;
    extern f32 float_0_80424ea0;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 r;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BushN64_802facb8;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0x104);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBushMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 100;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    zero = float_0_80424ea0;
    part = work + 0x34;
    for (i = 1; i < 5; i++) {
        r = rand();
        *(f32*)(part + 4) = (f32)(r % 30 - 15);
        *(f32*)(part + 8) = zero;
        r = rand();
        *(f32*)(part + 0xC) = (f32)((r & 0x1E) - 15);
        r = rand();
        *(f32*)(part + 0x18) = (f32)(r & 0x168);
        r = rand();
        *(f32*)(part + 0x20) = (f32)(r & 0x168);
        r = rand();
        *(f32*)(part + 0x1C) = (f32)((r % 100) / 10);
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = zero;
        part += 0x34;
    }

    return entry;
}

#pragma optimize_for_size on

