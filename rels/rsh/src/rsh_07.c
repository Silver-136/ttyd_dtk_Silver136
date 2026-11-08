// rsh_07 (列車, Excess Express): small room with a driver NPC, controls and schedule board, and a save block; door to
// rsh_04.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "rsh.h"
#include "evt/evt_npc.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

static BeroEntry bero_data_a[2];
static BeroEntry bero_data_b[2];
static BeroEntry bero_data_c[2];
static EvtWord init_unten[6];

#if VERSION <= VERSION_DEMO_JP
static EvtWord talk_unten[8];
#else
static EvtWord talk_unten[19];
#endif

static NpcSetupInfo npc_data[2];
static const char* access_tbl[5];
static EvtWord evt_access[14];
static EvtWord evt_access_entry[23];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord rsh_07_init_evt[];
#else
EvtWord rsh_07_init_evt[113];
#endif

EvtWord rsh_07_a_init_evt[11];
EvtWord rsh_07_b_init_evt[11];
static const char* window_tbl[5];
EvtWord rsh_07_c_init_evt[16];

static BeroEntry bero_data_a[] = {
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_a", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        BERO_DIR_E, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        BERO_DIR_E, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_04_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(init_unten)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

static EVT_BEGIN(talk_unten)
#if VERSION >= VERSION_JP
    IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
#endif
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_08"), 0, 0)
#if VERSION >= VERSION_JP
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_08_1"), 0, 0)
    END_IF
#endif
    RETURN
EVT_END()

/** NPCs of the small room: the driver. */
static NpcSetupInfo npc_data[] = {
    {
        "運転手", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_unten, // initEvent
        NULL, // regularEvent
        talk_unten, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

// Alternating interaction-hit names and message IDs, terminated by a null hit name.
static const char* access_tbl[] = {
    "nittei",
    "stg6_rsh_c_00_1",
    "soujyuu",
    "stg6_rsh_260",
    NULL,
};

/** Trigger script for the signboards: locks input, prints the message in LW(1) and unlocks. */
static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Registers the signboard triggers from access_tbl (hit name, message rows). */
static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
        READ_INT2(LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(rsh_07_init_evt)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
        IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
                CALL(evt_map_playanim, STRING("yure"), MAP_ANIM_PLAY_LOOP, 0)
                CALL(evt_map_playanim, STRING("hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
                CALL(evt_map_playanim, STRING("hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
                CALL(evt_map_playanim, STRING("hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
                CALL(evt_map_playanim, STRING("hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
            END_IF
        END_IF
    END_IF
    CALL(evt_npc_setup, PTR(npc_data))
    CALL_CHILD(evt_access_entry)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 210, 60, -145, 0, 0)
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
            IF_STR_EQUAL(LW(0), STRING("rsh_07_a"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
            END_IF

            // This variant leaves the current moving-train audio unchanged.
            IF_STR_EQUAL(LW(0), STRING("rsh_07_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_07_c"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Init script of rsh_07_a: sets up the door to rsh_04_a and runs the shared init.
 * Used on both the first trip and the second visit.
 */
EVT_BEGIN(rsh_07_a_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_a))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(rsh_07_init_evt)
    RETURN
EVT_END()

/**
 * Init script of rsh_07_b: sets up the door to rsh_04_b and runs the shared init.
 * Used on both the first trip and the second visit.
 */
EVT_BEGIN(rsh_07_b_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_b))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(rsh_07_init_evt)
    RETURN
EVT_END()

/** Map objects of the scenery that passes by the windows. */
static const char* window_tbl[] = {
    "mado_uturi_1",
    "mado_uturi_2",
    "mado_uturi_3",
    "mado_uturi_4",
    NULL,
};

/**
 * Init script of rsh_07_c: sets up the door to rsh_04_c,
 * runs the shared init and the window scenery (mado_uturi_1 to 4).
 * Used on both the first trip and the second visit.
 */
EVT_BEGIN(rsh_07_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(rsh_07_init_evt)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(evt_rsh_window)
    RETURN
EVT_END()
