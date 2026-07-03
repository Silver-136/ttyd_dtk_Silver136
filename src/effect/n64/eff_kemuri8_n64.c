#include "effect/n64/eff_kemuri8_n64.h"

void effKemuri8N64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x58) = camId;
}


u8 effKemuri8Disp(void) {
    return 0;
}


void* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri8Main(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri8N64_802fb2b0[];
    extern f32 float_14_804257c8;
    extern f32 float_28_804257cc;
    extern f32 float_42_804257d0;
    extern f32 float_neg3_804257d4;
    extern f32 float_0p2_804257d8;
    extern f32 float_0_804257a8;
    extern f32 float_1p75_804257dc;
    extern f32 float_neg0p16_804257e0;
    extern f32 float_6p2832_804257e4;
    extern f32 float_360_804257e8;
    void* entry;
    u8* work;
    u8* part;
    s32 variant;
    s32 count;
    s32 i;
    s32 angleDeg;
    f32 radius;
    f32 rad;

    entry = effEntry();
    variant = 0;
    if (type >= 3) {
        variant = 1;
        type -= 3;
    }
    switch (type) {
        case 0:
            radius = float_14_804257c8;
            count = 7;
            break;
        case 1:
            radius = float_28_804257cc;
            count = 9;
            break;
        default:
            count = 11;
            radius = float_42_804257d0;
            break;
    }

    *(char**)((s32)entry + 0x14) = str_Kemuri8N64_802fb2b0;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x5C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri8Main;
    *(s32*)(work + 0x38) = 0;
    *(s32*)(work + 0x3C) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x40) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x4C) = 0x11;
    *(s32*)(work + 0x58) = 4;

    part = work + 0x5C;
    for (i = 1; i < count; i++, part += 0x5C) {
        *(f32*)(part + 0x10) = float_neg3_804257d4;
        *(f32*)(part + 0x14) = float_0p2_804257d8;
        *(f32*)(part + 0x18) = float_0_804257a8;
        *(f32*)(part + 0x1C) = float_0_804257a8;
        *(f32*)(part + 0x20) = float_1p75_804257dc;
        *(f32*)(part + 0x24) = float_neg0p16_804257e0;
        *(f32*)(part + 0x28) = float_0_804257a8;
        angleDeg = ((i - 1) * 0x168) / (count - 1);
        rad = (float_6p2832_804257e4 * (f32)angleDeg) / float_360_804257e8;
        *(f32*)(part + 0x2C) = float_0_804257a8;
        *(f32*)(part + 4) = radius * -(f32)sin(rad);
        *(f32*)(part + 8) = radius * -(f32)cos(rad);
        *(f32*)(part + 0xC) = float_0_804257a8;
        *(f32*)(part + 0x30) = (f32)sin(rad);
        *(f32*)(part + 0x34) = (f32)cos(rad);
        *(s32*)(part + 0x50) = variant;
        if (variant == 0) {
            *(u8*)(part + 0x44) = 0xEE;
            *(u8*)(part + 0x45) = 0xDE;
            *(u8*)(part + 0x46) = 0xD7;
            *(u8*)(part + 0x47) = 0xA0;
        } else {
            *(u8*)(part + 0x44) = 0xFF;
            *(u8*)(part + 0x45) = 0xFF;
            *(u8*)(part + 0x46) = 0xFF;
            *(u8*)(part + 0x47) = 0xA0;
            *(s32*)(part + 0x54) = 0;
        }
    }
    return entry;
}

void effKemuri8Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKemuri8Disp(void);
    extern const Vec3 vec3_802fb298;
    extern f32 float_16_804257c4;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;
    s32 life;
    s32 nextFrame;
    s32 fadeIn;
    s32 fadeOut;
    s32 camera;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb298;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    frame = *(s32*)(work + 0x3C);
    life = *(s32*)(work + 0x4C);
    nextFrame = frame + 1;
    camera = *(s32*)(work + 0x58);
    fadeOut = (s32)((float_16_804257c4 * (f32)frame) / (f32)life);
    fadeIn = (s32)((float_16_804257c4 * (f32)nextFrame) / (f32)life);
    *(s32*)(work + 0x3C) = nextFrame;
    if (fadeOut > 0xF) {
        fadeOut = 0xF;
    }
    if (fadeIn > 0xF) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x40) = fadeIn;
    *(s32*)(work + 0x48) = fadeOut;

    part = work + 0x5C;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x5C) {
        *(f32*)(part + 0x18) += *(f32*)(part + 0x1C);
        *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
        *(f32*)(part + 4) += *(f32*)(part + 0x10) * *(f32*)(part + 0x30);
        *(f32*)(part + 8) += *(f32*)(part + 0x10) * *(f32*)(part + 0x34);
    }

    dispEntry(camera, 2, effKemuri8Disp, effect, dispCalcZ(&dispPos));
}

