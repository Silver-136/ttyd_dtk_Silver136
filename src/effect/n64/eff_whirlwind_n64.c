#include "effect/n64/eff_whirlwind_n64.h"


void effWhirlwindDisp(s32 cameraId, void* entry) {
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* obj);
    extern void GXLoadTexObj(void* obj, s32 map);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* m, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(s32 desc);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_0_80424bd4;
    extern f32 float_0p03125_80424bd0;
    extern f32 float_deg2rad_80424bd8;
    extern f32 float_3_80424bdc;
    extern f32 float_2_80424be0;

    void* cam;
    void* cam3d;
    s32* work;
    s32 i;
    s32 alpha;
    s32 type;
    f32 scale;
    f32 baseScale;
    f32 maxScale;
    f32 offset;
    u32 color;
    u8 texObj[0x20];
    f32 texMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 modelMtx[3][4];
    f32 concatMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(s32**)((s32)entry + 0x0C);
    type = work[0];
    baseScale = (f32)work[5];
    maxScale = (f32)work[6];
    alpha = work[7];

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_0p03125_80424bd0, float_0p03125_80424bd0, float_0_80424bd4);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    effGetTexObjN64(0x58, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xC);
    GXSetTevAlphaIn(0, 0, 4, 0, 7);

    PSMTXTrans(transMtx, (f32)work[2], (f32)work[3], (f32)work[4]);
    cam3d = camGetPtr(3);
    PSMTXRotRad(rotMtx, float_deg2rad_80424bd8 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, modelMtx);
    PSMTXConcat((void*)((s32)cam + 0x120), modelMtx, modelMtx);

    GXSetCullMode(0);
    effSetVtxDescN64(0x8039D418);

    offset = float_3_80424bdc * (baseScale - maxScale);
    for (i = 1; i < *(s32*)((s32)entry + 8); i++, work += 0x12) {
        PSMTXTrans(transMtx, (f32)work[0x14], (f32)work[0x15], (f32)work[0x16]);
        PSMTXRotRad(rotMtx, float_deg2rad_80424bd8 * (f32)work[0x1D], 'x');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXRotRad(rotMtx, float_deg2rad_80424bd8 * (f32)work[0x1F], 'z');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        scale = baseScale * (f32)work[0x17];
        PSMTXScale(concatMtx, scale, scale, scale);
        PSMTXConcat(transMtx, concatMtx, transMtx);
        PSMTXRotRad(rotMtx, float_deg2rad_80424bd8 * (f32)work[0x1E], 'y');
        PSMTXConcat(transMtx, rotMtx, rotMtx);

        if (type < 8) {
            PSMTXTrans(transMtx, float_3_80424bdc - offset / maxScale, float_0_80424bd4, float_0_80424bd4);
        } else {
            PSMTXTrans(transMtx, float_2_80424be0, float_0_80424bd4, float_0_80424bd4);
        }
        PSMTXConcat(rotMtx, transMtx, transMtx);
        PSMTXConcat(modelMtx, transMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);

        color = ((u8)alpha << 24) | (*(u8*)((s32)work + 0x8C) << 16) | (*(u8*)((s32)work + 0x8D) << 8) | *(u8*)((s32)work + 0x8E);
        GXSetTevColor(1, &color);
        GXBegin(0x90, 0, 0x48);
        tri2(0, 1, 2, 0, 0, 2, 3);
        tri2(4, 5, 1, 0, 4, 1, 0);
        tri2(3, 2, 6, 0, 3, 6, 7);
        tri2(8, 9, 10, 0, 8, 10, 0xB);
        tri2(0xC, 0xD, 9, 0, 0xC, 9, 8);
        tri2(0xE, 0xF, 0xD, 0, 0xE, 0xD, 0xC);
        tri2(0x10, 0x11, 0xF, 0, 0x10, 0xF, 0xE);
        tri2(7, 6, 0x12, 0, 7, 0x12, 0x13);
        tri2(0x14, 0x15, 0x11, 0, 0x14, 0x11, 0x10);
        tri2(0x16, 0x17, 0x15, 0, 0x16, 0x15, 0x14);
        tri2(0x18, 0x19, 0x17, 0, 0x18, 0x17, 0x16);
        tri2(0x13, 0x12, 0x19, 0, 0x13, 0x19, 0x18);
    }
}

void* effWhirlwindN64Entry(f32 scale, s32 type, void* follow, s32 lifetime) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 rand(void);
    extern void effWhirlwindMain(void*);
    extern const char str_WhirlwindN64_802faa90[];
    extern u8 col_r[];
    extern u8 col_g[];
    extern u8 col_b[];
    extern f32 float_0_80424bd4;
    extern f32 float_1p2_80424c04;
    extern f32 float_60_80424c08;
    extern f32 float_1_80424c0c;
    extern f32 float_neg60_80424c10;
    extern f32 float_0p5_80424c14;
    extern f32 float_0p25_80424c18;

    void* eff;
    s32* work;
    s32 i;
    s32 y;
    s32 yRot;
    s32 xRot;
    s32 zSeed;
    s32 colorIndex;
    s32 randBit;
    f32 zero;

    eff = effEntry();
    *(const char**)((s32)eff + 0x14) = str_WhirlwindN64_802faa90;
    *(s32*)((s32)eff + 8) = 5;
    work = __memAlloc(3, 0x168);
    *(s32**)((s32)eff + 0xC) = work;
    *(void**)((s32)eff + 0x10) = effWhirlwindMain;

    work[0] = type;
    work[1] = (s32)follow;
    work[5] = (s32)(float_1p2_80424c04 * scale);
    work[6] = (s32)(float_1p2_80424c04 * scale);
    work[7] = 0;
    work[8] = lifetime;
    work[9] = lifetime;
    work[10] = 0;

    i = 1;
    y = 0xB;
    yRot = 0x78;
    xRot = 0x19;
    zSeed = 0x26;
    do {
        zero = float_0_80424bd4;
        switch (type) {
            case 0:
            case 2:
            case 4:
            case 6:
                work[0x14] = (s32)zero;
                work[0x15] = y;
                work[0x16] = (s32)zero;
                work[0x20] = (s32)zero;
                work[0x21] = (s32)float_60_80424c08;
                work[0x22] = (s32)zero;
                work[0x1D] = (s32)zero;
                work[0x1E] = yRot;
                work[0x1F] = (s32)zero;
                work[0x17] = (s32)float_1_80424c0c;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
                work[0x14] = (s32)zero;
                work[0x15] = y;
                work[0x16] = (s32)zero;
                work[0x20] = (s32)zero;
                work[0x21] = (s32)float_neg60_80424c10;
                work[0x22] = (s32)zero;
                work[0x1D] = (s32)zero;
                work[0x1E] = yRot;
                work[0x1F] = (s32)zero;
                work[0x17] = (s32)float_1_80424c0c;
                *(u8*)((s32)work + 0x8C) = 0xFF;
                *(u8*)((s32)work + 0x8D) = 0xFF;
                *(u8*)((s32)work + 0x8E) = 0xFF;
                break;
            default:
                work[0x14] = (s32)zero;
                work[0x15] = (s32)(float_0p5_80424c14 * *(f32*)((s32)follow + 0x1BC));
                work[0x16] = (s32)zero;
                randBit = ((rand() & 1) * 2) - 1;
                work[0x20] = randBit * 4;
                work[0x21] = (s32)zero;
                randBit = ((rand() & 1) * 2) - 1;
                work[0x22] = randBit * 4;
                work[0x17] = (s32)((float_0p5_80424c14 * (f32)(i - 1) * float_0p25_80424c18) + float_0p5_80424c14);
                work[0x1D] = xRot;
                work[0x1E] = ((i - 1) * 0x168) >> 2;
                work[0x1F] = 0x168 - zSeed;
                break;
        }

        if (type < 6) {
            if (type < 4) {
                if (type < 2) {
                    colorIndex = 0;
                } else {
                    colorIndex = 1;
                }
            } else {
                colorIndex = 2;
            }
        } else if (type < 8) {
            colorIndex = (i & 1) + 1;
        } else {
            colorIndex = 0;
        }

        *(u8*)((s32)work + 0x8C) = col_r[colorIndex];
        *(u8*)((s32)work + 0x8D) = col_g[colorIndex];
        *(u8*)((s32)work + 0x8E) = col_b[colorIndex];

        i++;
        y += 7;
        yRot += 0x78;
        xRot += 0x19;
        zSeed += 0x26;
        work += 0x12;
    } while (i <= 4);

    return eff;
}

void effWhirlwindMain(int* entry) {
    extern void effDelete(void* eff);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 camId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effWhirlwindDisp(void);
    extern f32 float_0p05_80424be4;
    extern f32 float_0p3_80424be8;
    extern f32 float_0p8_80424bec;
    extern f32 float_0p1_80424bf0;
    extern f32 float_0p2_80424bf4;
    extern f32 float_40_80424bf8;
    extern f32 float_neg40_80424bfc;
    extern f32 float_10_80424c00;
    extern f32 float_2_80424be0;

    s32 pos[3];
    s32* work;
    s32 type;
    s32 timer;
    s32 i;

    work = (s32*)entry[3];
    pos[0] = work[2];
    pos[1] = work[3];
    pos[2] = work[4];

    work[9]--;
    work[10]++;
    timer = work[9];
    if (timer < 0) {
        effDelete(entry);
        return;
    }

    type = work[0];
    if (type == 8) {
        if (timer > 5) {
            work[7] = (s32)(float_0p05_80424be4 * (f32)(0xFF - work[7]) + (f32)work[7]);
        }
        work[5] = (s32)(((f32)work[6] * (f32)timer) / (f32)work[8]);
        if (timer < 10) {
            work[7] = timer * 0x19;
        }
    } else {
        if (timer > 5) {
            work[7] = (s32)(float_0p3_80424be8 * (f32)(100 - work[7]) + (f32)work[7]);
        }
        if (timer < 10) {
            work[7] = (s32)(float_0p8_80424bec * (f32)work[7]);
            work[5] = (s32)(float_0p1_80424bf0 * ((float_2_80424be0 * (f32)work[6]) - (f32)work[5]) + (f32)work[5]);
        }
    }

    if (work[1] != 0) {
        work[2] = *(s32*)(work[1] + 0x8C);
        work[3] = *(s32*)(work[1] + 0x90);
        work[4] = *(s32*)(work[1] + 0x94);
    }

    for (i = 1; i < entry[2]; i++, work += 0x12) {
        if (type == 8) {
            if ((i & 1) == 0) {
                work[0x21] = (s32)(float_0p2_80424bf4 * (float_neg40_80424bfc - (f32)work[0x21]) + (f32)work[0x21]);
            } else {
                work[0x21] = (s32)(float_0p2_80424bf4 * (float_40_80424bf8 - (f32)work[0x21]) + (f32)work[0x21]);
            }
            work[0x1D] = (s32)((f32)work[0x1D] + (f32)work[0x20]);
            work[0x1E] = (s32)((f32)work[0x1E] + (f32)work[0x21]);
            work[0x1F] = (s32)((f32)work[0x1F] + (f32)work[0x22]);
        } else if (timer < 10) {
            work[0x1E] = (s32)((f32)work[0x1E] + ((f32)work[0x21] * (f32)timer) / float_10_80424c00);
        } else {
            work[0x1E] = (s32)((f32)work[0x1E] + (f32)work[0x21]);
        }
    }

    dispEntry(3, 2, effWhirlwindDisp, entry, dispCalcZ(pos));
}

