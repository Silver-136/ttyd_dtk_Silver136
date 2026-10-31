// Confetti: forwards to the Paper Mario 64 confetti effect (effConfettiN64Entry) with scale 1.

#include "eff/eff_confetti.h"
#include "eff/n64/eff_confetti_n64.h"

/** Spawns a confetti burst at (x, y, z) via the PM64 effect (scale 1). Spawned by event scripts in aji, hei, tou. */
EffectEntry* effConfettiEntry(s32 type, s32 life, f32 x, f32 y, f32 z) {
    return effConfettiN64Entry(type, life, x, y, z, 1.0f);
}
