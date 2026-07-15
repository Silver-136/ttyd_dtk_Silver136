#include "effect/n64/eff_ripple_n64.h"

typedef struct EffRippleWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 scale;
    s32 frame;
    s32 lifetime;
    s32 field_20;
    s32 randAngle;
    f32 y2;
    f32 rx;
    f32 rz;
    s32 field_34;
} EffRippleWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_RippleN64_802fbe60[];
extern f32 float_0_80425f20;
extern f32 float_1_80425f2c;
extern f32 float_0p6_80425f3c;
extern f32 float_10_80425f40;

void effRippleN64SetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x2C) = rx;
    *(f32*)((s32)work + 0x30) = rz;
}


void effRippleDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void effGetTexObj(s32,void*);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void PSMTXScale(void*,f32,f32,f32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetTevColor(s32,void*);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_0p0625_80425f1c,float_0_80425f20,float_0p03125_80425f24,float_deg2rad_80425f28,float_1_80425f2c;
    u8 texObj[0x20];f32 base[3][4],rot[3][4],scale[3][4],mtx[3][4];u8* work=*(u8**)((s32)effect+0xC);s32 type=*(s32*)work;u32 color;
    if(type==10||type==20||(type>=1&&type<=3)||type==30)effGetTexObj(0x5B,texObj);else effGetTexObjN64(0x21,texObj);GXLoadTexObj(texObj,0);effGetTexObjN64(0x22,texObj);GXLoadTexObj(texObj,1);GXSetNumChans(0);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);PSMTXScale(scale,float_0p0625_80425f1c,float_0p0625_80425f1c,float_0_80425f20);GXLoadTexMtxImm(scale,0x1E,1);GXLoadTexMtxImm(scale,0x21,1);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,2);GXSetTevAlphaIn(0,0,7,1,7);color=*(u32*)(work+0x18);GXSetTevColor(1,&color);PSMTXScale(scale,float_0p03125_80425f24,float_0p03125_80425f24,float_0_80425f20);PSMTXTrans(rot,float_0_80425f20,(f32)*(s32*)(work+0x24),float_0_80425f20);PSMTXConcat(scale,rot,scale);GXLoadTexMtxImm(scale,0x21,1);PSMTXTrans(mtx,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x78,float_deg2rad_80425f28**(f32*)(work+0x2C));PSMTXConcat(mtx,rot,mtx);PSMTXRotRad(rot,0x7A,float_deg2rad_80425f28**(f32*)(work+0x30));PSMTXConcat(mtx,rot,mtx);PSMTXScale(scale,*(f32*)(work+0x14)**(f32*)(work+0x10),float_1_80425f2c,*(f32*)(work+0x14)**(f32*)(work+0x10));PSMTXConcat(mtx,scale,mtx);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A8008);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effRippleMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRippleDisp(void);
    extern void* effRippleN64Entry(s32 type, f32 x, f32 y, f32 z, f32 height);
    extern const Vec3 vec3_802fbe48;
    extern f32 float_0_80425f20;
    extern f32 float_0p3_80425f30;
    extern f32 float_1p05_80425f34;
    extern f32 float_0p1_80425f38;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;
    s32 type;
    void* child;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbe48;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    timer = *(s32*)(work + 0x1C);
    type = *(s32*)work;
    *(s32*)(work + 0x1C) = timer - 1;
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (timer > 5) {
        *(s32*)(work + 0x18) = (s32)((f32)(120 - *(s32*)(work + 0x18)) * float_0p3_80425f30 + (f32)*(s32*)(work + 0x18));
    }
    if (((u32)(type - 1) <= 1 || type == 3) && timer == 15) {
        child = effRippleN64Entry(type * 10, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC), float_0_80425f20);
        *(f32*)((s32)*(void**)((s32)child + 0xC) + 0x2C) = *(f32*)(work + 0x2C);
        *(f32*)((s32)*(void**)((s32)child + 0xC) + 0x30) = *(f32*)(work + 0x30);
    }
    if (type == 2 || type == 20) {
        *(f32*)(work + 0x10) *= float_1p05_80425f34;
    }
    if (type == 3 || type == 30) {
        *(f32*)(work + 0x10) *= float_1p05_80425f34;
    }
    if (timer < 0x20) {
        *(s32*)(work + 0x18) = timer << 3;
    }
    *(s32*)(work + 0x24) += 1;
    if (*(s32*)(work + 0x24) >= 0x20) {
        *(s32*)(work + 0x24) -= 0x20;
    }
    *(f32*)(work + 0x10) += float_0p1_80425f38;
    dispEntry(*(s32*)(work + 0x34), 2, effRippleDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effRippleN64Entry(f32 x, f32 y, f32 z, f32 height, s32 type) {
    void* entry;
    EffRippleWork* work;
    f32 one;
    f32 zero;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_RippleN64_802fbe60;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x38);
    *(EffRippleWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRippleMain;

    zeroInt = 0;
    zero = float_0_80425f20;
    one = float_1_80425f2c;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->frame = zeroInt;
    work->field_20 = zeroInt;
    work->rx = zero;
    work->rz = zero;
    work->scale = one;
    work->field_34 = 4;

    switch ((u32)type) {
        case 1:
        case 10:
            work->radius = float_0p6_80425f3c;
            break;
        case 2:
        case 20:
            work->radius = float_10_80425f40;
            break;
        case 3:
        case 30:
            work->radius = float_10_80425f40;
            break;
        default:
            work->radius = float_1_80425f2c;
            break;
    }

    work->lifetime = 0x20;
    work->randAngle = rand() % 32;
    work->y2 = y + height;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
