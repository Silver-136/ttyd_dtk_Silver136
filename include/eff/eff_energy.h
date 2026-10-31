// eff_energy.c: energy-orb effect: effEnergyEntry, effEnergyMain and effEnergyDisp.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effEnergyEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size, f32 rotSpeed,
                            f32 scaleSpeed);
void effEnergyMain(EffectEntry* effect);
void effEnergyDisp(CameraId cameraId, EffectEntry* effect);
