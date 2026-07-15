#include "effect/n64/eff_opuku_jetwrain_n64.h"


void effOpukuJetwrainMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effOpukuJetwrainDisp(void);
    extern f64 sin(f64);
    u8* work = *(u8**)((s32)effect + 0xC);
    Vec3 pos;
    s32 row, col, index;
    s32 timer;
    s16* heights = (s16*)(work + 0x11A);
    s16 next;

    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x20;
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
    if (timer < 0x20) {
        *(u8*)(work + 0x1F) = (u8)(timer << 3);
    } else if (*(s32*)(work + 0x14) < 0x10) {
        *(u8*)(work + 0x1F) = (u8)((*(s32*)(work + 0x14) << 4) + 0xF);
    } else {
        *(u8*)(work + 0x1F) = 0xFF;
    }

    for (row = 1; row < 18; row++) {
        index = row * 13;
        *(u8*)(work + 0x24 + index + 6) =
            (u8)(32.0f * (f32)sin((6.2832f * (f32)(*(s32*)(work + 0x14) * 8 + row * 12)) / 360.0f));
    }
    for (row = 1; row < 18; row++) {
        for (col = 1; col < 12; col++) {
            index = row * 13 + col;
            next = (s16)(((heights[index - 13] + heights[index + 13] +
                           heights[index - 1] + heights[index + 1]) >> 1) - heights[index]);
            next = (s16)((f32)next * 0.98f);
            if (timer < 0x20) {
                next = (s16)((f32)next * 0.7f);
            }
            heights[index] = next;
            *(u8*)(work + 0x24 + index) = (u8)(next >> 4);
        }
    }
    dispEntry(4, 2, effOpukuJetwrainDisp, effect, dispCalcZ(&pos));
}

u8 effOpukuJetwrainDisp(void) {
    return 0;
}


void* effOpukuJetwrainN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOpukuJetwrainMain(void*);
    extern char str_OpukuJetwrainN64_802fbba8[];
    void* entry;
    u8* work;
    s32 row;
    s32 col;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OpukuJetwrainN64_802fbba8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x308);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOpukuJetwrainMain;
    *(s32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x10) = 0x3E8;
    } else {
        *(s32*)(work + 0x10) = time;
    }
    *(u8*)(work + 0x1F) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x18) = scale;
    *(u8*)(work + 0x1C) = 0x0A;
    *(u8*)(work + 0x1D) = 0x6E;
    *(u8*)(work + 0x1E) = 0xFF;
    *(u8*)(work + 0x20) = 0xBE;
    *(u8*)(work + 0x21) = 0xDC;
    *(u8*)(work + 0x22) = 0xFF;
    *(u8*)(work + 0x23) = 0;

    for (row = 0; row < 0x13; row++) {
        for (col = 0; col < 0xD; col++) {
            *(u8*)(work + 0x24 + row * 0xD + col) = 0;
            *(s16*)(work + 0x11A + row * 0x1A + col * 2) = 0;
        }
    }
    return entry;
}

