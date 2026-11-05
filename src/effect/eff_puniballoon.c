// Puni balloon (puni: the Punies): a wobbling balloon (type 0) that pops into a burst of small pieces (type 1).

#include "driver/camdrv.h"
#include "eff/eff_puniballoon.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

static void effPuniBalloonMain(EffectEntry* effect);
static void effPuniBalloonDisp(CameraId cameraId, void* param);
static void balloon_1(CameraId cameraId, EffectEntry* effect);
static void balloon_0(CameraId cameraId, EffectEntry* effect);

/**
 * Spawns a Puni balloon (type 0) or its pop burst (type 1) at (x, y, z) scaled by `scale`;
 * used by the Great Tree (mri) Puni scenes and evt_eff ("puni_balloon").
 */
EffectEntry* effPuniBalloonEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    PuniBalloonWork* work;
    PuniBalloonWork* p;
    s32 i;
    s32 n;

    effect = effEntry();
    n = 1;
    switch (type) {
    case 0:
        n = 1;
        break;
    case 1:
        n = 11;
        break;
    }
    effect->type = "PuniBalloon";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(PuniBalloonWork));
    effect->userdata = work;
    effect->callback = effPuniBalloonMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->rotation = 0.0f;
    work->life = type == 0 ? 1000 : 30;
    work->frame = 0;
    work->scale = 0.0f;
    work->baseScale = scale;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->scale = scale;
        p->velocity.x = (f32)(rand() % 11 - 5);
        p->velocity.y = (f32)(rand() % 11 - 5);
        p->velocity.z = (f32)(rand() % 11 - 5);
    }
    return effect;
}

/**
 * Per frame: advances the frame counter and the burst pieces; a balloon (type 0) lasts until soft-deleted, then
 * pops 16 frames later into a type 1 burst, which ends after 30 frames; registers the draw callback.
 */
static void effPuniBalloonMain(EffectEntry* effect) {
    PuniBalloonWork* work = effect->userdata;
    Vec pos;
    Vec tmp;
    s32 type;
    s32 i;

    tmp = (Vec){0.0f, 0.0f, 0.0f};
    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    pos = tmp;
    type = work->type;

    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->frame++;
    if (work->life < 0) {
        if (type == 0) {
            effPuniBalloonEntry(1, work->position.x, work->position.y, work->position.z, 1.0f);
        }
        effDelete(effect);
        return;
    }
    work->scale += (1.0f - work->scale) / 10.0f;
    work->rotation = 20.0f * (f32)sin(work->frame / 20.0f);
    for (i = 1, work++; i < effect->count; i++, work++) {
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->velocity.y += -0.1f;
        work->velocity.x *= 0.98f;
        work->velocity.y *= 0.98f;
        work->velocity.z *= 0.98f;
        work->scale *= 0.9f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&pos), DISP_MODE_XLU, effPuniBalloonDisp, effect);
}

/** Draw callback: dispatches to balloon_0 (the balloon) or balloon_1 (the burst) by type. */
static void effPuniBalloonDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    PuniBalloonWork* work = effect->userdata;
    if (work->type == 0) {
        balloon_0(cameraId, effect);
    } else {
        balloon_1(cameraId, effect);
    }
}

/** Draws the wobbling balloon (scale pulses with a sine) as a camera-facing textured quad (effect texture 0x26). */
static void balloon_0(CameraId cameraId, EffectEntry* effect) {
    PuniBalloonWork* work = effect->userdata;
    CameraEntry* cam = camGetPtr(cameraId);
    f32 scale;
    f32 sv;
    Mtx mT, mR, mS, mX;
    GXTexObj texObj;

    scale = work->scale;
    sv = (f32)sin((f32)work->frame / 10.0f);
    scale += 0.1f * sv;
    scale *= work->baseScale;
    PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(mR, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mT, mR, mX);
    PSMTXConcat(cam->view, mX, mT);
    PSMTXRotRad(mR, 'z', MTXDegToRad(work->rotation));
    PSMTXScale(mS, scale, scale, scale);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mT);
    GXLoadPosMtxImm(mT, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1_2);
    effGetTexObj(0x26, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-12, 12, 0);
    GXTexCoord2s16(0, 0);
    GXPosition3s16(-12, -12, 0);
    GXTexCoord2s16(0, 1);
    GXPosition3s16(12, -12, 0);
    GXTexCoord2s16(2, 1);
    GXPosition3s16(12, 12, 0);
    GXTexCoord2s16(2, 0);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
}

/** Draws the burst of small pieces after the balloon pops, using the same texture. */
static void balloon_1(CameraId cameraId, EffectEntry* effect) {
    PuniBalloonWork* work = effect->userdata;
    CameraEntry* cam = camGetPtr(cameraId);
    s32 i;
    Mtx mC, mA, mB, mD;
    GXTexObj texObj;

    PSMTXTrans(mA, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(mB, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mA, mB, mC);
    PSMTXConcat(cam->view, mC, mC);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1_2);
    effGetTexObj(0x26, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    for (i = 1, work++; i < effect->count; i++, work++) {
        PSMTXTrans(mA, work->position.x, work->position.y, work->position.z);
        PSMTXScale(mD, work->scale, work->scale, work->scale);
        PSMTXConcat(mA, mD, mA);
        PSMTXConcat(mC, mA, mA);
        GXLoadPosMtxImm(mA, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(-12, 12, 0);
        GXTexCoord2s16(0, 0);
        GXPosition3s16(-12, -12, 0);
        GXTexCoord2s16(0, 1);
        GXPosition3s16(12, -12, 0);
        GXTexCoord2s16(2, 1);
        GXPosition3s16(12, 12, 0);
        GXTexCoord2s16(2, 0);
    }
}
