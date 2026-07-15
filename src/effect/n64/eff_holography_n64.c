#include "effect/n64/eff_holography_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHolographyWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 colorR;
    s32 colorG;
    s32 colorB;
    s32 alpha;
    f32 scaleStep;
} EffHolographyWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effHolographyMain(void* effEntry);

extern const Vec3 vec3_802fb0f8;
extern char str_HolographyN64_802fb110[];
extern f32 float_0p1_804254bc;


u8 effHolographyDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXBegin(s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC);void* cam=camGetPtr(cameraId);Mtx m,r,s;u8 tex[0x20];u32 color;s32 i,a,frame=*(s32*)(w+0x14);
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f);PSMTXScale(s,-*(f32*)(w+0x28),*(f32*)(w+0x28),*(f32*)(w+0x28));PSMTXConcat(m,r,m);PSMTXConcat(m,s,m);PSMTXConcat((u8*)cam+0x11C,m,m);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);
    GXSetNumChans(0);GXSetNumTevStages(3);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,8);GXSetTevAlphaIn(0,0,0,0,4);GXSetTevOrder(1,1,1,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,8,0,6,15);GXSetTevAlphaIn(1,4,0,6,7);GXSetTevOrder(2,1,1,0xFF);GXSetTevColorOp(2,0,0,0,1,0);GXSetTevAlphaOp(2,0,0,0,1,0);GXSetTevColorIn(2,0,0,0,0);GXSetTevAlphaIn(2,0,6,6,7);
    effGetTexObjN64(0x93,tex);GXLoadTexObj(tex,0);effGetTexObjN64(0x94,tex);GXLoadTexObj(tex,1);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x1E,0,0x7D);PSMTXScale(s,0.03125f,0.03125f,0.0f);GXLoadTexMtxImm(s,0x1E,1);
    for(i=0;i<10;i++){a=(s32)(2.0f*(f32)*(s32*)(w+0x24)*(1.0f-0.1f*(f32)i));if(a>255)a=255;color=((u8)*(s32*)(w+0x18)<<24)|((u8)*(s32*)(w+0x1C)<<16)|((u8)*(s32*)(w+0x20)<<8)|(u8)a;GXSetTevColor(1,&color);GXBegin(0x90,0,6);frame++;}
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effHolographyN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHolographyWork* work;
    s32 flags;
    f32 scaleStep;

    *(char**)((s32)entry + 0x14) = str_HolographyN64_802fb110;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffHolographyWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHolographyMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    scaleStep = float_0p1_804254bc * scale;
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scaleStep = scaleStep;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effHolographyMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHolographyWork* work;
    s32 flags;
    s32 frame;
    s32 timer;

    work = *(EffHolographyWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb0f8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if (flags & 4) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(effEntry);
    } else {
        frame = work->frame;
        if (frame < 16) {
            work->alpha = (frame << 4) + 15;
        } else {
            work->alpha = 0xFF;
        }
        if (timer < 16) {
            work->alpha = timer << 4;
        }
        dispEntry(4, 2, effHolographyDisp, effEntry, dispCalcZ(&dispPos));
    }
}
