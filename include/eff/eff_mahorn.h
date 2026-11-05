// eff_mahorn.c: Wizzerd (mahorn) effect entry (effMahornEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effMahornEntry(s32 type, s32 count, s32 duration, f32 x, f32 y, f32 z, f32 scale);
