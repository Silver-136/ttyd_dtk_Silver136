// eff_rubble_n64.c: rubble chunk effect entry (effRubbleN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effRubbleN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 groundY, f32 velY, f32 gravity, f32 scale);
