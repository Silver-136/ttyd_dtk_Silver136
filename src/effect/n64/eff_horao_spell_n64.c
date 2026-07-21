#include "effect/n64/eff_horao_spell_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHoraoSpellWork {
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
    s32 color2R;
    s32 color2G;
    s32 color2B;
    s32 color2A;
    f32 scale;
} EffHoraoSpellWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effHoraoSpellMain(void* effEntry);
void effHoraoSpellDisp(s32 cameraId, void* effect);

extern const Vec3 vec3_802fb120;
extern char str_HoraoSpellN64_802fb138[];


#pragma use_lmw_stmw off
void* effHoraoSpellN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHoraoSpellWork* work;
    s32 flags;

    *(char**)((s32)entry + 0x14) = str_HoraoSpellN64_802fb138;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffHoraoSpellWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHoraoSpellMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->color2R = 20;
    work->color2G = 230;
    work->color2B = 50;
    work->color2A = 0xFF;

    return entry;
}
#pragma use_lmw_stmw on

void effHoraoSpellMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHoraoSpellWork* work;
    s32 flags;
    s32 frame;
    s32 timer;

    work = *(EffHoraoSpellWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb120;
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
        if (timer < 16) {
            work->alpha = timer << 4;
        }
        if (frame < 16) {
            work->alpha = (frame << 4) + 15;
        }
        dispEntry(4, 2, effHoraoSpellDisp, effEntry, dispCalcZ(&dispPos));
    }
}


void effHoraoSpellDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32,void*); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); extern f64 sin(f64);
    u8* w=*(u8**)((u8*)effect+0xC); void* cam=camGetPtr(cameraId); Mtx base,m,r,s; u8 tex[0x20]; u32 c; s32 i,frame=*(s32*)(w+0x14);
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC)); PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f); PSMTXConcat(m,r,m); PSMTXScale(s,*(f32*)(w+0x38),*(f32*)(w+0x38),*(f32*)(w+0x38)); PSMTXConcat(m,s,m); PSMTXConcat((u8*)cam+0x11C,m,base);
    c=((u8)*(s32*)(w+0x18)<<24)|((u8)*(s32*)(w+0x1C)<<16)|((u8)*(s32*)(w+0x20)<<8)|(u8)*(s32*)(w+0x24); GXSetTevColor(1,&c); c=((u8)*(s32*)(w+0x28)<<24)|((u8)*(s32*)(w+0x2C)<<16)|((u8)*(s32*)(w+0x30)<<8)|(u8)*(s32*)(w+0x34); GXSetTevColor(2,&c);
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTevStages(2); GXSetTevOrder(0,0,0,4); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,1,0,0,1,0); GXSetTevColorIn(0,0,8,8,15); GXSetTevAlphaIn(0,0,4,5,5); GXSetTevOrder(1,0xFF,0xFF,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,2,4,0,15); GXSetTevAlphaIn(1,0,6,1,7);
    effGetTexObjN64(0x73,tex); GXLoadTexObj(tex,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(s,0.0078125f,0.03125f,0.0f); GXLoadTexMtxImm(s,0x1E,1); GXSetCullMode(0);
    for(i=0;i<16;i++){f32 angle=(f32)(-282+i*18);PSMTXRotRad(r,0x7A,angle*0.0174533f);PSMTXConcat(base,r,m);PSMTXRotRad(r,0x78,0.34907f);PSMTXTrans(s,6.0f,0.0f,0.0f);PSMTXConcat(r,s,s);PSMTXRotRad(r,0x79,-30.0f*(f32)sin((6.2832f*(78.75f+frame*8.0f))/360.0f)*0.0174533f);PSMTXConcat(s,r,s);PSMTXConcat(m,s,m);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);c=0x46B47800|(u8)*(s32*)(w+0x24);GXSetTevColor(1,&c);effSetVtxDescN64((void*)0x803A4A00);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}
