#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"


EvtWord muj_03_event_01[22];
static BeroEntry bero_entry_data[3];
EvtWord nigeru_suifu01[271];
static EvtWord evt_kusa_5[20];
static EvtWord evt_kusa_7[20];
static TreeAccess tree_tbl[5];
static KusaAccess kusa_tbl[9];
EvtWord muj_03_init_evt[112];

EVT_BEGIN(muj_03_event_01)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 234)
        IF_INT_EQUAL(GSWF(3131), 0)
            SPAWN_EVENT(PTR(nigeru_suifu01))
            SET_VALUE(GSWF(3131), 1)
            SET_VALUE(LW(0), 1)
            RETURN
        END_IF
    END_IF
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
        "muj_02", // nextMapName
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
        muj_03_event_01, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_04", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(nigeru_suifu01)
    CALL(evt_npc_set_position, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 570, 150, -50)
    CALL(evt_npc_set_ry, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 530, 150, -50)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_sound_data_set, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1L"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 610, 150, 0)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_pos, 0, 630, 150, 25)
    CALL(evt_party_set_dir, 0, 270, 0)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg5_muj_90_22"), 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 270)
        WAIT_MS(200)
        CALL(evt_npc_jump_position_nohit, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 464, 180, -50, 500, 50)
        WAIT_MS(300)
        CALL(evt_npc_jump_position_nohit, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 382, 180, -50, 500, 50)
        CALL(evt_npc_set_position, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_90_23"), 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 464, 180, -50, 500, 50)
    WAIT_MS(300)
    CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 382, 180, -50, 500, 50)
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 90)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 465, 180, -50, 500, 50)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 90)
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 465, 288, 263, 465, 240, -8, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 270)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_muj_90_24"), 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 382, 180, -50, 500, 50)
        CALL(evt_npc_set_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 90, 200)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 90, 200)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_msg_print_party, STRING("stg5_muj_90_25"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kusa_5)
    CALL(evt_item_delete_check, STRING("item_kusa_5"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_5"), ITEM_MINI_MR_MINI, -250, 10, 95, 4, GSWF(3155), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_kusa_7)
    CALL(evt_item_delete_check, STRING("item_kusa_7"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_7"), ITEM_COIN, 175, 210, -50, 4, GSWF(3156), 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess tree_tbl[] = {
    {5, "A_treeA_01", "treeA_01_m", "treeA_01_ha", 0, NULL},
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
    {5, "A_treeA_03", "treeA_03_m", "treeA_03_ha", 0, NULL},
    {5, "A_treeA_04", "treeA_04_m", "treeA_04_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static KusaAccess kusa_tbl[] = {
    {1, "A_kuwa_move_01", "S_kuwa_move_01", NULL},
    {1, "A_kuwa_move_02", "S_kuwa_move_02", NULL},
    {1, "A_kuwa_move_03", "S_kuwa_move_03", NULL},
    {1, "A_kuwa_move_04", "S_kuwa_move_04", NULL},
    {1, "A_kuwa_move_05", "S_kuwa_move_05", evt_kusa_5},
    {1, "A_kuwa_move_06", "S_kuwa_move_06", NULL},
    {1, "A_kuwa_move_07", "S_kuwa_move_07", evt_kusa_7},
    {1, "A_kuwa_move_08", "S_kuwa_move_08", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(muj_03_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ5"))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_badgeblk, STRING("mobj01"), -50, 9, 0, 121, 0, GSWF(3133), 0)
    CALL(evt_mobj_brick, STRING("mobj02"), -50, 94, 0, 0, 13, 0, GSWF(3134))
#ifdef VERSION_G8ME01
    CALL(evt_mobj_brick, STRING("bblk00"), -470, 155, -50, 0, 0, 0, LSWF(3))
#else
    CALL(evt_mobj_brick, STRING("bblk00"), -470, 155, -50, 0, 0, 0, GSWF(3145))
#endif
    CALL(evt_mobj_powerupblk, STRING("pblk01"), -470, 240, -50, 0, GSWF(5552), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj03"), -50, 270, -100, 129, 0, GSWF(3147), 0)
    CALL(evt_mobj_brick, STRING("mobj_hidden2"), -50, 209, 0, 156, 11, 0, GSWF(3153))
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 275, 10, 100, 16, GSWF(5642), 0)
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    RETURN
EVT_END()

