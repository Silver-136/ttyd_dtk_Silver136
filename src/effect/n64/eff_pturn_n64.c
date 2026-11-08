// Paper Mario 64 effect: a bobbing counter sprite whose digit steps toward the count given as `targetCount` (one step
// every 8 frames); a step down would drop a type-10 copy that falls away and fades, but the count starts at 0 and
// the target is fixed at entry, so it only counts up. Type 1 shows no digit.

#include "eff/n64/eff_pturn_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_n64.h"
#include "libc_decls.h"

typedef struct PturnElem {
    s32 type;   // 0x0
    s32 active; // 0x4
    Vec pos;    // 0x8
    Vec vel;    // 0x14
    f32 bobY;    // 0x20
    s32 alpha;  // 0x24
    s32 age;    // 0x28
    s32 frame;  // 0x2C
    f32 scale;  // 0x30
    f32 scale2; // 0x34
    s32 shownCount;     // 0x38
    s32 targetCount;     // 0x3C
    f32 rot;    // 0x40
    s32 delay;  // 0x44
} PturnElem;

/**
 * Size of the counter badge over its first frames: it shrinks in from five times its size
 * and settles with a bounce.
 */
f32 scale_dat[9] = {5.0f, 4.5f, 4.0f, 3.0f, 2.0f, 1.0f, 0.7f, 1.2f, 1.0f};

/** 48x48 quad drawn for the badge, its texture coordinates turn the texture 45 degrees. */
effN64Vtx size48x48_tex32x32_pturn_vtx[] = {
    {-24, -24, 0, -512, 512, 255, 255, 255, 255},
    {24, -24, 0, 512, -512, 255, 255, 255, 255},
    {24, 24, 0, 1536, 512, 255, 255, 255, 255},
    {-24, 24, 0, 512, 1536, 255, 255, 255, 255},
};

/** Yellow 24x24 quad for a single-digit count over the badge. */
effN64Vtx size24x24_tex32x32_pturn_vtx[] = {
    {-12, -12, 0, 0, 0, 250, 250, 0, 255},
    {12, -12, 0, 1024, 0, 250, 250, 0, 255},
    {12, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-12, 12, 0, 0, 1024, 250, 250, 0, 255},
};

/** Yellow quad for the left-hand digit of a two-digit count. */
effN64Vtx size16x24_tex32x32_l_pturn_vtx[] = {
    {-16, -12, 0, 0, 0, 250, 250, 0, 255},
    {4, -12, 0, 1024, 0, 250, 250, 0, 255},
    {4, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-16, 12, 0, 0, 1024, 250, 250, 0, 255},
};

/** Yellow quad for the right-hand digit of a two-digit count. */
effN64Vtx size16x24_tex32x32_r_pturn_vtx[] = {
    {-4, -12, 0, 0, 0, 250, 250, 0, 255},
    {16, -12, 0, 1024, 0, 250, 250, 0, 255},
    {16, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-4, 12, 0, 0, 1024, 250, 250, 0, 255},
};

static void effPturnMain(EffectEntry* effect);
static void effPturnDisp(CameraId cameraId, EffectEntry* effect);
static void pturn_main_dispp(CameraId cameraId, EffectEntry* effect);
static void pturn_num_dispp(CameraId cameraId, EffectEntry* effect);

/**
 * A bobbing badge (N64 texture 0x92) with a one- or two-digit count (texture 0x53) above (x, y, z);
 * the effect table labels it 行動不能 ("unable to act").
 * Type 0 steps its displayed count toward targetCount, type 1 shows the badge without digits,
 * and type 10 is the short-lived badge that type 0 throws off when the count falls.
 * Reached only through evt_eff64 "pturn_n64"; no script in the tree spawns it.
 * Guess: the immobilised-status icon with its turns left.
 */
EffectEntry* effPturnN64Entry(s32 type, s32 targetCount, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    PturnElem* work;

    effect = effEntry();
    effect->type = "PturnN64";
    effect->count = 2;
    work = __memAlloc(HEAP_EFFECT, sizeof(PturnElem) * 2);
    effect->userdata = work;
    effect->callback = effPturnMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->scale2 = 1.0f;
    if (type == 1) {
        work->shownCount = 1;
        work->targetCount = 0;
    } else {
        work->shownCount = 0;
        work->targetCount = targetCount;
    }
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    switch (type) {
        case 0:
            work->age = 100;
            break;
        case 1:
            work->age = 100;
            break;
        case 10:
            work->age = 21;
            break;
    }
    work->frame = 0;
    work->delay = 0;
    work->active = 1;
    work[1].pos.z = 0.0f;
    work[1].pos.y = 0.0f;
    work[1].pos.x = 0.0f;
    work[1].bobY = 0.0f;
    work[1].vel.z = 0.0f;
    work[1].vel.y = 0.0f;
    work[1].vel.x = 0.0f;
    work[1].rot = 0.0f;
    work[1].age = 0;
    work[1].alpha = 255;
    return effect;
}

/**
 * Steps the displayed count toward its target (one per 8 frames), bobs the badge,
 * and on delete tumbles it away with gravity while fading.
 */
static void effPturnMain(EffectEntry* effect) {
    PturnElem* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 targetCount;
    s32 frame;
    s32 shownCount;
    s32 alpha;
    s32 life;
    s32 type;
    f32 a;
    f32 b;
    f32 c;
    f32 scl;

    tmp.x = work->pos.x;
    tmp.y = work->pos.y;
    tmp.z = work->pos.z;
    position = tmp;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->age = 21;
    }
    if (work->age < 100) {
        work->age--;
    }
    if (work->age < 0) {
        effDelete(effect);
        return;
    }
    work->frame++;
    if (work->frame > 324000) {
        work->frame = 256;
    }
    type = work->type;
    life = work->age;
    frame = work->frame;
    targetCount = work->targetCount;
    shownCount = work->shownCount;
    scl = work->scale2;
    if (type == 0) {
        if (work->delay > 0) {
            work->delay--;
        } else if (targetCount != shownCount) {
            if (targetCount < 0) {
                targetCount = 0;
            }
            if (targetCount > shownCount) {
                alpha = 60;
                if (type == 0) {
                    alpha = 100;
                }
                work->age = alpha;
                work->frame = 1;
                work->shownCount++;
                work[1].age = 0;
            } else if (targetCount < shownCount) {
                effPturnN64Entry(10, 0, work->pos.x, work->pos.y + work[1].bobY, work->pos.z);
                work->shownCount--;
            }
            work->delay = 8;
        }
    }
    a = 6.2831855f * (f32)(frame * 12);
    b = 0.05f * (f32)life;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        s32 f2;
        s32 age = work->age;
        f32 y;
        work->scale = (age >= 9 || type == 10) ? 1.0f : scale_dat[age];
        work->scale = work->scale * scl;

        // FAKE: spare flag local for the condition; testing it directly changes the object
        f2 = (type == 0 || type == 1);
        if (f2) {
            c = (f32)cos(a / 360.0f);
            y = 4.0f * c;
        } else {
            y = 0.0f;
        }
        work->bobY = y;
        work->pos.x = work->pos.x + work->vel.x;
        work->pos.y = work->pos.y + work->vel.y;
        work->pos.z = work->pos.z + work->vel.z;
        if (life == 20) {
            work->vel.y = 4.0f;
            work->vel.x = -2.0f;
        }
        if (life < 20) {
            work->bobY *= b;
            work->vel.y += -0.5f;
            work->rot += 10.0f;
        }
        if (life < 10) {
            work->alpha = life * 25;
        }
        work->age++;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effPturnDisp, effect);
}

/** Draws the badge, and the digits for every type except 1, while the effect is active. */
static void effPturnDisp(CameraId cameraId, EffectEntry* effect) {
    PturnElem* work = effect->userdata;
    if (work->active != 0) {
        pturn_main_dispp(cameraId, effect);
        if (work->type != 1) {
            pturn_num_dispp(cameraId, effect);
        }
    }
}

/**
 * Draws the badge sprite (texture 0x92) as a camera-facing quad with the bob offset, scale and alpha,
 * once the count is above zero (always for type 10).
 */
static void pturn_main_dispp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    PturnElem* p;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    s32 i;
    s32 shownCount;
    s32 type;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, tmp);
    PSMTXConcat(camera->view, tmp, tmp);
    shownCount = p->shownCount;
    type = p->type;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        if (shownCount > 0 || type == 10) {
            effGetTexObjN64(0x92, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
            PSMTXScale(scale, p->scale, p->scale, 1.0f);
            PSMTXRotRad(rot, 'z', 0.017453292f * p->rot);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXTrans(scale, 0.0f, p->bobY, 0.0f);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(tmp, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, p->alpha});
            if (p->alpha == 255) {
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            } else {
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            }
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
            PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXSetCullMode(GX_CULL_NONE);
            effSetVtxDescN64(size48x48_tex32x32_pturn_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

/** Draws the one- or two-digit count (texture 0x53, digits 32 rows apart) over the badge when it is above zero. */
static void pturn_num_dispp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    s32 off0;
    s32 off1;
    s32 off2;
    PturnElem* p;
    s32 i;
    s32 n;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, tmp);
    PSMTXConcat(camera->view, tmp, tmp);
    n = p->shownCount;
    off0 = 352 - n * 32;
    off1 = 352 - (n % 10) * 32;
    off2 = 352 - (n / 10) * 32;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
        PSMTXScale(scale, p->scale, p->scale, 1.0f);
        PSMTXRotRad(rot, 'z', 0.017453292f * p->rot);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXTrans(scale, 0.0f, p->bobY, 0.0f);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(tmp, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        if (n > 0) {
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, p->alpha});
            effGetTexObjN64(0x53, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
            if (n < 10) {
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off0, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size24x24_tex32x32_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            } else {
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off1, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size16x24_tex32x32_r_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off2, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size16x24_tex32x32_l_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            }
        }
    }
}
