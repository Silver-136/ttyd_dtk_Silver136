#include "effect/n64/eff_magic_horao_n64.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_MagicHoraoN64_802fb4d0[];
extern f32 float_0_80425ae8;
extern f32 float_1_80425af8;


void effMagicHoraoMain(void* effect);
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

void effMagicHoraoDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void* smartAlloc(u32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
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
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern void DCFlushRange(void*, u32);
    extern void GXInvalidateVtxCache(void);
    extern f64 sin(f64);
    extern f64 double_to_int_802fb4c8;
    extern const f32 float_0p01_80425acc;
    extern const f32 float_100_80425ad0;
    extern const f32 float_6p2832_80425ad4;
    extern const f32 float_360_80425ad8;
    extern const f32 float_225_80425adc;
    extern const f32 float_25_80425ae0;
    extern const f32 float_0p015625_80425ae4;
    extern f32 float_0_80425ae8;
    extern const f32 float_500_80425aec;
    extern const f32 float_200_80425af0;
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 scale[3][4];
    f32 trans[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 state = *(s32*)(work + 0x2C);
    s32 angle = *(s32*)(work + 0x38);
    s32 alpha = *(s32*)(work + 0x3C);
    s32 timer = *(s32*)(work + 0x44);
    s32 pulse;

    PSMTXScale(scale, float_0p01_80425acc, float_0p01_80425acc, float_0p01_80425acc);
    PSMTXTrans(trans, *(f32*)(work + 4) * float_100_80425ad0,
                      *(f32*)(work + 8) * float_100_80425ad0,
                      *(f32*)(work + 0xC) * float_100_80425ad0);
    PSMTXConcat(scale, trans, trans);
    PSMTXScale(scale, *(f32*)(work + 0x24), *(f32*)(work + 0x24), *(f32*)(work + 0x24));
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat((u8*)camera + 0x11C, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);

    pulse = (s32)(float_25_80425ae0 * (f32)sin((double)((float_6p2832_80425ad4 *
              (f32)(timer * 30)) / float_360_80425ad8)) + float_225_80425adc);
    color = ((u32)(u8)pulse << 24) | ((u32)(u8)pulse << 16) |
            ((u32)(u8)pulse << 8) | (u8)alpha;
    GXSetTevColor(1, &color);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p015625_80425ae4, float_0p015625_80425ae4, float_0_80425ae8);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effSetVtxDescN64((void*)0x803A5DC8);

    if (state == 1) {
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0, 0, 0, 8);
        GXSetTevAlphaIn(0, 0, 4, 5, 7);
        effGetTexObjN64(0x56, texObj);
        GXLoadTexObj(texObj, 0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);

        alpha = ((30 - *(s32*)(work + 0x30)) * 255) / 30;
        color = 0xFFFFFF00 | (u8)alpha;
        GXSetTevColor(1, &color);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0, 0, 0, 8);
        GXSetTevAlphaIn(0, 0, 4, 5, 7);
        effGetTexObjN64(0x57, texObj);
        GXLoadTexObj(texObj, 0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    } else {
        s16* vertices;
        s16* v;
        s16 texS = 0;
        s16 yBase = 0;
        s32 i;
        s32 phase = timer * 10;

        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        if (state < 1) {
            GXSetTevColorIn(0, 0, 0, 0, 8);
            GXSetTevAlphaIn(0, 0, 4, 5, 7);
            effGetTexObjN64(0x56, texObj);
        } else {
            GXSetTevColorIn(0, 0, 0, 0, 1);
            GXSetTevAlphaIn(0, 0, 4, 5, 7);
            effGetTexObjN64(0x57, texObj);
        }
        GXLoadTexObj(texObj, 0);
        vertices = (s16*)smartAlloc(0x1C0, 3);
        v = vertices;
        for (i = 0; i < 16; i++, v += 14) {
            s32 x = (s32)(*(f32*)(work + 0x34) * float_500_80425aec *
                    (f32)sin((double)((float_6p2832_80425ad4 *
                    (f32)(angle + phase)) / float_360_80425ad8)));
            s32 y = (s32)(*(f32*)(work + 0x34) * float_200_80425af0 *
                    (f32)sin((double)((float_6p2832_80425ad4 *
                    (f32)(angle + phase)) / float_360_80425ad8)));
            v[0] = (s16)x - 0xC80;
            v[1] = (s16)y + yBase;
            v[2] = 0;
            v[3] = 0;
            v[4] = texS;
            v[7] = (s16)x + 0xC80;
            v[8] = (s16)y + yBase;
            v[9] = 0;
            v[10] = 0x800;
            v[11] = texS;
            phase += 60;
            yBase += 400;
            texS += 0x80;
        }
        DCFlushRange(vertices, 0x1C0);
        GXInvalidateVtxCache();
        effSetVtxDescN64(vertices);
        for (i = 0; i < 15; i++) {
            s32 n = i * 2;
            GXBegin(0x90, 0, 6);
            tri2((s16)n, (s16)n + 2, (s16)n + 1, n,
                 (s16)n + 1, (s16)n + 2, (s16)n + 3);
        }
    }
}

u8 size64x64_tex64x64_vtx[56] = {
    0xF3, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF,
    0x0C, 0x80, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0xFF,
    0x0C, 0x80, 0x19, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0xFF,
    0xF3, 0x80, 0x19, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0xFF,
};

const f32 vec3_802fb4b8[3] = { 0.0f, 0.0f, 0.0f };
const char str_MagicHoraoN64_802fb4d0[] = "MagicHoraoN64";

/* Target-owned renderer constants; late definitions preserve external-load codegen. */
const f32 float_0p01_80425acc = 0.01f;
const f32 float_100_80425ad0 = 100.0f;
const f32 float_6p2832_80425ad4 = 6.2831855f;
const f32 float_360_80425ad8 = 360.0f;
const f32 float_225_80425adc = 225.0f;
const f32 float_25_80425ae0 = 25.0f;
const f32 float_0p015625_80425ae4 = 0.015625f;
const f32 float_500_80425aec = 500.0f;
const f32 float_200_80425af0 = 200.0f;
