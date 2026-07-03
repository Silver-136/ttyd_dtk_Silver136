#include "effect/n64/eff_kameki_tornade_n64.h"


u8 effKamekiTornadeDisp(void) {
    return 0;
}


void* effKamekiTornadeN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKamekiTornadeMain(void*);
    extern s32 rand(void);
    extern char str_KamekiTornadeN64_802fb198[];
    extern f32 float_0p2_8042559c;
    extern f32 float_1_80425598;
    extern f32 float_0p1_80425564;
    extern f32 float_2_804255a0;
    extern f32 float_0_80425570;
    void* entry;
    u8* work;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_KamekiTornadeN64_802fb198;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x14C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKamekiTornadeMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x34) = scale;
    *(s32*)(work + 0x18) = 0xFF;
    *(s32*)(work + 0x1C) = 0xFF;
    *(s32*)(work + 0x20) = 0xFF;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x2C) = 0xFF;
    *(s32*)(work + 0x30) = 0xEB;

    for (i = 0; i < 8; i++) {
        *(f32*)(work + 0x38 + i * 4) = x;
        *(f32*)(work + 0x58 + i * 4) = y;
        *(f32*)(work + 0x78 + i * 4) = z;
        *(f32*)(work + 0x98 + i * 4) = float_1_80425598 + float_0p2_8042559c * (f32)i;
        *(f32*)(work + 0xB8 + i * 4) = (f32)(rand() % 0x168);
        *(f32*)(work + 0xD8 + i * 4) = float_2_804255a0 + float_0p1_80425564 * (f32)(rand() % 100);
        *(f32*)(work + 0xF8 + i * 4) = float_0p1_80425564 * (f32)(rand() % 30);
        *(f32*)(work + 0x118 + i * 4) = float_0_80425570;
    }
    *(f32*)(work + 0x138) = float_0_80425570;
    *(f32*)(work + 0x144) = float_0_80425570;
    *(f32*)(work + 0x13C) = float_0_80425570;
    *(f32*)(work + 0x140) = float_0_80425570;
    *(f32*)(work + 0x148) = float_1_80425598;

    return entry;
}

u8 effKamekiTornadeMain(u32* param_1) {
    return 0;
}
