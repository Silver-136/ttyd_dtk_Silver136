// eff_break.c: break-banner effect: effBreakEntry, effBreakMain and effBreakDisp.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effBreakEntry(s32 type, s32 duration, f32 x, f32 y, f32 z);
