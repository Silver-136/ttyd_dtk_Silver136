#include "effect/n64/eff_wangflush_n64.h"

typedef struct EffWangFlushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    s32 field_24;
    s32 field_28;
    s32 param;
    s32 field_30;
} EffWangFlushWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_WangflushN64_802fc258[];
extern f32 float_0_8042649c;
extern f32 float_30_804264c4;
extern f32 float_0p8_804264c8;

void effWangFlushDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2();
    extern f32 float_deg2rad_80426494;
    extern f32 float_0p03125_80426498;
    extern f32 float_0_8042649c;
    extern u32 dat_80426490;
    extern u8 color_rotation_data[];
    extern u8 unk_80429868;
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4], rot[3][4], model[3][4], scaleMtx[3][4];
    u8 texObj[0x20];
    u8 color[4];
    s32 type = *(s32*)work;
    s32 alpha0 = *(s32*)(work + 0x28);
    s32 colorIndex = *(s32*)(work + 0x30) * 3;
    s32 alpha1;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80426494 * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, model);
    PSMTXConcat((void*)((s32)camera + 0x11C), model, model);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 2);
    GXSetTevAlphaIn(0, 0, 1, 4, 7);
    effGetTexObjN64(0x1C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80426498, float_0p03125_80426498,
               float_0_8042649c);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803AB860);

    if (type == 0) {
        f32 scale = *(f32*)(work + 0x20);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(model, scaleMtx, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, 0);
        GXSetCurrentMtx(0);
        colorIndex %= 0x18;
        color[0] = color_rotation_data[colorIndex];
        color[1] = color_rotation_data[colorIndex + 1];
        color[2] = color_rotation_data[colorIndex + 2];
        color[3] = alpha0;
        GXSetTevColor(1, color);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }

    alpha1 = *(s32*)(work + 0x24);
    if (alpha1 > 0xFF) {
        alpha1 = 0xFF;
    }
    if (alpha1 > 0) {
        f32 scale = *(f32*)(work + 0x18);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80426494 * *(f32*)(work + 0x10));
        PSMTXConcat(rot, scaleMtx, scaleMtx);
        PSMTXConcat(model, scaleMtx, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, 0);
        GXSetCurrentMtx(0);
        color[0] = (dat_80426490 >> 24) & 0xFF;
        color[1] = (dat_80426490 >> 16) & 0xFF;
        color[2] = (dat_80426490 >> 8) & 0xFF;
        color[3] = alpha1;
        GXSetTevColor(1, color);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}

void effWangFlushMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effWangFlushDisp(void);
    extern Vec3 vec3_802fc240[];
    extern f32 float_0p5_804264a0;
    extern f32 float_0p78_804264a4;
    extern f32 float_0p29_804264a8;
    extern f32 float_0p93_804264ac;
    extern f32 float_6p2832_804264b0;
    extern f32 float_360_804264b4;
    extern f32 float_0p9_804264b8;
    extern f32 float_144_804264bc;
    extern f32 float_64_804264c0;
    extern double sin(double);
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 timer;
    s32 frame;
    f32 angle;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc240;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x2C) -= 1;
    *(s32*)(work + 0x30) += 1;
    timer = *(s32*)(work + 0x2C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x30);
    if (frame <= 6) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) + float_0p5_804264a0 * (f32)(0xDA - *(s32*)(work + 0x24)));
    }
    if (frame > 6) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p78_804264a4);
    }
    *(f32*)(work + 0x18) += *(f32*)(work + 0x1C);
    *(f32*)(work + 0x1C) += float_0p29_804264a8;
    *(f32*)(work + 0x10) += *(f32*)(work + 0x14);
    *(f32*)(work + 0x14) *= float_0p93_804264ac;
    angle = ((f32)(timer * 50) * float_6p2832_804264b0) / float_360_804264b4;
    *(f32*)(work + 0x20) = float_0p9_804264b8 + float_0p5_804264a0 * (f32)sin(angle);
    angle = ((f32)(timer * 40) * float_6p2832_804264b0) / float_360_804264b4;
    *(s32*)(work + 0x28) = (s32)(float_144_804264bc + float_64_804264c0 * (f32)sin(angle));
    dispEntry(4, 2, effWangFlushDisp, effect, dispCalcZ(&dispPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effWangFlushN64Entry(s32 type, s32 param, f32 x, f32 y, f32 z) {
    void* entry;
    EffWangFlushWork* work;
    f32 zero;
    f32 defaultAngle;
    f32 scale;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_WangflushN64_802fc258;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x34);
    *(EffWangFlushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effWangFlushMain;

    zeroInt = 0;
    zero = float_0_8042649c;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->param = param;
    work->field_30 = zeroInt;
    work->field_24 = zeroInt;
    work->field_10 = zero;
    if (type == 0) {
        defaultAngle = float_30_804264c4;
        work->field_14 = defaultAngle;
    } else {
        work->field_14 = zero;
    }
    zero = float_0_8042649c;
    zeroInt = 0;
    scale = float_0p8_804264c8;
    work->field_18 = zero;
    work->field_1C = scale;
    work->field_28 = zeroInt;
    work->field_20 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
