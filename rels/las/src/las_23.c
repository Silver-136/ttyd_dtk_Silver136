// las_23 (ラストダンジョン, Palace of Shadow): a junction room.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "las.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"


static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static BeroEntry bero_entry_data[6];
static BeroEntry w_bero_mapchg[1];
static HitDamageReturn dmgRetPoint[6];
#if VERSION >= VERSION_US
EvtWord las_23_init_evt[98];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord las_23_init_evt[90];
#else
EvtWord las_23_init_evt[93];
#endif
#endif

/**
 * Door animation script (closing) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l_01 and S_doa_l_02 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l_01 and S_doa_l_02 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the east (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r_01 and S_doa_r_02 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the east (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r_01 and S_doa_r_02 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the east (door 2) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r2_01
 * and S_doa_r2_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(e2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the east (door 2) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r2_01
 * and S_doa_r2_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(e2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Exits of las_23: west door w_bero to las_21 (e_bero_1); east door e_bero_1 to las_25 (w_bero); east door
 * e_bero_2 to las_25 (w_bero); north road n_bero_1 to las_22 (s_bero); north road n_bero_2 to las_24
 * (s_bero_1).
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_21", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_25", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_25", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_22", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_24", // nextMapName
        "s_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Exit of las_23: west door w_bero to las_21 (e_bero_2). */
static BeroEntry w_bero_mapchg[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_21", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
};

/** Where Mario is put back after falling into the pits A_oti_01 to A_oti_03 (a point far below the floor). */
static HitDamageReturn dmgRetPoint[] = {
    {"A_oti_01", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_02", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_03", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_04", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_05", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Map init script for las_23, a junction room of the Palace:
 * music (first or third Palace theme by story), the doors, return points for the bottomless pits,
 * and before STG8_06_MOVING_PLANETS it hides the lower floor and pipe;
 * afterwards it switches to the moved layout with the west door leading to a different room (w_bero_mapchg).
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Map init script for las_23, a junction room of the Palace:
 * music (first or third Palace theme by story), the doors, return points for the bottomless pits,
 * and before STG8_06_MOVING_PLANETS it hides the lower floor and pipe;
 * afterwards it switches to the moved layout with the west door leading to a different room (w_bero_mapchg).
 */
EVT_BEGIN(las_23_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST3"))
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_under"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_paipu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("jav_01"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("jav_02"), 1, 0)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_be"), 1, 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(w_bero_mapchg))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(bero_case_switch_on)
        SPAWN_EVENT(hikari_evt)
    END_IF
    CALL(evt_hit_damage_return_set, PTR(dmgRetPoint))
    CALL_CHILD(evt_gazigazi_entry)
    RETURN
EVT_END()
#else
/**
 * Map init script for las_23, a junction room of the Palace:
 * music (first or third Palace theme by story), the doors, return points for the bottomless pits,
 * and before STG8_06_MOVING_PLANETS it hides the lower floor and pipe;
 * afterwards it switches to the moved layout with the west door leading to a different room (w_bero_mapchg).
 */
EVT_BEGIN(las_23_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST3"))
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_under"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_paipu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("jav_01"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("jav_02"), 1, 0)
#if VERSION >= VERSION_US
        CALL(evt_hitobj_onoff, STRING("jav_03"), 1, 0)
#endif
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_be"), 1, 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(w_bero_mapchg))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        SPAWN_EVENT(PTR(hikari_evt))
    END_IF
    CALL(evt_hit_damage_return_set, PTR(dmgRetPoint))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    RETURN
EVT_END()
#endif

