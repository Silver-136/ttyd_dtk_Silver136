#include "himo.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void vivihimo(f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32* mtxA, f32* mtxB) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f32 PSVECMag(void* v);
    extern void PSMTXCopy(void* src, void* dst);
    extern void PSVECNormalize(void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern double __frsqrte(double x);
    extern double sin(double x);
    extern void spline_maketable(s32 count, f32* points, f32* out1, f32* out2);
    extern s32 camGetCurNo(void);
    extern void vivihimoDisp(s32 camera, void* work);
    extern f32 float_0_8042423c;
    extern f32 float_0p1_80424258;
    extern f32 float_0p2_80424264;
    extern f32 float_0p25_80424260;
    extern f32 float_0p33333_8042426c;
    extern f32 float_0p5_80424230;
    extern f32 float_1_80424244;
    extern f32 float_2p5_8042425c;
    extern f32 float_3p1416_80424248;
    extern f32 float_10_80424240;
    extern f32 float_neg1_80424268;
    extern double double_to_int_802f8920;
    extern double double_to_int_mask_802f8940;
    extern void* gp;

    f32 posA[3];
    f32 posB[3];
    Vec3 dir;
    Vec3 delta;
    f32 mtxCopyA[12];
    f32 mtxCopyB[12];
    f32 points[13];
    f32 tableA[4];
    f32 tableB[13];
    f32 magA;
    f32 magB;
    f32 width;
    f32 wobble;
    f32 amount;
    s32 i;
    s32 retrace;

    posA[0] = mtxA[3];
    posA[1] = mtxA[7] + param_3;
    posA[2] = mtxA[11];
    posB[0] = mtxB[3];
    posB[1] = mtxB[7] + param_4;
    posB[2] = mtxB[11];

    magA = PSVECMag(mtxA);
    magB = PSVECMag(mtxA + 8);
    width = (float_0p5_80424230 * magA) * (float_0p5_80424230 * magA) +
            (float_0p5_80424230 * magB) * (float_0p5_80424230 * magB);
    if (width > float_0_8042423c) {
        width = __frsqrte(width) * width;
    }
    width = param_2 * (float_0p5_80424230 * width);

    PSMTXCopy(mtxA, mtxCopyA);
    PSMTXCopy(mtxB, mtxCopyB);

    dir.x = mtxA[0];
    dir.y = mtxA[1];
    dir.z = mtxA[2];
    if (dir.x != float_0_8042423c || dir.y != float_0_8042423c || dir.z != float_0_8042423c) {
        PSVECNormalize(&dir, &dir);
        dir.x = -dir.x;
        dir.y = -dir.y;
        dir.z = -dir.z;
        retrace = *(s32*)((s32)gp + 0x1C);
        wobble = float_2p5_8042425c *
                 (f32)sin(float_0p1_80424258 * ((f32)retrace * param_1));
        PSVECSubtract(posB, posA, &delta);

        for (i = 0; i < 4; i++) {
            amount = float_10_80424240 *
                     (f32)sin(float_3p1416_80424248 * (f32)i * float_0p25_80424260) + wobble;
            if (i == 1) {
                amount = width * float_0p2_80424264 * amount;
            } else if (i == 2) {
                amount = width * amount;
            } else {
                amount = width * float_neg1_80424268 * (f32)(i & 1) * amount;
            }
            points[i * 3 + 0] = dir.x * amount + float_0p33333_8042426c * delta.x * (f32)i + posA[0];
            points[i * 3 + 1] = dir.y * amount + float_0p33333_8042426c * delta.y * (f32)i + posA[1];
            points[i * 3 + 2] = dir.z * amount + float_0p33333_8042426c * delta.z * (f32)i + posA[2];
        }
        points[0] = posA[0];
        points[1] = posA[1];
        points[2] = posA[2];
        points[9] = posB[0];
        points[10] = posB[1];
        points[11] = posB[2];
        spline_maketable(4, points, tableA, tableB);
        vivihimoDisp(camGetCurNo(), &posA);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void vivihimoDisp(s32 cameraId, void* work) {
    extern void* camGetPtr(s32 cameraId);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(u32 attr, u32 type);
    extern void GXSetVtxAttrFmt(u32 fmt, u32 attr, u32 cnt, u32 type, u32 frac);
    extern void GXSetArray(u32 attr, void* base, u8 stride);
    extern double tan(double x);
    extern void spline_getvalue(f32 t, f32* out, s32 count, f32* points, void* tableA, void* tableB);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(u32 prim, u32 fmt, u16 count);
    extern f32 float_0_8042423c;
    extern f32 float_0p066667_80424234;
    extern f32 float_0p5_80424230;
    extern f32 float_0p82_8042424c;
    extern f32 float_neg1p2881_80424238;
    extern f32 float_1_80424244;
    extern f32 float_1p2881_80424250;
    extern f32 float_3p1416_80424248;
    extern f32 float_10_80424240;
    extern f32 float_15_80424254;
    extern double double_1p2881_802f8918;
    extern double double_to_int_802f8920;
    extern u8 lbl_80418360[];
    extern u8 lbl_80418380[];
    extern u8 lbl_804183a0[];

    f32 pos[3];
    f32 mtx[12];
    f32 mtxA[12];
    f32 mtxB[12];
    f32 t;
    f32 invTan;
    f32 prevWidth;
    f32 width;
    f32 oneMinus;
    f32 angleTan;
    s32 i;
    volatile u8* fifo8;
    volatile f32* fifoF;

    camGetPtr(cameraId);
    GXClearVtxDesc();
    GXSetVtxDesc(0, 1);
    GXSetVtxDesc(9, 2);
    GXSetVtxDesc(0xA, 2);
    GXSetVtxDesc(0xB, 2);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 1, 0);
    GXSetVtxAttrFmt(0, 0xA, 0, 1, 7);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetArray(9, lbl_80418360, 3);
    GXSetArray(0xA, lbl_80418380, 3);
    GXSetArray(0xB, lbl_804183a0, 4);

    invTan = float_0p5_80424230 / (f32)tan(double_1p2881_802f8918);
    angleTan = (f32)tan(float_neg1p2881_80424238);
    t = float_0_8042423c;
    spline_getvalue(t, pos, 4, (f32*)((s32)work + 0x78), (void*)((s32)work + 0xA8), (void*)((s32)work + 0xB8));

    mtx[0] = float_10_80424240 * (float_1_80424244 * *(f32*)((s32)work + 0x18) + float_0_8042423c * *(f32*)((s32)work + 0x48));
    mtx[1] = float_1_80424244 * *(f32*)((s32)work + 0x1C) + float_0_8042423c * *(f32*)((s32)work + 0x4C);
    mtx[2] = float_1_80424244 * *(f32*)((s32)work + 0x20) + float_0_8042423c * *(f32*)((s32)work + 0x50);
    mtx[3] = pos[0];
    mtx[4] = float_10_80424240 * (float_1_80424244 * *(f32*)((s32)work + 0x28) + float_0_8042423c * *(f32*)((s32)work + 0x58));
    mtx[5] = float_1_80424244 * *(f32*)((s32)work + 0x2C) + float_0_8042423c * *(f32*)((s32)work + 0x5C);
    mtx[6] = float_1_80424244 * *(f32*)((s32)work + 0x30) + float_0_8042423c * *(f32*)((s32)work + 0x60);
    mtx[7] = pos[1];
    mtx[8] = float_10_80424240 * (float_1_80424244 * *(f32*)((s32)work + 0x38) + float_0_8042423c * *(f32*)((s32)work + 0x68));
    mtx[9] = float_1_80424244 * *(f32*)((s32)work + 0x3C) + float_0_8042423c * *(f32*)((s32)work + 0x6C);
    mtx[10] = float_1_80424244 * *(f32*)((s32)work + 0x40) + float_0_8042423c * *(f32*)((s32)work + 0x70);
    mtx[11] = pos[2];
    PSMTXConcat((void*)((s32)camGetPtr(cameraId) + 0x11C), mtx, mtxB);

    prevWidth = invTan * ((f32)tan(double_1p2881_802f8918) + angleTan);
    t = float_0p066667_80424234;
    for (i = 0; i < 0xF; i++) {
        spline_getvalue(t, pos, 4, (f32*)((s32)work + 0x78), (void*)((s32)work + 0xA8), (void*)((s32)work + 0xB8));
        GXLoadPosMtxImm(mtxB, 0);
        oneMinus = float_1_80424244 - t;
        mtx[0] = float_10_80424240 * (oneMinus * *(f32*)((s32)work + 0x18) + t * *(f32*)((s32)work + 0x48));
        mtx[1] = oneMinus * *(f32*)((s32)work + 0x1C) + t * *(f32*)((s32)work + 0x4C);
        mtx[2] = oneMinus * *(f32*)((s32)work + 0x20) + t * *(f32*)((s32)work + 0x50);
        mtx[3] = pos[0];
        mtx[4] = float_10_80424240 * (oneMinus * *(f32*)((s32)work + 0x28) + t * *(f32*)((s32)work + 0x58));
        mtx[5] = oneMinus * *(f32*)((s32)work + 0x2C) + t * *(f32*)((s32)work + 0x5C);
        mtx[6] = oneMinus * *(f32*)((s32)work + 0x30) + t * *(f32*)((s32)work + 0x60);
        mtx[7] = pos[1];
        mtx[8] = float_10_80424240 * (oneMinus * *(f32*)((s32)work + 0x38) + t * *(f32*)((s32)work + 0x68));
        mtx[9] = oneMinus * *(f32*)((s32)work + 0x3C) + t * *(f32*)((s32)work + 0x6C);
        mtx[10] = oneMinus * *(f32*)((s32)work + 0x40) + t * *(f32*)((s32)work + 0x70);
        mtx[11] = pos[2];
        PSMTXConcat((void*)((s32)camGetPtr(cameraId) + 0x11C), mtx, mtxA);
        GXLoadPosMtxImm(mtxA, 3);
        width = invTan * ((f32)tan(float_3p1416_80424248 * (float_0p82_8042424c * t) - float_1p2881_80424250) +
                          (f32)tan(double_1p2881_802f8918));

        GXBegin(0x80, 0, 4);
        fifo8 = (volatile u8*)0xCC008000;
        fifoF = (volatile f32*)0xCC008000;
        *fifo8 = 0;
        *fifo8 = (u8)(i + 2);
        *fifo8 = 1;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifoF = float_0_8042423c;
        *fifoF = prevWidth;
        *fifo8 = 3;
        *fifo8 = 1;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifoF = float_0_8042423c;
        *fifoF = width;
        *fifo8 = 3;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifoF = float_1_80424244;
        *fifoF = width;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifoF = float_1_80424244;
        *fifoF = prevWidth;

        t = (f32)(i + 2) / float_15_80424254;
        prevWidth = width;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

