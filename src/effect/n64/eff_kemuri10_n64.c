#include "effect/n64/eff_kemuri10_n64.h"


void effKemuri10Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC); void* cam=camGetPtr(cameraId); Mtx m,t,s; u8 tex[0x20]; u32 color; s32 frame=(s32)*(f32*)(w+0x4C);
    GXSetNumChans(0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(s,0.015625f,0.03125f,0.0f); PSMTXTrans(t,(f32)frame,0.0f,0.0f); PSMTXConcat(s,t,t); GXLoadTexMtxImm(t,0x1E,1); effGetTexObjN64(0x1F,tex); GXLoadTexObj(tex,0); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,0,0,0,2); GXSetTevAlphaIn(0,0,4,1,7); color=(*(u32*)(w+0x64)&0xFFFFFF00)|(u8)*(s32*)(w+0x58); GXSetTevColor(1,&color);
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC)); PSMTXConcat(m,w+0x1C,m); PSMTXScale(s,*(f32*)(w+0x54),*(f32*)(w+0x54),*(f32*)(w+0x54)); PSMTXConcat(m,s,m); PSMTXConcat((u8*)cam+0x11C,m,m); GXLoadPosMtxImm(m,0); GXSetCurrentMtx(0); effSetVtxDescN64((void*)0x803A4DC8); GXBegin(0x90,0,0xC0);
    tri2(0,1,2,0,0,2,3,0);tri2(3,2,4,0,3,4,5,0);tri2(2,6,7,0,2,7,4,0);tri2(1,8,6,0,1,6,2,0);tri2(4,7,9,0,4,9,10,0);tri2(5,4,10,0,5,10,11,0);tri2(10,9,8,0,10,8,1,0);tri2(11,10,1,0,11,1,0,0);tri2(6,12,13,0,6,13,7,0);tri2(8,14,12,0,8,12,6,0);tri2(7,13,15,0,7,15,9,0);tri2(9,15,14,0,9,14,8,0);tri2(16,17,18,0,16,18,19,0);tri2(20,21,17,0,20,17,16,0);tri2(22,20,16,0,22,16,23,0);tri2(23,16,19,0,23,19,24,0);
}

void* effKemuri10N64Entry(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, f32 speed, s32 camera) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKemuri10Main(void*);
    extern f32 sqrtf(f32);
    extern char str_Kemuri10N64_802fb318[];
    u8* work;
    void* entry;
    f32 len;

    if (dx == 0.0f && dy == 0.0f && dz == 0.0f) return 0;
    len = sqrtf(dx * dx + dy * dy + dz * dz);
    if (len == 0.0f) return 0;
    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri10N64_802fb318;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x70);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri10Main;
    *(s32*)work = camera;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = dx / len;
    *(f32*)(work + 0x14) = dy / len;
    *(f32*)(work + 0x18) = dz / len;
    *(f32*)(work + 0x4C) = speed;
    *(s32*)(work + 0x5C) = 10;
    *(s32*)(work + 0x60) = 0;
    *(s32*)(work + 0x6C) = camera;
    return entry;
}

void effKemuri10Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri10Disp(s32, s32);
    extern const Vec3 vec3_802fb2e8;
    extern f32 float_0p3_80425834;
    extern f32 float_0p8_80425838;
    extern f32 float_0p6_8042583c;
    extern f32 float_0p7_80425840;
    extern f32 float_1p05_80425844;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 camera;
    s32 i;
    s32 timer;
    s32 alpha;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb2e8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x5C) -= 1;
    *(s32*)(work + 0x60) += 1;
    timer = *(s32*)(work + 0x5C);
    camera = *(s32*)(work + 0x6C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x70) {
        if (timer > 5) {
            alpha = *(s32*)(work + 0x58);
            *(s32*)(work + 0x58) = (s32)((f32)alpha + (float_0p3_80425834 * (f32)(0xFF - alpha)));
            *(f32*)(work + 0x10) *= float_0p8_80425838;
            *(f32*)(work + 0x14) *= float_0p8_80425838;
            *(f32*)(work + 0x18) *= float_0p8_80425838;
        }
        if (timer < 5) {
            *(s32*)(work + 0x58) = (s32)((f32)*(s32*)(work + 0x58) * float_0p6_8042583c);
            *(f32*)(work + 0x10) *= float_0p7_80425840;
            *(f32*)(work + 0x14) *= float_0p7_80425840;
            *(f32*)(work + 0x18) *= float_0p7_80425840;
        }
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x4C) += *(f32*)(work + 0x50);
        *(f32*)(work + 0x54) *= float_1p05_80425844;
    }

    dispEntry(camera, 2, effKemuri10Disp, effect, dispCalcZ(&dispPos));
}

