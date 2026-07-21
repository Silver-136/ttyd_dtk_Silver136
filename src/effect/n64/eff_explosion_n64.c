#include "effect/n64/eff_explosion_n64.h"
void effExplosionMain(void* effect);
void effExplosionDisp(s32 cameraId, void* effect);

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffExplosionWork {
    u16 type;
    u8 pad_02[2];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 timer;
    s32 frame;
    s32 duration;
    f32 progress;
    f32 angle;
    f32 angleStep;
} EffExplosionWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fae38;
extern char str_ExplosionN64_802fae50[];
extern f32 float_0_804250f0;
extern f32 float_8_80425118;
extern f32 float_0p1_8042511c;
extern f32 float_40_80425120;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effExplosionN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffExplosionWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_ExplosionN64_802fae50;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffExplosionWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effExplosionMain;
    zero = float_0_804250f0;

    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = zero;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->angle = zero;
    if (type == 0) {
        work->angleStep = zero;
    } else {
        work->angleStep = float_40_80425120;
    }

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effExplosionMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffExplosionWork* work;

    work = *(EffExplosionWork**)((s32)effect + 0xC);
    zero = &vec3_802fae38;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_8_80425118 * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        work->angle += work->angleStep;
        if (work->type == 0) {
            work->scale += float_0p1_8042511c;
        }
        dispEntry(4, 2, effExplosionDisp, effect, dispCalcZ(&dispPos));
    }
}


void effExplosionDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);
    extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);
    extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetTevColor(s32,void*);extern void GXSetTevKColor(s32,void*);
    extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetTevKColorSel(s32,s32);
    extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC);void* cam=camGetPtr(cameraId);Mtx m,t,r,s;u8 tex[0x20];u32 c;f32 fade; s32 frame=(s32)*(f32*)(w+0x20);
    PSMTXTrans(t,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f);PSMTXScale(s,*(f32*)(w+0x10),*(f32*)(w+0x10),*(f32*)(w+0x10));
    PSMTXConcat(t,r,m);PSMTXConcat(m,s,m);PSMTXConcat((u8*)cam+0x11C,m,m);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);
    effGetTexObjN64(0xE,tex);GXLoadTexObj(tex,0);GXLoadTexObj(tex,1);
    if(*(s32*)w==0){fade=*(f32*)(w+0x20)>5.0f?1.0f-(*(f32*)(w+0x20)-5.0f):1.0f;if(fade<0.0f)fade=0.0f;c=((u8)(255.0f*fade)<<24)|((u8)(18.0f*fade)<<16)|((u8)(59.0f*fade)<<8)|0xFF;GXSetTevColor(1,&c);c=((u8)(211.0f*fade)<<24)|((u8)(255.0f*fade)<<16)|((u8)(216.0f*fade)<<8)|0xFF;GXSetTevKColor(0,&c);GXSetNumTevStages(4);}else{c=0xFFFFFFFF;GXSetTevColor(1,&c);GXSetNumTevStages(3);}
    GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,8);GXSetTevAlphaIn(0,0,0,0,4);
    GXSetTevOrder(1,1,1,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,0,8,6,15);GXSetTevAlphaIn(1,0,4,6,7);
    GXSetNumChans(0);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    PSMTXScale(s,0.00390625f,0.03125f,0.0f);PSMTXTrans(t,(f32)(frame*32),0.0f,0.0f);PSMTXConcat(s,t,s);GXLoadTexMtxImm(s,0x1E,1);
    PSMTXScale(s,0.00390625f,0.03125f,0.0f);PSMTXTrans(t,(f32)((frame+1)*32),0.0f,0.0f);PSMTXConcat(s,t,s);GXLoadTexMtxImm(s,0x21,1);
    GXSetCullMode(0);effSetVtxDescN64((void*)0x8039F2E0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
}
