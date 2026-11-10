// eki_01 (駅, Riverside Station): central hub: paper-fold (origami) switch, stairs, a key lock and six paper panels; doors to eki_00, eki_02 and eki_03.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cloud.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "mario/mario_pouch.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_mobj.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

static EvtWord e_close[12];
static EvtWord e_open[12];
static EvtWord w_close[12];
static EvtWord w_open[12];
static EvtWord n_close[12];
static EvtWord n_open[12];
static BeroEntry bero_data[4];
static EvtWord evt_origami[129];
static s32 key_tbl[2];

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_chk[21];
#else
static EvtWord evt_chk[23];
#endif

static EvtWord evt_lock[10];
static EvtWord evt_kami_1[5];
static EvtWord evt_kami_2[5];
static EvtWord evt_kami_3[5];
static EvtWord evt_kami_4[5];
static EvtWord evt_kami_5[5];
static EvtWord evt_kami_6[5];
EvtWord eki_01_init_evt[223];

/**
 * Door animation callback, closing for the e_bero exit to eki_03:
 * rotates map object "door_3" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_3"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the e_bero exit to eki_03:
 * rotates map object "door_3" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_3"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the w_dero exit to eki_00:
 * rotates map object "door_1" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_dero exit to eki_00:
 * rotates map object "door_1" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the n_bero exit to eki_02:
 * rotates map object "door_2" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(n_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the n_bero exit to eki_02:
 * rotates map object "door_2" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(n_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exits (beros) of this room: e_bero to eki_03, w_dero to eki_00 and n_bero to eki_02. */
static BeroEntry bero_data[] = {
    {
        "e_bero", // hitName
        1, // type
        3, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_03", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {
        "w_dero", // hitName
        1, // type
        3, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_00", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {
        "n_bero", // hitName
        1, // type
        3, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_02", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_close, // enterAnimeArg
        n_open, // exitAnimeArg
    },
    {0},
};

/**
 * Cutscene when Mario unfolds the paper staircase (Riverside Station):
 * the camera moves to the stairs, the "dann" map object is shown and "n_origami" is hidden
 * and the anm_kaidan animation plays while step sounds (SFX_STG6_PERA_KAI_DOWN1/2) play,
 * and the story moves to STG6_30_ORIGAMI_STAIRS.
 */
static EVT_BEGIN(evt_origami)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 516, 126, 408, 171, 90, -55, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("dann"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1666)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(266)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(366)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(333)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(333)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(333)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_DOWN2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("dann"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("n_origami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_oritatami"), 1, 1)
    CALL(evt_map_playanim, STRING("anm_kaidan"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("anm_kaidan"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_30_ORIGAMI_STAIRS)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Items offered in the item menu at the station lock: only the Station Key. */
static s32 key_tbl[] = {
    ITEM_STATION_KEY_0x19, -1,
};

/**
 * Check for the station door lock ("me"): if Mario carries the station key (ITEM_STATION_KEY_0x19),
 * opens the item select window with key_tbl; cancelling it cancels the lock execution so the door stays locked.
 */
static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, ITEM_STATION_KEY_0x19, LW(0))
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
 * Runs when the station door lock is opened with the key:
 * switches on the bero case (the door can be used) and sets the story to STG6_32_UNLOCKING_THE_DOOR.
 */
static EVT_BEGIN(evt_lock)
    SET_VALUE(LW(0), 0)
    CALL_CHILD(bero_case_switch_on)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_32_UNLOCKING_THE_DOOR)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_1)
    SET_VALUE(GSWF_EKI_01_PAPER_1_BLOWN_AWAY, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_2)
    SET_VALUE(GSWF_EKI_01_PAPER_2_BLOWN_AWAY, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_3)
    SET_VALUE(GSWF_EKI_01_PAPER_3_BLOWN_AWAY, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_4)
    SET_VALUE(GSWF_EKI_01_PAPER_4_BLOWN_AWAY, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_5)
    SET_VALUE(GSWF_EKI_01_PAPER_5_BLOWN_AWAY, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kami_6)
    SET_VALUE(GSWF_EKI_01_PAPER_6_BLOWN_AWAY, 1)
    RETURN
EVT_END()

/**
 * Map init for eki_01: sets up the doors (to eki_00, eki_02 and eki_03),
 * the blue switch that unfolds the origami stairs until that story step is passed,
 * the locked door that needs the Station Key until it has been unlocked,
 * and the six paper panels that Flurrie can blow away (those already blown are hidden with their hit areas off).
 */
EVT_BEGIN(eki_01_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI2_2"))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_30_ORIGAMI_STAIRS)
        CALL(evt_mobj_switch_blue, 0, STRING("origami_sw"), 90, 10, -60, PTR(evt_origami), GSWF_EKI_01_ORIGAMI_SWITCH_PRESSED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("dann"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("n_origami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_oritatami"), 1, 0)
    ELSE
        CALL(evt_map_playanim, STRING("anm_kaidan"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("dann"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("n_origami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_oritatami"), 1, 1)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_32_UNLOCKING_THE_DOOR)
        CALL(evt_mobj_lock, STRING("lock"), 25, 394, 10, 0, -80, PTR(evt_chk), PTR(evt_lock), GSWF_EKI_01_DOOR_UNLOCKED)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(bero_case_switch_off)
    END_IF
    CALL(evt_cloud_init)
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_1_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_1"), STRING("a_kami_1"), PTR(evt_kami_1))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_1"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_2_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_2"), STRING("a_kami_2"), PTR(evt_kami_2))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_2"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_3_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_3"), STRING("a_kami_3"), PTR(evt_kami_3))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_3"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_4_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_4"), STRING("a_kami_4"), PTR(evt_kami_4))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_4"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_5_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_5"), STRING("a_kami_5"), PTR(evt_kami_5))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_5"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF_EKI_01_PAPER_6_BLOWN_AWAY, 0)
        CALL(evt_cloud_ent, 0, STRING("kami_6"), STRING("a_kami_6"), PTR(evt_kami_6))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kami_6"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_kami_6"), 1, 0)
    END_IF
    RETURN
EVT_END()
