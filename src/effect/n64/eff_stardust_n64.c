#include "effect/n64/eff_stardust_n64.h"

void effStardustN64SetScale(void* effect, f32 scale) {
    *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = scale;
}

void effStardustN64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x3C) = camId;
}

void effStardustN64SetColor(void* effect, f32 r, f32 g, f32 b) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x28) = r;
    *(f32*)((s32)work + 0x2C) = g;
    *(f32*)((s32)work + 0x30) = b;
}

