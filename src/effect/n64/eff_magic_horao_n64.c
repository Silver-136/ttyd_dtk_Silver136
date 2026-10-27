#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_n64.h"
#include "eff/n64/eff_hokori_n64.h"
#include "eff/n64/eff_magic_horao_n64.h"

double sin(double);

typedef struct EffMagicHoraoN64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 vy;         // 0x10
    f32 startSize;  // 0x14
    f32 endSize;    // 0x18
    s32 startAlpha; // 0x1C
    s32 endAlpha;   // 0x20
    f32 size;       // 0x24
    f32 blendRate;  // 0x28
    s32 state;      // 0x2C
    s32 timer;      // 0x30
    f32 scale;      // 0x34
    s32 angle;      // 0x38
    s32 alpha;      // 0x3C
    s32 lifetime;   // 0x40
    s32 frame;      // 0x44
} EffMagicHoraoN64Work;

static effN64Vtx size64x64_tex64x64_vtx[] = {
    {-3200, 0, 0, 0, 0, 0, 0, 0, 0xFF},
    {3200, 0, 0, 2048, 0, 0, 0, 0, 0xFF},
    {3200, 6400, 0, 2048, 2048, 0, 0, 0, 0xFF},
    {-3200, 6400, 0, 0, 2048, 0, 0, 0, 0xFF},
};

void effMagicHoraoMain(EffectEntry* effect);
void effMagicHoraoDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effMagicHoraoN64Entry(s32 type, f32 x, f32 y, f32 z, f32 startSize, s32 startAlpha, f32 endSize, s32 endAlpha,
                                   f32 blendRate, s32 lifetime) {
    EffectEntry* effect;
    EffMagicHoraoN64Work* work;

    effect = effEntry();
    effect->type = "MagicHoraoN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffMagicHoraoN64Work));
    effect->userdata = work;
    effect->callback = effMagicHoraoMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->blendRate = blendRate;
    work->startSize = startSize;
    work->startAlpha = startAlpha;
    work->endSize = endSize;
    work->endAlpha = endAlpha;
    work->scale = 1.0f;
    work->angle = rand() % 360;
    work->lifetime = lifetime;
    work->frame = 0;
    work->alpha = startAlpha;
    work->size = startSize;
    work->vy = 0.0f;
    work->state = 0;
    return effect;
}

void effMagicHoraoMain(EffectEntry* effect) {
    EffMagicHoraoN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    switch (work->state) {
        case 0:
            work->size += work->blendRate * (work->endSize - work->size);
            work->alpha = work->alpha + work->blendRate * (work->endAlpha - work->alpha);
            work->scale -= 0.02f;
            if (work->scale < 0.0f) {
                work->scale = 0.0f;
                work->vy = 0.0f;
                work->timer = 10;
                work->state = 1;
            }
            break;
        case 1:
            work->timer--;
            if (work->timer <= 0) {
                work->state = 2;
                work->timer = 10;
            }
            break;
        case 2:
            work->timer--;
            if (work->timer <= 0) {
                work->state = 3;
            }
            break;
        case 3:
            work->vy -= 1.0f;
            work->position.y += work->vy;
            if (work->position.y < 0.0f) {
                work->position.y = 0.0f;
                effHokoriN64Entry(2, 30, 5.0f + work->position.x, work->position.y, work->position.z);
                effHokoriN64Entry(2, 30, work->position.x - 5.0f, work->position.y, work->position.z);
                work->state = 4;
                work->timer = 20;
            }
            break;
        case 4:
            work->timer--;
            if (work->timer <= 0) {
                work->state = 5;
            }
            break;
        case 5:
            if (work->lifetime < 10) {
                work->alpha = work->lifetime * 25;
            }
            work->scale -= 0.05f;
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMagicHoraoDisp, effect);
}

void effMagicHoraoDisp(CameraId cameraId, EffectEntry* effect) {
    effN64Vtx* vtx;
    effN64Vtx* v;
    s32 i;
    s32 angle;
    s32 j;
    CameraEntry* camera;
    EffMagicHoraoN64Work* work;
    s32 frame;
    s32 alpha;
    s32 y, x;
    f32 ang2;
    f32 ang;
    Mtx mtx;
    Mtx scale;
    GXTexObj texObj;
    f32 s;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    s = work->scale;
    angle = work->angle;
    alpha = work->alpha;
    frame = work->frame;
    PSMTXScale(scale, 0.01f, 0.01f, 0.01f);
    PSMTXTrans(mtx, 100.0f * work->position.x, 100.0f * work->position.y, 100.0f * work->position.z);
    PSMTXConcat(scale, mtx, mtx);
    PSMTXScale(scale, work->size, work->size, work->size);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    // FAKE: ang is assigned in a comma expression inside the initializer; assigning it in its own statement changes the object
    GXSetTevColor(GX_TEVREG0, (GXColor){(ang = 6.2831855f * (f32)(frame * 30) / 360.0f, 225.0f + 25.0f * (f32)sin(ang)), 225.0f + 25.0f * (f32)sin(ang), 0xFF, alpha});
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effSetVtxDescN64(size64x64_tex64x64_vtx);
    if (work->state == 1) {
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        effGetTexObjN64(0x56, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, (30 - work->timer) * 255 / 30});
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        effGetTexObjN64(0x57, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    } else {
        if (work->state >= 1) {
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
            effGetTexObjN64(0x57, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
        } else {
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
            effGetTexObjN64(0x56, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
        }
        vtx = smartAlloc(sizeof(effN64Vtx) * 32, 3)->address;
        for (i = 0; i < 16; i++) {
            v = vtx + i * 2;
            ang = 500.0f * (f32)sin(6.2831855f * (f32)(angle + (frame * 10 + i * 60)) / 360.0f);
            x = s * ang;
            ang2 = 200.0f * (f32)sin(6.2831855f * (f32)(angle + (frame * 10 + i * 6)) / 360.0f);
            y = s * ang2;
            v[0].x = x - 3200;
            v[0].y = y + i * 400;
            v[0].z = 0;
            v[0].s = 0;
            v[0].t = (i * 2) << 6;
            v[1].x = x + 3200;
            v[1].y = y + i * 400;
            v[1].z = 0;
            v[1].s = 2048;
            v[1].t = (i * 2) << 6;
        }
        DCFlushRange(vtx, sizeof(effN64Vtx) * 32);
        GXInvalidateVtxCache();
        effSetVtxDescN64(vtx);
        for (i = 0; i < 15; i++) {
            j = i * 2;
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(j, j + 2, j + 1, j, j + 1, j + 2, j + 3, j);
        }
    }
}
