// Fire breath (gonbaba_breath, ファイアブレス): type % 10 picks the colour set in col_tbl (0 normal, 1 poison, 2 mega,
// 3 cold, 4 confusion, 5 sleep, 6 mini, 7 Bowser); each type 0-7 also spawns its companion layer as type + 10.
// Particles drift out along a yaw. Used by Hooktail, Gloomtail, Bonetail and Bowser.

#include "version.h"
#include "eff/eff_gonbaba_breath.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "poly_trig.h"

/**
 * Colour sets of the dragon breath, one per breath kind: normal fire (orange), poison (olive green), mega
 * (pale yellow), cold (ice blue), confusion (green), sleep (pale grey-pink) and mini (purple). In each set the
 * first three colours tint the three nested layers of a breath puff, outermost first; the companion sparks fade
 * from the fourth colour to the first. col_tbl picks a set by type % 10 (type 7, Bowser's flame, reuses normal).
 */
static GXColor col_normal[] = {
    {0xF2, 0x65, 0x22, 0xFF}, {0xF6, 0x86, 0x1E, 0xFF}, {0xF6, 0xAC, 0x1E, 0xFF}, {0xFF, 0xF5, 0x68, 0xFF}, {0x96, 0x5E, 0x1A, 0xFF},
};
static GXColor col_poison[] = {
    {0x67, 0x85, 0x1C, 0xFF}, {0x77, 0x98, 0x26, 0xFF}, {0x89, 0xAF, 0x2B, 0xFF}, {0xC8, 0xE5, 0x05, 0xFF}, {0x85, 0x70, 0x1C, 0xFF},
};
static GXColor col_mega[] = {
    {0xD6, 0xCF, 0x0A, 0xFF}, {0xEB, 0xEB, 0x3E, 0xFF}, {0xFF, 0xFF, 0xB4, 0xFF}, {0xF7, 0xF7, 0xD7, 0xFF}, {0xA9, 0xA3, 0x04, 0xFF},
};
static GXColor col_cold[] = {
    {0x19, 0xBC, 0xF1, 0xFF}, {0x8B, 0xDE, 0xF9, 0xFF}, {0xD1, 0xF1, 0xFC, 0xFF}, {0xE1, 0xF5, 0xFC, 0xFF}, {0x2D, 0x77, 0xCA, 0xFF},
};
static GXColor col_confu[] = {
    {0x24, 0xCA, 0x67, 0xFF}, {0x37, 0xE3, 0x7D, 0xFF}, {0x49, 0xF2, 0x8D, 0xFF}, {0xC2, 0xFC, 0xEB, 0xFF}, {0x3E, 0x87, 0x2B, 0xFF},
};
static GXColor col_sleep[] = {
    {0xD3, 0xCA, 0xD2, 0xFF}, {0xEF, 0xE3, 0xEE, 0xFF}, {0xFC, 0xF6, 0xFC, 0xFF}, {0xF6, 0xF4, 0xF6, 0xFF}, {0xA1, 0x8F, 0x9F, 0xFF},
};
static GXColor col_mini[] = {
    {0xAE, 0x69, 0xE0, 0xFF}, {0xBF, 0x7C, 0xF0, 0xFF}, {0xCD, 0x97, 0xF5, 0xFF}, {0xF3, 0xC0, 0xFA, 0xFF}, {0x81, 0x31, 0xBB, 0xFF},
};
static GXColor* col_tbl[] = {col_normal, col_poison, col_mega, col_cold, col_confu, col_sleep, col_mini, col_normal};

#if VERSION >= VERSION_JP
#pragma force_active on
#if VERSION >= VERSION_US
u32 gap_07_8041E0A4_sdata[7] = {0,0,0,0,0,0,0};
#else
u32 gap_07_80417E8C_sdata[5] = {0,0,0,0,0};
#endif
#pragma force_active reset
#endif

void effGonbabaBreathMain(EffectEntry* effect);
void effGonbabaBreathDisp(CameraId cameraId, void* param);

#pragma opt_propagation off
/** Initialises one breath particle for the given type: delay by index, start offset, velocity and lifetime. */
void init_breath(EffGonbabaBreathWork* work, s32 idx, s32 type) {
    f32 ang;
    f32 spd;
    f32 rad;

    work->type = type;
    switch (type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            work->delay = idx << 2;
            work->position.x = 0.0f;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            work->scale = 0.0f;
            work->velX = -2.4f;
            work->velY = 0.1f;
            work->life = 64;
            work->alpha = 255;
            break;
        case 7:
            work->delay = idx << 3;
            work->position.x = 0.0f;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            work->scale = 0.0f;
            work->velX = -2.0f;
            work->velY = 0.1f;
            work->life = 64;
            work->alpha = 255;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            case 16: {
                f32 ang2, rad2, spd2;
                ang2 = 240.0f + irand(60);
                spd2 = (irand(50) + 10) / 10.0f;
                spd2 *= 1.5f;
                rad2 = 6.2831855f * ang2 / 360.0f;
                work->delay = idx;
                work->position.x = 0.0f;
                work->position.y = 0.0f;
                work->position.z = 0.0f;
                work->velX = spd2 * polySinf(rad2);
                work->velY = spd2 * polyCosf(rad2);
                work->scale = 1.0f;
                work->life = 30;
                work->alpha = 255;
                work->from[0] = col_tbl[type % 10][3].r;
                work->from[1] = col_tbl[type % 10][3].g;
                work->from[2] = col_tbl[type % 10][3].b;
                work->to[0] = col_tbl[type % 10][0].r;
                work->to[1] = col_tbl[type % 10][0].g;
                work->to[2] = col_tbl[type % 10][0].b;
                break;
            }
        case 17:
            ang = 225.0f + irand(90);
            spd = (irand(50) + 10) / 10.0f;
            spd *= 1.5f;
            work->delay = irand(100);
            work->position.x = 0.0f;
            work->position.y = 0.0f;
            work->position.z = 0.0f;
            rad = 6.2831855f * ang / 360.0f;
            work->velX = spd * polySinf(rad);
            work->velY = spd * polyCosf(rad);
            work->scale = 1.0f;
            work->life = 60;
            work->alpha = 255;
            work->from[0] = col_tbl[type % 10][3].r;
            work->from[1] = col_tbl[type % 10][3].g;
            work->from[2] = col_tbl[type % 10][3].b;
            work->to[0] = col_tbl[type % 10][0].r;
            work->to[1] = col_tbl[type % 10][0].g;
            work->to[2] = col_tbl[type % 10][0].b;
            break;
    }
}

#pragma opt_propagation reset

/**
 * Spawns a breath of the given type (0-7, plus companion layers 10-17) at (x, y, z); the last argument (named
 * yaw) is stored as the breath's scale, and the direction (work->yaw) starts at 0 for callers to set.
 * life <= 0 keeps it endless.
 * Spawned via evt_eff by the dragon bosses (gon, jon, las, tst) and Bowser (las, tou2), and by koopa.c.
 */
EffectEntry* effGonbabaBreathEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 yaw) {
    EffectEntry* effect;
    EffGonbabaBreathWork* work;
    s32 count;
    s32 i;

    effect = effEntry();
    switch (type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            count = 33;
            break;
        case 7:
            count = 17;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            count = 65;
            break;
    }
    effect->type = "GonbabaBreath";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffGonbabaBreathWork));
    effect->userdata = work;
    effect->callback = effGonbabaBreathMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = yaw;
    work->alpha = 255;
    work->timer = 0;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->yaw = 0.0f;
    work->child = NULL;
    if (type < 10) {
        work->child = effGonbabaBreathEntry(type + 10, life, x, y, z, yaw);
        ((EffGonbabaBreathWork*)work->child->userdata)->yaw = work->yaw;
        if (((EffGonbabaBreathWork*)work->child->userdata)->child) {
            ((EffGonbabaBreathWork*)((EffGonbabaBreathWork*)work->child->userdata)->child->userdata)->yaw = work->yaw;
        }
    }
    work++;
    for (i = 1; i < count; i++, work++) {
        init_breath(work, i, type);
    }
    return effect;
}

/** Moves and ages each particle, and on soft delete fades the breath over 32 frames (also soft-deleting the child). */
void effGonbabaBreathMain(EffectEntry* effect) {
    EffGonbabaBreathWork* work = effect->userdata;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 i;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 32;
        if (work->child) {
            effSoftDelete(work->child);
        }
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->timer++;
    if (work->life < 32) {
        work->alpha = work->life * 8;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        switch (work->type) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                if (work->life == 0) {
                    init_breath(work, i, work->type);
                    break;
                }
                work->life--;
                if (work->life < 32) {
                    work->alpha = work->life * 8;
                }
                if (work->scale < 1.4f) {
                    work->scale += (1.5f - work->scale) / 4.0f;
                } else {
                    work->scale *= 1.05f;
                }
                work->velX *= 0.995f;
                work->velY *= 1.05f;
                work->position.x += work->velX;
                work->position.y += work->velY;
                break;
            case 7:
                if (work->life == 0) {
                    init_breath(work, 0, work->type);
                    break;
                }
                work->life--;
                if (work->life < 32) {
                    work->alpha = work->life * 8;
                }
                if (work->scale < 1.4f) {
                    work->scale += (1.5f - work->scale) / 4.0f;
                } else {
                    work->scale *= 1.03f;
                }
                work->velX *= 0.995f;
                work->velY *= 1.05f;
                work->position.x += work->velX;
                work->position.y += work->velY;
                break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                if (work->life == 0) {
                    init_breath(work, i, work->type);
                    break;
                }
                work->life--;
                if (work->life < 16) {
                    work->alpha = work->life * 16;
                }
                if (work->scale < 1.4f) {
                    work->scale += (1.5f - work->scale) / 8.0f;
                } else {
                    work->scale *= 1.03f;
                }
                work->velX *= 0.9f;
                work->velY *= 0.9f;
                work->position.x += work->velX;
                work->position.y += work->velY;
                work->from[0] = intplGetValue(EASING_SINE_IN_OUT, work->from[0], work->to[0], 30 - work->life, 30);
                work->from[1] = intplGetValue(EASING_SINE_IN_OUT, work->from[1], work->to[1], 30 - work->life, 30);
                work->from[2] = intplGetValue(EASING_SINE_IN_OUT, work->from[2], work->to[2], 30 - work->life, 30);
                break;
            case 17:
                if (work->life == 0) {
                    init_breath(work, 0, work->type);
                    break;
                }
                work->life--;
                if (work->life < 32) {
                    work->alpha = work->life * 8;
                }
                if (work->scale < 1.4f) {
                    work->scale += (1.5f - work->scale) / 8.0f;
                } else {
                    work->scale *= 1.03f;
                }
                work->velX *= 0.9f;
                work->velY *= 0.9f;
                work->position.x += work->velX;
                work->position.y += work->velY;
                work->from[0] = intplGetValue(EASING_SINE_IN_OUT, work->from[0], work->to[0], 60 - work->life, 60);
                work->from[1] = intplGetValue(EASING_SINE_IN_OUT, work->from[1], work->to[1], 60 - work->life, 60);
                work->from[2] = intplGetValue(EASING_SINE_IN_OUT, work->from[2], work->to[2], 60 - work->life, 60);
                break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, effGonbabaBreathDisp, effect);
}

/** Draws the particles as camera-facing quads, coloured by type; companion layers sit slightly off the camera plane. */
void effGonbabaBreathDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    EffGonbabaBreathWork* work = effect->userdata;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    GXTexObj texObj;
    s32 i;
    s32 j;
    CameraEntry* cam;
    s32 type;
    s32 alpha;
    f32 quadSize = 4.0f;

    cam = camGetPtr(cameraId);
    type = work->type;
    alpha = work->alpha;
    if (type < 10) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    } else {
        f32 c = cos((6.2831855f * camGetPtr(CAMERA_3D)->viewYaw) / 360.0f);
        f32 s = sin((6.2831855f * camGetPtr(CAMERA_3D)->viewYaw) / 360.0f);
        PSMTXTrans(trans, work->position.x - (2.0f * s), work->position.y, work->position.z + (2.0f * c));
    }
    PSMTXRotRad(rot, 'y', (work->yaw - camGetPtr(cameraId)->viewYaw) * 0.01745329252f);
    if ((work->yaw > 90.0f) && (work->yaw < 270.0f)) {
        PSMTXScale(scale, work->scale, work->scale, -work->scale);
    } else {
        PSMTXScale(scale, work->scale, work->scale, work->scale);
    }
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scale, trans);
    PSMTXConcat(cam->view, trans, mtx);
    effGetTexObj(EFF_TEX(0x8e), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            continue;
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (work->type) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                for (j = 0; j < 3; j++) {
                    col_tbl[type % 10][j].a = (work->alpha * alpha) / 255;
                    GXSetChanMatColor(GX_COLOR0A0, col_tbl[type % 10][j]);
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32((-(16.0f - (4.0f * j))) / 2.0f, (16.0f - (4.0f * j)) / 2.0f, 0.0f);
                    GXTexCoord2f32(0.0f, 0.0f);
                    GXPosition3f32((16.0f - (4.0f * j)) / 2.0f, (16.0f - (4.0f * j)) / 2.0f, 0.0f);
                    GXTexCoord2f32(1.0f, 0.0f);
                    GXPosition3f32((16.0f - (4.0f * j)) / 2.0f, (-(16.0f - (4.0f * j))) / 2.0f, 0.0f);
                    GXTexCoord2f32(1.0f, 1.0f);
                    GXPosition3f32((-(16.0f - (4.0f * j))) / 2.0f, (-(16.0f - (4.0f * j))) / 2.0f, 0.0f);
                    GXTexCoord2f32(0.0f, 1.0f);
                }
                break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                case 17: {
                    GXColor c;
                    c.r = work->from[0];
                    c.g = work->from[1];
                    c.b = work->from[2];
                    c.a = (work->alpha * alpha) / 255;
                    GXSetChanMatColor(GX_COLOR0A0, c);
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32((-quadSize / 2.0f), quadSize * 0.5f, 0.0f);
                    GXTexCoord2f32(0.0f, 0.0f);
                    GXPosition3f32(quadSize * 0.5f, quadSize * 0.5f, 0.0f);
                    GXTexCoord2f32(1.0f, 0.0f);
                    GXPosition3f32(quadSize * 0.5f, (-quadSize / 2.0f), 0.0f);
                    GXTexCoord2f32(1.0f, 1.0f);
                    GXPosition3f32((-quadSize / 2.0f), (-quadSize / 2.0f), 0.0f);
                    GXTexCoord2f32(0.0f, 1.0f);
                    break;
                }
        }
    }
}
