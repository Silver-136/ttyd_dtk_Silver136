#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "dou.h"
#include "driver/hitdrv.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);

static BeroEntry bero_entry_data[4];
static EvtWord yari_trap_sub[119];
static EvtWord yari_trap[11];
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
static HitDamageReturn hitDmgRetPoints[8];
static EvtWord evt_ride_ship[15];
EvtWord dou_02_init_evt[139];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_12", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_12", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(yari_trap_sub)
    IF_INT_NOT_EQUAL(GW(15), 0)
        RETURN
    END_IF
    SET_VALUE(GW(15), 1)
    LOOP(0)
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("bb"), 2048)
        CALL(evt_mapobj_get_position, STRING("s02_yari"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 1, 0, 50, 150)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("s02_yari"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a02_yari"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1000)
        CALL(evt_mapobj_get_position, STRING("s02_yari"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 4, 50, 0, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("s02_yari"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a02_yari"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("bb"), 2048)
        WAIT_MS(2500)
        IF_INT_EQUAL(GW(15), 2)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GW(15), 0)
    RETURN
EVT_END()

static EVT_BEGIN(yari_trap)
    CALL(evt_hit_bind_mapobj, STRING("a02_yari"), STRING("s02_yari"))
    SET_VALUE(GW(15), 0)
    SPAWN_EVENT(PTR(yari_trap_sub))
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_init0)
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_rev_0)
    IF_INT_NOT_EQUAL(GW(0), 2)
        RETURN
    END_IF
    SET_VALUE(GW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main0)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(1), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_init1)
    SET_VALUE(GW(2), 0)
    SET_VALUE(GW(3), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_rev_1)
    IF_INT_NOT_EQUAL(GW(2), 2)
        RETURN
    END_IF
    SET_VALUE(GW(2), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main1)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(3), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {556.0f, 50.0f, -90.0f}},
    {"mod_08", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(evt_ride_ship)
    CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_ON1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

EVT_BEGIN(dou_02_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU3"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("moku"), 1, 0)
    SPAWN_EVENT(PTR(yari_trap))
    SET_VALUE(LW(0), STRING("s02_fune_01"))
    SET_VALUE(LW(1), STRING("a02_fune_01"))
    SET_VALUE(LW(2), STRING("a02_fune_ata1"))
    SET_VALUE(LW(3), PTR(evt_ride_ship))
    CALL_CHILD(PTR(dou_puka_event_init0))
    SPAWN_EVENT(PTR(dou_puka_event_main0))
    SPAWN_EVENT(PTR(dou_puka_event_sub0))
    CALL(evt_run_case_evt, 0, 1, STRING("a02_fune_ata1"), 0, PTR(dou_puka_event_0), 0)
    CALL(evt_run_case_evt, 1, 1, STRING("a02_fune_ata1"), 0, PTR(dou_puka_event_rev_0), 0)
    SET_VALUE(LW(0), STRING("s02_fune02"))
    SET_VALUE(LW(1), STRING("a02_fune_02"))
    SET_VALUE(LW(2), STRING("a02_fune_ata2"))
    SET_VALUE(LW(3), PTR(evt_ride_ship))
    CALL_CHILD(PTR(dou_puka_event_init1))
    SPAWN_EVENT(PTR(dou_puka_event_main1))
    SPAWN_EVENT(PTR(dou_puka_event_sub1))
    CALL(evt_run_case_evt, 0, 1, STRING("a02_fune_ata2"), 0, PTR(dou_puka_event_1), 0)
    CALL(evt_run_case_evt, 1, 1, STRING("a02_fune_ata2"), 0, PTR(dou_puka_event_rev_1), 0)
    CALL(evt_item_entry, STRING("handle"), 72, 147, 42, 101, 0, GSWF(2975), 0)
    CALL(evt_item_entry, STRING("item_00"), 125, -25, 25, 100, 16, GSWF(5644), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), -300, 145, -130, 0, GSWF(5554), 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change"), STRING("Ship_Change"))
    RETURN
EVT_END()

