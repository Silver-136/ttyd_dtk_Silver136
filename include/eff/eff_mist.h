// eff_mist.c: mist effect entry (effMistEntry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effMistEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 yaw);
