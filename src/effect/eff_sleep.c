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
