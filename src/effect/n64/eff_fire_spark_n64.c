#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"


typedef struct EffFireSparkN64Work {
    s32 type;           // 0x0
    Vec position;       // 0x4
    f32 rangeX;         // 0x10
    f32 rangeY;         // 0x14
    s32 lifetime;       // 0x18
    s32 frame;          // 0x1c
    s32 r, g, b;        // 0x20
    s32 alpha;          // 0x2c
    s32 r2, g2, b2, a2; // 0x30
    f32 scale;          // 0x40
    Vec vel;            // 0x44
    Vec velTgt;         // 0x50
    f32 uv[2];          // 0x5c
    s32 index;          // 0x64
    f32 speedY;         // 0x68
    f32 speedZ;         // 0x6c
} EffFireSparkN64Work;

void effFireSparkMain(EffectEntry* effect);
void effFireSparkDisp(CameraId cameraId, EffectEntry* effect);

static f32 scale_data[30] = {
    0.01f, 0.05f, 0.1f, 0.2f, 0.35f, 0.5f, 0.65f, 0.8f, 0.95f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.95f, 0.8f, 0.65f, 0.5f, 0.35f, 0.2f, 0.1f, 0.05f, 0.01f,
};

effN64Vtx size4x4_tex32x32_vtx[] = {
    {-2, -2, 0, 0, 0, 0, 0, 0, 255},
    {2, -2, 0, 1024, 0, 0, 0, 0, 255},
    {2, 2, 0, 1024, 1024, 0, 0, 0, 255},
    {-2, 2, 0, 0, 1024, 0, 0, 0, 255},
};

EffectEntry* effFireSparkN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 rangeX, f32 rangeY,
                                  f32 scale, f32 speedY, f32 speedZ) {
    EffectEntry* effect;
    EffFireSparkN64Work* work;
    f32 h = 0.5f;
    f32 step;
    s32 i;

    effect = effEntry();
    effect->type = "FireSparkN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFireSparkN64Work) * count);
    effect->userdata = work;
    effect->callback = effFireSparkMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->rangeX = rangeX;
    work->rangeY = rangeY;
    work->speedY = speedY;
    work->speedZ = speedZ;
    work->r = 127;
    work->g = 0;
    work->b = 0;
    work->r2 = 127;
    work->g2 = 127;
    work->b2 = 0;
    work->a2 = 255;
    step = 30.0f / (f32)(count - 1);
    step = step * h;
    work++;
    for (i = 1; i < count; i++, work++) {
        work->index = (s32)(-(f32)i * step) - 1;
    }
    return effect;
}

void effFireSparkMain(EffectEntry* effect) {
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 halfX;
    f32 speedY;
    f32 speedZ;
    s32 rx;
    s32 ry;
    s32 frame;
    EffFireSparkN64Work* work;
    s32 i;

    work = effect->userdata;
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->frame > 324000) {
        work->frame = 256;
    }
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (work->lifetime < 16) {
        work->alpha = work->lifetime * 16;
    }
    if (frame < 16) {
        work->alpha = frame * 16 + 15;
    }
    rx = (s32)work->rangeX;
    halfX = 0.5f * work->rangeX;
    ry = (s32)work->rangeY;
    speedY = work->speedY;
    speedZ = work->speedZ;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->index++;
        if (work->index >= 30) {
            work->index = 0;
        }
        if (work->index < 0) {
            continue;
        }
        if (work->index == 0) {
            work->position.x = (f32)(rand() % rx) - halfX;
            work->position.y = (f32)(rand() % ry);
            work->position.z = 0.0f;
            work->vel.x = 0.05f * (f32)(rand() % 20 - 10);
            work->vel.y = 0.05f * (speedY * (f32)(rand() % 40 - 1));
            work->vel.z = 0.0f;
            work->velTgt.x = 0.05f * (f32)(rand() % 20 - 10);
            work->velTgt.y = 0.05f * (speedZ * (f32)(rand() % 30 - 1));
            work->velTgt.z = 0.0f;
            work->uv[0] = (f32)(rand() % 16);
            work->uv[1] = (f32)(rand() % 16);
        }
        work->scale = scale_data[work->index];
        work->position.x += work->vel.x;
        work->position.y += work->vel.y;
        work->position.z += work->vel.z;
        work->uv[0] += 0.1f;
        work->uv[1] += 0.3f;
        work->vel.x += 0.04f * (work->velTgt.x - work->vel.x);
        work->vel.y += 0.04f * (work->velTgt.y - work->vel.y);
        work->vel.z += 0.04f * (work->velTgt.z - work->vel.z);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFireSparkDisp, effect);
}

void effFireSparkDisp(CameraId cameraId, EffectEntry* effect) {
    s32 i;
    EffFireSparkN64Work* work;
    CameraEntry* camera;
    s32 alpha;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, tmp);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, work->a2});
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_KONST, GX_CC_C1, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0 + 0, GX_FALSE, GX_PTIDENTITY + 0x0);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0 + 3, GX_FALSE, GX_PTIDENTITY + 0x0);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){0x7F, 0x7F, 0x7F, 0});
    effGetTexObjN64(0x8b, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObjN64(0x65, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size4x4_tex32x32_vtx);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->index < 0) {
            continue;
        }
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXConcat(tmp, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXScale(scale, 0.03125f, 0.0078125f, 0.0f);
        PSMTXTrans(mtx, 0.0f, (f32)((i % 4) * 32), 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0 + 0, GX_MTX2x4);
        PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, work->uv[0], work->uv[1], 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0 + 3, GX_MTX2x4);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
