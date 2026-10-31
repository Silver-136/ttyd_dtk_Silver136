#include "eff/eff_small_star.h"
#include "eff/n64/eff_small_star_n64.h"

/**
 * Forwards to the Paper Mario 64 small-star effect.
 * Types (from the effect table): 0 no initial spin, 1 initial spin, 2 falling, 3 fan, 4 party;
 * `count` stars are emitted with the given position and direction.
 */
EffectEntry* effSmallStarEntry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) {
    return effSmallStarN64Entry(type, count, x, y, z, dx, dy, dz);
}

/** Forwards to the N64 effect's camera change, switching which camera the stars are drawn with. */
void effSmallStarCamChg(EffectEntry* effect, s32 cameraId) {
    effSmallStarN64CamChg(effect, cameraId);
}
