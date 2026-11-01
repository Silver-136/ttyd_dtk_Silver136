// aji_18 (敵のアジト, X-Naut Fortress): the door with three key locks (key_lock_koujyou) and the machine.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "driver/fadedrv.h"
#include "evt/evt_bero.h"
#include "aji.h"
#include "driver/seqdrv.h"

s32 key_list[4];

static USER_FUNC(make_list);
static USER_FUNC(sysflag);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_machine[];
#else
EvtWord evt_machine[266];
#endif
EvtWord key_lock_koujyou[14];
static EvtWord evt_koopa_after[118];
#if VERSION >= VERSION_US
EvtWord aji_18_init_evt[301];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord aji_18_init_evt[296];
#else
EvtWord aji_18_init_evt[299];
#endif
#endif

/**
 * Script function: fills key_list with the Card Keys (item IDs 0x1D, 0x1E, 0x1F) that Mario carries,
 * ending the list with -1, for the key slot menu.
 */
static USER_FUNC(make_list) {
    s32 n = 0;

    if ((s32)pouchCheckItem(ITEM_CARD_KEY_0x1D) > 0) {
        key_list[n++] = ITEM_CARD_KEY_0x1D;
    }
    if ((s32)pouchCheckItem(ITEM_CARD_KEY_0x1E) > 0) {
        key_list[n++] = ITEM_CARD_KEY_0x1E;
    }
    if ((s32)pouchCheckItem(ITEM_CARD_KEY_0x1F) > 0) {
        key_list[n++] = ITEM_CARD_KEY_0x1F;
    }
    // At most three available keys precede the item-selection sentinel.
    key_list[n] = -1;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(w_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/** Exits of this room: the west door to aji_01 and the east door to aji_13. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "e_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_13", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Case script of the three key slots of the factory door (A_kagi1 to 3):
 * Mario puts a Card Key (ITEM_CARD_KEY_0x1D, 0x1E or 0x1F) from the item menu into the slot it belongs to.
 * A right key lights the slot (GSWF_AJI18_CARD_KEY_SLOT_1 to 3) and is used up; a wrong one gets stg7_aji_67_01.
 * When all three slots are filled the story becomes STG7_16_FACTORY_DOOR
 * and the door's lamp turns from red to green.
 */
EVT_BEGIN(evt_machine)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_67"), 0, 0)
    SET_VALUE(LW(10), 0)
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1D, LW(11))
    ADD(LW(10), LW(11))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1E, LW(11))
    ADD(LW(10), LW(11))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1F, LW(11))
    ADD(LW(10), LW(11))
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(make_list)
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(key_list), LW(10))
#else
    CALL(evt_win_item_select, 0, PTR(key_list), LW(10), 0)
#endif
    SWITCH(LW(10))
        CASE_EQUAL(ITEM_CARD_KEY_0x1D)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi1_aka"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi1_mid"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
#else
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
#endif
            CALL(evt_pouch_remove_item, LW(10), LW(1))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_DOOR_UNLOCK1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_exit_case_evt)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_aji_67_01"), 0, 0)
        END_IF
        CASE_EQUAL(ITEM_CARD_KEY_0x1E)
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi2_aka"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi2_mid"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
#else
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
#endif
            CALL(evt_pouch_remove_item, LW(10), LW(1))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_DOOR_UNLOCK1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_exit_case_evt)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_aji_67_01"), 0, 0)
        END_IF
        CASE_EQUAL(ITEM_CARD_KEY_0x1F)
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi3_aka"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi3_mid"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
#else
            SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
#endif
            CALL(evt_pouch_remove_item, LW(10), LW(1))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_DOOR_UNLOCK1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_exit_case_evt)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_aji_67_01"), 0, 0)
        END_IF
        CASE_DEFAULT
    END_SWITCH
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
            IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
                SET_VALUE(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
#else
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
            IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
                SET_VALUE(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
#endif
                CALL(evt_del_case_evt, 0, GW(10))
                SET_VALUE(LW(0), STRING("e_bero"))
#if VERSION <= VERSION_DEMO_US
                CALL_CHILD(bero_case_switch_on)
#else
                CALL_CHILD(PTR(bero_case_switch_on))
#endif
                CALL(evt_cam3d_evt_set, -140, 90, 369, 150, 60, -98, 500, EASING_SINE_IN_OUT)
                WAIT_MS(1200)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_DOOR_LAMP1"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(700)
                CALL(evt_cam3d_evt_off, 11, EASING_LINEAR)
            END_IF
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Case script of the exit e_bero while the factory door is locked: shows the locked-door text stg7_aji_67_02. */
EVT_BEGIN(key_lock_koujyou)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_67_02"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Scene played when Mario returns through e_bero after Lord Crump's defeat:
 * Mario and the partner walk in, the partner speaks stg7_aji_43 to 45 (Mario thinks, then nods),
 * and the partner is set to run after Mario again.
 */
static EVT_BEGIN(evt_koopa_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 75, 0, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_party_move_pos2, 0, 115, 0, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(500)
    CALL(evt_fade_end_wait)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print_party, STRING("stg7_aji_43"))
    CALL(evt_cam3d_evt_set, 95, 55, 292, 95, 41, -8, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg7_aji_44"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_Z"))
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("stg7_aji_45"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static USER_FUNC(sysflag) {
    if (gp->flags & GW_FLAG_SAVE_LOADED) {
        gp->flags &= ~GW_FLAG_SAVE_LOADED;
        fadeEntry(0x1C, 0, (GXColor){0, 0, 0, 0});
    }
    return EVT_RETURN_DONE;
}

/**
 * Init script of aji_18, the room with the factory door:
 * plays BGM_STG7_DUN2, sets up the exits, lights the three key slots by their flags (all filled from
 * STG7_16_FACTORY_DOOR on), attaches evt_machine to each empty slot
 * and locks the exit e_bero (key_lock_koujyou) before that point and between Lord Crump's defeat
 * and STG7_25_00_1_BOWSER_POSHLEY_SANCTUM.
 * In that last stretch, entering from e_bero puts a partner back with Mario if he has none,
 * heals Mario and the partners, and starts evt_koopa_after.
 */
#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(aji_18_init_evt)
    CALL(sysflag)
    // The fortress music and ambience start and the exits are set up.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    // From the factory-door story point on, all three card-key slots count as filled; each slot shows green
    // when filled and red when empty.
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi1_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi1_mid"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi1_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi1_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi2_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi2_mid"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi2_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi2_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi3_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi3_mid"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi3_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi3_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SET_VALUE(LW(0), 0)
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1D, LW(1))
    ADD(LW(0), LW(1))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1E, LW(1))
    ADD(LW(0), LW(1))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1F, LW(1))
    ADD(LW(0), LW(1))
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 0)
        SET_VALUE(LW(0), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi1"), 0, PTR(evt_machine), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 0)
        SET_VALUE(LW(0), 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi2"), 0, PTR(evt_machine), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 0)
        SET_VALUE(LW(0), 2)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi3"), 0, PTR(evt_machine), 0)
    END_IF
    // Before the factory door opens, the east exit is locked behind the key slots, and the light panels show
    // red.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(key_lock_koujyou), GW(10))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    // Between Lord Crump's defeat and the story point after the Moon, the east exit is locked again.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(key_lock_koujyou), 0)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
    END_SWITCH
    // In that stretch, entering from the east door gives Mario a partner back if he has none, heals everyone
    // and starts evt_koopa_after.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("e_bero"))
            CALL(evt_mario_get_party, LW(15))
            IF_INT_EQUAL(LW(15), 0)
                CALL(evt_mario_set_party, 0, 1)
                CALL(evt_party_run, 0)
CALL(evt_pouch_mario_recovery)
                CALL(evt_pouch_all_party_recovery)
END_IF
SPAWN_EVENT(evt_koopa_after)
        END_IF
    END_SWITCH
    // The emergency lights start.
    SPAWN_EVENT(evt_emergency)
    RETURN
EVT_END()
#else
EVT_BEGIN(aji_18_init_evt)
    CALL(sysflag)
    // The fortress music and ambience start and the exits are set up.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
        SET_VALUE(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi1_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi1_mid"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi1_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi1_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi2_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi2_mid"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi2_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi2_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi3_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi3_mid"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_kagi3_aka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kagi3_mid"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SET_VALUE(LW(0), 0)
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1D, LW(1))
    ADD(LW(0), LW(1))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1E, LW(1))
    ADD(LW(0), LW(1))
    CALL(evt_pouch_check_item, ITEM_CARD_KEY_0x1F, LW(1))
    ADD(LW(0), LW(1))
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED, 0)
        SET_VALUE(LW(0), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi1"), 0, PTR(evt_machine), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED, 0)
        SET_VALUE(LW(0), 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi2"), 0, PTR(evt_machine), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED, 0)
        SET_VALUE(LW(0), 2)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kagi3"), 0, PTR(evt_machine), 0)
    END_IF
    // Before the factory door opens, the east exit is locked behind the key slots, and the light panels show
    // red.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_16_FACTORY_DOOR)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(key_lock_koujyou), GW(10))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(key_lock_koujyou), 0)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_lights1_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_lights1_r"), MAP_OBJ_FLAG_HIDDEN)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("e_bero"))
            CALL(evt_mario_get_party, LW(15))
            IF_INT_EQUAL(LW(15), 0)
                CALL(evt_mario_set_party, 0, 1)
                CALL(evt_party_run, 0)
#if VERSION >= VERSION_US
            END_IF
#endif
                // US recovery is outside the missing-partner branch; the other version keeps it inside.
            CALL(evt_pouch_mario_recovery)
                CALL(evt_pouch_all_party_recovery)
#if VERSION >= VERSION_US
            CALL(evt_sub_status_gauge_force_update)
#else
            END_IF
#endif
            SPAWN_EVENT(PTR(evt_koopa_after))
        END_IF
    END_SWITCH
    // The emergency lights start.
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()
#endif

