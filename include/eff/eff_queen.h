// eff_queen.c: Shadow Queen tornado effect entry (effQueenEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effQueenEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size);
