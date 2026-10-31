// eff_crystal_n64.c: Paper Mario 64 Crystal effect entry (effCrystalN64Entry) and its work data (EffCrystalN64Work).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffCrystalN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 reserved_18;    // 0x18, only set to 0
    s32 variant;  // 0x1C
    s32 r;        // 0x20
    s32 g;        // 0x24
    s32 b;        // 0x28
    s32 a;        // 0x2C
    f32 scale;    // 0x30
    f32 scaleVel; // 0x34
    s32 camera;   // 0x38
} EffCrystalN64Work;

EffectEntry* effCrystalN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
