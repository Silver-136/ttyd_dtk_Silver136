#include "effect/n64/eff_thunder_n64.h"


u8 effThunderDisp(int param_1, int param_2) {
    return 0;
}


u8 effThunderMain(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderMain(void*);
    extern void* effThunderflareN64Entry(s32 type, s32 unk, f32 x, f32 y, f32 z, f32 scale);
    extern void* effAkariChargeN64Entry(s32 type, s32 unk, f32 x, f32 y, f32 z, f32 scale);
    extern void* effShockN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy);
    extern char str_ThunderN64_802fc1a8[];
    extern void* gp;
    extern f32 float_0_804263c0;
    extern f32 float_48_804263d8;
    extern f32 float_32_804263dc;
    extern f32 float_8_804263e0;
    extern f32 float_neg118_804263e4;
    extern f32 float_184_804263e8;
    extern f32 float_79_804263ec;
    extern f32 float_1_804263c4;
    extern f32 float_neg120_804263f0;
    extern f32 float_189_804263f4;
    extern f32 float_72_804263f8;
    void* entry;
    u8* work;
    s32 shockType;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderN64_802fc1a8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x4C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x18) = 0;
    if (type >= 3) {
        *(s32*)(work + 0x14) = 0x1E;
    } else {
        *(s32*)(work + 0x14) = 0x64;
    }
    *(s32*)(work + 0x10) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x20) = float_0_804263c0;
    *(f32*)(work + 0x1C) = float_0_804263c0;
    *(f32*)(work + 0x2C) = float_48_804263d8;
    *(f32*)(work + 0x30) = float_32_804263dc;
    *(f32*)(work + 0x28) = float_0_804263c0;
    *(f32*)(work + 0x24) = float_0_804263c0;
    *(f32*)(work + 0x34) = float_8_804263e0;
    *(f32*)(work + 0x38) = float_32_804263dc;
    *(s32*)(work + 0x3C) = 4;
    *(s32*)(work + 0x40) = 2;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x44) = 0;
    if (type == 1 || type == 2) {
        if (type == 1) {
            *(void**)(work + 0x44) = effThunderflareN64Entry(0, -1, float_neg118_804263e4, float_184_804263e8, float_79_804263ec, float_1_804263c4);
        }
        if (type != 1) {
            *(void**)(work + 0x48) = effAkariChargeN64Entry(0, 0, float_neg120_804263f0, float_189_804263f4, float_72_804263f8, float_1_804263c4);
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x28) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x2C) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x30) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x18) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x1C) = 0;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x20) = 0xFF;
        }
    }
    if (type >= 3) {
        shockType = 1;
        if (*(s32*)((s32)gp + 0x14) == 1) {
            shockType = 3;
        }
        effShockN64Entry(shockType, x, y, z, sx, sy);
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

