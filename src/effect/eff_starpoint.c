#include "effect/eff_starpoint.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effStarPointDisp(void* camera, void* effect) {
    f32 base[12];
    f32 rot[12];
    f32 mtx[12];
    void* work;
    s32 i;

    extern f32 float_deg2rad_80426f08;
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(void* m, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void iconDispGx2(void* mtx, s32 icon, s32 color);

    work = *(void**)((s32)effect + 0xC);
    PSMTXTrans(base, *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14));
    PSMTXRotRad(rot, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114)) * float_deg2rad_80426f08);
    PSMTXConcat(base, rot, base);

    i = 1;
    work = (void*)((s32)work + 0x34);
    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)((s32)work + 0x2C) != 0) {
            PSMTXTrans(mtx, *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14));
            PSMTXConcat(base, mtx, mtx);
            iconDispGx2(mtx, 0, 0x194);
        }
        i++;
        work = (void*)((s32)work + 0x34);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
