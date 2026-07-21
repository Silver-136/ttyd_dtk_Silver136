#include "effect/n64/eff_opuku_wgun_n64.h"

void* effOpukuWgunN64Entry(f32 x, f32 y, f32 z, f32 dstX, f32 dstY, f32 dstZ, f32 scale, s32 type, s32 time) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effOpukuWgunMain(void*);
    extern char str_OpukuWgunN64_802fbc00[];
    extern f32 float_10_80425c00, float_2_80425bc4, float_0_80425be4;
    void* entry;
    u8* work;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OpukuWgunN64_802fbc00;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x1E4);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuWgunMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(f32*)(work + 4) = x; *(f32*)(work + 8) = y; *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = x; *(f32*)(work + 0x14) = y; *(f32*)(work + 0x18) = z;
    *(f32*)(work + 0x1C) = dstX; *(f32*)(work + 0x20) = dstY; *(f32*)(work + 0x24) = dstZ;
    *(s32*)(work + 0x28) = time;
    *(s32*)(work + 0x2C) = time < 1 ? 1000 : (type == 0 ? time * 2 : time * 4);
    *(s32*)(work + 0x30) = 0; *(s32*)(work + 0x40) = type == 0 ? 200 : 255;
    *(f32*)(work + 0x50) = scale;
    for (i = 0; i < 12; i++) {
        *(f32*)(work + 0x58 + i * 4) = float_10_80425c00;
        *(f32*)(work + 0x88 + i * 4) = float_2_80425bc4;
        *(f32*)(work + 0xB8 + i * 4) = float_0_80425be4;
        *(s32*)(work + 0x178 + i * 4) = i + 1;
    }
    return entry;
}

void effOpukuWgunMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOpukuWgunDisp(void);
    extern f64 sin(f64);
    extern const Vec3 vec3_802fbbe8;
    extern f32 float_6p2832_80425bd8;
    extern f32 float_360_80425bdc;
    extern f32 float_180_80425bf4;
    extern f32 float_120_80425bf8;
    extern f32 float_255_80425bfc;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 duration;
    s32 i;
    s32 step;
    f32 t;
    f32 phase;
    s32 alpha;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbbe8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    type = *(s32*)work;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x2C) = 0x10;
    }
    if (*(s32*)(work + 0x2C) < 0x3E8) {
        *(s32*)(work + 0x2C) -= 1;
    }
    *(s32*)(work + 0x30) += 1;
    if (*(s32*)(work + 0x2C) < 0) {
        effDelete(effect);
        return;
    }

    duration = *(s32*)(work + 0x28);
    *(s32*)(work + 0x54) += 1;
    if (*(s32*)(work + 0x54) > 0xC) {
        *(s32*)(work + 0x54) = 0xC;
    }

    p = work;
    for (i = 0; i < 0xC; i++, p += 4) {
        if (*(s32*)(p + 0x178) != 0) {
            *(s32*)(p + 0x178) -= 1;
            if (*(s32*)(p + 0x178) != 0) {
                continue;
            }
        }

        step = *(s32*)(p + 0x1A8) + 1;
        *(s32*)(p + 0x1A8) = step;
        if (step >= duration) {
            step = duration;
        }

        t = (f32)step / (f32)duration;
        if (type == 0) {
            *(f32*)(p + 0xE8) = *(f32*)(work + 0x10) + t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10));
            *(f32*)(p + 0x118) = *(f32*)(work + 0x14) + t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14));
            *(f32*)(p + 0x148) = *(f32*)(work + 0x18) + t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
        } else {
            phase = (float_6p2832_80425bd8 * (float_180_80425bf4 * t)) / float_360_80425bdc;
            *(f32*)(p + 0xE8) = *(f32*)(work + 0x10) + t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10));
            alpha = (s32)(float_255_80425bfc - (float_255_80425bfc * t));
            *(f32*)(p + 0x118) = *(f32*)(work + 0x14) + t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14)) + float_120_80425bf8 * (f32)sin(phase);
            *(f32*)(p + 0x148) = *(f32*)(work + 0x18) + t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
            *(u8*)(work + 0x1D8 + i) = (u8)alpha;
        }
    }

    dispEntry(4, 2, effOpukuWgunDisp, effect, dispCalcZ(&dispPos));
}


void effOpukuWgunDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Vtx { s16 x,y,z,s,t; u8 r,g,b,a; } Vtx;
    extern void* camGetPtr(s32); extern void* smartAlloc(u32,s32); extern f64 angleABf(f32,f32,f32,f32);
    extern f64 sin(f64); extern f64 cos(f64); extern void GXSetTevColor(s32,void*); extern void GXSetCullMode(s32);
    extern void PSMTXScale(Mtx,f32,f32,f32); extern void DCFlushRange(void*,s32); extern void GXInvalidateVtxCache(void);
    extern void effSetVtxDescN64(void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* work=*(u8**)((u8*)effect+0xC); u8* camera=camGetPtr(cameraId); Vtx* v=smartAlloc(0x2A0,3); Mtx mtx; u8 tex[0x20];
    u32 color0,color1; s32 type=*(s32*)work; s32 i; s16 texS=0; f32 angle=0.0f,thickness,radius,sx,sy;
    color0=(*(u8*)(work+0x34)<<24)|(*(u8*)(work+0x38)<<16)|(*(u8*)(work+0x3C)<<8)|*(u8*)(work+0x40);
    color1=(*(u8*)(work+0x44)<<24)|(*(u8*)(work+0x48)<<16)|(*(u8*)(work+0x4C)<<8)|0xFF;
    GXSetTevColor(1,&color0); GXSetTevColor(2,&color1); GXSetCullMode(0);
    for(i=0;i<12;i++) {
        if(i==0 || (i!=11 && *(s32*)(work+0x178+i*4)==0))
            angle=-(f32)angleABf(*(f32*)(work+0x11C+i*4),-*(f32*)(work+0xEC+i*4),*(f32*)(work+0x118+i*4),-*(f32*)(work+0xE8+i*4));
        thickness=(type==0?2.0f:1.0f)+0.5f*(f32)*(s32*)(work+0x1A8+i*4); if(type==0&&thickness>6.0f)thickness=6.0f; if(type!=0&&thickness>30.0f)thickness=30.0f;
        radius=thickness**(f32*)(work+0x50); sx=radius*(f32)sin(6.2832f*angle/360.0f); sy=radius*(f32)cos(6.2832f*angle/360.0f);
        v[i*2].x=(s16)(20.0f*(*(f32*)(work+0xE8+i*4)+sx)); v[i*2].y=(s16)(20.0f*(*(f32*)(work+0x118+i*4)+sy)); v[i*2].z=(s16)(20.0f**(f32*)(work+0x148+i*4));
        v[i*2].s=texS; v[i*2].t=0; v[i*2].r=0xFF; v[i*2].g=0xFF; v[i*2].b=0xFF; v[i*2].a=*(u8*)(work+0x1D8+i);
        v[i*2+1].x=(s16)(20.0f*(*(f32*)(work+0xE8+i*4)-sx)); v[i*2+1].y=(s16)(20.0f*(*(f32*)(work+0x118+i*4)-sy)); v[i*2+1].z=(s16)(20.0f**(f32*)(work+0x148+i*4));
        v[i*2+1].s=texS; v[i*2+1].t=0x400; v[i*2+1].r=0xFF; v[i*2+1].g=0xFF; v[i*2+1].b=0xFF; v[i*2+1].a=*(u8*)(work+0x1D8+i); texS+=0x140;
    }
    PSMTXScale(mtx,0.05f,0.05f,0.05f); DCFlushRange(v,0x2A0); GXInvalidateVtxCache(); effSetVtxDescN64(v);
    GXLoadPosMtxImm(camera+0x11C,0); GXSetCurrentMtx(0); GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTevStages(2);
    GXSetTevOrder(0,0,0,type==0?-1:4); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,type==0?3:2,type==0?2:8,type==0?8:5,15); GXSetTevAlphaIn(0,7,5,4,7);
    GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(mtx,type==0?0.015625f:0.03125f,0.03125f,0.0f); GXLoadTexMtxImm(mtx,0x1E,1);
    effGetTexObjN64(type==0?0x8D:0x8E,tex); GXLoadTexObj(tex,0);
    for(i=0;i<11;i++){ GXBegin(0x90,0,6); tri2(i*2,i*2+2,i*2+1,i*2,i*2+1,i*2+2,i*2+3,0); }
}

