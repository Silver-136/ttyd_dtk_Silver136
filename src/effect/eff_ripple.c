// Water ripple entry points: thin wrappers over the Paper Mario 64 ripple effect (effRippleN64*)
// that set its position, tilt (rx/rz) and camera.

#include "eff/eff_ripple.h"
#include "eff/n64/eff_ripple_n64.h"
#include <dolphin/mtx.h>
#include <string.h>

/**
 * Spawns a water ripple of the given type at (x, y, z) by creating the PM64-style ripple effect;
 * callers are Mario's water steps (mario_kemuri.c), Vivian and evt_eff ("ripple").
 */
EffectEntry* effRippleEntry(s32 type, f32 x, f32 y, f32 z) {
    return effRippleN64Entry(type, x, y, z, 0.0f);
}

/** Sets the ripple's tilt around X and Z in degrees (las_29 uses rz = 176 for a ripple on a tilted surface). */
void effRippleSetRxRz(EffectEntry* effect, f32 rx, f32 rz) {
    effRippleN64SetRxRz(effect, rx, rz);
}

/** Sets the camera the ripple is drawn for; Vivian's code uses it. */
void effRippleSetCamId(EffectEntry* effect, s32 cameraId) {
    ((EffRippleN64Work*)effect->userdata)->cameraId = cameraId;
}

/** Moves the ripple to (x, y, z); ignored unless the effect is a "RippleN64". */
void effRippleSetPosition(EffectEntry* effect, f32 x, f32 y, f32 z) {
    if (strcmp(effect->type, "RippleN64") == 0) {
        ((EffRippleN64Work*)effect->userdata)->position.x = x;
        ((EffRippleN64Work*)effect->userdata)->position.y = y;
        ((EffRippleN64Work*)effect->userdata)->position.z = z;
    }
}
