#include "effect/n64/eff_rothammer_fail_n64.h"

typedef struct EffRotHammerFailParticle {
    s32 field_00;
    f32 x;
    f32 y;
    f32 z;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
} EffRotHammerFailParticle;

typedef struct EffRotHammerFailWork {
    s16 type;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    f32 field_1C;
    f32 field_20;
    s32 lifetime;
    s32 frame;
    s32 lifetime2;
    f32 alpha;
    EffRotHammerFailParticle particle;
} EffRotHammerFailWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
double sin(double x);
double cos(double x);
void effRotHammerFailMain(void* effect);
void effRotHammerFailDisp(s32 cameraId, void* effect);

extern const char str_RotHammerFailN64_802fbe88[];
extern f32 float_0_80425f5c;
extern f32 float_10_80425f74;
extern f32 float_30_80425f78;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effRotHammerFailN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    void* entry;
    EffRotHammerFailWork* work;
    EffRotHammerFailParticle* particle;
    f32 zero;
    f32 ten;
    f32 thirty;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_RotHammerFailN64_802fbe88;
    *(s32*)((s32)entry + 0x8) = 2;
    work = __memAlloc(3, 0x68);
    *(EffRotHammerFailWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRotHammerFailMain;

    zero = float_0_80425f5c;
    zeroInt = 0;
    ten = float_10_80425f74;
    work->lifetime = lifetime;
    particle = &work->particle;
    thirty = float_30_80425f78;
    work->frame = zeroInt;
    work->lifetime2 = lifetime;
    work->alpha = zero;
    work->type = (s16)type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->field_1C = ten;
    work->field_20 = thirty;
    particle->x = zero;
    particle->y = zero;
    particle->z = zero;
    particle->dirX = sin(zero);
    particle->dirY = zero;
    particle->dirZ = cos(zero);
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effRotHammerFailMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRotHammerFailDisp(void);
    extern const Vec3 vec3_802fbe70;
    extern f32 float_7_80425f64;
    extern f32 float_0p1_80425f68;
    extern f32 float_0p04_80425f6c;
    extern f32 float_0p9_80425f70;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    f32 scale;
    f32 accel;
    f32 drag;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbe70;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(f32*)(work + 0x30) = (float_7_80425f64 * (f32)*(s32*)(work + 0x28)) / (f32)*(s32*)(work + 0x2C);
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return;
    }
    *(f32*)(work + 0x1C) += float_0p1_80425f68 * (*(f32*)(work + 0x20) - *(f32*)(work + 0x1C));
    scale = *(f32*)(work + 0x1C);
    accel = float_0p04_80425f6c;
    drag = float_0p9_80425f70;
    i = 1;
    part = work + 0x34;
    for (; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        *(f32*)(part + 0x14) += accel;
        *(f32*)(part + 0x14) *= drag;
        *(f32*)(part + 4) = scale * *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) = scale * *(f32*)(part + 0x18);
    }
    dispEntry(4, 2, effRotHammerFailDisp, effect, dispCalcZ(&dispPos));
}

void effRotHammerFailDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void PSMTXScale(void*,f32,f32,f32);extern void GXSetTevColor(s32,void*);extern void GXLoadTexMtxImm(void*,s32,s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_256_80425f4c,float_deg2rad_80425f50,float_0p0044643_80425f54,float_0p03125_80425f58,float_0_80425f5c;
    u8 texObj[0x20];f32 trans[3][4],rot[3][4],mtx[3][4],scale[3][4];u8* work=*(u8**)((s32)effect+0xC);s32 frame=(s32)*(f32*)(work+0x30);u32 color;
    GXSetNumChans(0);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);effGetTexObjN64(0xD,texObj);GXLoadTexObj(texObj,0);GXLoadTexObj(texObj,1);GXSetNumTevStages(3);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,0);GXSetTevAlphaIn(0,0,0,0,4);GXSetTevOrder(1,1,1,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,0,0,0,0);GXSetTevAlphaIn(1,6,7,2,7);GXSetTevOrder(2,0xFF,0xFF,0xFF);GXSetTevColorOp(2,0,0,0,1,0);GXSetTevAlphaOp(2,0,0,0,1,0);GXSetTevColorIn(2,0,0,0,2);GXSetTevAlphaIn(2,0,6,1,7);GXSetCullMode(0);PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x79,float_deg2rad_80425f50*-*(f32*)((s32)camGetPtr(4)+0x114));PSMTXConcat(trans,rot,trans);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),trans,mtx);color=*(u32*)(work+0x2C);GXSetTevColor(1,&color);color=(u32)(float_256_80425f4c*(*(f32*)(work+0x30)-(f32)frame));GXSetTevColor(2,&color);PSMTXScale(scale,float_0p0044643_80425f54,float_0p03125_80425f58,float_0_80425f5c);GXLoadTexMtxImm(scale,0x1E,1);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);effSetVtxDescN64((void*)0x803A80C0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
}
