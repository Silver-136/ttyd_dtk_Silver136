// tik_03 (埋没した都市, Rogueport Sewers): where Mario meets the guide Puni (puni_soguu_3), who joins him (puni_nakama), and
// the switch that opens the way to Boggly Woods (win_06).

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


EvtWord bero_05_custom_in[30];
EvtWord bero_05_custom_out[12];
static BeroEntry bero_entry_data[3];
static BeroEntry bero_entry_data2[2];
static EvtWord guide_init[48];
static EvtWord guide_talk[11];
static NpcSetupInfo npcEnt[2];
static EvtWord puni_walk_se[20];
EvtWord puni_soguu_3[584];
EvtWord anm_para_snd[72];
EvtWord anm_rapa_snd[72];
EvtWord puni_switch[133];
EvtWord puni_nakama[93];
EvtWord tik03_switch_evt[70];
EvtWord tik_03_init_evt[116];

// Returning after the boss clears the visit-local gift and game-win flags.
EVT_BEGIN(bero_05_custom_in)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_25_AFTER_THE_BOSS_BATTLE)
        IF_INT_EQUAL(GSWF_MRI_LEFT_AFTER_BOSS_BATTLE, 0)
            SET_VALUE(GSWF_MRI_LEFT_AFTER_BOSS_BATTLE, 1)
        END_IF
        IF_INT_EQUAL(GSWF_MRI_04_BABY_JABBI_GIFT_THIS_VISIT, 1)
            SET_VALUE(GSWF_MRI_04_BABY_JABBI_GIFT_THIS_VISIT, 0)
        END_IF
        IF_INT_EQUAL(GSWF_MRI_PETUNI_GAME_WON_THIS_VISIT, 1)
            SET_VALUE(GSWF_MRI_PETUNI_GAME_WON_THIS_VISIT, 0)
        END_IF
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(bero_05_custom_out)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_35_PUNI_JOINS)
        SET_VALUE(GSW_STORY_PROGRESS, STG1_END_TO_STAGE_2)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero_1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_04", // nextMapName
        "n_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_05_custom_in, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "win_06", // nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Stand-in for the Boggly Woods pipe until chapter 2 has begun: it leads to the chapter 2 title screen
 * (sys_01, stg_2) instead. Swapped in by the map init script.
 */
static BeroEntry bero_entry_data2[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_E, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        bero_05_custom_out, // exitEvent
        "sys_01", // nextMapName
        "stg_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Init script of the guide Puni (ガイド) for tik_03.
 * Places him by story step: (-80, -149, 50) at STG1_32, (100, -150, 50) at STG1_33,
 * (230, -50, 80) at STG1_34, and out of sight otherwise.
 */
static EVT_BEGIN(guide_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG1_32_PUNI_ENCOUNTER_2)
        CALL(evt_npc_set_position, STRING("me"), -80, -149, 50)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_EQUAL(STG1_33_A_PUNI_PLEA_FOR_HELP)
        CALL(evt_npc_set_position, STRING("me"), 100, -150, 50)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CASE_EQUAL(STG1_34_PUNIS_SECRET_PASSAGE)
        CALL(evt_npc_set_position, STRING("me"), 230, -50, 80)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the guide Puni.
 * At STG1_33_A_PUNI_PLEA_FOR_HELP, after a second, he starts puni_switch; at other steps it does nothing.
 */
static EVT_BEGIN(guide_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG1_33_A_PUNI_PLEA_FOR_HELP)
        WAIT_MS(1000)
        CALL_CHILD(PTR(puni_switch))
    END_SWITCH
    RETURN
EVT_END()

/** NPCs of this room: Punio, the Puni who leads Mario to Boggly Woods. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"ガイド"*/"\x83\x4B\x83\x43\x83\x68", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        guide_init, // initEvent
        NULL, // regularEvent
        guide_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Footstep sound loop for the guide Puni: plays SFX_STG2_PUNI_MOVE2_1 at his position repeatedly while he walks.
 * Started and stopped by the scenes.
 */
static EVT_BEGIN(puni_walk_se)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_MOVE2_1"), LW(0), LW(1), LW(2), 0)
        WAIT_FRAMES(5)
    END_LOOP
    RETURN
EVT_END()

/**
 * Third meeting with the guide Puni (ガイド), at STG1_32_PUNI_ENCOUNTER_2.
 * He appears in the cave, runs off, and Mario follows him to the dead end
 * where he asks for help (mac_1_012 to mac_1_029_00, with a yes/no prompt at mac_1_014).
 * Ends with the music back to the sewer theme and story progress STG1_33_A_PUNI_PLEA_FOR_HELP.
 */
EVT_BEGIN(puni_soguu_3)
    // Mario arrives; Punio spots him and scurries off to the west end of the room.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(200)
    CALL(evt_mario_normalize)
    WAIT_MS(200)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ガイド"), STRING("R_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_key_onoff, 1)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    SPAWN_EVENT_GET_ID(PTR(puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), -455, 59, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(10))
    SET_VALUE(GW(0), 0)
    // When Mario follows him west, Punio turns back and forth in a panic until Mario catches up.
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(2), 0)
                IF_INT_LESS(LW(0), -370)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_normalize)
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 270, -1)
        SET_VALUE(GW(0), 1)
    END_INLINE_EVENT
    LOOP(GW(0))
        CALL(evt_npc_reverse_ry, STRING("ガイド"))
        WAIT_MS(800)
    END_LOOP
    // Mario and the partner walk up to Punio, the music changes, and he begs them not to eat him.
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("ガイド"))
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), 10)
        CALL(evt_mario_mov_pos2, LW(0), 10, FLOAT(120.0f))
    END_IF
    CALL(evt_mario_set_dir_npc, STRING("ガイド"))
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_anim, STRING("ガイド"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG2_PUNI1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_msg_print, 0, STRING("mac_1_012"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -400, -35, 432, -418, -105, 24, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    // Punio asks if they are bullies; answering "Guess again, nerd!" scares him and the partner scolds Mario.
    CALL(evt_msg_print, 0, STRING("mac_1_013"), 0, STRING("ガイド"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_1_014"), 0, STRING("ガイド"))
    CALL(evt_msg_select, 0, STRING("mac_1_014_yesno"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("mac_1_019"))
        ELSE
        CALL(evt_msg_print_add, 0, STRING("mac_1_015"))
        CALL(evt_mario_set_dir, 90, 300, 0)
        CALL(evt_msg_print_party, STRING("mac_1_016"))
        CALL(evt_mario_set_dir, 270, 300, 0)
        CALL(evt_msg_print_party, STRING("mac_1_017"))
        CALL(evt_msg_print, 0, STRING("mac_1_018"), 0, STRING("ガイド"))
    END_IF
    // The partner asks why a Puni from Boggly Woods is here; Punio tells of the X-Nauts in the Great Tree.
    WAIT_MS(333)
    CALL(evt_msg_print_party, STRING("mac_1_020"))
    CALL(evt_cam3d_evt_set, -435, -78, 289, -435, -130, -7, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_1_021"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -400, -33, 432, -400, -105, 24, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_1_022"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("mac_1_023"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -400, -33, 432, -400, -105, 24, 500, EASING_SINE_IN_OUT)
    WAIT_MS(333)
    // The partner links them to the Crystal Star; Punio asks for help and Mario nods twice to agree.
    CALL(evt_msg_print_party, STRING("mac_1_024"))
    CALL(evt_cam3d_evt_set, -445, -90, 257, -445, -141, -42, 0, EASING_SINE_IN_OUT)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_1_025"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -400, -23, 432, -400, -95, 24, 0, EASING_SINE_IN_OUT)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_1_026"), 0, STRING("ガイド"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 25)
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(60.0f))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_1_027"), 0, STRING("ガイド"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    // Punio thanks them, gives his name and leads the way east as Mario and the partner turn to follow.
    CALL(evt_msg_print_party, STRING("mac_1_028"))
    CALL(evt_msg_print, 0, STRING("mac_1_029"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, 100, -23, 432, 100, -95, 24, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), 100, 50, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(10))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_1_029_00"), 0, STRING("ガイド"))
    CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_MS(800)
    // The sewer music and control return, and story progress moves on to Punio's plea for help.
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_33_A_PUNI_PLEA_FOR_HELP)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Sound effects for the secret passage (para) appearing:
 * a SFX_STG1_LOAD_APPEAR1 step sound at each of eight positions from X = 55 to 425 and at -454, in sequence,
 * timed to the opening animation.
 */
EVT_BEGIN(anm_para_snd)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 55, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 115, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 175, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 245, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 305, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 365, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 425, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), -454, -50, -90, 0)
    RETURN
EVT_END()

/** The same as anm_para_snd played in reverse order, for the passage closing again. */
EVT_BEGIN(anm_rapa_snd)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), -454, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 425, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 365, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 305, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 245, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 175, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 115, -50, -90, 0)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_LOAD_APPEAR1"), 55, -50, -90, 0)
    RETURN
EVT_END()

/**
 * Scene where the guide Puni shows Mario the secret passage (STG1_33).
 * He walks to a switch at X = 220 and steps on it (mac_1_029_01, mac_1_030);
 * the camera shakes, success jingle plays, the wall (kabe_1) is replaced and the passage (para) opens.
 * Sets GSWF_TIK03_SECRET_DOOR_OPEN and story progress STG1_34_PUNIS_SECRET_PASSAGE.
 */
EVT_BEGIN(puni_switch)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_msg_print, 0, STRING("mac_1_029_01"), 0, STRING("me"))
    CALL(evt_npc_set_ry, STRING("me"), 90)
    SPAWN_EVENT_GET_ID(PTR(puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("me"), 220, 50, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(10))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_1_030"), 0, STRING("me"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BIKKURI_SWITCH_ON1"), 220, 50, 0, 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.01953125f), FLOAT(0.0f), 100)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    SET_VALUE(GSWF_TIK_03_SECRET_PASSAGE_OPEN, 1)
    CALL(evt_map_playanim, STRING("anm_para"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kabe_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kabe_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_para_d"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_para_d1"), 1, 0)
    CALL_CHILD(PTR(anm_para_snd))
    CALL(evt_npc_set_position, STRING("me"), 230, -50, 80)
    CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_34_PUNIS_SECRET_PASSAGE)
    CALL(evt_mario_key_onoff, 1)
    WAIT_MS(1500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("para"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    RETURN
EVT_END()

/**
 * Waits until Mario reaches the end of the passage (X >= 300, Z >= -90) at STG1_34,
 * then the guide Puni joins as the second partner (mac_1_031): his NPC is removed and he follows Mario.
 * Sets story progress STG1_35_PUNI_JOINS.
 */
// Replace the guide NPC with the extra partner only after Mario reaches the open passage.
EVT_BEGIN(puni_nakama)
    LOOP(0)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_34_PUNIS_SECRET_PASSAGE)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(2), -90)
                IF_INT_GREATER_OR_EQUAL(LW(0), 300)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 1)
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    CALL(evt_mario_set_party_pos, 1, PARTY_MEMBER_PUNIO, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 35)
    ADD(LW(2), 30)
    CALL(evt_party_move_pos2, 1, LW(0), LW(2), FLOAT(120.0f))
    CALL(evt_msg_print, 0, STRING("mac_1_031"), 0, STRING("extparty"))
    WAIT_MS(200)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_35_PUNI_JOINS)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 1)
    RETURN
EVT_END()

/**
 * The floor switch for the secret passage.
 * Toggles it: pressed while closed, it plays the opening animation (anm_para),
 * hides the wall and sets GSWF_TIK03_SECRET_DOOR_OPEN;
 * pressed while open, it plays the closing animation (anm_rapa), clears the flag and restores the hit areas.
 */
EVT_BEGIN(tik03_switch_evt)
    IF_INT_EQUAL(GSWF_TIK_03_SECRET_PASSAGE_OPEN, 0)
        CALL(evt_map_playanim, STRING("anm_para"), MAP_ANIM_PLAY_ONCE, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kabe_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kabe_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_para_d"), 1, 0)
        SET_VALUE(GSWF_TIK_03_SECRET_PASSAGE_OPEN, 1)
        CALL_CHILD(PTR(anm_para_snd))
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("para"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("para"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING("anm_rapa"), MAP_ANIM_PLAY_ONCE, 0)
        SET_VALUE(GSWF_TIK_03_SECRET_PASSAGE_OPEN, 0)
        CALL_CHILD(PTR(anm_rapa_snd))
        WAIT_MS(800)
        CALL(evt_hitobj_onoff, STRING("a_para_d"), 0, 1)
        CALL(evt_hitobj_onoff, STRING("a_para_d1"), 0, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Map init script of tik_03.
 * Starts the sewer music and ambience, sets up the exits and the guide Puni,
 * runs puni_soguu_3 and puni_nakama at their story steps,
 * restores the passage state from GSWF_TIK03_SECRET_DOOR_OPEN,
 * and places the Damage Dodge badge and the Star Piece panel.
 * Before Boggly Woods (STG2_00_BOGGLY_WOODS) the pipe exit (dokan) is switched off
 * and its destination replaced.
 */
EVT_BEGIN(tik_03_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_32_PUNI_ENCOUNTER_2)
        SPAWN_EVENT(PTR(puni_soguu_3))
    END_IF
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG1_33_A_PUNI_PLEA_FOR_HELP)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kabe_2"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        IF_INT_EQUAL(GSWF_TIK_03_SECRET_PASSAGE_OPEN, 0)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kabe_1"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kabe_1"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("para"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_para_d"), 1, 0)
            CALL(evt_hitobj_onoff, STRING("a_para_d1"), 1, 0)
        END_IF
    END_IF
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG1_34_PUNIS_SECRET_PASSAGE)
        SPAWN_EVENT(PTR(puni_nakama))
    END_IF
    CALL(evt_item_entry, STRING("item_00"), ITEM_DAMAGE_DODGE, -450, -50, -50, ITEM_TYPE_STILL, GSWF_TIK_03_DAMAGE_DODGE, 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 325, -50, 0, STRING("kakoi"), 125, GSWF_STARPIECE_23)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_00_BOGGLY_WOODS)
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan"), PTR(bero_entry_data2))
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()

