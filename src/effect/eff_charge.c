#pragma defer_codegen off
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffChargeWork {
    s32 state;    // 0x0
    s32 type;     // 0x4
    Vec position; // 0x8
    s32 alpha;    // 0x14
    s32 duration; // 0x18
    s32 frame;    // 0x1C
    f32 yaw;      // 0x20
    f32 pitch;    // 0x24
    f32 scale;    // 0x28
    f32 dist;     // 0x2C
    s32 r0;       // 0x30
    s32 g0;       // 0x34
    s32 b0;       // 0x38
    s32 r1;       // 0x3C
    s32 g1;       // 0x40
    s32 b1;       // 0x44
} EffChargeWork;

void effChargeDisp(CameraId cameraId, EffectEntry* effect);
void effChargeMain(EffectEntry* effect);

void effChargeDisp(CameraId cameraId, EffectEntry* effect) {
    EffChargeWork* work = effect->userdata;
    s32 i;
    s32 s0 = work->r0, s1 = work->g0, s2 = work->b0;
    Mtx pmtx;
    Mtx rot;
    Mtx smtx;
    Mtx vmtx;
    Mtx mtx;
    GXTexObj texObj;

    PSMTXScale(mtx, work->scale, work->scale, 1.0f);
    PSMTXTransApply(mtx, mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', -camGetPtr(cameraId)->viewYaw * 0.017453292f);
    PSMTXConcat(mtx, rot, mtx);
    effGetTexObj(30, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    {
        GXSetTevColor(GX_TEVREG1, (GXColor){ work->r1, work->g1, work->b1, 0 });
    }
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        f32 ps = work->scale;
        if (ps) {
            {
                GXSetTevColor(GX_TEVREG0, (GXColor){ s0, s1, s2, work->alpha });
            }
            PSMTXTrans(pmtx, work->position.x, work->position.y, work->position.z);
            PSMTXScale(smtx, ps, ps, ps);
            PSMTXConcat(pmtx, smtx, pmtx);
            PSMTXConcat(mtx, pmtx, pmtx);
            PSMTXConcat(camGetPtr(cameraId)->view, pmtx, vmtx);
            GXLoadPosMtxImm(vmtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            if (!(i & 1)) {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(-8.0f, -8.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(8.0f, -8.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(8.0f, -24.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(-8.0f, -24.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
            } else {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(-12.0f, -12.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(12.0f, -12.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(12.0f, -36.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(-12.0f, -36.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
            }
        }
    }
}

void effChargeMain(EffectEntry* effect) {
    s32 frame;
    s32 odd;
    EffChargeWork* work = effect->userdata;
    s32 i;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;

    if (work->duration < 1000) {
        work->duration--;
    }
    work->frame++;
    if (work->duration < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (frame <= 20) {
        work->alpha = frame * 12;
    } else {
        work->alpha = 255;
    }
    if (frame & 1) {
        work->alpha /= 2;
    }
    odd = work->frame & 1;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        f32 sa, ca, sb, cb, b;
        f32 pt;
        f32 a = 6.2831855f * work->yaw / 360.0f;
        f32 d = work->dist;
        pt = work->pitch;
        sa = sin(a);
        ca = cos(a);
        b = 6.2831855f * pt / 360.0f;
        sb = sin(b);
        cb = cos(b);

        work->position.x = cb * (d * sa);
        work->position.y = cb * (d * ca);
        work->position.z = d * sb;
        work->alpha = (s32)(100.0f - work->dist) * 510 / 100;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
        work->scale = 0.2f + work->dist / 80.0f;
        work->dist -= (f32)((i & 3) * 2 + 2);
        if (work->dist < 0.0f) {
            work->dist = 0.0f;
            work->scale = 0.0f;
        }
        work->yaw += (f32)(odd ? 5 : 2);
        work->pitch += (f32)(odd ? 2 : 5);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effChargeDisp, effect);
}

EffectEntry* effChargeEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffChargeWork* work;
    EffChargeWork* p;
    s32 i;

    effect = effEntry();
    effect->type = "Charge";
    effect->count = 31;
    work = __memAlloc(HEAP_EFFECT, 0x8b8);
    effect->userdata = work;
    effect->callback = effChargeMain;
    work->type = type;
    work->frame = 0;
    work->duration = duration;
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    switch (type) {
        case 0:
            work->r0 = 255;
            work->g0 = 0;
            work->b0 = 255;
            work->r1 = 255;
            work->g1 = 255;
            work->b1 = 255;
            break;
        case 1:
            work->r0 = 255;
            work->g0 = 20;
            work->b0 = 33;
            work->r1 = 50;
            work->g1 = 20;
            work->b1 = 10;
            break;
        case 2:
        default:
            work->r0 = 0;
            work->g0 = 0;
            work->b0 = 0;
            work->r1 = 255;
            work->g1 = 255;
            work->b1 = 255;
            break;
    }
    for (i = 1, p = work + 1; i <= 30; i++, p++) {
        p->yaw = 9.0f * (360.0f * (i - 1)) / 30.0f;
        p->pitch = 0.0f;
        p->dist = (i - 1) * 100 / 30;
        p->state = 0;
    }
    return effect;
}
