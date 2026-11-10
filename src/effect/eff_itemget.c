// Item get / use sparkle (ItemGet): sparkles spread around the item position, spawned when Mario obtains or uses an
// item. Element 0 is the header: types 0, 3 and 9 draw seven sparkles, type 10 (item use in battle) five.

#include "version.h"
#include "driver/camdrv.h"
#include "eff/eff_itemget.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

static void effItemGetMain(EffectEntry* effect);
static void effItemGetDisp(CameraId cameraId, void* payload);
static void effItemUseDisp(CameraId cameraId, void* payload);

/**
 * Pop-in size of the sparkle burst over its first 12 frames, in percent: it swells from nothing to 121%
 * and settles at 100%. Read by effItemGetMain.
 */
static u8 scale_data[12] = { 0x00, 0x12, 0x26, 0x3B, 0x4E, 0x60, 0x6E, 0x78, 0x79, 0x72, 0x69, 0x64 };
/** Light grey-white used as the sparkles' highlight colour beside the per-type tint; set by effItemGetDisp. */
GXColor white = { 0xE1, 0xE1, 0xE1, 0xFF };
/**
 * Sparkle tint per effect type 0-9 (gold, green, lime, red, yellow-green, blue, orange, pink, slate
 * blue, gold). Read by effItemGetDisp (effItemUseDisp draws untinted).
 */
static GXColor color_tbl[10] = {
    { 0xC8, 0xB2, 0x25, 0xFF }, { 0x26, 0xA3, 0x52, 0xFF }, { 0x5C, 0xCA, 0x10, 0xFF }, { 0xCB, 0x4C, 0x4D, 0xFF },
    { 0x7E, 0xC2, 0x1E, 0xFF }, { 0x30, 0x9C, 0xBF, 0xFF }, { 0xEB, 0x7D, 0x06, 0xFF }, { 0xE8, 0x5B, 0xD0, 0xFF },
    { 0x51, 0x75, 0x9D, 0xFF }, { 0xC8, 0xB2, 0x25, 0xFF },
};

/** Base size of the sparkles per effect type 0-9, in tenths: 5.0 for types 0-2 down to 1.0. */
#if VERSION <= VERSION_DEMO_JP
static u8 size_tbl[10] = {0x32, 0x32, 0x32, 0x28, 0x14, 0x14, 0x14, 0x0A, 0x0A, 0x0F};
#else
static u8 size_tbl[10] = { 0x32, 0x32, 0x32, 0x28, 0x0C, 0x0C, 0x0C, 0x0A, 0x0A, 0x0F};
#endif

/**
 * Spawns item sparkles at (x, y, z): types 0, 3 and 9 use seven, type 10 (battle item use) five.
 * Spawned when Mario obtains or uses an item (itemdrv.c, battle_event_cmd.c), and by evt_sub.c, eff_naniga.c and
 * event scripts.
 */
EffectEntry* effItemGetEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    ItemGetWork* work;
    ItemGetWork* elem;
    s32 i;
    s32 count;
    f32 cv;

    effect = effEntry();
    count = 1;
    if (type == 0 || type == 3 || type == 9) {
        count = 8;
    } else if (type == 10) {
        count = 6;
    }
    effect->type = "ItemGet";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(ItemGetWork));
    effect->userdata = work;
    effect->callback = effItemGetMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.0f;
    work->size = 0.75f * ((f32)size_tbl[work->type] / 10.0f);
    work->timer = 0;
    work->state = 0;
    work->camera = CAMERA_3D;
    work->alpha = 255;
    work->lifetime = 1000;
    elem = work + 1;
    for (i = 1; i < effect->count; i++, elem++) {
        elem->position.x = 0.0f;
        elem->position.y = 0.0f;
        elem->position.z = 0.0f;
        if (type == 10) {
            elem->angle = 1.0471976f + 6.2831855f * (f32)(i - 1) / 6.0f;
            elem->velocity.x = 5.0f * (f32)sin(elem->angle);

            cv = (f32)cos(elem->angle);
            elem->velocity.y = 5.0f * -cv;
            elem->velocity.z = 0.0f;
        } else {
            elem->angle = 6.2831855f * (f32)(i - 1) / (f32)(effect->count - 1);
            elem->velocity.x = (f32)sin(elem->angle);
            elem->velocity.y = -(f32)cos(elem->angle);
            elem->velocity.z = 0.0f;
        }
        elem->alpha = 0;
        elem->timer = 0;
    }
    return effect;
}

/**
 * Pops the sparkle scale in over 12 frames and moves the sparkles while each fades in and out; a soft delete fades
 * the whole effect out over 16 frames and deletes it. Type 10 is also deleted once all its sparkles have faded.
 */
static void effItemGetMain(EffectEntry* effect) {
    ItemGetWork* work = effect->userdata;
    Vec tmp;
    Vec position = { 0.0f, 0.0f, 0.0f };
    s32 type;
    s32 camera;
    s32 done;
    s32 i;

    done = 0;
    position.x = work->position.x;
    position.y = work->position.y;
    position.z = work->position.z;
    tmp = position;
    camera = work->camera;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    if (work->lifetime < 16) {
        work->alpha = work->lifetime << 4;
    }
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    work->timer++;
    if ((u32)work->timer < 12) {
        work->scale = (f32)scale_data[work->timer] / 100.0f;
    }
    if (work->timer > 1000) {
        work->timer = 1000;
    }
    switch (work->state) {
        case 0:
            break;
        case 1:
            if (work->scale < 100.0f) {
                work->scale *= 1.1f;
            }
            break;
        case 2:
            work->scale *= 0.9f;
            break;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        work->timer++;
        if (work->timer < 16) {
            work->alpha += 16;
            if (work->alpha > 255) {
                work->alpha = 255;
            }
        } else {
            work->alpha -= 8;
            if (work->alpha < 0) {
                work->alpha = 0;
            }
        }
        if (work->timer > 1000) {
            work->timer = 1000;
        }
        if (work->alpha == 0) {
            done++;
        } else {
            work->position.x += work->velocity.x;
            work->position.y += work->velocity.y;
            work->position.z += work->velocity.z;
            if (type == 10) {
                work->velocity.x *= 0.9f;
                work->velocity.y *= 0.9f;
                work->velocity.z *= 0.9f;
            }
        }
    }
    if (type == 10) {
        if (done >= effect->count - 1) {
            effDelete(effect);
            return;
        }
        dispEntry(camera, dispCalcZ(&tmp), DISP_MODE_XLU, effItemUseDisp, effect);
    } else {
        dispEntry(camera, dispCalcZ(&tmp), DISP_MODE_XLU, effItemGetDisp, effect);
    }
}

/** Draws the sparkle sprites for the item get types in two passes (shadow, then main). */
static void effItemGetDisp(CameraId cameraId, void* payload) {
    EffectEntry* effect = payload;
    ItemGetWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    Mtx m19c;
    Mtx m16c;
    Mtx m13c;
    Mtx m10c;
    Mtx mdc;
    Mtx mac;
    Mtx m7c;
    GXTexObj texObj;
    Vec eye = { 0.0f, 0.0f, 1000.0f };
    Vec up = { 0.0f, 1.0f, 0.0f };
    Vec target = { 0.0f, 0.0f, 0.0f };
    f32 scale;
    f32 d;
    f32 h;
    f32 s;
    f32 w;
    f32 cs;
    s32 alpha;
    s32 i;
    s32 al;
    s32 sh;
    s32 pass;

    scale = work->scale * work->size;
    alpha = work->alpha;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(m19c, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(m13c, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(m16c, scale, scale, scale);
    PSMTXConcat(m19c, m13c, m19c);
    PSMTXConcat(m19c, m16c, m7c);

    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    cs = (0, (f32)cos(6.2831855f * camGetPtr(cameraId)->viewYaw / 360.0f));
    s = (f32)sin(6.2831855f * camGetPtr(cameraId)->viewYaw / 360.0f);
    PSMTXTrans(m19c, 2.0f * s, 0.0f, -cs * 2.0f);
    PSMTXConcat(m19c, m7c, m7c);
    GXSetTevColor(GX_TEVREG0, white);
    GXSetTevColor(GX_TEVREG1, color_tbl[work->type]);
    C_MTXLightOrtho(mdc, 0.0f, 6.0f, 0.0f, 6.0f, 0.5f, 0.5f, 0.5f, 0.5f);
    PSMTXMultVec(m13c, &eye, &eye);
    C_MTXLookAt(mac, &eye, &up, &target);
    PSMTXConcat(mdc, mac, mdc);
    PSMTXConcat(mdc, m7c, m10c);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(m10c, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    effGetTexObj(0x2C, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(0x2A, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    w = 64.0f;
    sh = alpha * 0x80 / 0xFF;
    al = alpha * 0xFF / 0xFF;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            PSMTXConcat(camera->view, m7c, m19c);
            PSMTXTrans(m13c, 2.0f, -2.0f, 0.0f);
            PSMTXConcat(m19c, m13c, m19c);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, sh });
        } else {
            PSMTXConcat(camera->view, m7c, m19c);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, al });
        }
        GXLoadPosMtxImm(m19c, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2.0f, w / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w / 2.0f, w / 2.0f, 0.0f);
        GXTexCoord2f32(2.0f, 0.0f);
        GXPosition3f32(w / 2.0f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(2.0f, 2.0f);
        GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 2.0f);
    }
    GXSetTevColor(GX_TEVREG0, color_tbl[work->type]);
    GXSetTevColor(GX_TEVREG1, white);
    effGetTexObj(0x2B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    d = 10.666667f;
    h = 42.666668f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        for (pass = 0; pass < 2; pass++) {
            if (pass == 0) {
                al = work->alpha * 0x80 / 0xFF;
                if (work->alpha != 0xFF) {
                    al -= 0x14;
                    if (al < 0) {
                        al = 0;
                    }
                }
                PSMTXTrans(m19c, work->position.x, work->position.y, work->position.z);
                PSMTXConcat(m7c, m19c, m10c);
                PSMTXTrans(m19c, 2.0f, -2.0f, 0.0f);
                PSMTXConcat(m10c, m19c, m10c);
                PSMTXRotRad(m13c, 'z', work->angle);
                PSMTXConcat(m10c, m13c, m10c);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, al });
            } else {
                PSMTXTrans(m19c, work->position.x, work->position.y, work->position.z);
                PSMTXConcat(m7c, m19c, m10c);
                PSMTXRotRad(m13c, 'z', work->angle);
                PSMTXConcat(m10c, m13c, m10c);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha * 0xFF / 0xFF });
            }
            PSMTXConcat(camera->view, m10c, m19c);
            GXLoadPosMtxImm(m19c, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            PSMTXConcat(mdc, m10c, m19c);
            GXLoadTexMtxImm(m19c, GX_TEXMTX0, GX_MTX3x4);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-d / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(d / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(2.0f, 0.0f);
            GXPosition3f32(d / 2.0f, -h, 0.0f);
            GXTexCoord2f32(2.0f, 1.0f);
            GXPosition3f32(-d / 2.0f, -h, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

/** Draws the sparkles for type 10 (item use) as camera-facing quads. */
static void effItemUseDisp(CameraId cameraId, void* payload) {
    EffectEntry* effect = payload;
    ItemGetWork* work = effect->userdata;
    CameraEntry* camera;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx base;
    GXTexObj texObj;
    f32 h, w;
    s32 i;
    s32 pass;

    camera = camGetPtr(cameraId);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, 0.4f, 0.4f, 0.4f);
    PSMTXConcat(mtx, scale, base);
    PSMTXConcat(base, rot, base);
    PSMTXConcat(camera->view, base, base);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    effGetTexObj(0x2B, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    w = 21.333334f;
    h = 42.666668f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        for (pass = 0; pass < 2; pass++) {
            if (pass == 0) {
                PSMTXTrans(mtx, 2.0f + work->position.x, work->position.y - 2.0f, work->position.z);
                PSMTXRotRad(rot, 'z', work->angle);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXConcat(base, mtx, mtx);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, work->alpha * 0x40 / 0xFF });
            } else {
                PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
                PSMTXRotRad(rot, 'z', work->angle);
                PSMTXConcat(mtx, rot, mtx);
                PSMTXConcat(base, mtx, mtx);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha * 0x80 / 0xFF });
            }
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-w / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w / 2.0f, 0.0f, 0.0f);
            GXTexCoord2f32(2.0f, 0.0f);
            GXPosition3f32(w / 2.0f, -h, 0.0f);
            GXTexCoord2f32(2.0f, 1.0f);
            GXPosition3f32(-w / 2.0f, -h, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}
