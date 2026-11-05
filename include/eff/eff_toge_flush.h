// eff_toge_flush.c: spike flash effect entry (effTogeFlushEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effTogeFlushEntry(s32 type, f32 x, f32 y, f32 z, s32 life);
