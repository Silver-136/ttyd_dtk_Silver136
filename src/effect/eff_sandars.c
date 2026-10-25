#include "eff/eff_sandars.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/n64/eff_bomb_n64.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
double sin(double);
double cos(double);

void effSandarsMain(EffectEntry* effect);
void effSandarsDisp(CameraId cameraId, EffectEntry* effect);
void effSandarsDisp2(CameraId cameraId, EffectEntry* effect);

static u8 scale_data[] = { 0, 0, 0, 13, 0x1C, 0x2C, 0x3C, 0x4B, 0x59, 0x65, 0x6F, 0x75, 0x78, 0x4B, 0};

EffectEntry* effSandarsEntry(s32 type, f32 x, f32 y, f32 z, f32 radius) {
    EffSandarsWork* work;
    EffectEntry* effect;
    s32 camId;
    s32 count;
    f32 ang;
    s32 i;

    effect = effEntry();
    camId = gp->inBattle != 0;
    count = 1;
    if (type == 5) {
        count = 6;
    }
    effect->type = "Sandars";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffSandarsWork));
    effect->userdata = work;
    effect->callback = effSandarsMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->state = 0;
    work->frame = 0;
    work->poseId = -1;
    work->camId = 4;
    switch (type) {
    case 0:
    case 4:
        work->bombType = 1;
        work->bombScale = 1.0f;
        break;
    case 1:
    case 2:
    case 3:
        work->bombType = 2;
        work->bombScale = 4.0f;
        break;
    case 5:
        work->frame = 100;
        work->radius = radius;
        break;
    }
    if (work->type == 3 || work->type == 4) {
        animGroupBaseAsync("EFF_bakuhatu", camId, NULL);
        work->poseId = -1;
        work->alpha = 255;
    }
    ang = 6.2831855f * radius / 360.0f;
    work++;
    for (i = 1, camId = 10; i < effect->count; camId += 10, i++, work++) {
        work->position.x = (f32)camId * (f32)cos(ang);
        work->position.y = (f32)camId * (f32)sin(ang);
        work->position.z = 0.0f;
        work->scale2 = 0.2f * i;
        work->scale = 0.0f;
        work->alpha = (s32)(51.0f * i);
        work->delay = (i - 1) * 5;
        work->frame = 0;
        work->rot = 0.0f;
        work->rotSpeed = 0.0f;
        if (i == effect->count - 1) {
            work->scale2 = 1.1f;
            work->alpha = 204;
            work->rotSpeed = 15.0f;
        }
    }
    return effect;
}

static inline Vec effSandarsMakeVec(f32 x, f32 y, f32 z) {
    Vec pos = { 0.0f, 0.0f, 0.0f };
    pos.x = x;
    pos.y = y;
    pos.z = z;
    return pos;
}

void effSandarsMain(EffectEntry* effect) {
    EffSandarsWork* work = effect->userdata;
    Vec pos2;
    f32 bx, by, bz;
    s32 camId;
    s32 camera;
    s32 type;
    s32 i;

    pos2 = effSandarsMakeVec(bx = work->position.x, by = work->position.y, bz = work->position.z);
    type = work->type;
    camId = work->camId;
    camera = gp->inBattle != 0;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    switch (type) {
    case 5:
        work->frame--;
        if (work->frame < 0) {
            effDelete(effect);
            return;
        }
        work++;
        for (i = 1; i < effect->count; i++, work++) {
            if (work->delay != 0) {
                work->delay--;
            } else {
                if ((u32)work->frame < 15) {
                    work->scale = scale_data[work->frame] / 100.0f;
                }
                work->frame++;
                work->rot += work->rotSpeed;
            }
        }
        dispEntry(camId, dispCalcZ(&pos2), 2, (DispCallback)effSandarsDisp2, effect);
        return;
    default:
        break;
    }
    switch (work->state) {
    case 0:
        effBombN64SetCamId(effBombN64Entry(work->bombType, bx, by, bz), camId);
        work->frame = 0;
        work->state++;
        break;
    case 1:
        if (type < 3) {
            effDelete(effect);
            return;
        }
        if (work->frame != 0) {
            work->frame--;
        } else if (animGroupBaseAsync("EFF_bakuhatu", camera, NULL)) {
            work->poseId = animPoseEntry("EFF_bakuhatu", camera);
            animPoseSetAnim(work->poseId, "Z_1", TRUE);
            work->state++;
        }
        break;
    case 2:
        work->alpha -= 8;
        if (work->alpha < 0) {
            animPoseRelease(work->poseId);
            work->poseId = -1;
            effDelete(effect);
            return;
        }
        break;
    }
    dispEntry(camId, dispCalcZ(&pos2), 2, (DispCallback)effSandarsDisp, effect);
}

void effSandarsDisp(CameraId cameraId, EffectEntry* effect) {
    EffSandarsWork* work;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx view;

    work = effect->userdata;
    camGetPtr(cameraId);
    if (work->poseId == -1) {
        return;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, view);
    PSMTXScale(scale, work->bombScale, work->bombScale, work->bombScale);
    PSMTXConcat(view, scale, view);
    animPoseSetMaterialFlagOn(work->poseId, 0x40);
    animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
    animPoseMain(work->poseId);
    animPoseDrawMtx(work->poseId, view, 1, 0.0f, 4.0f);
    animPoseDrawMtx(work->poseId, view, 2, 0.0f, 4.0f);
    animPoseDrawMtx(work->poseId, view, 3, 0.0f, 4.0f);
}

void effSandarsDisp2(CameraId cameraId, EffectEntry* effect) {
    EffSandarsWork* work;
    CameraEntry* camera;
    f32 neg;
    f32 pos;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx view;
    GXTexObj texObj;
    s32 i;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, view);
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
    effGetTexObj(0x44, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    neg = 32.0f;
    neg = -neg / 2.0f;
    pos = 16.0f;
    for (i = 1, work++; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', MTXDegToRad(work->rot));
        PSMTXScale(scale, work->scale * work->scale2, work->scale * work->scale2, work->scale * work->scale2);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(neg, pos, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(pos, pos, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(pos, neg, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(neg, neg, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
