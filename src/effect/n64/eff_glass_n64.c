#include "effect/n64/eff_glass_n64.h"

#pragma optimize_for_size off

void* effGlassN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effGlassMain(void* effect);
    extern s32 rand(void);
    extern char str_GlassN64_802fb038[];
    extern f32 float_0_8042540c;
    extern f32 float_0p25_80425418;
    extern f32 float_0p1_80425414;
    extern f32 float_neg0p1_8042541c;
    void* entry;
    u8* work;
    f32 randomScale;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_GlassN64_802fb038;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x5C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGlassMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (duration > 0) {
        *(s32*)(work + 0x10) = duration;
    } else {
        *(s32*)(work + 0x10) = 1000;
    }
    *(s32*)(work + 0x24) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x38) = scale;

    if (type < 2) {
        if (type >= 0) {
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xFF;
            *(s32*)(work + 0x2C) = 0xFF;
            *(s32*)(work + 0x30) = 0xFF;
            *(s32*)(work + 0x34) = 0xFF;
        }
    } else if (type < 4) {
        *(s32*)(work + 0x18) = 0;
        *(s32*)(work + 0x1C) = 0xFF;
        *(s32*)(work + 0x20) = 0;
        *(s32*)(work + 0x28) = 0;
        *(s32*)(work + 0x2C) = 0xFF;
        *(s32*)(work + 0x30) = 0;
        *(s32*)(work + 0x34) = 0xFF;
    }

    *(f32*)(work + 0x44) = float_0_8042540c;
    randomScale = float_0p25_80425418 * (f32)((rand() % 2) * 2 - 1);
    *(f32*)(work + 0x48) = (float_0p1_80425414 * (f32)(rand() % 5) + float_0p1_80425414) * randomScale;
    *(f32*)(work + 0x3C) = (f32)(rand() % 360);
    *(f32*)(work + 0x4C) = (f32)(rand() % 11 - 5);
    *(f32*)(work + 0x50) = (f32)(rand() % 11 - 5);
    *(f32*)(work + 0x54) = (f32)(rand() % 11 - 5);
    *(f32*)(work + 0x40) = (f32)(rand() % 41 - 20);
    *(f32*)(work + 0x58) = float_neg0p1_8042541c;
    return entry;
}

#pragma optimize_for_size on

void effGlassMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effGlassDisp(s32, s32);
    extern Vec3 vec3_802fb020[];
    extern f32 float_0_8042540c;
    extern f32 float_1_80425410;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb020;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
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
    *(f32*)(work + 0x44) += *(f32*)(work + 0x48);
    if (*(f32*)(work + 0x44) < float_0_8042540c) {
        *(f32*)(work + 0x44) += float_1_80425410;
    }
    if (*(f32*)(work + 0x44) > float_1_80425410) {
        *(f32*)(work + 0x44) -= float_1_80425410;
    }
    *(f32*)(work + 0x50) += *(f32*)(work + 0x58);
    *(f32*)(work + 4) += *(f32*)(work + 0x4C);
    *(f32*)(work + 8) += *(f32*)(work + 0x50);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x54);
    *(f32*)(work + 0x3C) += *(f32*)(work + 0x40);
    dispEntry(4, 2, effGlassDisp, effect, dispCalcZ(&dispPos));
}


void effGlassDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC); void* cam=camGetPtr(cameraId); Mtx m,r,s,base; u8 tex[0x20]; u32 color; s32 frame=(s32)(4.0f**(f32*)(w+0x44)); s32 alpha=(s32)(256.0f*(4.0f**(f32*)(w+0x44)-(f32)frame));
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC)); PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f); PSMTXScale(s,*(f32*)(w+0x38),*(f32*)(w+0x38),*(f32*)(w+0x38)); PSMTXConcat(m,r,m); PSMTXConcat(m,s,m); PSMTXConcat((u8*)cam+0x11C,m,base); PSMTXRotRad(r,0x7A,*(f32*)(w+0x3C)*0.0174533f); PSMTXConcat(base,r,m); GXLoadPosMtxImm(m,0); GXSetCurrentMtx(0);
    GXSetNumChans(0); GXSetNumTevStages(3); GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,0,0,0,8); GXSetTevAlphaIn(0,0,0,0,4); GXSetTevOrder(1,1,1,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,0,8,6,15); GXSetTevAlphaIn(1,0,4,6,7); GXSetTevOrder(2,0xFF,0xFF,0xFF); GXSetTevColorOp(2,0,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0); GXSetTevColorIn(2,4,2,0,15); GXSetTevAlphaIn(2,0,6,1,7);
    effGetTexObjN64(0x59,tex); GXLoadTexObj(tex,0); GXLoadTexObj(tex,1); GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D); PSMTXScale(s,0.015625f,0.0078125f,0.0f); PSMTXTrans(r,(f32)((*(s32*)w&1)*32),(f32)(frame*32),0.0f); PSMTXConcat(s,r,s); GXLoadTexMtxImm(s,0x1E,1); PSMTXTrans(r,(f32)((*(s32*)w&1)*32),(f32)((frame+1)*32),0.0f); PSMTXConcat(s,r,s); GXLoadTexMtxImm(s,0x21,1);
    color=((u8)*(s32*)(w+0x18)<<24)|((u8)*(s32*)(w+0x1C)<<16)|((u8)*(s32*)(w+0x20)<<8)|(u8)*(s32*)(w+0x24); GXSetTevColor(1,&color); color=((u8)*(s32*)(w+0x28)<<24)|((u8)*(s32*)(w+0x2C)<<16)|((u8)*(s32*)(w+0x30)<<8)|(u8)alpha; GXSetTevColor(2,&color); GXSetCullMode(0); effSetVtxDescN64((void*)0x803A0368); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);
}

