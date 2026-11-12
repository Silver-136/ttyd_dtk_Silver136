#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct EffMagic1N64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec offset;   // 0x10
    Vec velocity; // 0x1C
    f32 scale;    // 0x28
    s32 age;      // 0x2C
    s32 life;     // 0x30
    s32 alpha;    // 0x34
} EffMagic1N64Work;

#include "libc_decls.h"

// sin of an angle given in degrees
static inline f32 effMagic1SinDeg(f32 deg) { return sin(6.2831855f * deg / 360.0f); }

EffectEntry* effMagic1N64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz);

extern GXColor magic1_zero_color;
