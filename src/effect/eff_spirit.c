#include "effect/eff_spirit.h"


/* CHATGPT STUB FILL: main/effect/eff_spirit 20260624_184929 */

/* stub-fill: effSpiritDisp | missing_definition | ghidra_signature */
u8 effSpiritDisp(int param_1, int param_2) {
    return 0;
}


/* CHATGPT STUB FILL: main/effect/eff_spirit 20260624_184929 */

/* stub-fill: effSpiritEntry | missing_definition | ghidra_signature */
void* effSpiritEntry(f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ, f32 scale, s32 kind, s32 flags) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_Spirit_80302988[];
    extern int effSpiritMain(void);
    extern f32 float_0_80428824;
    extern f32 float_2_80428828;
    extern f32 float_1_8042883c;

    void* entry;
    void* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Spirit_80302988;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x68);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSpiritMain;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = kind;
    *(f32*)((s32)work + 0x10) = x;
    *(f32*)((s32)work + 0x14) = y;
    *(f32*)((s32)work + 0x18) = z;
    *(f32*)((s32)work + 0x60) = scale;
    *(f32*)((s32)work + 0x64) = float_1_8042883c;
    *(s32*)((s32)work + 0xC) = 0;
    *(s32*)((s32)work + 8) = 1000;
    *(s32*)((s32)work + 4) = 0;
    *(f32*)((s32)work + 0x34) = float_0_80428824;
    *(f32*)((s32)work + 0x38) = float_2_80428828;
    *(f32*)((s32)work + 0x3C) = float_0_80428824;
    *(f32*)((s32)work + 0x1C) = *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 0x20) = *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0x24) = *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x28) = targetX;
    *(f32*)((s32)work + 0x2C) = targetY;
    *(f32*)((s32)work + 0x30) = targetZ;
    *(s32*)((s32)work + 0x48) = 0xFF;
    *(s32*)((s32)work + 0x4C) = 0xF2;
    *(s32*)((s32)work + 0x50) = 0;
    *(s32*)((s32)work + 0x54) = 0xCC;
    *(s32*)((s32)work + 0x58) = 0;
    *(s32*)((s32)work + 0x5C) = 0;
    *(s32*)((s32)work + 0x40) = 0;
    *(s32*)((s32)work + 0x44) = flags;
    return entry;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_spirit 20260624_191429 */

/* fallback stub-fill: map=effSpiritMain addr=0x80255824 size=0x000004c4 */
int effSpiritMain() {
    return 0;
}
