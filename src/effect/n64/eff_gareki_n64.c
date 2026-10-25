#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffGarekiN64Work {
    Vec position;  // 0x0
    f32 groundY;   // 0xC
    f32 velX;      // 0x10
    f32 velY;      // 0x14
    f32 offsetX;   // 0x18
    f32 offsetY;   // 0x1C
    s32 lifetime;  // 0x20
    s32 state;     // 0x24
    s32 alpha;     // 0x28
    f32 rotX;      // 0x2C
    f32 rotY;      // 0x30
    f32 rotZ;      // 0x34
    f32 rotVelX;   // 0x38
    f32 rotVelY;   // 0x3C
} EffGarekiN64Work;

void gareki_dl(s32 type);
void effGarekiMain(EffectEntry* effect);
void effGarekiDisp(CameraId cameraId, EffectEntry* effect);

static f32 trans_data[] = {-5.0f, -4.0f, -3.0f, -2.0f, 2.0f, -3.0f, 5.0f, -4.0f, -4.0f, 2.0f, -1.0f, 4.0f, 3.0f, 2.0f, 5.0f, 4.0f};

static effN64Vtx stg__s_v[] = {
    {-6, -6, 0, 0, 0, 255, 255, 255, 255},
    {6, -6, 0, 1024, 0, 255, 255, 255, 255},
    {6, 6, 0, 1024, 1536, 255, 255, 255, 255},
    {-6, 6, 0, 0, 1536, 255, 255, 255, 255},
    {-6, 6, 0, 0, 3072, 255, 255, 255, 255},
    {6, 6, 0, 1024, 3072, 255, 255, 255, 255},
    {6, -6, 0, 1024, 1536, 255, 255, 255, 255},
    {-6, -6, 0, 0, 1536, 255, 255, 255, 255},
    {-1, -2, 0, 0, 0, 255, 255, 255, 255},
    {3, -2, 0, 341, 0, 255, 255, 255, 255},
    {-1, 4, 0, 0, 768, 255, 255, 255, 255},
    {-1, 4, 0, 0, 2304, 255, 255, 255, 255},
    {3, -2, 0, 341, 1536, 255, 255, 255, 255},
    {-1, -2, 0, 0, 1536, 255, 255, 255, 255},
    {1, -4, 0, 341, 0, 255, 255, 255, 255},
    {3, 2, 0, 512, 768, 255, 255, 255, 255},
    {-3, 2, 0, 0, 768, 255, 255, 255, 255},
    {-3, 2, 0, 0, 2304, 255, 255, 255, 255},
    {3, 2, 0, 512, 2304, 255, 255, 255, 255},
    {1, -4, 0, 341, 1536, 255, 255, 255, 255},
    {-4, -3, 0, 341, 0, 255, 255, 255, 255},
    {0, -3, 0, 683, 0, 255, 255, 255, 255},
    {4, 3, 0, 1024, 768, 255, 255, 255, 255},
    {-2, 3, 0, 512, 768, 255, 255, 255, 255},
    {-2, 3, 0, 512, 2304, 255, 255, 255, 255},
    {4, 3, 0, 1024, 2304, 255, 255, 255, 255},
    {0, -3, 0, 683, 1536, 255, 255, 255, 255},
    {-4, -3, 0, 341, 1536, 255, 255, 255, 255},
    {-3, -2, 0, 683, 0, 255, 255, 255, 255},
    {1, -2, 0, 1024, 0, 255, 255, 255, 255},
    {1, 4, 0, 1024, 768, 255, 255, 255, 255},
    {1, 4, 0, 1024, 2304, 255, 255, 255, 255},
    {1, -2, 0, 1024, 1536, 255, 255, 255, 255},
    {-3, -2, 0, 683, 1536, 255, 255, 255, 255},
    {4, -2, 0, 512, 768, 255, 255, 255, 255},
    {-2, 4, 0, 0, 1536, 255, 255, 255, 255},
    {-2, -2, 0, 0, 768, 255, 255, 255, 255},
    {-2, -2, 0, 0, 2304, 255, 255, 255, 255},
    {-2, 4, 0, 0, 3072, 255, 255, 255, 255},
    {4, -2, 0, 512, 2304, 255, 255, 255, 255},
    {1, -4, 0, 512, 768, 255, 255, 255, 255},
    {3, 2, 0, 683, 1536, 255, 255, 255, 255},
    {-1, 2, 0, 341, 1536, 255, 255, 255, 255},
    {-5, 2, 0, 0, 1536, 255, 255, 255, 255},
    {-5, 2, 0, 0, 3072, 255, 255, 255, 255},
    {-1, 2, 0, 341, 3072, 255, 255, 255, 255},
    {3, 2, 0, 683, 3072, 255, 255, 255, 255},
    {1, -4, 0, 512, 2304, 255, 255, 255, 255},
    {3, -2, 0, 1024, 768, 255, 255, 255, 255},
    {-1, 4, 0, 683, 1536, 255, 255, 255, 255},
    {-3, -2, 0, 512, 768, 255, 255, 255, 255},
    {-3, -2, 0, 512, 2304, 255, 255, 255, 255},
    {-1, 4, 0, 683, 3072, 255, 255, 255, 255},
    {3, -2, 0, 1024, 2304, 255, 255, 255, 255},
    {1, -4, 0, 1024, 768, 255, 255, 255, 255},
    {1, 2, 0, 1024, 1536, 255, 255, 255, 255},
    {-3, 2, 0, 683, 1536, 255, 255, 255, 255},
    {-3, 2, 0, 683, 3072, 255, 255, 255, 255},
    {1, 2, 0, 1024, 3072, 255, 255, 255, 255},
    {1, -4, 0, 1024, 2304, 255, 255, 255, 255},
};

static f32 vx_data[] = {-1.0f, -0.5f, 0.5f, 1.0f, -1.0f, -0.5f, 0.5f, 1.0f};

EffectEntry* effGarekiN64Entry(f32 x, f32 y, f32 z, f32 groundY) {
    f32* tp;
    s32 i;
    EffGarekiN64Work* work;
    f32* vp;
    EffectEntry* effect;
    EffGarekiN64Work* piece;

    effect = effEntry();
    effect->type = "GarekiN64";
    effect->count = 9;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffGarekiN64Work) * 9);
    effect->userdata = work;
    effect->callback = effGarekiMain;
    work->state = 0;
    work->position.x = x;
    work->position.y = y;
    work->groundY = groundY;
    work->position.z = z;
    work->velY = -2.0f;
    work->rotX = 0.0f;
    work->rotY = 0.0f;
    work->rotZ = rand() % 60 - 30;
    work->alpha = 255;
    work->rotVelX = rand() % 60 - 30;
    work->rotVelY = 0.25f * work->rotVelX;
    work->lifetime = 20;
    tp = trans_data + 2;
    vp = vx_data + 1;
    piece = work + 1;
    for (i = 1; i < 9; i++, piece++, tp += 2, vp++) {
        piece->offsetX = tp[-2];
        piece->offsetY = tp[-1];
        piece->position.x = 0.0f;
        piece->position.y = 0.0f;
        piece->position.z = 0.0f;
        piece->velX = 2.0f * vp[-1];
        piece->velY = 2.0f * (0.1f * (rand() % 10)) + 1.0f;
        piece->rotX = 0.0f;
        piece->rotZ = 0.0f;
    }
    return effect;
}

void effGarekiMain(EffectEntry* effect) {
    EffGarekiN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (work->state == 0) {
        work->velY += -0.1f;
        work->position.y += work->velY;
        work->rotX += work->rotVelX;
        work->rotY += work->rotVelY;
        if (work->position.y <= work->groundY) {
            work->state = 1;
        }
    }
    if (work->state == 1) {
        work->lifetime--;
        if (work->lifetime < 0) {
            effDelete(effect);
            return;
        }
        if (work->lifetime < 10) {
            work->alpha *= 0.8f;
        }
        work++;
        for (i = 1; i < effect->count; i++, work++) {
            work->velY += -0.2f;
            work->velX *= 0.9f;
            work->position.x += work->velX;
            work->position.y += work->velY;
            work->rotX += 10.0f;
            work->rotZ += 20.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effGarekiDisp, effect);
}

void effGarekiDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffGarekiN64Work* work;
    s32 i;
    s32 alpha;
    f32 rad;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx rotX;
    Mtx mtx;
    Mtx pieceMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.010416667f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(23, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    if (work->state == 0) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(trans, scale, mtx);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0x66, 0x6D, 0x7B, work->alpha});
        gareki_dl(0);
    } else {
        alpha = work->alpha;
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
        PSMTXConcat(trans, scale, mtx);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rotX, 'x', 0.017453292f * work->rotX);
        rad = 0.017453292f;
        i = 1;
        work++;
        for (; i < effect->count; i++, work++) {
            PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
            PSMTXConcat(trans, rotX, pieceMtx);
            PSMTXTrans(trans, work->offsetX, work->offsetY, 0.0f);
            PSMTXConcat(pieceMtx, trans, pieceMtx);
            PSMTXRotRad(rot, 'x', rad * work->rotX);
            PSMTXConcat(pieceMtx, rot, pieceMtx);
            PSMTXRotRad(rot, 'z', rad * work->rotZ);
            PSMTXConcat(pieceMtx, rot, pieceMtx);
            PSMTXConcat(mtx, pieceMtx, pieceMtx);
            PSMTXConcat(camera->view, pieceMtx, pieceMtx);
            GXLoadPosMtxImm(pieceMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXSetTevColor(GX_TEVREG0, (GXColor){0x96, 0x96, 0xC8, alpha});
            gareki_dl(i);
        }
    }
}

void gareki_dl(s32 type) {
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(stg__s_v);
    switch (type) {
        case 0:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(4, 5, 6, 0, 4, 6, 7, 0);
            break;
        case 1:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(8, 9, 10, 8, 11, 12, 13, 8);
            break;
        case 2:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(14, 15, 16, 14, 17, 18, 19, 14);
            break;
        case 3:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(20, 21, 22, 20, 20, 22, 23, 20);
            tri2(24, 25, 26, 20, 24, 26, 27, 20);
            break;
        case 4:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(28, 29, 30, 28, 31, 32, 33, 28);
            break;
        case 5:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(34, 35, 36, 34, 37, 38, 39, 34);
            break;
        case 6:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(40, 41, 42, 40, 40, 42, 43, 40);
            tri2(44, 45, 46, 40, 44, 46, 47, 40);
            break;
        case 7:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(48, 49, 50, 48, 51, 52, 53, 48);
            break;
        case 8:
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(54, 55, 56, 54, 57, 58, 59, 54);
            break;
    }
}
