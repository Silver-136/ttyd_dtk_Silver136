// win_00 (ウィンディん家周辺, Boggly Woods): the clearing in the middle of the woods, where the Shadow Sirens (Beldam, Marilyn,
// Vivian) stop Mario; paths to win_01 and win_06.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_story.h"
#include "evt/evt_urouro.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"

#include "driver/itemdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "npc_event.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/seqdrv.h"


USER_FUNC(item_check_necklace_exist);
USER_FUNC(win_item_move_3d_position);
USER_FUNC(mariowalk_wait);

static EvtWord vivian_init[37];
static EvtWord vivian_talk[8];
static EvtWord maririn_init[19];
EvtWord maririn_talk[8];
EvtWord majorin_init[19];
EvtWord majorin_talk[8];
static EvtWord fabio_init[21];
static EvtWord fabio_talk[26];
static EvtWord kizayarou_regl[19];
static EvtWord kizayarou_talk[8];
static EvtWord kizayarou_init[38];
static NpcSetupInfo npcEnt[5];
static BeroEntry bero_entry_data[3];
EvtWord witchtrio_momeru[880];
EvtWord majorin_have_picture[25];
EvtWord witchtrio_picture[533];
EvtWord mario_vs_witchtrio[554];
EvtWord get_necklace[115];
EvtWord witchtrio_lose_item[19];
EvtWord witchtrio_mess_vivi[8];
EvtWord witchtrio_mess_mari[8];
EvtWord witchtrio_lose[563];
static EvtWord ki_data_evt_01[20];
static TreeAccess tree_access_data[10];
EvtWord iri_28_enemy_all_dead[148];
EvtWord iri_28_enemy_dead[13];
EvtWord iri_28_init[60];

#if VERSION <= VERSION_DEMO_US
extern EvtWord win_00_init_evt[];
#else
EvtWord win_00_init_evt[85];
#endif

/**
 * Init script of Vivian in Boggly Woods: clears her battle-info flag.
 * From STG2_01 (after Beldam takes her necklace in witchtrio_momeru) to STG2_05_02 she stands at
 * (-535, 0, -30) in the pose PTR_S_3 (talk pose PTR_T_3), the pose she is left in at the end of that scene;
 * from STG2_06 to STG2_07 she is only placed there.
 */
static EVT_BEGIN(vivian_init)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_01_A_SIREN_ARGUMENT, STG2_05_02_FLURRIE_REQUEST)
        CALL(evt_npc_set_position, STRING("me"), -535, 0, -30)
        CALL(evt_npc_set_force_regl_anim, STRING("me"), STRING("PTR_S_3"))
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("PTR_S_3"), STRING("PTR_T_3"))
        CASE_BETWEEN(STG2_06_THE_SKETCH, STG2_07_MARIO_VS_SHADOW_SIRENS)
        CALL(evt_npc_set_position, STRING("me"), -535, 0, -30)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of Vivian: one line, "stg2_win_e01_18". */
static EVT_BEGIN(vivian_talk)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_18"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Init script of Marilyn in Boggly Woods: clears her battle-info flag and,
 * between STG2_01 and STG2_07, places her at (-495, 0, -60).
 */
static EVT_BEGIN(maririn_init)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_01_A_SIREN_ARGUMENT, STG2_07_MARIO_VS_SHADOW_SIRENS)
        CALL(evt_npc_set_position, STRING("me"), -495, 0, -60)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of Marilyn, standing in the Boggly Woods clearing: one line, "stg2_win_e01_17". */
EVT_BEGIN(maririn_talk)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_17"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Init script of Beldam (Majolyne) in Boggly Woods:
 * clears her battle-info flag and, during the Shadow Sirens' first appearance (STG2_01 to STG2_07),
 * moves her to (-455, 0, -30).
 */
EVT_BEGIN(majorin_init)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_01_A_SIREN_ARGUMENT, STG2_07_MARIO_VS_SHADOW_SIRENS)
        CALL(evt_npc_set_position, STRING("me"), -455, 0, -30)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of Beldam: one line, "stg2_win_e01_16". */
EVT_BEGIN(majorin_talk)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_16"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Init script of Fabio, the client of trouble 28:
 * while the request is active he is placed at (-455, 0, -30), facing west.
 */
static EVT_BEGIN(fabio_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ROUST_THESE_CADS)
        CALL(evt_npc_set_position, STRING("me"), -455, 0, -30)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of Fabio during trouble 28: the first talk gives the request ("irai_28_001")
 * and sets GSWF_TROUBLE_WORK_BASE; later talks remind Mario ("irai_28_002").
 */
static EVT_BEGIN(fabio_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ROUST_THESE_CADS)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_28_001"), 0, STRING("me"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_28_002"), 0, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/** Regular behavior of the dandy in Boggly Woods: wanders around his start position (100 units across, 30 deep). */
static EVT_BEGIN(kizayarou_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the dandy in Boggly Woods: one line, "stg2_win_00_01". */
static EVT_BEGIN(kizayarou_talk)
    CALL(evt_msg_print, 0, STRING("stg2_win_00_01"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Spawns the dandy NPC ("キザ野郎", model c_kiza) at (-450, 0, -25) facing west and installs his talk
 * and wander scripts.
 */
static EVT_BEGIN(kizayarou_init)
    SET_VALUE(LW(0), STRING("キザ野郎"))
    CALL(evt_npc_entry, LW(0), STRING("c_kiza"))
    CALL(evt_npc_set_tribe, LW(0), STRING("キザ野郎"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(kizayarou_talk))
    CALL(evt_npc_change_interrupt, LW(0), 1, PTR(kizayarou_regl))
    CALL(evt_npc_set_position, LW(0), -450, 0, -25)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * NPCs of this map: the Shadow Sirens Vivian, Marilyn and Beldam, who appear here during chapter 2,
 * and Fabio, the client of trouble 28.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "ビビアン", // name
        NPC_FLAG_TALK_POSE, // flags
        0, // reactionFlags
        vivian_init, // initEvent
        NULL, // regularEvent
        vivian_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "マリリン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        maririn_init, // initEvent
        NULL, // regularEvent
        maririn_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "マジョリン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        majorin_init, // initEvent
        NULL, // regularEvent
        majorin_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ファビオ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        fabio_init, // initEvent
        NULL, // regularEvent
        fabio_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Exits of this map: west to win_06 (the pipe from Rogueport Sewers) and east to win_01. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_06", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * The Shadow Sirens' first scene, spawned by the init script up to STG2_00:
 * once Mario has entered, he and both partners walk in
 * and spot the Sirens (Vivian, Marilyn and Beldam at x -535 to -455) as their theme plays.
 * Beldam names their goals (find Mario, steal the map), Vivian shows a necklace she found by a tree,
 * they bicker over who lost the sketch, Marilyn points out a mustached man,
 * and Beldam snatches the necklace as punishment, leaving Vivian in tears (stg2_win_e01_00 to 14).
 * The story becomes STG2_01_A_SIREN_ARGUMENT.
 */
EVT_BEGIN(witchtrio_momeru)
    // The three Sirens stand in the clearing, turned toward each other.
    CALL(evt_npc_set_position, STRING("ビビアン"), -535, 0, -30)
    CALL(evt_npc_set_position, STRING("マリリン"), -495, 0, -60)
    CALL(evt_npc_set_position, STRING("マジョリン"), -455, 0, -30)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)

    // Mario's controls are locked until he has finished entering the map.
    CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
#endif
    CALL(evt_mario_normalize)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
#endif
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)

    // Mario, his partner and Punio walk in and spot the Sirens.
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -761, -35, FLOAT(120.0f))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 1, -785, 18, FLOAT(120.0f))
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -733, 0, FLOAT(120.0f))
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_FOLLOWER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(500)

    // The camera turns to the Sirens and their theme plays; Beldam quizzes Vivian on their goals:
    // find Mario and steal the map to the Crystal Stars.
    CALL(evt_cam3d_evt_set, FLOAT(-497.5f), 115, FLOAT(325.7998f), FLOAT(-497.5f), 45, -68, 500, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_WITCH1"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_00"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_01"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_02"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -465, 85, 227, -465, 37, -68, 500, EASING_LINEAR)
    WAIT_MS(1000)

    // Beldam notices Vivian fawning over something: a necklace Vivian found near a tree.
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_02_01"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -520, 85, 227, -520, 37, -68, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_U_1"))
    WAIT_MS(150)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_VIVI_NECKLACE1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(0), 17)
    ADD(LW(1), 11)
    ADD(LW(2), 5)
    CALL(evt_item_entry, STRING("necklace"), ITEM_NECKLACE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("necklace"), ITEM_FLAG_NO_TOUCH_GET)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_5"), STRING("PTR_T_6"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_03"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, FLOAT(-497.5f), 115, FLOAT(325.7998f), FLOAT(-497.5f), 45, -68, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_item_set_position, STRING("necklace"), 0, -1000, 0)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)

    // Beldam scolds her for picking up scraps and asks for the sketch of Mario;
    // Vivian insists Beldam took it, and Beldam blames her.
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_04"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_H_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_H_1"), STRING("PTR_T_2"))
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_06"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マジョリン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("マジョリン"), LW(0), LW(1), LW(2), 225, 30)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_07"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_08"), 0, STRING("ビビアン"))
    WAIT_MS(200)

    // Marilyn tries to point out Mario, who is puzzled; Beldam brushes her off.
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マリリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set_rel, 0, 70, 393, 0, 45, FLOAT(-20.799805f), 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, FLOAT(-497.5f), 100, FLOAT(227.39941f), FLOAT(-497.5f), FLOAT(47.59961f), -68, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_09"), 0, STRING("マリリン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -465, 85, 227, -465, 37, -68, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_10"), 0, STRING("マジョリン"))
    WAIT_MS(200)

    // Beldam accuses Vivian of losing the sketch and shouts her down.
    CALL(evt_cam3d_evt_set, FLOAT(-497.5f), 115, FLOAT(325.7998f), FLOAT(-497.5f), 45, -68, 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 25)
    CALL(evt_npc_move_position, STRING("マジョリン"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_11"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_12"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_13"), 0, STRING("マジョリン"))
    WAIT_MS(200)

    // As punishment Beldam snatches the necklace from Vivian, who is left in tears.
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_I_1"))
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_MAJO_NECKLACE_STEAL2"), LW(3), LW(4), LW(5), 0)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_H_1"))
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_MAJO_NECKLACE_STEAL1"), LW(0), LW(1), LW(2), LW(10))
    ADD(LW(0), 5)
    ADD(LW(3), -5)
    ADD(LW(1), 12)
    ADD(LW(4), 8)
    CALL(evt_item_set_position, STRING("necklace"), LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING(""), STRING("stardust_n64"), 1, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 0, 30, 375)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(10), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 30, 10, 375)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(10), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(win_item_move_3d_position, STRING("necklace"), LW(3), LW(4), LW(5), 750, EASING_LINEAR, GW(10))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_S_1"))
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_4"))
    CALL(evt_item_set_position, STRING("necklace"), 0, -1000, 0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(evt_npc_move_position, STRING("マジョリン"), LW(0), LW(2), 500, 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_13_01"), 0, STRING("マジョリン"))
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_3"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_3"), STRING("PTR_T_3"))
    CALL(evt_npc_set_force_regl_anim, STRING("ビビアン"), STRING("PTR_S_3"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(400)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e01_14"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(300)

    // The field music returns, the story advances and Mario's party can move again.
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ビビアン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_01_A_SIREN_ARGUMENT)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_FIELD)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

/**
 * Background loop that keeps the "picture" item (the Mario poster) glued to Beldam's position,
 * 20 units above her, while she holds it.
 */
EVT_BEGIN(majorin_have_picture)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        ADD(LW(2), 5)
        CALL(evt_item_set_position, STRING("picture"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Sirens-only scene at STG2_05_02 (Mario and the partners are moved off the map):
 * Vivian complains that Beldam had the sketch all along,
 * Beldam holds up the Mario poster (majorin_have_picture keeps it in her hands) and they study it,
 * then Beldam blames Vivian, refuses to give back the necklace and orders her to find Mario
 * (stg2_win_e06_00 to 12). The story becomes STG2_06_THE_SKETCH.
 */
EVT_BEGIN(witchtrio_picture)
    // Mario and his party are moved off the map: the scene shows the three Sirens alone.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, -495, 165, 582, -495, 45, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ビビアン"), -535, 0, -30)
    CALL(evt_npc_set_position, STRING("マリリン"), -495, 0, -60)
    CALL(evt_npc_set_position, STRING("マジョリン"), -455, 0, -30)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT | NPC_FLAG_VIVIAN_TYPE | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_VIVIAN_TYPE | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_VIVIAN_TYPE | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)

    // Vivian protests that Beldam had the sketch all along; Beldam tells her to study it instead.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_4"), STRING("PTR_T_5"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_00"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set, -460, 90, 237, -460, 30, -108, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マジョリン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_01"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -495, 90, 237, -495, 30, -108, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_02"), 0, STRING("マリリン"))

    // Beldam holds up the sketch of Mario and the three gather round;
    // Beldam admires the mustached man and thinks she has seen him before.
    CALL(evt_item_entry, STRING("picture"), ITEM_MARIO_POSTER, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("picture"), ITEM_FLAG_NO_TOUCH_GET)
    SPAWN_EVENT_GET_ID(PTR(majorin_have_picture), LW(10))
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_I_1"))
    WAIT_MS(500)
    DELETE_EVENT(LW(10))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_S_1"))
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    CALL(evt_item_set_position, STRING("picture"), LW(0), LW(1), LW(2))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マジョリン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マリリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ビビアン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マジョリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_03"), 0, STRING("マジョリン"))

    // Marilyn realizes they already met him; Beldam blames Vivian and promises punishment.
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
        CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
    END_INLINE_EVENT
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_04"), 0, STRING("マリリン"))
    CALL(evt_item_set_position, STRING("picture"), 0, -1000, 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_05"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -525, 90, 237, -525, 30, -108, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_06"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set, -460, 90, 237, -460, 30, -108, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_07"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -495, 90, 237, -495, 30, -108, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_08"), 0, STRING("ビビアン"))
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)

    // Beldam refuses to give the necklace back and orders Vivian to find Mario.
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_09"), 0, STRING("マジョリン"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_3"), STRING("PTR_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_10"), 0, STRING("ビビアン"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_11"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e06_12"), 0, STRING("マリリン"))

    // The camera pulls back, the story advances and the scene ends with a change to win_03.
    CALL(evt_cam3d_evt_set, -495, 165, 582, -495, 45, -108, 1500, EASING_SINE_IN_OUT)
    WAIT_MS(1200)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_06_THE_SKETCH)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("win_03"), STRING("n_bero"))
    RETURN
EVT_END()

/**
 * Trigger spawned at STG2_06: when Mario reaches x <= -325 the Shadow Sirens confront him;
 * the partner reacts, Vivian botches their name ("Shadow Beauties") and Beldam scolds her
 * (stg2_win_e07_00 to 08).
 * Then the fight starts (Beldam's battle info 4);
 * afterwards the story becomes STG2_07_MARIO_VS_SHADOW_SIRENS and witchtrio_lose starts.
 */
EVT_BEGIN(mario_vs_witchtrio)
    // The Sirens wait here until Mario walks west past x -325.
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_CULL | NPC_FLAG_NO_WALL_COLLISION)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), -325)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP

    // Everyone stops; Mario and his party face west as the Sirens notice them.
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_adjust_dir)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 1, 270, 200)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マジョリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マリリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ビビアン"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    WAIT_MS(500)

    // Beldam taunts Mario, the partner recognizes the Sirens, and Beldam says she will take the map.
    CALL(evt_cam3d_evt_set, -495, 93, 252, -495, 45, -19, 500, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_00"), 0, STRING("マジョリン"))
    CALL(evt_mario_set_pos, -325, 0, -10)
    CALL(evt_party_set_pos, 0, -277, 0, -10)
    CALL(evt_party_set_pos, 1, -300, 0, 0)
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -307, FLOAT(86.399414f), FLOAT(226.19922f), -307, FLOAT(42.0f), FLOAT(-19.0f), 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg2_win_e07_01"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -400, 155, 572, -400, 50, -19, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_1"), STRING("PTR_T_1"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg2_win_e07_02_01"))
    WAIT_MS(200)

    // Beldam introduces the trio, but Vivian finishes the line as "Shadow Beauties".
    CALL(evt_cam3d_evt_set, -451, FLOAT(88.59961f), FLOAT(274.0f), -451, FLOAT(35.5f), FLOAT(-19.0f), 0, EASING_LINEAR)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_02"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -495, FLOAT(105.09961f), FLOAT(274.8994f), -495, FLOAT(52.69922f), FLOAT(-19.299805f), 200, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_03"), 0, STRING("マリリン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -528, FLOAT(95.5f), FLOAT(274.8994f), -528, FLOAT(42.5f), FLOAT(-19.299805f), 200, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_04"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -307, FLOAT(86.399414f), FLOAT(226.19922f), -307, FLOAT(42.0f), FLOAT(-19.0f), 0, EASING_LINEAR)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg2_win_e07_02_05"))
    WAIT_MS(500)

    // Beldam corrects her (Shadow SIRENS), calls her ugly and promises punishment; Vivian sulks.
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_06"), 0, STRING("マジョリン"))
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_H_1"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, -495, 103, 276, -495, 50, -19, 500, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_07"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_3"), STRING("PTR_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_08"), 0, STRING("ビビアン"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_09"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_B_1"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_B_2"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_B_2"), STRING("PTR_T_4"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_02_10"), 0, STRING("ビビアン"))
    WAIT_MS(200)

    // Beldam rallies her sisters to deal with Mario first.
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_06"), 0, STRING("マジョリン"))
    WAIT_MS(200)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg2_win_e07_08"), 0, STRING("ビビアン"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e07_07"), 0, STRING("マリリン"))
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))

    // The three Sirens charge at Mario and the battle against them starts.
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        WAIT_MS(400)
        CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
        ADD(LW(0), 160)
        CALL(evt_npc_move_position, STRING("マジョリン"), LW(0), LW(2), 0, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
        ADD(LW(0), 160)
        CALL(evt_npc_move_position, STRING("マリリン"), LW(0), LW(2), 0, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(12))
        CALL(evt_npc_flag_onoff, 0, STRING("ビビアン"), NPC_FLAG_AUTO_MOVE_ANIM)
        WAIT_FRAMES(1)
        CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_R_3"))
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        ADD(LW(0), 160)
        CALL(evt_npc_move_position, STRING("ビビアン"), LW(0), LW(2), 0, 200, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -375, 144, 533, -375, 40, -58, 750, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_npc_set_battle_info, STRING("マジョリン"), 4)
    CALL(evt_npc_battle_start, STRING("マジョリン"))
    CALL(evt_npc_wait_battle_end)

    // After the battle Mario can move again, and witchtrio_lose plays the Sirens' defeat.
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    DELETE_EVENT(LW(12))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_07_MARIO_VS_SHADOW_SIRENS)
    SPAWN_EVENT(PTR(witchtrio_lose))
    RETURN
EVT_END()

EVT_BEGIN(get_necklace)
    CALL(item_check_necklace_exist)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_get_pos, 1, LW(10), LW(11), LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
            CALL(evt_mario_set_dir, 90, 200, 0)
        ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_get_pos, 1, LW(10), LW(11), LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
            CALL(evt_party_set_dir, 0, 90, 200)
        ELSE
            CALL(evt_party_set_dir, 0, 270, 200)
        END_IF
    END_INLINE_EVENT
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
        CALL(evt_party_set_dir, 1, 90, 200)
    ELSE
        CALL(evt_party_set_dir, 1, 270, 200)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg2_win_e08_03"), 0, STRING("extparty"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_FIELD)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

/**
 * Background loop that keeps the "necklace" item at the position of the dummy Beldam NPC ("ダミー")
 * while she carries it.
 */
EVT_BEGIN(witchtrio_lose_item)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("necklace"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Message helper: Vivian says "stg2_win_e08_02". */
EVT_BEGIN(witchtrio_mess_vivi)
    CALL(evt_msg_print, 0, STRING("stg2_win_e08_02"), 0, STRING("ビビアン"))
    RETURN
EVT_END()

/** Message helper: Marilyn says "stg2_win_e08_01". */
EVT_BEGIN(witchtrio_mess_mari)
    CALL(evt_msg_print, 0, STRING("stg2_win_e08_01"), 0, STRING("マリリン"))
    RETURN
EVT_END()

EVT_BEGIN(witchtrio_lose)
    // The Sirens and Mario's party are placed for the aftermath,
    // with a hidden stand-in Beldam that will carry the necklace.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("ビビアン"), -438, 0, -10)
    CALL(evt_npc_set_position, STRING("マリリン"), -425, 0, -60)
    CALL(evt_npc_set_position, STRING("マジョリン"), -387, 0, -30)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_mario_set_pos, -335, 0, -50)
    CALL(evt_party_set_pos, 0, -287, 0, -50)
    CALL(evt_party_set_pos, 1, -310, 0, -40)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_party_set_dir, 1, 270, -1)
    CALL(evt_npc_entry, STRING("ダミー"), STRING("c_majyorin"))
    CALL(evt_npc_set_position, STRING("ダミー"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_4"), STRING("PTR_T_4"))

    // The necklace is created out of sight; Marilyn and Vivian are left dizzy.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
    CALL(evt_cam3d_evt_set, -375, 144, 533, -375, 40, -58, 0, EASING_SINE_IN_OUT)
    CALL(evt_item_entry, STRING("necklace"), ITEM_NECKLACE, 0, -1000, 0, ITEM_TYPE_STILL, GSWF_WIN_00_NECKLACE, PTR(get_necklace))
    CALL(evt_item_flag_on, STRING("necklace"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GURUGURU_MARK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("effmaririn"), STRING("マリリン"), 2, 0, 0, 0, 0, 0, 0, -1)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GURUGURU_MARK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("effvivian"), STRING("ビビアン"), 2, 0, 0, 0, 0, 0, 0, -1)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)

    // Beldam blames her sisters and flees west; the necklace falls from her and bounces toward Mario.
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マジョリン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg2_win_e08_00"), 0, STRING("マジョリン"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
        CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SISTERS_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(0), -100)
        CALL(evt_npc_move_position, STRING("マジョリン"), LW(0), LW(2), 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, STRING("マジョリン"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(10), -361)
        SUBTRACT(LW(10), LW(0))
        DIVIDE(LW(10), 10)
        SET_VALUE(LW(11), LW(10))
        MULTIPLY(LW(11), 5)
        ADD(LW(11), LW(0))
        SET_VALUE(LW(12), LW(10))
        MULTIPLY(LW(12), 7)
        ADD(LW(12), LW(0))
        SET_VALUE(LW(13), LW(10))
        MULTIPLY(LW(13), 9)
        ADD(LW(13), LW(0))
        CALL(evt_npc_set_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        SPAWN_EVENT_GET_ID(PTR(witchtrio_lose_item), LW(15))
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(11), 0, LW(2), 625, FLOAT(30.0f))
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(12), 0, LW(2), 250, FLOAT(15.0f))
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(13), 0, LW(2), 250, FLOAT(7.5f))
        DELETE_EVENT(LW(15))
        CALL(evt_npc_set_position, STRING("ダミー"), 0, -1000, 0)
        CALL(evt_item_flag_off, STRING("necklace"), ITEM_FLAG_NO_TOUCH_GET)
        CALL(evt_item_set_position, STRING("necklace"), LW(13), 0, LW(2))
    END_INLINE_EVENT

    // Marilyn comes to and runs off after her.
    BEGIN_INLINE_EVENT
        CALL(evt_eff_softdelete, STRING("effmaririn"))
        CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("マリリン"), LW(0), LW(1), LW(2), 225, 30)
        SPAWN_EVENT(PTR(witchtrio_mess_mari))
        CALL(evt_npc_set_ry, STRING("マリリン"), 270)
        CALL(evt_npc_get_position, STRING("マリリン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SISTERS_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(0), -100)
        CALL(evt_npc_move_position, STRING("マリリン"), LW(0), LW(2), 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, STRING("マリリン"), 0, -1000, 0)
    END_INLINE_EVENT

    // Vivian comes to, starts to run, stops to look back at Mario, then flees too.
    WAIT_MS(425)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_eff_softdelete, STRING("effvivian"))
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
        SPAWN_EVENT(PTR(witchtrio_mess_vivi))
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SISTERS_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(0), -25)
        CALL(evt_npc_move_position, STRING("ビビアン"), LW(0), LW(2), 0, FLOAT(240.0f), 0)
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SISTERS_ESCAPE2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_F_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_4"))
        CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 225, 30)
        CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SISTERS_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(0), -100)
        CALL(evt_npc_move_position, STRING("ビビアン"), LW(0), LW(2), 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, STRING("ビビアン"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))

    // Once all three are gone the Boggly Woods music returns, the story advances and Mario can move again.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
    SET_VALUE(GSW_STORY_PROGRESS, STG2_08_SHADOW_SIRENS_LOSE)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_FIELD)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

static EVT_BEGIN(ki_data_evt_01)
    CALL(evt_item_delete_check, STRING("item_ki01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki01"), ITEM_SLEEPY_SHEEP, -694, 111, -130, ITEM_TYPE_FALL_NO_PICKUP, GSWF_WIN_00_SLEEPY_SHEEP_TREE, 0)
    END_IF
    RETURN
EVT_END()

/** Trees of this map that shake when Mario hammers them; the first drops a Sleepy Sheep once. */
static TreeAccess tree_access_data[] = {
    {4, "A_tree_01", "S_tree_mki_01", "S_tree_ha01", 0, ki_data_evt_01},
    {4, "A_tree_02", "S_tree_mki_02", "S_tree_ha02", 0, NULL},
    {4, "A_tree_03", "S_tree_mki_03", "S_tree_ha03", 0, NULL},
    {4, "A_tree_04", "S_tree_mki_04", "S_tree_ha04", 0, NULL},
    {4, "A_tree_05", "S_tree_mki_05", "S_tree_ha05", 0, NULL},
    {4, "A_tree_06", "S_tree_mki_06", "S_tree_ha06", 0, NULL},
    {4, "A_tree_07", "S_tree_mki_07", "S_tree_ha07", 0, NULL},
    {4, "A_tree_08", "S_tree_mki_08", "S_tree_ha08", 0, NULL},
    {4, "A_tree_09", "S_tree_mki_09", "S_tree_ha09", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(iri_28_enemy_all_dead)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    SUBTRACT(LW(3), 300)
    IF_INT_LESS(LW(2), -70)
        SET_VALUE(LW(2), -70)
    END_IF
    CALL(evt_npc_set_position, STRING("ファビオ"), LW(3), LW(1), LW(2))
    SUBTRACT(LW(0), 30)
    CALL(evt_npc_move_position, STRING("ファビオ"), LW(0), LW(2), 0, 120, 0)
    CALL(evt_mario_set_dir_npc, STRING("ファビオ"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("irai_28_003"), 0, STRING("ファビオ"))
    CALL(evt_sub_get_coin, 20)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("irai_28_005"), 0, STRING("ファビオ"))
    WAIT_MS(200)
    CALL(evt_npc_flag_onoff, 1, STRING("ファビオ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ファビオ"), LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), 500)
        CALL(evt_npc_move_position, STRING("ファビオ"), 780, 0, 0, 150, 0)
        CALL(evt_npc_move_position, STRING("ファビオ"), 950, 0, 0, 150, 0)
        CALL(evt_npc_set_position, STRING("ファビオ"), 0, -1000, 0)
    ELSE
        ADD(LW(0), 300)
        CALL(evt_npc_move_position, STRING("ファビオ"), LW(0), LW(2), 0, 150, 0)
        CALL(evt_npc_set_position, STRING("ファビオ"), 0, -1000, 0)
    END_IF
    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_ROUST_THESE_CADS), 1)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Per-enemy death handler for trouble 28: runs the normal enemy death event,
 * counts the kill in GSW_TROUBLE_WORK_FIRST and starts iri_28_enemy_all_dead when the count reaches 3.
 */
EVT_BEGIN(iri_28_enemy_dead)
    CALL_CHILD(PTR(enemy_common_dead_event))
    ADD(GSW_TROUBLE_WORK(0), 1)
    IF_INT_EQUAL(GSW_TROUBLE_WORK(0), 3)
        SPAWN_EVENT(PTR(iri_28_enemy_all_dead))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_28_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ROUST_THESE_CADS)
        SET_VALUE(GSW_TROUBLE_WORK(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_check, STRING("ドクガッスン１"), LW(0))
#else
        CALL(evt_npc_check, STRING("パックン１"), LW(0))
#endif
        IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_npc_change_interrupt, STRING("ドクガッスン１"), 8, PTR(iri_28_enemy_dead))
#else
            CALL(evt_npc_change_interrupt, STRING("パックン１"), 8, PTR(iri_28_enemy_dead))
#endif
        ELSE
            ADD(GSW_TROUBLE_WORK(0), 1)
        END_IF
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_check, STRING("モノクロシンエモン"), LW(0))
#else
        CALL(evt_npc_check, STRING("シンエモン"), LW(0))
#endif
        IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_npc_change_interrupt, STRING("モノクロシンエモン"), 8, PTR(iri_28_enemy_dead))
#else
            CALL(evt_npc_change_interrupt, STRING("シンエモン"), 8, PTR(iri_28_enemy_dead))
#endif
        ELSE
            ADD(GSW_TROUBLE_WORK(0), 1)
        END_IF
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_check, STRING("ドクガッスン３"), LW(0))
#else
        CALL(evt_npc_check, STRING("パックン２"), LW(0))
#endif
        IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_npc_change_interrupt, STRING("ドクガッスン３"), 8, PTR(iri_28_enemy_dead))
#else
            CALL(evt_npc_change_interrupt, STRING("パックン２"), 8, PTR(iri_28_enemy_dead))
#endif
        ELSE
            ADD(GSW_TROUBLE_WORK(0), 1)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(win_00_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_WIN1"))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSWF_WIN_00_NECKLACE, 1)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG2_25_AFTER_THE_BOSS_BATTLE)
            CALL_CHILD(PTR(kizayarou_init))
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_check_delete, STRING("ドクガッスン１_2"))
    CALL(evt_npc_check_delete, STRING("モノクロシンエモン_2"))
    CALL(evt_npc_check_delete, STRING("ドクガッスン３_2"))
#endif
    SET_VALUE(LW(0), PTR(tree_access_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG2_00_BOGGLY_WOODS)
        SPAWN_EVENT(PTR(witchtrio_momeru))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_05_02_FLURRIE_REQUEST)
        SPAWN_EVENT(PTR(witchtrio_picture))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_06_THE_SKETCH)
        SPAWN_EVENT(PTR(mario_vs_witchtrio))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_07_MARIO_VS_SHADOW_SIRENS)
        SPAWN_EVENT(PTR(witchtrio_lose))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_08_SHADOW_SIRENS_LOSE)
        CALL(evt_item_entry, STRING("necklace"), ITEM_NECKLACE, -361, 0, -22, ITEM_TYPE_STILL, GSWF_WIN_00_NECKLACE, PTR(get_necklace))
    END_IF
    CALL(evt_item_entry, STRING("i"), ITEM_HONEY_SYRUP, 385, 0, -115, ITEM_TYPE_GROUND, GSWF_WIN_00_HONEY_SYRUP, 0)
    CALL_CHILD(PTR(iri_28_init))
    RETURN
EVT_END()

USER_FUNC(mariowalk_wait) {
    return !(marioGetPtr()->flags & MARIO_FLAG_FORCED_MOVEMENT);
}

/**
 * Script helper: slides the named item (args[0]) from its current position to (args[1], args[2], args[3]) over
 * args[4] ms using interpolation mode args[5], plus a y offset args[6]; blocks until done.
 */
USER_FUNC(win_item_move_3d_position) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    f32 yOffset = evtGetFloat(event, args[6]);
    ItemEntry* item = itemNameToPtr(name);
    s32 elapsed;

    if (isFirstCall) {
        if (item == NULL) {
            return EVT_RETURN_DONE;
        }
        event->userdata[0] = gp->renderTime;
        event->userdataf[1] = item->position.x;
        event->userdataf[2] = item->position.y;
        event->userdata3f = item->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - event->userdata[0]);
    if (elapsed < frames) {
        // FAKE: elapsed is copied into isFirstCall; using elapsed directly changes the object.
        isFirstCall = elapsed;
        item->position.x = intplGetValue(mode, event->userdataf[1], x, isFirstCall, frames);
        item->position.y = yOffset + intplGetValue(mode, event->userdataf[2], y, isFirstCall, frames);
        item->position.z = intplGetValue(mode, event->userdata3f, z, isFirstCall, frames);
        return EVT_RETURN_BLOCK;
    }
    item->position.x = x;
    item->position.y = y + yOffset;
    item->position.z = z;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: blocks while the "necklace" item exists and is still in use,
 * so scenes wait until it has been picked up or removed.
 */
USER_FUNC(item_check_necklace_exist) {
    ItemEntry* item = itemNameToPtr("necklace");

    if (item != NULL && itemFlag(item, ITEM_FLAG_ACTIVE)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}
