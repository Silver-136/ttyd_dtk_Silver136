#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/n64/eff_water_damage_n64.h"
#include "eff/eff_n64.h"


typedef struct PointData {
    s8 x, y, z;
    s16 s, t;
    u8 r, g, b;
} PointData;

double sin(double);
double cos(double);

typedef struct EffOpukuWprotectN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 colorR;   // 0x18
    s32 colorG;   // 0x1C
    s32 colorB;   // 0x20
    s32 alpha;    // 0x24
    s32 alpha2;   // 0x28
    f32 scale;    // 0x2C
    f32 unk30;    // 0x30
    f32 unk34;    // 0x34
    f32 a[4];     // 0x38
    f32 b[4];     // 0x48
    f32 c[4];     // 0x58
    f32 d[4];     // 0x68
    f32 e[4];     // 0x78
    s32 g[4];     // 0x88
} EffOpukuWprotectN64Work;

void effOpukuWprotectMain(EffectEntry* effect);
void effOpukuWprotectDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effOpukuWprotectN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffOpukuWprotectN64Work* work;

    effect = effEntry();
    effect->type = "OpukuWprotectN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOpukuWprotectN64Work));
    effect->userdata = work;
    effect->callback = effOpukuWprotectMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->colorR = 47;
    work->colorG = 127;
    work->colorB = 255;
    work->unk30 = 0.0f;
    work->unk34 = 0.0f;
    work->g[0] = -1;
    work->g[1] = -5;
    work->g[2] = -9;
    work->g[3] = -13;
    return effect;
}

void effOpukuWprotectMain(EffectEntry* effect) {
    EffOpukuWprotectN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    f32 t;
    f32 s1;
    f32 s6;
    f32 c2;
    f32 c1;
    f32 a1;
    f32 u;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & 4) {
        if (type == 1) {
            effect->flags &= ~4;
            work->lifetime = 4;
        } else {
            effect->flags &= ~4;
            work->lifetime = 16;
        }
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->frame > 0x50000 - 0xE60) {
        work->frame = 256;
    }
    if (work->lifetime < 0) {
        if (type == 1) {
            effWaterDamageN64Entry(0, 30, work->position.x, 24.0f + work->position.y, work->position.z, 2.0f);
            effWaterDamageN64Entry(1, 30, work->position.x, 24.0f + work->position.y, work->position.z, 2.0f);
        }
        effDelete(effect);
        return;
    }
    i = work->frame;
    if (type == 0) {
        if (work->lifetime < 16) {
            work->alpha2 = work->alpha = work->lifetime << 4;
        }
        if (i < 16) {
            work->alpha2 = work->alpha = (i << 4) + 15;
        }
    } else {
        if (i < 4) {
            work->alpha2 = work->alpha = (i << 6) + 63;
        }
    }
    u = (f32)sin(6.2831855f * (f32)(i + i * 6) / 360.0f);
    work->colorG = (u8)(s32)(127.0f + 32.0f * u);
    t = 0.01f * (f32)i;
    a1 = 6.2831855f * t / 360.0f;
    s1 = (f32)sin(a1);
    s6 = (f32)sin(6.2831855f * (6.0f * t) / 360.0f);
    c2 = (f32)cos(6.2831855f * (2.0f * t) / 360.0f);
    c1 = (f32)cos(a1);
    work->unk30 = 32.0f * s6 * s1 + 256.0f + 32.0f * c2 + 32.0f * c1;
    work->unk34 += 0.2f;
    if (work->unk34 >= 128.0f) {
        work->unk34 = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        work->g[i]++;
        if (work->g[i] >= 0) {
            if (work->g[i] == 0) {
                work->a[i] = 0.0f;
                work->b[i] = 240.0f;
                work->c[i] = 0.2f * (f32)(rand() % 11 - 5);
                work->d[i] = (f32)(rand() % 11 + 2);
                work->e[i] = (f32)(rand() % 11 + 1);
            }
            work->a[i] += work->c[i];
            work->b[i] += work->d[i];
            work->c[i] *= 0.97f;
            work->d[i] += 0.1f * (10.0f - work->d[i]);
            if (work->b[i] > 480.0f) {
                work->g[i] = -(rand() % 11) - 1;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOpukuWprotectDisp, effect);
}

PointData point_data[40] = {
    {25, 4, -25, 1025, 72, 176, 176, 176},
    {22, 0, -22, 963, 0, 176, 176, 176},
    {-22, 0, -22, 61, 0, 176, 176, 176},
    {-25, 4, -25, 1, 72, 176, 176, 176},
    {25, 46, -25, 1025, 951, 176, 176, 176},
    {-25, 46, -25, 1, 951, 176, 176, 176},
    {22, 50, -22, 890, 997, 174, 208, 216},
    {-22, 50, -22, 133, 997, 174, 208, 216},
    {-22, 50, 22, 133, 997, 174, 208, 216},
    {22, 50, 22, 890, 997, 174, 208, 216},
    {-22, 0, -22, 133, 25, 148, 165, 176},
    {22, 0, -22, 890, 25, 148, 165, 176},
    {22, 0, 22, 890, 25, 148, 165, 176},
    {-22, 0, 22, 133, 25, 148, 165, 176},
    {-22, 50, 22, 963, 1021, 120, 110, 136},
    {-22, 50, -22, 61, 1021, 120, 110, 136},
    {-25, 46, -25, 0, 951, 120, 110, 136},
    {-25, 46, 25, 1024, 951, 120, 110, 136},
    {-25, 4, -25, 0, 72, 120, 110, 136},
    {-25, 4, 25, 1024, 72, 120, 110, 136},
    {-22, 0, -22, 61, 0, 120, 110, 136},
    {-22, 0, 22, 963, 0, 120, 110, 136},
    {-22, 50, -22, 61, 1021, 176, 176, 176},
    {22, 50, -22, 963, 1021, 176, 176, 176},
    {22, 0, 22, 963, 0, 120, 110, 136},
    {22, 0, -22, 61, 0, 120, 110, 136},
    {25, 4, -25, 0, 72, 120, 110, 136},
    {25, 4, 25, 1024, 72, 120, 110, 136},
    {25, 46, -25, 0, 951, 120, 110, 136},
    {25, 46, 25, 1024, 951, 120, 110, 136},
    {22, 50, -22, 61, 1021, 120, 110, 136},
    {-22, 0, 22, 61, 0, 240, 240, 240},
    {25, 4, 25, 1025, 72, 240, 240, 240},
    {25, 46, 25, 1025, 951, 240, 240, 240},
    {22, 50, 22, 963, 1021, 120, 110, 136},
    {22, 0, 22, 963, 0, 240, 240, 240},
    {-25, 4, 25, 1, 72, 240, 240, 240},
    {-25, 46, 25, 1, 951, 240, 240, 240},
    {22, 50, 22, 963, 1021, 240, 240, 240},
    {-22, 50, 22, 61, 1021, 240, 240, 240},
};

static effN64Vtx size8x8_tex32x32_vtx[4] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 1024, 0, 0, 0, 0, 255},
    {4, 4, 0, 1024, 1024, 0, 0, 0, 255},
    {-4, 4, 0, 0, 1024, 0, 0, 0, 255},
};

void effOpukuWprotectDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffOpukuWprotectN64Work* work;
    s32 i;
    s32 alpha;
    effN64Vtx* vtx;
    effN64Vtx* p;
    PointData* pd;
    u32 r;
    f32 fx, x, y, z, fy, fz;
    Mtx mtx2;
    Mtx scale;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXConcat(camera->view, mtx, mtx);
    vtx = p = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 80, 3)->address;
    pd = point_data;
    r = gp->retraceCount >> 1;
    for (i = 0; i < 40; i++) {
        x = pd->x * 10;
        y = pd->y * 10;
        z = pd->z * 10;
        if (y != 0.0f) {
            fx = (f32)sin((f32)(s32)(r + effTblRandN64(1000, (s32)(10.0f * y + x + 100.0f * z))));
            fy = (f32)sin((f32)(s32)(r + effTblRandN64(1000, (s32)(10.0f * x + 100.0f * y + z))));
            fz = (f32)sin((f32)(s32)(r + effTblRandN64(1000, (s32)(100.0f * x + y + 10.0f * z))));
            x = x + (f32)(20.0f * fx);
            y = y + (f32)(20.0f * fy);
            z = z + (f32)(20.0f * fz);
        }
        p->x = x;
        p->y = y;
        p->z = z;
        p->s = pd->s;
        p->t = pd->t;
        p->r = pd->r;
        p->g = pd->g;
        p->b = pd->b;
        p->a = 80;
        pd++;
        p++;
    }
    GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 255, work->alpha2});
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x91, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < 4; i++) {
        if (work->g[i] >= 0) {
            PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
            PSMTXTrans(mtx2, work->a[i], work->b[i], 0.0f);
            PSMTXConcat(scale, mtx2, mtx2);
            PSMTXScale(scale, work->e[i], work->e[i], work->e[i]);
            PSMTXConcat(mtx2, scale, mtx2);
            PSMTXConcat(mtx, mtx2, mtx2);
            GXLoadPosMtxImm(mtx2, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            effSetVtxDescN64(size8x8_tex32x32_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    effGetTexObjN64(0x90, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->colorR, work->colorG, work->colorB, (s32)(0.3f * (f32)alpha)});
    GXSetCullMode(GX_CULL_FRONT);
    DCFlushRange(vtx, 0x460);
    GXInvalidateVtxCache();
    PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(mtx, scale, scale);
    GXLoadPosMtxImm(scale, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effSetVtxDescN64(vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 60);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 0, 3, 0, 4, 3, 5, 0);
    tri2(6, 7, 8, 0, 6, 8, 9, 0);
    tri2(10, 11, 12, 0, 10, 12, 13, 0);
    tri2(14, 15, 16, 0, 14, 16, 17, 0);
    tri2(16, 18, 19, 0, 16, 19, 17, 0);
    tri2(18, 20, 21, 0, 18, 21, 19, 0);
    tri2(22, 23, 4, 0, 22, 4, 5, 0);
    tri2(24, 25, 26, 0, 24, 26, 27, 0);
    tri2(26, 28, 29, 0, 26, 29, 27, 0);
    effSetVtxDescN64(vtx + 28);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
    tri2(0, 2, 6, 0, 0, 6, 1, 0);
    tri2(3, 7, 4, 0, 3, 4, 8, 0);
    tri2(4, 5, 9, 0, 4, 9, 8, 0);
    tri2(5, 10, 11, 0, 5, 11, 9, 0);
}
