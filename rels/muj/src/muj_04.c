#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"

#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_damage.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "data_tables.h"

USER_FUNC(mario_moveride_check);

static NpcSetupInfo npcEnt[6];
static EvtWord evt_haikei_in[16];
static BeroEntry bero_entry_data[5];
const char* hitname[17];
const char* mapanim[17];
EvtWord mario_hashi_anime[34];
EvtWord erumos00_attack[50];
EvtWord erumos01_attack[50];
EvtWord sanders_attack[88];
EvtWord mario_katsugi[24];
EvtWord sanders_funto[477];
EvtWord yashi_yure_init[5];
EvtWord yashi_yure[255];
static HitDamageReturn hitDmgRetPoints[4];
static TreeAccess tree_tbl[3];
static KusaAccess kusa_tbl[5];
EvtWord muj_04_init_evt[131];

static NpcSetupInfo npcEnt[] = {
    {
        /*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_haikei_in)
    BEGIN_INLINE_EVENT
        CALL(evt_bero_exec_wait, 65536)
        WAIT_FRAMES(10)
        CALL(evt_mario_cam_keep_off)
        CALL(evt_mario_cam_y_on)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

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
        "muj_03", // nextMapName
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
        "muj_05", // nextMapName
        "w_bero", // nextBeroName
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
        NULL, 0, // exitEvent..nextMapName
        "A_dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_dokan", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        evt_haikei_in, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

const char* hitname[] = {
    "A_hasi01",
    "A_hasi02",
    "A_hasi03",
    "A_hasi04",
    "A_hasi05",
    "A_hasi06",
    "A_hasi07",
    "A_hasi08",
    "A_hasi09",
    "A_hasi010",
    "A_hasi011",
    "A_hasi012",
    "A_hasi013",
    "A_hasi014",
    "A_hasi015",
    "A_hasi016",
    "A_hasi017",
};

const char* mapanim[] = {
    "S_hasi01",
    "S_hasi02",
    "S_hasi03",
    "S_hasi04",
    "S_hasi05",
    "S_hasi06",
    "S_hasi07",
    "S_hasi08",
    "S_hasi09",
    "S_hasi10",
    "S_hasi11",
    "S_hasi12",
    "S_hasi13",
    "S_hasi14",
    "S_hasi15",
    "S_hasi16",
    "S_hasi17",
};

EVT_BEGIN(mario_hashi_anime)
    SET_VALUE(LW(1), 0)
    LOOP(0)
        CALL(mario_moveride_check)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BRIDGE_CREAK1"), LW(10), LW(11), LW(12), 0)
            CALL(evt_map_playanim, LW(0), 0, 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(erumos00_attack)
    CALL(evt_npc_sound_data_set, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_get_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), -25, 125, 0, 0, FLOAT(115.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), -35, 125, 0, 0, FLOAT(122.0f), 0, 11, 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(erumos01_attack)
    CALL(evt_npc_sound_data_set, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_get_position, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 25, 125, 0, 0, FLOAT(110.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 35, 125, 0, 0, FLOAT(121.0f), 0, 11, 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sanders_attack)
    WAIT_MS(600)
    BEGIN_INLINE_EVENT
        LOOP(0)
            WAIT_MS(200)
            CALL(evt_npc_get_position, STRING("サンダース"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_DAMAGED1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_sub_random, 30, LW(10))
            ADD(LW(10), -15)
            ADD(LW(10), LW(0))
            CALL(evt_sub_random, 30, LW(11))
            ADD(LW(11), LW(1))
            CALL(evt_eff, STRING(""), STRING("hit"), 3, LW(10), LW(11), LW(2), FLOAT(0.5f), 18, 0, 0, 0, 0, 0, 0)
            WAIT_MS(400)
        END_LOOP
    END_INLINE_EVENT
    LOOP(0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 360, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("サンダース"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mario_katsugi)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("めがね水夫"), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 5)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sanders_funto)
    CALL(evt_npc_set_position, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), 0, 112, 0)
    CALL(evt_npc_set_ry, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), 90)
    CALL(evt_npc_set_position, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), -25, 112, -20)
    CALL(evt_npc_set_ry, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 270)
    CALL(evt_npc_set_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 25, 112, -20)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_sound_data_set, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1L"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_npc_set_ry, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 90)
    CALL(evt_npc_set_ry, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), -60, 125, 0)
    CALL(evt_npc_set_position, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 60, 125, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print, 0, STRING("stg5_muj_23"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 260, 532, 0, 165, -10, 1000, 11)
    WAIT_MS(500)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -150, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -180, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    WAIT_MS(500)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_set_dir_to_target, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), STRING("mario"))
    CALL(evt_cam3d_evt_set, -60, 260, 532, -60, 165, -10, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_muj_24"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    CALL(evt_set_dir_to_target, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_25"), 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_26"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 360, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(200)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_get_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_THROW1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 270)
        CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), LW(0), LW(1), LW(2), 500, 70)
        CALL(evt_npc_flag_onoff, 1, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_ry, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 270)
        CALL(evt_npc_jump_position_nohit, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), -105, 116, 0, 500, 70)
        CALL(evt_npc_set_anim, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), STRING("F_1"))
        CALL(evt_npc_flag_onoff, 1, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_msg_print, 0, STRING("stg5_muj_27"), 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(350)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_D_8"))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -110, 260, 532, -110, 165, -10, 500, 11)
    WAIT_MS(500)
    SPAWN_EVENT(erumos00_attack)
    SPAWN_EVENT(erumos01_attack)
    SPAWN_EVENT(sanders_attack)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_anim, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), STRING("S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg5_muj_28"), 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    WAIT_MS(1000)
    CALL(evt_mario_cam_keep_off)
    CALL(evt_mario_cont_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), STRING("U_1"))
        CALL(evt_mario_set_pose, STRING("M_I_6"))
        SPAWN_EVENT(mario_katsugi)
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), STRING("R_4"))
        CALL(evt_npc_move_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), -300, -20, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), -300, -20, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -300, -20, FLOAT(180.0f))
    END_INLINE_EVENT
    WAIT_MS(2000)
    SET_VALUE(GSW_STORY_PROGRESS, 234)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("muj_03"), STRING("e_bero"))
    RETURN
EVT_END()

EVT_BEGIN(yashi_yure_init)
    SET_VALUE(GW(10), 0)
    RETURN
EVT_END()

EVT_BEGIN(yashi_yure)
    ADD(GW(10), 1)
    IF_INT_EQUAL(GW(10), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_get_position, STRING("S_yasi_ki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, 0, -40, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi02"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_yasi_mi01"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_COCONUTS_FALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_mario_key_onoff, 1)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_sub_area_check, 0, 40, -489, 20, LW(0), LW(2), LW(10))
                IF_INT_EQUAL(LW(10), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi02"), 0, -1000, 0)
            CALL(evt_item_entry, STRING("yashi00"), ITEM_COCONUT, 0, -1000, 0, 16, -1, 0)
            CALL(evt_item_flag_on, STRING("yashi00"), 2)
            CALL(evt_item_get_item, STRING("yashi00"))
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
        RETURN
    END_IF
    IF_INT_EQUAL(GW(10), 2)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_get_position, STRING("S_yasi_ki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(100)
        WAIT_MS(50)
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, 0, -40, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi01"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_yasi_mi01"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_COCONUTS_FALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_mario_key_onoff, 1)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_sub_area_check, 0, 50, -489, 20, LW(0), LW(2), LW(10))
                IF_INT_EQUAL(LW(10), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi01"), 0, -1000, 0)
            CALL(evt_item_entry, STRING("yashi01"), ITEM_COCONUT, 0, -1000, 0, 16, -1, 0)
            CALL(evt_item_flag_on, STRING("yashi01"), 2)
            CALL(evt_item_get_item, STRING("yashi01"))
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
        RETURN
    END_IF
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"S_l_dai_03", {-432.0f, 150.0f, 0.0f}},
    {"A_r_dai", {411.0f, 150.0f, 0.0f}},
    {"dokan_1", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static TreeAccess tree_tbl[] = {
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
    {5, "A_treeA_04", "treeA_04_m", "treeA_04_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static KusaAccess kusa_tbl[] = {
    {1, "A_kuwa_move_01", "S_kuwa_move_01", NULL},
    {1, "A_kuwa_move_02", "S_kuwa_move_02", NULL},
    {1, "A_kuwa_move_03", "S_kuwa_move_03", NULL},
    {1, "A_kuwa_move_04", "S_kuwa_move_04", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(muj_04_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ6"))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_item_entry, STRING("item_00"), ITEM_ICE_POWER, 290, 60, 10, 16, GSWF(3148), 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_INN_COUPON, -367, 150, -73, 16, GSWF(3158), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 450, 210, -90, 0, GSWF(5553), 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("S_mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("S_mizu_02"), 1, 0)
    SPAWN_EVENT(PTR(mario_hashi_anime))
    SPAWN_EVENT(PTR(yashi_yure_init))
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_hai_yasi"), 0, PTR(yashi_yure), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(233)
        SPAWN_EVENT(PTR(sanders_funto))
    END_SWITCH
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_hasi18"), 2097152)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_srt"), 2097152)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 150, 425, 10, 130, 0, 0)
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    RETURN
EVT_END()

USER_FUNC(mario_moveride_check) {
    s32 i;
    const char* name;
    const char* anim;
    HitEntry* hit = marioGetPtr()->hitobjStandOn;

    name = NULL;
    for (i = 0; i < 17; i++) {
        if (hit != NULL && strcmp(hitGetName(hit), hitname[i]) == 0) {
            name = hitname[i];
            anim = mapanim[i];
            break;
        }
    }
    event->lwData[0] = 0;
    if (name != NULL && event->lwData[1] != (EvtWord)name) {
        event->lwData[0] = (EvtWord)anim;
    }
    event->lwData[1] = (EvtWord)name;
    return EVT_RETURN_DONE;
}
