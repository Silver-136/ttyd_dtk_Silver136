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

static ShopItem goods_list[] = {
    {ITEM_HP_DRAIN_ITEM, 10},
    {ITEM_ICE_STORM, 15},
    {ITEM_MYSTERY, 3},
    {ITEM_MINI_MR_MINI, 8},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_MUSHROOM, 3},
};

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

static ShopperData35 shopper_data = {
    /*"プニ店員"*/"\x83\x76\x83\x6A\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"stg2_shop_00", "stg2_shop_01", "stg2_shop_02", "stg2_shop_03", "stg2_shop_04", "stg2_shop_05", "stg2_shop_06", "stg2_shop_07", "stg2_shop_08", "stg2_shop_09", "stg2_shop_10", "stg2_shop_11", "stg2_shop_12", "stg2_shop_13", "stg2_shop_14", "stg2_shop_15", "stg2_shop_16", "stg2_shop_17", "stg2_shop_18", "stg2_shop_19", "stg2_shop_20", "stg2_shop_21", "stg2_shop_22", "stg2_shop_23", "stg2_shop_24", "stg2_shop_25", "stg2_shop_26", "stg2_shop_27", "stg2_shop_28", "stg2_shop_29", "stg2_shop_30", "stg2_shop_31", "stg2_shop_32", "stg2_shop_33", "stg2_shop_34"}, // msgs
};

EVT_BEGIN(chibitoge_nakama)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_jump_position_nohit, STRING("me"), -167, 44, 41, 400, 50)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL(evt_puni_flag_onoff, 1, 0, 262144)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg2_mri_f01_00"), 0, STRING("ガイドsp"))
    CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, 11)
    CALL(evt_npc_set_ry_lr, STRING("プニ店員"), 0)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_f01_01"), 0, STRING("プニ店員"))
    CALL(evt_cam3d_evt_off, 0, 11)
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
        CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_04"), 0, STRING("プニ店員"))
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
        ELSE
        CALL(evt_msg_print_party, STRING("stg2_mri_f01_05"))
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_06"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TOGETTI_JUMP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("me"), -138, 0, 72, 300, 30)
        WAIT_MS(500)
        CALL(evt_cam3d_evt_set, 211, 151, 548, 211, 56, 7, 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_f01_07"), 0, STRING("プニ店員"))
        CALL(evt_cam3d_evt_off, 0, 11)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CALL(evt_puni_init_puni, 3, 1, LW(0), LW(1), LW(2), 0)
        SET_VALUE(GSWF(2878), 1)
    END_IF
    CALL(evt_puni_flag_onoff, 1, 1, 262144)
    CALL(evt_puni_set_mode, 1, 1)
    CALL(evt_npc_flag_onoff, 0, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(chibitoge_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(102, 105)
        CASE_GREATER_OR_EQUAL(118)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2836), 0)
            IF_INT_EQUAL(GSWF(2878), 0)
                CALL(evt_npc_set_position, STRING("me"), -170, 0, 0)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(chibitoge_talk)
    CALL_CHILD(PTR(chibitoge_nakama))
    RETURN
EVT_END()

EVT_BEGIN(punishop_init)
    CALL(evt_npc_set_position, STRING("me"), 207, 0, -6)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

EVT_BEGIN(punishop_talk)
    RETURN
EVT_END()

EVT_BEGIN(puniinfo_init)
    CALL(evt_npc_set_position, STRING("me"), 235, 0, 124)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

EVT_BEGIN(puniinfo_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(112)
        SWITCH(LSW(714))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_00"), 0, STRING("me"))
            SET_VALUE(LSW(714), 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_01"), 0, STRING("me"))
            SET_VALUE(LSW(714), 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02"), 0, STRING("me"))
            SET_VALUE(LSW(714), 0)
        END_SWITCH
        CASE_LESS(401)
        SWITCH(LSW(714))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_01"), 0, STRING("me"))
            SET_VALUE(LSW(714), 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_02"), 0, STRING("me"))
            SET_VALUE(LSW(714), 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_03"), 0, STRING("me"))
            SET_VALUE(LSW(714), 0)
        END_SWITCH
        CASE_DEFAULT
        SWITCH(LSW(714))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_04"), 0, STRING("me"))
            SET_VALUE(LSW(714), 1)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_05"), 0, STRING("me"))
            SET_VALUE(LSW(714), 2)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg2_mri_f00_02_06"), 0, STRING("me"))
            SET_VALUE(LSW(714), 0)
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

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

static BeroEntry bero_entry_data[] = {
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
        "mri_06", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

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
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 8, 290, 82, 120, 0, 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    RETURN
EVT_END()

