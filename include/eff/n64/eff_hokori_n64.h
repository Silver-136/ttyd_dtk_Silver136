// eff_hokori_n64.c: Paper Mario 64 Hokori effect entry (effHokoriN64Entry) and its work data (EffHokoriN64Work).

#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffHokoriN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 velY;     // 0x10
    f32 gravity;  // 0x14
    s32 alpha;    // 0x18
    f32 scaleY;   // 0x1C
    f32 scaleX;   // 0x20
    f32 growth;   // 0x24
    f32 growth2;  // 0x28
    s32 lifetime; // 0x2C
    s32 r;        // 0x30
    s32 g;        // 0x34
    s32 b;        // 0x38
    s32 r2;       // 0x3C
    s32 g2;       // 0x40
    s32 b2;       // 0x44
} EffHokoriN64Work;

EffectEntry* effHokoriN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z);
