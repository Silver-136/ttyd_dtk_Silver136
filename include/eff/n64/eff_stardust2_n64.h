// eff_stardust2_n64.c: stardust entry (effStardust2N64Entry) and draw camera setter (effStardust2N64SetDrawCam).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effStardust2N64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 vx,
                                  f32 vy);
void effStardust2N64SetDrawCam(EffectEntry* effect, s32 camera);
