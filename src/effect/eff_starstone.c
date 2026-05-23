#include "effect/eff_starstone.h"

void effStarStoneDraw(void* camera, void* effect) {
    extern void effStarStoneDisp_1(void* camera, void* effect);
    extern void effStarStoneDisp_2(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 6:
            effStarStoneDisp_1(camera, effect);
            break;
        default:
            effStarStoneDisp_2(camera, effect);
            break;
    }
}
