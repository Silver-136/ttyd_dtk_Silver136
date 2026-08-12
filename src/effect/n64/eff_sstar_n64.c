#include "effect/n64/eff_sstar_n64.h"

void* effSstarN64Entry(f32 x, f32 y, f32 z, f32 dstX, f32 dstY, f32 dstZ, f32 speed, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effSstarMain(void*);
    extern u8 vec3_802fbfe0[];
    extern s32 rand(void);
    extern f64 __frsqrte(f64);
    extern f64 cos(f64);
    extern f64 sin(f64);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern f32 float_0_8042610c, float_0p2_80426144, float_0p5_80426148;
    extern f32 float_6p2832_8042614c, float_360_80426150;
    extern f32 float_neg20_80426154, float_20_80426158;
    void* entry;
    u8* work;
    u8* camera;
    f32 dx, dy, dz, len, invLen;
    f32 radians, cs, sn, projectedX, projectedZ, projectedLen;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = (char*)vec3_802fbfe0 + 0x28;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x25C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSstarMain;
    *(s32*)work = 1;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x38) = type;
    *(f32*)(work + 0x1C) = float_0_8042610c;
    *(f32*)(work + 0x24) = float_0_8042610c;
    *(f32*)(work + 0x28) = float_0_8042610c;
    if (type == 3) {
        s32 first;
        s32 second;
        *(f32*)(work + 0x1C0) = float_0p2_80426144;
        first = rand() % 256;
        second = rand() % (255 - first);
        *(s32*)(work + 0x1C4) = first;
        *(s32*)(work + 0x1C8) = second;
        *(s32*)(work + 0x1CC) = 255 - first - second;
    } else {
        *(f32*)(work + 0x1C0) = float_0p5_80426148;
        *(s32*)(work + 0x1C4) = 0xD2;
        *(s32*)(work + 0x1C8) = 0xBA;
        *(s32*)(work + 0x1CC) = 0x5A;
    }

    dx = dstX - x;
    dy = dstY - y;
    dz = dstZ - z;
    len = dx * dx + dy * dy + dz * dz;
    if (len > float_0_8042610c) {
        f64 estimate = __frsqrte(len);
        f64 half = *(f64*)(vec3_802fbfe0 + 0x10);
        f64 three = *(f64*)(vec3_802fbfe0 + 0x18);
        estimate = half * estimate * (three - len * estimate * estimate);
        estimate = half * estimate * (three - len * estimate * estimate);
        estimate = half * estimate * (three - len * estimate * estimate);
        len = (f32)(len * estimate);
    }
    invLen = speed / len;
    *(f32*)(work + 0x10) = dx * invLen;
    *(f32*)(work + 0x14) = dy * invLen;
    *(f32*)(work + 0x18) = dz * invLen;

    camera = camGetPtr(4);
    radians = float_6p2832_8042614c * *(f32*)(camera + 0x114) / float_360_80426150;
    cs = (f32)cos(radians);
    sn = (f32)sin(radians);
    projectedX = -cs * *(f32*)(work + 0x10);
    projectedZ = -sn * *(f32*)(work + 0x18);
    projectedLen = projectedX * projectedX + projectedZ * projectedZ;
    if (projectedLen > float_0_8042610c) {
        f64 estimate = __frsqrte(projectedLen);
        f64 half = *(f64*)(vec3_802fbfe0 + 0x10);
        f64 three = *(f64*)(vec3_802fbfe0 + 0x18);
        estimate = half * estimate * (three - projectedLen * estimate * estimate);
        estimate = half * estimate * (three - projectedLen * estimate * estimate);
        estimate = half * estimate * (three - projectedLen * estimate * estimate);
        projectedLen = (f32)(projectedLen * estimate);
    }
    if (projectedX + projectedZ >= float_0_8042610c) {
        *(f32*)(work + 0x2C) = float_20_80426158;
    } else {
        projectedLen = -projectedLen;
        *(f32*)(work + 0x2C) = float_neg20_80426154;
    }
    *(f32*)(work + 0x24) = angleABf(float_0_8042610c, float_0_8042610c,
                                     -*(f32*)(work + 0x14), -projectedLen);
    *(f32*)(work + 0x20) = *(f32*)(work + 0x24);
    *(f32*)(work + 0x34) = -projectedLen;
    *(s32*)(work + 0x30) = (s32)(len / speed);
    for (i = 0; i < 8; i++) {
        PSMTXTrans(work + 0x40 + i * 0x30, x, y, z);
    }
    *(s32*)(work + 0x3C) = -1;
    return entry;
}

u8 effSstarMain(int* effect) {
    extern void* marioGetPtr(void);
    extern void* gpGlobals;
    extern s32 hitCheckFilter(f64, f64, f64, f64, f64, f64, s32,
                              f32*, f32*, f32*, f32*, f32*, f32*, f32*);
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSstarDisp(void);
    extern f64 sqrt(f64);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 float_0p01_80426118;
    extern f32 float_1_80426114;
    extern f32 float_32_8042611c;
    extern f32 FLOAT_80426120;
    extern f32 float_42_80426128;
    extern f32 float_0p6_8042612c;
    extern f32 float_0p7_80426130;
    extern f32 float_10_80426134;
    extern f32 float_neg0p5_80426138;
    extern f32 float_300_8042613c;
    extern f32 float_neg300_80426140;
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* mario = marioGetPtr();
    f32 oldPos[3];
    f32 length;
    f32 inverse;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 distance;
    f32 outNX;
    f32 outNY;
    f32 outNZ;

    oldPos[0] = *(f32*)(work + 4);
    oldPos[1] = *(f32*)(work + 8);
    oldPos[2] = *(f32*)(work + 0xC);
    if (*(s32*)(work + 0x38) > 1) {
        nx = *(f32*)(work + 0x10);
        ny = *(f32*)(work + 0x14);
        nz = *(f32*)(work + 0x18);
        length = nx * nx + ny * ny + nz * nz;
        inverse = length;
        if (length > float_0p01_80426118) {
            inverse = float_1_80426114 / (f32)sqrt((f64)length);
        }
        nx *= inverse;
        ny *= inverse;
        nz *= inverse;
        distance = FLOAT_80426120;
        if (*(s32*)work != 0 && *(f32*)(work + 0x14) < 0.0f &&
            hitCheckFilter(*(f32*)(work + 4) - float_32_8042611c * nx,
                           *(f32*)(work + 8) - float_32_8042611c * ny,
                           *(f32*)(work + 0xC) - float_32_8042611c * nz,
                           0.0, -1.0, 0.0, 0,
                           &outX, &outY, &outZ, &distance,
                           &outNX, &outNY, &outNZ) != 0 &&
            distance < float_42_80426128) {
            *(f32*)(work + 0x14) = -*(f32*)(work + 0x14) * float_0p6_8042612c;
            *(f32*)(work + 0x10) *= float_0p7_80426130;
            *(f32*)(work + 0x34) *= float_0p7_80426130;
            *(f32*)(work + 0x2C) *= float_0p7_80426130;
            *(f32*)(work + 0x1C) += float_1_80426114;
            *(s32*)work = 0;
            if (*(f32*)(work + 0x1C) >= float_10_80426134) {
                *(s32*)(work + 0x30) = -1;
            }
        }
        if (*(f32*)(work + 0x1C) != 0.0f) {
            *(f32*)(work + 0x14) += float_neg0p5_80426138;
            *(f32*)(work + 0x24) += *(f32*)(work + 0x2C);
            *(f32*)(work + 0x20) =
                angleABf(0.0f, 0.0f, -*(f32*)(work + 0x14), *(f32*)(work + 0x34));
        }
    }
    if (*(s32*)((u8*)gpGlobals + 0x14) == 0) {
        if (*(f32*)(mario + 0x90) - *(f32*)(work + 8) > float_300_8042613c) {
            *(s32*)(work + 0x30) = -1;
        }
    } else if (*(f32*)(work + 8) < float_neg300_80426140) {
        *(s32*)(work + 0x30) = -1;
    }
    *(f32*)(work + 4) += *(f32*)(work + 0x10);
    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effSstarDisp, effect, dispCalcZ(oldPos));
    }
    return 0;
}

const f32 vec3_802fbfe0[3] = { 0.0f, 0.0f, 0.0f };
const char str_SstarN64_802fc008[] = "SstarN64";


u8 effSstarDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    u32 color;
    s32 type = *(s32*)(work + 0x38);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXScale(scale, *(f32*)(work + 0x1C0), *(f32*)(work + 0x1C0),
               *(f32*)(work + 0x1C0));
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(model, scale, model);
    PSMTXRotRad(rot, *(f32*)(work + 0x24) * 0.017453292f, 'z');
    PSMTXConcat(model, rot, model);
    GXSetCullMode(0);
    color = (*(u8*)(work + 0x1C4) << 24) | (*(u8*)(work + 0x1C8) << 16) |
            (*(u8*)(work + 0x1CC) << 8) | 0xFF;
    GXSetTevColor(1, &color);
    color = 0x7F7F7F7F;
    GXSetTevColor(2, &color);
    color = 0x50505050;
    GXSetTevColor(3, &color);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObj(9, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 8, 6, 2);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, -1);
    GXSetTevColorOp(1, 1, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 4, 6, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 3, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 0, 10, 7);
    effSetVtxDescN64((void*)0x803A8C70);
    PSMTXConcat(camera + 0x11C, model, trans);
    GXLoadPosMtxImm(trans, 0);
    GXBegin(0x90, 0, 6);
    tri2(0,1,2,0,0,2,3,0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 1);
    GXSetTevAlphaIn(0, 7, 10, 6, 7);
    effSetVtxDescN64((void*)0x803A8CA8);
    GXBegin(0x90, 0, 0xC);
    tri2(0,1,4,0,1,2,4,0);
    tri2(0,4,3,0,4,2,3,0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 1);
    GXSetTevAlphaIn(0, 7, 10, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObj(type == 3 ? 0xB : 0xA, &tex);
    GXLoadTexObj(&tex, 0);
    GXBegin(0x90, 0, 6);
    return 0;
}
