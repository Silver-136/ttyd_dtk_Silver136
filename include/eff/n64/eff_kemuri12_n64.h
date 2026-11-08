// eff_kemuri12_n64.c: smoke effect 12 entry (effKemuri12N64Entry).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>

typedef struct EffKemuri12N64Work {
    u16 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    s32 timer;      // 0x14
    s32 frame;      // 0x18
    s32 duration;   // 0x1C
    f32 progress;   // 0x20
    u8 r, g, b, a;  // 0x24
} EffKemuri12N64Work;

EffectEntry* effKemuri12N64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);
