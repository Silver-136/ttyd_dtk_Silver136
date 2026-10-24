#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effBreathFireEntry(s32 type, s32 count, s32 seed, s32 lifetime, f32 x, f32 y, f32 z,
                                f32 endX, f32 endY, f32 endZ);
