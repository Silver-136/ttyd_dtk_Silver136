#include "eff/eff_sleep.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "mario/mariost.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
// US effect texture ids are 0x1C lower for these entries.
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))


//local prototypes
void effSleepDisp(CameraId camId, EffectEntry* effect);

EffectEntry* effSleepEntry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale, f32 rotation) {
    EffectEntry* effect;
    s32 smth;
    effSleepData* data;
    f32 radians;
    int i;
    f32 r;

    effect = effEntry();
    effect->type = "Sleep";
    effect->count = 4;
    data = __memAlloc(HEAP_EFFECT, effect->count * sizeof(effSleepData));
    effect->userdata = data;
    effect->callback = effSleepMain;
    effect->flags |= 2;
    data->type = type;
    data->position.x = x;
    data->position.y = y;
    data->position.z = z;
    data->scale = scale;
    data->alpha = 0;
    if (lifetime <= 0) {
        data->lifetime = 1000;
    } else {
        data->lifetime = lifetime;
    }
    data->age = 0;

    radians = (6.2831855f * rotation) / 360.0f;
    for (data++, i = 1, smth = 16; i < effect->count; i++, smth += 16, data++) {
        data->position.x = (r = 16.0f * (f32)i) * (f32)cos(radians);
        data->position.y = r * (f32)sin(radians);
        data->position.z = 0.0f;
        data->scale = (0.2f * (f32)i) + 1.0f;
        data->alpha = 0;
        data->delay = smth;
    }
    return effect;
}

void effSleepMain(EffectEntry* effect) {
    effSleepData* data = effect->userdata;
    int i;
    Vec position;

    position = (Vec){ data->position.x, data->position.y, data->position.z };
    if (effect->flags & 4) {
        effect->flags &= ~4;
        data->lifetime = 16;
    }
    if (data->lifetime < 1000) {
        data->lifetime--;
    }
    if (data->lifetime < 32) {
        data->alpha = data->lifetime * 8;
    }
    if (data->lifetime < 0) {
        effDelete(effect);
        return;
    }
    data->age++;
    if (data->age < 32) {
        data->alpha = data->age * 8 + 7;
    }
    for (i = 1, data++; i < effect->count; i++, data++) {
        if (data->delay != 0) {
            data->delay--;
        } else {
            data->alpha += 8;
            if (data->alpha > 255) {
                data->alpha = 255;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effSleepDisp, effect);
}

#pragma push

// FAKE: same values as the plain form, written so the code generator sees the target's shapes:
// - sin argument via `(t = tau * x, t *= 0.015625f)`: a compound assignment skips the front end's
//   commutative swap (CExpr optimizecomm), so the constant stays the right operand;
// - 0.5f / 0.015625f / 1.0f are literals (LICM-hoisted temps in first-use order), xh = x0 * 0.5f is
//   computed in the inner loop body, and the y products go through sleepPos so `1.0f * y` is not
//   folded away by the front end (CExpr_IsOne).
static inline void sleepPos(f32 x, f32 one, f32 y) { GXPosition3f32(x, one * y, 0.0f); }
void effSleepDisp(CameraId camId, EffectEntry* effect) {
    s32 off;
    f32 tau;
    f32 x0;
    Mtx mtx;
    Mtx scale;
    s32 base;
    Mtx rot;
    Mtx res;
    GXTexObj texObj;
    f32 w;
    CameraEntry* cam;
    effSleepData* data;
    int i;
    s32 j;
    s32 alpha;
    f32 xh;
    f32 s0, s1;
    f32 t;

    data = effect->userdata;
    cam = camGetPtr(camId);
    alpha = data->alpha;

    PSMTXTrans(mtx, data->position.x, data->position.y, data->position.z);
    PSMTXScale(scale, data->scale, data->scale, data->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(camId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, res);
    PSMTXConcat(cam->view, res, res);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    effGetTexObj(EFF_TEX(0x8C), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    w = 8.0f;
    x0 = -w;

    for (i = 1, data++, base = 50; i < effect->count; base += 50, i++, data++) {
        if (data->delay == 0) {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, data->alpha * alpha / 255 });
            PSMTXTrans(mtx, data->position.x, data->position.y, data->position.z);
            PSMTXScale(scale, data->scale, data->scale, data->scale);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(res, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            tau = 6.2831855f;
            j = 0;
            off = 0;
            while (j < 8) {
                s0 = 0.5f * (f32)sin((t = (tau * (f32)(u32)(gp->retraceCount + base + off)), t *= 0.015625f));
                s1 = 0.5f * (f32)sin((t = (tau * (f32)(u32)(gp->retraceCount + base + (j + 1) * 10)), t *= 0.015625f));
                xh = x0;
                xh *= 0.5f;
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                sleepPos(xh + s1, 1.0f, j + 1);
                GXTexCoord2f32(0.0f, 0.125f * (f32)(7 - j));
                sleepPos(4.0f + s1, 1.0f, j + 1);
                GXTexCoord2f32(1.0f, 0.125f * (f32)(7 - j));
                sleepPos(4.0f + s0, 1.0f, j);
                GXTexCoord2f32(1.0f, 0.125f * (f32)(8 - j));
                sleepPos(xh + s0, 1.0f, j);
                GXTexCoord2f32(0.0f, 0.125f * (f32)(8 - j));
                j++;
                off += 10;
            }
        }
    }
}




#pragma pop

