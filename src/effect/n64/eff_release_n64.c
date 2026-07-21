#include "effect/n64/eff_release_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effReleaseN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effReleaseMain(void*);
    extern char str_ReleaseN64_802fbe38[];
    extern f32 float_1_80425ee4;
    extern f32 float_0_80425ec0;
    void* entry;
    u8* work;
    s32 kind;
    s32 color;
    f32 one;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ReleaseN64_802fbe38;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x78);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effReleaseMain;
    kind = type & 0xFF00;
    color = type & 0xFF;
    *(s32*)work = kind;
    *(s32*)(work + 4) = color;
    *(s32*)(work + 0x18) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x14) = 1000;
    } else {
        *(s32*)(work + 0x14) = lifetime;
    }
    one = float_1_80425ee4;
    zero = float_0_80425ec0;
    *(s32*)(work + 0x28) = 0xFF;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    *(f32*)(work + 0x3C) = scale;
    *(s32*)(work + 0x1C) = 0xFF;
    *(s32*)(work + 0x20) = 0xFF;
    *(s32*)(work + 0x24) = 0xFF;
    *(s32*)(work + 0x2C) = 0xFF;
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0xFF;
    *(s32*)(work + 0x38) = 0xFF;
    *(s32*)(work + 0x60) = 0;
    *(s32*)(work + 0x64) = 0;
    *(s32*)(work + 0x68) = 0;
    *(s32*)(work + 0x6C) = 0;
    *(f32*)(work + 0x40) = one;
    *(f32*)(work + 0x44) = zero;
    *(f32*)(work + 0x48) = zero;
    *(f32*)(work + 0x4C) = zero;
    *(s32*)(work + 0x50) = 0xFF;
    *(s32*)(work + 0x54) = 0;
    *(s32*)(work + 0x58) = 0;
    *(s32*)(work + 0x5C) = 0;
    if (color == 3) {
        *(s32*)(work + 0x70) = 1;
    } else {
        *(s32*)(work + 0x70) = 0;
    }
    *(s32*)(work + 0x74) = 0;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effReleaseMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effReleaseDisp(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern Vec3 vec3_802fbe18;
    u8* work = *(u8**)((s32)effect + 0xC);
    Vec3 pos;
    s32 timer;
    s32 frame;
    s32 mode;
    f32 phase;
    f32 pulse;

    pos = vec3_802fbe18;
    pos.x = *(f32*)(work + 8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x14) = 0x10;
    }
    if (*(s32*)(work + 0x14) < 1000) {
        *(s32*)(work + 0x14) -= 1;
    }
    *(s32*)(work + 0x18) += 1;
    timer = *(s32*)(work + 0x14);
    frame = *(s32*)(work + 0x18);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 0x10) {
        *(s32*)(work + 0x28) = timer << 4;
    }

    phase = (6.2832f * (f32)(frame * 12)) / 360.0f;
    pulse = 0.5f + 0.5f * (f32)sin(phase);
    mode = *(s32*)(work + 0x70);
    if (mode == 0) {
        s32 stateTimer = *(s32*)(work + 0x74) + 1;
        *(s32*)(work + 0x74) = stateTimer;
        *(s32*)(work + 0x50) = (frame & 3) * 0x1E + 200;
        *(f32*)(work + 0x40) =
            0.04f * (f32)sin((6.2832f * (f32)(frame * 20)) / 360.0f) + 0.5f;
        if (*(s32*)work != 0) {
            *(f32*)(work + 8) +=
                (f32)sin((6.2832f * (f32)frame) / 360.0f);
            *(f32*)(work + 0xC) +=
                (f32)cos((6.2832f * 1.2356f * (f32)frame) / 360.0f);
            if (stateTimer > 0x32) {
                *(s32*)(work + 0x74) = 0;
                *(s32*)(work + 0x70) = *(s32*)(work + 4) == 3 ? 100 : 1;
            }
        }
    } else if (mode == 0x100) {
        *(f32*)(work + 0x40) = 0.5f + 0.04f * (f32)frame;
        *(f32*)(work + 0x44) = 1.2f * *(f32*)(work + 0x40) + 4.0f;
        *(f32*)(work + 0x48) = pulse;
        *(f32*)(work + 0x4C) = 0.0f;
        *(s32*)(work + 0x50) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x54) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x58) = 0;
    } else if (mode == 0x400) {
        *(f32*)(work + 0x40) = frame < 2 ? 8.0f : (frame == 2 ? 4.0f : 0.0f);
        *(f32*)(work + 0x44) = pulse;
        *(f32*)(work + 0x48) = pulse * 0.75f;
        *(s32*)(work + 0x50) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x54) = *(s32*)(work + 0x28);
    } else {
        *(f32*)(work + 0x40) = pulse;
        *(f32*)(work + 0x44) = 1.0f - pulse;
        *(f32*)(work + 0x48) = pulse * 0.5f;
        *(f32*)(work + 0x4C) = pulse * 0.25f;
        *(s32*)(work + 0x50) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x54) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x58) = *(s32*)(work + 0x28);
        *(s32*)(work + 0x5C) = *(s32*)(work + 0x28);
    }
    if (*(f32*)(work + 0x40) < 0.0f) *(f32*)(work + 0x40) = 0.0f;
    if (*(f32*)(work + 0x44) < 0.0f) *(f32*)(work + 0x44) = 0.0f;
    dispEntry(4, 2, effReleaseDisp, effect, dispCalcZ(&pos));
}


void effReleaseDisp(s32 cameraId, void* effect) {
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
    extern f32 float_deg2rad_80425ebc;
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4], rot[3][4], scale[3][4], base[3][4], model[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 i;
    s32 texture;

    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80425ebc * -*(f32*)((u8*)camGetPtr(4) + 0x114));
    PSMTXScale(scale, *(f32*)(work + 0x3C), *(f32*)(work + 0x3C), *(f32*)(work + 0x3C));
    PSMTXConcat(trans, rot, base);
    PSMTXConcat(base, scale, base);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 3, 2, 8, 0);
    GXSetTevAlphaIn(0, 0, 1, 7, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    color = (*(s32*)(work + 0x2C) << 24) | (*(s32*)(work + 0x30) << 16) |
            (*(s32*)(work + 0x34) << 8) | (*(s32*)(work + 0x38) & 0xFF);
    GXSetTevColor(2, &color);
    GXSetCullMode(0);

    for (i = 0; i < 4; i++) {
        f32 partScale = *(f32*)(work + 0x40 + i * 4);
        s32 alpha = *(s32*)(work + 0x50 + i * 4);
        if ((*(s32*)(work + 4) == 1 && i == 2) || partScale == 0.0f || alpha == 0) continue;
        PSMTXScale(scale, partScale, partScale, partScale);
        PSMTXConcat(base, scale, model);
        PSMTXConcat((u8*)camera + 0x11C, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        color = (*(s32*)(work + 0x1C) << 24) | (*(s32*)(work + 0x20) << 16) |
                (*(s32*)(work + 0x24) << 8) | alpha;
        GXSetTevColor(1, &color);
        texture = 0x44 + i;
        effGetTexObjN64(texture, texObj);
        GXLoadTexObj(texObj, 0);
        PSMTXScale(scale, i == 1 ? 0.00390625f : 0.015625f,
                          i == 2 ? 0.015625f : 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        effSetVtxDescN64(i == 0 ? (void*)0x803A7D30 : (i == 1 ? (void*)0x803A7D68 : (void*)0x803A7F28));
        GXBegin(0x90, 0, i == 1 ? 12 : 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        if (i == 1) tri2(4, 5, 6, 4, 4, 6, 7, 0);
        if (i == 2) {
            effGetTexObjN64(0x47, texObj);
            GXLoadTexObj(texObj, 0);
            PSMTXScale(scale, 0.015625f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, 0x1E, 1);
            effSetVtxDescN64((void*)0x803A7F60);
            GXBegin(0x90, 0, 12);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(4, 5, 6, 4, 4, 6, 7, 0);
        }
    }
}
