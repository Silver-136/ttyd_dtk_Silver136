// eff_snow_dust_n64.c: snow dust effect entry (effSnowDustN64Entry) and its work data (EffSnowDustN64Work).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffSnowDustN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 width;    // 0x10
    f32 height;   // 0x14
    f32 vx;       // 0x18
    f32 vy;       // 0x1C
    f32 pad_20;    // 0x20, never accessed
    f32 tx;       // 0x24
    f32 ty;       // 0x28
    f32 pad_2C;    // 0x2C, never accessed
    s32 seq;      // 0x30
    s32 lifetime; // 0x34
    s32 frame;    // 0x38
    s32 r;        // 0x3C
    s32 g;        // 0x40
    s32 b;        // 0x44
    s32 alpha;    // 0x48
    s32 kr;       // 0x4C
    s32 kg;       // 0x50
    s32 kb;       // 0x54
    s32 ka;       // 0x58
    f32 scale;    // 0x5C
    f32 u;        // 0x60
    f32 v;        // 0x64
    s32 camId;    // 0x68
} EffSnowDustN64Work;

EffectEntry* effSnowDustN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 w, f32 h, f32 scale);
