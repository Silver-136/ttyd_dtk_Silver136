#include "effect/n64/eff_thruhammer_n64.h"

typedef struct EffN64Vertex {
    s16 x;
    s16 y;
    s16 z;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} EffN64Vertex;
static EffN64Vertex stg_thruhammer_v[20];

extern void PSMTXTrans(void*, f32, f32, f32);
extern void PSMTXRotRad(void*, s32, f32);
extern void PSMTXConcat(void*, void*, void*);
static void main_dl(void*, void*);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThruHammerN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThruHammerMain(void*);
    extern char str_ThruHammerN64_802fc180[];
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;
    s32 groups;
    s32 remainder;

    entry = effEntry();
    if (type == 1) {
        goto count_one;
    }
    if (type >= 1) {
        goto count_one;
    }
    if (type < 0) {
        goto count_one;
    }
count_four:
    count = 4;
    goto count_done;
count_one:
    count = 1;
count_done:
    *(char**)((s32)entry + 0x14) = str_ThruHammerN64_802fc180;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x58);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThruHammerMain;
    *(s32*)(work + 0x28) = 0x3C;
    *(s32*)(work + 0x2C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x38) = 0xFF;
    *(u8*)(work + 0x50) = 0;
    *(u8*)(work + 0x51) = 0xFF;
    *(u8*)(work + 0x52) = 0x7A;
    *(u8*)(work + 0x53) = 0xF0;
    *(u8*)(work + 0x54) = 0xFF;
    *(u8*)(work + 0x55) = 0xFA;

    part = work + 0x58;
    i = 1;
    groups = count >> 2;
    while (groups != 0) {
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
        i++;
        part += 0x58;
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
        i++;
        part += 0x58;
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
        i++;
        part += 0x58;
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
        i++;
        part += 0x58;
        groups--;
    }
    remainder = count & 3;
    while (remainder != 0) {
        *(s32*)(part + 0x4C) = -1 - ((i - 1) << 2);
        *(f32*)(part + 0x34) = scale;
        *(f32*)(part + 0x30) = (f32)(i * 30);
        i++;
        part += 0x58;
        remainder--;
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void effThruHammerMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct GreenImpactWork {
        u16 type;
        u8 pad02[2];
        f32 x;
        f32 y;
        f32 z;
        f32 velX;
        f32 velY;
        s32 unk18;
        f32 scaleX;
        f32 scaleY;
        f32 scaleZ;
        s32 timer;
        s32 lifetime;
        f32 angleY;
        f32 angleZ;
        s32 alpha;
        f32 scroll0;
        f32 scrollVel0;
        f32 scroll1;
        f32 scrollVel1;
        s32 delay;
        u8 prim[3];
        u8 env[3];
        u8 pad56[2];
    } GreenImpactWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effThruHammerDisp(void);
    extern Vec3 vec3_802fc168[];
    extern f32 float_0_80426370;
    extern f32 float_0p8_8042637c;
    extern f32 float_6p2832_80426380;
    extern f32 float_360_80426384;
    extern f32 float_0p5_80426388;
    extern f32 float_32_8042638c;
    extern f32 float_10_80426390;
    extern f32 float_0p9_80426394;
    extern f32 float_0p2_80426398;
    extern f32 float_0p1_8042639c;
    extern f32 float_128_804263a0;
    extern double sin(double);
    extern double cos(double);
    GreenImpactWork* part;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 i;

    part = *(GreenImpactWork**)((s32)effect + 0xC);
    base = vec3_802fc168;
    pos = *base;
    pos.x = part->x;
    pos.y = part->y;
    pos.z = part->z;
    dispPos = pos;

    part->lifetime++;
    part->timer--;
    if (part->timer < 0) {
        effDelete(effect);
        return;
    }
    if (part->lifetime >= 8) {
        part->alpha = (s32)((f32)part->alpha * float_0p8_8042637c);
    }

    part++;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part++) {
        part->delay++;
        if (part->delay >= 0) {
            if (part->delay == 0) {
                part->x = float_0_80426370;
                part->y = float_0_80426370;
                part->z = float_0_80426370;
                part->velX = -(f32)sin(
                    (part->angleZ * float_6p2832_80426380) / float_360_80426384) *
                    float_0p5_80426388;
                part->velY = (f32)cos(
                    (part->angleZ * float_6p2832_80426380) / float_360_80426384) *
                    float_0p5_80426388;
                part->unk18 = 0;
                part->scaleY = float_10_80426390;
                part->scroll0 = float_32_8042638c;
                part->scrollVel0 = float_32_8042638c;
                part->scrollVel1 = float_32_8042638c;
                part->scaleX = float_0p5_80426388;
                part->scaleZ = float_0p5_80426388;
                part->scroll1 = float_32_8042638c;
            }
            part->scroll0 += part->scrollVel0;
            part->scroll1 += part->scrollVel1;
            part->scrollVel0 *= float_0p9_80426394;
            part->scaleX += float_0p2_80426398;
            part->scaleZ += float_0p2_80426398;
            part->scaleY += (float_0p1_8042639c - part->scaleY) * float_0p2_80426398;
            if (part->scroll0 > float_128_804263a0) {
                part->scroll0 = float_128_804263a0;
            }
        }
    }
    dispEntry(4, 2, effThruHammerDisp, effect, dispCalcZ(&dispPos));
}

void effThruHammerDisp(s32 cameraId, void* effect) {
    typedef struct GXColor {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColor;
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void main_dl(void*, void*);
    extern f32 float_deg2rad_80426360;
    extern u8 unk_80429840;
    extern u8 unk_80429844;
    extern u8 unk_80429848;
    extern u8 unk_8042984c;

    f32 trans[3][4];
    f32 rot[3][4];
    f32 view[3][4];
    u8 texObj[0x20];
    GXColor call0;
    GXColor color0;
    GXColor call1;
    GXColor color1;
    GXColor call2;
    GXColor color2;
    GXColor call3;
    GXColor color3;
    u8* work;
    void* camera;
    u8 primR;
    u8 primG;
    u8 primB;
    u8 envR;
    u8 envG;
    u8 envB;
    s32 alpha;

    camera = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    primR = *(u8*)(work + 0x50);
    primG = *(u8*)(work + 0x51);
    primB = *(u8*)(work + 0x52);
    envR = *(u8*)(work + 0x53);
    envG = *(u8*)(work + 0x54);
    envB = *(u8*)(work + 0x55);

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 4, 2, 8, 0xF);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    effGetTexObjN64(0x23, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x24, texObj);
    GXLoadTexObj(texObj, 1);
    effSetVtxDescN64(stg_thruhammer_v);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79,
                float_deg2rad_80426360 * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, view);

    alpha = *(s32*)(work + 0x38);
    color0 = *(GXColor*)&unk_80429840;
    color0.r = primR;
    color0.g = primG;
    color0.b = primB;
    color0.a = alpha;
    call0 = color0;
    GXSetTevColor(1, &call0);
    color1 = *(GXColor*)&unk_80429844;
    color1.r = envR;
    color1.g = envG;
    color1.b = envB;
    call1 = color1;
    GXSetTevColor(2, &call1);
    GXSetCullMode(2);
    main_dl(effect, view);

    color2 = *(GXColor*)&unk_80429848;
    color2.r = primR;
    color2.g = primG;
    color2.b = primB;
    color2.a = alpha;
    call2 = color2;
    GXSetTevColor(1, &call2);
    color3 = *(GXColor*)&unk_8042984c;
    color3.r = envR;
    color3.g = envG;
    color3.b = envB;
    call3 = color3;
    GXSetTevColor(2, &call3);
    GXSetCullMode(1);
    main_dl(effect, view);
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
static void main_dl(void* effect, void* view) {
    typedef struct GreenImpactWork {
        u16 type;
        u8 pad02[2];
        f32 x;
        f32 y;
        f32 z;
        f32 velX;
        f32 velY;
        s32 unk18;
        f32 scaleX;
        f32 scaleY;
        f32 scaleZ;
        s32 timer;
        s32 lifetime;
        f32 angleY;
        f32 angleZ;
        s32 alpha;
        f32 scroll0;
        f32 scrollVel0;
        f32 scroll1;
        f32 scrollVel1;
        s32 delay;
        u8 prim[3];
        u8 env[3];
        u8 pad56[2];
    } GreenImpactWork;
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_0p25_80426374;
    extern f32 float_0p12903_80426378;
    GreenImpactWork* part = *(GreenImpactWork**)((s32)effect + 0xC);
    f32 trans[3][4];
    f32 scale[3][4];
    f32 rot[3][4];
    s32 i;

    part++;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part++) {
        if (part->delay >= 0) {
            PSMTXTrans(trans, part->x, part->y, part->z);
            PSMTXRotRad(rot, 0x7A, 0.017453292f * part->angleZ);
            PSMTXConcat(trans, rot, trans);
            PSMTXScale(scale, 0.3f * part->scaleX,
                       0.3f * part->scaleY,
                       0.3f * part->scaleZ);
            PSMTXConcat(trans, scale, trans);
            PSMTXRotRad(rot, 0x79, 0.017453292f * part->angleY);
            PSMTXConcat(trans, rot, trans);
            PSMTXConcat(view, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);

            PSMTXScale(scale, 0.015873f, 0.032258f, 0.0f);
            PSMTXTrans(trans, 0.0f, part->scroll0 * float_0p25_80426374, 0.0f);
            PSMTXConcat(scale, trans, scale);
            GXLoadTexMtxImm(scale, 0x1E, 1);

            PSMTXScale(scale, float_0p12903_80426378, float_0p12903_80426378, 0.0f);
            PSMTXTrans(trans, 0.0f, part->scroll1 * float_0p25_80426374, 0.0f);
            PSMTXConcat(scale, trans, scale);
            GXLoadTexMtxImm(scale, 0x21, 1);

            GXBegin(0x90, 0, 0x36);
            tri2(0, 1, 2, 0, 2, 3, 4, 0);
            tri2(2, 4, 5, 0, 1, 3, 2, 0);
            tri2(4, 6, 5, 0, 5, 6, 7, 0);
            tri2(5, 7, 8, 0, 0, 9, 1, 0);
            tri2(10, 9, 0, 0, 11, 10, 0, 0);
            tri2(11, 12, 10, 0, 13, 12, 11, 0);
            tri2(8, 14, 15, 0, 7, 14, 8, 0);
            tri2(8, 15, 16, 0, 15, 17, 16, 0);
            tri2(16, 17, 18, 0, 16, 18, 19, 0);
        }
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

static EffN64Vertex stg_thruhammer_v[20] = {
    {    -34,     -2,     20,   4267,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {    -52,     50,      0,   3840,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {    -34,     -2,    -20,   3413,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {    -52,     50,    -30,   3413,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {    -26,     50,    -45,   2987,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {      0,     -2,    -40,   2560,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {      0,     50,    -60,   2560,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     26,     50,    -45,   2133,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     34,     -2,    -20,   1707,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {    -52,     50,     30,   4267,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {    -26,     50,     45,   4693,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {      0,     -2,     40,   5120,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {      0,     50,     60,   5120,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     26,     50,     45,   5547,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     52,     50,    -30,   1707,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     52,     50,      0,   1280,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     34,     -2,     20,    853,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
    {     52,     50,     30,    853,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {     26,     50,     45,    427,   2048, 0xFF, 0xFF, 0xFF, 0x00 },
    {      0,     -2,     40,      0,   1024, 0xFF, 0xFF, 0xFF, 0xFF },
};
