#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>

typedef struct EffRippleN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    f32 scaleY;   // 0x14
    s32 alpha;    // 0x18
    s32 lifetime; // 0x1C
    s32 frame;    // 0x20
    s32 offset;   // 0x24
    f32 unk28;    // 0x28
    f32 rx;       // 0x2C
    f32 rz;       // 0x30
    s32 cameraId; // 0x34, dispEntry camera
} EffRippleN64Work;

EffectEntry* effRippleN64Entry(s32 type, f32 x, f32 y, f32 z, f32 yOffset);
void effRippleN64SetRxRz(EffectEntry* effect, f32 rx, f32 rz);
