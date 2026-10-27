#include "mario/mario.h"
#include "eff/eff_n64.h"
#include "eff/n64/eff_whirlwind_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>

u8 col_r[3] = {0xFF, 0xFF, 0x40};
u8 col_g[3] = {0xFF, 0x40, 0x40};
u8 col_b[3] = {0xFF, 0x40, 0xFF};

effN64Vtx eff_whirlwind_v[] = {
    {-5, -2, 9, 939, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-5, 2, 9, 939, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-9, 2, 5, 853, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-9, -2, 5, 853, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, -2, 10, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, 2, 10, 1024, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-10, 2, 0, 768, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-10, -2, 0, 768, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, -2, 9, 85, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, 2, 9, 85, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, 2, 10, 0, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, -2, 10, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {9, -2, 5, 171, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {9, 2, 5, 171, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {10, -2, 0, 256, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {10, 2, 0, 256, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {9, -2, -5, 341, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {9, 2, -5, 341, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-9, 2, -5, 683, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-9, -2, -5, 683, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, -2, -9, 427, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {5, 2, -9, 427, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, -2, -10, 512, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {0, 2, -10, 512, 512, 0xFF, 0xFF, 0xFF, 0xFF},
    {-5, -2, -9, 597, 0, 0xFF, 0xFF, 0xFF, 0xFF},
    {-5, 2, -9, 597, 512, 0xFF, 0xFF, 0xFF, 0xFF},
};

void effWhirlwindMain(EffectEntry* effect);
void effWhirlwindDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effWhirlwindN64Entry(s32 type, MarioWork* target, f32 scale, s32 life) {
    f32 quarter;
    EffectEntry* effect;
    EffWhirlwindN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "WhirlwindN64";
    effect->count = 5;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffWhirlwindN64Work) * 5);
    effect->userdata = work;
    effect->callback = effWhirlwindMain;
    work->type = type;
    work->target = target;
    work->scale = 1.2f * scale;
    work->baseScale = 1.2f * scale;
    work->duration = life;
    work->life = life;
    work->alpha = 0;
    work->frame = 0;
    work++;
    for (i = 1; i < 5; i++, work++) {
        s32 idx;
        switch (type) {
            case 0:
            case 2:
            case 4:
            case 6:
                work->position.x = 0.0f;
                work->position.y = 7 * i + 4;
                work->position.z = 0.0f;
                work->vel.x = 0.0f;
                work->vel.y = 60.0f;
                work->vel.z = 0.0f;
                work->rot.x = 0.0f;
                work->rot.y = 120 * i;
                work->rot.z = 0.0f;
                work->scale = 1.0f;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
                work->position.x = 0.0f;
                work->position.y = 7 * i + 4;
                work->position.z = 0.0f;
                work->vel.x = 0.0f;
                work->vel.y = -60.0f;
                work->vel.z = 0.0f;
                work->rot.x = 0.0f;
                work->rot.y = 120 * i;
                work->rot.z = 0.0f;
                work->scale = 1.0f;
                work->b = 0xFF;
                work->g = 0xFF;
                work->r = 0xFF;
                break;
            default:
                work->position.x = 0.0f;
                work->position.y = 0.5f * target->wPlayerCollisionBox.y;
                work->position.z = 0.0f;
                work->vel.x = (rand() % 2 * 2 - 1) * 4;
                work->vel.y = 0.0f;
                work->vel.z = (rand() % 2 * 2 - 1) * 4;
                // FAKE: spare temporary for 0.25f; the plain literal changes the object
                quarter = 0.25f;
                work->scale = 0.5f + 0.5f * ((f32)(i - 1) * quarter);
                work->rot.x = 25 * i;
                work->rot.y = (i - 1) * 360 / 4;
                work->rot.z = 360 - 38 * i;
                break;
        }
        switch (type) {
            case 2:
            case 3:
                idx = 1;
                break;
            case 4:
            case 5:
                idx = 2;
                break;
            case 6:
            case 7:
                idx = (i & 1) + 1;
                break;
            default:
                idx = 0;
                break;
        }
        work->r = col_r[idx];
        work->g = col_g[idx];
        work->b = col_b[idx];
    }
    return effect;
}

void effWhirlwindMain(EffectEntry* effect) {
    EffWhirlwindN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 life;
    s32 type;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->life--;
    work->frame++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    type = work->type;
    switch (type) {
        case 8:
            if (life > 5) {
                work->alpha = work->alpha + 0.05f * (255 - work->alpha);
            }
            work->scale = work->baseScale * life / work->duration;
            if (life < 10) {
                work->alpha = life * 25;
            }
            break;
        default:
            if (life > 5) {
                work->alpha = work->alpha + 0.3f * (100 - work->alpha);
            }
            if (life < 10) {
                work->alpha *= 0.8f;
                work->scale += 0.1f * (2.0f * work->baseScale - work->scale);
            }
    }
    if (work->target != NULL) {
        Vec* p = &work->target->position;
        work->position.x = p->x;
        work->position.y = p->y;
        work->position.z = p->z;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        switch (type) {
            case 8:
                if (i & 1) {
                    work->vel.y += 0.2f * (40.0f - work->vel.y);
                } else {
                    work->vel.y += 0.2f * (-40.0f - work->vel.y);
                }
                work->rot.x += work->vel.x;
                work->rot.y += work->vel.y;
                work->rot.z += work->vel.z;
                break;
            default:
                if (life < 10) {
                    work->rot.y += work->vel.y * life / 10.0f;
                } else {
                    work->rot.y += work->vel.y;
                }
                break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effWhirlwindDisp, effect);
}

void effWhirlwindDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffWhirlwindN64Work* work;
    s32 i;
    s32 type;
    s32 alpha;
    f32 off;
    f32 scale;
    f32 baseScale;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    Mtx viewMtx;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    alpha = work->alpha;
    scale = work->scale;
    baseScale = work->baseScale;
    type = work->type;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x58, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, viewMtx);
    PSMTXConcat(camera->view, viewMtx, viewMtx);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(eff_whirlwind_v);
    off = 3.0f * (scale - baseScale);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'x', 0.017453292f * work->rot.x);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot.z);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(scaleMtx, scale * work->scale, scale * work->scale, scale * work->scale);
        PSMTXConcat(mtx, scaleMtx, mtx);
        PSMTXRotRad(rot, 'y', 0.017453292f * work->rot.y);
        PSMTXConcat(mtx, rot, rot);
        if (type < 8) {
            PSMTXTrans(mtx, 3.0f - off / baseScale, 0.0f, 0.0f);
            PSMTXConcat(rot, mtx, mtx);
        } else {
            PSMTXTrans(mtx, 2.0f, 0.0f, 0.0f);
            PSMTXConcat(rot, mtx, mtx);
        }
        PSMTXConcat(viewMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 72);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
        tri2(4, 5, 1, 0, 4, 1, 0, 0);
        tri2(3, 2, 6, 0, 3, 6, 7, 0);
        tri2(8, 9, 10, 0, 8, 10, 11, 0);
        tri2(12, 13, 9, 0, 12, 9, 8, 0);
        tri2(14, 15, 13, 0, 14, 13, 12, 0);
        tri2(16, 17, 15, 0, 16, 15, 14, 0);
        tri2(7, 6, 18, 0, 7, 18, 19, 0);
        tri2(20, 21, 17, 0, 20, 17, 16, 0);
        tri2(22, 23, 21, 0, 22, 21, 20, 0);
        tri2(24, 25, 23, 0, 24, 23, 22, 0);
        tri2(19, 18, 25, 0, 19, 25, 24, 0);
    }
}
