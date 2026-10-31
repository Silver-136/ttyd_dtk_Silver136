// mri_13 (巨木の中, The Great Tree): the room with the moving floor set off by the counter switches
// (yuka_event), a Puni-ball pad, a save block and a recovery block.

#include "mri_puni.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/hitdrv.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"


USER_FUNC(evt_puni_force_pose);
USER_FUNC(yuka_fall_check);

BeroEntry bero_dai3_kanbu[1];
static BeroEntry bero_entry_data[3];
static EvtWord punipuni_mario_halt[33];
static EvtWord punipuni_ball00_init[54];
static EvtWord punipuni_ball00[68];
static EvtWord yuka_init[48];
EvtWord yuka_slot[113];
EvtWord yuka_shake[11];
EvtWord yuka_shake2[68];
static EvtWord yuka_check[133];
EvtWord yuka_event[421];
static EvtWord evt_f1_ueki_01[20];
static KusaAccess ueki_access_data[7];
EvtWord mri_13_init_evt[205];

/** Exit of mri_13: east road e_bero to mri_14. */
BeroEntry bero_dai3_kanbu[] = {
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
        "mri_14", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

/** Exits of mri_13: east road e_bero to mri_14 (w_bero); pipe dokan to mri_16 (dokan7). */
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
        "mri_14", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_W, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_16", // nextMapName
        "dokan7", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Holds Mario for up to 4 seconds (GF(11)) while the Puni switch is being set, ending early when GF(10) is cleared. */
static EVT_BEGIN(punipuni_mario_halt)
    SET_VALUE(GF(11), 1)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        SET_VALUE(GF(11), 0)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(10), 0)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF_MRI_13_PUNI_ORB_PLACED, 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
            CALL(evt_item_set_position, LW(10), -55, 30, 0)
            ELSE
            CALL(evt_item_set_position, LW(10), -55, -279, 0)
        END_IF
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
        CALL(evt_target_set_position, LW(11), -55, 0, 50)
        ELSE
        CALL(evt_target_set_position, LW(11), -55, -309, 50)
    END_IF
    RETURN
EVT_END()

/**
 * Hit/talk script of the orb pedestal in the water room:
 * if empty, Mario picks an item to put on it (cancel ends the script)
 * and the orb is placed at the current pedestal height with the beam of light; if full, Mario takes it back.
 * GF(0) is set while the orb is set down. Mario is frozen during this.
 */
static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_MRI_13_PUNI_ORB_PLACED, 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF_MRI_13_PUNI_ORB_PLACED, 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
            CALL(evt_item_set_position, LW(10), -55, 30, 0)
            ELSE
            CALL(evt_item_set_position, LW(10), -55, -279, 0)
        END_IF
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(GF(0), 1)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        SET_VALUE(GF(0), 0)
        ELSE
        SET_VALUE(GSWF_MRI_13_PUNI_ORB_PLACED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init script of the lowered platform: binds the platform collision (A_dai, A_dai_02) to its model (S_dai),
 * and puts the platform, the water switch (S_sui_l) and its light 309 units down;
 * the switch collision and the shadow model are off.
 */
static EVT_BEGIN(yuka_init)
    CALL(evt_hit_bind_mapobj, STRING("A_dai"), STRING("S_dai"))
    CALL(evt_hit_bind_mapobj, STRING("A_dai_02"), STRING("S_dai"))
    CALL(evt_mapobj_trans, 1, STRING("S_dai"), 0, FLOAT(-309.0f), 0)
    CALL(evt_hit_bind_update, STRING("A_dai"))
    CALL(evt_hit_bind_update, STRING("A_dai_02"))
    CALL(evt_mapobj_trans, 1, STRING("S_sui_l"), 0, -309, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_sui_hikari"), 0, -309, 0)
    CALL(evt_hitobj_onoff, STRING("A_sui_l1"), 1, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_kage"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Runs the three-digit display on the wall of the elevator room:
 * shows the count GW(0) (Punies still needed) with the ones, tens and hundreds drums turned to match,
 * hides the hundreds drum while the count is under 10 and plays a click when the number changes.
 */
EVT_BEGIN(yuka_slot)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_isk_02_c"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(13), GW(0))
    LOOP(0)
        SET_VALUE(LW(2), GW(0))
        DIVIDE(LW(2), 100)
        SET_VALUE(LW(1), GW(0))
        MODULO(LW(1), 100)
        DIVIDE(LW(1), 10)
        SET_VALUE(LW(0), GW(0))
        MODULO(LW(0), 10)
        SET_VALUE(LW(12), LW(2))
        MULTIPLY(LW(12), 36)
        SET_VALUE(LW(11), LW(1))
        MULTIPLY(LW(11), 36)
        SET_VALUE(LW(10), LW(0))
        MULTIPLY(LW(10), 36)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_02"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_03"), LW(11), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_04"), LW(12), 0, 0)
        IF_INT_GREATER_OR_EQUAL(GW(0), 10)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        IF_INT_NOT_EQUAL(GW(0), LW(13))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SWITCH_COUNTER1"), -150, 0, -20, 0)
        END_IF
        SET_VALUE(LW(13), GW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(yuka_fall_check) {
    s32 i;

    for (i = 1; i < 103; i++) {
        PuniData* puni = &puniGetPunidat()[i];
        if (puni->flags & PUNI_FLAG_ACTIVE) {
            Vec dir = {0.0f, -1.0f, 0.0f};
            Vec fall = {0.0f, -3.0f, 0.0f};
            Vec pos;
            Vec hit;
            Vec n;
            f32 dist;

            dist = 3.0f;
            pos = puni->pos;
            if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &n.x, &n.y, &n.z, pos.x, pos.y, pos.z, dir.x, dir.y,
                               dir.z)) {
                puni->pos = hit;
            } else {
                PSVECAdd(&puni->pos, &fall, &puni->pos);
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Resets the display of the Puni whose slot index is args[0]:
 * clears its offsets, spin and velocity so it stands still.
 */
USER_FUNC(evt_puni_force_pose) {
    s32 offset = evtGetValue(event, event->args[0]) * sizeof(PuniData);
    PuniData* puni = (PuniData*)((u8*)puniGetPunidat() + offset);

    puni->dispOfs.y = puni->spin = puni->rotOffsetY = 0.0f;
    puni->vel = (Vec){0.0f, 0.0f, 0.0f};
    return EVT_RETURN_DONE;
}

EVT_BEGIN(yuka_shake)
    LOOP(5)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 1000)
    END_LOOP
    RETURN
EVT_END()

/**
 * Fading camera shake for the last part of the elevator descent:
 * a series of shakes that die down over about 10 seconds, ending in a final stronger one.
 */
EVT_BEGIN(yuka_shake2)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0087890625f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0078125f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0068359375f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.005859375f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0048828125f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.00390625f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0029296875f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.001953125f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0009765625f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0146484375f), 1000)
    RETURN
EVT_END()

/**
 * Counts every frame the Punies standing on the platform (alive, not lost, not in mode 13,
 * within the platform area and height band; the band moves down by 309 after the water has been drained)
 * and drives the platform with that weight.
 */
static EVT_BEGIN(yuka_check)
    LOOP(0)
        SET_VALUE(LW(10), 1)
        SET_VALUE(LW(11), 0)
        LOOP(0)
            CALL(evt_puni_get_flag, LW(10), LW(0))
            CALL(evt_puni_get_flag, LW(10), LW(1))
            AND(LW(0), PUNI_FLAG_ACTIVE)
            AND(LW(1), PUNI_FLAG_NPC_CHIBI_TOGE)
            CALL(evt_puni_get_mode, LW(10), LW(2))
            IF_INT_NOT_EQUAL(LW(0), 0)
                IF_INT_EQUAL(LW(1), 0)
                    IF_INT_NOT_EQUAL(LW(2), 13)
                        CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
                        IF_INT_LESS(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
                            IF_INT_GREATER_OR_EQUAL(LW(1), -20)
                                IF_INT_LESS_OR_EQUAL(LW(1), 20)
                                    CALL(evt_sub_area_check, 0, -50, 60, 50, LW(0), LW(2), LW(15))
                                    IF_INT_EQUAL(LW(15), 1)
                                        ADD(LW(11), 1)
                                    END_IF
                                END_IF
                            END_IF
                            ELSE
                            IF_INT_GREATER_OR_EQUAL(LW(1), -329)
                                IF_INT_LESS_OR_EQUAL(LW(1), -289)
                                    CALL(evt_sub_area_check, 0, -50, 60, 50, LW(0), LW(2), LW(15))
                                    IF_INT_EQUAL(LW(15), 1)
                                        ADD(LW(11), 1)
                                    END_IF
                                END_IF
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
        SET_VALUE(GW(0), 101)
        SUBTRACT(GW(0), LW(11))
        IF_INT_GREATER_OR_EQUAL(GW(0), 101)
            SET_VALUE(GW(0), 101)
        END_IF
        IF_INT_LESS_OR_EQUAL(GW(0), 0)
            SET_VALUE(GW(0), 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(yuka_event)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_area_check, 1, -50, 60, 60, LW(0), LW(2), LW(14))
        // Wait until the Puni Orb is on the pedestal, every Puni is on the platform and Mario and his partner
        // stand on it too.
        // Wait until the Puni Orb is on the pedestal, every Puni is on the platform and Mario and his partner
        // stand on it too.
        // Wait until the Puni Orb is on the pedestal, every Puni is on the platform and Mario and his partner
        // stand on it too.
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_area_check, 1, -50, 60, 60, LW(0), LW(2), LW(15))
        IF_INT_EQUAL(GSWF_MRI_13_PUNI_ORB_PLACED, 1)
            IF_INT_EQUAL(GW(0), 0)
                IF_INT_EQUAL(GF(0), 0)
                    IF_INT_EQUAL(LW(14), 1)
                        IF_INT_EQUAL(LW(15), 1)
                            BREAK_LOOP
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // Mario stops and the Punies freeze; the floor starts to quake.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_winddisable_onoff, 1)
    CALL(evt_target_flag_onoff, 0, 1, 1)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), PUNI_MODE_INERT)
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_force_pose, LW(10))
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_FRAMES(2)
    CALL(evt_mario_mov_pos2, -50, 60, FLOAT(120.0f))
    CALL(evt_mapobj_scale, 1, STRING("S_sui_kage"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    DELETE_EVENT(GW(1))
    DELETE_EVENT(GW(2))
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE1"), GW(4))
    SPAWN_EVENT_GET_ID(PTR(yuka_shake), LW(10))
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_INERT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    // Punio comments on the shaking floor while the camera drops to the shaft.
    CALL(evt_msg_print, 0, STRING("stg2_mri_e21_00"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_INERT)
    CALL(evt_cam3d_evt_set, 330, -240, 404, -50, -60, 50, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -225, -80, 355, -50, -270, 50, 9000, EASING_SINE_IN_OUT)
    CALL(evt_snd_sfxoff, GW(4))
    DELETE_EVENT(LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE1"), GW(3))
    SPAWN_EVENT(PTR(yuka_shake2))
    CALL(evt_hit_bind_mapobj, STRING("A_sui_l1"), STRING("S_sui_l"))
    CALL(evt_hit_bind_mapobj, STRING("A_dai"), STRING("S_dai"))
    CALL(evt_hit_bind_mapobj, STRING("A_dai_02"), STRING("S_dai"))
    // The platform, the Puni Orb and the light move down 309 units over ten seconds.
    CALL(evt_snd_sfxon, STRING("SFX_STG2_FLOOR_MOVE1"), GW(2))
    CALL(evt_item_get_position, STRING("punipuni"), LW(3), LW(4), LW(5))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, FLOAT(0.5f), FLOAT(1.0f), 10000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_scale, 1, STRING("S_sui_kage"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_kage"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, -30950, 10000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(LW(6), LW(4))
        ADD_FLOAT(LW(6), LW(0))
        CALL(evt_item_set_position, STRING("punipuni"), LW(3), LW(6), LW(5))
        CALL(evt_mapobj_trans, 1, STRING("S_sui_l"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("A_sui_l1"))
        CALL(evt_mapobj_trans, 1, STRING("S_dai"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("A_dai"))
        CALL(evt_hit_bind_update, STRING("A_dai_02"))
        CALL(evt_mapobj_trans, 1, STRING("S_sui_hikari"), 0, LW(0), 0)
        CALL(yuka_fall_check)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, GW(2))
    CALL(evt_snd_sfxoff, GW(3))
    // The platform stops; the Punies are released and the story advances.
    CALL(evt_snd_sfxon, STRING("SFX_STG2_FLOOR_STOP1"), 0)
    WAIT_MS(1000)
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_INERT)
    CALL(evt_npc_get_position, STRING("ガイドsp"), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e21_00_01"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), PUNI_MODE_JOIN)
        CALL(evt_puni_flag_onoff, LW(10), 0, PUNI_FLAG_STRAY)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("A_sui_l1"), 1, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SPAWN_EVENT(PTR(yuka_check))
    CALL(evt_target_flag_onoff, 0, 0, 1)
    CALL(evt_target_set_position, 0, -55, -309, 50)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
    CALL(evt_puni_set_hamari, -180, -310, 60, 2, -2, -306, -312)
    CALL(evt_puni_winddisable_onoff, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Planter hit script 1: the first time it is hit it launches a Mushroom out of the planter (remembered with a
 * flag).
 */
static EVT_BEGIN(evt_f1_ueki_01)
    CALL(evt_item_delete_check, STRING("item_ueki_01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_01"), ITEM_MUSHROOM, -190, -309, 115, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_MRI_13_MUSHROOM_PLANT, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Plants Mario can search in mri_13: A_f2_ueki02, A_f2_ueki03, A_f2_ueki04, A_f1_ueki01
 * (runs evt_f1_ueki_01 when searched), A_f1_ueki02, A_f1_ueki03.
 */
static KusaAccess ueki_access_data[] = {
    {1, "A_f2_ueki02", "S_f2_ueki02", NULL},
    {1, "A_f2_ueki03", "S_f2_ueki03", NULL},
    {1, "A_f2_ueki04", "S_f2_ueki04", NULL},
    {1, "A_f1_ueki01", "S_f1_ueki01", evt_f1_ueki_01},
    {1, "A_f1_ueki02", "S_f1_ueki02", NULL},
    {1, "A_f1_ueki03", "S_f1_ueki03", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_13_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 13, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, -250, 0, 60, 2, -2, -306, -312)
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
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -150, -249, 0, PTR(mri_puni_save), 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 8, -200, -249, 0, 0, 0)
    SET_VALUE(GW(0), 101)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_21_06_WATER_SWITCH)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak"), 1, 0)
        SPAWN_EVENT_GET_ID(PTR(yuka_slot), GW(1))
        SPAWN_EVENT_GET_ID(PTR(yuka_check), GW(2))
        SPAWN_EVENT(PTR(yuka_event))
        CALL(evt_puni_set_hamari, -250, 0, 60, 2, -2, -306, -312)
        ELSE
        SPAWN_EVENT(PTR(yuka_init))
        SPAWN_EVENT(PTR(yuka_check))
        CALL(evt_puni_set_hamari, -180, -310, 60, 2, -2, -306, -312)
    END_IF
    SET_VALUE(LW(0), STRING("S_sui_l"))
    SET_RAW_VALUE(LW(1), GW(0))
    SET_VALUE(LW(2), 100)
    SPAWN_EVENT(PTR(mri_hikari_color))
    SET_VALUE(LW(10), STRING("punipuni"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai"), 0, PTR(punipuni_ball00), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_22_LORD_CRUMP_APPEARS)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_dai3_kanbu))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()

