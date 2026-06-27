#include "effect/eff_kemutest.h"

void effKemuTestSetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x50) = rx;
    *(f32*)((s32)work + 0x54) = rz;
}

void effKemuTestDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = camId;
}



/* CHATGPT STUB FILL: main/effect/eff_kemutest 20260624_184128 */

/* stub-fill: effKemuTestMain | missing_definition | ghidra_signature */
u8 effKemuTestMain(s32 effEntry) {
    return 0;
}


/* CHATGPT STUB FILL: main/effect/eff_kemutest 20260624_184128 */

/* stub-fill: effKemuTestEntry | missing_definition | ghidra_signature */
u8 effKemuTestEntry(void) {
    return 0;
}


/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_kemutest 20260624_191429 */

/* fallback stub-fill: map=effKemuTestDisp addr=0x8020cf24 size=0x00000730 */
int effKemuTestDisp() {
    return 0;
}
