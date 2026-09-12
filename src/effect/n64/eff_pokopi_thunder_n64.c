#include "effect/n64/eff_pokopi_thunder_n64.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPokopiThunderN64Entry(s32 type, s32 time, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effPokopiThunderMain(void*);
    extern s32 rand(void);
    extern const u8 vec3_802fbd28[];
    extern f32 float_0_80425db0;
    extern f32 float_0p005_80425ddc;
    extern s32 __float_nan[];
    const u8* data = vec3_802fbd28;
    void* entry;
    u8* work;
    s32 camera;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 square;
    f32 dist;

    entry = effEntry();
    camera = 4;
    if (type >= 10) {
        camera = 8;
    }
    *(const char**)((s32)entry + 0x14) = (const char*)(data + 0x20);
    *(s32*)((s32)entry + 8) = 1;
    type %= 10;
    work = __memAlloc(3, 0x11C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPokopiThunderMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x2C) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x28) = 0x3E8;
    } else {
        *(s32*)(work + 0x28) = time;
    }
    *(s32*)(work + 0x3C) = 0xFF;

    dy = y0 - y1;
    dx = x0 - x1;
    dz = z0 - z1;
    square = dy * dy + dx * dx + dz * dz;
    *(f32*)(work + 0x10) = x0;
    *(f32*)(work + 0x14) = y0;
    *(f32*)(work + 0x18) = z0;
    *(f32*)(work + 0x1C) = x1;
    *(f32*)(work + 0x20) = y1;
    *(f32*)(work + 0x24) = z1;
    *(f32*)(work + 4) = x0;
    *(f32*)(work + 8) = y0;
    *(f32*)(work + 0xC) = z0;
    *(f32*)(work + 0x4C) = scale;
    *(s32*)(work + 0x118) = camera;

    if (square == float_0_80425db0) {
        return 0;
    }
    if (square > float_0_80425db0) {
        register f64 value = (f64)square;
        register f64 inv;
        asm { frsqrte inv, value }
        inv = *(const f64*)(data + 0x38) * inv *
              -((value * inv * inv) - *(const f64*)(data + 0x40));
        inv = *(const f64*)(data + 0x38) * inv *
              -((value * inv * inv) - *(const f64*)(data + 0x40));
        inv = *(const f64*)(data + 0x38) * inv *
              -((value * inv * inv) - *(const f64*)(data + 0x40));
        dist = (f32)(value * inv);
    } else if ((f64)square < *(const f64*)(data + 0x48)) {
        dist = *(f32*)__float_nan;
    } else {
        union {
            f32 value;
            u32 bits;
        } classify;
        u32 exponent;
        s32 kind;

        classify.value = square;
        exponent = classify.bits & 0x7F800000;
        if (exponent == 0x7F800000) {
            if ((classify.bits & 0x7FFFFF) != 0) {
                kind = 1;
            } else {
                kind = 2;
            }
        } else if ((exponent < 0x7F800000) && (exponent == 0)) {
            if ((classify.bits & 0x7FFFFF) != 0) {
                kind = 5;
            } else {
                kind = 3;
            }
        } else {
            kind = 4;
        }
        if (kind == 1) {
            dist = *(f32*)__float_nan;
        } else {
            dist = square;
        }
    }
    *(f32*)(work + 0x114) = dist;
    *(f32*)(work + 0x114) *= float_0p005_80425ddc;
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0xDC;
    *(s32*)(work + 0x38) = 0x14;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x110) = rand() % 0x168;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiThunderMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiThunderDisp(void);
    extern const Vec3 vec3_802fbd28;
    extern f32 float_0p5_80425dd4;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbd28;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x10;
    }
    if (*(s32*)(work + 0x28) < 0x3E8) {
        *(s32*)(work + 0x28) -= 1;
    }
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    type = *(s32*)work;
    frame = *(s32*)(work + 0x2C);
    if (type == 2) {
        if (frame >= 0x28) {
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
        }
    } else if (frame >= 0xB) {
        *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
    }
    dispEntry(*(s32*)(work + 0x118), 2, effPokopiThunderDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off



u8 effPokopiThunderDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void* smartAlloc(u32, s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void GXSetProjection(void*, s32);
    extern void GXSetProjectionv(f32*);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    extern void DCFlushRange(void*, u32);
    extern void GXInvalidateVtxCache(void);
    extern void effSetVtxDescN64(void*);
    extern u8 tri2(s16, s16, s16, s32, s16, s16, s16, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern f32 float_0p5_80425dd4;
    extern f32 float_0p1_80425da4;
    extern f32 float_0_80425db0;
    extern f32 float_6p2832_80425db8;
    extern f32 float_360_80425dbc;
    extern f32 float_0p04_80425dc0;
    extern f32 float_8_80425dc4;
    extern f32 float_neg90_80425dc8;
    extern f32 float_180_80425dcc;
    extern f32 float_neg180_80425dd0;
    extern f32 float_10_80425dd8;
    extern u8 yuragi_data[];
    extern u8* yuragi_data_table[];
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx scale, model;
    f32 projection[7];
    f32 viewport[6];
    f32* vertices;
    u32 color;
    s32 i;
    s32 frame;
    s32 type;
    u8* pattern;

    if (*(s32*)(work + 0x118) != 4) {
        camera = camGetPtr(4);
        GXGetProjectionv(projection);
        GXGetViewportv(viewport);
        GXSetProjection(camera + 0x15C, *(s32*)(camera + 0x19C));
    }

    PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(camera + 0x11C, scale, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    color = (*(u8*)(work + 0x30) << 24) | (*(u8*)(work + 0x34) << 16) |
            (*(u8*)(work + 0x38) << 8) | *(u8*)(work + 0x3C);
    GXSetTevColor(1, &color);
    color = (*(u8*)(work + 0x40) << 24) | (*(u8*)(work + 0x44) << 16) |
            (*(u8*)(work + 0x48) << 8) | 0xFF;
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(4);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
    GXSetTevColorIn(0, 15, 8, 8, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, -1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 15);
    GXSetTevAlphaIn(1, 6, 7, 5, 7);
    GXSetTevOrder(2, 1, 1, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 6, 4, 0);
    GXSetTevOrder(3, 0xFF, 0xFF, -1);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevColorIn(3, 15, 15, 15, 0);
    GXSetTevAlphaIn(3, 7, 5, 0, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    PSMTXScale(scale, 0.0078125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXLoadTexMtxImm(scale, 0x21, 1);
    effGetTexObj(0x54, &tex);
    GXLoadTexObj(&tex, 0);
    GXLoadTexObj(&tex, 1);
    GXSetCullMode(0);

    type = *(s32*)work;
    frame = *(s32*)(work + 0x2C) - 1;
    if (type == 2) {
        if (frame < 8) {
            pattern = yuragi_data + frame * 0x18;
        } else if (frame < 0x24) {
            pattern = yuragi_data + (frame % 6 + 2) * 0x18;
        } else {
            pattern = yuragi_data + 0xF0;
        }
    } else if (frame < 0xB) {
        pattern = yuragi_data_table[type] + frame * 0x18;
    } else {
        pattern = yuragi_data_table[type] + 0xF0;
    }

    /* Build the twelve-point center line before expanding it to a ribbon. */
    for (i = 0; i < 12; i++) {
        f32 t = (f32)i / 11.0f;
        f32 phase = float_6p2832_80425db8 *
            (f32)(*(s32*)(work + 0x110) + (i - *(s32*)(work + 0x2C)) * 10 - 60) /
            float_360_80425dbc;
        f32 amount = float_0p04_80425dc0 * (f32)pattern[i];
        f32 waveX = amount * (f32)cos(phase);
        f32 waveY = amount * (f32)sin(phase);
        *(f32*)(work + 0x50 + i * 4) = *(f32*)(work + 0x10) +
            t * (*(f32*)(work + 0x1C) - *(f32*)(work + 0x10)) + waveX;
        *(f32*)(work + 0x80 + i * 4) = *(f32*)(work + 0x14) +
            t * (*(f32*)(work + 0x20) - *(f32*)(work + 0x14)) + waveY;
        *(f32*)(work + 0xB0 + i * 4) = *(f32*)(work + 0x18) +
            t * (*(f32*)(work + 0x24) - *(f32*)(work + 0x18));
        *(f32*)(work + 0xE0 + i * 4) = float_0p1_80425da4 * (f32)pattern[i + 12];
    }

    vertices = smartAlloc(0x2A0, 3);
    {
        s16* dst = (s16*)vertices;
        s16 texS = 0;
        u8 texT = 0;
        u8 texT2 = 0;
        for (i = 0; i < 12; i++) {
            f32 angle;
            f32 width;
            f32 sideX;
            f32 sideY;
            if (i == 0) {
                angle = -(f32)angleABf(*(f32*)(work + 0x54), -*(f32*)(work + 0x84),
                                       *(f32*)(work + 0x50), -*(f32*)(work + 0x80));
            } else if (i == 11) {
                angle = float_neg90_80425dc8;
            } else {
                f32 a = -(f32)angleABf(*(f32*)(work + 0x54 + i * 4), -*(f32*)(work + 0x84 + i * 4),
                                       *(f32*)(work + 0x50 + i * 4), -*(f32*)(work + 0x80 + i * 4));
                f32 b = -(f32)angleABf(*(f32*)(work + 0x50 + i * 4), -*(f32*)(work + 0x80 + i * 4),
                                       *(f32*)(work + 0x4C + i * 4), -*(f32*)(work + 0x7C + i * 4));
                if (b - a > float_180_80425dcc) a += float_360_80425dbc;
                if (b - a < float_neg180_80425dd0) b += float_360_80425dbc;
                angle = (a + b) * float_0p5_80425dd4;
            }
            width = float_8_80425dc4 * *(f32*)(work + 0x4C) * *(f32*)(work + 0xE0 + i * 4);
            sideX = width * (f32)sin(float_6p2832_80425db8 * angle / float_360_80425dbc);
            sideY = width * (f32)cos(float_6p2832_80425db8 * angle / float_360_80425dbc);
            dst[0] = (s16)(float_10_80425dd8 * (*(f32*)(work + 0x50 + i * 4) + sideX));
            dst[1] = (s16)(float_10_80425dd8 * (*(f32*)(work + 0x80 + i * 4) + sideY));
            dst[2] = (s16)(float_10_80425dd8 * *(f32*)(work + 0xB0 + i * 4));
            dst[3] = (s16)((128 - i * 12) * 32);
            dst[4] = 0;
            ((u8*)dst)[10] = (u8)texS;
            ((u8*)dst)[11] = texT;
            ((u8*)dst)[12] = texT2;
            dst[7] = (s16)(float_10_80425dd8 * (*(f32*)(work + 0x50 + i * 4) - sideX));
            dst[8] = (s16)(float_10_80425dd8 * (*(f32*)(work + 0x80 + i * 4) - sideY));
            dst[9] = (s16)(float_10_80425dd8 * *(f32*)(work + 0xB0 + i * 4));
            dst[10] = dst[3];
            dst[11] = 0x400;
            ((u8*)dst)[24] = (u8)texS;
            ((u8*)dst)[25] = texT;
            ((u8*)dst)[26] = texT2;
            dst += 14;
            texS += 50;
            texT += 120;
            texT2 += 30;
        }
    }
    DCFlushRange(vertices, 0x2A0);
    GXInvalidateVtxCache();
    effSetVtxDescN64(vertices);
    for (i = 0; i < 11; i++) {
        GXBegin(0x90, 0, 6);
        tri2((s16)(i * 2), (s16)(i * 2 + 2), (s16)(i * 2 + 1), 2,
             (s16)(i * 2 + 1), (s16)(i * 2 + 2), (s16)(i * 2 + 3), 0);
    }
    if (*(s32*)(work + 0x118) != 4) {
        GXSetProjectionv(projection);
        GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    }
    return 0;
}
