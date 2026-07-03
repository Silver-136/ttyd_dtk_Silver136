#include "effect/eff_toiki.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effToikiEntry(s32 type, s32 arg1, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effToikiMain(void);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Toiki_80428228;
    extern f32 float_0_80428200;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_0p5_80428230;
    extern f32 float_255_80428234;
    extern f32 float_2p5_80428238;

    void* effect;
    void* work;
    f32 angle;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = &str_Toiki_80428228;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effToikiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = float_0p5_80428230;
    *(f32*)((s32)work + 0x20) = float_0_80428200;
    *(s32*)((s32)work + 0x30) = arg1;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x28) = float_255_80428234;
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x10) = float_2p5_80428238 * (f32)cos(angle);
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x14) = float_2p5_80428238 * (f32)sin(angle);
    *(f32*)((s32)work + 0x18) = float_0_80428200;
    *(f32*)((s32)work + 0x24) = float_0_80428200;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_toiki 20260624_184929 */

/* stub-fill: effToikiDisp | missing_definition | ghidra_signature */
u8 effToikiDisp(int param_1, int param_2) {
    return 0;
}

/* stub-fill: effToikiMain | prototype_only | source_prototype */
void effToikiMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effToikiDisp(s32 cameraId, void* effect);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXMultVec(f32 mtx[3][4], f32* src, f32* dst);
    extern void* effMistEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale);
    extern f32 float_0p95_80428208;
    extern f32 float_1_8042820c;
    extern f32 float_4_80428210;
    extern f32 float_320_80428214;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_deg2rad_80428204;
    extern f32 float_0_80428200;
    extern f32 float_0p015_80428224;

    void* work;
    f32 pos[3];
    f32 mist[3];
    f32 mtx[3][4];
    s32 timer;
    s32 type;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    type = *(s32*)((s32)work + 0);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        *(s32*)((s32)work + 0x30) = 0x10;
    }
    if (*(s32*)((s32)work + 0x30) < 1000) {
        *(s32*)((s32)work + 0x30) -= 1;
    }
    if (*(s32*)((s32)work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0xC) += *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x10) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x14) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x18) *= float_0p95_80428208;

    if (*(s32*)((s32)work + 0x30) < 0x10) {
        *(f32*)((s32)work + 0x28) = (f32)(*(s32*)((s32)work + 0x30) << 4);
    }

    if (*(f32*)((s32)work + 0x24) > float_1_8042820c &&
        *(f32*)((s32)work + 0x24) < float_4_80428210) {
        timer = *(s32*)((s32)work + 0x30);
        if ((timer % 20) == 0) {
            f32 radians = ((float_200_8042821c + *(f32*)((s32)work + 0x20)) * float_6p2832_80428218) / float_360_80428220;
            f32 dist = float_320_80428214 * *(f32*)((s32)work + 0x1C);
            mist[0] = *(f32*)((s32)work + 4) + (dist * (f32)cos(radians));
            mist[1] = *(f32*)((s32)work + 8) + (dist * (f32)sin(radians));
            mist[2] = *(f32*)((s32)work + 0xC) + float_1_8042820c;
            PSMTXRotRad(mtx, 0x79, float_deg2rad_80428204 * -*(f32*)((s32)camGetPtr(4) + 0x114));
            PSMTXMultVec(mtx, mist, mist);
            effMistEntry(type + 1, 0x3C, mist[0], mist[1], mist[2], float_0_80428200);
        }
    }

    *(s32*)((s32)work + 0x2C) += 1;
    *(f32*)((s32)work + 0x24) += float_0p015_80428224;
    if (*(f32*)((s32)work + 0x24) > float_4_80428210) {
        *(f32*)((s32)work + 0x24) = float_4_80428210;
    }
    dispEntry(4, 2, effToikiDisp, dispCalcZ(pos), effect);
}

