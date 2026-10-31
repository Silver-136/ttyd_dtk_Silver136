// eff_itemget.c: item-get sparkles (アイテム取得): 7 sparkles for types 0, 3 and 9 (item obtained), 5 for type 10 (item used in battle). effItemGetEntry and ItemGetWork.

#include "driver/camdrv.h"
#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct ItemGetWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    f32 size;     // 0x14
    s32 lifetime; // 0x18
    s32 timer;    // 0x1C
    s32 state;    // 0x20
    Vec velocity; // 0x24
    f32 angle;    // 0x30
    s32 alpha;    // 0x34
    CameraId camera;   // 0x38
} ItemGetWork;

EffectEntry* effItemGetEntry(s32 type, f32 x, f32 y, f32 z);
