#include "effect/eff_peach_fukidashi.h"

void effSetPeachFukiBatu(char* name) {
    extern void* effNameToPtr(char* name);

    void* effect = effNameToPtr(name);
    void* work = *(void**)((s32)effect + 0xC);
    *(u32*)work |= 0x80;
}
