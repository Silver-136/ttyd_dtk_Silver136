#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

EffectEntry* effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
void effIbukiMain(EffectEntry* effect);
void effIbukiDisp(CameraId cameraId, EffectEntry* effect);

typedef struct EffIbukiWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    Vec velocity;  // 0x10
    f32 x2;        // 0x1C
    f32 y2;        // 0x20
    f32 vx2;       // 0x24
    f32 vy2;       // 0x28
    f32 scale;     // 0x2C
    s32 life;      // 0x30
    s32 timer;     // 0x34
    s32 alpha;     // 0x38
    s32 alpha2;    // 0x3C
    f32 tex[4];    // 0x40
    s32 delay;     // 0x50
} EffIbukiWork;

__declspec(section ".data") f32 ibuki_negone_one_a[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 fd_eff_ibuki_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 ibuki_one_negone_a[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 fd_eff_ibuki_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 ibuki_negone_one_b[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 fd_eff_ibuki_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};
__declspec(section ".data") f32 ibuki_one_negone_b[2] __attribute__((aligned(8))) = {1.0f, -1.0f};
__declspec(section ".data") f32 fd_eff_ibuki_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

static s16 ibuki_vertex_tbl[] __attribute__((aligned(32))) = { 0, 0, 0, 3840, 0, 0, 7679, 0, 0, 17919, 0, 0, 21760, 0, 0, 0, 1280, 0, 3840, 1280, 0, 7679, 1280, 0, 17919, 1280, 0, 21760, 1280, 0, 0, 2560, 0, 3840, 3840, 0, 7679, 5120, 0, 17919, 8960, 0, 21760, 10240, 0, 0, 0, 0 };
static s16 ibuki_texcoord0_tbl[] __attribute__((aligned(32))) = { 0, 8192, 2891, 11468, 2891, 16384, 0, 16384, 5782, 13107, 5782, 16384, 13492, 14336, 13492, 16384, 16384, 14336, 16384, 16384, 0, 0, 2891, 0, 5782, 0, 13492, 0, 16384, 0};
static s16 ibuki_texcoord1_tbl[] __attribute__((aligned(32))) = { 0, 8192, 2891, 11468, 2891, 16384, 0, 16384, 5782, 13107, 5782, 16384, 13492, 14336, 13492, 16384, 16384, 14336, 16384, 16384, 0, 0, 2891, 0, 5782, 0, 13492, 0, 16384, 0};
static u8 ibuki_dl_0_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x0A, 0x0A, 0x0B, 0x00, 0x02, 0x0B, 0x0B, 0x05, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x01, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 ibuki_dl_0_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0B, 0x00, 0x02, 0x0B, 0x0B, 0x0C, 0x00, 0x03, 0x0C, 0x0C, 0x06, 0x00, 0x01, 0x01, 0x01, 0x07, 0x00, 0x01, 0x04, 0x04, 0x01, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 ibuki_dl_0_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0C, 0x00, 0x03, 0x0C, 0x0C, 0x0D, 0x00, 0x01, 0x0D, 0x0D, 0x07, 0x00, 0x01, 0x04, 0x04, 0x08, 0x00, 0x01, 0x06, 0x06, 0x02, 0x00, 0x00, 0x05, 0x05, 0x03, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 ibuki_dl_0_3[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0D, 0x00, 0x01, 0x0D, 0x0D, 0x0E, 0x00, 0x00, 0x0E, 0x0E, 0x08, 0x00, 0x01, 0x06, 0x06, 0x09, 0x00, 0x00, 0x08, 0x08, 0x03, 0x00, 0x00, 0x07, 0x07, 0x04, 0x00, 0x00, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00 };
static s8 ibuki_normal_tbl[] __attribute__((aligned(32))) = { 0, 0, 64};
static u8 ibuki_color0_tbl[] __attribute__((aligned(32))) = { 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x66, 0xFF, 0xFF, 0xFF, 0x4C, 0xFF, 0xFF, 0xFF, 0x7F };
static u8* ibuki_dl_0_tbl[] = { ibuki_dl_0_0, ibuki_dl_0_1, ibuki_dl_0_2, ibuki_dl_0_3 };
static u8 ibuki_dl_0_size_tbl[4] = { 2, 2, 2, 2 };

EffectEntry* effIbukiEntry(s32 type, s32 life, f32 x, f32 y, f32 z) {
    s32 delay;
    EffectEntry* effect;
    EffIbukiWork* work;
    EffIbukiWork* p;
    s32 i;
    f32 a;

    effect = effEntry();
    effect->type = "Ibuki";
    effect->count = 33;
    work = __memAlloc(HEAP_EFFECT, 33 * sizeof(EffIbukiWork));
    effect->userdata = work;
    effect->callback = effIbukiMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->timer = 0;
    work->scale = 1.0f;
    work->alpha = 0;
    work->alpha2 = 0;
    work->tex[0] = 0.0f;
    work->tex[1] = 0.0f;
    work->tex[2] = 0.0f;
    work->tex[3] = 0.0f;
    p = work + 1;
    for (i = 1, (0, delay = 10); i < effect->count; delay += 10, i++, p++) {
        a = 30 - irand(40);
        p->position.x = 0.0f;
        p->position.y = irand(30);
        a = 6.2831855f * a / 360.0f;
        p->position.z = 0.0f;
        p->velocity.x = cos(a);
        p->velocity.y = sin(a);
        p->velocity.z = 0.0f;
        p->scale = 1.0f;
        p->alpha = 0;
        p->delay = delay;
        p->x2 = 0.0f;
        p->y2 = 0.0f;
        if (irand(1000) < 500) {
            a = irand(10) + 10;
            p->vx2 = a;
            p->vy2 = 0.0f;
        } else {
            p->vx2 = 0.0f;
            p->vy2 = irand(10) + 10;
        }
    }
    return effect;
}

void effIbukiMain(EffectEntry* effect) {
    s32 delay;
    EffIbukiWork* work = effect->userdata;
    s32 i;
    f32 a;
    Mtx rot;
    Vec position;
    Vec spawn;
    Vec v = { 0.0f, 0.0f, 0.0f };

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16 + 15;
    }
    if (work->life < 16) {
        work->alpha = work->life * 16;
    }
    if (work->timer >= 60 && work->timer < 92) {
        work->alpha2 = (work->timer - 60) * 8 + 7;
    }
    work->timer++;
    work->tex[0] += -0.0125f;
    work->tex[1] += 0.0125f;
    work->tex[2] += -0.0125f;
    work->tex[3] = 0.0f;
    if (work->timer > 120 && work->timer % 16 == 0) {
        spawn.x = work->position.x + irand(300);
        spawn.y = 30.0f + (work->position.y + irand(100));
        spawn.z = work->position.z;
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(4)->viewYaw));
        PSMTXMultVec(rot, &spawn, &spawn);
        effVaporN64Entry(2, 30, spawn.x, spawn.y, spawn.z, 1.0f + irand(10) / 10.0f);
    }
    work++;
    for (i = 1, delay = 10; i < effect->count; delay += 10, i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            continue;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->x2 += work->vx2;
        work->y2 += work->vy2;
        work->velocity.x *= 1.01f;
        work->velocity.y *= 1.01f;
        work->velocity.z *= 1.01f;
        work->vx2 *= 1.01f;
        work->vy2 *= 1.01f;
        work->alpha += 5;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
        if (work->position.x > 300.0f) {
            a = 30 - irand(40);
            work->position.x = 0.0f;
            work->position.y = irand(30);
            a = 6.2831855f * a / 360.0f;
            work->position.z = 0.0f;
            work->velocity.x = cos(a);
            work->velocity.y = sin(a);
            work->velocity.z = 0.0f;
            work->scale = 1.0f;
            work->alpha = 0;
            work->delay = delay;
            work->x2 = 0.0f;
            work->y2 = 0.0f;
            if (irand(1000) < 500) {
                work->vx2 = irand(10) + 10;
                work->vy2 = 0.0f;
            } else {
                work->vx2 = 0.0f;
                work->vy2 = irand(10) + 10;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effIbukiDisp, effect);
}

void effIbukiDisp(CameraId cameraId, EffectEntry* effect) {
    s32 i;
u8* sz; u8** dl;
    EffIbukiWork* work = effect->userdata;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx final;
    GXTexObj texObj;
    CameraEntry* cam;
    s32 alpha;
    s32 alpha2;
    s32 a;
    f32 w = 16.0f;
    f32 h;

    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    alpha2 = work->alpha2;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 255, 255, 255, alpha * alpha2 / 255 });
    PSMTXTrans(trans, work->tex[0], work->tex[1], 0.0f);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(trans, work->tex[2], 0.02f, 0.0f);
    GXLoadTexMtxImm(trans, GX_TEXMTX1, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    if (work->type != 0) {
        PSMTXScale(scale, -work->scale, work->scale, work->scale);
    } else {
        PSMTXScale(scale, work->scale, work->scale, work->scale);
    }
    PSMTXConcat(trans, rot, final);
    PSMTXConcat(final, scale, final);
    PSMTXConcat(cam->view, final, final);
    GXLoadPosMtxImm(final, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    effGetTexObj(0x3C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(0x62, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 6);
    GXSetArray(GX_VA_POS, ibuki_vertex_tbl, 6);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetArray(GX_VA_NRM, ibuki_normal_tbl, 3);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, ibuki_color0_tbl, 4);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
    GXSetArray(GX_VA_TEX0, ibuki_texcoord0_tbl, 4);
    GXSetVtxDesc(GX_VA_TEX1, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_S16, 14);
    GXSetArray(GX_VA_TEX1, ibuki_texcoord1_tbl, 4);
    for (i = 0, sz = ibuki_dl_0_size_tbl, dl = ibuki_dl_0_tbl; (u32)i < 4; i++, sz++, dl++) {
        GXCallDisplayList(*dl, *sz * 32);
    }
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
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    effGetTexObj(0x63, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    work++;
    h = 8.0f;
    for (i = 1; i < effect->count; i++, work++) {
        a = work->alpha * alpha / 255;
        if (work->delay == 0) {
            PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
            PSMTXRotRad(rot, 'x', MTXDegToRad(work->x2));
            PSMTXConcat(trans, rot, trans);
            PSMTXRotRad(rot, 'y', MTXDegToRad(work->y2));
            PSMTXConcat(trans, rot, trans);
            PSMTXScale(scale, work->scale, work->scale, work->scale);
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(final, trans, trans);
            GXLoadPosMtxImm(trans, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 255, 255, 255, a });
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-w / 2.0f, h, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(h, h, 0.0f);
            GXTexCoord2f32(2.0f, 0.0f);
            GXPosition3f32(h, -w / 2.0f, 0.0f);
            GXTexCoord2f32(2.0f, 2.0f);
            GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
            GXTexCoord2f32(0.0f, 2.0f);
        }
    }
}
