#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"

typedef struct EffAkariSflushN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    s32 lifetime; // 0x10
    s32 frame;    // 0x14
    s32 r;        // 0x18
    s32 g;        // 0x1C
    s32 b;        // 0x20
    s32 alpha;    // 0x24
    f32 size;     // 0x28
    f32 scale;    // 0x2C
    s32 count;    // 0x30
} EffAkariSflushN64Work;

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {16, 0, 0, 0, 0, 0, 0, 0, 255},
    {3, 4, 0, 0, 0, 0, 0, 0, 255},
    {0, 8, 0, 0, 0, 0, 0, 0, 255},
    {0, -8, 0, 0, 0, 0, 0, 0, 255},
    {-3, -4, 0, 0, 0, 200, 200, 200, 255},
    {-16, 0, 0, 0, 0, 255, 255, 255, 255},
};

static const GXColor kWhite = {255, 255, 255, 0};

void effAkariSflushMain(EffectEntry* effect);
void effAkariSflushDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effAkariSflushN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 size) {
    EffectEntry* effect;
    EffAkariSflushN64Work* work;

    effect = effEntry();
    effect->type = "AkariSflushN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffAkariSflushN64Work));
    effect->userdata = work;
    effect->callback = effAkariSflushMain;
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
    work->size = size;
    work->r = 255;
    work->g = 230;
    work->b = 50;
    work->count = 0;
    work->scale = 1.0f;
    return effect;
}

void effAkariSflushMain(EffectEntry* effect) {
    EffAkariSflushN64Work* work = effect->userdata;
    s32 lifetime;
    s32 frame;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    lifetime = work->lifetime;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (lifetime < 8) {
        work->scale += 0.3f * (8 - lifetime);
    }
    if (frame < 16) {
        work->alpha = frame * 16 + 15;
    }
    if (lifetime < 8) {
        work->alpha = lifetime * 32 + 31;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effAkariSflushDisp, effect);
}

void effAkariSflushDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffAkariSflushN64Work* work;
    s32 i;
    s32 alpha;
    s32 frame;
    s32 j;
    f32 s;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx viewMtx;
    Mtx m44;
    Mtx m14;
    GXColor color2;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->size, work->size, work->size);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, viewMtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetNumTexGens(0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, alpha});
    color2 = kWhite;
    GXSetTevColor(GX_TEVREG1, color2);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    for (i = 0; i < work->count + 1; i++) {
        s = (f32)cos(6.2831855f * (f32)(frame * 8 + (i * 180) / (work->count + 1)) / 360.0f);
        s = 0.1f * s + (f32)(i + 1);
        s *= work->scale;
        PSMTXRotRad(rot, 'z', 0.017453292f * (f32)(i * 36));
        PSMTXScale(scale, s, s, s);
        PSMTXConcat(rot, scale, m44);
        for (j = 0; j < 5; j++) {
            PSMTXRotRad(rot, 'z', 0.017453292f * (f32)(90 + j * 72));
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
            PSMTXConcat(rot, scale, rot);
            PSMTXTrans(mtx, 30.0f, 0.0f, 0.0f);
            PSMTXConcat(rot, mtx, mtx);
            PSMTXRotRad(rot, 'x', 0.0f);
            PSMTXConcat(mtx, rot, m14);
            PSMTXConcat(viewMtx, m44, mtx);
            PSMTXConcat(mtx, m14, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0, 1, 2, 0, 1, 2, 3, 0);
            tri2(2, 3, 4, 0, 3, 4, 5, 0);
        }
    }
}

