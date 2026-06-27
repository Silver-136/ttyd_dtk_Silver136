#include "effect/eff_machinegun.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMachinegunMain(void* entry);
void effMachinegunDisp(void* camera, void* entry);
void effDelete(void* entry);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 order, void* callback, void* entry);
f64 cos(f64 value);
f64 sin(f64 value);

extern char str_Machinegun_80302a8c[];
extern Vec vec3_80302a80;
extern f32 float_6p2832_804289a0;
extern f32 float_360_804289a4;
extern f32 float_0p9_804289a8;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMachinegunMain(void* entry) {
    Vec pos = vec3_80302a80;
    Vec zpos;
    void* work = *(void**)((s32)entry + 0xC);

    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    zpos = pos;

    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x18) * (f32)cos((float_6p2832_804289a0 * *(f32*)((s32)work + 0x14)) / float_360_804289a4);
    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x18) * (f32)sin((float_6p2832_804289a0 * *(f32*)((s32)work + 0x14)) / float_360_804289a4);
    *(f32*)((s32)work + 0x18) *= float_0p9_804289a8;
    *(s32*)((s32)work + 0x1C) -= 10;

    if (*(s32*)((s32)work + 0x1C) < 0) {
        effDelete(entry);
    } else {
        dispCalcZ(&zpos);
        dispEntry(4, 2, effMachinegunDisp, entry);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma use_lmw_stmw off
void* effMachinegunEntry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 field10) {
    void* entry = effEntry();
    void* work;

    *(char**)((s32)entry + 0x14) = str_Machinegun_80302a8c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x28);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMachinegunMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = angle;
    *(f32*)((s32)work + 0x10) = field10;
    *(s32*)((s32)work + 0x1C) = 0xFF;
    *(f32*)((s32)work + 0x18) = speed;

    return entry;
}
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_machinegun 20260624_184929 */

/* stub-fill: effMachinegunDisp | prototype_only | source_prototype */
void effMachinegunDisp(void* camera, void* entry) {
    return;
}
