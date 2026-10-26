#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/n64/eff_kemuri4_n64.h"
#include "eff/n64/eff_flower_n64.h"

typedef struct EffFlowerN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 velX;     // 0x10
    f32 velY;     // 0x14
    f32 velZ;     // 0x18
    f32 accel1;   // 0x1C
    f32 accel2;   // 0x20
    f32 rotX;     // 0x24
    f32 rotY;     // 0x28
    f32 rotVelX;  // 0x2C
    f32 rotVelY;  // 0x30
    s32 lifetime; // 0x34
    s32 frame;    // 0x38
    s32 dir;      // 0x3C
    s32 swing;    // 0x40
    s32 texIdx;   // 0x44
} EffFlowerN64Work;

void effFlowerMain(EffectEntry* effect);
void effFlowerDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effFlowerN64Entry(s32 type, f32 x, f32 y, f32 z, s32 lifetime) {
    EffectEntry* effect;
    EffFlowerN64Work* work;
    static s32 kaiten;

    effect = effEntry();
    effect->type = "FlowerN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFlowerN64Work));
    effect->userdata = work;
    effect->callback = effFlowerMain;
    work->type = type;
    work->position.x = x;
    work->position.y = 10.0f + y;
    work->position.z = z;
    work->velX = 0.0f;
    work->velY = 0.0f;
    work->velZ = 0.0f;
    if (type == 0) {
        work->accel1 = 0.12f;
        work->accel2 = -0.0152f;
        work->velZ = 0.0f;
        work->rotX = 0.0f;
        work->rotY = 0.0f;
        work->rotVelX = 3.0f;
        work->rotVelY = kaiten != 0 ? -10 : 10;
        work->lifetime = lifetime;
    } else {
        work->accel1 = 0.12f;
        work->accel2 = -0.0152f;
        work->rotX = (rand() % 2) * 30 - 15;
        work->rotY = rand() % 361;
        work->rotVelX = 0.0f;
        work->rotVelY = (rand() % 2) * 8 - 4;
        work->lifetime = lifetime;
        work->frame = 0;
        work->texIdx = rand() % 11;
        work->swing = rand() % 21;
        work->dir = (rand() % 2) * 2 - 1;
    }
    kaiten++;
    if (kaiten > 1) {
        kaiten = 0;
    }
    return effect;
}

void effFlowerMain(EffectEntry* effect) {
    EffFlowerN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (work->lifetime < 150) {
        work->velX -= 0.001f;
        work->position.x += work->velX;
        work->position.z += work->velZ;
        work->rotX += work->rotVelX;
        if (type == 0) {
            if (work->rotX < 10.0f) {
                work->rotX = 10.0f;
                work->rotVelX = -work->rotVelX;
            }
            if (work->rotX > 45.0f) {
                work->rotX = 45.0f;
                work->rotVelX = -work->rotVelX;
            }
        }
        work->rotY += work->rotVelY;
        if (type == 0) {
            work->velY = 1.4f;
        } else {
            work->velY = 1.0f;
        }
    } else {
        work->accel2 += 0.0007f;
        work->accel1 += work->accel2;
        work->velY += work->accel1;
        if (work->velY > 1.0f) {
            work->velY = 1.0f;
        }
    }
    if (type == 1) {
        work->swing += work->dir;
        if (work->swing >= 20) {
            work->swing = 20;
            work->dir = -1;
        } else if (work->swing <= 0) {
            work->swing = 0;
            work->dir = 1;
        }
    }
    work->position.y += work->velY;
    work->lifetime--;
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFlowerDisp, effect);
}

void effFlowerDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFlowerN64Work* work;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 255});
    GXSetCullMode(GX_CULL_NONE);
    effGetTexObjN64(0x4C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    effSetVtxDescN64(flowers_v);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 2, 5, 0, 4, 5, 6, 0);
    tri2(7, 8, 2, 0, 7, 2, 9, 0);
    tri2(10, 2, 11, 0, 10, 11, 12, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_A0, GX_CA_ZERO);
    effSetVtxDescN64(&flowers_v[13]);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 24);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(2, 4, 5, 0, 2, 5, 3, 0);
    tri2(6, 7, 8, 0, 6, 8, 9, 0);
    tri2(8, 10, 11, 0, 8, 11, 9, 0);
}
