#include "eff/n64/eff_pikkyolo_n64.h"
#include "driver/effdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_n64.h"

typedef struct EffPikkyoloN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    s32 life;     // 0x1C
    s32 counter;  // 0x20
    f32 scale;    // 0x24
    f32 rot;      // 0x28
    s16 r0;       // 0x2C
    s16 g0;       // 0x2E
    s16 b0;       // 0x30
    s16 a0;       // 0x32
    s16 r1;       // 0x34
    s16 g1;       // 0x36
    s16 b1;       // 0x38
    s16 a1;       // 0x3A
} EffPikkyoloN64Work;

static effN64Vtx size16x16_tex16x16_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 512, 0, 0, 0, 0, 255},
    {8, 8, 0, 512, 512, 0, 0, 0, 255},
    {-8, 8, 0, 0, 512, 0, 0, 0, 255},
    {-8, -8, 0, 0, 512, 0, 0, 0, 255},
    {8, -8, 0, 512, 512, 0, 0, 0, 255},
    {8, 8, 0, 512, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
    {-8, -8, 0, 0, 1024, 0, 0, 0, 255},
    {8, -8, 0, 512, 1024, 0, 0, 0, 255},
    {8, 8, 0, 512, 1536, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1536, 0, 0, 0, 255},
    {-8, -8, 0, 0, 1536, 0, 0, 0, 255},
    {8, -8, 0, 512, 1536, 0, 0, 0, 255},
    {8, 8, 0, 512, 2048, 0, 0, 0, 255},
    {-8, 8, 0, 0, 2048, 0, 0, 0, 255},
};

void effPikkyoloMain(EffectEntry* effect);
void effPikkyoloDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPikkyoloN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffPikkyoloN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "PikkyoloN64";
    effect->count = 6;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPikkyoloN64Work) * 6);
    effect->userdata = work;
    effect->callback = effPikkyoloMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->counter = 0;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->a0 = 255;
    for (i = 0; i < effect->count; i++, work++) {
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        work->velocity.x = (rand() % 11 - 5) * 0.5f;
        work->velocity.y = (rand() % 6 + 1) * 0.8f;
        work->velocity.z = (rand() % 10 - 5) * 0.5f;
        work->scale = scale;
        work->r0 = work->r1 = rand() % 256;
        work->g0 = work->g1 = rand() % (255 - work->r1);
        work->b0 = work->b1 = 255 - work->r1 - work->g1;
        work->r0 += 168;
        work->g0 += 168;
        work->b0 += 168;
        if (work->r0 > 255) {
            work->r0 = 255;
        }
        if (work->g0 > 255) {
            work->g0 = 255;
        }
        if (work->b0 > 255) {
            work->b0 = 255;
        }
        work->r1 *= 0.6f;
        work->g1 *= 0.6f;
        work->b1 *= 0.6f;
        work->rot = rand() % 360;
    }
    return effect;
}

void effPikkyoloMain(EffectEntry* effect) {
    EffPikkyoloN64Work* work = effect->userdata;
    s32 i;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->counter++;
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    if (work->life < 8) {
        work->a0 = work->life << 5;
    }
    for (i = 0; i < effect->count; i++, work++) {
        work->velocity.y -= 0.5f;
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        if (work->position.y < 0.0f && work->velocity.y < 0.0f) {
            work->position.y = 0.0f;
            work->velocity.y *= -0.7f;
        }
        work->rot += ((i & 1) * 2 - 1) * 14;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effPikkyoloDisp, effect);
}

void effPikkyoloDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffPikkyoloN64Work* work;
    s32 i;
    s32 alpha;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->a0;
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x41, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.0625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex16x16_vtx);
    for (i = 0; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scaleMtx, 0.5f * work->scale, 0.5f * work->scale, 0.5f * work->scale);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){work->r0, work->g0, work->b0, alpha});
        GXSetTevColor(GX_TEVREG1, (GXColor){work->r1, work->g1, work->b1, work->a1});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2((i & 3) * 4, (i & 3) * 4 + 1, (i & 3) * 4 + 2, (i & 3) * 4, (i & 3) * 4, (i & 3) * 4 + 2, (i & 3) * 4 + 3, (i & 3) * 4);
    }
}
