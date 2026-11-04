// eff_kemuri10_n64.c: smoke effect 10 entry (effKemuri10N64Entry).

#pragma once

#include "version.h"
#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffKemuri10N64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    Vec velocity;   // 0x10
    Mtx rot;        // 0x1C
    f32 texOffset;  // 0x4C
    f32 texSpeed;   // 0x50
    f32 scale;      // 0x54
    s32 alpha;      // 0x58
    s32 timer;      // 0x5C
    s32 counter;    // 0x60
    u8 r, g, b;     // 0x64
    u8 r2, g2, b2;  // 0x67
    u8 pad_6a[2];   // 0x6A
#if VERSION >= VERSION_JP
    s32 camera;     // 0x6C
#endif
} EffKemuri10N64Work;

EffectEntry* effKemuri10N64Entry(s32 type, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, f32 scale);
