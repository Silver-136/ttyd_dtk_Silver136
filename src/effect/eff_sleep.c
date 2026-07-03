#include "effect/eff_sleep.h"
#include <dolphin/mtx.h>

void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effSleepDisp(void* camera, void* entry);

extern const Vec vec3_80302ac0;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSleepMain(void* entry) {
    void* work = *(void**)((s32)entry + 0xC);
    Vec zpos;
    Vec pos = vec3_80302ac0;
    u32 flags;
    s32 value;
    s32 i;
    void* child;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    flags = *(u32*)entry;
    if (flags & 4) {
        *(u32*)entry = flags & ~4;
        *(s32*)((s32)work + 0x18) = 0x10;
    }

    value = *(s32*)((s32)work + 0x18);
    if (value < 1000) {
        *(s32*)((s32)work + 0x18) = value - 1;
    }

    if (*(s32*)((s32)work + 0x18) < 0x20) {
        *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x18) << 3;
    }

    if (*(s32*)((s32)work + 0x18) < 0) {
        effDelete(entry);
    } else {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x1C) + 1;
        if (*(s32*)((s32)work + 0x1C) < 0x20) {
            *(s32*)((s32)work + 0x14) = (*(s32*)((s32)work + 0x1C) << 3) + 7;
        }

        i = 1;
        child = (void*)((s32)work + 0x24);
        while (i < *(s32*)((s32)entry + 8)) {
            value = *(s32*)((s32)child + 0x20);
            if (value != 0) {
                *(s32*)((s32)child + 0x20) = value - 1;
            } else {
                *(s32*)((s32)child + 0x14) = *(s32*)((s32)child + 0x14) + 8;
                if (*(s32*)((s32)child + 0x14) > 0xFF) {
                    *(s32*)((s32)child + 0x14) = 0xFF;
                }
            }
            i++;
            child = (void*)((s32)child + 0x24);
        }

        dispCalcZ(&zpos);
        dispEntry(4, 2, effSleepDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_sleep 20260624_184929 */

/* stub-fill: effSleepDisp | prototype_only | source_prototype */
void effSleepDisp(void* camera, void* entry) {
    return;
}

/* stub-fill: effSleepEntry | missing_definition | ghidra_signature */
void* effSleepEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effSleepMain(void* entry);
    extern const char str_Sleep_804289f0[];
    extern f32 cos(f32 x);
    extern f32 sin(f32 x);
    extern f32 float_6p2832_804289d4;
    extern f32 float_16_804289fc;
    extern f32 float_360_804289f8;
    extern f32 float_0_804289e4;
    extern f32 float_0p2_80428a00;
    extern f32 float_1_804289e0;

    void* entry;
    void* work;
    void* child;
    f32 rad;
    f32 radius;
    s32 i;
    s32 delay;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Sleep_804289f0;
    *(s32*)((s32)entry + 8) = 4;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x24);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSleepMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = angle;
    *(s32*)((s32)work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x18) = 1000;
    } else {
        *(s32*)((s32)work + 0x18) = timer;
    }
    *(s32*)((s32)work + 0x1C) = 0;

    rad = float_6p2832_804289d4 * angle / float_360_804289f8;
    child = (void*)((s32)work + 0x24);
    delay = 0x10;
    for (i = 1; i < *(s32*)((s32)entry + 8); i++) {
        radius = float_16_804289fc * (f32)i;
        *(f32*)((s32)child + 4) = radius * cos(rad);
        *(f32*)((s32)child + 8) = radius * sin(rad);
        *(f32*)((s32)child + 0xC) = float_0_804289e4;
        *(f32*)((s32)child + 0x10) = float_1_804289e0 + float_0p2_80428a00 * (f32)i;
        *(s32*)((s32)child + 0x14) = 0;
        *(s32*)((s32)child + 0x20) = delay;
        delay += 0x10;
        child = (void*)((s32)child + 0x24);
    }
    return entry;
}

