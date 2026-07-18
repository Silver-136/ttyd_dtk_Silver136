#include "effect/n64/eff_pikkyolo_n64.h"


void* effPikkyoloN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 time) {
    extern void* effEntry(void); extern void* __memAlloc(s32,s32); extern void effPikkyoloMain(void*); extern char str_PikkyoloN64_802fbcb0[]; extern s32 rand(void); extern f32 float_0p5_80425d0c,float_0p8_80425d18;
    void* entry;u8* work; s32 i;
    entry=effEntry();*(char**)((s32)entry+0x14)=str_PikkyoloN64_802fbcb0;*(s32*)((s32)entry+8)=6;work=__memAlloc(3,0x168);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effPikkyoloMain;*(s32*)entry|=2;
    for(i=0;i<6;i++,work+=0x3C){*(s32*)work=type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(f32*)(work+0x10)=float_0p5_80425d0c*(f32)(rand()%11-5);*(f32*)(work+0x14)=float_0p8_80425d18*(f32)(rand()%6+1);*(f32*)(work+0x18)=float_0p5_80425d0c*(f32)(rand()%10-5);*(s32*)(work+0x1C)=time<1?1000:time;*(s32*)(work+0x20)=0;*(f32*)(work+0x24)=scale;*(s16*)(work+0x32)=0xFF;}
    return entry;
}

void effPikkyoloDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32); extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void PSMTXScale(void*,f32,f32,f32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32,void*);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC); u8* cam=camGetPtr(cameraId); f32 a[3][4],b[3][4],c[3][4]; u8 tex[0x20]; u32 col; s32 i;
    GXSetNumChans(0); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,3,2,8,15); GXSetTevAlphaIn(0,7,5,4,7);
    effGetTexObjN64(0x41,tex); GXLoadTexObj(tex,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(b,0.0625f,0.015625f,0.0f); GXLoadTexMtxImm(b,0x1E,1); GXSetCullMode(0); effSetVtxDescN64((void*)0x803A6FE0);
    for(i=0;i<*(s32*)((u8*)effect+8);i++,w+=0x3C){ f32 s=0.5f**(f32*)(w+0x24); PSMTXTrans(a,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC)); PSMTXScale(b,s,s,s); PSMTXRotRad(c,0x79,-0.017453292f**(f32*)((u8*)camGetPtr(4)+0x114)); PSMTXConcat(a,c,a); PSMTXConcat(a,b,a); PSMTXRotRad(c,0x7A,0.017453292f**(f32*)(w+0x28)); PSMTXConcat(a,c,a); PSMTXConcat(cam+0x11C,a,a); GXLoadPosMtxImm(a,0); GXSetCurrentMtx(0); col=(*(u8*)(w+0x2C)<<24)|(*(u8*)(w+0x2E)<<16)|(*(u8*)(w+0x30)<<8)|*(u8*)(w+0x32); GXSetTevColor(1,&col); col=(*(u8*)(w+0x34)<<24)|(*(u8*)(w+0x36)<<16)|(*(u8*)(w+0x38)<<8)|*(u8*)(w+0x3A); GXSetTevColor(2,&col); GXBegin(0x90,0,6); tri2((i&3)*4,(i&3)*4+1,(i&3)*4+2,(i&3)*4,(i&3)*4+2,(i&3)*4+3,0,0); }
}

void effPikkyoloMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPikkyoloDisp(void);
    extern const Vec3 vec3_802fbc98;
    extern f32 float_0p5_80425d0c;
    extern f32 float_0_80425d08;
    extern f32 float_neg0p7_80425d14;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 offset;
    f32 half;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc98;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 0x3E8) {
        *(s32*)(work + 0x1C) -= 1;
    }
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 8) {
        *(s16*)(work + 0x32) = *(s32*)(work + 0x1C) << 5;
    }

    half = float_0p5_80425d0c;
    zero = float_0_80425d08;
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x3C) {
        *(f32*)(work + 0x14) -= half;
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        if (*(f32*)(work + 8) < zero && *(f32*)(work + 0x14) < zero) {
            *(f32*)(work + 8) = zero;
            *(f32*)(work + 0x14) *= float_neg0p7_80425d14;
        }
        offset = (((i & 1) << 1) - 1) * 0xE;
        *(f32*)(work + 0x28) += (f32)offset;
    }
    dispEntry(4, 2, effPikkyoloDisp, effect, dispCalcZ(&dispPos));
}

