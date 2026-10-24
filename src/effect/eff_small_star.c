#include "eff/eff_small_star.h"
#include "eff/n64/eff_small_star_n64.h"

EffectEntry* effSmallStarEntry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) {
    return effSmallStarN64Entry(type, count, x, y, z, dx, dy, dz);
}

void effSmallStarCamChg(EffectEntry* effect, s32 cameraId) {
    effSmallStarN64CamChg(effect, cameraId);
}
