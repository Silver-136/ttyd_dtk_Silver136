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


/* CHATGPT STUB FILL: main/effect/eff_starstone 20260624_184128 */

/* stub-fill: effStarStoneDisp_2 | prototype_only | source_prototype */
void effStarStoneDisp_2(void* camera, void* effect) {
    return;
}

/* stub-fill: effStarStoneDisp_1 | prototype_only | source_prototype */
void effStarStoneDisp_1(void* camera, void* effect) {
    return;
}

/* stub-fill: effStarStoneMain | missing_definition | ghidra_signature */
u8 effStarStoneMain(s32 effEntry) {
    return 0;
}

/* stub-fill: effStarStoneEntry | missing_definition | ghidra_signature */
u8 effStarStoneEntry(s64 xPos, s64 yPos, s64 zPos, s64 param_4, s32 param_5) {
    return 0;
}
