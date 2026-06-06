#include "effect/eff_updown.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_1_80424624;

const char str_Updown_802f9708[] = "Updown";


u8 effUpdownDisp(int param_1, int param_2) {
    return 0;
}


u8 polygon(void) {
    return 0;
}


u8 effUpdownMain(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUpdownEntry(f32 x, s32 type, f32 y, s32 arg1, f32 z, s32 arg2) {
    void* entry;
    void* work;
    s32 main;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Updown_802f9708;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x28);
    *(void**)((s32)entry + 0xC) = work;
    main = (s32)effUpdownMain;
    one = float_1_80424624;
    *(s32*)((s32)entry + 0x10) = main;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x10) = one;
    *(s32*)((s32)work + 0x18) = arg1;
    *(s32*)((s32)work + 0x1C) = arg2;
    *(s32*)((s32)work + 0x20) = 0;
    *(u8*)((s32)work + 0x24) = 0xFF;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
