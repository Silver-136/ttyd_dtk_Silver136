#include "eff/eff_n64.h"
#include "eff/n64/eff_radiation_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

void effRadiationMain(EffectEntry* effect);
void effRadiationDisp(CameraId cameraId, EffectEntry* effect);

effN64Vtx size150x20_tex32x32[] = {
    {0, 10, 0, 0, 0, 255, 255, 255, 255},
    {0, -10, 0, 1024, 0, 255, 255, 255, 255},
    {150, -10, 0, 1024, 1024, 255, 255, 255, 255},
    {150, 10, 0, 0, 1024, 255, 255, 255, 255},
};

s32 r_data[] = {
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

/* 0x2C bytes in the original: not a whole number of 14-byte effN64Vtx records, so kept as halfwords */
s16 size300x40_tex32x32[] = {
    0, 0, 0, 16, 0, -1, -1,
    300, 20, 0, 0, 1024, -5121, -2816,
    300, -20, 0, 1024, 1024, -5121, -2816,
};



EffectEntry* effRadiationN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffRadiationN64Work* work;
    EffRadiationN64Work* p;
    s32 count;
    s32 camera;
    s32 k;
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
    count = 0x11;
    if (type <= 1) {
        count = 9;
    }
    effect->type = "RadiationN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffRadiationN64Work));
    effect->userdata = work;
    effect->callback = effRadiationMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    work->life = 100;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale20 = scale;
    work->scale1c = scale;
    work->flag = flag;
    camera = 4;
    work->camera = camera;
    p = work;
    switch (type) {
    case 0:
    case 1:
        p++;
        for (i = 1; i < effect->count; i++) {
            p->initY = (f32)((i - 1) * 30 / (count - 1) - 10);
            p->initLife = 50 - (i - 1) * 20 / (count - 1);
            p->initYVel = 2.5f + 25.0f / (f32)p->initLife;
            p->initRotX = (f32)(s32)effTblRandN64(180, i * 3);
            p->initRotY = (f32)(s32)effTblRandN64(180, i * 3 + 1);
            p->initRotZ = (f32)(s32)effTblRandN64(180, i * 3 + 2);
            p->y = p->initY;
            p->life = p->initLife;
            p->yVel = p->initYVel;
            p->rotX = p->initRotX;
            p->rotY = p->initRotY;
            p->rotZ = p->initRotZ;
            p->alpha = 0;
            p->frame = 0;
            p++;
        }
        break;
    default:
        p++;
        for (j = 1, lf = 0x20; j < effect->count; lf += 2, j++, p++) {
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
        }
        break;
    }
    return effect;
}

void effRadiationMain(EffectEntry* effect) {
    s32 base1;
    s32 base2;
    s32 base;
    EffRadiationN64Work* p = effect->userdata;
    EffRadiationN64Work* w2;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 life;
    u32 k;
    s32 i;
    s32 lf;
    s32 j;

    tmp.x = p->pos0.x;
    tmp.y = p->pos0.y;
    tmp.z = p->pos0.z;
    position = tmp;
    type = p->type;
    if (p->life <= 10) {
        p->life--;
    }
    p->frame++;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        p->life = 10;
    }
    life = p->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    base1 = p->frame;
    switch (type) {
    case 0:
    case 1:
        base = (base1 * 10 + 1) * 3;
        base1 = base + 1;
        base2 = base + 2;
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
                p->initRotX = (f32)(s32)effTblRandN64(180, base);
                p->initRotY = (f32)(s32)effTblRandN64(180, base1);
                p->initRotZ = (f32)(s32)effTblRandN64(180, base2);
                p->y = p->initY;
                p->life = p->initLife;
                p->yVel = p->initYVel;
                p->rotX = p->initRotX;
                p->rotY = p->initRotY;
                p->rotZ = p->initRotZ;
                p->alpha = 0;
                p->frame = 0;
            }
        }
        break;
    default:
        p++;
        for (j = 1, lf = 0x20; j < effect->count; lf += 2, j++, p++) {
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
            }
            if (p->delay <= 0 || (--p->delay, p->delay <= 0)) {
                p->rotX += p->rotVelX;
                p->rotY += p->rotVelY;
                p->rotZ += p->rotVelZ;
            }
        }
        break;
    }
    w2 = effect->userdata;
    if (w2->flag == 0) {
        dispEntry(w2->camera, dispCalcZ(&position), 2, (DispCallback)effRadiationDisp, effect);
    } else {
        dispEntry(w2->camera, dispCalcZ(&position), 8, (DispCallback)effRadiationDisp, effect);
    }
}

void effRadiationDisp(CameraId cameraId, EffectEntry* effect) {
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
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
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
        GXSetCurrentMtx(0);
        switch (type) {
        case 0:
        case 1:
            vtx = size150x20_tex32x32;
            break;
        default:
            vtx = (effN64Vtx*)size300x40_tex32x32;
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
