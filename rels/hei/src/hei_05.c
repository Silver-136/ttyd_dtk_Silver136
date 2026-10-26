#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "npc_event.h"
#include "evt/evt_npc.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/npcdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[3];
static EvtWord dead_evt1[10];
static EvtWord dead_evt2[10];
static NpcSetupInfo npcEnt[3];
static EvtWord door_open[98];
static EvtWord shake_evt[39];
static EvtWord hashira_evt_sub[52];
static EvtWord hashira_evt[128];
EvtWord hei_05_init_evt[151];

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
        "hei_04", // nextMapName
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
        "hei_06", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(dead_evt1)
    SET_VALUE(GSWF(1792), 1)
    SET_VALUE(GSWF(1793), 1)
    CALL_CHILD(PTR(enemy_common_dead_event))
    RETURN
EVT_END()

static EVT_BEGIN(dead_evt2)
    SET_VALUE(GSWF(1793), 1)
    SET_VALUE(GSWF(1792), 1)
    CALL_CHILD(PTR(enemy_common_dead_event))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"シンエモン1"*/"\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93\x31", // name
        0, 0, // flags..reactionFlags
        kuriboo_init_event, // initEvent
        kuriboo_move_event, // regularEvent
        NULL, // talkEvent
        dead_evt1, // deadEvent
        kuriboo_find_event, // findEvent
        kuriboo_lost_event, // lostEvent
        kuriboo_return_event, // returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        1000.0f, // searchRange
        360.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        11, // battleInfoId
    },
    {
        /*"シンエモン2"*/"\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93\x32", // name
        0, 0, // flags..reactionFlags
        kuriboo_init_event, // initEvent
        kuriboo_move_event, // regularEvent
        NULL, // talkEvent
        dead_evt2, // deadEvent
        kuriboo_find_event, // findEvent
        kuriboo_lost_event, // lostEvent
        kuriboo_return_event, // returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        1000.0f, // searchRange
        360.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        11, // battleInfoId
    },
    {0},
};

static EVT_BEGIN(door_open)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_05_BRISTLE)
        CALL(evt_mario_key_onoff, 0)
        WAIT_MS(800)
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 6000)
        END_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAKE3"), LW(10))
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, -144, 61, 226, 276, 74, -43, 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1200)
        BEGIN_INLINE_EVENT
            CALL(evt_mapobj_get_position, STRING("MON"), LW(0), LW(1), LW(2))
            LOOP(8)
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GRID_UP2"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(140)
            END_LOOP
            WAIT_MS(280)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GRID_UP1"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_map_playanim, STRING("anm_mon"), MAP_ANIM_PLAY_ONCE, 0)
        CALL(evt_map_checkanim, STRING("anm_mon"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(evt_hitobj_onoff, STRING("a_mon"), 0, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG1_05_BRISTLE)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(shake_evt)
    CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
    LOOP(0)
        SET_VALUE(LW(1), LW(10))
        ADD(LW(1), 1)
        CALL(evt_npc_set_position, LW(0), LW(1), LW(11), LW(12))
        WAIT_FRAMES(2)
        SET_VALUE(LW(1), LW(10))
        SUBTRACT(LW(1), 1)
        CALL(evt_npc_set_position, LW(0), LW(10), LW(11), LW(12))
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(hashira_evt_sub)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_TOGEDA_JUMP1"), LW(10), LW(11), LW(12), 0)
    IF_INT_LESS(LW(10), 0)
        ADD(LW(10), 50)
        ELSE
        SUBTRACT(LW(10), 50)
    END_IF
    CALL(evt_npc_jump_position_nohit, LW(0), LW(10), 0, 100, 700, 30)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_TOGEDA_LANDING1"), LW(10), 0, 100, 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 100)
    RETURN
EVT_END()

static EVT_BEGIN(hashira_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_QUAKE6"), LW(10), LW(11), LW(12), LW(14))
    SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(15))
    WAIT_MS(500)
    CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_cam3d_evt_set_at, LW(10), LW(11), LW(12), 50, 600, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    WAIT_MS(1000)
    DELETE_EVENT(LW(15))
    CALL(evt_snd_sfxoff, LW(14))
    CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
    SET_VALUE(LW(0), LW(0))
    SPAWN_EVENT_GET_ID(PTR(hashira_evt_sub), LW(15))
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(GSWF(1789), 0)
            SET_VALUE(LW(0), LW(1))
            SPAWN_EVENT(PTR(hashira_evt_sub))
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(1788), 0)
            SET_VALUE(LW(0), LW(1))
            SPAWN_EVENT(PTR(hashira_evt_sub))
        END_IF
    END_IF
    LABEL(0)
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_link, STRING("シンエモン1"), STRING("シンエモン2"))
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mario_key_onoff, 1)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(GF(1), 1)
            CALL(evt_del_case_evt, 0, GW(1))
        END_IF
        ELSE
        IF_INT_EQUAL(GF(0), 1)
            CALL(evt_del_case_evt, 0, GW(0))
        END_IF
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(hei_05_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(1))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI4"))
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_map_playanim, STRING("anm_hokori"), MAP_ANIM_PLAY_LOOP, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_05_BRISTLE)
        IF_INT_EQUAL(GSWF(1792), 0)
            CALL(evt_npc_set_anim, STRING("シンエモン1"), STRING("KUR_Z_1"))
            CALL(evt_npc_set_position, STRING("シンエモン1"), -119, 65, -77)
            CALL(evt_npc_set_ry, STRING("シンエモン1"), 90)
            CALL(evt_npc_flag_onoff, 1, STRING("シンエモン1"), NPC_FLAG_INSTANT_TURN)
            SET_RAW_VALUE(LW(0), STRING("シンエモン1"))
            SET_RAW_VALUE(LW(1), STRING("シンエモン2"))
            SET_VALUE(LW(2), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_hasira_1"), 0, PTR(hashira_evt), GW(0))
            SET_VALUE(GF(0), 1)
        END_IF
        IF_INT_EQUAL(GSWF(1793), 0)
            CALL(evt_npc_set_anim, STRING("シンエモン2"), STRING("KUR_Z_1"))
            CALL(evt_npc_set_position, STRING("シンエモン2"), 120, 62, -77)
            CALL(evt_npc_set_ry, STRING("シンエモン2"), 270)
            CALL(evt_npc_flag_onoff, 1, STRING("シンエモン2"), NPC_FLAG_INSTANT_TURN)
            SET_RAW_VALUE(LW(0), STRING("シンエモン2"))
            SET_RAW_VALUE(LW(1), STRING("シンエモン1"))
            SET_VALUE(LW(2), 1)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_hasira_2"), 0, PTR(hashira_evt), GW(1))
            SET_VALUE(GF(1), 1)
        END_IF
        BEGIN_INLINE_EVENT
            LABEL(0)
            IF_INT_EQUAL(GSWF(1792), 0)
                WAIT_FRAMES(1)
                GOTO(0)
            END_IF
            IF_INT_EQUAL(GSWF(1793), 0)
                WAIT_FRAMES(1)
                GOTO(0)
            END_IF
            CALL_CHILD(PTR(door_open))
        END_INLINE_EVENT
        ELSE
        CALL(evt_hitobj_onoff, STRING("a_mon"), 0, 0)
        CALL(evt_map_playanim, STRING("anm_mon"), MAP_ANIM_PLAY_AT_END, 0)
    END_IF
    RETURN
EVT_END()

