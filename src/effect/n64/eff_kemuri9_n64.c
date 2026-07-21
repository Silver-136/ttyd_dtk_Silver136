#include "effect/n64/eff_kemuri9_n64.h"

void* effKemuri9N64Entry(s32 type, f32 x, f32 y, f32 z, s32 count, s32 life, f32 targetRadius, f32 angleOffset) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri9Main(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri9N64_802fb2d8[];
    extern f32 float_10_8042581c;
    extern f32 float_0_80425800;
    extern f32 float_6p2832_80425820;
    extern f32 float_360_80425824;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 angleDeg;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri9N64_802fb2d8;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x40);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri9Main;
    *(s32*)(work + 0x24) = life;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x2C) = life;
    *(s32*)(work + 0x30) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = float_10_8042581c;
    *(f32*)(work + 0x20) = targetRadius;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x3C) = life;
    *(s32*)(work + 0x38) = 0;

    part = work + 0x40;
    for (i = 1; i < count + 1; i++, part += 0x40) {
        *(f32*)(part + 4) = float_0_80425800;
        *(f32*)(part + 8) = float_0_80425800;
        angleDeg = ((i - 1) * 0x168) / count;
        *(f32*)(part + 0xC) = float_0_80425800;
        rad = (float_6p2832_80425820 * (angleOffset + (f32)angleDeg)) / float_360_80425824;
        *(f32*)(part + 0x10) = (f32)sin(rad);
        *(f32*)(part + 0x14) = float_0_80425800;
        *(f32*)(part + 0x18) = (f32)cos(rad);
    }
    return entry;
}

void effKemuri9Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri9Disp(void);
    extern const Vec3 vec3_802fb2c0;
    extern f32 float_16_80425804;
    extern f32 float_0p1_80425810;
    extern f32 float_0p04_80425814;
    extern f32 float_0p9_80425818;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;
    s32 life;
    s32 nextFrame;
    s32 fadeIn;
    s32 fadeOut;
    s32 countInt;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb2c0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    frame = *(s32*)(work + 0x38);
    life = *(s32*)(work + 0x3C);
    nextFrame = frame + 1;
    fadeOut = (s32)((float_16_80425804 * (f32)frame) / (f32)life);
    fadeIn = (s32)((float_16_80425804 * (f32)nextFrame) / (f32)life);
    *(s32*)(work + 0x38) = nextFrame;
    if (fadeOut > 0xF) {
        fadeOut = 0xF;
    }
    if (fadeIn > 0xF) {
        fadeIn = 0xF;
    }
    *(s32*)(work + 0x30) = fadeIn;
    *(s32*)(work + 0x34) = fadeOut;
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return;
    }

    *(f32*)(work + 0x1C) += float_0p1_80425810 * (*(f32*)(work + 0x20) - *(f32*)(work + 0x1C));
    countInt = (s32)*(f32*)(work + 0x1C);
    part = work + 0x40;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x40) {
        *(f32*)(part + 0x14) += float_0p04_80425814;
        *(f32*)(part + 0x14) *= float_0p9_80425818;
        *(f32*)(part + 4) = (f32)countInt * *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) = (f32)countInt * *(f32*)(part + 0x18);
    }

    dispEntry(4, 2, effKemuri9Disp, effect, dispCalcZ(&dispPos));
}


void effKemuri9Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXRotRad(Mtx,f32,char);
    extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXConcat(Mtx,Mtx,Mtx); extern void effGetTexObj(s32,void*);
    extern void GXLoadTexObj(void*,s32); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetChanMatColor(s32,void*); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXLoadPosMtxImm(Mtx,s32); extern void GXSetCurrentMtx(s32); extern void GXBegin(s32,s32,s32);
    extern f32 float_deg2rad_804257f4; extern f32 float_1_804257f8; extern f32 float_0p0625_804257fc; extern f32 float_0_80425800;
    extern f32 float_16_80425804; extern f32 float_32_80425808; extern f32 float_0p5_8042580c;
    volatile f32* fifo=(volatile f32*)0xCC008000;u8 tex[0x20];Mtx base,rotate,scale,model;u8* entry=(u8*)effect;u8* work=*(u8**)(entry+0xC);
    u8* camera=(u8*)camGetPtr(cameraId);s32 type=*(s16*)work;s32 frame0=*(s32*)(work+0x30);s32 frame1=*(s32*)(work+0x34);s32 i;u32 white=0xFFFFFFFF;
    PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rotate,float_deg2rad_804257f4*-*(f32*)(camera+0x114),'y');PSMTXScale(scale,float_1_804257f8,float_1_804257f8,float_1_804257f8);
    PSMTXConcat(base,rotate,base);PSMTXConcat(base,scale,base);PSMTXConcat((f32 (*)[4])(camera+0x11C),base,model);effGetTexObj(0x28,tex);GXLoadTexObj(tex,0);
    GXSetNumChans(1);GXSetChanCtrl(4,0,0,0,0,0,2);GXSetChanMatColor(4,&white);GXSetNumTexGens(2);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    PSMTXTrans(base,float_0p0625_804257fc*(f32)frame0,float_0_80425800,float_0_80425800);PSMTXScale(scale,float_0p0625_804257fc,float_1_804257f8,float_1_804257f8);PSMTXConcat(base,scale,base);GXLoadTexMtxImm(base,0x1E,1);
    PSMTXTrans(base,float_0p0625_804257fc*(f32)frame1,float_0_80425800,float_0_80425800);PSMTXConcat(base,scale,base);GXLoadTexMtxImm(base,0x21,1);
    GXSetNumTevStages(3);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,8);GXSetTevAlphaIn(0,7,7,7,4);
    GXSetTevOrder(1,1,0,0xFF);GXSetTevColorOp(1,0,0,0,1,0);GXSetTevAlphaOp(1,0,0,0,1,0);GXSetTevColorIn(1,0,8,2,15);GXSetTevAlphaIn(1,0,4,5,7);
    GXSetTevOrder(2,0xFF,0xFF,4);GXSetTevColorOp(2,0,0,0,1,0);GXSetTevAlphaOp(2,0,0,0,1,0);GXSetTevColorIn(2,15,0,10,15);GXSetTevAlphaIn(2,7,0,1,7);
    GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(0xD,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,0xD,1,4,0);
    work+=0x40;for(i=1;i<*(s32*)(entry+8);i++,work+=0x40){f32 size=type?float_16_80425804:float_32_80425808;f32 half=size*float_0p5_8042580c;
        PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXConcat(model,base,base);GXLoadPosMtxImm(base,0);GXSetCurrentMtx(0);GXBegin(0x80,0,4);
        *fifo=-half;*fifo=half;*fifo=float_0_80425800;*fifo=float_0_80425800;*fifo=float_0_80425800;
        *fifo=half;*fifo=half;*fifo=float_0_80425800;*fifo=float_1_804257f8;*fifo=float_0_80425800;
        *fifo=half;*fifo=-half;*fifo=float_0_80425800;*fifo=float_1_804257f8;*fifo=float_1_804257f8;
        *fifo=-half;*fifo=-half;*fifo=float_0_80425800;*fifo=float_0_80425800;*fifo=float_1_804257f8;}
}

