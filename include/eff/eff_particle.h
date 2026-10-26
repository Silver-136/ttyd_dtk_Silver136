#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effParticleEntry(s32 type, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 count, s32 life);
