// eff_torch.c: torch flame effect entry (effTorchEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effTorchEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
