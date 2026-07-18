#include "effect/n64/eff_flame_n64.h"


void effFlameDisp(int cameraId, int effectAddress) {
    typedef float Mtx[3][4];
    typedef struct Vec { float x, y, z; } Vec;
    extern void* camGetPtr(int);
    extern void GXSetNumChans(int);
    extern void GXSetNumTexGens(int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXMultVec(Mtx, Vec*, Vec*);
    extern void* memcpy(void*, const void*, unsigned int);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXSetCullMode(int);
    extern void GXInvalidateVtxCache(void);
    extern void DCFlushRange(void*, unsigned int);
    extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(int, void*);
    extern void GXLoadTexMtxImm(Mtx, int, int);
    extern void GXBegin(int, int, int);
    extern void tri2(int, int, int, int, int, int, int);
    extern float float_deg2rad_804252e0;
    extern float float_1_804252e4;
    extern float float_10_804252e8;
    extern float float_0_804252ec;
    extern float float_0p03125_804252f0;
    extern float float_0p0078125_804252f4;
    extern float float_30_804252f8;
    extern unsigned char stg_flame_v[], DAT_8039fd96[], DAT_8039fdc0[], DAT_8039fdce[];
    extern unsigned char stg_flame_3_v[], DAT_8039fe76[], DAT_8039fea0[], DAT_8039feae[];
    extern unsigned char DAT_8039fda4[], DAT_8039fdb2[], DAT_8039fddc[], DAT_8039fdea[];
    unsigned char texObj[0x20];
    Mtx model;
    Mtx transform;
    Mtx scale;
    Mtx rotation;
    unsigned char* work = *(unsigned char**)(effectAddress + 0xC);
    unsigned char* camera = (unsigned char*)camGetPtr(cameraId);
    unsigned char* camera3d;
    int type = *(int*)work;
    unsigned int color1;
    unsigned int color2;
    const unsigned char* sources[4];
    int offsets[4];
    int i;

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    if (type == 2) {
        GXSetNumTevStages(5);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 3);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
        GXSetTevColorIn(0, 15, 15, 15, 8);
        GXSetTevAlphaIn(0, 7, 7, 7, 4);
        GXSetTevOrder(1, 1, 1, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 0, 0);
        GXSetTevColorIn(1, 15, 2, 8, 2);
        GXSetTevAlphaIn(1, 7, 6, 4, 3);
        GXSetTevOrder(2, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(2, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(2, 1, 0, 0, 1, 0);
        GXSetTevColorIn(2, 15, 15, 15, 0);
        GXSetTevAlphaIn(2, 7, 5, 6, 0);
        GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(3, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(3, 0, 0, 0, 0, 0);
        GXSetTevColorIn(3, 15, 15, 15, 0);
        GXSetTevAlphaIn(3, 7, 0, 4, 7);
        GXSetTevOrder(4, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(4, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(4, 1, 0, 0, 1, 0);
        GXSetTevColorIn(4, 15, 15, 15, 0);
        GXSetTevAlphaIn(4, 7, 5, 4, 0);
        effGetTexObjN64(0x31, texObj);
        GXLoadTexObj(texObj, 0);
        effGetTexObjN64(0x32, texObj);
        GXLoadTexObj(texObj, 1);
    } else if (type >= 0 && type < 2) {
        GXSetNumTevStages(4);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 3);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
        GXSetTevColorIn(0, 15, 15, 15, 8);
        GXSetTevAlphaIn(0, 7, 7, 7, 4);
        GXSetTevOrder(1, 1, 1, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 0, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 15, 10, 10, 12);
        GXSetTevAlphaIn(1, 4, 7, 6, 4);
        GXSetTevOrder(2, 1, 1, 0xFF);
        GXSetTevColorOp(2, 1, 0, 0, 1, 0);
        GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
        GXSetTevColorIn(2, 15, 10, 8, 0);
        GXSetTevAlphaIn(2, 7, 6, 0, 7);
        GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(3, 1, 0, 0, 1, 0);
        GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
        GXSetTevColorIn(3, 15, 2, 0, 1);
        GXSetTevAlphaIn(3, 7, 4, 0, 7);
        effGetTexObjN64(0x2F, texObj);
        GXLoadTexObj(texObj, 0);
        effGetTexObjN64(0x30, texObj);
        GXLoadTexObj(texObj, 1);
    }

    PSMTXTrans(transform, *(float*)(work + 4), *(float*)(work + 8), *(float*)(work + 0xC));
    if (type == 2) {
        PSMTXRotRad(rotation, float_deg2rad_804252e0 * *(float*)(work + 0x64), 'y');
    } else {
        camera3d = (unsigned char*)camGetPtr(4);
        PSMTXRotRad(rotation, float_deg2rad_804252e0 * -*(float*)(camera3d + 0x114), 'y');
    }
    PSMTXConcat(transform, rotation, model);
    PSMTXScale(scale, *(float*)(work + 0x1C), *(float*)(work + 0x20), float_1_804252e4);
    PSMTXConcat(model, scale, model);
    if (type == 0) {
        PSMTXTrans(transform,
            float_10_804252e8 * (-(*(float*)(work + 0x1C) - *(float*)(work + 0x24)) / *(float*)(work + 0x24)),
            float_0_804252ec, float_0_804252ec);
        PSMTXConcat(model, transform, model);
    }
    offsets[0] = 0x70;
    offsets[1] = 0x7E;
    offsets[2] = 0xA8;
    offsets[3] = 0xB6;
    if (type == 1) {
        sources[0] = stg_flame_3_v;
        sources[1] = DAT_8039fe76;
        sources[2] = DAT_8039fea0;
        sources[3] = DAT_8039feae;
    } else if (type == 0) {
        sources[0] = stg_flame_v;
        sources[1] = DAT_8039fd96;
        sources[2] = DAT_8039fdc0;
        sources[3] = DAT_8039fdce;
    } else {
        sources[0] = DAT_8039fda4;
        sources[1] = DAT_8039fdb2;
        sources[2] = DAT_8039fddc;
        sources[3] = DAT_8039fdea;
    }
    for (i = 0; i < 4; i++) {
        short* packed = (short*)(work + offsets[i]);
        Vec vertexPos;
        memcpy(packed, sources[i], 0xE);
        vertexPos.x = (float)packed[0];
        vertexPos.y = (float)packed[1];
        vertexPos.z = (float)packed[2];
        PSMTXMultVec(model, &vertexPos, &vertexPos);
        packed[0] = (short)vertexPos.x;
        packed[1] = (short)vertexPos.y;
        packed[2] = (short)vertexPos.z;
    }
    GXInvalidateVtxCache();
    DCFlushRange(work + 0x70, 0x70);
    GXSetCullMode(0);
    effSetVtxDescN64(work + 0x70);
    GXLoadPosMtxImm((float (*)[4])(camera + 0x11C), 0);
    GXSetCurrentMtx(0);
    color1 = ((unsigned int)*(unsigned char*)(work + 0x68) << 24) |
             ((unsigned int)*(unsigned char*)(work + 0x69) << 16) |
             ((unsigned int)*(unsigned char*)(work + 0x6A) << 8) |
             *(unsigned char*)(work + 0x28);
    color2 = *(unsigned int*)(work + 0x6B);
    GXSetTevColor(1, &color1);
    GXSetTevColor(2, &color2);
    PSMTXScale(scale, float_0p03125_804252f0, float_0p03125_804252f0, float_0_804252ec);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    PSMTXScale(scale, float_0p03125_804252f0, float_0p0078125_804252f4, float_0_804252ec);
    PSMTXTrans(transform, *(float*)(work + 0x34), *(float*)(work + 0x40), float_0_804252ec);
    PSMTXConcat(scale, transform, transform);
    GXLoadTexMtxImm(transform, 0x21, 1);
    GXBegin(0x90, 0, 6);
    tri2(4, 5, 6, 0, 4, 6, 7);
    PSMTXScale(scale, float_0p03125_804252f0, float_0p0078125_804252f4, float_0_804252ec);
    PSMTXTrans(transform, float_30_804252f8 + *(float*)(work + 0x4C),
        float_30_804252f8 + *(float*)(work + 0x58), float_0_804252ec);
    PSMTXConcat(scale, transform, transform);
    GXLoadTexMtxImm(transform, 0x21, 1);
    if (type >= 0 && type < 2) {
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    } else {
        GXBegin(0x90, 0, 6);
        tri2(2, 1, 0, 0, 3, 2, 0);
    }
}

void effFlameMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFlameDisp(void);
    extern f32 float_0_804252ec;
    extern f32 float_0p5_804252fc;
    extern f32 float_0p02_80425300;
    extern f32 float_0p04_80425304;
    extern f32 float_64_80425308;
    extern f32 float_128_8042530c;
    extern f32 float_0p1_80425310;

    u8* work;
    f32 pos[3];
    s32 timer;
    s32 frame;
    f32 value;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x2C) = 0x10;
    }

    (*(s32*)(work + 0x30))++;
    if (*(s32*)(work + 0x30) > 0x4F1A0) {
        *(s32*)(work + 0x30) = 0x100;
    }
    if (*(s32*)(work + 0x2C) < 0x11) {
        (*(s32*)(work + 0x2C))--;
    }

    timer = *(s32*)(work + 0x2C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x30);
    if (*(s32*)work < 2) {
        if (frame < 0xB) {
            *(s32*)(work + 0x28) = (s32)(float_0p5_804252fc * (f32)(0x80 - *(s32*)(work + 0x28)) +
                                        (f32)*(s32*)(work + 0x28));
        }
    } else if (frame < 0xB) {
        *(s32*)(work + 0x28) = ((frame * 0xFF) / 10) + (((frame * 0xFF) / 10) >> 0x1F);
        *(s32*)(work + 0x28) -= (*(s32*)(work + 0x28) >> 0x1F);
    }

    if (timer < 0x10) {
        *(s32*)(work + 0x28) = timer << 4;
    }

    if (*(s32*)work == 0) {
        *(f32*)(work + 0x38) += float_0p02_80425300 * (*(f32*)(work + 0x3C) - *(f32*)(work + 0x38));
        *(f32*)(work + 0x44) += float_0p02_80425300 * (*(f32*)(work + 0x48) - *(f32*)(work + 0x44));
        *(f32*)(work + 0x1C) += float_0p04_80425304 * (*(f32*)(work + 0x24) - *(f32*)(work + 0x1C));
        *(f32*)(work + 0x20) += float_0p04_80425304 * (*(f32*)(work + 0x24) - *(f32*)(work + 0x20));
    } else {
        *(f32*)(work + 0x38) += float_0p02_80425300 * (*(f32*)(work + 0x3C) - *(f32*)(work + 0x38));
        *(f32*)(work + 0x44) += float_0p02_80425300 * (*(f32*)(work + 0x48) - *(f32*)(work + 0x44));
        *(f32*)(work + 0x50) += float_0p02_80425300 * (*(f32*)(work + 0x54) - *(f32*)(work + 0x50));
        *(f32*)(work + 0x5C) += float_0p02_80425300 * (*(f32*)(work + 0x60) - *(f32*)(work + 0x5C));
    }

    *(f32*)(work + 0x34) += *(f32*)(work + 0x38);
    value = *(f32*)(work + 0x34);
    if (float_0_804252ec <= value) {
        if (float_64_80425308 < value) {
            *(f32*)(work + 0x34) = value - float_64_80425308;
        }
    } else {
        *(f32*)(work + 0x34) = value + float_64_80425308;
    }

    *(f32*)(work + 0x40) += *(f32*)(work + 0x44);
    value = *(f32*)(work + 0x40);
    if (float_0_804252ec <= value) {
        if (float_128_8042530c < value) {
            *(f32*)(work + 0x40) = value - float_128_8042530c;
        }
    } else {
        *(f32*)(work + 0x40) = value + float_128_8042530c;
    }

    *(f32*)(work + 0x4C) += *(f32*)(work + 0x50);
    value = *(f32*)(work + 0x4C);
    if (float_0_804252ec <= value) {
        if (float_64_80425308 < value) {
            *(f32*)(work + 0x4C) = value - float_64_80425308;
        }
    } else {
        *(f32*)(work + 0x4C) = value + float_64_80425308;
    }

    *(f32*)(work + 0x58) += *(f32*)(work + 0x5C);
    value = *(f32*)(work + 0x58);
    if (float_0_804252ec <= value) {
        if (float_128_8042530c < value) {
            *(f32*)(work + 0x58) = value - float_128_8042530c;
        }
    } else {
        *(f32*)(work + 0x58) = value + float_128_8042530c;
    }

    *(f32*)(work + 0x10) += float_0p1_80425310 * (*(f32*)(work + 4) - *(f32*)(work + 0x10));
    *(f32*)(work + 0x14) += float_0p1_80425310 * (*(f32*)(work + 8) - *(f32*)(work + 0x14));
    *(f32*)(work + 0x18) += float_0p1_80425310 * (*(f32*)(work + 0xC) - *(f32*)(work + 0x18));

    dispEntry(4, 2, effFlameDisp, effect, dispCalcZ(pos));
}

void* effFlameN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFlameMain(void* effect);
    extern char str_FlameN64_802fafa0[];
    extern f32 float_0_804252ec;
    extern f32 float_2_80425314;
    extern f32 float_0p6_80425318;
    extern f32 float_neg0p9_8042531c;
    extern f32 float_0p9_80425320;
    extern f32 float_0p04_80425304;
    extern f32 float_0p2_80425324;
    extern f32 float_1p3_80425328;
    extern f32 float_0p16_8042532c;
    extern f32 float_1p04_80425330;
    extern f32 float_0p1_80425310;
    extern f32 float_1p7_80425334;
    extern f32 float_0p08_80425338;
    extern f32 float_1p36_8042533c;

    void* entry;
    u8* work;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FlameN64_802fafa0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0xE0);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFlameMain;
    *(u32*)entry |= 2;

    zero = float_0_804252ec;
    *(s32*)work = type;
    *(s32*)(work + 0x2C) = 100;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x28) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x64) = zero;
    *(f32*)(work + 0x40) = zero;
    *(f32*)(work + 0x34) = zero;
    *(f32*)(work + 0x58) = zero;
    *(f32*)(work + 0x4C) = zero;

    if (type == 1) {
        *(u8*)(work + 0x68) = 0x4B;
        *(u8*)(work + 0x69) = 0x4B;
        *(u8*)(work + 0x6A) = 0x4B;
        *(u8*)(work + 0x6B) = 0xFF;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0;
        *(u8*)(work + 0x6E) = 0;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p04_80425304;
        *(f32*)(work + 0x48) = float_0p9_80425320;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p04_80425304;
        *(f32*)(work + 0x60) = float_0p9_80425320;
    } else if (type < 1) {
        if (type > -1) {
            *(u8*)(work + 0x68) = 0x4B;
            *(u8*)(work + 0x69) = 0x4B;
            *(u8*)(work + 0x6A) = 0x4B;
            *(u8*)(work + 0x6B) = 0xFF;
            *(u8*)(work + 0x6C) = 0xF7;
            *(u8*)(work + 0x6D) = 0x9B;
            *(u8*)(work + 0x6E) = 0;
            *(f32*)(work + 0x1C) = float_2_80425314 * scale;
            *(f32*)(work + 0x20) = float_0p6_80425318 * scale;
            *(f32*)(work + 0x24) = scale;
            *(f32*)(work + 0x38) = float_neg0p9_8042531c;
            *(f32*)(work + 0x44) = float_0p04_80425304;
            *(f32*)(work + 0x3C) = float_0p04_80425304;
            *(f32*)(work + 0x48) = float_0p9_80425320;
            *(f32*)(work + 0x50) = float_neg0p9_8042531c;
            *(f32*)(work + 0x5C) = float_0p04_80425304;
            *(f32*)(work + 0x54) = float_0p04_80425304;
            *(f32*)(work + 0x60) = float_0p9_80425320;
        }
    } else if (type < 3) {
        *(u8*)(work + 0x68) = 0x19;
        *(u8*)(work + 0x69) = 0x79;
        *(u8*)(work + 0x6A) = 0x8E;
        *(u8*)(work + 0x6B) = 0x22;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0xFF;
        *(u8*)(work + 0x6E) = 0x51;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p2_80425324;
        *(f32*)(work + 0x48) = float_1p3_80425328;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p16_8042532c;
        *(f32*)(work + 0x60) = float_1p04_80425330;
    } else {
        *(u8*)(work + 0x68) = 0xB3;
        *(u8*)(work + 0x69) = 0xA9;
        *(u8*)(work + 0x6A) = 0x55;
        *(u8*)(work + 0x6B) = 0x22;
        *(u8*)(work + 0x6C) = 0;
        *(u8*)(work + 0x6D) = 0xFF;
        *(u8*)(work + 0x6E) = 0x3F;
        *(f32*)(work + 0x1C) = scale;
        *(f32*)(work + 0x20) = scale;
        *(f32*)(work + 0x24) = scale;
        *(f32*)(work + 0x38) = zero;
        *(f32*)(work + 0x44) = float_0p04_80425304;
        *(f32*)(work + 0x3C) = float_0p1_80425310;
        *(f32*)(work + 0x48) = float_1p7_80425334;
        *(f32*)(work + 0x50) = zero;
        *(f32*)(work + 0x5C) = float_0p04_80425304;
        *(f32*)(work + 0x54) = float_0p08_80425338;
        *(f32*)(work + 0x60) = float_1p36_8042533c;
    }

    *(f32*)(work + 0x10) = *(f32*)(work + 4);
    *(f32*)(work + 0x14) = *(f32*)(work + 8);
    *(f32*)(work + 0x18) = *(f32*)(work + 0xC);
    return entry;
}

