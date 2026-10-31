// eff_sweat_n64.c: Paper Mario 64 sweat drops flung out in a fan, 6 (type 0) or 4 (type 1); effSweatN64Entry and EffSweatN64Work.

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct EffSweatN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 rot;       // 0x10
    f32 unk_14;    // 0x14
    Vec velocity;  // 0x18
    s32 life;      // 0x24
    s32 age;       // 0x28
    s32 delay;     // 0x2C
    s32 alpha;     // 0x30
} EffSweatN64Work;

EffectEntry* effSweatN64Entry(s32 type, f32 x, f32 y, f32 z, f32 base, f32 angle, s32 life);
