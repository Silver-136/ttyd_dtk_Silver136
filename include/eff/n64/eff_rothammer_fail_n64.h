// eff_rothammer_fail_n64.c: hammer failure smoke effect entry (effRotHammerFailN64Entry).

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

// unused: passed by evt_eff (its sixth float), never read
EffectEntry* effRotHammerFailN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 unused);
