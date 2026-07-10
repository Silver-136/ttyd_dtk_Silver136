#include "effect/eff_mahorn.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effMahornMain(void* entry);

extern char str_Mahorn_80302968[];
extern f32 float_0_804287f8;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMahornEntry(s32 kind, s32 count, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    s32 workCount = count + 1;
    void* work;
    s32 value;
    void* child;
    s32 i;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_Mahorn_80302968;
    *(s32*)((s32)entry + 8) = workCount;
    work = __memAlloc(3, (u32)(workCount * 0x28));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMahornMain;

    *(s32*)((s32)work + 0) = kind;
    *(s32*)((s32)work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }

    *(f32*)((s32)work + 4) = x;
    zero = float_0_804287f8;
    child = (void*)((s32)work + 0x28);
    *(f32*)((s32)work + 8) = y;
    i = 1;
    value = 4;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x18) = 0xFF;
    while (i < *(s32*)((s32)entry + 8)) {
        *(f32*)((s32)child + 4) = zero;
        i++;
        *(f32*)((s32)child + 8) = zero;
        *(f32*)((s32)child + 0xC) = zero;
        *(f32*)((s32)child + 0x10) = zero;
        *(f32*)((s32)child + 0x14) = zero;
        *(s32*)((s32)child + 0x18) = 0xFF;
        *(s32*)((s32)child + 0x24) = value;
        value += 4;
        child = (void*)((s32)child + 0x28);
    }

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_mahorn 20260624_184929 */

/* stub-fill: effMahornDisp | missing_definition | ghidra_signature */
u8 effMahornDisp(void) {
    return 0;
}

/* stub-fill: effMahornMain | prototype_only | source_prototype */
#pragma optimize_for_size off
void effMahornMain(void* entry) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effMahornDisp(void);
    extern u32 vec3_80302950[];
    extern u8 scale_dt[];
    extern u8 scale_dt2[];
    extern f32 float_100_80428800;
    extern f32 float_20_80428804;
    extern f32 float_0p125_80428808;

    void* work;
    void* child;
    u32 i;
    u32 v;
    u32 idx;
    u32 neg;
    u32 pos[3];
    u32 dispPos[3];

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302950[0];
    pos[1] = vec3_80302950[1];
    pos[2] = vec3_80302950[2];
    *(f32*)&pos[0] = *(f32*)((s32)work + 4);
    *(f32*)&pos[1] = *(f32*)((s32)work + 8);
    *(f32*)&pos[2] = *(f32*)((s32)work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];

    if (*(s32*)((s32)work + 0x1C) < 1000) {
        *(s32*)((s32)work + 0x1C) -= 1;
    }
    *(s32*)((s32)work + 0x20) += 1;
    if (*(s32*)((s32)work + 0x1C) < 0x10) {
        *(s32*)((s32)work + 0x18) = *(s32*)((s32)work + 0x1C) << 4;
    }
    if (*(s32*)((s32)work + 0x1C) < 0) {
        effDelete(entry);
        return;
    }

    i = 1;
    child = (void*)((s32)work + 0x28);
    while ((s32)i < *(s32*)((s32)entry + 8)) {
        if (*(s32*)((s32)child + 0x24) == 0) {
            *(s32*)((s32)child + 0x20) += 1;
            idx = *(u32*)((s32)child + 0x20) % 0x15;
            if (i & 1) {
                v = scale_dt[idx];
            } else {
                v = scale_dt2[idx];
            }
            *(f32*)((s32)child + 0x10) = (f32)v / float_100_80428800;
            *(f32*)((s32)child + 0x14) =
                (float_20_80428804 - *(f32*)((s32)child + 0x14)) * float_0p125_80428808 +
                *(f32*)((s32)child + 0x14);
            neg = -*(s32*)((s32)child + 0x18);
            *(s32*)((s32)child + 0x18) =
                *(s32*)((s32)child + 0x18) + ((s32)neg >> 6) +
                ((s32)neg < 0 && (neg & 0x3F) != 0);
        } else {
            *(s32*)((s32)child + 0x24) -= 1;
        }
        i++;
        child = (void*)((s32)child + 0x28);
    }

    dispEntry(4, 2, effMahornDisp, entry, dispCalcZ(dispPos));
}
#pragma optimize_for_size on

