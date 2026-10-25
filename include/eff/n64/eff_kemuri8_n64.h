#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z);
void effKemuri8N64SetCamId(EffectEntry* effect, s32 camId);
