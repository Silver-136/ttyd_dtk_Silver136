#include "eff/n64/eff_onpu_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffOnpuN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 alpha;    // 0x14
    s32 lifetime; // 0x18
    s32 frame;    // 0x1C
    s32 texOfs;   // 0x20
    f32 vx;       // 0x24
    f32 vy;       // 0x28
    f32 targetVx; // 0x2C
    f32 targetVy; // 0x30
} EffOnpuN64Work;

static effN64Vtx size16x32_tex16x32_vtx[] = {
    {-8, 0, 0, 0, 0, 0, 0, 0, 255},
    {8, 0, 0, 512, 0, 0, 0, 0, 255},
    {8, 32, 0, 512, 1024, 0, 0, 0, 255},
    {-8, 32, 0, 0, 1024, 0, 0, 0, 255},
};

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

void effOnpuMain(EffectEntry* effect);
void effOnpuDisp(CameraId cameraId, EffectEntry* effect);

/**
 * Spawns one music note at (x, y, z) living 64 frames.
 * Type 0 rises straight up; type 1 drifts left and type 2 right while slowing to a stop.
 * Pansy units spawn types 1 and 2 via evt_eff64 "onpu_n64".
 */
EffectEntry* effOnpuN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffOnpuN64Work* work;

    effect = effEntry();
    effect->type = "OnpuN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffOnpuN64Work));
    effect->userdata = work;
    effect->callback = effOnpuMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.0f;
    work->lifetime = 64;
    work->alpha = 0;
    work->frame = 0;
    work->texOfs = rand() % 7;
    switch (type) {
        case 0:
            work->vx = 0.0f;
            work->targetVx = 0.0f;
            work->vy = 1.0f;
            work->targetVy = 1.0f;
            break;
        case 1:
            work->vx = -5.0f + 0.1f * (rand() % 11);
            work->vy = 0.5f + 0.1f * (rand() % 11);
            work->targetVx = 0.0f;
            work->targetVy = 0.0f;
            break;
        default:
            work->vx = 5.0f - 0.1f * (rand() % 11);
            work->vy = 1.0f + 0.1f * (rand() % 11);
            work->targetVx = 0.0f;
            work->targetVy = 0.0f;
            break;
    }
    return effect;
}

/** Grows and fades the note in, moves it by its easing velocity, and fades it out over the last 10 frames. */
void effOnpuMain(EffectEntry* effect) {
    EffOnpuN64Work* work = effect->userdata;
    s32 lifetime;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->lifetime--;
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (lifetime > 5) {
        work->alpha = work->alpha + 0.3f * (255 - work->alpha);
        work->scale += 0.3f * (1.0f - work->scale);
    }
    if (lifetime < 10) {
        work->alpha = lifetime * 25;
    }
    work->position.x += work->vx;
    work->position.y += work->vy;
    work->vx += 0.04f * (work->targetVx - work->vx);
    work->vy += 0.04f * (work->targetVy - work->vy);
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effOnpuDisp, effect);
}

/**
 * Draws the note as a camera-facing 16x32 quad using two scrolling note textures (0x28, 0x29) with the color
 * cycling.
 */
void effOnpuDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffOnpuN64Work* work;
    s32 offset;
    s32 colorIdx;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    offset = work->texOfs;
    colorIdx = work->frame * 3;
    colorIdx = colorIdx % 36;
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.004464286f, 0.0f);
    offset <<= 5;
    PSMTXTrans(mtx, 0.0f, offset, 0.0f);
    PSMTXConcat(scale, mtx, scale);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scale, 0.0625f, 0.004464286f, 0.0f);
    PSMTXTrans(mtx, 0.0f, offset, 0.0f);
    PSMTXConcat(scale, mtx, scale);
    GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){color_rotation_data[colorIdx], color_rotation_data[colorIdx + 1], color_rotation_data[colorIdx + 2], work->alpha});
    effGetTexObjN64(0x28, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x29, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, 0.0f);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x32_tex16x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
