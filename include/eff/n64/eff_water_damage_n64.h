// eff_water_damage_n64.c: water damage effect entry (effWaterDamageN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effWaterDamageN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
