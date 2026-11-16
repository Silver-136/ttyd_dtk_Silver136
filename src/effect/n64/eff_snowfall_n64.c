// Paper Mario 64 effect: snowfall: `count` snow particles drifting down the screen.

#include "version.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif

#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include <string.h>
#include "driver/mapdrv.h"

// One record type for the whole userdata array, as in Paper Mario 64: entry 0 is the header, the particles follow.
typedef struct EffSnowfallWork {
    s32 active;   // 0x0, header: draw switch (set to 1 at entry)
    s32 arg;      // 0x4, header: the entry's first argument, never read
    Vec position; // 0x8, particle position (the header's is only read, for the draw order)
    Vec velocity; // 0x14, particles
    s32 life;     // 0x20, header
    s32 frame;    // 0x24, header
    s32 alpha;    // 0x28, header: overall alpha; particles: 0 once hidden
    s32 count;    // 0x2C, header: number of particles
    s32 timer;    // 0x30, particles: start delay
} EffSnowfallWork;

static void effSnowfallMain(EffectEntry* effect);
static void effSnowfallDisp(CameraId cameraId, void* param);

/**
 * A loose cluster of eight small flake quads drawn for every snowfall particle,
 * so each one reads as several flakes.
 */
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

/** Places a particle at random within 1000 units of the camera with a random downward drift (halved). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void _init_proc(EffSnowfallWork* p) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);

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
}

/**
 * Wraps a particle back into the box of half-width w and half-height h around a centre taken from the camera
 * (target x, the mean of target and eye y, and (target z + viewDistance) / 2 for z);
 * below the box it either wraps to the top or, when life < 100, stops and hides the particle.
 */
static void cam_clip_chk(EffSnowfallWork* p, s32 w, s32 h, s32 life) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 cx, cy, cz, py, pz;
    s32 d;

    cx = cam->cameraAt.x;
    cy = (cam->cameraAt.y + cam->cameraPos.y) / 2.0f;

    // BUG: averages cameraAt.z with viewDistance (a length, not a coordinate) where y averages cameraAt and
    // cameraPos, so the box centre z shifts with the camera distance instead of lying between target and eye.
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

/**
 * Spawns `count` falling snow particles around the camera.
 * Used by the Fahr Outpost battle stage (bom) and event scripts.
 */
EffectEntry* effSnowfallN64Entry(s32 arg, s32 count) {
    EffectEntry* effect;
    EffSnowfallWork* work;
    s32 i, n;

    effect = effEntry();
    n = count + 1;
    effect->type = "SnowfallN64";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, n * sizeof(EffSnowfallWork));
    effect->userdata = work;
    effect->callback = effSnowfallMain;
    work->frame = 0;
    work->life = 100;
    work->arg = arg;
    work->count = count;
    work->alpha = 255;
    work->active = 1;
    work++;
    for (i = 0; i < count; i++, work++) {
        _init_proc(work);
        work->timer = 0;
    }
    return effect;
}

/**
 * Moves the particles by their velocity (after each one's start delay)
 * and wraps them around the camera with cam_clip_chk;
 * the life only counts down from below 100, fading alpha over the last 10 frames.
 * US and later draw on layer 10 (on CAMERA_3D_EFFECTB in battle), JP on layer 10, the early demos on layer 2.
 */
static void effSnowfallMain(EffectEntry* effect) {
    s32 i, life, count, w, h;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    EffSnowfallWork* work;

    work = effect->userdata;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    camGetPtr(CAMERA_3D);
    if (work->life < 100) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    work->frame++;
    life = work->life;
    count = work->count;
    if (life < 10) {
        work->alpha = life * 25;
    }

    // half-width of the wrap box and the half-height derived from it (166, converted at run time)
    w = 550;
    h = w * 0.3f + 1.0f;
    work++;
    for (i = 0; i < count; i++, work++) {
        if (work->timer > 0) {
            work->timer--;
            if (work->timer > 0) {
                continue;
            }
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        cam_clip_chk(work, w, h, life);
    }
#if VERSION <= VERSION_DEMO_JP
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, effSnowfallDisp, effect);
#elif VERSION >= VERSION_US
    if (gp->inBattle == 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU_AFTER, effSnowfallDisp, effect);
    } else {
        dispEntry(CAMERA_3D_EFFECTB, dispCalcZ(&position), DISP_MODE_XLU_AFTER, effSnowfallDisp, effect);
    }
#else
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU_AFTER, effSnowfallDisp, effect);
#endif
}

/**
 * Draws each visible particle as a camera-facing cluster of small flake quads (eff_snowfall_v) with texture 99,
 * after copying texture 100's 16x16 image into 99's mip chain each frame.
 * White, or the map's blend colour when the first map entry has blending on.
 */
static void effSnowfallDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    CameraEntry* cam;
    EffSnowfallWork* work;
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
    work = effect->userdata;
    if (work->active == 0) {
        return;
    }
    count = work->count;
    alpha = work->alpha;
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

    effSetVtxDescN64(eff_snowfall_v);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(cam->view, rot, rot);
    work++;
    for (i = 0; i < count; i++, work++) {
        if (work->timer > 0 || work->alpha == 0) {
            continue;
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
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
