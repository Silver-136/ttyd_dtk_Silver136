#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_batten.h"

typedef struct EffBattenWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 scale;     // 0x10
    u8 alpha;      // 0x14
    s32 state;     // 0x18
    s32 frame;     // 0x1C
    s32 pad20;     // 0x20
    s32 poseId;    // 0x24
    Mtx mtx;       // 0x28
    s32 pad58;     // 0x58
} EffBattenWork;

void effBattenMain(EffectEntry* effect);
void effBattenDisp(CameraId cameraId, EffectEntry* effect);
void effBattenDisp2(CameraId cameraId, EffectEntry* effect);

EffectEntry* effBattenEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffBattenWork* work;
    EffBattenWork* p;
    s32 count;
    s32 i;

    effect = effEntry();
    count = (type == 0) ? 4 : 1;
    effect->type = "Batten";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffBattenWork));
    effect->userdata = work;
    effect->callback = effBattenMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->alpha = 0xFF;
    if (duration <= 0) {
        work->frame = 1000;
    } else {
        work->frame = duration;
    }
    work->poseId = -1;
    PSMTXIdentity(work->mtx);
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->alpha = 0;
        p->scale = 3.0f;
        p->state = 0;
        p->frame = (i - 1) * 10;
    }
    return effect;
}

void effBattenMain(EffectEntry* effect) {
    EffBattenWork* work;
    s32 done;
    s32 type;
    s32 lang;
    s32 i;
    Vec tmp;
    Vec pos = { 0.0f, 0.0f, 0.0f };

    done = 0;
    work = effect->userdata;
    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    tmp = pos;
    type = work->type;
    lang = gp->inBattle != 0;
    switch (type) {
    case 1:
        if (!animGroupBaseAsync("EFF_batten_x", lang, NULL)) {
            return;
        }
        if (work->poseId == -1) {
            work->poseId = animPoseEntry("EFF_batten_x", lang);
            animPoseSetAnim(work->poseId, "A_1", 1);
        }
        break;
    case 2:
        if (!animGroupBaseAsync("EFF_batten_x", lang, NULL)) {
            return;
        }
        if (work->poseId == -1) {
            work->poseId = animPoseEntry("EFF_batten_x", lang);
            animPoseSetAnim(work->poseId, "A_2", 1);
        }
        break;
    case 3:
        if (!animGroupBaseAsync("EFF_batten_rocket", lang, NULL)) {
            return;
        }
        if (work->poseId == -1) {
            work->poseId = animPoseEntry("EFF_batten_rocket", lang);
            animPoseSetAnim(work->poseId, "A_1", 1);
        }
        break;
    }
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->frame = 0x20;
    }
    if (work->frame < 1000) {
        work->frame--;
    }
    if (work->frame < 0) {
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    if (work->frame < 0x20) {
        work->alpha = work->frame << 3;
    }
    if (type == 0) {
        for (i = 1, work++; i < effect->count; i++, work++) {
            switch (work->state) {
            case 0:
                if (--work->frame < 0) {
                    work->frame = 0;
                    work->state++;
                }
                break;
            case 1:
                if (work->frame < 30) {
                    work->scale += (1.0f - work->scale) / 8.0f;
                    work->alpha = (128 - work->alpha) / 8.0f + work->alpha;
                    work->frame++;
                } else {
                    work->frame = (3 - i) * 10;
                    work->state++;
                }
                break;
            case 2:
                if (--work->frame < 0) {
                    work->frame = 0;
                    work->state++;
                }
                break;
            case 3:
                if (work->frame < 30) {
                    work->scale += (10.0f - work->scale) / 8.0f;
                    work->alpha = (-work->alpha) / 8.0f + work->alpha;
                    work->frame++;
                } else {
                    done++;
                }
                break;
            }
        }
        if (done >= effect->count - 1) {
            effDelete(effect);
            return;
        }
        dispEntry(CAMERA_3D, dispCalcZ(&tmp), 2, (DispCallback)effBattenDisp, effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&tmp), 2, (DispCallback)effBattenDisp2, effect);
    }
}

void effBattenDisp(CameraId cameraId, EffectEntry* effect) {
    EffBattenWork* work;
    s32 i;
    CameraEntry* camera;
    s32 alpha;
    f32 neg;
    f32 w;
    f32 half;
    f32 pos;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx view;
    GXTexObj texObj;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    alpha = work->alpha;
    effGetTexObj(0x56, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, scale, scale);
    PSMTXConcat(scale, rot, view);
    PSMTXConcat(camera->view, view, view);
    w = 32.0f;
    half = 0.5f;
    neg = -w * half;
    pos = 16.0f;
    for (i = 1, work++; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, scale, scale);
        PSMTXConcat(view, scale, scale);
        GXLoadPosMtxImm(scale, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xE8, 0xCD, 0x26, alpha * work->alpha / 255 });
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(neg, pos, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(pos, pos, 0.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(pos, neg, 0.0f);
        GXTexCoord2f32(2.0f, 2.0f);
        GXPosition3f32(neg, neg, 0.0f);
        GXTexCoord2f32(0.0f, 2.0f);
    }
}

void effBattenDisp2(CameraId cameraId, EffectEntry* effect) {
    EffBattenWork* work;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    work = effect->userdata;
    if (work->poseId == -1) {
        return;
    }
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(mtx, work->mtx, mtx);
    animPoseSetMaterialFlagOn(work->poseId, 0x40);
    animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
    animPoseMain(work->poseId);
    animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 2.0f);
    animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 2.0f);
    animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 2.0f);
}
