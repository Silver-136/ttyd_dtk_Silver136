#include "effect/eff_toge_flush.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_0_804246c0;
extern f32 float_30_804246f4;
extern f32 float_0p8_804246f8;

const char str_TogeFlush_802f9748[] = "TogeFlush";


u8 effTogeFlushDisp(int param_1, int param_2) {
    return 0;
}


u8 effTogeFlushMain(int* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTogeFlushEntry(f32 x, s32 type, f32 y, s32 arg, f32 z) {
    void* entry;
    void* work;
    f32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_TogeFlush_802f9748;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTogeFlushMain;
    zero = float_0_804246c0;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x2C) = arg;
    *(s32*)((s32)work + 0x30) = 0;
    *(s32*)((s32)work + 0x24) = 0;
    *(f32*)((s32)work + 0x10) = zero;
    if (type == 0 || type == 3) {
        *(f32*)((s32)work + 0x14) = float_30_804246f4;
    } else {
        *(f32*)((s32)work + 0x14) = zero;
    }
    *(f32*)((s32)work + 0x18) = zero;
    *(f32*)((s32)work + 0x1C) = float_0p8_804246f8;
    *(s32*)((s32)work + 0x28) = 0;
    *(f32*)((s32)work + 0x20) = zero;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
