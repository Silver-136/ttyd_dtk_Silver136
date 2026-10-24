#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffKemuri5N64Work {
    s32 active;    // 0x0
    u16 field_4;   // 0x4
    s16 timer;     // 0x6
    u8 alpha;      // 0x8
    u8 pad_9[3];   // 0x9
    Vec position;  // 0xC
    Vec scale;     // 0x18
    f32 baseScaleX; // 0x24
    f32 baseScaleY; // 0x28
    f32 angle;     // 0x2C
    f32 speed;     // 0x30
    f32 velY;      // 0x34
    f32 accelY;    // 0x38
    f32 accelY2;   // 0x3C
    f32 field_40;  // 0x40
    f32 dirX;      // 0x44
    f32 dirZ;      // 0x48
} EffKemuri5N64Work;

double sin(double);
double cos(double);

static effN64Vtx kmr_a_1_v[] = {
    {-8, 7, 0, 0, 0, 255, 255, 255, 255},
    {7, 7, 0, 512, 0, 255, 255, 255, 255},
    {-8, -8, 0, 0, 512, 255, 255, 255, 255},
    {7, -8, 0, 512, 512, 255, 255, 255, 255},
};

void effKemuri5Main(EffectEntry* effect);
static inline GXColor mkc(u8 a) {
    GXColor c = {0x70, 0x60, 0x18, 0};
    c.a = a;
    return c;
}

void effKemuri5Disp(CameraId cameraId, EffectEntry* effect);

#pragma opt_propagation off
EffectEntry* effKemuri5N64Entry(f32 x, f32 y, f32 z, f32 angle) {
    s32 ang;
    EffectEntry* effect;
    EffKemuri5N64Work* work;
    f32 new_var;
    s32 i;
    f32 rad;
    f32 a = angle;

    effect = effEntry();
    effect->type = "Kemuri5N64";
    effect->count = 8;
    work = __memAlloc(HEAP_EFFECT, 8 * sizeof(EffKemuri5N64Work));
    effect->userdata = work;
    effect->callback = effKemuri5Main;
    for (i = 0, ang = 0; i < 8; i++, work++) {

        work->active = 1;
        work->field_40 = 0.0f;
        work->position.x = x;
        work->position.y = y;
        work->position.z = z;
        work->scale.x = 1.0f;
        work->scale.y = 1.0f;
        work->scale.z = 1.0f;
        work->alpha = 255;
        work->baseScaleX = 0.03f * (0, (f32)(rand() % 11)) + 1.0f;
        work->baseScaleY = 0.03f * (f32)(rand() % 11) + 1.7f;
        work->angle = (f32)(rand() % 61);
        work->timer = 30;
        new_var = (f32)ang; // FAKE: early float conversion temp; (f32)ang / implicit int at use both differ
        work->velY = 0.5f;
        work->accelY = -0.02f;
        work->accelY2 = 0.00005f;
        work->speed = -3.9f;
        rad = 6.2831855f * reviseAngle(a + new_var) / 360.0f;
        work->dirX = (f32)sin(rad);
        work->dirZ = (f32)cos(rad);
        ang += 45;
    }
    return effect;
}

void effKemuri5Main(EffectEntry* effect) {
    EffKemuri5N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 anyActive = 0;

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
        anyActive = 1;
        work->angle = reviseAngle(12.0f + work->angle);
        { f32 sv = (f32)sin(6.2831855f * work->angle / 360.0f); work->scale.x = 0.1f * sv + work->baseScaleX; }
        { f32 cv = (f32)cos(6.2831855f * work->angle / 360.0f); work->scale.y = 0.1f * cv + work->baseScaleY; }
        work->speed *= 0.83f;
        work->position.x = work->speed * work->dirX + work->position.x;
        work->position.z = work->speed * work->dirZ + work->position.z;
        work->accelY += work->accelY2;
        work->velY += work->accelY;
        work->position.y += work->velY;
        work->baseScaleX += 0.006f * (7.5f - work->baseScaleX);
        work->baseScaleY *= 0.98f;
        if (work->timer < 10) {
            work->baseScaleY *= 0.9f;
        }
        if (work->timer < 15) {
            work->alpha -= 16;
        }
    }
    if (anyActive == 0) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri5Disp, effect);
    }
}

#pragma opt_propagation reset

void effKemuri5Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri5N64Work* work = effect->userdata;
    s32 i;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    GXTexObj texObj;
    GXColor color;

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
        GXSetTevColor(GX_TEVREG0, mkc(work->alpha));
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(scaleMtx, work->scale.x, work->scale.y, work->scale.z);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 3, 1, 0, 0, 2, 3, 0);
    }
}
