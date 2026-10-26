#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "rsh.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_win_item_select);

EvtWord s_bero_close[12];
EvtWord s_bero_open[12];
static BeroEntry bero_data_a[4];
static BeroEntry bero_data_b[4];
static BeroEntry bero_data_c[4];
static const char* door_3[7];
static const char* door_4[7];
static EvtWord init_toron[70];
static EvtWord talk_toron[63];
static EvtWord init_madam[52];
static s32 item_tbl_madam[2];
static EvtWord talk_madam_ring_return[67];
static EvtWord talk_madam[195];
static NpcSetupInfo npc_data[7];
NpcSetupInfo npc_data_2[3];
static EvtWord evt_daiji_kami[41];
EvtWord evt_pikary_arrival[141];
static const char* access_tbl[7];
static EvtWord evt_access[25];
static EvtWord evt_access_entry[24];
EvtWord rsh_01_init_evt[190];
EvtWord rsh_01_a_init_evt[103];
EvtWord rsh_01_b_init_evt[14];
static const char* window_tbl[3];
EvtWord rsh_01_c_init_evt[24];
EvtWord talk_syasyou[19];
static BeroEntry bero_data_a_return[4];
static BeroEntry bero_data_b_return[4];
static BeroEntry bero_data_c_return[4];
BeroEntry gor_bero[2];
EvtWord rsh_01_init_evt_nidome[206];
EvtWord rsh_01_a_init_evt_nidome[49];
EvtWord rsh_01_b_init_evt_nidome[31];
EvtWord rsh_01_c_init_evt_nidome[41];

EVT_BEGIN(s_bero_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.3330078f))
    CALL(evt_mapobj_trans, 0, STRING("s_tobira"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(s_bero_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.3330078f))
    CALL(evt_mapobj_trans, 0, STRING("s_tobira"), LW(0), 0, 0)
    RETURN
EVT_END()

static BeroEntry bero_data_a[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_00_a", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        17, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_00", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_b", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_00_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        17, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "hom_00", // nextMapName
        "n_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_c", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_00_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        17, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_c", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

static const char* door_3[] = {
    "heya_3",
    "haikei_3",
    "tobira_3",
    "a_tobira_3",
    "3_door_open",
    "a_tobira_3_heya",
    "3_door_close",
};

static const char* door_4[] = {
    "heya_4",
    "haikei_4",
    "tobira_4",
    "a_tobira_4",
    "4_door_open",
    "a_tobira_4_heya",
    "4_door_close",
};

static EVT_BEGIN(init_toron)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(283)
        CALL(evt_npc_set_position, STRING("me"), -45, 0, -175)
        CASE_BETWEEN(283, 286)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(287, 302)
        CALL(evt_npc_set_position, STRING("me"), -45, 0, -175)
        CASE_BETWEEN(303, 309)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(310)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_toron)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(287)
        IF_INT_EQUAL(GSWF(3427), 0)
            SET_VALUE(GSWF(3427), 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_09"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_10"), 0, 0)
        END_IF
        CASE_LESS(291)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_61"), 0, 0)
        CASE_LESS(294)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_106"), 0, 0)
        SET_VALUE(GSWF(3437), 1)
        CALL_CHILD(PTR(evt_rsh_konarikin_irai_party_talk))
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_61"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_02"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_madam)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(328)
        CALL(evt_npc_set_position, STRING("me"), -275, 0, -225)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(333)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

static s32 item_tbl_madam[] = {
    ITEM_GOLD_RING, -1,
};

static EVT_BEGIN(talk_madam_ring_return)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_200"), 0, STRING("マダム"))
    CALL(evt_win_item_select, 0, PTR(item_tbl_madam), LW(1), 0)
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_201"), 0, STRING("マダム"))
        CALL(evt_sub_get_coin, 30)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_202"), 0, STRING("マダム"))
        CALL(evt_pouch_remove_item, ITEM_GOLD_RING, LW(0))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_201_2"), 0, STRING("マダム"))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_BETWEEN(334, 344)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_madam)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 288)
        IF_INT_EQUAL(GSWF(3430), 0)
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
            IF_INT_EQUAL(GSWF(3429), 0)
                SET_VALUE(GSWF(3429), 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_80"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_83"), 0, 0)
            END_IF
            CALL(evt_msg_select, 0, STRING("stg6_rsh_83_yn"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("stg6_rsh_81"))
                SET_VALUE(GSWF(3430), 1)
                ELSE
                CALL(evt_msg_print_add, 0, STRING("stg6_rsh_82"))
            END_IF
            SWITCH(GSW_STORY_PROGRESS)
                CASE_BETWEEN(310, 325)
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
                CASE_BETWEEN(334, 344)
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
                CASE_DEFAULT
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            END_SWITCH
            RETURN
        END_IF
    END_IF
    CALL(evt_pouch_check_item, ITEM_GOLD_RING, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(talk_madam_ring_return))
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(284)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_11"), 0, 0)
        CASE_LESS(287)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_53"), 0, 0)
        CASE_LESS(288)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_62"), 0, 0)
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_84"), 0, 0)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_03"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_160"), 0, 0)
        CASE_LESS(326)
        IF_INT_EQUAL(GSWF(3441), 0)
            SET_VALUE(GSWF(3441), 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_204"), 0, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_205"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_205"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_205_01"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_206"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_207"))
        END_IF
        CALL(evt_msg_print, 0, STRING("stg6_rsh_208"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_221"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"トロン"*/"\x83\x67\x83\x8D\x83\x93", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_toron, // initEvent
        NULL, // regularEvent
        talk_toron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マダム"*/"\x83\x7D\x83\x5F\x83\x80", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_madam, // initEvent
        NULL, // regularEvent
        talk_madam, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"車掌"*/"\x8E\xD4\x8F\xB6", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コック"*/"\x83\x52\x83\x62\x83\x4E", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ウェイトレス"*/"\x83\x45\x83\x46\x83\x43\x83\x67\x83\x8C\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

NpcSetupInfo npc_data_2[] = {
    {
        /*"トロン"*/"\x83\x67\x83\x8D\x83\x93", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_toron, // initEvent
        NULL, // regularEvent
        talk_toron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マダム"*/"\x83\x7D\x83\x5F\x83\x80", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_madam, // initEvent
        NULL, // regularEvent
        talk_madam, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_daiji_kami)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GSW_STORY_PROGRESS, 305)
    CALL(evt_item_entry, STRING("daiji"), ITEM_VITAL_PAPER, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("daiji"))
    CALL(evt_item_get_item_end_wait, STRING("daiji"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_pikary_arrival)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -100, 0, 0)
    CALL(evt_npc_set_position, STRING("車掌"), 0, 0, 0)
    CALL(evt_npc_set_position, STRING("コック"), 30, 0, 10)
    CALL(evt_npc_set_position, STRING("ウェイトレス"), 50, 0, 40)
    CALL(evt_mario_set_pos, -35, 0, 30)
    CALL(evt_mario_set_dir_npc, STRING("ウェイトレス"))
    CALL(evt_party_set_pos, 0, -50, 0, 20)
    CALL(evt_party_set_dir_npc, 0, STRING("ウェイトレス"))
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("コック"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("ウェイトレス"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_248"), 0, STRING("車掌"))
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("ウェイトレス"), STRING("S_1"), STRING("T_4"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_249"), 0, STRING("ウェイトレス"))
    CALL(evt_npc_set_autotalkpose, STRING("ウェイトレス"), STRING("S_1"), STRING("T_1"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_250"), 0, STRING("コック"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_251"), 0, STRING("ポワン探偵"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_252"), 0, STRING("車掌"))
    WAIT_MS(300)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, 333)
    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("pik_00"), STRING("resha"))
    RETURN
EVT_END()

static const char* access_tbl[] = {
    "tana_5",
    "stg6_rsh_262",
    "stg6_rsh_267",
    "tana_4",
    "stg6_rsh_263",
    "stg6_rsh_267",
    NULL,
};

static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 345)
        CALL(evt_msg_print, 0, LW(1), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, LW(2), 0, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CASE_GREATER_OR_EQUAL(333)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
    END_SWITCH
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL_CHILD(PTR(evt_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CASE_GREATER_OR_EQUAL(333)
        CASE_DEFAULT
        SET_VALUE(LW(0), 2)
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(1), STRING("stg6_rsh_261"))
        SET_VALUE(LW(2), STRING("stg6_rsh_261"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("s_bero"), 0, PTR(evt_access), 0)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(284, 286)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(304, 309)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(310, 325)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_BETWEEN(327, 328)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(329, 331)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(334, 344)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_01_a"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_01_b"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_01_c"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_a_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(332)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        SPAWN_EVENT(PTR(evt_pikary_arrival))
        CASE_DEFAULT
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_SWITCH
    CALL(evt_npc_setup, PTR(npc_data))
    CALL_CHILD(PTR(rsh_01_init_evt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 304)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_run_case_evt, CASE_STAND_ON_A, 1, STRING("a_kami"), 0, PTR(evt_daiji_kami), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 282)
        CALL(evt_mario_set_pos, 100, 0, 0)
        CALL(evt_party_set_pos, 0, 130, 0, 0)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_mario_adjust_dir)
        CALL(evt_cam_road_reset)
        SET_VALUE(GSW_STORY_PROGRESS, 282)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_b_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_b))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL_CHILD(PTR(rsh_01_init_evt))
    RETURN
EVT_END()

static const char* window_tbl[] = {
    "mado_uturi_3",
    "mado_uturi_4",
    NULL,
};

EVT_BEGIN(rsh_01_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL_CHILD(PTR(rsh_01_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    RETURN
EVT_END()

EVT_BEGIN(talk_syasyou)
    IF_INT_EQUAL(GSWF(3581), 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_14"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
    END_IF
    RETURN
EVT_END()

static BeroEntry bero_data_a_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_08_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_00", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_b", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_08_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "hom_00", // nextMapName
        "n_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_02_c", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_08_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_c", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

BeroEntry gor_bero[] = {
    {
        "s_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_04", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_bero_close, // enterAnimeArg
        s_bero_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(rsh_01_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3579), 0)
        IF_INT_EQUAL(GSWF(3580), 0)
            IF_INT_EQUAL(GSWF(3581), 0)
                CALL(evt_map_playanim, STRING("yure"), 1, 0)
                IF_INT_EQUAL(GSWF(3578), 0)
                    CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
                    ELSE
                    CALL(evt_map_playanim, STRING("rt_hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_saboten"), 1, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL_CHILD(PTR(evt_access_entry))
    IF_INT_EQUAL(GSWF(3579), 0)
        IF_INT_EQUAL(GSWF(3580), 0)
            IF_INT_EQUAL(GSWF(3581), 0)
                BEGIN_INLINE_EVENT
                    CALL(evt_bero_exec_wait, 65536)
                    SET_VALUE(LW(0), STRING("s_bero"))
                    CALL_CHILD(PTR(bero_case_switch_off))
                    SET_VALUE(LW(1), STRING("stg6_rsh_261"))
                    SET_VALUE(LW(2), STRING("stg6_rsh_261"))
                    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("s_bero"), 0, PTR(evt_access), 0)
                END_INLINE_EVENT
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(3460), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        ELSE
        IF_INT_EQUAL(GSWF(3461), 1)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            ELSE
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_01_a"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_01_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_01_c"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_01_rnd_npc_entry))
    IF_INT_EQUAL(GSWF(3578), 1)
        CALL(evt_npc_set_position, STRING("車掌"), 395, 0, -90)
        CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_set_ry, STRING("車掌"), 270)
        CALL(evt_npc_change_interrupt, STRING("車掌"), 6, PTR(talk_syasyou))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_a_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_a_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    SET_VALUE(GSWF(3453), 1)
    SET_VALUE(GSWF(3454), 0)
    SET_VALUE(GSWF(3455), 0)
    IF_INT_EQUAL(GSWF(3461), 1)
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("s_bero"), PTR(gor_bero))
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_b_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_b_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    SET_VALUE(GSWF(3453), 0)
    SET_VALUE(GSWF(3454), 1)
    SET_VALUE(GSWF(3455), 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_01_c_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_c_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    SET_VALUE(GSWF(3453), 0)
    SET_VALUE(GSWF(3454), 0)
    SET_VALUE(GSWF(3455), 1)
    RETURN
EVT_END()

