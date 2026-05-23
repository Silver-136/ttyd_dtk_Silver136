#include "window/win_party.h"

void winPartyMain2(void* work) {
    extern f32 float_0p25_804235f8;
    f32 target = *(f32*)((s32)work + 0x1E8);
    f32 current = *(f32*)((s32)work + 0x1E4);
    f32 rate = float_0p25_804235f8;

    *(f32*)((s32)work + 0x1E4) = (target - current) * rate + current;
}
