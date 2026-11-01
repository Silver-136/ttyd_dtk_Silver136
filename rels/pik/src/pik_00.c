// pik_00 (ピカリーヒルズ, Poshley Heights): the station where the Excess Express ends, with the rich families' villas; chapter
// 7's cannon permit is given here.

#include "driver/msgdrv.h"
#include "evt/evt_story.h"
#include "evt/evt_window.h"
#include "memory.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "statuswindow.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_telop.h"
#include "evt/evt_pouch.h"
#include "mario/mario_hit.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"
#include "driver/envdrv.h"

#include "driver/imgdrv.h"
#include "driver/animdrv.h"
#include "mario/mariost.h"
#include "mario/mario.h"
#include "party.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "pmario_sound.h"
#include "driver/npcdrv.h"
#include "driver/camdrv.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "driver/mapdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_damage.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"
#include "data_tables.h"
#include "cam_road.h"
#include "cam_shift.h"
#include "driver/seqdrv.h"
#include "map/sharin.h"
#include "map/sharin.h"


USER_FUNC(img_flag);
static USER_FUNC(ramp_r);
USER_FUNC(under);
static USER_FUNC(zoom);
#if VERSION >= VERSION_US
USER_FUNC(pik_00_statuswin_force_off);
#endif
void irai_29_15_cb(s32 type, smartEntry* smart);
static void resha_tev(MapTevCallbackInfo* info);

static HitDamageReturn dmgReturnPt[2];
static SharinData sharinDt[29];
static EvtWord evt_sharin_main[87];
static EvtWord evt_sharin[19];
static ClankData clankDt[9];
static EvtWord evt_clank_main[104];
static EvtWord evt_clank[21];
static EvtWord evt_bg[81];
static EvtWord evt_hoshi[88];
static EvtWord hom_10_evt_resha_start[244];
static EvtWord hom_10_evt_resha_start2[259];
static EvtWord hom_10_evt_resha_start3[11];
static EvtWord hom_10_evt_resha_start4[11];
static EvtWord stop_1[90];
static EvtWord stop_2[90];
static EvtWord hom_10_evt_resha_stop[36];
static EvtWord evt_ramp[33];
static const char* pika_tbl[15];
static const char* pika_tbl_rsh_06[10];
static EvtWord hom_10_evt_resha_sub[102];
static EvtWord hom_10_evt_resha_right[35];
static EvtWord hom_10_evt_resha_left[28];
#if VERSION <= VERSION_DEMO_US
static EvtWord hom_10_init_evt[];
#else
static EvtWord hom_10_init_evt[140];
#endif
EvtWord resha_noru[8];
static BeroEntry bero_data[3];
static const char* name_tbl[6];
EvtWord evt_pik_yajirushi[71];
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_train[];
#else
static EvtWord evt_train[295];
#endif
EvtWord init_ekiin[8];
#if VERSION <= VERSION_DEMO_US
extern EvtWord talk_ekiin[];
#else
EvtWord talk_ekiin[146];
#endif
static EvtWord init_papa[8];
EvtWord talk_papa_taihou_kyoka[VERSION_PICK(403, 405)];
static EvtWord talk_papa[141];
static EvtWord init_mama[8];
s32 iri_29_item_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord talk_mama[];
#else
static EvtWord talk_mama[318];
#endif
static EvtWord init_konari[13];
static EvtWord talk_konari[58];
static EvtWord init_madam[8];
static s32 item_tbl_madam[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord talk_madam_ring_return[];
#else
static EvtWord talk_madam_ring_return[54];
#endif
s32 iri_23_item_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord talk_madam[];
#else
static EvtWord talk_madam[185];
#endif
static EvtWord init_sarary[12];
static EvtWord talk_sarary[8];
static NpcSetupInfo npc_data[7];
static EvtWord evt_door[15];
static EvtWord evt_door2[12];
static EvtWord evt_wall[22];
const char* ie1_mobj[2];
static EvtWord door_1_open[12];
static EvtWord door_1_close[14];
static DoorParam door_1;
static EvtWord evt_door_2[12];
static EvtWord evt_door2_2[12];
static EvtWord evt_wall_2[29];
const char* door_2_npc[2];
static EvtWord door_2_open[12];
static EvtWord door_2_close[14];
static DoorParam door_2;
EvtWord evt_rot_door_sub[63];
EvtWord evt_rot_door[9];
EvtWord evt_pik_00_arrival[86];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_seacret[];
#else
EvtWord evt_seacret[131];
#endif
#if VERSION <= VERSION_DEMO_US
static EvtWord mobj_box00[];
#else
static EvtWord mobj_box00[34];
#endif
#if VERSION <= VERSION_DEMO_US
extern EvtWord pik_00_init_evt[];
#else
EvtWord pik_00_init_evt[410];
#endif

/** Return point of the hazard floor mod_1 of the station area. */
static HitDamageReturn dmgReturnPt[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static SharinData sharinDt[] = {
    {"sha_1_1", 0, 0, FLOAT(1.0f)},
    {"sha_1_2", 0, 0, FLOAT(1.0f)},
    {"sha_1_3", 2, 0, FLOAT(1.0f)},
    {"sha_1_4", 1, 1, FLOAT(1.0f)},
    {"sha_1_5", 1, 1, FLOAT(1.0f)},
    {"sha_2_1", 0, 0, FLOAT(1.0f)},
    {"sha_2_2", 0, 0, FLOAT(1.0f)},
    {"sha_2_3", 0, 0, FLOAT(1.0f)},
    {"sha_3_1", 0, 0, FLOAT(1.0f)},
    {"sha_3_2", 0, 0, FLOAT(1.0f)},
    {"sha_3_3", 0, 0, FLOAT(1.0f)},
    {"sha_4_1", 0, 0, FLOAT(1.0f)},
    {"sha_4_2", 0, 0, FLOAT(1.0f)},
    {"sha_4_3", 0, 0, FLOAT(1.0f)},
    {"sha_5_1", 0, 0, FLOAT(1.0f)},
    {"sha_5_2", 0, 0, FLOAT(1.0f)},
    {"sha_5_3", 0, 0, FLOAT(1.0f)},
    {"sha_6_1", 0, 0, FLOAT(1.0f)},
    {"sha_6_2", 0, 0, FLOAT(1.0f)},
    {"sha_6_3", 0, 0, FLOAT(1.0f)},
    {"sha_1_1_c", 0, 0, FLOAT(-1.0f)},
    {"sha_1_2_c", 0, 0, FLOAT(-1.0f)},
    {"sha_1_3_c", 2, 0, FLOAT(-1.0f)},
    {"sha_1_4_c", 1, 1, FLOAT(-1.0f)},
    {"sha_1_5_c", 1, 1, FLOAT(-1.0f)},
    {"sha_6_1_c", 0, 0, FLOAT(1.0f)},
    {"sha_6_2_c", 0, 0, FLOAT(1.0f)},
    {"sha_6_3_c", 0, 0, FLOAT(1.0f)},
    {0},
};

/**
 * Spins one wheel map object (LW(0)) of the train:
 * LW(1) picks the wheel's radius and LW(2) picks which wheel speed, GW(0) or GW(1), drives it;
 * every frame the angle grows by speed over radius and the object is rotated.
 */
static EVT_BEGIN(evt_sharin_main)
    SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
    SWITCH(LW(1))
        CASE_EQUAL(0)
        SET_FLOAT_VALUE(LW(11), FLOAT(23.889648f))
        CASE_EQUAL(1)
        SET_FLOAT_VALUE(LW(11), FLOAT(31.139648f))
        CASE_EQUAL(2)
        SET_FLOAT_VALUE(LW(11), FLOAT(55.01953f))
    END_SWITCH
    LOOP(0)
        SWITCH(LW(2))
            CASE_EQUAL(0)
            SET_FLOAT_VALUE(LW(9), GW(0))
            CASE_EQUAL(1)
            SET_FLOAT_VALUE(LW(9), GW(1))
        END_SWITCH
        DIVIDE_FLOAT(LW(9), LW(11))
        MULTIPLY_FLOAT(LW(9), FLOAT(57.29492f))
        ADD_FLOAT(LW(10), LW(9))
        IF_INT_GREATER(LW(10), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        IF_INT_LESS(LW(10), FLOAT(0.0f))
            ADD_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        SET_FLOAT_VALUE(LW(12), LW(10))
        MULTIPLY_FLOAT(LW(12), LW(3))
        CALL(evt_mapobj_rotate, 1, LW(0), 0, 0, LW(12))
        WAIT_FRAMES(1)
        LOOP(0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_MOVING, 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/** Starts evt_sharin_main for every wheel listed in sharinDt (sha_1_1 and so on). */
static EVT_BEGIN(evt_sharin)
    SET_INT_READ_ADDR(PTR(sharinDt))
    LOOP(0)
        READ_INT4(LW(0), LW(1), LW(2), LW(3))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        SPAWN_EVENT(PTR(evt_sharin_main))
    END_LOOP
    RETURN
EVT_END()

static ClankData clankDt[] = {
    {"bou_1", 0, 0, FLOAT(20.0f), FLOAT(1.0f)},
    {"bou_2", 1, 1, FLOAT(16.5f), FLOAT(1.0f)},
    {"bou_3", 0, 0, FLOAT(20.0f), FLOAT(1.0f)},
    {"bou_4", 1, 1, FLOAT(16.5f), FLOAT(1.0f)},
    {"bou_1_c", 0, 0, FLOAT(20.0f), FLOAT(-1.0f)},
    {"bou_2_c", 1, 1, FLOAT(16.5f), FLOAT(-1.0f)},
    {"bou_3_c", 0, 0, FLOAT(20.0f), FLOAT(-1.0f)},
    {"bou_4_c", 1, 1, FLOAT(16.5f), FLOAT(-1.0f)},
    {0},
};

static EVT_BEGIN(evt_clank_main)
    SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
    SWITCH(LW(1))
        CASE_EQUAL(0)
        SET_FLOAT_VALUE(LW(11), FLOAT(23.889648f))
        CASE_EQUAL(1)
        SET_FLOAT_VALUE(LW(11), FLOAT(31.139648f))
        CASE_EQUAL(2)
        SET_FLOAT_VALUE(LW(11), FLOAT(55.01953f))
    END_SWITCH
    LOOP(0)
        SWITCH(LW(2))
            CASE_EQUAL(0)
            SET_FLOAT_VALUE(LW(9), GW(0))
            CASE_EQUAL(1)
            SET_FLOAT_VALUE(LW(9), GW(1))
        END_SWITCH
        DIVIDE_FLOAT(LW(9), LW(11))
        MULTIPLY_FLOAT(LW(9), FLOAT(57.29492f))
        ADD_FLOAT(LW(10), LW(9))
        IF_INT_GREATER(LW(10), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        IF_INT_LESS(LW(10), FLOAT(0.0f))
            ADD_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        SET_FLOAT_VALUE(LW(12), LW(10))
        MULTIPLY_FLOAT(LW(12), LW(4))
        CALL(evt_sub_get_sincos, LW(12), LW(13), LW(14))
        MULTIPLY_FLOAT(LW(13), LW(3))
        MULTIPLY_FLOAT(LW(14), LW(3))
        SUBTRACT_FLOAT(LW(14), LW(3))
        MULTIPLY_FLOAT(LW(14), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, LW(0), LW(14), LW(13), FLOAT(0.0f))
        WAIT_FRAMES(1)
        LOOP(0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_MOVING, 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/** Starts evt_clank_main for every rod listed in clankDt. */
static EVT_BEGIN(evt_clank)
    SET_INT_READ_ADDR(PTR(clankDt))
    LOOP(0)
        READ_INT4(LW(0), LW(1), LW(2), LW(3))
        READ_INT(LW(4))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        SPAWN_EVENT(PTR(evt_clank_main))
    END_LOOP
    RETURN
EVT_END()

/**
 * Scrolls the background layers (ground, mountains) sideways every frame by speeds derived from the train speed
 * GW(0), so the scenery flows past the train.
 */
static EVT_BEGIN(evt_bg)
    SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(11), FLOAT(0.0f))
    LOOP(0)
        SET_FLOAT_VALUE(LW(0), GW(0))
        ADD_FLOAT(LW(10), LW(0))
        IF_INT_GREATER(LW(10), FLOAT(800.0f))
            SUBTRACT_FLOAT(LW(10), FLOAT(1600.0f))
        END_IF
        IF_INT_LESS(LW(10), FLOAT(-800.0f))
            ADD_FLOAT(LW(10), FLOAT(1600.0f))
        END_IF
        ADD_FLOAT(LW(11), LW(0))
        IF_INT_GREATER(LW(11), FLOAT(2400.0f))
            SUBTRACT_FLOAT(LW(11), FLOAT(4800.0f))
        END_IF
        IF_INT_LESS(LW(11), FLOAT(-2400.0f))
            ADD_FLOAT(LW(11), FLOAT(4800.0f))
        END_IF
        CALL(evt_mapobj_trans, 1, STRING("jimen"), LW(10), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("yama"), LW(10), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("iwayama"), LW(11), 0, 0)
        WAIT_FRAMES(1)
        LOOP(0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_MOVING, 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_hoshi)
    SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
    LOOP(0)
        SET_FLOAT_VALUE(LW(9), GW(0))
        DIVIDE_FLOAT(LW(9), FLOAT(30.0f))
        MULTIPLY_FLOAT(LW(9), FLOAT(57.29492f))
        ADD_FLOAT(LW(10), LW(9))
        IF_INT_GREATER(LW(10), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        IF_INT_LESS(LW(10), FLOAT(0.0f))
            ADD_FLOAT(LW(10), FLOAT(360.0f))
        END_IF
        CALL(evt_mapobj_rotate, 0, STRING("hosi_right"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("hosi_left"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("hosi_center"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("hosi_right_c"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("hosi_left_c"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("hosi_center_c"), LW(10), 0, 0)
        WAIT_FRAMES(1)
        LOOP(0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_MOVING, 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/**
 * Train departure at a station (shared by hom_00, gor_04 and pik_00):
 * sets the train-moving flag, clears the "train at station" flags, rings the bell twice,
 * starts the engine sound for the station and accelerates the train over 20 seconds to a speed of -15 (GW(0) is
 * the train's speed, which also scrolls the scenery; GW(1) drives the big drive wheels sha_1_4/sha_1_5 and rods
 * bou_2/bou_4: it follows GW(0), and after 1.5 seconds runs up to 10 faster for 2 seconds; Guess: wheel spin).
 */
static EVT_BEGIN(hom_10_evt_resha_start)
    // The train starts moving: the station flags are cleared.
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_VALUE(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
    CALL(evt_mapobj_get_position, STRING("ranpu"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_BELL1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_BELL1"), LW(0), LW(1), LW(2), 0)
    // The departure sound and the first speeds depend on the station (Rogueport, Riverside or Poshley
    // Heights).
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
            CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1_2"), 0)
            ELSE
            CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1_2_2"), 0)
        END_IF
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1_3"), 0)
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            LOOP(0)
                ADD_FLOAT(LW(0), GW(0))
                CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            LOOP(0)
                ADD_FLOAT(LW(0), GW(0))
                CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -1500, 20000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        LOOP(0)
            SET_FLOAT_VALUE(GW(0), FLOAT(-15.0f))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        // The train accelerates over about 20 seconds.
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            SET_FLOAT_VALUE(GW(1), GW(0))
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER(LW(1), 1500)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -1000, 1000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), GW(0))
        ADD_FLOAT(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1000, 0, 1000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), GW(0))
        ADD_FLOAT(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
        LOOP(0)
            SET_FLOAT_VALUE(GW(1), GW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

/** Same as hom_10_evt_resha_start but for the return trip: the train accelerates in the opposite direction to +15. */
static EVT_BEGIN(hom_10_evt_resha_start2)
    // The train starts moving on the return trip: the station flags are cleared.
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_VALUE(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
    CALL(evt_mapobj_get_position, STRING("ranpu"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_BELL1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_BELL1"), LW(0), LW(1), LW(2), 0)
    // The departure sound and the first speeds depend on the station.
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1_2_3"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE1_3"), 0)
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            LOOP(0)
                ADD_FLOAT(LW(0), GW(0))
                CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            LOOP(0)
                ADD_FLOAT(LW(0), GW(0))
                CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
            LOOP(0)
                ADD_FLOAT(LW(0), GW(0))
                CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1500, 20000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        LOOP(0)
            SET_FLOAT_VALUE(GW(0), FLOAT(15.0f))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        // The train accelerates over about 20 seconds.
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            SET_FLOAT_VALUE(GW(1), GW(0))
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER(LW(1), 1500)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1000, 1000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), GW(0))
        ADD_FLOAT(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1000, 0, 1000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), GW(0))
        ADD_FLOAT(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
        LOOP(0)
            SET_FLOAT_VALUE(GW(1), GW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Sets the train to travel at its cruising speed of -15 immediately (the outbound direction),
 * without the departure acceleration; used by the running-train scenes.
 */
static EVT_BEGIN(hom_10_evt_resha_start3)
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(-15.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(-15.0f))
    RETURN
EVT_END()

/**
 * Sets the train to travel at its cruising speed of +15 immediately (the return direction),
 * without the departure acceleration.
 */
static EVT_BEGIN(hom_10_evt_resha_start4)
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(15.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(15.0f))
    RETURN
EVT_END()

static EVT_BEGIN(stop_1)
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(3000.0f))
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2_2"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2_3"), 0)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 3000, 0, 10000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    MULTIPLY_FLOAT(LW(0), FLOAT(1.0f))
    CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
    SUBTRACT_FLOAT(LW(2), LW(0))
    SET_FLOAT_VALUE(GW(0), LW(2))
    SET_FLOAT_VALUE(GW(1), LW(2))
    MULTIPLY_FLOAT(GW(0), FLOAT(-1.0f))
    MULTIPLY_FLOAT(GW(1), FLOAT(-1.0f))
    SET_FLOAT_VALUE(LW(2), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 0)
    RETURN
EVT_END()

/** Arrival braking of the return-trip train: like stop_1 with the return-trip sound variants. */
static EVT_BEGIN(stop_2)
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(-3000.0f))
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2_2_2"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_TRAIN_MOVE2_3"), 0)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -3000, 0, 10000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    MULTIPLY_FLOAT(LW(0), FLOAT(1.0f))
    CALL(evt_mapobj_trans, 1, STRING("train"), LW(0), 0, 0)
    SUBTRACT_FLOAT(LW(2), LW(0))
    SET_FLOAT_VALUE(GW(0), LW(2))
    SET_FLOAT_VALUE(GW(1), LW(2))
    MULTIPLY_FLOAT(GW(0), FLOAT(-1.0f))
    MULTIPLY_FLOAT(GW(1), FLOAT(-1.0f))
    SET_FLOAT_VALUE(LW(2), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 0)
    RETURN
EVT_END()

/**
 * Train arrival at a station: picks the slowing-down script (stop_1 or stop_2) by the current map (pik_00,
 * gor_04 and hom_00 and the trip direction), which brings the train speed to zero
 * and clears the train-moving flag.
 */
static EVT_BEGIN(hom_10_evt_resha_stop)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        SPAWN_EVENT(PTR(stop_1))
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        SPAWN_EVENT(PTR(stop_2))
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
            SPAWN_EVENT(PTR(stop_1))
            ELSE
            SPAWN_EVENT(PTR(stop_2))
        END_IF
    END_IF
    RETURN
EVT_END()

/** Keeps the lamp objects (ranpu, ranpu_c) swinging every frame by the amount from ramp_r while the train is moving. */
static EVT_BEGIN(evt_ramp)
    LOOP(0)
        CALL(ramp_r)
        CALL(evt_mapobj_rotate, 1, STRING("ranpu"), 0, 0, LW(10))
        CALL(evt_mapobj_rotate, 1, STRING("ranpu_c"), 0, 0, LW(10))
        WAIT_FRAMES(1)
        LOOP(0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_MOVING, 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

static const char* pika_tbl[] = {
    "pika_1_1",
    "pika_1_2",
    "pika_1_3",
    "pika_1_4",
    "pika_2",
    "pika_3",
    "pika_4",
    "pika_5",
    "pika_6",
    "pika_1_1_c",
    "pika_1_2_c",
    "pika_1_3_c",
    "pika_1_4_c",
    "pika_6_c",
    NULL,
};

static const char* pika_tbl_rsh_06[] = {
    "pika_1_1",
    "pika_1_2",
    "pika_1_3",
    "pika_1_4",
    "pika_2",
    "pika_3",
    "pika_4",
    "pika_5",
    "pika_6",
    NULL,
};

/**
 * Common start of the train-exterior scenes: marks the train as not moving,
 * zeroes the wheel speeds and spawns the wheel, rod,
 * background and star scripts (plus lamp and sparks on the maps that have them).
 */
static EVT_BEGIN(hom_10_evt_resha_sub)
    SET_VALUE(GSWF_HOM_TRAIN_MOVING, 0)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SPAWN_EVENT(PTR(evt_sharin))
    SPAWN_EVENT(PTR(evt_clank))
    SET_VALUE(LF(0), 0)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_10"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_11"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_12"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_06_a"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SPAWN_EVENT(PTR(evt_bg))
    END_IF
    SPAWN_EVENT(PTR(evt_hoshi))
    SPAWN_EVENT(PTR(evt_ramp))
    CALL(evt_map_set_tevcallback, 1, PTR(resha_tev))
    SET_INT_READ_ADDR(PTR(&pika_tbl))
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("rsh_06_a"))
        SET_INT_READ_ADDR(PTR(&pika_tbl_rsh_06))
    END_IF
    LOOP(0)
        READ_INT(LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(0), MAP_OBJ_TEV_CALLBACK(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(0), MAP_OBJ_FLAG_DISPLAY_LIST)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(0), MAP_OBJ_FLAG_NO_COLOR_STAGE)
    END_LOOP
    RETURN
EVT_END()

/**
 * Sets up the train scene with the outbound (right-moving) train:
 * runs the shared set-up and hides the rear cars unless the map is rsh_06_a.
 */
static EVT_BEGIN(hom_10_evt_resha_right)
    CALL_CHILD(PTR(hom_10_evt_resha_sub))
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_NOT_EQUAL(LW(0), STRING("rsh_06_a"))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_7"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_8"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("renketu_6"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("renketu_7"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

/**
 * Sets up the train scene with the return (left-moving) train:
 * runs the shared set-up and hides the cars that are not part of the return view.
 */
static EVT_BEGIN(hom_10_evt_resha_left)
    CALL_CHILD(PTR(hom_10_evt_resha_sub))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("renketu_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("renketu_5"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

static EVT_BEGIN(hom_10_init_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH2"))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL_CHILD(hom_10_evt_resha_right)
        CALL_CHILD(hom_10_evt_resha_start3)
#else
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL_CHILD(PTR(hom_10_evt_resha_right))
        CALL_CHILD(PTR(hom_10_evt_resha_start3))
#endif
        SET_VALUE(LW(0), 500)
        SET_VALUE(LW(1), -500)
#if VERSION > VERSION_DEMO_US
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("renketu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("renketu_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("renketu_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("renketu_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("renketu_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("renketu_5"), MAP_OBJ_FLAG_HIDDEN)
#endif
        ELSE
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(hom_10_evt_resha_left)
        CALL_CHILD(hom_10_evt_resha_start4)
#else
        CALL_CHILD(PTR(hom_10_evt_resha_left))
        CALL_CHILD(PTR(hom_10_evt_resha_start4))
#endif
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(1), 1000)
        CALL(evt_mapobj_trans, 1, STRING("train"), -1500, 0, 0)
    END_IF
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, LW(0), 300, 1400, LW(0), 125, 0, 0, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_cam3d_evt_set, LW(1), 300, 1400, LW(1), 125, 0, 8000, EASING_LINEAR)
        WAIT_MS(4000)
        CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("nidome"))
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Exit event of the n_bero door towards the Excess Express (rsh_01_a):
 * sets GF(11) and clears LW(0) so the train-door script does not reset anything on the way out.
 */
EVT_BEGIN(resha_noru)
    SET_VALUE(GF(11), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Doors and exits of pik_00: se_bero to pik_04; n_bero to rsh_01_a. */
static BeroEntry bero_data[] = {
    {
        "se_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_04", // nextMapName
        "nw_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        resha_noru, // exitEvent
        "rsh_01_a", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "n_1_door_close", // enterAnimeArg
        "n_1_door_open", // exitAnimeArg
    },
    {0},
};

static const char* name_tbl[] = {
    "yajirusi_1",
    "yajirusi_2",
    "yajirusi_3",
    "yajirusi_4",
    "yajirusi_5",
    "yajirusi_6",
};

/**
 * Idle animation of one of the six arrow signs (yajirusi_1 to _6) around the station;
 * the map object named in LW(9) swings between -20 and 20 degrees,
 * and now and then (10 percent of the time) spins a full turn instead, forever.
 */
EVT_BEGIN(evt_pik_yajirushi)
    LABEL(0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 20, -20, 1000)
    LABEL(10)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 0, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
    CALL(evt_sub_random, 100, LW(0))
    IF_INT_LESS(LW(0), 90)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -20, 20, 1000)
        ELSE
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -20, 380, 1000)
    END_IF
    LABEL(20)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 0, LW(9), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(20)
    END_IF
    GOTO(0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_train)
#if VERSION > VERSION_DEMO_US
    SET_VALUE(GF(11), 1)
#endif
    CALL(evt_hitobj_onoff, STRING("n_bero"), 1, 0)
#if VERSION <= VERSION_DEMO_US
SET_VALUE(GSWF_RSH_02_DRIED_SHROOM_DRAWER, 0)
#endif
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
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_party_stop, 0)
    WAIT_MS(200)
#endif
    CALL(evt_mario_mov_pos2, -215, -360, FLOAT(80.0f))
    CALL(evt_mario_mov_pos2, -230, -425, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 300, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos2, 0, -215, -360, FLOAT(80.0f))
#endif
    CALL(evt_party_move_pos2, 0, -200, -425, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    WAIT_MS(200)
    CALL(evt_party_move_pos2, 1, -215, -420, FLOAT(80.0f))
    CALL(evt_party_set_dir, 1, 270, 300)
#if VERSION > VERSION_DEMO_US
    CALL(evt_party_stop, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_set_pos, 0, -200, LW(1), -425)
    CALL(evt_party_set_pos, 1, -215, LW(1), -420)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_set_dir, 1, 270, 200)
    CALL(evt_party_stop, 0)
#endif
    CALL(evt_party_stop, 1)
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
    CALL(evt_npc_move_position, STRING("駅員"), -215, -335, 0, FLOAT(60.0f), 1)
    CALL(evt_npc_set_ry, STRING("駅員"), 270)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("駅員"), 90)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("駅員"), 270)
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING("駅員"), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("train_07"), 0, STRING("駅員"))
    WAIT_MS(500)
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_npc_set_ry, STRING("駅員"), 270)
    END_INLINE_EVENT
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(hom_10_evt_resha_start2)
#else
    SPAWN_EVENT(PTR(hom_10_evt_resha_start2))
#endif
    WAIT_MS(10000)
#if VERSION <= VERSION_DEMO_US
    IF_INT_GREATER_OR_EQUAL(GSW(GSW_STORY_PROGRESS), STG6_56_ROGUEPORT_STATION)
        SET_VALUE(GSWF_RSH_NEW_PASSENGERS_PENDING, 1)
#else
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
        SET_VALUE(GSWF_RSH_NEW_PASSENGERS_PENDING, 1)
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
SET_VALUE(GSWF_HOM_TRAIN_BOARDED_UNUSED, 1)
#else
    SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
    SET_VALUE(GSWF_HOM_TRAIN_BOARDED_UNUSED, 1)
#endif
    WAIT_FRAMES(1)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW_HOM_TRAIN_LEG, 2)
#else
    SET_VALUE(GSW_HOM_TRAIN_LEG, 2)
#endif
    CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("nidome_start"))
    RETURN
EVT_END()

/** Init for the station attendant (駅員): puts him at (-250, 60, -350) next to the train door. */
EVT_BEGIN(init_ekiin)
    CALL(evt_npc_set_position, STRING("me"), -250, 60, -350)
    RETURN
EVT_END()

/**
 * Talk script for the station attendant.
 * Before the return to Rogueport (STG6_54_WELCOME_BACK) he gives a plain greeting;
 * later he tells Mario the train is not ready until the return trip has started,
 * and otherwise offers a ride back on the Excess Express.
 * If Mario has the Train Ticket he shows it, the attendant steps aside and evt_train boards the train;
 * without it he says Mario needs one.
 */
EVT_BEGIN(talk_ekiin)
#if VERSION <= VERSION_DEMO_US
    IF_INT_LESS(GSW(GSW_STORY_PROGRESS), STG6_54_WELCOME_BACK)
#else
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
#endif
        CALL(evt_msg_print, 0, STRING("stg6_pik_00"), 0, 0)
        RETURN
        ELSE
#if VERSION <= VERSION_DEMO_US
        IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
#else
        IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
#endif
            CALL(evt_msg_print, 0, STRING("train_06"), 0, 0)
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("train_00"), 0, 0)
        CALL(evt_msg_select, 0, STRING("train_01"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("train_03"))
            CALL(evt_pouch_check_item, ITEM_TRAIN_TICKET, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 0, STRING("train_05"), 0, 0)
                ELSE
                CALL(evt_mario_set_pose, STRING("M_C_3"))
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_mario_get_dispdir, LW(3))
                IF_INT_LESS(LW(3), 180)
                    ADD(LW(0), -20)
                    ADD(LW(1), 15)
                    ELSE
                    ADD(LW(0), 20)
                    ADD(LW(1), 15)
                END_IF
                CALL(evt_item_entry, STRING("ticket"), ITEM_TRAIN_TICKET, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                WAIT_MS(1000)
                CALL(evt_item_delete, STRING("ticket"))
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print, 0, STRING("train_04"), 0, 0)
                CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
                CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                CALL(evt_npc_move_position, STRING("me"), -260, -335, 0, FLOAT(120.0f), 1)
                CALL(evt_npc_set_ry, STRING("me"), 90)
#if VERSION <= VERSION_DEMO_US
                SET_VALUE(GSWF_RSH_TRAIN_AT_POSHLEY, 0)
CALL_CHILD(evt_train)
#else
                SET_VALUE(GSWF_RSH_TRAIN_AT_POSHLEY, 0)
                SET_VALUE(GSWF_RSH_TRAIN_AT_ROGUEPORT, 0)
                CALL_CHILD(PTR(evt_train))
#endif
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("train_02"))
        END_IF
    END_IF
    RETURN
EVT_END()

/** Init for Goldbob's husband (ナリキンパパ, Mr. Goldbob): puts him at (-195, 0, 350). */
static EVT_BEGIN(init_papa)
    CALL(evt_npc_set_position, STRING("me"), -195, 0, 350)
    RETURN
EVT_END()

#if VERSION >= VERSION_US
// FAKE: the zero Vec initialisers of zoom sit here in .rodata
static const Vec zoom_zero_a = {0.0f, 0.0f, 0.0f};
static const Vec zoom_zero_b = {0.0f, 0.0f, 0.0f};

#endif
/**
 * Talk script for Goldbob (ナリキンパパ) in chapter 7:
 * the first time he explains the Fahr Outpost cannon situation, and then he asks Mario a question.
 * The right answers take Mario's coins (a bribe) in a counter animation,
 * give the Goldbob Guide and then return the coins, set GSWF_PIK00_GOLDBOB_CANNON_APPROVAL and,
 * if General White has also been woken (GSWF_BOM02_GENERAL_WHITE_WOKEN),
 * advance the story to STG7_03_GOLDBOB. The camera zooms in on the pair for the conversation.
 */
EVT_BEGIN(talk_papa_taihou_kyoka)
    IF_INT_EQUAL(GSWF_PIK_GOLDBOB_CANNON_TALK_DONE, 0)
        SET_VALUE(GSWF_PIK_GOLDBOB_CANNON_TALK_DONE, 1)
        // Goldbob greets Mario and hears about the Fahr Outpost cannon.
        CALL(evt_msg_print, 0, STRING("stg7_pik_01"), 0, STRING("ナリキンパパ"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("stg7_pik_02"), 0, STRING("ナリキンパパ"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg7_pik_03"), 0, STRING("ナリキンパパ"))
        CALL(zoom, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg7_pik_04"), 0, STRING("ナリキンパパ"))
        ELSE
        CALL(zoom, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        // On later visits he asks how much Mario would pay.
        CALL(evt_msg_print, 0, STRING("stg7_pik_05"), 0, STRING("ナリキンパパ"))
    END_IF
    // Mario names a price: nothing, 1 coin, 64 coins or everything.
    CALL(evt_msg_select, 0, STRING("stg7_pik_04_yn"))
    DEBUG_PUT_REG(LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(-1)
        CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
        CASE_EQUAL(0)
        CALL(evt_msg_print_add, 0, STRING("stg7_pik_08"))
        CASE_EQUAL(1)
        CALL(evt_pouch_get_coin, LW(0))
        IF_INT_LESS(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_07"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_09"))
        END_IF
        CASE_EQUAL(2)
        CALL(evt_pouch_get_coin, LW(0))
        IF_INT_LESS(LW(0), 64)
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_07"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_10"))
        END_IF
        CASE_EQUAL(3)
        CALL(evt_pouch_get_coin, LW(0))
        IF_INT_LESS(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_07"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_11"))
            CALL(evt_msg_select, 0, STRING("stg7_pik_11_yn"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_12"))
            CALL(evt_msg_select, 0, STRING("stg7_pik_11_yn"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_13"))
            CALL(evt_msg_select, 0, STRING("stg7_pik_11_yn"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_14"))
            CALL(evt_msg_select, 0, STRING("stg7_pik_11_yn"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                RETURN
            END_IF
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_15"))
            CALL(evt_msg_select, 0, STRING("stg7_pik_11_yn"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_06"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                RETURN
            END_IF
            // Mario agrees to hand over all his coins.
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_16"))
            CALL(evt_pouch_get_coin, LW(10))
            MULTIPLY(LW(10), -1)
            CALL(evt_pouch_add_coin, LW(10))
            CALL(evt_sub_status_gauge_force_open)
            LOOP(0)
                CALL(evt_sub_status_gauge_check_update, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_MS(500)
            CALL(evt_sub_status_gauge_force_close)
            CALL(evt_msg_print, 0, STRING("stg7_pik_17"), 0, STRING("ナリキンパパ"))
            // Goldbob gives Mario the Goldbob Guide and returns the coins.
            CALL(evt_item_entry, STRING("a"), ITEM_GOLDBOB_GUIDE, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
            CALL(evt_item_get_item, STRING("a"))
            CALL(evt_item_get_item_end_wait, STRING("a"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("stg7_pik_18"), 0, STRING("ナリキンパパ"))
            MULTIPLY(LW(10), -1)
            CALL(evt_pouch_add_coin, LW(10))
            CALL(evt_sub_status_gauge_force_open)
            LOOP(0)
                CALL(evt_sub_status_gauge_check_update, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_MS(500)
            CALL(evt_sub_status_gauge_force_close)
#if VERSION >= VERSION_US
            CALL(pik_00_statuswin_force_off)
#endif
            CALL(evt_msg_print, 0, STRING("stg7_pik_19"), 0, STRING("ナリキンパパ"))
            // His approval of the cannon is recorded.
            SET_VALUE(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 1)
            SET_VALUE(LW(0), 0)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 1)
                ADD(LW(0), 1)
            END_IF
            IF_INT_EQUAL(GSWF_BOM_GENERAL_WHITE_AGREED, 1)
                ADD(LW(0), 1)
            END_IF
            IF_INT_EQUAL(LW(0), 2)
                // If General White has also been woken, the story advances.
                SET_VALUE(GSW_STORY_PROGRESS, STG7_03_GOLDBOB)
            END_IF
        END_IF
    END_SWITCH
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    RETURN
EVT_END()

/**
 * Talk script for Mr. Goldbob.
 * During Trouble Center request 25 he gives Mario a Package for General White,
 * who he says is back in Fahr Outpost (irai_25_00, 01), then repeats where White is (irai_25_02);
 * once Mario has picked up White's trail elsewhere (GSWF_TROUBLE_WORK_3,
 * set in Rogueport and Glitzville) he says White was just here
 * and left for Fahr Outpost again (irai_25_03, setting GSWF_TROUBLE_WORK_4);
 * and once the package is delivered he pays 64 coins and finishes the request (irai_25_04).
 * During request 29 he mentions that Bub and his wife had a row (irai_29_18_1).
 * Otherwise his line depends on the story: wanting a gold train set (stg6_pik_01)
 * until the return from the train trip, stg7_pik_19_01_1 until Mayor Bob,
 * then (chapters 7 and 8) the cannon permit talk (talk_papa_taihou_kyoka) until it is approved
 * and stg7_pik_19_01 after, and stg7_pik_19_01_2 after the game.
 */
static EVT_BEGIN(talk_papa)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_25_00"), 0, 0)
            CALL(evt_item_entry, STRING("butu"), ITEM_PACKAGE, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_item_get_item, STRING("butu"))
            CALL(evt_item_get_item_end_wait, STRING("butu"))
            CALL(evt_msg_print, 0, STRING("irai_25_01"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(3), 0)
            CALL(evt_msg_print, 0, STRING("irai_25_02"), 0, 0)
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(6), 0)
            CALL(evt_msg_print, 0, STRING("irai_25_03"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(4), 1)
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("irai_25_04"), 0, 0)
        CALL(evt_sub_get_coin, 64)
        SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
        SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_DELIVERY_PLEASE), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_ME_MAKE_UP)
        CALL(evt_msg_print, 0, STRING("irai_29_18_1"), 0, 0)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("stg6_pik_01"), 0, 0)
        CASE_LESS(STG7_02_MAYOR_BOB)
        CALL(evt_msg_print, 0, STRING("stg7_pik_19_01_1"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 0)
            CALL_CHILD(PTR(talk_papa_taihou_kyoka))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_pik_19_01"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_19_01_2"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Init for Goldbob's wife (ナリキンママ): puts her at (-133, 0, 330). */
static EVT_BEGIN(init_mama)
    CALL(evt_npc_set_position, STRING("me"), -133, 0, 330)
    RETURN
EVT_END()

/** Items the player may offer for Trouble Center request 29: the Present only, ended by -1. */
s32 iri_29_item_tbl[] = {
    ITEM_PRESENT, -1,
};

/**
 * Talk script for Mrs. Goldbob.
 * During Trouble Center request 29, until she has had the Present:
 * if Mario has one she greets him (irai_29_14),
 * takes it from the item window and reads Bub's apology letter (irai_29_15);
 * then (Mario and the partner stepping aside if they stand in her way) she walks off bawling (irai_29_16),
 * comes back, asks Mario to tell Bub she is not angry (irai_29_17),
 * restarts the music and sets GSWF_TROUBLE_WORK_1.
 * Without the Present she says she has no son (irai_29_18_2); after the handover she says irai_29_18.
 * Otherwise her story lines are stg6_pik_02 (looking for a present for Bub)
 * until the return from the train trip, stg7_pik_21_1 until Mayor Bob,
 * then in chapters 7 and 8 stg7_pik_21_1 or, after the first cannon permit talk,
 * stg7_pik_20 until the cannon is approved and stg7_pik_21 after, and stg7_pik_21_2 after the game.
 */
static EVT_BEGIN(talk_mama)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_ME_MAKE_UP)
#else
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_ME_MAKE_UP)
#endif
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
            CALL(evt_pouch_check_item, ITEM_PRESENT, LW(0))
            IF_INT_GREATER(LW(0), 0)
                // Trouble Center request 29: Goldbob's wife asks what Mario needs, and he shows the Present.
                CALL(evt_msg_print, 0, STRING("irai_29_14"), 0, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(iri_29_item_tbl), LW(0))
#else
                CALL(evt_win_item_select, 0, PTR(iri_29_item_tbl), LW(0), 0)
#endif
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_pouch_remove_item, ITEM_PRESENT, LW(0))
                    // She learns the Present is from Little Bub and is moved.
                    CALL(evt_msg_print, 0, STRING("irai_29_15"), PTR(irai_29_15_cb), 0)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_npc_get_position, STRING("ナリキンママ"), LW(3), LW(4), LW(5))
                    SET_VALUE(LW(6), LW(5))
                    SUBTRACT(LW(6), 15)
                    SET_VALUE(LW(7), LW(5))
                    ADD(LW(7), 15)
                    SET_VALUE(LF(0), 0)
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(3))
                        IF_INT_GREATER_OR_EQUAL(LW(2), LW(6))
                            IF_INT_LESS_OR_EQUAL(LW(2), LW(7))
                                SET_VALUE(LF(0), 1)
                            END_IF
                        END_IF
                    END_IF
                    IF_INT_EQUAL(LF(0), 1)
                        IF_INT_LESS_OR_EQUAL(LW(2), LW(5))
                            SET_VALUE(LW(6), 0)
                            ELSE
                            SET_VALUE(LW(6), 180)
                        END_IF
                        SET_VALUE(LW(7), LW(0))
                        SET_VALUE(LW(8), LW(5))
                        CALL(evt_npc_add_dirdist, LW(7), LW(8), LW(6), 15)
                        BEGIN_INLINE_EVENT
                            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                            CALL(evt_party_move_pos, 0, LW(0), LW(8), 150)
                        END_INLINE_EVENT
                        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
                        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
                        CALL(evt_mario_mov_pos, LW(7), LW(8), 150)
                        CALL(evt_mario_adjust_dir)
                        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
                        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
                    END_IF
                    BEGIN_INLINE_EVENT
                        WAIT_MS(500)
                        CALL(evt_mario_set_dir_npc, STRING("ナリキンママ"))
                    END_INLINE_EVENT
                    BEGIN_INLINE_EVENT
                        WAIT_MS(500)
                        CALL(evt_party_set_dir_npc, 0, STRING("ナリキンママ"))
                    END_INLINE_EVENT
                    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
                    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                    CALL(evt_npc_move_position, STRING("me"), 115, 330, 0, 150, 0)
                    WAIT_MS(1000)
                    // She is delighted with the gift.
                    CALL(evt_msg_print, 0, STRING("irai_29_16"), 0, 0)
                    WAIT_MS(1500)
                    BEGIN_INLINE_EVENT
                        WAIT_MS(4000)
                        CALL(evt_mario_set_dir_npc, STRING("ナリキンママ"))
                    END_INLINE_EVENT
                    BEGIN_INLINE_EVENT
                        WAIT_MS(4000)
                        CALL(evt_party_set_dir_npc, 0, STRING("ナリキンママ"))
                    END_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING("me"), -133, 330, 0, 60, 0)
                    CALL(evt_npc_status_onoff, 0, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
                    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
                    WAIT_MS(300)
                    CALL(evt_msg_print, 0, STRING("irai_29_17"), 0, 0)
                    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
                    SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                END_IF
                RETURN
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_29_18_2"), 0, 0)
                RETURN
            END_IF
            ELSE
            // She thanks Mario and the request moves on.
            CALL(evt_msg_print, 0, STRING("irai_29_18"), 0, 0)
            RETURN
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_US
    SWITCH(GSW(GSW_STORY_PROGRESS))
#else
    SWITCH(GSW_STORY_PROGRESS)
#endif
        CASE_LESS(STG6_54_WELCOME_BACK)
        // With no request pending, her talk follows the story.
        CALL(evt_msg_print, 0, STRING("stg6_pik_02"), 0, 0)
        CASE_LESS(STG7_02_MAYOR_BOB)
        CALL(evt_msg_print, 0, STRING("stg7_pik_21_1"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
#if VERSION <= VERSION_DEMO_US
        IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 0)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_CANNON_TALK_DONE, 0)
#else
        IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 0)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_CANNON_TALK_DONE, 0)
#endif
                CALL(evt_msg_print, 0, STRING("stg7_pik_21_1"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_pik_20"), 0, 0)
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_pik_21"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_21_2"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init for Bub (コナリ), the Goldbobs' son: puts him at (-150, 0, 400), unless Trouble Center request 29 is active,
 * when he keeps his position from npc_data.
 */
static EVT_BEGIN(init_konari)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_ME_MAKE_UP)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), -150, 0, 400)
    RETURN
EVT_END()

/**
 * Talk script for Bub: stg6_pik_03 (he is getting a gold train set) until the return from the train trip,
 * stg7_pik_23_1 until Mayor Bob, then in chapters 7 and 8 stg7_pik_23_1 (before the first cannon permit talk)
 * or stg7_pik_22 (after it) until the cannon is approved and stg7_pik_23 afterwards,
 * and stg7_pik_23_2 after the game.
 */
static EVT_BEGIN(talk_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("stg6_pik_03"), 0, 0)
        CASE_LESS(STG7_02_MAYOR_BOB)
        CALL(evt_msg_print, 0, STRING("stg7_pik_23_1"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 0)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_CANNON_TALK_DONE, 0)
                CALL(evt_msg_print, 0, STRING("stg7_pik_23_1"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_pik_22"), 0, 0)
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("stg7_pik_23"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_23_2"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Init for Toodles (マダム): puts her at (352, 12, -86). */
static EVT_BEGIN(init_madam)
    CALL(evt_npc_set_position, STRING("me"), 352, 12, -86)
    RETURN
EVT_END()

/** Items the player may give the madam: the Gold Ring only, ended by -1. */
static s32 item_tbl_madam[] = {
    ITEM_GOLD_RING, -1,
};

static EVT_BEGIN(talk_madam_ring_return)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
#else
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
#endif
    CALL(evt_msg_print, 0, STRING("stg6_pik_04_06"), 0, STRING("マダム"))
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(item_tbl_madam), LW(1))
#else
    CALL(evt_win_item_select, 0, PTR(item_tbl_madam), LW(1), 0)
#endif
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_04_07"), 0, STRING("マダム"))
        CALL(evt_sub_get_coin, 30)
        CALL(evt_msg_print, 0, STRING("stg6_pik_04_08"), 0, STRING("マダム"))
        CALL(evt_pouch_remove_item, ITEM_GOLD_RING, LW(0))
#if VERSION <= VERSION_DEMO_US
        SET_VALUE(GSWF_PIK_MADAM_GOLD_RING_RETURNED, 1)
#else
        SET_VALUE(GSWF_PIK_MADAM_GOLD_RING_RETURNED, 1)
#endif
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_pik_04_10"), 0, STRING("マダム"))
    END_IF
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    RETURN
EVT_END()

/** Items the player may offer for Trouble Center request 23: the Wrestling Mag only, ended by -1. */
s32 iri_23_item_tbl[] = {
    ITEM_WRESTLING_MAG, -1,
};

static EVT_BEGIN(talk_madam)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_I_MUST_HAVE_THAT_BOOK)
#else
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_I_MUST_HAVE_THAT_BOOK)
#endif
        CALL(evt_pouch_check_item, ITEM_WRESTLING_MAG, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_23_01"), 0, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(iri_23_item_tbl), LW(0))
#else
            CALL(evt_win_item_select, 0, PTR(iri_23_item_tbl), LW(0), 0)
#endif
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(evt_pouch_remove_item, ITEM_WRESTLING_MAG, LW(0))
                CALL(evt_msg_print, 0, STRING("irai_23_02"), 0, 0)
                CALL(evt_item_entry, STRING("mg2"), ITEM_PLATINUM_CARD, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("mg2"))
                CALL(evt_item_get_item_end_wait, STRING("mg2"))
                CALL(evt_msg_print, 0, STRING("irai_23_03"), 0, 0)
#if VERSION <= VERSION_DEMO_US
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
#else
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
#endif
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_I_MUST_HAVE_THAT_BOOK), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_23_00"), 0, 0)
        END_IF
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 0)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 0)
            SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 1)
#else
    IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 0)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        IF_INT_EQUAL(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 0)
            SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_HEARD, 1)
#endif
            CALL(evt_msg_print, 0, STRING("stg6_pik_04_01"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_pik_04_04"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_pik_04_05"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_pik_04_02"))
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 1)
#else
            SET_VALUE(GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED, 1)
#endif
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_pik_04_03"))
        END_IF
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
        RETURN
    END_IF
    CALL(evt_pouch_check_item, ITEM_GOLD_RING, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(talk_madam_ring_return)
#else
        CALL_CHILD(PTR(talk_madam_ring_return))
#endif
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
    SWITCH(GSW(GSW_STORY_PROGRESS))
#else
    SWITCH(GSW_STORY_PROGRESS)
#endif
        CASE_LESS(STG6_70_THOUSAND_YEAR_DOOR)
        CALL(evt_msg_print, 0, STRING("stg6_pik_04"), 0, 0)
        CASE_DEFAULT
#if VERSION <= VERSION_DEMO_US
        IF_INT_EQUAL(GSWF_PIK_MADAM_GOLD_RING_RETURNED, 0)
#else
        IF_INT_EQUAL(GSWF_PIK_MADAM_GOLD_RING_RETURNED, 0)
#endif
            CALL(evt_msg_print, 0, STRING("stg6_pik_04"), 0, 0)
            ELSE
#if VERSION <= VERSION_DEMO_US
            IF_INT_LESS_OR_EQUAL(GSW(GSW_STORY_PROGRESS), STG8_END_AFTER_THE_BATTLE)
#else
            IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
#endif
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_01"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_01_1"), 0, 0)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init for the salaryman (サラリーマン), sightseeing while he waits for business partners (stg6_pik_05):
 * before STG6_56_ROGUEPORT_STATION it puts him at (85, -5, 35).
 */
static EVT_BEGIN(init_sarary)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
        CALL(evt_npc_set_position, STRING("me"), 85, -5, 35)
    END_IF
    RETURN
EVT_END()

/** Talk script for the salaryman: always stg6_pik_05. */
static EVT_BEGIN(talk_sarary)
    CALL(evt_msg_print, 0, STRING("stg6_pik_05"), 0, 0)
    RETURN
EVT_END()

/**
 * NPCs of the station and villas: the station attendant, Goldbob's wife and Goldbob, Little Narikin, the
 * madam and the salaryman.
 */
static NpcSetupInfo npc_data[] = {
    {
        /*"駅員"*/"\x89\x77\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ekiin, // initEvent
        NULL, // regularEvent
        talk_ekiin, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_mama, // initEvent
        NULL, // regularEvent
        talk_mama, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_papa, // initEvent
        NULL, // regularEvent
        talk_papa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_konari, // initEvent
        NULL, // regularEvent
        talk_konari, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マダム"*/"\x83\x7D\x83\x5F\x83\x80", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_madam, // initEvent
        NULL, // regularEvent
        talk_madam, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_sarary, // initEvent
        NULL, // regularEvent
        talk_sarary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * evtIn callback of door_1, the left villa's front door:
 * the door scripts run it with LW(0) going from 0 to 180 over 500 ms as the door opens
 * and back to 0 as it shuts, so door_1 swings around y by up to about 100 degrees (0.5546875 times, negated)
 * and back.
 */
static EVT_BEGIN(evt_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/** evtOut callback of door_1: run with LW(0) from 0 to 180 (with the door_down_se sound) and back, it tips door_1 down around x by up to 90 degrees. */
static EVT_BEGIN(evt_door2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * evtWall callback of door_1: run with LW(0) from 0 to 180 as the villa opens up and back as it closes,
 * it swings the front walls ie_1_N and ie_1_H open around y by up to 135 degrees, in opposite directions.
 */
static EVT_BEGIN(evt_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_1_N"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_1_H"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Map objects inside the first villa: the badge chest. */
const char* ie1_mobj[] = {
    "mobj_badge_gat",
    NULL,
};

/**
 * Door 1 open callback: dips the BGM and ambience to 50
 * and low-pass filters the ambience (the muffled indoor/outdoor change).
 */
static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Door 1 close callback: resumes the BGM and ambience from the dip and removes the ambience low-pass filter. */
static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** Door of the first villa: the outer door, its hit areas and the inside room (naka). */
static DoorParam door_1 = {
    0, // flags
    2, // type
    1, // se
    "door_1", // mapobj
    "a_door_1_N", // hitIn
    "a_door_1", // hitOut
    "soto_1", // outside
    NULL, // outsideHit
    "naka", // inside
    "a_ie_1_naka", // insideHit
    evt_door, // evtIn
    evt_door2, // evtOut
    evt_wall, // evtWall
    NULL, // npcs
    (const char**)&ie1_mobj, // mobjs
    NULL, // items
    door_1_open, // evtOpen
    door_1_close, // evtClose
};

/**
 * evtIn callback of door_2, the front door of Toodles's villa on the right:
 * the door scripts run it with LW(0) from 0 to 180 and back,
 * so door_2 swings around y by up to 90 degrees (negated) and back.
 */
static EVT_BEGIN(evt_door_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** evtOut callback of door_2: run with LW(0) from 0 to 180 and back, it tips door_2 down around x by up to 90 degrees. */
static EVT_BEGIN(evt_door2_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * evtWall callback of door_2: run with LW(0) from 0 to 180 as the villa opens up and back as it closes,
 * it swings the walls ie_2_N and ie_2_H open around y by up to 135 degrees in opposite directions
 * and tips ie_2_U around x.
 */
static EVT_BEGIN(evt_wall_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_2_N"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_2_H"), 0, LW(0), 0)
    CALL(evt_mapobj_rotate, 1, STRING("ie_2_U"), LW(0), 0, 0)
    RETURN
EVT_END()

/** NPCs inside the second villa: the madam. */
const char* door_2_npc[] = {
    /*"マダム"*/"\x83\x7D\x83\x5F\x83\x80",
    NULL,
};

/** Door 2 open callback: dips the BGM and ambience to 50 and low-pass filters the ambience, like door_1_open. */
static EVT_BEGIN(door_2_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Door 2 close callback: resumes the BGM and ambience from the dip and removes the ambience low-pass filter,
 * like door_1_close.
 */
static EVT_BEGIN(door_2_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** Door of the second villa: the outer door, its hit areas and the inside room (naka_). */
static DoorParam door_2 = {
    0, // flags
    2, // type
    1, // se
    "door_2", // mapobj
    "a_door_2_N", // hitIn
    "a_door_2", // hitOut
    "soto_2", // outside
    NULL, // outsideHit
    "naka_", // inside
    "a_ie_2_nak", // insideHit
    evt_door_2, // evtIn
    evt_door2_2, // evtOut
    evt_wall_2, // evtWall
    (const char**)&door_2_npc, // npcs
    NULL, NULL, // mobjs..items
    door_2_open, // evtOpen
    door_2_close, // evtClose
};

/**
 * Runs the spinning door of the house (map object door_3, hit area a_door_3):
 * its spin speed (GW(10)) decays by about 4 percent per frame, the angle accumulates in GW(11),
 * and a turning sound plays while the speed stays above 0.3.
 */
EVT_BEGIN(evt_rot_door_sub)
    SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(11), FLOAT(0.0f))
    CALL(evt_hit_bind_mapobj, STRING("a_door_3"), STRING("door_3"))
    SET_VALUE(LF(0), 0)
    LABEL(0)
    MULTIPLY_FLOAT(GW(10), FLOAT(0.95996094f))
    ADD_FLOAT(GW(11), GW(10))
    CALL(evt_mapobj_rotate, 1, STRING("door_3"), 0, GW(11), 0)
    CALL(evt_hit_bind_update, STRING("a_door_3"))
    WAIT_FRAMES(1)
    IF_FLOAT_GREATER(GW(10), FLOAT(0.2998047f))
        IF_INT_EQUAL(LF(0), 0)
            SET_VALUE(LF(0), 1)
            CALL(evt_snd_sfxon, STRING("SFX_DOOR_OPEN_TURN1"), LW(10))
        END_IF
        ELSE
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LF(0), 0)
            CALL(evt_snd_sfxoff, LW(10))
        END_IF
    END_IF
    GOTO(0)
    RETURN
EVT_END()

/** Called when Mario pushes the spinning door: adds 0.5 to its spin speed GW(10), capped at 10. */
EVT_BEGIN(evt_rot_door)
    IF_INT_LESS(GW(10), 10)
        ADD_FLOAT(GW(10), FLOAT(0.5f))
    END_IF
    RETURN
EVT_END()

/**
 * Cutscene when the Excess Express pulls into Poshley Heights (entered through "resha"):
 * Mario and the partner are hidden, the camera pulls back over the station with the "Poshley Heights" title
 * card, and after ten seconds the game
 * changes to a train map where Mario steps off (which one depends on story progress).  Sets GSWF_TRAIN_AT_POSHLEY.
 */
#if VERSION <= VERSION_DEMO_US
static EVT_BEGIN(erase_house)
    IF_INT_EQUAL(GF(11), 1)
        RETURN
    END_IF
    CALL(under)
    IF_INT_EQUAL(GF(10), 1)
        SET_VALUE(GF(10), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("ie_1"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()
static EVT_BEGIN(appear_house)
    IF_INT_EQUAL(GF(11), 1)
        RETURN
    END_IF
    CALL(under)
    IF_INT_EQUAL(GF(10), 0)
        SET_VALUE(GF(10), 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ie_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("naka"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()
#endif
EVT_BEGIN(evt_pik_00_arrival)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 507, 309, 1118, 3, 81, -72, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_telop_entry, 12)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 347, 236, 740, 3, 81, -72, 10000, EASING_SINE_IN_OUT)
    WAIT_MS(10000)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_49_POSHLEY_HEIGHTS)
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
        CALL(evt_bero_mapchange, STRING("rsh_02_a"), 0)
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_bero_mapchange, STRING("rsh_01_a"), STRING("w_bero"))
        CASE_DEFAULT
        CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("pik_touchaku"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Secret of the left villa: when Mario steps on the floor of ie_1_2 a paper-flip effect (offscreen capture of
 * the interior with the OFF_d_two_meku_up paper animation) reveals the interior of naka_1,
 * and when he returns to the ie_1_1 floor the flip plays backwards and the interior is hidden again.
 */
EVT_BEGIN(evt_seacret)
    CALL(evt_paper_entry, STRING("OFF_d_two_meku_up"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_two_meku_up"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, 0, 0, 0, 0)
    CALL(evt_img_alloc_capture, STRING("img"), 2, STRING("ofs"), 1, 0, 0, 0, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_img_set_position, STRING("img"), -4, -298)
    CALL(evt_img_set_virtual_point, STRING("img"), 0, 0, 5)
#else
    CALL(evt_img_clear_virtual_point, STRING("img"))
#endif
    CALL(evt_snd_sfxon, STRING("SFX_STG6_MAP_CHANGE1"), 0)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(img_flag)
#if VERSION <= VERSION_DEMO_US
CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("ie_1_sakai"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_set_offscreen, 1, STRING("e"), STRING("ofs"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("ie_1_sakai"), STRING("ofs"))
#else
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("naka_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_set_offscreen, 1, STRING("naka_1"), STRING("ofs"))
#endif
    WAIT_FRAMES(1)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("naka_1"))
    LOOP(0)
        CALL(evt_mario_get_name_hitobj_under, LW(0))
        IF_STR_EQUAL(LW(0), STRING("ie_1_1_yuka"))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxon, STRING("SFX_STG6_MAP_CHANGE2"), 0)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("naka_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("naka_1"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_paper_delete, STRING("OFF_d_two_meku_up"))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_badge_gat"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_HP_DRAIN_P, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH)
#else
    CALL(evt_item_flag_on, STRING("item00"), 258)
#endif
    CALL(evt_mobj_wait_animation_end, STRING("mobj_badge_gat"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script for pik_00: picks the train state (arriving or the return trip,
 * depending on the story and on the door Mario came in through), starts the arrow signs, music, NPCs,
 * save block, star piece panel, the HP Drain P chest and two Star Pieces, the two villa doors and the entrances,
 * the spinning platform, the secret floor trigger, pit-fall return points,
 * and sets GF(11) so the villa roof stays visible when entering through n_bero.
 */
EVT_BEGIN(pik_00_init_evt)
#if VERSION <= VERSION_DEMO_US
SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 1)
    IF_INT_LESS(GSW(GSW_STORY_PROGRESS), STG6_54_WELCOME_BACK)
        SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL_CHILD(hom_10_evt_resha_right)
#else
    SET_VALUE(GF(11), 0)
    SET_VALUE(GSWF_HOM_TRAIN_AT_POSHLEY, 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
        SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        CALL_CHILD(PTR(hom_10_evt_resha_right))
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("resha"))
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
            CALL_CHILD(hom_10_evt_resha_right)
#else
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
            CALL_CHILD(PTR(hom_10_evt_resha_right))
#endif
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("n_bero"))
#if VERSION <= VERSION_DEMO_US
            IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                CALL_CHILD(hom_10_evt_resha_right)
#else
            IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                CALL_CHILD(PTR(hom_10_evt_resha_right))
#endif
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
                ELSE
#if VERSION <= VERSION_DEMO_US
                CALL_CHILD(hom_10_evt_resha_left)
#else
                CALL_CHILD(PTR(hom_10_evt_resha_left))
#endif
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_7"), MAP_OBJ_FLAG_HIDDEN)
            END_IF
        END_IF
        IF_STR_EQUAL(LW(0), STRING("se_bero"))
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
            CALL_CHILD(hom_10_evt_resha_left)
#else
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
            CALL_CHILD(PTR(hom_10_evt_resha_left))
#endif
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_7"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        CALL(evt_sub_get_system_flag, LW(0))
        IF_BITS_SET(LW(0), GW_FLAG_SAVE_LOADED)
#if VERSION <= VERSION_DEMO_US
            IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                CALL_CHILD(hom_10_evt_resha_right)
#else
            IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                CALL_CHILD(PTR(hom_10_evt_resha_right))
#endif
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
                ELSE
#if VERSION <= VERSION_DEMO_US
                CALL_CHILD(hom_10_evt_resha_left)
#else
                CALL_CHILD(PTR(hom_10_evt_resha_left))
#endif
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_7"), MAP_OBJ_FLAG_HIDDEN)
            END_IF
        END_IF
        CALL(evt_sub_get_system_flag, LW(0))
        IF_BITS_SET(LW(0), GW_FLAG_NO_ENTRANCE)
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
            CALL_CHILD(hom_10_evt_resha_left)
#else
            SET_VALUE(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
            CALL_CHILD(PTR(hom_10_evt_resha_left))
#endif
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_5"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("train_6"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_7"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
    END_IF
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(6)
        READ_INT(LW(9))
        // Start the arrow signs and the background music.
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_pik_yajirushi)
#else
        SPAWN_EVENT(PTR(evt_pik_yajirushi))
#endif
    END_LOOP
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_PIK1"))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -100, 60, 25, 0, 0)
    // Place the spinning platform, the HP Drain P chest and the two Star Pieces.
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 8, -5, 130, STRING("jimenn"), 125, GSWF_STARPIECE_88)
    CALL(evt_mobj_itembox, STRING("mobj_badge_gat"), -225, 15, -15, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF_PIK_00_HP_DRAIN_P_CHEST)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 488, 0, 15, ITEM_TYPE_STILL, GSWF_STARPIECE_87, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, -460, 0, 100, ITEM_TYPE_STILL, GSWF_STARPIECE_89, 0)
#else
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 8, -5, 130, STRING("jimenn"), 125, GSWF_STARPIECE_88)
    CALL(evt_mobj_itembox, STRING("mobj_badge_gat"), -225, 15, -15, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF_PIK_00_HP_DRAIN_P_CHEST)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 488, 0, 15, ITEM_TYPE_STILL, GSWF_STARPIECE_87, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, -460, 0, 100, ITEM_TYPE_STILL, GSWF_STARPIECE_89, 0)
#endif
    SET_VALUE(LW(0), PTR(&door_1))
    // Set up the two villa doors.
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_door_entry)
#else
    CALL_CHILD(PTR(evt_door_entry))
#endif
    SET_VALUE(LW(0), PTR(&door_2))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_door_entry)
#else
    CALL_CHILD(PTR(evt_door_entry))
#endif
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("resha"))
        CALL(evt_npc_set_position, STRING("サラリーマン"), 0, -1000, 0)
        // Entering from the train plays the arrival scene.
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_pik_00_arrival)
        CALL_CHILD(hom_10_evt_resha_stop)
#else
        SPAWN_EVENT(PTR(evt_pik_00_arrival))
        CALL_CHILD(PTR(hom_10_evt_resha_stop))
#endif
        RETURN
        ELSE
        SET_VALUE(LW(0), PTR(bero_data))
        CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(evt_bero_info_run)
#else
        CALL_CHILD(PTR(evt_bero_info_run))
#endif
        WAIT_FRAMES(1)
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
#else
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 1)
#endif
        SET_VALUE(LW(0), STRING("n_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
    END_IF
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(0)
            CALL(evt_mapobj_rotate, 1, STRING("puro"), 0, LW(0), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("bero_"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ie_1_N"), MAP_OBJ_FLAG_CLIP)
#if VERSION <= VERSION_DEMO_US
CALL(evt_run_case_evt, CASE_UNDER, 1, STRING("a_home_yuka"), 0, PTR(erase_house), 0)
    CALL(evt_run_case_evt, CASE_NOT_UNDER, 1, STRING("a_home_yuka"), 0, PTR(appear_house), 0)
#else
    BEGIN_INLINE_EVENT
        CALL(under)
    END_INLINE_EVENT
#endif
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("n_bero"))
        SET_VALUE(GF(11), 1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("ie_1"), MAP_OBJ_FLAG_HIDDEN)
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            SET_VALUE(GF(11), 0)
        END_INLINE_EVENT
    END_IF
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("a_ie_1_2_yuka"), 0, PTR(evt_seacret), 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("se_bero"), 4)
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    // Finally the entrances, the biting Gazigazi and the mail check.
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_gazigazi_entry)
    SPAWN_EVENT(evt_sub_mail_event)
#else
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SPAWN_EVENT(PTR(evt_sub_mail_event))
#endif
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("train_2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("group1878"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

USER_FUNC(img_flag) {
    animPoseSetMaterialFlagOn(imgNameToPtr("img", gp->inBattle != 0)->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    return EVT_RETURN_DONE;
}

#if VERSION > VERSION_DEMO_US
USER_FUNC(under) {
    HitEntry* hit;
    BOOL flag;

    marioGetPtr();
    partyGetPtr(0);
    flag = FALSE;
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    hit = marioSearchUnder();
    if (evtGetValue(event, GF(11))) {
        flag = TRUE;
    }
    if ((hit != NULL && strcmp(hitGetName(hit), "a_home_yuka") == 0) || flag) {
        if (event->userdata[0] == 0 || flag) {
            mapGrpFlagOn("ie_1", MAP_OBJ_FLAG_HIDDEN);
            event->userdata[0] = 1;
        }
    } else if (event->userdata[0] != 0) {
        mapGrpFlagOff("ie_1", MAP_OBJ_FLAG_HIDDEN);
        event->userdata[0] = 0;
    }
    return EVT_RETURN_BLOCK;
}
#endif

/**
 * Message callback for Trouble Center request 29 (irai_29_15):
 * when the second page of the text appears it starts the BGM_EVT_NOKO_SAIKAI1 (reunion) music.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(under) {
    PartyEntry* party = partyGetPtr(0);

    if (party != NULL && party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        marioGetPtr()->hitobjUnder = marioSearchUnder();
    }
    return EVT_RETURN_DONE;
}
#endif
void irai_29_15_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    if (type == 3 && info->curPage == 1) {
        psndBGMOn(PSND_FADEOUT, "BGM_EVT_NOKO_SAIKAI1");
    }
}

#if VERSION >= VERSION_US

/** Script helper: force-closes the status window (statusWinForceOff). */
USER_FUNC(pik_00_statuswin_force_off) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script helper for the Goldbob scene's camera:
 * computes a camera position 70 percent of the way from the current camera target to the camera,
 * shifted in x to the middle between Mario and Goldbob, so the shot frames both.
 * Writes the 3D camera position to args[0] to args[2] and the target to args[3] to args[5];
 * used with evt_cam3d_evt_set.
 */
static USER_FUNC(zoom) {
    EvtWord* args = event->args;
    // ナリキンパパ
    NpcEntry* npc = npcNameToPtr("\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70");
    MarioWork* mario = marioGetPtr();
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec v;
#if VERSION >= VERSION_US
    CameraParams out;
    f32 bank = 0.0f;

    camShiftMain(cam, mario, &bank);
    camRoadMain(&out, cam->blendPos.x, cam->blendPos.y, cam->blendPos.z, cam->blendAt.x, cam->blendAt.y, cam->blendAt.z, bank);
    {
        Vec pos;
        Vec at;

        at = zoom_zero_b;
        pos = zoom_zero_a;
        at.x = out.at.x;
        at.y = out.at.y;
        at.z = out.at.z;
        pos.x = out.pos.x;
        pos.y = out.pos.y;
        pos.z = out.pos.z;
        PSVECSubtract(&pos, &at, &v);
    }
#else
    PSVECSubtract(&cam->cameraPos, &cam->cameraAt, &v);
#endif
    PSVECScale(&v, &v, 0.7f);
    PSVECAdd(&cam->cameraAt, &v, &v);
    evtSetFloat(event, args[0], (npc->position.x + mario->position.x) / 2);
    evtSetFloat(event, args[1], v.y);
    evtSetFloat(event, args[2], v.z);
    evtSetFloat(event, args[3], (npc->position.x + mario->position.x) / 2);
    evtSetFloat(event, args[4], cam->cameraAt.y);
    evtSetFloat(event, args[5], cam->cameraAt.z);
    return EVT_RETURN_DONE;
}


/** Map TEV callback for the train model: forwards the map object's TEV info to envAddTev with index 7. */
static void resha_tev(MapTevCallbackInfo* info) {
    envAddTev(7, info);
}


/**
 * Script helper: writes into LW(10) a sine wobble (1.8 times |GW(0)| times sin(frame count / 15));
 * evt_ramp rotates the station's ramp (ranpu) by it, so the boarding ramp sways while the train stands there.
 */
static USER_FUNC(ramp_r) {
    f32 amp = __fabsf(evtGetFloat(event, GW(0)));
    f32 wave = sin(gp->retraceCount / 15.0f);

    evtSetFloat(event, LW(10), 1.8f * amp * wave);
    return EVT_RETURN_DONE;
}

