// dou_09 (洞窟, Pirate's Grotto): hub room linking dou_03, dou_10 and dou_13 with bobbing platforms.
// A wall that can be bombed open and a hidden brick.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "dou.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_mobj.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[5];
static EvtWord dou_puka_event_init0[8];
static EvtWord dou_puka_event_0[24];
static EvtWord dou_puka_event_rev_0[10];
static EvtWord dou_puka_event_main0[21];
static EvtWord dou_puka_event_sub0[45];
static EvtWord dou_puka_event_init1[8];
static EvtWord dou_puka_event_1[24];
static EvtWord dou_puka_event_rev_1[10];
static EvtWord dou_puka_event_main1[21];
static EvtWord dou_puka_event_sub1[45];
static EvtWord dou_puka_event_init2[8];
static EvtWord dou_puka_event_2[24];
static EvtWord dou_puka_event_rev_2[10];
static EvtWord dou_puka_event_main2[21];
static EvtWord dou_puka_event_sub2[45];
static EvtWord dou_puka_event_init3[8];
static EvtWord dou_puka_event_3[24];
static EvtWord dou_puka_event_rev_3[10];
static EvtWord dou_puka_event_main3[21];
static EvtWord dou_puka_event_sub3[45];
static HitDamageReturn hitDmgRetPoints[5];
static EvtWord evt_ride_taru[15];
static EvtWord evt_ride_ship[15];
static EvtWord bombwall_bomb[138];
static EvtWord bombwall_init[40];
EvtWord dou_09_init_evt[211];

/**
 * Exits (beros) of this room: w_bero to dou_13, e_bero_1 to dou_10, e_bero_2 to dou_10 and n_bero to dou_03.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_13", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_10", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_10", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_03", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Clears the state and offset registers of floating platform 0 (state 0 = idle, 1 = being ridden,
 * 2 = stepped off; offset = its current height).
 */
// Four floating objects use state/height pairs GW(0/1), GW(2/3), GW(4/5), GW(6/7).
static EVT_BEGIN(dou_puka_event_init0)
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 0:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks it as stepped off.
 */
static EVT_BEGIN(dou_puka_event_0)
    IF_INT_NOT_EQUAL(GW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GW(0), 1)
    SET_RAW_VALUE(LW(6), GW(1))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(0), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 0:
 * once the dip has finished (state 2) it sets the platform back to idle so it can bob again.
 */
static EVT_BEGIN(dou_puka_event_rev_0)
    IF_INT_NOT_EQUAL(GW(0), 2)
        RETURN
    END_IF
    SET_VALUE(GW(0), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 0: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main0)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(1), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 0: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, pauses it while the platform is being ridden and restarts it once Mario steps off.
 */
static EVT_BEGIN(dou_puka_event_sub0)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(1))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(0), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 1 (state 0 = idle, 1 = being ridden,
 * 2 = stepped off; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init1)
    SET_VALUE(GW(2), 0)
    SET_VALUE(GW(3), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 1:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks it as stepped off.
 */
static EVT_BEGIN(dou_puka_event_1)
    IF_INT_NOT_EQUAL(GW(2), 0)
        RETURN
    END_IF
    SET_VALUE(GW(2), 1)
    SET_RAW_VALUE(LW(6), GW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(2), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 1:
 * once the dip has finished (state 2) it sets the platform back to idle so it can bob again.
 */
static EVT_BEGIN(dou_puka_event_rev_1)
    IF_INT_NOT_EQUAL(GW(2), 2)
        RETURN
    END_IF
    SET_VALUE(GW(2), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 1: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main1)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(3), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 1: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, pauses it while the platform is being ridden and restarts it once Mario steps off.
 */
static EVT_BEGIN(dou_puka_event_sub1)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(3))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(2), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(2), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 2 (state 0 = idle, 1 = being ridden,
 * 2 = stepped off; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init2)
    SET_VALUE(GW(4), 0)
    SET_VALUE(GW(5), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 2:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks it as stepped off.
 */
static EVT_BEGIN(dou_puka_event_2)
    IF_INT_NOT_EQUAL(GW(4), 0)
        RETURN
    END_IF
    SET_VALUE(GW(4), 1)
    SET_RAW_VALUE(LW(6), GW(5))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(4), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 2:
 * once the dip has finished (state 2) it sets the platform back to idle so it can bob again.
 */
static EVT_BEGIN(dou_puka_event_rev_2)
    IF_INT_NOT_EQUAL(GW(4), 2)
        RETURN
    END_IF
    SET_VALUE(GW(4), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 2: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main2)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(5), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 2: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, pauses it while the platform is being ridden and restarts it once Mario steps off.
 */
static EVT_BEGIN(dou_puka_event_sub2)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(5))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(4), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(4), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 3 (state 0 = idle, 1 = being ridden,
 * 2 = stepped off; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init3)
    SET_VALUE(GW(6), 0)
    SET_VALUE(GW(7), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 3:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks it as stepped off.
 */
static EVT_BEGIN(dou_puka_event_3)
    IF_INT_NOT_EQUAL(GW(6), 0)
        RETURN
    END_IF
    SET_VALUE(GW(6), 1)
    SET_RAW_VALUE(LW(6), GW(7))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(6), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 3:
 * once the dip has finished (state 2) it sets the platform back to idle so it can bob again.
 */
static EVT_BEGIN(dou_puka_event_rev_3)
    IF_INT_NOT_EQUAL(GW(6), 2)
        RETURN
    END_IF
    SET_VALUE(GW(6), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 3: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main3)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(7), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 3: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, pauses it while the platform is being ridden and restarts it once Mario steps off.
 */
static EVT_BEGIN(dou_puka_event_sub3)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(7))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(6), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(6), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Hazard-water floors of this room (map hit names) and the point each puts Mario back at after he is bitten.
 */
static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Extra callback when Mario steps onto a floating barrel platform:
 * plays SFX_STG5_BARREL_ON1 at the map object named in LW(0).
 */
static EVT_BEGIN(evt_ride_taru)
    CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_ON1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

/**
 * Extra callback when Mario steps onto a floating boat platform:
 * plays SFX_STG5_SHIP_ON1 at the map object named in LW(0).
 */
static EVT_BEGIN(evt_ride_ship)
    CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_ON1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

/**
 * Case script run when Bobbery bombs the cracked wall S_ki:
 * plays SFX_OFF_WALL_BREAK1, enables the w_bero exit,
 * sets GSWF_DOU09_BOMB_WALL_BLOWN and shatters the wall as a folding paper image (OFF_d_bom_ver2)
 * before hiding it and its hit area.
 */
static EVT_BEGIN(bombwall_bomb)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("w_bero"), HIT_ATTR_NPC_ONLY)
    SET_VALUE(GSWF_DOU_09_BOMB_WALL_BLOWN, 1)
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ki"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    // Skip the capture animation only when both bounding-box dimensions are zero.
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(12), 0)
        IF_INT_EQUAL(LW(13), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_img_clear_virtual_point, STRING("capture_img"))
        CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ki1"), 1, 0)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ki1"), 1, 0)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ki"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

/**
 * Sets up the bomb wall: until GSWF_DOU09_BOMB_WALL_BLOWN is set it closes the w_bero exit
 * and lets Bobbery's bomb trigger bombwall_bomb on A_ki1; afterwards the wall is already hidden.
 */
static EVT_BEGIN(bombwall_init)
    IF_INT_EQUAL(GSWF_DOU_09_BOMB_WALL_BLOWN, 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("w_bero"), HIT_ATTR_NPC_ONLY)
        CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_ki1"), 0, PTR(bombwall_bomb), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ki1"), 1, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Init script of dou_09, the hub linking dou_03, dou_10 and dou_13:
 * music and ambience, exits, which gate model is shown (closed before STG5_22_FLOODGATE_HANDLE, open after),
 * three bobbing barrels and a boat that dip when stood on, a Shine Sprite block, a hidden brick,
 * and the wall that Bobbery can blow open.
 */
EVT_BEGIN(dou_09_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU11"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("moku"), MAP_ANIM_PLAY_LOOP, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_22_FLOODGATE_HANDLE)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("mon_b"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("mon_a"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SET_VALUE(LW(0), STRING("S_taru01"))
    SET_VALUE(LW(1), STRING("A_taru01"))
    SET_VALUE(LW(2), STRING("A_taru01"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init0))
    SPAWN_EVENT(PTR(dou_puka_event_main0))
    SPAWN_EVENT(PTR(dou_puka_event_sub0))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru01"), 0, PTR(dou_puka_event_0), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru01"), 0, PTR(dou_puka_event_rev_0), 0)
    SET_VALUE(LW(0), STRING("S_taru02"))
    SET_VALUE(LW(1), STRING("A_taru02"))
    SET_VALUE(LW(2), STRING("A_taru02"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init1))
    SPAWN_EVENT(PTR(dou_puka_event_main1))
    SPAWN_EVENT(PTR(dou_puka_event_sub1))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru02"), 0, PTR(dou_puka_event_1), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru02"), 0, PTR(dou_puka_event_rev_1), 0)
    SET_VALUE(LW(0), STRING("S_taru03"))
    SET_VALUE(LW(1), STRING("A_taru03"))
    SET_VALUE(LW(2), STRING("A_taru03"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init2))
    SPAWN_EVENT(PTR(dou_puka_event_main2))
    SPAWN_EVENT(PTR(dou_puka_event_sub2))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru03"), 0, PTR(dou_puka_event_2), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru03"), 0, PTR(dou_puka_event_rev_2), 0)
    SET_VALUE(LW(0), STRING("s09_fune01"))
    SET_VALUE(LW(1), STRING("a09_fune01"))
    SET_VALUE(LW(2), STRING("a09_fune_ata1"))
    SET_VALUE(LW(3), PTR(evt_ride_ship))
    CALL_CHILD(PTR(dou_puka_event_init3))
    SPAWN_EVENT(PTR(dou_puka_event_main3))
    SPAWN_EVENT(PTR(dou_puka_event_sub3))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("a09_fune_ata1"), 0, PTR(dou_puka_event_3), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("a09_fune_ata1"), 0, PTR(dou_puka_event_rev_3), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 500, 140, -100, 0, GSWF_SHINE_32, 0)
    CALL(evt_mobj_brick, STRING("hblk1"), 500, 50, -100, ITEM_NULL, MOBJ_BRICK_HIDDEN_TEN_COIN, 0, GSWF_DOU_09_HIDDEN_TEN_COIN_BLOCK)
    SPAWN_EVENT(PTR(bombwall_init))
    RETURN
EVT_END()

