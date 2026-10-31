// eff_charge.c: battle charge (バトル・チャージ): 30 sparkles spiralling in to the unit; effChargeEntry.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effChargeEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);
