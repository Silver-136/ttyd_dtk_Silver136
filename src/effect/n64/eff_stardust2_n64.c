#include "effect/n64/eff_stardust2_n64.h"

void effStardust2N64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x2C) = camId;
}
