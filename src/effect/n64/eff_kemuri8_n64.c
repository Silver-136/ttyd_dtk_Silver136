#include "effect/n64/eff_kemuri8_n64.h"

void effKemuri8N64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x58) = camId;
}
