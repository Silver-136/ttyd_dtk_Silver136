#include "effect/n64/eff_peach_dust_n64.h"


void* effPeachDustN64Entry(f32 x, f32 y, f32 z, s32 unused, f32 angle, s32 type) {
    extern void* effEntry(void); extern void* __memAlloc(s32,s32); extern void effPeachDustMain(void*); extern char str_PeachDustN64_802fbc80[];
    extern s32 rand(void); extern f64 sin(f64),cos(f64); extern f32 float_6p2832_80425cf0,float_360_80425cf4,float_0p1_80425cec,float_0p04_80425cf8,float_0p001_80425cfc,float_0_80425cd4;
    void* entry;u8* work;u8* part;s32 i,r;f32 a;
    entry=effEntry();*(char**)((s32)entry+0x14)=str_PeachDustN64_802fbc80;*(s32*)((s32)entry+8)=0x65;work=__memAlloc(3,0x115C);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effPeachDustMain;
    *(s32*)work=type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(s32*)(work+0x1C)=0x50;*(s32*)(work+0x20)=0;
    a=float_6p2832_80425cf0*angle/float_360_80425cf4;part=work+0x2C;
    for(i=0;i<100;i++,part+=0x2C){r=rand()%20-10;*(f32*)(part+4)=(f32)r*(f32)cos(a);*(f32*)(part+8)=(f32)(i/16);*(f32*)(part+0xC)=(f32)r*(f32)sin(a);*(f32*)(part+0x10)=float_0p04_80425cf8**(f32*)(part+4);*(f32*)(part+0x14)=float_0p04_80425cf8**(f32*)(part+8);*(f32*)(part+0x18)=float_0p001_80425cfc*(f32)(rand()%10);*(s32*)(part+0x1C)=rand()%7+0x1E;*(s32*)(part+0x20)=0;*(s32*)(part+0x28)=-1;}
    return entry;
}

void effPeachDustMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPeachDustDisp(s32, s32);
    extern const Vec3 vec3_802fbc68;
    extern u8 scale_data[];
    extern f32 float_0p01_80425ce8;
    extern f32 float_0p4_80425ce4;
    extern f32 float_0p1_80425cec;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 dead;
    s32 frame;
    s32 count;
    f32 scale;

    dead = 0;
    i = 0;
    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc68;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x1C) -= 1;
    *(s32*)(work + 0x20) += 1;

    frame = *(s32*)(work + 0x20);
    part = work + 0x2C;
    while (i < *(s32*)((s32)effect + 8) - 1) {
        *(s32*)(part + 0x28) = (frame + i + i) & 7;
        if (*(s32*)(part + 0x1C) < 0) {
            dead++;
        }
        *(s32*)(part + 0x1C) -= 1;
        count = *(s32*)(part + 0x1C);
        if (count > 0x1E || count < 0) {
            *(s32*)(part + 0x28) = -1;
        } else {
            *(s32*)(part + 0x20) += 1;
            if (*(s32*)(part + 0x20) - 1 < 0x19) {
                *(f32*)(part + 0x24) =
                    float_0p4_80425ce4 * (float_0p01_80425ce8 * (f32)scale_data[*(s32*)(part + 0x20) - 1]);
            } else {
                scale = *(f32*)(part + 0x24);
                *(f32*)(part + 0x24) = scale + float_0p1_80425cec * (float_0p1_80425cec - scale);
            }
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x10);
        }
        i++;
        part += 0x2C;
    }
    if (dead >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effPeachDustDisp, effect, dispCalcZ(&dispPos));
    }
}

void effPeachDustDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32); extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void PSMTXScale(void*,f32,f32,f32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); extern void effSetVtxDescN64(void*); extern f32 float_0p03125_80425ce0,float_0_80425cd4,float_deg2rad_80425cd8,float_1_80425cdc; extern u8 size32x16_tex64x32_vtx[];
    f32 trans[3][4],rot[3][4],scale[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);void* camera=camGetPtr(cameraId);s32 i;
    GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80425ce0,float_0p03125_80425ce0,float_0_80425cd4);GXLoadTexMtxImm(scale,0x1E,1);effGetTexObjN64(0x33,texObj);GXLoadTexObj(texObj,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0xF,0xF,0xF,2);GXSetTevAlphaIn(0,7,1,4,7);GXSetCullMode(0);
    for(i=0;i<*(s32*)((s32)effect+8)-1;i++,work+=0x2C){if(*(s32*)(work+0x54)<0)continue;PSMTXTrans(trans,*(f32*)(work+0x30),*(f32*)(work+0x34),*(f32*)(work+0x38));PSMTXRotRad(rot,0x79,float_deg2rad_80425cd8*-*(f32*)((s32)camGetPtr(4)+0x114));PSMTXScale(scale,*(f32*)(work+0x50),*(f32*)(work+0x50),float_1_80425cdc);PSMTXConcat(trans,rot,trans);PSMTXConcat(trans,scale,trans);PSMTXConcat((void*)((s32)camera+0x11C),trans,trans);GXLoadPosMtxImm(trans,0);GXSetCurrentMtx(0);effSetVtxDescN64(size32x16_tex64x32_vtx);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}
