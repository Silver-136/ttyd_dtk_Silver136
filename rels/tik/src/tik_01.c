#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_movefloor.h"


USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_free_capture);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_set_offscreen);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_kururing_floor);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_damage_anim);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_run_anim);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_offscreen_delete);
USER_FUNC(evt_offscreen_entry);
USER_FUNC(evt_offscreen_get_boundingbox2);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_dir_npc);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_get_equipbadgecnt);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);

f32 fd_tik_01_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_01_neg_one_382;
f32 fd_tik_01___local_PI_383;
f32 fd_tik_01___local_3PIO2_385;
f32 fd_tik_01_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_01___local_PIO2_384;
f32 fd_tik_01_one_381;
f32 fd_tik_01_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_01_neg_one_365;
f32 fd_tik_01___local_PI_366;
f32 fd_tik_01___local_3PIO2_368;
f32 fd_tik_01_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_01___local_PIO2_367;
f32 fd_tik_01_one_364;
EvtWord lecture_badge_after_00_evt[34];
EvtWord dokan_2_in_evt[22];
static BeroEntry bero_entry_data[12];
EvtWord ashioto[35];
EvtWord gorotuki_sanninngumi[694];
EvtWord wall_bomb00[139];
EvtWord wall_bomb01[140];
static EvtWord mobj_box00[34];
EvtWord tik_01_init_evt[208];

f32 fd_tik_01_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_01_neg_one_382 = -1.0f;

f32 fd_tik_01___local_PI_383 = 3.1415927f;

f32 fd_tik_01___local_3PIO2_385 = 4.712389f;

f32 fd_tik_01_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_01___local_PIO2_384 = 1.5707964f;

f32 fd_tik_01_one_381 = 1.0f;

f32 fd_tik_01_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_01_neg_one_365 = -1.0f;

f32 fd_tik_01___local_PI_366 = 3.1415927f;

f32 fd_tik_01___local_3PIO2_368 = 4.712389f;

f32 fd_tik_01_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_01___local_PIO2_367 = 1.5707964f;

f32 fd_tik_01_one_364 = 1.0f;

EVT_BEGIN(lecture_badge_after_00_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, -375, -105, FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_msg_print, 0, STRING("lecture_badge_after_00"), 0, STRING("party"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dokan_2_in_evt)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 20)
        IF_INT_LESS(GSW(0), 21)
            CALL(evt_pouch_get_equipbadgecnt, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                SPAWN_EVENT(PTR(lecture_badge_after_00_evt))
            END_IF
        END_IF
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_02", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_02", // nextMapName
        "w_bwro_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)dokan_2_in_evt, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "gor_02", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_5", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_4", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_6", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_5", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_6", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        4096, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "tik_11", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_dero_2", // hitName
        4096, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "tik_12", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(ashioto)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_KURI_MOVE1L"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(50)
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_KURI_MOVE1R"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(50)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(gorotuki_sanninngumi)
    CALL(evt_npc_set_position, STRING("クリボー１"), 110, 75, 65)
    CALL(evt_npc_set_position, STRING("クリボー２"), 140, 95, 35)
    CALL(evt_npc_set_position, STRING("クリボー３"), 155, 75, 85)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), 1073742368)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー２"), 1073742368)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー３"), 1073742368)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -100)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("mac_0_086"), 0, STRING("クリボー１"))
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_dir_npc, STRING("クリボー１"))
    CALL(evt_party_set_dir_npc, 0, STRING("クリボー１"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, 125, 148, 360, 125, 110, 37, 500, 11)
    WAIT_MS(400)
    SET_VALUE(LW(0), -100)
    SET_VALUE(LW(2), 70)
    CALL(evt_mario_set_pos, LW(0), 0, LW(2))
    SUBTRACT(LW(0), 35)
    CALL(evt_party_set_pos, 0, LW(0), 0, LW(2))
    SUBTRACT(LW(0), 35)
    CALL(evt_party_set_pos, 1, LW(0), 0, LW(2))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_msg_print, 0, STRING("mac_0_087"), 0, STRING("クリボー２"))
    CALL(evt_msg_print, 0, STRING("mac_0_088"), 0, STRING("クリボー３"))
    CALL(evt_cam3d_evt_set, -140, 65, 310, -140, 33, 37, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_089"))
    CALL(evt_cam3d_evt_set, 125, 148, 360, 125, 110, 37, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_090"), 0, STRING("クリボー１"))
    CALL(evt_msg_print, 0, STRING("mac_0_091"), 0, STRING("クリボー２"))
    CALL(evt_msg_print, 0, STRING("mac_0_092"), 0, STRING("クリボー３"))
    CALL(evt_cam3d_evt_off, 500, 11)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        SPAWN_EVENT_GET_ID(PTR(ashioto), LW(10))
        CALL(evt_npc_set_run_anim, STRING("クリボー１"))
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("クリボー１"), 50, LW(2), 0, FLOAT(150.0f), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_KURI_JUMP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー１"), -30, 0, LW(2), 350, 45)
        SPAWN_EVENT_GET_ID(PTR(ashioto), GW(0))
        CALL(evt_npc_move_position, STRING("クリボー１"), -90, LW(2), 0, FLOAT(150.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_npc_set_run_anim, STRING("クリボー２"))
        CALL(evt_npc_get_position, STRING("クリボー２"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("クリボー２"), 50, LW(2), 0, FLOAT(200.0f), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー２"), -30, 20, LW(2), 350, 45)
        CALL(evt_npc_move_position, STRING("クリボー２"), -80, LW(2), 0, FLOAT(150.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(12))
        CALL(evt_npc_set_run_anim, STRING("クリボー３"))
        CALL(evt_npc_get_position, STRING("クリボー３"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("クリボー３"), 50, LW(2), 0, FLOAT(250.0f), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー３"), -30, 0, LW(2), 350, 45)
        CALL(evt_npc_move_position, STRING("クリボー３"), -80, LW(2), 0, FLOAT(150.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(350)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    WAIT_MS(530)
    DELETE_EVENT(GW(0))
    CALL(evt_npc_set_battle_info, STRING("クリボー１"), 4)
    CALL(evt_npc_battle_start, STRING("クリボー１"))
    CALL(evt_npc_wait_battle_end)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    DELETE_EVENT(LW(12))
    CALL(evt_npc_set_position, STRING("クリボー１"), -65, 0, 65)
    CALL(evt_npc_set_position, STRING("クリボー２"), -55, 20, 35)
    CALL(evt_npc_set_position, STRING("クリボー３"), -55, 0, 85)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_set_damage_anim, STRING("クリボー１"))
    CALL(evt_npc_set_damage_anim, STRING("クリボー２"))
    CALL(evt_npc_set_damage_anim, STRING("クリボー３"))
    CALL(evt_cam3d_evt_set, -120, 150, 588, -120, 55, 37, 0, 11)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("クリボー１"), 512)
    CALL(evt_msg_print, 0, STRING("mac_0_093"), 0, STRING("クリボー１"))
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(60)
        CALL(evt_npc_set_run_anim, STRING("クリボー１"))
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_KURI_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        SPAWN_EVENT_GET_ID(PTR(ashioto), LW(10))
        CALL(evt_npc_move_position, STRING("クリボー１"), -170, LW(2), 0, FLOAT(170.0f), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_get_position, STRING("クリボー１"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_KURI_JUMP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー１"), -255, 75, LW(2), 500, 50)
        SPAWN_EVENT_GET_ID(PTR(ashioto), LW(10))
        CALL(evt_npc_move_position, STRING("クリボー１"), -450, LW(2), 0, FLOAT(300.0f), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_set_position, STRING("クリボー１"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_run_anim, STRING("クリボー２"))
        CALL(evt_npc_get_position, STRING("クリボー２"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("クリボー２"), -170, LW(2), 0, FLOAT(170.0f), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー２"), -255, 95, LW(2), 500, 50)
        CALL(evt_npc_move_position, STRING("クリボー２"), -450, LW(2), 0, FLOAT(300.0f), 0)
        CALL(evt_npc_set_position, STRING("クリボー２"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(30)
        CALL(evt_npc_set_run_anim, STRING("クリボー３"))
        CALL(evt_npc_get_position, STRING("クリボー３"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("クリボー３"), -170, LW(2), 0, FLOAT(170.0f), 0)
        CALL(evt_npc_jump_position_nohit, STRING("クリボー３"), -255, 75, LW(2), 500, 50)
        CALL(evt_npc_move_position, STRING("クリボー３"), -450, LW(2), 0, FLOAT(300.0f), 0)
        CALL(evt_npc_set_position, STRING("クリボー３"), 0, -1000, 0)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mario_set_dir_npc, STRING("クリボー３"))
        CALL(evt_party_set_dir_npc, 0, STRING("クリボー３"))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_WHILE(GF(1))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -140, 65, 310, -140, 33, 37, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_094"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_0_094_01"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    SET_VALUE(GSW(0), 12)
    CALL(evt_hitobj_onoff, STRING("a_rec"), 1, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_TIK1"), 2000)
    RETURN
EVT_END()

EVT_BEGIN(wall_bomb00)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("bom_kabe_1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_hitobj_onoff, STRING("a_dom_kabe_1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_dom_kabe_2"), 1, 1)
    SET_VALUE(GSWF(1331), 1)
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("bom_kabe_1"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(12), 0)
        IF_INT_EQUAL(LW(13), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_img_clear_virtual_point, STRING("capture_img"))
        CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_kabe_2"), 1)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_kabe_2"), 1)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("bom_kabe_1"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

EVT_BEGIN(wall_bomb01)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("bom_kabe_3"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("e_bero_2"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("e_bero_2"), 4)
    SET_VALUE(GSWF(1332), 1)
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("bom_kabe_3"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(12), 0)
        IF_INT_EQUAL(LW(13), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_img_clear_virtual_point, STRING("capture_img"))
        CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_kabe_4"), 1)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bom_kabe_4"), 1)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("bom_kabe_3"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 275, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_01_init_evt)
    SET_VALUE(GSWF(254), 1)
    IF_INT_EQUAL(GSWF(1369), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
        ELSE
        CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_TIK1"), 3000)
        SET_VALUE(GSWF(1369), 1)
    END_IF
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSWF(1191), 0)
        IF_INT_EQUAL(GSWF(1190), 1)
            IF_INT_GREATER_OR_EQUAL(GSW(0), 56)
                SET_VALUE(GSWF(1191), 1)
            END_IF
        END_IF
    END_IF
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    IF_INT_EQUAL(GSWF(1331), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_2"), 1)
        CALL(evt_hitobj_onoff, STRING("a_dom_kabe_2"), 1, 0)
        CALL(evt_run_case_evt, 15, 1, STRING("a_dom_kabe_1"), 0, PTR(wall_bomb00), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_1"), 1)
        CALL(evt_hitobj_onoff, STRING("a_dom_kabe_1"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(1332), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_4"), 1)
        SET_VALUE(LW(0), STRING("e_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("e_bero_2"), 4)
        CALL(evt_run_case_evt, 15, 1, STRING("e_bero_2"), 0, PTR(wall_bomb01), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bom_kabe_3"), 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 540, 125, -100, 0, 0, PTR(mobj_box00), GSWF(1355))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -100, 0, 50, STRING("jimen"), 125, GSWF(5589))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -455, 135, -60, 0, 0)
    SET_VALUE(LW(0), STRING("ugo_yuka_1"))
    SET_VALUE(LW(1), STRING("a_ugo_yuka_1"))
#ifdef VERSION_G8ME01
    SET_VALUE(LW(2), -15)
    SET_VALUE(LW(3), 103)
    SET_VALUE(LW(4), 2700)
#else
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 85)
    SET_VALUE(LW(4), 2000)
#endif
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi1"))
    IF_INT_LESS(GSW(0), 19)
        CALL(evt_npc_check_delete, STRING("トゲクリボー"))
    END_IF
    IF_INT_LESS_OR_EQUAL(GSW(0), 11)
        SPAWN_EVENT(PTR(gorotuki_sanninngumi))
        ELSE
        CALL(evt_hitobj_onoff, STRING("a_rec"), 1, 0)
    END_IF
    RETURN
EVT_END()

