#include "effect/n64/eff_kemuri7_n64.h"

typedef f32 Mtx[3][4];

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri7Work {
    s32 active;
    u8 pad_04[2];
    s16 timer;
    u8 alpha;
    u8 pad_09[3];
    f32 x;
    f32 y;
    f32 z;
    u8 pad_18[0x10];
    f32 angle;
    u8 pad_2c[4];
    Mtx matrix;
    u8 pad_60[0x28];
} EffKemuri7Work;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s32 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
void effKemuri7Disp(s32 cameraId, void* effect);

extern const Vec3 vec3_802fb280;
extern f32 float_deg2rad_80425774;

void* effKemuri7N64Entry(f32 x, f32 y, f32 z, f32 angle, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri7Main(void*);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri7N64_802fb28c[];
    extern f32 float_0_80425768;
    extern f32 float_1_80425770;
    extern f32 float_30_80425778;
    extern f32 float_15_8042577c;
    extern f32 float_180_80425780;
    extern f32 float_6p2832_80425784;
    extern f32 float_360_80425788;
    extern f32 float_45_8042578c;
    extern f32 float_5_80425790;
    void* entry;
    u8* work;
    f32 revised;
    f32 rad;
    f32 adjusted;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri7N64_802fb28c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x88);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri7Main;
    *(s32*)work = 1;
    *(f32*)(work + 0x6C) = float_0_80425768;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x18) = float_1_80425770;
    *(f32*)(work + 0x1C) = float_1_80425770;
    *(f32*)(work + 0x20) = float_1_80425770;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x60) = scale;
    *(f32*)(work + 0x70) = float_0_80425768;
    *(f32*)(work + 0x74) = float_0_80425768;
    *(s16*)(work + 6) = 0xFF;
    *(f32*)(work + 0x24) = float_0_80425768;
    *(f32*)(work + 0x28) = angle + (float_30_80425778 * scale) - float_15_8042577c;
    *(f32*)(work + 0x2C) = float_0_80425768;

    revised = reviseAngle(angle);
    rad = (float_6p2832_80425784 * revised) / float_360_80425788;
    *(f32*)(work + 0x80) = (f32)sin(rad);
    *(f32*)(work + 0x84) = (f32)cos(rad);

    adjusted = float_45_8042578c + revised - (float_180_80425780 * scale);
    rad = (float_6p2832_80425784 * adjusted) / float_360_80425788;
    *(f32*)(work + 0xC) += float_5_80425790 * (f32)sin(rad);
    *(f32*)(work + 0x14) += float_5_80425790 * (f32)cos(rad);
    *(u8*)(work + 8) = 0xFF;
    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effKemuri7Main(void* effEntry) {
    Mtx rot;
    Mtx trans;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri7Work* work;
    register s32 i;
    register s32 anyAlive;
    s32 timer;

    anyAlive = 0;
    i = 0;
    work = *(EffKemuri7Work**)((s32)effEntry + 0xC);
    zero = &vec3_802fb280;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    while (i < *(s32*)((s32)effEntry + 0x8)) {
        if (work->active != 0) {
            timer = work->timer - 1;
            work->timer = timer;
            if ((s16)timer <= 0) {
                work->active = 0;
            } else {
                anyAlive = 1;
                PSMTXTrans(trans, work->x, work->y, work->z);
                PSMTXRotRad(rot, 0x79, float_deg2rad_80425774 * work->angle);
                PSMTXConcat(trans, rot, work->matrix);
                work->alpha--;
            }
        }
        i++;
        work++;
    }

    if (anyAlive == 0) {
        effDelete(effEntry);
    } else {
        dispEntry(4, 2, effKemuri7Disp, effEntry, dispCalcZ(&dispPos));
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effKemuri7Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*); extern void GXSetTevColor(s32,void*); extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_0p03125_80425764; extern f32 float_0_80425768; extern f32 float_neg1_8042576c; extern f32 float_1_80425770;
    u8 tex[0x20]; Mtx texMtx,draw; u8* entry=(u8*)effect;u8* work=*(u8**)(entry+0xC);u8* camera=(u8*)camGetPtr(cameraId);s32 i;
    GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_0p03125_80425764,float_0p03125_80425764,float_0_80425768);GXLoadTexMtxImm(texMtx,0x1E,1);
    GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,15,15,15,15);GXSetTevAlphaIn(0,7,4,5,7);effGetTexObjN64(0x4F,tex);GXLoadTexObj(tex,0);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A4D90);
    for(i=0;i<*(s32*)(entry+8);i++,work+=0x88){if(*(s32*)work!=0){u32 color=0xFFFFFF00|*(u8*)(work+8);GXSetTevColor(1,&color);
        PSMTXConcat((f32 (*)[4])(camera+0x11C),(f32 (*)[4])(work+0x30),draw);if(*(f32*)(work+0x60)==float_0_80425768){PSMTXScale(texMtx,float_neg1_8042576c,float_1_80425770,float_1_80425770);PSMTXConcat(draw,texMtx,draw);}
        GXLoadPosMtxImm(draw,0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3);}}
}
