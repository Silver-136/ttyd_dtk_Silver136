// eff_laser.c: laser effect entry (effLaserEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effLaserEntry(s32 type, f32 x, f32 y, f32 z);
void effLaserMain(EffectEntry* effect);
void effLaserDisp(CameraId cameraId, EffectEntry* effect);
