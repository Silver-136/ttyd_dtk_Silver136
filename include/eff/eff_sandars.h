// eff_sandars.c: Sandars explosion effect entry (effSandarsEntry) and its work data (EffSandarsWork).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffSandarsWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 scale;     // 0x10
    s32 state;     // 0x14
    s32 frame;     // 0x18
    s32 poseId;    // 0x1C
    s32 alpha;     // 0x20
    s32 bombType;  // 0x24
    f32 bombScale; // 0x28
    f32 scale2;    // 0x2C
    f32 radius;    // 0x30
    s32 delay;     // 0x34
    f32 rot;       // 0x38
    f32 rotSpeed;  // 0x3C
    s32 camId;     // 0x40
} EffSandarsWork;

EffectEntry* effSandarsEntry(s32 type, f32 x, f32 y, f32 z, f32 radius);
