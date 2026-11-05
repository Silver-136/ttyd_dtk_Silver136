// Stardust: wrapper over the Paper Mario 64 stardust2 effect (effStardust2N64Entry).

#include "eff/eff_stardust.h"
#include "eff/n64/eff_stardust2_n64.h"

/**
 * Forwards to the Paper Mario 64 stardust2 effect.
 * Type selects the look (see the evt_eff names: coin get, HP recovery, ...);
 * `lifetime` is the effect lifetime in frames, passed on to effStardust2N64Entry;
 * (x, y, z) is the centre and vx/vy the spread.
 */
EffectEntry* effStardustEntry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 vx, f32 vy) {
    return effStardust2N64Entry(type, count, lifetime, x, y, z, vx, vy);
}
