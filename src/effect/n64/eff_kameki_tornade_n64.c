#include "effect/n64/eff_kameki_tornade_n64.h"


u8 effKamekiTornadeDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void* smartAlloc(u32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s16);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425560;
    extern f32 float_0p1_80425564;
    extern f32 float_0p02_80425568;
    extern f32 float_10_8042556c;
    extern f32 float_0_80425570;
    extern f32 float_0p015625_80425574;
    extern f32 float_0p03125_80425578;
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* camera = (u8*)camGetPtr(cameraId);
    u8 tex[0x20];
    Mtx base;
    Mtx transform;
    Mtx rotation;
    Mtx scale;
    u32 color0;
    u32 color1;
    s32 alpha = (s32)((f32)(*(s32*)(work + 0x24) >> 1) * *(f32*)(work + 0x148));
    s32 strand;

    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 8, 0, 0, 15);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevOrder(2, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 2, 4, 0, 15);
    GXSetTevAlphaIn(2, 7, 0, 5, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    effGetTexObjN64(0x6C, tex);
    GXLoadTexObj(tex, 0);
    GXLoadTexObj(tex, 1);
    color0 = ((u32)*(u8*)(work + 0x18) << 24) | ((u32)*(u8*)(work + 0x1C) << 16) |
             ((u32)*(u8*)(work + 0x20) << 8) | (u8)alpha;
    color1 = ((u32)*(u8*)(work + 0x28) << 24) | ((u32)*(u8*)(work + 0x2C) << 16) |
             ((u32)*(u8*)(work + 0x30) << 8) | 0xFF;
    GXSetTevColor(1, &color0);
    GXSetTevColor(2, &color1);
    GXSetCullMode(0);
    PSMTXRotRad(rotation, float_deg2rad_80425560 * *(f32*)(work + 0x144), 'z');
    PSMTXScale(scale, float_0p1_80425564, float_0p1_80425564, float_0p1_80425564);
    PSMTXConcat(rotation, scale, base);
    for (strand = 0; strand < 7; strand++) {
        s32 segment;
        void* vertices = smartAlloc(0x1C0, 3);
        f32 strandScale = *(f32*)(work + 0x98 + strand * 4) * *(f32*)(work + 0x34);
        effSetVtxDescN64(vertices);
        PSMTXTrans(transform,
                   float_10_8042556c * *(f32*)(work + 0x38 + strand * 4),
                   float_10_8042556c * *(f32*)(work + 0x58 + strand * 4),
                   float_10_8042556c * *(f32*)(work + 0x78 + strand * 4));
        PSMTXRotRad(rotation, float_deg2rad_80425560 * *(f32*)(work + 0x118 + strand * 4), 'z');
        PSMTXConcat(transform, rotation, transform);
        PSMTXScale(scale, strandScale, *(f32*)(work + 0x34), strandScale);
        PSMTXConcat(transform, scale, transform);
        PSMTXConcat(base, transform, transform);
        PSMTXScale(scale, float_0p015625_80425574, float_0p03125_80425578, float_0_80425570);
        PSMTXTrans(rotation, *(f32*)(work + 0x13C) + (f32)(strand * 4), float_0_80425570, float_0_80425570);
        PSMTXConcat(scale, rotation, scale);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        PSMTXScale(scale, float_0p015625_80425574, float_0p03125_80425578, float_0_80425570);
        PSMTXTrans(rotation, *(f32*)(work + 0x140) + (f32)(strand * 2), float_0_80425570, float_0_80425570);
        PSMTXConcat(scale, rotation, scale);
        GXLoadTexMtxImm(scale, 0x21, 1);
        GXLoadPosMtxImm((f32 (*)[4])(camera + 0x11C), 0);
        GXSetCurrentMtx(0);
        for (segment = 0; segment < 15; segment++) {
            GXBegin(0x90, 0, 6);
            tri2(segment + 0x10, segment + 1, segment, 0,
                 segment + 1, segment + 0x10, segment + 0x11);
        }
    }
    return 0;
}

#pragma optimize_for_size off

void* effKamekiTornadeN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKamekiTornadeMain(void*);
    extern s32 rand(void);
    extern char str_KamekiTornadeN64_802fb198[];
    extern f32 float_0p2_8042559c;
    extern f32 float_1_80425598;
    extern f32 float_0p1_80425564;
    extern f32 float_2_804255a0;
    extern f32 float_0_80425570;
    void* entry;
    u8* work;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_KamekiTornadeN64_802fb198;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x14C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKamekiTornadeMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x34) = scale;
    *(s32*)(work + 0x18) = 0xFF;
    *(s32*)(work + 0x1C) = 0xFF;
    *(s32*)(work + 0x20) = 0xFF;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x2C) = 0xFF;
    *(s32*)(work + 0x30) = 0xEB;

    for (i = 0; i < 8; i++) {
        *(f32*)(work + 0x38 + i * 4) = x;
        *(f32*)(work + 0x58 + i * 4) = y;
        *(f32*)(work + 0x78 + i * 4) = z;
        *(f32*)(work + 0x98 + i * 4) = float_1_80425598 + float_0p2_8042559c * (f32)i;
        *(f32*)(work + 0xB8 + i * 4) = (f32)(rand() % 0x168);
        *(f32*)(work + 0xD8 + i * 4) = float_2_804255a0 + float_0p1_80425564 * (f32)(rand() % 100);
        *(f32*)(work + 0xF8 + i * 4) = float_0p1_80425564 * (f32)(rand() % 30);
        *(f32*)(work + 0x118 + i * 4) = float_0_80425570;
    }
    *(f32*)(work + 0x138) = float_0_80425570;
    *(f32*)(work + 0x144) = float_0_80425570;
    *(f32*)(work + 0x13C) = float_0_80425570;
    *(f32*)(work + 0x140) = float_0_80425570;
    *(f32*)(work + 0x148) = float_1_80425598;

    return entry;
}

#pragma optimize_for_size on

void effKamekiTornadeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKamekiTornadeDisp(void);
    extern Vec3 vec3_802fb168[];
    extern f32 float_4_80425580;
    extern f32 float_5_8042557c;
    extern f32 float_10_8042556c;
    extern f32 float_12p96_8042558c;
    extern f32 float_17p28_80425590;
    extern f32 float_64_80425594;
    extern f32 float_90_80425584;
    extern f32 float_neg90_80425588;
    u8* work;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    f32 scale;
    f32 delta;
    f32 angle;
    s32 i;
    s32 timer;
    s32 frame;
    u8* p;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb168;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(u32*)effect & 4) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    scale = *(f32*)(work + 0x34);
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    frame = *(s32*)(work + 0x14);
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }

    *(f32*)(work + 0x38) = *(f32*)(work + 4);
    *(f32*)(work + 0x58) = *(f32*)(work + 8);
    *(f32*)(work + 0x78) = *(f32*)(work + 0xC);
    p = work + 0x1C;
    for (i = 0; i < 7; i++) {
        delta = (*(f32*)(p + 0x34) - *(f32*)(p + 0x38)) * float_4_80425580;
        *(f32*)(p + 0x118) = delta;
        *(f32*)(p + 0x38) = *(f32*)(p + 0x34);
        *(f32*)(p + 0x58) = *(f32*)(p + 0x54) + float_5_8042557c * scale;
        *(f32*)(p + 0x78) = *(f32*)(p + 0x74);
        angle = *(f32*)(p + 0x118);
        if (angle > float_90_80425584) {
            *(f32*)(p + 0x118) = float_90_80425584;
        } else if (angle < float_neg90_80425588) {
            *(f32*)(p + 0x118) = float_neg90_80425588;
        }
        p -= 4;
    }

    *(f32*)(work + 0x138) += float_10_8042556c;
    *(f32*)(work + 0x13C) += float_12p96_8042558c;
    *(f32*)(work + 0x140) += float_17p28_80425590;
    if (*(f32*)(work + 0x13C) > float_64_80425594) {
        *(f32*)(work + 0x13C) -= float_64_80425594;
    }
    if (*(f32*)(work + 0x140) > float_64_80425594) {
        *(f32*)(work + 0x140) -= float_64_80425594;
    }

    *(f32*)(work + 0xB8) += *(f32*)(work + 0xD8);
    *(f32*)(work + 0xBC) += *(f32*)(work + 0xDC);
    *(f32*)(work + 0xC0) += *(f32*)(work + 0xE0);
    *(f32*)(work + 0xC4) += *(f32*)(work + 0xE4);
    *(f32*)(work + 0xC8) += *(f32*)(work + 0xE8);
    *(f32*)(work + 0xCC) += *(f32*)(work + 0xEC);
    *(f32*)(work + 0xD0) += *(f32*)(work + 0xF0);
    *(f32*)(work + 0xD4) += *(f32*)(work + 0xF4);
    dispEntry(4, 2, effKamekiTornadeDisp, effect, dispCalcZ(&dispPos));
}

