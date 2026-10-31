// eff_confetti_n64.c: Paper Mario 64 Confetti effect entry (effConfettiN64Entry).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effConfettiN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale);
void effConfettiN64SetCamId(EffectEntry* effect, s32 camId);
