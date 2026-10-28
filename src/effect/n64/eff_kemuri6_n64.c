#include "eff/n64/eff_kemuri6_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffKemuri6N64Work {
    BOOL active;     // 0x0
    u16 type;        // 0x4
    s16 timer;       // 0x6
    u8 alpha;        // 0x8
    u8 pad_9[3];     // 0x9
    Vec position;    // 0xC
    f32 rotation;    // 0x18
    f32 scaleX;      // 0x1C
    f32 scaleY;      // 0x20
    f32 scaleZ;      // 0x24
    f32 baseX;       // 0x28
    f32 baseY;       // 0x2C
    f32 angle;       // 0x30
    u8 pad_34[4];    // 0x34
    f32 velocity;    // 0x38
    f32 accel;       // 0x3C
    f32 accelAdd;    // 0x40
    f32 accelAddStep;  // 0x44
    u8 pad_48[8];    // 0x48
} EffKemuri6N64Work;

static effN64Vtx kmr_a_1_v[] = {
    {-8, 7, 0, 0, 0, 255, 255, 255, 255},
    {7, 7, 0, 512, 0, 255, 255, 255, 255},
    {-8, -8, 0, 0, 512, 255, 255, 255, 255},
    {7, -8, 0, 512, 512, 255, 255, 255, 255},
};

void effKemuri6Main(EffectEntry* effect);
void effKemuri6Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri6N64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffKemuri6N64Work* work;
    int i;

    effect = effEntry();
    effect->type = "Kemuri6N64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri6N64Work));
    effect->userdata = work;
    effect->callback = effKemuri6Main;
    for (i = 0; i < 1; i++) {
        work->active = TRUE;
        work->type = type;
        work->accelAdd = 0.0f;
        work->accelAddStep = 0.0f;
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        work->scaleX = 1.0f;
        work->scaleY = 1.0f;
        work->scaleZ = 1.0f;
        work->alpha = 255;
        work->baseX = 0.03f * (rand() % 11) + 1.4f;
        work->baseY = 0.03f * (rand() % 11) + 1.5f;
        work->angle = rand() % 61;
        work->type = type;
        work->velocity = 2.0f;
        work->accel = -0.5f;
        work->timer = 15;
        work->rotation = rand() % 361;
        work->alpha = 255;
    }
    return effect;
}

void effKemuri6Main(EffectEntry* effect) {
    EffKemuri6N64Work* work;
    int i;
    BOOL disp;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    disp = FALSE;
    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    for (i = 0; i < effect->count; i++, work++) {
        if (work->active == 0) {
            continue;
        }
        if (--work->timer <= 0) {
            work->active = 0;
            continue;
        }
        disp = TRUE;
        work->angle = reviseAngle(12.0f + work->angle);
        {
            f32 s = (f32)sin((6.2831855f * work->angle) / 360.0f);
            work->scaleX = 0.1f * s + work->baseX;
        }
        work->scaleY = 0.1f * (f32)cos((6.2831855f * work->angle) / 360.0f) + work->baseY;
        // Holding this puff returns from the whole update, so the remaining elements and this frame's dispEntry are skipped.
        if (work->type == 0 && work->timer > 4 && work->timer <= 5 && marioGetPtr()->currMotionId == MARIO_MOTION_STAY) {
            work->timer++;
            return;
        }
        work->accel += work->accelAdd;
        work->velocity += work->accel;
        work->position.y += work->velocity;
        work->alpha -= 15;
        work->baseX += 0.14f;
        work->baseY *= 0.94f;
        if (work->velocity < 0.0f) {
            work->accel = 0.0f;
            work->velocity = 0.0f;
        }
    }
    if (!disp) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri6Disp, effect);
    }
}

void effKemuri6Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffKemuri6N64Work* work;
    int i;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    effGetTexObjN64(0x4E, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(kmr_a_1_v);
    for (i = 0; i < effect->count; i++, work++) {
        if (work->active == 0) {
            continue;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){112, 96, 24, work->alpha});
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(scaleMtx, work->scaleX, work->scaleY, work->scaleZ);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotation);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 3, 1, 0, 0, 2, 3, 0);
    }
}
