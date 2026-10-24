#include "eff/n64/eff_kemuri10_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"
#include <math.h>

static effN64Vtx stg_houki_kemuri_v[] = {
    {0, -50, 20, 0, 512, 255, 255, 255, 255},
    {35, -35, 20, 256, 512, 255, 255, 255, 255},
    {21, -21, 0, 256, 256, 255, 255, 255, 255},
    {0, -30, 0, 0, 256, 255, 255, 255, 255},
    {35, -35, -20, 256, 0, 255, 255, 255, 255},
    {0, -50, -20, 0, 0, 255, 255, 255, 255},
    {30, 0, 0, 512, 256, 255, 255, 255, 255},
    {50, 0, -20, 512, 0, 255, 255, 255, 255},
    {50, 0, 20, 512, 512, 255, 255, 255, 255},
    {70, 0, 0, 512, 256, 255, 255, 255, 255},
    {49, -49, 0, 256, 256, 255, 255, 255, 255},
    {0, -70, 0, 0, 256, 255, 255, 255, 255},
    {21, 21, 0, 768, 256, 255, 255, 255, 255},
    {35, 35, -20, 768, 0, 255, 255, 255, 255},
    {35, 35, 20, 768, 512, 255, 255, 255, 255},
    {49, 49, 0, 768, 256, 255, 255, 255, 255},
    {-35, -35, 20, 1792, 512, 255, 255, 255, 255},
    {0, -50, 20, 2048, 512, 255, 255, 255, 255},
    {0, -30, 0, 2048, 256, 255, 255, 255, 255},
    {-21, -21, 0, 1792, 256, 255, 255, 255, 255},
    {-49, -49, 0, 1792, 256, 255, 255, 255, 255},
    {0, -70, 0, 2048, 256, 255, 255, 255, 255},
    {-70, 0, 0, 1536, 256, 255, 255, 255, 255},
    {-50, 0, 20, 1536, 512, 255, 255, 255, 255},
    {-30, 0, 0, 1536, 256, 255, 255, 255, 255},
    {0, -50, -20, 2048, 0, 255, 255, 255, 255},
    {-35, -35, -20, 1792, 0, 255, 255, 255, 255},
    {-50, 0, -20, 1536, 0, 255, 255, 255, 255},
    {-35, 35, 20, 1280, 512, 255, 255, 255, 255},
    {-21, 21, 0, 1280, 256, 255, 255, 255, 255},
    {-35, 35, -20, 1280, 0, 255, 255, 255, 255},
    {-49, 49, 0, 1280, 256, 255, 255, 255, 255},
    {0, 30, 0, 1024, 256, 255, 255, 255, 255},
    {0, 50, -20, 1024, 0, 255, 255, 255, 255},
    {0, 70, 0, 1024, 256, 255, 255, 255, 255},
    {0, 50, 20, 1024, 512, 255, 255, 255, 255},
};

void effKemuri10Main(EffectEntry* effect);
void effKemuri10Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri10N64Entry(s32 type, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, f32 scale) {
    EffectEntry* effect;
    EffKemuri10N64Work* work;
    f32 inv;
    f32 zr;
    f32 az, ay, ax;
    f32 bx, by, bz;

    effect = effEntry();
    zr = 0.0f;
    if (dx == zr && dy == zr && dz == zr) {
        return NULL;
    }
    inv = dx * dx + dy * dy + dz * dz;
    if (inv == zr) {
        return NULL;
    }
    inv = 1.0f / sqrtf(inv);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    if (dx != zr || dz != zr) {
        az = dz;
        ay = zr;
        ax = -dx;
    } else {
        az = 1.0f;
        ax = zr;
        ay = zr;
    }
    inv = az * az + ay * ay + ax * ax;
    if (inv == zr) {
        return NULL;
    }
    inv = 1.0f / sqrtf(inv);
    az *= inv;
    ax *= inv;
    ay *= inv;
    bx = ay * dz - ax * dy;
    by = ax * dx - az * dz;
    bz = az * dy - ay * dx;
    inv = bx * bx + by * by + bz * bz;
    if (inv == zr) {
        return NULL;
    }
    inv = 1.0f / sqrtf(inv);
    effect->type = "Kemuri10N64";
    effect->count = 1;
    bx *= inv;
    by *= inv;
    bz *= inv;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri10N64Work));
    effect->userdata = work;
    effect->callback = effKemuri10Main;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->velocity.x = 4.0f * -dx;
    work->velocity.y = 4.0f * -dy;
    work->velocity.z = 4.0f * -dz;
    work->timer = 16;
    work->texOffset = 278.0f;
    work->texSpeed = 32.0f;
    work->alpha = 0;
    work->counter = 0;
    work->r = 0xF8;
    work->g = 0x5C;
    work->b = 0xE5;
    work->r2 = 0xF8;
    work->g2 = 0x5C;
    work->b2 = 0xE5;
    work->rot[0][0] = az;
    work->rot[1][0] = ay;
    work->rot[2][0] = ax;
    work->rot[0][1] = bx;
    work->rot[1][1] = by;
    work->rot[2][1] = bz;
    work->rot[0][2] = dx;
    work->rot[1][2] = dy;
    work->rot[2][2] = dz;
    work->rot[0][3] = zr;
    work->rot[1][3] = zr;
    work->rot[2][3] = zr;
    work->scale = 0.1f * scale;
    work->camera = 4;
    return effect;
}

void effKemuri10Main(EffectEntry* effect) {
    EffKemuri10N64Work* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 camera;
    s32 timer;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    timer = work->timer;
    camera = work->camera;
    work->timer = timer - 1;
    work->counter++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    for (i = 0; i < effect->count; i++) {
        if (timer > 5) {
            work->alpha = (s32)((f32)work->alpha + 0.3f * (f32)(255 - work->alpha));
            work->velocity.x *= 0.8f;
            work->velocity.y *= 0.8f;
            work->velocity.z *= 0.8f;
        }
        if (timer < 5) {
            { f32 fa = work->alpha; work->alpha = (fa *= 0.6f); }
            work->velocity.x *= 0.7f;
            work->velocity.y *= 0.7f;
            work->velocity.z *= 0.7f;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->texOffset += work->texSpeed;
        work->scale *= 1.05f;
        work++;
    }
    dispEntry((CameraId)camera, dispCalcZ(&position), 2, (DispCallback)effKemuri10Disp, effect);
}

void effKemuri10Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri10N64Work* work = effect->userdata;
    s32 idx;
    Mtx mtx;
    Mtx scaleMtx;
    GXTexObj texObj;

    idx = (s32)work->texOffset;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.015625f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, (f32)idx, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x1F, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, work->alpha});
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXConcat(mtx, work->rot, mtx);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effSetVtxDescN64(stg_houki_kemuri_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 192);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(3, 2, 4, 0, 3, 4, 5, 0);
    tri2(2, 6, 7, 0, 2, 7, 4, 0);
    tri2(1, 8, 6, 0, 1, 6, 2, 0);
    tri2(4, 7, 9, 0, 4, 9, 10, 0);
    tri2(5, 4, 10, 0, 5, 10, 11, 0);
    tri2(10, 9, 8, 0, 10, 8, 1, 0);
    tri2(11, 10, 1, 0, 11, 1, 0, 0);
    tri2(6, 12, 13, 0, 6, 13, 7, 0);
    tri2(8, 14, 12, 0, 8, 12, 6, 0);
    tri2(7, 13, 15, 0, 7, 15, 9, 0);
    tri2(9, 15, 14, 0, 9, 14, 8, 0);
    tri2(16, 17, 18, 0, 16, 18, 19, 0);
    tri2(20, 21, 17, 0, 20, 17, 16, 0);
    tri2(22, 20, 16, 0, 22, 16, 23, 0);
    tri2(23, 16, 19, 0, 23, 19, 24, 0);
    tri2(19, 18, 25, 0, 19, 25, 26, 0);
    tri2(24, 19, 26, 0, 24, 26, 27, 0);
    tri2(28, 23, 24, 0, 28, 24, 29, 0);
    tri2(29, 24, 27, 0, 29, 27, 30, 0);
    tri2(26, 25, 21, 0, 26, 21, 20, 0);
    tri2(27, 26, 20, 0, 27, 20, 22, 0);
    tri2(30, 27, 22, 0, 30, 22, 31, 0);
    tri2(31, 22, 23, 0, 31, 23, 28, 0);
    tri2(32, 29, 30, 0, 32, 30, 33, 0);
    tri2(12, 32, 33, 0, 12, 33, 13, 0);
    tri2(13, 33, 34, 0, 13, 34, 15, 0);
    tri2(33, 30, 31, 0, 33, 31, 34, 0);
    tri2(34, 31, 28, 0, 34, 28, 35, 0);
    tri2(15, 34, 35, 0, 15, 35, 14, 0);
    tri2(35, 28, 29, 0, 35, 29, 32, 0);
    tri2(14, 35, 32, 0, 14, 32, 12, 0);
}
