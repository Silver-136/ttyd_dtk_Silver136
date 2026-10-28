#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "party.h"
#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_mario_dispflag_onoff);
static USER_FUNC(mario_kirimomi);
USER_FUNC(mario_reset);
USER_FUNC(party_kirimomi);
USER_FUNC(party_reset);

static BeroEntry bero_entry_data[3];
static f32 spline_dt[9];
static EvtWord party_damage_pose[52];
EvtWord party_damage_pose2[52];
static EvtWord evt_first[525];
#ifdef VERSION_G8ME01
EvtWord rock_main[105];
#else
static EvtWord rock_main[79];
#endif
#ifdef VERSION_G8ME01
static EvtWord rock_evt0[15];
#else
static EvtWord rock_evt0[21];
#endif
#ifdef VERSION_G8ME01
static EvtWord rock_evt1[15];
#else
static EvtWord rock_evt1[21];
#endif
EvtWord moo_00_init_evt[133];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "moo_07", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
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
        "moo_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static f32 spline_dt[] = {
    -320.0f, 200.0f, -170.0f, -220.0f, 100.0f, -170.0f, -120.0f, 0.0f,
    -170.0f,
};

static EVT_BEGIN(party_damage_pose)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_D_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_D_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("D_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_D_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_D_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_D_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_D_1"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(party_damage_pose2)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_D_5"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_D_5"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("D_4"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_D_5"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_D_5"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_D_4"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_D_4"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(evt_first)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_F_1"))
    CALL_CHILD(PTR(party_damage_pose))
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 5540, 1000)
        LABEL(30)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(party_kirimomi, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(30)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_sub_spline_init, 1, 1, PTR(spline_dt), 3, 0, 1000)
        LABEL(10)
        CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
        ADD(LW(0), -50)
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(3), 1)
            GOTO(10)
        END_IF
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("kemuri_test"), 14, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_MARIO_MOON_FALL1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 300)
        CALL_CHILD(PTR(party_damage_pose2))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 5620, 1000)
        LABEL(20)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(mario_kirimomi, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(20)
        END_IF
    END_INLINE_EVENT
    CALL(evt_sub_spline_init, 1, 1, PTR(spline_dt), 3, 0, 1000)
    LABEL(0)
    CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(3), 1)
        GOTO(0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    ADD(LW(2), 5)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_MARIO_MOON_FALL1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, 0, STRING("kemuri_test"), 14, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        LOOP(3)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            ADD(LW(0), 3)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            WAIT_FRAMES(6)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 3)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            WAIT_FRAMES(8)
        END_LOOP
        CALL(evt_party_cont_onoff, 0, 1)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(party_reset)
        CALL(evt_party_jump_pos, 0, LW(0), 0, LW(2), 500, 0, -1000)
        DEBUG_PUT_MSG(STRING("ビビアンしっぽ有効"))
        CALL(evt_party_vivian_tail, 0, 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    LOOP(3)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 3)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        WAIT_FRAMES(6)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 3)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        WAIT_FRAMES(8)
    END_LOOP
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(mario_reset)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP5"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(0), 0, LW(2), 500, 0, -1000)
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set, -145, 40, 105, -145, 40, -170, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_msg_print_party, STRING("stg7_moo_03"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_W_4B"))
        CALL(evt_mario_set_dir, 90, 300, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 270, 300, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 90, 300, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 270, 300, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_msg_print_party, STRING("stg7_moo_04"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG7_06_TO_THE_MOON)
    CALL(evt_telop_entry, 14)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(rock_main)
#else
static EVT_BEGIN(rock_main)
#endif
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, LW(10), STRING("offscreen"))
#ifdef VERSION_G8ME01
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(0), LW(1), LW(2), LW(3))
    IF_INT_GREATER(LW(2), 0)
        IF_INT_GREATER(LW(3), 0)
#else
    CALL(evt_mapobj_flag_onoff, 0, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
#endif
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
#ifdef VERSION_G8ME01
            CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 1, 0, 0, 0, 0)
#else
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 0, 0)
#endif
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_shadow, STRING("img"), 2)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
#ifdef VERSION_G8ME01
            CALL(evt_mapobj_flag_onoff, 0, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, LW(11), 1, 0)
#endif
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_release, STRING("img"))
#ifdef VERSION_G8ME01
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 0, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, LW(11), 1, 0)
#endif
    CALL(evt_offscreen_delete, STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 0, LW(10))
#ifndef VERSION_G8ME01
    CALL(evt_hitobj_onoff, LW(11), 1, 0)
#endif
    RETURN
EVT_END()

static EVT_BEGIN(rock_evt0)
#ifdef VERSION_G8ME01
    SET_VALUE(GSWF_MOO_00_ROCK_01_BLOWN, 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_01"))
    SET_VALUE(LW(11), STRING("A_iwa_01"))
    CALL_CHILD(PTR(rock_main))
#ifndef VERSION_G8ME01
    SET_VALUE(GSWF_MOO_00_ROCK_01_BLOWN, 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(rock_evt1)
#ifdef VERSION_G8ME01
    SET_VALUE(GSWF_MOO_00_ROCK_08_BLOWN, 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_08"))
    SET_VALUE(LW(11), STRING("A_iwa_08"))
    CALL_CHILD(PTR(rock_main))
#ifndef VERSION_G8ME01
    SET_VALUE(GSWF_MOO_00_ROCK_08_BLOWN, 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(moo_00_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_06_TO_THE_MOON)
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CASE_DEFAULT
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_SWITCH
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_MON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_MOO1"))
    CALL(evt_map_playanim, STRING("hosi_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 245, 49, -205, 0, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_06_TO_THE_MOON)
        CALL(evt_cam3d_evt_set, -150, 55, 330, -145, 55, -170, 0, EASING_SINE_IN_OUT)
        SPAWN_EVENT(PTR(evt_first))
        WAIT_FRAMES(1)
    END_SWITCH
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    IF_INT_EQUAL(GSWF_MOO_00_ROCK_01_BLOWN, 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_01"), 0, PTR(rock_evt0), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_01"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_MOO_00_ROCK_08_BLOWN, 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_08"), 0, PTR(rock_evt1), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_08"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_08"), 1, 0)
    END_IF
    CALL(evt_item_entry, STRING("item_01"), ITEM_STOPWATCH, -250, 0, -445, ITEM_TYPE_STILL, GSWF_MOO_00_STOPWATCH, 0)
    RETURN
EVT_END()

USER_FUNC(party_reset) {
    PartyEntry* party = partyGetPtr(0);

    party->scale.x = party->scale.y = party->scale.z = 1.0f;
    party->rotOfsY = 0.0f;
    party->rotation = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_reset) {
    MarioWork* mario = marioGetPtr();

    mario->scale.x = mario->scale.y = mario->scale.z = 1.0f;
    mario->rotOfs.y = 0.0f;
    mario->rot.z = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(party_kirimomi) {
    f32 angle = evtGetFloat(event, event->args[0]);
    PartyEntry* party = partyGetPtr(0);

    party->scale.x = party->scale.y = party->scale.z = 1.0f;
    party->rotOfsY = party->height / 2 * party->scale.y;
    party->rotation = angle;
    party->rotation = fmod(party->rotation, 360.0);
    return EVT_RETURN_DONE;
}

static USER_FUNC(mario_kirimomi) {
    f32 angle = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();

    mario->scale.x = mario->scale.y = mario->scale.z = 1.0f;
    mario->rotOfs.y = mario->wPlayerCollisionBox.y / 2 * mario->scale.y;
    mario->rot.z = angle;
    mario->rot.z = fmod(mario->rot.z, 360.0);
    return EVT_RETURN_DONE;
}

