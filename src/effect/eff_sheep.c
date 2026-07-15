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
void effSheepMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* gp;
    extern void animPoseRelease(s32);
    extern void effDelete(void*);
    extern u32 animGroupBaseAsync(char*, s32, s32);
    extern s32 animPoseEntry(char*, u32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern u32 psndSFXOn_3D(char*, Vec*);
    extern void psndSFX_pos(s32, Vec*);
    extern void psndSFXOff(s32);
    extern s32 irand(s32);
    extern void effKemuTestEntry(f32, f32, f32, f32, s32);
    extern void animPoseMain(s32);
    extern f64 dispCalcZ(Vec*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSheepDisp(s32, void*);
    extern char str_EFF_hituji_80300340[];
    extern char str_A_1_80427eb0[];
    extern char str_SFX_ITEM_SLEEP1_80300320[];
    extern char str_SFX_ITEM_SLEEP2_80300330[];

    u8* entry = effect;
    u8* work = *(u8**)(entry + 0x0C);
    Vec base;
    s32 inBattle = *(s32*)((u8*)gp + 0x14) != 0;
    s32 finished = 0;
    s32 count = *(s32*)(entry + 0x08);
    s32 i;

    base.x = *(f32*)(work + 0x04);
    base.y = *(f32*)(work + 0x08);
    base.z = *(f32*)(work + 0x0C);

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        for (i = 1; i < count; i++) {
            u8* part = work + i * 0x34;
            if (*(s32*)(part + 0x20) != -1) {
                animPoseRelease(*(s32*)(part + 0x20));
            }
        }
        psndSFXOff(*(s32*)(work + 0x64));
        psndSFXOff(*(s32*)(work + count * 0x34 - 4));
        effDelete(effect);
        return;
    }

    for (i = 1; i < count; i++) {
        u8* part = work + i * 0x34;
        s32 state = *(s32*)(part + 0x24);

        switch (state) {
            case 0:
                if (animGroupBaseAsync(str_EFF_hituji_80300340, inBattle, 0) != 0) {
                    *(s32*)(part + 0x20) = animPoseEntry(str_EFF_hituji_80300340, inBattle);
                    animPoseSetAnim(*(s32*)(part + 0x20), str_A_1_80427eb0, 1);
                    (*(s32*)(part + 0x24))++;
                }
                break;

            case 1:
                if ((s32)--*(u32*)(part + 0x28) < 0) {
                    Vec pos;
                    *(s32*)(part + 0x28) = 0;
                    pos.x = base.x + *(f32*)(part + 0x04);
                    pos.y = base.y + *(f32*)(part + 0x08);
                    pos.z = base.z + *(f32*)(part + 0x0C);
                    if (i == 1) {
                        *(s32*)(part + 0x30) = psndSFXOn_3D(str_SFX_ITEM_SLEEP1_80300320, &pos);
                    } else if (i == count - 1) {
                        *(s32*)(part + 0x30) = psndSFXOn_3D(str_SFX_ITEM_SLEEP2_80300330, &pos);
                    }
                    (*(s32*)(part + 0x24))++;
                }
                break;

            case 2:
                if (i == 1 || i == count - 1) {
                    Vec pos;
                    pos.x = base.x + *(f32*)(part + 0x04);
                    pos.y = base.y + *(f32*)(part + 0x08);
                    pos.z = base.z + *(f32*)(part + 0x0C);
                    psndSFX_pos(*(s32*)(part + 0x30), &pos);
                }
                if (*(f32*)(part + 0x04) >= 500.0f) {
                    (*(s32*)(part + 0x24))++;
                } else {
                    *(f32*)(part + 0x04) += *(f32*)(part + 0x2C);
                    if (*(s32*)part == 1) {
                        *(f32*)(part + 0x08) += *(f32*)(part + 0x18);
                        *(f32*)(part + 0x18) -= *(f32*)(part + 0x1C);
                        if (*(f32*)(part + 0x08) < 0.0f) {
                            *(f32*)(part + 0x08) = 0.0f;
                            *(f32*)(part + 0x18) *= -1.0f;
                        }
                    }
                }
                if ((*(s32*)((u8*)gp + 0x1C) + i) % 10 == 0) {
                    effKemuTestEntry(base.x + *(f32*)(part + 0x04),
                                     base.y + *(f32*)(part + 0x08) + 10.0f,
                                     base.z + *(f32*)(part + 0x0C),
                                     0.3f + (f32)irand(30) / 100.0f, 0);
                }
                break;

            case 3:
                if (i == count - 2) {
                    psndSFXOff(*(s32*)(work + 0x64));
                }
                if (i == count - 1) {
                    psndSFXOff(*(s32*)(part + 0x30));
                }
                animPoseRelease(*(s32*)(part + 0x20));
                *(s32*)(part + 0x20) = -1;
                (*(s32*)(part + 0x24))++;
                break;

            case 4:
                finished++;
                break;
        }

        if (*(s32*)(part + 0x20) != -1) {
            animPoseMain(*(s32*)(part + 0x20));
        }
    }

    if (finished < count - 1) {
        dispEntry(3, 1, effSheepDisp, effect, (f32)dispCalcZ(&base));
    } else {
        effDelete(effect);
    }
}

