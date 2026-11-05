// eff_lensflare_n64.c: lens flare effect entry (effLensflareN64Entry).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

// unused: passed by evt_eff (its sixth float), never read
EffectEntry* effLensflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 unused);
