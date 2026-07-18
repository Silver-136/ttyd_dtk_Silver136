#include "effect/n64/eff_magic_horao_n64.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_MagicHoraoN64_802fb4d0[];
extern f32 float_0_80425ae8;
extern f32 float_1_80425af8;


void effMagicHoraoDisp(s32 cameraId, void* effect) {
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
    extern f64 sin(f64);
    extern f32 float_deg2rad_80425ae4;
    extern f32 float_0_80425ae8;
    extern f32 float_0p015625_80425aec;
    extern f32 float_0p01_80425af0;
    extern f32 float_1_80425af8;
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4], rot[3][4], scale[3][4], model[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 state = *(s32*)(work + 0x2C);
    s32 texture;
    s32 pulse;

    PSMTXScale(scale, float_0p01_80425af0, float_0p01_80425af0, float_0p01_80425af0);
    PSMTXTrans(trans, *(f32*)(work + 4) * 100.0f,
                      *(f32*)(work + 8) * 100.0f,
                      *(f32*)(work + 0xC) * 100.0f);
    PSMTXConcat(scale, trans, trans);
    PSMTXScale(scale, *(f32*)(work + 0x24), *(f32*)(work + 0x24), *(f32*)(work + 0x24));
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat((u8*)camera + 0x11C, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);

    pulse = (s32)(225.0f + 25.0f * (f32)sin((6.2832f * (f32)(*(s32*)(work + 0x44) * 30)) / 360.0f));
    color = (pulse << 24) | (pulse << 16) | (pulse << 8) | (*(s32*)(work + 0x3C) & 0xFF);
    GXSetTevColor(1, &color);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p015625_80425aec, float_0p015625_80425aec, float_0_80425ae8);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effSetVtxDescN64((void*)0x803A5DC8);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 2, 8, 0);
    GXSetTevAlphaIn(0, 0, 1, 7, 7);
    GXSetCullMode(0);

    texture = state < 1 ? 0x56 : 0x57;
    effGetTexObjN64(texture, texObj);
    GXLoadTexObj(texObj, 0);
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);

    if (state == 1) {
        color = 0xFFFFFF00 | ((10 - *(s32*)(work + 0x30)) * 25);
        GXSetTevColor(1, &color);
        effGetTexObjN64(0x57, texObj);
        GXLoadTexObj(texObj, 0);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80425ae4 * (f32)*(s32*)(work + 0x38));
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagicHoraoMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effMagicHoraoDisp(void);
    extern void* effHokoriN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z);
    extern const Vec3 vec3_802fb4b8;
    extern f32 float_0p02_80425af4;
    extern f32 float_0_80425ae8;
    extern f32 float_1_80425af8;
    extern f32 float_5_80425afc;
    extern f32 float_0p05_80425b00;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 timer;
    s32 state;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb4b8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x40) -= 1;
    *(s32*)(work + 0x44) += 1;
    timer = *(s32*)(work + 0x40);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    state = *(s32*)(work + 0x2C);
    switch (state) {
        case 0:
            *(f32*)(work + 0x24) += *(f32*)(work + 0x28) * (*(f32*)(work + 0x18) - *(f32*)(work + 0x24));
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) +
                                          (*(f32*)(work + 0x28) * (f32)(*(s32*)(work + 0x20) - *(s32*)(work + 0x3C))));
            *(f32*)(work + 0x34) -= float_0p02_80425af4;
            if (*(f32*)(work + 0x34) < float_0_80425ae8) {
                *(f32*)(work + 0x34) = float_0_80425ae8;
                *(f32*)(work + 0x10) = float_0_80425ae8;
                *(s32*)(work + 0x30) = 10;
                *(s32*)(work + 0x2C) = 1;
            }
            break;
        case 1:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 2;
                *(s32*)(work + 0x30) = 10;
            }
            break;
        case 2:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 3;
            }
            break;
        case 3:
            *(f32*)(work + 0x10) -= float_1_80425af8;
            *(f32*)(work + 8) += *(f32*)(work + 0x10);
            if (*(f32*)(work + 8) < float_0_80425ae8) {
                *(f32*)(work + 8) = float_0_80425ae8;
                effHokoriN64Entry(2, 0x1E, *(f32*)(work + 4) + float_5_80425afc, *(f32*)(work + 8), *(f32*)(work + 0xC));
                effHokoriN64Entry(2, 0x1E, *(f32*)(work + 4) - float_5_80425afc, *(f32*)(work + 8), *(f32*)(work + 0xC));
                *(s32*)(work + 0x2C) = 4;
                *(s32*)(work + 0x30) = 0x14;
            }
            break;
        case 4:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 5;
            }
            break;
        case 5:
            if (timer < 10) {
                *(s32*)(work + 0x3C) = timer * 0x19;
            }
            *(f32*)(work + 0x34) -= float_0p05_80425b00;
            break;
    }

    dispEntry(4, 2, effMagicHoraoDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma optimize_for_size off

void* effMagicHoraoN64Entry(s32 type, f32 x, f32 y, f32 z, s32 param4, s32 param5, f32 param6, f32 param7, f32 param8, s32 param9) {
    void* entry;
    void* work;
    s32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_MagicHoraoN64_802fb4d0;
    *(s32*)((s32)entry + 0x8) = 1;

    work = __memAlloc(3, 0x48);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagicHoraoMain;

    *(s32*)((s32)work + 0x0) = type;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x28) = param8;
    *(f32*)((s32)work + 0x14) = param6;
    *(s32*)((s32)work + 0x1C) = param4;
    *(f32*)((s32)work + 0x18) = param7;
    *(s32*)((s32)work + 0x20) = param5;
    *(f32*)((s32)work + 0x34) = float_1_80425af8;

    zero = 0;
    *(s32*)((s32)work + 0x38) = rand() % 360;
    *(s32*)((s32)work + 0x40) = param9;
    *(s32*)((s32)work + 0x44) = zero;
    *(s32*)((s32)work + 0x3C) = param4;
    *(f32*)((s32)work + 0x24) = param6;
    *(f32*)((s32)work + 0x10) = float_0_80425ae8;
    *(s32*)((s32)work + 0x2C) = zero;

    return entry;
}

#pragma optimize_for_size on

#pragma no_register_save_helpers off
