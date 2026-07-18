#include "effect/n64/eff_hit_n64.h"


void effHitDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void PSMTXRotAxisRad(Mtx, f32*, f32);
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
    extern void GXSetCullMode(s32);
    extern void main_dl(void*, Mtx);
    extern f32 float_5_80425458;
    extern f32 float_deg2rad_80425470;
    extern f32 vec3_802fb0ac[];
    u8 texObj[0x20];
    Mtx base;
    Mtx rotation;
    Mtx model;
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* camera3d;
    f32 frameValue = float_5_80425458 * *(f32*)(work + 0x38);
    s32 frame = (s32)frameValue;
    s32 i;
    u32 color0;
    u32 color1;

    if (frame >= 5) {
        return;
    }
    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    camera3d = (u8*)camGetPtr(4);
    PSMTXRotRad(rotation, float_deg2rad_80425470 * -*(f32*)(camera3d + 0x114), 'y');
    PSMTXScale(model, *(f32*)(work + 0x28), *(f32*)(work + 0x28), *(f32*)(work + 0x28));
    PSMTXConcat(base, rotation, base);
    PSMTXConcat(base, model, base);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 12, 0, 15);
    GXSetTevAlphaIn(1, 0, 4, 6, 7);
    color0 = ((u32)*(u8*)(work + 0x3C) << 24) |
             ((u32)*(u8*)(work + 0x3D) << 16) |
             ((u32)*(u8*)(work + 0x3E) << 8) |
             *(u8*)(work + 0x2C);
    color1 = ((u32)*(u8*)(work + 0x3F) << 24) |
             ((u32)*(u8*)(work + 0x40) << 16) |
             ((u32)*(u8*)(work + 0x41) << 8) |
             (u8)(256.0f * (frameValue - (f32)frame));
    GXSetTevColor(1, &color0);
    GXSetTevColor(2, &color1);
    effGetTexObjN64(0x4A, texObj);
    GXLoadTexObj(texObj, 0);
    GXLoadTexObj(texObj, 1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetCullMode(0);
    for (i = 0; i < 3; i++) {
        PSMTXRotAxisRad(rotation, vec3_802fb0ac, float_deg2rad_80425470 * (f32)(i * 120));
        PSMTXConcat(base, rotation, model);
        main_dl(effect, model);
    }
}

void* effHitN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effHitMain(void*);
    extern s32 rand(void);
    extern char str_HitN64_802fb0c8[];
    extern u8 cnt_table[];
    extern f32 float_0_80425468;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 n;

    entry = effEntry();
    n = ((count * 5 + 0x14) / 3) + 1;
    *(char**)((s32)entry + 0x14) = str_HitN64_802fb0c8;
    *(s32*)((s32)entry + 8) = n;
    work = __memAlloc(3, n * 0x44);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHitMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x30) = cnt_table[type];
    *(s32*)(work + 0x34) = 0;
    *(f32*)(work + 0x38) = float_0_80425468;

    switch (type) {
        case 0:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0xFF;
            *(u8*)(work + 0x3F) = 0xFF;
            *(u8*)(work + 0x40) = 0xFF;
            *(u8*)(work + 0x41) = 0xFF;
            break;
        case 1:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0;
            *(u8*)(work + 0x3F) = 0xFF;
            *(u8*)(work + 0x40) = 0;
            *(u8*)(work + 0x41) = 0;
            break;
        case 2:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0;
            *(u8*)(work + 0x3F) = 0xC8;
            *(u8*)(work + 0x40) = 0xFF;
            *(u8*)(work + 0x41) = 0xE6;
            break;
        default:
            *(u8*)(work + 0x3C) = 0;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0xFF;
            *(u8*)(work + 0x3F) = 0;
            *(u8*)(work + 0x40) = 0;
            *(u8*)(work + 0x41) = 0xE6;
            break;
    }
    *(f32*)(work + 0x28) = scale;

    part = work + 0x44;
    for (i = 1; i < n; i++) {
        *(f32*)(part + 4) = float_0_80425468;
        *(f32*)(part + 8) = float_0_80425468;
        *(f32*)(part + 0xC) = float_0_80425468;
        *(f32*)(part + 0x10) = float_0_80425468;
        *(f32*)(part + 0x14) = float_0_80425468;
        *(f32*)(part + 0x20) = float_0_80425468;
        *(s32*)part = 0;
        *(f32*)(part + 0x1C) = (f32)(0x168 / (n - 1));
        *(f32*)(part + 0x24) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x10) = float_0_80425468;
        *(s32*)(part + 0x2C) = 0xFF;
        part += 0x44;
    }

    return entry;
}

void main_dl(void* effect, f32 model[3][4]) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_5_80425458;
    extern f32 float_4_8042545c;
    extern f32 float_0p03125_80425460;
    extern f32 float_0p0078125_80425464;
    extern f32 float_0_80425468;
    extern f32 float_96_8042546c;
    extern f32 float_deg2rad_80425470;
    extern u8 size32x32_tex32x32_vtx[];
    u8* entry = (u8*)effect;
    u8* part = *(u8**)(entry + 0xC);
    Mtx texMtx;
    Mtx trans;
    Mtx rotY;
    Mtx rotZ;
    Mtx draw;
    s32 frame = (s32)(float_5_80425458 * *(f32*)(part + 0x38));
    s32 i;

    PSMTXScale(texMtx, float_0p03125_80425460, float_0p0078125_80425464, float_0_80425468);
    PSMTXTrans(trans, float_0_80425468, float_96_8042546c, float_0_80425468);
    PSMTXConcat(texMtx, trans, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    PSMTXScale(texMtx, float_0p03125_80425460, float_0p0078125_80425464, float_0_80425468);
    GXLoadTexMtxImm(texMtx, 0x21, 1);
    if (float_5_80425458 * *(f32*)(part + 0x38) >= float_4_8042545c) {
        PSMTXScale(texMtx, float_0_80425468, float_0_80425468, float_0_80425468);
        GXLoadTexMtxImm(texMtx, 0x21, 1);
    }
    part += 0x44;
    for (i = 1; i < *(s32*)(entry + 8); i++, part += 0x44) {
        u8* camera;
        PSMTXRotRad(rotY, float_deg2rad_80425470 * *(f32*)(part + 0x24), 'y');
        PSMTXRotRad(rotZ, float_deg2rad_80425470 * *(f32*)(part + 0x1C), 'z');
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXConcat(rotY, rotZ, rotZ);
        PSMTXConcat(rotZ, trans, trans);
        PSMTXConcat(model, trans, draw);
        camera = (u8*)camGetPtr(4);
        PSMTXConcat((f32 (*)[4])(camera + 0x11C), draw, draw);
        GXLoadPosMtxImm(draw, 0);
        GXSetCurrentMtx(0);
        effSetVtxDescN64(size32x32_tex32x32_vtx + frame * 0x38);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}

void effHitMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effHitDisp(void);
    extern Vec3 vec3_802fb0a0[];
    extern u8 cnt_table[];
    extern u8 anim_data[];
    extern u8 radius_data[];
    extern f32 float_0p0039062_80425478;
    extern f32 float_1_8042547c;
    extern f32 float_0_80425468;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 frame;
    s32 type;
    s32 idx;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb0a0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    type = *(s32*)work;
    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x34) += 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x34);
    if (frame <= cnt_table[type]) {
        *(f32*)(work + 0x38) = (f32)anim_data[frame - 1] * float_0p0039062_80425478;
    } else {
        *(f32*)(work + 0x38) = float_1_8042547c;
    }

    part = work + 0x44;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (frame > cnt_table[type]) {
            idx = 0xC;
        } else {
            idx = frame - 1;
        }
        *(f32*)(part + 4) = (f32)radius_data[idx];
        *(f32*)(part + 8) = float_0_80425468;
        *(f32*)(part + 0xC) = float_0_80425468;
        part += 0x44;
    }
    dispEntry(4, 2, effHitDisp, effect, dispCalcZ(&dispPos));
}

