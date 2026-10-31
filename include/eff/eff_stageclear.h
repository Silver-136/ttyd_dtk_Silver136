// eff_stageclear.c: stage clear banner effect entry (effStageClearEntry) and its end check (effStageClearEndChk).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effStageClearEntry(s32 type, f32 x, f32 y, f32 z);

s32 effStageClearEndChk(EffectEntry* effect);
