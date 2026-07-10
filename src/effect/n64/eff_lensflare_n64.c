#include "effect/n64/eff_lensflare_n64.h"


u8 effLensflareDisp(void) {
    return 0;
}


u8 effLensflareMain(void) {
    return 0;
}


void* effLensflareN64Entry(f32 x, f32 y, f32 z, s32 arg3, s32 life) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effLensflareMain(void);
    extern s32 rand(void);
    extern char str_LensflareN64_802fb408[];
    extern f32 float_0p1_804259a0;
    extern f32 float_0p4_804259c0;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 step;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_LensflareN64_802fb408;
    *(s32*)((s32)entry + 8) = 6;
    work = __memAlloc(3, 0x138);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effLensflareMain;
    *(u32*)entry |= 2;

    *(s32*)(work + 4) = arg3;
    *(s32*)(work + 0x1C) = 0;
    if (life < 1) {
        *(s32*)(work + 0x18) = 100;
    } else {
        *(s32*)(work + 0x18) = life;
    }
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;

    part = work + 0x34;
    step = 2;
    for (i = 1; i < 6; i++, step += 2, part += 0x34) {
        *(f32*)(part + 0x20) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x2C) = (f32)((step % 0x14) + 0x3C);
        *(f32*)(part + 0x28) = float_0p1_804259a0 * (f32)(rand() % 0x10) + float_0p4_804259c0;
        *(f32*)(part + 0x30) = float_0p1_804259a0 * (f32)(rand() % 0xB) + float_0p1_804259a0;
    }

    return entry;
}

