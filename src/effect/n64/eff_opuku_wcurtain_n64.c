#include "effect/n64/eff_opuku_wcurtain_n64.h"

void* effOpukuWcurtainN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOpukuWcurtainMain(void*);
    extern s32 rand(void);
    extern char str_OpukuWcurtainN64_802fbbcc[];
    extern f32 float_0_80425ba0;
    extern f32 float_neg1_80425bb4;
    extern f32 float_0p01_80425bb8;
    extern f32 float_0p5_80425bbc;
    void* entry;
    u8* work;
    u8* p;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OpukuWcurtainN64_802fbbcc;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x8C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuWcurtainMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x10) = 0x3E8;
    } else {
        *(s32*)(work + 0x10) = time;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = scale;
    *(s32*)(work + 0x18) = 0x14;
    *(s32*)(work + 0x1C) = 0x78;
    *(s32*)(work + 0x20) = 0xFF;

    p = work;
    for (i = 0; i < 12; i++, p += 4) {
        *(f32*)(p + 0x2C) = float_0_80425ba0;
        *(f32*)(p + 0x5C) = (float_neg1_80425bb4 + (float_0p01_80425bb8 * (f32)(rand() % 11))) * float_0p5_80425bbc;
    }
    return entry;
}

void effOpukuWcurtainMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOpukuWcurtainDisp(void);
    extern const Vec3 vec3_802fbbc0;
    extern f32 float_neg0p05_80425bac;
    extern f32 float_5_80425bb0;
    extern f32 float_0p1_80425b98;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;
    s32 frame;
    f32 v;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbbc0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 0x3E8) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x14);
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }

    p = work;
    for (i = 0; i < 12; i++, p += 4) {
        *(f32*)(p + 0x2C) += *(f32*)(p + 0x5C);
        *(f32*)(p + 0x5C) += float_neg0p05_80425bac;
        v = *(f32*)(p + 0x5C);
        if (v > float_5_80425bb0) {
            *(f32*)(p + 0x5C) = v + float_0p1_80425b98 * (float_5_80425bb0 - v);
        }
    }

    dispEntry(4, 2, effOpukuWcurtainDisp, effect, dispCalcZ(&dispPos));
}


void effOpukuWcurtainDisp(s32 cameraId, void* effect) {
    extern void GXSetTevColor(s32, void*); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32, void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void PSMTXScale(void*,f32,f32,f32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetCullMode(s32);
    extern void* smartAlloc(s32,s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*);
    extern void DCFlushRange(void*,s32); extern void GXInvalidateVtxCache(void); extern void effSetVtxDescN64(void*); extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_0p03125_80425b9c,float_0_80425ba0,float_0p1_80425b98;
    f32 scale[3][4], trans[3][4]; u8 texObj[0x20]; u8* work=*(u8**)((s32)effect+0xC); u32 color=*(u32*)(work+0x18); void* vertices; s32 i;
    GXSetTevColor(1,&color); GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,4);
    GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,2,1,8,0); GXSetTevAlphaIn(0,0,7,5,7);
    effGetTexObjN64(0x8F,texObj); GXLoadTexObj(texObj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(scale,float_0p03125_80425b9c,float_0p03125_80425b9c,float_0_80425ba0); GXLoadTexMtxImm(scale,0x1E,1); GXSetCullMode(0);
    vertices=smartAlloc(0x2A0,3); PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC)); PSMTXScale(scale,float_0p1_80425b98**(f32*)(work+0x28),float_0p1_80425b98**(f32*)(work+0x28),float_0p1_80425b98**(f32*)(work+0x28)); PSMTXConcat(trans,scale,trans);
    DCFlushRange(vertices,0x2A0); GXInvalidateVtxCache(); effSetVtxDescN64(vertices); GXLoadPosMtxImm((void*)((s32)camGetPtr(cameraId)+0x11C),0); GXSetCurrentMtx(0);
    for(i=0;i<12;i++){ GXBegin(0x90,0,6); tri2(i*2,i*2+2,i*2+1,i*2,i*2+1,i*2+2,i*2+3,0); }
}

