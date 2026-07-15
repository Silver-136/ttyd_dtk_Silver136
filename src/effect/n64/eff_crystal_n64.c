#include "effect/n64/eff_crystal_n64.h"


void effCrystalMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effCrystalDisp(s32 cameraId, void* effect);
    extern double sin(f64 x);
    extern f32 float_6p2832_80425050;
    extern f32 float_360_80425054;
    extern f32 float_10_80425058;
    extern f32 float_15_8042505c;
    extern f32 float_1p1_80425060;
    u8* work;
    LocalVec3 pos;
    s32 timer;
    s32 frame;
    s32 alpha;
    s32 alpha2;
    s32 colorA;
    s32 colorB;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x1E;
    }

    if (*(s32*)(work + 0x10) < 100) {
        (*(s32*)(work + 0x10))--;
    }
    (*(s32*)(work + 0x14))++;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x14);
    alpha = 0xFF;
    alpha2 = 0xFF;
    if (frame < 0x10) {
        alpha = frame * 0x10 - 1;
    }
    if (timer < 0x10) {
        alpha = (alpha * timer) / 0xFF;
        alpha2 = timer << 4;
    }

    switch (*(s32*)(work + 0x1C)) {
        case 0:
            *(s32*)(work + 0x20) = 0x46;
            *(s32*)(work + 0x24) = 0xB4;
            *(s32*)(work + 0x28) = 0x78;
            colorA = (s32)(float_10_80425058 *
                (f32)sin((float_6p2832_80425050 * (f32)(frame * 2)) / float_360_80425054)) + 0xBE;
            *(s32*)(work + 0x2C) = (colorA * alpha) / 0xFF;
            break;
        case 1:
            *(s32*)(work + 0x20) = 0x46;
            *(s32*)(work + 0x24) = 0xFF;
            *(s32*)(work + 0x28) = 0xE6;
            *(s32*)(work + 0x2C) = (0xFF * alpha) / 0xFF;
            break;
        case 2:
            *(s32*)(work + 0x20) = 0xAA;
            *(s32*)(work + 0x24) = 0x28;
            *(s32*)(work + 0x28) = 0x6E;
            colorA = (s32)(float_10_80425058 *
                (f32)sin((float_6p2832_80425050 * (f32)(frame * 2)) / float_360_80425054)) + 0xBE;
            *(s32*)(work + 0x2C) = (colorA * alpha) / 0xFF;
            break;
        case 3:
            *(s32*)(work + 0x20) = 0xDC;
            *(s32*)(work + 0x24) = 0x28;
            *(s32*)(work + 0x28) = 0xAA;
            *(s32*)(work + 0x2C) = (0xFF * alpha) / 0xFF;
            break;
        case 4:
            *(s32*)(work + 0x20) = 0xEB;
            *(s32*)(work + 0x24) = 0xEB;
            *(s32*)(work + 0x28) = 0xC8;
            colorB = (s32)(float_15_8042505c *
                (f32)sin((float_6p2832_80425050 * (f32)(frame * 2)) / float_360_80425054)) + 0xD7;
            *(s32*)(work + 0x2C) = (colorB * alpha) / 0xFF;
            break;
        case 5:
            *(s32*)(work + 0x20) = 0xEB;
            *(s32*)(work + 0x24) = 0xEB;
            *(s32*)(work + 0x28) = 0xC8;
            *(s32*)(work + 0x2C) = (0xFF * alpha) / 0xFF;
            break;
        case 6:
            *(s32*)(work + 0x20) = 0x46;
            *(s32*)(work + 0x24) = 0xB4;
            *(s32*)(work + 0x28) = 0x78;
            *(s32*)(work + 0x2C) = (0x7F * alpha2) / 0xFF;
            *(f32*)(work + 0x30) *= float_1p1_80425060;
            break;
        case 7:
            *(s32*)(work + 0x20) = 0xA2;
            *(s32*)(work + 0x24) = 0xBD;
            *(s32*)(work + 0x28) = 0xAE;
            *(s32*)(work + 0x2C) = (0xA0 * alpha2) / 0xFF;
            *(f32*)(work + 0x30) *= float_1p1_80425060;
            break;
        case 8:
            *(s32*)(work + 0x20) = 0xAA;
            *(s32*)(work + 0x24) = 0x28;
            *(s32*)(work + 0x28) = 0x6E;
            *(s32*)(work + 0x2C) = (0x7F * alpha2) / 0xFF;
            *(f32*)(work + 0x30) *= float_1p1_80425060;
            break;
        case 9:
            *(s32*)(work + 0x20) = 0xB4;
            *(s32*)(work + 0x24) = 0xB2;
            *(s32*)(work + 0x28) = 0xAE;
            *(s32*)(work + 0x2C) = (0xA0 * alpha2) / 0xFF;
            *(f32*)(work + 0x30) *= float_1p1_80425060;
            break;
        default:
            *(s32*)(work + 0x20) = 0x32;
            *(s32*)(work + 0x24) = 0x32;
            *(s32*)(work + 0x28) = 0xFF;
            *(s32*)(work + 0x2C) = (0xA0 * alpha2) / 0xFF;
            *(f32*)(work + 0x30) += *(f32*)(work + 0x34);
            break;
    }

    dispEntry(*(s32*)(work + 0x38), 2, effCrystalDisp, effect, dispCalcZ(&pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effCrystalDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_deg2rad_80425040;
    extern f32 float_0p03125_80425044;
    extern f32 float_0p0625_80425048;
    extern f32 float_0_8042504c;
    extern char size108x108_tex32x32_vtx[];
    extern char size100x100_tex32x32_vtx[];
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8* work;
    void* cam;
    void* cam3d;
    u32 color;
    s32 type;
    s32 angle;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    type = *(s32*)work;
    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80425040 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXScale(scaleMtx, *(f32*)(work + 0x30), *(f32*)(work + 0x30), *(f32*)(work + 0x30));
    PSMTXConcat(transMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = ((*(u8*)(work + 0x20) << 24) | (*(u8*)(work + 0x24) << 16) |
             (*(u8*)(work + 0x28) << 8) | *(u8*)(work + 0x2C));
    GXSetTevColor(1, &color);
    GXSetCullMode(0);

    if (type < 3) {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 8, 0xE, 4, 0xF);
        GXSetTevAlphaIn(0, 7, 4, 6, 7);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(scaleMtx, float_0p03125_80425044, float_0p0625_80425048, float_0_8042504c);
        GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
        effGetTexObjN64(0x6D, texObj);
        GXLoadTexObj(texObj, 0);

        angle = *(s32*)(work + 0x14);
        PSMTXRotRad(rotMtx, float_deg2rad_80425040 * (f32)angle, 'z');
        PSMTXConcat(baseMtx, rotMtx, rotMtx);
        GXLoadPosMtxImm(rotMtx, 0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size108x108_tex32x32_vtx);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);

        PSMTXRotRad(rotMtx, float_deg2rad_80425040 * (f32)(angle * -8), 'z');
        PSMTXConcat(baseMtx, rotMtx, rotMtx);
        GXLoadPosMtxImm(rotMtx, 0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size100x100_tex32x32_vtx);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    } else {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
        GXSetTevAlphaIn(0, 7, 6, 4, 7);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(scaleMtx, float_0p0625_80425048, float_0p0625_80425048, float_0_8042504c);
        GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
        effGetTexObjN64(0x6E, texObj);
        GXLoadTexObj(texObj, 0);
        GXLoadPosMtxImm(baseMtx, 0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size100x100_tex32x32_vtx);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void* effCrystalN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCrystalMain(void*);
    extern char str_CrystalN64_802fadc8[];
    extern f32 float_0p1_80425064;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CrystalN64_802fadc8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x3C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCrystalMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    *(s32*)(work + 0x38) = 4;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 100;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(s32*)(work + 0x2C) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x30) = scale;
    *(s32*)(work + 0x18) = 0;

    switch (type) {
        case 0:
            *(s32*)(work + 0x1C) = 0;
            break;
        case 1:
            *(s32*)(work + 0x1C) = 2;
            break;
        case 2:
            *(s32*)(work + 0x1C) = 4;
            break;
        case 3:
            *(s32*)(work + 0x1C) = 6;
            break;
        case 4:
            *(s32*)(work + 0x1C) = 7;
            break;
        case 5:
            *(s32*)(work + 0x1C) = 8;
            break;
        case 6:
            *(s32*)(work + 0x1C) = 9;
            break;
        default: {
            f32 rate = -(scale - float_0p1_80425064) / (f32)timer;
            *(s32*)(work + 0x1C) = 10;
            *(f32*)(work + 0x34) = rate;
            break;
        }
    }

    return entry;
}

