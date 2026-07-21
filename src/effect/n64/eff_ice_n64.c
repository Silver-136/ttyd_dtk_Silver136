#include "effect/n64/eff_ice_n64.h"

#pragma optimize_for_size off

#pragma optimize_for_size off

void* effIceN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effIceMain(void*);
    extern s32 rand(void);
    extern char str_IceN64_802fb160[];
    extern f32 float_10_80425544;
    extern f32 float_0_8042551c;
    extern f32 float_2_80425530;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    f32 ten;
    f32 zero;
    f32 two;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_IceN64_802fb160;
    *(s32*)((s32)entry + 8) = 9;
    work = __memAlloc(3, 0x1B0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effIceMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 0x1E;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    ten = float_10_80425544;
    zero = float_0_8042551c;
    two = float_2_80425530;

    part = work + 0x30;
    for (i = 1; i < 9; i++) {
        *(f32*)(part + 4) = (f32)(rand() % 10 - 5);
        *(f32*)(part + 8) = (f32)(rand() % 10 + 5);
        *(f32*)(part + 0xC) = ten;
        *(f32*)(part + 0x18) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x20) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x1C) = (f32)((rand() % 100) / 10);
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = two;
        part += 0x30;
    }

    return entry;
}

#pragma optimize_for_size on

#pragma optimize_for_size on



void effIceMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 effTblRandN64(s32, s32);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effIceDisp(s32, s32);
    extern Vec3 vec3_802fb150[];
    extern f32 float_0p3_80425524;
    extern f32 float_0p8_80425528;
    extern f32 float_6p2832_8042552c;
    extern f32 float_2_80425530;
    extern f32 float_360_80425534;
    extern f32 float_neg0p05_80425538;
    extern f32 float_0p2_8042553c;
    extern f32 float_0p92_80425540;
    extern f32 float_10_80425544;
    extern f32 float_0p5_80425548;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;
    s32 time;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb150;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x28) -= 1;
    *(s32*)(work + 0x2C) += 1;
    time = *(s32*)(work + 0x28);
    if (time < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) < 10) {
        *(s32*)(work + 0x24) = (s32)(float_0p3_80425524 * (255 - *(s32*)(work + 0x24)) + *(s32*)(work + 0x24));
    }
    if (time < 10) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p8_80425528);
    }

    part = work + 0x30;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        *(f32*)(part + 0x14) += float_neg0p05_80425538;
        *(f32*)(part + 0x10) += float_0p2_8042553c * (f32)sin((f64)((float_6p2832_8042552c * float_2_80425530 * *(f32*)(part + 0x18)) / float_360_80425534));
        *(f32*)(part + 0x14) += float_neg0p05_80425538;
        *(f32*)(part + 0x10) *= float_0p92_80425540;
        *(f32*)(part + 0x1C) += (f32)(effTblRandN64(50, time + i * 0x14) - 25);
        *(f32*)(part + 0x18) += float_10_80425544 * (f32)sin((f64)((float_6p2832_8042552c * *(f32*)(part + 0x1C)) / float_360_80425534));
        *(f32*)(part + 0x20) += float_10_80425544 * (f32)cos((f64)((float_6p2832_8042552c * *(f32*)(part + 0x1C) * float_0p5_80425548) / float_360_80425534));
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        part += 0x30;
    }
    dispEntry(4, 2, effIceDisp, effect, dispCalcZ(&dispPos));
}

void effIceDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXSetTevColor(s32, void*);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425514;
    extern f32 float_0p015625_80425518;
    extern f32 float_0_8042551c;
    extern f32 float_0p03125_80425520;
    extern u8 ice_vtx[];
    extern u8 ice_s_vtx[];
    u8 texObj[0x20];
    Mtx base;
    Mtx rotate;
    Mtx texMtx;
    Mtx draw;
    u8* entry = (u8*)effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* part;
    u8* camera = (u8*)camGetPtr(cameraId);
    u8* camera3d;
    u32 color;
    s32 i;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 14);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    camera3d = (u8*)camGetPtr(4);
    PSMTXRotRad(rotate, float_deg2rad_80425514 * -*(f32*)(camera3d + 0x114), 'y');
    PSMTXConcat(base, rotate, draw);
    PSMTXConcat((f32 (*)[4])(camera + 0x11C), draw, draw);
    color = 0x14641400 | *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    part = work + 0x30;
    for (i = 1; i < *(s32*)(entry + 8); i++, part += 0x30) {
        void* vtx;
        if (i & 1) {
            vtx = ice_s_vtx;
            effGetTexObjN64(8, texObj);
            GXLoadTexObj(texObj, 0);
            PSMTXScale(texMtx, float_0p015625_80425518, float_0p015625_80425518, float_0_8042551c);
        } else {
            vtx = ice_vtx;
            effGetTexObjN64(7, texObj);
            GXLoadTexObj(texObj, 0);
            PSMTXScale(texMtx, float_0p03125_80425520, float_0p03125_80425520, float_0_8042551c);
        }
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        effSetVtxDescN64(vtx);
        PSMTXTrans(base, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        if (!(i & 1)) {
            PSMTXRotRad(rotate, float_deg2rad_80425514 * *(f32*)(part + 0x18), 'z');
            PSMTXConcat(base, rotate, base);
            PSMTXRotRad(rotate, float_deg2rad_80425514 * *(f32*)(part + 0x20), 'y');
            PSMTXConcat(base, rotate, base);
        }
        PSMTXConcat(draw, base, base);
        GXLoadPosMtxImm(base, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}
