// eff_opuku_jetw_n64.c: water-jet effect entry (effOpukuJetwN64Entry).

#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffOpukuJetwN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 colorR;   // 0x18
    s32 colorG;   // 0x1C
    s32 colorB;   // 0x20
    s32 alpha;    // 0x24
    s32 color2R;  // 0x28
    s32 color2G;  // 0x2C
    s32 color2B;  // 0x30
    f32 scale;    // 0x34
    f32 rotZ;     // 0x38
    f32 scaleX;   // 0x3C
    f32 scaleY;   // 0x40
} EffOpukuJetwN64Work;

EffectEntry* effOpukuJetwN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
