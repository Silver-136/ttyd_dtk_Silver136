#include "effect/n64/eff_fireworks_n64.h"


u8 effFireworksDisp(void) {
    return 0;
}


u8 effFireworksMain(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireworksN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireworksMain(void*);
    extern char str_FireworksN64_802faf78[];
    extern f32 float_0_804252b0;
    extern f32 float_1000_804252dc;
    void* entry;
    u8* work;
    f32 zero;
    f32 yOff;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireworksN64_802faf78;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0xB0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireworksMain;

    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x1C) = 1000;
    } else {
        *(s32*)(work + 0x1C) = timer + 0x20;
    }
    *(s32*)(work + 0x4C) = 0;
    *(s32*)(work + 0x30) = 0xFF;
    zero = float_0_804252b0;
    yOff = y - float_1000_804252dc;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x44) = zero;
    *(f32*)(work + 0x10) = vx;
    *(f32*)(work + 0x14) = vy;
    *(f32*)(work + 0x18) = vz;
    *(f32*)(work + 0x48) = scale;
    *(s32*)(work + 0x24) = 0xFF;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x2C) = 100;
    *(s32*)(work + 0x34) = 0xFF;
    *(s32*)(work + 0x38) = 100;
    *(s32*)(work + 0x3C) = 0x96;
    *(s32*)(work + 0x40) = 0xFF;

    *(f32*)(work + 0x50) = x;
    *(f32*)(work + 0x60) = yOff;
    *(f32*)(work + 0x70) = z;
    *(f32*)(work + 0x80) = zero;
    *(f32*)(work + 0x90) = zero;
    *(f32*)(work + 0xA0) = zero;
    *(f32*)(work + 0x54) = x;
    *(f32*)(work + 0x64) = yOff;
    *(f32*)(work + 0x74) = z;
    *(f32*)(work + 0x84) = zero;
    *(f32*)(work + 0x94) = zero;
    *(f32*)(work + 0xA4) = zero;
    *(f32*)(work + 0x58) = x;
    *(f32*)(work + 0x68) = yOff;
    *(f32*)(work + 0x78) = z;
    *(f32*)(work + 0x88) = zero;
    *(f32*)(work + 0x98) = zero;
    *(f32*)(work + 0xA8) = zero;
    *(f32*)(work + 0x5C) = x;
    *(f32*)(work + 0x6C) = yOff;
    *(f32*)(work + 0x7C) = z;
    *(f32*)(work + 0x8C) = zero;
    *(f32*)(work + 0x9C) = zero;
    *(f32*)(work + 0xAC) = zero;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

