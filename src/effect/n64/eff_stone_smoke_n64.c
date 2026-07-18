#include "effect/n64/eff_stone_smoke_n64.h"

typedef struct EffStoneSmokeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    f32 scale;
    s32 field_38;
} EffStoneSmokeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StoneSmokeN64_802fc0e0[];

void effStoneSmokeDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p03125_804262d0;
    extern f32 float_0_804262d4;
    extern u8 unk_80429834;

    f32 model[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    u8 color0[4];
    u8 color1[4];
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 scale = *(f32*)(work + 0x34) * *(f32*)(work + 0x38);

    PSMTXTrans(model, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(model, scaleMtx, model);
    PSMTXConcat((void*)((s32)camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    color0[0] = *(s32*)(work + 0x18);
    color0[1] = *(s32*)(work + 0x1C);
    color0[2] = *(s32*)(work + 0x20);
    color0[3] = *(s32*)(work + 0x24);
    GXSetTevColor(1, color0);
    color1[0] = *(s32*)(work + 0x28);
    color1[1] = *(s32*)(work + 0x2C);
    color1[2] = *(s32*)(work + 0x30);
    color1[3] = unk_80429834;
    GXSetTevColor(2, color1);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_804262d0, float_0p03125_804262d0,
               float_0_804262d4);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 1, 8, 0);
    GXSetTevAlphaIn(0, 0, 1, 4, 7);
    effGetTexObjN64(0x5C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A93A8);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}

void effStoneSmokeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effStoneSmokeDisp(void);
    extern Vec3 vec3_802fc0c8[];
    extern f32 float_0p6_804262d8;
    extern f32 float_0p01_804262dc;
    extern u8 scale_data[];
    extern u8 scale_data2[];
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc0c8;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
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
    frame = *(s32*)(work + 0x14);
    *(f32*)(work + 8) += float_0p6_804262d8;
    if (frame < 7) {
        *(f32*)(work + 0x38) = (f32)scale_data[frame] * float_0p01_804262dc;
    } else {
        *(f32*)(work + 0x38) = (f32)scale_data2[(frame - 7) % 7] * float_0p01_804262dc;
    }
    dispEntry(4, 2, effStoneSmokeDisp, effect, dispCalcZ(&dispPos));
}

#pragma use_lmw_stmw off
void* effStoneSmokeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStoneSmokeWork* work;
    s32 zero;
    s32 full;
    s32 ten;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StoneSmokeN64_802fc0e0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffStoneSmokeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStoneSmokeMain;
    *(u32*)entry |= 2;

    zero = 0;
    work->type = type;
    work->frame = zero;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    full = 0xFF;
    ten = 10;
    work->a0 = full;
    half = 0x78;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = full;
    work->field_28 = ten;
    work->field_2C = ten;
    work->field_30 = half;
    return entry;
}
#pragma use_lmw_stmw on
