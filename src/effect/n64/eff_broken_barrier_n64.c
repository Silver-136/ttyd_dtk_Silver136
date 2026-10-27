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

typedef struct EffBrokenBarrierN64Work {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 width;      // 0x10
    f32 height;     // 0x14
    Vec velocity;   // 0x18
    Vec targetVel;  // 0x24
    s32 timer;      // 0x30
    f32 rot;        // 0x34
    s32 lifetime;   // 0x38
    s32 frame;      // 0x3C
    s32 r;          // 0x40
    s32 g;          // 0x44
    s32 b;          // 0x48
    s32 alpha;      // 0x4C
    s32 r2;         // 0x50
    s32 g2;         // 0x54
    s32 b2;         // 0x58
    s32 alpha2;     // 0x5C
    f32 scale;      // 0x60
    f32 rotVelA;    // 0x64
    f32 rotVelB;    // 0x68
} EffBrokenBarrierN64Work;

void effBrokenBarrierMain(EffectEntry* effect);
void effBrokenBarrierDisp(CameraId cameraId, EffectEntry* effect);
static void main_dl(EffectEntry* effect, Mtx viewMtx);

static f32 scale_data[] = {0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f,
                           0.2f, 0.4f, 0.8f, 1.3f, 1.5f, 1.3f, 0.9f, 0.6f, 0.4f, 0.3f};

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

EffectEntry* effBrokenBarrierN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffBrokenBarrierN64Work* work;
    EffBrokenBarrierN64Work* piece;
    f32 step;
    s32 i;

    effect = effEntry();
    effect->type = "BrokenBarrierN64";
    effect->count = 61;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBrokenBarrierN64Work) * 61);
    effect->userdata = work;
    effect->callback = effBrokenBarrierMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (life <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = life;
    }
    work->alpha = 255;
    work->position.x = x;
    piece = work + 1;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->width = 30.0f;
    work->height = 30.0f;
    work->r = 255;
    work->g = 255;
    work->b = 255;
    work->r2 = 215;
    work->g2 = 255;
    work->b2 = 235;
    work->alpha2 = 255;
    if (life < 20) {
        step = 1.0f / 3.0f;
    } else {
        step = (f32)life / 60.0f;
    }
    for (i = 1; i < 61; i++, piece++) {
        piece->timer = (s32)(-(f32)i * step) - 1;
    }
    return effect;
}

void effBrokenBarrierMain(EffectEntry* effect) {
    s32 w;
    s32 h;
    EffBrokenBarrierN64Work* work = effect->userdata;
    s32 i;
    s32 type;
    f32 halfW;
    f32 halfH;
    f32 width;
    f32 height;
    f32 ca;
    f32 b;
    f32 a;
    f32 t;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    if (work->lifetime < 8) {
        work->alpha = work->lifetime * 32;
    }
    width = work->width;
    height = work->height;
    switch (type) {
    case 2:
    case 3:
        width *= 4.0f;
        height *= 4.0f;
        break;
    }
    w = (s32)width;
    halfW = 0.5f * width;
    halfH = 0.5f * height;
    h = (s32)height;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->timer++;
        if (work->timer >= 20) {
            work->timer = 0;
        }
        if (work->timer < 0) {
            continue;
        }
        if (work->timer == 0) {
            a = (f32)(rand() % 360);
            t = (f32)(rand() % 360);
            switch (type) {
            case 0:
            case 2: {
                    f32 lat = 6.2831855f * t / 360.0f;
                    f32 lon;
                    work->position.x = width * (f32)sin(lon = 6.2831855f * a / 360.0f) * (f32)cos(lat);
                    work->position.y = height * (f32)cos(lon) * (f32)cos(lat);
                    work->position.z = width * (f32)sin(lat);
                }
                work->velocity.x = 0.4f * work->position.x;
                work->velocity.y = 0.4f * work->position.y;
                work->velocity.z = 0.4f * work->position.z;
                work->targetVel.x = 0.1f * work->velocity.x;
                work->targetVel.y = 0.1f * work->velocity.y;
                work->targetVel.z = 0.1f * work->velocity.z;
                break;
            case 1:
            default:
                work->position.x = (f32)(rand() % w) - halfW;
                work->position.y = (f32)(rand() % h) - halfH;
                work->position.z = (f32)(rand() % w) - halfW;
                work->velocity.x = 0.3f * work->position.x;
                work->velocity.y = 0.3f * work->position.y;
                work->velocity.z = 0.3f * work->position.z;
                work->targetVel.x = 0.02f * work->velocity.x;
                work->targetVel.y = 0.02f * work->velocity.y;
                work->targetVel.z = 0.02f * work->velocity.z;
            }
            work->rotVelA = (f32)(rand() % 15);
            work->rotVelB = (f32)(rand() % 15);
            work->rot = 0.0f;
            work->alpha = 255;
        }
        work->scale = scale_data[work->timer];
        work->rot += 4.0f;
        switch (type) {
        case 2:
        case 3:
            work->velocity.x = 0.05f * -work->position.x;
            work->velocity.y = 0.05f * -work->position.y;
            work->velocity.z = 0.05f * -work->position.z;
            if (sqrtf(work->velocity.x * work->velocity.x + work->velocity.y * work->velocity.y +
                      work->velocity.z * work->velocity.z) < 1.0f) {
                a = (f32)(rand() % 360);
                t = (f32)(rand() % 360);
                b = 6.2831855f * t / 360.0f;
                work->position.x = width * (f32)sin(a = 6.2831855f * a / 360.0f) * (f32)cos(b);
                work->position.y = height * (f32)cos(a) * (f32)cos(b);
                work->position.z = width * (f32)sin(b);
            }
            break;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->rotVelA += 0.2f;
        work->rotVelB += 0.6f;
        work->velocity.x += 0.04f * (work->targetVel.x - work->velocity.x);
        work->velocity.y += 0.04f * (work->targetVel.y - work->velocity.y);
        work->velocity.z += 0.04f * (work->targetVel.z - work->velocity.z);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBrokenBarrierDisp, effect);
}

void effBrokenBarrierDisp(CameraId cameraId, EffectEntry* effect) {
    Mtx trans;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx mtx;
    Mtx mtx2;
    GXTexObj texObj;
    CameraEntry* camera;
    EffBrokenBarrierN64Work* work;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scaleMtx, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, scaleMtx, mtx);
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r, work->g, work->b, work->alpha2});
    effGetTexObjN64(32, &texObj);
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
    PSMTXConcat(camera->view, mtx, mtx2);
    main_dl(effect, mtx2);
    {
        Vec axis = {0.4f, 0.0f, 0.8f};

        PSMTXRotAxisRad(rotMtx, &axis, 2.0943952f);
        PSMTXConcat(mtx, rotMtx, mtx2);
        PSMTXConcat(camera->view, mtx2, mtx2);
        main_dl(effect, mtx2);
        PSMTXRotAxisRad(rotMtx, &axis, -2.0943952f);
        PSMTXConcat(mtx, rotMtx, mtx2);
        PSMTXConcat(camera->view, mtx2, mtx2);
        main_dl(effect, mtx2);
    }
}

static void main_dl(EffectEntry* effect, Mtx viewMtx) {
    EffBrokenBarrierN64Work* work = effect->userdata;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    f32 alpha;
    s32 r = work->r2;
    s32 g = work->g2;
    s32 b = work->b2;
    s32 i;
    effN64Vtx* vtx;

    alpha = (f32)work->alpha / 255.0f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->timer < 0) {
            continue;
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
        PSMTXScale(scaleMtx, work->scale * alpha, work->scale * alpha, work->scale * alpha);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat(viewMtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, (s32)((f32)work->alpha * alpha)});
        switch (i & 7) {
            case 0: vtx = &size8x8_tex22x22_vtx[12]; break;
            case 1: vtx = &size8x8_tex22x22_vtx[16]; break;
            case 2: vtx = &size8x8_tex22x22_vtx[20]; break;
            case 3: vtx = &size8x8_tex22x22_vtx[24]; break;
            case 4: vtx = &size8x8_tex22x22_vtx[28]; break;
            case 5: vtx = &size8x8_tex22x22_vtx[0]; break;
            case 6: vtx = &size8x8_tex22x22_vtx[4]; break;
            case 7: vtx = &size8x8_tex22x22_vtx[8]; break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
