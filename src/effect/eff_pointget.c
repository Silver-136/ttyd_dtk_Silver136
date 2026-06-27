#include "effect/eff_pointget.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPointGetEntry(s32 kind, f32 x, f32 y, f32 z, s32 arg4, s32 arg5) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effPointGetMain(void);
    extern const char str_PointGet_802fed08[];
    extern f32 float_2000_8042743c;
    extern f32 float_30_80427440;
    extern f32 float_1_80427430;

    void* entry = effEntry();
    void* work;
    f32 scale;
    f32 posY;
    f32 posX;
    u32 flags;
    s32 zero;

    *(const char**)((s32)entry + 0x14) = str_PointGet_802fed08;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x24);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effPointGetMain;
    zero = 0;
    posX = x - float_2000_8042743c;
    flags = *(u32*)entry | 2;
    posY = y - float_30_80427440;
    scale = float_1_80427430;
    *(u32*)entry = flags;
    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = posX;
    *(f32*)((s32)work + 8) = posY;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x10) = arg4;
    *(f32*)((s32)work + 0x14) = scale;
    *(s32*)((s32)work + 0x18) = arg5;
    *(s32*)((s32)work + 0x1C) = zero;
    *(s32*)((s32)work + 0x20) = zero;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_pointget 20260624_184823 */

/* stub-fill: effPointGetDisp | missing_definition | ghidra_signature */
u8 effPointGetDisp(void) {
    return 0;
}

/* stub-fill: effPointGetMain | prototype_only | source_prototype */
void effPointGetMain(void) {
    return;
}
