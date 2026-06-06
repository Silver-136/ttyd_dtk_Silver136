#include "effect/eff_energy.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effEnergyMain(void* entry);
void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effEnergyDisp(void* camera, void* entry);

extern char str_Energy_80302aec[];
extern const Vec vec3_80302ae0;
extern f32 float_0_80428a18;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effEnergyMain(void* entry) {
    Vec zpos;
    Vec pos = vec3_80302ae0;
    void* work = *(void**)((s32)entry + 0xC);
    u32 flags;
    s32 value;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    flags = *(u32*)entry;
    if (flags & 4) {
        *(u32*)entry = flags & ~4;
        *(s32*)((s32)work + 0x28) = 0x10;
    }

    value = *(s32*)((s32)work + 0x28);
    if (value < 1000) {
        *(s32*)((s32)work + 0x28) = value - 1;
    }

    if (*(s32*)((s32)work + 0x28) < 0x10) {
        *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x28) << 4;
    }

    if (*(s32*)((s32)work + 0x28) < 0) {
        effDelete(entry);
    } else {
        *(f32*)((s32)work + 0x1C) = *(f32*)((s32)work + 0x1C) + *(f32*)((s32)work + 0x20);
        *(f32*)((s32)work + 0x14) = *(f32*)((s32)work + 0x14) + *(f32*)((s32)work + 0x18);
        *(s32*)((s32)work + 0x2C) = *(s32*)((s32)work + 0x2C) + *(s32*)((s32)work + 0x30);

        if (*(s32*)((s32)work + 0x2C) > 0xFF) {
            *(s32*)((s32)work + 0x2C) = 0xFF;
            *(s32*)((s32)work + 0x30) = -0x20;
        }
        if (*(s32*)((s32)work + 0x2C) < 0) {
            *(s32*)((s32)work + 0x2C) = 0;
            *(s32*)((s32)work + 0x30) = 0x20;
        }

        dispCalcZ(&zpos);
        dispEntry(4, 2, effEnergyDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effEnergyEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field10, f32 field20, f32 field18) {
    void* entry = effEntry();
    void* work;
    f32 zero;
    s32 alpha;
    s32 zeroInt;
    s32 delta;

    *(char**)((s32)entry + 0x14) = str_Energy_80302aec;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x34));
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a18;
    *(void**)((s32)entry + 0x10) = effEnergyMain;
    alpha = 0xFF;
    zeroInt = 0;
    delta = 0x20;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x20) = field20;
    *(f32*)((s32)work + 0x10) = field10;
    *(f32*)((s32)work + 0x14) = zero;
    *(f32*)((s32)work + 0x18) = field18;
    *(s32*)((s32)work + 0x24) = alpha;
    *(s32*)((s32)work + 0x2C) = zeroInt;
    *(s32*)((s32)work + 0x30) = delta;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
