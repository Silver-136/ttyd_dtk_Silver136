#include "eff/eff_confetti.h"
#include "eff/n64/eff_confetti_n64.h"

EffectEntry* effConfettiEntry(s32 type, s32 life, f32 x, f32 y, f32 z) {
    return effConfettiN64Entry(type, life, x, y, z, 1.0f);
}
