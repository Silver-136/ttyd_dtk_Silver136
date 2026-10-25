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


void effItemGetMain(EffectEntry* effect);
void effItemGetDisp(CameraId cameraId, EffectEntry* effect);
void effItemUseDisp(CameraId cameraId, EffectEntry* effect);

static u8 scale_data[12] = { 0x00, 0x12, 0x26, 0x3B, 0x4E, 0x60, 0x6E, 0x78, 0x79, 0x72, 0x69, 0x64 };
GXColor white = { 0xE1, 0xE1, 0xE1, 0xFF };
static GXColor color_tbl[10] = {
    { 0xC8, 0xB2, 0x25, 0xFF }, { 0x26, 0xA3, 0x52, 0xFF }, { 0x5C, 0xCA, 0x10, 0xFF }, { 0xCB, 0x4C, 0x4D, 0xFF },
    { 0x7E, 0xC2, 0x1E, 0xFF }, { 0x30, 0x9C, 0xBF, 0xFF }, { 0xEB, 0x7D, 0x06, 0xFF }, { 0xE8, 0x5B, 0xD0, 0xFF },
    { 0x51, 0x75, 0x9D, 0xFF }, { 0xC8, 0xB2, 0x25, 0xFF },
};
u8 size_tbl[10] = { 0x32, 0x32, 0x32, 0x28, 0x0C, 0x0C, 0x0C, 0x0A, 0x0A, 0x0F};

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
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 0.0f;
    work->size = 0.75f * ((f32)size_tbl[work->type] / 10.0f);
    work->timer = 0;
    work->state = 0;
    work->camera = 4;
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

void effItemGetMain(EffectEntry* effect) {
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
    if (effect->flags & 4) {
        effect->flags &= ~4;
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
        dispEntry(camera, dispCalcZ(&tmp), 2, (DispCallback)effItemUseDisp, effect);
    } else {
        dispEntry(camera, dispCalcZ(&tmp), 2, (DispCallback)effItemGetDisp, effect);
    }
}

void effItemGetDisp(CameraId cameraId, EffectEntry* effect) {
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
    cs = (0, (f32)cos(6.2831855f * camGetPtr(cameraId)->viewYaw / 360.0f));
    s = (0, (f32)sin(6.2831855f * camGetPtr(cameraId)->viewYaw / 360.0f));
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

void effItemUseDisp(CameraId cameraId, EffectEntry* effect) {
    ItemGetWork* work = effect->userdata;
    CameraEntry* camera;
    Mtx mc8;
    Mtx m98;
    Mtx m68;
    Mtx m38;
    GXTexObj texObj;
    f32 d, a, b, h, w;
    s32 i;
    s32 pass;

    camera = camGetPtr(cameraId);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(mc8, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(m68, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(m98, 0.4f, 0.4f, 0.4f);
    PSMTXConcat(mc8, m98, m38);
    PSMTXConcat(m38, m68, m38);
    PSMTXConcat(camera->view, m38, m38);
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
                PSMTXTrans(mc8, 2.0f + work->position.x, work->position.y - 2.0f, work->position.z);
                PSMTXRotRad(m68, 'z', work->angle);
                PSMTXConcat(mc8, m68, mc8);
                PSMTXConcat(m38, mc8, mc8);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, work->alpha * 0x40 / 0xFF });
            } else {
                PSMTXTrans(mc8, work->position.x, work->position.y, work->position.z);
                PSMTXRotRad(m68, 'z', work->angle);
                PSMTXConcat(mc8, m68, mc8);
                PSMTXConcat(m38, mc8, mc8);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha * 0x80 / 0xFF });
            }
            GXLoadPosMtxImm(mc8, GX_PNMTX0);
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
