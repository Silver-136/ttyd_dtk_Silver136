// eff_kiss.c: kiss effect (チュチュリーナキス, Ms. Mowz's kiss), also used by other partners' battle moves and by
// field scenes; effKissEntry.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effKissEntry(s32 type, f32 x, f32 y, f32 z, f32 angle);
