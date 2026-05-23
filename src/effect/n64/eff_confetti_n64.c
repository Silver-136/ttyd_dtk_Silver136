#include "effect/n64/eff_confetti_n64.h"

void effConfettiN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x34) = camId;
}
