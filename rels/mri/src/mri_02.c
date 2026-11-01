// mri_02 (巨木の中, The Great Tree): the room where a Puni switch (punipuni_switch) makes a pipe rise out of
// the floor (dokan_event); planters and pipes to mri_01, mri_05 and mri_20.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/hitdrv.h"
#include "driver/mapdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "mri_puni.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "pmario_sound.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"

typedef struct Mri02Switch {
    f32 base;
    f32 weight;
    f32 target;
    f32 pos;
} Mri02Switch;
Mri02Switch mri02switch;

USER_FUNC(getSwitch);
USER_FUNC(moveSwitch);
USER_FUNC(setPuniWeight);

static BeroEntry bero_entry_data[4];
static EvtWord evt_ueki_04[20];
static KusaAccess ueki_access_data[6];
EvtWord punipuni_switch[4];
EvtWord dokan_event[123];
EvtWord dokan_init[35];
static EvtWord yuka_main[51];
static EvtWord yuka_move[130];
static EvtWord punipuni_ball00_init[35];
static EvtWord punipuni_ball00[59];
EvtWord mri_02_init_evt[167];

/**
 * Exits of mri_02: pipe dokan1 to mri_01 (dokan); pipe dokan2 to mri_05 (dokan3); pipe dokan3 to mri_20
 * (dokan2).
 */
static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_01", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_05", // nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_E, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_20", // nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Planter hit script 4: the first time the planter is hit it launches a Power Punch badge out of it;
 * it is remembered with a flag so it comes out only once.
 */
static EVT_BEGIN(evt_ueki_04)
    CALL(evt_item_delete_check, STRING("item_ueki_04"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_04"), ITEM_POWER_PUNCH, 100, 0, -20, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_MRI_02_POWER_PUNCH_PLANT, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Plants Mario can search in mri_02: A_ueki01, A_ueki02, A_ueki03, A_ueki04
 * (runs evt_ueki_04 when searched), A_ueki05.
 */
static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "s_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", evt_ueki_04},
    {1, "A_ueki05", "S_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

/**
 * Per-frame control of the Puni weight floor: the plate sinks (down to -10) by the weight of the Punies standing
 * on it (sw->weight, set by setPuniWeight) and rises when they leave, 0.25 units per frame.
 * Plays the switch-on or switch-off sound when the movement starts and rebinds the S_sui_l collision.
 * Runs as the punipuni_switch script forever.
 */
USER_FUNC(moveSwitch) {
    Mri02Switch* sw = &mri02switch;
    f32 delta;

    if (isFirstCall) {
        sw->target = sw->pos = sw->base = sw->weight = 0.0f;
        event->userdataf[0] = 0.0f;
        event->userdata[1] = 0;
        event->userdata[2] = 0;
    }
    sw->target = sw->base + sw->weight;
    if (sw->target < -10.0f) {
        sw->target = -10.0f;
    }
    delta = sw->target - sw->pos;
    if (delta >= 0.25f) {
        delta = 0.25f;
    } else if (delta <= -0.25f) {
        delta = -0.25f;
    }
    sw->pos += delta;
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        BOOL changed = FALSE;

        event->userdata[2]--;
        if (event->userdata[2] < 0) {
            event->userdata[2] = 0;
            if (event->userdataf[0] != delta) {
                changed = TRUE;
            }
            event->userdataf[0] = delta;
        }
        if (changed) {
            if (event->userdata[1] != 0) {
                psndSFXOff(event->userdata[1]);
            }
            if (delta < 0.0f) {
                event->userdata[1] = psndSFXOn_3D("SFX_STG2_SWITCH_ON1", &pos);
                event->userdata[2] = 3;
            }
            if (delta > 0.0f) {
                event->userdata[1] = psndSFXOn_3D("SFX_STG2_SWITCH_OFF1", &pos);
                event->userdata[2] = 3;
            }
        }
    }
    mapObjTranslate("S_sui_l", 0.0f, sw->pos, 0.0f);
    hitBindUpdate("A_sui_l");
    return EVT_RETURN_BLOCK;
}

/** Returns the current height of the weight floor, which the Puni code reads to place the Punies on it. */
f32 getMri02Yuka(void) {
    return mri02switch.pos;
}

/** Sets the weight (a negative number, one unit per Puni) that pushes the floor down. */
USER_FUNC(setPuniWeight) {
    mri02switch.weight = evtGetFloat(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Returns 1 in args[0] when the weight floor is pressed all the way down (-10), otherwise 0. */
USER_FUNC(getSwitch) {
    EvtWord* args = event->args;
    s32 on = FALSE;

    if (mri02switch.pos <= -10.0f) {
        on = TRUE;
    }
    evtSetValue(event, args[0], on);
    return EVT_RETURN_DONE;
}

/** Script that runs the weight floor: calls moveSwitch every frame. */
EVT_BEGIN(punipuni_switch)
    CALL(moveSwitch)
    RETURN
EVT_END()

/**
 * Cutscene when enough Punies press the floor with the Puni Orb on its pedestal:
 * the pipe (dokan2) rises out of the floor with a quake, the party comments on it, and the Puni pipe is enabled.
 * Sets the story to STG2_16_THE_PUNI_ELDERS_POWERS.
 */
EVT_BEGIN(dokan_event)
    WAIT_WHILE(GF(15))
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 4500)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("S_dokan2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOKAN_QUAKE1"), LW(0), LW(1), LW(2), LW(10))
        WAIT_MS(4500)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 60, 126, 363, 60, 62, -3, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_mapobj_get_position, STRING("S_dokan2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -50, 0, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_dokan2"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("A_dokan2"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("stg2_mri_e16_00"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_16_THE_PUNI_ELDERS_POWERS)
    CALL(evt_puni_dokan_onoff, STRING("dokan2"), 0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Waits for the weight floor to be pressed while the Puni Orb is on its pedestal,
 * then runs dokan_event to raise the pipe.
 */
EVT_BEGIN(dokan_init)
    CALL(evt_hit_bind_mapobj, STRING("A_dokan2"), STRING("S_dokan2"))
    CALL(evt_mapobj_trans, 1, STRING("S_dokan2"), 0, -50, 0)
    CALL(evt_hit_bind_update, STRING("A_dokan2"))
    LOOP(0)
        CALL(getSwitch, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            IF_INT_EQUAL(GSWF_MRI_02_PUNI_ORB_PLACED, 1)
                CALL_CHILD(PTR(dokan_event))
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Display event of the Puni weight dial: hides the S_isk_01_03_C part
 * and every frame turns two dial drums to show GW(0) (0 to 10):
 * the tens drum (S_isk_01_02) and the ones drum (S_isk_01_03), 36 degrees per digit.
 */
static EVT_BEGIN(yuka_main)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_isk_01_03_C"), MAP_OBJ_FLAG_HIDDEN)
    LOOP(0)
        SET_VALUE(LW(1), GW(0))
        DIVIDE(LW(1), 10)
        SET_VALUE(LW(0), GW(0))
        MODULO(LW(0), 10)
        SET_VALUE(LW(11), LW(1))
        MULTIPLY(LW(11), 36)
        SET_VALUE(LW(10), LW(0))
        MULTIPLY(LW(10), 36)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01_02"), LW(11), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01_03"), LW(10), 0, 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Counts the Punies standing on the weight platform every frame
 * and keeps the dial value GW(0) at 10 minus the count (clamped to 0 to 10).
 * Each Puni that is alive, not lost and within the platform's square and height band reduces the number by one.
 * The negative count is passed to setPuniWeight as the weight on the switch,
 * but only once the orb is on the pedestal; the dial click sound plays whenever the value changes.
 */
static EVT_BEGIN(yuka_move)
    SET_VALUE(LW(12), GW(0))
    LOOP(0)
        SET_VALUE(LW(10), 1)
        SET_VALUE(LW(11), 0)
        LOOP(0)
            CALL(evt_puni_get_flag, LW(10), LW(0))
            CALL(evt_puni_get_flag, LW(10), LW(1))
            AND(LW(0), PUNI_FLAG_ACTIVE)
            AND(LW(1), PUNI_FLAG_NPC_CHIBI_TOGE)
            IF_INT_NOT_EQUAL(LW(0), 0)
                IF_INT_EQUAL(LW(1), 0)
                    CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
                    IF_INT_GREATER_OR_EQUAL(LW(1), -20)
                        IF_INT_LESS_OR_EQUAL(LW(1), 20)
                            CALL(evt_sub_area_check, 0, 0, -20, 50, LW(0), LW(2), LW(15))
                            IF_INT_EQUAL(LW(15), 1)
                                ADD(LW(11), 1)
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
            ADD(LW(10), 1)
            IF_INT_EQUAL(LW(10), 103)
                BREAK_LOOP
            END_IF
        END_LOOP
        SET_VALUE(GW(0), 10)
        SUBTRACT(GW(0), LW(11))
        IF_INT_GREATER_OR_EQUAL(GW(0), 10)
            SET_VALUE(GW(0), 10)
        END_IF
        IF_INT_LESS_OR_EQUAL(GW(0), 0)
            SET_VALUE(GW(0), 0)
        END_IF
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SUBTRACT_FLOAT(LW(0), LW(11))
        IF_INT_EQUAL(GSWF_MRI_02_PUNI_ORB_PLACED, 1)
            CALL(setPuniWeight, LW(0))
        ELSE
            CALL(setPuniWeight, 0)
        END_IF
        IF_INT_NOT_EQUAL(GW(0), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SWITCH_COUNTER1"), 0, 0, -305, 0)
        END_IF
        SET_VALUE(LW(12), GW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF_MRI_02_PUNI_ORB_PLACED, 1)
        CALL(evt_item_set_position, LW(10), 0, 30, -68)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_target_set_position, LW(11), 0, 0, -8)
    SET_VALUE(GF(15), 0)
    RETURN
EVT_END()

/**
 * Hit/talk script of the orb pedestal: if the orb is not placed,
 * Mario picks an item to put on it (cancel ends the script);
 * the orb appears on the pedestal with the light beam and the flag is set.
 * If it is already placed, Mario takes it back, the beam is hidden and the flag is cleared.
 * Mario is frozen during this.
 */
static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_MRI_02_PUNI_ORB_PLACED, 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        CALL(evt_item_set_position, LW(10), 0, 30, -68)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(GF(15), 1)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        SET_VALUE(GSWF_MRI_02_PUNI_ORB_PLACED, 1)
        WAIT_MS(500)
        SET_VALUE(GF(15), 0)
    ELSE
        SET_VALUE(GSWF_MRI_02_PUNI_ORB_PLACED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script of mri_02, the Puni weight-floor room:
 * sets up the Puni, the planters, the Star Piece panel in the floor,
 * the Puni Orb pedestal (punipuni) and the weight floor,
 * plays the bomb-alarm music while the time bomb is active,
 * and spawns the floor counter GW(0) (10 down to 0) with its light and the pipe event.
 */
EVT_BEGIN(mri_02_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 2, 0, -310, -1000, -1000, 1, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
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
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 155, 0, 50, STRING("S_yuka"), 125, GSWF_STARPIECE_45)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("sui_hikari"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_hit_bind_mapobj, STRING("A_sui_l"), STRING("S_sui_l"))
    SPAWN_EVENT(PTR(punipuni_switch))
    SET_VALUE(LW(10), STRING("punipuni"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai_mae"), 0, PTR(punipuni_ball00), 0)
    SET_VALUE(GW(0), 10)
    SPAWN_EVENT(PTR(yuka_main))
    SPAWN_EVENT(PTR(yuka_move))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_16_THE_PUNI_ELDERS_POWERS)
        SPAWN_EVENT(PTR(dokan_init))
        BEGIN_INLINE_EVENT
            CALL(evt_bero_exec_wait, 65536)
            CALL(evt_puni_dokan_onoff, STRING("dokan2"), 1)
        END_INLINE_EVENT
    END_IF
    CALL(evt_npc_change_fbat_mode, 1)
    SET_VALUE(LW(0), STRING("S_sui_l_02"))
    SET_RAW_VALUE(LW(1), GW(0))
    SET_VALUE(LW(2), 9)
    SPAWN_EVENT(PTR(mri_hikari_color))
    RETURN
EVT_END()

