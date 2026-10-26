#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "eff/n64/eff_fall_leaf_n64.h"
// US effect texture ids are 0x1C lower for these entries.
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))



typedef struct EffFallLeafN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 vx;        // 0x10
    f32 vy;        // 0x14
    f32 angleZ;    // 0x18
    f32 spin;      // 0x1C
    f32 angleY;    // 0x20
    s32 alpha;     // 0x24
    s32 lifetime;  // 0x28
    s32 frame;     // 0x2C
} EffFallLeafN64Work;

static effN64Vtx size12x12_tex64x64_vtx[] = {
    {-6, -6, 0, 0, 0, 0x19, 0x55, 0x11, 0xFF},
    {6, -6, 0, 2048, 0, 0x19, 0x55, 0x11, 0xFF},
    {6, 6, 0, 2048, 2048, 0x68, 0xAC, 0x10, 0xFF},
    {-6, 6, 0, 0, 2048, 0x68, 0xAC, 0x10, 0xFF},
    {-6, -6, 0, 0, 2048, 0x18, 0x37, 0x21, 0xFF},
    {6, -6, 0, 2048, 2048, 0x18, 0x37, 0x21, 0xFF},
    {6, 6, 0, 2048, 4096, 0x30, 0x6C, 0x25, 0xFF},
    {-6, 6, 0, 0, 4096, 0x30, 0x6C, 0x25, 0xFF},
};


void effFallLeafMain(EffectEntry* effect);
void effFallLeafDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effFallLeafN64Entry(s32 type, f32 x, f32 y, f32 z, s32 lifetime) {
    EffectEntry* effect;
    EffFallLeafN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "FallLeafN64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFallLeafN64Work) * 5);
    effect->userdata = work;
    effect->callback = effFallLeafMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = lifetime;
    work->frame = 0;
    work->alpha = 0;
    work++;
    for (i = 1; i < 5; i++, work++) {
        switch (type) {
            case 0:
            case 2:
            case 3:
            case 4:
                work->position.x = rand() % 50 - 25;
                work->position.y = rand() % 50 - 25;
                work->position.z = 0.0f;
                work->angleZ = rand() % 360;
                work->angleY = rand() % 360;
                work->spin = rand() % 100 / 10.0f;
                work->vx = 0.0f;
                work->vy = 0.0f;
                break;
            case 1:
                work->position.x = rand() % 10 - 5;
                work->position.y = rand() % 10 + 5;
                work->position.z = 10.0f;
                work->angleZ = rand() % 360;
                work->angleY = rand() % 360;
                work->spin = rand() % 100 / 10.0f;
                work->vx = 0.0f;
                work->vy = 2.0f;
                break;
        }
    }
    return effect;
}

void effFallLeafMain(EffectEntry* effect) {
    EffFallLeafN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    f32 c;
    s32 lifetime;
    s32 frame;
    f32 s;
    f32 s2;
    f32 dx;

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
    frame = work->frame;
    type = work->type;
    if (frame < 10) {
        work->alpha = work->alpha + 0.3f * (255 - work->alpha);
    }
    if (lifetime < 10) {
        work->alpha *= 0.8f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        s = (f32)sin(6.2831855f * (2.0f * work->angleZ) / 360.0f);
        dx = 0.2f * s;
        work->vy += -0.05f;
        work->vx += dx;
        switch (type) {
            case 0:
            case 2:
            case 3:
            case 4:
                work->vx *= 0.94f;
                s2 = (f32)sin(6.2831855f * work->angleZ / 360.0f);
                work->vy *= 0.05f * s2 + 0.95f;
                break;
            case 1:
                work->vy += -0.05f;
                work->vx *= 0.92f;
                break;
        }
        work->spin += (f32)((s32)effTblRandN64(50, frame + i * 20) - 25);
        work->angleZ += 10.0f * (f32)sin(6.2831855f * work->spin / 360.0f);
        c = (f32)cos(6.2831855f * (work->spin / 2.0f) / 360.0f);
        work->angleY += 10.0f * c;
        work->position.x += work->vx;
        work->position.y += work->vy;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFallLeafDisp, effect);
}

void effFallLeafDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFallLeafN64Work* work;
    s32 type;
    s32 i;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, viewMtx);
    PSMTXConcat(camera->view, viewMtx, viewMtx);
    switch (type) {
        case 0:
        case 1:
            GXSetTevColor(GX_TEVREG0, (GXColor){20, 100, 20, work->alpha});
            break;
        case 2:
            GXSetTevColor(GX_TEVREG0, (GXColor){50, 145, 60, work->alpha});
            break;
        case 3:
            GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, work->alpha});
            break;
        case 4:
            GXSetTevColor(GX_TEVREG0, (GXColor){117, 146, 166, work->alpha});
            break;
    }
    work++;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    switch (type) {
        case 0:
        case 1:
            PSMTXScale(scale, 0.03125f, 0.0078125f, 1.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            effGetTexObjN64(0x14, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            break;
        default:
            PSMTXScale(scale, 0.03125f, 0.015625f, 1.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            effGetTexObj(EFF_TEX(0x87), &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            break;
    }
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size12x12_tex64x64_vtx);
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->angleZ);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->angleY);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(6, 5, 4, 4, 7, 6, 4, 4);
    }
}
