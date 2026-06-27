#include "effect/n64/eff_stardust2_n64.h"

void effStardust2N64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x2C) = camId;
}


void effStardust2N64Entry(void) {
    ;
}


u8 effStardust2Main(void* effEntry) {
    return 0;
}


u8 effStardust2Disp(s32 param_1, void* effEntry) {
    return 0;
}
