#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "driver/swdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "mario/mariost.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <stdlib.h>
#include <string.h>

typedef struct TelopWork {
    s32 id;
    s32 alpha;
    OSTime time;
} TelopWork;

#ifdef VERSION_G8ME01
#define TELOP_TEX_A 0x65
#define TELOP_TEX_B 0x64
#else
#define TELOP_TEX_A 0x81
#define TELOP_TEX_B 0x80
#endif
static const char* msg_tbl[] = {
    "menu_monosiri_gor", "menu_monosiri_hei", "menu_monosiri_nok", "menu_monosiri_gon",
    "menu_monosiri_win", "menu_monosiri_mri", "menu_monosiri_tou", "menu_monosiri_usu",
    "menu_monosiri_jin", "menu_monosiri_muj", "menu_monosiri_dou", "menu_monosiri_hom",
    "menu_monosiri_pik", "menu_monosiri_bom", "menu_monosiri_moo", NULL,
    "menu_monosiri_aji", "menu_monosiri_las",
};

static TelopWork work;
static TelopWork* wp = &work;

void telop(CameraId cameraId, void* param);

USER_FUNC(evt_telop_entry) {
    EvtWord* args = event->args;
    u32 ms;

    if (isFirstCall) {
        event->userdata[0] = 0;
        memset(wp, 0, sizeof(TelopWork));
        wp->id = evtGetValue(event, args[0]);
        if (swGet(wp->id + 0x28)) {
            return EVT_RETURN_DONE;
        }
        swSet(wp->id + 0x28);
    }
    switch (event->userdata[0]) {
    case 0:
        wp->time = event->lifetime;
        event->userdata[0]++;
        break;
    case 1:
        ms = OSTicksToMilliseconds((u32)event->lifetime - (u32)wp->time);
        if (ms < 1000) {
            wp->alpha = intplGetValue(11, -256.0f, 256.0f, ms, 1000);
        } else {
            wp->time = event->lifetime;
            event->userdata[0]++;
        }
        dispEntry(CAMERA_2D, 300.0f, 0, telop, NULL);
        break;
    case 2:
        ms = OSTicksToMilliseconds((u32)event->lifetime - (u32)wp->time);
        if (ms > 5000) {
            wp->time = event->lifetime;
            event->userdata[0]++;
        }
        dispEntry(CAMERA_2D, 300.0f, 0, telop, NULL);
        break;
    case 3:
        ms = OSTicksToMilliseconds((u32)event->lifetime - (u32)wp->time);
        if (ms < 1000) {
            wp->alpha = intplGetValue(11, 256.0f, -256.0f, ms, 1000);
        } else {
            event->userdata[0]++;
        }
        dispEntry(CAMERA_2D, 300.0f, 0, telop, NULL);
        break;
    case 4:
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

void telop(CameraId cameraId, void* param) {
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx mtx2;
    Mtx scaleMtx2;
    CameraEntry* camera;
    f32 texW, texH;
    f32 y, x, scale;
    s32 alpha, a;
    const char* msg;
    s32 k, i;
    u16 width;

    camera = camGetPtr(cameraId);
    effGetTexObj(TELOP_TEX_A, &texObj);
    texW = GXGetTexObjWidth(&texObj);
    texH = GXGetTexObjHeight(&texObj);

    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);

    y = (f32)(rand() % 64) / texH;
    x = (f32)(rand() % 256) / texW;
    PSMTXTrans(mtx, x, y, 0.0f);
    PSMTXScale(scaleMtx, 0.2f, 0.2f, 1.0f);
    PSMTXConcat(mtx, scaleMtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);

    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C1, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_A1, GX_CA_ZERO);

    GXSetTevColorS10(GX_TEVREG0, (GXColorS10){0, 0, 0, wp->alpha});

    effGetTexObj(TELOP_TEX_A, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(TELOP_TEX_B, &texObj);
    GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    i = 1;
    k = 5;
    do {
        if (i != 0) {
            GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, 0x60});
        } else {
            GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
        PSMTXTrans(mtx, -15.0f + (f32)k, -105.0f - (f32)k, 0.0f);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(texW, 0.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(texW, -texH, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, -texH, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        k -= 5;
    } while (--i >= 0);

#ifdef VERSION_G8ME01
    {
        const char* def = (u32)gp->language != 0 ? "???" : "\x81H\x81H\x81H";
        msg = (msg_tbl[wp->id] != NULL) ? msgSearch(msg_tbl[wp->id]) : def;
    }
#else
    msg = (msg_tbl[wp->id] != NULL) ? msgSearch(msg_tbl[wp->id]) : "\x81H\x81H\x81H";
#endif
    width = FontGetMessageWidth(msg);
    scale = 1.0f;
    if ((u32)gp->language != 0U) {
        if ((f32)width / (texW - 100.0f) < scale) {
            // FAKE: no-op self-assignment (the branch keeps scale); without it the comparison is laid out differently
            scale = scale;
        } else {
            scale = (texW - 100.0f) / (f32)width;
        }
    }
    a = wp->alpha;
    alpha = (a < 0) ? 0 : ((a > 255) ? 255 : a);
    FontDrawStart();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, alpha});
    PSMTXTrans(mtx2, (texW - scale * (f32)FontGetMessageWidth(msg)) / 2.0f + -15.0f, -105.0f - (texH - 24.0f) / 2.0f, 0.0f);
    PSMTXScale(scaleMtx2, scale, 1.0f, 1.0f);
    PSMTXConcat(mtx2, scaleMtx2, mtx2);
    FontDrawStringMtx(mtx2, msg);
}
