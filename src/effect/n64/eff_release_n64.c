#include "effect/n64/eff_release_n64.h"


u8 effReleaseDisp(int param_1, int param_2) {
    return 0;
}


u8 effReleaseMain(u32* param_1) {
    return 0;
}


void* effReleaseN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effReleaseMain(void*);
    extern char str_ReleaseN64_802fbe38[];
    extern f32 float_1_80425ee4;
    extern f32 float_0_80425ec0;
    void* entry;
    u8* work;
    s32 kind;
    s32 color;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ReleaseN64_802fbe38;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x78);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effReleaseMain;
    kind = type & 0xFF00;
    color = type & 0xFF;
    *(s32*)work = kind;
    *(s32*)(work + 4) = color;
    *(s32*)(work + 0x18) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x14) = 1000;
    } else {
        *(s32*)(work + 0x14) = lifetime;
    }
    *(s32*)(work + 0x28) = 0xFF;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    *(f32*)(work + 0x3C) = scale;
    *(s32*)(work + 0x1C) = 0xFF;
    *(s32*)(work + 0x20) = 0xFF;
    *(s32*)(work + 0x24) = 0xFF;
    *(s32*)(work + 0x2C) = 0xFF;
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0xFF;
    *(s32*)(work + 0x38) = 0xFF;
    *(s32*)(work + 0x60) = 0;
    *(s32*)(work + 0x64) = 0;
    *(s32*)(work + 0x68) = 0;
    *(s32*)(work + 0x6C) = 0;
    *(f32*)(work + 0x40) = float_1_80425ee4;
    *(f32*)(work + 0x44) = float_0_80425ec0;
    *(f32*)(work + 0x48) = float_0_80425ec0;
    *(f32*)(work + 0x4C) = float_0_80425ec0;
    *(s32*)(work + 0x50) = 0xFF;
    *(s32*)(work + 0x54) = 0;
    *(s32*)(work + 0x58) = 0;
    *(s32*)(work + 0x5C) = 0;
    if (color == 3) {
        *(s32*)(work + 0x70) = 1;
    } else {
        *(s32*)(work + 0x70) = 0;
    }
    *(s32*)(work + 0x74) = 0;
    return entry;
}

