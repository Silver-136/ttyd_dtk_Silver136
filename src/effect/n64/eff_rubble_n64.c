#include "effect/n64/eff_rubble_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off

void* effRubbleN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 groundY, f32 vy, f32 gravity, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRubbleMain(void*);
    extern s32 rand(void);
    extern char str_RubbleN64_802fbee0[];
    extern f32 float_0_80425fbc;
    extern f32 float_20_80425fc0;
    extern f32 float_neg20_80425fc4;
    extern f32 float_2_80425fc8;
    void* entry;
    u8* work;
    f32 zero;
    f32 two;
    f32 twenty;
    f32 negTwenty;
    f32 scaled;

    entry = effEntry();
    zero = float_0_80425fbc;
    *(char**)((s32)entry + 0x14) = str_RubbleN64_802fbee0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x50);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRubbleMain;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = groundY;
    *(f32*)(work + 0x18) = vy;
    *(f32*)(work + 0x20) = gravity;
    *(s32*)(work + 0x48) = lifetime;
    *(s32*)(work + 0x4C) = 0;
    *(f32*)(work + 0x14) = zero;
    *(f32*)(work + 0x1C) = zero;
    *(f32*)(work + 0x24) = zero;
    *(f32*)(work + 0x28) = (f32)(rand() % 360);
    *(f32*)(work + 0x2C) = (f32)(rand() % 360);
    two = float_2_80425fc8;
    scaled = two * scale;
    twenty = float_20_80425fc0;
    negTwenty = float_neg20_80425fc4;
    *(f32*)(work + 0x30) = zero;
    *(f32*)(work + 0x38) = twenty;
    *(f32*)(work + 0x34) = negTwenty;
    *(s32*)(work + 0x44) = 0xFF;
    *(f32*)(work + 0x3C) = scaled;
    *(s32*)(work + 0x40) = 0;
    return entry;
}

#pragma optimize_for_size on

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effRubbleMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRubbleDisp(void);
    extern void* effHokoriN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z);
    extern const Vec3 vec3_802fbec8;
    extern f32 float_0p8_80425fb8;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbec8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x48) -= 1;
    *(s32*)(work + 0x4C) += 1;
    timer = *(s32*)(work + 0x48);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (*(f32*)(work + 8) != *(f32*)(work + 0x10)) {
        *(f32*)(work + 0x18) += *(f32*)(work + 0x20);
        *(f32*)(work + 8) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x24) += *(f32*)(work + 0x30);
        *(f32*)(work + 0x2C) += *(f32*)(work + 0x38);
        *(f32*)(work + 0x28) += *(f32*)(work + 0x34);
        if (*(f32*)(work + 8) <= *(f32*)(work + 0x10)) {
            *(f32*)(work + 8) = *(f32*)(work + 0x10);
            effHokoriN64Entry(2, 0x14, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
            *(s32*)(work + 0x48) = 0x1E;
        }
    }
    if (timer < 10) {
        *(s32*)(work + 0x44) = (s32)((f32)*(s32*)(work + 0x44) * float_0p8_80425fb8);
    }
    dispEntry(4, 2, effRubbleDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off



void effRubbleDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetTevColor(s32,void*);extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTexGens(s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_deg2rad_80425fb4;u8* work=*(u8**)((s32)effect+0xC);f32 trans[3][4],rot[3][4],scale[3][4];u32 color=*(u32*)(work+0x44);s32 type=*(s32*)work;
    PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x78,float_deg2rad_80425fb4**(f32*)(work+0x24));PSMTXConcat(trans,rot,trans);PSMTXRotRad(rot,0x79,float_deg2rad_80425fb4**(f32*)(work+0x2C));PSMTXConcat(trans,rot,trans);PSMTXRotRad(rot,0x7A,float_deg2rad_80425fb4**(f32*)(work+0x28));PSMTXConcat(trans,rot,trans);PSMTXScale(scale,*(f32*)(work+0x3C),*(f32*)(work+0x3C),*(f32*)(work+0x3C));PSMTXConcat(trans,scale,trans);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),trans,trans);GXLoadPosMtxImm(trans,0);GXSetCurrentMtx(0);GXSetTevColor(1,&color);GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetNumTexGens(0);GXSetNumTevStages(1);GXSetTevOrder(0,0xFF,0xFF,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,2,1,0);GXSetTevAlphaIn(0,0,0,0,1);GXSetCullMode(0);
    if(type==3){effSetVtxDescN64((void*)0x803A82B8);GXBegin(0x90,0,0x1E);}else{effSetVtxDescN64((void*)0x803A8130);GXBegin(0x90,0,0x1E);}tri2(0,1,2,0,3,4,5,0);tri2(6,7,8,0,9,10,11,0);tri2(12,13,14,0,15,16,17,0);
}
