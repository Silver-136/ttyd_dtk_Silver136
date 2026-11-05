// eff_levelup.c: level-up effect entry (effLevelupEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effLevelupEntry(s32 type, f32 x, f32 y, f32 z, s32 limit);
