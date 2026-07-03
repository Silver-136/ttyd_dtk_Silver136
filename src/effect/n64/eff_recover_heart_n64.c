#include "effect/n64/eff_recover_heart_n64.h"


u8 effRecoverHeartDisp(int param_1, int param_2) {
    return 0;
}


u8 effRecoverHeartMain(void) {
    return 0;
}


void* effRecoverHeartN64Entry(s32 type, s32 direction, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRecoverHeartMain(void*);
    extern char str_RecoverHeartN64_802fbe08[];
    extern f32 float_0_80425e90;
    extern f32 float_1p4_80425eb4;
    void* entry;
    u8* work;
    s32 mode;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_RecoverHeartN64_802fbe08;
    *(s32*)((s32)entry + 8) = 2;
    work = __memAlloc(3, 0x90);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRecoverHeartMain;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x2C) = 0x2E;
    *(s32*)(work + 0x30) = 0;
    if (direction < 0) {
        direction = -direction;
        *(s32*)(work + 0x44) = 1;
    } else {
        *(s32*)(work + 0x44) = 0;
    }
    *(s32*)(work + 0x40) = direction;
    mode = 1;
    *(f32*)(work + 0x4C) = float_0_80425e90;
    *(f32*)(work + 0x50) = float_0_80425e90;
    *(f32*)(work + 0x54) = float_0_80425e90;
    *(f32*)(work + 0x6C) = float_0_80425e90;
    *(f32*)(work + 0x58) = float_0_80425e90;
    *(f32*)(work + 0x60) = float_0_80425e90;
    *(f32*)(work + 0x68) = float_0_80425e90;
    *(f32*)(work + 0x64) = float_0_80425e90;
    if (mode == 1) {
        *(f32*)(work + 0x4C) = float_0_80425e90;
        *(s32*)(work + 0x7C) = 0;
        *(f32*)(work + 0x80) = float_0_80425e90;
        *(f32*)(work + 0x84) = float_0_80425e90;
        *(f32*)(work + 0x5C) = float_1p4_80425eb4;
    } else {
        *(f32*)(work + 0x4C) = (f32)(((((mode & 1) << 1) - 1) * (0 >> 1)) << 2);
        *(f32*)(work + 0x5C) = float_1p4_80425eb4;
        *(s32*)(work + 0x7C) = 0xB;
        *(f32*)(work + 0x80) = float_0_80425e90;
        *(f32*)(work + 0x84) = float_0_80425e90;
    }
    *(s32*)(work + 0x74) = 0;
    *(s32*)(work + 0x70) = 0xFF;
    return entry;
}

