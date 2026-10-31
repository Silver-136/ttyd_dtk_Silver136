// eff_spirit.c: spirit orb effect entry (effSpiritEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effSpiritEntry(s32 type, s32 time, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 scale);
void effSpiritMain(EffectEntry* effect);
void effSpiritDisp(CameraId cameraId, EffectEntry* effect);
