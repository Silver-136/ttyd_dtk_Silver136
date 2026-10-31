// eff_batten.c: Magnus von Grapple 2.0 effects (バッテンダーエックス): types 1-2 the target mark (EFF_batten_x), type 3 the rocket jets; effBattenEntry and EffBattenWork.

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"

EffectEntry* effBattenEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);
