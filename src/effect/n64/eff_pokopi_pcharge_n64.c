#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"

__declspec(section ".data") f32 lbl_pcharge_0[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 fd_pcharge_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 lbl_pcharge_1[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 fd_pcharge_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 lbl_pcharge_2[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 fd_pcharge_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 lbl_pcharge_3[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 fd_pcharge_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

static effN64Vtx size10x10_tex32x32_vtx[] = {
    {0, -5, 0, 0, 0, 0, 0, 0, 255},
    {10, -5, 0, 1024, 0, 0, 0, 0, 255},
    {10, 5, 0, 1024, 1024, 0, 0, 0, 255},
    {0, 5, 0, 0, 1024, 0, 0, 0, 255},
    {0, -5, 0, 0, 1024, 0, 0, 0, 255},
    {10, -5, 0, 1024, 1024, 0, 0, 0, 255},
    {10, 5, 0, 1024, 2048, 0, 0, 0, 255},
    {0, 5, 0, 0, 2048, 0, 0, 0, 255},
    {0, -5, 0, 0, 2048, 0, 0, 0, 255},
    {10, -5, 0, 1024, 2048, 0, 0, 0, 255},
    {10, 5, 0, 1024, 3072, 0, 0, 0, 255},
    {0, 5, 0, 0, 3072, 0, 0, 0, 255},
    {0, -5, 0, 0, 3072, 0, 0, 0, 255},
    {10, -5, 0, 1024, 3072, 0, 0, 0, 255},
    {10, 5, 0, 1024, 4096, 0, 0, 0, 255},
    {0, 5, 0, 0, 4096, 0, 0, 0, 255},
};

static effN64Vtx size10x10_tex32x32_2_vtx[] = {
    {0, -5, 0, 1024, 0, 0, 0, 0, 255},
    {10, -5, 0, 0, 0, 0, 0, 0, 255},
    {10, 5, 0, 0, 1024, 0, 0, 0, 255},
    {0, 5, 0, 1024, 1024, 0, 0, 0, 255},
    {0, -5, 0, 1024, 1024, 0, 0, 0, 255},
    {10, -5, 0, 0, 1024, 0, 0, 0, 255},
    {10, 5, 0, 0, 2048, 0, 0, 0, 255},
    {0, 5, 0, 1024, 2048, 0, 0, 0, 255},
    {0, -5, 0, 1024, 2048, 0, 0, 0, 255},
    {10, -5, 0, 0, 2048, 0, 0, 0, 255},
    {10, 5, 0, 0, 3072, 0, 0, 0, 255},
    {0, 5, 0, 1024, 3072, 0, 0, 0, 255},
    {0, -5, 0, 1024, 3072, 0, 0, 0, 255},
    {10, -5, 0, 0, 3072, 0, 0, 0, 255},
    {10, 5, 0, 0, 4096, 0, 0, 0, 255},
    {0, 5, 0, 1024, 4096, 0, 0, 0, 255},
};

static effN64Vtx size128x10_tex32x32_vtx[] = {
    {0, -5, 0, 0, 0, 0, 0, 0, 255},
    {128, -5, 0, 1024, 0, 0, 0, 0, 255},
    {128, 5, 0, 1024, 1024, 0, 0, 0, 255},
    {0, 5, 0, 0, 1024, 0, 0, 0, 255},
    {0, -5, 0, 0, 1024, 0, 0, 0, 255},
    {128, -5, 0, 1024, 1024, 0, 0, 0, 255},
    {128, 5, 0, 1024, 2048, 0, 0, 0, 255},
    {0, 5, 0, 0, 2048, 0, 0, 0, 255},
    {0, -5, 0, 0, 2048, 0, 0, 0, 255},
    {128, -5, 0, 1024, 2048, 0, 0, 0, 255},
    {128, 5, 0, 1024, 3072, 0, 0, 0, 255},
    {0, 5, 0, 0, 3072, 0, 0, 0, 255},
    {0, -5, 0, 0, 3072, 0, 0, 0, 255},
    {128, -5, 0, 1024, 3072, 0, 0, 0, 255},
    {128, 5, 0, 1024, 4096, 0, 0, 0, 255},
    {0, 5, 0, 0, 4096, 0, 0, 0, 255},
};

static effN64Vtx size128x10_tex32x32_2_vtx[] = {
    {0, -5, 0, 1024, 0, 0, 0, 0, 255},
    {128, -5, 0, 0, 0, 0, 0, 0, 255},
    {128, 5, 0, 0, 1024, 0, 0, 0, 255},
    {0, 5, 0, 1024, 1024, 0, 0, 0, 255},
    {0, -5, 0, 1024, 1024, 0, 0, 0, 255},
    {128, -5, 0, 0, 1024, 0, 0, 0, 255},
    {128, 5, 0, 0, 2048, 0, 0, 0, 255},
    {0, 5, 0, 1024, 2048, 0, 0, 0, 255},
    {0, -5, 0, 1024, 2048, 0, 0, 0, 255},
    {128, -5, 0, 0, 2048, 0, 0, 0, 255},
    {128, 5, 0, 0, 3072, 0, 0, 0, 255},
    {0, 5, 0, 1024, 3072, 0, 0, 0, 255},
    {0, -5, 0, 1024, 3072, 0, 0, 0, 255},
    {128, -5, 0, 0, 3072, 0, 0, 0, 255},
    {128, 5, 0, 0, 4096, 0, 0, 0, 255},
    {0, 5, 0, 1024, 4096, 0, 0, 0, 255},
};
u8 n_data[16] = {11, 11, 11, 11, 17, 11, 11, 21, 21, 21, 21, 21, 21};
static u8 vx_data[16] = {20, 5, 20, 20, 10, 20, 20, 8, 8, 8, 1, 1, 1};

void effPokopiPchargeMain(EffectEntry* effect);
void effPokopiPchargeDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPokopiPchargeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffPokopiPchargeN64Elem* work;
    s32 count;
    EffectEntry* effect;
    EffPokopiPchargeN64Elem* p;
    s32 i;
    f32 base;

    effect = effEntry();
    count = n_data[type];
    effect->type = "PokopiPchargeN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffPokopiPchargeN64Elem));
    effect->userdata = work;
    effect->callback = effPokopiPchargeMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    p = work + 1;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->r = 255;
    work->g = 255;
    work->b = 255;
    work->r2 = 255;
    work->g2 = 245;
    work->b2 = 0;
    work->mult = 1.0f;
    work->speed = (f32)vx_data[type];
    base = (f32)(rand() % 360);
    for (i = 1; i < effect->count; i++, p++) {
        p->type = type % 3;
        switch (type) {
            case 0:
            case 1:
            case 2:
                p->speed = -(0.2f * (f32)(rand() % 10) + 0.2f);
                p->lifetime = 20;
                p->scale = 1.0f - 0.1f * (f32)(i % 3);
                p->rotVel = 0.0f;
                break;
            case 3:
            case 5:
                p->speed = 0.2f * (f32)(rand() % 10) + 0.2f;
                p->lifetime = 20;
                p->scale = 1.0f - 0.1f * (f32)(i % 3);
                p->rotVel = 0.0f;
                break;
            case 6:
                p->speed = (f32)-(rand() % 10 + 4);
                p->lifetime = 10;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = 0.0f;
                break;
            case 7:
                p->speed = (f32)-(rand() % 10 + 4);
                p->lifetime = 10;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = 0.0f;
                break;
            case 8:
                p->speed = 0.2f * (f32)(rand() % 10) + 0.6f;
                p->lifetime = 20;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = -1.0f;
                break;
            case 9:
                p->speed = -(0.2f * (f32)(rand() % 10) + 0.6f);
                p->lifetime = 20;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = -2.0f;
                work->r = 255;
                work->g = 245;
                work->b = 215;
                work->r2 = 220;
                work->g2 = 60;
                work->b2 = 10;
                break;
            case 10:
                p->speed = 0.1f * (f32)(rand() % 10) + 2.0f;
                p->lifetime = 10;
                p->scale = 0.5f * (1.0f - 0.3f * (f32)(i % 3));
                p->rotVel = 0.0f;
                break;
            case 11:
                p->speed = 0.05f * (f32)(rand() % 10) + 0.2f;
                p->lifetime = 30;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = -1.0f;
                break;
            case 4:
            case 12:
                p->speed = 0.2f * (f32)(rand() % 10) + 0.6f;
                p->lifetime = 20;
                p->scale = 1.0f - 0.3f * (f32)(i % 3);
                p->rotVel = -2.0f;
                break;
        }
        p->frame = -(rand() % p->lifetime);
        if (p->speed < 0.0f) {
            p->position.x = -p->speed * (f32)(p->lifetime - p->frame);
        } else {
            p->position.x = (f32)-p->frame * p->speed;
        }
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->rotation = base + (f32)((i - 1) * 360 / (count - 1));
    }
    return effect;
}

void effPokopiPchargeMain(EffectEntry* effect) {
    EffPokopiPchargeN64Elem* p = effect->userdata;
    s32 i;
    s32 lifetime;
    s32 frame;
    f32 sv;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = p->position.x;
    tmp.y = p->position.y;
    tmp.z = p->position.z;
    position = tmp;
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
    for (i = 1; i < effect->count; i++, p++) {
        p->frame++;
        if (p->frame >= p->lifetime) {
            p->frame = 0;
            if (p->speed < 0.0f) {
                p->position.x = (f32)-p->lifetime * p->speed;
            } else {
                p->position.x = 0.0f;
            }
            p->rotation = (f32)(rand() % 360);
        }
        p->position.x += p->speed;
        p->rotation += p->rotVel;
        sv = (f32)(p->frame * 180 / p->lifetime);
        if (sv < 0.0f || sv > 180.0f) {
            p->alpha = 0;
        } else {
            sv = (f32)sin(6.2831855f * sv / 360.0f);
            p->alpha = (s32)(255.0f * sv);
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effPokopiPchargeDisp, effect);
}

void effPokopiPchargeDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffPokopiPchargeN64Elem* p;
    s32 i;
    s32 alpha;
    s32 type;
    s32 r;
    s32 g;
    s32 b;
    f32 s;
    f32 off;
    f32 sc;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    alpha = p->alpha;
    type = p->type;
    r = p->r;
    g = p->g;
    b = p->b;
    s = p->scale * p->mult;
    off = p->speed;
    PSMTXTrans(mtx, p->position.x, p->position.y, p->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, s, s, s);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, viewMtx);
    GXSetTevColor(GX_TEVREG0, (GXColor){p->r, p->g, p->b, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){p->r2, p->g2, p->b2});
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.0078125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x86, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        sc = p->scale;
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, alpha * p->alpha / 255});
        PSMTXRotRad(rot, 'z', 0.017453292f * p->rotation);
        PSMTXTrans(mtx, p->position.x + off, 0.0f, 0.0f);
        PSMTXConcat(rot, mtx, mtx);
        if (type == 11 || type == 10) {
            PSMTXScale(scale, sc, 1.0f, 1.0f);
        } else {
            PSMTXScale(scale, sc, sc, 1.0f);
        }
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (type) {
            case 0:
            case 1:
            case 2:
                effSetVtxDescN64(size10x10_tex32x32_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(type * 4, type * 4 + 1, type * 4 + 2, 0, type * 4, type * 4 + 2, type * 4 + 3, 0);
                break;
            case 3:
            case 4:
            case 5:
                effSetVtxDescN64(size10x10_tex32x32_2_vtx);
                type -= 3;
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(type * 4, type * 4 + 1, type * 4 + 2, 0, type * 4, type * 4 + 2, type * 4 + 3, 0);
                break;
            case 6:
            case 7:
            case 8:
            case 11:
                effSetVtxDescN64(size128x10_tex32x32_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            case 9:
            case 12:
                effSetVtxDescN64(&size10x10_tex32x32_2_vtx[8]);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            case 10:
                effSetVtxDescN64(size128x10_tex32x32_2_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
        }
    }
}
