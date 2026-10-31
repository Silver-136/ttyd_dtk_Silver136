// eff_miss_star.c: small-star effect entry (effMissStarEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effMissStarEntry(s32 count, s32 delay, f32 x, f32 y, f32 z, s32 index);
void effMissStarMain(EffectEntry* effect);
void effMissStarDisp(CameraId cameraId, EffectEntry* effect);
