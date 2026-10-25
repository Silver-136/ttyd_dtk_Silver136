#include "eff/n64/eff_breath_fire_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

void effBreathFireMain(EffectEntry* effect);
void effBreathFireDisp(CameraId cameraId, EffectEntry* effect);

static effN64Vtx stg_fire_breath_v[] = {
    {100, -100, 0, 3072, 0, 0, 0, 0, 255},
    {100, 100, 0, 3072, 1024, 0, 0, 0, 255},
    {0, 0, 0, 2560, 512, 255, 255, 255, 255},
    {-100, 100, 0, 2048, 1024, 0, 0, 0, 255},
    {-100, -100, 0, 2048, 0, 0, 0, 0, 255},
    {100, -100, 5, 1024, 0, 0, 0, 0, 255},
    {100, 100, 5, 1024, 1024, 0, 0, 0, 255},
    {0, 0, 5, 512, 512, 255, 255, 255, 255},
    {-100, 100, 5, 0, 1024, 0, 0, 0, 255},
    {-100, -100, 5, 0, 0, 0, 0, 0, 255},
};

EffectEntry* effBreathFireN64Entry(s32 type, s32 count, s32 seed, s32 lifetime, f32 x, f32 y, f32 z, f32 endX,
                                   f32 endY, f32 endZ) {
    EffectEntry* effect;
    EffBreathFireN64Work* work;

    effect = effEntry();
    effect->type = "BreathFireN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBreathFireN64Work));
    effect->userdata = work;
    effect->callback = effBreathFireMain;
    work->type = type;
    work->count = count;
    work->seed = seed;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->start.x = x;
    work->start.y = y;
    work->start.z = z;
    work->end.x = endX;
    work->end.y = endY;
    work->end.z = endZ;
    if (type == 0) {
        work->scale = 0.05f;
    } else {
        work->scale = 0.040000003f;
    }
    work->baseScale = work->scale;
    work->lifetime = lifetime;
    work->remaining = lifetime;
    work->frame = 0;
    work->alpha = 255;
    work->texFrame = 0.0f;
    work->scaleRate = 0.1f;
    work->targetScale = work->baseScale;
    if (type == 0) {
        work->vel.x = 0.2f * (0.2f * (endX - x) * (f32)((s32)effTblRandN64(10, seed) - 5));
        work->vel.y = 0.2f * (0.2f * (endY - y) * (f32)((s32)effTblRandN64(10, seed + 1) - 5));
        work->vel.z = 0.2f * (0.2f * (endZ - z) * (f32)((s32)effTblRandN64(10, seed + 2) - 5));
    } else {
        work->vel.x = 0.2f * (endX - x) * (f32)((s32)effTblRandN64(10, seed + 3) - 5);
        work->vel.y = 0.2f * (endY - y) * (f32)((s32)effTblRandN64(10, seed + 4) - 5);
        work->vel.z = 0.2f * (endZ - z) * (f32)((s32)effTblRandN64(10, seed + 5) - 5);
    }
    work->velY = 0.0f;
    work->color[0] = 255;
    work->color[1] = 170;
    work->color[2] = 42;
    work->color[3] = 243;
    work->color[4] = 48;
    work->color[5] = 0;
    return effect;
}

void effBreathFireMain(EffectEntry* effect) {
    s32 remaining;
    EffBreathFireN64Work* work;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 lifetime;
    s32 frame;
    EffectEntry* child;

    work = effect->userdata;
    tmp.x = work->pos.x;
    tmp.y = work->pos.y;
    tmp.z = work->pos.z;
    position = tmp;
    work->texFrame = 4.0f * work->frame / 10.0f;
    work->remaining--;
    work->frame++;
    remaining = work->remaining;
    if (remaining < 0) {
        effDelete(effect);
    } else {
        frame = work->frame;
        lifetime = work->lifetime;
        if (remaining > 5 && work->type == 0) {
            work->scale += 0.05f * (2.5f - work->scale);
        }
        work->pos.x = work->start.x + frame * (work->vel.x + (work->end.x - work->start.x)) / lifetime;
        work->pos.y = work->start.y + frame * (work->vel.y + (work->end.y - work->start.y)) / lifetime;
        work->pos.z = work->start.z + frame * (work->vel.z + (work->end.z - work->start.z)) / lifetime;
        if (work->type == 1) {
            work->velY = 0.01f * frame + work->velY;
            work->pos.y = work->pos.y + work->velY;
        }
        if (frame == work->seed + 1 && work->count > 0) {
            child = effBreathFireN64Entry(work->type, work->count - 1, work->seed, lifetime, work->start.x,
                                          work->start.y, work->start.z, work->end.x, work->end.y, work->end.z);
            ((EffBreathFireN64Work*)child->userdata)->color[0] = work->color[0];
            ((EffBreathFireN64Work*)child->userdata)->color[1] = work->color[1];
            ((EffBreathFireN64Work*)child->userdata)->color[2] = work->color[2];
            ((EffBreathFireN64Work*)child->userdata)->color[3] = work->color[3];
            ((EffBreathFireN64Work*)child->userdata)->color[4] = work->color[4];
            ((EffBreathFireN64Work*)child->userdata)->color[5] = work->color[5];
            ((EffBreathFireN64Work*)child->userdata)->baseScale = ((EffBreathFireN64Work*)child->userdata)->scale = work->baseScale;
            ((EffBreathFireN64Work*)child->userdata)->targetScale = work->targetScale;
            ((EffBreathFireN64Work*)child->userdata)->scaleRate = work->scaleRate;
        }
        if (remaining < 10 && work->type == 0) {
            work->alpha = remaining * 25;
        }
        if (work->type == 1) {
            work->alpha = remaining * 224 / lifetime;
            work->scale += 0.008f * (0.3 - work->scale);
        }
        if (work->type == 2) {
            work->alpha = remaining * 224 / lifetime;
            work->scale += work->scaleRate * (work->targetScale - work->scale);
        }
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBreathFireDisp, effect);
    }
}

void effBreathFireDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffBreathFireN64Work* work;
    GXTexObj texObj;
    Mtx scale;
    Mtx mtx;
    Mtx rot;
    f32 f;
    s32 n;
    s32 m;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    f = work->texFrame;
    n = (s32)f;
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 4, 2, 10, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 0xff);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 0);
    GXSetTevAlphaIn(1, 0, 4, 2, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 30, 0, 0x7d);
    GXSetTexCoordGen2(1, 1, 4, 33, 0, 0x7d);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->color[0], work->color[1], work->color[2], work->alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color[3], work->color[4], work->color[5], (u8)(256.0f * (f - n))});
    effGetTexObjN64(0x77, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x77, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    m = n << 5;
    PSMTXScale(mtx, 1.0f / 128.0f, 1.0f / 32.0f, 0.0f);
    PSMTXTrans(scale, m, 0.0f, 0.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, 30, GX_MTX2x4);
    PSMTXScale(mtx, 1.0f / 128.0f, 1.0f / 32.0f, 0.0f);
    PSMTXTrans(scale, m + 32, 0.0f, 0.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, 33, GX_MTX2x4);
    PSMTXTrans(scale, work->pos.x, work->pos.y, work->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(mtx, work->scale, work->scale, 0.0f);
    PSMTXConcat(scale, rot, scale);
    PSMTXConcat(scale, mtx, scale);
    PSMTXConcat(camera->view, scale, scale);
    GXLoadPosMtxImm(scale, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(&stg_fire_breath_v[5]);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
    tri2(0, 1, 2, 0, 1, 3, 2, 0);
    tri2(4, 2, 3, 0, 4, 0, 2, 0);
}
