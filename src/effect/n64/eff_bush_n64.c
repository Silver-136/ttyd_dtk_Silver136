#include "effect/n64/eff_bush_n64.h"


u8 effBushMain(void) {
    return 0;
}


u8 effBushDisp(void) {
    return 0;
}


#pragma optimize_for_size off

void* effBushN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBushMain(void);
    extern s32 rand(void);
    extern char str_BushN64_802facb8[];
    extern f64 double_to_int_802facb0;
    extern f32 float_0_80424ea0;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 r;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BushN64_802facb8;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0x104);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBushMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 100;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    zero = float_0_80424ea0;
    part = work + 0x34;
    for (i = 1; i < 5; i++) {
        r = rand();
        *(f32*)(part + 4) = (f32)(r % 30 - 15);
        *(f32*)(part + 8) = zero;
        r = rand();
        *(f32*)(part + 0xC) = (f32)((r & 0x1E) - 15);
        r = rand();
        *(f32*)(part + 0x18) = (f32)(r & 0x168);
        r = rand();
        *(f32*)(part + 0x20) = (f32)(r & 0x168);
        r = rand();
        *(f32*)(part + 0x1C) = (f32)((r % 100) / 10);
        *(f32*)(part + 0x10) = zero;
        *(f32*)(part + 0x14) = zero;
        part += 0x34;
    }

    return entry;
}

#pragma optimize_for_size on

