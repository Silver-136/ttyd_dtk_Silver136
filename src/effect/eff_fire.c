#include "eff/eff_fire.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"

typedef struct EffFireSmokeWork {
    s32 unk0;       // 0x0
    Vec position;   // 0x4
    Vec velocity;   // 0x10
    s32 pad1c[3];   // 0x1C
    f32 rotation;   // 0x28
    f32 rotSpeed;   // 0x2C
    u8 color[4];    // 0x30
    s32 life;       // 0x34
    s32 age;        // 0x38
    f32 scale;      // 0x3C
} EffFireSmokeWork;

typedef struct FireScaleKey {
    s32 frame;
    f32 value;
} FireScaleKey;

FireScaleKey scale1[] = {
    { 0, 1.0f },     { 20, 1.1f },    { 30, 1.0f },    { 45, 1.05f },   { 60, 0.95f },
    { 70, 1.05f },   { 80, 1.025f },  { 100, 1.1f },   { 120, 1.0f },   { 130, 1.025f },
    { 160, 0.95f },  { 180, 1.025f }, { 200, 1.0f },
};
static GXColor col_tbl[3][8] = {
    { { 0xAC, 0x2E, 0x00, 0xFF }, { 0xE0, 0x22, 0x00, 0xFF }, { 0xFD, 0x5F, 0x00, 0xFF }, { 0xFD, 0x5F, 0x04, 0xFF },
      { 0xFF, 0x69, 0x00, 0xFF }, { 0xFF, 0x8A, 0x00, 0xFF }, { 0xFF, 0xB7, 0x00, 0xFF }, { 0xFF, 0xD7, 0x00, 0xFF } },
    { { 0x0A, 0xE0, 0xFF, 0xFF }, { 0x10, 0xC3, 0xFF, 0xFF }, { 0x09, 0x61, 0xFF, 0xFF }, { 0x09, 0x61, 0xFF, 0xFF },
      { 0xD1, 0xF9, 0xFF, 0xFF }, { 0xD1, 0xF9, 0xFF, 0xFF }, { 0xA3, 0xE5, 0xFF, 0xFF }, { 0x3A, 0xCA, 0xFF, 0xFF } },
    { { 0x5B, 0xF6, 0x3D, 0xFF }, { 0x3B, 0xDE, 0x1C, 0xFF }, { 0x3B, 0xDE, 0x1C, 0xFF }, { 0x1D, 0xB9, 0x00, 0xFF },
      { 0xDE, 0xF6, 0x18, 0xFF }, { 0xA0, 0xF6, 0x06, 0xFF }, { 0xA0, 0xF6, 0x06, 0xFF }, { 0x92, 0xE4, 0x00, 0xFF } },
};

static u8 scale_dt[] = {
    128, 128, 129, 127, 130, 127, 131, 126, 132, 126, 132, 126, 133, 125, 134, 125, 135, 124, 136, 124, 137, 124, 138, 123, 138, 123, 139, 123, 140, 123, 141, 122, 141, 122, 142, 122, 143, 121, 143, 121, 144, 121, 145, 121, 146, 120, 146, 120, 147, 120, 148, 119, 149, 119, 149, 119, 150, 118, 151, 118, 152, 118, 152, 118, 153, 117, 154, 117, 155, 116, 156, 116, 157, 116, 158, 115, 159, 115, 160, 115, 161, 114, 162, 114, 163, 113, 165, 113, 166, 112, 167, 112, 169, 111, 170, 111, 171, 110, 173, 109, 174, 109, 176, 108, 178, 107, 180, 107, 181, 106, 183, 105, 185, 104, 187, 104, 189, 103, 192, 102,
};

void effFireMain(EffectEntry* effect);
static inline void effFireQuad(s32 iw, s32 ih) {
    f32 w, h;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    w = iw;
    h = ih;
    GXPosition3f32(-w / 2.0f, h, 0.0f);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w / 2.0f, h, 0.0f);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w / 2.0f, 0.0f, 0.0f);
    GXColor4u8(0x00, 0x00, 0x00, 0xFF);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-w / 2.0f, 0.0f, 0.0f);
    GXColor4u8(0x00, 0x00, 0x00, 0xFF);
    GXTexCoord2f32(0.0f, 1.0f);
}
void effFireDisp(CameraId cameraId, EffectEntry* effect);
void effFireDisp2(CameraId cameraId, EffectEntry* effect);
void effFireDisp3(CameraId cameraId, EffectEntry* effect);
EffectEntry* effFireSparkN64Entry(s32 type, s32 count, s32 lifetime, f32 x, f32 y, f32 z, f32 rangeX,
                                  f32 rangeY, f32 a, f32 b, f32 c);

FireScaleKey scale2[] = {
    { 0, 1.0f },
    { 100, 1.2f },
    { 160, 0.95f },
    { 200, 1.0f },
};

void effFireSmokeDisp(CameraId cameraId, EffectEntry* effect);
void effFireSmokeMain(EffectEntry* effect);


static void effFireSmokeEntry(f32 x, f32 y, f32 z, f32 scale);

EffectEntry* effFireEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffFireWork* work;

    effect = effEntry();
    effect->type = "Fire";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, 0x8c);
    effect->userdata = work;
    effect->callback = effFireMain;
    effect->flags |= 2;
    if (duration <= 0) {
        duration = 1000;
    }
    work->type = type;
    work->alpha = 0xFF;
    {
        Vec pos = { 0.0f, 0.0f, 0.0f };
        pos.x = x;
        pos.y = y;
        pos.z = z;
        work->position = pos;
    }
    work->scale = scale;
    work->duration = duration;
    work->delay = 0;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    work->sparkTimer = 0;
    work->range = rand() % 30 + 30;
    work->smokePhase0 = 0;
    work->smokePhase1 = 0;
    work->f4c[0] = 0.0f;
    work->f4c[1] = 0.0f;
    work->f4c[2] = 0.0f;
    work->f4c[3] = 0.0f;
    work->f4c[4] = 0.0f;
    work->f4c[5] = 0.0f;
    switch (type) {
        case 0:
            work->smokeLife = 12;
            work->smokeInt = work->smokeLife * 2;
            break;
        case 1:
            work->smokeLife = 4;
            work->smokeInt = work->smokeLife * 2;
            break;
        case 2:
            work->smokeLife = 10;
            work->smokeInt = work->smokeLife * 2;
            break;
        case 3:
        case 4:
        case 8:
            work->smokeLife = 12;
            work->smokeInt = work->smokeLife * 2;
            work->alpha = 0xCC;
            break;
        case 5:
        case 6:
        case 7:
            work->smokeLife = 12;
            work->smokeInt = work->smokeLife * 2;
            work->alpha = 0xFF;
            break;
    }
    work->delay = rand() % work->smokeInt;
    work->keyIdx = 0;
    work->keyFrame = 0;
    work->pulseScale = 0.0f;
    work->reserved_80 = 0;
    work->reserved_84 = 0;
    work->useMapBlend = 0;
    return effect;
}

void effFireMain(EffectEntry* effect) {
    EffFireWork* work = effect->userdata;
    s32 type = work->type;
    FireScaleKey* tbl;
    s32 count;
    s32 idx;

    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->duration = 16;
    }
    if (work->duration < 1000) {
        work->duration--;
    }
    if (work->duration < 16) {
        work->alpha = work->duration * 16;
    }
    if (work->duration < 0) {
        effDelete(effect);
        return;
    }
    switch (type) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            work->smokePhase0 = (f32)work->smokeLife * (f32)work->delay / (f32)work->smokeInt;
            work->smokePhase1 = (f32)work->smokeLife * (f32)(work->delay + 1) / (f32)work->smokeInt;
            if (work->smokePhase0 >= work->smokeLife) {
                work->smokePhase0 = 0;
            }
            if (work->smokePhase1 >= work->smokeLife) {
                work->smokePhase1 = 0;
            }
            work->glowAlpha = 128;
            break;
    }
    work->delay++;
    if (work->delay > work->smokeInt) {
        work->delay = 0;
    }
    switch (type) {
        case 3:
        case 4:
        case 8: {
            f32 t, v;
            s32 kf;
            switch (type) {
                case 3:
                case 8:
                    tbl = scale1;
                    count = 13;
                    break;
                case 4:
                    tbl = scale2;
                    count = 4;
                    break;
            }
            while (1) {
                kf = work->keyFrame; idx = work->keyIdx;
                if (kf < tbl[idx].frame) {
                    break;
                }
                work->keyIdx = idx + 1;
                if (work->keyIdx >= count) {
                    work->keyIdx = 0;
                    work->keyFrame = 0;
                }
            }
            t = (f32)(tbl[idx].frame - kf) / (f32)(tbl[idx].frame - tbl[idx - 1].frame);
            v = (1.0f - t) * tbl[idx].value + t * tbl[idx - 1].value;
            work->pulseScale = v;
            work->pulseScale *= 1.4f;
            work->pulseScale *= 1.2f;
            work->keyFrame++;
            break;
        }
        case 5:
        case 6:
        case 7:
            work->sparkTimer++;
            if (work->sparkTimer > work->range) {
                effFireSparkN64Entry(type, 2, 200, work->position.x, 10.0f * work->scale + work->position.y,
                                     work->position.z, 30.0f * work->scaleX, 30.0f, work->scale, 1.0f,
                                     1.0f);
                work->sparkTimer = 0;
                work->range = rand() % 30 + 30;
            }
            break;
        default:
            work->sparkTimer++;
            if (work->sparkTimer > work->range) {
                u8 offs[4] = { 5, 10, 10, 10 };
                f32 px, py, pz, sc;
                pz = work->position.z - 5.0f;
                py = work->position.y + (f32)offs[type];
                sc = work->scale;
                px = work->position.x;
                effFireSmokeEntry(px, py, pz, sc);
                work->sparkTimer = 0;
                work->range = rand() % 30 + 30;
            }
            break;
    }
    switch (type) {
        case 3:
        case 4:
        case 8:
            dispEntry(CAMERA_3D, dispCalcZ(&work->position), 2, (DispCallback)effFireDisp2, effect);
            break;
        case 5:
        case 6:
        case 7:
            dispEntry(CAMERA_3D, dispCalcZ(&work->position), 2, (DispCallback)effFireDisp3, effect);
            break;
        default:
            dispEntry(CAMERA_3D, dispCalcZ(&work->position), 8, (DispCallback)effFireDisp, effect);
            break;
    }
}

void effFireDisp(CameraId cameraId, EffectEntry* effect) {
    s32 w;
    s32 h;
    EffFireWork* work;
    CameraEntry* camera;
    s32 type;
    s32 n;
    s32 frame;
    GXTexObj texObj;
    Mtx trans, scaleMtx, rot, m6c, m3c;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    type = work->type;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    switch (type) {
        case 0:
            effGetTexObj(0x22, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            n = 12;
            w = 32;
            h = 32;
            break;
        case 1:
            effGetTexObj(0x46, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            n = 4;
            w = 32;
            h = 40;
            break;
        case 2:
            effGetTexObj(0x47, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            n = 10;
            w = 24;
            h = 40;
            break;
    }
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    frame = (work->delay >> 1) % n;
    PSMTXScale(scaleMtx, 1.0f / n, 1.0f, 1.0f);
    PSMTXTrans(trans, frame, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, trans, m6c);
    GXLoadTexMtxImm(m6c, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    {
        f32 s = 20.0f * work->scale; s *= 0.03125f;
        PSMTXScale(scaleMtx, s * work->scaleX, s * work->scaleY, s);
    }
    PSMTXConcat(trans, rot, m6c);
    PSMTXConcat(m6c, scaleMtx, m6c);
    PSMTXConcat(camera->view, m6c, m6c);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(m3c, 0.01f, -0.01f, 0.0f);
    PSMTXConcat(m6c, m3c, m3c);
    GXLoadPosMtxImm(m3c, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_KONST);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A0, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){ 0xFF, 0x00, 0x00, 0xFF });
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0x00, 0x64 });
    effFireQuad(w, h);
    GXLoadPosMtxImm(m6c, GX_PNMTX0);
    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_KONST, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){ 0xF8, 0x99, 0x28, 0xFF });
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ONE, GX_CC_KONST, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K1);
    GXSetTevKColor(GX_KCOLOR1, (GXColor){ 0xF1, 0x9E, 0x8F, 0xFF });
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_TEXA, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
    GXSetTevColor(GX_TEVREG2, (GXColor){ 0x00, 0x00, 0x00, 0x64 });
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_C1, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_TEXA, GX_CA_ZERO, GX_CA_APREV, GX_CA_APREV);
    effFireQuad(w, h);
}

void effFireDisp2(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffFireWork* work;
    s32 type;
    GXTexObj texObj;
    Mtx trans, scaleMtx, rot, m54;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    type = work->type;
    effGetTexObj(0x48, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 1.0f / work->smokeLife, 1.0f, 1.0f);
    PSMTXTrans(trans, work->smokePhase0, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scaleMtx, 1.0f / work->smokeLife, 1.0f, 1.0f);
    PSMTXTrans(trans, work->smokePhase1, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX1, GX_MTX2x4);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scaleMtx, work->scale * work->scaleX, work->scale * work->scaleY, work->scale);
    PSMTXConcat(trans, rot, m54);
    PSMTXConcat(m54, scaleMtx, m54);
    PSMTXConcat(camera->view, m54, m54);
    GXLoadPosMtxImm(m54, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    switch (type) {
        case 3:
        case 4:
            GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xCD, 0x15, 0xFF });
            GXSetTevColor(GX_TEVREG1, (GXColor){ 0xFF, 0x51, 0x00, 0xFF });
            break;
        case 8:
            GXSetTevColor(GX_TEVREG0, (GXColor){ 0x59, 0x33, 0x59, 0xFF });
            GXSetTevColor(GX_TEVREG1, (GXColor){ 0x00, 0x00, 0x00, 0xFF });
            break;
    }
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A2, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG2, (GXColor){ work->glowAlpha, work->glowAlpha, work->glowAlpha, work->glowAlpha });
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, (GXColor){ 0, 0, 0, work->alpha });
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        f32 w = 24.0f;
        f32 h = 40.0f;
        GXPosition3f32(-w / 2.0f, h, 0.0f);
        GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w * 0.5f, h, 0.0f);
        GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w * 0.5f, 0.0f, 0.0f);
        GXColor4u8(0x00, 0x00, 0x00, 0xFF);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2.0f, 0.0f, 0.0f);
        GXColor4u8(0x00, 0x00, 0x00, 0xFF);
        GXTexCoord2f32(0.0f, 1.0f);
    }
    switch (type) {
        case 3:
        case 8:
            PSMTXTrans(trans, 0.0f, 20.0f, 0.0f);
            break;
        case 4:
            PSMTXTrans(trans, 0.0f, 25.0f, 0.0f);
            break;
    }
    PSMTXScale(scaleMtx, work->pulseScale, work->pulseScale, work->pulseScale);
    PSMTXConcat(trans, scaleMtx, trans);
    if (type == 4) {
        PSMTXRotRad(rot, 'z', 0.7853982f);
        PSMTXConcat(trans, rot, trans);
    }
    PSMTXConcat(m54, trans, trans);
    GXLoadPosMtxImm(trans, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    switch (type) {
        case 3:
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xEA, 0x5D, 0x4C });
            break;
        case 4:
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xEA, 0x5D, 0x26 });
            break;
        case 8:
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x00, 0x00, 0x00, 0x4C });
            break;
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    switch (type) {
        case 3:
        case 8:
            effGetTexObj(0x5f, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 4:
            effGetTexObj(0x60, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        f32 u, v;
        f32 w = 32.0f;
        if (type == 3 || type == 8) {
            u = 2.0f;
            v = 1.0f;
        } else {
            u = 2.0f;
            v = 2.0f;
        }
        GXPosition3f32(-w / 2.0f, w * 0.5f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w * 0.5f, w * 0.5f, 0.0f);
        GXTexCoord2f32(u, 0.0f);
        GXPosition3f32(w * 0.5f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(u, v);
        GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, v);
    }
}

void effFireDisp3(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    GXColor* col;
    EffFireWork* work;
    s32 i;
    s32 type;
    s32 pass;
    f32 w = 32.0f;
    Vec p;
    Vec scr;
    GXTexObj texObj;
    Mtx trans, scaleMtx, rot, mb0, m80;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    type = work->type;
    effGetTexObj(0x49, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(0x49, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 1.0f / work->smokeLife, 1.0f, 1.0f);
    PSMTXTrans(trans, work->smokePhase0, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    PSMTXScale(scaleMtx, 1.0f / work->smokeLife, 1.0f, 1.0f);
    PSMTXTrans(trans, work->smokePhase1, 0.0f, 0.0f);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX1, GX_MTX2x4);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scaleMtx, work->scale * work->scaleX, work->scale * work->scaleY, work->scale);
    PSMTXConcat(trans, rot, mb0);
    PSMTXConcat(mb0, scaleMtx, mb0);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    if (work->useMapBlend && (mapGetWork()->entries[0].flags & 2)) {
        GXColor blend;
        mapGetBlend(&blend);
        GXSetNumTevStages(4);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C1, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);
        GXSetTevColor(GX_TEVREG1, blend);
    }
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, work->alpha });
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work->scrMinX = 10000;
    work->scrMaxY = -10000;
    work->scrMaxX = -10000;
    work->scrMinY = 10000;
    col = col_tbl[type - 5];
    for (pass = 0; pass < 2; pass++) {
        switch (pass) {
            case 0:
                PSMTXIdentity(scaleMtx);
                break;
            case 1:
                PSMTXTrans(trans, 0.0f, 5.0f, 1.0f);
                PSMTXScale(scaleMtx, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(trans, scaleMtx, scaleMtx);
                break;
        }
        PSMTXConcat(mb0, scaleMtx, m80);
        PSMTXConcat(camera->view, m80, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 12);
        for (i = 0; i < 3; i++) {
            s32 n = i + 1;
            GXPosition3f32(-w / 2.0f, (40.0f / 3.0f) * n, 0.0f);
            GXColor4u8(col[n].r, col[n].g, col[n].b, col[n].a);
            GXTexCoord2f32(0.0f, (1.0f / 3.0f) * (2 - i));
            GXPosition3f32(w * 0.5f, (40.0f / 3.0f) * n, 0.0f);
            GXColor4u8(col[n].r, col[n].g, col[n].b, col[n].a);
            GXTexCoord2f32(1.0f, (1.0f / 3.0f) * (2 - i));
            GXPosition3f32(w * 0.5f, (40.0f / 3.0f) * i, 0.0f);
            GXColor4u8(col[i].r, col[i].g, col[i].b, col[i].a);
            GXTexCoord2f32(1.0f, (1.0f / 3.0f) * (3 - i));
            GXPosition3f32(-w / 2.0f, (40.0f / 3.0f) * i, 0.0f);
            GXColor4u8(col[i].r, col[i].g, col[i].b, col[i].a);
            GXTexCoord2f32(0.0f, (1.0f / 3.0f) * (3 - i));
        }
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = -w / 2.0f;
            t.y = 40.0f;
            p = t;
        }
        PSMTXMultVec(m80, &p, &p);
        getScreenPoint(&p, &scr);
        if (scr.x < work->scrMinX) {
            work->scrMinX = scr.x;
        }
        if (scr.x > work->scrMaxX) {
            work->scrMaxX = scr.x;
        }
        if (scr.y > work->scrMaxY) {
            work->scrMaxY = scr.y;
        }
        if (scr.y < work->scrMinY) {
            work->scrMinY = scr.y;
        }
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = w * 0.5f;
            t.y = 40.0f;
            p = t;
        }
        PSMTXMultVec(m80, &p, &p);
        getScreenPoint(&p, &scr);
        if (scr.x < work->scrMinX) {
            work->scrMinX = scr.x;
        }
        if (scr.x > work->scrMaxX) {
            work->scrMaxX = scr.x;
        }
        if (scr.y > work->scrMaxY) {
            work->scrMaxY = scr.y;
        }
        if (scr.y < work->scrMinY) {
            work->scrMinY = scr.y;
        }
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = w * 0.5f;
            p = t;
        }
        PSMTXMultVec(m80, &p, &p);
        getScreenPoint(&p, &scr);
        if (scr.x < work->scrMinX) {
            work->scrMinX = scr.x;
        }
        if (scr.x > work->scrMaxX) {
            work->scrMaxX = scr.x;
        }
        if (scr.y > work->scrMaxY) {
            work->scrMaxY = scr.y;
        }
        if (scr.y < work->scrMinY) {
            work->scrMinY = scr.y;
        }
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = -w / 2.0f;
            p = t;
        }
        PSMTXMultVec(m80, &p, &p);
        getScreenPoint(&p, &scr);
        if (scr.x < work->scrMinX) {
            work->scrMinX = scr.x;
        }
        if (scr.x > work->scrMaxX) {
            work->scrMaxX = scr.x;
        }
        if (scr.y > work->scrMaxY) {
            work->scrMaxY = scr.y;
        }
        if (scr.y < work->scrMinY) {
            work->scrMinY = scr.y;
        }
        col += 4;
    }
    work->scrMinX += 304;
    work->scrMaxX += 304;
    work->scrMaxY = 240 - work->scrMaxY;
    work->scrMinY = 240 - work->scrMinY;
    if (work->scrMinX < 0) {
        work->scrMinX = 0;
    }
    if (work->scrMaxX > 608) {
        work->scrMaxX = 608;
    }
    if (work->scrMaxY < 0) {
        work->scrMaxY = 0;
    }
    if (work->scrMinY > 480) {
        work->scrMinY = 480;
    }
    work->bboxValid = 1;
}







static void effFireSmokeEntry(f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* smoke = effEntry();
    EffFireSmokeWork* sw;
    smoke->type = "FireSmoke";
    smoke->count = 1;
    sw = __memAlloc(HEAP_EFFECT, 0x40);
    smoke->userdata = sw;
    smoke->callback = effFireSmokeMain;
    smoke->flags |= 2;
    sw->unk0 = 0;
    sw->color[3] = 0x80;
    sw->position = (Vec){ x, y, z };
    sw->scale = scale;
    sw->life = 60;
    sw->age = 0;
    sw->velocity.x = 0.0f;
    sw->velocity.y = 1.0f;
    sw->velocity.z = 0.0f;
    sw->rotSpeed = -8.0f;
    sw->color[0] = 0xCF;
    sw->color[1] = 0x67;
    sw->color[2] = 0x38;
}

void effFireSmokeMain(EffectEntry* effect) {
    EffFireSmokeWork* work = effect->userdata;

    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 16) {
        work->color[3] = work->life * 8;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    work->age++;
    work->velocity.x *= 0.95f;
    work->velocity.y *= 0.95f;
    work->velocity.z *= 0.95f;
    work->position.x += work->velocity.x;
    work->position.y += work->velocity.y;
    work->position.z += work->velocity.z;
    work->rotation += work->rotSpeed;
    work->color[0] *= 0.99f;
    work->color[1] *= 0.99f;
    work->color[2] *= 0.99f;
    dispEntry(CAMERA_3D, dispCalcZ(&work->position), 2, (DispCallback)effFireSmokeDisp, effect);
}

void effFireSmokeDisp(CameraId cameraId, EffectEntry* effect) {
    EffFireSmokeWork* work = effect->userdata;
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx trans, scaleMtx, rot, m;
    f32 sx, sy;
    u32 idx;
    f32 w = 1.0f;

    camera = camGetPtr(cameraId);
    idx = work->age * 2;
    if (idx < 60) {
        sx = scale_dt[idx] / 128.0f;
        sy = scale_dt[idx + 1] / 128.0f;
    } else {
        sx = 1.5f;
        sy = 0.8f;
    }
    effGetTexObj(33, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ work->color[0], work->color[1], work->color[2], work->color[3] });
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    {
        f32 s = 8.0f * work->scale;
        PSMTXScale(scaleMtx, s * sx, s * sy, s);
    }
    PSMTXConcat(trans, rot, m);
    PSMTXConcat(m, scaleMtx, m);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->rotation));
    PSMTXConcat(m, rot, m);
    PSMTXConcat(camera->view, m, m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-w / 2.0f, w * 0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w * 0.5f, w * 0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w * 0.5f, -w / 2.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
}
