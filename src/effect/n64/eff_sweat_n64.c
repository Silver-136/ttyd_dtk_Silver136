#include "effect/n64/eff_sweat_n64.h"

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


void effSweatDisp(s32 cameraId, void* effect) {
    typedef union SweatColor {
        u32 word;
        u8 chan[4];
    } SweatColor;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern u32 dat_80426328;
    extern f32 float_9p6_80426330;
    extern f32 float_6p4_80426334;
    extern f32 float_neg1_80426338;
    extern f32 float_0_8042633c;
    extern f32 float_0p5_80426340;
    extern f32 float_4p8_80426344;
    extern f32 float_1_80426348;
    u8* camera = (u8*)camGetPtr(cameraId);
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 trans[3][4];
    f32 rot[3][4];
    f32 base[3][4];
    u8 tex[0x20];
    f32 halfHeight;
    s32 i;

    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, -0.017453292f * *(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(base, rot, trans);
    PSMTXConcat(camera + 0x11C, trans, trans);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    effGetTexObj(0x5D, tex);
    GXLoadTexObj(tex, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    halfHeight = -float_9p6_80426330;
    work += 0x34;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work += 0x34) {
        if (*(s32*)(work + 0x2C) == 0) {
            SweatColor color;
            volatile f32* fifo = (volatile f32*)0xCC008000;
            f32 right = float_6p4_80426334;
            f32 left;
            f32 top = float_4p8_80426344;
            f32 bottom;

            color.word = dat_80426328;
            color.chan[3] = *(u8*)(work + 0x30);
            GXSetChanMatColor(4, &color);
            PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
            PSMTXRotRad(rot, 0x7A, 0.017453292f * *(f32*)(work + 0x10));
            PSMTXConcat(base, rot, base);
            PSMTXConcat(trans, base, base);
            GXLoadPosMtxImm(base, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);
            if (*(f32*)(work + 0x10) > float_0_8042633c) {
                right *= float_neg1_80426338;
            }
            left = -right * float_0p5_80426340;
            right *= float_0p5_80426340;
            bottom = halfHeight * float_0p5_80426340;
            *fifo = left; *fifo = top; *fifo = float_0_8042633c; *fifo = float_0_8042633c; *fifo = float_0_8042633c;
            *fifo = right; *fifo = top; *fifo = float_0_8042633c; *fifo = float_1_80426348; *fifo = float_0_8042633c;
            *fifo = right; *fifo = bottom; *fifo = float_0_8042633c; *fifo = float_1_80426348; *fifo = float_1_80426348;
            *fifo = left; *fifo = bottom; *fifo = float_0_8042633c; *fifo = float_0_8042633c; *fifo = float_1_80426348;
        }
    }
}

const f32 vec3_802fc138[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802fc144[3] = { 0.0f, 0.0f, 0.0f };
const char str_SweatN64_802fc150[] = "SweatN64";
