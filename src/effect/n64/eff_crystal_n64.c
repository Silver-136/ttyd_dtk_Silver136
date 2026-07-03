#include "effect/n64/eff_crystal_n64.h"


u8 effCrystalMain(u32* param_1) {
    return 0;
}


u8 effCrystalDisp(int param_1, int param_2) {
    return 0;
}


void* effCrystalN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCrystalMain(void*);
    extern char str_CrystalN64_802fadc8[];
    extern f32 float_0p1_80425064;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CrystalN64_802fadc8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x3C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCrystalMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    *(s32*)(work + 0x38) = 4;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 100;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(s32*)(work + 0x2C) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x30) = scale;
    *(s32*)(work + 0x18) = 0;

    switch (type) {
        case 0:
            *(s32*)(work + 0x1C) = 0;
            break;
        case 1:
            *(s32*)(work + 0x1C) = 2;
            break;
        case 2:
            *(s32*)(work + 0x1C) = 4;
            break;
        case 3:
            *(s32*)(work + 0x1C) = 6;
            break;
        case 4:
            *(s32*)(work + 0x1C) = 7;
            break;
        case 5:
            *(s32*)(work + 0x1C) = 8;
            break;
        case 6:
            *(s32*)(work + 0x1C) = 9;
            break;
        default:
            *(s32*)(work + 0x1C) = 10;
            *(f32*)(work + 0x34) = -(scale - float_0p1_80425064) / (f32)timer;
            break;
    }

    return entry;
}

