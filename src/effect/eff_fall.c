#include "effect/eff_fall.h"

s32 effFallCheckHit(void* eff) {
    void* work = *(void**)((s32)eff + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 1:
        case 4:
        case 5:
            if (*(s32*)((s32)work + 0xA0) >= 2) {
                return 1;
            }
            break;
        case 2:
        case 3:
            if (*(s32*)((s32)work + 0xA0) >= 1) {
                return 1;
            }
            break;
        case 6:
            if (*(s32*)((s32)work + 0xA0) < 2) {
                if (*(s32*)((s32)work + 0xA0) != 1) {
                    break;
                }
                if (*(s32*)((s32)work + 0x8C) <= 90) {
                    break;
                }
            }
            return 1;
        case 7:
            if (*(s32*)((s32)work + 0xA0) >= 4) {
                return 1;
            }
            break;
        case 8:
            if (*(s32*)((s32)work + 0xA0) >= 3) {
                return 1;
            }
            break;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFallDispInseki(s32 cameraId, void* eff) {
    typedef f32 MtxRaw[3][4];
    typedef struct VecRaw { f32 x; f32 y; f32 z; } VecRaw;

    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(MtxRaw m, double x, double y, double z);
    extern void PSMTXRotRad(MtxRaw m, s32 axis, double angle);
    extern void PSMTXRotAxisRad(MtxRaw m, VecRaw* axis, double angle);
    extern void PSMTXScale(MtxRaw m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(MtxRaw a, MtxRaw b, MtxRaw out);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseDrawMtx(s32 poseId, MtxRaw mtx, s32 mode, double rot, double scale);

    extern f32 float_deg2rad_804274c0;
    extern f32 float_6p2832_804274f4;
    extern f32 float_360_804274f8;
    extern f32 float_2p05_804274fc;
    extern f32 float_2_804274d8;
    extern f32 float_0_804274d4;
    extern f32 float_10_80427500;
    extern f32 float_2p1_80427504;
    extern VecRaw vec3_802fedc4[];
    extern u32 dat_804274ac;
    extern u32 dat_804274b0;

    MtxRaw mtxA;
    MtxRaw mtxD;
    MtxRaw mtxB;
    MtxRaw mtxC;
    VecRaw axis;
    VecRaw axis0;
    u32 color0;
    u32 color1;
    void* cam;
    void* work;
    f32 camAngle;
    f32 deg;
    f32 scale;
    s32 i;

    work = *(void**)((s32)eff + 0xC);

    camGetPtr(cameraId);
    PSMTXTrans(mtxA,
        (double)*(f32*)((s32)work + 0x4),
        (double)*(f32*)((s32)work + 0x8),
        (double)*(f32*)((s32)work + 0xC));

    cam = camGetPtr(cameraId);
    camAngle = *(f32*)((s32)cam + 0x114);
    deg = float_deg2rad_804274c0;
    camAngle = -camAngle;
    PSMTXRotRad(mtxB, 'y', (double)(deg * camAngle));
    PSMTXConcat(mtxA, mtxB, mtxC);

    if (*(s32*)((s32)work + 0xC4) != -1 && *(s32*)((s32)work + 0xC8) != -1) {
        axis0 = vec3_802fedc4[0];
        axis0.x = (f32)cos((double)((float_6p2832_804274f4 * *(f32*)((s32)work + 0x80)) / float_360_804274f8));
        axis0.z = (f32)sin((double)((float_6p2832_804274f4 * *(f32*)((s32)work + 0x80)) / float_360_804274f8));
        axis = axis0;

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                PSMTXTrans(mtxA,
                    (double)*(f32*)((s32)work + 0x6C),
                    (double)*(f32*)((s32)work + 0x70),
                    (double)*(f32*)((s32)work + 0x74));
                camAngle = *(f32*)((s32)work + 0xB0);
                deg = float_deg2rad_804274c0;
                camAngle = -camAngle;
                PSMTXRotAxisRad(mtxB, &axis, (double)(deg * camAngle));
                scale = float_2p05_804274fc * *(f32*)((s32)work + 0x94);
                PSMTXScale(mtxD, scale, scale, scale);
                PSMTXConcat(mtxC, mtxA, mtxA);
                PSMTXConcat(mtxA, mtxB, mtxA);
                PSMTXConcat(mtxA, mtxD, mtxA);
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0xC4), 0x40);
                color0 = dat_804274ac;
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0xC4), &color0);
            } else {
                PSMTXTrans(mtxA,
                    (double)*(f32*)((s32)work + 0x6C),
                    (double)*(f32*)((s32)work + 0x70),
                    (double)*(f32*)((s32)work + 0x74));
                camAngle = *(f32*)((s32)work + 0xB0);
                deg = float_deg2rad_804274c0;
                camAngle = -camAngle;
                PSMTXRotAxisRad(mtxB, &axis, (double)(deg * camAngle));
                scale = float_2_804274d8 * *(f32*)((s32)work + 0x94);
                PSMTXScale(mtxD, scale, scale, scale);
                PSMTXConcat(mtxC, mtxA, mtxA);
                PSMTXConcat(mtxA, mtxB, mtxA);
                PSMTXConcat(mtxA, mtxD, mtxA);
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0xC4), 0x40);
                color1 = dat_804274b0;
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0xC4), &color1);
            }

            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 1, (double)float_0_804274d4, (double)float_10_80427500);
            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 2, (double)float_0_804274d4, (double)float_10_80427500);
            animPoseDrawMtx(*(s32*)((s32)work + 0xC4), mtxA, 3, (double)float_0_804274d4, (double)float_10_80427500);
        }

        PSMTXTrans(mtxA,
            (double)*(f32*)((s32)work + 0x6C),
            (double)*(f32*)((s32)work + 0x70),
            (double)*(f32*)((s32)work + 0x74));
        scale = float_2p1_80427504 * *(f32*)((s32)work + 0x94);
        PSMTXScale(mtxD, scale, scale, scale);
        PSMTXConcat(mtxC, mtxA, mtxA);
        PSMTXConcat(mtxA, mtxD, mtxA);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 1, (double)float_0_804274d4, (double)float_10_80427500);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 2, (double)float_0_804274d4, (double)float_10_80427500);
        animPoseDrawMtx(*(s32*)((s32)work + 0xC8), mtxA, 3, (double)float_0_804274d4, (double)float_10_80427500);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void* effFallEntry(f32 x, s32 type, f32 y, s32 count, f32 z, s32 param, f32 rot, f32 arg5, f32 arg6) {
    typedef struct EffEntryRaw {
        u32 flags;
        u32 inBattle;
        s32 count;
        void* work;
        void* main;
        char* name;
        char pad[0x10];
    } EffEntryRaw;
    typedef struct VecRaw { f32 x; f32 y; f32 z; } VecRaw;

    extern EffEntryRaw* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effFallMain(void);
    extern s32 irand(s32 max);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern u32 psndSFXOn_3D(char* name, VecRaw* pos);

    extern char str_Fall_804275a8;
    extern char str_SFX_BTL_STAGE_FALL_W_802feedc[];
    extern VecRaw vec3_802fed70;
    extern f64 double_to_int_802fedd8;
    extern f32 float_3p1416_8042758c;
    extern f32 float_180_80427510;
    extern f32 float_300_804275b0;
    extern f32 float_0_804274d4;
    extern f32 float_0p2_80427564;
    extern f32 float_0p5_804274cc;
    extern f32 float_1p2_804275b4;
    extern f32 float_1p6_804275b8;
    extern f32 float_0p1_804275bc;
    extern f32 float_200_80427518;
    extern f32 float_1_804274d0;
    extern f32 float_0p6_804275c0;
    extern f32 float_30_80427530;
    extern f32 float_1p3_804275c4;
    extern f32 float_400_804275c8;
    extern f32 float_0p25_804274f0;

    EffEntryRaw* eff;
    void* base;
    void* child;
    VecRaw pos;
    s32 effCount;
    s32 mode;
    s32 i;
    s32 rotInt;
    f32 zero;
    f32 pi;
    f32 deg180;
    f32 heightBase;
    f32 angle;
    f32 radius;
    f32 temp;

    eff = effEntry();
    effCount = count + 1;
    mode = count;
    if (type != 0 && type > -1 && type < 9) {
        mode = 1;
        effCount = 2;
    }

    eff->name = &str_Fall_804275a8;
    eff->count = effCount;
    base = __memAlloc(3, eff->count * 0x68);
    eff->work = base;
    eff->main = (void*)effFallMain;
    eff->flags |= 2;

    rotInt = (s32)rot;
    pi = float_3p1416_8042758c;
    deg180 = float_180_80427510;
    heightBase = float_300_804275b0;
    zero = float_0_804274d4;

    *(s32*)((s32)base + 0x00) = type;
    *(f32*)((s32)base + 0x04) = x;
    *(f32*)((s32)base + 0x08) = y;
    *(f32*)((s32)base + 0x0C) = z;
    *(f32*)((s32)base + 0x10) = rot;
    *(f32*)((s32)base + 0x14) = arg5;
    *(f32*)((s32)base + 0x18) = arg6;
    *(s32*)((s32)base + 0x1C) = mode;
    *(s32*)((s32)base + 0x24) = 0;
    *(s32*)((s32)base + 0x28) = 0;
    *(s32*)((s32)base + 0x20) = param;
    *(s32*)((s32)base + 0x5C) = -1;
    *(s32*)((s32)base + 0x60) = -1;

    child = (void*)((s32)base + 0x68);
    for (i = 1; i < eff->count; i++, child = (void*)((s32)child + 0x68)) {
        angle = (pi * (f32)irand(0x168)) / deg180;
        radius = (f32)irand(rotInt);
        *(f32*)((s32)child + 0x04) = radius * (f32)sin((double)angle);
        *(f32*)((s32)child + 0x08) = heightBase + (f32)irand(eff->count);
        *(f32*)((s32)child + 0x0C) = radius * (f32)cos((double)angle);
        *(s32*)((s32)child + 0x24) = 0;
        *(s32*)((s32)child + 0x28) = 0;
        *(s32*)((s32)child + 0x3C) = 0xFF;
        *(f32*)((s32)child + 0x48) = zero;
        if ((u32)irand(100) <= 50U) {
            *(f32*)((s32)child + 0x18) = 180.0f;
        } else {
            *(f32*)((s32)child + 0x18) = zero;
        }
        *(s32*)((s32)child + 0x38) = 0;
        *(s32*)((s32)child + 0x5C) = -1;
        *(s32*)((s32)child + 0x60) = -1;

        switch (type) {
            case 0:
            case 1:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p2_80427564;
                if (i == 1) {
                    *(f32*)((s32)child + 0x04) = zero;
                    *(f32*)((s32)child + 0x0C) = zero;
                    *(s32*)((s32)child + 0x24) = 0;
                } else {
                    *(s32*)((s32)child + 0x24) = irand(eff->count) + 0x3C;
                }
                if (type == 0) {
                    *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                } else {
                    *(f32*)((s32)child + 0x2C) = float_1p2_804275b4;
                }
                break;
            case 2:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_1p6_804275b8;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x0C) = zero;
                *(s32*)((s32)child + 0x24) = 0;
                *(f32*)((s32)child + 0x44) = zero;
                *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                break;
            case 3:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p1_804275bc;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x50) = zero;
                *(f32*)((s32)child + 0x54) = zero;
                *(f32*)((s32)child + 0x58) = zero;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                pos = vec3_802fed70;
                pos.x = x;
                pos.y = y;
                pos.z = z;
                *(u32*)((s32)child + 0x64) = psndSFXOn_3D(str_SFX_BTL_STAGE_FALL_W_802feedc, &pos);
                break;
            case 4:
            case 5:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p6_804275c0;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x0C) = zero;
                *(s32*)((s32)child + 0x24) = 0;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                break;
            case 6:
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                temp = float_30_80427530;
                *(f32*)((s32)child + 0x50) = -*(f32*)((s32)child + 0x04) / temp;
                *(f32*)((s32)child + 0x54) = -*(f32*)((s32)child + 0x08) / temp;
                *(f32*)((s32)child + 0x58) = -*(f32*)((s32)child + 0x0C) / temp;
                *(f32*)((s32)child + 0x2C) = float_1p3_804275c4;
                *(f32*)((s32)child + 0x48) = temp;
                *(f32*)((s32)child + 0x4C) = zero;
                break;
            case 7:
                *(f32*)((s32)child + 0x04) = float_400_804275c8;
                *(f32*)((s32)child + 0x08) = float_200_80427518;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x2C) = float_0p5_804274cc;
                break;
            case 8:
                *(f32*)((s32)child + 0x34) = zero;
                *(f32*)((s32)child + 0x30) = float_0p25_804274f0;
                *(f32*)((s32)child + 0x04) = zero;
                *(f32*)((s32)child + 0x08) = float_400_804275c8;
                *(f32*)((s32)child + 0x0C) = zero;
                *(f32*)((s32)child + 0x2C) = float_1_804274d0;
                break;
        }
    }

    return eff;
}


/* CHATGPT STUB FILL: main/effect/eff_fall 20260624_184823 */

/* stub-fill: effFallDispWater | missing_definition | ghidra_signature */
u8 effFallDispWater(void) {
    return 0;
}

/* stub-fill: effFallDisp | missing_definition | ghidra_signature */
u8 effFallDisp(void) {
    return 0;
}

/* stub-fill: effFallMain | prototype_only | source_prototype */
void effFallMain(void) {
    return;
}
