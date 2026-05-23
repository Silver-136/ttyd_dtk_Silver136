#include "effect/eff_stageclear.h"

s32 effStageClearEndChk(void* effect) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 value = *(s32*)((s32)work + 0x28);
    return (u32)(-value | value) >> 31;
}
