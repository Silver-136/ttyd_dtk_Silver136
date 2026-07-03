#include "effect/n64/eff_akari_charge_n64.h"


u8 effAkariChargeMain(void) {
    return 0;
}


u8 effAkariChargeDisp(void) {
    return 0;
}


void* effAkariChargeN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effAkariChargeMain(void);
    extern char str_AkariChargeN64_802faad0[];
    extern f32 float_0_80424c24;
    extern f32 float_1_80424c2c;
    void* entry;
    void* work;
    s32 i;
    s32 j;
    s32 seq;
    u8* particle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_AkariChargeN64_802faad0;
    *(s32*)((s32)entry + 8) = 0x16;
    work = __memAlloc(3, 0x5D8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariChargeMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)((s32)work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x10) = 1000;
    } else {
        *(s32*)((s32)work + 0x10) = timer;
    }
    *(s32*)((s32)work + 0x24) = 0;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x38) = scale;
    *(s32*)((s32)work + 0x18) = 0xFF;
    *(s32*)((s32)work + 0x1C) = 0xE6;
    *(s32*)((s32)work + 0x20) = 0;
    *(s32*)((s32)work + 0x28) = 0xFF;
    *(s32*)((s32)work + 0x2C) = 0xFF;
    *(s32*)((s32)work + 0x30) = 0xFF;

    particle = (u8*)work + 0x44;
    seq = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++, seq++, particle += 0x44) {
            *(s32*)(particle + 0x10) = 0x14;
            *(s32*)(particle + 0x14) = 0;
            *(f32*)(particle + 4) = float_0_80424c24;
            *(f32*)(particle + 8) = float_0_80424c24;
            *(f32*)(particle + 0x34) = float_1_80424c2c;
            *(s32*)(particle + 0x3C) = (seq * 360) / 21;
            *(s32*)(particle + 0x40) = seq & 0xF;
        }
    }

    return entry;
}

