#include "battle/battle_unit.h"
#include "eff/n64/eff_guruguru_n64.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/npcdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_syuryou.h"
#include "driver/camdrv.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "manager/evtmgr_cmd.h"
#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "las.h"
#include "driver/seqdrv.h"

static char name[8];

static USER_FUNC(eff_guruguru);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_shuryolight_init);
USER_FUNC(evt_shuryolight_run_evt);
USER_FUNC(evt_shuryolight_run_head_evt);
USER_FUNC(evt_snd_envoff_f);
static USER_FUNC(make_name);
USER_FUNC(syuryo_baria);
USER_FUNC(syuryo_baria_recalc_pos);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord syuryo_init[30];
EvtWord koopa_init[7];
EvtWord koopa_talk[8];
static EvtWord baba_init[7];
static EvtWord baba_talk[8];
static NpcSetupInfo npcEnt[5];
static EvtWord first_evt[170];
EvtWord syuryo_thunder[419];
EvtWord syuryo_evt[1254];
EvtWord gareki_fall[45];
EvtWord koopa_evt[1671];
static EvtWord shuryolight_init[18];
static EvtWord shan_init[46];
EvtWord las_28_init_evt[238];

USER_FUNC(syuryo_baria) {
    // ピーチ単
    NpcEntry* npc = npcNameToPtr("\x83\x73\x81\x5B\x83\x60\x95\x50");
    EffectEntry* eff = effSyuryouEntry(1, 1000, npc->position.x, npc->position.y - 8.0f, 10.0f + npc->position.z);

    ((EffSyuryouWork*)eff->userdata)->scale = 2.0f;
    effSetName(eff, "baria");
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_27", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
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
        "las_30", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(syuryo_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_14_LOCK)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG8_14_01_QUEENS_CHAMBER)
            CALL(evt_npc_set_position, STRING("me"), 410, 25, 0)
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(koopa_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

EVT_BEGIN(koopa_talk)
    CALL(evt_msg_print, 0, STRING("stg8_las_107"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(baba_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(baba_talk)
    CALL(evt_msg_print, 0, STRING("stg8_las_107_01"), 0, STRING("me"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"シュリョー"*/"\x83\x56\x83\x85\x83\x8A\x83\x87\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        syuryo_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クッパ"*/"\x83\x4E\x83\x62\x83\x70", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        koopa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        baba_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(first_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 314, 606, 1081, -353, 240, -148, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, -620, -124, 0)
    CALL(evt_party_set_pos, 0, -640, -124, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 332, 200, 1115, -352, 0, -148, 3200, EASING_LINEAR)
    WAIT_MS(1800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
            CALL_CHILD(PTR(w_door_open))
            BREAK_LOOP
            ELSE
            CALL_CHILD(PTR(w_door_open))
        END_IF
    END_LOOP
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_mario_mov_pos2, -500, 0, FLOAT(150.0f))
    CALL(evt_party_run, 0)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            CALL_CHILD(PTR(w_door_close))
            BREAK_LOOP
            ELSE
            CALL_CHILD(PTR(w_door_close))
        END_IF
    END_LOOP
    WAIT_MS(200)
    CALL(evt_party_stop, 0)
    CALL(evt_cam3d_evt_set, -525, 18, 428, -525, -75, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_50"))
    SPAWN_EVENT(PTR(syuryo_evt))
    SET_VALUE(GSW_STORY_PROGRESS, STG8_14_01_QUEENS_CHAMBER)
    CALL_CHILD(PTR(bero_case_entry))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

USER_FUNC(syuryo_baria_recalc_pos) {
    // ピーチ単
    NpcEntry* npc = npcNameToPtr("\x83\x73\x81\x5B\x83\x60\x95\x50");
    static const Vec base = {0.0f, -8.0f, 10.0f};
    Vec offset = base;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    EffectEntry* eff = effNameToPtr("baria");
    Mtx mtx;
    f32 angle;

    angle = angleABf(cam->cameraPos.x, cam->cameraPos.z, npc->position.x, npc->position.z);
    PSMTXRotRad(mtx, 'Y', 0.017453292f * -angle);
    PSMTXMultVec(mtx, &offset, &offset);
    ((EffSyuryouWork*)eff->userdata)->position.x = npc->position.x + offset.x;
    ((EffSyuryouWork*)eff->userdata)->position.y = npc->position.y + offset.y;
    ((EffSyuryouWork*)eff->userdata)->position.z = npc->position.z + offset.z;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(syuryo_thunder)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 6, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(5), FLOAT(1.0f), 6, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    ADD(LW(0), LW(3))
    ADD(LW(1), LW(4))
    ADD(LW(2), LW(5))
    DIVIDE(LW(0), 2)
    DIVIDE(LW(1), 2)
    DIVIDE(LW(2), 2)
    CALL(evt_eff64, 0, STRING("thunder_n64"), 5, LW(0), LW(1), LW(2), FLOAT(20.0f), FLOAT(37.0f), 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_SHURYO_THUNDER1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.059570313f), 500)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_5"))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_D_5"))
            CASE_EQUAL(4)
            CALL(evt_party_set_pose, 0, STRING("PYS_D_5"))
            CASE_EQUAL(6)
            CALL(evt_party_set_pose, 0, STRING("PTR_D_4"))
            CASE_EQUAL(3)
            CALL(evt_party_set_pose, 0, STRING("D_4"))
            CASE_EQUAL(5)
            CALL(evt_party_set_pose, 0, STRING("PWD_D_5"))
            CASE_EQUAL(7)
            CALL(evt_party_set_pose, 0, STRING("PCH_D_4"))
        END_SWITCH
        WAIT_MS(100)
        CALL(evt_party_get_dispdir, 0, LW(0))
        LOOP(90)
            CALL(evt_party_set_dispdir, 0, LW(0))
            ADD(LW(0), 12)
            IF_INT_GREATER_OR_EQUAL(LW(0), 360)
                SUBTRACT(LW(0), 360)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_3"))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_D_3"))
            CASE_EQUAL(4)
            CALL(evt_party_set_pose, 0, STRING("PYS_D_3"))
            CASE_EQUAL(6)
            CALL(evt_party_set_pose, 0, STRING("PTR_D_3"))
            CASE_EQUAL(3)
            CALL(evt_party_set_pose, 0, STRING("D_3"))
            CASE_EQUAL(5)
            CALL(evt_party_set_pose, 0, STRING("PWD_D_3"))
            CASE_EQUAL(7)
            CALL(evt_party_set_pose, 0, STRING("PCH_D_3"))
        END_SWITCH
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_eff, 0, STRING("kemuri"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(500)
        ADD(LW(1), 20)
        LOOP(3)
            CALL(evt_sub_random, 20, LW(3))
            SUBTRACT(LW(3), 10)
            ADD(LW(3), LW(0))
            CALL(evt_eff64, 0, STRING("kemuri12_n64"), 0, LW(3), LW(1), LW(2), FLOAT(1.0f), 18, 0, 0, 0, 0, 0, 0)
            WAIT_MS(400)
        END_LOOP
        WAIT_MS(500)
        CALL(evt_party_set_dir, 0, 90, -1)
        CALL(evt_party_cont_onoff, 0, 1)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pose, STRING("M_D_1"))
    CALL(evt_mario_get_dispdir, LW(0))
    LOOP(90)
        CALL(evt_mario_set_dispdir, LW(0))
        ADD(LW(0), 12)
        IF_INT_GREATER_OR_EQUAL(LW(0), 360)
            SUBTRACT(LW(0), 360)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_adjust_dir)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_D_2"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        ADD(LW(1), 20)
        LOOP(3)
            CALL(evt_sub_random, 20, LW(3))
            SUBTRACT(LW(3), 10)
            ADD(LW(3), LW(0))
            CALL(evt_eff64, 0, STRING("kemuri12_n64"), 0, LW(3), LW(1), LW(2), FLOAT(1.0f), 18, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_SHURYO_THUNDER2"), LW(3), LW(1), LW(2), 0)
            WAIT_MS(400)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(500)
    RETURN
EVT_END()

EVT_BEGIN(syuryo_evt)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_check_landon, LW(3))
        IF_INT_GREATER_OR_EQUAL(LW(0), -160)
            IF_INT_GREATER_OR_EQUAL(LW(1), 0)
                IF_INT_EQUAL(LW(3), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 30)
    ADD(LW(2), 25)
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(4), 0)
        CALL(evt_party_move_pos, 0, LW(0), LW(2), 600)
        ELSE
        CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 600, 0, -1000)
    END_IF
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_51"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -777, 37, 200, 400, 125, -16, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_3RDHEAD1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_52"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 8, 80, 62, 401, 80, -10, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_53"), 0, STRING("シュリョー"))
    CALL(evt_mario_set_pos, -150, 0, 0)
    CALL(evt_party_set_pos, 0, -180, 0, 25)
    CALL(evt_cam3d_evt_set, -160, 84, 329, -160, 35, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_54"))
    CALL(evt_cam3d_evt_set, 12, 140, 55, 400, 80, -16, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_55"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -298, 115, 55, 90, 55, -16, 5000, EASING_LINEAR)
    CALL(evt_npc_move_position, STRING("シュリョー"), 100, 0, 0, FLOAT(60.0f), 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_56"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -160, 77, 279, -160, 35, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg8_las_57"))
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 90, 85, 305, 90, 50, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_58"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 0, 270, 1072, 0, 120, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_60"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -160, 67, 230, -160, 32, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print_party, STRING("stg8_las_61"))
    CALL(evt_cam3d_evt_set, 90, 85, 305, 90, 50, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_62"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -160, 67, 230, -160, 32, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_63"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 80)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 80)
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, 95, 75, 230, 95, 50, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, -100, 0, 0)
    CALL(evt_party_set_pos, 0, -150, 25, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_64"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 0, 270, 1072, 0, 120, -18, 1250, EASING_QUADRATIC_IN)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -40, 25, FLOAT(120.0f))
        CALL(evt_party_set_breed_pose, 0, 3)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 10, 0, FLOAT(120.0f))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_R_1"))
    WAIT_WHILE(GF(0))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_npc_set_battle_info, STRING("シュリョー"), 15)
    CALL(evt_npc_battle_start, STRING("シュリョー"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_tenmetu), PTR(shuryolight_head_off), PTR(shuryolight_tue_normal))
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -75, 0, 0)
    CALL(evt_party_set_pos, 0, -100, 0, 25)
    CALL(evt_npc_set_position, STRING("シュリョー"), 80, 0, 0)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_3"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_cam3d_evt_set, 0, 137, 530, 0, 60, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("シュリョー"), STRING("S_3"), STRING("T_4"))
    CALL(evt_msg_print, 0, STRING("stg8_las_70"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -80, 73, 254, -80, 35, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_71"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_72"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, 415, 105, 379, 415, 62, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_3RDHEAD1"))
    CALL(evt_paper_entry, STRING("OFF_hei_07"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_hei_07"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 152, 88, 304, 304)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_PEACH_APPEAR1"), 420, 25, 0, 0)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 420, 25, 0)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_6F"))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(syuryo_baria)
        CALL(evt_npc_get_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_SRYO_LIGHT_ROUND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_hei_07"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_A_4F"))
    CALL(evt_npc_wait_anim, STRING("ピーチ姫"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_3F"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_3F"), STRING("P_T_3F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_73"), 0, STRING("ピーチ姫"))
    SET_VALUE(LW(0), 0)
    LOOP(2)
        LOOP(0)
            ADD(LW(0), 12)
            IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                SET_VALUE(LW(0), 180)
                CALL(evt_npc_set_rotate, STRING("ピーチ姫"), 0, LW(0), 0)
                BREAK_LOOP
                ELSE
                CALL(evt_npc_set_rotate, STRING("ピーチ姫"), 0, LW(0), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(150)
        LOOP(0)
            SUBTRACT(LW(0), 12)
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                SET_VALUE(LW(0), 0)
                CALL(evt_npc_set_rotate, STRING("ピーチ姫"), 0, LW(0), 0)
                BREAK_LOOP
                ELSE
                CALL(evt_npc_set_rotate, STRING("ピーチ姫"), 0, LW(0), 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(150)
    END_LOOP
    CALL(evt_npc_get_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_B_1F"))
    ADD(LW(1), 48)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, 0, 0, 0, 0, LW(0), LW(1), LW(2), 0, 0, 60)
    WAIT_MS(2000)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_4F"), STRING("P_T_4F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_74"), 0, STRING("ピーチ姫"))
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -80, 73, 254, -80, 35, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_75"))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_5F"))
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 55)
    CALL(evt_cam3d_evt_set, -600, 50, 350, 15, 80, 16, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_set_dir, 0, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    WAIT_FRAMES(1)
    CALL(syuryo_baria_recalc_pos)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_76"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -75, 95, 328, -75, 45, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 1)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_77"))
    CALL(evt_cam3d_evt_set, 60, 95, 328, 60, 45, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg8_las_78"), 0, STRING("シュリョー"))
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_tenmetu))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3A"))
    WAIT_MS(170)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3B"))
    WAIT_MS(250)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3C"))
    CALL(evt_cam3d_evt_set, -15, 185, 675, -15, 85, -18, 500, EASING_SINE_IN_OUT)
    CALL(evt_env_blur_on, 0, 500)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_3"))
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_off))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(syuryo_thunder))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 410, 100, 230, 410, 65, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(syuryo_baria_recalc_pos)
    CALL(evt_msg_print, 0, STRING("stg8_las_79"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 75, 86, 230, 75, 50, -18, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_80"), 0, STRING("シュリョー"))
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_tenmetu))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3A"))
    WAIT_MS(170)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3B"))
    WAIT_MS(250)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_S_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_S_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("S_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_S_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_S_1"))
    END_SWITCH
    CALL(evt_cam3d_evt_set, -80, 102, 378, -80, 45, -18, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    WAIT_MS(200)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(syuryo_thunder))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_4F"))
    CALL(evt_cam3d_evt_set, 410, 100, 230, 410, 65, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(syuryo_baria_recalc_pos)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_81"), 0, STRING("ピーチ姫"))
    CALL(evt_mario_set_pose, STRING("M_S_3"))
    CALL(evt_cam3d_evt_set, -85, 65, 230, -85, 30, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_82"))
    CALL(evt_msg_select, 0, STRING("stg8_las_82_yesno"))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_3"))
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_off))
    CALL(evt_cam3d_evt_set, 70, 86, 230, 70, 50, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("stg8_las_83"), 0, STRING("シュリョー"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg8_las_84"), 0, STRING("シュリョー"))
        CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
        WAIT_MS(300)
    END_IF
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_tenmetu))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3A"))
    WAIT_MS(170)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3B"))
    WAIT_MS(250)
    CALL(evt_party_run, 0)
    SPAWN_EVENT(PTR(koopa_evt))
    RETURN
EVT_END()


static USER_FUNC(eff_guruguru) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 w = evtGetFloat(event, args[4]);
    EffectEntry* eff = effGuruguruN64Entry(0, 3, x, y, z, w);

    ((EffGuruguruN64Work*)eff->userdata)->scale = 0.8f;
    effSetName(eff, name);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(gareki_fall)
    SET_VALUE(LW(7), LW(0))
    SET_VALUE(LW(8), LW(1))
    SET_VALUE(LW(9), LW(2))
    SET_VALUE(LW(10), LW(3))
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(7), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 350, 0, LW(10))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, LW(7), LW(8), LW(0), LW(9))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(koopa_evt)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_QUAKE1"), 75, 300, 0, LW(0))
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 1500)
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_3"))
    CALL(evt_shuryolight_run_head_evt, PTR(shuryolight_head_off))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_CEILING_BREAK1"), 75, 300, 0, LW(0))
    CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 75, 300, 0, 90, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_snd_sfxoff, LW(0))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 75, 187, 675, 75, 90, -18, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("シュリョー"), STRING("S_3"), STRING("T_4"))
    CALL(evt_msg_print, 0, STRING("stg8_las_85"), 0, STRING("シュリョー"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_QUAKE1"), 75, 300, 0, LW(0))
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.029296875f), 1500)
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_CEILING_BREAK1"), 75, 300, 0, LW(0))
    CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 75, 300, 0, 72, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1200)
        CALL(evt_snd_sfxoff, LW(0))
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(2), LW(3), LW(4))
    ADD(LW(3), 300)
    CALL(evt_npc_set_position, STRING("クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_npc_set_ry, STRING("クッパ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クッパ"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print, 0, STRING("stg8_las_86"), 0, STRING("クッパ"))
    CALL(evt_npc_set_anim, STRING("クッパ"), STRING("KPA_D_1"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_CEILING_BREAK2"), LW(2), LW(3), LW(4), 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        SET_VALUE(LW(0), STRING("S_gareki1"))
        SET_VALUE(LW(1), 50)
        SET_VALUE(LW(2), -80)
        SET_VALUE(LW(3), 880)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(50)
        SET_VALUE(LW(0), STRING("S_gareki8"))
        SET_VALUE(LW(1), 70)
        SET_VALUE(LW(2), -140)
        SET_VALUE(LW(3), 880)
        SPAWN_EVENT(PTR(gareki_fall))
        SET_VALUE(LW(0), STRING("S_gareki4"))
        SET_VALUE(LW(1), -80)
        SET_VALUE(LW(2), -10)
        SET_VALUE(LW(3), 680)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(50)
        SET_VALUE(LW(0), STRING("S_gareki9"))
        SET_VALUE(LW(1), 30)
        SET_VALUE(LW(2), 50)
        SET_VALUE(LW(3), 880)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(100)
        SET_VALUE(LW(0), STRING("S_gareki5"))
        SET_VALUE(LW(1), -150)
        SET_VALUE(LW(2), -80)
        SET_VALUE(LW(3), 680)
        SPAWN_EVENT(PTR(gareki_fall))
        SET_VALUE(LW(0), STRING("S_gareki7"))
        SET_VALUE(LW(1), 0)
        SET_VALUE(LW(2), -140)
        SET_VALUE(LW(3), 800)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(100)
        SET_VALUE(LW(0), STRING("S_gareki10"))
        SET_VALUE(LW(1), -40)
        SET_VALUE(LW(2), 60)
        SET_VALUE(LW(3), 800)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(50)
        SET_VALUE(LW(0), STRING("S_gareki6"))
        SET_VALUE(LW(1), -80)
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 800)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(50)
        SET_VALUE(LW(0), STRING("S_gareki3"))
        SET_VALUE(LW(1), 0)
        SET_VALUE(LW(2), -10)
        SET_VALUE(LW(3), 800)
        SPAWN_EVENT(PTR(gareki_fall))
        WAIT_MS(100)
        SET_VALUE(LW(0), STRING("S_gareki2"))
        SET_VALUE(LW(1), 80)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(3), 680)
        SPAWN_EVENT(PTR(gareki_fall))
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, LW(3), 40, 800)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("クッパ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.029296875f), FLOAT(0.0f), 1000)
    END_INLINE_EVENT
    CALL(evt_eff, 0, STRING("hit"), 0, LW(2), LW(0), LW(4), FLOAT(1.0f), 6, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_STRIKE1"), LW(2), LW(0), LW(4), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_STRIKE2"), LW(2), LW(0), LW(4), 0)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("D_4"))
    SET_VALUE(GW(0), LW(0))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(5), LW(0))
        ADD(LW(5), 30)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(0), LW(5), 200)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(5), 0, 300)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    SET_VALUE(LW(5), LW(4))
    ADD(LW(5), 25)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(4), LW(5), 600)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("クッパ"), LW(2), GW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SATIATED2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.01953125f), FLOAT(0.0f), 1000)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    CALL(evt_eff64, 0, STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("クッパ"), STRING("KPA_D_2"))
    CALL(evt_npc_pera_onoff, STRING("クッパ"), 0)
    CALL(evt_npc_set_ry, STRING("クッパ"), 0)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 50)
    ADD(LW(1), 60)
    CALL(eff_guruguru, STRING("koopa_piyo"), LW(0), LW(1), LW(2), 20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_PIYO1"), LW(0), LW(1), LW(2), LW(15))
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 0)
    WAIT_MS(500)
    SET_VALUE(LW(0), 30)
    LOOP(10)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gare"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(LW(0))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gare"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(LW(0))
        SUBTRACT(LW(0), 2)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gare"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 65, 95, 329, 65, 45, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("クッパ"), STRING("KPA_Z_3"), STRING("KPA_T_8"))
    CALL(evt_msg_print, 0, STRING("stg8_las_87"), 0, STRING("クッパ"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -210, 300, 619, 223, 65, -12, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(syuryo_baria_recalc_pos)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_88"), 0, STRING("ピーチ姫"))
    CALL(evt_cam3d_evt_set, 75, 110, 403, 75, 50, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 1)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_pera_onoff, STRING("クッパ"), 1)
    CALL(evt_npc_set_ry, STRING("クッパ"), 270)
    CALL(evt_msg_print, 0, STRING("stg8_las_89"), 0, STRING("クッパ"))
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    ADD(LW(1), 200)
    ADD(LW(2), 50)
    CALL(evt_npc_set_position, STRING("ババ"), 120, LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_90"), 0, STRING("ババ"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_kemuri2, STRING("ババ"), 3000, 0)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ババ"), LW(2), LW(3), LW(4))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(3), 25, 3000)
        LABEL(4)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("ババ"), LW(2), LW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(4)
        END_IF
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_eff_softdelete, STRING("koopa_piyo"))
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, 0, STRING("クッパ"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(600)
    CALL(evt_npc_flag_onoff, 1, STRING("クッパ"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_autotalkpose, STRING("クッパ"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("クッパ"), LW(2), LW(3), LW(4))
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 20, 200)
        LABEL(5)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("クッパ"), LW(2), LW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(5)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(0), 0, 200)
        LABEL(6)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("クッパ"), LW(2), LW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(6)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("クッパ"), STRING("KPA_U_1"))
    WAIT_MS(600)
    CALL(evt_cam3d_evt_set, -85, 84, 330, -85, 35, -18, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 75, 120, 403, 75, 60, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_91"), 0, STRING("クッパ"))
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    WAIT_MS(800)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, 0, STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_get_position, STRING("ババ"), LW(2), LW(3), LW(4))
    SET_VALUE(LW(5), LW(3))
    ADD(LW(5), 5)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(3), LW(5), 100)
    LABEL(7)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("ババ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(7)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(5), LW(3), 100)
    LABEL(8)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("ババ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(8)
    END_IF
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_92"), 0, STRING("ババ"))
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    CALL(evt_cam3d_evt_set, 65, 107, 279, 65, 65, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg8_las_93"), 0, STRING("クッパ"))
    CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 10)
    CALL(evt_npc_move_position, STRING("クッパ"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -80, 77, 279, -80, 35, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_94"))
    CALL(evt_cam3d_evt_set, 65, 107, 279, 65, 65, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_95"), 0, STRING("クッパ"))
    CALL(evt_cam3d_evt_set, 0, 270, 1072, 0, 120, -18, 400, EASING_QUADRATIC_IN)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("クッパ"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 40)
        CALL(evt_npc_move_position, STRING("クッパ"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 40)
        CALL(evt_npc_kamek_move_position, STRING("ババ"), LW(0), LW(2), 5, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_npc_set_battle_info, STRING("クッパ"), 13)
    CALL(evt_npc_battle_start, STRING("クッパ"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_eff_delete, STRING("baria"))
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -30, 0, 0)
    CALL(evt_party_set_pos, 0, -70, 0, 20)
    CALL(evt_npc_set_position, STRING("クッパ"), 40, 0, -10)
    CALL(evt_npc_set_width, STRING("クッパ"), 105)
    CALL(evt_npc_set_height, STRING("クッパ"), 55)
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（地上）"))
    CALL(evt_npc_set_position, STRING("ババ"), 70, 0, 20)
    CALL(evt_npc_set_width, STRING("ババ"), 45)
    CALL(evt_npc_set_height, STRING("ババ"), 30)
    CALL(evt_npc_set_anim, STRING("クッパ"), STRING("KPA_Z_3"))
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("D_2"))
    CALL(evt_npc_set_autotalkpose, STRING("クッパ"), STRING("KPA_Z_3"), STRING("KPA_T_8"))
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 650, 25, 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 650, 25, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_NO_DRAW)
    CALL(eff_guruguru, STRING("koopa_piyo"), -10, 60, -10, 20)
    CALL(eff_guruguru, STRING("baba_piyo"), 70, 30, 20, 20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KOOPA_PIYO1"), -10, 60, -10, 0)
    CALL(evt_cam3d_evt_set, 0, 132, 530, 0, 55, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_101"), 0, STRING("クッパ"))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print_party, STRING("stg8_las_102"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -50, 84, 330, -50, 35, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("stg8_las_103"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_cam3d_evt_set, 100, 84, 330, 100, 35, -18, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -40, 65, 230, -40, 30, -18, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(750)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER2"))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg8_las_104"), 0, STRING("ピーチ姫"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_105"))
    CALL(evt_mapobj_trans, 1, STRING("S_gyoku"), -100, 0, 0)
    CALL(evt_hit_bind_update, STRING("A_gyoku"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_i02_f2_isu_04C"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_isu_tao"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_i02_f2_isu_04C1"), 1, 1)
    CALL(evt_npc_pera_onoff, STRING("クッパ"), 0)
    CALL(evt_npc_pera_onoff, STRING("ババ"), 0)
    CALL(evt_npc_set_ry, STRING("クッパ"), 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 0)
    CALL(evt_npc_set_position, STRING("シュリョー"), 600, 25, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), 1073750016)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_tribe, STRING("シュリョー"), STRING("第三勢力総統（ヒビ）"))
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_move_position, STRING("シュリョー"), 700, 0, 0, 150, 0)
        CALL(evt_npc_set_position, STRING("シュリョー"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 30, 80, 173, 500, 60, 5, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(2000)
    CALL(evt_npc_pera_onoff, STRING("クッパ"), 1)
    CALL(evt_npc_pera_onoff, STRING("ババ"), 1)
    CALL(evt_npc_set_ry, STRING("クッパ"), 270)
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("クッパ"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("クッパ"), NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_change_interrupt, STRING("クッパ"), 6, PTR(koopa_talk))
    CALL(evt_npc_change_interrupt, STRING("ババ"), 6, PTR(baba_talk))
    SET_VALUE(GSW_STORY_PROGRESS, STG8_15_GRODUS)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(shuryolight_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_14_LOCK)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG8_15_GRODUS)
            CALL(evt_shuryolight_init, STRING("シュリョー"))
            CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_normal), PTR(shuryolight_head_normal), PTR(shuryolight_tue_normal))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(20)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(9.0f))
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.69921875f), 1000, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(las_28_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER2"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS9"))
    END_IF
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_14_LOCK)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(shuryolight_init))
    CALL(evt_hit_bind_mapobj, STRING("A_gyoku"), STRING("S_gyoku"))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_mapobj_trans, 1, STRING("S_gyoku"), -100, 0, 0)
        CALL(evt_hit_bind_update, STRING("A_gyoku"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_i02_f2_isu_04C"), 1, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isu_tao"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_i02_f2_isu_04C1"), 1, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gare"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_gare"), 1, 0)
    CALL_CHILD(PTR(shan_init))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_14_LOCK)
        SPAWN_EVENT(PTR(first_evt))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_14_01_QUEENS_CHAMBER)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("koopa_evt"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_mario_set_pos, -75, 0, 0)
                CALL(evt_mario_set_dir, 90, 0, 0)
                CALL(evt_party_set_pos, 0, -100, 0, 25)
                CALL(evt_party_set_dir, 0, 90, 0)
                CALL(evt_npc_set_position, STRING("シュリョー"), 80, 0, 0)
                CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
                CALL(evt_npc_set_position, STRING("ピーチ姫"), 420, 25, 0)
                CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
                CALL(evt_cam3d_evt_set, 70, 86, 230, 70, 50, -18, 0, EASING_SINE_IN_OUT)
                CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_tenmetu), PTR(shuryolight_head_off), PTR(shuryolight_tue_normal))
                CALL(syuryo_baria)
                CALL(evt_seq_wait, SEQ_GAME)
                CALL(evt_mario_set_pose, STRING("M_S_3"))
                CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3B"))
                SPAWN_EVENT(PTR(koopa_evt))
            END_INLINE_EVENT
            ELSE
            SPAWN_EVENT(PTR(syuryo_evt))
        END_IF
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("from26"))
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_set_pos, 200, 300, 0)
        CALL(evt_party_set_pos, 0, 200, 300, 0)
        CALL(evt_cam_road_reset)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_mario_cont_onoff, 1)
            WAIT_MS(180)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_mario_wait_landon)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;

    sprintf(name, "hi%d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

