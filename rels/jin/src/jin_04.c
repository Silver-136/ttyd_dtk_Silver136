#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "driver/npcdrv.h"
#include "system.h"

#include "mario/mario_pouch.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "jin.h"
#include "evt/evt_npc.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(evt_kagemario_change);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_pouch_party_join);
USER_FUNC(evt_pouch_party_left);
static USER_FUNC(getYoshiColor);
USER_FUNC(vivian_away);

static EvtWord s_close[12];
static EvtWord s_open[12];
static BeroEntry bero_entry_data[2];
EvtWord ranperu_toujou[338];
EvtWord starstone_jump_se[15];
EvtWord irekawatta_mario_init[67];
EvtWord irekawatta_mario[495];
EvtWord yoshi_color[65];
EvtWord ranperu_lastbattle[378];
EvtWord hontonosugata_mario_init[125];
EvtWord hontonosugata_mario[948];
EvtWord ranperu_init[20];
EvtWord nisemario_init[22];
static NpcSetupInfo npcEnt[7];
EvtWord jin_04_jumpstand[181];
EvtWord jin_04_init_evt[103];

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "n_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(ranperu_toujou)
    CALL(evt_mario_normalize)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("S_2"))
    CALL(evt_npc_set_autotalkpose, STRING("ランペル"), STRING("S_2"), STRING("T_2"))
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_00"), 0, 0)
    CALL(evt_cam3d_evt_set, 108, 766, 141, 105, 726, -98, 800, 11)
    WAIT_MS(800)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_RPL_APPEAR1"))
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_jin_01"), 0, STRING("ランペル"))
    CALL(evt_cam3d_evt_set, -75, 770, 344, -75, 713, 50, 0, 11)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg4_jin_02"))
    CALL(evt_cam3d_evt_set, 108, 766, 141, 105, 726, -98, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg4_jin_03"), 0, STRING("ランペル"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), 92)
    SET_VALUE(LW(4), 670)
    SET_VALUE(LW(5), -82)
    SUBTRACT(LW(3), LW(0))
    SUBTRACT(LW(4), LW(1))
    SUBTRACT(LW(5), LW(2))
    ADD(LW(4), 40)
    CALL(evt_cam3d_evt_set_rel, -30, 60, 300, LW(3), LW(4), LW(5), 600, 11)
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ランペル"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 85, 670, -65, 350, FLOAT(30.0f))
    WAIT_MS(250)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(6), 0)
        LOOP(0)
            CALL(evt_npc_get_position, STRING("ランペル"), LW(3), LW(4), LW(5))
            IF_INT_GREATER_OR_EQUAL(LW(5), 45)
                SET_VALUE(LW(6), 1)
            END_IF
            SUBTRACT(LW(3), LW(0))
            SUBTRACT(LW(4), LW(1))
            SUBTRACT(LW(5), LW(2))
            ADD(LW(4), 40)
            CALL(evt_cam3d_evt_set_rel, -30, 60, 300, LW(3), LW(4), LW(5), 100, 0)
            WAIT_MS(100)
            IF_INT_NOT_EQUAL(LW(6), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ランペル"), 67, 50, 1400, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_04"), 0, STRING("ランペル"))
    CALL(evt_cam3d_evt_set, -85, 750, 294, -85, 710, 47, 0, 11)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg4_jin_05"))
    CALL(evt_cam3d_evt_set, 55, 750, 294, 55, 710, 47, 1500, 11)
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("stg4_jin_06"), 0, STRING("ランペル"))
    CALL(evt_cam3d_evt_set, -85, 750, 294, -85, 710, 47, 0, 11)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 55, 750, 294, 55, 710, 47, 0, 11)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_jin_07"), 0, STRING("ランペル"))
    CALL(evt_cam3d_evt_set, 10, 900, 732, 10, 774, 15, 1500, 11)
    WAIT_MS(1500)
    CALL(evt_npc_set_battle_info, STRING("ランペル"), 5)
    CALL(evt_npc_battle_start, STRING("ランペル"))
    CALL(evt_npc_wait_battle_end)
    CALL_CHILD(PTR(irekawatta_mario_init))
    CALL(evt_snd_bgmoff, 512)
    SET_VALUE(GSW_STORY_PROGRESS, 200)
    SET_VALUE(LW(0), 0)
    SPAWN_EVENT(PTR(irekawatta_mario))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(starstone_jump_se)
    CALL(evt_item_get_position, STRING("rubystar"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(irekawatta_mario_init)
    CALL(evt_party_stop, 0)
    CALL(evt_kagemario_change, 1)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("にせマリオ"), -8, 670, 41)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_party_set_pos, 0, -40, 670, 41)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_pos, 80, 670, 50)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_cam3d_evt_set, 75, 785, 422, 75, 715, 25, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("にせマリオ"), NPC_FLAG_INSTANT_TURN)
    RETURN
EVT_END()

EVT_BEGIN(irekawatta_mario)
    IF_INT_EQUAL(LW(0), 1)
        GOTO(100)
    END_IF
    CALL_CHILD(PTR(irekawatta_mario_init))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(100)
    CALL(evt_mario_set_pose, STRING("M_D_1"))
    WAIT_MS(250)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 1440, 1200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MODULO(LW(0), 360)
        CALL(evt_mario_set_dispdir, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DOWN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_D_2"))
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -8, 800, 422, -8, 730, 28, 2000, 11)
    CALL(evt_item_entry, STRING("rubystar"), ITEM_RUBY_STAR, 0, -1000, 0, 17, -1, 0)
    CALL(evt_npc_sound_data_set, STRING("ランペル"), 0, 0, 0, 0, 0)
    CALL(evt_npc_set_position, STRING("ランペル"), 85, 470, 51)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
            ADD(LW(1), 200)
            ADD(LW(2), 10)
            CALL(evt_item_set_position, STRING("rubystar"), LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(0), -8)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 35, 470, 51, 600, FLOAT(30.0f))
    SPAWN_EVENT(PTR(starstone_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 10, 470, 51, 300, FLOAT(15.0f))
    SPAWN_EVENT(PTR(starstone_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), -2, 470, 51, 150, FLOAT(7.5f))
    SPAWN_EVENT(PTR(starstone_jump_se))
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), -8, 470, 51, 79, FLOAT(3.25f))
    SPAWN_EVENT(PTR(starstone_jump_se))
    CALL(evt_npc_set_position, STRING("ランペル"), -8, 470, 51)
    WAIT_MS(300)
    WAIT_MS(500)
    SET_VALUE(LW(0), -1)
    SET_VALUE(LW(1), STRING("rubystar"))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL_CHILD(PTR(evt_sub_starstone))
    WAIT_MS(500)
    GOTO(101)
    LABEL(100)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_kagemario_change, 1)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("にせマリオ"), -8, 670, 41)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 90)
    CALL(evt_party_set_pos, 0, -40, 670, 41)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_pos, 80, 670, 50)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_cam3d_evt_set_npc_rel, STRING("にせマリオ"), 0, 50, 397, 0, 70, 25, 0, 11)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_D_2"))
    CALL(evt_seq_wait, 2)
    CALL(evt_fade_end_wait)
    LABEL(101)
    CALL(evt_npc_set_stay_anim, STRING("にせマリオ"))
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 270)
    CALL(evt_cam3d_evt_set, 0, 823, 570, 0, 727, 27, 500, 11)
    WAIT_MS(500)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg4_jin_10"))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("にせマリオ"), -45, 105, 500, 0, 0)
    CALL(evt_npc_set_position, STRING("ランペル"), -45, 470, 105)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_jump_position_nohit, STRING("ランペル"), -84, 400, 105, 400, FLOAT(60.0f))
        CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_LANPEL_M_JUMP1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_J_1B"))
        LOOP(0)
            SET_VALUE(LW(3), LW(1))
            CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
            ADD(LW(1), 200)
            CALL(evt_npc_set_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
            IF_INT_LESS(LW(1), LW(3))
                CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_J_1C"))
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(1), 400)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_position, STRING("にせマリオ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(100)
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_party_move_pos2, 0, -45, 105, FLOAT(80.0f))
    CALL(evt_party_jump_pos, 0, -92, 600, 105, 400, 0, -1000)
    CALL(evt_party_run, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_pouch_party_left, 1)
    CALL(evt_pouch_party_left, 2)
    CALL(evt_pouch_party_left, 5)
    CALL(evt_pouch_party_left, 4)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_JIN1"))
    CALL(evt_cam3d_evt_off, 1000, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 201)
    SET_VALUE(GSWF(2081), 1)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(yoshi_color)
    CALL(evt_npc_delete, STRING("ヨッシー"))
    CALL(getYoshiColor, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("c_babyyoshi"))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("c_babyyoshi2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("c_babyyoshi3"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("c_babyyoshi4"))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("c_babyyoshi5"))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("c_babyyoshi6"))
        CASE_EQUAL(6)
        SET_VALUE(LW(0), STRING("c_babyyoshi7"))
    END_SWITCH
    CALL(evt_npc_entry, STRING("ヨッシー"), LW(0))
    CALL(evt_npc_set_tribe, STRING("ヨッシー"), STRING("ヨッシー"))
    CALL(evt_npc_flag_onoff, 1, STRING("ヨッシー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("ヨッシー"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(ranperu_lastbattle)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_hello_party, 0, 6)
    END_IF
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING("にせマリオ"), 75, 670, 41)
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 113, 670, 43)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), 135, 670, 61)
    CALL(evt_npc_set_position, STRING("クラウダ"), 157, 670, 41)
    CALL(evt_npc_set_position, STRING("ヨッシー"), 175, 670, 75)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 270)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 270)
    CALL(evt_npc_set_ry, STRING("クラウダ"), 270)
    CALL(evt_npc_set_ry, STRING("ヨッシー"), 270)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_11"), 0, STRING("にせマリオ"))
    CALL(evt_cam3d_evt_set, 0, 780, 389, 0, 711, -6, 800, 11)
    WAIT_MS(800)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_RPL_APPEAR1"))
    CALL(evt_msg_print, 0, STRING("stg4_jin_12"), 0, STRING("にせマリオ"))
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 90)
    CALL(evt_msg_continue)
    CALL(evt_cam3d_evt_set, 100, 780, 389, 100, 711, -6, 600, 11)
    WAIT_MS(600)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_msg_print, 0, STRING("stg4_jin_13"), 0, STRING("クリスチーヌ"))
    CALL(evt_cam3d_evt_set, 114, 735, 240, 114, 692, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_14"), 0, STRING("クリスチーヌ"))
    CALL(evt_cam3d_evt_set, 133, 735, 240, 133, 692, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_15"), 0, STRING("ノコタロウ"))
    CALL(evt_cam3d_evt_set, 156, 744, 240, 156, 701, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_16"), 0, STRING("クラウダ"))
    CALL(evt_cam3d_evt_set, 170, 723, 240, 170, 680, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_17"), 0, STRING("ヨッシー"))
    CALL(evt_cam3d_evt_set, 71, 735, 215, 71, 692, -6, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_U_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 270)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_jin_18"), 0, STRING("にせマリオ"))
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_4"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_5"))
    CALL(evt_cam3d_evt_set, -95, 740, 288, -95, 712, 90, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg4_jin_19"))
    CALL(evt_msg_select, 0, STRING("stg4_jin_19_select"))
    CALL(evt_msg_print_add, 0, STRING("stg4_jin_20"))
    CALL(evt_msg_continue)
    CALL(evt_cam3d_evt_set, 71, 735, 215, 71, 692, -6, 0, 11)
    WAIT_FRAMES(1)
    CALL(vivian_away)
    CALL(evt_msg_print, 0, STRING("stg4_jin_21"), 0, STRING("にせマリオ"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_cam3d_evt_set, 10, 900, 732, 10, 774, 15, 1500, 11)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_npc_set_battle_info, STRING("にせマリオ"), 6)
    CALL(evt_npc_battle_start, STRING("にせマリオ"))
    CALL(evt_npc_wait_battle_end)
    CALL_CHILD(PTR(hontonosugata_mario_init))
    SET_VALUE(GSW_STORY_PROGRESS, 211)
    SPAWN_EVENT(PTR(hontonosugata_mario))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(hontonosugata_mario_init)
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_1"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_1"))
    CALL(evt_npc_set_position, STRING("ランペル"), -15, 670, 41)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("にせマリオ"), 0, 670, 41)
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 109, 670, 41)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), 78, 670, 75)
    CALL(evt_npc_set_position, STRING("クラウダ"), 50, 670, 59)
    CALL(evt_npc_set_position, STRING("ヨッシー"), 24, 670, 43)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 270)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 270)
    CALL(evt_npc_set_ry, STRING("クラウダ"), 270)
    CALL(evt_npc_set_ry, STRING("ヨッシー"), 270)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_S_1"))
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("D_2"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_pos, -106, 670, 35)
    CALL(evt_party_set_pos, 0, -66, 670, 35)
    CALL(evt_cam3d_evt_set, -22, 745, 290, -22, 695, -6, 0, 11)
    CALL(evt_snd_bgmoff, 512)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

EVT_BEGIN(hontonosugata_mario)
    CALL_CHILD(PTR(hontonosugata_mario_init))
    CALL(evt_item_entry, STRING("rubystar"), ITEM_RUBY_STAR, 0, -1000, 0, 17, -1, 0)
    CALL(evt_npc_entry, STRING("ダミー"), STRING("c_mario"))
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 30)
    ADD(LW(2), 5)
    CALL(evt_npc_set_position, STRING("ダミー"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_LANPEL_M_DOWN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_D_4"))
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_LAND_DAMAGE2"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 25)
        ADD(LW(2), 20)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(0), 670, LW(2), 300, FLOAT(15.0f))
        SPAWN_EVENT(PTR(starstone_jump_se))
        SUBTRACT(LW(0), 12)
        ADD(LW(2), 10)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(0), 670, LW(2), 150, FLOAT(7.5f))
        SPAWN_EVENT(PTR(starstone_jump_se))
        SUBTRACT(LW(0), 6)
        ADD(LW(2), 5)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(0), 670, LW(2), 75, FLOAT(3.25f))
        SPAWN_EVENT(PTR(starstone_jump_se))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("ダミー"), LW(0), LW(1), LW(2))
            CALL(evt_item_set_position, STRING("rubystar"), LW(0), LW(1), LW(2))
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(340)
    SET_VALUE(LW(4), 100)
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_DRAW)
    WAIT_MS(LW(4))
    LOOP(0)
        CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_DRAW)
        CALL(evt_npc_flag_onoff, 0, STRING("にせマリオ"), NPC_FLAG_NO_DRAW)
        WAIT_MS(LW(4))
        CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_NO_DRAW)
        CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_DRAW)
        WAIT_MS(LW(4))
        SUBTRACT(LW(4), 10)
        IF_INT_LESS_OR_EQUAL(LW(4), 10)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(600)
    CALL(evt_cam3d_evt_set, 67, 750, 290, 67, 700, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_jin_27"), 0, 0)
    CALL(evt_cam3d_evt_set, -106, 751, 282, -106, 719, 92, 800, 11)
    WAIT_MS(800)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_1"))
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_RETURN1_1"), LW(0), LW(1), LW(2), 0)
        LOOP(10)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_eff64, 0, STRING("stardust2_n64"), 4, LW(0), LW(1), LW(2), 30, 40, 10, 30, 0, 0, 0, 0)
            WAIT_MS(400)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_dispdir, LW(0))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_dispdir, LW(0))
        SET_VALUE(LW(1), LW(0))
        ADD(LW(1), 7200)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, LW(0), LW(1), 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mario_set_dispdir, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_GREATER(LW(0), 6120)
                IF_INT_EQUAL(LF(0), 0)
                    SET_VALUE(LF(0), 1)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CHANGE_LANPEL1"), LW(0), LW(1), LW(2), 0)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_PAUSE1"), LW(0), LW(1), LW(2), 0)
                    ADD(LW(2), 5)
                    CALL(evt_eff, 0, STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
                    DELETE_EVENT(LW(9))
                END_IF
            END_IF
            IF_INT_GREATER(LW(0), 6480)
                IF_INT_EQUAL(LF(1), 0)
                    SET_VALUE(LF(1), 1)
                    CALL(evt_kagemario_change, 0)
                END_IF
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(4100)
    CALL(evt_mario_set_dispdir, LW(0))
    CALL(evt_kagemario_change, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -8, 823, 564, -8, 727, 22, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("O_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_tribe, STRING("ランペル"), STRING("ランペル音付き"))
    CALL(evt_npc_set_autotalkpose, STRING("ランペル"), STRING("S_3"), STRING("T_3"))
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("S_3"))
    CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), LW(0), LW(1), LW(2), 300, 20)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("S_3"))
    WAIT_MS(400)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    WAIT_MS(400)
    CALL(evt_npc_set_ry, STRING("ランペル"), 270)
    WAIT_MS(400)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    WAIT_MS(400)
    CALL(evt_npc_set_ry, STRING("ランペル"), 270)
    CALL(evt_msg_print, 0, STRING("stg4_jin_28"), 0, STRING("ランペル"))
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("ランペル"), STRING("R_2"))
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 47, 696, -66, 500, 80)
    CALL(evt_npc_set_ry, STRING("ランペル"), 270)
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 21, 690, -117, 300, 40)
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), 5, 600, -304, 800, 100)
    CALL(evt_npc_set_position, STRING("にせマリオ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 110, 730, 240, 110, 690, -6, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg4_jin_29"), 0, STRING("クリスチーヌ"))
    CALL(evt_cam3d_evt_set, 76, 732, 261, 76, 692, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_30"), 0, STRING("ノコタロウ"))
    CALL(evt_cam3d_evt_set, 50, 740, 261, 50, 700, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_31"), 0, STRING("クラウダ"))
    CALL(evt_cam3d_evt_set, 25, 733, 251, 25, 693, -6, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg4_jin_32"), 0, STRING("ヨッシー"))
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_4"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_5"))
    CALL(evt_cam3d_evt_set, -75, 752, 238, -75, 719, 92, 800, 11)
    WAIT_MS(800)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg4_jin_33"))
    CALL(evt_cam3d_evt_set, 14, 790, 433, 14, 731, 92, 800, 11)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_mov_pos, -66, 20, 400)
    CALL(evt_mario_mov_pos, -16, 35, 500)
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(600)
    CALL(evt_msg_print_party, STRING("stg4_jin_34"))
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_1"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_1"))
    CALL(evt_cam3d_evt_set, 48, 781, 380, 48, 731, 92, 300, 11)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg4_jin_35"), 0, STRING("ヨッシー"))
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -46, 751, 232, -46, 719, 92, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("stg4_jin_36"))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_msg_continue)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_jin_37"), 0, 0)
    CALL(evt_cam3d_evt_set, -16, 790, 433, -16, 731, 92, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_party_dispflg_onoff, 0, 16, 0)
    CALL(evt_item_get_position, STRING("rubystar"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 5)
    CALL(evt_party_move_pos, 0, LW(0), LW(2), 500)
    WAIT_MS(300)
    CALL(evt_jin_vivian_pose_set, 2, STRING("PTR_W_4"))
    CALL(evt_jin_vivian_pose_set, 3, STRING("PTR_W_4"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_6"))
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_5"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            ADD_FLOAT(LW(0), FLOAT(20.0f))
            ADD_FLOAT(LW(1), FLOAT(10.0f))
            CALL(evt_item_set_position, STRING("rubystar"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_party_move_pos, 0, LW(0), LW(2), 800)
    WAIT_MS(600)
    CALL(evt_msg_print_party, STRING("stg4_jin_38"))
    CALL(evt_party_set_pose, 0, STRING("PTR_U_1"))
    WAIT_MS(1000)
    DELETE_EVENT(LW(10))
    CALL(evt_item_set_position, STRING("rubystar"), 0, -1000, 0)
    CALL(evt_party_set_pose, 0, STRING("PTR_S_1"))
    CALL(evt_jin_vivian_pose_set, 2, STRING("PTR_W_1"))
    CALL(evt_jin_vivian_pose_set, 3, STRING("PTR_R_1"))
    CALL(evt_jin_vivian_pose_set, 7, STRING("PTR_T_1"))
    CALL(evt_jin_vivian_pose_set, 1, STRING("PTR_S_1"))
    CALL(evt_pouch_party_join, 1)
    CALL(evt_pouch_party_join, 2)
    CALL(evt_pouch_party_join, 5)
    CALL(evt_pouch_party_join, 4)
    CALL(evt_pouch_party_join, 6)
    SET_VALUE(GSW_STORY_PROGRESS, 212)
    SET_VALUE(GSW_STORY_PROGRESS, 213)
    SET_VALUE(LW(0), 4)
    SET_VALUE(LW(1), STRING("rubystar"))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL_CHILD(PTR(evt_sub_starstone))
    RETURN
EVT_END()

EVT_BEGIN(ranperu_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("me"), 110, 695, -85)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nisemario_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(200)
        CALL(evt_npc_set_position, STRING("me"), -8, 670, 81)
        BREAK_SWITCH
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        BREAK_SWITCH
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        ranperu_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"にせマリオ"*/"\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        nisemario_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリスチーヌ"*/"\x83\x4E\x83\x8A\x83\x58\x83\x60\x81\x5B\x83\x6B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコタロウ"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クラウダ"*/"\x83\x4E\x83\x89\x83\x45\x83\x5F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ヨッシー"*/"\x83\x88\x83\x62\x83\x56\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(jin_04_jumpstand)
    IF_INT_EQUAL(LW(0), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_OR_EQUAL(199)
            CASE_OR_EQUAL(210)
            CALL(evt_cam3d_evt_set, -55, 790, 438, -55, 727, 79, 600, 11)
            CALL(evt_mario_key_onoff, 0)
            CASE_END
        END_SWITCH
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_dispflag_onoff, 1, 4)
        CALL(L_evt_party_dokan, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BEGIN_INLINE_EVENT_GET_ID(LW(10))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mobj_get_position, STRING("mobj_jump"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_party_jump_pos, 0, -100, 670, 50, 1000, 0, -1000)
                CALL(evt_party_run, 0)
            END_INLINE_EVENT
        END_IF
        CALL(evt_mario_jump_pos, -75, 670, 50, 1000, 0, -1000)
        LOOP(0)
            IS_EVENT_RUNNING(LW(10), LW(11))
            IF_INT_EQUAL(LW(11), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_dispflag_onoff, 0, 4)
        ELSE
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_dispflag_onoff, 1, 4)
        CALL(L_evt_party_dokan, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BEGIN_INLINE_EVENT_GET_ID(LW(10))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mobj_get_position, STRING("mobj_jump"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_party_jump_pos, 0, -60, 450, 130, 500, 0, -1000)
                CALL(evt_party_run, 0)
            END_INLINE_EVENT
        END_IF
        CALL(evt_hitobj_onoff, STRING("MOBJ_mobj_jump"), 0, 0)
        CALL(evt_mario_jump_pos, -30, 450, 110, 500, 0, -1000)
        CALL(evt_hitobj_onoff, STRING("MOBJ_mobj_jump"), 0, 1)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_dispflag_onoff, 0, 4)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(jin_04_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(200)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envoff, 512)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG4_JIN1"))
    END_SWITCH
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 10, -100, 60, 150, 0, 0)
    CALL(evt_mobj_jumpstand_red, 0, STRING("mobj_jump"), -75, 450, 100, PTR(jin_04_jumpstand), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_run_case_evt, 0, 1, STRING("a_yuka"), 0, PTR(ranperu_toujou), 0)
        CASE_EQUAL(210)
        CALL_CHILD(PTR(yoshi_color))
        CALL(evt_run_case_evt, 0, 1, STRING("a_yuka"), 0, PTR(ranperu_lastbattle), 0)
        CASE_EQUAL(200)
        CALL_CHILD(PTR(yoshi_color))
        SET_VALUE(LW(0), 1)
        SPAWN_EVENT(PTR(irekawatta_mario))
        CASE_EQUAL(211)
        CALL(evt_mario_key_onoff, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            SPAWN_EVENT(PTR(hontonosugata_mario))
        END_INLINE_EVENT
    END_SWITCH
    RETURN
EVT_END()

static USER_FUNC(getYoshiColor) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], pouchGetPartyColor(PARTNER_YOSHI));
    return EVT_RETURN_DONE;
}
USER_FUNC(vivian_away) {
    pouchGetPtr()->partyData[6].flags |= 2;
    return EVT_RETURN_DONE;
}
