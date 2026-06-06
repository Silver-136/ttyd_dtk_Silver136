#include "effect/eff_fall.h"

s32 effFallCheckHit(void* eff) {
    void* work = *(void**)((s32)eff + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 1:
        case 4:
        case 5:
            if (*(s32*)((s32)work + 0xA0) >= 2) {
                return 1;
            }
            break;
        case 2:
        case 3:
            if (*(s32*)((s32)work + 0xA0) >= 1) {
                return 1;
            }
            break;
        case 6:
            if (*(s32*)((s32)work + 0xA0) < 2) {
                if (*(s32*)((s32)work + 0xA0) != 1) {
                    break;
                }
                if (*(s32*)((s32)work + 0x8C) <= 90) {
                    break;
                }
            }
            return 1;
        case 7:
            if (*(s32*)((s32)work + 0xA0) >= 4) {
                return 1;
            }
            break;
        case 8:
            if (*(s32*)((s32)work + 0xA0) >= 3) {
                return 1;
            }
            break;
    }
    return 0;
}

