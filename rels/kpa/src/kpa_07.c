#include "version.h"
#include "eff/eff_starstone.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "system.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/npcdrv.h"
#include "string.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_memcard.h"
#include "evt/evt_npc.h"
#include "driver/msgdrv.h"
#include "driver/seqdrv.h"
#ifdef VERSION_G8ME01
#endif


USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_npc_set_stay_anim);
static USER_FUNC(get_starstone_pos);
USER_FUNC(kpa_chg_pose);
static USER_FUNC(set_starstone_pos);
USER_FUNC(set_starstone_ry);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_fall);
void kpa_stg6_014_cb(s32 type, smartEntry* smart);
void kpa_stg6_012_1_cb(s32 type, smartEntry* smart);

static NpcSetupInfo npcEnt[3];
EvtWord kpa_07_hipattack_eff[149];
EvtWord kpa_07_evt[1224];
EvtWord kpa_07_init_evt[52];

static NpcSetupInfo npcEnt[] = {
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブレッドハート"*/"\x83\x75\x83\x8C\x83\x62\x83\x68\x83\x6E\x81\x5B\x83\x67", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(kpa_07_hipattack_eff)
    CALL(evt_npc_get_position, STRING("ブレッドハート"), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    BEGIN_INLINE_EVENT
        WAIT_MS(50)
        SET_VALUE(LW(3), LW(1))
        ADD(LW(3), 20)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(3), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    SUBTRACT(LW(3), 30)
    ADD(LW(4), 20)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    SUBTRACT(LW(3), 10)
    ADD(LW(4), 5)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    ADD(LW(3), 5)
    ADD(LW(4), 25)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    ADD(LW(3), 35)
    ADD(LW(4), 15)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(kpa_07_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -249, 0, 0)
    CALL(evt_npc_set_position, STRING("ババ"), -349, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_DRAW)
    CALL(starstone_entry, STRING("nise"), 9, 180, 55, 0, FLOAT(0.75f))
    CALL(set_starstone_ry, STRING("nise"), 270)
    CALL(evt_cam3d_evt_set, 158, 190, 441, -109, 90, -61, 0, 11)
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（息切れ）"))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 158, 160, 441, -109, 60, -61, 1500, 11)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_DRAW)
        CALL(evt_npc_move_position, STRING("ババ"), -168, 0, 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_mov_pos2, -68, 0, FLOAT(60.0f))
    WAIT_MS(1000)
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（地上）"))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("K_1"), STRING("T_7"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_009"), 0, STRING("ババ"))
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("S_1"))
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_010"), 0, STRING("mario"))
    CALL(evt_npc_move_position, STRING("ババ"), -135, 30, 0, FLOAT(90.0f), 0)
    CALL(evt_npc_move_position, STRING("ババ"), -75, 50, 0, FLOAT(90.0f), 0)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("P_1"), STRING("T_6"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_011"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_1A"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -117, 127, 302, 196, 59, -14, 1500, 11)
    WAIT_MS(1500)
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_cam3d_evt_zoom_in, 60, 600, 0, 11)
    WAIT_MS(600)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_POWER_SHINE1"), 196, 59, -14, 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_012"), 0, STRING("mario"))
    CALL(evt_mario_set_pos, 60, 0, 0)
    CALL(evt_npc_set_position, STRING("ババ"), 60, 0, 40)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, 95, 124, 428, 95, 55, -17, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_012_1"), PTR(kpa_stg6_012_1_cb), STRING("ババ"))
    CALL(evt_npc_move_position, STRING("ババ"), 140, 0, 0, FLOAT(60.0f), 0)
    CALL(evt_npc_set_position, STRING("ブレッドハート"), -150, 0, 0)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_013"), 0, STRING("ブレッドハート"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_013_1"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, -144, 82, 240, -144, 42, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_014"), PTR(kpa_stg6_014_cb), STRING("ブレッドハート"))
    CALL(evt_cam3d_evt_set, -60, 157, 551, -60, 70, -17, 1000, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 90)
        CALL(evt_npc_move_position, STRING("ババ"), LW(0), LW(2), 1000, 0, 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 70)
    CALL(evt_mario_mov_pos, LW(0), LW(2), 1000)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_3"), STRING("KPA_T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_015"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_016"), 0, STRING("ブレッドハート"))
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_cam3d_evt_set, 12, 135, 392, 12, 72, -17, 1000, 11)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    CALL(evt_npc_get_position, STRING("ブレッドハート"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_BRED_TACKLE1"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GW(0), LW(1))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブレッドハート"), LW(2), LW(3), LW(4))
        SET_VALUE(LW(5), LW(2))
        ADD(LW(5), 30)
        CALL(evt_npc_move_position, STRING("ブレッドハート"), LW(5), 0, 200, 0, 0)
        WAIT_FRAMES(1)
        CALL(evt_npc_set_anim, STRING("ブレッドハート"), STRING("A_2A"))
        ADD(GW(0), 10)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(5), 10, 800)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("ブレッドハート"), LW(0), GW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        SET_VALUE(GW(0), 0)
        CALL(evt_npc_set_anim, STRING("ブレッドハート"), STRING("S_7"))
        CALL(evt_npc_get_position, STRING("ブレッドハート"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_BRED_TACKLE_HIT1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 1000, 333, 200)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("ブレッドハート"), FLOAT(1.0f), LW(0), FLOAT(1.0f))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_npc_get_height, STRING("ブレッドハート"), LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(0.33203125f))
        CALL(evt_npc_set_height, STRING("ブレッドハート"), LW(0))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        SPAWN_EVENT(PTR(kpa_07_hipattack_eff))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_ANGRY1_1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_koopa_hip_attack, LW(0), LW(1), LW(2), 60, 300)
        ADD(LW(0), 45)
        SUBTRACT(LW(2), 50)
        CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 300, 1, 20)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("D_1"))
        CALL(evt_npc_get_position, STRING("ババ"), LW(2), LW(3), LW(4))
        SET_VALUE(LW(5), LW(2))
        SUBTRACT(LW(5), 5)
        SET_VALUE(LW(6), LW(3))
        ADD(LW(6), 20)
        SET_VALUE(LW(7), LW(4))
        ADD(LW(7), 10)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(5), LW(6), LW(7), 1, 5, 0, 0, 0, 0, 0, 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(2), 160, 400)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_position, STRING("ババ"), LW(0), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("D_2"))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(2))
        SUBTRACT(LW(3), 2)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(GF(0), 0)
                WAIT_FRAMES(1)
                ELSE
                ADD(LW(3), 4)
                CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(3))
                WAIT_MS(50)
                SUBTRACT(LW(3), 4)
                CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(3))
                WAIT_MS(50)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 103, 302, 0, 40, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("ブレッドハート"), STRING("S_7"), STRING("T_7"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_017"), 0, STRING("ブレッドハート"))
    CALL(evt_cam3d_evt_set, 105, 115, 378, 105, 55, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("D_2"), STRING("T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_019"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 85, 136, 477, 85, 60, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_018"), 0, STRING("mario"))
    WAIT_MS(500)
    SET_VALUE(GF(1), 1)
    CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(3), FLOAT(-1.2998047f))
    SET_FLOAT_VALUE(LW(4), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(5), FLOAT(1.0996094f))
    SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(7), FLOAT(-0.39941406f))
    SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
    SET_VALUE(LF(0), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STONE_FALL1"), LW(0), LW(1), LW(2), LW(15))
    LOOP(0)
        ADD_FLOAT(LW(0), LW(3))
        ADD_FLOAT(LW(1), LW(4))
        ADD_FLOAT(LW(2), LW(5))
        CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LF(0), 0)
            IF_INT_LESS_OR_EQUAL(LW(1), 30)
                SET_VALUE(LF(0), 1)
                MULTIPLY_FLOAT(LW(4), FLOAT(-0.5f))
                CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STONE_HIT1"), LW(0), LW(1), LW(2), 0)
            END_IF
            ELSE
            IF_INT_LESS_OR_EQUAL(LW(1), 10)
                BREAK_LOOP
            END_IF
        END_IF
        ADD_FLOAT(LW(3), LW(6))
        ADD_FLOAT(LW(4), LW(7))
        ADD_FLOAT(LW(5), LW(8))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    CALL(set_starstone_pos, STRING("nise"), LW(0), 10, LW(2))
    CALL(starstone_fall, STRING("nise"))
    CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STONE_BREAK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_5"), STRING("KPA_T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_020_1"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 130, 80, 304, 130, 30, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ババ"), LW(0), LW(1), LW(2), 300, 40)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("P_1"), STRING("T_6"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_020_2"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 85, 136, 477, 85, 60, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 10)
    ADD(LW(2), 10)
    CALL(evt_npc_move_position, STRING("ババ"), LW(0), LW(2), 0, 40, 0)
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, 1, 0, STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_020_3"), 0, STRING("ババ"))
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_021"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 0, 103, 302, 0, 40, -17, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_022"), 0, STRING("ブレッドハート"))
    CALL(evt_cam3d_evt_set, 85, 136, 477, 85, 60, -17, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_023"), 0, STRING("mario"))
    CALL(kpa_chg_pose, STRING("KPA_I_2"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 150)
            WAIT_MS(150)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)
    SET_VALUE(GSW_STORY_PROGRESS, 344)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    SET_VALUE(GF(0), 1)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 18)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, 59, 0, 58, 0)
    CALL(evt_bero_mapchange, STRING("gor_04"), STRING("resha_return"))
    RETURN
EVT_END()

EVT_BEGIN(kpa_07_init_evt)
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_entry, STRING("belt"), STRING("c_bred_h"))
    CALL(evt_npc_set_tribe, STRING("belt"), STRING("ブレッドハート"))
    CALL(evt_npc_flag_onoff, 1, STRING("belt"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_pera_onoff, STRING("belt"), 0)
    CALL(evt_npc_set_anim, STRING("belt"), STRING("C_1"))
    CALL(evt_npc_set_ry, STRING("belt"), 80)
    CALL(evt_npc_set_position, STRING("belt"), 190, 52, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 343)
        SPAWN_EVENT(PTR(kpa_07_evt))
    END_IF
    RETURN
EVT_END()

void kpa_stg6_014_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;
    NpcEntry* npc;

    switch (type) {
        case 3:
            if (info->curPage == 1) {
                // ブレッドハート
                npc = npcNameToPtr("\x83\x75\x83\x8C\x83\x62\x83\x68\x83\x6E\x81\x5B\x83\x67");
                strcpy(npc->stayAnim, "S_5");
                strcpy(npc->talkAnim, "T_5");
            }
            if (info->curPage == 2) {
                // ブレッドハート
                npc = npcNameToPtr("\x83\x75\x83\x8C\x83\x62\x83\x68\x83\x6E\x81\x5B\x83\x67");
                strcpy(npc->stayAnim, "S_1");
                strcpy(npc->talkAnim, "T_1");
            }
            break;
    }
}

void kpa_stg6_012_1_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 0:
            // ババ
            animPoseSetAnim(npcNameToPtr("\x83\x6F\x83\x6F")->poseId, "A_2B", TRUE);
            break;
        case 5:
            if (info->nextEntry == 8) {
                // ババ
                animPoseSetAnim(npcNameToPtr("\x83\x6F\x83\x6F")->poseId, "T_1", TRUE);
            }
            break;
    }
}

static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    f32 scale = evtGetFloat(event, args[5]);
    EffectEntry* eff = effStarStoneEntry(type, x, y, z, scale);

    effSetName(eff, name);
    ((EffStarStoneWork*)eff->userdata)->flags |= 0x10;
    return EVT_RETURN_DONE;
}

static USER_FUNC(get_starstone_pos) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], ((EffStarStoneWork*)eff->userdata)->position.x);
    evtSetFloat(event, args[2], ((EffStarStoneWork*)eff->userdata)->position.y);
    evtSetFloat(event, args[3], ((EffStarStoneWork*)eff->userdata)->position.z);
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_starstone_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(set_starstone_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_fall) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((EffStarStoneWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

