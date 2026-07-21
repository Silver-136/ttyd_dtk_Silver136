#include "effect/n64/eff_expbom_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffExpBomWork {
    s16 unk_00;
    s16 life;
    s16 unk_04;
    s16 divTimer;
    s32 timer;
    f32 baseX;
    f32 baseY;
    f32 baseZ;
    f32 x;
    f32 y;
    f32 vx;
    f32 vy;
    u8 pad_28[4];
} EffExpBomWork;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effExpBomDisp(int cameraId, int effect);

extern const Vec3 vec3_802fae10;
extern f32 float_1p25_804250d0;

void* effExpBomN64Entry(f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effExpBomMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_ExpBomN64_802fae28[];
    extern f32 float_0_804250ac;
    extern f32 float_2_804250d4;
    extern f32 float_6p2832_804250b4;
    extern f32 float_90_804250b8;
    extern f32 float_360_804250c0;
    extern f32 bom_dir[];
    extern f32 bom_spd[];
    void* entry;
    u8* work;
    s32 i;
    f32 angle;
    f32 spd;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ExpBomN64_802fae28;
    *(s32*)((s32)entry + 8) = 10;
    work = __memAlloc(3, 0x1B8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effExpBomMain;

    for (i = 0; i < *(s32*)((s32)entry + 8); i++) {
        *(s16*)(work + 0) = 0;
        *(s16*)(work + 4) = (s16)(rand() % 6 + 0x10);
        *(s16*)(work + 2) = *(s16*)(work + 4);
        *(s16*)(work + 6) = 4;
        *(s32*)(work + 8) = 0;
        *(f32*)(work + 0xC) = x;
        *(f32*)(work + 0x10) = y;
        *(f32*)(work + 0x14) = z;
        *(f32*)(work + 0x18) = float_0_804250ac;
        *(f32*)(work + 0x1C) = float_0_804250ac;
        angle = (float_90_804250b8 + bom_dir[i]) * float_6p2832_804250b4 / float_360_804250c0;
        spd = float_2_804250d4 * bom_spd[i];
        *(f32*)(work + 0x20) = spd * (f32)sin(angle);
        *(f32*)(work + 0x24) = -spd * (f32)cos(angle);
        work += 0x2C;
    }

    return entry;
}

void effExpBomMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffExpBomWork* work;
    register s32 i;
    register s32 anyAlive;
    f32 div;

    anyAlive = 0;
    i = 0;
    work = *(EffExpBomWork**)((s32)effect + 0xC);
    zero = &vec3_802fae10;
    pos = *zero;
    pos.x = work->baseX;
    pos.y = work->baseY;
    pos.z = work->baseZ;
    dispPos = pos;

    while (i < *(s32*)((s32)effect + 0x8)) {
        if (work->life >= 0) {
            work->timer--;
            if (work->timer < 0) {
                anyAlive = 1;
                work->x += work->vx;
                work->y += work->vy;
                work->unk_00++;
                work->life--;
                if (work->life >= 0) {
                    work->divTimer--;
                    if (work->divTimer < 0) {
                        div = float_1p25_804250d0;
                        work->vx /= div;
                        work->vy /= div;
                    }
                }
            }
        }
        i++;
        work++;
    }

    if (anyAlive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effExpBomDisp, effect, dispCalcZ(&dispPos));
    }
}


void effExpBomDisp(int cameraId, int effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetTevColor(s32,void*); extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32); extern void GXBegin(s32,s32,s32); extern f64 sin(f64);
    u8* work=*(u8**)(effect+0xC); u8* p=work; void* cam=camGetPtr(cameraId);
    Mtx base,trans,rot,scale; u8 texObj[0x20]; u32 color; s32 i,frame,tex; f32 phase;
    PSMTXTrans(base,*(f32*)(work+0xC),*(f32*)(work+0x10),*(f32*)(work+0x14));
    PSMTXRotRad(rot,0x79,-*(f32*)((u8*)cam+0x114)*0.0174533f); PSMTXConcat(base,rot,base);
    PSMTXConcat((u8*)cam+0x11C,base,base);
    for(i=0;i<*(s32*)(effect+8);i++,p+=0x2C){
        if(*(s16*)(p+2)<0||*(s32*)(p+8)>=0)continue;
        frame=*(s16*)(p+2);if(frame>16)frame=16;
        PSMTXTrans(trans,*(f32*)(p+0x18),*(f32*)(p+0x1C),0.0f);
        PSMTXScale(scale,*(f32*)(p+0x28),*(f32*)(p+0x28),*(f32*)(p+0x28));
        PSMTXRotRad(rot,0x7A,*(f32*)(p+0x24)*0.0174533f);
        PSMTXConcat(trans,scale,trans);PSMTXConcat(trans,rot,trans);PSMTXConcat(base,trans,trans);GXLoadPosMtxImm(trans,0);
        phase = 6.2832f * 90.0f * ((f32)*(s16*)p - 1.0f) / ((f32)*(s16*)(p + 4) * 360.0f);
        tex = (s32)(7.0f * (f32)sin((f64)phase));
        if(tex<0)tex=0;if(tex>6)tex=6;
        color=0xFFFFFF00|(u8)((frame*210)>>4);GXSetTevColor(2,&color);
        GXSetNumChans(0);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);GXSetTexCoordGen2(1,1,4,0x3C,0,0x7D);
        GXSetNumTevStages(3);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,8);GXSetTevAlphaIn(0,0,0,0,4);
        GXSetTevOrder(1,1,1,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,0,8,6,15);GXSetTevAlphaIn(1,0,4,6,7);
        GXSetTevOrder(2,0xFF,0xFF,0xFF);GXSetTevColorOp(2,0,0,0,1,0);GXSetTevAlphaOp(2,0,0,0,1,0);GXSetTevColorIn(2,0,0,0,0);GXSetTevAlphaIn(2,0,6,1,7);
        GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(0xD,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,0xD,1,4,0);
        effGetTexObjN64(tex,texObj);GXLoadTexObj(texObj,0);effGetTexObjN64(tex==6?6:tex+1,texObj);GXLoadTexObj(texObj,1);GXBegin(0x80,0,4);
    }
}
