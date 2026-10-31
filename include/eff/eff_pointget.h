// eff_pointget.c: point-get effect entry (effPointGetEntry).

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"

EffectEntry* effPointGetEntry(s32 type, f32 x, f32 y, f32 z, s32 number, s32 duration);
