#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffShockN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 c10;      // 0x10
    s32 flashAlpha; // 0x14, TEV alpha of the yellow flash drawn for type 1; starts 255, *= 0.9 per frame
    s32 alpha;    // 0x18, TEV alpha of col0; starts 255, *= 0.8 per frame in the last 5 frames
    s32 life;     // 0x1c
    s32 age;      // 0x20
    f32 scale;    // 0x24
    f32 offset;   // 0x28
    f32 offset2;  // 0x2c
    s32 col0[3];  // 0x30
    s32 col1[3];  // 0x3c
} EffShockN64Work;

EffectEntry* effShockN64Entry(s32 type, f32 x, f32 y, f32 z, f32 a, f32 b);
