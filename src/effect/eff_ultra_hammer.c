#include "effect/eff_ultra_hammer.h"
#include <dolphin/mtx.h>

void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
void effUltraHammerDisp(void* camera, void* entry);

extern const Vec vec3_8030301c;
extern f32 scroll_tbl[];
extern f32 float_3p5_80428ba0;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effUltraHammerMain(void* entry) {
    void* work = *(void**)((s32)entry + 0xC);
    Vec zpos;
    Vec pos = vec3_8030301c;
    s32 timer;
    s32 i;
    f32 scroll;
    f32 hammerScale;

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    timer = *(s32*)((s32)work + 0x20);
    if (timer < 1000) {
        *(s32*)((s32)work + 0x20) = timer - 1;
    }
    if (*(s32*)((s32)work + 0x20) < 0x10) {
        *(s32*)((s32)work + 0x1C) = *(s32*)((s32)work + 0x20) << 4;
    }
    if (*(s32*)((s32)work + 0x20) < 0) {
        effDelete(entry);
    } else {
        i = 1;
        work = (void*)((s32)work + 0x2C);
        while (i < *(s32*)((s32)entry + 8)) {
            timer = *(s32*)((s32)work + 0x28);
            if (timer != 0) {
                *(s32*)((s32)work + 0x28) = timer - 1;
            } else if (*(s32*)((s32)work + 0x24) < 0x19) {
                hammerScale = float_3p5_80428ba0;
                scroll = scroll_tbl[*(s32*)((s32)work + 0x24)];
                *(f32*)((s32)work + 0x14) = scroll;
                *(f32*)((s32)work + 0x10) = hammerScale;
                *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x24) + 1;
            } else {
                *(s32*)((s32)work + 0x28) = 0xF;
                *(s32*)((s32)work + 0x24) = 0;
            }
            i++;
            work = (void*)((s32)work + 0x2C);
        }
        dispCalcZ(&zpos);
        dispEntry(4, 2, effUltraHammerDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_ultra_hammer 20260624_185035 */

/* stub-fill: effUltraHammerDisp | prototype_only | source_prototype */
void effUltraHammerDisp(void* camera, void* entry) {
    return;
}

/* stub-fill: effUltraHammerEntry | missing_definition | ghidra_signature */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUltraHammerEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effUltraHammerMain(void* entry);
    extern char str_UltraHammer_80303028[];
    extern u32 dat_80303010[];
    extern f32 float_0_80428b9c;
    extern f32 float_neg4_80428ba4;
    extern f32 float_1_80428b98;

    void* entry;
    void* work;
    void* child;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_UltraHammer_80303028;
    *(s32*)((s32)entry + 8) = 4;
    work = __memAlloc(3, 0xB0);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effUltraHammerMain;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x20) = 1000;
    } else {
        *(s32*)((s32)work + 0x20) = timer;
    }
    *(s32*)((s32)work + 0x1C) = 0xFF;
    *(s32*)((s32)work + 0x24) = 0;

    child = (void*)((s32)work + 0x2C);
    for (i = 1; i < *(s32*)((s32)entry + 8); i++) {
        *(f32*)((s32)child + 4) = float_0_80428b9c;
        *(f32*)((s32)child + 8) = float_neg4_80428ba4 * (f32)(i - 1);
        *(f32*)((s32)child + 0xC) = float_0_80428b9c;
        *(f32*)((s32)child + 0x10) = float_1_80428b98;
        *(u32*)((s32)child + 0x28) = dat_80303010[i - 1];
        *(s32*)((s32)child + 0x24) = 0;
        *(s32*)((s32)child + 0x20) = 0;
        child = (void*)((s32)child + 0x2C);
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

