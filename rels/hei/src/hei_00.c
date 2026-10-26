#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/npcdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "cam_road.h"
#include "driver/seqdrv.h"

typedef struct GonbabaProj { GXTexObj obj; smartEntry* tex; } GonbabaProj;
void make_gonbaba_projection(CameraId camera, void* param);

USER_FUNC(cam_test);
USER_FUNC(dist_check);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_win_item_select);
USER_FUNC(make_gonbaba_projection_evt);
static USER_FUNC(proj_mtx);
static USER_FUNC(proj_off);
static USER_FUNC(proj_on);
USER_FUNC(shake);
USER_FUNC(shake_reset);
USER_FUNC(hei_00_mario_keyoff_check);
USER_FUNC(cam_test2);

EvtWord make_proj_evt[4];
EvtWord evt_gonbaba_shadow[161];
static EvtWord first_evt[197];
EvtWord gonbaba_evt[447];
static EvtWord first_evt_dokan[18];
static BeroEntry bero_entry_data[6];
static BeroEntry e3_bero[2];
static EvtWord cam_shake[33];
static EvtWord cam_shake2[27];
EvtWord evt_snd[234];
EvtWord moji_msg[22];
static EvtWord switch_evt[303];
static EvtWord dokan_evt[345];
EvtWord setting_sun_stone[43];
static s32 itemsel_table00[2];
EvtWord sun_stone_set_event[68];
EvtWord setting_moon_stone[43];
static s32 itemsel_table01[2];
EvtWord moon_stone_set_event[68];
static EvtWord ki_data_evt_01[20];
EvtWord ki_data_evt_02[20];
static TreeAccess ki_data[4];
static NpcSetupInfo npcEnt[2];
#ifdef VERSION_G8ME01
EvtWord hei_00_koopa_evt[272];
#else
EvtWord hei_00_koopa_evt[269];
#endif
static EvtWord epilogue_npc_entry[30];
static EvtWord epilogue_evt[90];
EvtWord hei_00_init_evt[297];

static USER_FUNC(proj_on) {
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[0], 0x400);
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[1], 0x400);
    animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[2], 0x400);
    animPoseSetMaterialFlagOn(partyGetPtr(marioGetPartyId())->paperAnimGroup, 0x400);
    return EVT_RETURN_DONE;
}

static USER_FUNC(proj_off) {
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[0], 0x400);
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[1], 0x400);
    animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[2], 0x400);
    animPoseSetMaterialFlagOff(partyGetPtr(marioGetPartyId())->paperAnimGroup, 0x400);
    return EVT_RETURN_DONE;
}

void make_gonbaba_projection(CameraId camera, void* param) {
    GonbabaProj* wp = param;
    CameraEntry saved;
    Mtx44 proj;
    Mtx view;
    Mtx mtx;
    f32 viewport[6];
    f32 projv[7];
    NpcEntry* npc;
    u32 left;
    u32 top;
    u32 width;
    u32 height;

    GXGetViewportv(viewport);
    GXGetProjectionv(projv);
    GXGetScissor(&left, &top, &width, &height);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(1, 1, 606, 478);
    C_MTXPerspective(proj, 25.0f, 1.2666667f, 1.0f, 1000.0f);
    GXSetProjection(proj, GX_PERSPECTIVE);
    C_MTXLookAt(view, &(Vec){0.0f, 190.0f, 0.0f}, &(Vec){0.0f, 0.0f, -1.0f}, &(Vec){0.0f, 0.0f, 0.0f});
    saved = *camGetCurPtr();
    npc = npcNameToPtr("\203S\203\223\203o\203o\211e");
    PSMTXCopy(view, camGetCurPtr()->view);
    PSMTXIdentity(mtx);
    animPoseDrawMtx(npc->poseId, mtx, 1, 0.0f, 2.0f);
    animPoseDrawMtx(npc->poseId, mtx, 2, 0.0f, 2.0f);
    animPoseDrawMtx(npc->poseId, mtx, 3, 0.0f, 2.0f);
    PSMTXCopy(saved.view, camGetCurPtr()->view);
    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(608, 480, GX_TF_RGBA8, GX_FALSE);
    GXCopyTex(wp->tex->address, GX_TRUE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projv);
    GXSetScissor(left, top, width, height);
}

USER_FUNC(make_gonbaba_projection_evt) {
    GonbabaProj* wp;

    if (isFirstCall) {
        event->userdata[0] = (EvtWord)_mapAlloc(sizeof(GonbabaProj));
    }
    wp = (GonbabaProj*)event->userdata[0];
    wp->tex = smartAlloc(GXGetTexBufferSize(608, 480, GX_TF_RGBA8, GX_FALSE, 0), 3);
    GXInitTexObj(&wp->obj, wp->tex->address, 608, 480, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&wp->obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    mapSetProjTexObj(&wp->obj, wp->tex);
    dispEntry(CAMERA_SHADOW, 0.0f, 3, make_gonbaba_projection, wp);
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(proj_mtx) {
    EvtWord* args = event->args;
    f32 px = evtGetFloat(event, args[0]);
    f32 py = evtGetFloat(event, args[1]);
    f32 pz = evtGetFloat(event, args[2]);
    f32 tx = evtGetFloat(event, args[3]);
    f32 ty = evtGetFloat(event, args[4]);
    f32 tz = evtGetFloat(event, args[5]);
    Mtx view;
    Mtx proj;
    Mtx rot;
    Vec pos;
    Vec target;
    Vec up;

    pos = (Vec){px, py, pz};
    target = (Vec){tx, ty, tz};
    up = (Vec){1.0f, 0.0f, 0.0f};
    C_MTXLightPerspective(proj, 25.0f, 1.2666667f, 0.5f, 0.5f, 0.5f, 0.5f);
    C_MTXLookAt(view, &pos, &up, &target);
    PSMTXRotRad(rot, 'y', 0.17453292f);
    PSMTXConcat(proj, view, proj);
    PSMTXConcat(proj, rot, proj);
    mapSetProjMtx(proj);
    return EVT_RETURN_DONE;
}

USER_FUNC(shake) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
    }
    event->userdata[1]--;
    if (event->userdata[1] < 0) {
        event->userdata[0] = 1 - event->userdata[0];
        event->userdata[1] = 2;
    }
    if (event->userdata[0] != 0) {
        camera->viewOffset.z = 3.0f;
    } else {
        camera->viewOffset.z = 0.0f;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(shake_reset) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);

    camera->viewOffset.z = 0.0f;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(make_proj_evt)
    CALL(make_gonbaba_projection_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_gonbaba_shadow)
    SPAWN_EVENT_GET_ID(PTR(make_proj_evt), LW(9))
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_init, EASING_LINEAR, -220, -20, 360)
        LOOP(361)
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 0, -300, 30)
        LOOP(361)
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(GW(1), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_init, EASING_LINEAR, 1200, -1200, 360)
        LOOP(361)
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(GW(2), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HAIKEI"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HAIKEI"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(proj_on)
    BEGIN_INLINE_EVENT_GET_ID(LW(8))
        CALL(shake)
    END_INLINE_EVENT
    LOOP(361)
        SET_VALUE(LW(0), GW(0))
        SET_VALUE(LW(1), GW(1))
        SET_VALUE(LW(2), GW(2))
        ADD(LW(0), 0)
        ADD(LW(1), 3500)
        ADD(LW(2), 0)
        SET_VALUE(LW(3), GW(0))
        SET_VALUE(LW(4), GW(1))
        SET_VALUE(LW(5), GW(2))
        CALL(proj_mtx, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        WAIT_FRAMES(1)
    END_LOOP
    DELETE_EVENT(LW(9))
    DELETE_EVENT(LW(8))
    CALL(shake_reset)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_PROJ_TEX)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(proj_off)
    RETURN
EVT_END()

USER_FUNC(cam_test) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 cx = (mario->position.x + party->position.x) / 2;
    f32 cz = (mario->position.z + party->position.z) / 2;
    CameraParams road;
    Vec dir;

    camRoadMain(&road, 0.0f, 25.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f);
    dir = (Vec){road.pos.x - road.at.x, road.pos.y - road.at.y, road.pos.z - road.at.z};
    PSVECNormalize(&dir, &dir);
    event->lwData[0] = cx + road.at.x + 250.0f * dir.x;
    event->lwData[1] = road.at.y + 250.0f * dir.y;
    event->lwData[2] = cz + road.at.z + 250.0f * dir.z;
    event->lwData[3] = cx + road.at.x;
    event->lwData[4] = road.at.y;
    event->lwData[5] = cz + road.at.z;
    return EVT_RETURN_DONE;
}

USER_FUNC(cam_test2) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 cx = (mario->position.x + party->position.x) / 2;
    f32 cz = (mario->position.z + party->position.z) / 2;
    CameraParams road;
    Vec dir;

    camRoadMain(&road, 0.0f, 75.0f, 0.0f, 0.0f, 75.0f, 0.0f, 0.0f);
    dir = (Vec){road.pos.x - road.at.x, road.pos.y - road.at.y, road.pos.z - road.at.z};
    PSVECNormalize(&dir, &dir);
    event->lwData[0] = cx + road.at.x + 270.0f * dir.x;
    event->lwData[1] = road.at.y + 270.0f * dir.y;
    event->lwData[2] = cz + road.at.z + 270.0f * dir.z;
    event->lwData[3] = cx + road.at.x;
    event->lwData[4] = road.at.y;
    event->lwData[5] = cz + road.at.z;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(first_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_fade_set_mapchange_type, 0, -1, -1, 9, 300)
    CALL(evt_fade_set_mapchange_type, 1, -1, -1, 9, 300)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_cam3d_evt_set, 250, 350, 1041, 250, 130, -36, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_telop_entry, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -250, 350, 1041, -250, 130, -36, 6000, 11)
    WAIT_MS(6000)
    CALL(evt_mario_cont_onoff, 1)
    SET_VALUE(GF(0), 0)
    WAIT_FRAMES(1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -325, 0, FLOAT(60.0f))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -340, 175, 263, -340, 125, -33, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -1)
    ADD(LW(2), 1)
    CALL(evt_mario_mov_pos, LW(0), LW(2), 30)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    LOOP(2)
        CALL(evt_party_set_dir, 0, 270, 300)
        WAIT_MS(600)
        CALL(evt_party_set_dir, 0, 90, 300)
        WAIT_MS(600)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg1_hei_00"), 0, STRING("party"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 23)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(gonbaba_evt)
    LABEL(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -100)
        WAIT_FRAMES(1)
        GOTO(1)
    END_IF
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 1)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_GONBABA_FLY1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_ROAR1"), -117, 300, 1000, 0)
    WAIT_MS(700)
    CALL(evt_eff_fukidashi, 0, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("eff1"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 270, 300, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_I_N"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL_CHILD(PTR(evt_gonbaba_shadow))
    CALL(evt_mario_get_pos, 0, LW(0), LW(2), LW(2))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_party_set_dir, 0, 270, 200)
        ELSE
        CALL(evt_party_set_dir, 0, 90, 200)
    END_IF
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_hei_01"), 0, STRING("party"))
    CALL(evt_npc_set_position, STRING("（・ｘ・）"), 1000, -1000, -1000)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_mario_set_dir_npc, STRING("（・ｘ・）"))
    WAIT_MS(1000)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_npc_set_position, STRING("（・ｘ・）"), 0, 210, -500)
    CALL(evt_npc_set_anim, STRING("（・ｘ・）"), STRING("GNB_F_1"))
    CALL(evt_npc_pera_onoff, STRING("（・ｘ・）"), 0)
    CALL(evt_npc_set_ry, STRING("（・ｘ・）"), 135)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_ROAR1"), -117, 300, -433, 0)
        WAIT_MS(3000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_ROAR1"), 0, 290, -500, 0)
        WAIT_MS(3000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_ROAR1"), 80, 280, -600, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CONVERT_MS_TO_FRAME(LW(0), 10000)
        CALL(evt_sub_intpl_init, EASING_LINEAR, 75, 18, LW(0))
        LOOP(601)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_npc_set_scale, STRING("（・ｘ・）"), LW(0), LW(0), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CONVERT_MS_TO_FRAME(LW(0), 4166)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 210, 114, LW(0))
        LOOP(251)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_npc_get_position, STRING("（・ｘ・）"), LW(2), LW(3), LW(4))
            CALL(evt_npc_set_position, STRING("（・ｘ・）"), LW(2), LW(0), LW(4))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CONVERT_MS_TO_FRAME(LW(0), 10000)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 0, 290, 600)
        LOOP(601)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_npc_get_position, STRING("（・ｘ・）"), LW(2), LW(3), LW(4))
            CALL(evt_npc_set_position, STRING("（・ｘ・）"), LW(0), LW(3), LW(4))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CONVERT_MS_TO_FRAME(LW(0), 10000)
        CALL(evt_sub_intpl_init, EASING_LINEAR, -287, -480, 600)
        LOOP(601)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_npc_get_position, STRING("（・ｘ・）"), LW(2), LW(3), LW(4))
            CALL(evt_npc_set_position, STRING("（・ｘ・）"), LW(2), LW(3), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_set_xyz, 270, LW(1), LW(2), 0, 7000, 11)
    WAIT_MS(9500)
    CALL(evt_npc_set_position, STRING("（・ｘ・）"), 0, -1000, 0)
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    CALL(cam_test)
    CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_party_set_pos, 0, LW(3), LW(4), LW(2))
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_msg_print, 0, STRING("stg1_hei_02"), 0, STRING("party"))
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 23)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 24)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(first_evt_dokan)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 23)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GF(0), 1)
    WAIT_WHILE(GF(0))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "hei_13", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "a_usiro_bero", // hitName
        4096, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "gon_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "a_h_dokan", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "a_h_dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        first_evt_dokan, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_02", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry e3_bero[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "hei_00", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(cam_shake)
    CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

static EVT_BEGIN(cam_shake2)
    CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 10, 0, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_snd)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(233)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(166)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(83)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(116)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(33)
    WAIT_MS(116)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(66)
    WAIT_MS(16)
    WAIT_MS(50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(16)
    WAIT_MS(83)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(33)
    WAIT_MS(150)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(66)
    WAIT_MS(166)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(50)
    WAIT_MS(33)
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(116)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(16)
    WAIT_MS(33)
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF1_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(83)
    WAIT_MS(33)
    WAIT_MS(33)
    WAIT_MS(33)
    WAIT_MS(50)
    WAIT_MS(50)
    WAIT_MS(16)
    WAIT_MS(50)
    WAIT_MS(33)
    WAIT_MS(33)
    WAIT_MS(50)
    WAIT_MS(166)
    RETURN
EVT_END()

USER_FUNC(dist_check) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    Vec pos = {150.0f, 25.0f, 0.0f};

    event->lwData[0] = 0;
    if (party == NULL) {
        return EVT_RETURN_DONE;
    }
    if (PSVECDistance(&pos, &party->position) < 100.0f) {
        event->lwData[0] = 1;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(moji_msg)
    CALL(hei_00_mario_keyoff_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg1_hei_02_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(switch_evt)
    IF_INT_EQUAL(GSWF(1776), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(1777), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(2), 0)
            ADD(LW(2), -50)
            ELSE
            ADD(LW(2), 50)
        END_IF
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(80.0f))
        CALL(evt_mario_adjust_dir)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(2), 0)
            ADD(LW(2), -50)
            ELSE
            ADD(LW(2), 50)
        END_IF
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_STG1ROCK_QUAKE1"), LW(10))
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 1500)
    CALL(evt_snd_sfxoff, LW(10))
    WAIT_MS(200)
    CALL(evt_snd_sfxon, STRING("SFX_STG1_ROCK_MOVE1"), LW(10))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 1000, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(80.0f))
        CALL(evt_mapobj_trans, 1, STRING("isi_3"), LW(2), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("tuki_ana"), LW(2), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("tukinoisi"), LW(2), 0, 0)
        MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("isi_4"), LW(2), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("taiyou_ana"), LW(2), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("taiyounoisi"), LW(2), 0, 0)
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(1080.0f))
        CALL(evt_mapobj_rotate, 1, STRING("isi_3"), 0, LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("tuki_ana"), 0, LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("tukinoisi"), 0, LW(3), 0)
        MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("isi_4"), 0, LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("taiyou_ana"), 0, LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("taiyounoisi"), 0, LW(3), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_hit_bind_mapobj, STRING("a_isi_3"), STRING("isi_3"))
    CALL(evt_hit_bind_mapobj, STRING("a_isi_4"), STRING("isi_4"))
    CALL(evt_hit_bind_update, STRING("a_isi_3"))
    CALL(evt_hit_bind_update, STRING("a_isi_4"))
    CALL(evt_cam3d_evt_set, 146, 127, 328, 146, 75, 33, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_moji"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_moji"), 1, 1)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_moji"), 0, PTR(moji_msg), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG1ROCK_FONT_APPEAR1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_color, 1, STRING("s_moji"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(dokan_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(dist_check)
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_LESS(LW(0), 150)
                CALL(evt_mario_set_dir, 90, 300, 0)
                IF_INT_GREATER(LW(0), 90)
                    CALL(evt_party_move_pos, 0, 90, 0, 500)
                    CALL(evt_party_set_dir, 0, 90, 300)
                END_IF
                ELSE
                CALL(evt_mario_set_dir, 270, 300, 0)
                IF_INT_LESS(LW(0), 200)
                    CALL(evt_party_move_pos, 0, 200, 0, 500)
                    CALL(evt_party_set_dir, 0, 270, 300)
                END_IF
            END_IF
        END_IF
    END_INLINE_EVENT
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_QUAKE1"), LW(0), LW(1), LW(2), LW(3))
        LOOP(0)
            IF_INT_EQUAL(GF(1), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(cam_shake), LW(0))
    WAIT_MS(2000)
    DELETE_EVENT(LW(0))
    CALL(evt_cam3d_evt_set, 145, 100, 450, 145, 40, -140, 1000, 11)
    CALL_CHILD(PTR(cam_shake2))
    WAIT_MS(1000)
    SET_VALUE(GF(1), 1)
    CALL(evt_paper_entry, STRING("OFF_hei_00"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, STRING("isi_0"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, STRING("s_moji"), STRING("offscreen"))
    WAIT_FRAMES(1)
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_hei_00"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 0, 0)
    CALL(evt_img_set_virtual_point, STRING("img"), 150, 25, 20)
    CALL(evt_img_set_position, STRING("img"), 9, -14)
    CALL(evt_img_set_shadow, STRING("img"), 2)
    WAIT_FRAMES(1)
    SPAWN_EVENT(PTR(evt_snd))
    BEGIN_INLINE_EVENT
        WAIT_MS(3200)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_SUCCESS1"))
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("isi_0"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("s_moji"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_free_capture, STRING("img"), 1)
    CALL(evt_img_set_shadow, STRING("img"), 0)
    CALL(evt_img_onoff, STRING("img"), 0)
    WAIT_MS(830)
    CALL(evt_mapobj_get_position, STRING("h_dokan"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_trans, 0, STRING("h_dokan"), 0, -20, 0)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("h_dokan"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, -20, 0, 60)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(evt_mapobj_trans, 0, STRING("h_dokan"), 0, LW(0), 0)
    IF_INT_EQUAL(LW(1), 1)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("isi_0"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_moji"))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("isi_0"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_isi_0"), 0, 0)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("s_moji"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_moji"), 0, 0)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("h_dokan"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), STRING("dokan"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, 34)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG1_HEI1"), 2000)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

USER_FUNC(hei_00_mario_keyoff_check) {
    EvtWord* args = event->args;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(setting_sun_stone)
    IF_INT_EQUAL(GSWF(1776), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("taiyou_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("taiyounoisi"), MAP_OBJ_FLAG_HIDDEN)
        IF_INT_EQUAL(GSWF(1777), 1)
            CALL(evt_mapobj_trans, 1, STRING("isi_4"), -80, 0, 0)
            CALL(evt_mapobj_trans, 1, STRING("taiyounoisi"), -80, 0, 0)
            CALL(evt_hit_bind_mapobj, STRING("a_isi_4"), STRING("isi_4"))
            CALL(evt_hit_bind_update, STRING("a_isi_4"))
        END_IF
    END_IF
    RETURN
EVT_END()

static s32 itemsel_table00[] = {
    ITEM_SUN_STONE, -1,
};

EVT_BEGIN(sun_stone_set_event)
    IF_INT_EQUAL(GSWF(1776), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_win_item_select, 0, PTR(itemsel_table00), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        SET_VALUE(GSWF(1776), 1)
        CALL(evt_pouch_remove_item, ITEM_SUN_STONE, LW(0))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("taiyou_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("taiyounoisi"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, STRING("taiyounoisi"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_STONE_SET1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg1_hei_00_sun"), 0, 0)
        CALL_CHILD(PTR(switch_evt))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(setting_moon_stone)
    IF_INT_EQUAL(GSWF(1777), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tuki_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tukinoisi"), MAP_OBJ_FLAG_HIDDEN)
        IF_INT_EQUAL(GSWF(1776), 1)
            CALL(evt_mapobj_trans, 1, STRING("isi_3"), 80, 0, 0)
            CALL(evt_mapobj_trans, 1, STRING("tukinoisi"), 80, 0, 0)
            CALL(evt_hit_bind_mapobj, STRING("a_isi_3"), STRING("isi_3"))
            CALL(evt_hit_bind_update, STRING("a_isi_3"))
        END_IF
    END_IF
    RETURN
EVT_END()

static s32 itemsel_table01[] = {
    ITEM_MOON_STONE, -1,
};

EVT_BEGIN(moon_stone_set_event)
    IF_INT_EQUAL(GSWF(1777), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_win_item_select, 0, PTR(itemsel_table01), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        SET_VALUE(GSWF(1777), 1)
        CALL(evt_pouch_remove_item, ITEM_MOON_STONE, LW(0))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tuki_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tukinoisi"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, STRING("tukinoisi"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_STONE_SET1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg1_hei_00_moon"), 0, 0)
        CALL_CHILD(PTR(switch_evt))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(ki_data_evt_01)
    CALL(evt_item_delete_check, STRING("item_ki01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki01"), ITEM_STAR_PIECE, -468, 139, -127, ITEM_TYPE_FALL_NO_PICKUP, GSWF(5598), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ki_data_evt_02)
    CALL(evt_item_delete_check, STRING("item_ki02"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki02"), ITEM_MUSHROOM, -408, 139, -127, ITEM_TYPE_FALL_NO_PICKUP, GSWF(1778), 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess ki_data[] = {
    {0, "a_ki_1", "miki_1", "ha_temae_1", "ha_usiro_1", ki_data_evt_01},
    {0, "a_ki_2", "miki_2", "ha_temae_2", "ha_usiro_2", ki_data_evt_02},
    {0, "a_ki_3", "miki_3", "ha_temae_3", "ha_usiro_3", NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static NpcSetupInfo npcEnt[] = {
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(hei_00_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -312, 75, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_npc_set_position, STRING("ババ"), -500, 75, 0)
    CALL(evt_cam3d_evt_set, -148, 195, 539, -148, 95, -22, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
#ifdef VERSION_G8ME01
    CALL(evt_mario_kill_party, 0)
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI1"))
    CALL(evt_mario_mov_pos2, -262, -50, 60)
    CALL(evt_mario_jump_pos, -190, 25, -40, 700, 0, -1000)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 300)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -105, -40, 60)
    CALL(evt_mario_wait_move_end)
    CALL(evt_cam3d_evt_set, -148, 175, 427, -148, 95, -22, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_000"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 270, 400, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("ババ"), -348, 75, -95)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_kemuri1, STRING("ババ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_glide_position, STRING("ババ"), -200, 50, -40, 1500, 60, 40, 11, 0)
    SET_VALUE(GF(0), 1)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_001"), 0, STRING("ババ"))
    CALL(evt_mario_set_dir, 90, 400, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_002"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_003"), 0, STRING("ババ"))
    SET_VALUE(GSW_STORY_PROGRESS, 115)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_LAUGH2_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_mov_pos2, 150, -40, 120)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_move_position, STRING("ババ"), 120, -40, 20, 0, 120, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(2000)
    SET_VALUE(GF(0), 1)
    CALL(evt_bero_mapchange, STRING("kpa_02"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -200, 25, 35)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI1"))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_cam3d_evt_set, -200, 140, 574, -8, 70, 10, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("クリスチーヌ"), -50, 35, 0, 60, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_37"), 0, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("クリスチーヌ"), 130, 35, 0, 60, 0)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(hei_00_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 114)
        CALL(evt_mario_set_mode, 2)
        SPAWN_EVENT(PTR(hei_00_koopa_evt))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), STRING("dokan"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 23)
        SPAWN_EVENT(PTR(first_evt))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 24)
        SPAWN_EVENT(PTR(gonbaba_evt))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 34)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("isi_0"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_isi_0"), 0, 0)
        CALL(evt_mapobj_trans, 0, STRING("h_dokan"), 0, 0, 0)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("h_dokan"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
        SET_VALUE(GSWF(1774), 1)
        SET_VALUE(GSWF(1775), 1)
        SET_VALUE(GSWF(1776), 1)
        SET_VALUE(GSWF(1777), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("h_dokan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_trans, 0, STRING("h_dokan"), 0, -20, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_moji"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_moji"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 34)
        CALL(evt_mapobj_get_position, STRING("isi_3"), LW(0), LW(1), LW(2))
        CALL(evt_mapobj_get_position, STRING("isi_4"), LW(10), LW(11), LW(12))
        CALL(evt_mobj_timerswitch, 0, STRING("sw_1"), LW(0), 25, LW(2), STRING("sw_2"), LW(10), 25, LW(12), 120, PTR(dokan_evt), GSWF(1804))
        IF_INT_EQUAL(GSWF(1776), 1)
            IF_INT_EQUAL(GSWF(1777), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_moji"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_moji"), 1, 1)
                CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_moji"), 0, PTR(moji_msg), 0)
            END_IF
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("taiyounoisi"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_EQUAL(GSWF(1774), 1)
        IF_INT_EQUAL(GSWF(1776), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_isi_4"), 0, PTR(sun_stone_set_event), 0)
            ELSE
            CALL_CHILD(PTR(setting_sun_stone))
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tukinoisi"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_EQUAL(GSWF(1775), 1)
        IF_INT_EQUAL(GSWF(1777), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_isi_3"), 0, PTR(moon_stone_set_event), 0)
            ELSE
            CALL_CHILD(PTR(setting_moon_stone))
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(ki_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    CALL(evt_map_playanim, STRING("anm_mugi_1"), 1, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_hatenablk"), 450, 60, 0, ITEM_MUSHROOM, 0, GSWF(1780), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -455, 135, -75, 0, 0)
    CALL(evt_npc_pera_onoff, STRING("ゴンババ影"), 0)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 114)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 402)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI1"))
        END_IF
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        SET_VALUE(LW(0), STRING("dokan_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_2"), PTR(e3_bero))
        SET_VALUE(LW(0), STRING("dokan_2"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    DEBUG_PUT_MSG(STRING("おわったよ"))
    RETURN
EVT_END()

