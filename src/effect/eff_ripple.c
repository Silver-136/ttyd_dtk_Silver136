#include "eff/eff_ripple.h"
#include "eff/n64/eff_ripple_n64.h"
#include <dolphin/mtx.h>
#include <string.h>

EffectEntry* effRippleEntry(s32 type, f32 x, f32 y, f32 z) {
    return effRippleN64Entry(type, x, y, z, 0.0f);
}

void effRippleSetRxRz(EffectEntry* effect, f32 rx, f32 rz) {
    effRippleN64SetRxRz(effect, rx, rz);
}

void effRippleSetCamId(EffectEntry* effect, s32 cameraId) {
    ((EffRippleN64Work*)effect->userdata)->cameraId = cameraId;
}

void effRippleSetPosition(EffectEntry* effect, f32 x, f32 y, f32 z) {
    if (strcmp(effect->type, "RippleN64") == 0) {
        ((EffRippleN64Work*)effect->userdata)->position.x = x;
        ((EffRippleN64Work*)effect->userdata)->position.y = y;
        ((EffRippleN64Work*)effect->userdata)->position.z = z;
    }
}
