#include "effect/n64/eff_pokopi_pcharge_n64.h"

extern s32 rand(void);

u8 n_data[13] = {
    11, 11, 11, 11, 17, 11, 11, 21, 21, 21, 21, 21, 21
};

static u8 vx_data[13] = {
    20, 5, 20, 20, 10, 20, 20, 8, 8, 8, 1, 1, 1
};

void* effPokopiPchargeN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 lifetime) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effPokopiPchargeMain(void*);
    extern char str_PokopiPchargeN64_802fbcd8[];
    extern const f32 float_0_80425d2c;
    extern const f32 float_1_80425d30;
    extern const f32 float_0p2_80425d44;
    extern const f32 float_0p1_80425d48;
    extern const f32 float_0p3_80425d4c;
    extern const f32 float_0p6_80425d50;
    extern const f32 float_neg1_80425d54;
    extern const f32 float_neg2_80425d58;
    extern const f32 float_2_80425d5c;
    extern const f32 float_0p5_80425d60;
    extern const f32 float_0p05_80425d64;

    void* entry;
    u8* base;
    u8* part;
    s32 count;
    s32 countMinusOne;
    s32 typeMod3;
    s32 i;
    s32 random;
    s32 mod3;
    s32 limit;
    s32 age;
    f32 baseAngle;
    f32 speed;

    entry = effEntry();
    count = n_data[type];

    *(char**)((u8*)entry + 0x14) = str_PokopiPchargeN64_802fbcd8;
    *(s32*)((u8*)entry + 8) = count;

    base = __memAlloc(3, count * 0x48);
    *(u8**)((u8*)entry + 0xC) = base;
    *(void**)((u8*)entry + 0x10) = effPokopiPchargeMain;
    *(s32*)entry |= 2;

    *(s32*)base = type;
    *(s32*)(base + 0x14) = 0;

    if (lifetime <= 0) {
        *(s32*)(base + 0x10) = 1000;
    } else {
        *(s32*)(base + 0x10) = lifetime;
    }

    *(s32*)(base + 0x24) = 0;
    *(f32*)(base + 4) = x;
    *(f32*)(base + 8) = y;
    *(f32*)(base + 0xC) = z;
    *(f32*)(base + 0x34) = scale;
    *(s32*)(base + 0x18) = 0xFF;
    *(s32*)(base + 0x1C) = 0xFF;
    *(s32*)(base + 0x20) = 0xFF;
    *(s32*)(base + 0x28) = 0xFF;
    *(s32*)(base + 0x2C) = 0xF5;
    *(s32*)(base + 0x30) = 0;
    *(f32*)(base + 0x44) = float_1_80425d30;
    *(f32*)(base + 0x38) = (f32)vx_data[type];

    random = rand();
    random = random - (random / 360) * 360;
    baseAngle = (f32)random;

    countMinusOne = count - 1;
    typeMod3 = type - (type / 3) * 3;

    part = base + 0x48;
    for (i = 1; i < *(s32*)((u8*)entry + 8); i++, part += 0x48) {
        *(s32*)part = typeMod3;

        switch (type) {
        case 0:
        case 1:
        case 2:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                -(float_0p2_80425d44 * (f32)random + float_0p2_80425d44);
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p1_80425d48 * (f32)mod3;
            *(f32*)(part + 0x40) = float_0_80425d2c;
            break;

        case 3:
        case 5:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                float_0p2_80425d44 * (f32)random + float_0p2_80425d44;
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p1_80425d48 * (f32)mod3;
            *(f32*)(part + 0x40) = float_0_80425d2c;
            break;

        case 6:
        case 7:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) = -(f32)(random + 4);
            *(s32*)(part + 0x10) = 10;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p3_80425d4c * (f32)mod3;
            *(f32*)(part + 0x40) = float_0_80425d2c;
            break;

        case 8:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                float_0p2_80425d44 * (f32)random + float_0p6_80425d50;
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p3_80425d4c * (f32)mod3;
            *(f32*)(part + 0x40) = float_neg1_80425d54;
            break;

        case 9:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                -(float_0p2_80425d44 * (f32)random + float_0p6_80425d50);
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p3_80425d4c * (f32)mod3;
            *(f32*)(part + 0x40) = float_neg2_80425d58;

            *(s32*)(base + 0x18) = 0xFF;
            *(s32*)(base + 0x1C) = 0xF5;
            *(s32*)(base + 0x20) = 0xD7;
            *(s32*)(base + 0x28) = 0xDC;
            *(s32*)(base + 0x2C) = 0x3C;
            *(s32*)(base + 0x30) = 10;
            break;

        case 10:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                float_0p1_80425d48 * (f32)random + float_2_80425d5c;
            *(s32*)(part + 0x10) = 10;
            *(f32*)(part + 0x34) =
                float_0p5_80425d60 * (float_1_80425d30 - float_0p3_80425d4c * (f32)mod3);
            *(f32*)(part + 0x40) = float_0_80425d2c;
            break;

        case 11:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                float_0p05_80425d64 * (f32)random + float_0p2_80425d44;
            *(s32*)(part + 0x10) = 30;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p3_80425d4c * (f32)mod3;
            *(f32*)(part + 0x40) = float_neg1_80425d54;
            break;

        case 4:
        case 12:
            random = rand();
            random = random - (random / 10) * 10;
            mod3 = i - (i / 3) * 3;

            *(f32*)(part + 0x38) =
                float_0p2_80425d44 * (f32)random + float_0p6_80425d50;
            *(s32*)(part + 0x10) = 20;
            *(f32*)(part + 0x34) =
                float_1_80425d30 - float_0p3_80425d4c * (f32)mod3;
            *(f32*)(part + 0x40) = float_neg2_80425d58;
            break;
        }

        random = rand();
        limit = *(s32*)(part + 0x10);
        random = random - (random / limit) * limit;
        age = -random;
        *(s32*)(part + 0x14) = age;

        speed = *(f32*)(part + 0x38);
        if (speed < float_0_80425d2c) {
            *(f32*)(part + 4) =
                (-speed) *
                (f32)(limit - age);
        } else {
            *(f32*)(part + 4) =
                (f32)(-age) * speed;
        }

        *(f32*)(part + 8) = float_0_80425d2c;
        *(f32*)(part + 0xC) = float_0_80425d2c;
        *(f32*)(part + 0x3C) =
            baseAngle +
            (f32)(((i - 1) * 360) / countMinusOne);
    }

    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiPchargeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiPchargeDisp(void);
    extern f64 sin(f64);
    extern const Vec3 vec3_802fbcc0;
    extern f64 double_to_int_802fbcd0;
    extern const f32 float_0_80425d2c;
    extern f32 float_180_80425d34;
    extern f32 float_6p2832_80425d38;
    extern const f32 float_360_80425d3c;
    extern f32 float_255_80425d40;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 frame;
    s32 limit;
    f32 angle;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbcc0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 0x3E8) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x10) << 4;
    }
    if (*(s32*)(work + 0x14) < 0x10) {
        *(s32*)(work + 0x24) = (*(s32*)(work + 0x14) << 4) + 0xF;
    }

    zero = float_0_80425d2c;
    p = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, p += 0x48) {
        *(s32*)(p + 0x14) += 1;
        frame = *(s32*)(p + 0x14);
        limit = *(s32*)(p + 0x10);
        if (frame >= limit) {
            *(s32*)(p + 0x14) = 0;
            if (*(f32*)(p + 0x38) < zero) {
                *(f32*)(p + 4) = (f32)(-limit) * *(f32*)(p + 0x38);
            } else {
                *(f32*)(p + 4) = zero;
            }
            *(f32*)(p + 0x3C) = (f32)(rand() % 0x168);
        }

        *(f32*)(p + 4) += *(f32*)(p + 0x38);
        *(f32*)(p + 0x3C) += *(f32*)(p + 0x40);
        angle = (f32)((*(s32*)(p + 0x14) * 0xB4) / *(s32*)(p + 0x10));
        if (angle < zero || angle > float_180_80425d34) {
            *(s32*)(p + 0x24) = 0;
        } else {
            *(s32*)(p + 0x24) = (s32)(float_255_80425d40 * (f32)sin((float_6p2832_80425d38 * angle) / float_360_80425d3c));
        }
    }

    dispEntry(4, 2, effPokopiPchargeDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effPokopiPchargeDisp(s32 cameraId, void* effect) {
    typedef struct GXColorLocal {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColorLocal;

    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCullMode(s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);

    extern f32 float_deg2rad_80425d20;
    extern f32 float_0p03125_80425d24;
    extern f32 float_0p0078125_80425d28;
    extern const f32 float_0_80425d2c;
    extern const f32 float_1_80425d30;

    extern u32 unk_804297b0;
    extern u32 unk_804297b4;
    extern u32 unk_804297b8;

    extern u8 size10x10_tex32x32_vtx[];
    extern u8 size10x10_tex32x32_2_vtx[];
    extern u8 size128x10_tex32x32_vtx[];
    extern u8 size128x10_tex32x32_2_vtx[];

    f32 trans[3][4];
    f32 scaleMtx[3][4];
    f32 rot[3][4];
    f32 baseMtx[3][4];
    u8 texObj[0x20];
    GXColorLocal color1;
    GXColorLocal color1Arg;
    GXColorLocal color2;
    GXColorLocal color2Arg;
    GXColorLocal partColor;
    GXColorLocal partColorArg;

    void* camera;
    u8* work;
    u8* part;
    s32 baseAlpha;
    s32 type;
    s32 red;
    s32 green;
    s32 blue;
    s32 i;
    s32 idx0;
    s32 idx1;
    s32 idx2;
    s32 idx3;
    s32 alpha;
    f32 baseScale;
    f32 xOffset;
    f32 partScale;
    f32 deg2rad;

    camera = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);

    baseScale = *(f32*)(work + 0x34) * *(f32*)(work + 0x44);
    baseAlpha = *(s32*)(work + 0x24);
    type = *(s32*)work;
    red = *(s32*)(work + 0x18);
    green = *(s32*)(work + 0x1C);
    blue = *(s32*)(work + 0x20);
    xOffset = *(f32*)(work + 0x38);

    PSMTXTrans(
        trans,
        *(f32*)(work + 4),
        *(f32*)(work + 8),
        *(f32*)(work + 0xC));

    PSMTXRotRad(
        rot,
        0x79,
        float_deg2rad_80425d20 *
            -*(f32*)((s32)camGetPtr(4) + 0x114));

    PSMTXScale(scaleMtx, baseScale, baseScale, baseScale);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, baseMtx);

    *(u32*)&color1 = unk_804297b0;
    color1.r = (u8)red;
    color1.g = (u8)green;
    color1.b = (u8)blue;
    color1.a = (u8)baseAlpha;
    color1Arg = color1;
    GXSetTevColor(1, &color1Arg);

    *(u32*)&color2 = unk_804297b4;
    color2.r = (u8)*(s32*)(work + 0x28);
    color2.g = (u8)*(s32*)(work + 0x2C);
    color2.b = (u8)*(s32*)(work + 0x30);
    color2Arg = color2;
    GXSetTevColor(2, &color2Arg);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 4, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    PSMTXScale(
        scaleMtx,
        float_0p03125_80425d24,
        float_0p0078125_80425d28,
        float_0_80425d2c);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);

    effGetTexObjN64(0x86, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);

    idx0 = type * 4;
    idx1 = idx0 + 1;
    idx2 = idx0 + 2;
    idx3 = idx0 + 3;
    deg2rad = float_deg2rad_80425d20;

    part = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x48) {
        partScale = *(f32*)(part + 0x34);

        *(u32*)&partColor = unk_804297b8;
        partColor.r = (u8)red;
        partColor.g = (u8)green;
        partColor.b = (u8)blue;

        alpha = (baseAlpha * *(s32*)(part + 0x24)) / 255;
        partColor.a = (u8)alpha;
        partColorArg = partColor;
        GXSetTevColor(1, &partColorArg);

        PSMTXRotRad(
            rot,
            0x7A,
            deg2rad * *(f32*)(part + 0x3C));

        PSMTXTrans(
            trans,
            *(f32*)(part + 4) + xOffset,
            float_0_80425d2c,
            float_0_80425d2c);

        PSMTXConcat(rot, trans, trans);

        if (type == 11 || type == 10) {
            PSMTXScale(
                scaleMtx,
                partScale,
                float_1_80425d30,
                float_1_80425d30);
        } else {
            PSMTXScale(
                scaleMtx,
                partScale,
                partScale,
                float_1_80425d30);
        }

        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat(baseMtx, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);

        switch (type) {
        case 0:
        case 1:
        case 2:
            effSetVtxDescN64(size10x10_tex32x32_vtx);
            GXBegin(0x90, 0, 6);
            tri2(idx0, idx1, idx2, 0, idx0, idx2, idx3, 0);
            break;

        case 3:
        case 4:
        case 5:
            effSetVtxDescN64(size10x10_tex32x32_2_vtx);

            idx3 -= 12;
            idx2 -= 12;
            idx0 -= 12;
            idx1 -= 12;
            type -= 3;

            GXBegin(0x90, 0, 6);
            tri2(idx0, idx1, idx2, 0, idx0, idx2, idx3, 0);
            break;

        case 6:
        case 7:
        case 8:
        case 11:
            effSetVtxDescN64(size128x10_tex32x32_vtx);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;

        case 9:
        case 12:
            effSetVtxDescN64(size10x10_tex32x32_2_vtx + 112);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;

        case 10:
            effSetVtxDescN64(size128x10_tex32x32_2_vtx);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;

        default:
            break;
        }
    }
}

const f32 float_0_80425d2c = 0.0f;
const f32 float_1_80425d30 = 1.0f;
const f32 float_360_80425d3c = 360.0f;
const f32 float_0p2_80425d44 = 0.2f;
const f32 float_0p1_80425d48 = 0.1f;
const f32 float_0p3_80425d4c = 0.3f;
const f32 float_0p6_80425d50 = 0.6f;
const f32 float_neg1_80425d54 = -1.0f;
const f32 float_neg2_80425d58 = -2.0f;
const f32 float_2_80425d5c = 2.0f;
const f32 float_0p5_80425d60 = 0.5f;
const f32 float_0p05_80425d64 = 0.05f;
