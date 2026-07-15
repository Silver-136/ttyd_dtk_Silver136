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


u8 effOpukuWprotectDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void* smartAlloc(u32, s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, scale, model;
    f32* ring = smartAlloc(0x280, 3);
    s32 i;

    PSMTXTrans(model, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXConcat(camera + 0x118, model, model);
    for (i = 0; i < 40; i++) {
        ring[i * 3] = (f32)(i - 20);
        ring[i * 3 + 1] = *(f32*)(work + 0x34);
        ring[i * 3 + 2] = 0.0f;
    }
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTevOrder(0, 0, 0, -1);
    effGetTexObj(0x34, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetCullMode(0);
    for (i = 0; i < 4; i++) {
        PSMTXTrans(trans, *(f32*)(work + 0x38 + i * 4), *(f32*)(work + 0x48 + i * 4), 0.0f);
        PSMTXScale(scale, *(f32*)(work + 0x2C), *(f32*)(work + 0x2C), 1.0f);
        PSMTXConcat(model, trans, trans);
        PSMTXConcat(trans, scale, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
    }
    GXSetNumChans(1);
    GXSetNumTevStages(2);
    GXSetCullMode(1);
    GXLoadPosMtxImm(model, 0);
    GXBegin(0x90, 0, 0x3C);
    GXBegin(0x90, 0, 0x18);
    return 0;
}

void effOpukuWprotectMain(void* effect) {
    extern void effDelete(void*); extern void effWaterDamageN64Entry(f32,f32,f32,f32,s32,s32);
    extern f64 sin(f64), cos(f64); extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effOpukuWprotectDisp(void); extern f32 float_24_80425c30,float_2_80425c34,float_6p2832_80425c38,float_360_80425c3c;
    extern f32 float_32_80425c44,float_127_80425c40,float_0p01_80425c48,float_256_80425c50,float_0p2_80425c54,float_128_80425c58,float_0_80425c10;
    u8* work=*(u8**)((s32)effect+0xC); s32 type=*(s32*)work; s32 timer; s32 frame;
    if(*(s32*)effect&4){ *(s32*)effect&=~4; *(s32*)(work+0x10)=type==1?4:0x10; }
    if(*(s32*)(work+0x10)<1000) *(s32*)(work+0x10)-=1;
    *(s32*)(work+0x14)+=1; timer=*(s32*)(work+0x10); frame=*(s32*)(work+0x14);
    if(timer<0){ if(type==1){ effWaterDamageN64Entry(*(f32*)(work+4),float_24_80425c30+*(f32*)(work+8),*(f32*)(work+0xC),float_2_80425c34,0,0x1E); } effDelete(effect); return; }
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
