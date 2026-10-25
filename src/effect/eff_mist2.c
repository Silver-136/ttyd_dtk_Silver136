#include "driver/camdrv.h"
#include "eff/eff_mist2.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

void effMist2Main(EffectEntry* effect);
void effMist2Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effMist2Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 a, f32 b) {
    EffectEntry* effect;
    EffMist2Work* work;
    EffMist2Work* p;
    s32 i;
    s32 count;
    f32 ang;
    f32 amp;

    effect = effEntry();
    count = (type == 0 ? 30 : 10) + 1;
    effect->type = "Mist2";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffMist2Work));
    effect->userdata = work;
    effect->callback = effMist2Main;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->timer = 0;
    work->alpha = 255.0f;
    work->scale = 1.0f;
    work->r = 0;
    work->g = 0;
    work->b8 = 0;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->a = a;
        p->b = b;
        amp = irand((s32)p->a);
        ang = 6.2831855f * irand(360) / 360.0f;
        p->position.x = amp * (f32)sin(ang);
        p->position.y = 0.0f;
        p->position.z = amp * (f32)cos(ang);
        p->scale = 0.0f;
        p->scaleTarget = 0.5f + irand(100) / 100.0f;
        p->alpha = 255.0f;
        p->life = irand(60) + 60;
        p->timer = 0;
        p->rotSpeed = 5 - irand(10);
        p->rot = 0.0f;
        p->delay = irand(30);
        p->decay = 0.2f;
        p->vy = 0.0f;
        if (type == 1) {
            f32 t = 6.2831855f * irand(360) / 360.0f;
            p->position.x = p->a * (f32)sin(t);
            p->position.y = p->b;
            p->position.z = p->a * (f32)cos(t);
        }
    }
    return effect;
}

void effMist2Main(EffectEntry* effect) {
    EffMist2Work* work = effect->userdata;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    f32 ang;
    f32 amp;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 32;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    if (work->life < 32) {
        work->alpha = work->life * 8;
    }
    work->timer++;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            continue;
        }
        work->life--;
        if (work->life < 32) {
            work->alpha = work->life * 8;
        }
        if (work->alpha <= 0.0f) {
            amp = irand((s32)work->a);
            ang = 6.2831855f * irand(360) / 360.0f;
            work->position.x = amp * (f32)sin(ang);
            work->position.y = 0.0f;
            work->position.z = amp * (f32)cos(ang);
            work->scale = 0.0f;
            work->scaleTarget = 0.5f + irand(100) / 100.0f;
            work->alpha = 255.0f;
            work->life = irand(60) + 60;
            work->timer = 0;
            work->rotSpeed = 5 - irand(10);
            work->rot = 0.0f;
            work->delay = irand(30);
            work->decay = 0.2f;
            work->vy = 0.0f;
        } else {
            work->rot += work->rotSpeed;
            work->scale += (work->scaleTarget - work->scale) / 10.0f;
            if (type == 1) {
                work->position.y += work->vy;
                work->vy -= work->decay;
                if (work->position.y < 0.0f) {
                    work->position.y = 0.0f;
                }
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMist2Disp, effect);
}

void effMist2Disp(CameraId cameraId, EffectEntry* effect) {
    EffMist2Work* work = effect->userdata;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    GXTexObj texObj;
    CameraEntry* cam;
    s32 i;
    s32 alpha;
    u8 r, g, b;
    f32 w = 16.0f;
    f32 k = 255.0f;
    f32 h = 8.0f;

    cam = camGetPtr(cameraId);
    r = work->r;
    g = work->g;
    b = work->b8;
    alpha = work->alpha;
    effGetTexObj(0x5e, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXRotRad(rot, 'z', MTXDegToRad(work->rot));
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){r, g, b, work->alpha * alpha / k});
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2.0f, h, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(h, h, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(h, -w / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
