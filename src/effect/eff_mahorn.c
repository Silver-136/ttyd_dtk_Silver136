#include "effect/eff_mahorn.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMahornMain(void);

extern char str_Mahorn_80302968[];
extern f32 float_0_804287f8;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMahornEntry(s32 kind, s32 count, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    s32 workCount = count + 1;
    void* work;
    s32 value;
    void* child;
    s32 i;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_Mahorn_80302968;
    *(s32*)((s32)entry + 8) = workCount;
    work = __memAlloc(3, (u32)(workCount * 0x28));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMahornMain;

    *(s32*)((s32)work + 0) = kind;
    *(s32*)((s32)work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }

    *(f32*)((s32)work + 4) = x;
    zero = float_0_804287f8;
    child = (void*)((s32)work + 0x28);
    *(f32*)((s32)work + 8) = y;
    i = 1;
    value = 4;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x18) = 0xFF;
    while (i < *(s32*)((s32)entry + 8)) {
        *(f32*)((s32)child + 4) = zero;
        i++;
        *(f32*)((s32)child + 8) = zero;
        *(f32*)((s32)child + 0xC) = zero;
        *(f32*)((s32)child + 0x10) = zero;
        *(f32*)((s32)child + 0x14) = zero;
        *(s32*)((s32)child + 0x18) = 0xFF;
        *(s32*)((s32)child + 0x24) = value;
        value += 4;
        child = (void*)((s32)child + 0x28);
    }

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
