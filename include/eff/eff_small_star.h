#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effSmallStarEntry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy,
                               f32 dz);
void effSmallStarCamChg(EffectEntry* effect, s32 cameraId);
