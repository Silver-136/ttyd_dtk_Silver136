#include "effect/n64/eff_snowfall_n64.h"


void effSnowfallDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void PSMTXScale(void*,f32,f32,f32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetTevColor(s32,void*);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_0p03125_80426090,float_0_80426098,float_deg2rad_8042609c,float_1_804260a0;
    f32 trans[3][4],rot[3][4],scale[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);u8* part=work+0x34;s32 i;u32 color;
    GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80426090,float_0p03125_80426090,float_0_80426098);GXLoadTexMtxImm(scale,0x1E,1);effGetTexObjN64(0x2A,texObj);GXLoadTexObj(texObj,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0xF,0xF,0xF,2);GXSetTevAlphaIn(0,7,1,4,7);GXSetCullMode(0);
    for(i=0;i<*(s32*)(work+0x2C);i++,part+=0x34){if(*(s32*)(part+0x30)>0)continue;PSMTXTrans(trans,*(f32*)(part+8),*(f32*)(part+0xC),*(f32*)(part+0x10));PSMTXRotRad(rot,0x79,float_deg2rad_8042609c*-*(f32*)((s32)camGetPtr(4)+0x114));PSMTXConcat(trans,rot,trans);PSMTXScale(scale,*(f32*)(part+0x20),*(f32*)(part+0x20),float_1_804260a0);PSMTXConcat(trans,scale,trans);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),trans,trans);GXLoadPosMtxImm(trans,0);GXSetCurrentMtx(0);color=*(u32*)(work+0x28);GXSetTevColor(1,&color);effSetVtxDescN64((void*)0x803A8500);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}

void* effSnowfallN64Entry(s32 flags, s32 count) {
    extern void* effEntry(void);extern void* __memAlloc(s32,s32);extern void effSnowfallMain(void*);extern char str_SnowfallN64_802fbf9c[];extern void* camGetPtr(s32);extern s32 rand(void);extern f32 float_1000_804260a8,float_0p05_804260ac,float_10_804260b0,float_neg1p2_804260b4,float_0p01_804260b8,float_0p5_804260bc;
    void* entry;u8* work;u8* part;u8* cam;s32 i;entry=effEntry();*(char**)((s32)entry+0x14)=str_SnowfallN64_802fbf9c;*(s32*)((s32)entry+8)=count+1;work=__memAlloc(3,(count+1)*0x34);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effSnowfallMain;*(s32*)work=1;*(s32*)(work+4)=flags;*(s32*)(work+0x20)=100;*(s32*)(work+0x24)=0;*(s32*)(work+0x28)=0xFF;*(s32*)(work+0x2C)=count;cam=camGetPtr(4);part=work+0x34;
    for(i=0;i<count;i++,part+=0x34){*(f32*)(part+8)=*(f32*)(cam+0x18)+(f32)(rand()%2000)-float_1000_804260a8;*(f32*)(part+0xC)=*(f32*)(cam+0x1C)+(f32)(rand()%2000)-float_1000_804260a8;*(f32*)(part+0x10)=*(f32*)(cam+0x20)+(f32)(rand()%2000)-float_1000_804260a8;*(f32*)(part+0x14)=float_0p05_804260ac*((f32)(rand()%20)-float_10_804260b0)*float_0p5_804260bc;*(f32*)(part+0x18)=-(float_0p01_804260b8*(f32)(rand()%80)-float_neg1p2_804260b4)*float_0p5_804260bc;*(f32*)(part+0x1C)=float_0p05_804260ac*((f32)(rand()%20)-float_10_804260b0)*float_0p5_804260bc;*(s32*)(part+0x2C)=0xFF;*(s32*)(part+0x30)=0;}
    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void cam_clip_chk(u8* work, s32 xRange, s32 yRange, s32 timer) {
    extern void* camGetPtr(s32 camera);
    extern f32 float_0p5_804260bc;
    extern f32 float_0_80426098;
    u8* cam;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    s32 delta;
    s32 count;

    cam = camGetPtr(4);
    centerX = *(f32*)(cam + 0x18);
    centerY = (*(f32*)(cam + 0x1C) + *(f32*)(cam + 0x10)) * float_0p5_804260bc;
    centerZ = (*(f32*)(cam + 0x20) + *(f32*)(cam + 0x118)) * float_0p5_804260bc;

    delta = (s32)(*(f32*)(work + 8) - centerX);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX + (f32)xRange - (f32)delta;
    }

    delta = (s32)(*(f32*)(work + 0xC) - centerY);
    if (delta > yRange) {
        count = delta / yRange;
        delta -= count * yRange;
        *(f32*)(work + 0xC) = centerY - (f32)yRange + (f32)delta;
    } else if (delta < -yRange) {
        if (timer < 100) {
            *(f32*)(work + 0x18) = float_0_80426098;
            *(s32*)(work + 0x28) = 0;
        } else {
            delta = -delta;
            count = delta / yRange;
            delta -= count * yRange;
            *(f32*)(work + 0xC) = centerY + (f32)yRange - (f32)delta;
        }
    }

    delta = (s32)(*(f32*)(work + 0x10) - centerZ);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ + (f32)xRange - (f32)delta;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effSnowfallMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* camGetPtr(s32 camera);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern s32 cam_clip_chk(void* work, u32 flags, s32 value, s32 timer);
    extern void effSnowfallDisp(void);
    extern const Vec3 vec3_802fbf90;
    extern f32 float_166_804260a4;
    extern void* gp;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;
    s32 count;
    s32 value;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbf90;
    pos.x = *(f32*)(work + 8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    dispPos = pos;
    camGetPtr(4);
    if (*(s32*)(work + 0x20) < 100) {
        *(s32*)(work + 0x20) -= 1;
    }
    if (*(s32*)(work + 0x20) < 0) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x24) += 1;
    timer = *(s32*)(work + 0x20);
    count = *(s32*)(work + 0x2C);
    if (timer < 10) {
        *(s32*)(work + 0x28) = timer * 25;
    }
    value = (s32)float_166_804260a4;
    part = work + 0x34;
    for (i = 0; i < count; i++, part += 0x34) {
        if (*(s32*)(part + 0x30) > 0) {
            *(s32*)(part + 0x30) -= 1;
            if (*(s32*)(part + 0x30) > 0) {
                continue;
            }
        }
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x10) += *(f32*)(part + 0x1C);
        cam_clip_chk(part, 0x226, value, timer);
    }
    if (*(s32*)((s32)gp + 0x14) == 0) {
        dispEntry(4, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(7, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    }
}

