#include "effect/n64/eff_pokopi_thunder_n64.h"


u8 effPokopiThunderDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void* smartAlloc(u32, s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx scale, model;
    f32* vertices = smartAlloc(0x480, 3);
    u32 color;
    s32 i;

    PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(camera + 0x118, scale, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    color = (*(u8*)(work + 0x30) << 24) | (*(u8*)(work + 0x34) << 16) |
            (*(u8*)(work + 0x38) << 8) | *(u8*)(work + 0x3C);
    GXSetTevColor(1, &color);
    color = (*(u8*)(work + 0x40) << 24) | (*(u8*)(work + 0x44) << 16) |
            (*(u8*)(work + 0x48) << 8) | 0xFF;
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(4);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
    GXSetTevColorIn(0, 15, 8, 8, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, -1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 15);
    GXSetTevAlphaIn(1, 6, 7, 5, 7);
    GXSetTevOrder(2, 1, 1, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 6, 4, 0);
    GXSetTevOrder(3, 0xFF, 0xFF, -1);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevColorIn(3, 15, 15, 15, 0);
    GXSetTevAlphaIn(3, 7, 5, 0, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    PSMTXScale(scale, 0.0078125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXLoadTexMtxImm(scale, 0x21, 1);
    effGetTexObj(0x54, &tex);
    GXLoadTexObj(&tex, 0);
    GXLoadTexObj(&tex, 1);
    GXSetCullMode(0);
    for (i = 0; i < 12; i++) {
        f32 t = (f32)i / 11.0f;
        vertices[i * 3] = *(f32*)(work + 0x10) + t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10));
        vertices[i * 3 + 1] = *(f32*)(work + 0x14) + t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14));
        vertices[i * 3 + 2] = *(f32*)(work + 0x18) + t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
    }
    for (i = 0; i < 11; i++) {
        GXBegin(0x90, 0, 6);
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPokopiThunderN64Entry(s32 type, s32 time, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effPokopiThunderMain(void*);
    extern s32 rand(void);
    extern f64 sqrt(f64);
    extern const char str_PokopiThunderN64_802fbd48[];
    extern f32 float_0_80425db0;
    extern f32 float_0p005_80425ddc;
    void* entry;
    u8* work;
    s32 camera;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    entry = effEntry();
    camera = 4;
    if (type >= 10) {
        camera = 8;
    }
    *(const char**)((s32)entry + 0x14) = str_PokopiThunderN64_802fbd48;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x11C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPokopiThunderMain;
    *(u32*)entry |= 2;

    *(s32*)work = type % 10;
    *(s32*)(work + 0x2C) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x28) = 0x3E8;
    } else {
        *(s32*)(work + 0x28) = time;
    }
    *(s32*)(work + 0x3C) = 0xFF;
    *(f32*)(work + 0x10) = x0;
    *(f32*)(work + 0x14) = y0;
    *(f32*)(work + 0x18) = z0;
    *(f32*)(work + 0x1C) = x1;
    *(f32*)(work + 0x20) = y1;
    *(f32*)(work + 0x24) = z1;
    *(f32*)(work + 4) = x0;
    *(f32*)(work + 8) = y0;
    *(f32*)(work + 0xC) = z0;
    *(f32*)(work + 0x4C) = scale;
    *(s32*)(work + 0x118) = camera;

    dx = x0 - x1;
    dy = y0 - y1;
    dz = z0 - z1;
    dist = (f32)sqrt(dx * dx + dy * dy + dz * dz);
    if (dist == float_0_80425db0) {
        return 0;
    }
    *(f32*)(work + 0x114) = dist * float_0p005_80425ddc;
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0xDC;
    *(s32*)(work + 0x38) = 0x14;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x110) = rand() % 0x168;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiThunderMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiThunderDisp(void);
    extern const Vec3 vec3_802fbd28;
    extern f32 float_0p5_80425dd4;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbd28;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x10;
    }
    if (*(s32*)(work + 0x28) < 0x3E8) {
        *(s32*)(work + 0x28) -= 1;
    }
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    type = *(s32*)work;
    frame = *(s32*)(work + 0x2C);
    if (type == 2) {
        if (frame >= 0x28) {
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
        }
    } else if (frame >= 0xB) {
        *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
    }
    dispEntry(*(s32*)(work + 0x118), 2, effPokopiThunderDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

