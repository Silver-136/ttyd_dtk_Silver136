#include "effect/n64/eff_confetti_n64.h"

void effConfettiN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x34) = camId;
}


void* effConfettiN64Entry(f32 x, f32 y, f32 z, s32 type, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern s32 rand(void);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern void effConfettiMain(void* effect);
    extern char str_ConfettiN64_802fad78[];
    extern f32 float_0_80424f78;
    extern f32 float_0p1_80424f7c;
    extern f32 float_35_80424f80;
    extern f32 float_15_80424f84;
    extern f32 float_0p03_80424f88;
    extern f32 float_5_80424f8c;
    extern f32 float_neg1p7_80424f90;
    extern f32 float_0p01_80424f94;
    extern f32 float_0p05_80424f98;
    extern f32 float_10_80424f9c;
    extern f32 float_3_80424fa8;
    extern f32 float_4_80424fac;
    extern f32 float_0p5_80424fb0;
    extern f32 float_6p2832_80424fb4;
    extern f32 float_360_80424fb8;
    extern f32 float_1_80424fbc;
    extern f32 float_neg0p02_80424fc0;
    extern f32 float_0p02_80424fc4;
    void* entry;
    u8* work;
    u8* part;
    s32 typeMod;
    s32 count;
    s32 i;
    s32 r;
    s32 baseAngle;
    f32 spread;
    f32 radius;
    f32 angle;
    f32 randScale;

    entry = effEntry();
    typeMod = type % 10;
    switch (typeMod) {
        case 0:
        case 4:
            count = 0xC;
            break;
        case 1:
        case 5:
            count = 0x12;
            break;
        case 2:
        case 6:
            count = 0x18;
            break;
        default:
            count = 0x30;
            break;
    }

    spread = float_3_80424fa8;
    if (typeMod != 4) {
        spread = float_4_80424fac;
        if (!((typeMod == 1) || ((typeMod < 1) && (typeMod > -1))) &&
            !((typeMod != 6) && (typeMod < 6))) {
            spread = float_5_80424f8c;
        }
    }

    *(char**)((s32)entry + 0x14) = str_ConfettiN64_802fad78;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x38);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effConfettiMain;

    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x24) = 0;
    *(s32*)(work + 0x20) = timer;
    *(s32*)work = type;
    *(s32*)(work + 0x2C) = count;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x34) = 4;

    baseAngle = rand() % 0x168;
    radius = float_0p5_80424fb0 * spread + float_0p5_80424fb0 * spread;
    part = work;
    for (i = 0; i < count; i++, part += 0x38) {
        *(s32*)(part + 0x38) = type;
        r = rand();
        randScale = spread * float_0p01_80424f94 * (f32)(r % 0x65);
        angle = (float_6p2832_80424fb4 * ((f32)baseAngle + (float_360_80424fb8 * (f32)i) / (f32)count)) /
                float_360_80424fb8;

        if (typeMod == 3) {
            r = rand();
            *(f32*)(part + 0x3C) = float_0p1_80424f7c * (f32)(r % 0x2BD) - float_35_80424f80;
            *(f32*)(part + 0x40) = float_0_80424f78;
            r = rand();
            *(f32*)(part + 0x44) = float_0p1_80424f7c * (f32)(r % 0x12D) - float_15_80424f84;
            r = rand();
            *(f32*)(part + 0x48) = float_0p03_80424f88 *
                ((float_0p1_80424f7c * (f32)(r % 0x65) + *(f32*)(part + 0x3C)) - float_5_80424f8c);
            r = rand();
            *(f32*)(part + 0x4C) =
                -(float_0p01_80424f94 * float_0p1_80424f7c * (f32)(r % 0x321) - float_neg1p7_80424f90);
            r = rand();
            *(f32*)(part + 0x50) =
                float_0p05_80424f98 * (float_0p1_80424f7c * (f32)(r % 0xC9) - float_10_80424f9c);
            *(s32*)(part + 0x58) = 0x78;
            *(s32*)(part + 0x68) = i * 3;
        } else {
            *(f32*)(part + 0x3C) = (f32)cos(angle) * randScale * (f32)sin(angle);
            *(f32*)(part + 0x40) = (f32)cos(angle) * randScale * (f32)cos(angle);
            *(f32*)(part + 0x44) = randScale * (f32)sin(angle);
            *(f32*)(part + 0x48) = (f32)cos(angle) * radius * (f32)sin(angle);
            *(f32*)(part + 0x4C) = (f32)cos(angle) * radius * (f32)cos(angle) + float_1_80424fbc;
            *(f32*)(part + 0x50) = radius * (f32)sin(angle);
            if (typeMod < 4) {
                *(f32*)(part + 0x54) = float_0_80424f78;
            } else {
                *(f32*)(part + 0x54) =
                    -(float_0p02_80424fc4 * (f32)((i + 1) % 3) - float_neg0p02_80424fc0);
            }
            *(s32*)(part + 0x68) = i & 3;
        }
    }

    return entry;
}

void effConfettiMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effConfettiDisp(s32 cameraId, void* effect);
    extern f32 float_0_80424f78;
    extern f32 float_0p1_80424f7c;
    extern f32 float_35_80424f80;
    extern f32 float_15_80424f84;
    extern f32 float_0p03_80424f88;
    extern f32 float_5_80424f8c;
    extern f32 float_neg1p7_80424f90;
    extern f32 float_0p01_80424f94;
    extern f32 float_0p05_80424f98;
    extern f32 float_10_80424f9c;
    extern f32 float_0p98_80424fa0;
    extern f32 float_neg0p5_80424fa4;
    u8* work;
    u8* part;
    LocalVec3 pos;
    s32 cameraId;
    s32 type;
    s32 typeMod;
    s32 count;
    s32 frame;
    s32 i;
    s32 r;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    cameraId = *(s32*)(work + 0x34);

    (*(s32*)(work + 0x20))--;
    if (*(s32*)(work + 0x20) < 0) {
        effDelete(effect);
        return;
    }

    (*(s32*)(work + 0x24))++;
    if (*(s32*)(work + 0x24) > 0x4F1A0) {
        *(s32*)(work + 0x24) = 10;
    }

    frame = *(s32*)(work + 0x24);
    type = *(s32*)work;
    count = *(s32*)(work + 0x2C);
    if (*(s32*)(work + 0x20) < 10) {
        *(s32*)(work + 0x28) = *(s32*)(work + 0x20) * 0x19;
    }

    typeMod = type % 10;
    part = work;
    for (i = 0; i < count; i++, part += 0x38) {
        if (typeMod == 3) {
            if ((*(s32*)(part + 0x68) < 1) ||
                ((*(s32*)(part + 0x68))--, *(s32*)(part + 0x68) < 1)) {
                (*(s32*)(part + 0x58))--;
                if (*(s32*)(part + 0x58) < 0) {
                    r = rand();
                    *(f32*)(part + 0x3C) = float_0p1_80424f7c * (f32)(r % 701) - float_35_80424f80;
                    *(f32*)(part + 0x40) = float_0_80424f78;
                    r = rand();
                    *(f32*)(part + 0x44) = float_0p1_80424f7c * (f32)(r % 301) - float_15_80424f84;
                    r = rand();
                    *(f32*)(part + 0x48) = float_0p03_80424f88 *
                        ((float_0p1_80424f7c * (f32)(r % 101) + *(f32*)(part + 0x3C)) - float_5_80424f8c);
                    r = rand();
                    *(f32*)(part + 0x4C) =
                        -(float_0p01_80424f94 * float_0p1_80424f7c * (f32)(r % 801) - float_neg1p7_80424f90);
                    r = rand();
                    *(f32*)(part + 0x50) =
                        float_0p05_80424f98 * (float_0p1_80424f7c * (f32)(r % 201) - float_10_80424f9c);
                    *(s32*)(part + 0x58) = 0x78;
                    (*(s32*)(part + 0x58))--;
                }
                *(f32*)(part + 0x3C) += *(f32*)(part + 0x48);
                *(f32*)(part + 0x40) += *(f32*)(part + 0x4C);
                *(f32*)(part + 0x44) += *(f32*)(part + 0x50);
            }
        } else if ((*(s32*)(part + 0x68) < 1) ||
                   ((*(s32*)(part + 0x68))--, *(s32*)(part + 0x68) < 1)) {
            if (typeMod > 3) {
                *(f32*)(part + 0x48) *= float_0p98_80424fa0;
                *(f32*)(part + 0x4C) *= float_0p98_80424fa0;
                *(f32*)(part + 0x50) *= float_0p98_80424fa0;
                if ((frame > 5) && (float_neg0p5_80424fa4 < *(f32*)(part + 0x4C))) {
                    *(f32*)(part + 0x4C) += *(f32*)(part + 0x54);
                }
            }
            *(f32*)(part + 0x3C) += *(f32*)(part + 0x48);
            *(f32*)(part + 0x40) += *(f32*)(part + 0x4C);
            *(f32*)(part + 0x44) += *(f32*)(part + 0x50);
        }
    }

    if (type < 10) {
        dispEntry(cameraId, 2, effConfettiDisp, effect, dispCalcZ(&pos));
    } else if (type < 20) {
        dispEntry(5, 2, effConfettiDisp, effect, dispCalcZ(&pos));
    } else {
        dispEntry(1, 2, effConfettiDisp, effect, dispCalcZ(&pos));
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effConfettiDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_deg2rad_80424f6c;
    extern f32 float_0p011489_80424f68;
    extern f32 float_0p0625_80424f70;
    extern f32 float_0p0039062_80424f74;
    extern f32 float_0_80424f78;
    extern u8 color_table[];
    extern u8 anm_seq[];
    extern u8 size6x6_tex16x16_2_vtx[];
    extern u8 size6x6_tex16x16_vtx[];
    void* cam;
    void* cam3d;
    u8* work;
    u8* part;
    s32 frame;
    s32 type;
    s32 count;
    s32 alpha;
    s32 typeDiv;
    s32 i;
    u32 color;
    u8 texObj[0x20];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 baseMtx[3][4];
    f32 texMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    frame = *(s32*)(work + 0x24);
    type = *(s32*)work;
    count = *(s32*)(work + 0x2C);
    alpha = *(s32*)(work + 0x28);

    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424f6c * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    effGetTexObjN64(0x4B, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 1);
    GXSetTevAlphaIn(0, 7, 0, 5, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    typeDiv = type / 10;
    part = work;
    for (i = 0; i < count; i++, part += 0x38) {
        if (*(s32*)(part + 0x68) < 1) {
            PSMTXTrans(transMtx, *(f32*)(part + 0x3C), *(f32*)(part + 0x40), *(f32*)(part + 0x44));
            PSMTXConcat(baseMtx, transMtx, transMtx);
            GXLoadPosMtxImm(transMtx, 0);
            GXSetCurrentMtx(0);

            color = ((u32)color_table[((i % 12) * 3) + 0] << 24) |
                    ((u32)color_table[((i % 12) * 3) + 1] << 16) |
                    ((u32)color_table[((i % 12) * 3) + 2] << 8) |
                    (alpha & 0xFF);
            GXSetTevColor(1, &color);
            color = *(u32*)&float_0p011489_80424f68;
            GXSetTevColor(2, &color);

            PSMTXScale(texMtx, float_0p0625_80424f70, float_0p0039062_80424f74, float_0_80424f78);
            PSMTXTrans(transMtx, float_0_80424f78,
                       (f32)(((i + anm_seq[(frame >> 1) % 6]) * 0x40) & 0x3C0),
                       float_0_80424f78);
            PSMTXConcat(texMtx, transMtx, texMtx);
            GXLoadTexMtxImm(texMtx, 0x1E, 1);

            if (type + (typeDiv - (typeDiv >> 31)) * -10 == 3) {
                effSetVtxDescN64(size6x6_tex16x16_2_vtx);
                GXBegin(0x90, 0, 0x18);
                tri2(0, 1, 2, 0, 0, 2, 3);
                tri2(4, 5, 6, 4, 4, 6, 7);
                tri2(8, 9, 10, 8, 8, 10, 0xB);
                tri2(0xC, 0xD, 0xE, 0xC, 0xC, 0xE, 0xF);
            } else {
                effSetVtxDescN64(size6x6_tex16x16_vtx);
                GXBegin(0x90, 0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3);
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

