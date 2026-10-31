// eff_vapor_n64.c: vapor effect entry (effVaporN64Entry) and its work data (EffVaporN64Work).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffVaporN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    s32 r1;        // 0x18
    s32 g1;        // 0x1C
    s32 b1;        // 0x20
    s32 alpha;     // 0x24
    s32 r2;        // 0x28
    s32 g2;        // 0x2C
    s32 b2;        // 0x30
    s32 a2;        // 0x34
    f32 anim;      // 0x38
    f32 vel;       // 0x3C
    f32 scale;     // 0x40
} EffVaporN64Work;

EffectEntry* effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
