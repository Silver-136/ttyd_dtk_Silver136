#include "effect/n64/eff_snow_dust_n64.h"


u8 effSnowDustDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    u8 texObj[0x20];
    Mtx trans, rot, scale, model;
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* part = work + 0x6C;
    u8* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 count = *(s32*)(effectAddress + 8);
    s32 color;
    s32 i;

    GXSetNumChans(0);
    GXSetNumTevStages(type == 2 ? 1 : 2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 0);
    GXSetTevAlphaIn(0, 0, 4, 5, 7);
    effGetTexObjN64(type == 1 ? 0x66 : type == 2 ? 0x67 : type == 6 ? 8 : 0x68, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetCullMode(0);

    for (i = 1; i < count; i++, part += 0x6C) {
        if (*(s32*)(part + 0x30) < 0) {
            continue;
        }
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)((u8*)camGetPtr(4) + 0x114), 'y');
        PSMTXConcat(trans, rot, model);
        PSMTXScale(scale, *(f32*)(part + 0x5C), *(f32*)(part + 0x5C), *(f32*)(part + 0x5C));
        PSMTXConcat(model, scale, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        color = (*(u8*)(part + 0x58) * *(s32*)(work + 0x48)) / 255;
        GXSetTevColor(1, &color);
        PSMTXScale(scale, type == 1 ? 0.03125f : 0.015625f,
                          type == 2 ? 0.015625f : 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        effSetVtxDescN64((void*)(type == 2 ? 0x803ABAF0 : 0x803ABAB8));
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
    return 0;
}

u8 effSnowDustMain(s32 effectAddress) {
    extern void effDelete(void*);
    extern void* camGetPtr(s32);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    u8* effect = (u8*)effectAddress;
    u8* work = *(u8**)(effect + 0xC);
    u8* part = work + 0x6C;
    s32 type = *(s32*)work;
    s32 timer;
    s32 frame;
    s32 i;
    s32 width;
    s32 height;
    f32 halfWidth;
    f32 angle;
    f32 cs;
    f32 sn;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x34) = 16;
    }
    timer = *(s32*)(work + 0x34);
    if (timer < 1000) {
        timer--;
        *(s32*)(work + 0x34) = timer;
    }
    (*(s32*)(work + 0x38))++;
    if (timer < 0) {
        effDelete(effect);
        return 0;
    }
    if (timer < 8) {
        *(s32*)(work + 0x48) = timer << 5;
    }

    width = (s32)*(f32*)(work + 0x10);
    height = (s32)*(f32*)(work + 0x14);
    halfWidth = *(f32*)(work + 0x10) * 0.5f;
    angle = 6.2832f * *(f32*)((u8*)camGetPtr(4) + 0x114) / 360.0f;
    cs = (f32)cos(angle);
    sn = (f32)sin(angle);

    for (i = 1; i < *(s32*)(effect + 8); i++, part += 0x6C) {
        frame = ++*(s32*)(part + 0x30);
        if (frame > 29) {
            *(s32*)(part + 0x30) = timer < 30 ? 20 : 0;
            frame = *(s32*)(part + 0x30);
        }
        if (frame < 0) {
            continue;
        }
        if (frame == 0) {
            f32 offset = (f32)(rand() % width) - halfWidth;
            *(f32*)(part + 4) = *(f32*)(work + 4) + offset * cs - sn;
            *(f32*)(part + 8) = *(f32*)(work + 8) + (f32)(rand() % height);
            *(f32*)(part + 0xC) = *(f32*)(work + 0xC) + offset * sn + cs;
            *(s32*)(part + 0x3C) = *(s32*)(work + 0x3C);
            *(s32*)(part + 0x40) = *(s32*)(work + 0x40);
            *(s32*)(part + 0x44) = *(s32*)(work + 0x44);
            *(f32*)(part + 0x5C) = 0.5f + 0.05f * (f32)(rand() % 10);
            *(f32*)(part + 0x4C) = 0.0f;
            *(f32*)(part + 0x50) = 0.0f;
        }
        if (type < 4 || type == 6) {
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x4C) += 0.2f;
            *(f32*)(part + 0x50) += 0.6f;
            *(f32*)(part + 0x10) += 0.04f * (*(f32*)(part + 0x18) - *(f32*)(part + 0x10));
            *(f32*)(part + 0x14) += 0.04f * (*(f32*)(part + 0x1C) - *(f32*)(part + 0x14));
        }
        *(s32*)(part + 0x58) = 255 - frame * 8;
    }
    return 0;
}

void* effSnowDustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 arg4, f32 arg5, f32 scale, s32 count, s32 lifetime) {
    extern void* effEntry(void); extern void* __memAlloc(s32,u32); extern s32 rand(void); extern void effSnowDustMain(void);
    void* effect=effEntry(); u8* work; s32 i; f32 step;
    *(s32*)((s32)effect+8)=count+1; work=__memAlloc(3,(count+1)*0x6C); *(u8**)((s32)effect+0xC)=work; *(void**)((s32)effect+0x10)=effSnowDustMain; *(u32*)effect|=2;
    *(s32*)work=type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(f32*)(work+0x10)=arg4;*(f32*)(work+0x14)=arg5;*(s32*)(work+0x34)=lifetime>0?lifetime:1000;*(s32*)(work+0x38)=0;*(s32*)(work+0x3C)=255;*(s32*)(work+0x40)=255;*(s32*)(work+0x44)=255;*(f32*)(work+0x5C)=scale;*(s32*)(work+0x68)=4;
    if(type==0){*(s32*)(work+0x4C)=255;*(s32*)(work+0x50)=255;*(s32*)(work+0x54)=255;*(s32*)(work+0x58)=255;}else if(type==1){*(s32*)(work+0x4C)=0;*(s32*)(work+0x50)=127;*(s32*)(work+0x54)=255;*(s32*)(work+0x58)=255;}else{*(s32*)(work+0x4C)=127;*(s32*)(work+0x50)=127;*(s32*)(work+0x54)=127;*(s32*)(work+0x58)=127;}
    step=(lifetime<30?(f32)lifetime:30.0f)/(f32)count;for(i=1;i<=count;i++){u8* p=work+i*0x6C;*(s32*)(p+0x30)=-(s32)((f32)i*step)-1;}return effect;
}

