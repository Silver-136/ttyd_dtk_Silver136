#include "effect/n64/eff_opuku_jetwrain_n64.h"


u8 effOpukuJetwrainMain(void) {
    return 0;
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

