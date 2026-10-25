#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"
#include "mario/mario_pouch.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_dou_normal_puka);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_pauseanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mobj_kururing_floor);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_win_item_select);

EvtWord dou_03_custom[10];
static BeroEntry bero_entry_data[8];
EvtWord watergate_handle_init[34];
static s32 itemsel_table[2];
EvtWord watergate_lever[20];
EvtWord watergate_geer_small[20];
EvtWord watergate_geer_large[20];
EvtWord nagare_taru[38];
EvtWord nagare_taru_se[45];
EvtWord nagare_bin[33];
EvtWord watergate_handle[251];
static EvtWord dou_puka_event_main0[60];
static EvtWord dou_puka_event_main1[60];
static EvtWord dou_puka_event_main2[60];
static HitDamageReturn hitDmgRetPoints[18];
EvtWord dou_03_init_evt[130];

EVT_BEGIN(dou_03_custom)
    CALL(evt_hitobj_onoff, STRING("on_off"), 1, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

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
        "dou_12", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)dou_03_custom, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_12", // nextMapName
        "e_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_3", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_04", // nextMapName
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
        (s32*)dou_03_custom, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_05", // nextMapName
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
        "dou_05", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_3", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_05", // nextMapName
        "w_bero_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_09", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(watergate_handle_init)
    IF_INT_LESS(GSW(0), 248)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_reba"), 1)
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("s_bero"), 128)
        CALL(evt_hitobj_onoff, STRING("A_reba"), 1, 0)
        ELSE
        CALL(evt_map_playanim, STRING("mon_up"), 0, 0)
    END_IF
    RETURN
EVT_END()

static s32 itemsel_table[] = {
    ITEM_GATE_HANDLE, -1,
};

EVT_BEGIN(watergate_lever)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, STRING("S_reba"), LW(0), 0, 0)
        ADD(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(watergate_geer_small)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai1"), LW(0), 0, 0)
        ADD(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(watergate_geer_large)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai2"), LW(0), 0, 0)
        ADD(LW(0), -4)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(nagare_taru)
    CALL(evt_sub_intpl_msec_init, 1, 0, 100, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_taru"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru"), 1)
    CALL(evt_hitobj_onoff, STRING("A_taru"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(nagare_taru_se)
    CALL(evt_mapobj_get_position, STRING("S_taru"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_MOVE1"), LW(0), LW(1), LW(2), LW(9))
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("S_taru"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_MOVE1"), LW(0), LW(1), LW(2), LW(9))
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("S_taru"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_MOVE1"), LW(0), LW(1), LW(2), LW(9))
    RETURN
EVT_END()

EVT_BEGIN(nagare_bin)
    CALL(evt_sub_intpl_msec_init, 1, 0, 100, 1200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_bin"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_bin"), 1)
    RETURN
EVT_END()

EVT_BEGIN(watergate_handle)
    IF_INT_LESS(GSW(0), 248)
        CALL(evt_pouch_check_item, 72, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_msg_toge, 1, 0, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_03_before"), 0, 0)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        CALL(evt_win_item_select, 0, PTR(itemsel_table), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            RETURN
        END_IF
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_reba"), 1)
        CALL(evt_hitobj_onoff, STRING("A_reba"), 1, 1)
        CALL(evt_pouch_remove_item, 72, LW(0))
        CALL(evt_snd_sfxon, STRING("SFX_STG5_FLOODGATE_SHAKE1"), LW(10))
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 500)
        WAIT_MS(500)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_cam3d_evt_set, 98, 151, 563, 98, 56, 21, 500, 11)
        WAIT_MS(1000)
        CALL(evt_mapobj_get_position, STRING("s03_mon"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_FLOODGATE_UP1"), LW(0), LW(1), LW(2), LW(9))
        CALL(evt_map_playanim, STRING("mae"), 1, 0)
        CALL(evt_map_playanim, STRING("mon"), 0, 0)
        CALL(evt_map_checkanim, STRING("mon"), LW(0), LW(1))
        SPAWN_EVENT_GET_ID(PTR(watergate_lever), LW(10))
        SPAWN_EVENT_GET_ID(PTR(watergate_geer_small), LW(11))
        SPAWN_EVENT_GET_ID(PTR(watergate_geer_large), LW(12))
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), LW(1))
        END_INLINE_EVENT
        WAIT_MS(LW(1))
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 250)
        END_INLINE_EVENT
        CALL(evt_map_pauseanim, 0, STRING("mae"))
        DELETE_EVENT(LW(10))
        DELETE_EVENT(LW(11))
        DELETE_EVENT(LW(12))
        CALL(evt_snd_sfxoff, LW(9))
        CALL(evt_mapobj_get_position, STRING("s03_mon"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_FLOODGATE_UP2"), LW(0), LW(1), LW(2), LW(9))
        DELETE_EVENT(GW(10))
        SPAWN_EVENT(PTR(nagare_taru))
        SPAWN_EVENT(PTR(nagare_taru_se))
        DELETE_EVENT(GW(11))
        SPAWN_EVENT(PTR(nagare_bin))
        WAIT_MS(2000)
        CALL(evt_cam3d_evt_off, 300, 11)
        WAIT_MS(500)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
        CALL(evt_msg_print, 0, STRING("stg5_dou_02"), 0, 0)
        WAIT_MS(500)
        CALL(evt_party_stop, 0)
        CALL(evt_msg_print_party, STRING("stg5_dou_03"))
        CALL(evt_party_run, 0)
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("s_bero"), 128)
        SET_VALUE(GSW(0), 248)
        CALL(evt_mario_key_onoff, 1)
        ELSE
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg5_dou_03_after"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_main1)
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

static EVT_BEGIN(dou_puka_event_main2)
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

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {230.0f, 50.0f, 50.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {"mod_09", {0.0f, -1000.0f, 0.0f}},
    {"mod_10", {0.0f, -1000.0f, 0.0f}},
    {"mod_11", {0.0f, -1000.0f, 0.0f}},
    {"mod_12", {0.0f, -1000.0f, 0.0f}},
    {"mod_13", {0.0f, -1000.0f, 0.0f}},
    {"mod_14", {0.0f, -1000.0f, 0.0f}},
    {"mod_15", {0.0f, -1000.0f, 0.0f}},
    {"mod_16", {0.0f, -1000.0f, 0.0f}},
    {"mod_17", {435.0f, 25.0f, 10.0f}},
    {"Ship_Change", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(dou_03_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU4"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("moku"), 1, 0)
    CALL(evt_map_playanim, STRING("tubo"), 1, 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 450, 275, -550, STRING("s03_iwadan2"), 125, GSWF(5645))
    CALL(evt_run_case_evt, 9, 1, STRING("A_makimaki"), 0, PTR(watergate_handle), 0)
    SPAWN_EVENT(PTR(watergate_handle_init))
    IF_INT_GREATER_OR_EQUAL(GSW(0), 245)
        CALL(evt_mapobj_trans, 0, STRING("tetu"), 1, 100, 0)
    END_IF
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    IF_INT_LESS(GSW(0), 248)
        SET_VALUE(LW(0), STRING("S_taru"))
        SET_VALUE(LW(1), STRING("A_taru"))
        SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main0), LW(10))
        SET_VALUE(GW(10), LW(10))
        SET_VALUE(LW(0), STRING("S_bin"))
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main1), LW(10))
        SET_VALUE(GW(11), LW(10))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru"), 1)
        CALL(evt_hitobj_onoff, STRING("A_taru"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_bin"), 1)
    END_IF
    SET_VALUE(LW(0), STRING("s03_fune"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main2), LW(10))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change"), STRING("Ship_Change"))
    RETURN
EVT_END()

