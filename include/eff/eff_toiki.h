// eff_toiki.c: the Shadow Queen's breath (影の女王・吐息): a drifting cloud with a scrolling texture and mist puffs (unit_boss_black_peach.c). effToikiEntry with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effToikiEntry(s32 type, s32 duration, f32 x, f32 y, f32 z);
void effToikiMain(EffectEntry* effect);
void effToikiDisp(CameraId cameraId, EffectEntry* effect);
