#pragma once

#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct effSleepData {
    s32 type; //0x0
    Vec position; //0x4
    f32 scale; //0x10
    s32 alpha; //0x14, fades in over 32 frames, out over the last 32
    s32 lifetime; //0x18, frames left (1000 = endless until effect flag 4)
    s32 age; //0x1C
    s32 delay; //0x20, per-bubble stagger (16, 32, ...)
} effSleepData;

EffectEntry* effSleepEntry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale, f32 rotation);
void effSleepMain(EffectEntry* effect);
