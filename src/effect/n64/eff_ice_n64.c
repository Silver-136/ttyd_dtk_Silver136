#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffIceN64Piece {
    s32 unk0;     // 0x0
    f32 x;        // 0x4
    f32 y;        // 0x8
    f32 z;        // 0xC
    f32 velX;     // 0x10
    f32 velY;     // 0x14
    f32 rotZ;     // 0x18
    f32 rotVel;   // 0x1C
    f32 rotY;     // 0x20
    f32 unk24;    // 0x24
    f32 unk28;    // 0x28
    f32 unk2C;    // 0x2C
} EffIceN64Piece;

typedef struct EffIceN64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    s32 pad10[5];   // 0x10
    s32 alpha;      // 0x24
    s32 lifetime;   // 0x28
    s32 frame;      // 0x2C
    EffIceN64Piece pieces[8]; // 0x30
} EffIceN64Work;

effN64Vtx ice_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, 2048, 0, 0, 0, 0, 255},
    {16, 16, 0, 2048, 2048, 0, 0, 0, 255},
    {-16, 16, 0, 0, 2048, 0, 0, 0, 255},
};

effN64Vtx ice_s_vtx[] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 2048, 0, 0, 0, 0, 255},
    {4, 4, 0, 2048, 2048, 0, 0, 0, 255},
    {-4, 4, 0, 0, 2048, 0, 0, 0, 255},
};

static void effIceMain(EffectEntry* effect);
static void effIceDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effIceN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffIceN64Work* work;
    EffIceN64Piece* p;
    s32 i;

    effect = effEntry();
    effect->type = "IceN64";
    effect->count = 9;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffIceN64Work));
    effect->userdata = work;
    effect->callback = effIceMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->lifetime = 30;
    work->frame = 0;
    work->alpha = 0;
    p = work->pieces;
    for (i = 1; i < 9; i++, p++) {
        p->x = rand() % 10 - 5;
        p->y = rand() % 10 + 5;
        p->z = 10.0f;
        p->rotZ = rand() % 360;
        p->rotY = rand() % 360;
        p->rotVel = (rand() % 100) / 10;
        p->velX = 0.0f;
        p->velY = 2.0f;
    }
    return effect;
}

static void effIceMain(EffectEntry* effect) {
    EffIceN64Work* work = effect->userdata;
    Vec calcPos;
    Vec pos = {0.0f, 0.0f, 0.0f};
    f32 t;
    f32 u;
    f32 s;
    s32 idx;
    s32 i;
    s32 life;
    EffIceN64Piece* p;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    calcPos = pos;

    work->lifetime--;
    work->frame++;
    life = work->lifetime;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    if (work->frame < 10) {
        work->alpha = work->alpha + 0.3f * (255 - work->alpha);
    }
    if (life < 10) {
        work->alpha *= 0.8f;
    }
    work = (EffIceN64Work*)((EffIceN64Piece*)work + 1);
    idx = life + 20;
    for (i = 1; i < effect->count; idx += 20, i++, work = (EffIceN64Work*)((EffIceN64Piece*)work + 1)) {
        p = (EffIceN64Piece*)work;
        t = sin(6.2831855f * (2.0f * p->rotZ) / 360.0f);
        p->velY += -0.05f;
        p->velX += (f32)(0.2f * t);
        p->velY += -0.05f;
        p->velX *= 0.92f;
        p->rotVel += (s32)effTblRandN64(50, idx) - 25;
        s = sin(6.2831855f * p->rotVel / 360.0f);
        p->rotZ += 10.0f * s;
        u = cos(6.2831855f * (p->rotVel / 2.0f) / 360.0f);
        p->rotY += 10.0f * u;
        p->x += p->velX;
        p->y += p->velY;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&calcPos), 2, (DispCallback)effIceDisp, effect);
}

static void effIceDisp(CameraId cameraId, EffectEntry* effect) {
    EffIceN64Work* work;
    CameraEntry* cam;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    s32 i;
    effN64Vtx* vtx;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x14, 0x64, 0x14, work->alpha});
    work = (EffIceN64Work*)((EffIceN64Piece*)work + 1);
    for (i = 1; i < effect->count; i++, work = (EffIceN64Work*)((EffIceN64Piece*)work + 1)) {
        if (i & 1) {
            vtx = ice_s_vtx;
            effGetTexObjN64(8, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        } else {
            vtx = ice_vtx;
            effGetTexObjN64(7, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        }
        effSetVtxDescN64(vtx);
        if (i & 1) {
            PSMTXTrans(trans, ((EffIceN64Piece*)work)->x, ((EffIceN64Piece*)work)->y, ((EffIceN64Piece*)work)->z);
        } else {
            PSMTXTrans(trans, ((EffIceN64Piece*)work)->x, ((EffIceN64Piece*)work)->y, ((EffIceN64Piece*)work)->z);
            PSMTXRotRad(rot, 'z', 0.017453292f * ((EffIceN64Piece*)work)->rotZ);
            PSMTXConcat(trans, rot, trans);
            PSMTXRotRad(rot, 'y', 0.017453292f * ((EffIceN64Piece*)work)->rotY);
            PSMTXConcat(trans, rot, trans);
        }
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
