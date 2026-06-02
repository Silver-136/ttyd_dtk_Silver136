#include "effect/eff_peach_fukidashi.h"

extern f32 float_0_80426f94;

void effPeachFukidashiDisp(void);
void effDelete(void* effect);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

void effSetPeachFukiBatu(char* name) {
    extern void* effNameToPtr(char* name);

    void* effect = effNameToPtr(name);
    void* work = *(void**)((s32)effect + 0xC);
    *(u32*)work |= 0x80;
}

void effPeachFukidashiMain(void* effect) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 timer = *(s32*)((s32)work + 0x1C);

    if (timer < 1000) {
        *(s32*)((s32)work + 0x1C) = timer - 1;
    }
    if (*(s32*)((s32)work + 0x1C) < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effPeachFukidashiDisp, effect, float_0_80426f94);
    }
}
