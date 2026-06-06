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
