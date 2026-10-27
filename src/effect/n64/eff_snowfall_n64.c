#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include <string.h>
#include "driver/mapdrv.h"

typedef struct EffSnowfallParticle {
    u8 pad0[8];   // 0x0
    Vec position; // 0x8
    Vec velocity; // 0x14
    u8 pad20[8];  // 0x20
    s32 alpha;    // 0x28
    s32 pad2c;    // 0x2C
    s32 timer;    // 0x30
} EffSnowfallParticle;

typedef struct EffSnowfallWork {
    s32 active;                // 0x0
    s32 arg;                   // 0x4
    Vec position;              // 0x8
    u8 pad14[0xC];             // 0x14
    s32 life;                  // 0x20
    s32 frame;                 // 0x24
    s32 alpha;                 // 0x28
    s32 count;                 // 0x2C
    s32 pad30;                 // 0x30
    EffSnowfallParticle p[1];  // 0x34
} EffSnowfallWork;

void effSnowfallMain(EffectEntry* effect);
void effSnowfallDisp(CameraId cameraId, EffectEntry* effect);

static effN64Vtx eff_snowfall_v[] = {
    {-78, 3, -38, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-68, 3, -38, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-68, 12, -38, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-78, 12, -38, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-33, -33, -62, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-23, -33, -62, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-23, -24, -62, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-33, -24, -62, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-30, 39, -14, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-20, 39, -14, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-20, 48, -14, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-30, 48, -14, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-13, -66, -38, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-3, -66, -38, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {-3, -57, -38, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-13, -57, -38, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-5, -4, 0, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, -4, 0, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, 5, 0, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-5, 5, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {76, -13, -46, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {86, -13, -46, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {86, -4, -46, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {76, -4, -46, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {45, 37, -13, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {55, 37, -13, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {55, 46, -13, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {45, 46, -13, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {27, -53, -57, 0, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {37, -53, -57, 2048, 2048, 0xFF, 0xFF, 0xFF, 0xFF},
    {37, -44, -57, 2048, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {27, -44, -57, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
};

void cam_clip_chk(EffSnowfallParticle* p, s32 w, s32 h, s32 life) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 cx, cy, cz, py, pz;
    s32 d;

    cx = cam->cameraAt.x;
    cy = (cam->cameraAt.y + cam->cameraPos.y) / 2.0f;
    cz = (cam->cameraAt.z + cam->viewDistance) / 2.0f;
    d = (s32)(p->position.x - cx);
    py = p->position.y;
    pz = p->position.z;
    if (d > w) {
        d %= w;
        p->position.x = (cx - w) + d;
    } else if (d < -w) {
        d = -d;
        d %= w;
        p->position.x = (cx + w) - d;
    }
    d = (s32)(py - cy);
    if (d > h) {
        d %= h;
        p->position.y = (cy - h) + d;
    } else if (d < -h) {
        if (life < 100) {
            p->velocity.y = 0.0f;
            p->alpha = 0;
        } else {
            d = -d;
            d %= h;
            p->position.y = (cy + h) - d;
        }
    }
    d = (s32)(pz - cz);
    if (d > w) {
        d %= w;
        p->position.z = (cz - w) + d;
    } else if (d < -w) {
        d = -d;
        d %= w;
        p->position.z = (cz + w) - d;
    }
}

EffectEntry* effSnowfallN64Entry(s32 arg, s32 count) {
    EffectEntry* effect;
    EffSnowfallWork* work;
    EffSnowfallParticle* p;
    CameraEntry* cam;
    s32 i, n;

    effect = effEntry();
    n = count + 1;
    effect->type = "SnowfallN64";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, n * sizeof(EffSnowfallParticle));
    effect->userdata = work;
    effect->callback = effSnowfallMain;
    work->frame = 0;
    work->life = 100;
    work->arg = arg;
    work->count = count;
    work->alpha = 255;
    work->active = 1;
    p = work->p;
    for (i = 0; i < count; i++, p++) {
        cam = camGetPtr(CAMERA_3D);
        p->position.x = cam->cameraPos.x + (rand() % 2000) - 1000.0f;
        p->position.y = cam->cameraPos.y + (rand() % 2000) - 1000.0f;
        p->position.z = cam->cameraPos.z + (rand() % 2000) - 1000.0f;
        p->velocity.x = 0.05f * ((rand() % 20) - 10.0f);
        p->velocity.y = -1.2f - 0.01f * (rand() % 80);
        p->velocity.z = 0.05f * ((rand() % 20) - 10.0f);
        p->alpha = 255;
        p->velocity.x *= 0.5f;
        p->velocity.y *= 0.5f;
        p->velocity.z *= 0.5f;
        p->timer = 0;
    }
    return effect;
}

// The first userdata entry holds the EffSnowfallWork header; the particles follow it.
#define SNOWFALL_WORK ((EffSnowfallWork*)p)
void effSnowfallMain(EffectEntry* effect) {
    s32 i, life, count, h;
    f32 hf;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    EffSnowfallParticle* p;

    p = effect->userdata;

    tmp.x = SNOWFALL_WORK->position.x;
    tmp.y = SNOWFALL_WORK->position.y;
    tmp.z = SNOWFALL_WORK->position.z;
    position = tmp;
    camGetPtr(CAMERA_3D);
    if (SNOWFALL_WORK->life < 100) {
        SNOWFALL_WORK->life--;
    }
    if (SNOWFALL_WORK->life < 0) {
        effDelete(effect);
        return;
    }
    SNOWFALL_WORK->frame++;
    life = SNOWFALL_WORK->life;
    count = SNOWFALL_WORK->count;
    if (life < 10) {
        SNOWFALL_WORK->alpha = life * 25;
    }
    // FAKE: the height goes through a float local (converted at run time); h = 166 changes the object
    hf = 166.0f;
    h = hf;
    p++;
    for (i = 0; i < count; i++, p++) {
        if (p->timer > 0) {
            p->timer--;
            if (p->timer > 0) {
                continue;
            }
        }
        p->position.x += p->velocity.x;
        p->position.y += p->velocity.y;
        p->position.z += p->velocity.z;
        cam_clip_chk(p, 550, h, life);
    }
#ifdef VERSION_G8ME01
    if (gp->inBattle == 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 10, (DispCallback)effSnowfallDisp, effect);
    } else {
        dispEntry(CAMERA_3D_EFFECTB, dispCalcZ(&position), 10, (DispCallback)effSnowfallDisp, effect);
    }
#else
    dispEntry(CAMERA_3D, dispCalcZ(&position), 10, (DispCallback)effSnowfallDisp, effect);
#endif
}

void effSnowfallDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* cam;
    EffSnowfallParticle* p;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    GXTexObj texA;
    GXTexObj texB;
    GXColor blend;
    s32 count;
    u8* dataA;
    s32 i;
    s32 alpha;
    u8* dataB;
    int j;

    cam = camGetPtr(cameraId);
    p = effect->userdata;
    if (SNOWFALL_WORK->active == 0) {
        return;
    }
    count = SNOWFALL_WORK->count;
    alpha = SNOWFALL_WORK->alpha;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    effGetTexObjN64(99, &texA);
    effGetTexObjN64(100, &texB);
    dataA = (u8*)GXGetTexObjData(&texA) + OS_BASE_CACHED; // physical -> cached
    dataB = (u8*)GXGetTexObjData(&texB) + OS_BASE_CACHED;
    dataA += GXGetTexBufferSize(64, 64, GX_TF_I4, GX_TRUE, 2);
    memcpy(dataA, dataB, GXGetTexBufferSize(16, 16, GX_TF_I4, GX_FALSE, 0));
    DCFlushRange(dataA, GXGetTexBufferSize(64, 64, GX_TF_I4, GX_TRUE, 3));
    GXInitTexObjLOD(&texA, GX_LIN_MIP_LIN, GX_LINEAR, 0.0f, 2.0f, 0.0f, GX_TRUE, GX_TRUE, GX_ANISO_1);
    GXLoadTexObj(&texA, GX_TEXMAP0);
    if (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND) {
        mapGetBlend(&blend);
        GXSetTevColor(GX_TEVREG0, (GXColor){blend.r * 0xFF, blend.g * 0xFF, blend.b * 0xFF, alpha * blend.a});
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, alpha});
    }
    GXSetCullMode(GX_CULL_NONE);
    // FAKE: discarded compound literal with no effect; removing it changes the object
    (void)(GXColor){0, 0, 0, alpha};
    effSetVtxDescN64(eff_snowfall_v);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(cam->view, rot, rot);
    p++;
    for (i = 0; i < count; i++, p++) {
        if (p->timer > 0 || p->alpha == 0) {
            continue;
        }
        PSMTXTrans(trans, p->position.x, p->position.y, p->position.z);
        PSMTXConcat(rot, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 32);
        for (j = 0; j < 32; j++) {
            GXPosition1x16(j);
            GXPosition1x16(j);
            GXPosition1x16(j);
        }
    }
}
#undef SNOWFALL_WORK
