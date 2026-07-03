#include "effect/n64/eff_cloud_n64.h"


u8 effCloudDisp(void) {
    return 0;
}


u8 effCloudMain(u32* param_1) {
    return 0;
}


void* effCloudN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCloudMain(void*);
    extern s32 rand(void);
    extern char str_CloudN64_802fad30[];
    extern f32 float_0_80424f18;
    void* entry;
    u8* work;
    s32 i;
    s32 r;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CloudN64_802fad30;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x324);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCloudMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x34) = scale;

    switch (type) {
        case 0:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xE1;
            *(s32*)(work + 0x2C) = 0xE1;
            *(s32*)(work + 0x30) = 0xE1;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 1:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xF5;
            *(s32*)(work + 0x20) = 0xE6;
            *(s32*)(work + 0x28) = 0x2D;
            *(s32*)(work + 0x2C) = 0x23;
            *(s32*)(work + 0x30) = 0xF;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 2:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0;
            *(s32*)(work + 0x20) = 0;
            *(s32*)(work + 0x28) = 0x9B;
            *(s32*)(work + 0x2C) = 0x7D;
            *(s32*)(work + 0x30) = 0x7D;
            *(s32*)(work + 0x24) = 0xFF;
            break;
        case 3:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xFF;
            *(s32*)(work + 0x2C) = 0x41;
            *(s32*)(work + 0x30) = 0x14;
            *(s32*)(work + 0x24) = 0xFF;
            break;
    }

    *(f32*)(work + 0x38) = x;
    *(f32*)(work + 0x3C) = y;
    *(f32*)(work + 0x40) = z;
    for (i = 0; i < 0x7C; i += 4) {
        *(s32*)(work + 0x230 + i) = 0;
    }
    *(f32*)(work + 0x50) = x;
    *(f32*)(work + 0xC8) = y;
    *(f32*)(work + 0x140) = z;
    *(s32*)(work + 0x230) = 1;
    *(s32*)(work + 0x1B8) = 0;
    *(f32*)(work + 0x2AC) = float_0_80424f18;
    r = rand();
    *(f32*)(work + 0x44) = (f32)(r % 30 + 10);
    *(f32*)(work + 0x48) = float_0_80424f18;
    *(f32*)(work + 0x4C) = float_0_80424f18;

    return entry;
}

