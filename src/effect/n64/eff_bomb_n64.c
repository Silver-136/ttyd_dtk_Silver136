#include "effect/n64/eff_bomb_n64.h"

void effBombN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x40) = camId;
}
