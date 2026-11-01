// hei_08 (平原, Petal Meadows): Shhwonk Fortress room off hei_09 where a Fuzzy (chorobon) jumps in and fights Mario.
// Holds the Moon Stone item.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"

#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "npc_event.h"

static USER_FUNC(npc_dead);
static USER_FUNC(npc_set_me);
extern s32 hei_test_water_tev[];

static BeroEntry bero_entry_data[2];
static EvtWord evt_chorobon[136];
#if VERSION <= VERSION_DEMO_US
extern EvtWord hei_08_init_evt[];
#else
EvtWord hei_08_init_evt[104];
#endif

/** Doors of this room: the east exit (e_bero) to hei_09. */
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
        "hei_09", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Script helper: makes the Fuzzy (Chorobon) NPC the owner of the running event, so its script commands act on it. */
static USER_FUNC(npc_set_me) {
    // チョロボン
    event->ownerNPC = npcNameToPtr("\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93");
    return EVT_RETURN_DONE;
}

/**
 * The Fuzzy ambush at the Moon Stone: sets GSWF_HEI_MOON_STONE_TAKEN,
 * then a Fuzzy (チョロボン) drops in on Mario with SFX_STG1_CHORO_FALL1 and Mario is startled.
 * Battle starts (interrupt chorobon_move_event);
 * if Mario flees, the Fuzzy is removed with the common dead event,
 * otherwise the enemy flags are restored and field battle mode is switched on.
 */
static EVT_BEGIN(evt_chorobon)
    SET_VALUE(GSWF_HEI_MOON_STONE_TAKEN, 1)
    CALL(evt_mario_wait_movable)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), NPC_FLAG_ENEMY)
    CALL(evt_npc_flag_onoff, 1, STRING("チョロボン"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("チョロボン"), LW(3), LW(4), LW(5))
    CALL(evt_npc_set_position, STRING("チョロボン"), LW(0), LW(4), LW(2))
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHORO_FALL1"), LW(0), LW(4), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position, STRING("チョロボン"), 0, 0, 0, 600, 0, FLOAT(30.0f), 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    WAIT_MS(500)
    CALL(evt_npc_change_interrupt, STRING("チョロボン"), 1, PTR(chorobon_move_event))
    CALL(evt_npc_restart_regular_event, STRING("チョロボン"))
    CALL(evt_npc_battle_start, STRING("チョロボン"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_npc_get_battle_result, LW(0))
    CALL(evt_npc_check_escape_battle, LW(0))
    IF_INT_EQUAL(LW(0), BATTLE_RESULT_WIN)
        CALL(npc_set_me)
        CALL(npc_dead)
        CALL_CHILD(PTR(enemy_common_dead_event))
        ELSE
        BEGIN_INLINE_EVENT
            CALL(evt_npc_flag_onoff, 1, STRING("チョロボン"), NPC_FLAG_ENEMY)
            CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), NPC_FLAG_NO_HIT)
        END_INLINE_EVENT
        CALL(evt_npc_change_fbat_mode, 1)
    END_IF
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Petal Meadows hei_08 map init: sets the fortress music and reverb and the water effect;
 * until the Moon Stone is taken, parks the Fuzzy above the room
 * and places the Moon Stone item whose pickup runs evt_chorobon; later the Fuzzy gets its battle info back.
 */
EVT_BEGIN(hei_08_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_TORIDE1"))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI3"))
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_npc_change_fbat_mode, 1)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_HEI_MOON_STONE_TAKEN, 0)
#else
    IF_INT_EQUAL(GSWF_HEI_MOON_STONE_TAKEN, 0)
#endif
        CALL(evt_npc_change_interrupt, STRING("チョロボン"), 1, 0)
        CALL(evt_npc_restart_regular_event, STRING("チョロボン"))
        CALL(evt_npc_set_position, STRING("チョロボン"), 0, 400, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_npc_change_fbat_mode, 7)
        ELSE
        CALL(evt_npc_change_fbat_mode, 1)
#if VERSION > VERSION_DEMO_US
        CALL(evt_npc_check, STRING("チョロボン"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_set_battle_info, STRING("チョロボン"), 16)
#endif
        END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_HEI_MOON_STONE_TAKEN, 0)
#else
    END_IF
    IF_INT_EQUAL(GSWF_HEI_MOON_STONE_TAKEN, 0)
#endif
        CALL(evt_item_entry, STRING("moon_st"), ITEM_MOON_STONE, 0, 0, 0, ITEM_TYPE_GROUND, -1, PTR(evt_chorobon))
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(hei_test_water_tev))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("mizu_1"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("mizu_1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()

/** Script helper: removes the whole Fuzzy (Chorobon) group from the map after the fight. */
static USER_FUNC(npc_dead) {
    // チョロボン
    npcGroupDead(npcNameToPtr("\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93"), FALSE);
    return EVT_RETURN_DONE;
}

