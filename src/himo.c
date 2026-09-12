#include "himo.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void vivihimo(f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32* mtxA, f32* mtxB) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef union FloatBits {
        f32 value;
        u32 bits;
    } FloatBits;
    extern f32 PSVECMag(void* v);
    extern void PSMTXCopy(void* src, void* dst);
    extern void PSVECNormalize(void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f64 __frsqrte(f64 x);
    extern double sin(double x);
    extern void spline_maketable(s32 count, f32* points, f32* out1, f32* out2);
    extern s32 camGetCurNo(void);
    extern void vivihimoDisp(s32 camera, void* work);
    extern f32 float_0_8042423c;
    extern f32 float_0p5_80424230;
    extern f32 float_1_80424244;
    extern f32 float_3p1416_80424248;
    extern f32 float_10_80424240;
    extern f32 float_0p1_80424258;
    extern f32 float_0p2_80424264;
    extern f32 float_0p25_80424260;
    extern f32 float_0p33333_8042426c;
    extern f32 float_2p5_8042425c;
    extern f32 float_neg1_80424268;
    extern double double_to_int_mask_802f8940;
    extern f64 double_0p5_802f8928;
    extern f64 double_3_802f8930;
    extern f64 double_0_802f8938;
    extern f32 __float_nan;
    extern Vec3 vec3_802f8908;
    extern void* gp;

    typedef struct Work {
        Vec3 posA;
        Vec3 posB;
        f32 mtxCopyA[12];
        f32 mtxCopyB[12];
        Vec3 points[4];
        f32 tableA[4];
        f32 tableB[13];
    } Work;
    Work work;
    Vec3 dir;
    Vec3 negDir;
    Vec3 delta;
    Vec3 temp;
    f32 magA;
    f32 magB;
    f32 width;
    f32 wobble;
    f32 amount;
    f32 pi;
    f32 quarter;
    f32 ten;
    f32 third;
    f32 baseX;
    f32 deltaY;
    f32 deltaZ;
    f32 coordIndex;
    register f64 invRoot;
    u8* constants;
    Vec3* point;
    FloatBits classified;
    s32 floatClass;
    s32 i;
    s32 retrace;

    constants = (u8*)&vec3_802f8908;
    work.posA.x = mtxA[3];
    work.posA.y = mtxA[7] + param_3;
    work.posA.z = mtxA[11];
    work.posB.x = mtxB[3];
    work.posB.y = mtxB[7] + param_4;
    work.posB.z = mtxB[11];

    magA = PSVECMag(mtxA);
    magB = PSVECMag(mtxA + 8);
    width = (float_0p5_80424230 * magA) * (float_0p5_80424230 * magA) +
            (float_0p5_80424230 * magB) * (float_0p5_80424230 * magB);
    if (width > float_0_8042423c) {
        invRoot = __frsqrte((f64)width);
        invRoot = (*(f64*)(constants + 0x20) * invRoot) *
                  (*(f64*)(constants + 0x28) - (f64)width * invRoot * invRoot);
        invRoot = (*(f64*)(constants + 0x20) * invRoot) *
                  (*(f64*)(constants + 0x28) - (f64)width * invRoot * invRoot);
        invRoot = (*(f64*)(constants + 0x20) * invRoot) *
                  (*(f64*)(constants + 0x28) - (f64)width * invRoot * invRoot);
        width = (f32)((f64)width * invRoot);
    } else if ((f64)width < *(f64*)(constants + 0x30)) {
        width = __float_nan;
    } else {
        classified.value = width;
        if ((classified.bits & 0x7F800000) == 0x7F800000) {
            if ((classified.bits & 0x7FFFFF) != 0) {
                floatClass = 1;
            } else {
                floatClass = 2;
            }
        } else if ((classified.bits & 0x7F800000) == 0) {
            if ((classified.bits & 0x7FFFFF) != 0) {
                floatClass = 5;
            } else {
                floatClass = 3;
            }
        } else {
            floatClass = 4;
        }
        if (floatClass == 1) {
            width = __float_nan;
        }
    }
    width = param_2 * (float_0p5_80424230 * width);

    PSMTXCopy(mtxA, work.mtxCopyA);
    PSMTXCopy(mtxB, work.mtxCopyB);

    dir.x = mtxA[0];
    dir.y = mtxA[1];
    dir.z = mtxA[2];
    if (dir.x != float_0_8042423c || dir.y != float_0_8042423c || dir.z != float_0_8042423c) {
        PSVECNormalize(&dir, &dir);
        temp = *(Vec3*)constants;
        temp.x = -dir.x;
        temp.y = -dir.y;
        temp.z = -dir.z;
        negDir = temp;
        retrace = *(s32*)((s32)gp + 0x1C);
        wobble = float_2p5_8042425c *
                 (f32)sin(float_0p1_80424258 * ((f32)retrace * param_1));
        PSVECSubtract(&work.posB, &work.posA, &delta);
        pi = float_3p1416_80424248;
        quarter = float_0p25_80424260;
        ten = float_10_80424240;
        third = float_0p33333_8042426c;
        baseX = negDir.x;
        deltaY = delta.y;
        deltaZ = delta.z;

        point = work.points;
        for (i = 0; i < 4; i++, point++) {
            amount = ten * (f32)sin(pi * (f32)i * quarter) + wobble;
            if (i == 1) {
                amount = width * float_0p2_80424264 * amount;
            } else if (i == 2) {
                amount = width * amount;
            } else {
                amount = width * float_neg1_80424268 * (f32)(i & 1) * amount;
            }
            coordIndex = (f32)i;
            point->x = baseX * amount + (third * (delta.x * coordIndex) + work.posA.x);
            point->y = negDir.y * amount + (third * (deltaY * coordIndex) + work.posA.y);
            point->z = negDir.z * amount + (third * (deltaZ * coordIndex) + work.posA.z);
        }
        work.points[0] = work.posA;
        work.points[3] = work.posB;
        spline_maketable(4, (f32*)work.points, work.tableA, work.tableB);
        vivihimoDisp(camGetCurNo(), &work);
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
    extern f32 float_0p5_80424230;
    extern f32 float_1_80424244;
    extern f32 float_3p1416_80424248;
    extern f32 float_10_80424240;
    extern f32 float_0p066667_80424234;
    extern f32 float_0p82_8042424c;
    extern f32 float_neg1p2881_80424238;
    extern f32 float_1p2881_80424250;
    extern f32 float_15_80424254;
    extern double double_1p2881_802f8918;
    extern u8 Pos[];
    extern u8 Nrm[];
    extern u8 Col[];

    f32 pos[3];
    f32 mtxB[12];
    f32 mtxA[12];
    f32 mtx[12];
    f32 t;
    f32 baseTan;
    f32 invTan;
    f32 prevWidth;
    f32 width;
    f32 oneMinus;
    f32 angleTan;
    f32 one;
    f32 ten;
    f32 pi;
    f32 ratio;
    f32 angleOffset;
    f32 divisor;
    s32 i;
    f32* points;
    void* tableA;
    void* tableB;
    void* camera;
    volatile u8* fifo8;
    volatile f32* fifoF;

    points = (f32*)((s32)work + 0x78);
    tableA = (void*)((s32)work + 0xA8);
    tableB = (void*)((s32)work + 0xB8);
    camera = camGetPtr(cameraId);
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
    GXSetArray(9, Pos, 3);
    GXSetArray(0xA, Nrm, 3);
    GXSetArray(0xB, Col, 4);

    baseTan = (f32)tan(double_1p2881_802f8918);
    invTan = float_0p5_80424230 / baseTan;
    angleTan = (f32)tan(float_neg1p2881_80424238);
    t = float_0_8042423c;
    spline_getvalue(t, pos, 4, points, tableA, tableB);

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
    PSMTXConcat((void*)((s32)camera + 0x11C), mtx, mtxB);

    prevWidth = invTan * (baseTan + angleTan);
    t = float_0p066667_80424234;
    one = float_1_80424244;
    ten = float_10_80424240;
    pi = float_3p1416_80424248;
    ratio = float_0p82_8042424c;
    angleOffset = float_1p2881_80424250;
    divisor = float_15_80424254;
    for (i = 0; i < 0xF; i++) {
        spline_getvalue(t, pos, 4, points, tableA, tableB);
        GXLoadPosMtxImm(mtxB, 0);
        oneMinus = one - t;
        mtx[0] = ten * (oneMinus * *(f32*)((s32)work + 0x18) + t * *(f32*)((s32)work + 0x48));
        mtx[1] = oneMinus * *(f32*)((s32)work + 0x1C) + t * *(f32*)((s32)work + 0x4C);
        mtx[2] = oneMinus * *(f32*)((s32)work + 0x20) + t * *(f32*)((s32)work + 0x50);
        mtx[3] = pos[0];
        mtx[4] = ten * (oneMinus * *(f32*)((s32)work + 0x28) + t * *(f32*)((s32)work + 0x58));
        mtx[5] = oneMinus * *(f32*)((s32)work + 0x2C) + t * *(f32*)((s32)work + 0x5C);
        mtx[6] = oneMinus * *(f32*)((s32)work + 0x30) + t * *(f32*)((s32)work + 0x60);
        mtx[7] = pos[1];
        mtx[8] = ten * (oneMinus * *(f32*)((s32)work + 0x38) + t * *(f32*)((s32)work + 0x68));
        mtx[9] = oneMinus * *(f32*)((s32)work + 0x3C) + t * *(f32*)((s32)work + 0x6C);
        mtx[10] = oneMinus * *(f32*)((s32)work + 0x40) + t * *(f32*)((s32)work + 0x70);
        mtx[11] = pos[2];
        PSMTXConcat((void*)((s32)camera + 0x11C), mtx, mtxA);
        GXLoadPosMtxImm(mtxA, 3);
        width = invTan * ((f32)tan(pi * (ratio * t) - angleOffset) + baseTan);

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
        *fifoF = one;
        *fifoF = width;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifo8 = 0;
        *fifoF = one;
        *fifoF = prevWidth;

        t = (f32)(i + 2) / divisor;
        prevWidth = width;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 Pos[6] = { 1, 0, 0, 0xFF, 0, 0 };
u8 gap_07_80418366_sdata[26] = { 0 };
u8 Nrm[3] = { 0, 0, 0x7F };
u8 gap_07_80418383_sdata[29] = { 0 };
u8 Col[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

const f32 vec3_802f8908[3] = { 0.0f, 0.0f, 0.0f };
const f64 double_1p2881_802f8918 = 1.288053035736084;
const f64 double_to_int_802f8920 = 4503601774854144.0;
const f64 double_0p5_802f8928 = 0.5;
const f64 double_3_802f8930 = 3.0;
const f64 double_0_802f8938 = 0.0;
const f64 double_to_int_mask_802f8940 = 4503599627370496.0;

const f32 float_0p5_80424230 = 0.5f;
const f32 float_0p066667_80424234 = 0.06666667f;
const f32 float_neg1p2881_80424238 = -1.288053f;
const f32 float_0_8042423c = 0.0f;
const f32 float_10_80424240 = 10.0f;
const f32 float_1_80424244 = 1.0f;
const f32 float_3p1416_80424248 = 3.1415927f;
const f32 float_0p82_8042424c = 0.82f;
const f32 float_1p2881_80424250 = 1.288053f;
const f32 float_15_80424254 = 15.0f;
const f32 float_0p1_80424258 = 0.1f;
const f32 float_2p5_8042425c = 2.5f;
const f32 float_0p25_80424260 = 0.25f;
const f32 float_0p2_80424264 = 0.2f;
const f32 float_neg1_80424268 = -1.0f;
const f32 float_0p33333_8042426c = 0.33333334f;
