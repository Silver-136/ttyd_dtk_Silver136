#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effStardust2N64Entry(s32 type, s32 count, s32 camera, f32 x, f32 y, f32 z, f32 vx,
                                  f32 vy);
EffectEntry* effStardust2N64Entry(s32 type, s32 count, s32 camera, f32 x, f32 y, f32 z, f32 vx, f32 vy);
void effStardust2N64SetDrawCam(EffectEntry* effect, s32 camera);
