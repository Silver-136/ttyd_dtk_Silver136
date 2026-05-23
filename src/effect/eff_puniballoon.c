#include "effect/eff_puniballoon.h"

void effPuniBalloonDisp(void* camera, void* effect) {
    extern void balloon_0(void* camera, void* effect);
    extern void balloon_1(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    if (*(s32*)work == 0) {
        balloon_0(camera, effect);
    } else {
        balloon_1(camera, effect);
    }
}
