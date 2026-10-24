#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "stdio.h"
#include "memory.h"
#include "stdlib.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

s32* tbl;

USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_road_reset);
USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_balloon_bikkuri);
USER_FUNC(evt_mario_balloon_off);
USER_FUNC(evt_mario_cam_off);
USER_FUNC(evt_mario_cam_on);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_kururing_floor);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_switch_blue);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_set_dir_to_target);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_rumble_onoff);
USER_FUNC(evt_win_item_select);
USER_FUNC(free_tbl);
static USER_FUNC(get_pos);
USER_FUNC(iri_13_make_name);
USER_FUNC(make_tbl);

f32 fd_tou_06_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tou_06_neg_one_382;
f32 fd_tou_06___local_PI_383;
f32 fd_tou_06___local_3PIO2_385;
f32 fd_tou_06_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tou_06___local_PIO2_384;
f32 fd_tou_06_one_381;
f32 fd_tou_06_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tou_06_neg_one_365;
f32 fd_tou_06___local_PI_366;
f32 fd_tou_06___local_3PIO2_368;
f32 fd_tou_06_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tou_06___local_PIO2_367;
f32 fd_tou_06_one_364;
static EvtWord n_door_open[12];
static EvtWord n_door_close[12];
static EvtWord w_door_open[12];
static EvtWord w_door_close[12];
static BeroEntry bero_data[3];
EvtWord evt_ana[9];
EvtWord evt_kontena[21];
EvtWord evt_kaidan[91];
const char* cloud_kontena2_list[5];
EvtWord evt_kontena2[23];
static EvtWord evt_block_broken[5];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
static EvtWord evt_lock[14];
EvtWord evt_chuchu[626];
static s32 list[75];
EvtWord iri_13_pants_00_evt[5];
EvtWord iri_13_init[124];
static NpcSetupInfo npcEnt[2];
static EvtWord mobj_box00[34];
EvtWord tou_06_init_evt[544];

f32 fd_tou_06_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tou_06_neg_one_382 = -1.0f;

f32 fd_tou_06___local_PI_383 = 3.1415927f;

f32 fd_tou_06___local_3PIO2_385 = 4.712389f;

f32 fd_tou_06_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tou_06___local_PIO2_384 = 1.5707964f;

f32 fd_tou_06_one_381 = 1.0f;

f32 fd_tou_06_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tou_06_neg_one_365 = -1.0f;

f32 fd_tou_06___local_PI_366 = 3.1415927f;

f32 fd_tou_06___local_3PIO2_368 = 4.712389f;

f32 fd_tou_06_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tou_06___local_PIO2_367 = 1.5707964f;

f32 fd_tou_06_one_364 = 1.0f;

static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "n_bero", // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        "s_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {
        "w_bero", // hitName
        1, // type
        4, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_12", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_ana)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_mapchange, STRING("tou_05"), STRING("fall"))
    RETURN
EVT_END()

EVT_BEGIN(evt_kontena)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    SET_VALUE(GSW(0), 149)
    RETURN
EVT_END()

EVT_BEGIN(evt_kaidan)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, -90, 208, 680, -90, 80, -8, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 1)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        LOOP(8)
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_PERA_KAIDAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(350)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kaidan"), 1)
    CALL(evt_hitobj_onoff, STRING("a_kaidan"), 1, 1)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    SET_VALUE(GSW(0), 150)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

const char* cloud_kontena2_list[] = {
    "kontena_2",
    "fuda_1",
    "fuda_2",
    "fuda_3",
    NULL,
};

EVT_BEGIN(evt_kontena2)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_2"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_3"), 1)
    SET_VALUE(GSWF(2435), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_block_broken)
    SET_VALUE(GSW(0), 151)
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_STORAGE_KEY_0x13, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, 19, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_lock)
    SET_VALUE(GSW(0), 155)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_pouch_remove_item, 19, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_chuchu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_CYUCYURINA1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), -100, 50, -60)
    CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 270)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(200)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir_npc, STRING("チュチュリーナ"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 300)
        CALL(evt_eff_fukidashi, 3, STRING("p"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_set_dir_to_target, STRING("チュチュリーナ"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -75, 117, 224, -75, 88, -8, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg3_tou_308"), 0, STRING("チュチュリーナ"))
    WAIT_MS(300)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 16, 0, -60)
    CALL(evt_party_set_pos, 0, 40, 0, -65)
    CALL(evt_cam3d_evt_set, 4, 67, 224, 4, 38, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg3_tou_309"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -75, 117, 224, -75, 88, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_310"), 0, STRING("チュチュリーナ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 4, 67, 224, 4, 38, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg3_tou_311"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -75, 117, 224, -75, 88, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_312"), 0, STRING("チュチュリーナ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 4, 67, 224, 4, 38, -8, 1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), 32)
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), -65, -60, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -10, 0, -60, 300, 30)
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 0, -60, 0, FLOAT(80.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_313"), 0, STRING("チュチュリーナ"))
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_CHURINA_KISS1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_CHURINA_KISS2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_K_1"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), -20, -60, 0, FLOAT(80.0f), 1)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set, 25, 67, 224, 25, 38, -8, 300, 11)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_314"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 4, 67, 224, 4, 38, -8, 300, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_315"), 0, STRING("チュチュリーナ"))
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 0, -140, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("チュチュリーナ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("チュチュリーナ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
    CALL(evt_snd_bgmoff, 26624)
    CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
    CALL(evt_msg_print_party, STRING("stg3_tou_316"))
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1500)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION2"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    LOOP(0)
        IS_EVENT_RUNNING(LW(11), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_316_01"), 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_316_02"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_I_Z"))
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_pouch_receive_mail, 12)
    CALL(evt_pouch_open_mail, 12)
    SET_VALUE(GSW(0), 148)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static s32 list[] = {
    -305, 250, 130, -221, 200, 5, -235, 200,
    85, -159, 250, -14, -129, 200, 45, 80,
    200, 39, 67, 260, -25, 26, 200, 89,
    -85, 130, 126, -180, 60, 79, -72, 0,
    -72, -18, 0, -21, 61, 0, -74, 119,
    0, -35, 162, 0, 31, 111, 0, 43,
    47, 0, 37, 12, 0, -4, -56, 0,
    33, -20, 0, 97, 58, 0, 90, 116,
    0, 105, 158, 0, 112, -78, 0, 131,
    -116, 0, -111,
};

EVT_BEGIN(iri_13_pants_00_evt)
    SET_VALUE(GSWF(5408), 1)
    RETURN
EVT_END()

EVT_BEGIN(iri_13_init)
    IF_INT_EQUAL(GSWF(5406), 1)
        RETURN
    END_IF
    CALL(evt_pouch_check_item, 107, LW(0))
    SET_VALUE(LW(1), 20)
    SUBTRACT(LW(1), LW(0))
    CALL(make_tbl)
    SET_VALUE(LW(2), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        CALL(get_pos, LW(2), LW(3), LW(4), LW(5))
        CALL(iri_13_make_name, LW(2), LW(6))
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LW(2), 0)
            IF_INT_EQUAL(GSWF(5408), 0)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_item_entry, LW(6), 107, LW(3), LW(4), LW(5), 16, -1, PTR(iri_13_pants_00_evt))
            ELSE
            CALL(evt_item_entry, LW(6), 107, LW(3), LW(4), LW(5), 16, -1, 0)
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gor_hae"), 1)
        ADD(LW(2), 1)
        SUBTRACT(LW(1), 1)
    END_LOOP
    CALL(free_tbl)
    IF_INT_NOT_EQUAL(LW(2), 0)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_sub_random, 1000, LW(0))
                ADD(LW(0), 3000)
                WAIT_MS(LW(0))
                CALL(evt_mapobj_get_position, STRING("gor_hae"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_FLY1"), LW(0), LW(1), LW(2), 0)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"チュチュリーナ"*/"\x83\x60\x83\x85\x83\x60\x83\x85\x83\x8A\x81\x5B\x83\x69", // name
        0x40000600, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 257, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tou_06_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_run_case_evt, 0, 1, STRING("ana"), 0, PTR(evt_ana), 0)
    IF_INT_LESS(GSW(0), 149)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("kontena"), STRING("a_kon_H"), PTR(evt_kontena))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kontena"), 1)
        CALL(evt_hitobj_onoff, STRING("a_kon_H"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(2435), 0)
        IF_INT_GREATER_OR_EQUAL(GSW(0), 149)
            CALL(evt_cloud_init)
        END_IF
        CALL(evt_cloud_ent, 1, PTR(&cloud_kontena2_list), STRING("a_kon_N"), PTR(evt_kontena2))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kontena_2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuda_3"), 1)
        CALL(evt_hitobj_onoff, STRING("a_kon_N"), 1, 0)
    END_IF
    CALL(evt_mobj_lv_blk, STRING("i"), 126, -1, -126, 0, GSWF(2433), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("j"), 176, -1, -76, 0, GSWF(2434), STRING("MOBJ_Lv1BigBlock"))
    IF_INT_EQUAL(GSWF(2433), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("i"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2434), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("j"), 64)
    END_IF
    CALL(evt_mobj_lv_blk, STRING("a"), -126, -1, -126, 0, GSWF(2425), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("b"), -76, -1, -126, 0, GSWF(2426), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("c"), -176, -1, -76, 0, GSWF(2427), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("d"), -126, -1, -76, 0, GSWF(2428), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("e"), -76, -1, -76, 0, GSWF(2429), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("f"), -176, -1, -26, 0, GSWF(2430), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("g"), -126, -1, -26, 0, GSWF(2431), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("h"), -76, -1, -26, 0, GSWF(2432), STRING("MOBJ_Lv1BigBlock"))
    IF_INT_EQUAL(GSWF(2425), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("a"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2426), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("b"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2427), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("c"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2428), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("d"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2429), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("e"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2430), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("f"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2431), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("g"), 64)
    END_IF
    IF_INT_EQUAL(GSWF(2432), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("h"), 64)
    END_IF
    IF_INT_LESS(GSW(0), 150)
        CALL(evt_mobj_switch_blue, 0, STRING("sw"), -165, 0, -115, PTR(evt_kaidan), GSWF(2438))
        CALL(evt_mobj_flag_onoff, 0, STRING("sw"), 64)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kaidan"), 1)
        CALL(evt_hitobj_onoff, STRING("a_kaidan"), 1, 0)
        CALL(evt_paper_entry, STRING("OFF_d_ori"))
        CALL(evt_img_entry, STRING("img"))
        CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_ori"))
        CALL(evt_img_onoff, STRING("img"), 0)
    END_IF
    IF_INT_LESS(GSW(0), 151)
        CALL(evt_mobj_lv_blk, STRING("aa"), -450, 200, 55, PTR(evt_block_broken), GSWF(2439), STRING("MOBJ_Lv1BigBigBlock"))
        CALL(evt_mobj_flag_onoff, 0, STRING("aa"), 64)
    END_IF
    IF_INT_LESS(GSW(0), 155)
        CALL(evt_mobj_lock, STRING("lock"), 19, 95, 210, 50, -80, PTR(evt_chk), PTR(evt_lock), GSWF(2437))
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -30, 260, -30, 0, GSWF(5547), 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 50, 0, 0, STRING("tou_6_sita"), 125, GSWF(5626))
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("MOBJ_kururin1"), 4194304)
    CALL(evt_item_entry, STRING("item_01"), 279, -450, 250, -25, 16, GSWF(2521), 0)
    CALL(evt_item_entry, STRING("item_02"), 257, 175, 0, -125, 16, GSWF(2523), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("fall"))
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_set_pos, 125, 0, -30)
        CALL(evt_cam_road_reset)
        CALL(evt_mario_cam_off)
        CALL(evt_mario_set_pos, 125, 100, -30)
        CALL(evt_party_set_pos, 0, 125, 100, -30)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_mario_cont_onoff, 1)
            CALL(evt_mario_wait_landon)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_mario_cam_on)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_party_cont_onoff, 0, 1)
        END_INLINE_EVENT
    END_IF
    CALL(evt_map_playanim, STRING("anm_hae"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gor_hae"), 1)
    IF_INT_EQUAL(GSW(1420), 13)
        CALL_CHILD(PTR(iri_13_init))
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kaidan"), 2)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_LESS(GSW(0), 148)
        SPAWN_EVENT(PTR(evt_chuchu))
        ELSE
        IF_INT_EQUAL(GSW(0), 163)
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envoff, 512)
            ELSE
            SWITCH(GSW(0))
                CASE_BETWEEN(127, 137)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
                CALL(evt_snd_bgmoff, 26624)
                CASE_GREATER_OR_EQUAL(165)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
                CALL(evt_snd_bgmoff, 26624)
                CASE_DEFAULT
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
            END_SWITCH
        END_IF
    END_IF
    RETURN
EVT_END()

USER_FUNC(iri_13_make_name) {
    static char name[8];
    EvtWord* args = event->args;

    sprintf(name, "pants%02d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(free_tbl) {
    _mapFree(tbl);
    tbl = NULL;
    return EVT_RETURN_DONE;
}

static USER_FUNC(get_pos) {
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    s32* t = tbl;
    s32 idx = t[n];

    evtSetValue(event, args[1], list[idx * 3 + 0]);
    evtSetValue(event, args[2], list[idx * 3 + 1]);
    evtSetValue(event, args[3], list[idx * 3 + 2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(make_tbl) {
    s32 i;
    s32 a;
    s32 b;
    s32 tmp;

    tbl = _mapAlloc(100);
    for (i = 0; i < 25; i++) {
        tbl[i] = i;
    }
    for (i = 0; i < 50; i++) {
        a = rand() % 25;
        b = rand() % 25;
        tmp = tbl[a];
        tbl[a] = tbl[b];
        tbl[b] = tmp;
    }
    if (!evtGetValue(NULL, GSWF(5408))) {
        for (i = 0; i < 25; i++) {
            if (tbl[i] == 0) {
                tmp = tbl[0];
                tbl[0] = tbl[i];
                tbl[i] = tmp;
                break;
            }
        }
    }
    return EVT_RETURN_DONE;
}

