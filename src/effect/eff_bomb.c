#include "eff/eff_bomb.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "eff/eff_confetti.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffBombWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scaleX;   // 0x10
    f32 scaleY;   // 0x14
    f32 scale;    // 0x18, element 0: overall scale
    f32 angle;    // 0x1C
    s32 frame;    // 0x20, element 0: age
    s32 offset;   // 0x24
} EffBombWork;

// per frame: texture frame (of 16), x scale and y scale in percent; scale_dt for elements 1-4, scale_dt2 for 5-8
static u8 scale_dt[] = {
    0,  100, 100, 0,  103, 103, 1,  106, 106, 1,  110, 110, 2,  113, 113, 2,  117, 117,
    3,  120, 120, 3,  124, 124, 4,  127, 127, 4,  131, 131, 5,  134, 134, 5,  137, 137,
    6,  141, 141, 6,  144, 144, 7,  148, 148, 7,  151, 151, 8,  155, 155, 8,  158, 158,
    9,  162, 162, 9,  165, 165, 10, 168, 168, 10, 172, 172, 11, 175, 175, 11, 179, 179,
    12, 182, 182, 12, 186, 186, 13, 189, 189, 13, 193, 193, 14, 196, 196, 14, 200, 200,
    15, 200, 200, 15, 200, 200, 15, 200, 200, 15, 200, 200, 15, 200, 200, 15, 200, 200,
};

static u8 scale_dt2[] = {
    0,  0,   0,   0,  0,   0,   0,  0,   0,   0,  0,   0,   0,  100, 100, 0,  101, 101,
    1,  103, 103, 1,  105, 105, 2,  106, 106, 2,  108, 108, 3,  110, 110, 3,  112, 112,
    4,  113, 113, 4,  115, 115, 5,  117, 117, 5,  118, 118, 6,  120, 120, 6,  122, 122,
    7,  124, 124, 7,  125, 125, 8,  127, 127, 8,  129, 129, 9,  131, 131, 9,  132, 132,
    10, 134, 134, 10, 136, 136, 11, 137, 137, 11, 139, 139, 12, 141, 141, 12, 143, 143,
    13, 144, 144, 13, 146, 146, 14, 148, 148, 14, 150, 150, 15, 148, 148, 15, 150, 150,
};

static void effBombDisp(CameraId cameraId, EffectEntry* effect);
static void effBombMain(EffectEntry* effect);

EffectEntry* effBombEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffBombWork* work;
    EffBombWork* bomb;
    f32 size;
    f32 s;
    s32 i;

    effect = effEntry();
    effect->type = "Bomb";
    effect->count = 9;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffBombWork));
    effect->userdata = work;
    effect->callback = effBombMain;
    size = 10.0f * scale;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->frame = 0;
    work->offset = 0;
    bomb = work + 1;
    for (i = 1; i < effect->count; i++, bomb++) {
        bomb->angle = 90.0f * (f32)(i - 1) + (i > 4 ? 45.0f : 0.0f);
        s = sin(0.017453292f * bomb->angle);
        bomb->position.x = size * s;
        s = cos(0.017453292f * bomb->angle);
        bomb->position.y = size * s;
        bomb->position.z = 0.0f;
        bomb->scaleX = 0.0f;
        bomb->scaleY = 0.0f;
        bomb->offset = 0;
        bomb->frame = 0;
    }
    effConfettiEntry(4, 40, x, y, z);
    return effect;
}

static void effBombMain(EffectEntry* effect) {
    EffBombWork* work = effect->userdata;
    Vec position;
    Vec local = { 0.0f, 0.0f, 0.0f };
    s32 i, frame;

    local.x = work->position.x;
    local.y = work->position.y;
    local.z = work->position.z;
    position = local;
    frame = work->frame;
    work->frame = frame + 1;
    if (work->frame > 36) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (i < 5) {
            if ((u32)frame < 54) {
                work->offset = scale_dt[frame * 3];
                work->scaleX = scale_dt[frame * 3 + 1] / 100.0f;
                work->scaleY = scale_dt[frame * 3 + 2] / 100.0f;
            }
        } else if ((u32)frame < 54) {
            work->offset = scale_dt2[frame * 3];
            work->scaleX = scale_dt2[frame * 3 + 1] / 100.0f;
            work->scaleY = scale_dt2[frame * 3 + 2] / 100.0f;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effBombDisp, effect);
}

static void effBombDisp(CameraId cameraId, EffectEntry* effect) {
    EffBombWork* work;
    CameraEntry* camera;
    GXTexObj texObj;
    s32 i;
    f32 scale;
    f32 one;
    f32 left;
    f32 half;
    f32 deg;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx rot;
    Mtx base;
    Mtx texMtx;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    scale = work->scale;
    effGetTexObj(36, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                    GX_TEVREG0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_KONST, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1_2);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                    GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_TEXA, GX_CA_KONST, GX_CA_A1, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_1_2);
    GXSetTevColor(GX_TEVREG1, (GXColor){ 0xFF, 0xFF, 0xFF, 0x30 });
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_C0, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_APREV, GX_CA_ZERO, GX_CA_A0, GX_CA_A0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(camera->view, base, base);
    GXSetCurrentMtx(GX_PNMTX0);
    deg = 0.017453292f;
    scale = 30.0f * scale;
    half = 0.5f;
    one = 1.0f;

    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(mtx, 0.0625f * work->offset, 0.0f, 0.0f);
        PSMTXScale(scaleMtx, 0.0625f, 1.0f, 1.0f);
        PSMTXConcat(mtx, scaleMtx, texMtx);
        GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
        PSMTXTrans(mtx, -0.05f, -0.05f, 0.0f);
        PSMTXConcat(texMtx, mtx, texMtx);

        GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX2x4);
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'z', deg * -work->angle);
        PSMTXScale(scaleMtx, scale * work->scaleX, scale * work->scaleY, scale);
        PSMTXConcat(mtx, rot, texMtx);
        PSMTXConcat(texMtx, scaleMtx, texMtx);
        PSMTXConcat(base, texMtx, texMtx);
        GXLoadPosMtxImm(texMtx, GX_PNMTX0);
        left = -one / 2.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(left, one, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(half, one, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(half, 0.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(left, 0.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}
