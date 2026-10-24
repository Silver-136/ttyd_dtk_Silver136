#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/imgdrv.h"
#include "manager/filemgr.h"
#include "manager/fontmgr.h"
#include "driver/msgdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include "sdk/texPalette.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
#include "string.h"
#include "stdio.h"
#include "evt/evt_cam.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "statuswindow.h"

typedef struct DemoWork { s32 field_0x0; f32 bookRot; s32 page; s32 captured; f32 fade; f32 scale; f32 x; f32 y; void** capBuf; FileEntry* tpl; s32 winOn; s32 winAlpha; } DemoWork;
static DemoWork work;
char buf[0x200];
static DemoWork* wp;
double sin(double x);
void demo_img(CameraId cameraId, void* param);
void demo_book(CameraId cameraId, void* param);
void demo_desk(CameraId cameraId, void* param);
void demo_window(CameraId cameraId, void* param);

USER_FUNC(N_evt_img_set_z);
USER_FUNC(adjust);
USER_FUNC(book_r);
USER_FUNC(demo_scale);
USER_FUNC(demo_winonoff);
USER_FUNC(eff_cam);
USER_FUNC(eff_cam1);
USER_FUNC(evt_bg_disp_onoff);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_free_capture);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_reverse_ry);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_npc_set_camid);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_seq_set_seq);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_sfxon_);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(make_msg);
static USER_FUNC(next);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
USER_FUNC(demo_draw);
void demo_window(CameraId cameraId, void* param);
void demo_book(CameraId cameraId, void* param);
void demo_img(CameraId cameraId, void* param);
void demo_desk(CameraId cameraId, void* param);
void demo_cap(void);
void demo_exit(void);
USER_FUNC(demo_init);

f32 fd_dmo_00_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dmo_00_neg_one_382;
f32 fd_dmo_00___local_PI_383;
f32 fd_dmo_00___local_3PIO2_385;
f32 fd_dmo_00_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dmo_00___local_PIO2_384;
f32 fd_dmo_00_one_381;
f32 fd_dmo_00_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dmo_00_neg_one_365;
f32 fd_dmo_00___local_PI_366;
f32 fd_dmo_00___local_3PIO2_368;
f32 fd_dmo_00_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dmo_00___local_PIO2_367;
f32 fd_dmo_00_one_364;
f64 fd_dmo_00__three_localstatic4_sqrtf;
f64 fd_dmo_00__half_localstatic3_sqrtf;
EvtWord buttonCheck[32];
EvtWord demo_evt[1489];
static EvtWord koushukei_dai[63];
EvtWord roten_move[25];
EvtWord kuribo_move[25];
EvtWord chusan_move[25];
#ifdef VERSION_G8ME01
EvtWord dmo_00_init_evt[268];
#else
EvtWord dmo_00_init_evt[262];
#endif

USER_FUNC(demo_init) {
    if (isFirstCall) {
        memset(wp, 0, sizeof(DemoWork));
        wp->bookRot = 0.0f;
        wp->page = -1;
        wp->captured = 0;
        wp->fade = 0.0f;
        wp->scale = 1.0f;
        wp->x = 0.0f;
        wp->y = 0.0f;
        statusWinDispOff();
        psndSetFlag(4);
    }
    if (fileAsyncf(4, NULL, "%s/demo.tpl", getMarioStDvdRoot()) != 0U) {
        wp->tpl = fileAllocf(4, "%s/demo.tpl", getMarioStDvdRoot());
        gp->areaFadeOutType = 9;
        gp->areaFadeOutDuration = 500;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

void demo_exit(void) {
    CameraEntry* cam = camGetPtr(8);

    if (wp->tpl != NULL) {
        fileFree(wp->tpl);
    }
    psndClearFlag(4);
    cam = camGetPtr(4);
    cam->viewport[0] = 0.0f;
    cam->viewport[1] = 0.0f;
    cam->viewport[2] = 608.0f;
    cam->viewport[3] = 480.0f;
    statusWinDispOn();
}

void demo_cap(void) {
    if (wp->page == 4) {
        if (wp->captured == 0) {
            GXSetTexCopySrc(0, 0, 608, 480);
            GXSetTexCopyDst(608, 480, GX_TF_RGBA8, GX_FALSE);
            GXCopyTex(*wp->capBuf, GX_FALSE);
            GXTexModeSync();
            wp->captured = 1;
        } else if (wp->fade != 0.0f) {
            GXSetTexCopySrc(0, 0, 608, 480);
            GXSetTexCopyDst(608, 480, GX_TF_RGBA8, GX_FALSE);
            GXCopyTex(*wp->capBuf, GX_FALSE);
            GXTexModeSync();
        }
    }
}

void demo_desk(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(8);
    GXTexObj obj;
    Mtx mtx;
    Mtx scale;
    f32 w;
    f32 h;

    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * (1.0f - wp->fade)});
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 0);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    PSMTXTrans(mtx, wp->x, wp->y, 0.0f);
    PSMTXScale(scale, wp->scale, wp->scale, wp->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    w = 608.0f;
    h = 480.0f;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-w / 2, h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w / 2, h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w / 2, -h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-w / 2, -h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
}

void demo_img(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(8);
    CameraEntry* cam2;
    Mtx mtx;
    Mtx scale;
    GXTexObj obj;
    f32 proj[7];
    f32 vp[6];
    Vec p0;
    Vec p1;
    Vec p2;
    Vec p3;
    f32 w;
    f32 h;

    if (wp->page != -1) {
        GXSetProjection(cam->projection, cam->projType);
        GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * (1.0f - wp->fade)});
        TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, wp->page + 8);
        GXLoadTexObj(&obj, GX_TEXMAP0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        PSMTXTrans(mtx, wp->x, wp->y, 0.0f);
        PSMTXScale(scale, wp->scale, wp->scale, wp->scale);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(cam->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetCullMode(GX_CULL_BACK);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXGetViewportv(vp);
        GXGetProjectionv(proj);
        w = 202.66667f;
        h = 160.0f;
        GXProject(-w / 2, h / 2 + 35.0f, 0.0f, mtx, proj, vp, &p0.x, &p0.y, &p0.z);
        GXProject(w / 2, h / 2 + 35.0f, 0.0f, mtx, proj, vp, &p1.x, &p1.y, &p1.z);
        GXProject(w / 2, -h / 2 + 35.0f, 0.0f, mtx, proj, vp, &p2.x, &p2.y, &p2.z);
        GXProject(-w / 2, -h / 2 + 35.0f, 0.0f, mtx, proj, vp, &p3.x, &p3.y, &p3.z);
        cam2 = camGetPtr(4);
        cam2->viewport[0] = p0.x;
        cam2->viewport[1] = p0.y;
        cam2->viewport[2] = p2.x - p0.x;
        cam2->viewport[3] = p2.y - p0.y;
        cam2 = camGetPtr(6);
        cam2->viewport[0] = 0.0f;
        cam2->viewport[1] = 0.0f;
        cam2->viewport[2] = 608.0f;
        cam2->viewport[3] = 480.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2, h / 2 + 35.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w / 2, h / 2 + 35.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w / 2, -h / 2 + 35.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2, -h / 2 + 35.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

void demo_book(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(8);
    GXTexObj obj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    f32 w;
    f32 h;
    f32 pw;
    f32 ph;

    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 2);
    w = GXGetTexObjWidth(&obj);
    PSMTXTrans(mtx, wp->x, wp->y, 0.0f);
    PSMTXScale(scale, wp->scale, wp->scale, wp->scale);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(cam->view, mtx, scale);
    PSMTXTrans(mtx, -w / 2 + 8.0f, 0.0f, 0.0f);
    PSMTXConcat(scale, mtx, mtx);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * (1.0f - wp->fade)});
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 2);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    w = GXGetTexObjWidth(&obj);
    h = GXGetTexObjHeight(&obj);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w, h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w, -h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.0f, -h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXEnd();
    w = GXGetTexObjWidth(&obj);
    h = GXGetTexObjHeight(&obj);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    if (wp->page >= 0) {
        TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, wp->page + 3);
        GXLoadTexObj(&obj, GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        ph = 0.95f * h;
        pw = 0.95f * w;
        GXPosition3f32(0.0f, ph / 2, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(pw, ph / 2, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(pw, -ph / 2, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, -ph / 2, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
        demo_img(cameraId, param);
    }
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    PSMTXRotRad(rot, 'y', 0.017453292f * -wp->bookRot);
    PSMTXConcat(mtx, rot, rot);
    GXLoadPosMtxImm(rot, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w, h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w, -h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.0f, -h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    GXSetCullMode(GX_CULL_FRONT);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 2);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w, h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w, -h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.0f, -h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXEnd();
}

void demo_window(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(cameraId);
    GXTexObj obj;
    Mtx mtx;
    f32 w;
    f32 h;
    f32 hw;
    f32 hh;

    if (wp->winOn) {
        wp->winAlpha += 10;
    } else {
        wp->winAlpha -= 10;
    }
    if (wp->winAlpha > 204) {
        wp->winAlpha = 204;
    }
    if (wp->winAlpha < 0) {
        wp->winAlpha = 0;
    }
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &obj, 13);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    w = GXGetTexObjWidth(&obj);
    h = GXGetTexObjHeight(&obj);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, wp->winAlpha * 255 / 255});
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(mtx, 0.0f, -130.0f, 0.0f);
    PSMTXConcat(cam->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    hw = w / 2;
    hh = h / 2;
    GXPosition3f32(-hw, hh, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(hw, hh, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(hw, -hh, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(-hw, -hh, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXEnd();
}

USER_FUNC(demo_draw) {
    dispEntry(4, 9.0f, 7, demo_desk, NULL);
    dispEntry(4, 8.0f, 7, demo_book, NULL);
    if (wp->fade != 0.0f) {
        dispEntry(7, 0.0f, 7, demo_img, NULL);
    }
    dispEntry(8, 0.0f, 3, demo_window, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(demo_winonoff) {
    EvtWord* args = event->args;

    wp->winOn = evtGetValue(event, args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(demo_scale) {
    OSTime now = event->lifetime;
    f32 t;

    if (isFirstCall) {
        event->savedTime = now;
    }
    t = (s32)OSTicksToMilliseconds((u64)(s32)((u32)now - (u32)event->savedTime)) / 12000.0f;
    wp->scale = 600.0f * (1.0f - t) + 1500.0f * t;
    wp->scale /= 1000.0f;
    if (wp->scale >= 1.5f) {
        wp->scale = 1.5f;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(book_r) {
    EvtWord* args = event->args;

    wp->bookRot = evtGetFloat(event, args[0]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(next) {
    wp->page++;
    return EVT_RETURN_DONE;
}

USER_FUNC(adjust) {
    EvtWord* args = event->args;
    Mtx trans;
    Mtx scale;
    ImageEntry* img;
    f32 v;
    f32 s;

    v = evtGetFloat(event, args[0]);
    img = imgNameToPtr("img4", gp->inBattle != 0);
    wp->fade = v;
    s = (f32)sin(1.5707964f * wp->fade);
    wp->scale = 1.5f * s + 1.5f;
    wp->y = -105.0f * (f32)sin(1.5707964f * wp->fade);
    PSMTXTrans(trans, wp->x / 10.0f, wp->y / 10.0f, 0.0f);
    PSMTXScale(scale, wp->scale / 1.5f, wp->scale / 1.5f, wp->scale / 1.5f);
    PSMTXConcat(trans, scale, img->mtx);
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_cam) {
    ((s32*)effNameToPtr("eff0")->userdata)[0x38 / 4] = 5;
    ((s32*)effNameToPtr("eff1")->userdata)[0x38 / 4] = 5;
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_cam1) {
    ((s32*)effNameToPtr("eff2")->userdata)[0x98 / 4] = 5;
    return EVT_RETURN_DONE;
}

USER_FUNC(make_msg) {
    EvtWord* args = event->args;
    u16 lines;
    u16 width;

    width = FontGetMessageWidthLine(msgSearch((const char*)evtGetValue(event, args[0])), &lines);
    sprintf(buf, "<clear><wpos %d %d %d %d><once_stop>", -width / 2 - 62, lines * 28 / 2 - 84, 400, 200);
    return EVT_RETURN_DONE;
}

f32 fd_dmo_00_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dmo_00_neg_one_382 = -1.0f;

f32 fd_dmo_00___local_PI_383 = 3.1415927f;

f32 fd_dmo_00___local_3PIO2_385 = 4.712389f;

f32 fd_dmo_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dmo_00___local_PIO2_384 = 1.5707964f;

f32 fd_dmo_00_one_381 = 1.0f;

f32 fd_dmo_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dmo_00_neg_one_365 = -1.0f;

f32 fd_dmo_00___local_PI_366 = 3.1415927f;

f32 fd_dmo_00___local_3PIO2_368 = 4.712389f;

f32 fd_dmo_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dmo_00___local_PIO2_367 = 1.5707964f;

f32 fd_dmo_00_one_364 = 1.0f;

f64 fd_dmo_00__three_localstatic4_sqrtf = 3.0;

f64 fd_dmo_00__half_localstatic3_sqrtf = 0.5;

static DemoWork* wp = &work;

EVT_BEGIN(buttonCheck)
    CALL(evt_fade_end_wait)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), 4352)
            SET_VALUE(GF(10), 1)
            CALL(evt_fade_set_mapchange_type, 1, 17, 500, -1, -1)
            CALL(evt_seq_set_seq, 3, STRING("title"), 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(demo_evt)
    CALL(evt_seq_wait, 2)
    SET_VALUE(GF(10), 0)
    SPAWN_EVENT_GET_ID(PTR(buttonCheck), GW(10))
    CALL(evt_snd_bgmon, 513, STRING("BGM_DEMO1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        WAIT_MS(4000)
        CALL(demo_winonoff, 1)
        CALL(make_msg, STRING("op_book_01"))
        CALL(evt_msg_print, 1, PTR(buf), 0, 0)
        CALL(evt_msg_print_add, 0, STRING("op_book_01"))
        CALL(make_msg, STRING("op_book_02"))
        CALL(evt_msg_print, 1, PTR(buf), 0, 0)
        CALL(evt_msg_print_add, 0, STRING("op_book_02"))
        CALL(demo_winonoff, 0)
    END_INLINE_EVENT
    CALL(demo_scale, LW(0))
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_03"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_03"))
    CALL(make_msg, STRING("op_book_04"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_04"))
    CALL(demo_winonoff, 0)
    LOOP(0)
        IS_EVENT_RUNNING(LW(9), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(next)
    CALL(evt_snd_sfxon_, STRING("SFX_EVT_OPN_BOOK_OPEN1"), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 1800, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(book_r, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_05"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_05"))
    CALL(make_msg, STRING("op_book_06"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_06"))
    CALL(make_msg, STRING("op_book_07"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_07"))
    CALL(make_msg, STRING("op_book_08"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_08"))
    CALL(demo_winonoff, 0)
    CALL(evt_img_entry, STRING("img1"))
    CALL(evt_img_entry, STRING("img2"))
    CALL(evt_img_entry, STRING("img3"))
    CALL(evt_img_entry, STRING("img4"))
    CALL(N_evt_img_set_z, STRING("img1"), FLOAT(4.0f))
    CALL(N_evt_img_set_z, STRING("img2"), FLOAT(3.0f))
    CALL(N_evt_img_set_z, STRING("img3"), FLOAT(2.0f))
    CALL(N_evt_img_set_z, STRING("img4"), FLOAT(1.0f))
    CALL(evt_img_set_paper, STRING("img1"), STRING("OFF_d_meku_right"))
    CALL(evt_img_clear_virtual_point, STRING("img1"))
    CALL(evt_img_alloc_capture, STRING("img1"), 0, 0, 1, 124, 0, 386, 480)
    CALL(evt_img_onoff, STRING("img1"), 1)
    WAIT_FRAMES(1)
    CALL(next)
    CALL(evt_snd_sfxon_, STRING("SFX_EVT_OPN_PAGE_OPEN1"), 0)
    CALL(evt_img_set_paper_anim, STRING("img1"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img1"))
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_09"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_09"))
    CALL(make_msg, STRING("op_book_10"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_10"))
    CALL(make_msg, STRING("op_book_11"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_11"))
    CALL(make_msg, STRING("op_book_12"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_12"))
    CALL(demo_winonoff, 0)
    CALL(evt_img_set_paper, STRING("img2"), STRING("OFF_d_meku_right"))
    CALL(evt_img_clear_virtual_point, STRING("img2"))
    CALL(evt_img_alloc_capture, STRING("img2"), 0, 0, 1, 124, 0, 386, 480)
    CALL(evt_img_onoff, STRING("img2"), 1)
    WAIT_FRAMES(1)
    CALL(next)
    CALL(evt_snd_sfxon_, STRING("SFX_EVT_OPN_PAGE_OPEN1"), 0)
    CALL(evt_img_set_paper_anim, STRING("img2"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img2"))
    CALL(evt_img_free_capture, STRING("img1"), 0)
    CALL(evt_img_release, STRING("img1"))
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_13"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_13"))
    CALL(make_msg, STRING("op_book_14"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_14"))
    CALL(make_msg, STRING("op_book_15"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_15"))
    CALL(make_msg, STRING("op_book_16"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_16"))
    CALL(demo_winonoff, 0)
    CALL(evt_img_set_paper, STRING("img3"), STRING("OFF_d_meku_right"))
    CALL(evt_img_clear_virtual_point, STRING("img3"))
    CALL(evt_img_alloc_capture, STRING("img3"), 0, 0, 1, 124, 0, 386, 480)
    CALL(evt_img_onoff, STRING("img3"), 1)
    WAIT_FRAMES(1)
    CALL(next)
    CALL(evt_snd_sfxon_, STRING("SFX_EVT_OPN_PAGE_OPEN1"), 0)
    CALL(evt_img_set_paper_anim, STRING("img3"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img3"))
    CALL(evt_img_free_capture, STRING("img2"), 0)
    CALL(evt_img_release, STRING("img2"))
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_17"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_17"))
    CALL(make_msg, STRING("op_book_18"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_18"))
    CALL(make_msg, STRING("op_book_19"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_19"))
    CALL(make_msg, STRING("op_book_20"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_20"))
    CALL(make_msg, STRING("op_book_21"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_21"))
    CALL(demo_winonoff, 0)
    CALL(evt_img_set_paper, STRING("img4"), STRING("OFF_d_meku_right"))
    CALL(evt_img_clear_virtual_point, STRING("img4"))
    CALL(evt_img_alloc_capture, STRING("img4"), 0, 0, 1, 124, 0, 386, 480)
    CALL(evt_img_onoff, STRING("img4"), 1)
    WAIT_FRAMES(1)
    CALL(next)
    CALL(evt_snd_sfxon_, STRING("SFX_EVT_OPN_PAGE_OPEN1"), 0)
    CALL(evt_img_set_paper_anim, STRING("img4"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img4"))
    CALL(evt_img_free_capture, STRING("img3"), 0)
    CALL(evt_img_release, STRING("img3"))
    WAIT_MS(1000)
    CALL(demo_winonoff, 1)
    CALL(make_msg, STRING("op_book_22"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_22"))
    CALL(make_msg, STRING("op_book_23"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_23"))
    CALL(make_msg, STRING("op_book_24"))
    CALL(evt_msg_print, 1, PTR(buf), 0, 0)
    CALL(evt_msg_print_add, 0, STRING("op_book_24"))
    CALL(demo_winonoff, 0)
    CALL(evt_bg_disp_onoff, 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_f1_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_02_f2_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_04_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gai_kabe_k"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gai_kabe_k_01"), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("peach"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("nazo"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("hako"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("ロテン１"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("クリボー１"), 128)
    CALL(evt_npc_flag_onoff, 0, STRING("チューさん１"), 128)
    CALL(evt_mobj_flag_onoff, 0, STRING("hako"), 16384)
    CALL(evt_snd_bgmoff_f, 513, 6000)
    CALL(evt_sub_intpl_msec_init, 0, 0, 1000, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(adjust, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_free_capture, STRING("img4"), 0)
    CALL(evt_img_release, STRING("img4"))
    DELETE_EVENT(GW(0))
    CALL(evt_paper_delete, STRING("OFF_d_meku_right"))
    LABEL(100)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, 513, STRING("BGM_DEMO2"))
    CALL(evt_npc_move_position, STRING("peach"), 0, 450, 0, FLOAT(60.0f), 1)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("op_00"), 0, 0)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 100, 834, 0, 50, 437, 800, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("op_01"), 0, STRING("peach"))
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ標準"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 200, 837, 0, 200, 437, 4000, 11)
    WAIT_MS(300)
    LOOP(4)
        CALL(evt_npc_reverse_ry, STRING("peach"))
        WAIT_MS(500)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("op_02"), 0, 0)
    WAIT_MS(300)
    WAIT_MS(1000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("op_03"), 0, STRING("nazo"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 89, 739, 0, 44, 442, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("peach"), STRING("P_B_1"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("peach"), STRING("P_B_2"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("peach"))
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("peach"), STRING("P_B_2"), STRING("P_T_7"))
    CALL(evt_msg_print, 0, STRING("op_04"), 0, STRING("peach"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -100, 89, 739, -100, 44, 442, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ標準"))
    CALL(evt_npc_move_position, STRING("peach"), -55, 450, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING("op_05"), 0, STRING("nazo"))
    WAIT_MS(300)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("op_06"), 0, STRING("peach"))
    WAIT_MS(300)
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ不安"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("peach"), STRING("P_H_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("peach"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("peach"), STRING("P_S_7"))
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ標準2"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -120, 89, 739, -120, 44, 442, 1000, 11)
    CALL(evt_npc_move_position, STRING("peach"), -80, 450, 0, FLOAT(60.0f), 1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("op_07"), 0, STRING("peach"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("hako"), LW(7), LW(8), LW(9))
    CALL(evt_sub_intpl_msec_init, 11, 0, 100, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("hako"), LW(7), LW(0), LW(9))
        SUBTRACT_FLOAT(LW(7), FLOAT(0.099609375f))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print, 0, STRING("op_08"), 0, STRING("nazo"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("nazo"), STRING("A_1"))
    WAIT_MS(700)
    CALL(evt_npc_set_anim, STRING("nazo"), STRING("S_1"))
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ不安"))
    CALL(evt_msg_print, 0, STRING("op_09"), 0, STRING("peach"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -450, 250, 1030, -135, 100, 482, 0, 11)
    CALL(evt_npc_flag_onoff, 1, STRING("peach"), 32768)
    CALL(evt_npc_flag_onoff, 1, STRING("nazo"), 32768)
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ標準2"))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gai_kabe_k"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gai_kabe_k_01"), 1)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("op_10"), 0, STRING("nazo"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("op_11"), 0, STRING("peach"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 0, STRING("peach"), 2)
    CALL(evt_npc_status_onoff, 0, STRING("nazo"), 2)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_get_position, STRING("peach"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_npc_move_position, STRING("peach"), LW(0), LW(2), 0, FLOAT(40.0f), 1)
        SET_VALUE(GF(0), 1)
        LOOP(60)
            CALL(evt_npc_get_position, STRING("hako"), LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(0.08300781f))
            ADD_FLOAT(LW(1), FLOAT(0.16601563f))
            CALL(evt_npc_set_position, STRING("hako"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(GF(1), 1)
        CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ物持ち"))
        WAIT_MS(300)
        CALL(evt_npc_get_position, STRING("peach"), LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        CALL(evt_npc_move_position, STRING("peach"), LW(0), LW(2), 0, FLOAT(40.0f), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("hako"), LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("nazo"), LW(3), LW(4), LW(5))
        SUBTRACT_FLOAT(LW(0), LW(3))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("nazo"), LW(10), LW(11), LW(12))
            ADD_FLOAT(LW(10), LW(0))
            CALL(evt_npc_set_position, STRING("hako"), LW(10), LW(1), LW(2))
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(0)
            IF_INT_EQUAL(GF(1), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_get_position, STRING("hako"), LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("peach"), LW(3), LW(4), LW(5))
        SUBTRACT_FLOAT(LW(0), LW(3))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("peach"), LW(10), LW(11), LW(12))
            ADD_FLOAT(LW(10), LW(0))
            CALL(evt_npc_set_position, STRING("hako"), LW(10), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("nazo"), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    CALL(evt_npc_move_position, STRING("nazo"), LW(0), LW(2), 0, FLOAT(40.0f), 1)
    WAIT_MS(300)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("nazo"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 20)
    CALL(evt_npc_move_position, STRING("nazo"), LW(0), LW(2), 0, FLOAT(40.0f), 1)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("hako"), 225)
    CALL(evt_cam3d_evt_set, -90, 89, 739, -90, 44, 442, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("op_12"), 0, STRING("peach"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -85, 50, 590, -85, 28, 442, 15000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_camid, STRING("hako"), 5)
    CALL(evt_npc_set_anim, STRING("hako"), STRING("A_1"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("hako"), LW(0), LW(1), LW(2))
        ADD(LW(1), 10)
        CALL(evt_eff64, STRING("eff0"), STRING("crystal_n64"), 2, LW(0), LW(1), LW(2), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff64, STRING("eff1"), STRING("crystal_n64"), 6, LW(0), LW(1), LW(2), FLOAT(0.5f), 60, 0, 0, 0, 0, 0, 0)
        CALL(eff_cam)
        WAIT_MS(500)
        CALL(evt_eff64, STRING("eff2"), STRING("radiation_n64"), 3, LW(0), LW(1), LW(2), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        CALL(eff_cam1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("peach"), STRING("P_B_3"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("peach"), STRING("P_B_4"))
    END_INLINE_EVENT
    WAIT_MS(2000)
    LOOP(0)
        IF_INT_EQUAL(GF(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DELETE_EVENT(GW(10))
    CALL(evt_fade_set_mapchange_type, 1, 17, 2000, -1, -1)
    CALL(evt_seq_set_seq, 3, STRING("title"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(koushukei_dai)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, 11, 10, -10, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_rop"), LW(0), 0, 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
        CALL(evt_sub_intpl_msec_init, 11, -10, 10, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_rop"), LW(0), 0, 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(roten_move)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("ロテン１"), 450, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("ロテン１"), -350, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kuribo_move)
    CALL(evt_npc_set_position, STRING("クリボー１"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("クリボー１"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("クリボー１"))
    RETURN
EVT_END()

EVT_BEGIN(chusan_move)
    CALL(evt_npc_set_position, STRING("チューさん１"), -175, 0, 100)
    CALL(evt_npc_get_position, STRING("チューさん１"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("チューさん１"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("チューさん１"))
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(dmo_00_init_evt)
    CALL(demo_init)
    CALL(evt_npc_entry, STRING("peach"), STRING("c_peach"))
    CALL(evt_npc_set_tribe, STRING("peach"), STRING("デモピーチ標準2"))
    CALL(evt_npc_set_ry, STRING("peach"), 90)
    CALL(evt_npc_set_position, STRING("peach"), -2, 0, 550)
    CALL(evt_npc_status_onoff, 1, STRING("peach"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("peach"), 1074267648)
    CALL(evt_npc_entry, STRING("nazo"), STRING("c_nazo"))
    CALL(evt_npc_set_tribe, STRING("nazo"), STRING("謎の人"))
    CALL(evt_npc_set_ry, STRING("nazo"), 90)
    CALL(evt_npc_set_position, STRING("nazo"), -145, 0, 450)
    CALL(evt_npc_status_onoff, 1, STRING("nazo"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("nazo"), 1074266112)
    CALL(evt_npc_set_height, STRING("nazo"), 40)
    CALL(evt_npc_entry, STRING("hako"), STRING("MOBJ_GoldenTreasureBox"))
    CALL(evt_npc_set_anim, STRING("hako"), STRING("S_1"))
    CALL(evt_npc_set_position, STRING("hako"), -125, 0, 450)
    CALL(evt_npc_flag_onoff, 1, STRING("hako"), 1073741824)
    CALL(evt_npc_pera_onoff, STRING("hako"), 0)
    CALL(evt_mobj_itembox, STRING("hako"), 340, 110, -20, 1, 0, 0, 0)
    CALL(evt_mobj_flag_onoff, 1, STRING("hako"), 16384)
    CALL(evt_npc_entry, STRING("ロテン１"), STRING("c_roten"))
    CALL(evt_npc_set_tribe, STRING("ロテン１"), STRING("ロテン"))
    CALL(evt_npc_flag_onoff, 1, STRING("ロテン１"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING("ロテン１"), 2)
    CALL(evt_npc_change_interrupt, STRING("ロテン１"), 1, PTR(roten_move))
    CALL(evt_npc_entry, STRING("クリボー１"), STRING("c_kuribo_n"))
    CALL(evt_npc_set_tribe, STRING("クリボー１"), STRING("NPCクリボー"))
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING("クリボー１"), 2)
    CALL(evt_npc_change_interrupt, STRING("クリボー１"), 1, PTR(kuribo_move))
    CALL(evt_npc_entry, STRING("チューさん１"), STRING("c_tyusan"))
    CALL(evt_npc_set_tribe, STRING("チューさん１"), STRING("チューさん"))
    CALL(evt_npc_flag_onoff, 1, STRING("チューさん１"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING("チューさん１"), 2)
    CALL(evt_npc_change_interrupt, STRING("チューさん１"), 1, PTR(chusan_move))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_paper_entry, STRING("OFF_d_meku_right"))
    CALL(evt_cam3d_evt_set, 0, 385, 1473, 0, 100, 0, 0, 11)
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 1)
    CALL(evt_bg_disp_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("peach"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("nazo"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("hako"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("ロテン１"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("チューさん１"), 128)
    BEGIN_INLINE_EVENT_GET_ID(GW(0))
        SET_TYPE(0)
        CALL(demo_draw)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(demo_evt))
    SPAWN_EVENT(PTR(koushukei_dai))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_animgroup_async, STRING("OFF_d_maku_center"))
    END_INLINE_EVENT
    CALL(evt_fade_set_mapchange_type, 1, -1, -1, -1, 900)
    RETURN
EVT_END()
#else
EVT_BEGIN(dmo_00_init_evt)
    CALL(demo_init)
    CALL(evt_npc_entry, STRING("peach"), STRING("c_peach"))
    CALL(evt_npc_set_tribe, STRING("peach"), STRING(/*"デモピーチ標準2"*/"\x83\x66\x83\x82\x83\x73\x81\x5B\x83\x60\x95\x57\x8F\x80\x32"))
    CALL(evt_npc_set_ry, STRING("peach"), 90)
    CALL(evt_npc_set_position, STRING("peach"), -2, 0, 550)
    CALL(evt_npc_status_onoff, 1, STRING("peach"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("peach"), 1074267648)
    CALL(evt_npc_entry, STRING("nazo"), STRING("c_nazo"))
    CALL(evt_npc_set_tribe, STRING("nazo"), STRING(/*"謎の人"*/"\x93\xE4\x82\xCC\x90\x6C"))
    CALL(evt_npc_set_ry, STRING("nazo"), 90)
    CALL(evt_npc_set_position, STRING("nazo"), -145, 0, 450)
    CALL(evt_npc_status_onoff, 1, STRING("nazo"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("nazo"), 1074266112)
    CALL(evt_npc_set_height, STRING("nazo"), 40)
    CALL(evt_npc_entry, STRING("hako"), STRING("MOBJ_GoldenTreasureBox"))
    CALL(evt_npc_set_anim, STRING("hako"), STRING("S_1"))
    CALL(evt_npc_set_position, STRING("hako"), -125, 0, 450)
    CALL(evt_npc_flag_onoff, 1, STRING("hako"), 1073741824)
    CALL(evt_npc_pera_onoff, STRING("hako"), 0)
    CALL(evt_mobj_itembox, STRING("hako"), 340, 110, -20, 1, 0, 0, 0)
    CALL(evt_mobj_flag_onoff, 1, STRING("hako"), 16384)
    CALL(evt_npc_entry, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), STRING("c_roten"))
    CALL(evt_npc_set_tribe, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), STRING(/*"ロテン"*/"\x83\x8D\x83\x65\x83\x93"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), 2)
    CALL(evt_npc_change_interrupt, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), 1, PTR(roten_move))
    CALL(evt_npc_entry, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), STRING("c_kuribo_n"))
    CALL(evt_npc_set_tribe, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), STRING(/*"NPCクリボー"*/"\x4E\x50\x43\x83\x4E\x83\x8A\x83\x7B\x81\x5B"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), 2)
    CALL(evt_npc_change_interrupt, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), 1, PTR(kuribo_move))
    CALL(evt_npc_entry, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), STRING("c_tyusan"))
    CALL(evt_npc_set_tribe, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), STRING(/*"チューさん"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), 1074267648)
    CALL(evt_npc_status_onoff, 1, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), 2)
    CALL(evt_npc_change_interrupt, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), 1, PTR(chusan_move))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_paper_entry, STRING("OFF_d_meku_right"))
    CALL(evt_cam3d_evt_set, 0, 385, 1473, 0, 100, 0, 0, 11)
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 1)
    CALL(evt_bg_disp_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("peach"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("nazo"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING("hako"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50"), 128)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50"), 128)
    BEGIN_INLINE_EVENT_GET_ID(GW(0))
        SET_TYPE(0)
        CALL(demo_draw)
    END_INLINE_EVENT
    SPAWN_EVENT(demo_evt)
    SPAWN_EVENT(koushukei_dai)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_animgroup_async, STRING("OFF_d_maku_center"))
    END_INLINE_EVENT
    RETURN
EVT_END_PAD()
#endif

