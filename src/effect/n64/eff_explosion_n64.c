#include "eff/eff_n64.h"
#include "eff/n64/eff_explosion_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffExplosionN64Work {
    u16 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 lifetime; // 0x14
    s32 frame;    // 0x18
    s32 maxLife;  // 0x1C
    f32 progress; // 0x20
    f32 rot;      // 0x24
    f32 rotVel;   // 0x28
} EffExplosionN64Work;

static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, -1024, 0, 0, 0, 0, 255},
    {16, 16, 0, -1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 1024, 0, 0, 0, 255},
};

void effExplosionMain(EffectEntry* effect);
void effExplosionDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effExplosionN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffExplosionN64Work* work;

    effect = effEntry();
    effect->type = "ExplosionN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffExplosionN64Work));
    effect->userdata = work;
    effect->callback = effExplosionMain;
    work->lifetime = duration;
    work->frame = 0;
    work->maxLife = duration;
    work->progress = 0.0f;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->rot = 0.0f;
    if (type == 0) {
        work->rotVel = 0.0f;
    } else {
        work->rotVel = 40.0f;
    }
    return effect;
}

void effExplosionMain(EffectEntry* effect) {
    EffExplosionN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->progress = 8.0f * work->frame / work->maxLife;
    work->frame++;
    work->lifetime--;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    work->rot += work->rotVel;
    if (work->type == 0) {
        work->scale += 0.1f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effExplosionDisp, effect);
}

void effExplosionDisp(CameraId cameraId, EffectEntry* effect) {
    s32 last;
    s32 s;
    CameraEntry* camera;
    EffExplosionN64Work* work;
    s32 x;
    s32 type;
    s32 alpha;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx world;
    f32 progress;
    f32 fade;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    type = work->type;
    s = work->progress;
    alpha = 256.0f * (work->progress - s);
    last = s == 7;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, world);
    PSMTXConcat(world, scale, world);
    PSMTXConcat(camera->view, world, world);
    GXLoadPosMtxImm(world, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effGetTexObjN64(14, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    if (type == 0) {
        progress = work->progress;
        if (progress > 5.0f) {
            fade = 1.0f - (progress - 5.0f);
            if (fade < 0.0f) {
                fade = 0.0f;
            }
        } else {
            fade = 1.0f;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){255.0f * fade, 18.0f * fade, 59.0f * fade, 0});
        GXSetTevColor(GX_TEVREG1, (GXColor){255.0f * fade, 53.0f * fade, 24.0f * fade, alpha});
        GXSetTevKColor(GX_KCOLOR0, (GXColor){211.0f * fade, 255.0f * fade, 216.0f * fade, 0});
        GXSetNumTevStages(4);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG2);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C0, GX_CC_C2, GX_CC_C1);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_KONST, GX_CC_C2, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K0);
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0x28, 0x7F});
        GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0x8B, alpha});
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A1, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ONE, GX_CC_C0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    }
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    x = s * 32;
    PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, x, 0.0f, 0.0f);
    PSMTXConcat(scale, mtx, scale);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    if (last) {
        PSMTXScale(scale, 0.0f, 0.0f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
    } else {
        PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, x + 32, 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, scale);
        GXLoadTexMtxImm(scale, GX_TEXMTX1, GX_MTX2x4);
    }
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size32x32_tex32x32_vtx);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
