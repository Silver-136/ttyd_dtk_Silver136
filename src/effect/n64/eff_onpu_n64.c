#include "effect/n64/eff_onpu_n64.h"


void effOnpuDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_0p0625_80425b10, float_0p0044643_80425b14;
    extern f32 float_0_80425b18, float_deg2rad_80425b1c;
    extern u8 size32x16_tex64x32_vtx[];
    f32 trans[3][4], rot[3][4], scale[3][4];
    u8 texObj[0x20];
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    u32 color;

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    PSMTXScale(scale, float_0p0625_80425b10, float_0p0044643_80425b14, float_0_80425b18);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXLoadTexMtxImm(scale, 0x21, 1);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 0, 8, 0);
    GXSetTevAlphaIn(0, 0, 0, 0, 7);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 0, 0, 0);
    GXSetTevAlphaIn(1, 0, 1, 7, 7);
    color = *(u32*)(work + 0x14);
    GXSetTevColor(1, &color);
    effGetTexObjN64(0x28, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x29, texObj);
    GXLoadTexObj(texObj, 1);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80425b1c * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXScale(scale, *(f32*)(work + 0x10), *(f32*)(work + 0x10), float_0_80425b18);
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x16_tex64x32_vtx);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effOnpuN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOnpuMain(void*);
    extern s32 rand(void);
    extern char str_OnpuN64_802fbb40[];
    extern f32 float_0_80425b18;
    extern f32 float_1_80425b24;
    extern f32 float_neg5_80425b2c;
    extern f32 float_0p1_80425b30;
    extern f32 float_0p5_80425b34;
    extern f32 float_5_80425b38;
    void* entry;
    u8* work;
    s32 r;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OnpuN64_802fbb40;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOnpuMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = float_0_80425b18;
    *(s32*)(work + 0x18) = 0x40;
    *(s32*)(work + 0x14) = 0;
    *(s32*)(work + 0x1C) = 0;
    *(s32*)(work + 0x20) = rand() % 7;

    switch (type) {
        case 0:
            *(f32*)(work + 0x24) = float_0_80425b18;
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x28) = float_1_80425b24;
            *(f32*)(work + 0x30) = float_1_80425b24;
            break;
        case 1:
            r = rand() % 11;
            *(f32*)(work + 0x24) = float_neg5_80425b2c + (float_0p1_80425b30 * (f32)r);
            r = rand() % 11;
            *(f32*)(work + 0x28) = float_0p5_80425b34 + (float_0p1_80425b30 * (f32)r);
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x30) = float_0_80425b18;
            break;
        default:
            r = rand() % 11;
            *(f32*)(work + 0x24) = float_5_80425b38 - (float_0p1_80425b30 * (f32)r);
            r = rand() % 11;
            *(f32*)(work + 0x28) = float_1_80425b24 + (float_0p1_80425b30 * (f32)r);
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x30) = float_0_80425b18;
            break;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effOnpuMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOnpuDisp(s32, s32);
    extern const Vec3 vec3_802fbb28;
    extern f32 float_0p3_80425b20;
    extern f32 float_1_80425b24;
    extern f32 float_0p04_80425b28;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;
    s32 alpha;
    f32 accel;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbb28;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x18) -= 1;
    *(s32*)(work + 0x1C) += 1;
    timer = *(s32*)(work + 0x18);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer > 5) {
        alpha = *(s32*)(work + 0x14);
        *(s32*)(work + 0x14) = (s32)((f32)alpha + (float_0p3_80425b20 * (f32)(0xFF - alpha)));
        *(f32*)(work + 0x10) += float_0p3_80425b20 * (float_1_80425b24 - *(f32*)(work + 0x10));
    }
    if (timer < 10) {
        *(s32*)(work + 0x14) = timer * 0x19;
    }

    accel = float_0p04_80425b28;
    *(f32*)(work + 4) += *(f32*)(work + 0x24);
    *(f32*)(work + 8) += *(f32*)(work + 0x28);
    *(f32*)(work + 0x24) += accel * (*(f32*)(work + 0x2C) - *(f32*)(work + 0x24));
    *(f32*)(work + 0x28) += accel * (*(f32*)(work + 0x30) - *(f32*)(work + 0x28));
    dispEntry(4, 2, effOnpuDisp, effect, dispCalcZ(&dispPos));
}

