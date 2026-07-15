#include "effect/n64/eff_sunlight_n64.h"

typedef struct EffSunlightWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    u8 color0[7];
    u8 pad_1F;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    f32 field_2C;
    f32 field_30;
    f32 scale;
    s32 field_38;
    s32 field_3C;
} EffSunlightWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SunlightN64_802fc128[];
extern f32 float_0_80426300;


void effSunlightDisp(s32 cameraId, void* effect)  {
    extern void GXGetProjectionv(f32*);
    extern void C_MTXOrtho(void*, f32, f32, f32, f32, f32, f32);
    extern void C_MTXLookAt(void*, void*, void*, void*);
    extern void GXSetProjection(void*, s32);
    extern void GXSetProjectionv(f32*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8* work=*(u8**)((s32)effect+0xC);
    f32 projection[7], ortho[4][4], look[3][4], tex[3][4], trans[3][4];
    f32 up[3]= {
        0.0f,1.0f,0.0f
    }
    , pos[3]= {
        0.0f,0.0f,1.0f
    }
    , target[3]= {
        0.0f,0.0f,0.0f
    }
    ;
    u8 texObj[0x20];
    u32 color;
    s32 i;
    s32 alpha=*(s32*)(work+0x38);
    if(alpha==0)return;
    GXGetProjectionv(projection);
    if(*(s32*)work==0)C_MTXOrtho(ortho,1200.0f,-1200.0f,1600.0f,-1600.0f,1.0f,32768.0f);
    else C_MTXOrtho(ortho,1200.0f,-1200.0f,-1600.0f,1600.0f,1.0f,32768.0f);
    GXSetProjection(ortho,1);
    C_MTXLookAt(look,pos,up,target);
    GXLoadPosMtxImm(look,0);
    GXSetCurrentMtx(0);
    color=(*(u32*)(work+0x18)&0xFFFFFF00)|((alpha>>1)&0xFF);
    GXSetTevColor(1,&color);
    color=*(u32*)(work+0x1B);
    GXSetTevColor(2,&color);
    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,1,0,0,2);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,4);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,3,2,8,0);
    GXSetTevAlphaIn(0,0,5,4,7);
    GXSetTevOrder(1,1,1,0xFF);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,3,2,8,0);
    GXSetTevAlphaIn(1,0,6,7,7);
    GXSetTevOrder(2,1,1,0xFF);
    GXSetTevColorOp(2,0,0,0,1,0);
    GXSetTevAlphaOp(2,1,0,0,1,0);
    GXSetTevColorIn(2,0,0,0,1);
    GXSetTevAlphaIn(2,0,4,7,0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    PSMTXScale(tex,0.0625f,0.015625f,0.0f);
    GXLoadTexMtxImm(tex,0x1E,1);
    effGetTexObjN64(0x42,texObj);
    GXLoadTexObj(texObj,0);
    effGetTexObjN64(0x43,texObj);
    GXLoadTexObj(texObj,1);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A93E0);
    for(i=0;i<5;i++) {
        PSMTXScale(tex,0.00390625f,0.125f,0.0f);
        PSMTXTrans(trans,*(f32*)(work+0x20+i*4)+(f32)(i*11),0.0f,0.0f);
        PSMTXConcat(tex,trans,tex);
        GXLoadTexMtxImm(tex,0x21,1);
        GXBegin(0x90,0,(i==0||i==3)?12:6);
        if(i==0) {
            tri2(0,1,2,0,0,2,3,0);
            tri2(4,5,6,0,4,6,7,0);
        } else if(i==1)tri2(8,9,10,8,8,10,11,0);
        else if(i==2)tri2(12,13,14,12,12,14,15,0);
        else if(i==3) {
            tri2(16,17,18,16,16,18,19,0);
            tri2(20,21,22,16,20,22,23,0);
        } else tri2(24,25,26,24,24,26,27,0);
    }
    GXSetProjectionv(projection);
}

u8 effSunlightMain(void* effect) {
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 pos[3];
    s32 i;
    s32 timer;
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    if (*(u32*)effect & 4) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) *(s32*)(work + 0x10) -= 1;
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x14) > 0x4F1A0) *(s32*)(work + 0x14) = 0x100;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return 0;
    }
    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x38) -= 0x10;
        if (*(s32*)(work + 0x38) < 0) *(s32*)(work + 0x38) = 0;
    }
    if (*(s32*)(work + 0x3C) < 0) *(s32*)(work + 0x3C) = 0;
    if (*(s32*)(work + 0x3C) > 0xFF) *(s32*)(work + 0x3C) = 0xFF;
    timer = *(s32*)(work + 0x38);
    if (*(s32*)(work + 0x3C) < timer) {
        timer -= 8;
        if (timer < *(s32*)(work + 0x3C)) timer = *(s32*)(work + 0x3C);
        *(s32*)(work + 0x38) = timer;
    } else if (timer < *(s32*)(work + 0x3C)) {
        timer += 8;
        if (*(s32*)(work + 0x3C) < timer) timer = *(s32*)(work + 0x3C);
        *(s32*)(work + 0x38) = timer;
    }
    for (i = 0; i < 5; i++) {
        f32 a = 6.2832f * (0.25f * (f32)*(s32*)(work + 0x14) + (f32)(i * i * 20)) / 360.0f;
        f32 b = 6.2832f * (f32)(*(s32*)(work + 0x14) * 2) / 360.0f;
        f32* value = (f32*)(work + 0x20 + i * 4);
        *value = -(4.0f * (0.01f * (f32)sin(b) + 0.05f) * (f32)sin(a) - *value);
        if (*value < 0.0f) *value += 256.0f;
        if (*value > 256.0f) *value -= 256.0f;
    }
    dispEntry(4, 10, effSunlightDisp, effect, (f32)dispCalcZ(pos));
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSunlightN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffSunlightWork* work;
    f32 zero;
    s32 zeroInt;
    s32 full;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SunlightN64_802fc128;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x40);
    *(EffSunlightWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSunlightMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    work->x = x;
    full = 0xFF;
    zero = float_0_80426300;
    zeroInt = 0;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->color0[0] = full;
    work->color0[1] = full;
    work->color0[2] = full;
    work->color0[3] = full;
    work->color0[4] = full;
    work->color0[5] = full;
    work->color0[6] = full;
    work->field_20 = zero;
    work->field_24 = zero;
    work->field_28 = zero;
    work->field_2C = zero;
    work->field_30 = zero;
    work->field_3C = full;
    work->field_38 = zeroInt;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
