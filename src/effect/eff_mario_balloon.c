#include "driver/camdrv.h"
#include "eff/eff_mario_balloon.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "driver/npcdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

void effMarioBalloonMain(EffectEntry* effect);
void effMarioBalloonDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effMarioBalloonEntry(s32 type, NpcEntry* npc) {
    EffectEntry* effect;
    MarioBalloonWork* work;

    effect = effEntry();
    effect->type = "MarioBalloon";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(MarioBalloonWork));
    effect->userdata = work;
    effect->callback = effMarioBalloonMain;
    work->type = type;
    work->rotation = 0.0f;
    work->npc = npc;
    work->camera = 4;
    switch (type) {
    case 0:
        work->scale = 0.4f;
        break;
    case 1:
        work->scale = 0.8f;
        break;
    }
    return effect;
}

void effMarioBalloonMain(EffectEntry* effect) {
    MarioBalloonWork* work = effect->userdata;
    NpcEntry* npc;
    Vec pos;

    npc = work->npc;
    if (npc == NULL) {
        MarioWork* mario = marioGetPtr();
        work->position.x = mario->position.x;
        work->position.y = 10.0f + (mario->position.y + mario->wPlayerCollisionBox.y);
        work->position.z = mario->position.z;
    } else {
        work->position.x = npc->position.x;
        work->position.y = 10.0f + (npc->position.y + npc->height);
        work->position.z = npc->position.z;
    }
    work->rotation += 4.0f;
    pos = (Vec){0.0f, 0.0f, 0.0f};
    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    dispEntry(work->camera, dispCalcZ(&pos), 8, (DispCallback)effMarioBalloonDisp, effect);
}

void effMarioBalloonDisp(CameraId cameraId, EffectEntry* effect) {
    MarioBalloonWork* work = effect->userdata;
    Mtx mT, mR, mS, mTA, mN;
    GXTexObj texObj;

    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(mT, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(mR, 'y', 0.017453292f * (work->rotation - camGetPtr(cameraId)->viewYaw));
    {
        f32 s = 0.7f * work->scale;
        PSMTXScale(mS, s, s, s);
    }
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mT);
    PSMTXConcat(camGetPtr(cameraId)->view, mT, mT);
    GXSetCurrentMtx(GX_PNMTX0);
    switch (work->type) {
    case 0:
        effGetTexObj(15, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        PSMTXTransApply(mT, mTA, 1.0f, -1.0f, 0.0f);
        GXLoadPosMtxImm(mTA, GX_PNMTX0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 1.0f);
        GXPosition3f32(-16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        effGetTexObj(14, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, 255});
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXLoadPosMtxImm(mT, GX_PNMTX0);
        PSMTXInvXpose(mT, mN);
        GXLoadNrmMtxImm(mN, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 128, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetCullMode(GX_CULL_BACK);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 1.0f);
        GXPosition3f32(-16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXSetTevColor(GX_TEVREG0, (GXColor){160, 160, 160, 255});
        GXSetCullMode(GX_CULL_FRONT);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(16.0f, 64.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(2.0f, 1.0f);
        GXPosition3f32(-16.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        break;
    case 1:
        effGetTexObj(17, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        PSMTXTransApply(mT, mTA, 1.0f, -1.0f, 0.0f);
        GXLoadPosMtxImm(mTA, GX_PNMTX0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        effGetTexObj(16, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, 255});
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXLoadPosMtxImm(mT, GX_PNMTX0);
        PSMTXInvXpose(mT, mN);
        GXLoadNrmMtxImm(mN, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetCullMode(GX_CULL_BACK);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXSetTevColor(GX_TEVREG0, (GXColor){160, 160, 160, 255});
        GXSetCullMode(GX_CULL_FRONT);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(20.0f, 32.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-20.0f, 0.0f, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        break;
    }
}
