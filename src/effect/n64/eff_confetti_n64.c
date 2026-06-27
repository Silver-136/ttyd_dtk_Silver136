#include "effect/n64/eff_confetti_n64.h"

void effConfettiN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x34) = camId;
}


void effConfettiN64Entry(void) {
    ;
}


u8 effConfettiMain(int* param_1) {
    return 0;
}


u8 effConfettiDisp(int param_1, int param_2) {
    return 0;
}
