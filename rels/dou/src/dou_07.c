#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "npc_event.h"
#include "dou.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"

static USER_FUNC(bgm_start_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_set_virtual_point);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_init_camid);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_camid);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_set_anim);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_set_color);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_win_item_select);
USER_FUNC(lect_cam_load);
USER_FUNC(lect_cam_save);
static void majin_callback(s32 type);

f32 fd_dou_07_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dou_07_neg_one_382;
f32 fd_dou_07___local_PI_383;
f32 fd_dou_07___local_3PIO2_385;
f32 fd_dou_07_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dou_07___local_PIO2_384;
f32 fd_dou_07_one_381;
f32 fd_dou_07_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dou_07_neg_one_365;
f32 fd_dou_07___local_PI_366;
f32 fd_dou_07___local_3PIO2_368;
f32 fd_dou_07_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dou_07___local_PIO2_367;
f32 fd_dou_07_one_364;
static EvtWord erumosu_regl[2];
#ifdef VERSION_G8ME01
EvtWord erumosu_dead[28];
#else
EvtWord erumosu_dead[25];
#endif
static NpcSetupInfo npcEnt[2];
static BeroEntry bero_entry_data[2];
static s32 key_tbl[2];
EvtWord apper_elmos[43];
EvtWord peramajin_before[396];
EvtWord peramajin_after[297];
EvtWord dou_07_init_evt[89];

f32 fd_dou_07_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_07_neg_one_382 = -1.0f;

f32 fd_dou_07___local_PI_383 = 3.1415927f;

f32 fd_dou_07___local_3PIO2_385 = 4.712389f;

f32 fd_dou_07_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_07___local_PIO2_384 = 1.5707964f;

f32 fd_dou_07_one_381 = 1.0f;

f32 fd_dou_07_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_07_neg_one_365 = -1.0f;

f32 fd_dou_07___local_PI_366 = 3.1415927f;

f32 fd_dou_07___local_3PIO2_368 = 4.712389f;

f32 fd_dou_07_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_07___local_PIO2_367 = 1.5707964f;

f32 fd_dou_07_one_364 = 1.0f;

static EVT_BEGIN(erumosu_regl)
    RETURN
EVT_END()

EVT_BEGIN(erumosu_dead)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(GSWF(2989), 1)
    SPAWN_EVENT(PTR(enemy_event_dead_event))
#ifdef VERSION_G8ME01
    WAIT_MS(1000)
    ADD(LW(1), 150)
    CALL(evt_item_entry, STRING("item_key"), 36, LW(0), LW(1), LW(2), 13, GSWF(2987), 0)
#else
    WAIT_MS(500)
    CALL(evt_item_entry, STRING("item_key"), 36, LW(0), LW(1), LW(2), 4, GSWF(2987), 0)
#endif
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"エルモス"*/"\x83\x47\x83\x8B\x83\x82\x83\x58", // name
        0x608, // flags
        0, // reactionFlags
        bubble_init_event, // initEvent
        bubble_move_event, // regularEvent
        NULL, // talkEvent
        erumosu_dead, // deadEvent
        bubble_find_event, // findEvent
        bubble_lost_event, // lostEvent
        bubble_return_event, // returnEvent
        NULL, 0, // blowEvent..territoryType
        {100.0f, -6.0f, -37.0f}, // territoryBase
        {50.0f, 50.0f, 50.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        15, // battleInfoId
    },
    {0},
};

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_08", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static s32 key_tbl[] = {
    ITEM_BLACK_KEY_0x24, -1,
};

EVT_BEGIN(apper_elmos)
    SET_VALUE(GSWF(2988), 1)
    CALL(evt_npc_set_position, STRING("エルモス"), 100, -6, -37)
    CALL(evt_npc_set_color, STRING("エルモス"), 255, 255, 255, 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_color, STRING("エルモス"), 255, 255, 255, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(peramajin_before)
    IF_INT_EQUAL(GSW(0), 246)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        CALL(evt_mario_adjust_dir)
        CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                CALL(evt_mario_set_dir, 90, 200, 0)
                ELSE
                CALL(evt_mario_set_dir, 270, 200, 0)
            END_IF
        END_INLINE_EVENT
        CALL(evt_pouch_check_item, 36, LW(10))
        IF_INT_LESS_OR_EQUAL(LW(10), 0)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_08"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("mobj_box00"))
            CALL(evt_party_run, 0)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_09"), PTR(majin_callback), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(10), 0)
        IF_INT_EQUAL(LW(10), -1)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_10"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("mobj_box00"))
            CALL(evt_party_run, 0)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            ADD(LW(3), 35)
            ADD(LW(5), 15)
            CALL(evt_party_move_pos2, 0, LW(3), LW(5), FLOAT(80.0f))
            IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                CALL(evt_party_set_dir, 0, 90, -1)
                ELSE
                CALL(evt_party_set_dir, 0, 270, -1)
            END_IF
        END_INLINE_EVENT
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_11"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_N"))
        CALL(evt_cam3d_evt_set, -33, 92, 412, -33, 36, 92, 500, 11)
        CALL(evt_pouch_remove_item, 36, LW(10))
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
            CALL(evt_mario_set_dir, 90, 200, 0)
            ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_IF
    END_INLINE_EVENT
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_01"), 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(0), LW(1), LW(2), LW(15))
    CALL(evt_mobj_set_anim, STRING("mobj_box00"), STRING("A_3"))
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(300)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_02"), PTR(majin_callback), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_03"), PTR(majin_callback), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_04"), PTR(majin_callback), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_05"), PTR(majin_callback), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_06"), PTR(majin_callback), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_07"), PTR(majin_callback), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_APPEAR1"), 0)
    CALL_CHILD(PTR(apper_elmos))
    CALL(evt_mobj_exec_cancel, STRING("mobj_box00"))
    SET_VALUE(GSW(0), 246)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peramajin_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_pouch_add_item, 5, LW(0))
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_PERAMAJIN1"))
    CALL(evt_snd_envoff, 1024)
    CALL(bgm_start_wait)
    SET_VALUE(LW(1), 60)
    LOOP(LW(1))
        SET_VALUE(LW(0), 255)
        MULTIPLY(LW(0), LW(1))
        DIVIDE(LW(0), 60)
        CALL(evt_mapobj_color, 1, STRING("s02_moku"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_color, 1, STRING("s02_moku"), 255, 255, 255, 0)
    CALL(evt_paper_entry, STRING("OFF_d_kusya"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_kusya"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_virtual_point, STRING("img"), -33, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_img_clear_virtual_point, STRING("img"))
    END_INLINE_EVENT
    CALL(evt_img_onoff, STRING("img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_APPEAR1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_COME1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_mario_set_camid, 7)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_mapobj_color, 1, STRING("s02_moku"), 255, 255, 255, 255)
    CALL(lect_cam_save)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(ship_majin_lecture_init))
    CALL(evt_party_run, 0)
    CALL_CHILD(PTR(ship_majin_lecture))
    CALL(evt_party_stop, 0)
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, 1)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, 1)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, 513, 2000)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_kusya"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print_party, STRING("stg1_gon_majin4_19"))
    SET_VALUE(GSW(0), 247)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    RETURN
EVT_END()

EVT_BEGIN(dou_07_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSWF(2989), 0)
        CALL(evt_npc_setup, PTR(npcEnt))
    END_IF
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("Ship_Change_01"))
    CALL(evt_map_playanim, STRING("moku"), 1, 0)
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    SET_VALUE(GF(0), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -33, -18, 56, 3, PTR(peramajin_before), PTR(peramajin_after), GSWF(2980))
    CALL(evt_item_entry, STRING("item_00"), 270, -75, -30, -15, 16, GSWF(2985), 0)
    IF_INT_EQUAL(GSWF(2988), 1)
        IF_INT_EQUAL(GSWF(2989), 0)
            SPAWN_EVENT(PTR(apper_elmos))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2989), 1)
        CALL(evt_item_entry, STRING("item_key"), 36, 100, -6, -37, 14, GSWF(2987), 0)
    END_IF
    RETURN
EVT_END()

static void majin_callback(s32 type) {
    MapObjectEntry* mobj = mobjNameToPtr("mobj_box00");

    if (type == 0) {
        animPoseSetAnim(mobj->poseId, "A_2", TRUE);
    } else if (type == 5) {
        if (animPoseGetLoopTimes(mobj->poseId) >= 1.0f) {
            animPoseSetAnim(mobj->poseId, "A_2", TRUE);
        }
    } else if (type == 2) {
        animPoseSetAnim(mobj->poseId, "S_1", TRUE);
    }
}

BOOL psndBGMStartCheck(s32 id);

static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}
