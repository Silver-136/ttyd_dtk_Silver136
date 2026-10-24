#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

typedef struct EffRubbleN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 groundY;  // 0x10
    f32 velX;     // 0x14
    f32 velY;     // 0x18
    f32 velZ;     // 0x1C
    f32 gravity;  // 0x20
    f32 rotX;     // 0x24
    f32 rotZ;     // 0x28
    f32 rotY;     // 0x2C
    f32 rotVelX;  // 0x30
    f32 rotVelZ;  // 0x34
    f32 rotVelY;  // 0x38
    f32 scale;    // 0x3C
    s32 unk40;    // 0x40
    s32 alpha;    // 0x44
    s32 lifetime; // 0x48
    s32 frame;    // 0x4C
} EffRubbleN64Work;

EffectEntry* effHokoriN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z);

effN64Vtx stg_hahen_v[58] = {
    {3, 3, -3, 0, 0, 50, 50, 50, 255},
    {-3, -3, -3, -579, 0, 50, 50, 50, 255},
    {-3, 3, 3, -290, -502, 50, 50, 50, 255},
    {3, 3, 3, 106, -396, 50, 50, 50, 255},
    {3, 3, -3, 0, 0, 50, 50, 50, 255},
    {3, -3, -3, -1024, 0, 224, 224, 224, 255},
    {3, -3, 3, -1024, -410, 224, 224, 224, 255},
    {-3, -3, 3, -614, -410, 224, 224, 224, 255},
    {-3, -3, -3, -614, 0, 224, 224, 224, 255},
    {3, 3, -3, -869, 0, 50, 50, 50, 255},
    {3, -3, -3, -1159, 290, 50, 50, 50, 255},
    {-3, -3, -3, -1448, 0, 50, 50, 50, 255},
    {3, 3, -3, -614, -1434, 160, 160, 160, 255},
    {3, 3, 3, -614, -1024, 160, 160, 160, 255},
    {3, -3, 3, -1024, -1024, 160, 160, 160, 255},
    {3, -3, -3, -1024, -1434, 160, 160, 160, 255},
    {-3, -3, -3, -1014, -1756, 50, 50, 50, 255},
    {-3, -3, 3, -618, -1650, 50, 50, 50, 255},
    {-3, 3, 3, -724, -1254, 50, 50, 50, 255},
    {-3, 3, 3, 0, -1434, 200, 200, 200, 255},
    {-3, -3, 3, 0, -1024, 200, 200, 200, 255},
    {3, -3, 3, -410, -1024, 200, 200, 200, 255},
    {3, 3, 3, -410, -1434, 200, 200, 200, 255},
    {-4, 0, -4, 546, 478, 255, 255, 255, 255},
    {-4, 0, 4, 546, 1024, 255, 255, 255, 255},
    {-4, 1, -1, 546, 700, 255, 255, 255, 255},
    {4, 0, 1, 0, 819, 255, 255, 255, 255},
    {4, 0, -4, 0, 478, 255, 255, 255, 255},
    {2, 3, 2, -277, -317, 50, 50, 50, 255},
    {-3, 0, -5, 0, 0, 50, 50, 50, 255},
    {-5, 0, 3, -384, 0, 50, 50, 50, 255},
    {2, 3, 2, 0, 0, 50, 50, 50, 255},
    {-5, 0, 3, -335, 0, 50, 50, 50, 255},
    {2, 0, 5, -51, -184, 142, 142, 142, 255},
    {2, 3, 2, -661, -501, 50, 50, 50, 255},
    {2, 0, 5, -519, -627, 142, 142, 142, 255},
    {4, 0, -1, -494, -334, 140, 140, 140, 255},
    {2, 3, 2, -829, 0, 50, 50, 50, 255},
    {4, 0, -1, -661, -167, 140, 140, 140, 255},
    {0, 0, -6, -415, 0, 50, 50, 50, 255},
    {2, 3, 2, -829, 0, 50, 50, 50, 255},
    {0, 0, -6, -415, 0, 50, 50, 50, 255},
    {-3, 0, -5, -434, 146, 50, 50, 50, 255},
    {0, 0, -6, 0, 0, 120, 120, 120, 255},
    {4, 0, -1, -247, -167, 234, 234, 234, 255},
    {-1, -3, -1, -23, -282, 140, 140, 140, 255},
    {4, 0, -1, 0, 0, 234, 234, 234, 255},
    {2, 0, 5, -294, 0, 220, 220, 220, 255},
    {-1, -3, -1, -77, -240, 140, 140, 140, 255},
    {2, 0, 5, -324, 0, 220, 220, 220, 255},
    {-5, 0, 3, -129, -278, 62, 62, 62, 255},
    {-1, -3, -1, 0, 0, 140, 140, 140, 255},
    {-5, 0, 3, -234, -304, 62, 62, 62, 255},
    {-3, 0, -5, 0, 0, 50, 50, 50, 255},
    {-1, -3, -1, -269, 0, 140, 140, 140, 255},
    {-3, 0, -5, -52, -138, 50, 50, 50, 255},
    {0, 0, -6, 0, 0, 120, 120, 120, 255},
    {-1, -3, -1, -282, 0, 140, 140, 140, 255},
};

void effRubbleMain(EffectEntry* effect);
void effRubbleDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRubbleN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 groundY, f32 velY, f32 gravity, f32 scale) {
    EffectEntry* effect;
    EffRubbleN64Work* work;

    effect = effEntry();
    effect->type = "RubbleN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRubbleN64Work));
    effect->userdata = work;
    effect->callback = effRubbleMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->groundY = groundY;
    work->velY = velY;
    work->gravity = gravity;
    work->lifetime = lifetime;
    work->frame = 0;
    work->velX = 0.0f;
    work->velZ = 0.0f;
    work->rotX = 0.0f;
    work->rotZ = rand() % 360;
    work->rotY = rand() % 360;
    work->rotVelX = 0.0f;
    work->rotVelY = 20.0f;
    work->rotVelZ = -20.0f;
    work->alpha = 255;
    work->scale = 2.0f * scale;
    work->unk40 = 0;
    return effect;
}

void effRubbleMain(EffectEntry* effect) {
    EffRubbleN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 lifetime;

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
    if (work->position.y != work->groundY) {
        work->velY += work->gravity;
        work->position.y += work->velY;
        work->rotX += work->rotVelX;
        work->rotY += work->rotVelY;
        work->rotZ += work->rotVelZ;
        if (work->position.y <= work->groundY) {
            work->position.y = work->groundY;
            effHokoriN64Entry(2, 20, work->position.x, work->position.y, work->position.z);
            work->lifetime = 30;
        }
    }
    if (lifetime < 10) {
        work->alpha *= 0.8f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effRubbleDisp, effect);
}

void effRubbleDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffRubbleN64Work* work;
    s32 alpha;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'x', 0.017453292f * work->rotX);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0xD7, 0xD7, 0x6E, alpha});
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetCullMode(GX_CULL_NONE);
    switch (work->type) {
        case 0:
        case 3:
            effSetVtxDescN64(&stg_hahen_v[28]);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 30);
            tri2(0, 1, 2, 0, 3, 4, 5, 0);
            tri2(6, 7, 8, 0, 9, 10, 11, 0);
            tri2(12, 13, 14, 0, 15, 16, 17, 0);
            tri2(18, 19, 20, 0, 21, 22, 23, 0);
            tri2(24, 25, 26, 0, 27, 28, 29, 0);
            break;
        case 1:
        case 4:
            effSetVtxDescN64(&stg_hahen_v[23]);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 18);
            tri2(0, 1, 2, 0, 1, 3, 2, 0);
            tri2(1, 0, 4, 0, 1, 4, 3, 0);
            tri2(4, 0, 2, 0, 3, 4, 2, 0);
            break;
        case 2:
        case 5:
            effSetVtxDescN64(stg_hahen_v);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 30);
            tri2(0, 1, 2, 0, 2, 3, 4, 0);
            tri2(5, 6, 7, 0, 5, 7, 8, 0);
            tri2(9, 10, 11, 0, 12, 13, 14, 0);
            tri2(12, 14, 15, 0, 16, 17, 18, 0);
            tri2(19, 20, 21, 0, 19, 21, 22, 0);
            break;
    }
}
