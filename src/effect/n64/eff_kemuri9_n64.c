// Paper Mario 64 smoke effect 9 (kemuri: smoke): a set of puffs spreading to an end radius over a given life.

#include "version.h"
#include "eff/n64/eff_kemuri9_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffKemuri9N64Work {
    u16 type;        // 0x0
    Vec position;    // 0x4
    f32 dirX;        // 0x10
    f32 velY;        // 0x14
    f32 dirZ;        // 0x18
    f32 radius;      // 0x1C
    f32 radiusEnd;   // 0x20
    s32 timer;       // 0x24
    s32 counter;     // 0x28
    s32 life;        // 0x2C
#if VERSION <= VERSION_DEMO_JP
    f32 anim;
#else
    s32 texFrame;        // 0x30
    s32 texFrameNext;        // 0x34
    s32 frame;       // 0x38
    s32 frameMax;    // 0x3C
#endif
} EffKemuri9N64Work;

#if VERSION <= VERSION_DEMO_JP
static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, -1024, 0, 0, 0, 0, 255},
    {8, 8, 0, -1024, 1024, 0, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 0, 0, 0, 255},
};
static effN64Vtx size32x32_tex32x32_vtx[] = {
    {-16, -16, 0, 0, 0, 0, 0, 0, 255},
    {16, -16, 0, -1024, 0, 0, 0, 0, 255},
    {16, 16, 0, -1024, 1024, 0, 0, 0, 255},
    {-16, 16, 0, 0, 1024, 0, 0, 0, 255},
};
#endif

static void effKemuri9Main(EffectEntry* effect);
static void effKemuri9Disp(CameraId cameraId, EffectEntry* effect);

/**
 * Spawns a ring of count smoke puffs around (x, y, z), starting at the angle,
 * that expand out to radiusEnd while rising, over life frames.
 * Used for Hooktail/Gloomtail/Bonetail leg smoke (type 1) and via evt_eff64 "kemuri9_n64".
 */
EffectEntry* effKemuri9N64Entry(s32 type, s32 count, s32 life, f32 x, f32 y, f32 z, f32 radiusEnd, f32 angle) {
    EffectEntry* effect;
    EffKemuri9N64Work* work;
    EffKemuri9N64Work* p;
    s32 i;
    f32 twoPi;
    f32 zero;
    f32 ang;

    effect = effEntry();
    effect->type = "Kemuri9N64";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, (count + 1) * sizeof(EffKemuri9N64Work));
    effect->userdata = work;
    effect->callback = effKemuri9Main;
    zero = 0.0f;
    twoPi = 6.2831855f;
    work->timer = life;
    work->counter = 0;
    work->life = life;
#if VERSION <= VERSION_DEMO_JP
    work->anim = 0.0f;
#else
    work->texFrame = 0;
#endif
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->radius = 10.0f;
    work->radiusEnd = radiusEnd;
#if VERSION >= VERSION_JP
    work->texFrame = 0;
    work->texFrameNext = 0;
    work->frameMax = life;
    work->frame = 0;
    zero = 0.0f;
    twoPi = 6.2831855f;
#endif
    p = work + 1;
    for (i = 1; i < count + 1; i++, p++) {
        p->position.x = zero;
        p->position.y = zero;
        p->position.z = zero;
        ang = twoPi * (angle + (f32)((i - 1) * 360 / count)) / 360.0f;
        p->dirX = sin(ang);
        p->velY = zero;
        p->dirZ = cos(ang);
    }
    return effect;
}

/** Eases the ring radius toward radiusEnd, lifts each puff, advances the texture animation; deletes at end of life. */
static void effKemuri9Main(EffectEntry* effect) {
    EffKemuri9N64Work* work = effect->userdata;
    s32 i;
    s32 ir;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
#if VERSION <= VERSION_DEMO_JP
    work->anim = 8.0f * (f32)work->counter / (f32)work->life;
#else
    {
        s32 cur = (s32)(16.0f * (f32)work->frame / (f32)work->frameMax);
        s32 next = (s32)(16.0f * (f32)++work->frame / (f32)work->frameMax);
        if (next > 15) {
            next = 15;
        }
        if (cur > 15) {
            cur = 15;
        }
        work->texFrame = cur;
        work->texFrameNext = next;
    }
#endif
    work->counter++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->radius += 0.1f * (work->radiusEnd - work->radius);
    ir = (s32)work->radius;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->velY += 0.04f;
        work->velY *= 0.9f;
        work->position.x = (f32)ir * work->dirX;
        work->position.y += work->velY;
        work->position.z = (f32)ir * work->dirZ;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effKemuri9Disp, effect);
}

#if VERSION <= VERSION_DEMO_JP
/**
 * The early demos draw the smoke as N64 quads, blending two of the 8 frames of texture strip 0xE;
 * JP and later use the 16-cell GX texture 0x28 and draw the quads directly.
 */
static void effKemuri9Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri9N64Work* work = effect->userdata;
    BOOL last;
    s32 i;
    s32 frame;
    s32 u;
    s32 blend;
#else
static void effKemuri9Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri9N64Work* work = effect->userdata;
    s32 i;
    s32 u;
#endif
    s32 type;
    s32 v;
    GXColor color;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx view;
    f32 w;
    f32 half = 0.5f;
    f32 h;

#if VERSION >= VERSION_JP
    u = work->texFrame;
    v = work->texFrameNext;
#endif
    type = work->type;
#if VERSION <= VERSION_DEMO_JP
    frame = (s32)work->anim;
    blend = (s32)(256.0f * (work->anim - (f32)frame));
    last = frame == 7;
    effGetTexObjN64(0xE, &texObj);
#else
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, view);
    effGetTexObj(0x28, &texObj);
#endif
    GXLoadTexObj(&texObj, GX_TEXMAP0);
#if VERSION <= VERSION_DEMO_JP
    if ((s32)type < 2) {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xE6, 0xDE, 0xDE, 0x82});
    } else {
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0x28, 0x0A, 0xA0});
    }
    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, blend});
    GXSetNumChans(0);
#else
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    color = (GXColor){0xE6, 0xDE, 0xDE, 0xFF};
    GXSetChanMatColor(GX_COLOR0A0, color);
#endif
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
#if VERSION >= VERSION_JP
    PSMTXTrans(mtx, 0.0625f * (f32)u, 0.0f, 0.0f);
    PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(mtx, 0.0625f * (f32)v, 0.0f, 0.0f);
    PSMTXScale(scale, 0.0625f, 1.0f, 1.0f);
    PSMTXConcat(mtx, scale, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
#endif
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
#if VERSION <= VERSION_DEMO_JP
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
#else
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
#endif
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
#if VERSION <= VERSION_DEMO_JP
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
#else
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
#endif
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
#if VERSION <= VERSION_DEMO_JP
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
#else
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
#endif
    GXSetCullMode(GX_CULL_NONE);
#if VERSION <= VERSION_DEMO_JP
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, view);
    PSMTXConcat(camera->view, view, view);
    u = frame * 32;
    PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
    PSMTXTrans(mtx, (f32)u, 0.0f, 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    if (last) {
        PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(u + 32), 32.0f, 0.0f);
        PSMTXConcat(scale, mtx, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    } else {
        PSMTXScale(scale, 0.00390625f, 0.03125f, 0.0f);
        PSMTXTrans(mtx, (f32)(u + 32), 0.0f, 0.0f);
        PSMTXConcat(scale, mtx, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    }
#else
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
#endif
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXConcat(view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
#if VERSION <= VERSION_DEMO_JP
        effSetVtxDescN64((s32)type == 1 ? size16x16_tex32x32_vtx : size32x32_tex32x32_vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
#else
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        if (type != 0) {
            h = 16.0f;
            w = h;
        } else {
            h = 32.0f;
            w = h;
        }
        GXPosition3f32(-w * half, h * half, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w * half, h * half, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w * half, -h * half, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w * half, -h * half, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
#endif
    }
}
