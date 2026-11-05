// eff_glass_n64.c: Paper Mario 64 Glass effect entry (effGlassN64Entry) and its work data (EffGlassN64Work).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct EffGlassN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    s32 r1, g1, b1; // 0x18
    s32 alpha;     // 0x24
    s32 r2, g2, b2; // 0x28
    s32 alpha2;    // 0x34
    f32 scale;     // 0x38
    f32 rotZ;      // 0x3C
    f32 rotZSpeed;      // 0x40
    f32 texV;      // 0x44
    f32 texVSpeed; // 0x48
    f32 vx;        // 0x4C
    f32 vy;        // 0x50
    f32 vz;        // 0x54
    f32 gravity;   // 0x58
} EffGlassN64Work;

EffectEntry* effGlassN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
