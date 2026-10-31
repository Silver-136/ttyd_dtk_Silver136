// eff_ripple.c: water ripple entry (effRippleEntry) and setters for rx/rz, camera and position.

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effRippleEntry(s32 type, f32 x, f32 y, f32 z);
void effRippleSetRxRz(EffectEntry* effect, f32 rx, f32 rz);
void effRippleSetCamId(EffectEntry* effect, s32 cameraId);
void effRippleSetPosition(EffectEntry* effect, f32 x, f32 y, f32 z);
