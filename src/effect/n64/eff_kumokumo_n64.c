#include "effect/n64/eff_kumokumo_n64.h"


u8 effKumokumoDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXCallDisplayList(void*, u32);
    u8* effect = (u8*)effectAddress;
    u8* work = *(u8**)(effect + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    s32 i;

    GXSetNumChans(1);
    GXSetNumTevStages(3);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 5, 0x21, 0, 0x7D);
    effGetTexObj(0x68, &tex);
    GXLoadTexObj(&tex, 0);
    effGetTexObj(0x69, &tex);
    GXLoadTexObj(&tex, 1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevOrder(1, 1, 1, -1);
    GXSetTevOrder(2, -1, -1, 4);
    GXSetCullMode(0);
    for (i = 0; i < *(s32*)(effect + 8); i++) {
        u8* part = work + i * 0x64;
        f32 size = *(f32*)(part + 0x58);
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
        PSMTXConcat(trans, rot, model);
        PSMTXRotRad(rot, *(f32*)(part + 0x58) * 0.017453292f, 'z');
        PSMTXConcat(model, rot, model);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(model, scale, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXCallDisplayList(*(void**)(part + 0x5C), (u32)*(u8*)(part + 0x60) << 5);
    }
    return 0;
}

void effKumokumoMain(void* effect) {
    extern void effDelete(void*); extern s32 irand(s32); extern f64 sin(f64); extern f64 cos(f64); extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32); extern void effKumokumoDisp(s32,void*);
    u8* e=effect; u8* w=*(u8**)(e+0xC); u8* p=w; f32 pos[3]; s32 type=*(s32*)w,timer,i,camera=*(s32*)(w+0x60),delay=30;
    pos[0]=*(f32*)(w+4);pos[1]=*(f32*)(w+8);pos[2]=*(f32*)(w+0xC);if((*(u32*)e&4)!=0){*(u32*)e&=~4;*(s32*)(w+0x10)=16;}if(*(s32*)(w+0x10)<1000)(*(s32*)(w+0x10))--;(*(s32*)(w+0x14))++;timer=*(s32*)(w+0x10);if(timer<0){effDelete(effect);return;}if(timer<16)*(s32*)(w+0x24)=timer<<4;if(*(s32*)(w+0x14)<16)*(s32*)(w+0x24)=*(s32*)(w+0x14)*16+15;
    *(f32*)(w+0x38)+=*(f32*)(w+0x3C);*(f32*)(w+0x40)+=*(f32*)(w+0x44);*(f32*)(w+0x48)+=*(f32*)(w+0x4C);*(f32*)(w+0x50)+=*(f32*)(w+0x54);if(type==3)*(f32*)(w+0x58)-=2.0f;
    for(i=1;i<*(s32*)(e+8);i++,p+=0x64,delay+=30){if(*(s32*)(p+0x74)==0){if(type==0||type==4){(*(s32*)(p+0x88))-=2;if(*(s32*)(p+0x88)<0){*(f32*)(p+0x68)=0.0f;*(f32*)(p+0x6C)=3.0f;*(f32*)(p+0x70)=0.0f;*(f32*)(p+0xC0)=0.0f;*(s32*)(p+0x88)=0xCC;*(s32*)(p+0x74)=delay;*(f32*)(p+0xBC)=-(f32)irand(20);*(f32*)(p+0x9C)=8.0f;}else{*(f32*)(p+0xC0)+=(1.5f-*(f32*)(p+0xC0))/30.0f;*(f32*)(p+0xBC)+=(30.0f-*(f32*)(p+0xBC))/30.0f;*(f32*)(p+0x68)+=*(f32*)(p+0x9C)*(f32)cos(6.2832f**(f32*)(p+0xBC)/360.0f);*(f32*)(p+0x6C)+=2.0f**(f32*)(p+0x9C)*(f32)sin(6.2832f**(f32*)(p+0xBC)/360.0f);*(f32*)(p+0x9C)+=(2.0f-*(f32*)(p+0x9C))/30.0f;}}else{*(s32*)(p+0x88)+=(0xC0-*(s32*)(p+0x88))/4;*(f32*)(p+0x68)*=0.96875f;*(f32*)(p+0x6C)*=0.96875f;*(f32*)(p+0x70)*=0.96875f;}}else (*(s32*)(p+0x74))--;}
    dispEntry(camera,2,effKumokumoDisp,effect,dispCalcZ(pos));
}

void* effKumokumoN64Entry(f32 x, f32 y, f32 z, f32 scale, f32 unused, f32 angle, s32 type, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKumokumoMain(void*);
    extern char str_KumokumoN64_802fb3e0[];
    extern f32 float_0_8042590c;
    extern f32 float_0p018182_80425940;
    extern f32 float_0p04_80425944;
    extern f32 float_neg1_80425900;
    void* entry;
    u8* work;
    s32 count;

    entry = effEntry();
    count = type == 1 ? 1 : 11;
    if (type == 2) scale = float_0_8042590c;
    *(char**)((s32)entry + 0x14) = str_KumokumoN64_802fb3e0;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 100);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKumokumoMain;
    *(u32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    *(s32*)(work + 0x10) = duration > 0 ? duration : 1000;
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x5C) = angle;
    *(s32*)(work + 0x34) = 0xFF;
    *(f32*)(work + 0x58) = scale;
    *(s32*)(work + 0x60) = 4;
    *(f32*)(work + 0x38) = float_0_8042590c;
    *(f32*)(work + 0x40) = float_0_8042590c;
    *(f32*)(work + 0x48) = float_0_8042590c;
    *(f32*)(work + 0x50) = float_0_8042590c;
    *(f32*)(work + 0x3C) = float_0_8042590c;
    *(f32*)(work + 0x44) = float_0p018182_80425940;
    *(f32*)(work + 0x4C) = float_0_8042590c;
    *(f32*)(work + 0x54) = float_0p04_80425944;
    if (type == 1 || type == 3) {
        *(f32*)(work + 0x3C) *= float_neg1_80425900;
        *(f32*)(work + 0x44) *= float_neg1_80425900;
        *(f32*)(work + 0x4C) *= float_neg1_80425900;
        *(f32*)(work + 0x54) *= float_neg1_80425900;
    }
    return entry;
}

