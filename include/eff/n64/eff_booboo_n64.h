// eff_booboo_n64.c: Paper Mario 64 booboo effect entry (effBoobooN64Entry). The entry ignores its arguments; evt_eff
// passes the same six as to its siblings (effGlassN64Entry, effPikkyoloN64Entry), whose names the parameters take.

#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effBoobooN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
