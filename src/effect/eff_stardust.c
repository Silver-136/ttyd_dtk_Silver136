#include "eff/eff_stardust.h"
#include "eff/n64/eff_stardust2_n64.h"

EffectEntry* effStardustEntry(s32 type, s32 count, s32 camera, f32 x, f32 y, f32 z, f32 vx, f32 vy) {
    return effStardust2N64Entry(type, count, camera, x, y, z, vx, vy);
}
