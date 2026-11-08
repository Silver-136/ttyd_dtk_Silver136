// eff_cloud_n64.c: Paper Mario 64 Cloud effect entry (effCloudN64Entry) and its work data (EffCloudN64Work).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct EffCloudN64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    s32 lifetime;   // 0x10
    s32 frame;      // 0x14
    s32 r;          // 0x18
    s32 g;          // 0x1C
    s32 b;          // 0x20
    s32 fade;       // 0x24
    s32 r2;         // 0x28
    s32 g2;         // 0x2C
    s32 b2;         // 0x30
    f32 scale;      // 0x34
    Vec pos;        // 0x38
    Vec vel;        // 0x44
    f32 trailX[30]; // 0x50
    f32 trailY[30]; // 0xC8
    f32 trailZ[30]; // 0x140
    s32 age[30];    // 0x1B8
    s32 active[30]; // 0x230
    s32 head;       // 0x2A8
    f32 len[30];    // 0x2AC
} EffCloudN64Work;

EffectEntry* effCloudN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
