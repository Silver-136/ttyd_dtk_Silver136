// Butterfly: forwards to the Paper Mario 64 butterfly effect (effButterflyN64Entry).

#include "eff/eff_butterfly.h"
#include "eff/n64/eff_butterfly_n64.h"

/** Spawns butterflies at (x, y, z) by forwarding to the Paper Mario 64 effect; spawned by event scripts in nok. */
EffectEntry* effButterflyEntry(s32 type, f32 x, f32 y, f32 z) {
    return effButterflyN64Entry(type, x, y, z);
}
