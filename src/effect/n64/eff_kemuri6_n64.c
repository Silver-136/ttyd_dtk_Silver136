#include "effect/n64/eff_kemuri6_n64.h"

void* effKemuri6N64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri6Main(void*);
    extern s32 rand(void);
    extern char str_Kemuri6N64_802fb26c[];
    extern f32 float_0_80425728;
    extern f32 float_1_80425748;
    extern f32 float_1p4_8042574c;
    extern f32 float_0p03_80425750;
    extern f32 float_1p5_80425754;
    extern f32 float_2_80425758;
    extern f32 float_neg0p5_8042575c;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri6N64_802fb26c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x50);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri6Main;

    *(s32*)work = 1;
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x40) = float_0_80425728;
    *(f32*)(work + 0x44) = float_0_80425728;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x1C) = float_1_80425748;
    *(f32*)(work + 0x20) = float_1_80425748;
    *(f32*)(work + 0x24) = float_1_80425748;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x28) = float_1p4_8042574c + float_0p03_80425750 * (f32)(rand() % 11);
    *(f32*)(work + 0x2C) = float_1p5_80425754 + float_0p03_80425750 * (f32)(rand() % 11);
    *(f32*)(work + 0x30) = (f32)(rand() % 61);
    *(s16*)(work + 4) = (s16)type;
    *(f32*)(work + 0x38) = float_2_80425758;
    *(f32*)(work + 0x3C) = float_neg0p5_8042575c;
    *(s16*)(work + 6) = 0xF;
    *(f32*)(work + 0x18) = (f32)(rand() % 0x169);
    *(u8*)(work + 8) = 0xFF;

    return entry;
}

void effKemuri6Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri6Disp(s32, s32);
    extern void* marioGetPtr(void);
    extern Vec3 vec3_802fb260;
    extern f32 float_12_80425730;
    extern f32 float_6p2832_80425734;
    extern f32 float_360_80425738;
    extern f32 float_0p1_8042573c;
    extern f32 float_0p14_80425740;
    extern f32 float_0p94_80425744;
    extern f32 float_0_80425728;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 alive;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb260;
    pos.x = *(f32*)(work + 0xC);
    pos.y = *(f32*)(work + 0x10);
    pos.z = *(f32*)(work + 0x14);
    dispPos = pos;
    alive = 0;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        if (*(s32*)work != 0) {
            *(s16*)(work + 6) -= 1;
            if (*(s16*)(work + 6) <= 0) {
                *(s32*)work = 0;
            } else {
                alive = 1;
                *(f32*)(work + 0x30) = reviseAngle(float_12_80425730 + *(f32*)(work + 0x30));
                *(f32*)(work + 0x1C) = *(f32*)(work + 0x28) +
                    float_0p1_8042573c *
                    (f32)sin(float_6p2832_80425734 * *(f32*)(work + 0x30) / float_360_80425738);
                *(f32*)(work + 0x20) = *(f32*)(work + 0x2C) +
                    float_0p1_8042573c *
                    (f32)cos(float_6p2832_80425734 * *(f32*)(work + 0x30) / float_360_80425738);
                if (*(u16*)(work + 4) == 0 && *(s16*)(work + 6) > 4 && *(s16*)(work + 6) <= 5) {
                    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0) {
                        *(s16*)(work + 6) += 1;
                        return;
                    }
                }
                *(f32*)(work + 0x3C) += *(f32*)(work + 0x40);
                *(f32*)(work + 0x38) += *(f32*)(work + 0x3C);
                *(f32*)(work + 0x10) += *(f32*)(work + 0x38);
                *(u8*)(work + 8) -= 0xF;
                *(f32*)(work + 0x28) += float_0p14_80425740;
                *(f32*)(work + 0x2C) *= float_0p94_80425744;
                if (*(f32*)(work + 0x38) < float_0_80425728) {
                    *(f32*)(work + 0x3C) = float_0_80425728;
                    *(f32*)(work + 0x38) = float_0_80425728;
                }
            }
        }
        work += 0x50;
    }

    if (alive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri6Disp, effect, dispCalcZ(&dispPos));
    }
}


void effKemuri6Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(s32,void*); extern void PSMTXTrans(Mtx,f64,f64,f64);
    extern void PSMTXRotRad(Mtx,f32,char); extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32); extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_0p0625_80425724; extern f32 float_0_80425728; extern f32 float_deg2rad_8042572c;
    u8 tex[0x20]; Mtx texMtx,trans,rotate; u8* entry=(u8*)effect; u8* work=*(u8**)(entry+0xC);
    u8* camera=(u8*)camGetPtr(cameraId); s32 i;
    GXSetNumChans(0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(texMtx,float_0p0625_80425724,float_0p0625_80425724,float_0_80425728); GXLoadTexMtxImm(texMtx,0x1E,1);
    GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,8,15,15,2); GXSetTevAlphaIn(0,7,4,5,7); effGetTexObjN64(0x4E,tex); GXLoadTexObj(tex,0);
    GXSetCullMode(0); effSetVtxDescN64((void*)0x803A4D58);
    for(i=0;i<*(s32*)(entry+8);i++,work+=0x50){if(*(s32*)work!=0){u8* camera3d;u32 color=0xFFFFFF00|*(u8*)(work+8);GXSetTevColor(1,&color);
        PSMTXTrans(trans,*(f32*)(work+0xC),*(f32*)(work+0x10),*(f32*)(work+0x14));camera3d=(u8*)camGetPtr(4);
        PSMTXRotRad(rotate,float_deg2rad_8042572c*-*(f32*)(camera3d+0x114),'y');PSMTXConcat(trans,rotate,trans);
        PSMTXScale(texMtx,*(f32*)(work+0x1C),*(f32*)(work+0x20),*(f32*)(work+0x24));PSMTXConcat(trans,texMtx,trans);
        PSMTXRotRad(rotate,float_deg2rad_8042572c**(f32*)(work+0x18),'z');PSMTXConcat(trans,rotate,trans);
        PSMTXConcat((f32 (*)[4])(camera+0x11C),trans,trans);GXLoadPosMtxImm(trans,0);GXBegin(0x90,0,6);tri2(0,3,1,0,0,2,3);}}
}

