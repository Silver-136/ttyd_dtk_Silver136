#include "effect/n64/eff_star_power_n64.h"

typedef struct EffStarPowerWork {
    s16 field_00;
    s16 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 r1;
    s32 g1;
    s32 b1;
    s32 a1;
    f32 scale;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
} EffStarPowerWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StarPowerN64_802fc058[];
extern f32 float_0_804261ac;

u8 effStarPowerDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObjN64(s32, void*);
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
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s16);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx model;
    s32 r0 = *(s32*)(work + 0x18);
    s32 g0 = *(s32*)(work + 0x1C);
    s32 b0 = *(s32*)(work + 0x20);
    s32 a0 = *(s32*)(work + 0x24);
    s32 r1 = *(s32*)(work + 0x28);
    s32 g1 = *(s32*)(work + 0x2C);
    s32 b1 = *(s32*)(work + 0x30);
    s32 a1 = *(s32*)(work + 0x34);
    f32 size;
    u32 color;

    if (r0 > 0xFF) r0 = 0xFF;
    if (g0 > 0xFF) g0 = 0xFF;
    if (b0 > 0xFF) b0 = 0xFF;
    if (a0 > 0xFF) a0 = 0xFF;
    if (r1 > 0xFF) r1 = 0xFF;
    if (g1 > 0xFF) g1 = 0xFF;
    if (b1 > 0xFF) b1 = 0xFF;
    if (a1 > 0xFF) a1 = 0xFF;
    if (r0 < 0) r0 = 0;
    if (g0 < 0) g0 = 0;
    if (b0 < 0) b0 = 0;
    if (a0 < 0) a0 = 0;
    if (r1 < 0) r1 = 0;
    if (g1 < 0) g1 = 0;
    if (b1 < 0) b1 = 0;
    if (a1 < 0) a1 = 0;

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObjN64(0x49, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetCullMode(0);

    if (a1 != 0) {
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)((u8*)camGetPtr(4) + 0x114), 'y');
        size = 0.4f * *(f32*)(work + 0x54);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x48), 'x');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x4C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x50), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x11C, trans, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        color = (r0 << 24) | (g0 << 16) | (b0 << 8) | (a1 >> 1);
        GXSetTevColor(1, &color);
        color = (r1 << 24) | (g1 << 16) | (b1 << 8) | (a1 >> 1);
        GXSetTevColor(2, &color);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 8, 10, 15);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        effSetVtxDescN64((void*)0x803A8ED0);
        GXBegin(0x90, 0, 0x3C);
        tri2(0,1,2,0,0,2,3,0); tri2(4,0,3,0,4,3,5,0);
        tri2(1,6,7,0,1,7,2,0); tri2(6,8,9,0,6,9,7,0);
        tri2(10,9,8,0,10,8,11,0); tri2(11,12,13,0,11,13,10,0);
        tri2(14,4,5,0,14,5,15,0); tri2(12,16,17,0,12,17,13,0);
        tri2(16,18,19,0,16,19,17,0); tri2(18,14,15,0,18,15,19,0);
    }

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)((u8*)camGetPtr(4) + 0x114), 'y');
    size = 0.4f * *(f32*)(work + 0x38);
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scale, trans);
    PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x3C), 'x');
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x40), 'y');
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x44), 'z');
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(camera + 0x11C, trans, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    if (a1 != 0) {
        GXBegin(0x90, 0, 0x3C);
        tri2(0,1,2,0,0,2,3,0); tri2(4,0,3,0,4,3,5,0);
        tri2(1,6,7,0,1,7,2,0); tri2(6,8,9,0,6,9,7,0);
        tri2(10,9,8,0,10,8,11,0); tri2(11,12,13,0,11,13,10,0);
        tri2(14,4,5,0,14,5,15,0); tri2(12,16,17,0,12,17,13,0);
        tri2(16,18,19,0,16,19,17,0); tri2(18,14,15,0,18,15,19,0);
    }
    color = (r0 << 24) | (g0 << 16) | (b0 << 8) | a0;
    GXSetTevColor(1, &color);
    color = (r1 << 24) | (g1 << 16) | (b1 << 8) | a0;
    GXSetTevColor(2, &color);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 8, 10, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 1);
    effSetVtxDescN64((void*)0x803A8DB8);
    GXBegin(0x90, 0, 0x3C);
    tri2(0,1,2,0,0,2,3,0); tri2(4,0,3,0,4,3,5,0);
    tri2(1,6,7,0,1,7,2,0); tri2(6,8,9,0,6,9,7,0);
    tri2(10,9,8,0,10,8,11,0); tri2(11,12,13,0,11,13,10,0);
    tri2(14,4,5,0,14,5,15,0); tri2(12,16,17,0,12,17,13,0);
    tri2(16,18,19,0,16,19,17,0); tri2(18,14,15,0,18,15,19,0);
    return 0;
}

u8 effStarPowerMain(u32* effect) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effStarPowerDisp(void);
    extern f64 sin(f64);
    extern f32 float_0p4953_804261b8;
    extern f32 float_4_804261bc;
    extern f32 float_0p2234_804261c0;
    extern f32 float_6p2832_804261c4;
    extern f32 float_7p1234_804261c8;
    extern f32 float_360_804261cc;
    extern f32 float_128_804261d0;
    extern f32 float_127_804261d4;
    extern f32 float_1p231_804261d8;
    extern f32 float_215_804261dc;
    extern f32 float_0p531_804261e0;
    extern f32 float_3p231_804261e4;
    extern f32 float_0p298_804261e8;
    extern f32 float_188_804261ec;
    extern f32 float_0p831_804261f0;
    extern f32 float_2p231_804261f4;
    extern f32 float_2p044_804261f8;
    extern f32 float_0p7_804261fc;
    extern f32 float_0p3_80426200;
    extern Vec vec3_802fc040;
    u8* work = *(u8**)((u8*)effect + 0xC);
    Vec oldPos = vec3_802fc040;
    Vec oldPosCopy;
    s16 type;
    s32 timer;
    s32 frame;
    s32 value;
    u32 colorValue;
    f32 radians;
    f32 frame4;

    oldPos.x = *(f32*)(work + 4);
    oldPos.y = *(f32*)(work + 8);
    oldPos.z = *(f32*)(work + 0xC);
    oldPosCopy = oldPos;
    type = *(s16*)(work + 2);
    if ((*effect & 4) != 0) {
        *effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return 0;
    }
    frame = *(s32*)(work + 0x14);
    if (timer < 0x10) {
        value = timer << 4;
        if (value < *(s32*)(work + 0x24)) {
            *(s32*)(work + 0x24) = value;
        }
        if (value < *(s32*)(work + 0x34)) {
            *(s32*)(work + 0x34) = value;
        }
    }
    if (frame < 0x10) {
        value = frame * 0x10 + 0xF;
        if (*(s32*)(work + 0x24) < value) {
            *(s32*)(work + 0x24) = value;
        }
        if (*(s32*)(work + 0x34) < value) {
            *(s32*)(work + 0x34) = value;
        }
    }
    *(f32*)(work + 0x48) = *(f32*)(work + 0x3C);
    *(f32*)(work + 0x4C) = *(f32*)(work + 0x40);
    *(f32*)(work + 0x50) = *(f32*)(work + 0x44);
    *(f32*)(work + 0x54) = *(f32*)(work + 0x38);
    if (type == 1) {
        frame4 = float_4_804261bc * (f32)frame;
        radians = (float_6p2832_804261c4 * float_7p1234_804261c8 * (f32)frame) /
                  float_360_804261cc;
        *(f32*)(work + 0x3C) = frame4;
        *(f32*)(work + 0x40) = float_0p4953_804261b8 * frame4;
        *(f32*)(work + 0x44) = float_0p2234_804261c0 * frame4;
        *(s32*)(work + 0x24) = 0xFF;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_128_804261d0);
        *(u32*)(work + 0x34) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_1p231_804261d8 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_215_804261dc);
        *(u32*)(work + 0x18) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_0p531_804261e0 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_215_804261dc);
        *(u32*)(work + 0x1C) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_3p231_804261e4 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_215_804261dc);
        *(u32*)(work + 0x20) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_0p298_804261e8 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_188_804261ec);
        *(u32*)(work + 0x28) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_0p831_804261f0 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_188_804261ec);
        *(u32*)(work + 0x2C) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_2p231_804261f4 * (f32)frame) /
                  float_360_804261cc;
        colorValue = (u32)(float_127_804261d4 * (f32)sin((f64)radians) + float_188_804261ec);
        *(u32*)(work + 0x30) = colorValue & 0xFF;
        radians = (float_6p2832_804261c4 * float_2p044_804261f8 * (f32)frame) /
                  float_360_804261cc;
        *(f32*)(work + 0x38) = float_0p3_80426200 * (f32)sin((f64)radians) +
                               float_0p7_804261fc;
    }
    if (frame == 1) {
        *(f32*)(work + 0x48) = *(f32*)(work + 0x3C);
        *(f32*)(work + 0x4C) = *(f32*)(work + 0x40);
        *(f32*)(work + 0x50) = *(f32*)(work + 0x44);
        *(f32*)(work + 0x54) = *(f32*)(work + 0x38);
    }
    dispEntry(4, 2, effStarPowerDisp, effect, dispCalcZ(&oldPosCopy));
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effStarPowerN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStarPowerWork* work;
    f32 zero;
    s32 one;
    s32 zeroInt;
    s32 full;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarPowerN64_802fc058;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x58);
    *(EffStarPowerWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStarPowerMain;
    *(u32*)entry |= 2;

    one = 1;
    zeroInt = 0;
    work->field_00 = one;
    work->type = (s16)type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->x = x;
    full = 0xFF;
    half = 0x78;
    zeroInt = 0;
    work->y = y;
    zero = float_0_804261ac;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = half;
    work->r1 = full;
    work->g1 = half;
    work->b1 = zeroInt;
    work->a1 = full;
    work->a0 = full;
    work->field_3C = zero;
    work->field_40 = zero;
    work->field_44 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
