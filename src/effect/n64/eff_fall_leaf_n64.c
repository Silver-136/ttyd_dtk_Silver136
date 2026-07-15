#include "effect/n64/eff_fall_leaf_n64.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFallLeafDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 dat_80425128;
    extern u32 dat_8042512c;
    extern u32 dat_80425130;
    extern f32 float_deg2rad_80425138;
    extern f32 float_0p03125_8042513c;
    extern f32 float_0p0078125_80425140;
    extern f32 float_1_80425144;
    extern f32 float_0p015625_80425148;
    extern char size12x12_tex64x64_vtx[];
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8* work;
    u8* part;
    void* cam;
    void* cam3d;
    u32 type;
    u32 color;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    type = *(u32*)work;
    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80425138 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    if (type == 3) {
        color = dat_80425130;
    } else if ((s32)type < 3) {
        if ((s32)type < 2) {
            if ((s32)type > -1) {
                color = dat_80425128;
            }
        } else {
            color = dat_8042512c;
        }
    } else if ((s32)type < 5) {
        color = 0x7592A600;
    }
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    if (((s32)type < 2) && ((s32)type > -1)) {
        PSMTXScale(scaleMtx, float_0p03125_8042513c, float_0p0078125_80425140, float_1_80425144);
        GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
        effGetTexObjN64(0x14, texObj);
        GXLoadTexObj(texObj, 0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
        GXSetTevAlphaIn(0, 7, 4, 6, 7);
    } else {
        PSMTXScale(scaleMtx, float_0p03125_8042513c, float_0p015625_80425148, float_1_80425144);
        GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
        effGetTexObj(0x6B, texObj);
        GXLoadTexObj(texObj, 0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
        GXSetTevAlphaIn(0, 7, 4, 6, 7);
    }

    GXSetCullMode(0);
    effSetVtxDescN64(size12x12_tex64x64_vtx);
    part = work;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        part += 0x30;
        PSMTXTrans(transMtx, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rotMtx, float_deg2rad_80425138 * *(f32*)(part + 0x18), 'z');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXRotRad(rotMtx, float_deg2rad_80425138 * *(f32*)(part + 0x20), 'y');
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

void* effFallLeafN64Entry(f32 x, f32 y, f32 z, u32 type, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern s32 rand(void);
    extern void effFallLeafMain(void* effect);
    extern char str_FallLeafN64_802fae78[];
    extern f32 float_0_80425180;
    extern f32 float_2_80425158;
    extern f32 float_10_80425178;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 r;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FallLeafN64_802fae78;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0xF0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFallLeafMain;

    *(u32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = timer;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    part = work + 0x30;
    for (i = 1; i < 5; i++, part += 0x30) {
        if (type == 1) {
            r = rand();
            *(f32*)(part + 4) = (f32)((r % 10) - 5);
            r = rand();
            *(f32*)(part + 8) = (f32)((r % 10) + 5);
            *(f32*)(part + 0xC) = float_10_80425178;
            r = rand();
            *(f32*)(part + 0x18) = (f32)(r % 0x168);
            r = rand();
            *(f32*)(part + 0x20) = (f32)(r % 0x168);
            r = rand();
            *(f32*)(part + 0x1C) = (f32)(r % 100) / float_10_80425178;
            *(f32*)(part + 0x10) = float_0_80425180;
            *(f32*)(part + 0x14) = float_2_80425158;
        } else if ((s32)type < 1 ? ((s32)type > -1) : ((s32)type < 5)) {
            r = rand();
            *(f32*)(part + 4) = (f32)((r % 0x32) - 0x19);
            r = rand();
            *(f32*)(part + 8) = (f32)((r % 0x32) - 0x19);
            *(f32*)(part + 0xC) = float_0_80425180;
            r = rand();
            *(f32*)(part + 0x18) = (f32)(r % 0x168);
            r = rand();
            *(f32*)(part + 0x20) = (f32)(r % 0x168);
            r = rand();
            *(f32*)(part + 0x1C) = (f32)(r % 100) / float_10_80425178;
            *(f32*)(part + 0x10) = float_0_80425180;
            *(f32*)(part + 0x14) = float_0_80425180;
        }
    }

    return entry;
}

void effFallLeafMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern s32 effTblRandN64(s32 max, s32 seed);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFallLeafDisp(s32 cameraId, void* effect);
    extern f32 float_0p3_8042514c;
    extern f32 float_0p8_80425150;
    extern f32 float_6p2832_80425154;
    extern f32 float_2_80425158;
    extern f32 float_360_8042515c;
    extern f32 float_neg0p05_80425160;
    extern f32 float_0p2_80425164;
    extern f32 float_0p94_80425168;
    extern f32 float_0p95_8042516c;
    extern f32 float_0p05_80425170;
    extern f32 float_0p92_80425174;
    extern f32 float_10_80425178;
    extern f32 float_0p5_8042517c;
    u8* work;
    u8* part;
    LocalVec3 pos;
    s32 frame;
    s32 type;
    s32 i;
    s32 seed;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);

    (*(s32*)(work + 0x28))--;
    (*(s32*)(work + 0x2C))++;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x2C);
    type = *(s32*)work;
    if (frame < 10) {
        *(s32*)(work + 0x24) =
            (s32)(float_0p3_8042514c * (f32)(0xFF - *(s32*)(work + 0x24)) + (f32)*(s32*)(work + 0x24));
    }
    if (*(s32*)(work + 0x28) < 10) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p8_80425150);
    }

    part = work + 0x30;
    seed = frame;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x30) {
        seed += 0x14;
        *(f32*)(part + 0x14) += float_neg0p05_80425160;
        *(f32*)(part + 0x10) += float_0p2_80425164 *
            (f32)sin((float_6p2832_80425154 * float_2_80425158 * *(f32*)(part + 0x18)) /
                     float_360_8042515c);
        if (type == 1) {
            *(f32*)(part + 0x14) += float_neg0p05_80425160;
            *(f32*)(part + 0x10) *= float_0p92_80425174;
        } else if ((type < 1) ? (type > -1) : (type < 5)) {
            *(f32*)(part + 0x10) *= float_0p94_80425168;
            *(f32*)(part + 0x14) *=
                float_0p05_80425170 *
                    (f32)sin((float_6p2832_80425154 * *(f32*)(part + 0x18)) / float_360_8042515c) +
                float_0p95_8042516c;
        }

        *(f32*)(part + 0x1C) += (f32)(effTblRandN64(0x32, seed) - 0x19);
        *(f32*)(part + 0x18) =
            float_10_80425178 *
                (f32)sin((float_6p2832_80425154 * *(f32*)(part + 0x1C)) / float_360_8042515c) +
            *(f32*)(part + 0x18);
        *(f32*)(part + 0x20) =
            float_10_80425178 *
                (f32)cos((float_6p2832_80425154 * *(f32*)(part + 0x1C) * float_0p5_8042517c) /
                         float_360_8042515c) +
            *(f32*)(part + 0x20);
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
    }

    dispEntry(4, 2, effFallLeafDisp, effect, dispCalcZ(&pos));
}

