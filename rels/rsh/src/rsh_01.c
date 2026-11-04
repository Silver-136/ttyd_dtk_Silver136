// rsh_01 (列車, Excess Express): car joining the driver's cab (rsh_00) and rsh_02: Toron, Madam, Detective Pennington, conductor, cook and
// waitress NPCs; doors to Rogueport (gor_04), Riverside (hom_00) and Poshley Heights (pik_00) stations.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_window.h"
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

#if VERSION <= VERSION_DEMO_JP
static EvtWord talk_madam_ring_return[65];
#else
static EvtWord talk_madam_ring_return[67];
#endif

static EvtWord talk_madam[195];
static NpcSetupInfo npc_data[7];
NpcSetupInfo npc_data_2[3];
static EvtWord evt_daiji_kami[41];
EvtWord evt_pikary_arrival[141];

#if VERSION <= VERSION_DEMO_JP
static const char* access_tbl[5];
static EvtWord evt_access[14];
static EvtWord evt_access_entry[23];
extern EvtWord rsh_01_init_evt[];
#else
static const char* access_tbl[7];
static EvtWord evt_access[25];
static EvtWord evt_access_entry[24];
EvtWord rsh_01_init_evt[190];
#endif

EvtWord rsh_01_a_init_evt[103];
EvtWord rsh_01_b_init_evt[14];
static const char* window_tbl[3];
EvtWord rsh_01_c_init_evt[24];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord talk_syasyou[];
#else
EvtWord talk_syasyou[19];
#endif

static BeroEntry bero_data_a_return[4];
static BeroEntry bero_data_b_return[4];
static BeroEntry bero_data_c_return[4];
BeroEntry gor_bero[2];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord rsh_01_init_evt_nidome[];
#else
EvtWord rsh_01_init_evt_nidome[206];
#endif

EvtWord rsh_01_a_init_evt_nidome[49];
EvtWord rsh_01_b_init_evt_nidome[31];
EvtWord rsh_01_c_init_evt_nidome[41];

/**
 * Enter animation of the south door (s_bero) of rsh_01:
 * slides the s_tobira door object by the door animation's progress value in LW(0), scaled by -0.333.
 */
EVT_BEGIN(s_bero_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.3330078f))
    CALL(evt_mapobj_trans, 0, STRING("s_tobira"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Exit animation of the south door (s_bero) of rsh_01:
 * slides the s_tobira door object by the door animation's progress value in LW(0),
 * scaled by -0.333 (same motion as s_bero_close).
 */
EVT_BEGIN(s_bero_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.3330078f))
    CALL(evt_mapobj_trans, 0, STRING("s_tobira"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Doors and exits of rsh_01 variant a: w_bero to rsh_02_a; e_bero to rsh_00_a; s_bero to pik_00 (Poshley
 * Heights).
 */
static BeroEntry bero_data_a[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Doors and exits of rsh_01 variant b: w_bero to rsh_02_b; e_bero to rsh_00_b; s_bero to hom_00 (Excess
 * Express platform).
 */
static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Doors and exits of rsh_01 variant c: w_bero to rsh_02_c; e_bero to rsh_00_c; s_bero to rsh_01_c. */
static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Map objects of cabin 3 for evt_rsh_door_entry: the room, backdrop, door, door hit areas and open and close
 * animations.
 */
static const char* door_3[] = {
    "heya_3",
    "haikei_3",
    "tobira_3",
    "a_tobira_3",
    "3_door_open",
    "a_tobira_3_heya",
    "3_door_close",
};

/**
 * Map objects of cabin 4 for evt_rsh_door_entry: the room, backdrop, door, door hit areas and open and close
 * animations.
 */
static const char* door_4[] = {
    "heya_4",
    "haikei_4",
    "tobira_4",
    "a_tobira_4",
    "4_door_open",
    "a_tobira_4_heya",
    "4_door_close",
};

/**
 * Init event of Zip Toad (Toron) in rsh_01: places him at (-45, 0, -175) before STG6_01_THE_CONDUCTOR
 * and again from the Galley Pot stage to the end of day 1; at other stages he is parked below the floor.
 */
static EVT_BEGIN(init_toron)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_01_THE_CONDUCTOR)
        CALL(evt_npc_set_position, STRING("me"), -45, 0, -175)
        CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_04_GRILLING_HEFF_T)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_05_GALLEY_KETTLE_DISCOVERED, STG6_20_END_OF_DAY_1)
        CALL(evt_npc_set_position, STRING("me"), -45, 0, -175)
        CASE_BETWEEN(STG6_21_DAY_2, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_27_RIVERSIDE_STATION)
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
        CASE_LESS(STG6_05_GALLEY_KETTLE_DISCOVERED)
        IF_INT_EQUAL(GSWF_RSH_ZIP_TOAD_FIRST_TALK_DONE, 0)
            SET_VALUE(GSWF_RSH_ZIP_TOAD_FIRST_TALK_DONE, 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_09"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_10"), 0, 0)
        END_IF
        CASE_LESS(STG6_09_BUBS_REQUEST)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_61"), 0, 0)
        CASE_LESS(STG6_12_GIVE_AUTOGRAPH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_106"), 0, 0)
        SET_VALUE(GSWF_RSH_ZIP_TOAD_BUB_HINT_HEARD, 1)
        CALL_CHILD(PTR(evt_rsh_konarikin_irai_party_talk))
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_61"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_02"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Init event of Madam: places her at (-275, 0, -225) before day 3; later she is parked below the floor. */
static EVT_BEGIN(init_madam)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_45_DAY_3_STARTS)
        CALL(evt_npc_set_position, STRING("me"), -275, 0, -225)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_00_TRAIN_ARRIVAL)
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

/** Items the player may give the madam: the Gold Ring only, ended by -1. */
static s32 item_tbl_madam[] = {
    ITEM_GOLD_RING, -1,
};

/**
 * Talk script used when Mario holds a Gold Ring: opens the item menu;
 * giving the ring pays Mario 30 coins and she thanks him (stg6_rsh_201, 202), cancelling prints stg6_rsh_201_2,
 * then the station-dependent music state is applied.
 */

// Item-selection cancellation gives no reward; a non-cancelled selection pays 30 coins, then removes the ring.
static EVT_BEGIN(talk_madam_ring_return)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_200"), 0, STRING("マダム"))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_win_item_select, PTR(item_tbl_madam), LW(1))
#else
    CALL(evt_win_item_select, 0, PTR(item_tbl_madam), LW(1), 0)
#endif
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_201"), 0, STRING("マダム"))
        CALL(evt_sub_get_coin, 30)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_202"), 0, STRING("マダム"))
        CALL(evt_pouch_remove_item, ITEM_GOLD_RING, LW(0))
    ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_201_2"), 0, STRING("マダム"))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_BETWEEN(STG6_49_POSHLEY_HEIGHTS, STG6_55_04_BOWSER_RAWK_HAWK)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_madam)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_06_PENNINGTONS_ASSISTANT)
        IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 0)
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
            IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 0)
                SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_80"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_83"), 0, 0)
            END_IF
            CALL(evt_msg_select, 0, STRING("stg6_rsh_83_yn"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("stg6_rsh_81"))
                SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 1)
            ELSE
                CALL(evt_msg_print_add, 0, STRING("stg6_rsh_82"))
            END_IF
            SWITCH(GSW_STORY_PROGRESS)
                CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
                CASE_BETWEEN(STG6_49_POSHLEY_HEIGHTS, STG6_55_04_BOWSER_RAWK_HAWK)
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
        CASE_LESS(STG6_02_A_CASE_OF_LOST_STEW)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_11"), 0, 0)
        CASE_LESS(STG6_05_GALLEY_KETTLE_DISCOVERED)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_53"), 0, 0)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_62"), 0, 0)
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_84"), 0, 0)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_03"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_160"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        IF_INT_EQUAL(GSWF_RSH_MADAM_POSHLEY_TALK_DONE, 0)
            SET_VALUE(GSWF_RSH_MADAM_POSHLEY_TALK_DONE, 1)
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
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_221"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** NPCs of the car on the first visit: Toron, the madam, Detective Pennington, the conductor, the cook and the waitress. */
static NpcSetupInfo npc_data[] = {
    {
        "トロン", // name
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
        "マダム", // name
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
        "ポワン探偵", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "車掌", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "コック", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ウェイトレス", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Second NPC setup of the car, used by the later-story entries: Toron and the madam. */
NpcSetupInfo npc_data_2[] = {
    {
        "トロン", // name
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
        "マダム", // name
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

/**
 * Trigger when Mario steps on the paper spot (a_kami) at STG6_22_DOCUMENT:
 * sets STG6_23_VITAL_PAPER and gives him the Vital Paper item with the item-get fanfare.
 */
static EVT_BEGIN(evt_daiji_kami)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_23_VITAL_PAPER)
    CALL(evt_item_entry, STRING("daiji"), ITEM_VITAL_PAPER, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("daiji"))
    CALL(evt_item_get_item_end_wait, STRING("daiji"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Cutscene when the train pulls into Poshley Heights:
 * the conductor, waitress, cook and Detective Pennington are lined up around Mario,
 * each says a line (stg6_rsh_248 to 252), then the story advances to STG6_49_00_TRAIN_ARRIVAL
 * and the map changes to pik_00 with an iris fade.
 */
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
    SET_VALUE(GSW_STORY_PROGRESS, STG6_49_00_TRAIN_ARRIVAL)
    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("pik_00"), STRING("resha"))
    RETURN
EVT_END()

// Retail triples: interaction hit, message before Rogueport Station, message from that story point onward.
// The demos use pairs containing only the interaction hit and its message.
static const char* access_tbl[] = {
    "tana_5",
    "stg6_rsh_262",
#if VERSION >= VERSION_JP
    "stg6_rsh_267",
#endif
    "tana_4",
    "stg6_rsh_263",
#if VERSION >= VERSION_JP
    "stg6_rsh_267",
#endif
    NULL,
};

/**
 * Trigger script for the shelves (tana_4, tana_5):
 * locks input and prints the shelf's message, a different one (LW(2)) from the Rogueport station stage on in the
 * releases.
 */
static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
#if VERSION >= VERSION_JP
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
#endif
        CALL(evt_msg_print, 0, LW(1), 0, 0)
#if VERSION >= VERSION_JP
        ELSE
        CALL(evt_msg_print, 0, LW(2), 0, 0)
    END_IF
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Registers the shelf triggers from access_tbl (hit name and message rows);
 * each runs evt_access when Mario presses A while facing it.
 */
static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
#if VERSION <= VERSION_DEMO_JP
        READ_INT2(LW(0), LW(1))
#else
        READ_INT3(LW(0), LW(1), LW(2))
#endif
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

/**
 * Shared map init for the three variants of rsh_01 on the first trip:
 * starts the scenery animations, registers the two cabin doors (heya 3 and 4)
 * and the shelf signs (tana_4, tana_5), closes the south door with the stg6_rsh_261 message,
 * and starts the music and ambience for the story stage (Pennington theme, Riverside, Poshley, danger theme,
 * or the per-variant train theme).
 */
EVT_BEGIN(rsh_01_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CASE_GREATER_OR_EQUAL(STG6_49_00_TRAIN_ARRIVAL)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
    END_SWITCH
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL_CHILD(PTR(evt_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CASE_GREATER_OR_EQUAL(STG6_49_00_TRAIN_ARRIVAL)
        CASE_DEFAULT
        SET_VALUE(LW(0), 2)
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(1), STRING("stg6_rsh_261"))
#if VERSION >= VERSION_JP
        SET_VALUE(LW(2), STRING("stg6_rsh_261"))
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("s_bero"), 0, PTR(evt_access), 0)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_02_A_CASE_OF_LOST_STEW, STG6_04_GRILLING_HEFF_T)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(STG6_22_DOCUMENT, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_BETWEEN(STG6_44_END_OF_DAY_2, STG6_45_DAY_3_STARTS)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_46_ENGINEER, STG6_47_ESCAPING_THE_SMORGS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(STG6_49_POSHLEY_HEIGHTS, STG6_55_04_BOWSER_RAWK_HAWK)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_01_a"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_01_b"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_01_c"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of rsh_01_a (first visit): sets the doors (during the Smorg miasma stage it arms the exits and
 * starts the arrival scene instead of the normal entrance flow), runs the shared init and swaps the paper and plain floors.
 * It requests the Smorg window effect during STG6_46_ENGINEER through STG6_47_ESCAPING_THE_SMORGS
 * and gives the Vital Paper from the a_kami spot at STG6_22_DOCUMENT.
 * If the story is before STG6_00_START, it places Mario and the partner in the car and advances the story to that stage.
 */
EVT_BEGIN(rsh_01_a_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG6_48_SMORG_MIASMA)
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
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_22_DOCUMENT)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_run_case_evt, CASE_STAND_ON_A, 1, STRING("a_kami"), 0, PTR(evt_daiji_kami), 0)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_3_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_3"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_NO_ENTRANCE)
        RETURN
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_00_START)
        CALL(evt_mario_set_pos, 100, 0, 0)
        CALL(evt_party_set_pos, 0, 130, 0, 0)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_mario_adjust_dir)
        CALL(evt_cam_road_reset)
        SET_VALUE(GSW_STORY_PROGRESS, STG6_00_START)
    END_IF
    RETURN
EVT_END()

/**
 * Init script of rsh_01_b (first visit): sets the doors,
 * sets up the car's NPCs (npc_data_2) and runs the shared init.
 */
EVT_BEGIN(rsh_01_b_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_b))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL_CHILD(PTR(rsh_01_init_evt))
    RETURN
EVT_END()

/** Map objects of the scenery that passes by the windows of the car. */
static const char* window_tbl[] = {
    "mado_uturi_3",
    "mado_uturi_4",
    NULL,
};

/**
 * Init script of rsh_01_c (first visit): sets the doors, sets up the car's NPCs (npc_data_2),
 * runs the shared init, plays the lamp animation and sets up the window scenery.
 */
EVT_BEGIN(rsh_01_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL_CHILD(PTR(rsh_01_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    RETURN
EVT_END()

/**
 * Talk script of the conductor (syasyou) on the return trip (hooked in rsh_01_init_evt_nidome):
 * says stg6_rsh_c_14, or in the releases stg6_rsh_c_09_1 once the train has reached Rogueport.
 */
EVT_BEGIN(talk_syasyou)
#if VERSION >= VERSION_JP
    IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
#endif
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_14"), 0, 0)
#if VERSION >= VERSION_JP
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
    END_IF
#endif
    RETURN
EVT_END()

/**
 * Doors and exits of rsh_01 variant a (return trip): w_bero to rsh_02_a; e_bero to rsh_08_a; s_bero to pik_00
 * (Poshley Heights).
 */
static BeroEntry bero_data_a_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Doors and exits of rsh_01 variant b (return trip): w_bero to rsh_02_b; e_bero to rsh_08_a; s_bero to hom_00
 * (Excess Express platform).
 */
static BeroEntry bero_data_b_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Doors and exits of rsh_01 variant c (return trip): w_bero to rsh_02_c; e_bero to rsh_08_a; s_bero to
 * rsh_01_c.
 */
static BeroEntry bero_data_c_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement exit used when the train stands at Rogueport: s_bero leads to gor_04. */
BeroEntry gor_bero[] = {
    {
        "s_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
    IF_INT_EQUAL(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
        IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
                CALL(evt_map_playanim, STRING("yure"), MAP_ANIM_PLAY_LOOP, 0)
                IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                    CALL(evt_map_playanim, STRING("hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
                ELSE
                    CALL(evt_map_playanim, STRING("rt_hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
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
    IF_INT_EQUAL(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
        IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)

                // Wait for the Bero execution bit before replacing the side exit with its interaction message.
                BEGIN_INLINE_EVENT
                    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
                    SET_VALUE(LW(0), STRING("s_bero"))
                    CALL_CHILD(PTR(bero_case_switch_off))
                    SET_VALUE(LW(1), STRING("stg6_rsh_261"))
#if VERSION >= VERSION_JP
                    SET_VALUE(LW(2), STRING("stg6_rsh_261"))
#endif
                    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("s_bero"), 0, PTR(evt_access), 0)
                END_INLINE_EVENT
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_RSH_TRAIN_AT_POSHLEY, 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    ELSE
        IF_INT_EQUAL(GSWF_RSH_TRAIN_AT_ROGUEPORT, 1)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR5"))
#else
            CALL(evt_snd_envoff, PSND_FADEOUT)
#endif
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        ELSE
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_01_a"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF

            // Revisit variant B leaves the current moving-train audio unchanged.
            IF_STR_EQUAL(LW(0), STRING("rsh_01_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_01_c"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_01_rnd_npc_entry))
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
        CALL(evt_npc_set_position, STRING("車掌"), 395, 0, -90)
        CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_set_ry, STRING("車掌"), 270)
        CALL(evt_npc_change_interrupt, STRING("車掌"), 6, PTR(talk_syasyou))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

// Each revisit variant records one route flag for the shared rsh_08_a return map.
EVT_BEGIN(rsh_01_a_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_a_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 1)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 0)
    IF_INT_EQUAL(GSWF_RSH_TRAIN_AT_ROGUEPORT, 1)
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("s_bero"), PTR(gor_bero))
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()

/**
 * Init script of rsh_01_b on the second visit: outbound or return-trip doors, shared second-visit init,
 * and marks variant B active.
 */
EVT_BEGIN(rsh_01_b_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_b_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 1)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 0)
    RETURN
EVT_END()

/**
 * Init script of rsh_01_c on the second visit: outbound or return-trip doors, shared second-visit init,
 * and marks variant C active.
 */
EVT_BEGIN(rsh_01_c_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_c_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_01_init_evt_nidome))
    CALL(evt_npc_setup, PTR(npc_data_2))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 1)
    RETURN
EVT_END()

