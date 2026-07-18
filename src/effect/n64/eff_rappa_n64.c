#include "effect/n64/eff_rappa_n64.h"


void effRappaDisp(s32 cameraId, void* effect){extern void* camGetPtr(s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void PSMTXScale(void*,f32,f32,f32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXSetTevColor(s32,void*);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern u8 color_rotation_data[];u8* w=*(u8**)((u8*)effect+0xC);u8* cam=camGetPtr(cameraId);f32 a[3][4],b[3][4],c[3][4];u8 tex[0x20];u32 col;s32 i,idx=(*(s32*)(w+0x34)-1)*3;GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(b,0.0625f,0.03125f,0.0f);GXLoadTexMtxImm(b,0x1E,1);effGetTexObjN64(0x1D,tex);GXLoadTexObj(tex,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,-1);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,4,5,7);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A79C8);for(i=1;i<*(s32*)((u8*)effect+8);i++,w+=0x38,idx+=3){col=(color_rotation_data[idx%36]<<24)|(color_rotation_data[(idx+1)%36]<<16)|(color_rotation_data[(idx+2)%36]<<8)|*(u8*)(w+0x2C);GXSetTevColor(1,&col);PSMTXTrans(a,*(f32*)(w+0x3C),*(f32*)(w+0x40),*(f32*)(w+0x44));PSMTXRotRad(b,0x79,-0.017453292f**(f32*)((u8*)camGetPtr(4)+0x114));PSMTXScale(c,*(f32*)(w+0x5C),*(f32*)(w+0x5C),1.0f);PSMTXConcat(a,b,a);PSMTXConcat(a,c,a);PSMTXConcat(cam+0x11C,a,a);GXLoadPosMtxImm(a,0);GXSetCurrentMtx(0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}}

void* effRappaN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 baseAngle, f32 spreadAngle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRappaMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_RappaN64_802fbde0[];
    extern f32 float_6p2832_80425e78;
    extern f32 float_5_80425e7c;
    extern f32 float_0p25_80425e80;
    extern f32 float_360_80425e84;
    extern f32 float_10_80425e74;
    void* entry;
    u8* work;
    u8* part;
    f32 baseRad;
    f32 angleRad;
    f32 speed;
    f32 sinBase;
    f32 cosBase;
    f32 sinAngle;
    f32 cosAngle;
    s32 i;
    s32 randValue;
    s32 angleOffset;

    entry = effEntry();
    if (count == 0) {
        return NULL;
    }

    *(char**)((s32)entry + 0x14) = str_RappaN64_802fbde0;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x38);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRappaMain;

    baseRad = (float_6p2832_80425e78 * baseAngle) / float_360_80425e84;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x30) = 0x1E;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x2C) = 0xFF;

    part = work + 0x38;
    for (i = 1; i < count + 1; i++, part += 0x38) {
        randValue = rand();
        *(s32*)part = type;
        *(f32*)(part + 4) = x;
        *(f32*)(part + 8) = y;
        *(f32*)(part + 0xC) = z;
        speed = (f32)(randValue % 10) * float_0p25_80425e80 + float_5_80425e7c;
        sinBase = (f32)sin(baseRad);
        angleOffset = ((i & 2) * (i / 2)) << 2;
        angleRad = (float_6p2832_80425e78 * (spreadAngle + (f32)angleOffset)) / float_360_80425e84;
        cosAngle = (f32)cos(angleRad);
        *(f32*)(part + 0x10) = cosAngle * sinBase;
        sinAngle = (f32)sin(angleRad);
        *(f32*)(part + 0x14) = sinAngle;
        cosBase = (f32)cos(baseRad);
        cosAngle = (f32)cos(angleRad);
        *(f32*)(part + 0x18) = cosAngle * cosBase;
        *(f32*)(part + 0x28) = speed;
        *(f32*)(part + 0x24) = speed / float_10_80425e74;
    }
    return entry;
}

void effRappaMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRappaDisp(void);
    extern const Vec3 vec3_802fbdc8;
    extern f32 float_0p8_80425e6c;
    extern f32 float_0p9_80425e70;
    extern f32 float_10_80425e74;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    f32 drag;
    f32 ten;
    s32 i;
    s32 timer;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbdc8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x34) += 1;
    timer = *(s32*)(work + 0x30);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 10) {
        *(s32*)(work + 0x2C) = (s32)((f32)*(s32*)(work + 0x2C) * float_0p8_80425e6c);
    }

    drag = float_0p9_80425e70;
    i = 1;
    part = work + 0x38;
    if (timer < 10) {
        ten = float_10_80425e74;
        for (; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= drag;
            *(f32*)(part + 0x24) += *(f32*)(part + 0x28) / ten;
        }
    } else {
        for (; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= drag;
        }
    }
    dispEntry(4, 2, effRappaDisp, effect, dispCalcZ(&dispPos));
}

