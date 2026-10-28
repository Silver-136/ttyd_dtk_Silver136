#include "eff/n64/eff_balloon_heiho_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffBalloonHeihoN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 speed;    // 0x10
    f32 accel;    // 0x14
    f32 scale;    // 0x18
    s32 lifetime; // 0x1C
    s32 frame;    // 0x20
} EffBalloonHeihoN64Work;

static effN64Vtx size32x64_tex32x64_vtx[] = {
    {-16, 64, 0, 0, 2048, 0, 0, 0, 255},
    {16, 64, 0, 1024, 2048, 0, 0, 0, 255},
    {-16, 32, 0, 0, 1024, 0, 0, 0, 255},
    {16, 32, 0, 1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 512, 0, 0, 0, 255},
    {16, 16, 0, 1024, 512, 0, 0, 0, 255},
    {-16, 0, 0, 0, 0, 0, 0, 0, 255},
    {16, 0, 0, 1024, 0, 0, 0, 0, 255},
};

void effBalloonHeihoMain(EffectEntry* effect);
void effBalloonHeihoDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effBalloonHeihoN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 accel, f32 scale) {
    EffectEntry* effect;
    EffBalloonHeihoN64Work* work;

    effect = effEntry();
    effect->type = "BalloonHeihoN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBalloonHeihoN64Work));
    effect->userdata = work;
    effect->callback = effBalloonHeihoMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->speed = 0.0f;
    work->lifetime = lifetime;
    work->frame = 0;
    work->accel = accel;
    work->scale = scale;
    return effect;
}

void effBalloonHeihoMain(EffectEntry* effect) {
    EffBalloonHeihoN64Work* work = effect->userdata;
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
    work->speed += work->accel;
    if (work->speed > 2.0f) {
        work->speed = 2.0f;
    }
    work->position.y += work->speed;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBalloonHeihoDisp, effect);
}

void effBalloonHeihoDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffBalloonHeihoN64Work* work;
    s32 type;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    mapSetMaterialFog();
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, 1.0f);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    switch (type) {
        case 0:
            effGetTexObjN64(0x60, &texObj);
            break;
        case 1:
            effGetTexObjN64(0x61, &texObj);
            break;
        case 2:
            effGetTexObjN64(0x62, &texObj);
            break;
    }
    // texObj is set only for type 0..2 above; any other type loads it uninitialized.
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x64_tex32x64_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
    tri2(0, 1, 2, 0, 1, 2, 3, 0);
    tri2(0, 1, 2, 0, 1, 2, 3, 0);
    tri2(2, 3, 4, 0, 3, 4, 5, 0);
    tri2(4, 5, 6, 0, 5, 6, 7, 0);
}
