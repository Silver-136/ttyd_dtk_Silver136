#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include "eff/eff_indirect.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffIndirectWork {
    s32 type;          // 0x0
    Vec position;      // 0x4
    f32 field_0x10;    // 0x10
    f32 field_0x14;    // 0x14
    f32 rotZ;          // 0x18
    f32 scale;         // 0x1C
    u8 alpha;          // 0x20
    u8 field_0x21[7];  // 0x21
    s32 timer;         // 0x28, counts down while fading out
    s32 field_0x2C;    // 0x2C
    f32 field_0x30;    // 0x30
    smartEntry* work;  // 0x34
} EffIndirectWork;

EffectEntry* effIndirectEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffIndirectWork* work;

    effect = effEntry();
    effect->type = "Indirect";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffIndirectWork));
    effect->userdata = work;
    effect->callback = effIndirectMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.0f;
    work->alpha = 255;
    work->work = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), 0);
    work->field_0x10 = 0.0f;
    work->field_0x14 = 0.0f;
    work->rotZ = 0.0f;
    work->timer = duration;
    work->field_0x2C = 0;
    work->field_0x30 = -0.05f;
    return effect;
}

void effIndirectMain(EffectEntry* effect) {
    EffIndirectWork* work = effect->userdata;
    Vec position;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    switch (work->type) {
        case 0:
        case 1:
            // soft delete: start fading out
            if (effect->flags & 4) {
                effect->flags &= ~4;
                work->timer = 16;
            }
            if (work->timer < 1000) {
                work->timer--;
            }
            if (work->timer < 0) {
                smartFree(work->work);
                effDelete(effect);
                return;
            }
            if (work->timer < 32) {
                work->alpha = work->timer * 8;
            }
#ifdef VERSION_G8ME01
            work->rotZ += -300.0f / (f32)gp->framerate;
            work->scale += 60.0f / (f32)gp->framerate;
#else
            work->rotZ += -5.0f;
            work->scale += 1.0f;
#endif
            break;
    }
    dispEntry(CAMERA_3D_EFFECTB, dispCalcZ(&position), 2, (DispCallback)effIndirectDisp, effect);
}

void effIndirectDisp(CameraId cameraId, EffectEntry* effect) {
    EffIndirectWork* work = effect->userdata;
    CameraEntry* camera;
    s32 type;
    u8 alpha;
    void* buffer;
    f32 u, v, size, half;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx final;
    GXTexObj texObj;

    camera = camGetPtr(CAMERA_2D);
    type = work->type;
    alpha = work->alpha;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    switch (type) {
        case 0:
#ifdef VERSION_G8ME01
            effGetTexObj(0x6A, &texObj);
#else
            effGetTexObj(0x86, &texObj);
#endif
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 1:
            effGetTexObj(0x45, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
    }

    // copy the framebuffer for the distortion
    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(304, 240, GX_TF_RGB565, GX_TRUE);
    buffer = smartAlloc(GXGetTexBufferSize(304, 240, GX_TF_RGB565, GX_FALSE, 0), 3)->address;
    GXCopyTex(buffer, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&texObj, buffer, 304, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&texObj, GX_TEXMAP1);

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camera->viewYaw));
    PSMTXConcat(mtx, scale, scale);
    PSMTXConcat(scale, rot, final);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->rotZ));
    PSMTXConcat(final, rot, final);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    C_MTXLightPerspective(mtx, 25.0f, 1.2666667f, 0.5f, -0.5f, 0.505f, 0.505f);
    PSMTXConcat(mtx, camera->view, mtx);
    PSMTXConcat(mtx, final, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX3x4);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXConcat(camera->view, final, final);
    GXLoadPosMtxImm(final, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    v = u = 1.0f;
    switch (type) {
        case 0:
            u = 2.0f;
            break;
        case 1:
            v = u = 2.0f;
            break;
    }
    size = 32.0f;
    half = 16.0f;
    GXPosition3f32(-size / 2, half, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(half, half, 0.0f);
    GXTexCoord2f32(u, 0.0f);
    GXPosition3f32(half, -size / 2, 0.0f);
    GXTexCoord2f32(u, v);
    GXPosition3f32(-size / 2, -size / 2, 0.0f);
    GXTexCoord2f32(0.0f, v);
    GXEnd();
}
