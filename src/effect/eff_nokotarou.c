#include "effect/eff_nokotarou.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effNokotarouMain(void* entry);

extern char str_Nokotarou_80302b18[];
extern f32 float_0_80428a34;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effNokotarouEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field1C) {
    void* entry = effEntry();
    void* work;
    f32 zero;
    s32 alpha;

    *(char**)((s32)entry + 0x14) = str_Nokotarou_80302b18;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a34;
    *(void**)((s32)entry + 0x10) = effNokotarouMain;
    alpha = 0xFF;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = *(f32*)((s32)work + 4);
    *(f32*)((s32)work + 0x14) = *(f32*)((s32)work + 8);
    *(f32*)((s32)work + 0x18) = *(f32*)((s32)work + 0xC);
    *(f32*)((s32)work + 0x1C) = field1C;
    *(f32*)((s32)work + 0x20) = zero;
    *(s32*)((s32)work + 0x24) = alpha;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }
    *(s32*)((s32)work + 0x30) = -1;
    *(s32*)((s32)work + 0x34) = -1;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
