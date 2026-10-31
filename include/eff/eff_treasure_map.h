// eff_treasure_map.c: treasure map effect entry (effTreasureMapEntry) and work data (EffTreasureMapWork).

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

typedef struct EffTreasureMapWork {
    s32 type;          // 0x0
    Vec position;      // 0x4
    f32 scale;         // 0x10
    s32 alpha;         // 0x14
    s32 timer;         // 0x18, counts down while fading out
    s32 frame;         // 0x1C
    f32 rotY;          // 0x20
    s32 poseId;        // 0x24
    CameraId cameraId; // 0x28
} EffTreasureMapWork;

EffectEntry* effTreasureMapEntry(s32 type, f32 x, f32 y, f32 z);
void effTreasureMapMain(EffectEntry* effect);
void effTreasureMapDisp(CameraId cameraId, EffectEntry* effect);
