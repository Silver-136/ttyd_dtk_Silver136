#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "driver/dispdrv.h"
#include "evt/evt_cmd.h"
#include "dolphin/gx.h"
#include "memory.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "manager/filemgr.h"
#include "string.h"
#include "win/win_main.h"
#include "pmario_sound.h"
static inline f32 dd_sin(f32 x) {
    f32 a = 0.00761f;
    f32 b = 0.16605f;
    f32 c = 1.0f;
    f32 y;
    f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x; a = a * x2 - b; a = a * x2 + c; a = a * x;
        } else {
            y = 1.5707964f - (x - 1.5707964f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f; x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y; a = -a;
    } else {
        y = 1.5707964f - (x - 4.712389f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y; a = -a;
    }
    return a;
}

static inline f32 dd_cos(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 y;
    f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x; a = a * x2 - b; a = a * x2 + c;
        } else {
            y = 1.5707964f - (x - 1.5707964f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = -a;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f; x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = -a;
    } else {
        y = 1.5707964f - (x - 4.712389f); x2 = y * y; a = a * x2 - b; a = a * x2 + c;
    }
    return a;
}

double sin(double x);
void mabutaDisp(CameraId camera, void* param);
static void depthDisp(CameraId camera, void* param);
typedef struct TegamiWork { s32 alpha; FileEntry* tpl; } TegamiWork;
static f32 view_dy;
static s32 view_trg;
static TegamiWork* wp;
static void draw(CameraId camera, void* param);

USER_FUNC(cam_yure);
static USER_FUNC(depth);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam_ctrl_onoff);
USER_FUNC(evt_cam_letter_box_camid);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_party_dispflg_onoff);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxchk);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(mabuta);
USER_FUNC(tegami);
void mabutaDisp(CameraId camera, void* param);
static void depthDisp(CameraId camera, void* param);
void tegami_delete(void);
static void draw(CameraId camera, void* param);

static void draw(CameraId camera, void* param) {
    winTexInit(*(TEXPalette**)wp->tpl->data);
    winTexSet(4, (Vec){0.0f, -74.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, wp->alpha});
}

USER_FUNC(tegami) {
    if (isFirstCall) {
        wp = _mapAlloc(sizeof(TegamiWork));
        memset(wp, 0, sizeof(TegamiWork));
        wp->alpha = 0;
        wp->tpl = fileAllocf(4, "%s/mariost.tpl", getMarioStDvdRoot());
    }
    wp->alpha += 2;
    if (wp->alpha > 128) {
        wp->alpha = 128;
    }
    dispEntry(CAMERA_2D, 0.0f, 0, draw, wp);
    return EVT_RETURN_BLOCK;
}

void tegami_delete(void) {
    if (wp != NULL && wp->tpl != NULL) {
        fileFree(wp->tpl);
    }
}

#pragma opt_propagation off
#pragma opt_lifetimes off
static void depthDisp(CameraId camera, void* param) {
    f32 amp = evtGetFloat(NULL, GW(0)) / 100.0f;
    CameraEntry* cam = camGetPtr(camera);
    smartEntry* buf = smartAlloc(GXGetTexBufferSize(608, 480, GX_TF_RGB565, GX_FALSE, 0), 3);
    GXTexObj obj;
    Mtx mtx;
    f32 a;
    f32 s;
    f32 c;
    s32 i;
    BOOL first;
    GXTevColorArg carg;

    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(608, 480, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(buf->address, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&obj, buf->address, 608, 480, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(8);
    GXSetNumTevStages(8);
    for (i = 0; i < 8; i++) {
        a = 6.2831855f * i / 8.0f;
        s = dd_sin(a);
        c = dd_cos(a);
        MTXTrans(mtx, (amp * s) / 100.0f, (amp * c) / 100.0f, 0.0f);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0 + i * 3, GX_MTX2x4);
        GXSetTexCoordGen2((GXTexCoordID)i, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0 + i * 3, GX_FALSE, GX_PTIDENTITY);
        GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_8);
        GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1_8);
        GXSetTevOrder((GXTevStageID)i, (GXTexCoordID)i, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        first = !i;
        carg = first ? GX_CC_ZERO : GX_CC_CPREV;
        GXSetTevColorIn((GXTevStageID)i, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, carg);
        GXSetTevAlphaIn((GXTevStageID)i, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, first ? GX_CA_ZERO : GX_CA_APREV);
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-304, 240, 0);
    GXTexCoord2s16(0, 0);
    GXPosition3s16(304, 240, 0);
    GXTexCoord2s16(1, 0);
    GXPosition3s16(304, -240, 0);
    GXTexCoord2s16(1, 1);
    GXPosition3s16(-304, -240, 0);
    GXTexCoord2s16(0, 1);
    GXEnd();
    sysWaitDrawSync();
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

static USER_FUNC(depth) {
    dispEntry(CAMERA_2D, 1200.0f, 0, depthDisp, NULL);
    return EVT_RETURN_BLOCK;
}

EvtWord tegami_init_evt[78];
EvtWord evt_mezame[226];
EvtWord mezame_init_evt[61];
EvtWord gor_10_init_evt[29];

EVT_BEGIN(tegami_init_evt)
    CALL(evt_snd_envoff, 1024)
    CALL(evt_snd_envon, 257, STRING("ENV_STG0_GOR6"))
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    BEGIN_INLINE_EVENT
        CALL(tegami)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, 2)
        CALL(evt_fade_end_wait)
        CALL(evt_cam_letter_box_camid, 8)
        WAIT_MS(200)
        CALL(evt_msg_print, 1, STRING("<diary><nowinse><once_stop>"), 0, 0)
        CALL(evt_msg_print_add, 0, STRING("pro_04"))
        CALL(evt_cam_letter_box_camid, 10)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("aaa_00"), STRING("prologue2"))
    END_INLINE_EVENT
    RETURN
EVT_END()

void mabutaDisp(CameraId camera, void* param) {
    f32 h = evtGetFloat(NULL, GW(1));
    CameraEntry* cam = camGetPtr(camera);

    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0x00, 0x00, 0x00, 0xFF});
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 8);
    GXPosition3s16(-304, 240, 0);
    GXPosition3s16(304, 240, 0);
    GXPosition3s16(304, (s16)(240.0f - h), 0);
    GXPosition3s16(-304, (s16)(240.0f - h), 0);
    GXPosition3s16(-304, (s16)(-240.0f + h), 0);
    GXPosition3s16(304, (s16)(-240.0f + h), 0);
    GXPosition3s16(304, -240, 0);
    GXPosition3s16(-304, -240, 0);
    GXEnd();
}

USER_FUNC(mabuta) {
    dispEntry(CAMERA_2D, 1100.0f, 0, mabutaDisp, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(cam_yure) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 s;

    if (isFirstCall) {
        view_dy = cam->viewOffset.y;
        view_trg = 0;
    }
    s = sin(gp->retraceCount / 50.0f);
    cam->viewOffset.y += 0.3f * s;
    if (!view_trg) {
        if (cam->viewOffset.y > view_dy) {
            view_trg = 1;
        }
    } else if (cam->viewOffset.y < view_dy) {
        psndSFXOn("SFX_EVT_OPN_SHIP_CREAK1");
        view_trg = 0;
    }
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(evt_mezame)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    CALL(evt_fade_end_wait)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        WAIT_MS(2000)
        LOOP(0)
            CALL(evt_snd_sfxon, STRING("SFX_VOICE_MARIO_SLEEP1"), GW(2))
            WAIT_MS(3000)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("mac_0_012"), 0, 0)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("mac_0_013"), 0, 0)
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_GOR0"))
    DELETE_EVENT(LW(10))
    WAIT_MS(300)
    CALL(evt_snd_sfxchk, GW(2), LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_snd_sfxoff, GW(2))
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_VOICE_MARIO_FIND1_2"), 0)
    CALL(evt_sub_intpl_msec_init, 11, 2400, 1200, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 1, 1200, 2400, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 1, 2400, 1200, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 1, 1200, 2400, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 1, 500, 0, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 1, 2400, 0, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("mac_0_013_01"), 0, 0)
    WAIT_MS(300)
    WAIT_MS(2000)
    CALL(evt_bero_mapchange, STRING("gor_00"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mezame_init_evt)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("marukongou"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("densha"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("pukupuku"), 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(50.0f))
    BEGIN_INLINE_EVENT
        CALL(depth)
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(1), FLOAT(240.0f))
    BEGIN_INLINE_EVENT
        CALL(mabuta)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_yure)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(evt_mezame))
    RETURN
EVT_END()

EVT_BEGIN(gor_10_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), 1, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 2)
        CALL_CHILD(PTR(tegami_init_evt))
        ELSE
        CALL_CHILD(PTR(mezame_init_evt))
    END_IF
    RETURN
EVT_END()
