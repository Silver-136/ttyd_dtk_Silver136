#include "effect/eff_treasure_map.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTreasureMapEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effTreasureMapMain(void);
    extern const char str_Treasure_Map_802ff078[];
    extern f32 float_1_80427940;
    extern f32 float_0_80427934;

    void* entry = effEntry();
    void* work;

    *(const char**)((s32)entry + 0x14) = str_Treasure_Map_802ff078;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x2C);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effTreasureMapMain;
    *(u32*)entry |= 2;
    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x18) = 1000;
    *(s32*)((s32)work + 0x14) = 0xFF;
    *(f32*)((s32)work + 0x10) = float_1_80427940;
    *(f32*)((s32)work + 0x20) = float_0_80427934;
    *(s32*)((s32)work + 0x24) = -1;
    *(s32*)((s32)work + 0x28) = 4;
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_treasure_map 20260624_184823 */

/* stub-fill: effTreasureMapDisp | missing_definition | ghidra_signature */
u8 effTreasureMapDisp(void) {
    return 0;
}

/* stub-fill: effTreasureMapMain | prototype_only | source_prototype */
void effTreasureMapMain(void) {
    return;
}
