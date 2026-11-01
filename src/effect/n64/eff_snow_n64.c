// Paper Mario 64 effect: a single snowflake that falls while tumbling for `lifetime` frames.

#include "eff/n64/eff_snow_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffSnowN64Work {
    Vec position;   // 0x0
    Vec velocity;   // 0xC
    f32 rotX;       // 0x18
    f32 rotY;       // 0x1C
    f32 rotSpeedX;  // 0x20
    f32 rotSpeedY;  // 0x24
    s32 timer;      // 0x28
} EffSnowN64Work;

void effSnowMain(EffectEntry* effect);
void effSnowDisp(CameraId cameraId, EffectEntry* effect);

/** Spawns one snowflake at (x, y, z) living `lifetime` frames. Spawned by event scripts through evt_eff. */
EffectEntry* effSnowN64Entry(s32 lifetime, f32 x, f32 y, f32 z) {
    static s32 kaiten;
    EffectEntry* effect;
    EffSnowN64Work* work;

    effect = effEntry();
    effect->type = "SnowN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSnowN64Work));
    effect->userdata = work;
    effect->callback = effSnowMain;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->velocity.x = 0.0f;
    work->velocity.y = -1.5f;
    work->velocity.z = 0.0f;
    work->rotX = 0.0f;
    work->rotY = 0.0f;
    work->rotSpeedX = 1.5f;
    work->rotSpeedY = kaiten ? -5.0f : 5.0f;
    work->timer = lifetime;
    if (++kaiten > 1) {
        kaiten = 0;
    }
    return effect;
}

/**
 * Moves the flake by its velocity, rocks its tilt between 10 and 45 degrees and turns it about Y;
 * it wraps to stay within 200 units of Mario on X and Z so the snow follows him.
 * Deletes when the timer runs out. CAMERA_3D, layer 2.
 */
void effSnowMain(EffectEntry* effect) {
    EffSnowN64Work* work = effect->userdata;
    MarioWork* mario;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->position.x += work->velocity.x;
    work->position.y += work->velocity.y;
    work->position.z += work->velocity.z;
    work->rotX += work->rotSpeedX;
    if (work->rotX < 10.0f) {
        work->rotX = 10.0f;
        work->rotSpeedX = -work->rotSpeedX;
    }
    if (work->rotX > 45.0f) {
        work->rotX = 45.0f;
        work->rotSpeedX = -work->rotSpeedX;
    }
    work->rotY += work->rotSpeedY;
    mario = marioGetPtr();
    if (work->position.x - mario->position.x > 200.0f) {
        work->position.x -= 400.0f;
    } else if (mario->position.x - work->position.x > 200.0f) {
        work->position.x += 400.0f;
    }
    if (work->position.z - mario->position.z > 200.0f) {
        work->position.z -= 400.0f;
    } else if (mario->position.z - work->position.z > 200.0f) {
        work->position.z += 400.0f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effSnowDisp, effect);
}

/**
 * Draws the flake as a 32x32 white quad of texture 7 (tiled twice) at its position,
 * facing the camera and then tilted and turned by its rotation.
 */
void effSnowDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffSnowN64Work* work;
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
    PSMTXConcat(mtx, rot, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    effGetTexObjN64(7, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-16.0f, 0.0f, 16.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(16.0f, 0.0f, 16.0f);
    GXTexCoord2f32(2.0f, 0.0f);
    GXPosition3f32(16.0f, 0.0f, -16.0f);
    GXTexCoord2f32(2.0f, 2.0f);
    GXPosition3f32(-16.0f, 0.0f, -16.0f);
    GXTexCoord2f32(0.0f, 2.0f);
}
