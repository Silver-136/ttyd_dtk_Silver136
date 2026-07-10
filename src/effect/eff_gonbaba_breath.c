#include "effect/eff_gonbaba_breath.h"


/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathDisp | missing_definition | ghidra_signature */
u8 effGonbabaBreathDisp(void) {
    return 0;
}


/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathEntry | missing_definition | ghidra_signature */
void* effGonbabaBreathEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effGonbabaBreathMain(void* entry);
    extern void init_breath(void* work, s32 index, s32 type);
    extern char str_GonbabaBreath_80302b90[];
    extern f32 float_0_80428ab4;

    void* entry;
    void* work;
    s32 count;
    s32 i;
    void* child;
    void* childWork;
    void* grandChild;

    entry = effEntry();
    if (type == 7) {
        count = 0x11;
    } else if (type < 7) {
        if (type >= 0) {
            count = 0x21;
        }
    } else if (type < 0x12) {
        if (type >= 0xA) {
            count = 0x41;
        }
    }

    *(char**)((s32)entry + 0x14) = str_GonbabaBreath_80302b90;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x50);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGonbabaBreathMain;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x40) = scale;
    *(s32*)((s32)work + 0x3C) = 0xFF;
    *(s32*)((s32)work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(f32*)((s32)work + 0x48) = float_0_80428ab4;
    *(void**)((s32)work + 0x4C) = 0;

    if (type < 10) {
        child = effGonbabaBreathEntry(type + 10, x, y, z, scale, timer);
        *(void**)((s32)work + 0x4C) = child;
        childWork = *(void**)((s32)child + 0xC);
        *(f32*)((s32)childWork + 0x48) = *(f32*)((s32)work + 0x48);
        grandChild = *(void**)((s32)childWork + 0x4C);
        if (grandChild != 0) {
            *(f32*)((s32)*(void**)((s32)grandChild + 0xC) + 0x48) = *(f32*)((s32)work + 0x48);
        }
    }

    work = (void*)((s32)work + 0x50);
    i = 1;
    while (i < count) {
        init_breath(work, i, type);
        i += 1;
        work = (void*)((s32)work + 0x50);
    }

    return entry;
}

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: init_breath | missing_definition | ghidra_signature */
u8 init_breath(void) {
    return 0;
}


/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_gonbaba_breath 20260624_191429 */

/* fallback stub-fill: map=effGonbabaBreathMain addr=0x8025b270 size=0x00000694 */
int effGonbabaBreathMain() {
    return 0;
}
