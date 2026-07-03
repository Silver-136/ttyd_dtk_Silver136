#include "effect/n64/eff_broken_barrier_n64.h"


u8 effBrokenBarrierMain(void) {
    return 0;
}


void* effBrokenBarrierN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBrokenBarrierMain(void);
    extern char str_BrokenBarrierN64_802fac88[];
    extern f32 float_30_80424e80;
    extern f32 float_0p33333_80424e84;
    extern f32 float_60_80424e88;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 j;
    f32 step;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BrokenBarrierN64_802fac88;
    *(s32*)((s32)entry + 8) = 0x3D;
    work = __memAlloc(3, 0x19BC);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBrokenBarrierMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x3C) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x38) = 1000;
    } else {
        *(s32*)(work + 0x38) = timer;
    }

    *(s32*)(work + 0x4C) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x60) = scale;
    *(f32*)(work + 0x10) = float_30_80424e80;
    *(f32*)(work + 0x14) = float_30_80424e80;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x50) = 0xD7;
    *(s32*)(work + 0x54) = 0xFF;
    *(s32*)(work + 0x58) = 0xEB;
    *(s32*)(work + 0x5C) = 0xFF;

    if (timer < 20) {
        step = float_0p33333_80424e84;
    } else {
        step = (f32)timer / float_60_80424e88;
    }

    part = work + 0x6C;
    for (i = 0; i < 12; i++) {
        for (j = 1; j < 6; j++) {
            *(s32*)(part + 0x30 + (j - 1) * 0x6C) = (s32)(-(f32)j * step) - 1;
        }
        part += 0x21C;
    }

    return entry;
}

u8 effBrokenBarrierDisp(int param_1, int param_2) {
    return 0;
}


u8 main_dl(void) {
    return 0;
}
