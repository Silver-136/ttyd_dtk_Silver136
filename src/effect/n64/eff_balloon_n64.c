#include "effect/n64/eff_balloon_n64.h"

typedef struct EffBalloonWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 unk14;
    f32 unk18;
    s32 timer;
    s32 frame;
} EffBalloonWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_BalloonN64_802fab68[];
extern f32 float_0_80424cbc;

void effBalloonDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void PSMTXScale(void*,f32,f32,f32);extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetCullMode(s32);extern void GXBegin(s32,s32,s32);extern void tri1(s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC);void* cam=camGetPtr(cameraId);u8 tex[0x20];Mtx m,r,s; s32 i,type=*(s32*)(w+0x1C);
    effGetTexObjN64(type==0?0x5F:type==1?0x5E:0x5D,tex);GXLoadTexObj(tex,0);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(s,0.0625f,0.0625f,0.0f);GXLoadTexMtxImm(s,0x1E,1);
    GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,12);GXSetTevAlphaIn(0,0,0,0,4);
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f);PSMTXConcat(m,r,m);PSMTXScale(s,*(f32*)(w+0x10),*(f32*)(w+0x10),1.0f);PSMTXConcat(m,s,m);PSMTXConcat((u8*)cam+0x11C,m,m);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);GXSetCullMode(0);
    if(type<100){GXBegin(0x90,0,3);tri1(0x15,2,1);}else{GXBegin(0x90,0,6);tri1(0,1,2);tri1(0,0x15,1);}for(i=0;i<19;i++){GXBegin(0x90,0,3);tri1(1,i+2,i+3);}
}

void effBalloonMain(void* entry) {
    extern void effDelete(void* entry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effBalloonDisp(void);
    extern f32 float_0_80424cbc;
    extern f32 float_0p1_80424cc4;
    extern f32 float_6p2832_80424cc8;
    extern f32 float_360_80424ccc;
    extern f32 float_17_80424cd0;
    extern f32 float_21_80424cd4;
    extern f32 float_30_80424cd8;
    extern f32 float_5_80424cdc;
    extern f32 float_2_80424ce0;
    extern f32 float_1p5_80424ce4;
    extern f32 float_50_80424ce8;
    extern f32 float_1_80424cec;
    extern f32 float_1p2_80424cf0;
    EffBalloonWork* work = *(EffBalloonWork**)((s32)entry + 0xC);
    f32 pos[3];
    f32* points;
    s32 timer;
    s32 frame;
    s32 i;
    f32 phase;
    f32 amp;

    points = (f32*)((s32)work + 0x24);
    pos[0] = work->x;
    pos[1] = work->y;
    pos[2] = work->z;

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        work->timer = 99;
    }
    if (work->timer < 100) {
        work->timer--;
    }
    work->frame++;
    if (work->frame > 0x4F1A0) {
        work->frame = 0x100;
    }

    timer = work->timer;
    if (timer < 0) {
        effDelete(entry);
        return;
    }

    frame = work->frame;
    phase = (float_6p2832_80424cc8 * (f32)(frame * 3)) / float_360_80424ccc;
    work->unk18 = float_0p1_80424cc4 * (work->unk14 - work->unk18) + work->unk18;
    amp = float_0p1_80424cc4 * work->unk18 * (f32)sin((f64)phase) + work->unk18;

    for (i = 0; i < 0x14; i++) {
        f32 angle;
        f32 wobble;
        f32 extraAngle;

        angle = (float_6p2832_80424cc8 * (float_17_80424cd0 + (float_360_80424ccc * (f32)i) / float_21_80424cd4)) / float_360_80424ccc;
        points[0] = amp * (f32)sin((f64)angle);
        points[1] = -amp * (f32)cos((f64)angle);

        wobble = float_0p1_80424cc4 * (f32)sin((f64)((float_6p2832_80424cc8 * (f32)(i * 0x35)) / float_360_80424ccc)) + float_2_80424ce0;
        extraAngle = (float_6p2832_80424cc8 * ((float_30_80424cd8 * (float_5_80424cdc + (f32)i)) + ((f32)frame * wobble))) / float_360_80424ccc;
        points[0] += float_1p5_80424ce4 * (f32)sin((f64)extraAngle);

        wobble = float_0p1_80424cc4 * (f32)cos((f64)((float_6p2832_80424cc8 * (f32)(i * 0x24)) / float_360_80424ccc)) + float_2_80424ce0;
        extraAngle = (float_6p2832_80424cc8 * ((float_50_80424ce8 * (float_5_80424cdc + (f32)i)) + ((f32)frame * wobble))) / float_360_80424ccc;
        points[1] += float_1p5_80424ce4 * (f32)cos((f64)extraAngle);

        points += 2;
    }

    *(f32*)((s32)work + 0xC4) = float_0_80424cbc;
    if (timer < 100) {
        *(f32*)((s32)work + 0xC8) += float_1_80424cec;
    } else {
        *(f32*)((s32)work + 0xC8) = float_1p2_80424cf0 * amp;
    }

    dispEntry(4, 2, effBalloonDisp, entry, dispCalcZ(pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBalloonN64Entry(s32 type, f32 x, f32 y, f32 z, f32 unk14, f32 scale) {
    void* entry = effEntry();
    EffBalloonWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BalloonN64_802fab68;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0xCC);
    *(EffBalloonWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBalloonMain;
    zero = float_0_80424cbc;
    *(u32*)entry |= 2;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->timer = 100;
    work->unk14 = unk14;
    work->unk18 = zero;
    work->frame = 0;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
