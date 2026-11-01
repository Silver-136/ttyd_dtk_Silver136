// mri_07 (巨木の中, The Great Tree): the Puni shop and the Puni information seller, with the little Jabbi
// (chibitoge) and a recovery block.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "mri.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "evt/evt_cam.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "mri_puni.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/npcdrv.h"
#include "pmario_sound.h"


static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
EvtWord chibitoge_nakama[287];
static EvtWord chibitoge_init[29];
static EvtWord chibitoge_talk[4];
EvtWord punishop_init[12];
EvtWord punishop_talk[2];
EvtWord puniinfo_init[12];
EvtWord puniinfo_talk[118];
static NpcSetupInfo npcEnt[4];
static BeroEntry bero_entry_data[2];
EvtWord mri_07_init_evt[108];

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

/**
 * What the Puni clerk sells and at what price in coins: HP Drain item, Ice Storm, Mystery, Mini Mr. Mini,
 * Honey Syrup and Mushroom.
 */
static ShopItem goods_list[] = {
    {ITEM_HP_DRAIN_ITEM, 10},
    {ITEM_ICE_STORM, 15},
    {ITEM_MYSTERY, 3},
    {ITEM_MINI_MR_MINI, 8},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_MUSHROOM, 3},
};

/** What the clerk buys from Mario and at what price (the trade-in list of the shop). */
static ShopDb trade_list[] = {
    {ITEM_HP_DRAIN_ITEM, 5, 0},
    {ITEM_ICE_STORM, 7, 0},
    {ITEM_MYSTERY, 1, 0},
    {ITEM_MINI_MR_MINI, 4, 0},
    {ITEM_HONEY_SYRUP, 2, 0},
    {ITEM_MUSHROOM, 1, 0},
    {ITEM_SUPER_SHROOM, 14, 0},
    {ITEM_ULTRA_SHROOM, 75, 0},
    {ITEM_LIFE_SHROOM, 43, 0},
    {ITEM_FRIGHT_MASK, 4, 0},
    {ITEM_POWER_PUNCH, 17, 0},
    {0},
};

/** The Puni clerk (プニ店員) and his 35 shop messages (stg2_shop_00 and up). */
static ShopperData35 shopper_data = {
    /*"プニ店員"*/"\x83\x76\x83\x6A\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"stg2_shop_00", "stg2_shop_01", "stg2_shop_02", "stg2_shop_03", "stg2_shop_04", "stg2_shop_05", "stg2_shop_06", "stg2_shop_07", "stg2_shop_08", "stg2_shop_09", "stg2_shop_10", "stg2_shop_11", "stg2_shop_12", "stg2_shop_13", "stg2_shop_14", "stg2_shop_15", "stg2_shop_16", "stg2_shop_17", "stg2_shop_18", "stg2_shop_19", "stg2_shop_20", "stg2_shop_21", "stg2_shop_22", "stg2_shop_23", "stg2_shop_24", "stg2_shop_25", "stg2_shop_26", "stg2_shop_27", "stg2_shop_28", "stg2_shop_29", "stg2_shop_30", "stg2_shop_31", "stg2_shop_32", "stg2_shop_33", "stg2_shop_34"}, // msgs
};

/**
 * Talk script of the little Jabbi in the Puni shop room:
 * Punio, the clerk and the party talk about the little Jabbi joining the Puni group and the player answers yes
 * or no. If Mario agrees the Jabbi joins the Puni group (GSWF_MRI_JABBI_IN_PUNI_GROUP);
 * if not, the Jabbi hops away.
 */
EVT_BEGIN(chibitoge_nakama)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_jump_position_nohit, STRING("me"), -167, 44, 41, 400, 50)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg2_mri_f01_00"), 0, STRING("ガイドsp"))
    CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_ry_lr, STRING("プニ店員"), 0)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_f01_01"), 0, STRING("プニ店員"))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("stg2_mri_f01_02"))
    CALL(evt_msg_select, 0, STRING("stg2_mri_f01_yesno"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_JUMP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("ちびトゲ"), -138, 0, 72, 300, 30)
        WAIT_MS(200)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_MOVE1"), LW(0), LW(1), LW(2), LW(9))
        ADD(LW(0), 200)
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_snd_sfxoff, LW(9))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_03"), 0, STRING("ガイドsp"))
        CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_04"), 0, STRING("プニ店員"))
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        ELSE
        CALL(evt_msg_print_party, STRING("stg2_mri_f01_05"))
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_06"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_JUMP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("me"), -138, 0, 72, 300, 30)
        WAIT_MS(500)
        CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_07"), 0, STRING("プニ店員"))
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CALL(evt_puni_init_puni, 3, 1, LW(0), LW(1), LW(2), 0)
        SET_VALUE(GSWF_MRI_JABBLE_JOINED, 1)
    END_IF
    CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_JOIN)
    CALL(evt_npc_flag_onoff, 0, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Init script of the little Jabbi in the room: unless the story is in the second Puni-versus-Jabbi fight window
 * (STG2_21 to 21_03) or past the praise, and he is neither waiting at the cell nor already in the Puni
 * group, he stands at the left of the room facing east.
 */
static EVT_BEGIN(chibitoge_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_21_BUBBLES_2, STG2_21_03_SUPER_BOOTS)
        CASE_GREATER_OR_EQUAL(STG2_26_PRAISE_FROM_THE_PUNI)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF_MRI_PUNIO_WAITING_AT_JAIL, 0)
            IF_INT_EQUAL(GSWF_MRI_JABBLE_JOINED, 0)
                CALL(evt_npc_set_position, STRING("me"), -170, 0, 0)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of the little Jabbi: runs chibitoge_nakama, the scene in which he asks to join the Puni group. */
static EVT_BEGIN(chibitoge_talk)
    CALL_CHILD(PTR(chibitoge_nakama))
    RETURN
EVT_END()

/** Init for the Puni clerk of the shop: places the clerk at (207, 0, -6) facing left. */
EVT_BEGIN(punishop_init)
    CALL(evt_npc_set_position, STRING("me"), 207, 0, -6)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

/** Talk script of the Puni clerk: does nothing; the shop menu itself is opened by the shop setup (evt_shop_setup). */
EVT_BEGIN(punishop_talk)
    RETURN
EVT_END()

/** Init for the Puni information seller: places it at (235, 0, 124) facing left. */
EVT_BEGIN(puniinfo_init)
    CALL(evt_npc_set_position, STRING("me"), 235, 0, 124)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

/**
 * Talk script of the Puni information seller: cycles through three lines of tips (counter LSW(714));
 * the set of lines changes after the boss fight and again at the end of the story.
 */
EVT_BEGIN(puniinfo_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        SWITCH(LSW_MRI_07_PUNI_INFO_TALK_INDEX)
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_00"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_01"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 0)
        END_SWITCH
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        SWITCH(LSW_MRI_07_PUNI_INFO_TALK_INDEX)
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_01"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_02"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_03"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 0)
        END_SWITCH
        CASE_DEFAULT
        SWITCH(LSW_MRI_07_PUNI_INFO_TALK_INDEX)
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_04"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_05"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_06"), 0, STRING("me"))
            SET_VALUE(LSW_MRI_07_PUNI_INFO_TALK_INDEX, 0)
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

/** NPCs of the Puni shop: Jabble (ちびトゲ), the Puni clerk (プニ店員) and the information seller (プニ情報屋). */
static NpcSetupInfo npcEnt[] = {
    {
        /*"ちびトゲ"*/"\x82\xBF\x82\xD1\x83\x67\x83\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        chibitoge_init, // initEvent
        NULL, // regularEvent
        chibitoge_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ店員"*/"\x83\x76\x83\x6A\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        punishop_init, // initEvent
        NULL, // regularEvent
        punishop_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ情報屋"*/"\x83\x76\x83\x6A\x8F\xEE\x95\xF1\x89\xAE", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        puniinfo_init, // initEvent
        NULL, // regularEvent
        puniinfo_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Exit of mri_07: east road e_bero to mri_06 (w_bero). */
static BeroEntry bero_entry_data[] = {
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
        "mri_06", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Map init script of mri_07, the Puni shop room:
 * sets up the Puni, the recovery block, the little Jabbi, the shop clerk and the information seller,
 * and registers the shop (goods and trade lists).
 */
EVT_BEGIN(mri_07_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 7, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, -129, 0, 96, 2, -2, -1000, -1000)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 8, 290, 82, 120, 0, 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    RETURN
EVT_END()

