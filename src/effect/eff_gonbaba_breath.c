#include "effect/eff_gonbaba_breath.h"

__declspec(section ".rodata") u32 vec3_80302b78[3] = { 0, 0, 0 };
__declspec(section ".rodata") u32 gap_04_80302B84_rodata = 0;

/* Target-owned eff_gonbaba_breath .sdata2 scalar catalog. */
extern const f32 float_6p2832_80428a90;
extern const f32 float_360_80428a94;
extern const f32 float_2_80428a98;
extern const f32 float_deg2rad_80428a9c;
extern const f32 float_90_80428aa0;
extern const f32 float_270_80428aa4;
extern const f32 float_4_80428aa8;
extern const f32 float_16_80428aac;
extern const f32 float_0p5_80428ab0;
extern const f32 float_0_80428ab4;
extern const f32 float_1_80428ab8;
extern const f32 float_1p4_80428abc;
extern const f32 float_1p5_80428ac0;
extern const f32 float_0p25_80428ac4;
extern const f32 float_1p05_80428ac8;
extern const f32 float_0p995_80428acc;
extern const f32 float_1p03_80428ad0;
extern const f32 float_0p125_80428ad4;
extern const f32 float_0p9_80428ad8;
extern const f32 float_neg2p4_80428adc;
extern const f32 float_0p1_80428ae0;
extern const f32 float_neg2_80428ae4;
extern const f32 float_240_80428ae8;
extern const f32 float_10_80428aec;
extern const f32 float_0p00761_80428af0;
extern const f32 float_0p16605_80428af4;
extern const f32 float_3p1416_80428af8;
extern const f32 float_1p5708_80428afc;
extern const f32 float_4p7124_80428b00;
extern const f32 float_0p03705_80428b04;
extern const f32 float_0p4967_80428b08;
extern const f32 float_225_80428b0c;

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: init_breath | missing_definition | ghidra_signature */
void init_breath(void* workPtr, s32 index, s32 type) {
    extern s32 irand(s32);
    extern u8* col_tbl[];
    extern u8* PTR_col_normal_8041e0a0;
    u8* work = workPtr;
    f32 angle;
    f32 speed;
    f32 sinv;
    f32 cosv;
    f32 t;
    f32 t2;
    s32 colorType;

    *(s32*)work = type;

    if (type < 10) {
        if (type >= 0 && type < 7) {
            *(s32*)(work + 0x44) = index << 2;
            *(f32*)(work + 4) = float_0_80428ab4;
            *(f32*)(work + 8) = float_0_80428ab4;
            *(f32*)(work + 0xC) = float_0_80428ab4;
            *(f32*)(work + 0x40) = float_0_80428ab4;
            *(f32*)(work + 0x10) = float_neg2p4_80428adc;
            *(f32*)(work + 0x14) = float_0p1_80428ae0;
            *(s32*)(work + 0x1C) = 0x40;
            *(s32*)(work + 0x3C) = 0xFF;
        } else if (type == 7) {
            *(s32*)(work + 0x44) = index << 3;
            *(f32*)(work + 4) = float_0_80428ab4;
            *(f32*)(work + 8) = float_0_80428ab4;
            *(f32*)(work + 0xC) = float_0_80428ab4;
            *(f32*)(work + 0x40) = float_0_80428ab4;
            *(f32*)(work + 0x10) = float_neg2_80428ae4;
            *(f32*)(work + 0x14) = float_0p1_80428ae0;
            *(s32*)(work + 0x1C) = 0x40;
            *(s32*)(work + 0x3C) = 0xFF;
        }
    } else if (type < 0x11) {
        angle = (float_240_80428ae8 + (f32)irand(0x3C)) * float_6p2832_80428a90 / float_360_80428a94;
        speed = ((f32)(irand(0x32) + 10) / float_10_80428aec) * float_1p5_80428ac0;

        *(s32*)(work + 0x44) = index;
        *(f32*)(work + 4) = float_0_80428ab4;
        *(f32*)(work + 8) = float_0_80428ab4;
        *(f32*)(work + 0xC) = float_0_80428ab4;

        if (float_3p1416_80428af8 < angle) {
            if (float_4p7124_80428b00 <= angle) {
                t = float_1p5708_80428afc - (angle - float_4p7124_80428b00);
                t2 = t * t;
                sinv = -(((float_0p00761_80428af0 * t2 -
                           float_0p16605_80428af4) * t2 + float_1_80428ab8) * t);
            } else {
                t = angle - float_3p1416_80428af8;
                t2 = t * t;
                sinv = -(((float_0p00761_80428af0 * t2 -
                           float_0p16605_80428af4) * t2 + float_1_80428ab8) * t);
            }
        } else if (float_1p5708_80428afc < angle) {
            t = float_1p5708_80428afc - (angle - float_1p5708_80428afc);
            t2 = t * t;
            sinv = ((float_0p00761_80428af0 * t2 -
                     float_0p16605_80428af4) * t2 + float_1_80428ab8) * t;
        } else {
            t2 = angle * angle;
            sinv = ((float_0p00761_80428af0 * t2 -
                     float_0p16605_80428af4) * t2 + float_1_80428ab8) * angle;
        }

        *(f32*)(work + 0x10) = speed * sinv;

        if (float_3p1416_80428af8 < angle) {
            if (float_4p7124_80428b00 <= angle) {
                t = float_1p5708_80428afc - (angle - float_4p7124_80428b00);
                t2 = t * t;
                cosv = (float_0p03705_80428b04 * t2 -
                        float_0p4967_80428b08) * t2 + float_1_80428ab8;
            } else {
                t = angle - float_3p1416_80428af8;
                t2 = t * t;
                cosv = -((float_0p03705_80428b04 * t2 -
                          float_0p4967_80428b08) * t2 + float_1_80428ab8);
            }
        } else if (float_1p5708_80428afc < angle) {
            t = float_1p5708_80428afc - (angle - float_1p5708_80428afc);
            t2 = t * t;
            cosv = -((float_0p03705_80428b04 * t2 -
                      float_0p4967_80428b08) * t2 + float_1_80428ab8);
        } else {
            t2 = angle * angle;
            cosv = (float_0p03705_80428b04 * t2 -
                    float_0p4967_80428b08) * t2 + float_1_80428ab8;
        }

        *(f32*)(work + 0x14) = speed * cosv;
        *(f32*)(work + 0x40) = float_1_80428ab8;
        *(s32*)(work + 0x1C) = 0x1E;
        *(s32*)(work + 0x3C) = 0xFF;

        colorType = type % 10;
        {
            u8* colors = col_tbl[colorType];
            *(s32*)(work + 0x24) = colors[0xC];
            *(s32*)(work + 0x28) = colors[0xD];
            *(s32*)(work + 0x2C) = colors[0xE];
            *(s32*)(work + 0x30) = colors[0];
            *(s32*)(work + 0x34) = colors[1];
            *(s32*)(work + 0x38) = colors[2];
        }

    } else if (type == 0x11) {
        angle = (float_225_80428b0c + (f32)irand(0x5A)) * float_6p2832_80428a90 / float_360_80428a94;
        speed = ((f32)(irand(0x32) + 10) / float_10_80428aec) * float_1p5_80428ac0;

        *(s32*)(work + 0x44) = irand(100);
        *(f32*)(work + 4) = float_0_80428ab4;
        *(f32*)(work + 8) = float_0_80428ab4;
        *(f32*)(work + 0xC) = float_0_80428ab4;

        if (float_3p1416_80428af8 < angle) {
            if (float_4p7124_80428b00 <= angle) {
                t = float_1p5708_80428afc - (angle - float_4p7124_80428b00);
                t2 = t * t;
                sinv = -(((float_0p00761_80428af0 * t2 -
                           float_0p16605_80428af4) * t2 + float_1_80428ab8) * t);
            } else {
                t = angle - float_3p1416_80428af8;
                t2 = t * t;
                sinv = -(((float_0p00761_80428af0 * t2 -
                           float_0p16605_80428af4) * t2 + float_1_80428ab8) * t);
            }
        } else if (float_1p5708_80428afc < angle) {
            t = float_1p5708_80428afc - (angle - float_1p5708_80428afc);
            t2 = t * t;
            sinv = ((float_0p00761_80428af0 * t2 -
                     float_0p16605_80428af4) * t2 + float_1_80428ab8) * t;
        } else {
            t2 = angle * angle;
            sinv = ((float_0p00761_80428af0 * t2 -
                     float_0p16605_80428af4) * t2 + float_1_80428ab8) * angle;
        }

        *(f32*)(work + 0x10) = speed * sinv;

        if (float_3p1416_80428af8 < angle) {
            if (float_4p7124_80428b00 <= angle) {
                t = float_1p5708_80428afc - (angle - float_4p7124_80428b00);
                t2 = t * t;
                cosv = (float_0p03705_80428b04 * t2 -
                        float_0p4967_80428b08) * t2 + float_1_80428ab8;
            } else {
                t = angle - float_3p1416_80428af8;
                t2 = t * t;
                cosv = -((float_0p03705_80428b04 * t2 -
                          float_0p4967_80428b08) * t2 + float_1_80428ab8);
            }
        } else if (float_1p5708_80428afc < angle) {
            t = float_1p5708_80428afc - (angle - float_1p5708_80428afc);
            t2 = t * t;
            cosv = -((float_0p03705_80428b04 * t2 -
                      float_0p4967_80428b08) * t2 + float_1_80428ab8);
        } else {
            t2 = angle * angle;
            cosv = (float_0p03705_80428b04 * t2 -
                    float_0p4967_80428b08) * t2 + float_1_80428ab8;
        }

        *(f32*)(work + 0x14) = speed * cosv;
        *(f32*)(work + 0x40) = float_1_80428ab8;
        *(s32*)(work + 0x1C) = 0x3C;
        *(s32*)(work + 0x3C) = 0xFF;

        *(s32*)(work + 0x24) = PTR_col_normal_8041e0a0[0xC];
        *(s32*)(work + 0x28) = PTR_col_normal_8041e0a0[0xD];
        *(s32*)(work + 0x2C) = PTR_col_normal_8041e0a0[0xE];
        *(s32*)(work + 0x30) = PTR_col_normal_8041e0a0[0];
        *(s32*)(work + 0x34) = PTR_col_normal_8041e0a0[1];
        *(s32*)(work + 0x38) = PTR_col_normal_8041e0a0[2];

    }
}

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathEntry | missing_definition | ghidra_signature */
void* effGonbabaBreathEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void init_breath(void* work, s32 index, s32 type);
    extern char str_GonbabaBreath_80302b90[];

    void* entry;
    void* work;
    s32 count;
    s32 i;
    void* child;
    void* childWork;
    void* grandChild;

    entry = effEntry();
    if (type == 7) {
        count = 0x11;
    } else if (type < 7) {
        if (type >= 0) {
            count = 0x21;
        }
    } else if (type < 0x12) {
        if (type >= 0xA) {
            count = 0x41;
        }
    }

    *(char**)((s32)entry + 0x14) = str_GonbabaBreath_80302b90;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x50);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGonbabaBreathMain;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x40) = scale;
    *(s32*)((s32)work + 0x3C) = 0xFF;
    *(s32*)((s32)work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(f32*)((s32)work + 0x48) = float_0_80428ab4;
    *(void**)((s32)work + 0x4C) = 0;

    if (type < 10) {
        child = effGonbabaBreathEntry(type + 10, x, y, z, scale, timer);
        *(void**)((s32)work + 0x4C) = child;
        childWork = *(void**)((s32)child + 0xC);
        *(f32*)((s32)childWork + 0x48) = *(f32*)((s32)work + 0x48);
        grandChild = *(void**)((s32)childWork + 0x4C);
        if (grandChild != 0) {
            *(f32*)((s32)*(void**)((s32)grandChild + 0xC) + 0x48) = *(f32*)((s32)work + 0x48);
        }
    }

    work = (void*)((s32)work + 0x50);
    i = 1;
    while (i < count) {
        init_breath(work, i, type);
        i += 1;
        work = (void*)((s32)work + 0x50);
    }

    return entry;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_gonbaba_breath 20260624_191429 */

/* fallback stub-fill: map=effGonbabaBreathMain addr=0x8025b270 size=0x00000694 */
void effGonbabaBreathMain(void* effect) {
    extern void effDelete(void*);
    extern void effSoftDelete(void*);
    extern void init_breath(void*, s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern f64 dispCalcZ(f32*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    u8* entry = effect;
    u8* prev = *(u8**)(entry + 0xC);
    s32 count = *(s32*)(entry + 8);
    u32 position[3];
    s32 i;

    position[0] = vec3_80302b78[0];
    position[1] = vec3_80302b78[1];
    position[2] = vec3_80302b78[2];
    *(f32*)&position[0] = *(f32*)(prev + 4);
    *(f32*)&position[1] = *(f32*)(prev + 8);
    *(f32*)&position[2] = *(f32*)(prev + 0xC);
    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        *(s32*)(prev + 0x1C) = 0x20;
        if (*(void**)(prev + 0x4C) != 0) {
            effSoftDelete(*(void**)(prev + 0x4C));
        }
    }
    if (*(s32*)(prev + 0x1C) < 1000) {
        --*(s32*)(prev + 0x1C);
    }
    ++*(s32*)(prev + 0x20);
    if (*(s32*)(prev + 0x1C) < 0x20) {
        *(s32*)(prev + 0x3C) = *(s32*)(prev + 0x1C) << 3;
    }
    if (*(s32*)(prev + 0x1C) < 0) {
        effDelete(effect);
    } else {
        for (i = 1; i < count; i++) {
            u8* work = prev + 0x50;
            s32 type;
            if (*(s32*)(prev + 0x94) == 0) {
                type = *(s32*)work;
                if (type < 10) {
                    if (type == 7) {
                        if (*(s32*)(prev + 0x6C) == 0) {
                            init_breath(work, 0);
                        } else {
                            --*(s32*)(prev + 0x6C);
                            if (*(s32*)(prev + 0x6C) < 0x20) {
                                *(s32*)(prev + 0x8C) = *(s32*)(prev + 0x6C) << 3;
                            }
                            if (*(f32*)(prev + 0x90) >= float_1p4_80428abc) {
                                *(f32*)(prev + 0x90) *= float_1p03_80428ad0;
                            } else {
                                *(f32*)(prev + 0x90) +=
                                    (float_1p5_80428ac0 - *(f32*)(prev + 0x90)) * float_0p25_80428ac4;
                            }
                            *(f32*)(prev + 0x60) *= float_0p995_80428acc;
                            *(f32*)(prev + 0x64) *= float_1p05_80428ac8;
                            *(f32*)(prev + 0x54) += *(f32*)(prev + 0x60);
                            *(f32*)(prev + 0x58) += *(f32*)(prev + 0x64);
                        }
                    } else if (type >= 0 && type < 7) {
                        if (*(s32*)(prev + 0x6C) == 0) {
                            init_breath(work, i);
                        } else {
                            --*(s32*)(prev + 0x6C);
                            if (*(s32*)(prev + 0x6C) < 0x20) {
                                *(s32*)(prev + 0x8C) = *(s32*)(prev + 0x6C) << 3;
                            }
                            if (*(f32*)(prev + 0x90) >= float_1p4_80428abc) {
                                *(f32*)(prev + 0x90) *= float_1p05_80428ac8;
                            } else {
                                *(f32*)(prev + 0x90) +=
                                    (float_1p5_80428ac0 - *(f32*)(prev + 0x90)) * float_0p25_80428ac4;
                            }
                            *(f32*)(prev + 0x60) *= float_0p995_80428acc;
                            *(f32*)(prev + 0x64) *= float_1p05_80428ac8;
                            *(f32*)(prev + 0x54) += *(f32*)(prev + 0x60);
                            *(f32*)(prev + 0x58) += *(f32*)(prev + 0x64);
                        }
                    }
                } else if (type == 0x11) {
                    if (*(s32*)(prev + 0x6C) == 0) {
                        init_breath(work, 0);
                    } else {
                        --*(s32*)(prev + 0x6C);
                        if (*(s32*)(prev + 0x6C) < 0x20) {
                            *(s32*)(prev + 0x8C) = *(s32*)(prev + 0x6C) << 3;
                        }
                        if (*(f32*)(prev + 0x90) >= float_1p4_80428abc) {
                            *(f32*)(prev + 0x90) *= float_1p03_80428ad0;
                        } else {
                            *(f32*)(prev + 0x90) +=
                                (float_1p5_80428ac0 - *(f32*)(prev + 0x90)) * float_0p125_80428ad4;
                        }
                        *(f32*)(prev + 0x60) *= float_0p9_80428ad8;
                        *(f32*)(prev + 0x64) *= float_0p9_80428ad8;
                        *(f32*)(prev + 0x54) += *(f32*)(prev + 0x60);
                        *(f32*)(prev + 0x58) += *(f32*)(prev + 0x64);
                        *(s32*)(prev + 0x74) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x74), (f32)*(s32*)(prev + 0x80), 11,
                            0x3C - *(s32*)(prev + 0x6C), 0x3C);
                        *(s32*)(prev + 0x78) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x78), (f32)*(s32*)(prev + 0x84), 11,
                            0x3C - *(s32*)(prev + 0x6C), 0x3C);
                        *(s32*)(prev + 0x7C) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x7C), (f32)*(s32*)(prev + 0x88), 11,
                            0x3C - *(s32*)(prev + 0x6C), 0x3C);
                    }
                } else if (type < 0x11) {
                    if (*(s32*)(prev + 0x6C) == 0) {
                        init_breath(work, i);
                    } else {
                        --*(s32*)(prev + 0x6C);
                        if (*(s32*)(prev + 0x6C) < 0x10) {
                            *(s32*)(prev + 0x8C) = *(s32*)(prev + 0x6C) << 4;
                        }
                        if (*(f32*)(prev + 0x90) >= float_1p4_80428abc) {
                            *(f32*)(prev + 0x90) *= float_1p03_80428ad0;
                        } else {
                            *(f32*)(prev + 0x90) +=
                                (float_1p5_80428ac0 - *(f32*)(prev + 0x90)) * float_0p125_80428ad4;
                        }
                        *(f32*)(prev + 0x60) *= float_0p9_80428ad8;
                        *(f32*)(prev + 0x64) *= float_0p9_80428ad8;
                        *(f32*)(prev + 0x54) += *(f32*)(prev + 0x60);
                        *(f32*)(prev + 0x58) += *(f32*)(prev + 0x64);
                        *(s32*)(prev + 0x74) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x74), (f32)*(s32*)(prev + 0x80), 11,
                            0x1E - *(s32*)(prev + 0x6C), 0x1E);
                        *(s32*)(prev + 0x78) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x78), (f32)*(s32*)(prev + 0x84), 11,
                            0x1E - *(s32*)(prev + 0x6C), 0x1E);
                        *(s32*)(prev + 0x7C) = (s32)intplGetValue(
                            (f32)*(s32*)(prev + 0x7C), (f32)*(s32*)(prev + 0x88), 11,
                            0x1E - *(s32*)(prev + 0x6C), 0x1E);
                    }
                }
            } else {
                --*(s32*)(prev + 0x94);
            }
            prev = work;
        }
        dispEntry(4, 2, effGonbabaBreathDisp, effect, (f32)dispCalcZ((f32*)position));
    }
}

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathDisp | missing_definition | ghidra_signature */
void effGonbabaBreathDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern u8 col_tbl[];

    u8 texObj[0x20];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    f32 baseMtx[3][4];
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* child = work + 0x50;
    void* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 baseAlpha = *(s32*)(work + 0x3C);
    s32 colorSet = type % 10;
    s32 i;

    if (type < 10) {
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    } else {
        void* camera3d = camGetPtr(4);
        f32 angle = (*(volatile const f32*)&float_6p2832_80428a90) * *(f32*)((u8*)camera3d + 0x114) / (*(volatile const f32*)&float_360_80428a94);
        PSMTXTrans(trans,
            *(f32*)(work + 4) - (*(volatile const f32*)&float_2_80428a98) * (f32)sin(angle),
            *(f32*)(work + 8),
            *(f32*)(work + 0xC) + (*(volatile const f32*)&float_2_80428a98) * (f32)cos(angle));
    }
    PSMTXRotRad(rot, 'y', float_deg2rad_80428a9c * (*(f32*)(work + 0x48) - *(f32*)((u8*)camera + 0x114)));
    if (*(f32*)(work + 0x48) > (*(volatile const f32*)&float_90_80428aa0) && *(f32*)(work + 0x48) < (*(volatile const f32*)&float_270_80428aa4)) {
        PSMTXScale(scaleMtx, *(f32*)(work + 0x40), *(f32*)(work + 0x40), -*(f32*)(work + 0x40));
    } else {
        PSMTXScale(scaleMtx, *(f32*)(work + 0x40), *(f32*)(work + 0x40), *(f32*)(work + 0x40));
    }
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), trans, baseMtx);
    effGetTexObj(0x72, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++, child += 0x50) {
        if (*(s32*)(child + 0x4C) == 0) {
            f32 childTrans[3][4];
            f32 childScale[3][4];
            volatile f32* fifo = (volatile f32*)0xCC008000;
            s32 childType = *(s32*)(child + 0x50);
            s32 alpha = (*(s32*)(child + 0x4C - 0x0C) * baseAlpha) / 0xFF;
            u32 color;

            PSMTXTrans(childTrans, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
            PSMTXScale(childScale, *(f32*)(child + 0x40), *(f32*)(child + 0x40), *(f32*)(child + 0x40));
            PSMTXConcat(childTrans, childScale, childTrans);
            PSMTXConcat(baseMtx, childTrans, childTrans);
            GXLoadPosMtxImm(childTrans, 0);
            GXSetCurrentMtx(0);
            if (childType < 10) {
                s32 j;
                f32 edgeHalf = (*(volatile const f32*)&float_0p5_80428ab0);
                f32 edgeFour = (*(volatile const f32*)&float_4_80428aa8);
                f32 edgeSixteen = (*(volatile const f32*)&float_16_80428aac);
                for (j = 0; j < 3; j++) {
                    f32 edge = -((edgeFour * (f32)j) - edgeSixteen) * edgeHalf;
                    color = *(u32*)(col_tbl + colorSet * 16 + j * 4);
                    ((u8*)&color)[3] = (u8)alpha;
                    GXSetChanMatColor(4, &color);
                    GXBegin(0x80, 0, 4);
                    *fifo = -edge; *fifo = edge; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4);
                    *fifo = edge; *fifo = edge; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8); *fifo = (*(volatile const f32*)&float_0_80428ab4);
                    *fifo = edge; *fifo = -edge; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8); *fifo = (*(volatile const f32*)&float_1_80428ab8);
                    *fifo = -edge; *fifo = -edge; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8);
                }
            } else {
                f32 halfWidth = (*(volatile const f32*)&float_neg2_80428ae4);
                ((u8*)&color)[0] = *(u8*)(child + 0x74);
                ((u8*)&color)[1] = *(u8*)(child + 0x78);
                ((u8*)&color)[2] = *(u8*)(child + 0x7C);
                ((u8*)&color)[3] = (u8)alpha;
                GXSetChanMatColor(4, &color);
                GXBegin(0x80, 0, 4);
                *fifo = halfWidth; *fifo = (*(volatile const f32*)&float_2_80428a98); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4);
                *fifo = (*(volatile const f32*)&float_2_80428a98); *fifo = (*(volatile const f32*)&float_2_80428a98); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8); *fifo = (*(volatile const f32*)&float_0_80428ab4);
                *fifo = (*(volatile const f32*)&float_2_80428a98); *fifo = halfWidth; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8); *fifo = (*(volatile const f32*)&float_1_80428ab8);
                *fifo = halfWidth; *fifo = halfWidth; *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_0_80428ab4); *fifo = (*(volatile const f32*)&float_1_80428ab8);
            }
        }
    }
}

u8 col_normal[20] = {
    0xF2, 0x65, 0x22, 0xFF, 0xF6, 0x86, 0x1E, 0xFF, 0xF6, 0xAC,
    0x1E, 0xFF, 0xFF, 0xF5, 0x68, 0xFF, 0x96, 0x5E, 0x1A, 0xFF,
};
u8 col_poison[20] = {
    0x67, 0x85, 0x1C, 0xFF, 0x77, 0x98, 0x26, 0xFF, 0x89, 0xAF,
    0x2B, 0xFF, 0xC8, 0xE5, 0x05, 0xFF, 0x85, 0x70, 0x1C, 0xFF,
};
u8 col_mega[20] = {
    0xD6, 0xCF, 0x0A, 0xFF, 0xEB, 0xEB, 0x3E, 0xFF, 0xFF, 0xFF,
    0xB4, 0xFF, 0xF7, 0xF7, 0xD7, 0xFF, 0xA9, 0xA3, 0x04, 0xFF,
};
u8 col_cold[20] = {
    0x19, 0xBC, 0xF1, 0xFF, 0x8B, 0xDE, 0xF9, 0xFF, 0xD1, 0xF1,
    0xFC, 0xFF, 0xE1, 0xF5, 0xFC, 0xFF, 0x2D, 0x77, 0xCA, 0xFF,
};
u8 col_confu[20] = {
    0x24, 0xCA, 0x67, 0xFF, 0x37, 0xE3, 0x7D, 0xFF, 0x49, 0xF2,
    0x8D, 0xFF, 0xC2, 0xFC, 0xEB, 0xFF, 0x3E, 0x87, 0x2B, 0xFF,
};
u8 col_sleep[20] = {
    0xD3, 0xCA, 0xD2, 0xFF, 0xEF, 0xE3, 0xEE, 0xFF, 0xFC, 0xF6,
    0xFC, 0xFF, 0xF6, 0xF4, 0xF6, 0xFF, 0xA1, 0x8F, 0x9F, 0xFF,
};
u8 col_mini[20] = {
    0xAE, 0x69, 0xE0, 0xFF, 0xBF, 0x7C, 0xF0, 0xFF, 0xCD, 0x97,
    0xF5, 0xFF, 0xF3, 0xC0, 0xFA, 0xFF, 0x81, 0x31, 0xBB, 0xFF,
};
u8* col_tbl[8] = {
    col_normal,
    col_poison,
    col_mega,
    col_cold,
    col_confu,
    col_sleep,
    col_mini,
    col_normal,
};

__declspec(section ".sdata") u8 gap_07_8041E0A4_sdata[28] = { 0 };
/*
 * Complete target .sdata2 catalog for eff_gonbaba_breath.
 * Target order/size is 32 x f32 = 0x80 bytes.
 */
__declspec(section ".sdata2") const f32 float_6p2832_80428a90 = 6.2831855f;
__declspec(section ".sdata2") const f32 float_360_80428a94 = 360.0f;
__declspec(section ".sdata2") const f32 float_2_80428a98 = 2.0f;
__declspec(section ".sdata2") const f32 float_deg2rad_80428a9c = 0.017453292f;
__declspec(section ".sdata2") const f32 float_90_80428aa0 = 90.0f;
__declspec(section ".sdata2") const f32 float_270_80428aa4 = 270.0f;
__declspec(section ".sdata2") const f32 float_4_80428aa8 = 4.0f;
__declspec(section ".sdata2") const f32 float_16_80428aac = 16.0f;
__declspec(section ".sdata2") const f32 float_0p5_80428ab0 = 0.5f;
__declspec(section ".sdata2") const f32 float_0_80428ab4 = 0.0f;
__declspec(section ".sdata2") const f32 float_1_80428ab8 = 1.0f;
__declspec(section ".sdata2") const f32 float_1p4_80428abc = 1.4f;
__declspec(section ".sdata2") const f32 float_1p5_80428ac0 = 1.5f;
__declspec(section ".sdata2") const f32 float_0p25_80428ac4 = 0.25f;
__declspec(section ".sdata2") const f32 float_1p05_80428ac8 = 1.05f;
__declspec(section ".sdata2") const f32 float_0p995_80428acc = 0.995f;
__declspec(section ".sdata2") const f32 float_1p03_80428ad0 = 1.03f;
__declspec(section ".sdata2") const f32 float_0p125_80428ad4 = 0.125f;
__declspec(section ".sdata2") const f32 float_0p9_80428ad8 = 0.9f;
__declspec(section ".sdata2") const f32 float_neg2p4_80428adc = -2.4f;
__declspec(section ".sdata2") const f32 float_0p1_80428ae0 = 0.1f;
__declspec(section ".sdata2") const f32 float_neg2_80428ae4 = -2.0f;
__declspec(section ".sdata2") const f32 float_240_80428ae8 = 240.0f;
__declspec(section ".sdata2") const f32 float_10_80428aec = 10.0f;
__declspec(section ".sdata2") const f32 float_0p00761_80428af0 = 0.00761f;
__declspec(section ".sdata2") const f32 float_0p16605_80428af4 = 0.16605f;
__declspec(section ".sdata2") const f32 float_3p1416_80428af8 = 3.1415927f;
__declspec(section ".sdata2") const f32 float_1p5708_80428afc = 1.5707964f;
__declspec(section ".sdata2") const f32 float_4p7124_80428b00 = 4.712389f;
__declspec(section ".sdata2") const f32 float_0p03705_80428b04 = 0.03705f;
__declspec(section ".sdata2") const f32 float_0p4967_80428b08 = 0.4967f;
__declspec(section ".sdata2") const f32 float_225_80428b0c = 225.0f;
