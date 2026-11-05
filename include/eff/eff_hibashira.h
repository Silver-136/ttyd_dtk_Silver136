// eff_hibashira.c: fire-pillar effect: effHibashiraEntry, effHibashiraMain and effHibashiraDisp.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effHibashiraEntry(s32 type, s32 count, s32 duration, f32 x, f32 y, f32 z, f32 endX,
                               f32 endY, f32 endZ, f32 scale);
