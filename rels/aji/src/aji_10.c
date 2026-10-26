#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "system.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/dispdrv.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "memory.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "aji.h"
#include "driver/seqdrv.h"

typedef struct MonitorWork { void* buf; s32 ready; s32 kanbu; } MonitorWork;
static MonitorWork* monitor_wp;
void monitor_cap_capture(CameraId cameraId, void* param);

USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_shuryolight_init);
USER_FUNC(evt_shuryolight_run_evt);
USER_FUNC(evt_snd_sfx_dist);
USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(monitor_cap_evt);
USER_FUNC(monitor_cap_init);
USER_FUNC(monitor_kanbu_flag_onoff);
static USER_FUNC(peach_evt_toumei);
static USER_FUNC(peach_set_pose);
static USER_FUNC(viewport);

void monitor_cap_tev(MapTevCallbackInfo* info);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord bero_peach_evt[30];
static BeroEntry bero_peach_data[2];
EvtWord monitor_on[33];
EvtWord monitor_off[37];
EvtWord monitor_init[41];
static EvtWord peach_evt_stg1[1411];
static EvtWord peach_evt_stg2[463];
static EvtWord peach_evt_stg3[251];
EvtWord peach_move[72];
EvtWord peach_evt_stg3_syuryo_talk[138];
static EvtWord peach_evt_stg4[235];
static EvtWord peach_evt_stg5[281];
static EvtWord peach_evt_stg6[349];
EvtWord eff_test[36];
static EvtWord teshita1_init[9];
static EvtWord teshita2_init[9];
static EvtWord teshita3_init[9];
EvtWord kanbu_init[9];
static EvtWord syuryo_init[24];
static NpcSetupInfo npcEnt[9];
EvtWord aji_10_init_evt[279];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_e_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_e_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_e_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_e_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "e_bero3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_11", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(bero_peach_evt)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_set_ry, STRING("みはり"), 90)
        WAIT_MS(300)
        CALL(evt_eff_fukidashi, 1, STRING(""), STRING("みはり"), 1, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_peach_data[] = {
    {
        "e_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        bero_peach_evt, // exitEvent
        "aji_11", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

USER_FUNC(monitor_cap_init) {
    u32 size = GXGetTexBufferSize(0x260, 0x1E0, GX_TF_I4, GX_FALSE, 0);

    monitor_wp = _mapAlloc(sizeof(MonitorWork));
    monitor_wp->buf = _mapAlloc(size);
    monitor_wp->ready = 0;
    monitor_wp->kanbu = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(monitor_kanbu_flag_onoff) {
    monitor_wp->kanbu = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

void monitor_cap_capture(CameraId cameraId, void* param) {
    CameraEntry saved = *camGetCurPtr();
    Mtx44 projMtx;
    Mtx lookAt;
    Mtx ident;
    f32 viewport[6];
    f32 proj[7];
    u32 sx, sy, sw, sh;
    NpcEntry* npc;

    GXGetViewportv(viewport);
    GXGetProjectionv(proj);
    GXGetScissor(&sx, &sy, &sw, &sh);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(1, 1, 606, 478);
    C_MTXPerspective(projMtx, 60.0f, 1.2666667f, 1.0f, 1000.0f);
    GXSetProjection(projMtx, GX_PERSPECTIVE);
    if (monitor_wp->kanbu) {
        npc = npcNameToPtr("\203J\203\223\203u\201[");
        {
            static const Vec monitor_kanbu_pos = {0.0f, 28.0f, 46.0f};
            static const Vec monitor_kanbu_up = {0.0f, 1.0f, 0.0f};
            static const Vec monitor_kanbu_target = {0.0f, 3.0f, 0.0f};
            Vec pos;
            Vec up;
            Vec target;

            target = monitor_kanbu_target;
            up = monitor_kanbu_up;
            pos = monitor_kanbu_pos;

            C_MTXLookAt(lookAt, &pos, &up, &target);
        }
    } else {
        npc = npcNameToPtr("\216\350\211\272\202P");
        {
            static const Vec monitor_teshita_pos = {0.0f, 25.0f, 30.0f};
            static const Vec monitor_teshita_up = {0.0f, 1.0f, 0.0f};
            static const Vec monitor_teshita_target = {0.0f, 0.0f, 0.0f};
            Vec pos;
            Vec up;
            Vec target;

            target = monitor_teshita_target;
            up = monitor_teshita_up;
            pos = monitor_teshita_pos;

            C_MTXLookAt(lookAt, &pos, &up, &target);
        }
    }
    PSMTXCopy(lookAt, camGetCurPtr()->view);
    PSMTXIdentity(ident);
    animPosePeraOff(npc->poseId);
    animPoseMain(npc->poseId);
    animPoseDrawMtx(npc->poseId, ident, 1, 0.0f, 1.0f);
    animPoseDrawMtx(npc->poseId, ident, 2, 0.0f, 1.0f);
    animPoseDrawMtx(npc->poseId, ident, 3, 0.0f, 1.0f);
    animPosePeraOn(npc->poseId);
    PSMTXCopy(saved.view, camGetCurPtr()->view);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x130, 0xF0, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(monitor_wp->buf, GX_TRUE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(proj);
    GXSetScissor(sx, sy, sw, sh);
    monitor_wp->ready = 1;
}

USER_FUNC(monitor_cap_evt) {
    dispEntry(CAMERA_3D, 1000.0f, 0, monitor_cap_capture, NULL);
    return EVT_RETURN_BLOCK;
}

void monitor_cap_tev(MapTevCallbackInfo* info) {
    Mtx proj;
    Mtx view;
    GXTexObj tex;
    Vec camPos;
    Vec pos;

    if (monitor_wp->ready) {
        GXInitTexObj(&tex, monitor_wp->buf, 0x130, 0xF0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_HALF, GX_CC_RASC, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        mapObjGetPos("monitor_1", &pos);
        pos.y -= 70.0f;
        camPos = pos;
        camPos.z += 600.0f;
        {
            Vec up = {0.0f, -1.0f, 0.0f};

            C_MTXLightPerspective(proj, 25.0f, 1.2666667f, 0.5f, 0.5f, 0.5f, 0.5f);
            C_MTXLookAt(view, &camPos, &up, &pos);
        }
        PSMTXConcat(proj, view, proj);
        PSMTXConcat(proj, info->mtx, proj);
        GXLoadTexMtxImm(proj, GX_TEXMTX0, GX_MTX3x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        info->tevStage = 1;
        info->texCoord = 1;
        info->texMap = 1;
    }
}

static USER_FUNC(peach_set_pose) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}

static USER_FUNC(viewport) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 w = evtGetFloat(event, args[2]);
    f32 h = evtGetFloat(event, args[3]);
    CameraEntry* cam;

    cam = camGetPtr(CAMERA_3D);
    cam->viewport[0] = x;
    cam->scissor[0] = x;
    cam->viewport[1] = y;
    cam->scissor[1] = y;
    cam->viewport[2] = w;
    cam->scissor[2] = w;
    cam->viewport[3] = h;
    cam->scissor[3] = h;
    cam = camGetPtr(CAMERA_2D);
    cam->viewport[0] = x;
    cam->viewport[1] = y;
    cam->viewport[2] = w;
    cam->viewport[3] = h;
    return EVT_RETURN_DONE;
}

static USER_FUNC(peach_evt_toumei) {
    MarioWork* mario = marioGetPtr();

    mario->scale.z = mario->scale.y = mario->scale.x = 0.0f;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(monitor_on)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("monitor"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("monitor"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_SCREEN_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("monitore_1"), 0, 0)
    CALL(evt_map_checkanim, STRING("monitore_1"), LW(0), LW(1))
    WAIT_MS(LW(1))
    RETURN
EVT_END()

EVT_BEGIN(monitor_off)
    CALL(evt_map_playanim, STRING("monitore_2"), 0, 0)
    CALL(evt_map_checkanim, STRING("monitore_2"), LW(0), LW(1))
    CALL(evt_mapobj_get_position, STRING("monitor"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d_ex, STRING("SFX_PEACH_SCREEN_SHUT1"), LW(2), LW(3), LW(4), 255, 255, LW(1), 16, 0)
    WAIT_MS(LW(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

EVT_BEGIN(monitor_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor"), MAP_OBJ_FLAG_HIDDEN)
    CALL(monitor_cap_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 261)
        CALL(monitor_kanbu_flag_onoff, 1)
        ELSE
        CALL(monitor_kanbu_flag_onoff, 0)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(monitor_cap_evt)
    END_INLINE_EVENT
    CALL(evt_map_set_tevcallback, 1, PTR(monitor_cap_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("monitor_1"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("monitor_1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_fade_tec_onoff, 1, 0)
    CALL(viewport, 0, 0, 1, 1)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_npc_set_position, STRING("手下３"), -420, 0, 0)
    CALL(evt_npc_set_ry, STRING("手下３"), 90)
    CALL(evt_npc_set_position, STRING("手下２"), -420, 0, 0)
    CALL(evt_npc_set_ry, STRING("手下２"), 90)
    CALL(evt_npc_set_position, STRING("カンブー"), 0, 0, -150)
    CALL(evt_npc_set_ry, STRING("カンブー"), 270)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_set_position, STRING("マジョリン"), 40, 0, -450)
    CALL(evt_npc_set_position, STRING("マリリン"), 40, 0, -450)
    CALL(evt_npc_set_position, STRING("ビビアン"), 40, 0, -450)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_cam3d_evt_set, -320, 123, 431, -320, 45, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_peach_set_condition, 4)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 303, 0, 200)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(LW(2), 608)
    SUBTRACT(LW(2), LW(0))
    SUBTRACT(LW(2), LW(0))
    CALL(viewport, LW(0), 240, LW(2), 1)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 239, 0, 200)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(LW(2), 480)
    SUBTRACT(LW(2), LW(0))
    SUBTRACT(LW(2), LW(0))
    CALL(viewport, 0, LW(0), 608, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(viewport, 0, 0, 608, 480)
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(1400)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), -315, 0, 0, 60, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_mov_pos2, -345, 0, 60)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("手下２"), -375, 0, 0, 60, 0)
    END_INLINE_EVENT
    WAIT_MS(1750)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("手下３"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_get_position, STRING("手下３"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("手下３"), STRING("S_1"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), -135, 0, 0, 60, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_mov_pos2, -303, 0, 84)
        CALL(evt_mario_wait_move_end)
        CALL(evt_mario_mov_pos2, -165, 0, 60)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下２"), -195, 0, 0, 60, 0)
    END_INLINE_EVENT
    WAIT_MS(3500)
    CALL(evt_npc_set_position, STRING("手下３"), -60, 0, 0)
    CALL(evt_mario_set_pos, -90, 0, 0)
    CALL(evt_npc_set_position, STRING("手下２"), -120, 0, 0)
    CALL(evt_cam3d_evt_set, 60, 123, 431, 60, 45, -13, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1700)
        CALL(evt_npc_set_ry, STRING("カンブー"), 90)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), 90, 0, 0, 60, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 60, 0, 60)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下２"), 30, 0, 0, 60, 0)
    END_INLINE_EVENT
    WAIT_MS(2800)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), 90, -50, 700, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下２"), 30, -50, 700, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(900)
    CALL(evt_msg_print, 0, STRING("peach_aji_00_01"), 0, STRING("手下３"))
    CALL(evt_cam3d_evt_set, 60, 80, 233, 60, 37, -13, 500, 0)
    WAIT_MS(700)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_PEACH_PIC_BIRIBIRI1"), 0)
    CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 11, 30, 120, 120, 80, -120, 120, FLOAT(3.0f), 25, 0, 0, 0)
    WAIT_MS(230)
    CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 11, 90, 120, 120, 0, -120, 120, FLOAT(3.0f), 25, 0, 0, 0)
    WAIT_MS(230)
    CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 11, 10, 75, 120, 110, 55, 120, FLOAT(3.0f), 25, 0, 0, 0)
    WAIT_MS(730)
    CALL(evt_fade_tec_onoff, 0, 1)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("peach_aji_00"), 0, STRING("シュリョー"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 800, 11)
    WAIT_MS(900)
    CALL(evt_peach_set_condition, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("peach_aji_01"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 95, 283, 60, 43, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_02"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_03"), 0, STRING("シュリョー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_04"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 125, 226, 576, 125, 110, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(monitor_on))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 500, 0)
        WAIT_MS(500)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("手下１"), STRING("P_1"))
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_dist, LW(3), 7)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("手下１"), STRING("S_1"))
    WAIT_MS(300)
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 50)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("peach_aji_05"), 0, STRING("手下１"))
    CALL(evt_msg_print, 0, STRING("peach_aji_06"), 0, STRING("シュリョー"))
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 50)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("peach_aji_07"), 0, STRING("手下１"))
    CALL(evt_cam3d_evt_set, 160, 181, 379, 160, 103, -13, 700, 11)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("peach_aji_08"), 0, STRING("シュリョー"))
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 50)
    ADD(LW(1), 50)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("peach_aji_09"), 0, STRING("手下１"))
    CALL(evt_msg_print, 0, STRING("peach_aji_10"), 0, STRING("mario"))
    CALL(evt_npc_move_position, STRING("シュリョー"), 190, 0, 300, 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_11"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 85, 233, 60, 42, -13, 600, 11)
    WAIT_MS(600)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SAD2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_B_2"), STRING("P_T_7"))
    CALL(evt_msg_print, 0, STRING("peach_aji_12"), 0, STRING("mario"))
    CALL(peach_set_pose, STRING("P_C_1"))
    CALL(evt_cam3d_evt_set, 130, 162, 429, 130, 75, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_13"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_14"), 0, STRING("手下３"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), 90, 0, 700, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下２"), 30, 0, 700, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING("peach_aji_15"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_16"), 0, STRING("手下３"))
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("手下３"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_get_position, STRING("手下３"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("手下３"), 270)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("手下２"), 270)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("手下３"), STRING("S_1"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(peach_set_pose, STRING("P_S_1"))
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下３"), -60, 0, 0, 60, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -90, 0, 60)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("手下２"), -120, 0, 0, 60, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("手下３"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(PTR(monitor_off))
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("peach_aji_17"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 0, 119, 183, 0, 80, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_17_01"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_18"), 0, STRING("カンブー"))
    CALL(evt_npc_flag_onoff, 1, STRING("カンブー"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_move_position, STRING("カンブー"), -150, -150, 0, 100, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("カンブー"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_position, STRING("シュリョー"), 200, 30, 0)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    WAIT_MS(300)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_19"), 0, STRING("シュリョー"))
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("peach_aji_20"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 130, 162, 429, 130, 75, -13, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("手下２"), -30, 0, 0)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("手下２"), 60, 0, 1000, 0, 4)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("S_1"))
    CALL(evt_msg_print, 0, STRING("peach_aji_21"), 0, STRING("手下２"))
    CALL(evt_msg_print, 0, STRING("peach_aji_22"), 0, STRING("シュリョー"))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("手下２"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_23"), 0, STRING("手下２"))
    CALL(evt_msg_print, 0, STRING("peach_aji_24"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_25"), 0, STRING("手下２"))
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("手下２"), 270)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("S_1"))
    CALL(evt_npc_move_position, STRING("手下２"), -30, 0, 1000, 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("手下２"), NPC_FLAG_NO_DRAW)
    WAIT_MS(1000)
    LABEL(100)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_25_01"), 0, 0)
    CALL(evt_cam3d_evt_set, 40, 95, 283, 40, 43, -13, 800, 11)
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_majo_disp_on, STRING("マジョリン"), 60, 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_majo_disp_on, STRING("マリリン"), 30, 0, -30, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_majo_disp_on, STRING("ビビアン"), 20, 0, 30, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_26"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_27"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 40, 95, 283, 40, 43, -13, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_28"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, 130, 162, 429, 130, 75, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_29"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 40, 95, 283, 40, 43, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_30"), 0, STRING("マジョリン"))
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_msg_print, 0, STRING("peach_aji_31"), 0, STRING("マジョリン"))
    CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_32"), 0, STRING("マリリン"))
    CALL(evt_npc_majo_disp_off, STRING("マリリン"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_A1_1"))
    CALL(evt_msg_print, 0, STRING("peach_aji_33"), 0, STRING("ビビアン"))
    WAIT_MS(500)
    CALL(evt_npc_majo_disp_off, STRING("ビビアン"))
    WAIT_MS(300)
    CALL(evt_fade_tec_onoff, 1, 1)
    WAIT_MS(1000)
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 0, 12, 100, 11, 100)
    CALL(evt_bero_mapchange, STRING("aji_01"), STRING("peach_evt_stg1"))
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_set_position, STRING("研究員"), 60, 0, 0)
    CALL(evt_npc_set_ry, STRING("研究員"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("研究員"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_flag_onoff, 0, STRING("研究員"), NPC_FLAG_INSTANT_TURN)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_01"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 90, 283, 60, 38, -12, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_02"), 0, STRING("研究員"))
    CALL(evt_cam3d_evt_set, -350, 90, 283, -350, 38, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING("手下２"), -420, 0, 0)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(w_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(1400)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("手下２"), -350, 0, 0, 60, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_03"), 0, STRING("手下２"))
    CALL(evt_npc_move_position, STRING("手下２"), -260, 0, 0, 60, 0)
    CALL(evt_npc_set_position, STRING("研究員"), 45, 0, -50)
    CALL(evt_npc_set_position, STRING("手下２"), -25, 0, 0)
    CALL(evt_cam3d_evt_set, 135, 170, 478, 135, 84, -13, 0, 11)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下２"), 60, 0, 0, 60, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("peach_aji_72_04"), 0, STRING("シュリョー"))
    LOOP(0)
        IF_INT_NOT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("peach_aji_72_05"), 0, STRING("手下２"))
    CALL(evt_msg_print, 0, STRING("peach_aji_72_06"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_72_07"), 0, STRING("手下２"))
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_08"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 90, 283, 60, 38, -12, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_09"), 0, STRING("手下２"))
    CALL(evt_msg_print, 0, STRING("peach_aji_72_10"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 135, 170, 478, 135, 84, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_11"), 0, STRING("手下２"))
    CALL(evt_npc_set_anim, STRING("手下２"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("手下２"), 270)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("手下２"), -50, 0, 0, 60, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_12"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_72_13"), 0, STRING("研究員"))
    CALL(evt_npc_set_anim, STRING("研究員"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("研究員"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("研究員"), 270)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("研究員"), -50, -50, 0, 60, 0)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_72_14"), 0, STRING("シュリョー"))
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("aji_16"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_set_position, STRING("マジョリン"), 60, 0, 0)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("マリリン"), 30, 0, -30)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("ビビアン"), 20, 0, 30)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ビビアン"), NPC_FLAG_INSTANT_TURN)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_103"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 40, 95, 283, 40, 43, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_104"), 0, STRING("マジョリン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_105"), 0, STRING("シュリョー"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_106"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, 120, 172, 478, 120, 75, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_107"), 0, STRING("シュリョー"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_108"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, 40, 95, 283, 40, 43, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_109"), 0, STRING("マジョリン"))
    CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_110"), 0, STRING("マリリン"))
    CALL(evt_npc_majo_disp_off, STRING("マリリン"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_A1_1"))
    CALL(evt_msg_print, 0, STRING("peach_aji_111"), 0, STRING("ビビアン"))
    WAIT_MS(500)
    CALL(evt_npc_majo_disp_off, STRING("ビビアン"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("aji_16"), STRING("peach_evt_stg3"))
    RETURN
EVT_END()

EVT_BEGIN(peach_move)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), LW(3))
    ADD(LW(6), 35)
    IF_INT_GREATER(LW(0), LW(6))
        SET_VALUE(LW(6), LW(5))
        SUBTRACT(LW(6), 10)
        SET_VALUE(LW(7), LW(5))
        ADD(LW(7), 10)
        IF_INT_GREATER_OR_EQUAL(LW(2), LW(6))
            IF_INT_LESS_OR_EQUAL(LW(2), LW(7))
                SUBTRACT(LW(0), 5)
                CALL(evt_mario_mov_pos2, LW(0), -15, 70)
            END_IF
        END_IF
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), -130)
        CALL(evt_mario_mov_pos2, -130, LW(2), 70)
    END_IF
    CALL(evt_mario_mov_pos2, -150, 0, 70)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_stg3_syuryo_talk)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_NO_HIT)
    CALL_CHILD(PTR(peach_move))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("シュリョー"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_141"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_142"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -125, 116, 374, -125, 57, -18, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_143"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_144"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_145"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_146"), 0, STRING("シュリョー"))
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("シュリョー"), -90, 0, 500, 0, 0)
        CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("peach_aji_147"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_148"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_149"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_150"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_151"), 0, STRING("mario"))
    CALL(evt_mario_mov_pos2, -300, 0, FLOAT(80.0f))
    SET_VALUE(GSW_STORY_PROGRESS, 168)
    CALL(evt_bero_mapchange, STRING("aji_09"), STRING("s_bero"))
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_set_position, STRING("研究員"), 60, 0, 0)
    CALL(evt_npc_set_ry, STRING("研究員"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("研究員"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_flag_onoff, 0, STRING("研究員"), NPC_FLAG_INSTANT_TURN)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_01"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 90, 283, 60, 38, -12, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_02"), 0, STRING("研究員"))
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_03"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 135, 170, 478, 135, 84, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_04"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_05"), 0, STRING("研究員"))
    CALL(evt_npc_set_anim, STRING("研究員"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("研究員"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("研究員"), 270)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("研究員"), -50, 0, 0, 60, 0)
    CALL(evt_npc_set_position, STRING("研究員"), 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_06"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_166_00_07"), 0, STRING("シュリョー"))
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("aji_17"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_position, STRING("手下１"), -375, 0, -70)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("monitor"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("monitore_1"), 2, 0)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_196"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 165, 77, 481, 165, 100, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 50)
    ADD(LW(1), 50)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("peach_aji_197"), 0, STRING("カンブー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_198"), 0, STRING("シュリョー"))
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("monitor_1"), LW(0), LW(1), LW(2))
    ADD(LW(0), 50)
    ADD(LW(1), 50)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("peach_aji_199"), 0, STRING("カンブー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_200"), 0, STRING("シュリョー"))
    CALL_CHILD(PTR(monitor_off))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_201"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 165, 170, 530, 165, 85, -13, 800, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("手下１"), -100, 0, 0)
    CALL(evt_npc_move_position, STRING("手下１"), 80, 0, 0, FLOAT(90.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_202"), 0, STRING("手下１"))
    CALL(evt_msg_print, 0, STRING("peach_aji_203"), 0, STRING("シュリョー"))
    CALL(evt_msg_print, 0, STRING("peach_aji_204"), 0, STRING("手下１"))
    CALL(evt_npc_set_anim, STRING("手下１"), STRING("P_1"))
    CALL(evt_npc_get_position, STRING("手下１"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_3RD_BATTEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("手下１"), 270)
    CALL(evt_npc_wait_pera, STRING("手下１"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("手下１"), -100, 0, 0, FLOAT(90.0f), 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 202, 189, 234, 202, 80, -13, 1500, 11)
    WAIT_MS(1500)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_205"), 0, STRING("シュリョー"))
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("aji_17"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 210, 30, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_set_position, STRING("手下１"), 60, 0, 0)
    CALL(evt_npc_set_ry, STRING("手下１"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("手下１"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_flag_onoff, 0, STRING("手下１"), NPC_FLAG_INSTANT_TURN)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_269"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 60, 90, 283, 60, 38, -12, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_270"), 0, STRING("手下１"))
    CALL(evt_cam3d_evt_set, 135, 170, 478, 135, 84, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_271"), 0, STRING("シュリョー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_272"), 0, STRING("手下１"))
    CALL(evt_cam3d_evt_set, 200, 130, 283, 200, 78, -13, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("peach_aji_273"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 135, 170, 478, 135, 84, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_274"), 0, STRING("手下１"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_275"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 190, 150, 354, 190, 75, -13, 1200, 11)
    CALL(evt_npc_move_position, STRING("手下１"), 115, 0, 0, FLOAT(95.0f), 4)
    CALL(evt_npc_get_position, STRING("手下１"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("手下１"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    LOOP(5)
        ADD(LW(0), 2)
        ADD(LW(1), 2)
        CALL(evt_npc_set_position, STRING("手下１"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_position, STRING("手下１"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("手下１"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("手下１"), 145, -5, 0, FLOAT(95.0f), 4)
    CALL(evt_npc_get_position, STRING("手下１"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("手下１"), NPC_FLAG_NO_WALL_COLLISION)
    LOOP(5)
        ADD(LW(0), 2)
        ADD(LW(1), 2)
        CALL(evt_npc_set_position, STRING("手下１"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_position, STRING("手下１"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("手下１"), 165, -5, 0, FLOAT(95.0f), 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_276"), 0, STRING("手下１"))
    CALL(evt_npc_status_onoff, 0, STRING("手下１"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("手下１"), 160, -5, 300, 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_277"), 0, STRING("シュリョー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_278"), 0, STRING("手下１"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_279"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 0, 273, 972, 0, 100, -13, 3000, 11)
    WAIT_MS(1500)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("aji_16"), STRING("peach_evt_stg6"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(eff_test)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_fade_tec_onoff, 1, 0)
    WAIT_MS(1000)
    CALL(evt_eff64, STRING(""), STRING("akari_charge_n64"), 0, -300, 95, 200, FLOAT(2.5f), 48, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_fade_tec_onoff, 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(teshita1_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(teshita2_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(teshita3_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

EVT_BEGIN(kanbu_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(syuryo_init)
    CALL_CHILD(PTR(npc_init_evt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 167)
        CALL(evt_npc_set_position, STRING("me"), -100, 0, 0)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, PTR(peach_evt_stg3_syuryo_talk))
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"手下１"*/"\x8E\xE8\x89\xBA\x82\x50", // name
        0, 0, // flags..reactionFlags
        teshita1_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手下２"*/"\x8E\xE8\x89\xBA\x82\x51", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        teshita2_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手下３"*/"\x8E\xE8\x89\xBA\x82\x52", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        teshita3_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カンブー"*/"\x83\x4A\x83\x93\x83\x75\x81\x5B", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        kanbu_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"シュリョー"*/"\x83\x56\x83\x85\x83\x8A\x83\x87\x81\x5B", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        syuryo_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ビビアン"*/"\x83\x72\x83\x72\x83\x41\x83\x93", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(aji_10_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, 352)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI5"))
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI5"))
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_LESS(GSW_STORY_PROGRESS, 352)
        DEBUG_PUT_MSG(STRING("npc setup"))
        CALL(evt_npc_setup, PTR(npcEnt))
        CALL(evt_npc_check_delete, STRING("バリアーンカスタム"))
        CALL(evt_shuryolight_init, STRING("シュリョー"))
        CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_normal), PTR(shuryolight_head_normal), PTR(shuryolight_tue_normal))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(56)
        CALL(evt_mario_set_mode, 1)
        CALL_CHILD(PTR(monitor_init))
        SPAWN_EVENT(PTR(peach_evt_stg1))
        CASE_EQUAL(112)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_npc_entry, STRING("研究員"), STRING("c_zako_m"))
        CALL(evt_npc_set_tribe, STRING("研究員"), STRING("第三勢力研究員"))
        CALL(evt_npc_set_stay_anim, STRING("研究員"))
        CALL(evt_npc_flag_onoff, 1, STRING("研究員"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        SPAWN_EVENT(PTR(peach_evt_stg2))
        CASE_EQUAL(164)
        CALL(evt_mario_set_mode, 1)
        SPAWN_EVENT(PTR(peach_evt_stg3))
        CASE_EQUAL(167)
        CALL(evt_mario_set_mode, 1)
        CASE_EQUAL(214)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_npc_entry, STRING("研究員"), STRING("c_zako_m"))
        CALL(evt_npc_set_tribe, STRING("研究員"), STRING("第三勢力研究員"))
        CALL(evt_npc_set_stay_anim, STRING("研究員"))
        CALL(evt_npc_flag_onoff, 1, STRING("研究員"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        SPAWN_EVENT(PTR(peach_evt_stg4))
        CASE_EQUAL(261)
        CALL(evt_mario_set_mode, 1)
        CALL_CHILD(PTR(monitor_init))
        SPAWN_EVENT(PTR(peach_evt_stg5))
        CASE_BETWEEN(263, 264)
        CALL(evt_mario_set_mode, 1)
        CALL(peach_evt_toumei)
        CALL(evt_npc_entry, STRING("みはり"), STRING("c_baria_c"))
        CALL(evt_npc_set_tribe, STRING("みはり"), STRING("バリアーンカスタム"))
        CALL(evt_npc_set_position, STRING("みはり"), 370, 60, -65)
        CALL(evt_npc_set_ry, STRING("みはり"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("みはり"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        BEGIN_INLINE_EVENT
            CALL(evt_bero_exec_wait, 65536)
            SET_VALUE(LW(0), STRING("e_bero"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
            SET_VALUE(LW(0), STRING("e_bero"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_INLINE_EVENT
        CASE_EQUAL(339)
        CALL(evt_mario_set_mode, 1)
        SPAWN_EVENT(PTR(peach_evt_stg6))
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("comp"), 1, 0)
    CALL(evt_map_playanim, STRING("kabe"), 1, 0)
    CALL(evt_map_playanim, STRING("sikaku"), 1, 0)
    CALL(evt_map_playanim, STRING("mozaiku"), 1, 0)
    CALL(evt_map_playanim, STRING("moji"), 1, 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        CALL(evt_npc_check_delete, STRING("バリアーンカスタム"))
    END_IF
    SET_VALUE(GSWF(4195), 1)
    IF_INT_EQUAL(GSWF(4195), 0)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_kill_party, 0)
        IF_INT_EQUAL(GSWF(4204), 1)
            CALL(evt_peach_transform_gundan_on)
        END_IF
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 167)
            SET_VALUE(LW(0), STRING("e_bero"))
            CALL_CHILD(PTR(bero_case_switch_off))
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 263)
            IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 264)
                SET_VALUE(LW(0), STRING("e_bero"))
                CALL_CHILD(PTR(bero_case_switch_on))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()
