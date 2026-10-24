#include "eff/eff_hit.h"
#include "eff/n64/eff_hit_n64.h"

EffectEntry* effHitEntry(s32 type, s32 size, f32 x, f32 y, f32 z, f32 scale) {
    return effHitN64Entry(type, size, x, y, z, scale);
}
