#include "effect/n64/eff_kemuri12_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri12Work {
    s16 type;
    u8 pad_02[2];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 timer;
    s32 frame;
    s32 duration;
    f32 progress;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 colorA;
} EffKemuri12Work;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effKemuri12Main(void* effect);

extern const Vec3 vec3_802fb350;
extern char str_Kemuri12N64_802fb368[];
extern f32 float_0_804258d4;
extern f32 float_8_804258dc;


void effKemuri12Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32); extern void PSMTXTrans(Mtx,f64,f64,f64);
    extern void PSMTXRotRad(Mtx,f32,char); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx); extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32,void*);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_256_804258c0; extern f32 float_deg2rad_804258c4; extern f32 float_0p34907_804258c8;
    extern f32 float_0p0039062_804258cc; extern f32 float_0p03125_804258d0; extern f32 float_0_804258d4; extern f32 float_32_804258d8;
    extern u8 size32x32_tex32x32_vtx[]; extern u8 size32x32_tex32x32_yougan_vtx[];
    u8 tex[0x20]; Mtx model,rotation,scale; u8* entry=(u8*)effect; u8* work=*(u8**)(entry+0xC);
    u8* camera=(u8*)camGetPtr(cameraId); u8* camera3d=(u8*)camGetPtr(4); s32 type=*(u16*)work;
    f32 progress=*(f32*)(work+0x20); s32 frame=(s32)progress; s32 blend=(s32)(float_256_804258c0*(progress-(f32)frame)); u32 color0=*(u32*)(work+0x24); u32 color1=0xFFFFFF00|(u8)blend; void* vtx;
    GXSetNumChans(0);GXSetNumTevStages(3);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,7,7,4);
    GXSetTevOrder(1,1,1,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,15,15,15,0);GXSetTevAlphaIn(1,0,4,6,7);
    GXSetTevOrder(2,0xFF,0xFF,0xFF);GXSetTevColorOp(2,0,0,0,1,0);GXSetTevAlphaOp(2,0,0,0,1,0);GXSetTevColorIn(2,15,15,15,0);GXSetTevAlphaIn(2,7,0,5,7);
    effGetTexObjN64(0xE,tex);GXLoadTexObj(tex,0);GXLoadTexObj(tex,1);
    PSMTXTrans(model,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rotation,float_deg2rad_804258c4*-*(f32*)(camera3d+0x114),'y');PSMTXScale(scale,*(f32*)(work+0x10),*(f32*)(work+0x10),*(f32*)(work+0x10));PSMTXConcat(model,rotation,model);PSMTXConcat(model,scale,model);PSMTXRotRad(rotation,float_0p34907_804258c8,'z');PSMTXConcat(model,rotation,model);PSMTXConcat((f32(*)[4])(camera+0x11C),model,model);GXLoadPosMtxImm(model,0);GXSetCurrentMtx(0);
    GXSetTevColor(1,&color0);GXSetTevColor(2,&color1);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    PSMTXScale(scale,float_0p0039062_804258cc,float_0p03125_804258d0,float_0_804258d4);PSMTXTrans(rotation,(f32)(frame*0x20),float_0_804258d4,float_0_804258d4);PSMTXConcat(scale,rotation,scale);GXLoadTexMtxImm(scale,0x1E,1);
    PSMTXScale(scale,float_0p0039062_804258cc,float_0p03125_804258d0,float_0_804258d4);PSMTXTrans(rotation,(f32)((frame==7?8:frame+1)*0x20),frame==7?float_32_804258d8:float_0_804258d4,float_0_804258d4);PSMTXConcat(scale,rotation,scale);GXLoadTexMtxImm(scale,0x21,1);
    vtx=(type==0||type==2)?size32x32_tex32x32_vtx:size32x32_tex32x32_yougan_vtx;GXSetCullMode(0);effSetVtxDescN64(vtx);
    if(type==0||type==2){GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3);}else{GXBegin(0x90,0,0x12);tri2(8,9,10,0,8,10,11);tri2(4,5,6,0,4,6,7);tri2(0,1,2,0,0,2,3);}
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effKemuri12N64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffKemuri12Work* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_Kemuri12N64_802fb368;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x28);
    *(EffKemuri12Work**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri12Main;
    zero = float_0_804258d4;
    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = zero;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    if (type == 0) {
        work->colorB = 0;
        work->colorG = 0;
        work->colorR = 0;
        work->colorA = 0x82;
    } else if (type == 1) {
        work->colorR = 0xFF;
        work->colorB = 0;
        work->colorG = 0;
        work->colorA = 0x82;
    } else {
        work->colorR = 0xE1;
        work->colorG = 0xD7;
        work->colorB = 0xFF;
        work->colorA = 0xB4;
    }

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effKemuri12Main(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri12Work* work;

    work = *(EffKemuri12Work**)((s32)effect + 0xC);
    zero = &vec3_802fb350;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_8_804258dc * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri12Disp, effect, dispCalcZ(&dispPos));
    }
}
