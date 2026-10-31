// eff_nokotarou.c: Koops (nokotarou) effect entry (effNokotarouEntry), its work struct and main/draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

typedef struct EffNokotarouWork {
    s32 type;       // 0x0
    Vec start;      // 0x4
    Vec position;   // 0x10
    f32 size;       // 0x1C
    f32 scale;      // 0x20, eases in and out
    s32 alpha;      // 0x24
    s32 timer;      // 0x28, counts down while fading out
    s32 pad_2C; // 0x2C, never accessed
    s32 poseId;     // 0x30
    s32 poseId2;    // 0x34, stretched between start and position
} EffNokotarouWork;

EffectEntry* effNokotarouEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size);
void effNokotarouMain(EffectEntry* effect);
void effNokotarouDisp(CameraId cameraId, EffectEntry* effect);
