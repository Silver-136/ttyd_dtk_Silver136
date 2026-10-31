// eff_sunlight_n64.c: sunlight effect entry (effSunlightN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include "eff/eff_n64.h"

EffectEntry* effSunlightN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 w);

extern effN64Vtx testsrc__s_v[];
