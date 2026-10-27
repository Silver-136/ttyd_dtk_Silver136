#include "eff/eff_n64.h"
#include "eff/n64/eff_fire_dust_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"

typedef struct EffFireDustWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 range;      // 0x10
    f32 height;     // 0x14
    Vec vel;        // 0x18
    Vec targetVel;  // 0x24
    s32 index;      // 0x30
    f32 rot;        // 0x34
    s32 lifetime;   // 0x38
    s32 frame;      // 0x3c
    s32 cr, cg, cb; // 0x40
    s32 alpha;      // 0x4c
    s32 r, g, b;    // 0x50
    s32 ca;         // 0x5c
    f32 scale;      // 0x60
    f32 unk64;      // 0x64
    f32 unk68;      // 0x68
} EffFireDustWork;

static f32 scale_data[] = {
    0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f,
    0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f,
};

static effN64Vtx size8x8_tex22x22_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, 8, 0, 0, 704, 0, 0, 0, 255},
    {-8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, 8, 0, 4928, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, -8, 0, 5632, 0, 0, 0, 0, 255},
    {8, 8, 0, 5632, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4928, 704, 0, 0, 0, 255},
};

void effFireDustMain(EffectEntry* effect);
void effFireDustDisp(CameraId cameraId, EffectEntry* effect);
static void main_dl(EffectEntry* effect, Mtx mtx);

EffectEntry* effFireDustN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 range, f32 height) {
    EffectEntry* effect;
    EffFireDustWork* work;
    EffFireDustWork* p;
    f32 step;
    s32 i;

    effect = effEntry();
    effect->type = "FireDustN64";
    effect->count = count + 2;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFireDustWork) * (count + 2));
    effect->userdata = work;
    effect->callback = effFireDustMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->range = range;
    work->height = height;
    work->cr = 255;
    work->cg = 255;
    work->cb = 255;
    work->r = 255;
    work->g = 255;
    work->b = 127;
    work->ca = 255;
    p = work + 1;
    if (lifetime < 20) {
        step = 20.0f / (count + 1);
    } else {
        step = (f32)lifetime / (count + 1);
    }
    for (i = 1; i < count + 2; i++, p++) {
        p->index = (s32)(-(f32)i * step) - 1;
    }
    return effect;
}

void effFireDustMain(EffectEntry* effect) {
    s32 height;
    EffFireDustWork* p;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 half;
    f32 range;
    s32 i;
    f32 angle;

    p = effect->userdata;
    tmp.x = p->position.x;
    tmp.y = p->position.y;
    tmp.z = p->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        p->lifetime = 16;
    }
    if (p->lifetime < 1000) {
        p->lifetime--;
    }
    p->frame++;
    if (p->lifetime < 0) {
        effDelete(effect);
    } else {
        if (p->lifetime < 8) {
            p->alpha = p->lifetime * 32;
        }
        height = (s32)p->height;
        range = p->range;
        half = 0.5f * p->height;
        p++;
        for (i = 1; i < effect->count; i++, p++) {
            p->index++;
            if (p->index >= 20) {
                p->index = 0;
            }
            if (p->index < 0) {
                continue;
            }
            if (p->index == 0) {
                angle = (f32)(rand() % 360);
                rand();
                angle = (6.2831855f * angle) / 360.0f;
                p->position.x = range * (f32)sin(angle);
                p->position.y = (f32)(rand() % height) - half;
                p->position.z = range * (f32)cos(angle);
                p->vel.x = 15.0f * (f32)sin(angle);
                p->vel.y = 0.4f * p->position.y;
                p->vel.z = 15.0f * (f32)cos(angle);
                p->targetVel.x = 0.5f * p->vel.x;
                p->targetVel.y = 0.5f * p->vel.y;
                p->targetVel.z = 0.5f * p->vel.z;
                p->unk64 = (f32)(rand() % 15);
                p->unk68 = (f32)(rand() % 15);
                p->rot = 0.0f;
                p->alpha = 255;
            }
            p->scale = 2.0f * scale_data[p->index];
            p->rot += 4.0f;
            p->position.x += p->vel.x;
            p->position.y += p->vel.y;
            p->position.z += p->vel.z;
            p->unk64 += 0.2f;
            p->unk68 += 0.6f;
            p->vel.x += 0.04f * (p->targetVel.x - p->vel.x);
            p->vel.y += 0.04f * (p->targetVel.y - p->vel.y);
            p->vel.z += 0.04f * (p->targetVel.z - p->vel.z);
        }
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effFireDustDisp, effect);
    }
}

void effFireDustDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFireDustWork* work;
    GXTexObj texObj;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    Mtx base;
    Mtx mtx;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scaleMtx, base);
    GXSetTevColor(GX_TEVREG1, (GXColor){work->cr, work->cg, work->cb, work->ca});
    effGetTexObjN64(0x20, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.0056818184f, 0.045454547f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    PSMTXConcat(camera->view, base, mtx);
    main_dl(effect, mtx);
    PSMTXRotRad(rot, 'y', 2.0943952f);
    PSMTXConcat(base, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    main_dl(effect, mtx);
    PSMTXRotRad(rot, 'y', -2.0943952f);
    PSMTXConcat(base, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    main_dl(effect, mtx);
}

static void main_dl(EffectEntry* effect, Mtx cam) {
    EffFireDustWork* p;
    Mtx tr;
    Mtx scaleMtx;
    Mtx rot;
    f32 a;
    s32 r, g, b;
    s32 i;
    effN64Vtx* vtx;

    p = effect->userdata;
    a = (f32)p->alpha / 255.0f;
    r = p->r;
    g = p->g;
    b = p->b;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        if (p->index < 0) {
            continue;
        }
        PSMTXTrans(tr, p->position.x, p->position.y, p->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * p->rot);
        PSMTXScale(scaleMtx, p->scale * a, p->scale * a, p->scale * a);
        PSMTXConcat(tr, rot, tr);
        PSMTXConcat(tr, scaleMtx, tr);
        PSMTXConcat(cam, tr, tr);
        GXLoadPosMtxImm(tr, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, (s32)((f32)p->alpha * a)});
        switch (i & 7) {
            case 0:
                vtx = &size8x8_tex22x22_vtx[12];
                break;
            case 1:
                vtx = &size8x8_tex22x22_vtx[16];
                break;
            case 2:
                vtx = &size8x8_tex22x22_vtx[20];
                break;
            case 3:
                vtx = &size8x8_tex22x22_vtx[24];
                break;
            case 4:
                vtx = &size8x8_tex22x22_vtx[28];
                break;
            case 5:
                vtx = &size8x8_tex22x22_vtx[0];
                break;
            case 6:
                vtx = &size8x8_tex22x22_vtx[4];
                break;
            case 7:
                vtx = &size8x8_tex22x22_vtx[8];
                break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
