#include "effect/n64/eff_kemuri8_n64.h"

void effKemuri8N64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x58) = camId;
}

void* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri8Main(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri8N64_802fb2b0[];
    extern f32 float_14_804257c8;
    extern f32 float_28_804257cc;
    extern f32 float_42_804257d0;
    extern f32 float_neg3_804257d4;
    extern f32 float_0p2_804257d8;
    extern f32 float_0_804257a8;
    extern f32 float_1p75_804257dc;
    extern f32 float_neg0p16_804257e0;
    extern f32 float_6p2832_804257e4;
    extern f32 float_360_804257e8;
    void* entry;
    u8* work;
    u8* part;
    s32 variant;
    s32 count;
    s32 i;
    s32 angleDeg;
    f32 radius;
    f32 rad;

    entry = effEntry();
    variant = 0;
    if (type >= 3) {
        variant = 1;
        type -= 3;
    }
    switch (type) {
        case 0:
            radius = float_14_804257c8;
            count = 7;
            break;
        case 1:
            radius = float_28_804257cc;
            count = 9;
            break;
        default:
            count = 11;
            radius = float_42_804257d0;
            break;
    }

    *(char**)((s32)entry + 0x14) = str_Kemuri8N64_802fb2b0;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x5C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri8Main;
    *(s32*)(work + 0x38) = 0;
    *(s32*)(work + 0x3C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x40) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x4C) = 0x11;
    *(s32*)(work + 0x58) = 4;

    part = work + 0x5C;
    for (i = 1; i < count; i++, part += 0x5C) {
        *(f32*)(part + 0x10) = float_neg3_804257d4;
        *(f32*)(part + 0x14) = float_0p2_804257d8;
        *(f32*)(part + 0x18) = float_0_804257a8;
        *(f32*)(part + 0x1C) = float_0_804257a8;
        *(f32*)(part + 0x20) = float_1p75_804257dc;
        *(f32*)(part + 0x24) = float_neg0p16_804257e0;
        *(f32*)(part + 0x28) = float_0_804257a8;
        angleDeg = ((i - 1) * 0x168) / (count - 1);
        rad = (float_6p2832_804257e4 * (f32)angleDeg) / float_360_804257e8;
        *(f32*)(part + 0x2C) = float_0_804257a8;
        *(f32*)(part + 4) = radius * -(f32)sin(rad);
        *(f32*)(part + 8) = radius * -(f32)cos(rad);
        *(f32*)(part + 0xC) = float_0_804257a8;
        *(f32*)(part + 0x30) = (f32)sin(rad);
        *(f32*)(part + 0x34) = (f32)cos(rad);
        *(s32*)(part + 0x50) = variant;
        if (variant == 0) {
            *(u8*)(part + 0x44) = 0xEE;
            *(u8*)(part + 0x45) = 0xDE;
            *(u8*)(part + 0x46) = 0xD7;
            *(u8*)(part + 0x47) = 0xA0;
        } else {
            *(u8*)(part + 0x44) = 0xFF;
            *(u8*)(part + 0x45) = 0xFF;
            *(u8*)(part + 0x46) = 0xFF;
            *(u8*)(part + 0x47) = 0xA0;
            *(s32*)(part + 0x54) = 0;
        }
    }
    return entry;
}

void effKemuri8Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri8Disp(void);
    extern const Vec3 vec3_802fb298;
    extern f32 float_16_804257c4;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;
    s32 life;
    s32 nextFrame;
    s32 fadeIn;
    s32 fadeOut;
    s32 camera;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb298;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    frame = *(s32*)(work + 0x3C);
    life = *(s32*)(work + 0x4C);
    nextFrame = frame + 1;
    camera = *(s32*)(work + 0x58);
    fadeOut = (s32)((float_16_804257c4 * (f32)frame) / (f32)life);
    fadeIn = (s32)((float_16_804257c4 * (f32)nextFrame) / (f32)life);
    *(s32*)(work + 0x3C) = nextFrame;
    if (fadeOut > 0xF) {
        fadeOut = 0xF;
    }
    if (fadeIn > 0xF) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x40) = fadeIn;
    *(s32*)(work + 0x48) = fadeOut;

    part = work + 0x5C;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x5C) {
        *(f32*)(part + 0x18) += *(f32*)(part + 0x1C);
        *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
        *(f32*)(part + 4) += *(f32*)(part + 0x10) * *(f32*)(part + 0x30);
        *(f32*)(part + 8) += *(f32*)(part + 0x10) * *(f32*)(part + 0x34);
    }

    dispEntry(camera, 2, effKemuri8Disp, effect, dispCalcZ(&dispPos));
}


void effKemuri8Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void effGetTexObj(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetChanMatColor(s32,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXBegin(s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC); u8* p=w+0x5C; void* cam=camGetPtr(cameraId); Mtx base,m,r,s; u8 tex[0x20]; u32 color; s32 i,j,type=*(s16*)w; f32 size,u0=0.0625f*(f32)*(s32*)(w+0x40),u1=0.0625f*(f32)*(s32*)(w+0x48);
    PSMTXTrans(m,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC)); PSMTXRotRad(r,0x79,-*(f32*)((u8*)cam+0x114)*0.0174533f); PSMTXScale(s,1.0f,1.0f,1.0f); PSMTXConcat(m,r,m); PSMTXConcat(m,s,m); PSMTXConcat((u8*)cam+0x11C,m,base);
    effGetTexObj(0x28,tex); GXLoadTexObj(tex,0); GXSetNumChans(1); GXSetChanCtrl(4,0,0,0,0,0,2); GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D); GXSetNumTevStages(3); GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,0,0,0,8); GXSetTevAlphaIn(0,0,0,0,4); GXSetTevOrder(1,1,0,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,0,8,2,15); GXSetTevAlphaIn(1,0,4,1,7); color=0xFFFFFFFF; GXSetTevColor(1,&color); GXSetTevOrder(2,0xFF,0xFF,4); GXSetCullMode(0); GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxDesc(0xD,1); GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,0xD,1,4,0);
    for(i=1;i<*(s32*)((u8*)effect+8);i++,p+=0x5C){if(*(s32*)(p+0x40)>=16)continue;PSMTXTrans(m,u0,0.0f,0.0f);PSMTXScale(s,0.0625f,1.0f,1.0f);PSMTXConcat(m,s,m);GXLoadTexMtxImm(m,0x1E,1);PSMTXTrans(m,u1,0.0f,0.0f);PSMTXConcat(m,s,m);GXLoadTexMtxImm(m,0x21,1);size=type?32.0f:24.0f;for(j=0;j<2;j++){if(j==0){color=0xFFFFFF60;GXSetChanMatColor(4,&color);PSMTXTrans(m,*(f32*)(p+4)+2.0f,*(f32*)(p+8)-2.0f,*(f32*)(p+0xC));}else{color=*(u32*)(p+0x44);GXSetChanMatColor(4,&color);PSMTXTrans(m,*(f32*)(p+4),*(f32*)(p+8),*(f32*)(p+0xC));}PSMTXConcat(base,m,m);GXLoadPosMtxImm(m,0);GXBegin(0x80,0,4);size*=0.5f;}}
}

