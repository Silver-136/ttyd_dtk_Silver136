#include "effect/n64/eff_opuku_wcurtain_n64.h"


u8 effOpukuWcurtainDisp(void) {
    return 0;
}


void effOpukuWcurtainMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOpukuWcurtainDisp(void);
    extern const Vec3 vec3_802fbbc0;
    extern f32 float_neg0p05_80425bac;
    extern f32 float_5_80425bb0;
    extern f32 float_0p1_80425b98;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;
    s32 frame;
    f32 v;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbbc0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 0x3E8) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x14);
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }

    p = work;
    for (i = 0; i < 12; i++, p += 4) {
        *(f32*)(p + 0x2C) += *(f32*)(p + 0x5C);
        *(f32*)(p + 0x5C) += float_neg0p05_80425bac;
        v = *(f32*)(p + 0x5C);
        if (v > float_5_80425bb0) {
            *(f32*)(p + 0x5C) = v + float_0p1_80425b98 * (float_5_80425bb0 - v);
        }
    }

    dispEntry(4, 2, effOpukuWcurtainDisp, effect, dispCalcZ(&dispPos));
}

void* effOpukuWcurtainN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOpukuWcurtainMain(void*);
    extern s32 rand(void);
    extern char str_OpukuWcurtainN64_802fbbcc[];
    extern f32 float_0_80425ba0;
    extern f32 float_neg1_80425bb4;
    extern f32 float_0p01_80425bb8;
    extern f32 float_0p5_80425bbc;
    void* entry;
    u8* work;
    u8* p;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OpukuWcurtainN64_802fbbcc;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x8C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuWcurtainMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x10) = 0x3E8;
    } else {
        *(s32*)(work + 0x10) = time;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = scale;
    *(s32*)(work + 0x18) = 0x14;
    *(s32*)(work + 0x1C) = 0x78;
    *(s32*)(work + 0x20) = 0xFF;

    p = work;
    for (i = 0; i < 12; i++, p += 4) {
        *(f32*)(p + 0x2C) = float_0_80425ba0;
        *(f32*)(p + 0x5C) = (float_neg1_80425bb4 + (float_0p01_80425bb8 * (f32)(rand() % 11))) * float_0p5_80425bbc;
    }
    return entry;
}

