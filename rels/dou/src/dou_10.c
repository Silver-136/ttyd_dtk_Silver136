// dou_10 (洞窟, Pirate's Grotto): room linking dou_09, dou_06 and dou_11 with a wave-dodging section and a spear trap (story STG5_22_FLOODGATE_HANDLE to STG5_29_WAVE_DODGING).
// Bobbing platforms.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "driver/mapdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "mot/mot_ship.h"
#include "dou.h"
#include "evt/evt_mobj.h"
#include "system.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"

static inline f32 addf(f32 a, f32 b) { return a + b; }
typedef struct EntryMgrEntry {
    s32 used;
    s32 id;
    f32 reg[3];
} EntryMgrEntry;
typedef struct EntryMgr {
    s32 count;
    EntryMgrEntry* entries;
} EntryMgr;

USER_FUNC(dou_namiatari_check);
USER_FUNC(evt_sub_entrymgr_check);
USER_FUNC(evt_sub_entrymgr_delete);
USER_FUNC(evt_sub_entrymgr_entry);
USER_FUNC(evt_sub_entrymgr_getreg);
USER_FUNC(evt_sub_entrymgr_init);
USER_FUNC(evt_sub_entrymgr_setreg);
USER_FUNC(nami_running_func);
USER_FUNC(nami_running_init_func);
USER_FUNC(namiyoke_fail_stop);
USER_FUNC(namiyoke_se_check);
USER_FUNC(namiyoke_set_mapobj);

static BeroEntry bero_entry_data[5];
const char* namiyoke_mapobj[15];
EvtWord nami_running[72];
EvtWord namiyoke_fail_kaiten[34];
EvtWord namiyoke_fail_shizumu[49];
EvtWord namiyoke_fail_recover[190];
EvtWord namiyoke_fail[142];
EvtWord namiyoke_atari_check[21];
EvtWord namiyoke_main[135];
EvtWord namiyoke_init[97];
EvtWord namiyoke_se[100];
EvtWord namiyoke_check[34];
static EvtWord yari_trap_sub[107];
static EvtWord yari_trap[11];
static EvtWord dou_puka_event_main0[60];
static HitDamageReturn hitDmgRetPoints[4];
EvtWord dou_10_init_evt[64];

/**
 * Exits (beros) of this room: w_bero_1 to dou_09, w_bero_2 to dou_09, n_bero to dou_06 and e_bero to dou_11.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_09", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_09", // nextMapName
        "e_bero_2", // nextBeroName
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
        "dou_06", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        "dou_11", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Map objects of the 15 waves that are moved along their lanes by the wave script. */
const char* namiyoke_mapobj[] = {
    "S_nami1",
    "S_nami2",
    "S_nami3",
    "S_nami4",
    "S_nami5",
    "S_nami6",
    "S_nami7",
    "S_nami8",
    "S_nami9",
    "S_nami10",
    "S_nami11",
    "S_nami12",
    "S_nami13",
    "S_nami14",
    "S_nami15",
};

/**
 * One travelling wave of the wave-dodging section:
 * takes a free slot in the wave table (LW(9) = table, LW(10) = wave id),
 * moves the wave object namiyoke_mapobj[id] along the lane from the start values in LW(11) to LW(13)
 * until it reaches the end, and sets GW(0) to 1 if it touches Mario on the way.
 * Frees the slot and parks the wave object under the map at the end.
 */
EVT_BEGIN(nami_running)
    CALL(evt_sub_entrymgr_entry, LW(9), LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(nami_running_init_func)
    LOOP(0)
        CALL(nami_running_func, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(15), 1)
            BREAK_LOOP
        END_IF
        CALL(namiyoke_set_mapobj, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_sub_entrymgr_setreg, LW(9), LW(10), LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(dou_namiatari_check, LW(0), LW(1), LW(2), LW(1))
        IF_INT_EQUAL(LW(1), 1)
            SET_VALUE(GW(0), 1)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(namiyoke_set_mapobj, LW(10), 0, -1000, 0)
    CALL(evt_sub_entrymgr_delete, LW(9), LW(10), LW(0))
    RETURN
EVT_END()

/** Failure animation, part 1: spins Mario's facing three full turns (1080 degrees) over one second. */
EVT_BEGIN(namiyoke_fail_kaiten)
    CALL(evt_mario_get_dispdir, LW(0))
    SET_VALUE(LW(1), LW(0))
    ADD(LW(1), 1080)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(0), LW(1), 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_dispdir, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Failure animation, part 2: takes control away from Mario
 * and sinks him 100 units below the water over four seconds.
 */
EVT_BEGIN(namiyoke_fail_shizumu)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MINIGAME_SHIP_SINK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, -100, 4000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_get_pos, 0, LW(7), LW(8), LW(9))
        CALL(evt_mario_set_pos, LW(7), LW(0), LW(9))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Failure animation, part 3: Mario bobs back up at (-660, 0, 100), the start of the wave room, from 100 below,
 * with three smaller bounces (-10, -5, -1) as he settles.
 */
EVT_BEGIN(namiyoke_fail_recover)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MINIGAME_SHIP_FLOAT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -100, 0, 800)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -10, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -5, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -5, 0, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -1, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1, 0, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mario_set_pos, -660, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Mario was hit by a wave: stops the boat, aims the camera, spins and sinks Mario,
 * puts him back at the start (-660,
 * 100) with a ripple effect and the camera pulled to the start, and gives control back.
 */
EVT_BEGIN(namiyoke_fail)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(namiyoke_fail_stop)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT_GET_ID(PTR(namiyoke_fail_kaiten), LW(10))
    SPAWN_EVENT_GET_ID(PTR(namiyoke_fail_shizumu), LW(11))
    WAIT_MS(2000)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    CALL(evt_mario_set_pos, -660, -100, 100)
    CALL(evt_cam3d_evt_set, -630, 152, 578, -630, 57, 37, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(0), 5, LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(0), 5, LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(namiyoke_fail_recover))
    CALL(evt_cam_road_reset)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/** Watches GW(0) forever; whenever a wave sets it to 1 (touching Mario), runs the failure sequence and clears it. */
EVT_BEGIN(namiyoke_atari_check)
    SET_VALUE(GW(0), 0)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 1)
            DEBUG_PUT_MSG(STRING("マリオと波の接触"))
            CALL_CHILD(PTR(namiyoke_fail))
            SET_VALUE(GW(0), 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Wave spawner of the wave-dodging section: first a run of 9 waves staggered along the room (the near lanes are
 * only used for later ones), then forever one new wave every 1.5 to 2 seconds at the far end (X = 1040) in one
 * of four lanes (Z = -125, -50, 25, 100), up to 15 at once.
 */
EVT_BEGIN(namiyoke_main)
    SET_VALUE(LW(10), 0)
    LOOP(0)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 15)
            SET_VALUE(LW(10), 0)
        END_IF
        SET_VALUE(LW(12), 0)
        CALL(evt_sub_random, 3, LW(14))
        SET_VALUE(LW(11), 162)
        MULTIPLY(LW(11), LW(10))
        ADD(LW(11), -500)
        SWITCH(LW(14))
            CASE_EQUAL(0)
            SET_VALUE(LW(13), -125)
            IF_INT_GREATER_OR_EQUAL(LW(10), 6)
                SPAWN_EVENT(PTR(nami_running))
            END_IF
            CASE_EQUAL(1)
            SET_VALUE(LW(13), -50)
            IF_INT_GREATER_OR_EQUAL(LW(10), 6)
                SPAWN_EVENT(PTR(nami_running))
            END_IF
            CASE_EQUAL(2)
            SET_VALUE(LW(13), 25)
            SPAWN_EVENT(PTR(nami_running))
            CASE_EQUAL(3)
            SET_VALUE(LW(13), 100)
            SPAWN_EVENT(PTR(nami_running))
        END_SWITCH
        IF_INT_EQUAL(LW(10), 9)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 15)
            SET_VALUE(LW(10), 0)
        END_IF
        SET_VALUE(LW(11), 1040)
        SET_VALUE(LW(12), -50)
        CALL(evt_sub_random, 3, LW(14))
        SWITCH(LW(14))
            CASE_EQUAL(0)
            SET_VALUE(LW(13), -125)
            CASE_EQUAL(1)
            SET_VALUE(LW(13), -50)
            CASE_EQUAL(2)
            SET_VALUE(LW(13), 25)
            CASE_EQUAL(3)
            SET_VALUE(LW(13), 100)
        END_SWITCH
        SPAWN_EVENT(PTR(nami_running))
        CALL(evt_sub_random, 500, LW(0))
        ADD(LW(0), 1500)
        WAIT_MS(LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Parks all 15 wave objects below the map (y = -1000) and switches off their hit areas
 * so the waves start out of sight.
 */
EVT_BEGIN(namiyoke_init)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(namiyoke_set_mapobj, LW(0), 0, -1000, 0)
        ADD(LW(0), 1)
        IF_INT_EQUAL(LW(0), 15)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("A_nami1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami2"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami3"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami4"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami5"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami6"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami7"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami8"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami9"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami10"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami11"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami12"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami13"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami14"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_nami15"), 1, 0)
    RETURN
EVT_END()

/**
 * Sound loop for the waves (namiyoke_check spawns it): every frame it finds the active wave nearest Mario among
 * the 15 wave slots (namiyoke_se_check) and starts SFX_STG5_MINIGAME_WAVE1 there or moves it there;
 * it stops the sound when no wave is active. The first start passes the wave's x as its z position too.
 */
EVT_BEGIN(namiyoke_se)
    SET_VALUE(LW(12), 0)
    LOOP(0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), 100000000)
        LOOP(0)
            CALL(evt_sub_entrymgr_check, LW(9), LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SET_VALUE(LW(11), 1)
                CALL(evt_sub_entrymgr_getreg, LW(9), LW(10), LW(0), LW(1), LW(2))
                CALL(namiyoke_se_check, LW(0), LW(1), LW(2))
            END_IF
            ADD(LW(10), 1)
            IF_INT_EQUAL(LW(10), 15)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(LW(11), 1)
            CALL(evt_sub_entrymgr_getreg, LW(9), LW(6), LW(3), LW(4), LW(5))
            IF_INT_EQUAL(LW(12), 0)
                // Initial sound position repeats X as Z; later updates use the stored Z.
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MINIGAME_WAVE1"), LW(3), LW(4), LW(3), LW(13))
                SET_VALUE(LW(12), 1)
            ELSE
                CALL(evt_snd_sfx_pos, LW(13), LW(3), LW(4), LW(5))
            END_IF
        ELSE
            IF_INT_EQUAL(LW(12), 1)
                CALL(evt_snd_sfxoff, LW(13))
                SET_VALUE(LW(12), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Starts the wave-dodging section when dou_10 is entered:
 * moves the story from STG5_22_FLOODGATE_HANDLE to STG5_29_WAVE_DODGING, creates the wave table,
 * and starts the wave setup, the wave spawner and the wave sound.
 * The collision watcher is started only when entering from w_bero_1 or e_bero.
 */
EVT_BEGIN(namiyoke_check)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_22_FLOODGATE_HANDLE)
        SET_VALUE(GSW_STORY_PROGRESS, STG5_29_WAVE_DODGING)
    END_IF
    CALL(evt_sub_entrymgr_init, LW(9), 15)
    SPAWN_EVENT(PTR(namiyoke_init))
    SPAWN_EVENT(PTR(namiyoke_main))
    SPAWN_EVENT(PTR(namiyoke_se))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("w_bero_1"))
        SPAWN_EVENT(PTR(namiyoke_atari_check))
    END_IF
    IF_STR_EQUAL(LW(0), STRING("e_bero"))
        SPAWN_EVENT(PTR(namiyoke_atari_check))
    END_IF
    RETURN
EVT_END()

/**
 * Spear trap loop (only one at a time, guarded by GW(15)):
 * the spear S_yari shoots up and retracts on a timer,
 * repeating until GW(15) is set to 2 (same pattern as yari_trap_sub in dou_02).
 */
static EVT_BEGIN(yari_trap_sub)
    IF_INT_NOT_EQUAL(GW(15), 0)
        RETURN
    END_IF
    SET_VALUE(GW(15), 1)
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING("S_yari"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 150, 150)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yari"), 0, 0, LW(0))
            CALL(evt_hit_bind_update, STRING("A_yari"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1000)
        CALL(evt_mapobj_get_position, STRING("S_yari"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 150, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yari"), 0, 0, LW(0))
            CALL(evt_hit_bind_update, STRING("A_yari"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1000)
        IF_INT_EQUAL(GW(15), 2)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GW(15), 0)
    RETURN
EVT_END()

/**
 * Starts the spear trap: binds the spear's hit area A_yari to its map object S_yari,
 * clears GW(15) and spawns yari_trap_sub.
 */
static EVT_BEGIN(yari_trap)
    CALL(evt_hit_bind_mapobj, STRING("A_yari"), STRING("S_yari"))
    SET_VALUE(GW(15), 0)
    SPAWN_EVENT(PTR(yari_trap_sub))
    RETURN
EVT_END()

/**
 * Bobbing loop for floating object 0: binds the hit area in LW(1) (if any) to the map object in LW(0),
 * waits a random 0.2 to 0.6 seconds, then every 130 ms steps through the 16 heights of the bobbing table
 * (evt_dou_normal_puka), moves the object to that height and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main0)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/**
 * Hazard-water floors of this room (map hit names) and the point each puts Mario back at after he is bitten.
 */
static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {-700.0f, 50.0f, -90.0f}},
    {"mod_03", {-130.0f, 50.0f, -90.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Init script of dou_10, the wave-dodging room:
 * music and ambience, exits, waves and boat animation, the spear trap, gazigazi hazards,
 * the wave-dodging scripts, a bobbing boat and a Shine Sprite block.
 */
EVT_BEGIN(dou_10_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU10"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("moku"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("fune_02"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(PTR(yari_trap))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SPAWN_EVENT(PTR(namiyoke_check))
    SET_VALUE(LW(0), STRING("s10_fune_01"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main0), LW(10))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 130, 50, -135, 0, GSWF_SHINE_33, 0)
    RETURN
EVT_END()

USER_FUNC(namiyoke_se_check) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    Vec pos;
    s32 dist;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    dist = PSVECSquareDistance(&pos, &mario->position);
    if (dist < event->lwData[7]) {
        event->lwData[6] = event->lwData[10];
        event->lwData[7] = dist;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: stops Mario's boat immediately (marioShipForceStop). */
USER_FUNC(namiyoke_fail_stop) {
    marioShipForceStop();
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0..2] is a wave's position; sets args[3] to 1 if Mario is touching it.
 * The hit box is 30 wide and deep plus margins of 20 in X and 10 in Z,
 * shrinking while the wave sinks (y between -5 and -45); a wave lower than -45 never hits.
 */
USER_FUNC(dou_namiatari_check) {
    // Waves at Y <= -45 are inactive; between -45 and -5 their X/Z extents grow linearly.
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 w = 30.0f;
    f32 d = w;
    f32 mx = mario->position.x;
    f32 mz = mario->position.z;
    BOOL hit;

    if (y <= -45.0f) {
        evtSetValue(event, args[3], 0);
        return EVT_RETURN_DONE;
    }
    if (y <= -5.0f) {
        f32 r = (50.0f + y) / 45.0f;

        w *= r;
        d *= r;
    }
    hit = TRUE;
    // FAKE: the addf inline fixes the fadds operand order
    if (__fabsf(x - mx) > __fabsf(addf(w, 20.0f))) {
        hit = FALSE;
    }
    if (__fabsf(z - mz) > __fabsf(addf(d, 10.0f))) {
        hit = FALSE;
    }
    evtSetValue(event, args[3], hit);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: moves one wave forward by 1.35 per frame (X) and bobs it with a sine;
 * the wave rises out of the water at the start and sinks again after 200 (or -340 in the far lanes).
 * Writes the position to args[0..2] and sets LW(15) to 1 once the wave has finished its run (X < 0, or < -540 in
 * the far lanes).
 */
USER_FUNC(nami_running_func) {
    f32* lwf = event->lwDataF;
    EvtWord* args = event->args;
    f32 s;

    lwf[6] -= 1.3499999f;
    if (lwf[6] > 840.0f) {
        lwf[7] += 0.35f;
        if (lwf[7] > 0.0f) {
            lwf[7] = 0.0f;
        }
    }
    switch (event->lwData[14]) {
        case 0:
        case 1:
            if (lwf[6] < 200.0f) {
                lwf[7] -= 0.4f;
            }
            break;
        case 2:
        case 3:
            if (lwf[6] < -340.0f) {
                lwf[7] -= 0.4f;
            }
            break;
    }
    event->lwData[15] = 0;
    switch (event->lwData[14]) {
        case 0:
        case 1:
            if (lwf[6] < 0.0f) {
                event->lwData[15] = 1;
            }
            break;
        case 2:
        case 3:
            if (lwf[6] < -540.0f) {
                event->lwData[15] = 1;
            }
            break;
    }
    s = sin(6.2831855f * (2.0f * gp->retraceCount) / 360.0f);
    evtSetFloat(event, args[0], lwf[6]);
    evtSetFloat(event, args[1], lwf[7] + (5.0f * s - 5.0f));
    evtSetFloat(event, args[2], lwf[8]);
    return EVT_RETURN_DONE;
}

/** Script helper: copies the start position of a wave (LW(11) to LW(13)) into its working floats. */
USER_FUNC(nami_running_init_func) {
    f32* lwf = event->lwDataF;

    lwf[6] = event->lwData[11];
    lwf[7] = event->lwData[12];
    lwf[8] = event->lwData[13];
    return EVT_RETURN_DONE;
}

USER_FUNC(namiyoke_set_mapobj) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    mapObjTranslate(namiyoke_mapobj[idx], x, y, z);
    return EVT_RETURN_DONE;
}

/** Script helper (wave table): args[0] is the table; copies the three floats stored with id args[1] into args[2..4]. */
USER_FUNC(evt_sub_entrymgr_getreg) {
    EvtWord* args = event->args;
    EntryMgr* mgr = (EntryMgr*)evtGetValue(event, args[0]);
    EntryMgrEntry* e = mgr->entries;
    s32 id = evtGetValue(event, args[1]);
    s32 i;

    for (i = 0; i < mgr->count; i++, e++) {
        if (e->id == id) {
            break;
        }
    }
    evtSetFloat(event, args[2], e->reg[0]);
    evtSetFloat(event, args[3], e->reg[1]);
    evtSetFloat(event, args[4], e->reg[2]);
    return EVT_RETURN_DONE;
}

/** Script helper (wave table): args[0] is the table; stores the floats args[2..4] with id args[1]. */
USER_FUNC(evt_sub_entrymgr_setreg) {
    EvtWord* args = event->args;
    EntryMgr* mgr = (EntryMgr*)evtGetValue(event, args[0]);
    EntryMgrEntry* e = mgr->entries;
    s32 id = evtGetValue(event, args[1]);
    s32 i;

    for (i = 0; i < mgr->count; i++, e++) {
        if (e->id == id) {
            break;
        }
    }
    e->reg[0] = evtGetFloat(event, args[2]);
    e->reg[1] = evtGetFloat(event, args[3]);
    e->reg[2] = evtGetFloat(event, args[4]);
    return EVT_RETURN_DONE;
}

/** Script helper (wave table): sets args[2] to 1 if id args[1] is in use in the table args[0], else 0. */
USER_FUNC(evt_sub_entrymgr_check) {
    // BUG: an unused entry does not advance e, so later entries are not checked.
    EvtWord* args = event->args;
    EntryMgr* mgr = (EntryMgr*)evtGetValue(event, args[0]);
    EntryMgrEntry* e = mgr->entries;
    s32 id = evtGetValue(event, args[1]);
    BOOL ok = FALSE;
    s32 i;

    for (i = 0; i < mgr->count; i++) {
        if (e->used) {
            if (e->id == id) {
                ok = TRUE;
                break;
            }
            e++;
        }
    }
    evtSetValue(event, args[2], ok);
    return EVT_RETURN_DONE;
}

/** Script helper (wave table): frees the slot with id args[1] in the table args[0]; args[2] is 1 if it was found. */
USER_FUNC(evt_sub_entrymgr_delete) {
    EvtWord* args = event->args;
    EntryMgr* mgr = (EntryMgr*)evtGetValue(event, args[0]);
    EntryMgrEntry* e = mgr->entries;
    s32 id = evtGetValue(event, args[1]);
    BOOL ok = FALSE;
    s32 i;

    for (i = 0; i < mgr->count; i++, e++) {
        if (e->id == id) {
            e->used = FALSE;
            e->id = 0;
            ok = TRUE;
            break;
        }
    }
    evtSetValue(event, args[2], ok);
    return EVT_RETURN_DONE;
}

/**
 * Script helper (wave table): claims the first free slot in the table args[0] for id args[1];
 * args[2] is 1 on success and 0 if the table is full.
 */
USER_FUNC(evt_sub_entrymgr_entry) {
    EvtWord* args = event->args;
    EntryMgr* mgr = (EntryMgr*)evtGetValue(event, args[0]);
    EntryMgrEntry* e = mgr->entries;
    s32 id = evtGetValue(event, args[1]);
    BOOL ok = FALSE;
    s32 i;

    for (i = 0; i < mgr->count; i++, e++) {
        if (!e->used) {
            e->used = TRUE;
            e->id = id;
            ok = TRUE;
            break;
        }
    }
    evtSetValue(event, args[2], ok);
    return EVT_RETURN_DONE;
}

/**
 * Script helper (wave table): allocates a table of args[1] slots from the map heap, zeroed,
 * and returns its address in args[0].
 */
USER_FUNC(evt_sub_entrymgr_init) {
    EvtWord* args = event->args;
    EntryMgr* mgr = _mapAlloc(sizeof(EntryMgr));

    evtSetValue(event, args[0], (EvtWord)mgr);
    mgr->count = evtGetValue(event, args[1]);
    mgr->entries = _mapAlloc(mgr->count * sizeof(EntryMgrEntry));
    memset(mgr->entries, 0, mgr->count * sizeof(EntryMgrEntry));
    return EVT_RETURN_DONE;
}
