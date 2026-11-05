// eff_starpoint.c: Star Points effect entry (effStarPointEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effStarPointEntry(s32 type, s32 count, f32 x, f32 y, f32 z);
