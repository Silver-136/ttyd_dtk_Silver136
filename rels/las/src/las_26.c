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
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

USER_FUNC(bonbaba_wait_landon);
USER_FUNC(evt_snd_sfxon_3d_ex);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord box_evt[30];
EvtWord bonbaba_evt[819];
EvtWord bonbaba_init[62];
static NpcSetupInfo npcEnt[2];
static EvtWord sekai_yami_evt[118];
EvtWord bomb_wall[155];
EvtWord las_26_init_evt[194];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
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
        "las_25", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "las_28", // nextMapName
        "from26", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_STAR_KEY, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(bonbaba_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_set_party, 0, 1)
    END_IF
    CALL(evt_cam3d_evt_set, -435, 150, 459, -435, 67, -13, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_set_pos, -600, 0, 0)
    CALL(evt_party_set_pos, 0, -600, 0, 0)
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, FLOAT(-70.0f), 0)
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, FLOAT(70.0f), 0)
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_mario_mov_pos2, -420, 0, FLOAT(150.0f))
    CALL(evt_party_run, 0)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
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
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_44"), 0, STRING("ボンババ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER2"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG8_GNB_ROAR1"), -400, 30, 0, 255, 255, 4300, 16, 0)
        CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_U_1"))
        CALL(evt_npc_wait_anim, STRING("ボンババ"), FLOAT(1.0f))
        CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_S_1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_STEP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 800)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kesu"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -900, 31, 490, -205, 175, 20, 5000, 11)
    WAIT_MS(8000)
    CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
    ADD(LW(0), -200)
    ADD(LW(1), 100)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg8_las_45"), 0, STRING("ボンババ"))
    CALL(evt_party_stop, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -435, 109, 353, -435, 45, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg8_las_46"))
    CALL(evt_cam3d_evt_set, -545, -14, 417, -225, 227, 12, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
    ADD(LW(0), -200)
    ADD(LW(1), 100)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg8_las_47"), 0, STRING("ボンババ"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_archi1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 77, 455, 380, -147, 293, 183, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
    ADD(LW(0), -200)
    ADD(LW(1), 100)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg8_las_48"), 0, STRING("ボンババ"))
    CALL(evt_cam3d_evt_set, -299, 25, 748, -328, 134, -34, 3000, 11)
    WAIT_MS(2500)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_npc_set_battle_info, STRING("ボンババ"), 14)
    CALL(evt_npc_battle_start, STRING("ボンババ"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_archi1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 0, 25, 845, 0, 100, 150, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING("ボンババ"), 0, 70, -100)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_GNB_DOWN1"), 0)
    CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_H_1"))
    CALL(evt_mario_set_pos, 25, 10, 150)
    CALL(evt_party_set_pos, 0, -25, 10, 150)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 1, STRING("ボンババ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(bonbaba_wait_landon)
        CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_H_2"))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 50, 5, 50)
        LOOP(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 50)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    WAIT_MS(1000)
    WAIT_MS(5000)
    CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_H_3"))
    CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(600)
    CALL(evt_cam3d_evt_set, 255, 220, 648, 255, 170, 150, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mobj_itembox, STRING("box"), 270, 60, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(box_evt), GSWF(4361))
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ボンババ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_THROAT_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1100)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_H_4"))
    CALL(evt_npc_wait_anim, STRING("ボンババ"), FLOAT(0.7792969f))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_JUMP1"), LW(0), LW(1), LW(2), GW(0))
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(60.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(25.0f))
        SET_FLOAT_VALUE(LW(2), FLOAT(-0.5996094f))
        SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(4), FLOAT(1.2998047f))
        SET_VALUE(LF(0), 0)
        LOOP(0)
            ADD_FLOAT(LW(0), LW(1))
            ADD_FLOAT(LW(3), LW(4))
            CALL(evt_mobj_set_y_position, STRING("box"), LW(0))
            CALL(evt_mobj_set_z_position, STRING("box"), LW(3))
            ADD_FLOAT(LW(1), LW(2))
            IF_INT_LESS(LW(1), -14)
                SET_FLOAT_VALUE(LW(1), FLOAT(-14.0f))
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_snd_sfxoff, GW(0))
                    SET_VALUE(LF(0), 1)
                END_IF
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
                CALL(evt_mobj_get_position, STRING("box"), LW(5), LW(6), LW(7))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_LANDING1"), LW(5), LW(6), LW(7), 0)
                IF_INT_LESS_OR_EQUAL(LW(1), 2)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mobj_set_y_position, STRING("box"), 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ボンババ"), STRING("GNB_H_3"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 255, 105, 648, 255, 55, 150, 500, 11)
    WAIT_MS(1800)
    WAIT_WHILE(GF(0))
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG8_04_GLOOMTAIL)
    CALL(evt_hitobj_onoff, STRING("baba"), 1, 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ボンババ"), NPC_FLAG_NO_HIT)
    CALL_CHILD(PTR(bero_case_entry))
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(bonbaba_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG8_03_THE_PALACE_GARDEN)
        CALL(evt_npc_set_position, STRING("me"), 100, 10, 45)
        CALL(evt_npc_set_ry, STRING("me"), 0)
        CALL(evt_npc_pera_onoff, STRING("me"), 0)
        CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(1.2998047f))
        CASE_GREATER_OR_EQUAL(STG8_04_GLOOMTAIL)
        IF_INT_EQUAL(GSWF(4380), 0)
            CALL(evt_npc_set_position, STRING("me"), 0, 0, -100)
            CALL(evt_npc_set_ry, STRING("me"), 0)
            CALL(evt_npc_pera_onoff, STRING("me"), 0)
            CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(1.2998047f))
            CALL(evt_npc_set_anim, STRING("me"), STRING("GNB_H_3"))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

USER_FUNC(bonbaba_wait_landon) {
    // ボンババ
    return npcNameToPtr("\x83\x7B\x83\x93\x83\x6F\x83\x6F")->groundHit ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static NpcSetupInfo npcEnt[] = {
    {
        /*"ボンババ"*/"\x83\x7B\x83\x93\x83\x6F\x83\x6F", // name
        NPC_FLAG_NO_CULL | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION, // flags
        0, // reactionFlags
        bonbaba_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 100, 5, 77, 0, 35, -150, 0, 11)
    CALL(evt_npc_entry, STRING("クリハカセ"), STRING("c_kuri_h"))
    CALL(evt_npc_set_tribe, STRING("クリハカセ"), STRING("クリハカセ"))
    CALL(evt_npc_set_position, STRING("クリハカセ"), -58, 0, -280)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_TALK_POSE)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_CEILING_BREAK1"), LW(10))
    CALL(evt_eff64, 0, STRING("hokori_n64"), 0, -58, 200, -280, 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(10))
    WAIT_MS(1000)
    LOOP(3)
        CALL(evt_npc_reverse_ry, STRING("クリハカセ"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("クリハカセ"))
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_dark_09"), 0, STRING("クリハカセ"))
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 16, 300)
    CALL(evt_bero_mapchange, STRING("las_29"), STRING("sekai_yami2"))
    RETURN
EVT_END()

EVT_BEGIN(bomb_wall)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_before1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana_before1"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox2, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(LW(3), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_wait_animend, STRING("img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana_before1"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_hitobj_onoff, STRING("A_ana_after"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_ana_before1"), 1, 0)
    SET_VALUE(GSWF(4360), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(las_26_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_03_THE_PALACE_GARDEN)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
        CALL(evt_snd_set_rev_mode, 2)
        ELSE
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
            CALL(evt_snd_bgmoff, PSND_FADEOUT)
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_set_rev_mode, 2)
            ELSE
            CALL(evt_snd_bgmoff, PSND_FADEOUT)
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
            CALL(evt_snd_set_rev_mode, 2)
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSWF(4360), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ana_after"), 1, 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_ana_before1"), 0, PTR(bomb_wall), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ana_before1"), 1, 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yukaato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_yukaato"), 1, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yukamae"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_yukamae"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_03_THE_PALACE_GARDEN)
        SPAWN_EVENT(PTR(bonbaba_evt))
        ELSE
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG8_03_THE_PALACE_GARDEN)
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
        CASE_GREATER_OR_EQUAL(STG8_04_GLOOMTAIL)
        IF_INT_EQUAL(GSWF(4380), 0)
            CALL(evt_hitobj_onoff, STRING("baba"), 1, 1)
            ELSE
            CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
    END_SWITCH
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_04_GLOOMTAIL)
        CALL(evt_mobj_itembox, STRING("box"), 270, 0, 178, MOBJ_ITEMBOX_NORMAL, 0, PTR(box_evt), GSWF(4361))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        SPAWN_EVENT(PTR(sekai_yami_evt))
    END_IF
    CALL(evt_mobj_badgeblk, STRING("mobj_01"), 1000, 60, -150, ITEM_ULTRA_SHROOM, 0, GSWF(4385), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_02"), 1365, 60, -150, ITEM_JAMMIN_JELLY, 0, GSWF(4386), 0)
    RETURN
EVT_END()

