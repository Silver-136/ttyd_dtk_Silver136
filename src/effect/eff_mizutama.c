#include "effect/eff_mizutama.h"

typedef struct EffMizutamaVec {
    f32 x;
    f32 y;
    f32 z;
} EffMizutamaVec;

void effMizutamaMain(void* entry) {
    extern const EffMizutamaVec vec3_802fefe0;
    extern f32 float_0p97_804277e0;
    extern f32 float_0p3_804277e4;
    extern f32 float_0p99_804277e8;
    extern f32 float_0p01_804277ec;
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void effMizutamaDisp(void);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

    void* work;
    EffMizutamaVec pos;
    EffMizutamaVec base;
    s32 i = 1;
    s32 dead = 0;

    work = *(void**)((s32)entry + 0xC);
    base = vec3_802fefe0;
    base.x = *(f32*)((s32)work + 4);
    base.y = *(f32*)((s32)work + 8);
    base.z = *(f32*)((s32)work + 0xC);
    pos = base;

    work = (void*)((s32)work + 0x28);
    while (i < *(s32*)((s32)entry + 8)) {
        if (*(s32*)((s32)work + 0x24) != 0) {
            *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x24) - 1;
        } else {
            f32 damp = float_0p97_804277e0;
            f32 rise = float_0p3_804277e4;
            f32 fade = float_0p99_804277e8;
            f32 min = float_0p01_804277ec;

            *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
            *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
            *(f32*)((s32)work + 0x10) *= damp;
            *(f32*)((s32)work + 0x14) *= damp;
            *(f32*)((s32)work + 8) += rise;
            *(f32*)((s32)work + 0x18) *= fade;
            if (*(f32*)((s32)work + 0x18) < min) {
                dead++;
            }
        }
        i++;
        work = (void*)((s32)work + 0x28);
    }

    if (dead >= *(s32*)((s32)entry + 8) - 1) {
        effDelete(entry);
    } else {
        dispEntry(4, 2, effMizutamaDisp, entry, dispCalcZ(&pos));
    }
}

