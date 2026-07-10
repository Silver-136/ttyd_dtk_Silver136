#include "effect/eff_ripple.h"

void* effRippleN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 time);
void effRippleN64SetRxRz(void* effect, f32 rx, f32 rz);
extern char str_RippleN64_802f9bd8[];
int strcmp(const char* s1, const char* s2);

void effRippleSetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x34) = camId;
}

void effRippleSetRxRz(void* effect, f32 rx, f32 rz) {
    effRippleN64SetRxRz(effect, rx, rz);
}

void* effRippleEntry(f32 x, f32 y, f32 z, f32 scale, s32 time) {
    return effRippleN64Entry(x, y, z, 0.0f, time);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effRippleSetPosition(void* effect, f32 x, f32 y, f32 z) {
    if (strcmp(*(char**)((s32)effect + 0x14), str_RippleN64_802f9bd8) == 0) {
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x4) = x;
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0x8) = y;
        *(f32*)((s32)*(void**)((s32)effect + 0xC) + 0xC) = z;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

