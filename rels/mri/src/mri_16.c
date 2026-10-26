#include "mri_puni.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "mri.h"
#include "driver/hitdrv.h"

static s32 ssf;

USER_FUNC(puni_get_id);
static USER_FUNC(ss_check);
static USER_FUNC(ss_set);

static BeroEntry bero_entry_data[7];
static EvtWord cam_shake[33];
static EvtWord cam_shake2[27];
EvtWord water_shikake[312];
EvtWord bar_shikake[7];
EvtWord bar_shikake_up[7];
EvtWord switch_up_down[106];
static HitDamageReturn hitDmgRetPoints[5];
static EvtWord mobj_box00[34];
EvtWord mri_16_init_evt[282];

static USER_FUNC(ss_check) {
    s32 a;
    s32 z;

    // FAKE: the dead condition moves the ssf load ahead of event->args; casts, temps and
    // u32 index variants do not reproduce it
    a = event->args[0];
    z = !ssf;
    if ((z && !ssf) != 0) {
    }
    evtSetFloat(event, a, ssf);
    return EVT_RETURN_DONE;
}

static USER_FUNC(ss_set) {
    ssf = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(puni_get_id) {
    s32 offset;
    EvtWord* args = event->args;
    PuniData* puni;

    offset = evtGetValue(event, args[0]) * sizeof(PuniData);
    puni = (PuniData*)((u8*)puniGetPunidat() + offset);
    evtSetValue(event, args[1], puni->index);
    return EVT_RETURN_DONE;
}

static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_18", // nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan6", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan4", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan6", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan7", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_13", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(cam_shake)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 7, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0068359375f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

static EVT_BEGIN(cam_shake2)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 7, 0, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(water_shikake)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_set_mov_spd, FLOAT(0.0f))
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(3))
    SET_VALUE(LW(10), 1)
    LOOP(0)
        SET_VALUE(LF(0), 1)
        CALL(evt_puni_get_flag, LW(10), LW(0))
        AND_RAW(LW(0), 1024)
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LF(0), 0)
        END_IF
        CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(1), -200)
            SET_VALUE(LF(0), 0)
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_puni_set_mode, LW(10), 16)
            CALL(evt_puni_flag_onoff, LW(10), 0, PUNI_FLAG_COLLIDE)
            CALL(puni_get_id, LW(10), LW(3))
            AND(LW(3), 8)
            SUBTRACT(LW(3), 4)
            ADD(LW(3), -230)
            CALL(evt_puni_set_position, LW(10), LW(3), -320, LW(2))
        END_IF
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_l03"), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_before"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mizu_now"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE1"), GW(0))
    CALL(evt_snd_sfxon, STRING("SFX_STG2_WATER_FLOW1"), GW(1))
    SPAWN_EVENT_GET_ID(PTR(cam_shake), LW(10))
    CALL(evt_map_playanim, STRING("mizu_guru"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -154, 186, 520, -14, 100, -48, 1500, EASING_SINE_IN_OUT)
        WAIT_MS(3500)
        CALL(evt_cam3d_evt_set, -154, -184, 520, -14, -271, -48, 5000, EASING_SINE_IN_OUT)
        WAIT_MS(2500)
        CALL(evt_snd_sfxoff, GW(0))
        WAIT_MS(2500)
        WAIT_MS(8000)
        CALL(evt_snd_sfxoff, GW(1))
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    CALL(evt_map_checkanim, STRING("mizu_guru"), LW(0), LW(1))
    WAIT_MS(LW(1))
    DELETE_EVENT(LW(10))
    CALL_CHILD(PTR(cam_shake2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_now"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mizu_after"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_mizu_after"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f2_kaidan"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_mizu_after"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f2_kai_rak"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak_ana"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa01"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_isk_03"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa01"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa02"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa03"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa04"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa05"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_mkusa_06"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f1_kaidan"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_mizu_before"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak_l03"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_l04"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_f1_rak_r03"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_mizi_kusa"), 1, 1)
    CALL(evt_puni_set_ymode, 5)
    CALL(evt_puni_set_maze)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(bar_shikake)
    DEBUG_PUT_MSG(STRING("のったとき"))
    CALL(ss_set, 1)
    RETURN
EVT_END()

EVT_BEGIN(bar_shikake_up)
    DEBUG_PUT_MSG(STRING("のらなかったとき"))
    CALL(ss_set, 2)
    RETURN
EVT_END()

EVT_BEGIN(switch_up_down)
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    IF_INT_EQUAL(GSWF(2867), 0)
        CALL(evt_mobj_get_y_position, STRING("mobj00"), LW(3))
        ELSE
        SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    END_IF
    SET_FLOAT_VALUE(LW(4), LW(3))
    LOOP(0)
        CALL(ss_check, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            SET_FLOAT_VALUE(LW(1), FLOAT(-0.3330078f))
            ADD_FLOAT(LW(2), LW(1))
            MULTIPLY_FLOAT(LW(1), FLOAT(5.0f))
            ADD_FLOAT(LW(4), LW(1))
            IF_FLOAT_LESS_OR_EQUAL(LW(2), FLOAT(-10.0f))
                SET_FLOAT_VALUE(LW(2), FLOAT(-10.0f))
                SET_FLOAT_VALUE(LW(4), FLOAT(-50.0f))
                ADD_FLOAT(LW(4), LW(3))
                CALL(ss_set, 0)
            END_IF
            CASE_EQUAL(2)
            SET_FLOAT_VALUE(LW(1), FLOAT(0.08300781f))
            ADD_FLOAT(LW(2), LW(1))
            MULTIPLY_FLOAT(LW(1), FLOAT(5.0f))
            ADD_FLOAT(LW(4), LW(1))
            IF_FLOAT_GREATER_OR_EQUAL(LW(2), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(4), LW(3))
                CALL(ss_set, 0)
            END_IF
        END_SWITCH
        CALL(evt_mapobj_trans, 1, STRING("S_sikake"), 0, LW(2), 0)
        CALL(evt_hit_bind_update, STRING("A_sikake"))
        IF_INT_EQUAL(GSWF(2867), 0)
            CALL(evt_mobj_set_y_position, STRING("mobj00"), LW(4))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"S_f2_yuka01_l", {-248.0f, 80.0f, 27.0f}},
    {"S_f2_yuka01_r", {248.0f, 80.0f, 27.0f}},
    {"A_f1_yuka04", {-244.0f, -319.0f, 34.0f}},
    {"A_f1_yuka03", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_SHRINK_STOMP, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mri_16_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 16, 80, -319, -1000, -1000, 4, 3)
    CALL(evt_puni_set_hamari, -231, -319, -71, 82, -2, -317, -401)
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
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI10"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mobj_switch_blue, 0, STRING("mobj00"), 0, 79, -120, PTR(water_shikake), GSWF(2867))
    IF_INT_EQUAL(GSWF(2867), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("mobj00"), MOBJ_FLAG_GRAVITY)
    END_IF
    CALL(evt_hit_bind_mapobj, STRING("A_sikake"), STRING("S_sikake"))
    CALL(ss_set, 0)
    CALL(evt_mobj_rideswitch_lightblue, STRING("mobj01"), 125, 80, -120, PTR(bar_shikake), PTR(bar_shikake_up), GSWF(2868))
    SPAWN_EVENT(PTR(switch_up_down))
    IF_INT_EQUAL(GSWF(2867), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_now"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_f2_kaidan"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_mizu_after"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_l03"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_kai_rak"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_ana"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_mizi_kusa"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa01"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa02"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa03"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa04"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_mkusa05"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_mkusa_06"), 1, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_now"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mizu_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_f1_kaidan"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_mizu_before"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_l03"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_l04"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f1_rak_r03"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_isk_03"), 1, 0)
        CALL(evt_puni_set_ymode, 5)
    END_IF
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_item_entry, STRING("item_00"), ITEM_DIZZY_DIAL, -115, 0, -25, ITEM_TYPE_GROUND, GSWF(2869), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -125, 0, 125, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF(2885))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 65, -260, -70, 0, GSWF_SHINE_19, 0)
    RETURN
EVT_END()

