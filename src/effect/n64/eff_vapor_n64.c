#include "effect/n64/eff_vapor_n64.h"
void effVaporMain(void* effect);
void effVaporDisp(s32 cameraId, void* effect);

typedef struct EffVaporWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 r1;
    s32 g1;
    s32 b1;
    s32 a1;
    f32 rot;
    f32 yVel;
    f32 scale;
} EffVaporWork;

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effDelete(void* effect);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param);
f32 dispCalcZ(Vec3* pos);

extern const char str_VaporN64_802fc230[];
extern f32 float_0_80426464;
extern f32 float_0p5_8042645c;
extern f32 float_0p02_80426480;
extern f32 float_neg1p5_80426484;
extern f32 float_1_80426488;
extern Vec3 vec3_802fc210[];


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffVaporWork* work;
    s32 full;
    s32 zeroInt;
    f32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_VaporN64_802fc230;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x44);
    *(EffVaporWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effVaporMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    full = 0xFF;
    zero = float_0_80426464;
    work->a0 = full;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = full;
    work->r1 = full;
    work->g1 = full;
    work->b1 = full;
    work->a1 = full;
    work->rot = zero;
    work->yVel = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effVaporMain(void* effect) {
    EffVaporWork* work;
    Vec3 dispPos;
    Vec3 pos;
    u32 flags;
    s32 type;

    work = *(EffVaporWork**)((s32)effect + 0xC);
    pos = vec3_802fc210[0];
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(u32*)effect;
    type = work->type;
    if (flags & 4) {
        *(u32*)effect = flags & ~4;
        work->lifetime = 16;
    }

    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;

    if (work->lifetime < 0) {
        effDelete(effect);
    } else {
        if (work->lifetime < 16) {
            work->a0 = work->lifetime << 4;
        }
        if (type < 2) {
            if (type >= 0) {
                work->yVel += float_0p02_80426480;
            }
        }
        work->y += work->yVel;
        if (work->yVel > float_0p5_8042645c) {
            work->yVel = float_0p5_8042645c;
        }
        if (work->yVel < float_neg1p5_80426484) {
            work->yVel = float_neg1p5_80426484;
        }
        work->rot += float_1_80426488;
        dispCalcZ(&dispPos);
        dispEntry(4, 2, effVaporDisp, effect);
    }
}

void effVaporDisp(s32 cameraId, void* effect)  {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void GXSetTevColor(s32,void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((s32)effect+0xC);
    void* cam=camGetPtr(cameraId);
    f32 t[3][4],r[3][4],s[3][4],base[3][4];
    u8 texObj[0x20];
    u32 c;
    s32 type=*(s32*)w;
    s32 frame=*(s32*)(w+0x14);
    s32 seq,cell,frac;
    PSMTXTrans(t,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));
    PSMTXScale(s,*(f32*)(w+0x40),*(f32*)(w+0x40),*(f32*)(w+0x40));
    PSMTXRotRad(r,0x79,0.017453292f*-*(f32*)((u8*)camGetPtr(cameraId)+0x114));
    PSMTXConcat(t,r,t);
    PSMTXConcat(t,s,t);
    PSMTXConcat((u8*)cam+0x11C,t,base);
    c=(*(s32*)(w+0x18)<<24)|(*(s32*)(w+0x1C)<<16)|(*(s32*)(w+0x20)<<8)|((*(s32*)(w+0x24)>>1)&0xFF);
    GXSetTevColor(1,&c);
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,0xFF);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,0,0,0,8);
    GXSetTevAlphaIn(0,0,0,0,4);
    GXSetTevOrder(1,1,1,0xFF);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,0,8,7,0);
    GXSetTevAlphaIn(1,6,4,7,7);
    GXSetTevOrder(2,0xFF,0xFF,0xFF);
    GXSetTevColorOp(2,0,0,0,1,0);
    GXSetTevAlphaOp(2,0,0,0,1,0);
    GXSetTevColorIn(2,3,2,0,0);
    GXSetTevAlphaIn(2,0,7,6,7);
    effGetTexObjN64((type==0||type==2)?0x69:0x6A,texObj);
    GXLoadTexObj(texObj,0);
    GXLoadTexObj(texObj,1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803AB748);
    if(frame>29)frame=29;
    PSMTXTrans(t,0.0f,0.0f,0.0f);
    PSMTXRotRad(r,0x7A,-0.017453292f*(f32)frame);
    PSMTXScale(s,1.0f,1.0f,1.0f);
    PSMTXConcat(t,r,t);
    PSMTXConcat(t,s,t);
    PSMTXConcat(base,t,t);
    GXLoadPosMtxImm(t,0);
    GXSetCurrentMtx(0);
    seq=(type*30+frame)%25;
    cell=(s32)(5.0f*(f32)seq*0.00390625f);
    frac=(s32)(256.0f*(5.0f*(f32)seq*0.00390625f-(f32)cell));
    PSMTXScale(s,0.032258f,0.007874f,0.0f);
    PSMTXTrans(t,0.0f,32.0f,0.0f);
    PSMTXConcat(s,t,s);
    GXLoadTexMtxImm(s,0x1E,1);
    if(cell<5) {
        c=(*(s32*)(w+0x28)<<24)|(*(s32*)(w+0x2C)<<16)|(*(s32*)(w+0x30)<<8)|(frac&0xFF);
        GXSetTevColor(2,&c);
        GXBegin(0x90,0,6);
        tri2(cell*4,cell*4+1,cell*4+2,0,cell*4,cell*4+2,cell*4+3,0);
    }
}
