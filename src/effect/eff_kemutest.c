#include "effect/eff_kemutest.h"

void effKemuTestSetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x50) = rx;
    *(f32*)((s32)work + 0x54) = rz;
}

void effKemuTestDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = camId;
}

