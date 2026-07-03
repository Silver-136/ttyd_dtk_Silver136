#include "effect/n64/eff_ice_n64.h"


u8 effIceMain(void) {
    return 0;
}


u8 effIceDisp(int param_1, int param_2) {
    return 0;
}


void* effIceN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effIceMain(void*);
    extern s32 rand(void);
    extern char str_IceN64_802fb160[];
    extern f32 float_10_80425544;
    extern f32 float_0_8042551c;
    extern f32 float_2_80425530;
    void* entry;
    u8* work;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_IceN64_802fb160;
    *(s32*)((s32)entry + 8) = 9;
    work = __memAlloc(3, 0x1B0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effIceMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 0x1E;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    part = work + 0x30;
    for (i = 1; i < 9; i++) {
        *(f32*)(part + 4) = (f32)(rand() % 10 - 5);
        *(f32*)(part + 8) = (f32)(rand() % 10 + 5);
        *(f32*)(part + 0xC) = float_10_80425544;
        *(f32*)(part + 0x18) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x20) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x1C) = (f32)((rand() % 100) / 10);
        *(f32*)(part + 0x10) = float_0_8042551c;
        *(f32*)(part + 0x14) = float_2_80425530;
        part += 0x30;
    }

    return entry;
}

