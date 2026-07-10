#include "effect/eff_recovery.h"


u8 effRecoveryMain(void) {
    return 0;
}


u8 effRecoveryDisp(int param_1, int param_2) {
    return 0;
}


u8 effRecoveryDisp3(void) {
    return 0;
}


u8 effRecoveryDisp4(void) {
    return 0;
}


void effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z) {
    ;
}


void effRecoveryDisp2(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern s32 sprintf(char* str, char* fmt, ...);
    extern u32 strlen(char* str);
    extern void PSMTXTrans(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void PSMTXConcat(LocalMtx a, LocalMtx b, LocalMtx ab);
    extern void PSMTXScale(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void effNumberGX(LocalMtx mtx, s32 colorType, s32 value, s32 alpha, s32 flag);
    extern char str_PCTd_80422df8[];
    extern f32 float_deg2rad_80422dc0;
    extern f32 float_0_80422dcc;
    extern f32 float_1_80422dd0;
    extern f32 float_2_80422dd8;
    extern f32 float_0p5_80422ddc;
    extern f32 float_10_80422de4;
    extern f32 float_0p035_80422dfc;
    extern f32 float_24_80422e00;
    LocalMtx textMtx;
    LocalMtx rotMtx;
    LocalMtx tmpMtx;
    void* work;
    char buf[32];
    s32 type;
    s32 value;
    u32 len;

    work = *(void**)((s32)effEntry + 0xC);
    camGetPtr(cameraId);
    value = *(s8*)((s32)work + 0x1);
    type = *(s8*)((s32)work + 0x0);

    sprintf(buf, str_PCTd_80422df8, value);
    len = strlen(buf);

    PSMTXTrans(textMtx, *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10));
    PSMTXRotRad(rotMtx, float_deg2rad_80422dc0 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(textMtx, rotMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_0_80422dcc, float_1_80422dd0);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXScale(tmpMtx, float_10_80422de4, float_10_80422de4, float_10_80422de4);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_2_80422dd8, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXConcat(textMtx, (void*)(*(s32*)((s32)work + 0x38) + 0x14), textMtx);
    PSMTXScale(tmpMtx, float_0p035_80422dfc, float_0p035_80422dfc, float_0p035_80422dfc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_24_80422e00 * (f32)-(s32)(len - 1) * float_0p5_80422ddc, float_0_80422dcc, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);

    switch (type) {
        case 0:
        case 2:
        case 7:
            effNumberGX(textMtx, 0, value, 0xFF, 1);
            break;
        case 1:
        case 3:
        case 8:
            effNumberGX(textMtx, 1, value, 0xFF, 1);
            break;
    }
}

u8 effRecoveryMain2(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* effStardustEntry(f64 x, f64 y, f64 z, f64 a, f64 b, s32 c, s32 d, s32 e);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effRecoveryDisp4(void);
    extern f32 float_0_80422dcc;
    extern f32 float_2_80422dd8;
    extern f32 float_40_80422e14;
    extern f32 float_45_80422e18;
    extern f32 float_0p95_80422e1c;
    extern f32 float_6p2832_80422e20;
    extern f32 float_360_80422e24;
    extern double sin(double x);
    extern double cos(double x);
    LocalVec3 pos;
    void* work;
    void* part;
    s32 i;
    f32 rad;

    work = *(void**)((s32)effEntry + 0xC);
    pos.x = *(f32*)((s32)work + 0x8);
    pos.y = *(f32*)((s32)work + 0xC);
    pos.z = *(f32*)((s32)work + 0x10);

    if (*(s16*)((s32)work + 0x2) < 1000) {
        *(s16*)((s32)work + 0x2) -= 1;
    }
    if (*(s16*)((s32)work + 0x2) < 0x20) {
        *(s32*)((s32)work + 0x4) = *(s16*)((s32)work + 0x2) << 3;
    }
    if (*(s16*)((s32)work + 0x2) < 0) {
        effStardustEntry(*(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10),
                         float_40_80422e14, float_45_80422e18, 2, 6, 0x1E);
        effDelete(effEntry);
        return 0;
    }

    part = (void*)((s32)work + 0x44);
    for (i = 1; i < *(s32*)((s32)effEntry + 0x8); i++, part = (void*)((s32)part + 0x44)) {
        if (*(s32*)((s32)part + 0x3C) == 0) {
            *(f32*)((s32)part + 0x8) += *(f32*)((s32)part + 0x14);
            *(f32*)((s32)part + 0xC) += *(f32*)((s32)part + 0x18);
            *(f32*)((s32)part + 0x10) += *(f32*)((s32)part + 0x1C);
            *(f32*)((s32)part + 0x14) *= float_0p95_80422e1c;
            *(f32*)((s32)part + 0x18) *= float_0p95_80422e1c;
            *(f32*)((s32)part + 0x1C) *= float_0p95_80422e1c;
            if (i == 1) {
                *(s32*)((s32)part + 0x4) += 5;
                if (*(s32*)((s32)part + 0x4) > 0x80) {
                    *(s32*)((s32)part + 0x4) = 0x80;
                }
            } else {
                *(s32*)((s32)part + 0x4) -= 8;
                if (*(s32*)((s32)part + 0x4) < 0) {
                    *(f32*)((s32)part + 0x8) = float_0_80422dcc;
                    *(f32*)((s32)part + 0xC) = float_0_80422dcc;
                    *(f32*)((s32)part + 0x10) = float_0_80422dcc;
                    rad = (float_6p2832_80422e20 * *(f32*)((s32)part + 0x30)) / float_360_80422e24;
                    *(f32*)((s32)part + 0x14) = float_2_80422dd8 * (f32)cos(rad);
                    *(f32*)((s32)part + 0x18) = float_2_80422dd8 * (f32)sin(rad);
                    *(s32*)((s32)part + 0x4) = 0xFF;
                }
            }
        } else {
            *(s32*)((s32)part + 0x3C) -= 1;
        }
    }

    dispEntry(4, 2, effRecoveryDisp4, effEntry, dispCalcZ(&pos));
    return 0;
}

