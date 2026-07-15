#include "effect/n64/eff_sstar_n64.h"


u8 effSstarDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    s32 pass;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXScale(scale, *(f32*)(work + 0x1C0), *(f32*)(work + 0x1C0), 1.0f);
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(model, scale, model);
    PSMTXRotRad(rot, *(f32*)(work + 0x24) * 0.017453292f, 'z');
    PSMTXConcat(model, rot, model);
    PSMTXConcat(camera + 0x118, model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    effGetTexObj(0x6C, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumTevStages(3);
    for (pass = 0; pass < 3; pass++) {
        GXSetTevOrder(pass, pass == 0 ? 0 : -1, pass == 0 ? 0 : -1, pass == 2 ? 4 : -1);
    }
    GXBegin(0x90, 0, 6);
    GXSetNumTevStages(1);
    GXBegin(0x90, 0, 0xC);
    GXBegin(0x90, 0, 6);
    return 0;
}

void* effSstarN64Entry(f32 x, f32 y, f32 z, f32 dstX, f32 dstY, f32 dstZ, f32 speed, s32 type) {
    extern void* effEntry(void);extern void* __memAlloc(s32,s32);extern void effSstarMain(void*);extern char str_SstarN64_802fc008[];extern s32 rand(void);extern f64 sqrt(f64);extern f32 float_0_8042610c,float_0p2_80426144,float_0p5_80426148;
    void* entry;u8* work;f32 dx,dy,dz,len;entry=effEntry();*(char**)((s32)entry+0x14)=str_SstarN64_802fc008;*(s32*)((s32)entry+8)=1;work=__memAlloc(3,0x25C);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effSstarMain;*(s32*)work=1;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(s32*)(work+0x38)=type;*(f32*)(work+0x1C)=float_0_8042610c;*(f32*)(work+0x24)=float_0_8042610c;*(f32*)(work+0x28)=float_0_8042610c;
    if(type==3){*(f32*)(work+0x1C0)=float_0p2_80426144;*(s32*)(work+0x1C4)=rand()&0xFF;*(s32*)(work+0x1C8)=rand()&0xFF;*(s32*)(work+0x1CC)=rand()&0xFF;}else{*(f32*)(work+0x1C0)=float_0p5_80426148;*(s32*)(work+0x1C4)=0xD2;*(s32*)(work+0x1C8)=0xBA;*(s32*)(work+0x1CC)=0x5A;}
    dx=dstX-x;dy=dstY-y;dz=dstZ-z;len=dx*dx+dy*dy+dz*dz;if(len>float_0_8042610c){*(f32*)(work+0x10)=dx*speed/(f32)sqrt(len);*(f32*)(work+0x14)=dy*speed/(f32)sqrt(len);*(f32*)(work+0x18)=dz*speed/(f32)sqrt(len);}return entry;
}

u8 effSstarMain(int* effect) {
    extern void effDelete(void*);extern f32 dispCalcZ(void*);extern void dispEntry(s32,s32,void*,void*,f32);extern void effSstarDisp(void);extern f64 sqrt(f64);extern f32 angleABf(f32,f32,f32,f32);extern f32 float_0p01_80426118,float_1_80426114,float_neg0p5_80426138;
    u8* work=*(u8**)((s32)effect+0xC);f32 speed,dx,dy,dz,len;dx=*(f32*)(work+0x10);dy=*(f32*)(work+0x14);dz=*(f32*)(work+0x18);if(*(s32*)(work+0x38)>1){len=dx*dx+dy*dy+dz*dz;if(len>float_0p01_80426118){speed=float_1_80426114/(f32)sqrt(len);dx*=speed;dy*=speed;dz*=speed;}if(*(f32*)(work+0x14)!=0.0f){*(f32*)(work+0x14)+=float_neg0p5_80426138;*(f32*)(work+0x24)+=*(f32*)(work+0x2C);*(f32*)(work+0x20)=angleABf(0.0f,0.0f,-*(f32*)(work+0x14),*(f32*)(work+0x2C));}}
    *(f32*)(work+4)+=*(f32*)(work+0x10);*(f32*)(work+8)+=*(f32*)(work+0x14);*(f32*)(work+0xC)+=*(f32*)(work+0x18);if(*(s32*)(work+0x30)<0){effDelete(effect);}else{dispEntry(4,2,effSstarDisp,effect,dispCalcZ(work+4));}return 0;
}

