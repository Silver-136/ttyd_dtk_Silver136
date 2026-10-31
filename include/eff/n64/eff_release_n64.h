// eff_release_n64.c: glowing orb effect entry (effReleaseN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effReleaseN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
