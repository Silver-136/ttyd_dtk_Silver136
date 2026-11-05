// eff_indirect.c: screen-distortion effect: effIndirectEntry, effIndirectMain and effIndirectDisp.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effIndirectEntry(s32 type, s32 duration, f32 x, f32 y, f32 z);
