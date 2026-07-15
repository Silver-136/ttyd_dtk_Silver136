#include "effect/n64/eff_kameki_tornade_n64.h"


u8 effKamekiTornadeDisp(s32 cameraId, s32 effectAddress) {
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
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    f32* points = smartAlloc(0x300, 3);
    s32 strand;
    s32 segment;

    effGetTexObj(0x68, &tex);
    GXLoadTexObj(&tex, 0);
    GXLoadTexObj(&tex, 1);
    GXSetNumChans(0);
    GXSetNumTevStages(3);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevOrder(1, 1, 1, -1);
    GXSetTevOrder(2, -1, -1, -1);
    GXSetCullMode(0);
    for (strand = 0; strand < 7; strand++) {
        for (segment = 0; segment < 16; segment++) {
            s32 index = strand * 48 + segment * 3;
            points[index] = *(f32*)(work + 0x38 + strand * 4);
            points[index + 1] = *(f32*)(work + 0x58 + strand * 4) + (f32)segment;
            points[index + 2] = *(f32*)(work + 0x78 + strand * 4);
        }
        PSMTXTrans(trans, *(f32*)(work + 0x38 + strand * 4), *(f32*)(work + 0x58 + strand * 4), *(f32*)(work + 0x78 + strand * 4));
        PSMTXRotRad(rot, *(f32*)(work + 0x118 + strand * 4) * 0.017453292f, 'z');
        PSMTXScale(scale, *(f32*)(work + 0x98 + strand * 4), *(f32*)(work + 0x98 + strand * 4), 1.0f);
        PSMTXConcat(trans, rot, model);
        PSMTXConcat(model, scale, model);
        PSMTXConcat(camera + 0x118, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
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

