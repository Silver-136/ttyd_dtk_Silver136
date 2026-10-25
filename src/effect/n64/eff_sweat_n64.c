#include "eff/n64/eff_sweat_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(float);
double cos(float);

void effSweatMain(EffectEntry* effect);
void effSweatDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effSweatN64Entry(s32 type, f32 x, f32 y, f32 z, f32 base, f32 angle, s32 life) {
    EffectEntry* effect;
    EffSweatN64Work* work;
    EffSweatN64Work* w;
    s32 count;
    s32 i;
    f32 rot;
    f32 len;
    f32 spread;
    Mtx mtx;

    effect = effEntry();
    count = 1;
    switch (type) {
    case 0:
        count = 7;
        break;
    case 1:
        count = 5;
        break;
    }
    effect->type = "SweatN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSweatN64Work) * count);
    effect->userdata = work;
    effect->callback = effSweatMain;
    len = 16.0f + base;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    w = work + 1;
    for (i = 1; i < effect->count; i++, w++) {
        Vec axis;
        Vec dir = {0.0f, 0.0f, 0.0f};
        dir.x = (f32)sin(0.0f);
        dir.z = -(f32)cos(0.0f);
        axis = dir;
        switch (type) {
        case 0:
            spread = (i - 1) % 3 * 14;
            break;
        case 1:
            spread = (i - 1) % 4 * 16;
            break;
        }
        if (angle > 0.0f) {
            spread *= -1.0f;
        }
        rot = angle + spread;
        PSMTXRotAxisRad(mtx, &axis, 0.017453292f * -rot);
        w->position.x = mtx[0][1] * len;
        w->position.y = mtx[1][1] * len;
        w->position.z = mtx[2][2] * len;
        w->velocity.x = mtx[0][1];
        w->velocity.y = mtx[1][1];
        w->velocity.z = mtx[2][1];
        w->rot = rot;
        w->life = life;
        w->age = 0;
        w->delay = irand(10);
        w->alpha = 0;
        if (type == 0 && i >= 4) {
            w->delay += 16;
        }
    }
    return effect;
}

void effSweatMain(EffectEntry* effect) {
    EffSweatN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 done = 0;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            continue;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->velocity.x *= 0.94f;
        work->velocity.y -= 0.05f;
        if (work->velocity.y < -0.1f) {
            work->velocity.y = -0.1f;
        }
        work->velocity.z *= 0.94f;
        if (work->age < 8) {
            work->alpha = work->age * 32 + 31;
        }
        if (work->life < 16) {
            work->alpha = work->life * 16;
        }
        if (work->alpha > 255) {
            work->alpha = 255;
        }
        if (work->alpha < 0) {
            work->alpha = 0;
        }
        work->age++;
        work->life--;
        if (work->life < 0) {
            done++;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effSweatDisp, effect);
    }
}

void effSweatDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffSweatN64Work* work;
    Mtx trans;
    Mtx rot;
    Mtx base;
    GXTexObj texObj;
    s32 i;
    f32 x2;
    f32 nsize;
    f32 x1;
    f32 yt;
    f32 size;
    f32 yb;
    f32 h = 9.6f;
    f32 neg;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(trans, rot, base);
    PSMTXConcat(camera->view, base, base);
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
    GXSetCullMode(GX_CULL_NONE);
    effGetTexObj(93, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    neg = -h;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            continue;
        }
        {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){255, 255, 255, work->alpha});
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(base, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        size = 6.4f;
        if (work->rot > 0.0f) {
            size *= -1.0f;
        }
        yt = 4.8f;
        nsize = 0.5f;
        x1 = -size * nsize;
        x2 = size * nsize;
        yb = neg * nsize;
        GXPosition3f32(x1, yt, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(x2, yt, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(x2, yb, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(x1, yb, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
