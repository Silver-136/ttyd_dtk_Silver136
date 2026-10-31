// Fire breath: forwards to the Paper Mario 64 fire-breath effect (effBreathFireN64Entry).

#include "eff/eff_breath_fire.h"
#include "eff/n64/eff_breath_fire_n64.h"

/**
 * Spawns the fire-breath effect from (x, y, z) toward (endX, endY, endZ); just calls effBreathFireN64Entry.
 * Used by the script effect command in evt_eff.c.
 */
EffectEntry* effBreathFireEntry(s32 type, s32 count, s32 seed, s32 lifetime, f32 x, f32 y, f32 z, f32 endX,
                                f32 endY, f32 endZ) {
    return effBreathFireN64Entry(type, count, seed, lifetime, x, y, z, endX, endY, endZ);
}
