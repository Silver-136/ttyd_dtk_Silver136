#include "eff/n64/eff_kumokumo_chip_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"
#include <math.h>

typedef struct EffKumokumoChipN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    Vec velocity;  // 0x10
    s32 lifetime;  // 0x1C
    s32 frame;     // 0x20
    s32 r1;        // 0x24
    s32 g1;        // 0x28
    s32 b1;        // 0x2C
    s32 a1;        // 0x30
    s32 r2;        // 0x34
    s32 g2;        // 0x38
    s32 b2;        // 0x3C
    s32 a2;        // 0x40
    f32 scale;     // 0x44
} EffKumokumoChipN64Work;

static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, 0, 0, 0, 0, 0, 0, 0, 255},
    {16, 0, 0, 1024, 0, 0, 0, 0, 255},
    {16, 32, 0, 1024, 1024, 0, 0, 0, 255},
    {-16, 32, 0, 0, 1024, 0, 0, 0, 255},
};

void effKumokumoChipMain(EffectEntry* effect);
void effKumokumoChipDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKumokumoChipN64Entry(s32 type, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz, f32 speed, f32 scale) {
    EffectEntry* effect;
    EffKumokumoChipN64Work* work;
    f32 dx, dy, dz, dist;
    s32 lifetime;
    f32 inv;

    effect = effEntry();
    effect->type = "KumokumoChipN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKumokumoChipN64Work));
    effect->userdata = work;
    effect->callback = effKumokumoChipMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    dx = tx - x;
    dy = ty - y;
    dz = tz - z;
    dist = dx * dx + dy * dy + dz * dz;
    {
        f32 zero = 0.0f;
        if (dist != zero) {
            f32 r;
            if (dist > zero) {
                f64 guess = __frsqrte(dist);
                guess = 0.5 * guess * (3.0 - guess * guess * dist);
                guess = 0.5 * guess * (3.0 - guess * guess * dist);
                guess = 0.5 * guess * (3.0 - guess * guess * dist);
                r = dist * guess;
            } else if (dist < 0.0) {
                r = NAN;
            } else if (isnan(dist)) {
                r = NAN;
            } else {
                r = dist;
            }
            dist = r;
        }
    }
    if (speed > 0.0f) {
        lifetime = dist / speed;
    } else {
        lifetime = 0;
    }
    if (lifetime != 0) {
        inv = 1.0f / lifetime;
        work->velocity.x = dx * inv;
        work->velocity.y = dy * inv;
        work->velocity.z = dz * inv;
    } else {
        work->velocity.x = 0.0f;
        work->velocity.y = 0.0f;
        work->velocity.z = 0.0f;
    }
    if (lifetime < 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->a1 = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    switch (type) {
        case 0:
        case 2:
            work->r1 = 215;
            work->g1 = 215;
            work->b1 = 185;
            work->r2 = 215;
            work->g2 = 200;
            work->b2 = 100;
            work->a2 = 255;
            break;
        default:
            work->r1 = 255;
            work->g1 = 255;
            work->b1 = 255;
            work->r2 = 220;
            work->g2 = 245;
            work->b2 = 255;
            work->a2 = 255;
            break;
    }
    return effect;
}

void effKumokumoChipMain(EffectEntry* effect) {
    EffKumokumoChipN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 fr;
    s32 lifetime;
    s32 type;

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
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    fr = work->frame;
    if (type == 0 || type == 2) {
        if (fr < 16) {
            work->a1 = fr * 16 + 15;
        } else {
            work->a1 = 255;
        }
        if (lifetime < 16) {
            work->a1 = lifetime * 16;
        }
    } else {
        if (work->frame < 4) {
            work->a1 = fr * 64 + 63;
        } else {
            work->a1 = 255;
        }
        if (lifetime < 4) {
            work->a1 = lifetime * 64;
        }
    }
    work->position.x += work->velocity.x;
    work->position.y += work->velocity.y;
    work->position.z += work->velocity.z;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKumokumoChipDisp, effect);
}

void effKumokumoChipDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffKumokumoChipN64Work* work;
    s32 alpha;
    Mtx mtx;
    Mtx scale;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->a1;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r1, work->g1, work->b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, work->a2});
    effGetTexObjN64(0x6B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x32_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
