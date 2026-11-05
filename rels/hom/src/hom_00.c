// hom_00 (途中駅ホーム, Excess Express platform): the platform where the Excess Express stops; the locked door into the station
// (eki_00) needs the Station Key.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "driver/envdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "driver/hitdrv.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "memory.h"
#include "mario/mariost.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"
#include "evt/evt_bero.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "party/party_motion.h"
#include "hom.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"
#include "driver/camdrv.h"

static smartEntry* ind_lp;
void envGlareFilter(s32 on, s16 x0, s16 y0, s16 x1, s16 y1);

static USER_FUNC(test_glare);
USER_FUNC(tokei);
static void sun_tev(MapTevCallbackInfo* info);

static EvtWord n_2_door_open[22];
static EvtWord n_2_door_close[22];
static BeroEntry bero_data[3];
BeroEntry bero_data_nidome[2];
EvtWord evt_carry_up_bridge[92];
EvtWord init_powan_toron_konari_papa_gurume[23];
static EvtWord talk_powan[8];
static EvtWord talk_toron[8];
static EvtWord talk_gurume[8];
static EvtWord talk_papa[8];
static EvtWord talk_konari[8];
EvtWord init_ekiin1[2];
EvtWord talk_ekiin1[32];
EvtWord init_ekiin2[13];
EvtWord talk_ekiin2[28];
static NpcSetupInfo npc_data[9];
EvtWord evt_bridge_down[144];
static s32 key_tbl[2];

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_chk[21];
#else
static EvtWord evt_chk[23];
#endif

static EvtWord evt_lock[10];
EvtWord evt_train_door[13];
static EvtWord evt_train[262];
EvtWord evt_train_nidome[307];
EvtWord evt_hom_touchaku[460];
EvtWord evt_hom_touchaku_nidome[56];

#if VERSION >= VERSION_US
EvtWord hom_00_init_evt[259];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord hom_00_init_evt[223];
#else
EvtWord hom_00_init_evt[247];
#endif
#endif

/**
 * Door-swing callback when opening the station door (n_bero_2): LW(0) is the door angle;
 * the two leaves tobira_1 and tobira_2 turn around Y by 0.39 times it in opposite directions.
 */
static EVT_BEGIN(n_2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door-swing callback when closing the station door (n_bero_2):
 * same as n_2_door_open with the angle direction reversed.
 */
static EVT_BEGIN(n_2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Doors of this room: the north exit (n_bero_1) to rsh_01_b (Excess Express) and the north exit (n_bero_2)
 * to eki_00 (Riverside Station).
 */
static BeroEntry bero_data[] = {
    {
        "n_bero_1", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        45, // length
        NULL, // enterEvent
        -1, // caseType
        evt_train_door, // exitEvent
        "rsh_01_b", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "n_1_door_close", // enterAnimeArg
        "n_1_door_open", // exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_00", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_2_door_close, // enterAnimeArg
        n_2_door_open, // exitAnimeArg
    },
    {0},
};

/** Doors of this room: the north exit (n_bero_1) to rsh_01_b (Excess Express). */
BeroEntry bero_data_nidome[] = {
    {
        "n_bero_1", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        45, // length
        NULL, // enterEvent
        -1, // caseType
        evt_train_nidome, // exitEvent
        "rsh_01_b", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "n_1_door_close", // enterAnimeArg
        "n_1_door_open", // exitAnimeArg
    },
    {0},
};

/**
 * Talk script of station attendant 1 (ekiin1) about the raised drawbridge:
 * explains the bridge (stg6_hom_04/05, or stg6_hom_10 on a repeat),
 * then asks whether Mario wants the Station Key (yes/no);
 * on yes he is given the key (ITEM_STATION_KEY_0x18) and the story becomes "Station Key obtained".
 */
EVT_BEGIN(evt_carry_up_bridge)
    IF_INT_EQUAL(GSWF_HOM_DRAWBRIDGE_PROBLEM_EXPLAINED, 0)
        CALL(evt_msg_print, 0, STRING("stg6_hom_04"), 0, STRING("me"))
        CALL(evt_cam3d_evt_set, -65, 33, 575, 213, 64, 102, 3000, EASING_SINE_IN_OUT)
        WAIT_MS(6000)
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        WAIT_MS(200)
    END_IF
    IF_INT_EQUAL(GSWF_HOM_DRAWBRIDGE_PROBLEM_EXPLAINED, 0)
        SET_VALUE(GSWF_HOM_DRAWBRIDGE_PROBLEM_EXPLAINED, 1)
        CALL(evt_msg_print, 0, STRING("stg6_hom_05"), 0, STRING("me"))
    ELSE
        CALL(evt_msg_print, 0, STRING("stg6_hom_10"), 0, STRING("me"))
    END_IF
    CALL(evt_msg_select, 0, STRING("stg6_hom_10_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg6_hom_06"))
        CALL(evt_item_entry, STRING("a"), ITEM_STATION_KEY_0x18, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_item_get_item, STRING("a"))
        CALL(evt_item_get_item_end_wait, STRING("a"))
        CALL(evt_msg_print, 0, STRING("stg6_hom_08"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, STG6_28_STATION_KEY_OBTAINED)
    ELSE
        CALL(evt_msg_print_add, 0, STRING("stg6_hom_09"))
    END_IF
    RETURN
EVT_END()

/**
 * Init script shared by the platform NPCs Pennington, the fake Zip Toad, Heff T., Goldbob and Bub:
 * removes the NPC from the map once the drawbridge is down (story STG6_42_DRAWBRIDGE_DOWN),
 * and the fake Zip Toad once the station has been reached (story STG6_27_RIVERSIDE_STATION).
 */
EVT_BEGIN(init_powan_toron_konari_papa_gurume)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_27_RIVERSIDE_STATION)
        CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

/** Talk event of Pennington the detective (ポワン探偵): says stg6_hom_00. */
static EVT_BEGIN(talk_powan)
    CALL(evt_msg_print, 0, STRING("stg6_hom_00"), 0, 0)
    RETURN
EVT_END()

/** Talk event of the fake Zip Toad (トロン, Doopliss in disguise): says stg6_hom_01 (left untranslated in US). */
static EVT_BEGIN(talk_toron)
    CALL(evt_msg_print, 0, STRING("stg6_hom_01"), 0, 0)
    RETURN
EVT_END()

/** Talk event of Heff T., the gourmet Toad (グルメキノピオ): says stg6_rsh_211. */
static EVT_BEGIN(talk_gurume)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_211"), 0, 0)
    RETURN
EVT_END()

/** Talk event of Goldbob (ナリキンパパ): says stg6_hom_02. */
static EVT_BEGIN(talk_papa)
    CALL(evt_msg_print, 0, STRING("stg6_hom_02"), 0, 0)
    RETURN
EVT_END()

/** Talk event of Bub, Goldbob's son (コナリキン): says stg6_hom_03. */
static EVT_BEGIN(talk_konari)
    CALL(evt_msg_print, 0, STRING("stg6_hom_03"), 0, 0)
    RETURN
EVT_END()

/** Init script of station attendant 1; does nothing. */
EVT_BEGIN(init_ekiin1)
    RETURN
EVT_END()

/**
 * Talk script of station attendant 1: before the key is obtained, runs evt_carry_up_bridge;
 * then gives the drawbridge, waiting-for-the-train and after-departure lines by story progress.
 */
EVT_BEGIN(talk_ekiin1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_28_STATION_KEY_OBTAINED)
        CALL_CHILD(evt_carry_up_bridge)
        CASE_LESS(STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_msg_print, 0, STRING("stg6_hom_11"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_hom_12"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_hom_13"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of station attendant 2: once the drawbridge is down (story STG6_42_DRAWBRIDGE_DOWN),
 * moves him to the gate at (-865, 80, 100).
 */
EVT_BEGIN(init_ekiin2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), -865, 80, 100)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of station attendant 2 (at the ticket gate):
 * different lines before the drawbridge is down, while waiting for the conductor, and afterwards.
 */
EVT_BEGIN(talk_ekiin2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_eki_04"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_hom_16"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * NPCs of the platform: Pennington the detective, the fake Zip Toad, Heff T. (the gourmet Toad), Goldbob, Bub,
 * the two station attendants and Doopliss (ランペル).
 */
static NpcSetupInfo npc_data[] = {
    {
        "ポワン探偵", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "トロン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_toron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "グルメキノピオ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_gurume, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ナリキンパパ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_papa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "コナリキン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_konari, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "駅員1", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ekiin1, // initEvent
        NULL, // regularEvent
        talk_ekiin1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "駅員2", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ekiin2, // initEvent
        NULL, // regularEvent
        talk_ekiin2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ランペル", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_bridge_down)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set_xyz, -350, 15, 0, 0, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set_xyz, 100, 15, 0, 0, 5000, EASING_LINEAR)
    WAIT_MS(5300)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("a"), STRING("駅員1"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("駅員1"), 90)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -65, 33, 575, 213, 64, 102, 2000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_map_playanim, STRING("hasi"), MAP_ANIM_PLAY_ONCE, 0)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG6_BRIDGE_MOVE1"), LW(10))
        CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_BRIDGE_MOVE2"), 0)
    END_INLINE_EVENT
    CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
    MULTIPLY(LW(1), 2)
    DIVIDE(LW(1), 3)
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 275, 260, 1400, 275, 210, 0, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0009765625f), FLOAT(0.0009765625f), 200)
    WAIT_MS(2000)
    CALL(evt_bero_mapchange, STRING("eki_00"), STRING("s_bero"))
    RETURN
EVT_END()

/** Item list of the station door's key picker: only the Station Key (-1 ends the list). */
static s32 key_tbl[] = {
    ITEM_STATION_KEY_0x18, -1,
};

/**
 * Check script of the locked station door: if Mario carries a Station Key it opens the key picker (key_tbl)
 * and cancels the unlock (evt_mobj_exec_cancel) if he backs out of it.
 */
static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, ITEM_STATION_KEY_0x18, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_win_item_select, PTR(key_tbl), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Runs when the Station Key is used on the lock:
 * switches the station door's entry on (bero_case_switch_on with LW(0) = 1)
 * and sets story progress to STG6_29_STATION_DOOR_UNLOCKED.
 */
static EVT_BEGIN(evt_lock)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(bero_case_switch_on)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_29_STATION_DOOR_UNLOCKED)
    RETURN
EVT_END()

/**
 * Exit event of the train door: returns 1 in LW(0) only while the story is at "Drawbridge down",
 * so the door can be used just then.
 */
EVT_BEGIN(evt_train_door)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG6_42_DRAWBRIDGE_DOWN)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_train)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(10), LW(11), LW(12), LW(13), LW(14), LW(15))
    CALL(evt_cam3d_evt_set, LW(10), LW(11), LW(12), LW(13), LW(14), LW(15), 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -820, 30, 500)
        CALL(evt_mario_set_dir_npc, STRING("駅員2"))
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -810, 70, 300)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -810, 20, 500)
    CALL(evt_party_set_dir_npc, 0, STRING("駅員2"))
    CALL(evt_npc_status_onoff, 1, STRING("駅員2"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("駅員2"), -800, 80, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_close"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_close"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(300)

    // He calls the train off and the crossing barrier opens.
    CALL(evt_msg_print, 0, STRING("stg6_eki_05"), 0, STRING("駅員2"))
    WAIT_MS(400)
    CALL(evt_cam3d_evt_set, 318, 80, 347, -134, 65, 35, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_CROSSING_OPEN1"), -170, 30, 0, LW(10))
    CALL(evt_map_playanim, STRING("shadanki"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("shadanki"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_sfxoff, LW(10))
    SET_VALUE(GF(15), 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("eki_03_S"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("group614"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    SPAWN_EVENT(hom_10_evt_resha_start)
    WAIT_MS(4500)
    CALL(evt_cam3d_evt_set, 50, 125, 500, 50, 75, 0, 1500, EASING_SINE_OUT)
    WAIT_MS(1500)
    WAIT_MS(4500)
    CALL(evt_npc_set_ry, STRING("駅員1"), 90)
    CALL(evt_cam3d_evt_set, -150, 25, 250, 273, 75, -10, 1500, EASING_SINE_IN)
    WAIT_MS(1500)
    WAIT_MS(4000)
    SET_VALUE(GSWF_HOM_TRAIN_BOARDED_UNUSED, 1)
    SET_VALUE(GSWF_HOM_BOARDED_AT_RIVERSIDE_UNUSED, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_43_CONDUCTOR_APPEARS)
    WAIT_FRAMES(1)
    CALL(evt_bero_mapchange, STRING("rsh_02_c"), STRING("hom_hassya"))
    RETURN
EVT_END()

/**
 * Exit event of the train door on the second trip:
 * Mario boards, attendant 2 calls the train off, the crossing barrier opens,
 * the train starts (hom_10_evt_resha_start, or start2 on the return trip),
 * the train-leg counter is set to 1 (outbound) or 3 (return) and the map changes to rsh_02_c.
 */
EVT_BEGIN(evt_train_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
    END_IF

    // Mario freezes and the train door slides open.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("n_bero_1"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_open"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_open"), LW(0), LW(1))
    WAIT_MS(LW(1))

    // Mario and his partners walk up to the door.
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -820, 12, 500)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -800, 40, 300)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -800, 20, 500)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_stop, 1)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 1, -810, 40, 300)
    CALL(evt_party_stop, 1)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 1, -810, 30, 500)
    CALL(evt_party_set_dir, 1, 270, 200)

    // Attendant 2 walks over and the door closes behind them.
    CALL(evt_npc_status_onoff, 1, STRING("駅員2"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("駅員2"), -800, 80, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_close"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_close"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_eki_05"), 0, STRING("駅員2"))
    WAIT_MS(400)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_CROSSING_OPEN1"), -170, 30, 0, LW(10))
    CALL(evt_map_playanim, STRING("shadanki"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("shadanki"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_sfxoff, LW(10))

    // Everyone vanishes from the platform and the train pulls out, outbound or on the return trip.
    SET_VALUE(GF(15), 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("eki_03_S"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("group614"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(6000)
            CALL(evt_npc_set_ry, STRING("駅員2"), 90)
        END_INLINE_EVENT
        SPAWN_EVENT(hom_10_evt_resha_start)
    ELSE
        BEGIN_INLINE_EVENT
            WAIT_MS(6000)
            CALL(evt_npc_set_ry, STRING("駅員2"), 270)
        END_INLINE_EVENT
        SPAWN_EVENT(hom_10_evt_resha_start2)
    END_IF
    WAIT_MS(10000)

    // The train is marked as departed, the trip leg is set and the map changes to the train (rsh_02_c).
    SET_VALUE(GSWF_HOM_TRAIN_BOARDED_UNUSED, 1)
    SET_VALUE(GSWF_HOM_BOARDED_AT_RIVERSIDE_UNUSED, 1)
    WAIT_FRAMES(1)
    SET_VALUE(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(GSW_HOM_TRAIN_LEG, 1)
    ELSE
        SET_VALUE(GSW_HOM_TRAIN_LEG, 3)
    END_IF
    CALL(evt_bero_mapchange, STRING("rsh_02_c"), STRING("nidome_start"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/**
 * Arrival cutscene at Riverside Station (story before 27):
 * the Excess Express pulls in, the doors open, the fake Zip Toad and Pennington step off, the fake turns into
 * Doopliss, who speaks and hops away, the story becomes "Riverside Station" and the map changes to rsh_02_b.
 */
EVT_BEGIN(evt_hom_touchaku)
    // The party is hidden and the platform NPCs are moved off screen.
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 275, 260, 1400, 275, 210, 0, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("グルメキノピオ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ナリキンパパ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("コナリキン"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("トロン"), 90)
    CALL(evt_npc_set_ry, STRING("ポワン探偵"), 90)
    CALL(evt_npc_set_position, STRING("駅員2"), -750, 80, 100)
    CALL(evt_seq_wait, SEQ_GAME)

    // A caption is shown while the Excess Express rolls into the platform and stops.
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_telop_entry, 11)
    END_INLINE_EVENT
    CALL_CHILD(hom_10_evt_resha_stop)
    WAIT_MS(2500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 458, 97, 366, 78, 110, 189, 3000, EASING_SINE_IN_OUT)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)

    // The camera moves to the door; Pennington and the fake Zip Toad step off while the door opens and closes.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -810, 232, 628, -810, 137, 87, 0, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -815, 80, 15)
    CALL(evt_npc_set_position, STRING("トロン"), -800, 80, 20)
    CALL(evt_set_dir_to_target, STRING("駅員2"), STRING("ポワン探偵"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -40, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("tobira"), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_status_onoff, 1, STRING("駅員2"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("トロン"), -800, 100, 0, FLOAT(60.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("トロン"), -780, 100, 0, FLOAT(60.0f), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ポワン探偵"), -815, 100, 0, FLOAT(60.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("トロン"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -40, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("tobira"), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF

    // The fake Zip Toad turns around, then changes into Doopliss in a puff of smoke.
    LOOP(3)
        CALL(evt_npc_reverse_ry, STRING("トロン"))
        WAIT_MS(500)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg6_hom_01_01"), 0, STRING("トロン"))
    CALL(evt_npc_set_anim, STRING("駅員2"), STRING("G_1"))
    CALL(evt_npc_set_anim, STRING("ポワン探偵"), STRING("O_1"))
    CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_LANPEL_CHANGE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
        CALL(evt_npc_set_position, STRING("ランペル"), LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    ADD(LW(2), 20)
    LOOP(3)
        CALL(evt_eff, 0, STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -780, 194, 481, -780, 125, 87, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)

    // Doopliss says his piece and hops away across the platform.
    CALL(evt_msg_print, 0, STRING("stg6_hom_01_02"), 0, STRING("ランペル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -780, 259, 776, -780, 137, 87, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_sound_data_set, STRING("ランペル"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, STRING("SFX_STG6_LANPEL_JUMP1"), EVT_NULLPTR)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_status_onoff, 1, STRING("ランペル"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
        ADD(LW(0), 60)
        CALL(evt_npc_jump_position_nohit, STRING("ランペル"), LW(0), LW(1), LW(2), 400, 40)
        IF_INT_GREATER(LW(0), -500)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_bgm_scope, 0, 0)

    // The story advances to Riverside Station and the map changes to rsh_02_b.
    SET_VALUE(GSW_STORY_PROGRESS, STG6_27_RIVERSIDE_STATION)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("hom_touchaku"))
    RETURN
EVT_END()

/**
 * Arrival cutscene on the second visit (entering through "nidome"):
 * the train pulls in, GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE is set and the map changes to rsh_02_b.
 */
EVT_BEGIN(evt_hom_touchaku_nidome)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 406, 317, 596, -389, 137, -44, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL_CHILD(hom_10_evt_resha_stop)
    WAIT_MS(2500)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    SET_VALUE(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 1)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("hom_touchaku"))
    RETURN
EVT_END()

/**
 * Riverside Station platform (hom_00) map init:
 * sets up the NPCs, music, save block and recovery block, the train on the correct side,
 * the sun's heat-haze effect and glare, the station clock,
 * the drawbridge and the locked station door (opened with the Station Key),
 * and starts the arrival or drawbridge scene as the story requires.
 */
EVT_BEGIN(hom_00_init_evt)
    // NPCs, music, ambience, the save block and the recovery block.
    SET_VALUE(GF(15), 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_HOM1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI1"))
#else
#if VERSION >= VERSION_US
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("hasi_L"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("hasi_R"), MAP_OBJ_FLAG_CLIP)
#endif
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_HOM1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI1"))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
#endif
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -175, 140, -170, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 15, -125, 140, -170, 0, 0)

    // The train is placed on the correct side of the platform.
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL_CHILD(hom_10_evt_resha_right)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL_CHILD(hom_10_evt_resha_left)
    END_IF

    // Late in the story the crossing barrier is shown in its open position.
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_map_playanim, STRING("shadanki"), MAP_ANIM_PLAY_AT_END, 0)
#else
        CALL(evt_bero_get_entername, LW(0))
        CALL(evt_sub_get_system_flag, LW(1))
        IF_STR_NOT_EQUAL(LW(0), STRING("n_bero_1"))
            IF_STR_NOT_EQUAL(LW(0), STRING("n_bero_2"))
                IF_BITS_CLEAR(LW(1), 1)
                    CALL(evt_map_playanim, STRING("shadanki"), MAP_ANIM_PLAY_AT_END, 0)
                END_IF
            END_IF
        END_IF
#endif
    END_IF

    // The sun's heat haze and glare, and the station clock hands.
    CALL(evt_map_set_tevcallback, 2, PTR(sun_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sun"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("sun"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sun"), MAP_OBJ_FLAG_NO_COLOR_STAGE)
    BEGIN_INLINE_EVENT
        CALL(test_glare)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(tokei)
            CALL(evt_mapobj_rotate, 1, STRING("short"), 0, LW(0), 0)
            CALL(evt_mapobj_rotate, 0, STRING("long"), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT

    // The drawbridge scene, the doors, and the arrival scenes of the first and second visits.
    IF_INT_EQUAL(GSWF_EKI_BRIDGE_LOWERING_IN_PROGRESS, 1)
        SPAWN_EVENT(evt_bridge_down)
        RETURN
    END_IF
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_27_RIVERSIDE_STATION)
        SPAWN_EVENT(evt_hom_touchaku)
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome"))
        SPAWN_EVENT(evt_hom_touchaku_nidome)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_55_PEACH_GRODUS_X_NAUT)
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("n_bero_1"), PTR(bero_data_nidome))
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(bero_case_switch_on)
    END_IF

    // The station door stays locked until the Station Key has been used.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_29_STATION_DOOR_UNLOCKED)
        CALL(evt_mobj_lock, STRING("lock"), 24, -260, 90, -190, 0, PTR(evt_chk), PTR(evt_lock), GSWF_HOM_00_STATION_DOOR_LOCK)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(bero_case_switch_off)
    END_IF

    // From the drawbridge-down step on, the bridge is lowered; at that step, the train's boarding event is registered.
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_map_playanim, STRING("hasi"), MAP_ANIM_PLAY_AT_END, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_hitobj_attr_onoff, 0, 1, STRING("n_bero_1"), HIT_ATTR_BERO)
        CALL(evt_run_case_evt, CASE_FRONT, 1, STRING("n_bero_1"), 0, PTR(evt_train), 0)
        CALL(evt_map_playanim, STRING("n_1_door_open"), MAP_ANIM_PLAY_AT_END, 0)
    END_IF
    SPAWN_EVENT(evt_sub_mail_event)
#if VERSION >= VERSION_JP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_2"), MAP_OBJ_FLAG_CLIP)
#endif
    RETURN
EVT_END()

/**
 * Texture-environment callback for the sun: every frame it fills a 64x64 indirect texture with a scrolling sine
 * wave and sets up an indirect warp so the sun shimmers like a heat haze.
 */
static void sun_tev(MapTevCallbackInfo* info) {
    static const s32 texmtx_tbl[] = {
        GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4,
        GX_TEXMTX5, GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9,
    };
    s32 stage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;
    GXTexObj obj;
    f32 mtx[2][3];
    u8* buf;
    s32 i;
    s32 j;

    // a scrolling sine-wave warp texture for the sun's heat haze
    if (ind_lp == NULL) {
        ind_lp = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), SMART_GROUP_MAP);
    }
    buf = ind_lp->address;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            f32 v = sin(31.41592f * j / 64 - gp->retraceCount / 8.0f);
            s32 off;

            off = i / 4;
            off = (off << 5) + ((j / 4) << 9);
            off += ((i % 4) + ((j % 4) << 2)) * 2;
            buf[off] = 3.0f * v + 128.0f;
            buf[off + 1] = 0x80;
        }
    }
    DCFlushRange(buf, 0x2000);
    GXInitTexObj(&obj, buf, 64, 64, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, texMap);
    mtx[0][0] = 0.5f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.5f;
    mtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, mtx, 1);
    GXSetIndTexOrder(indStage, GX_TEXCOORD0, texMap);
    GXSetIndTexCoordScale(indStage, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, indStage, GX_TRUE, GX_FALSE, GX_ITM_0);
    info->tevStage = stage;
    info->texCoord = texCoord;
    info->indStage = indStage + 1;
    info->texMap = texMap + 1;
}

/**
 * Script helper, runs forever: draws a glare filter over the sun's screen rectangle (grown by 50 pixels),
 * switching it off when the sun is off screen or the train has left (GF(15)).
 */
static USER_FUNC(test_glare) {
    MapObject* sun = mapGetMapObj("sun");
    u16* rect;

    if (isFirstCall) {
        sun->flags |= MAP_OBJ_FLAG_SCREEN_RECT;
        return EVT_RETURN_BLOCK;
    }
    rect = sun->screenRect;
    if (rect[0] == 0x3FF || rect[1] == 0x3FF || rect[2] == 0x3FF || rect[3] == 0x3FF) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else if (rect[0] == 0 && rect[1] == 0 && rect[2] == 0 && rect[3] == 0) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else if (evtGetValue(event, GF(15))) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else {
        envGlareFilter(1, rect[1] - 50, rect[0] - 50, rect[3] + 50, rect[2] + 50);
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: sets the hour and minute hand rotation of the station clock (LW(0) and LW(1)) from the
 * console's real-time clock.
 */
USER_FUNC(tokei) {
    s32 min;
    OSCalendarTime time;
    u32 sec;

    OSTicksToCalendarTime(OSGetTime(), &time);
    min = time.min * 60;
    sec = time.hour * 3600 + min + time.sec;
    event->lwData[0] = FLOAT(-(360.0f * (sec / 3600.0f) / 12.0f));
    event->lwData[1] = FLOAT(-(360.0f * (sec / 60.0f) / 60.0f));
    return EVT_RETURN_DONE;
}
