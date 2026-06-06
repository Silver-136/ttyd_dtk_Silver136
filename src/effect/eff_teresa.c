#include "effect/eff_teresa.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTeresaEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern const char str_Teresa_802ff24c[];
    extern void effTeresaMain(void);
    extern f32 float_30_80427ad4;
    extern f32 float_0_80427ab4;
    extern const Vec vec3_802ff228;
    extern u32 psndSFXOn_3D(s32 lookup, Vec* position);

    void* entry;
    void* work;
    Vec pos;
    f32 modeFloat;
    s32 zero;
    s32 alpha;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Teresa_802ff24c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x40);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTeresaMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    if (type == 0) {
        modeFloat = float_30_80427ad4;
    } else {
        modeFloat = float_0_80427ab4;
    }
    alpha = (type != 0) ? 0xFF : 0;
    zero = 0;
    *(f32*)((s32)work + 0x34) = modeFloat;
    *(s32*)((s32)work + 0x30) = zero;
    *(s32*)((s32)work + 0x2C) = zero;
    *(f32*)((s32)work + 0x28) = (f32)alpha;
    *(s32*)((s32)work + 0x3C) = zero;
    if (type == 0) {
        pos = vec3_802ff228;
        pos.x = *(f32*)((s32)work + 4);
        pos.y = *(f32*)((s32)work + 8);
        pos.z = *(f32*)((s32)work + 0xC);
        psndSFXOn_3D(0x5D5, &pos);
    }
    return entry;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

