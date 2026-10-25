#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effStardustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 a);
void effStardustN64SetColor(EffectEntry* effect, f32 r, f32 g, f32 b);
void effStardustN64SetDrawCam(EffectEntry* effect, s32 camera);
void effStardustN64SetScale(EffectEntry* effect, f32 scale);
