#include "eff/eff_queen2.h"
#include "eff/eff_indirect.h"
#include "eff/eff_teresa.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "driver/effdrv.h"

typedef struct EffQueen2Work {
    s32 type;            // 0x0
    Vec position;        // 0x4
    f32 scale;           // 0x10
    f32 baseScale;       // 0x14
    s32 alpha;           // 0x18
    s32 field_0x1C;      // 0x1C
    s32 timer;           // 0x20
    s32 state;           // 0x24
    EffectEntry* charge; // 0x28
} EffQueen2Work;

EffectEntry* effQueen2Entry(s32 type, f32 x, f32 y, f32 z) {
    EffQueen2Work* work;
    EffectEntry* effect;

    effect = effEntry();
    effect->type = "Queen2";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffQueen2Work) * effect->count);
    effect->userdata = work;
    effect->callback = effQueen2Main;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    {
        Vec position = { 0.0f, 0.0f, 0.0f };

        position.x = x;
        position.y = y;
        position.z = z;
        work->baseScale = PSVECDistance(&camGetPtr(CAMERA_3D)->cameraPos, &position) / 10.0f;
    }
    work->scale = work->baseScale;
    work->alpha = 0;
    work->state = 0;
    work->timer = 0;
    work->charge = NULL;
    return effect;
}

void effQueen2Main(EffectEntry* effect) {
    EffQueen2Work* work = effect->userdata;
    Vec position;

    position = (Vec){ work->position.x, work->position.y, work->position.z };

    // soft delete
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (work->charge) {
            effDelete(work->charge);
        }
        effDelete(effect);
        return;
    }
    switch (work->state) {
        case 0:
            work->alpha = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 128.0f, work->timer++, 60);
            if (work->timer > 60) {
                work->timer = 0;
                work->state++;
            }
            break;
        case 1: {
                f32 base = work->baseScale;
                s32 t = work->timer++;
                work->scale = intplGetValue(EASING_SINE_IN_OUT, base, 3.0f, t, 120);
            }
            if (work->timer > 120) {
                work->timer = 0;
                work->state++;
                work->charge = effAkariChargeN64Entry(0, 0, work->position.x, work->position.y,
                                                      work->position.z, 1.0f);
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].r = 0;
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].g = 0x9A;
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].b = 0xFF;
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].r2 = 0xFF;
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].g2 = 0xFF;
                ((EffAkariChargeN64Work*)work->charge->userdata)->elem[0].b2 = 0xFF;
            }
            break;
        case 2:
            if (++work->timer > 120) {
                work->timer = 0;
                work->state++;
                effTeresaEntry(1, work->position.x, work->position.y, work->position.z);
                effIndirectEntry(1, 60, work->position.x, work->position.y, work->position.z);
            }
            break;
        case 3:
            if (++work->timer > 30) {
                work->timer = 0;
                work->state++;
                effTeresaEntry(1, work->position.x, work->position.y, work->position.z);
                effIndirectEntry(1, 60, work->position.x, work->position.y, work->position.z);
                effSoftDelete(work->charge);
            }
            break;
        case 4:
            effDelete(effect);
            return;
    }
    if (work->state >= 3) {
        work->alpha -= 2;
        if (work->alpha < 0) {
            work->alpha = 0;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effQueen2Disp, effect);
}

void effQueen2Disp(CameraId cameraId, EffectEntry* effect) {
    EffQueen2Work* work = effect->userdata;
    CameraEntry* camera;
    Mtx trans;
    Mtx mtx;

    camera = camGetPtr(cameraId);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, work->alpha });
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXScale(mtx, 10.0f * work->scale, 10.0f * work->scale, 10.0f * work->scale);
    PSMTXConcat(trans, mtx, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXDrawSphere(16, 16);
}
