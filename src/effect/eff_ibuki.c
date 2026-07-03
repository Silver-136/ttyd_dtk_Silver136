#include "effect/eff_ibuki.h"


/* CHATGPT STUB FILL: main/effect/eff_ibuki 20260624_184929 */

/* stub-fill: effIbukiDisp | missing_definition | ghidra_signature */
u8 effIbukiDisp(void) {
    return 0;
}

/* stub-fill: effIbukiMain | missing_definition | ghidra_signature */
u8 effIbukiMain(void) {
    return 0;
}

/* stub-fill: effIbukiEntry | missing_definition | ghidra_signature */
void* effIbukiEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effIbukiMain(void);
    extern s32 irand(s32 max);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Ibuki_804284e0[];
    extern f32 float_1_804284c8;
    extern f32 float_0_804284a0;
    extern f32 float_6p2832_804284d8;
    extern f32 float_360_804284dc;

    void* effect;
    void* work;
    void* part;
    s32 i;
    s32 angle;
    f32 radians;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Ibuki_804284e0;
    *(s32*)((s32)effect + 8) = 0x21;
    work = __memAlloc(3, 0xAD4);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effIbukiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x30) = (duration > 0) ? duration : 1000;
    *(s32*)((s32)work + 0x34) = 0;
    *(f32*)((s32)work + 0x2C) = float_1_804284c8;
    *(s32*)((s32)work + 0x38) = 0;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x40) = float_0_804284a0;
    *(f32*)((s32)work + 0x44) = float_0_804284a0;
    *(f32*)((s32)work + 0x48) = float_0_804284a0;
    *(f32*)((s32)work + 0x4C) = float_0_804284a0;

    part = (void*)((s32)work + 0x54);
    for (i = 1, angle = 10; i < *(s32*)((s32)effect + 8); i++, angle += 10, part = (void*)((s32)part + 0x54)) {
        angle = 0x1E - irand(0x28);
        *(f32*)((s32)part + 4) = float_0_804284a0;
        *(f32*)((s32)part + 8) = (f32)irand(0x1E);
        *(f32*)((s32)part + 0xC) = float_0_804284a0;
        radians = (float_6p2832_804284d8 * (f32)angle) / float_360_804284dc;
        *(f32*)((s32)part + 0x10) = (f32)cos(radians);
        *(f32*)((s32)part + 0x14) = (f32)sin(radians);
        *(f32*)((s32)part + 0x18) = float_0_804284a0;
        *(f32*)((s32)part + 0x2C) = float_1_804284c8;
        *(s32*)((s32)part + 0x38) = 0;
        *(s32*)((s32)part + 0x50) = i * 10;
        *(f32*)((s32)part + 0x1C) = float_0_804284a0;
        *(f32*)((s32)part + 0x20) = float_0_804284a0;
        if (irand(1000) < 500) {
            *(f32*)((s32)part + 0x24) = (f32)(irand(10) + 10);
            *(f32*)((s32)part + 0x28) = float_0_804284a0;
        } else {
            *(f32*)((s32)part + 0x24) = float_0_804284a0;
            *(f32*)((s32)part + 0x28) = (f32)(irand(10) + 10);
        }
    }

    return effect;
}

