#include "effect/n64/eff_thunderflare_n64.h"

extern f32 float_0_80426408;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderflareMain(void*);
    extern char str_ThunderflareN64_802fc1d0[];
    void* entry;
    u8* work;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderflareN64_802fc1d0;
    *(s32*)((s32)entry + 8) = 0x1F;
    work = __memAlloc(3, 0x8B8);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderflareMain;
    *(s32*)(work + 4) = type;
    *(s32*)(work + 0x1C) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x18) = 0x64;
    } else {
        *(s32*)(work + 0x18) = lifetime;
    }
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (type == 1) {
        goto color_other;
    }
    if (type >= 1) {
        goto color_other;
    }
    if (type >= 0) {
        goto color_zero;
    }
    goto color_other;
color_zero:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x38) = 0xFF;
    *(s32*)(work + 0x3C) = 0xFF;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    goto color_done;
color_other:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0x14;
    *(s32*)(work + 0x38) = 0x21;
    *(s32*)(work + 0x3C) = 0x32;
    *(s32*)(work + 0x40) = 0x14;
    *(s32*)(work + 0x44) = 0xA;
color_done:
    part = work + 0x48;
    for (i = 1; i < 0x1F; i += 2, part += 0x90) {
        *(f32*)(part + 0x20) = (f32)(((i - 1) * 0xCA8) / 0xF);
        *(f32*)(part + 0x24) = float_0_80426408;
        *(f32*)(part + 0x2C) = (f32)(((i - 1) * 0x64) / 0xF);
        *(s32*)part = 0;
        *(f32*)(part + 0x68) = (f32)((i * 0xCA8) / 0xF);
        *(f32*)(part + 0x6C) = float_0_80426408;
        *(f32*)(part + 0x74) = (f32)((i * 0x64) / 0xF);
        *(s32*)(part + 0x48) = 0;
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset



void effThunderflareMain(void* effect) {
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effThunderflareDisp(void);
    extern f32 float_0p5_8042640c;
    extern f32 float_6p2832_80426410;
    extern f32 float_360_80426414;
    extern f32 float_2_80426418;
    extern f32 float_255_8042641c;
    extern f32 float_100_80426420;
    extern f32 float_0p2_80426424;
    extern f32 float_80_80426428;
    extern const f32 vec3_802fc1b8[3];
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* particle = work;
    f32 position[3];
    s32 frame;
    s32 odd;
    s32 i;
    f32 zero;
    f32 tau;
    f32 degrees;
    f32 two;
    f32 maxAlpha;
    f32 maxRadius;
    f32 baseScale;
    f32 radiusScale;

    ((u32*)position)[0] = ((const u32*)vec3_802fc1b8)[0];
    ((u32*)position)[1] = ((const u32*)vec3_802fc1b8)[1];
    ((u32*)position)[2] = ((const u32*)vec3_802fc1b8)[2];
    position[0] = *(f32*)(work + 8);
    position[1] = *(f32*)(work + 0xC);
    position[2] = *(f32*)(work + 0x10);
    if (*(s32*)(work + 0x18) < 100) {
        *(s32*)(work + 0x18) -= 1;
    }
    *(s32*)(work + 0x1C) += 1;
    if (*(s32*)(work + 0x18) < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x1C);
    if (frame < 0x15) {
        *(s32*)(work + 0x14) = frame * 12;
    } else {
        *(s32*)(work + 0x14) = 0xFF;
    }
    if (frame & 1) {
        *(s32*)(work + 0x14) =
            (s32)(float_0p5_8042640c * (f32)*(s32*)(work + 0x14));
    }
    odd = frame & 1;

    zero = float_0_80426408;
    tau = float_6p2832_80426410;
    degrees = float_360_80426414;
    two = float_2_80426418;
    maxAlpha = float_255_8042641c;
    maxRadius = float_100_80426420;
    baseScale = float_0p2_80426424;
    radiusScale = float_80_80426428;

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, particle += 0x48) {
        f32 radius = *(f32*)(particle + 0x74);
        f32 angleA = tau * *(f32*)(particle + 0x68) / degrees;
        f32 angleB = tau * *(f32*)(particle + 0x6C) / degrees;
        f32 sinA = (f32)sin(angleA);
        f32 cosA = (f32)cos(angleA);
        f32 sinB = (f32)sin(angleB);
        f32 cosB = (f32)cos(angleB);
        s32 stepA;
        s32 stepB;

        *(f32*)(particle + 0x50) = cosB * radius * sinA;
        *(f32*)(particle + 0x54) = cosB * radius * cosA;
        *(f32*)(particle + 0x58) = radius * sinB;
        *(s32*)(particle + 0x5C) =
            (s32)((two * maxAlpha * (maxRadius - radius)) / maxRadius);
        if (*(s32*)(particle + 0x5C) > 0xFF) {
            *(s32*)(particle + 0x5C) = 0xFF;
        }
        *(f32*)(particle + 0x70) =
            baseScale + radius / radiusScale;
        radius -= (f32)(((i & 3) * 2) + 2);
        *(f32*)(particle + 0x74) = radius;
        if (radius < zero) {
            *(f32*)(particle + 0x74) = zero;
            *(f32*)(particle + 0x70) = zero;
        }
        stepA = odd ? 5 : 2;
        stepB = odd ? 2 : 5;
        *(f32*)(particle + 0x68) += (f32)stepA;
        *(f32*)(particle + 0x6C) += (f32)stepB;
    }
    dispEntry(4, 2, effThunderflareDisp, effect, dispCalcZ(position));
}

void effThunderflareDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80426400;
    extern f32 float_0p03125_80426404;
    extern f32 float_0_80426408;
    extern u32 unk_80429850;
    extern u8 size16x16_tex32x32_vtx[];
    u8* camera;
    u8* work;
    u8* particle;
    f32 base[3][4];
    f32 view[3][4];
    f32 scale[3][4];
    f32 rot[3][4];
    u8 tex[0x20];
    u32 color;
    u32 particleColor;
    f32 zero;
    f32 radius;
    s32 i;
    u16 vtx;

    camera = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    PSMTXTrans(base, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 'y', -0.017453292f * *(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(base, rot, base);
    PSMTXConcat(camera + 0x11C, base, view);

    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 0xF);
    GXSetTevAlphaIn(1, 7, 5, 0, 7);
    color = (*(u32*)(work + 0x3C) << 24) | (*(u32*)(work + 0x40) << 16) |
            (*(u32*)(work + 0x44) << 8) | (u8)unk_80429850;
    GXSetTevColor(2, &color);
    effGetTexObjN64(0x55, tex);
    GXLoadTexObj(tex, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, float_0p03125_80426404, float_0p03125_80426404, float_0_80426408);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);

    zero = float_0_80426408;
    particle = work;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, particle += 0x48) {
        radius = *(f32*)(particle + 0x70);
        if (radius != zero) {
            particleColor = (*(u32*)(work + 0x30) << 24) | (*(u32*)(work + 0x34) << 16) |
                            (*(u32*)(work + 0x38) << 8) | *(u32*)(particle + 0x5C);
            GXSetTevColor(1, &particleColor);
            PSMTXTrans(base, *(f32*)(particle + 0x50), *(f32*)(particle + 0x54),
                       *(f32*)(particle + 0x58));
            PSMTXScale(scale, radius, radius, radius);
            PSMTXConcat(base, scale, base);
            PSMTXConcat(view, base, base);
            GXLoadPosMtxImm(base, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x90, 0, 6);
            vtx = (i << 2) & 4;
            tri2(vtx, vtx + 1, vtx + 2, 0, vtx, vtx + 2, vtx + 3);
        }
    }
}

u8 size16x16_tex32x32_vtx[] = {
    0xFF, 0xF8, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0xFA, 0xFA, 0xFF, 0x00, 0x08,
    0xFF, 0xF8, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xFA, 0xFA, 0xFA, 0xFF, 0x00, 0x08, 0x00, 0x08,
    0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFA, 0x00,
    0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0xFA, 0x00, 0x00, 0xFF,
    0xFF, 0xF4, 0xFF, 0xF4, 0x00, 0x00, 0xFE, 0x00,
    0x02, 0x00, 0xFA, 0xFA, 0xFA, 0xFF, 0x00, 0x0C,
    0xFF, 0xF4, 0x00, 0x00, 0x02, 0x00, 0xFE, 0x00,
    0xFA, 0xFA, 0xFA, 0xFF, 0x00, 0x0C, 0x00, 0x0C,
    0x00, 0x00, 0x06, 0x00, 0x02, 0x00, 0xFA, 0x00,
    0x00, 0xFF, 0xFF, 0xF4, 0x00, 0x0C, 0x00, 0x00,
    0x02, 0x00, 0x06, 0x00, 0xFA, 0x00, 0x00, 0xFF,
};

const f32 vec3_802fc1b8[3] = { 0.0f, 0.0f, 0.0f };
const char str_ThunderflareN64_802fc1d0[] = "ThunderflareN64";
