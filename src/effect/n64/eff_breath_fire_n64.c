#include "effect/n64/eff_breath_fire_n64.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBreathFireDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetTevColor(s32 id, void* color);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_256_80424df8;
    extern f32 float_0p0078125_80424dfc;
    extern f32 float_0p03125_80424e00;
    extern f32 float_0_80424e04;
    extern f32 float_deg2rad_80424e08;
    extern u8 DAT_8039e1ce[];
    void* cam;
    void* cam3d;
    f32* work;
    u32 frame;
    s32 alpha;
    u32 color;
    u8 texObj[0x20];
    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    f32 posMtx[3][4];
    f32 rotMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(f32**)((s32)effect + 0xC);
    frame = (u32)work[23];

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 0xA, 0xB, 0xF);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
    GXSetTevAlphaIn(1, 0, 5, 1, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);

    color = ((*(u32*)&work[25] & 0xFF) << 24) |
            ((*(u32*)&work[26] & 0xFF) << 16) |
            ((*(u32*)&work[27] & 0xFF) << 8) |
            (*(u32*)&work[16] & 0xFF);
    GXSetTevColor(1, &color);

    alpha = (s32)(float_256_80424df8 * (work[23] - (f32)frame));
    color = ((*(u32*)&work[28] & 0xFF) << 24) |
            ((*(u32*)&work[29] & 0xFF) << 16) |
            ((*(u32*)&work[30] & 0xFF) << 8) |
            (alpha & 0xFF);
    GXSetTevColor(2, &color);

    effGetTexObjN64(0x77, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x77, texObj);
    GXLoadTexObj(texObj, 1);

    PSMTXScale(scaleMtx, float_0p0078125_80424dfc, float_0p03125_80424e00, float_0_80424e04);
    PSMTXTrans(transMtx, (f32)(frame * 0x20), float_0_80424e04, float_0_80424e04);
    PSMTXConcat(scaleMtx, transMtx, scaleMtx);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);

    PSMTXScale(scaleMtx, float_0p0078125_80424dfc, float_0p03125_80424e00, float_0_80424e04);
    PSMTXTrans(transMtx, (f32)(frame * 0x20 + 0x20), float_0_80424e04, float_0_80424e04);
    PSMTXConcat(scaleMtx, transMtx, scaleMtx);
    GXLoadTexMtxImm(scaleMtx, 0x21, 1);

    PSMTXTrans(posMtx, work[3], work[4], work[5]);
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424e08 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXScale(scaleMtx, work[14], work[14], float_0_80424e04);
    PSMTXConcat(posMtx, rotMtx, posMtx);
    PSMTXConcat(posMtx, scaleMtx, posMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), posMtx, posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    effSetVtxDescN64(DAT_8039e1ce);
    GXBegin(0x90, 0, 0xC);
    tri2(0, 1, 2, 0, 1, 3, 2);
    tri2(4, 2, 3, 0, 4, 0, 2);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void effBreathFireMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effBreathFireDisp(s32 cameraId, void* effect);
    extern void* effBreathFireN64Entry(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                                       s32 type, s32 count, s32 seed, s32 duration);
    extern f32 float_4_80424e0c;
    extern f32 float_10_80424e10;
    extern f32 float_0p05_80424e14;
    extern f32 float_2p5_80424e18;
    extern f32 float_0p01_80424e1c;
    f32* work;
    void* child;
    f32 progress;
    f32 duration;
    s32 timeLeft;
    s32 total;
    LocalVec3 pos;

    work = *(f32**)((s32)effect + 0xC);
    pos.x = work[3];
    pos.y = work[4];
    pos.z = work[5];

    *(s32*)&work[23] = (s32)((float_4_80424e0c * (f32)*(s32*)&work[19]) / float_10_80424e10);
    (*(s32*)&work[18])--;
    (*(s32*)&work[19])++;
    timeLeft = *(s32*)&work[18];

    if (timeLeft < 0) {
        effDelete(effect);
        return;
    }

    total = *(s32*)&work[17];
    if ((timeLeft > 5) && (*(s32*)&work[0] == 0)) {
        work[14] += float_0p05_80424e14 * (float_2p5_80424e18 - work[14]);
    }

    progress = (f32)*(s32*)&work[19];
    duration = (f32)total;
    work[3] = work[6] + (progress * (work[20] + (work[9] - work[6]))) / duration;
    work[4] = work[7] + (progress * (work[21] + (work[10] - work[7]))) / duration;
    work[5] = work[8] + (progress * (work[22] + (work[11] - work[8]))) / duration;

    if (*(s32*)&work[0] == 1) {
        work[24] += float_0p01_80424e1c * progress;
        work[4] += work[24];
    }

    if ((*(s32*)&work[19] == *(s32*)&work[2] + 1) && (*(s32*)&work[1] > 0)) {
        child = effBreathFireN64Entry(work[6], work[7], work[8], work[9], work[10], work[11],
                                      *(s32*)&work[0], *(s32*)&work[1] - 1,
                                      *(s32*)&work[2], total);
        {
            f32* childWork = *(f32**)((s32)child + 0xC);
            *(s32*)&childWork[25] = *(s32*)&work[25];
            *(s32*)&childWork[26] = *(s32*)&work[26];
            *(s32*)&childWork[27] = *(s32*)&work[27];
            *(s32*)&childWork[28] = *(s32*)&work[28];
            *(s32*)&childWork[29] = *(s32*)&work[29];
            *(s32*)&childWork[30] = *(s32*)&work[30];
            childWork[14] = work[12];
            childWork[12] = work[12];
            childWork[13] = work[13];
            childWork[15] = work[15];
        }
    }

    if ((timeLeft < 10) && (*(s32*)&work[0] == 0)) {
        *(s32*)&work[16] = timeLeft * 0x19;
    }

    if (*(s32*)&work[0] == 1) {
        *(s32*)&work[16] = (timeLeft * 0xE0) / total;
        work[14] += 0.008 * (0.3 - work[14]);
    }
    if (*(s32*)&work[0] == 2) {
        *(s32*)&work[16] = (timeLeft * 0xE0) / total;
        work[14] += work[15] * (work[13] - work[14]);
    }

    pos.x = work[3];
    pos.y = work[4];
    pos.z = work[5];
    dispEntry(4, 2, effBreathFireDisp, effect, dispCalcZ(&pos));
}

void* effBreathFireN64Entry(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                            s32 type, s32 count, s32 seed, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern s32 effTblRandN64(s32 max, s32 seed);
    extern void effBreathFireMain(void* effect);
    extern char str_BreathFireN64_802fac40[];
    extern f32 float_0p05_80424e14;
    extern f32 float_0p04_80424e20;
    extern f32 float_0_80424e04;
    extern f32 float_0p1_80424e24;
    extern f32 float_0p2_80424e28;
    void* entry;
    f32* work;
    f32 accelScale;
    s32 randValue;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BreathFireN64_802fac40;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x7C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBreathFireMain;

    *(s32*)&work[0] = type;
    *(s32*)&work[1] = count;
    *(s32*)&work[2] = seed;
    work[3] = x0;
    work[4] = y0;
    work[5] = z0;
    work[6] = x0;
    work[7] = y0;
    work[8] = z0;
    work[9] = x1;
    work[10] = y1;
    work[11] = z1;

    if (type == 0) {
        work[14] = float_0p05_80424e14;
    } else {
        work[14] = float_0p04_80424e20;
    }

    work[12] = work[14];
    work[17] = (f32)duration;
    work[18] = (f32)duration;
    work[19] = float_0_80424e04;
    *(s32*)&work[16] = 0xFF;
    work[23] = float_0_80424e04;
    work[15] = float_0p1_80424e24;
    work[13] = work[12];

    if (type == 0) {
        accelScale = float_0p2_80424e28 * float_0p2_80424e28;
        randValue = effTblRandN64(10, seed);
        work[20] = accelScale * (x1 - x0) * (f32)(randValue - 5);
        randValue = effTblRandN64(10, seed + 1);
        work[21] = accelScale * (y1 - y0) * (f32)(randValue - 5);
        randValue = effTblRandN64(10, seed + 2);
        work[22] = accelScale * (z1 - z0) * (f32)(randValue - 5);
    } else {
        randValue = effTblRandN64(10, seed + 3);
        work[20] = float_0p2_80424e28 * (x1 - x0) * (f32)(randValue - 5);
        randValue = effTblRandN64(10, seed + 4);
        work[21] = float_0p2_80424e28 * (y1 - y0) * (f32)(randValue - 5);
        randValue = effTblRandN64(10, seed + 5);
        work[22] = float_0p2_80424e28 * (z1 - z0) * (f32)(randValue - 5);
    }

    work[24] = float_0_80424e04;
    *(s32*)&work[25] = 0xFF;
    *(s32*)&work[26] = 0xAA;
    *(s32*)&work[27] = 0x2A;
    *(s32*)&work[28] = 0xF3;
    *(s32*)&work[29] = 0x30;
    *(s32*)&work[30] = 0;
    return entry;
}

