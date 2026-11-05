// eff_rankup.c: rank-up banner (ランクアップ), used by ItemEvent_Teki_Kyouka; effRankupEntry with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effRankupEntry(s32 type, f32 x, f32 y, f32 z);
