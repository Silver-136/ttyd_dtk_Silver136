#include "eff/eff_kemuri.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>

double sin(double);
double cos(double);

void effKemuriDisp(CameraId camId, void* param);
static void effKemuri2Main(EffectEntry* entry);
static void effKemuri2Disp(CameraId camId, void* param);

u8 touchdown_scale[30][2] = {
    {0x80, 0x80}, {0x82, 0x7F}, {0x84, 0x7E}, {0x87, 0x7D}, {0x89, 0x7C}, {0x8C, 0x7A},
    {0x8F, 0x79}, {0x92, 0x78}, {0x95, 0x77}, {0x98, 0x76}, {0x9B, 0x75}, {0x9D, 0x74},
    {0xA0, 0x72}, {0xA3, 0x71}, {0xA6, 0x70}, {0xA9, 0x6F}, {0xAC, 0x6E}, {0xAE, 0x6D},
    {0xB1, 0x6C}, {0xB3, 0x6B}, {0xB5, 0x6A}, {0xB7, 0x69}, {0xB9, 0x69}, {0xBB, 0x68},
    {0xBC, 0x67}, {0xBD, 0x67}, {0xBE, 0x66}, {0xBF, 0x66}, {0xBF, 0x66}, {0xC0, 0x66},
};

u8 heavy_scale[30][2] = {
    {0x80, 0x80}, {0x84, 0x80}, {0x89, 0x81}, {0x8E, 0x82}, {0x93, 0x83}, {0x99, 0x85},
    {0x9E, 0x86}, {0xA4, 0x87}, {0xAA, 0x88}, {0xB0, 0x89}, {0xB6, 0x8A}, {0xBB, 0x8B},
    {0xC1, 0x8D}, {0xC7, 0x8E}, {0xCD, 0x8F}, {0xD2, 0x90}, {0xD8, 0x91}, {0xDD, 0x92},
    {0xE2, 0x93}, {0xE6, 0x94}, {0xEB, 0x95}, {0xEF, 0x96}, {0xF2, 0x96}, {0xF6, 0x97},
    {0xF9, 0x98}, {0xFB, 0x98}, {0xFD, 0x99}, {0xFE, 0x99}, {0xFF, 0x99}, {0xFF, 0x99},
};


EffectEntry* effKemuriEntry(s32 int2, f32 float3, f32 float4, f32 float5) {
    effKemuriData* d;
    effKemuriData* data;
    s32 numEffects, i;
    
    EffectEntry* entry = effEntry();
    numEffects = 1;
    switch (int2) {
        case 0:
            numEffects = 3;
            break;
        case 1:
            numEffects = 2;
            break;
        case 2:
            numEffects = 2;
            break;
    }
    entry->type = "Kemuri";
    entry->count = numEffects;
    data = (effKemuriData*)__memAlloc(HEAP_EFFECT, sizeof(effKemuriData) * numEffects);
    entry->userdata = data;
    entry->callback = effKemuriMain;

    data->type = int2;
    data->x = float3;
    data->y = float4;
    data->z = float5;

    data->frame = 0;

    data->scaleX = 1.0f;
    data->scaleY = 1.0f;
    data->scale = 1.0f;
    for (i = 1, d = data + 1; i < entry->count; i++, d++) {
        d->x = 0.0f;
        d->y = 0.0f;
        d->z = 0.0f;

        d->scaleX = 1.0f;
        d->scaleY = 1.0f;
        d->scale = 1.0f;

        d->offsetX = 0.0f;
        d->reserved_5C = 0.0f;

        d->frame = 0;
        d->texFrame0 = 0;
        d->texFrame1 = 0;

        switch (int2) {
            case 0:
                d->velX = i & 1 ? 3.0f : -3.0f;
                d->lifetime = 32;
                break;
            case 1:
                d->velX = 0.0f;
                d->lifetime = 32;
                break;
            case 2:
                d->velX = 0.0f;
                d->lifetime = 64;
                break;
        }
    }
    return entry;
}

#pragma opt_propagation off
EffectEntry* effKemuri2Entry(f32 x, f32 y, f32 z, f32 dx, f32 dz) {
    EffectEntry* entry;
    effKemuri2Data* data;
    f32 rv, vy, speed, rot, zz;
    f32 rad, angle, vz, vx;
    s32 ia;

    entry = effEntry();
    entry->type = "Kemuri2";
    entry->count = 1;
    data = (effKemuri2Data*)__memAlloc(HEAP_EFFECT, sizeof(effKemuri2Data));
    entry->userdata = data;
    memset(data, 0, sizeof(effKemuri2Data));
    entry->callback = effKemuri2Main;
    angle = 360.0f * angleABf(0.0f, 0.0f, vx = 1000.0f * dx, vz = 1000.0f * dz) / 6.2831855f;
    rot = camGetPtr(4)->viewYaw;
    rv = 360.0f * angleABf(0.0f, 0.0f, vx, vz) / 6.2831855f;
    data->angle = reviseAngle(rv + rot);
    ia = (s32)angle;
    rad = 6.2831855f * ia / 360.0f;
    zz = 5.0f + z;
    data->timer = 0;
    speed = 1.0f;
    vy = 0.7f;
    rv = -20.0f;
    rot = 0.0f;
    data->dirX = sin(rad);
    data->dirZ = cos(rad);
    data->x = x;
    data->y = y;
    data->z = zz;
    data->speed = speed;
    data->vy = vy;
    data->rotVel = rv;
    data->rot = rot;
    data->alpha = 255;
    data->scale = speed;
    data->scale2 = speed;
    return entry;
}
#pragma opt_propagation reset

void effKemuriMain(EffectEntry* effect) {
    effKemuriData* data = effect->userdata;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;

    pos.x = data->x;
    pos.y = data->y;
    pos.z = data->z;
    pos2 = pos;
    type = data->type;
    data++;
    for (i = 1; i < effect->count; i++, data++) {
        data->texFrame0 = 16.0f * data->frame / data->lifetime;
        data->texFrame1 = 16.0f * (data->frame + 1) / data->lifetime;
        if (data->texFrame1 > 15) {
            data->texFrame1 = 15;
        }
        switch (type) {
            case 0:
                if ((u32)data->frame < 30) {
                    data->scaleX = touchdown_scale[data->frame][0] / 128.0f;
                    data->scaleY = touchdown_scale[data->frame][1] / 128.0f;
                }
                else {
                    data->scaleX = 1.5f;
                    data->scaleY = 0.8f;
                }
                data->velX *= 0.85f;
                data->offsetX += data->velX;
                data->frame++;
                break;
            case 1:
                data->frame++;
                break;
            case 2:
                if ((u32)data->frame < 30) {
                    data->scaleX = heavy_scale[data->frame][0] / 128.0f;
                    data->scaleY = heavy_scale[data->frame][1] / 128.0f;
                }
                else {
                    data->scaleX = 2.0f;
                    data->scaleY = 1.2f;
                }
                data->frame++;
                break;
        }
        if (data->frame >= data->lifetime) {
            effDelete(effect);
            return;
        }
    }
    dispEntry(4, dispCalcZ(&pos2), 2, effKemuriDisp, effect);
}

#pragma opt_propagation off
static void effKemuri2Main(EffectEntry* entry) {
    effKemuri2Data* data;
    effKemuri2Data* p = entry->userdata;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};
    f32 s;
    s32 i;
    pos.x = p->x;
    pos.y = p->y;
    pos.z = p->z;
    data = p;
    pos2 = pos;
    s = sin(3.1415927f * data->timer / 30.0f);
    data->timer++;
    if (data->timer > 30) {
        effDelete(entry);
        return;
    }
    for (i = 0; i < entry->count; i++, data++) {
        p = data;
        p->x += p->speed * p->dirX;
        p->z += p->speed * p->dirZ;
        p->vy *= 0.9f;
        p->y += p->vy;
        p->rotVel *= 0.95f;
        p->rot += p->rotVel;
        p->scale = s;
        if (data->timer > 15) {
            p->alpha *= 0.8f;
        }
    }
    dispEntry(4, dispCalcZ(&pos2), 2, effKemuri2Disp, entry);
}
#pragma opt_propagation reset

void effKemuriDisp(CameraId camId, void* param) {
    Mtx t, s, r, m, m2;
    GXTexObj tex;
    s32 i, j, off;
    EffectEntry* entry = param;
    effKemuriData* data = entry->userdata;
    s32 type = data->type;
    CameraEntry* cam = camGetPtr(camId);
    f32 w = 1.0f;

    switch (type) {
        case 0:
            effGetTexObj(0x23, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
        case 1:
            effGetTexObj(0x24, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
        case 2:
            effGetTexObj(0x23, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
    }
    PSMTXTrans(t, data->x, data->y, data->z);
    PSMTXRotRad(r, 'y', 0.017453292f * -camGetPtr(camId)->viewYaw);
    PSMTXScale(s, data->scale, data->scale, 1.0f);
    PSMTXConcat(t, r, m);
    PSMTXConcat(m, s, m);
    PSMTXConcat(cam->view, m, m);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x80, 0x80, 0x80, 0x80});
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    data++;
    for (i = 1; i < entry->count; i++, data++) {
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0x00, 0x00, 0x00, 0x60});
            }
            else {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
            off = 1 - j;
            PSMTXTrans(t, data->offsetX + off, -off, 0.0f);
            PSMTXScale(s, 20.0f * data->scaleX, 20.0f * data->scaleY, 20.0f);
            PSMTXConcat(m, t, m2);
            PSMTXConcat(m2, s, m2);
            GXLoadPosMtxImm(m2, 0);
            PSMTXScale(s, 0.0625f, 1.0f, 1.0f);
            PSMTXTrans(t, data->texFrame0, -0.2f, 0.0f);
            PSMTXConcat(s, t, t);
            GXLoadTexMtxImm(t, GX_TEXMTX0, GX_MTX2x4);
            PSMTXScale(s, 0.0625f, 1.0f, 1.0f);
            PSMTXTrans(t, data->texFrame1, -0.2f, 0.0f);
            PSMTXConcat(s, t, t);
            GXLoadTexMtxImm(t, GX_TEXMTX1, GX_MTX2x4);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-w / 2.0f, w, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w / 2.0f, w, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(w / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-w / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

static void effKemuri2Disp(CameraId camId, void* param) {
    Mtx t, r, rz, s, m, tmp;
    GXTexObj tex;
    EffectEntry* entry = param;
    effKemuri2Data* data = entry->userdata;
    CameraEntry* cam = camGetPtr(camId);
    s32 i, off;
    f32 w2 = 12.8f;
    f32 w = 12.0f;
    f32 h = 8.0f;

    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    mapSetMaterialFog();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < 2; i++) {
        if ((data->angle > 20.0f && data->angle < 160.0f) || (data->angle > 180.0f && data->angle < 340.0f)) {
            off = 1 - i;
            PSMTXTrans(t, data->x + off, data->y - off, data->z);
            PSMTXRotRad(r, 'y', 0.017453292f * -camGetPtr(camId)->viewYaw);
            PSMTXRotRad(rz, 'z', 0.17453292f);
            PSMTXScale(s, data->scale, data->scale, data->scale);
            PSMTXConcat(t, r, m);
            if (data->angle < 180.0f) {
                PSMTXScale(tmp, -1.0f, 1.0f, -1.0f);
                PSMTXConcat(m, tmp, m);
            }
            PSMTXTrans(tmp, 10.0f, -6.0f, 0.0f);
            PSMTXConcat(m, tmp, m);
            PSMTXScale(tmp, data->scale2, data->scale2, data->scale2);
            PSMTXConcat(m, tmp, m);
            PSMTXTrans(tmp, -10.0f, 6.0f, 0.0f);
            PSMTXConcat(m, tmp, m);
            PSMTXConcat(m, rz, m);
            PSMTXConcat(m, s, m);
            PSMTXConcat(cam->view, m, m);
            PSMTXTrans(t, 6.0f, -4.0f, 0.0f);
            PSMTXConcat(m, t, m);
            GXLoadPosMtxImm(m, 0);
            GXSetCurrentMtx(0);
            effGetTexObj(0xd, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            if (i == 0) {
                GXSetTevColor(GX_TEVREG0, (GXColor){0x00, 0x00, 0x00, data->alpha});
            }
            else {
                GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, data->alpha});
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            { f32 nw = -w / 2.0f; f32 nh = -h / 2.0f;
            GXPosition3f32(nw, h * 0.5f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w * 0.5f, h * 0.5f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(w * 0.5f, nh, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(nw, nh, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f); }
        }
        off = 1 - i;
        PSMTXTrans(t, data->x + off, data->y - off, data->z);
        PSMTXRotRad(r, 'y', 0.017453292f * -camGetPtr(camId)->viewYaw);
        PSMTXRotRad(rz, 'z', 0.017453292f * data->rot);
        PSMTXScale(s, data->scale, data->scale, data->scale);
        PSMTXConcat(t, r, m);
        if (data->angle < 180.0f) {
            PSMTXScale(tmp, -1.0f, 1.0f, -1.0f);
            PSMTXConcat(m, tmp, m);
        }
        PSMTXTrans(tmp, 10.0f, -6.0f, 0.0f);
        PSMTXConcat(m, tmp, m);
        PSMTXScale(tmp, data->scale2, data->scale2, data->scale2);
        PSMTXConcat(m, tmp, m);
        PSMTXTrans(tmp, -10.0f, 6.0f, 0.0f);
        PSMTXConcat(m, tmp, m);
        PSMTXConcat(m, rz, m);
        PSMTXConcat(m, s, m);
        PSMTXConcat(cam->view, m, m);
        GXLoadPosMtxImm(m, 0);
        GXSetCurrentMtx(0);
        if (i == 0) {
            effGetTexObj(0xc, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXSetTevColor(GX_TEVREG0, (GXColor){0x00, 0x00, 0x00, data->alpha});
        }
        else {
            effGetTexObj(0xb, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, data->alpha});
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w2 / 2.0f, w2 / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w2 / 2.0f, w2 / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w2 / 2.0f, -w2 / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w2 / 2.0f, -w2 / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}

