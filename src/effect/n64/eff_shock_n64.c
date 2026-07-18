#include "effect/n64/eff_shock_n64.h"


u8 effShockDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    u32 color;
    s32 type = *(s32*)work;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)((char*)camGetPtr(4) + 0x114), 'y');
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(camera + 0x118, model, model);
    color = (*(u8*)(work + 0x30) << 24) | (*(u8*)(work + 0x34) << 16) |
            (*(u8*)(work + 0x38) << 8) | *(u8*)(work + 0x18);
    GXSetTevColor(1, &color);
    color = (*(u8*)(work + 0x3C) << 24) | (*(u8*)(work + 0x40) << 16) |
            (*(u8*)(work + 0x44) << 8) | 0xFF;
    GXSetTevColor(2, &color);
    GXSetCullMode(0);
    if ((u32)(type - 1) < 2 || type == 3) {
        PSMTXTrans(trans, 0.0f, *(f32*)(work + 0x2C), 0.0f);
        PSMTXScale(scale, 0.25f * *(f32*)(work + 0x24),
                   0.25f * *(f32*)(work + 0x24), 1.0f);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(model, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        effGetTexObj(0x85, &tex);
        GXLoadTexObj(&tex, 0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 8, 10, 15);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        effSetVtxDescN64((void*)0x803A8460);
        GXBegin(0x90, 0, 0x90);
        tri2(0x30,0x31,0x32,0,0x30,0x32,0x33,0);
        tri2(0x31,0x34,0x35,0,0x31,0x35,0x32,0);
        tri2(0x34,0x36,0x37,0,0x34,0x37,0x35,0);
        tri2(0x38,0x30,0x33,0,0x38,0x33,0x39,0);
        tri2(0x3A,0x38,0x39,0,0x3A,0x39,0x3B,0);
        tri2(0x3C,0x3A,0x3B,0,0x3C,0x3B,0x3D,0);
        tri2(0x3E,0x3C,0x3D,0,0x3E,0x3D,0x3F,0);
        tri2(0x36,0x40,0x41,0,0x36,0x41,0x37,0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 8, 10, 15);
        GXSetTevAlphaIn(0, 7, 7, 7, 1);
        GXBegin(0x90, 0, 0x90);
        tri2(0,1,2,0,0,2,3,0);
        tri2(1,4,5,0,1,5,2,0);
        tri2(4,6,7,0,4,7,5,0);
        tri2(8,0,3,0,8,3,9,0);
        tri2(10,8,9,0,10,9,11,0);
        tri2(12,10,11,0,12,11,13,0);
        tri2(14,12,13,0,14,13,15,0);
        tri2(6,16,17,0,6,17,7,0);
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effShockMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effShockDisp(void);
    extern const Vec3 vec3_802fbef0;
    extern f32 float_1p1_80425fe8;
    extern f32 float_0p05_80425fec;
    extern f32 float_8_80425ff0;
    extern f32 float_0p9_80425ff4;
    extern f32 float_0p8_80425ff8;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbef0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    timer = *(s32*)(work + 0x1C);
    type = *(s32*)work;
    if (timer < 100) {
        *(s32*)(work + 0x1C) = timer - 1;
    }
    *(s32*)(work + 0x20) += 1;
    timer = *(s32*)(work + 0x1C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x20);
    if (type == 2) {
        if (frame <= 20) {
            *(f32*)(work + 0x24) *= float_1p1_80425fe8;
        }
    } else {
        *(f32*)(work + 0x24) += float_0p05_80425fec * (float_8_80425ff0 - *(f32*)(work + 0x24));
    }
    *(s32*)(work + 0x14) = (s32)((f32)*(s32*)(work + 0x14) * float_0p9_80425ff4);
    if (timer < 5) {
        *(s32*)(work + 0x18) = (s32)((f32)*(s32*)(work + 0x18) * float_0p8_80425ff8);
    }
    if (frame > 6) {
        *(s32*)(work + 0x10) -= 13;
        if (*(s32*)(work + 0x10) < 0) {
            *(s32*)(work + 0x10) = 0;
        }
    }
    dispEntry(4, 2, effShockDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effShockN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effShockMain(void*);
    extern char str_ShockN64_802fbf08[];
    extern f32 float_1_80425fe0;
    extern f32 float_0_80425fd8;
    extern f32 float_0p5_80425ffc;
    void* entry;
    u8* work;
    f32 half;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ShockN64_802fbf08;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x48);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effShockMain;
    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;
    *(s32*)(work + 0x1C) = 0x1E;
    *(s32*)(work + 0x10) = 0xFF;
    *(s32*)(work + 0x14) = 0xFF;
    *(s32*)(work + 0x18) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    if (type == 2) {
        *(f32*)(work + 0x24) = float_1_80425fe0;
    } else {
        *(f32*)(work + 0x24) = float_0_80425fd8;
    }
    half = float_0p5_80425ffc;
    *(f32*)(work + 0x28) = half * sx;
    *(f32*)(work + 0x2C) = half * sy;
    if (type == 2) {
        goto colored;
    }
    if (type < 2) {
        if (type >= 0) {
            goto white;
        }
        goto colored;
    }
    if (type >= 4) {
        goto colored;
    }
white:
    {
        *(s32*)(work + 0x30) = 0xFF;
        *(s32*)(work + 0x34) = 0xFF;
        *(s32*)(work + 0x38) = 0;
        *(s32*)(work + 0x3C) = 0xFF;
        *(s32*)(work + 0x40) = 0xFF;
        *(s32*)(work + 0x44) = 0xFF;
    }
    goto done;
colored:
    {
        *(s32*)(work + 0x30) = 0x46;
        *(s32*)(work + 0x34) = 0xB4;
        *(s32*)(work + 0x38) = 0x78;
        *(s32*)(work + 0x3C) = 0xFF;
        *(s32*)(work + 0x40) = 0xFF;
        *(s32*)(work + 0x44) = 0xFF;
    }
done:
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

