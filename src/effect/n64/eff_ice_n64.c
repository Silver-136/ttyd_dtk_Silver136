// Paper Mario 64 effect: ice shards, eight ice pieces thrown up and spinning.

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

/**
 * One 0x30-byte record: element 0 is the effect's header (type, position, alpha, lifetime, frame), elements 1..8
 * are the shards (position relative to the header's, velocity, rotations). Same layout as Paper Mario 64's
 * BigSnowflakesFXData.
 */
typedef struct EffIceN64Work {
    s32 type;     // 0x0 header
    f32 x;        // 0x4
    f32 y;        // 0x8
    f32 z;        // 0xC
    f32 velX;     // 0x10 shard
    f32 velY;     // 0x14 shard
    f32 rotZ;     // 0x18 shard
    f32 rotVel;   // 0x1C shard
    f32 rotY;     // 0x20 shard
    s32 alpha;    // 0x24 header
    s32 lifetime; // 0x28 header
    s32 frame;    // 0x2C header
} EffIceN64Work;

/** Large 32x32 ice shard with texture 7, used by the even-numbered (spinning) shards. */
static effN64Vtx ice_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, 2048, 0, 0, 0, 0, 255},
    {16, 16, 0, 2048, 2048, 0, 0, 0, 255},
    {-16, 16, 0, 0, 2048, 0, 0, 0, 255},
};

/** Small 8x8 ice shard with texture 8, used by the odd-numbered (plain) shards. */
static effN64Vtx ice_s_vtx[] = {
    {-4, -4, 0, 0, 0, 0, 0, 0, 255},
    {4, -4, 0, 2048, 0, 0, 0, 0, 255},
    {4, 4, 0, 2048, 2048, 0, 0, 0, 255},
    {-4, 4, 0, 0, 2048, 0, 0, 0, 255},
};

static void effIceMain(EffectEntry* effect);
static void effIceDisp(CameraId cameraId, void* payload);

/**
 * Spawns eight ice shards at (x, y, z) that fly up and spin for 30 frames.
 * battle_damage.c spawns it on ice damage; also reachable through evt_eff64 (no script uses that).
 */
EffectEntry* effIceN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffIceN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "IceN64";
    effect->count = 9;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffIceN64Work) * 9);
    effect->userdata = work;
    effect->callback = effIceMain;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->lifetime = 30;
    work->frame = 0;
    work->alpha = 0;
    work++;
    for (i = 1; i < 9; i++, work++) {
        work->x = rand() % 10 - 5;
        work->y = rand() % 10 + 5;
        work->z = 10.0f;
        work->rotZ = rand() % 360;
        work->rotY = rand() % 360;
        work->rotVel = (rand() % 100) / 10;
        work->velX = 0.0f;
        work->velY = 2.0f;
    }
    return effect;
}

/** Fades the shards in, then out over the last 10 frames, and moves each shard with gravity, drag and spin. */
static void effIceMain(EffectEntry* effect) {
    EffIceN64Work* work = effect->userdata;
    Vec calcPos;
    Vec pos = {0.0f, 0.0f, 0.0f};
    f32 t;
    f32 u;
    f32 s;
    s32 i;
    s32 life;

    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
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
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        t = sin(6.2831855f * (2.0f * work->rotZ) / 360.0f);
        work->velY += -0.05f;
        {
            f32 drift = 0.2f * t;
            work->velX += drift;
        }
        work->velY += -0.05f;
        work->velX *= 0.92f;
        work->rotVel += (s32)effTblRandN64(50, life + i * 20) - 25;
        s = sin(6.2831855f * work->rotVel / 360.0f);
        work->rotZ += 10.0f * s;
        u = cos(6.2831855f * (work->rotVel / 2.0f) / 360.0f);
        work->rotY += 10.0f * u;
        work->x += work->velX;
        work->y += work->velY;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&calcPos), DISP_MODE_XLU, effIceDisp, effect);
}

/**
 * Draws the eight shards, alternating between a plain shard (texture 8, odd indices)
 * and a spinning one (texture 7, even indices).
 */
static void effIceDisp(CameraId cameraId, void* payload) {
    EffectEntry* effect = payload;
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
    PSMTXTrans(trans, work->x, work->y, work->z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x14, 0x64, 0x14, work->alpha});
    work++;
    for (i = 1; i < effect->count; i++, work++) {
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
            PSMTXTrans(trans, work->x, work->y, work->z);
        } else {
            PSMTXTrans(trans, work->x, work->y, work->z);
            PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
            PSMTXConcat(trans, rot, trans);
            PSMTXRotRad(rot, 'y', 0.017453292f * work->rotY);
            PSMTXConcat(trans, rot, trans);
        }
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
