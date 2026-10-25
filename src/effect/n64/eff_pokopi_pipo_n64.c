#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffPokopiPipoN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    Vec start;     // 0x10
    Vec end;       // 0x1C
    s32 lifetime;  // 0x28
    s32 frame;     // 0x2C
    s32 r;         // 0x30
    s32 g;         // 0x34
    s32 b;         // 0x38
    s32 alpha;     // 0x3C
    f32 scale;     // 0x40
    f32 velX;      // 0x44
    f32 velY;      // 0x48
    f32 velZ;      // 0x4C
    f32 angle;     // 0x50
    f32 angleVel;  // 0x54
    f32 scaleX;    // 0x58
    f32 scaleY;    // 0x5C
    s32 scaleIdx;  // 0x60
    s32 falling;   // 0x64
    s32 duration;  // 0x68
} EffPokopiPipoN64Work;


static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, 1024, 0, 0, 0, 0, 255},
    {16, 16, 0, 1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 1024, 0, 0, 0, 255},
};

static u8 w_scale_data[7] = {110, 150, 130, 110, 100, 95, 100};
static u8 h_scale_data[7] = {80, 60, 80, 100, 120, 110, 100};

void effPokopiPipoMain(EffectEntry* effect);
void effPokopiPipoDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPokopiPipoN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 x2, f32 y2, f32 z2, f32 scale) {
    EffectEntry* effect;
    EffPokopiPipoN64Work* work;

    effect = effEntry();
    effect->type = "PokopiPipoN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPokopiPipoN64Work));
    effect->userdata = work;
    effect->callback = effPokopiPipoMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = duration + 60;
    }
    work->duration = duration;
    work->alpha = 255;
    work->start.x = x;
    work->start.y = y;
    work->start.z = z;
    work->end.x = x2;
    work->end.y = y2;
    work->end.z = z2;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->velX = (x2 - x) / (f32)duration;
    work->velY = (y2 - y) / (f32)duration - -0.1f * (f32)duration;
    work->velZ = (z2 - z) / (f32)duration;
    if (y2 - y < 0.0f) {
        work->falling = 1;
    } else {
        work->falling = 0;
    }
    work->scale = scale;
    work->r = 70;
    work->g = 180;
    work->b = 120;
    work->angle = (f32)(rand() % 360);
    work->angleVel = (f32)(rand() % 10 + 5);
    work->scaleIdx = -1;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    return effect;
}

void effPokopiPipoMain(EffectEntry* effect) {
    EffPokopiPipoN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 idx;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
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
    idx = work->scaleIdx;
    frame = work->frame;
    if (idx >= 0 && idx < 7) {
        work->scaleX = 0.01f * w_scale_data[idx];
        work->scaleY = 0.01f * h_scale_data[idx];
        work->angle -= 40.0f;
        work->scaleIdx++;
    } else {
        work->velY += -0.2f;
        work->position.x += work->velX;
        work->position.y += work->velY;
        work->position.z += work->velZ;
        work->angle += work->angleVel;
    }
    if (frame - 1 == work->duration) {
        work->velY *= -1.0f;
        work->angleVel = -4.0f;
        work->scaleIdx = 0;
    } else if (work->velY < 0.0f && work->position.y < 14.285715f) {
        work->velY -= work->velY;
        work->position.y = 14.285715f;
        work->angleVel = -20.0f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effPokopiPipoDisp, effect);
}

void effPokopiPipoDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffPokopiPipoN64Work* work;
    s32 alpha;
    s32 type;
    s32 id;
    f32 scale;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    type = work->type;
    scale = 0.71428573f * work->scale;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scaleMtx, scale * work->scaleX, scale * work->scaleY, scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scaleMtx, mtx);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->angle);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    id = 0x96;
    if (type == 0) {
        id = 0x95;
    }
    effGetTexObjN64(id, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x32_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
