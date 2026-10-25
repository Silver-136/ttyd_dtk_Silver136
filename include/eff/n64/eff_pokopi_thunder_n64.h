#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffPokopiThunderN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec start;    // 0x10
    Vec end;      // 0x1C
    s32 lifetime; // 0x28
    s32 frame;    // 0x2C
    s32 r;        // 0x30
    s32 g;        // 0x34
    s32 b;        // 0x38
    s32 alpha;    // 0x3C
    s32 r2;       // 0x40
    s32 g2;       // 0x44
    s32 b2;       // 0x48
    f32 scale;    // 0x4C
    f32 x[12];    // 0x50
    f32 y[12];    // 0x80
    f32 z[12];    // 0xB0
    f32 w[12];    // 0xE0
    s32 seed;     // 0x110
    f32 length;   // 0x114
    s32 cameraId; // 0x118
} EffPokopiThunderN64Work;

EffectEntry* effPokopiThunderN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 x2, f32 y2, f32 z2, f32 scale);
