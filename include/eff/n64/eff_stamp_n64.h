// eff_stamp_n64.c: stamp effect entry (effStampN64Entry) and its work data (EffStampN64Work).

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct EffStampN64Work {
    s16 type;      // 0x0
    Vec position;  // 0x4
    Vec velocity;  // 0x10
    f32 scaleY;    // 0x1C
    f32 scaleX;    // 0x20
    s32 lifetime;  // 0x24
    s32 frame;     // 0x28
    f32 angle;     // 0x2C
    f32 radius;    // 0x30
    s32 alpha;     // 0x34
    u8 r, g, b;    // 0x38
    u8 r2, g2, b2; // 0x3B
} EffStampN64Work;

EffectEntry* effStampN64Entry(s32 type, f32 x, f32 y, f32 z);
