#include "effect/n64/eff_snow_n64.h"

typedef struct EffSnowWork {
    f32 x;
    f32 y;
    f32 z;
    f32 field_0C;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    s32 type;
} EffSnowWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SnowN64_802fbf84[];
extern f32 float_0_80426060;
extern f32 float_neg1p5_8042607c;
extern f32 float_1p5_80426080;
extern f32 float_neg5_80426084;
extern f32 float_5_80426088;

void effSnowDisp(s32 cameraId, void* effect){extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void GXClearVtxDesc(void);extern void GXSetVtxDesc(s32,s32);extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXBegin(s32,s32,s32);u8* w=*(u8**)((u8*)effect+0xC);u8* cam=camGetPtr(cameraId);f32 a[3][4],b[3][4],c[3][4];u8 tex[0x20];PSMTXTrans(a,*(f32*)w,*(f32*)(w+4),*(f32*)(w+8));PSMTXRotRad(b,0x79,-0.017453292f**(f32*)((u8*)camGetPtr(cameraId)+0x114));PSMTXConcat(a,b,c);PSMTXConcat(cam+0x11C,c,c);PSMTXRotRad(b,0x78,0.017453292f**(f32*)(w+0x18));PSMTXConcat(c,b,c);PSMTXRotRad(b,0x79,0.017453292f**(f32*)(w+0x1C));PSMTXConcat(c,b,c);GXLoadPosMtxImm(c,0);GXSetCurrentMtx(0);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,-1);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,14);GXSetTevAlphaIn(0,7,7,7,4);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);effGetTexObjN64(7,tex);GXLoadTexObj(tex,0);GXBegin(0x80,0,4);}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSnowMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effSnowDisp(void);
    extern void* marioGetPtr(void);
    extern const Vec3 vec3_802fbf78;
    extern f32 float_10_8042606c;
    extern f32 float_45_80426070;
    extern f32 float_200_80426074;
    extern f32 float_400_80426078;
    u8* work;
    u8* mario;
    Vec3 dispPos;
    Vec3 pos;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbf78;
    pos.x = *(f32*)(work + 0);
    pos.y = *(f32*)(work + 4);
    pos.z = *(f32*)(work + 8);
    dispPos = pos;
    *(s32*)(work + 0x28) -= 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    *(f32*)(work + 0) += *(f32*)(work + 0xC);
    *(f32*)(work + 4) += *(f32*)(work + 0x10);
    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(f32*)(work + 0x18) += *(f32*)(work + 0x20);
    if (*(f32*)(work + 0x18) < float_10_8042606c) {
        *(f32*)(work + 0x18) = float_10_8042606c;
        *(f32*)(work + 0x20) = -*(f32*)(work + 0x20);
    }
    if (*(f32*)(work + 0x18) > float_45_80426070) {
        *(f32*)(work + 0x18) = float_45_80426070;
        *(f32*)(work + 0x20) = -*(f32*)(work + 0x20);
    }
    *(f32*)(work + 0x1C) += *(f32*)(work + 0x24);
    mario = marioGetPtr();
    if (*(f32*)work - *(f32*)(mario + 0x8C) > float_200_80426074) {
        *(f32*)work -= float_400_80426078;
    } else if (*(f32*)(mario + 0x8C) - *(f32*)work > float_200_80426074) {
        *(f32*)work += float_400_80426078;
    }
    if (*(f32*)(work + 8) - *(f32*)(mario + 0x94) > float_200_80426074) {
        *(f32*)(work + 8) -= float_400_80426078;
    } else if (*(f32*)(mario + 0x94) - *(f32*)(work + 8) > float_200_80426074) {
        *(f32*)(work + 8) += float_400_80426078;
    }
    dispEntry(4, 2, effSnowDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSnowN64Entry(s32 type, f32 x, f32 y, f32 z) {
    static s32 kaiten;
    void* entry;
    EffSnowWork* work;
    f32 onePointFive;
    f32 negOnePointFive;
    f32 zero;
    f32 sideSpeed;
    s32 next;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SnowN64_802fbf84;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffSnowWork**)((s32)entry + 0xC) = work;

    zero = float_0_80426060;
    *(void**)((s32)entry + 0x10) = effSnowMain;
    negOnePointFive = float_neg1p5_8042607c;
    work->x = x;
    onePointFive = float_1p5_80426080;
    work->y = y;
    work->z = z;
    work->field_0C = zero;
    work->field_10 = negOnePointFive;
    work->field_14 = zero;
    work->field_18 = zero;
    work->field_1C = zero;
    work->field_20 = onePointFive;
    if (kaiten != 0) {
        sideSpeed = float_neg5_80426084;
    } else {
        sideSpeed = float_5_80426088;
    }
    work->field_24 = sideSpeed;
    work->type = type;
    next = kaiten + 1;
    kaiten = next;
    if (next > 1) {
        kaiten = 0;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
