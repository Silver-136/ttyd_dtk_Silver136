#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>

typedef struct EffBreathFireN64Work {
    s32 type;       // 0x0
    s32 count;      // 0x4
    s32 seed;       // 0x8
    Vec pos;        // 0xc
    Vec start;      // 0x18
    Vec end;        // 0x24
    f32 baseScale;      // 0x30
    f32 targetScale;      // 0x34
    f32 scale;      // 0x38
    f32 scaleRate;      // 0x3c
    s32 alpha;      // 0x40
    s32 lifetime;   // 0x44
    s32 remaining;  // 0x48
    s32 frame;      // 0x4c
    Vec vel;        // 0x50
    f32 texFrame;      // 0x5c
    f32 velY;       // 0x60
    s32 color[6];   // 0x64
} EffBreathFireN64Work;

EffectEntry* effBreathFireN64Entry(s32 type, s32 count, s32 seed, s32 lifetime, f32 x, f32 y, f32 z,
                                   f32 endX, f32 endY, f32 endZ);
