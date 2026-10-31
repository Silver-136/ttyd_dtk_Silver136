// eff_whirlwind_n64.c: whirlwind effect entry (effWhirlwindN64Entry) and its work data (EffWhirlwindN64Work).

#pragma once

#include "driver/effdrv.h"
#include "mario/mario.h"
#include <dolphin/mtx.h>

typedef struct EffWhirlwindN64Work {
    s32 type;                // 0x0
    MarioWork* target;       // 0x4, followed actor (position and collision-box height are read)
    Vec position;            // 0x8
    f32 scale;               // 0x14
    f32 baseScale;           // 0x18
    s32 alpha;               // 0x1C
    s32 duration;            // 0x20
    s32 life;                // 0x24
    s32 frame;               // 0x28
    Vec rot;                 // 0x2C
    Vec vel;                 // 0x38
    u8 r, g, b, a;           // 0x44
} EffWhirlwindN64Work;

EffectEntry* effWhirlwindN64Entry(s32 type, MarioWork* target, f32 scale, s32 life);
