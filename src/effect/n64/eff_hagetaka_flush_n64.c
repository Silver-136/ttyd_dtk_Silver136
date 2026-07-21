#include "effect/n64/eff_hagetaka_flush_n64.h"
void effHagetakaFlushMain(void* effEntry);
u8 effHagetakaFlushDisp(s32 cameraId, void* effect);

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHagetakaFlushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 colorR;
    s32 colorG;
    s32 colorB;
    s32 alpha;
    f32 scale;
    f32 scale2;
} EffHagetakaFlushWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fb070;
extern char str_HagetakaFlushN64_802fb088[];
extern f32 float_0_80425448;
extern f32 float_0p5_8042544c;
extern f32 float_0p3_80425450;
extern f32 float_0p2_80425454;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effHagetakaFlushN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHagetakaFlushWork* work;
    s32 flags;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_HagetakaFlushN64_802fb088;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x30);
    *(EffHagetakaFlushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHagetakaFlushMain;
    flags = *(s32*)entry;
    zero = float_0_80425448;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    work->timer = 10;
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->scale2 = zero;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effHagetakaFlushMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHagetakaFlushWork* work;
    s32 flags;
    s32 frame;

    work = *(EffHagetakaFlushWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb070;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if (flags & 4) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 10;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    if (work->timer < 0) {
        effDelete(effEntry);
    } else {
        frame = work->frame;
        if (work->timer < 4) {
            work->scale2 *= float_0p5_8042544c;
        } else {
            work->scale2 = float_0p3_80425450 + (float_0p2_80425454 * (f32)frame);
        }
        dispEntry(4, 2, effHagetakaFlushDisp, effEntry, dispCalcZ(&dispPos));
    }
}


u8 effHagetakaFlushDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425440;
    extern f32 float_0p03125_80425444;
    extern f32 float_0_80425448;
    u8 texObj[0x20];
    Mtx model;
    Mtx rotation;
    Mtx scale;
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    u8* camera3d = (u8*)camGetPtr(4);
    f32 size = *(f32*)(work + 0x28) * *(f32*)(work + 0x2C);
    u32 color;

    PSMTXTrans(model, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rotation, float_deg2rad_80425440 * -*(f32*)(camera3d + 0x114), 'y');
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(model, rotation, model);
    PSMTXConcat(model, scale, model);
    PSMTXConcat((f32 (*)[4])(camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 2);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80425444, float_0p03125_80425444, float_0_80425448);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObjN64(0x1C, texObj);
    GXLoadTexObj(texObj, 0);
    color = ((u32)*(u8*)(work + 0x18) << 24) |
            ((u32)*(u8*)(work + 0x1C) << 16) |
            ((u32)*(u8*)(work + 0x20) << 8) |
            *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A0420);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3);
    return 0;
}
