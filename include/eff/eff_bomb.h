// eff_bomb.c: bomb blast (ボム): eight rays at 45-degree steps that stretch along two scale tables, plus confetti; 36 frames. effBombEntry.

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effBombEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
