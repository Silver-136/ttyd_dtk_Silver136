#include "effect/n64/eff_small_star_n64.h"

void effSmallStarN64CamChg(void* effect, s8 camId) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 i;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x34)) {
        *(s8*)((s32)work + 0x30) = camId;
    }
}
