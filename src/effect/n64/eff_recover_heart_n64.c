#include "effect/n64/eff_recover_heart_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effRecoverHeartN64Entry(s32 type, s32 direction, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRecoverHeartMain(void*);
    extern char str_RecoverHeartN64_802fbe08[];
    extern f32 float_0_80425e90;
    extern f32 float_1p4_80425eb4;
    void* entry;
    u8* work;
    s32 mode;
    f32 zero;
    f32 onePointFour;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_RecoverHeartN64_802fbe08;
    *(s32*)((s32)entry + 8) = 2;
    work = __memAlloc(3, 0x90);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRecoverHeartMain;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x2C) = 0x2E;
    *(s32*)(work + 0x30) = 0;
    if (direction < 0) {
        direction = -direction;
        *(s32*)(work + 0x44) = 1;
    } else {
        *(s32*)(work + 0x44) = 0;
    }
    *(s32*)(work + 0x40) = direction;
    mode = 1;
    zero = float_0_80425e90;
    if (mode == 1) {
        *(f32*)(work + 0x4C) = zero;
    }
    *(f32*)(work + 0x50) = zero;
    *(f32*)(work + 0x54) = zero;
    *(f32*)(work + 0x6C) = zero;
    *(f32*)(work + 0x58) = zero;
    *(f32*)(work + 0x60) = zero;
    *(f32*)(work + 0x68) = zero;
    *(f32*)(work + 0x64) = zero;
    if (mode == 1) {
        *(f32*)(work + 0x4C) = zero;
        *(s32*)(work + 0x7C) = 0;
        onePointFour = float_1p4_80425eb4;
        *(f32*)(work + 0x80) = zero;
        *(f32*)(work + 0x84) = zero;
        *(f32*)(work + 0x5C) = onePointFour;
    } else {
        *(f32*)(work + 0x4C) = (f32)(((((mode & 1) << 1) - 1) * (0 >> 1)) << 2);
        onePointFour = float_1p4_80425eb4;
        *(f32*)(work + 0x5C) = onePointFour;
        *(s32*)(work + 0x7C) = 0xB;
        *(f32*)(work + 0x80) = zero;
        *(f32*)(work + 0x84) = zero;
    }
    *(s32*)(work + 0x74) = 0;
    *(s32*)(work + 0x70) = 0xFF;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effRecoverHeartMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRecoverHeartDisp(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern const Vec3 vec3_802fbdf0;
    extern f32 scale_w_dat[];
    extern f32 scale_h_dat[];
    extern f32 float_0p05_80425e9c;
    extern f32 float_6p2832_80425ea0;
    extern f32 float_360_80425ea4;
    extern f32 float_neg30_80425ea8;
    extern f32 float_8_80425eac;
    extern f32 float_0p9_80425eb0;
    extern f32 float_1_80425e94;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    f32 angle;
    f32 fadeScale;
    s32 i;
    s32 timer;
    s32 angleCounter;
    s32 alphaValue;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbdf0;
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

    fadeScale = (f32)timer * float_0p05_80425e9c;
    i = 1;
    part = work + 0x48;
    angleCounter = timer * 0xC + 0x26;
    alphaValue = timer * 0x19;
    for (; i < *(s32*)((s32)effect + 8); i++, part += 0x48, angleCounter += 0x26) {
        s32 wait = *(s32*)(part + 0x34);
        s32 frame = *(s32*)(part + 0x2C);
        if (wait > 0) {
            *(s32*)(part + 0x34) = wait - 1;
            if (*(s32*)(part + 0x34) > 0) {
                continue;
            }
        }

        if (frame >= 7) {
            *(f32*)(part + 0x3C) = float_1_80425e94;
            *(f32*)(part + 0x38) = float_1_80425e94;
        } else {
            *(f32*)(part + 0x38) = scale_w_dat[frame];
            *(f32*)(part + 0x3C) = scale_h_dat[frame];
        }

        angle = (float_6p2832_80425ea0 * (f32)angleCounter) / float_360_80425ea4;
        *(f32*)(part + 4) += *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x1C) = float_neg30_80425ea8 * (f32)sin(angle);
        *(f32*)(part + 0x24) = float_8_80425eac * (f32)cos(angle);
        if (timer < 20) {
            *(f32*)(part + 0x24) *= fadeScale;
            *(f32*)(part + 0x1C) *= fadeScale;
        }
        if (timer < 10) {
            *(s32*)(part + 0x28) = alphaValue;
            *(f32*)(part + 0x14) *= float_0p9_80425eb0;
        }
        *(s32*)(part + 0x2C) += 1;
    }

    dispEntry(4, 2, effRecoverHeartDisp, effect, dispCalcZ(&dispPos));
}


void effRecoverHeartDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425e88;
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* part = work + 0x48;
    void* camera = camGetPtr(cameraId);
    f32 base[3][4], rot[3][4], trans[3][4], scale[3][4], model[3][4];
    u8 texObj[0x20];
    s32 type = *(s32*)work;
    s32 direction = *(s32*)(work + 0x44);
    s32 i;
    s32 texture;

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80425e88 * -*(f32*)((u8*)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, base);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetCullMode(0);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x48) {
        u32 color;
        if (*(s32*)(part + 0x34) > 0) continue;
        color = 0xFFFFFF00 | (*(s32*)(part + 0x28) & 0xFF);
        GXSetTevColor(1, &color);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0, 8, 10, 0);
        GXSetTevAlphaIn(0, 0, 4, 7, 7);
        texture = (type == 0 || type == 2) ? (direction == 0 ? 0x50 : 0x51) : 0x52;
        effGetTexObjN64(texture, texObj);
        GXLoadTexObj(texObj, 0);
        PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        PSMTXTrans(trans, *(f32*)(part + 4) + *(f32*)(part + 0x1C),
                          *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXScale(scale, *(f32*)(part + 0x38), *(f32*)(part + 0x3C), 1.0f);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(base, trans, model);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80425e88 * *(f32*)(part + 0x24));
        PSMTXConcat(model, rot, model);
        PSMTXConcat((u8*)camera + 0x11C, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64((void*)0x803A7A78);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
