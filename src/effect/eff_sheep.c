#include "effect/eff_sheep.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSheepDisp(s32 cameraId, void* effect) {
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_0_80427ea8;
    extern f32 float_10_80427eac;

    f32 mtx[3][4];
    f32 scaleMtx[3][4];
    f32 base[3][4];
    void* work;
    s32 i;
    void* part;

    work = *(void**)((s32)effect + 0xC);
    PSMTXTrans(base, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));

    i = 1;
    part = (void*)((s32)work + 0x34);
    while (i < *(s32*)((s32)effect + 8)) {
        if (*(s32*)((s32)part + 0x20) != -1) {
            PSMTXTrans(mtx, *(f32*)((s32)part + 4), *(f32*)((s32)part + 8), *(f32*)((s32)part + 0xC));
            PSMTXScale(scaleMtx, *(f32*)((s32)part + 0x10), *(f32*)((s32)part + 0x10), *(f32*)((s32)part + 0x10));
            PSMTXConcat(mtx, scaleMtx, mtx);
            PSMTXConcat(base, mtx, mtx);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 1, float_0_80427ea8, float_10_80427eac);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 2, float_0_80427ea8, float_10_80427eac);
            animPoseDrawMtx(*(s32*)((s32)part + 0x20), mtx, 3, float_0_80427ea8, float_10_80427eac);
        }
        i++;
        part = (void*)((s32)part + 0x34);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_sheep 20260624_184929 */

/* stub-fill: effSheepEntry | missing_definition | ghidra_signature */
void* effSheepEntry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern s32 irand(s32 max);
    extern void* gp;
    extern void* _battleWorkPointer;
    extern void effSheepMain(void);
    extern char str_sheep_80427ec4[];
    extern f32 float_0_80427ea8;
    extern f32 float_10_80427eac;
    extern f32 float_1_80427ecc;
    extern f32 float_neg500_80427ed0;
    extern f32 float_0p2_80427ed4;
    extern f32 float_0p5_80427ed8;

    void* effect;
    void* work;
    void* part;
    s32 i;

    effect = effEntry(type);
    if (*(s32*)((s32)gp + 0x14) != 0) {
        y = BattleGetFloorHeight(_battleWorkPointer, x, y, z);
    }

    *(void**)((s32)effect + 0x14) = str_sheep_80427ec4;
    *(s32*)((s32)effect + 8) = 0x33;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x34);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effSheepMain;
    *(u32*)effect |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = float_1_80427ecc;
    *(s32*)((s32)work + 0x24) = 0;

    part = (void*)((s32)work + 0x34);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part = (void*)((s32)part + 0x34)) {
        s32 side;

        side = ((irand(0x64) ^ 0x14) - (((irand(1) ^ 0x14) & 0x14))) != 0;
        *(s32*)((s32)part + 0) = side;
        *(f32*)((s32)part + 4) = float_neg500_80427ed0;
        *(f32*)((s32)part + 8) = float_0_80427ea8;
        *(f32*)((s32)part + 0xC) = float_0_80427ea8;
        *(s32*)((s32)part + 0x20) = -1;
        *(s32*)((s32)part + 0x24) = 0;
        *(f32*)((s32)part + 0x10) = ((f32)(irand(5) + 8)) / float_10_80427eac;
        *(f32*)((s32)part + 0x2C) = ((f32)(irand(0x14) + 0x14)) / float_10_80427eac;
        *(s32*)((s32)part + 0x28) = i + irand(0x1E);
        *(f32*)((s32)part + 0xC) = (f32)(0x64 - irand(0xC8));

        if (*(s32*)((s32)part + 0) != 0) {
            s32 frame = irand(0x32) + 0x32;
            *(s32*)((s32)part + 0x14) = (s32)((f32)frame / *(f32*)((s32)part + 0x2C));
            *(f32*)((s32)part + 0x1C) = float_0p2_80427ed4;
            *(f32*)((s32)part + 0x18) = (float_0p2_80427ed4 * (f32)*(s32*)((s32)part + 0x14)) * float_0p5_80427ed8;
        }

        if (*(s32*)((s32)effect + 8) - 1 == i) {
            *(f32*)((s32)part + 0xC) = float_0_80427ea8;
            *(s32*)((s32)part + 0) = 0;
            *(f32*)((s32)part + 0x10) = float_1_80427ecc;
            *(s32*)((s32)part + 0x28) = *(s32*)((s32)effect + 8) + 0xC8;
        }
    }

    return effect;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_sheep 20260624_191429 */

/* fallback stub-fill: map=effSheepMain addr=0x802385dc size=0x000004b4 */
int effSheepMain() {
    return 0;
}
