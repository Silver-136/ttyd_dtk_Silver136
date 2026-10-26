#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effMagicHoraoN64Entry(s32 type, f32 x, f32 y, f32 z, f32 startSize, s32 startAlpha, f32 endSize, s32 endAlpha, f32 blendRate, s32 lifetime);
