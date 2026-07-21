#include "effect/n64/eff_akari_charge_n64.h"

void* effAkariChargeN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effAkariChargeMain(void);
    extern char str_AkariChargeN64_802faad0[];
    extern f32 float_0_80424c24;
    extern f32 float_1_80424c2c;
    void* entry;
    void* work;
    s32 i;
    s32 j;
    s32 seq;
    u8* particle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_AkariChargeN64_802faad0;
    *(s32*)((s32)entry + 8) = 0x16;
    work = __memAlloc(3, 0x5D8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariChargeMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)((s32)work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x10) = 1000;
    } else {
        *(s32*)((s32)work + 0x10) = timer;
    }
    *(s32*)((s32)work + 0x24) = 0;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x38) = scale;
    *(s32*)((s32)work + 0x18) = 0xFF;
    *(s32*)((s32)work + 0x1C) = 0xE6;
    *(s32*)((s32)work + 0x20) = 0;
    *(s32*)((s32)work + 0x28) = 0xFF;
    *(s32*)((s32)work + 0x2C) = 0xFF;
    *(s32*)((s32)work + 0x30) = 0xFF;

    particle = (u8*)work + 0x44;
    seq = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++, seq++, particle += 0x44) {
            *(s32*)(particle + 0x10) = 0x14;
            *(s32*)(particle + 0x14) = 0;
            *(f32*)(particle + 4) = float_0_80424c24;
            *(f32*)(particle + 8) = float_0_80424c24;
            *(f32*)(particle + 0x34) = float_1_80424c2c;
            *(s32*)(particle + 0x3C) = (seq * 360) / 21;
            *(s32*)(particle + 0x40) = seq & 0xF;
        }
    }

    return entry;
}


void effAkariChargeMain(void* entry) {
    extern void effDelete(void* entry);
    extern int rand(void);
    extern f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f64 sin(f64 x);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effAkariChargeDisp(void);
    extern f32 float_0_80424c24;
    extern f32 float_6p2832_80424c38;
    extern f32 float_360_80424c3c;
    extern f32 float_1024_80424c40;
    s32* work = *(s32**)((s32)entry + 0xC);
    f32 pos[3];
    s32 type;
    s32 i;
    u8* particle;

    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    type = work[0];

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        work[4] = 0x10;
    }
    if (work[4] < 1000) {
        work[4]--;
    }
    work[5]++;

    if (work[4] < 0) {
        effDelete(entry);
        return;
    }

    if (work[4] < 0x10) {
        work[9] = work[4] << 4;
    }
    if (work[5] < 0x10) {
        work[9] = (work[5] << 4) + 0xF;
    }

    particle = (u8*)work + 0x44;
    for (i = 1; i < *(s32*)((s32)entry + 8); i += 3, particle += 0xCC) {
        s32 life = *(s32*)(particle + 0x10);
        s32 age = *(s32*)(particle + 0x14);
        f32 oldX;
        f32 oldY;

        if (life <= age) {
            *(s32*)(particle + 0x10) = (rand() % 0x32) + 0x14;
            *(s32*)(particle + 0x14) = 0;
            *(f32*)(particle + 0x3C) = (f32)(rand() % 0x168);
            *(f32*)(particle + 4) = float_0_80424c24;
            *(f32*)(particle + 8) = float_0_80424c24;
        }

        if ((rand() % 100) < 0x33) {
            s32 j;
            s32* src;
            s32* dst;
            f32 dx;
            f32 dy;
            f32 dist2;

            oldX = *(f32*)(particle + 4);
            oldY = *(f32*)(particle + 8);
            dx = (f32)((rand() % 0x11) - 8);
            dy = (f32)((rand() % 0x11) - 8);

            src = (s32*)(particle + 0x40);
            dst = (s32*)(particle + 0x84);
            for (j = 0; j < 0x11; j++) {
                dst[j] = src[j];
            }

            src = (s32*)(particle - 4);
            dst = (s32*)(particle + 0x40);
            for (j = 0; j < 0x11; j++) {
                dst[j] = src[j];
            }

            *(f32*)(particle + 4) = oldX + dx;
            *(f32*)(particle + 8) = oldY + dy;
            *(f32*)(particle + 0x3C) = angleABf(float_0_80424c24, float_0_80424c24, dx, dy);
            if ((dx == float_0_80424c24) && (dy == float_0_80424c24)) {
                *(f32*)(particle + 0x34) = float_0_80424c24;
            } else {
                dist2 = (dx * dx) + (dy * dy);
                *(f32*)(particle + 0x34) = dist2;
            }
        }

        *(s32*)(particle + 0x24) = (s32)(float_1024_80424c40 * (f32)sin((f64)((float_6p2832_80424c38 * (f32)((*(s32*)(particle + 0x14) * 0xB4) / *(s32*)(particle + 0x10))) / float_360_80424c3c)));
        if (*(s32*)(particle + 0x24) > 0xFF) {
            *(s32*)(particle + 0x24) = 0xFF;
        }
        (*(s32*)(particle + 0x14))++;
    }

    particle = (u8*)work + 0x44;
    for (i = 1; i < *(s32*)((s32)entry + 8); i++, particle += 0x44) {
        *(s32*)(particle + 0x40) = rand() & 0xF;
    }

    if (type == 0) {
        dispEntry(4, 2, effAkariChargeDisp, entry, dispCalcZ(pos));
    } else {
        dispEntry(8, 2, effAkariChargeDisp, entry, dispCalcZ(pos));
    }
}

void effAkariChargeDisp(s32 cameraId, void* entry) {
    extern void* camGetPtr(s32 id);
    extern void GXGetProjectionv(void* dst);
    extern void GXGetViewportv(void* dst);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetProjectionv(void* src);
    extern void GXSetViewport(f32 x, f32 y, f32 w, f32 h, f32 nearz, f32 farz);
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* m, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 map);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(s32 desc);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_deg2rad_80424c20;
    extern f32 float_0_80424c24;
    extern f32 float_0p0625_80424c28;
    extern f32 float_1_80424c2c;
    extern f32 float_0p0039062_80424c30;
    extern f32 float_0p03125_80424c34;

    void* cam;
    void* cam3d;
    s32* work;
    s32 type;
    s32 age;
    s32 alpha;
    s32 r;
    s32 g;
    s32 b;
    s32 i;
    u32 color;
    f32 proj[7];
    f32 viewport[6];
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 tmpMtx[3][4];

    cam = camGetPtr(4);
    work = *(s32**)((s32)entry + 0xC);
    type = work[0];
    age = work[5];
    alpha = work[9];
    r = work[6];
    g = work[7];
    b = work[8];

    if (type == 1) {
        GXGetProjectionv(proj);
        GXGetViewportv(viewport);
        GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
        GXSetZMode(0, 7, 0);
    }

    PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424c20 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x38), *(f32*)((s32)work + 0x38), *(f32*)((s32)work + 0x38));
    PSMTXConcat(transMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = ((u8)work[10] << 24) | ((u8)work[0xB] << 16) | ((u8)work[0xC] << 8) | 0xFF;
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 0xF);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    effGetTexObjN64(0x70, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetCullMode(0);
    effSetVtxDescN64(0x8039D5A8);

    work += 0x11;
    for (i = 1; i < *(s32*)((s32)entry + 8); i++, work += 0x11) {
        s32 particleAlpha = (work[0x9] * alpha) / 0xFF;
        color = ((u8)r << 24) | ((u8)g << 16) | ((u8)b << 8) | (u8)particleAlpha;
        GXSetTevColor(1, &color);
        PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), float_0_80424c24);
        PSMTXRotRad(rotMtx, float_deg2rad_80424c20 * *(f32*)((s32)work + 0x3C), 'z');
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x34) * float_0p0625_80424c28, *(f32*)((s32)work + 0x34) * float_0p0625_80424c28, float_1_80424c2c);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        PSMTXConcat(baseMtx, transMtx, tmpMtx);
        GXLoadPosMtxImm(tmpMtx, 0);
        GXSetCurrentMtx(0);
        PSMTXScale(scaleMtx, float_0p0039062_80424c30, float_0p03125_80424c34, float_0_80424c24);
        PSMTXTrans(transMtx, (f32)(work[0x10] << 4), float_0_80424c24, float_0_80424c24);
        PSMTXConcat(scaleMtx, transMtx, scaleMtx);
        GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
        PSMTXScale(scaleMtx, float_0p0039062_80424c30, float_0p03125_80424c34, float_0_80424c24);
        PSMTXTrans(transMtx, (f32)(age * 3), float_0_80424c24, float_0_80424c24);
        PSMTXConcat(scaleMtx, transMtx, scaleMtx);
        GXLoadTexMtxImm(scaleMtx, 0x21, 1);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }

    if (type == 1) {
        GXSetProjectionv(proj);
        GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    }
}

