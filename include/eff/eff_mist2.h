// eff_mist2.c: black mist (黒い霧) in las and around the Shadow Queen (also the smoke in tik_05); effMist2Entry and EffMist2Work.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct EffMist2Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 pad10;     // 0x10
    f32 scaleTarget; // 0x14
    f32 scale;     // 0x18
    f32 spawnRadius;         // 0x1C
    f32 spawnHeight;         // 0x20
    f32 rot;       // 0x24
    f32 rotSpeed;  // 0x28
    f32 alpha;     // 0x2C
    s32 timer;     // 0x30
    s32 life;      // 0x34
    u8 r, g, b8;   // 0x38
    u8 pad;
    s32 delay;     // 0x3C
    f32 gravity;     // 0x40
    f32 vy;        // 0x44
} EffMist2Work;

EffectEntry* effMist2Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 a, f32 b);
