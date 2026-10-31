// eff_mizutama.c: underwater bubbles (水中の泡) released at an angle, slowing and drifting up; Bowser swimming (koopa_motion.c). effMizutamaEntry with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effMizutamaEntry(s32 type, f32 x, f32 y, f32 z, f32 angle);
void effMizutamaMain(EffectEntry* effect);
void effMizutamaDisp(CameraId cameraId, EffectEntry* effect);
