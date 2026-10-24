#include "eff/n64/eff_akari_charge_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>

static effN64Vtx size2x8_tex8x16_vtx[] = {
    {-4, -4, 0, 0, 0, 255, 255, 255, 255},
    {4, -4, 0, 512, 0, 0, 0, 0, 255},
    {4, 20, 0, 512, 1024, 0, 0, 0, 255},
    {-4, 20, 0, 0, 1024, 255, 0, 0, 255},
};

void effAkariChargeMain(EffectEntry* effect);
void effAkariChargeDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effAkariChargeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 size) {
    EffectEntry* effect;
    EffAkariChargeN64Work* work;
    EffAkariChargeN64Elem* p;
    s32 i;

    effect = effEntry();
    effect->type = "AkariChargeN64";
    effect->count = 22;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffAkariChargeN64Work));
    effect->userdata = work;
    effect->callback = effAkariChargeMain;
    effect->flags |= 2;
    work->elem[0].type = type;
    work->elem[0].frame = 0;
    if (lifetime <= 0) {
        work->elem[0].lifetime = 1000;
    } else {
        work->elem[0].lifetime = lifetime;
    }
    work->elem[0].alpha = 0;
    work->elem[0].position.x = x;
    work->elem[0].position.y = y;
    work->elem[0].position.z = z;
    work->elem[0].size = size;
    work->elem[0].r = 255;
    work->elem[0].g = 230;
    work->elem[0].b = 0;
    work->elem[0].r2 = 255;
    work->elem[0].g2 = 255;
    work->elem[0].b2 = 255;
    p = &work->elem[1];
    for (i = 1; i < 22; i++, p++) {
        p->lifetime = 20;
        p->frame = 0;
        p->rotation = (f32)((i - 1) * 360 / 21);
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->scale = 1.0f;
        p->index = i & 15;
    }
    return effect;
}

void effAkariChargeMain(EffectEntry* effect) {
    EffAkariChargeN64Elem* p = effect->userdata;
    s32 i;
    s32 type;
    f32 sv;
    s32 lifetime;
    s32 frame;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = p->position.x;
    tmp.y = p->position.y;
    tmp.z = p->position.z;
    position = tmp;
    type = p->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        p->lifetime = 16;
    }
    if (p->lifetime < 1000) {
        p->lifetime--;
    }
    p->frame++;
    lifetime = p->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = p->frame;
    if (lifetime < 16) {
        p->alpha = lifetime * 16;
    }
    if (frame < 16) {
        p->alpha = frame * 16 + 15;
    }
    p++;
    for (i = 1; i < effect->count; i += 3, p += 3) {
        if (p->frame >= p->lifetime) {
            p->lifetime = rand() % 50 + 20;
            p->frame = 0;
            p->rotation = (f32)(rand() % 360);
            p->position.x = 0.0f;
            p->position.y = 0.0f;
        }
        if (rand() % 100 <= 50) {
            f32 ox = p->position.x;
            f32 oy = p->position.y;
            f32 dx = (f32)(rand() % 17 - 8);
            f32 dy = (f32)(rand() % 17 - 8);
            p[2] = p[1];
            p[1] = p[0];
            p->position.x = ox + dx;
            p->position.y = oy + dy;
            p->rotation = angleABf(0.0f, 0.0f, dx, dy);
            if (dx || dy) {
                p->scale = sqrtf(dx * dx + dy * dy);
            } else {
                p->scale = 0.0f;
            }
        }
        sv = (f32)sin(6.2831855f * (f32)(p->frame * 180 / p->lifetime) / 360.0f);
        p->alpha = (s32)(1024.0f * sv);
        if (p->alpha > 255) {
            p->alpha = 255;
        }
        p->frame++;
    }
    p = effect->userdata;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        p->index = rand() % 16;
    }
    if (type == 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effAkariChargeDisp, effect);
    } else {
        dispEntry(CAMERA_2D, dispCalcZ(&position), 2, (DispCallback)effAkariChargeDisp, effect);
    }
}

void effAkariChargeDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffAkariChargeN64Elem* p;
    s32 i;
    s32 alpha;
    s32 type;
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    GXTexObj texObj;
    f32 proj[7];
    f32 viewport[6];
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;

    camera = camGetPtr(CAMERA_3D);
    p = effect->userdata;
    type = p->type;
    frame = p->frame;
    alpha = p->alpha;
    r = p->r;
    g = p->g;
    b = p->b;
    if (type == 1) {
        GXGetProjectionv(proj);
        GXGetViewportv(viewport);
        GXSetProjection(camera->projection, camera->projType);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    PSMTXTrans(mtx, p->position.x, p->position.y, p->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, p->size, p->size, p->size);
    PSMTXConcat(mtx, scale, viewMtx);
    PSMTXConcat(camera->view, viewMtx, viewMtx);
    GXSetTevColor(GX_TEVREG1, (GXColor){p->r2, p->g2, p->b2, 0});
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    effGetTexObjN64(0x70, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size2x8_tex8x16_vtx);
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, p->alpha * alpha / 255});
        PSMTXTrans(mtx, p->position.x, p->position.y, 0.0f);
        PSMTXRotRad(rot, 'z', 0.017453292f * p->rotation);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(scale, p->scale / 16.0f, p->scale / 16.0f, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(p->index * 16), 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(frame * 3), 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
    if (type == 1) {
        GXSetProjectionv(proj);
        GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    }
}
