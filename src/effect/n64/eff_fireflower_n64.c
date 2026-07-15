#include "effect/n64/eff_fireflower_n64.h"


void effFireFlowerDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*);
    extern void GXSetTevColor(s32,void*); extern void GXSetCullMode(s32); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC); u8* p; void* cam=camGetPtr(cameraId); Mtx base,m,t,s; u8 tex[0x20]; u32 color; s32 i,state=*(s32*)(w+4),frame=*(s32*)(w+0x34); f32 flip=*(s32*)(w+0x50)?-1.0f:1.0f;
    PSMTXTrans(m,*(f32*)(w+8),*(f32*)(w+0xC),*(f32*)(w+0x10)); PSMTXRotRad(t,0x79,-*(f32*)((u8*)camGetPtr(4)+0x114)*0.0174533f); PSMTXScale(s,flip,1.0f,1.0f); PSMTXConcat(m,t,base); PSMTXConcat(base,s,base);
    color=0xFFFFFF00|(u8)*(s32*)(w+0x3C); GXSetTevColor(1,&color); color=0xFFFFFF00|(u8)*(s32*)(w+0x38); GXSetTevColor(2,&color); GXSetCullMode(0); GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2);
    if(state>3&&state<7){
        PSMTXTrans(m,0.0f,16.0f,0.0f); PSMTXConcat(base,m,t); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,4); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,0,8,10,15); GXSetTevAlphaIn(0,0,4,5,7);
        effGetTexObjN64(0x36,tex); GXLoadTexObj(tex,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(s,0.0625f,0.0625f,0.0f); GXLoadTexMtxImm(s,0x1E,1);
        p=w+0x54; for(i=1;i<*(s32*)((u8*)effect+8);i++,p+=0x54){if(*(s32*)(p+0x4C)==0){PSMTXRotRad(m,0x7A,*(f32*)(p+0x24)*0.0174533f); PSMTXTrans(s,*(f32*)(p+8),*(f32*)(p+0xC),*(f32*)(p+0x10)); PSMTXConcat(m,s,s); PSMTXConcat(t,s,s); PSMTXConcat((u8*)cam+0x11C,s,s); GXLoadPosMtxImm(s,0); GXSetCurrentMtx(0); effSetVtxDescN64(*(s32*)(p+4)?(void*)0x8039FA28:(void*)0x8039F9F0); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);}}
    }
    GXSetNumTevStages(3); GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,0,0,0,8); GXSetTevAlphaIn(0,0,0,0,4); GXSetTevOrder(1,1,1,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,0,8,6,15); GXSetTevAlphaIn(1,0,4,6,7);
    effGetTexObjN64(0x37,tex); GXLoadTexObj(tex,0); GXLoadTexObj(tex,1); GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D); PSMTXScale(s,0.03125f,0.0078125f,0.0f); PSMTXTrans(m,0.0f,32.0f*(3-frame),0.0f); PSMTXConcat(s,m,m); GXLoadTexMtxImm(m,0x1E,1); PSMTXTrans(m,*(f32*)(w+0x2C),0.0f,0.0f); PSMTXScale(s,*(f32*)(w+0x30),*(f32*)(w+0x30),*(f32*)(w+0x30)); PSMTXConcat(m,s,m); PSMTXConcat(base,m,m); PSMTXConcat((u8*)cam+0x11C,m,m); GXLoadPosMtxImm(m,0); GXSetCurrentMtx(0); effSetVtxDescN64((void*)0x8039FA60); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);
}

void effFireFlowerMain(void* effect) {
    extern void effDelete(void* effect);
    extern void effRotHammerSuccessN64Entry(f32 x, f32 y, f32 z, s32 type, s32 count, s32 timer);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFireFlowerDisp(void);
    extern s8 scale_data[];
    extern f32 float_0_80425254;
    extern f32 float_10_8042526c;
    extern f32 float_0p01_80425270;
    extern f32 float_1_80425250;
    extern f32 float_1p05_80425274;
    extern f32 float_neg10_80425278;
    extern f32 float_18_8042527c;
    extern f32 float_0p9_80425280;
    extern f32 float_720_80425284;
    extern f32 float_200_80425288;

    u8* work;
    u8* partBase;
    f32 pos[3];
    s32 state;
    s32 frame;
    s32 i;
    s32 count;
    f32 x;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 8);
    pos[1] = *(f32*)(work + 0xC);
    pos[2] = *(f32*)(work + 0x10);

    (*(s32*)(work + 0x44))++;
    (*(s32*)(work + 0x48))++;

    state = *(s32*)(work + 4);
    if (state == 7) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x44);
    switch (state) {
        case 0:
            if (*(s32*)(work + 0x50) == 0) {
                x = *(f32*)(work + 8) - float_10_8042526c;
            } else {
                x = float_10_8042526c + *(f32*)(work + 8);
            }
            effRotHammerSuccessN64Entry(x, *(f32*)(work + 0xC), *(f32*)(work + 0x10), 0, 7, 0x14);
            *(s32*)(work + 0x34) = 0;
            *(s32*)(work + 0x38) = 0;
            (*(s32*)(work + 4))++;
            *(s32*)(work + 0x44) = 0;
            break;
        case 1:
            if (frame < 0x10) {
                *(f32*)(work + 0x30) = float_0p01_80425270 * (f32)scale_data[frame - 1];
                *(s32*)(work + 0x34) = 0;
                *(s32*)(work + 0x38) = 0;
            } else {
                *(s32*)(work + 4) = state + 1;
                *(s32*)(work + 0x44) = 0;
            }
            break;
        case 2:
            if (frame < 4) {
                *(s32*)(work + 0x34) = 1;
            } else {
                *(s32*)(work + 4) = state + 1;
                *(s32*)(work + 0x44) = 0;
            }
            break;
        case 3:
            if (frame < 0x1E) {
                *(s32*)(work + 0x34) = 2;
            } else {
                *(s32*)(work + 4) = state + 1;
                *(s32*)(work + 0x44) = 0;
            }
            break;
        case 4:
            if (*(s32*)(work + 0x40) < frame) {
                *(s32*)(work + 4) = state + 1;
                *(s32*)(work + 0x44) = 0;
            } else {
                *(f32*)(work + 0x2C) = float_0_80425254;
                *(s32*)(work + 0x34) = 3;
                if (((u32)frame & 4) == 0) {
                    *(f32*)(work + 0x30) = float_1p05_80425274;
                } else {
                    *(f32*)(work + 0x30) = float_1_80425250;
                }
            }
            break;
        case 5:
            if (frame < 0x14) {
                *(s32*)(work + 0x34) = 0;
                *(f32*)(work + 0x2C) = float_neg10_80425278;
            } else {
                *(s32*)(work + 4) = state + 1;
                *(s32*)(work + 0x44) = 0;
            }
            break;
        case 6:
            *(s32*)(work + 0x34) = 0;
            *(f32*)(work + 0x28) += float_18_8042527c;
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p9_80425280);
            if (float_720_80425284 <= *(f32*)(work + 0x28)) {
                *(f32*)(work + 0x28) = float_720_80425284;
                (*(s32*)(work + 4))++;
            }
            break;
    }

    state = *(s32*)(work + 4);
    if ((state > 3) && (state < 7)) {
        count = *(s32*)((s32)effect + 8);
        partBase = work;
        for (i = 1; i < count; i++, partBase += 0x54) {
            if ((*(s32*)(partBase + 0xA0) < 1) ||
                ((*(s32*)(partBase + 0xA0) = *(s32*)(partBase + 0xA0) - 1), *(s32*)(partBase + 0xA0) < 1)) {
                *(f32*)(partBase + 0x6C) += *(f32*)(partBase + 0x74);
                *(f32*)(partBase + 0x5C) += *(f32*)(partBase + 0x68);
                *(f32*)(partBase + 0x60) += *(f32*)(partBase + 0x6C);
                *(f32*)(partBase + 0x64) += *(f32*)(partBase + 0x70);
                if ((state == 4) && (float_200_80425288 < *(f32*)(partBase + 0x5C))) {
                    *(f32*)(partBase + 0x5C) = float_0_80425254;
                    *(f32*)(partBase + 0x60) = float_0_80425254;
                    *(f32*)(partBase + 0x6C) = float_0_80425254;
                    *(f32*)(partBase + 0x64) = float_0_80425254;
                }
            }
        }
    }

    dispEntry(4, 2, effFireFlowerDisp, effect, dispCalcZ(pos));
}

#pragma optimize_for_size off

void* effFireFlowerN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireFlowerMain(void*);
    extern s32 rand(void);
    extern char str_FireFlowerN64_802faf18[];
    extern f32 float_0_80425254;
    extern f32 float_neg10_80425278;
    extern f32 float_0p5_8042528c;
    extern f32 float_8_80425290;
    extern f32 float_0p05_80425294;
    extern f32 float_1p5_80425298;
    void* entry;
    u8* work;
    u8* part;
    f32 zero;
    f32 eight;
    f32 point05;
    f32 divScale;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireFlowerN64_802faf18;
    *(s32*)((s32)entry + 8) = 0x1F;
    work = __memAlloc(3, 0xA2C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireFlowerMain;

    if (type == 1) {
        *(s32*)(work + 0x50) = 1;
        type = 0;
    }
    *(s32*)(work + 4) = 0;
    *(s32*)work = type;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (timer <= 0) {
        timer = 1000;
    }
    *(s32*)(work + 0x40) = timer;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x3C) = 0xFF;
    *(f32*)(work + 0x30) = float_0p5_8042528c;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x38) = 0;
    zero = float_0_80425254;
    *(f32*)(work + 0x28) = zero;
    *(f32*)(work + 0x2C) = float_neg10_80425278;
    eight = float_8_80425290;
    point05 = float_0p05_80425294;
    divScale = float_1p5_80425298;

    part = work + 0x54;
    for (i = 1; i < 0x1F; i++) {
        *(s32*)part = 1;
        *(f32*)(part + 8) = zero;
        *(f32*)(part + 0xC) = zero;
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = eight;
        *(f32*)(part + 0x18) = zero;
        *(f32*)(part + 0x1C) = (f32)(rand() % 20 - 8);
        *(f32*)(part + 0x20) = point05 * (f32)(rand() % 10 - 5);
        *(f32*)(part + 0x24) = (f32)(rand() % 80 + 5);
        *(s32*)(part + 0x3C) = 0xFF;
        *(s32*)(part + 0x4C) = (i - 1) * 2;
        *(s32*)(part + 0x44) = 0xA;
        *(f32*)(part + 0x14) /= divScale;
        *(f32*)(part + 0x18) /= divScale;
        *(f32*)(part + 0x1C) /= divScale;
        part += 0x54;
    }

    return entry;
}

#pragma optimize_for_size on

