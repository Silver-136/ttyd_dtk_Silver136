// las_07 (ラストダンジョン, Palace of Shadow): a connecting hall.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"


static EvtWord w1_door_close[22];
static EvtWord w1_door_open[22];
static EvtWord w2_door_close[22];
static EvtWord w2_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static EvtWord door_matigai_evt[16];
static BeroEntry bero_entry_data[5];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord las_07_init_evt[];
#else
EvtWord las_07_init_evt[57];
#endif

/**
 * Door animation script (closing) for the west (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l1_01
 * and S_doa_l1_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(w1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l1_01
 * and S_doa_l1_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(w1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the west (door 2) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l2_01
 * and S_doa_l2_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(w2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west (door 2) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_l2_01
 * and S_doa_l2_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(w2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the east (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r1_01
 * and S_doa_r_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the east (door 1) door,
 * used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_doa_r1_01
 * and S_doa_r_02 by it, the second one the opposite way.
 */
static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
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
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_matigai_evt)
    IF_INT_EQUAL(GSWF_LAS_WRONG_DOOR_SFX_PENDING, 1)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_DOOR_NG_OPEN1"), 0)
        SET_VALUE(GSWF_LAS_WRONG_DOOR_SFX_PENDING, 0)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Exits of las_07: west door w_bero_1 to las_06 (e_bero); east door e_bero_1 to las_08 (w_bero_1); west door
 * w_bero_2 to las_08 (e_bero_2); east door e_bero_2 to las_08 (w_bero_2).
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_06", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_door_close, // enterAnimeArg
        w1_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_door_close, // enterAnimeArg
        w2_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Map init script for las_07: music and ambience,
 * the four doors (one to las_06 and three to las_08, each of which can be the wrong door of the Palace's door puzzle),
 * two ? blocks with a Coin and a Jammin' Jelly, and a badge block with P-Up, D-Down, P.
 */
EVT_BEGIN(las_07_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
#if VERSION >= VERSION_JP
    CALL(evt_snd_set_rev_mode, 2)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_cam_road_reset)
    END_INLINE_EVENT
    CALL(evt_mobj_badgeblk, STRING("mobj00"), -100, 60, -300, ITEM_COIN, 0, GSWF_LAS_07_COIN_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj01"), 100, 60, -300, ITEM_JAMMIN_JELLY, 0, GSWF_LAS_07_JAMMIN_JELLY_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), 0, 60, -300, ITEM_P_UP_D_DOWN_P, 0, GSWF_LAS_07_P_UP_D_DOWN_P_BADGE_BLOCK, 1)
    RETURN
EVT_END()

