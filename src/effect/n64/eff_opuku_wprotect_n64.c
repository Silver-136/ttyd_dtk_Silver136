#include "effect/n64/eff_opuku_wprotect_n64.h"

typedef struct EffOpukuWprotectWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 field_18;
    s32 field_1c;
    s32 field_20;
    s32 field_24;
    u8 pad_28[4];
    f32 scale;
    f32 field_30;
    f32 field_34;
    u8 pad_38[0x50];
    s32 field_88;
    s32 field_8c;
    s32 field_90;
    s32 field_94;
} EffOpukuWprotectWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_OpukuWprotectN64_802fbc28[];
extern f32 float_0_80425c10;


void effOpukuWprotectDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; typedef struct Vtx { s16 x,y,z,s,t; u8 r,g,b,a; } Vtx;
    extern void* camGetPtr(s32); extern void* smartAlloc(u32,s32); extern s32 effTblRandN64(s32,s32); extern f64 sin(f64);
    extern void PSMTXTrans(Mtx,f32,f32,f32); extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXSetTevColor(s32,void*); extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetCullMode(s32); extern void GXLoadPosMtxImm(Mtx,s32); extern void GXSetCurrentMtx(s32); extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); extern void DCFlushRange(void*,s32);
    extern void GXInvalidateVtxCache(void); extern u8 point_data[]; extern u32 gpGlobals[];
    u8* work=*(u8**)((u8*)effect+0xC); u8* camera=camGetPtr(cameraId); Mtx base,scale,trans; u8 tex[0x20];
    Vtx* vertices=smartAlloc(0x460,3); u32 color1,color2; u32 retrace=gpGlobals[0]>>1; s32 i;
    PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC)); PSMTXConcat((f32 (*)[4])(camera+0x11C),base,base);
    for(i=0;i<40;i++) {
        s8* p=(s8*)point_data+i*12; f32 x=(f32)(p[0]*10),y=(f32)(p[1]*10),z=(f32)(p[2]*10);
        if(y!=0.0f) {
            x+=20.0f*(f32)sin((f32)(retrace+effTblRandN64(1000,(s32)(100.0f*z+10.0f*y+x))));
            y+=20.0f*(f32)sin((f32)(retrace+effTblRandN64(1000,(s32)(z+10.0f*x+100.0f*y))));
            z+=20.0f*(f32)sin((f32)(retrace+effTblRandN64(1000,(s32)(10.0f*z+100.0f*x+y))));
        }
        vertices[i].x=(s16)x; vertices[i].y=(s16)y; vertices[i].z=(s16)z; vertices[i].s=*(s16*)(p+4); vertices[i].t=*(s16*)(p+6);
        vertices[i].r=p[8]; vertices[i].g=p[9]; vertices[i].b=p[10]; vertices[i].a=0x50;
    }
    color1=0xFFFFFF00|(u8)*(s32*)(work+0x24); color2=0xFFFFFF00|(u8)*(s32*)(work+0x28); GXSetTevColor(1,&color1); GXSetTevColor(2,&color2);
    GXSetNumChans(0); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,3,2,8,15); GXSetTevAlphaIn(0,7,5,4,7); effGetTexObjN64(0x91,tex); GXLoadTexObj(tex,0); GXSetNumTexGens(1);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(scale,0.03125f,0.03125f,0.0f); GXLoadTexMtxImm(scale,0x1E,1); GXSetCullMode(0);
    for(i=0;i<4;i++) if(*(s32*)(work+0x88+i*4)>=0) {
        PSMTXScale(scale,0.1f,0.1f,0.1f); PSMTXTrans(trans,*(f32*)(work+0x38+i*4),*(f32*)(work+0x48+i*4),0.0f); PSMTXConcat(scale,trans,trans);
        PSMTXScale(scale,*(f32*)(work+0x78+i*4),*(f32*)(work+0x78+i*4),*(f32*)(work+0x78+i*4)); PSMTXConcat(trans,scale,trans); PSMTXConcat(base,trans,trans);
        GXLoadPosMtxImm(trans,0); GXSetCurrentMtx(0); effSetVtxDescN64((void*)0x803A6A90); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);
    }
    GXSetNumChans(1); GXSetNumTevStages(2); effGetTexObjN64(0x90,tex); GXLoadTexObj(tex,0); DCFlushRange(vertices,0x460); GXInvalidateVtxCache();
    PSMTXScale(scale,0.1f,0.1f,0.1f); PSMTXConcat(base,scale,scale); GXLoadPosMtxImm(scale,0); GXSetCurrentMtx(0); effSetVtxDescN64(vertices);
    GXBegin(0x90,0,0x3C); GXBegin(0x90,0,0x18);
}

void effOpukuWprotectMain(void* effect) {
    extern void effDelete(void*); extern void effWaterDamageN64Entry(f32,f32,f32,f32,s32,s32);
    extern f64 sin(f64), cos(f64); extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effOpukuWprotectDisp(void); extern f32 float_24_80425c30,float_2_80425c34,float_6p2832_80425c38,float_360_80425c3c;
    extern f32 float_32_80425c44,float_127_80425c40,float_0p01_80425c48,float_256_80425c50,float_0p2_80425c54,float_128_80425c58,float_0_80425c10;
    u8* work=*(u8**)((s32)effect+0xC); s32 type=*(s32*)work; s32 timer; s32 frame;
    if(*(s32*)effect&4){ *(s32*)effect&=~4; *(s32*)(work+0x10)=type==1?4:0x10; }
    if(*(s32*)(work+0x10)<1000) *(s32*)(work+0x10)-=1;
    *(s32*)(work+0x14)+=1; if(*(s32*)(work+0x14)>0x4F1A0)*(s32*)(work+0x14)=0x100; timer=*(s32*)(work+0x10); frame=*(s32*)(work+0x14);
    if(timer<0){ if(type==1){ effWaterDamageN64Entry(*(f32*)(work+4),float_24_80425c30+*(f32*)(work+8),*(f32*)(work+0xC),float_2_80425c34,0,0x1E); effWaterDamageN64Entry(*(f32*)(work+4),float_24_80425c30+*(f32*)(work+8),*(f32*)(work+0xC),float_2_80425c34,1,0x1E); } effDelete(effect); return; }
    if(type==0){ if(timer<0x10){*(s32*)(work+0x24)=timer<<4;*(s32*)(work+0x28)=timer<<4;} if(frame<0x10){*(s32*)(work+0x24)=(frame<<4)+0xF;*(s32*)(work+0x28)=(frame<<4)+0xF;} }
    else if(frame<4){ *(s32*)(work+0x24)=(frame<<6)+0x3F; *(s32*)(work+0x28)=*(s32*)(work+0x24); }
    *(s32*)(work+0x1C)=(s32)(float_32_80425c44*(f32)sin((f64)(float_6p2832_80425c38*(f32)(frame*7)/float_360_80425c3c))+float_127_80425c40);
    *(f32*)(work+0x34)+=float_0p2_80425c54; if(*(f32*)(work+0x34)>=float_128_80425c58)*(f32*)(work+0x34)=float_0_80425c10;
    dispEntry(4,2,effOpukuWprotectDisp,effect,dispCalcZ(work+4));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effOpukuWprotectN64Entry(s32 type, f32 x, f32 y, f32 z, s32 timer, f32 scale) {
    void* entry;
    EffOpukuWprotectWork* work;
    s32 zero;
    f32 zeroFloat;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_OpukuWprotectN64_802fbc28;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x98);
    *(EffOpukuWprotectWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuWprotectMain;

    zero = 0;
    *(u32*)entry |= 2;
    work->type = type;
    work->frame = zero;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }

    work->field_24 = 0;
    zeroFloat = float_0_80425c10;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->field_18 = 0x2F;
    work->field_1c = 0x7F;
    work->field_20 = 0xFF;
    work->field_30 = zeroFloat;
    work->field_34 = zeroFloat;
    work->field_88 = -1;
    work->field_8c = -5;
    work->field_90 = -9;
    work->field_94 = -13;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
