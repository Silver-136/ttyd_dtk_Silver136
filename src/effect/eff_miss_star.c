#include "effect/eff_miss_star.h"


u8 effMissStarDisp(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void PSMTXTrans(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void PSMTXScale(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void* camGetPtr(s32 cameraId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern f32 float_deg2rad_80422e50;
    extern f32 float_neg16_80422e54;
    extern f32 float_16_80422e58;
    extern f32 float_0_80422e5c;
    extern f32 float_1_80422e60;
    LocalMtx trans;
    LocalMtx rotY;
    LocalMtx rotZ;
    LocalMtx scale;
    void* work;
    void* cam;
    u8 texObj[0x20];
    s32 i;
    volatile f32* fifo;

    work = *(void**)((s32)effEntry + 0xC);
    effGetTexObj(0x18, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, work = (void*)((s32)work + 0x44)) {
        if (*(s32*)((s32)work + 0x28) == 0) {
            PSMTXTrans(trans, *(f32*)((s32)work + 0x0), *(f32*)((s32)work + 0x4), *(f32*)((s32)work + 0x8));
            cam = camGetPtr(4);
            PSMTXRotRad(rotY, float_deg2rad_80422e50 * -*(f32*)((s32)cam + 0x114), 'y');
            PSMTXRotRad(rotZ, float_deg2rad_80422e50 * *(f32*)((s32)work + 0x2C), 'z');
            PSMTXScale(scale, *(f32*)((s32)work + 0x38), *(f32*)((s32)work + 0x38), *(f32*)((s32)work + 0x38));
            PSMTXConcat(rotY, trans, trans);
            PSMTXConcat(trans, rotZ, trans);
            PSMTXConcat(trans, scale, trans);
            cam = camGetPtr(4);
            PSMTXConcat((void*)((s32)cam + 0x11C), trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);
            fifo = (volatile f32*)0xCC008000;
            *fifo = float_neg16_80422e54;
            *fifo = float_16_80422e58;
            *fifo = float_0_80422e5c;
            *fifo = float_0_80422e5c;
            *fifo = float_0_80422e5c;
            *fifo = float_16_80422e58;
            *fifo = float_16_80422e58;
            *fifo = float_0_80422e5c;
            *fifo = float_1_80422e60;
            *fifo = float_0_80422e5c;
            *fifo = float_16_80422e58;
            *fifo = float_neg16_80422e54;
            *fifo = float_0_80422e5c;
            *fifo = float_1_80422e60;
            *fifo = float_1_80422e60;
            *fifo = float_neg16_80422e54;
            *fifo = float_neg16_80422e54;
            *fifo = float_0_80422e5c;
            *fifo = float_0_80422e5c;
            *fifo = float_1_80422e60;
        }
    }
    return 0;
}

void* effMissStarEntry(f64 x, f64 y, f64 z, s32 count, s32 delay, s32 seed) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* camGetPtr(s32 cameraId);
    extern u8 effMissStarMain(void* effEntry);
    extern char str_SmallStar_802f3c9c[];
    extern f32 spd_tbl[];
    extern f32 scale_tbl[];
    extern f32 float_0_80422e5c;
    extern f32 float_1_80422e60;
    extern f32 float_0p8_80422e64;
    extern f32 float_0p4_80422e6c;
    extern f32 float_0p7_80422e70;
    extern f32 float_2_80422e74;
    extern f32 float_255_80422e78;
    void* entry;
    void* work;
    void* cam;
    f32 speedScale;
    f32 rotScale;
    f32 dir;
    s32 i;
    s32 idx;

    entry = effEntry();
    speedScale = float_0p8_80422e64;
    rotScale = float_1_80422e60;
    if (count < 1) {
        count = 1;
        speedScale = float_0p4_80422e6c;
        rotScale = float_0p7_80422e70;
    }
    dir = float_1_80422e60;
    if (seed < 0) {
        seed = -seed;
        dir = -float_1_80422e60;
    }
    while (seed > 5) {
        seed -= 5;
    }

    *(char**)((s32)entry + 0x14) = str_SmallStar_802f3c9c;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 0x44);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMissStarMain;

    for (i = 0; i < count; i++, work = (void*)((s32)work + 0x44), seed++) {
        idx = seed;
        while (idx > 5) {
            idx -= 5;
        }
        idx *= 3;
        *(f32*)((s32)work + 0x0) = (f32)x;
        *(f32*)((s32)work + 0x4) = (f32)y;
        *(f32*)((s32)work + 0x8) = (f32)z;
        *(f32*)((s32)work + 0x18) = rotScale * dir * float_2_80422e74 * spd_tbl[idx + 0];
        *(f32*)((s32)work + 0x1C) = rotScale * float_2_80422e74 * spd_tbl[idx + 1];
        *(f32*)((s32)work + 0x20) = rotScale * float_2_80422e74 * spd_tbl[idx + 2];
        *(f32*)((s32)work + 0xC) = rotScale * dir * spd_tbl[idx + 0];
        *(f32*)((s32)work + 0x10) = rotScale * spd_tbl[idx + 1];
        *(f32*)((s32)work + 0x14) = rotScale * spd_tbl[idx + 2];
        *(f32*)((s32)work + 0x38) = speedScale * scale_tbl[(i & 7) * 3];
        *(f32*)((s32)work + 0x2C) = float_0_80422e5c;
        *(f32*)((s32)work + 0x30) = dir * 107.0f;
        cam = camGetPtr(4);
        *(f32*)((s32)work + 0x34) = -*(f32*)((s32)cam + 0x114);
        *(s32*)((s32)work + 0x24) = 14;
        *(s32*)((s32)work + 0x28) = delay;
        *(s32*)((s32)work + 0x3C) = 10;
        *(f32*)((s32)work + 0x40) = float_255_80422e78;
    }
    return entry;
}

u8 effMissStarMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* camGetPtr(s32 cameraId);
    extern f32 reviseAngle(f32 angle);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effMissStarDisp(void);
    extern f32 float_0p8_80422e64;
    extern f32 float_0p5_80422e68;
    LocalVec3 pos;
    void* work;
    void* cam;
    s32 i;

    work = *(void**)((s32)effEntry + 0xC);
    pos.x = *(f32*)((s32)work + 0x0);
    pos.y = *(f32*)((s32)work + 0x4);
    pos.z = *(f32*)((s32)work + 0x8);

    i = 0;
    while (i < *(s32*)((s32)effEntry + 0x8)) {
        if (*(s32*)((s32)work + 0x28) != 0) {
            *(s32*)((s32)work + 0x28) -= 1;
            if (*(s32*)((s32)work + 0x28) == 0) {
                break;
            }
        }

        if (*(s32*)((s32)work + 0x24) > -1) {
            *(f32*)((s32)work + 0x0) += *(f32*)((s32)work + 0x18);
            *(f32*)((s32)work + 0x4) += *(f32*)((s32)work + 0x1C);
            *(f32*)((s32)work + 0x8) += *(f32*)((s32)work + 0x20);
        }
        cam = camGetPtr(4);
        *(f32*)((s32)work + 0x34) = -*(f32*)((s32)cam + 0x114);
        *(f32*)((s32)work + 0x2C) += *(f32*)((s32)work + 0x30);
        *(f32*)((s32)work + 0x2C) = reviseAngle(*(f32*)((s32)work + 0x2C));
        *(f32*)((s32)work + 0x30) *= float_0p8_80422e64;

        if (*(s32*)((s32)work + 0x24) < 10) {
            *(f32*)((s32)work + 0xC) *= float_0p5_80422e68;
            *(f32*)((s32)work + 0x10) *= float_0p5_80422e68;
            *(f32*)((s32)work + 0x14) *= float_0p5_80422e68;
            *(f32*)((s32)work + 0x18) = *(f32*)((s32)work + 0xC);
            *(f32*)((s32)work + 0x1C) = *(f32*)((s32)work + 0x10);
            *(f32*)((s32)work + 0x20) = *(f32*)((s32)work + 0x14);
        }

        *(s32*)((s32)work + 0x24) -= 1;
        if (*(s32*)((s32)work + 0x24) < 0) {
            *(s32*)((s32)work + 0x3C) -= 1;
            if (*(s32*)((s32)work + 0x3C) < 0) {
                effDelete(effEntry);
                return 0;
            }
        }
        i++;
        work = (void*)((s32)work + 0x44);
    }

    dispEntry(4, 1, effMissStarDisp, effEntry, dispCalcZ(&pos));
    return 0;
}

