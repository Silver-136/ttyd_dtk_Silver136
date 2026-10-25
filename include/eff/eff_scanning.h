#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct effScanningData {
    s32 type; //0x0
    Vec position; //0x4
    f32 scale; //0x10
    s32 animId; //0x14
    s32 state; //0x18
    s32 field_0x1C; //0x1C
    s32 field_0x20; //0x20
} effScanningData;

EffectEntry* effScanningEntry(s32 int2, f32 float3, f32 float4, f32 float5);
