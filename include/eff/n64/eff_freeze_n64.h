// eff_freeze_n64.c: Paper Mario 64 Freeze effect entry (effFreezeN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effFreezeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
