#include "effect/n64/eff_thunderflare_n64.h"


u8 effThunderflareMain(void) {
    return 0;
}


u8 effThunderflareDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderflareMain(void*);
    extern char str_ThunderflareN64_802fc1d0[];
    extern f32 float_0_80426408;
    void* entry;
    u8* work;
    u8* part;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderflareN64_802fc1d0;
    *(s32*)((s32)entry + 8) = 0x1F;
    work = __memAlloc(3, 0x8B8);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderflareMain;
    *(s32*)(work + 4) = type;
    *(s32*)(work + 0x1C) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x18) = 0x64;
    } else {
        *(s32*)(work + 0x18) = lifetime;
    }
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (type == 1) {
        goto color_other;
    }
    if (type >= 1) {
        goto color_other;
    }
    if (type >= 0) {
        goto color_zero;
    }
    goto color_other;
color_zero:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x38) = 0xFF;
    *(s32*)(work + 0x3C) = 0xFF;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    goto color_done;
color_other:
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0x14;
    *(s32*)(work + 0x38) = 0x21;
    *(s32*)(work + 0x3C) = 0x32;
    *(s32*)(work + 0x40) = 0x14;
    *(s32*)(work + 0x44) = 0xA;
color_done:
    part = work + 0x48;
    for (i = 1; i < 0x1F; i += 2, part += 0x90) {
        *(f32*)(part + 0x20) = (f32)(((i - 1) * 0xCA8) / 0xF);
        *(f32*)(part + 0x24) = float_0_80426408;
        *(f32*)(part + 0x2C) = (f32)(((i - 1) * 0x64) / 0xF);
        *(s32*)part = 0;
        *(f32*)(part + 0x68) = (f32)((i * 0xCA8) / 0xF);
        *(f32*)(part + 0x6C) = float_0_80426408;
        *(f32*)(part + 0x74) = (f32)((i * 0x64) / 0xF);
        *(s32*)(part + 0x48) = 0;
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

