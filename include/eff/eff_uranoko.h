// eff_uranoko.c: uranoko effect entry (effUranokoEntry), its callbacks and work data (EffUranokoWork).

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

typedef struct EffUranokoWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10, eases towards 1.0
    f32 baseScale;  // 0x14
    s32 frame;      // 0x18
    s32 timer;      // 0x1C, counts down while fading out
    s32 poseId;     // 0x20
    s32 alpha;      // 0x24
} EffUranokoWork;

EffectEntry* effUranokoEntry(s32 type, s32 duration, f32 x, f32 y, f32 z);
