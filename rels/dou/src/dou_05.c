#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "memory.h"
#include "cam_road.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

static u8* _ap;

static USER_FUNC(alpha_init_func);
static USER_FUNC(alpha_set_func);
USER_FUNC(cam_displacement);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_mov_spd);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_mobj_switch_blue);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_win_item_select);
static void alpha_tev(MapTevCallbackInfo* info);

static BeroEntry bero_entry_data[5];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
static EvtWord evt_unlock[7];
EvtWord bigdoor_event_init[36];
EvtWord bigdoor_event[152];
static HitDamageReturn hitDmgRetPoints[6];
static EvtWord mobj_box00[28];
EvtWord waterfall_main[30];
EvtWord waterfall_init[25];
EvtWord ride_waterfall[5];
EvtWord nonride_waterfall[19];
EvtWord dou_05_init_evt[136];

static void alpha_tev(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    u8* alpha = _ap;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, *alpha});
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(cam_displacement) {
    CamRoadWork* work = camRoadGetWorkPtr();

    work->f_19c[10] = 1.7f;
    return EVT_RETURN_DONE;
}

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
        "dou_03", // nextMapName
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
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_03", // nextMapName
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
        "dou_03", // nextMapName
        "e_bero_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_06", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "door_01", // enterAnimeArg
        "door_02", // exitAnimeArg
    },
    {0},
};

static s32 key_tbl[] = {
    ITEM_GROTTO_KEY, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, 20, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(bigdoor_event_init)
    IF_INT_LESS(GSW(0), 245)
        DEBUG_PUT_MSG(STRING("dou_05：扉が開く前の処理"))
        SET_VALUE(LW(0), STRING("w_bero_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_trans, 0, STRING("tetu"), 1, 0, 0)
        ELSE
        DEBUG_PUT_MSG(STRING("dou_05：扉が開く後の処理"))
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("w_bero_3"), 4)
        CALL(evt_mapobj_trans, 0, STRING("tetu"), 1, 100, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(bigdoor_event)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_set_mov_spd, FLOAT(0.0f))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 150, 432, 40, 0, 337, -480, 500, 11)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 200)
        WAIT_MS(200)
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 400)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_SLIDE_DOOR1_UP"), LW(10))
    CALL(evt_sub_intpl_msec_init, 0, 0, 100, 2000)
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_trans, 0, STRING("tetu"), 1, LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_SLIDE_DOOR1_DOWN"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 250)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_10"), 0, 0)
    SET_VALUE(LW(0), STRING("w_bero_3"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("w_bero_3"), 4)
    SET_VALUE(GSW(0), 245)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
#ifdef VERSION_G8ME01
    CALL(evt_item_entry, STRING("item00"), 275, 0, -1000, 0, 16, -1, 0)
#else
    CALL(evt_item_entry, STRING("item00"), 277, 0, -1000, 0, 16, -1, 0)
#endif
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(waterfall_main)
    SET_VALUE(LW(0), 255)
    LOOP(0)
        ADD(LW(0), GW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 255)
            SET_VALUE(LW(0), 255)
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(0), 0)
        END_IF
        CALL(alpha_set_func, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(waterfall_init)
    CALL(alpha_init_func)
    CALL(evt_map_set_tevcallback, 1, PTR(alpha_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_ata"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mizu_ata"), 256)
    SET_VALUE(GW(0), 0)
    SPAWN_EVENT(PTR(waterfall_main))
    RETURN
EVT_END()

EVT_BEGIN(ride_waterfall)
    SET_VALUE(GW(0), -4)
    RETURN
EVT_END()

EVT_BEGIN(nonride_waterfall)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(2), -700)
        SET_VALUE(GW(0), -4)
        ELSE
        SET_VALUE(GW(0), 4)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(dou_05_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU5"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("tubo"), 1, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 175, 120, -135, 0, 0)
    CALL(evt_mobj_lock, STRING("mobj_lock00"), 20, 190, 10, 0, 270, PTR(evt_chk), PTR(evt_unlock), GSWF(2979))
    IF_INT_EQUAL(GSWF(2979), 1)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        ELSE
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL(evt_mobj_switch_blue, 0, STRING("mobj00"), -60, 330, -565, PTR(bigdoor_event), GSWF(2978))
    SPAWN_EVENT(PTR(bigdoor_event_init))
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 500, 275, -800, 0, 0, PTR(mobj_box00), GSWF(2986))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), -65, 280, -175, 0, GSWF(5556), 0)
    CALL(evt_mobj_brick, STRING("mobj_hidden1"), -35, 255, -175, 121, 11, 0, GSWF(2993))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change"), STRING("Ship_Change"))
    CALL_CHILD(PTR(waterfall_init))
    CALL(evt_run_case_evt, 0, 1, STRING("mizu_atari"), 0, PTR(ride_waterfall), 0)
    CALL(evt_run_case_evt, 1, 1, STRING("mizu_atari"), 0, PTR(nonride_waterfall), 0)
    CALL(cam_displacement)
    RETURN
EVT_END()

