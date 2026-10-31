// eff_blow_n64.c: Paper Mario 64 Blow effect entry (effBlowN64Entry).

#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"

EffectEntry* effBlowN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz);
