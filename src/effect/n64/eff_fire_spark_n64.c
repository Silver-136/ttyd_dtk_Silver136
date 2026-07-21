#include "effect/n64/eff_fire_spark_n64.h"
void* effFireSparkN64Entry(f32 x, f32 y, f32 z, f32 width, f32 height, f32 unkScale,
                           f32 velScaleA, f32 velScaleB, s32 type, s32 count, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireSparkMain(void* effect);
    extern char str_FireSparkN64_802faef0[];
    extern f32 float_30_80425238;
    extern f32 float_0p5_80425224;

    void* entry;
    u8* work;
    u8* part;
    f32 step;
    u32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireSparkN64_802faef0;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x70);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireSparkMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x1C) = 0;
    if (duration < 1) {
        *(s32*)(work + 0x18) = 1000;
    } else {
        *(s32*)(work + 0x18) = duration;
    }

    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = width;
    *(f32*)(work + 0x14) = height;
    *(f32*)(work + 0x40) = unkScale;
    *(f32*)(work + 0x68) = velScaleA;
    *(f32*)(work + 0x6C) = velScaleB;

    *(s32*)(work + 0x20) = 0x7F;
    *(s32*)(work + 0x24) = 0;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x30) = 0x7F;
    *(s32*)(work + 0x34) = 0x7F;
    *(s32*)(work + 0x38) = 0;
    *(s32*)(work + 0x3C) = 0xFF;

    step = (float_30_80425238 / (f32)(count - 1)) * float_0p5_80425224;
    part = work + 0x70;
    for (i = 1; i < (u32)count; i++, part += 0x70) {
        *(s32*)(part + 0xD4) = (s32)(-(f32)i * step) - 1;
    }

    return entry;
}


void effFireSparkMain(void* effect) {
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFireSparkDisp(s32 cameraId, void* effect);
    extern f32 scale_data[];
    extern f32 float_0_80425220;
    extern f32 float_0p5_80425224;
    extern f32 float_0p05_80425228;
    extern f32 float_0p1_8042522c;
    extern f32 float_0p3_80425230;
    extern f32 float_0p04_80425234;

    u8* work;
    u8* part;
    f32 pos[3];
    f32 halfWidth;
    f32 xRange;
    f32 yRange;
    s32 count;
    s32 i;
    s32 r;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x18) = 0x10;
    }
    if (*(s32*)(work + 0x18) < 1000) {
        (*(s32*)(work + 0x18))--;
    }
    (*(s32*)(work + 0x1C))++;
    if (*(s32*)(work + 0x1C) > 0x4F1A0) {
        *(s32*)(work + 0x1C) = 0x100;
    }

    if (*(s32*)(work + 0x18) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x18) < 0x10) {
        *(s32*)(work + 0x2C) = *(s32*)(work + 0x18) << 4;
    }
    if (*(s32*)(work + 0x1C) < 0x10) {
        *(s32*)(work + 0x2C) = (*(s32*)(work + 0x1C) * 0x10) + 0xF;
    }

    count = *(s32*)((s32)effect + 8);
    xRange = *(f32*)(work + 0x10);
    yRange = *(f32*)(work + 0x14);
    halfWidth = float_0p5_80425224 * xRange;

    part = work;
    for (i = 1; i < count; i++) {
        (*(s32*)(part + 0xD4))++;
        if (*(s32*)(part + 0xD4) > 0x1D) {
            *(s32*)(part + 0xD4) = 0;
        }
        if (*(s32*)(part + 0xD4) > -1) {
            if (*(s32*)(part + 0xD4) == 0) {
                r = rand();
                *(f32*)(part + 0x74) = (f32)(r % (s32)xRange) - halfWidth;
                r = rand();
                *(f32*)(part + 0x78) = (f32)(r % (s32)yRange);
                *(f32*)(part + 0x7C) = float_0_80425220;

                r = rand();
                *(f32*)(part + 0xB4) = float_0p05_80425228 * (f32)((r % 0x14) - 10);
                r = rand();
                *(f32*)(part + 0xB8) = float_0p05_80425228 * *(f32*)(work + 0x68) * (f32)((r % 0x28) - 1);
                *(f32*)(part + 0xBC) = float_0_80425220;
                r = rand();
                *(f32*)(part + 0xC0) = float_0p05_80425228 * (f32)((r % 0x14) - 10);
                r = rand();
                *(f32*)(part + 0xC4) = float_0p05_80425228 * *(f32*)(work + 0x6C) * (f32)((r % 0x1E) - 1);
                *(f32*)(part + 0xC8) = float_0_80425220;
                r = rand();
                *(f32*)(part + 0xCC) = (f32)(r % 0x10);
                r = rand();
                *(f32*)(part + 0xD0) = (f32)(r % 0x10);
            }

            *(f32*)(part + 0xB0) = scale_data[*(s32*)(part + 0xD4)];
            *(f32*)(part + 0x74) += *(f32*)(part + 0xB4);
            *(f32*)(part + 0x78) += *(f32*)(part + 0xB8);
            *(f32*)(part + 0x7C) += *(f32*)(part + 0xBC);
            *(f32*)(part + 0xCC) += float_0p1_8042522c;
            *(f32*)(part + 0xD0) += float_0p3_80425230;
            *(f32*)(part + 0xB4) += float_0p04_80425234 * (*(f32*)(part + 0xC0) - *(f32*)(part + 0xB4));
            *(f32*)(part + 0xB8) += float_0p04_80425234 * (*(f32*)(part + 0xC4) - *(f32*)(part + 0xB8));
            *(f32*)(part + 0xBC) += float_0p04_80425234 * (*(f32*)(part + 0xC8) - *(f32*)(part + 0xBC));
        }
        part += 0x70;
    }

    dispEntry(4, 2, effFireSparkDisp, effect, dispCalcZ(pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFireSparkDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, char axis);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevKColorSel(s32 stage, s32 sel);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetTevKColor(s32 id, void* color);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 dat_80425210;
    extern f32 float_deg2rad_80425214;
    extern f32 float_0p03125_80425218;
    extern f32 float_0p0078125_8042521c;
    extern f32 float_0_80425220;
    extern char stg__s_v[];

    u8 texObj[0x20];
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    f32 mtxC[3][4];
    f32 mtxD[3][4];
    u8* work;
    u8* part;
    void* cam;
    void* cam3d;
    u32 color0;
    u32 color1;
    u32 kcolor;
    f32 scale;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);

    PSMTXTrans(mtxD, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(mtxB, float_deg2rad_80425214 * -*(f32*)((s32)cam3d + 0x114), 'y');
    scale = *(f32*)(work + 0x40);
    PSMTXScale(mtxC, scale, scale, scale);
    PSMTXConcat(mtxD, mtxB, mtxD);
    PSMTXConcat(mtxD, mtxC, mtxD);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxD, mtxA);

    color0 = (*(u8*)(work + 0x20) << 24) | (*(u8*)(work + 0x24) << 16) |
             (*(u8*)(work + 0x28) << 8) | *(u8*)(work + 0x2C);
    GXSetTevColor(1, &color0);
    color1 = (*(u8*)(work + 0x30) << 24) | (*(u8*)(work + 0x34) << 16) |
             (*(u8*)(work + 0x38) << 8) | *(u8*)(work + 0x3C);
    GXSetTevColor(2, &color1);

    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 1, 1, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 2, 1);
    GXSetTevAlphaIn(0, 7, 7, 7, 7);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevColorOp(1, 1, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0xE, 2, 0);
    GXSetTevAlphaIn(1, 7, 6, 4, 7);
    GXSetTevKColorSel(1, 0xC);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    kcolor = dat_80425210;
    GXSetTevKColor(0, &kcolor);
    effGetTexObjN64(0x8B, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x65, texObj);
    GXLoadTexObj(texObj, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(stg__s_v);

    part = work;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (*(s32*)(part + 0xD4) > -1) {
            PSMTXTrans(mtxD, *(f32*)(part + 0x74), *(f32*)(part + 0x78), *(f32*)(part + 0x7C));
            PSMTXConcat(mtxA, mtxD, mtxD);
            GXLoadPosMtxImm(mtxD, 0);
            GXSetCurrentMtx(0);

            PSMTXScale(mtxC, float_0p03125_80425218, float_0p0078125_8042521c, float_0_80425220);
            PSMTXTrans(mtxD, float_0_80425220, (f32)((i % 4) * 0x20), float_0_80425220);
            PSMTXConcat(mtxC, mtxD, mtxC);
            GXLoadTexMtxImm(mtxC, 0x1E, 1);

            PSMTXScale(mtxC, float_0p03125_80425218, float_0p03125_80425218, float_0_80425220);
            PSMTXTrans(mtxD, *(f32*)(part + 0xCC), *(f32*)(part + 0xD0), float_0_80425220);
            PSMTXConcat(mtxC, mtxD, mtxC);
            GXLoadTexMtxImm(mtxC, 0x21, 1);

            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3);
        }
        part += 0x70;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
