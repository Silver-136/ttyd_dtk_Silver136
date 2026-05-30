#include "effect/n64/eff_kemuri8_n64.h"

void effKemuri8N64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x58) = camId;
}


u8 effKemuri8Disp(void) {
    return 0;
}


u8 effKemuri8N64Entry(void) {
    return 0;
}


u8 effKemuri8Main(int* param_1) {
    return 0;
}
