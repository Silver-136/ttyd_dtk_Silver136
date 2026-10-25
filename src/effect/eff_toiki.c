#include "eff/eff_toiki.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

void* effMistEntry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 angle);

typedef struct EffToikiWork {
    s32 type;         // 0x0
    Vec position;     // 0x4
    Vec velocity;     // 0x10
    f32 scale;        // 0x1C
    f32 angle;        // 0x20
    f32 texOffset;    // 0x24
    f32 alpha;        // 0x28
    s32 counter;      // 0x2C
    s32 timer;        // 0x30
} EffToikiWork;


static s16 queen_toiki_vertex_tbl[] __attribute__((aligned(32))) = {
    -20614, -6729, 0, -20385, -6698, 0, -19918, -6319,
    0, -19960, -6208, 0, -20494, -6254, 0, -22117,
    -6948, 0, -22371, -7251, 0, -22119, -7687, 0,
    -21487, -7712, 0, -20289, -7445, 0, -6210, -1817,
    0, -21254, -6859, 0, -20363, -7017, 0, -19239,
    -6349, 0, -19416, -5753, 0, -20134, -5715, 0,
    -22732, -6947, 0, -23030, -7425, 0, -22417, -7994,
    0, -21502, -8010, 0, -20149, -7666, 0, -6090,
    -1986, 0, -19842, -6843, 0, -20110, -6569, 0,
    -21396, -6515, 0, -21573, -6183, 0, -22914, -7767,
    0, -22353, -7491, 0, -19169, -5914, 0, -8707,
    -2815, 0, -8582, -2992, 0, -13924, -6394, 0,
    -13736, -6259, 0, -13136, -6212, 0, -13091, -6321,
    0, -13510, -6655, 0, -15155, -7222, 0, -15588,
    -7171, 0, -15611, -6693, 0, -15165, -6226, 0,
    -14093, -5615, 0, -9, 0, 0, -14477, -6740,
    0, -13940, -6012, 0, -12665, -5722, 0, -12383,
    -6276, 0, -12877, -6798, 0, -15610, -7694, 0,
    -16155, -7553, 0, -16102, -6718, 0, -15450, -6076,
    0, -14231, -5394, 0, -126, 169, 0, -13443,
    -5780, 0, -13448, -6163, 0, -14343, -7087, 0,
    -14243, -7450, 0, -16306, -7225, -2, -15710, -6975,
    0, -12315, -5990, 0, -3702, -1240, 0, -3562,
    -1416, 0,
};
static s16 queen_toiki_texcoord0_tbl[] __attribute__((aligned(32))) = {
    -19660, 16384, -19660, 0, -20479, 0, -20479, 16384,
    -18022, 16384, -18022, 0, -18841, 0, -18841, 16384,
    -17203, 16384, -17203, 0, -16384, 16384, -16384, 0,
    -15564, 16384, -15564, 0, -13926, 16384, -13926, 0,
    -14745, 0, -14745, 16384, -13107, 16384, -13107, 0,
    -12288, 16384, -12288, 0, -11468, 16384, -11468, 0,
    -4056, 16384, -2457, 16384, -2457, 0, -4054, 0,
    -10649, 0, -10649, 16384, -9830, 0, -9011, 0,
    -9011, 16384, -9830, 16384, -8192, 0, -8192, 16384,
    -7372, 0, -7372, 16384, -6553, 0, -6553, 16384,
    -1661, 0, 0, 0, 0, 16384, -1653, 16384,
};
static s8 queen_toiki_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64, 32, 36, 40,
};
static u8 queen_toiki_color0_tbl[] __attribute__((aligned(32))) = {
    0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x00,
};
static u8 queen_toiki_dl_0_0[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x15, 0x1D, 0x00, 0x00, 0x18, 0x1E, 0x00, 0x00, 0x1B, 0x09, 0x00, 0x00, 0x16, 0x14,
    0x00, 0x00, 0x17, 0x08, 0x00, 0x00, 0x14, 0x13, 0x00, 0x00, 0x15, 0x07, 0x00, 0x00, 0x12, 0x12,
    0x00, 0x00, 0x13, 0x1B, 0x00, 0x00, 0x12, 0x1A, 0x00, 0x00, 0x13, 0x06, 0x00, 0x00, 0x0E, 0x11,
    0x00, 0x00, 0x0F, 0x05, 0x00, 0x00, 0x11, 0x10, 0x00, 0x00, 0x10, 0x18, 0x00, 0x00, 0x0C, 0x19,
    0x00, 0x00, 0x0D, 0x04, 0x00, 0x00, 0x0A, 0x0F, 0x00, 0x00, 0x0B, 0x03, 0x00, 0x00, 0x08, 0x0E,
    0x00, 0x00, 0x09, 0x1C, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_1[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x0B, 0x2D, 0x01, 0x00, 0x15, 0x22, 0x01, 0x00, 0x14, 0x3B, 0x01, 0x00, 0x15, 0x21,
    0x01, 0x00, 0x12, 0x2C, 0x01, 0x00, 0x13, 0x36, 0x01, 0x00, 0x0E, 0x35, 0x01, 0x00, 0x0F, 0x20,
    0x01, 0x00, 0x11, 0x2B, 0x01, 0x00, 0x10, 0x1F, 0x01, 0x00, 0x0C, 0x2A, 0x01, 0x00, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_2[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x0A, 0x03, 0x00, 0x00, 0x08, 0x1C, 0x00, 0x00, 0x09, 0x02, 0x00, 0x00, 0x04, 0x0D,
    0x00, 0x00, 0x05, 0x17, 0x00, 0x00, 0x07, 0x16, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x0C,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x0B, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_3[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x29, 0x01, 0x01, 0x2A, 0x3D, 0x01, 0x00, 0x2B, 0x34, 0x01, 0x01, 0x29, 0x3C,
    0x01, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_4[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x3D, 0x01, 0x00, 0x2B, 0x28, 0x01, 0x00, 0x27, 0x3C, 0x01, 0x00, 0x28, 0x33,
    0x01, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_5[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x28, 0x01, 0x00, 0x27, 0x27, 0x01, 0x00, 0x25, 0x33, 0x01, 0x00, 0x26, 0x32,
    0x01, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_6[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x27, 0x01, 0x00, 0x25, 0x26, 0x01, 0x00, 0x23, 0x32, 0x01, 0x00, 0x24, 0x31,
    0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_7[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x26, 0x01, 0x00, 0x23, 0x3A, 0x01, 0x00, 0x23, 0x31, 0x01, 0x00, 0x22, 0x39,
    0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_8[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x3A, 0x01, 0x00, 0x23, 0x25, 0x01, 0x00, 0x20, 0x39, 0x01, 0x00, 0x22, 0x30,
    0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_9[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x25, 0x01, 0x00, 0x20, 0x24, 0x01, 0x00, 0x21, 0x30, 0x01, 0x00, 0x1F, 0x2F,
    0x01, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_10[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x24, 0x01, 0x00, 0x21, 0x37, 0x01, 0x00, 0x1D, 0x2F, 0x01, 0x00, 0x1E, 0x38,
    0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_11[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x37, 0x01, 0x00, 0x1D, 0x23, 0x01, 0x00, 0x16, 0x38, 0x01, 0x00, 0x1C, 0x2E,
    0x01, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_12[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x23, 0x01, 0x00, 0x16, 0x22, 0x01, 0x00, 0x14, 0x2E, 0x01, 0x00, 0x17, 0x2D,
    0x01, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8 queen_toiki_dl_0_13[] __attribute__((aligned(32))) = {
    0x98, 0x00, 0x04, 0x15, 0x00, 0x01, 0x1A, 0x1E, 0x00, 0x00, 0x1B, 0x0A, 0x00, 0x01, 0x19, 0x1D,
    0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static u8* queen_toiki_dl_0_tbl[] = { queen_toiki_dl_0_0, queen_toiki_dl_0_1, queen_toiki_dl_0_2, queen_toiki_dl_0_3, queen_toiki_dl_0_4, queen_toiki_dl_0_5, queen_toiki_dl_0_6, queen_toiki_dl_0_7, queen_toiki_dl_0_8, queen_toiki_dl_0_9, queen_toiki_dl_0_10, queen_toiki_dl_0_11, queen_toiki_dl_0_12, queen_toiki_dl_0_13 };
static u8 queen_toiki_dl_0_size_tbl[] = { 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

EffectEntry* effToikiEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffToikiWork* work;

    effect = effEntry();
    effect->type = "Toiki";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffToikiWork));
    effect->userdata = work;
    effect->callback = effToikiMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.5f;
    work->angle = 0.0f;
    work->timer = duration;
    work->counter = 0;
    work->alpha = 255.0f;
    work->velocity.x = 2.5f * (f32)cos(6.2831855f * (200.0f + work->angle) / 360.0f);
    work->velocity.y = 2.5f * (f32)sin(6.2831855f * (200.0f + work->angle) / 360.0f);
    work->velocity.z = 0.0f;
    work->texOffset = 0.0f;
    return effect;
}

void effToikiMain(EffectEntry* effect) {
    EffToikiWork* work = effect->userdata;
    Vec position;
    Vec mist;
    Vec tmp = { 0.0f, 0.0f, 0.0f };
    Mtx rot;
    s32 type;
    f32 angle;
    f32 radius;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    type = work->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    work->position.x += work->velocity.x;
    work->position.y += work->velocity.y;
    work->position.z += work->velocity.z;
    work->velocity.x *= 0.95f;
    work->velocity.y *= 0.95f;
    work->velocity.z *= 0.95f;
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (work->texOffset > 1.0f && work->texOffset < 4.0f && work->timer % 20 == 0) {
        radius = 320.0f * work->scale;
        angle = 6.2831855f * (200.0f + work->angle) / 360.0f;
        mist.x = radius * (f32)cos(angle) + work->position.x;
        mist.y = radius * (f32)sin(angle) + work->position.y;
        mist.z = 1.0f + work->position.z;
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(CAMERA_3D)->viewYaw));
        PSMTXMultVec(rot, &mist, &mist);
        effMistEntry(type + 1, 60, mist.x, mist.y, mist.z, 0.0f);
    }
    work->counter++;
    work->texOffset += 0.015f;
    if (work->texOffset > 4.0f) {
        work->texOffset = 4.0f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effToikiDisp, effect);
}

void effToikiDisp(CameraId cameraId, EffectEntry* effect) {
    EffToikiWork* work = effect->userdata;
    static u8 color_tbl[3][4] = { { 0x03, 0x02, 0x21, 0xFF }, { 0xD8, 0x0F, 0x51, 0xFF }, { 0x0B, 0x1B, 0x00, 0xFF } };
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    CameraEntry* camera;
    u8** dl;
    u32 i;
    u8* sz;
    s32 type;

    camera = camGetPtr(cameraId);
    type = work->type;
    effGetTexObj(0x5C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetTevColor(GX_TEVREG0, (GXColor){ color_tbl[type][0], color_tbl[type][1], color_tbl[type][2],
                                         (u8)work->alpha });
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(mtx, work->texOffset, 0.0f, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', MTXDegToRad(work->angle));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 6);
    GXSetArray(GX_VA_POS, queen_toiki_vertex_tbl, 6);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetArray(GX_VA_NRM, queen_toiki_normal_tbl, 3);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, queen_toiki_color0_tbl, 4);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
    GXSetArray(GX_VA_TEX0, queen_toiki_texcoord0_tbl, 4);
    {
        for (i = 0, sz = queen_toiki_dl_0_size_tbl, dl = queen_toiki_dl_0_tbl; i < 14; i++, sz++, dl++) {
            GXCallDisplayList(*dl, *sz << 5);
        }
    }
}
