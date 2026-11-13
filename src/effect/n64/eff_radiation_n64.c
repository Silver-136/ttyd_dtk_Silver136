// Paper Mario 64 effect: rays from a point: types 0-1 eight pale-yellow textured rays that move out and fade, other
// types 16 spinning near-white untextured rays (their texture scroll offsets are advanced but never drawn);
// type + 10 draws in another display layer with plain alpha blending and no depth test.

#include "eff/eff_n64.h"
#include "eff/n64/eff_radiation_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

static void effRadiationMain(EffectEntry* effect);
static void effRadiationDisp(CameraId cameraId, void* param);

/** 150x20 quad, starting at the centre and reaching outward, drawn for each ray of types 0-1. */
static effN64Vtx size150x20_tex32x32[] = {
    {0, 10, 0, 0, 0, 255, 255, 255, 255},
    {0, -10, 0, 1024, 0, 255, 255, 255, 255},
    {150, -10, 0, 1024, 1024, 255, 255, 255, 255},
    {150, 10, 0, 0, 1024, 255, 255, 255, 255},
};

/** Three vertices: the centre (opaque white) and two 300 units out at y +-20 (alpha 0), drawn for each ray of the other types. */
static effN64Vtx size300x40_tex32x32[] = {
    {0, 0, 0, 16, 0, 255, 255, 255, 255},
    {300, 20, 0, 0, 1024, 235, 255, 245, 0},
    {300, -20, 0, 1024, 1024, 235, 255, 245, 0},
};

/**
 * Fixed pose of each of the 16 spinning rays, five numbers per ray: X, Y and Z tilt in degrees,
 * an unused value and the Z spin speed in tenths of a degree per frame.
 */
static s32 r_data[] = {
    0, -20, 0, 1, 18,
    -10, -60, 45, -1, 4,
    -20, -20, 90, 1, 1,
    -10, -60, 135, 1, -4,
    0, -20, 180, -8, 1,
    -10, -60, 225, -1, -1,
    -20, -20, 270, 1, -9,
    -10, -60, 315, 4, 4,
    0, 10, 22, 4, 1,
    -10, 20, 67, 1, -1,
    -20, 10, 112, -1, -1,
    -10, 20, 157, -8, 1,
    0, 10, 202, 1, -4,
    -10, 20, 247, 1, -18,
    -20, 10, 292, -1, 9,
    -10, 20, 337, 1, 1,
};

/** Respawns a type 0-1 ray at a random tilt (table seeds n * 3 .. n * 3 + 2) and resets it to its start values. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void type1_init(EffRadiationN64Work* p, s32 n) {
    p->initRotX = (f32)(s32)effTblRandN64(180, n * 3);
    p->initRotY = (f32)(s32)effTblRandN64(180, n * 3 + 1);
    p->initRotZ = (f32)(s32)effTblRandN64(180, n * 3 + 2);
    p->y = p->initY;
    p->life = p->initLife;
    p->yVel = p->initYVel;
    p->rotX = p->initRotX;
    p->rotY = p->initRotY;
    p->rotZ = p->initRotZ;
    p->alpha = 0;
    p->frame = 0;
}

/** Respawns a ray of type 2 or later on its fixed r_data pose j (life lf) with its scroll and spin reset; returns the next ray. */
// Original name and size (0x114) from the JP demo's link map (UNUSED there): every call is inlined.
static EffRadiationN64Work* type2_init(EffRadiationN64Work* p, s32 j, s32 lf) {
    s32 k;

    p->y = 0.0f;
    k = (j - 1) * 5;
    p->life = lf;
    p->rotX = (f32)r_data[k++];
    p->rotY = (f32)r_data[k++];
    p->rotZ = (f32)r_data[k++];
    p->alpha = 0;
    p->frame = 0;
    p->scroll2 = 0.0f;
    p->scroll0 = 0.0f;
    p->scrollVel0 = 0.0f;
    p->scrollVel2 = -0.6f;
    p->scroll1 = 10.0f;
    p->scroll3 = 0.0f;
    p->scrollVel1 = 0.0f;
    p->scrollVel3 = 0.6f;
    p->rotVelX = 0.0f;
    p->rotVelY = 0.0f;
    k++;
    p->rotVelZ = 0.1f * (f32)r_data[k++];
    p->delay = 0;
    return p + 1;
}

/**
 * Spawns the radiation rays at (x, y, z): types 0-1 use 8 rising rays, other types 16 spinning rays;
 * type + 10 draws on another display layer.
 * Used by the Palace of Shadow (las_29) and by event scripts through evt_eff.
 */
EffectEntry* effRadiationN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffRadiationN64Work* work;
    EffRadiationN64Work* p;
    s32 count;
    s32 camera;
    s32 lf;
    s32 i;
    s32 flag;
    s32 j;

    effect = effEntry();
    flag = 0;
    if (type >= 10) {
        flag = 1;
        type = type % 10;
    }
    count = 17;
    if (type <= 1) {
        count = 9;
    }
    effect->type = "RadiationN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffRadiationN64Work));
    effect->userdata = work;
    effect->callback = effRadiationMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    work->life = 100;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale20 = scale;
    work->scale1c = scale;
    work->flag = flag;
    work->camera = CAMERA_3D;
    p = work;
    switch (type) {
        case 0:
        case 1:
            p++;
            for (i = 1; i < effect->count; i++) {
                p->initY = (f32)((i - 1) * 30 / (count - 1) - 10);
                p->initLife = 50 - (i - 1) * 20 / (count - 1);
                p->initYVel = 2.5f + 25.0f / (f32)p->initLife;
                type1_init(p, i);
                p++;
            }
            break;
        default:
            p++;
            for (j = 1, lf = 0x20; j < effect->count; lf += 2, j++) {
                p = type2_init(p, j, lf);
            }
            break;
    }
    return effect;
}

/** Advances the four texture scroll offsets of a ray, wrapping them into 0-256. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void tx_scroll(EffRadiationN64Work* p) {
    p->scroll0 += p->scrollVel0;
    if (p->scroll0 > 256.0f) {
        p->scroll0 -= 256.0f;
    }
    if (p->scroll0 < 0.0f) {
        p->scroll0 += 256.0f;
    }
    p->scroll1 += p->scrollVel1;
    if (p->scroll1 > 256.0f) {
        p->scroll1 -= 256.0f;
    }
    if (p->scroll1 < 0.0f) {
        p->scroll1 += 256.0f;
    }
    p->scroll2 += p->scrollVel2;
    if (p->scroll2 > 256.0f) {
        p->scroll2 -= 256.0f;
    }
    if (p->scroll2 < 0.0f) {
        p->scroll2 += 256.0f;
    }
    p->scroll3 += p->scrollVel3;
    if (p->scroll3 > 256.0f) {
        p->scroll3 -= 256.0f;
    }
    if (p->scroll3 < 0.0f) {
        p->scroll3 += 256.0f;
    }
}

/**
 * Types 0-1: each ray moves out along its own direction by yVel, fades in over 10 frames and out over its last 5,
 * then respawns at a random tilt (0-179 degrees per axis).
 * Other types scroll four texture offsets, fade each ray in and out over 5 frames,
 * spin it about Z at a speed from r_data and respawn it on its fixed r_data pose.
 * The effect itself only counts down once a delete request sets its life to 10, then deletes
 * (the rays of types 2 and up fade out over those frames).
 * Drawn on layer 2, or 8 when `flag` is set.
 */
static void effRadiationMain(EffectEntry* effect) {
    s32 frame;
    EffRadiationN64Work* p = effect->userdata;
    EffRadiationN64Work* w2;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 life;
    s32 i;
    s32 lf;
    s32 j;

    // BUG: pos0 is never written (Entry fills position), so the depth passed to dispCalcZ below comes from
    // uncleared heap memory.
    tmp.x = p->pos0.x;
    tmp.y = p->pos0.y;
    tmp.z = p->pos0.z;
    position = tmp;
    type = p->type;
    if (p->life <= 10) {
        p->life--;
    }
    p->frame++;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        p->life = 10;
    }
    life = p->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    frame = p->frame;
    switch (type) {
        case 0:
        case 1:
            p++;
            for (i = 1; i < effect->count; i++, p++) {
                p->life--;
                p->frame++;
                if (p->frame < 10) {
                    p->alpha = p->frame * 12;
                }
                if (p->life < 5) {
                    p->alpha = p->life * 25;
                }
                p->y += p->yVel;
                if (p->life <= 0) {
                    type1_init(p, frame * 10 + 1);
                }
            }
            break;
        default:
            p++;
            for (j = 1, lf = 0x20; j < effect->count; lf += 2, j++, p++) {
                tx_scroll(p);
                p->life--;
                p->frame++;
                if (p->frame < 5) {
                    p->alpha = p->frame * 50;
                }
                if (p->life < 5) {
                    p->alpha = p->life * 50;
                }
                if (life < 10) {
                    p->alpha = p->alpha * life / 10;
                }
                if (p->life <= 0) {
                    type2_init(p, j, lf);
                }

                if (p->delay > 0) {
                    --p->delay;
                    if (p->delay > 0) {
                        continue;
                    }
                }
                p->rotX += p->rotVelX;
                p->rotY += p->rotVelY;
                p->rotZ += p->rotVelZ;
            }
            break;
    }
    w2 = effect->userdata;
    if (w2->flag == 0) {
        dispEntry(w2->camera, dispCalcZ(&position), DISP_MODE_XLU, effRadiationDisp, effect);
    } else {
        dispEntry(w2->camera, dispCalcZ(&position), DISP_MODE_CUSTOM_MID, effRadiationDisp, effect);
    }
}

/**
 * Types 0-1 draw each ray as a 150x20 quad of texture 0x2C tinted pale yellow;
 * other types draw an untextured 300x40 triangle in near-white, fading toward its tip, whose width follows its
 * alpha with a factor picked by i & 3 and whose length grows from 0.75 to 1 with it (type 3 turned 45 degrees about Z).
 * Both faces drawn; with `flag` set it uses plain alpha blending without depth test or writes.
 */
static void effRadiationDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    CameraEntry* camera;
    EffRadiationN64Work* p;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    Mtx trans;
    effN64Vtx* vtx;
    s32 i;
    s32 type;
    f32 a;
    f32 sx;
    f32 sz;
    f32 sy;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    type = p->type;
    if (p->flag != 0) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    }
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    switch (type) {
        case 0:
        case 1:
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            effGetTexObjN64(0x2C, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
            PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            break;
        default:
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetNumTexGens(0);
            break;
    }
    GXSetCullMode(GX_CULL_NONE);
    PSMTXTrans(mtx, p->position.x, p->position.y, p->position.z);
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        a = (f32)p->alpha / 255.0f;
        PSMTXRotRad(rot, 'x', 0.017453292f * p->rotX);
        PSMTXConcat(mtx, rot, tmp);
        if (type >= 2) {
            PSMTXRotRad(rot, 'z', 0.017453292f * (p->rotZ + (f32)(type == 3 ? 45 : 0)));
            PSMTXConcat(tmp, rot, tmp);
            PSMTXRotRad(rot, 'y', 0.017453292f * p->rotY);
            PSMTXConcat(tmp, rot, tmp);
            PSMTXTrans(trans, p->y, 0.0f, 0.0f);
            PSMTXConcat(tmp, trans, tmp);
            sx = sz = 0.25f * (3.0f + a);

            switch (i & 3) {
                case 0:
                    sy = 0.3f * a;
                    break;
                case 1:
                    sy = 0.5f * a;
                    break;
                case 2:
                    sy = 0.9f * a;
                    break;
                default:
                    sy = a;
                    break;
            }
            PSMTXScale(scale, sx, sy, sz);
            PSMTXConcat(tmp, scale, tmp);
            GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFA, p->alpha});
        } else {
            PSMTXRotRad(rot, 'y', 0.017453292f * p->rotY);
            PSMTXConcat(tmp, rot, tmp);
            PSMTXRotRad(rot, 'z', 0.017453292f * (p->rotZ + (f32)(type == 3 ? 45 : 0)));
            PSMTXConcat(tmp, rot, tmp);
            PSMTXTrans(trans, p->y, 0.0f, 0.0f);
            PSMTXConcat(tmp, trans, tmp);
            GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xB5, p->alpha});
        }
        PSMTXConcat(camera->view, tmp, tmp);
        GXLoadPosMtxImm(tmp, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (type) {
            case 0:
            case 1:
                vtx = size150x20_tex32x32;
                break;
            default:
                vtx = size300x40_tex32x32;
                break;
        }
        effSetVtxDescN64(vtx);
        switch (type) {
            case 0:
            case 1:
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                break;
            default:
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
                tri1(0, 1, 2, 0);
                break;
        }
    }
}
