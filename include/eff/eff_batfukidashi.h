// eff_batfukidashi.c: the recovery block's coin price bubble (コイン吹き出し): effCoinFukidashiEntry, effCoinFukidashiMain and effCoinFukidashiDisp.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effCoinFukidashiEntry(f32 x, f32 y, f32 z, s32 type, s32 number, s32 duration);
