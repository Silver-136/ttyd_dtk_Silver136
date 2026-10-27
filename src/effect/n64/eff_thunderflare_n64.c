#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffThunderflareN64Elem {
    s32 unk0;     // 0x0
    s32 type;     // 0x4
    Vec position; // 0x8
    s32 alpha;    // 0x14
    s32 lifetime; // 0x18
    s32 frame;    // 0x1C
    f32 rotA;     // 0x20
    f32 rotB;     // 0x24
    f32 scale;    // 0x28
    f32 dist;     // 0x2C
    s32 color[6]; // 0x30
} EffThunderflareN64Elem;

static effN64Vtx size16x16_tex32x32_vtx[] = {
    {-8, -8, 0, 0, 0, 250, 250, 250, 255},
    {8, -8, 0, 1024, 0, 250, 250, 250, 255},
    {8, 8, 0, 1024, 1024, 250, 0, 0, 255},
    {-8, 8, 0, 0, 1024, 250, 0, 0, 255},
    {-12, -12, 0, -512, 512, 250, 250, 250, 255},
    {12, -12, 0, 512, -512, 250, 250, 250, 255},
    {12, 12, 0, 1536, 512, 250, 0, 0, 255},
    {-12, 12, 0, 512, 1536, 250, 0, 0, 255},
};

void effThunderflareMain(EffectEntry* effect);
void effThunderflareDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effThunderflareN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffThunderflareN64Elem* work;
    EffThunderflareN64Elem* e;
    s32 i;

    effect = effEntry();
    effect->type = "ThunderflareN64";
    effect->count = 31;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffThunderflareN64Elem) * 31);
    effect->userdata = work;
    effect->callback = effThunderflareMain;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 100;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    switch (type) {
        case 0:
            work->color[0] = 255;
            work->color[1] = 0;
            work->color[2] = 255;
            work->color[3] = 255;
            work->color[4] = 255;
            work->color[5] = 255;
            break;
        case 1:
        default:
            work->color[0] = 255;
            work->color[1] = 20;
            work->color[2] = 33;
            work->color[3] = 50;
            work->color[4] = 20;
            work->color[5] = 10;
            break;
    }
    e = work + 1;
    for (i = 1; i < 31; i++, e++) {
        e->rotA = (i - 1) * 3240 / 30;
        e->rotB = 0.0f;
        e->dist = (i - 1) * 100 / 30;
        e->unk0 = 0;
    }
    return effect;
}

void effThunderflareMain(EffectEntry* effect) {
    EffThunderflareN64Elem* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 odd;
    s32 i;
    f32 sa, ca, sb, cb, d, a, ta, tb, b;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    if (work->lifetime < 100) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (frame <= 20) {
        work->alpha = frame * 12;
    } else {
        work->alpha = 255;
    }
    if (frame & 1) {
        work->alpha = 0.5f * work->alpha;
    }
    odd = work->frame & 1;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        a = work->rotA;
        d = work->dist;
        b = work->rotB;
        ta = 6.2831855f * a / 360.0f;
        sa = sin(ta);
        ca = cos(ta);
        tb = 6.2831855f * b / 360.0f;
        sb = sin(tb);
        cb = cos(tb);
        work->position.x = cb * (d * sa);
        work->position.y = cb * (d * ca);
        work->position.z = d * sb;
        work->alpha = 2.0f * (255.0f * (100.0f - work->dist)) / 100.0f;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
        work->scale = 0.2f + work->dist / 80.0f;
        work->dist -= (i & 3) * 2 + 2;
        if (work->dist < 0.0f) {
            work->dist = 0.0f;
            work->scale = 0.0f;
        }
        work->rotA += odd ? 5 : 2;
        work->rotB += odd ? 2 : 5;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effThunderflareDisp, effect);
}

void effThunderflareDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffThunderflareN64Elem* work;
    s32 cr, cg, cb;
    f32 sc;
    s32 a;
    s32 i;
    GXTexObj tex;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx view;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    cr = work->color[0];
    cg = work->color[1];
    cb = work->color[2];
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, view);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG1, (GXColor){work->color[3], work->color[4], work->color[5]});
    work++;
    effGetTexObjN64(0x55, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    for (i = 1; i < effect->count; i++, work++) {
        sc = work->scale;
        if (sc) {
            GXSetTevColor(GX_TEVREG0, (GXColor){cr, cg, cb, work->alpha});
            PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
            PSMTXScale(scale, sc, sc, sc);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(view, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            a = (i & 1) * 4;
            tri2(a, a + 1, a + 2, 0, a, a + 2, a + 3, 0);
        }
    }
}
