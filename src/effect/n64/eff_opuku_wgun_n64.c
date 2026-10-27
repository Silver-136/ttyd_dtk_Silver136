#include "system.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os/OSFastCast.h>
#include <stdlib.h>
#include "eff/eff_n64.h"
#include "eff/n64/eff_opuku_wgun_n64.h"
#include "dolphin/os.h"

double sin(double);
double cos(double);

static inline f32 dsin(f32 a) { return sin(6.2831855f * a / 360.0f); }

typedef struct EffOpukuWgunN64Work {
    s32 type;        // 0x0
    Vec position;    // 0x4
    Vec start;       // 0x10
    Vec end;         // 0x1C
    s32 total;       // 0x28
    s32 lifetime;    // 0x2C
    s32 frame;       // 0x30
    s32 colorR;      // 0x34
    s32 colorG;      // 0x38
    s32 colorB;      // 0x3C
    s32 alpha;       // 0x40
    s32 color2R;     // 0x44
    s32 color2G;     // 0x48
    s32 color2B;     // 0x4C
    f32 scale;       // 0x50
    s32 count;       // 0x54
    f32 a[12];       // 0x58
    f32 b[12];       // 0x88
    f32 c[12];       // 0xB8
    f32 x[12];       // 0xE8
    f32 y[12];       // 0x118
    f32 z[12];       // 0x148
    s32 delay[12];   // 0x178
    s32 counter[12]; // 0x1A8
    u8 alphas[12];   // 0x1D8
} EffOpukuWgunN64Work;

void effOpukuWgunMain(EffectEntry* effect);
void effOpukuWgunDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effOpukuWgunN64Entry(s32 type, s32 lifetime, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2,
                                  f32 scale) {
    EffectEntry* effect;
    EffOpukuWgunN64Work* work;
    s32 i;
    s32 k;

    effect = effEntry();
    effect->type = "OpukuWgunN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOpukuWgunN64Work));
    effect->userdata = work;
    effect->callback = effOpukuWgunMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else if (type == 0) {
        work->lifetime = lifetime * 2;
    } else {
        work->lifetime = lifetime * 4;
    }
    work->total = lifetime;
    if (type == 0) {
        work->alpha = 200;
    } else {
        work->alpha = 255;
    }
    work->start.x = x1;
    work->start.y = y1;
    work->start.z = z1;
    work->end.x = x2;
    work->end.y = y2;
    work->end.z = z2;
    work->scale = scale;
    work->colorR = 210;
    work->colorG = 230;
    work->colorB = 255;
    work->color2R = 0;
    work->color2G = 30;
    work->color2B = 205;
    work->count = 0;
    work->position.x = work->start.x;
    work->position.y = work->start.y;
    work->position.z = work->start.z;
    for (i = 0; i < 12; i++) {
        work->a[i] = 10.0f;
        work->b[i] = 2.0f;
        work->c[i] = 0.0f;
        if (type == 0) {
            work->delay[i] = i + 1;
        } else {
            work->delay[i] = 1 + i * 5;
        }
        work->x[i] = work->position.x;
        work->y[i] = work->position.y;
        work->z[i] = work->position.z;
        work->counter[i] = 0;
    }
    return effect;
}

void effOpukuWgunMain(EffectEntry* effect) {
    EffOpukuWgunN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    s32 total;
    s32 c;
    f32 t;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    total = work->total;
    work->count++;
    if (work->count > 12) {
        work->count = 12;
    }
    for (i = 0; i < 12; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
            if (work->delay[i] != 0) {
                continue;
            }
        }
        work->counter[i]++;
        c = work->counter[i];
        if (c >= total) {
            c = total;
        }
        t = (f32)c / (f32)total;
        if (type == 0) {
            work->x[i] = work->start.x + t * (work->end.x - work->start.x);
            work->y[i] = work->start.y + t * (work->end.y - work->start.y);
            work->z[i] = work->start.z + t * (work->end.z - work->start.z);
        } else {
            work->x[i] = work->start.x + t * (work->end.x - work->start.x);
            work->y[i] = 120.0f * dsin(180.0f * t) + (work->start.y + t * (work->end.y - work->start.y));
            work->z[i] = work->start.z + t * (work->end.z - work->start.z);
            work->alphas[i] = (u8)(255.0f - 255.0f * t);
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOpukuWgunDisp, effect);
}

static u8 a_data[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 200, 128, 32};

void effOpukuWgunDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffOpukuWgunN64Work* work;
    Mtx scale;
    GXTexObj texObj;
    Vec v;
    f32 scl;
    f32 zero = 0.0f;
    f32 rad;
    f32 dx;
    f32 dy;
    f32 ang;
    f32 s;
    s32 i;
    s32 type;
    effN64Vtx* vtx;
    effN64Vtx* p;
    effN64Vtx* q;
    s32 n;
    s32 j;
    s32 a;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    scl = work->scale;

    GXSetTevColor(GX_TEVREG0, (GXColor){work->colorR, work->colorG, work->colorB, work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color2R, work->color2G, work->color2B, 200});
    GXSetCullMode(GX_CULL_NONE);
    p = (effN64Vtx*)smartAlloc(sizeof(effN64Vtx) * 48, 3)->address;
    vtx = p;
    for (i = 0; i < 12; i++) {
        // FAKE: the duplicated i == 0 test; the single test changes the object
        if (i == 0 || i == 0) {
            ang = -angleABf(work->y[i + 1], -work->x[i + 1], work->y[i], -work->x[i]);
        } else if (i != 11 && work->delay[i] == 0) {
            ang = -angleABf(work->y[i + 1], -work->x[i + 1], work->y[i], -work->x[i]);
        }
        if (work->type == 0) {
            s = 2.0f + 0.5f * (f32)work->counter[i];
            if (s > 6.0f) {
                s = 6.0f;
            }
            a = a_data[i];
        } else {
            s = 1.0f + 0.5f * (f32)work->counter[i];
            if (s > 30.0f) {
                s = 30.0f;
            }
            a = a_data[i] * work->alphas[i] / 255;
        }
        s = s * scl;
        rad = 6.2831855f * ang / 360.0f;
        dx = s * (f32)sin(rad);
        dy = s * (f32)cos(rad);
        p->x = 20.0f * (work->x[i] + dx);
        p->y = 20.0f * (work->y[i] + dy);
        p->z = 20.0f * (work->z[i] + zero);
        // FAKE: (u32)(i * 64) * 5 == i * 320 (the old k IV) for i in 0..11; the unsigned multiply is strength-reduced by the back-end loop pass, so no li k = 0 competes with the work IV copy in the block after smartAlloc (target order li r28 / mr r30)
        p->s = (u32)(i * 64) * 5;
        p->t = 0;
        p->r = (i & 1) * 255;
        p->g = ((i >> 1) & 1) * 255;
        p->b = ((i >> 2) & 1) * 255;
        p->a = a;
        p++;
        p->x = 20.0f * (work->x[i] - dx);
        p->y = 20.0f * (work->y[i] - dy);
        p->z = 20.0f * (work->z[i] + zero);
        p->s = (u32)(i * 64) * 5;
        p->t = 1024;
        p->r = (i & 1) * 255;
        p->g = ((i >> 1) & 1) * 255;
        p->b = ((i >> 2) & 1) * 255;
        p->a = a;
        p++;
    }
    PSMTXScale(scale, 0.05f, 0.05f, 0.05f);
    q = vtx;
    n = i * 2;
    for (j = 0; j < n; j++) {
        OSInitFastCast();
        OSs16tof32(&q->x, &v.x);
        OSs16tof32(&q->y, &v.y);
        OSs16tof32(&q->z, &v.z);
        PSMTXMultVec(scale, &v, &v);
        OSInitFastCast();
        OSf32tos16(&v.x, &q->x);
        OSf32tos16(&v.y, &q->y);
        OSf32tos16(&v.z, &q->z);
        q++;
    }
    DCFlushRange(vtx, sizeof(effN64Vtx) * 48);
    GXInvalidateVtxCache();
    effSetVtxDescN64(vtx);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    if (type == 0) {
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scale, 0.015625f, 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        effGetTexObjN64(0x8D, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    } else {
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_TEXC, GX_CC_RASA, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C1, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        effGetTexObjN64(0x8E, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
    }
    a = i - 1;
    j = 0;
    n = j;
    // FAKE: j >= 0 always holds here; the loop without the test changes the object
    for (; j < a; j++) {
        if (j >= 0) {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(n, n + 2, n + 1, n, n + 1, n + 2, n + 3, n);
        }
        n += 2;
    }
}


