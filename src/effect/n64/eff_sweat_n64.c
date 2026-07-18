#include "effect/n64/eff_sweat_n64.h"


void effSweatDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 trans[3][4], rot[3][4], base[3][4], mtx[3][4];
    u8 tex[0x20];
    s32 i;
    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, -0.017453292f * *(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(base, rot, base);
    PSMTXConcat((u8*)camGetPtr(cameraId) + 0x11C, base, base);
    GXSetNumChans(1); GXSetNumTexGens(1); GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1); GXSetTevOrder(0, 0, 0, 4); GXSetCullMode(0);
    effGetTexObj(0x5D, tex); GXLoadTexObj(tex, 0);
    GXClearVtxDesc(); GXSetVtxDesc(9, 1); GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0); GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        u8* part = work + i * 0x34;
        if (*(s32*)(part + 0x2C) == 0) {
            PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
            PSMTXRotRad(rot, 0x7A, 0.017453292f * *(f32*)(part + 0x10));
            PSMTXConcat(trans, rot, mtx); PSMTXConcat(base, mtx, mtx);
            GXLoadPosMtxImm(mtx, 0); GXSetCurrentMtx(0); GXBegin(0x80, 0, 4);
        }
    }
}

void* effSweatN64Entry(
    f32 x, f32 y, f32 z, f32 size, f32 angle, s32 type, s32 delay) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effSweatMain(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 irand(s32);
    extern char str_SweatN64_802fc150[];
    void* effect = effEntry();
    u8* work;
    s32 count = 1;
    s32 i;
    f32 radius = size + 16.0f;

    if (type == 1) {
        count = 5;
    } else if (type == 0) {
        count = 7;
    }

    *(char**)((s32)effect + 0x14) = str_SweatN64_802fc150;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x34);
    *(u8**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effSweatMain;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;

    for (i = 1; i < count; i++) {
        u8* part = work + i * 0x34;
        f32 offset = 0.0f;
        f32 particleAngle;
        f32 radians;
        f32 sine;
        f32 cosine;

        if (type == 1) {
            offset = (f32)((i - 1) & 3) * 16.0f;
        } else if (type == 0) {
            offset = (f32)((i - 1) % 3) * 14.0f;
        }
        if (angle > 0.0f) {
            offset = -offset;
        }
        particleAngle = angle + offset;
        radians = -0.017453292f * particleAngle;
        sine = (f32)sin(radians);
        cosine = (f32)cos(radians);

        *(f32*)(part + 4) = sine * radius;
        *(f32*)(part + 8) = cosine * radius;
        *(f32*)(part + 0xC) = 0.0f;
        *(f32*)(part + 0x10) = particleAngle;
        *(f32*)(part + 0x18) = sine;
        *(f32*)(part + 0x1C) = cosine;
        *(f32*)(part + 0x20) = 0.0f;
        *(s32*)(part + 0x24) = delay;
        *(s32*)(part + 0x28) = 0;
        *(s32*)(part + 0x2C) = irand(10);
        *(s32*)(part + 0x30) = 0;
        if (type == 0 && i > 3) {
            *(s32*)(part + 0x2C) += 0x10;
        }
    }
    return effect;
}

void effSweatMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effSweatDisp(void);
    extern const Vec3 vec3_802fc144;
    extern f32 float_0p94_8042634c;
    extern f32 float_0p05_80426350;
    extern f32 float_neg0p1_80426354;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 dead;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc144;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    dead = 0;
    part = work + 0x34;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        if (*(s32*)(part + 0x2C) != 0) {
            *(s32*)(part + 0x2C) -= 1;
            continue;
        }
        *(f32*)(part + 4) += *(f32*)(part + 0x18);
        *(f32*)(part + 8) += *(f32*)(part + 0x1C);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x20);
        *(f32*)(part + 0x18) *= float_0p94_8042634c;
        *(f32*)(part + 0x1C) -= float_0p05_80426350;
        if (*(f32*)(part + 0x1C) < float_neg0p1_80426354) {
            *(f32*)(part + 0x1C) = float_neg0p1_80426354;
        }
        *(f32*)(part + 0x20) *= float_0p94_8042634c;
        if (*(s32*)(part + 0x28) < 8) {
            *(s32*)(part + 0x30) = (*(s32*)(part + 0x28) << 5) + 0x1F;
        }
        if (*(s32*)(part + 0x24) < 0x10) {
            *(s32*)(part + 0x30) = *(s32*)(part + 0x24) << 4;
        }
        if (*(s32*)(part + 0x30) > 0xFF) {
            *(s32*)(part + 0x30) = 0xFF;
        }
        if (*(s32*)(part + 0x30) < 0) {
            *(s32*)(part + 0x30) = 0;
        }
        *(s32*)(part + 0x28) += 1;
        *(s32*)(part + 0x24) -= 1;
        if (*(s32*)(part + 0x24) < 0) {
            dead++;
        }
    }
    if (dead >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effSweatDisp, effect, dispCalcZ(&dispPos));
    }
}

