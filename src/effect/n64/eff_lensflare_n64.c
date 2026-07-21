#include "effect/n64/eff_lensflare_n64.h"

void* effLensflareN64Entry(f32 x, f32 y, f32 z, s32 arg3, s32 life) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effLensflareMain(void);
    extern s32 rand(void);
    extern char str_LensflareN64_802fb408[];
    extern f32 float_0p1_804259a0;
    extern f32 float_0p4_804259c0;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 step;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_LensflareN64_802fb408;
    *(s32*)((s32)entry + 8) = 6;
    work = __memAlloc(3, 0x138);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effLensflareMain;
    *(u32*)entry |= 2;

    *(s32*)(work + 4) = arg3;
    *(s32*)(work + 0x1C) = 0;
    if (life < 1) {
        *(s32*)(work + 0x18) = 100;
    } else {
        *(s32*)(work + 0x18) = life;
    }
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;

    part = work + 0x34;
    step = 2;
    for (i = 1; i < 6; i++, step += 2, part += 0x34) {
        *(f32*)(part + 0x20) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x2C) = (f32)((step % 0x14) + 0x3C);
        *(f32*)(part + 0x28) = float_0p1_804259a0 * (f32)(rand() % 0x10) + float_0p4_804259c0;
        *(f32*)(part + 0x30) = float_0p1_804259a0 * (f32)(rand() % 0xB) + float_0p1_804259a0;
    }

    return entry;
}

void effLensflareMain(void* effect) {
    extern void effDelete(void*);
    extern f64 cos(f64);
    extern s32 rand(void);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effLensflareDisp(s32, void*);
    u8* entry = effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* part = work;
    f32 pos[3];
    s32 timer;
    s32 i;

    pos[0] = *(f32*)(work + 8);
    pos[1] = *(f32*)(work + 0xC);
    pos[2] = *(f32*)(work + 0x10);
    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        *(s32*)(work + 0x18) = 30;
    }
    if (*(s32*)(work + 0x18) < 100) {
        (*(s32*)(work + 0x18))--;
    }
    (*(s32*)(work + 0x1C))++;
    timer = *(s32*)(work + 0x18);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 21) {
        *(s32*)(work + 0x14) = *(s32*)(work + 0x1C) * 12;
    } else if (timer < 20) {
        *(s32*)(work + 0x14) = timer * 12;
    }
    for (i = 1; i < *(s32*)(entry + 8); i++, part += 0x34) {
        *(s32*)(part + 0x48) = (s32)(255.0f * (f32)cos(
            6.2832f * (90.0f * (*(f32*)(part + 0x60) - 70.0f) / 10.0f) / 360.0f));
        *(f32*)(part + 0x60) += *(f32*)(part + 0x64);
        if (*(f32*)(part + 0x60) > 80.0f) {
            *(f32*)(part + 0x60) = 60.0f;
            *(f32*)(part + 0x5C) = 0.1f * (f32)(rand() % 16) + 0.4f;
            *(f32*)(part + 0x64) = 0.1f * (f32)(rand() % 11) + 0.1f;
            *(f32*)(part + 0x54) = (f32)(rand() % 360);
        }
    }
    dispEntry(4, 2, effLensflareDisp, effect, dispCalcZ(pos));
}


void effLensflareDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXSetTevColor(s32,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetCullMode(s32); extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC);u8* p=w+0x34;void* cam=camGetPtr(cameraId);Mtx base,m,r,s;u8 tex[0x20];u32 color;u32 flags=*(u32*)(w+0x1C);s32 alpha=*(s32*)(w+0x14),i,angle=10,yrot=35;if(flags&1)alpha=(s32)(0.95f*(f32)alpha);
    PSMTXTrans(m,*(f32*)(w+8),*(f32*)(w+0xC),*(f32*)(w+0x10));PSMTXRotRad(r,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f);PSMTXConcat(m,r,base);PSMTXConcat((u8*)cam+0x11C,base,base);color=0xFFFFFF00|(u8)alpha;GXSetTevColor(1,&color);PSMTXRotRad(r,0x7A,(f32)(flags+(flags&1)*2)*0.0174533f);PSMTXConcat(base,r,m);GXLoadPosMtxImm(m,0);GXSetCurrentMtx(0);GXSetCullMode(0);GXSetNumChans(0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,2,4,8,15);GXSetTevAlphaIn(0,0,1,4,7);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(s,0.0625f,0.0625f,0.0f);GXLoadTexMtxImm(s,0x1E,1);effGetTexObjN64(0x6F,tex);GXLoadTexObj(tex,0);effSetVtxDescN64((void*)0x803A59A0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
    effGetTexObjN64(0x86,tex);GXLoadTexObj(tex,0);PSMTXScale(s,0.03125f,0.0078125f,0.0f);GXLoadTexMtxImm(s,0x1E,1);effSetVtxDescN64((void*)0x803A59D8);
    for(i=1;i<*(s32*)((u8*)effect+8);i++,p+=0x34,angle+=10,yrot+=35){f32 sc=*(f32*)(p+0x28);if(sc==0.0f)continue;color=0xFFFFFF00|(u8)((*(s32*)(p+0x14)*alpha)>>8);GXSetTevColor(1,&color);PSMTXRotRad(r,0x7A,*(f32*)(p+0x20)*0.0174533f);PSMTXTrans(m,*(f32*)(p+0x2C),0.0f,0.0f);PSMTXConcat(r,m,m);PSMTXScale(s,sc,sc,sc);PSMTXConcat(m,s,m);PSMTXConcat(base,m,m);GXLoadPosMtxImm(m,0);GXBegin(0x90,0,6);tri2(12,13,14,0,12,14,15,0);PSMTXScale(s,2.3f-sc,2.3f-sc,2.3f-sc);PSMTXConcat(m,s,m);GXLoadPosMtxImm(m,0);GXBegin(0x90,0,6);tri2(8,9,10,0,8,10,11,0);PSMTXRotRad(r,0x7A,(14.0f+*(f32*)(p+0x20))*0.0174533f);PSMTXRotRad(s,0x79,(f32)(flags+angle)*0.0174533f);PSMTXConcat(r,s,r);PSMTXTrans(m,20.0f,0.0f,0.0f);PSMTXConcat(r,m,m);PSMTXConcat(base,m,m);GXLoadPosMtxImm(m,0);GXBegin(0x90,0,6);tri2(4,5,6,0,4,6,7,0);}
}

