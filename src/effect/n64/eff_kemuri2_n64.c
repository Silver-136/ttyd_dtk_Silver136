#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_n64.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct EffKemuri2N64Work {
    s32 active;     // 0x0
    u16 type;       // 0x4
    u8 pad_6[2];    // 0x6
    Vec position;   // 0x8
    u8 pad_14[0x44 - 0x14];
    f32 field_44;   // 0x44
    f32 velY;       // 0x48
    f32 accelY;     // 0x4C
    f32 dirX;       // 0x50
    f32 dirZ;       // 0x54
    s32 kind;       // 0x58
    s32 counter;    // 0x5C
    s32 frame;      // 0x60
} EffKemuri2N64Work;

static s8 anim_data_0[] = {0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, -1};
static s8 anim_data_1[] = {0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, -1};
static s8* anim_data_table[] = {anim_data_0, anim_data_1};

void effKemuri2Main(EffectEntry* effect);
static void effKemuri2Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKemuri2N64Entry(s32 type, f32 x, f32 y, f32 z, f32 dirX, f32 dirZ) {
    EffectEntry* effect;
    EffKemuri2N64Work* work;

    effect = effEntry();
    effect->type = "Kemuri2N64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffKemuri2N64Work));
    effect->userdata = work;
    effect->callback = effKemuri2Main;
    work->kind = (type == 2);
    work->counter = 0;
    work->type = type;
    work->active = 1;
    work->dirX = dirX;
    work->dirZ = dirZ;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->field_44 = 0.33333334f;
    work->velY = 1.75f;
    work->accelY = -0.16f;
    return effect;
}

void effKemuri2Main(EffectEntry* effect) {
    EffKemuri2N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if ((work->frame = anim_data_table[work->kind][work->counter++]) < 0) {
        effDelete(effect);
        return;
    }
    for (i = 0; i < effect->count; i++, work++) {
        work->position.x += work->field_44 * work->dirX;
        work->position.z += work->field_44 * work->dirZ;
        work->velY += work->accelY;
        work->position.y += work->velY;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effKemuri2Disp, effect);
}

static void main_dl(s32 type) {
    GXTexObj texObj;
    s16 w;
    s16 h;

    switch (type) {
        case 0:
            effGetTexObjN64(0xC, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 1:
            effGetTexObjN64(0xD, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 2:
        default:
            effGetTexObjN64(0xE, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
    }
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    switch (type) {
        case 0:
            w = 16;
            h = 16;
            break;
        case 1:
        default:
            w = 24;
            h = 24;
            break;
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-w / 2, h / 2, 0);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3s16(w / 2, h / 2, 0);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3s16(w / 2, -h / 2, 0);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3s16(-w / 2, -h / 2, 0);
    GXTexCoord2f32(0.0f, 1.0f);
}

static void effKemuri2Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera = camGetPtr(cameraId);
    EffKemuri2N64Work* work = effect->userdata;
    s32 cell;
    s32 i;
    s32 wf = work->frame;
    s32 type = work->type;
    s32 flag = 0;
    s32 frame;

    f32 texW;
    f32 sx;
    f32 texH;
    f32 sy;
    f32 ty;
    Mtx trans;
    Mtx rot;
    Mtx scaleMtx;
    Mtx mtx;
    GXColor c1;
    GXColor c2;

    if ((work->kind == 0 && wf == 6) || (work->kind == 1 && wf == 7)) {
        flag = 1;
    }
    c1 = (GXColor){0xE6, 0xDE, 0xDE, 0x6E};
    GXSetTevColor(GX_TEVREG0, c1);
    c2 = (GXColor){0, 0, 0, 0x7F};
    GXSetTevColor(GX_TEVREG1, c2);
    switch (type) {
        case 0:
            texW = 176.0f;
            texH = 24.0f;
            break;
        case 1:
            texW = 224.0f;
            texH = 32.0f;
            break;
        case 2:
        default:
            texW = 256.0f;
            texH = 32.0f;
            break;
    }
    cell = 32;
    if (type == 0) {
        cell = 24;
    }
    frame = wf * cell;
    sx = (f32)cell / texW;
    sy = (f32)cell / texH;
    PSMTXScale(scaleMtx, sx, sy, 1.0f);
    ty = 0.0f / texH;
    PSMTXTrans(trans, (f32)frame / texW, ty, 0.0f);
    PSMTXConcat(trans, scaleMtx, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    if (flag) {
        PSMTXScale(scaleMtx, sx, 0.0f, 1.0f);
        PSMTXTrans(trans, (f32)(frame + cell) / texW, ty, 0.0f);
        PSMTXConcat(trans, scaleMtx, trans);
        GXLoadTexMtxImm(trans, GX_TEXMTX1, GX_MTX2x4);
    } else {
        PSMTXScale(scaleMtx, sx, sy, 1.0f);
        PSMTXTrans(trans, (f32)(frame + cell) / texW, ty, 0.0f);
        PSMTXConcat(trans, scaleMtx, trans);
        GXLoadTexMtxImm(trans, GX_TEXMTX1, GX_MTX2x4);
    }
    for (i = 0; i < effect->count; i++, work++) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
        PSMTXConcat(trans, rot, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        main_dl(type);
    }
}
