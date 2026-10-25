#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct PuniBalloonWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    f32 baseScale;
    f32 scale;    // 0x20
    f32 rotation; // 0x24
    s32 life;     // 0x28
    s32 frame;    // 0x2C
} PuniBalloonWork;

EffectEntry* effPuniBalloonEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
