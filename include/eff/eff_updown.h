// eff_updown.c: up/down number pop-up effect entry (effUpdownEntry).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effUpdownEntry(s32 type, f32 x, f32 y, f32 z, s32 number, s32 timer);
