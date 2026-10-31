// eff_pokopi_pcharge_n64.c: pokopi charge effect entry (effPokopiPchargeN64Entry).

#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffPokopiPchargeN64Elem {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 r;        // 0x18
    s32 g;        // 0x1C
    s32 b;        // 0x20
    s32 alpha;    // 0x24
    s32 r2;       // 0x28
    s32 g2;       // 0x2C
    s32 b2;       // 0x30
    f32 scale;    // 0x34
    f32 speed;    // 0x38
    f32 rotation; // 0x3C
    f32 rotVel;   // 0x40
    f32 mult;     // 0x44
} EffPokopiPchargeN64Elem;

EffectEntry* effPokopiPchargeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
