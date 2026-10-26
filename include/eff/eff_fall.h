#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"

s32 effFallCheckHit(EffectEntry* effect);
EffectEntry* effFallEntry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 drop, f32 dir, s32 num, s32 duration);
