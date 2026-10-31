// eff_stone_smoke_n64.c: stone smoke entry (effStoneSmokeN64Entry) and work data (EffStoneSmokeN64Work).

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct EffStoneSmokeN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 timer;     // 0x10
    s32 frame;     // 0x14
    s32 r;         // 0x18
    s32 g;         // 0x1C
    s32 b;         // 0x20
    s32 alpha;     // 0x24
    s32 r2;        // 0x28
    s32 g2;        // 0x2C
    s32 b2;        // 0x30
    f32 baseScale; // 0x34
    f32 scale;     // 0x38
} EffStoneSmokeN64Work;

EffectEntry* effStoneSmokeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
