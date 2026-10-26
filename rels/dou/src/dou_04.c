#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "system.h"
#include "driver/mapdrv.h"
#include "battle/battle_sub.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "party.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_npc.h"


USER_FUNC(evt_box_pukapuka);
USER_FUNC(evt_mario_check_menu);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_name_hitobj_head);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_mapgrp_translate2);

static BeroEntry bero_entry_data[2];
EvtWord box_trans[76];
EvtWord box_pukapuka[5];
EvtWord box_event[222];
EvtWord lift_event_00[223];
EvtWord lift_event_01[236];
EvtWord lift_main_00[53];
EvtWord lift_main_01[53];
EvtWord lift_event_init[17];
EvtWord lift_event[5];
static HitDamageReturn hitDmgRetPoints[4];
static EvtWord kuribo_init[16];
EvtWord kuribo_talk[740];
static NpcSetupInfo npcEnt[2];
EvtWord dou_04_init_evt[96];

USER_FUNC(evt_mario_check_menu) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], marioCheckMenuDisable());
    return EVT_RETURN_DONE;
}

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_03", // nextMapName
        "w_bero_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(box_trans)
    SET_FLOAT_VALUE(LW(3), 0)
    LOOP(0)
        CALL(evt_mapobj_trans, 1, STRING("s04_m_hako_a"), GW(0), GW(1), GW(2))
        CALL(evt_mapobj_trans, 1, STRING("mizuhashi"), GW(0), 0, GW(2))
        CALL(evt_hit_bind_update, STRING("a04_m_hako_03"))
        SET_FLOAT_VALUE(LW(4), GW(0))
        SUBTRACT_FLOAT(LW(4), LW(3))
        CALL(evt_mario_get_name_hitobj_ride, LW(2))
        IF_STR_EQUAL(LW(2), STRING("a04_m_hako_03"))
            CALL(evt_mario_set_hosei_xyz, LW(4), 0, 0)
        END_IF
        CALL(evt_party_get_name_hitobj_ride, 0, LW(2))
        IF_STR_EQUAL(LW(2), STRING("a04_m_hako_03"))
            CALL(evt_party_set_hosei_xyz, 0, LW(4), 0, 0)
        END_IF
        CALL(evt_party_get_name_hitobj_ride, 1, LW(2))
        IF_STR_EQUAL(LW(2), STRING("a04_m_hako_03"))
            CALL(evt_party_set_hosei_xyz, 1, LW(4), 0, 0)
        END_IF
        SET_FLOAT_VALUE(LW(3), GW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(box_pukapuka)
    CALL(evt_box_pukapuka, GW(1))
    RETURN
EVT_END()

EVT_BEGIN(box_event)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    SPAWN_EVENT(PTR(box_trans))
    SPAWN_EVENT(PTR(box_pukapuka))
    CALL(evt_hit_bind_mapobj, STRING("a04_m_hako_03"), STRING("s04_m_hako_a"))
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 3, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 3, 50, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("s04_m_hako_a"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_FLOAT1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_FLOAT2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 50, 49, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 49, 50, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 50, 47, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 47, 0, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("s04_m_hako_a"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_FLOAT1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_FLOAT2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, GW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(lift_event_00)
    IF_INT_EQUAL(LW(0), 0)
        SET_FLOAT_VALUE(LW(0), LW(3))
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("s04_m_hako_b"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a04_m_hako_02"))
            CALL(evt_mario_get_name_hitobj_ride, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_mario_set_hosei_xyz, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 0, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_party_set_hosei_xyz, 0, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 1, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_party_set_hosei_xyz, 1, 0, LW(0), 0)
            END_IF
            SET_VALUE(LF(10), 0)
            CALL(evt_mario_get_name_hitobj_head, LW(2))
            IF_STR_EQUAL(LW(2), STRING("soko2"))
                SET_VALUE(LF(10), 1)
            END_IF
            CALL(evt_mario_check_menu, LF(11))
            IF_INT_EQUAL(LF(10), 1)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), FLOAT(-0.75f))
            WAIT_FRAMES(1)
            IF_FLOAT_LESS_OR_EQUAL(LW(0), FLOAT(-180.0f))
                SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
                CALL(evt_mapobj_trans, 1, STRING("s04_m_hako_b"), 0, LW(0), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("s04_m_hako_b"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_DOWN1"), LW(10), LW(11), LW(12), 0)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0009765625f), 150)
        ELSE
        SET_FLOAT_VALUE(LW(0), LW(3))
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("s04_m_hako_b"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a04_m_hako_02"))
            CALL(evt_mario_get_name_hitobj_ride, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_mario_set_hosei_xyz, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 0, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_party_set_hosei_xyz, 0, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 1, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_02"))
                CALL(evt_party_set_hosei_xyz, 1, 0, LW(0), 0)
            END_IF
            SET_VALUE(LF(10), 0)
            CALL(evt_mario_get_name_hitobj_head, LW(2))
            IF_STR_EQUAL(LW(2), STRING("soko1"))
                SET_VALUE(LF(10), 1)
            END_IF
            CALL(evt_mario_check_menu, LF(11))
            IF_INT_EQUAL(LF(10), 1)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), FLOAT(0.75f))
            WAIT_FRAMES(1)
            IF_FLOAT_GREATER_OR_EQUAL(LW(0), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                CALL(evt_mapobj_trans, 1, STRING("s04_m_hako_b"), 0, LW(0), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_IF
    SET_FLOAT_VALUE(LW(4), LW(0))
    RETURN
EVT_END()

EVT_BEGIN(lift_event_01)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_get_position, STRING("s04_m_ita_b"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_UP1"), LW(0), LW(1), LW(2), LW(9))
        SET_FLOAT_VALUE(LW(0), LW(3))
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("s04_m_ita_b"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a04_m_hako_01"))
            CALL(evt_mario_get_name_hitobj_ride, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_mario_set_hosei_xyz, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 0, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_party_set_hosei_xyz, 0, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 1, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_party_set_hosei_xyz, 1, 0, LW(0), 0)
            END_IF
            SET_VALUE(LF(10), 0)
            CALL(evt_mario_get_name_hitobj_head, LW(2))
            IF_STR_EQUAL(LW(2), STRING("soko2"))
                SET_VALUE(LF(10), 1)
            END_IF
            CALL(evt_mario_check_menu, LF(11))
            IF_INT_EQUAL(LF(10), 1)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), FLOAT(0.5332031f))
            WAIT_FRAMES(1)
            IF_FLOAT_GREATER_OR_EQUAL(LW(0), FLOAT(128.0f))
                SET_FLOAT_VALUE(LW(0), FLOAT(128.0f))
                CALL(evt_mapobj_trans, 1, STRING("s04_m_ita_b"), 0, LW(0), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, LW(9))
        ELSE
        CALL(evt_mapobj_get_position, STRING("s04_m_ita_b"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_STEP_FALL1"), LW(0), LW(1), LW(2), LW(9))
        SET_FLOAT_VALUE(LW(0), LW(3))
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("s04_m_ita_b"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a04_m_hako_01"))
            CALL(evt_mario_get_name_hitobj_ride, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_mario_set_hosei_xyz, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 0, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_party_set_hosei_xyz, 0, 0, LW(0), 0)
            END_IF
            CALL(evt_party_get_name_hitobj_ride, 1, LW(2))
            IF_STR_EQUAL(LW(2), STRING("a04_m_hako_01"))
                CALL(evt_party_set_hosei_xyz, 1, 0, LW(0), 0)
            END_IF
            SET_VALUE(LF(10), 0)
            CALL(evt_mario_get_name_hitobj_head, LW(2))
            IF_STR_EQUAL(LW(2), STRING("soko1"))
                SET_VALUE(LF(10), 1)
            END_IF
            CALL(evt_mario_check_menu, LF(11))
            IF_INT_EQUAL(LF(10), 1)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), FLOAT(-0.5332031f))
            WAIT_FRAMES(1)
            IF_FLOAT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                CALL(evt_mapobj_trans, 1, STRING("s04_m_ita_b"), 0, LW(0), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, LW(9))
    END_IF
    SET_FLOAT_VALUE(LW(4), LW(0))
    RETURN
EVT_END()

EVT_BEGIN(lift_main_00)
    SET_VALUE(LF(10), 0)
    LOOP(0)
        WAIT_WHILE(GF(10))
        IF_INT_EQUAL(LF(10), 0)
            SET_VALUE(LW(0), 0)
            SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
            ELSE
            SET_VALUE(LW(0), 0)
            SET_FLOAT_VALUE(LW(3), LW(4))
        END_IF
        CALL_CHILD(PTR(lift_event_00))
        WAIT_MS(4000)
        IF_INT_EQUAL(LF(10), 0)
            SET_VALUE(LW(0), 1)
            SET_FLOAT_VALUE(LW(3), FLOAT(-180.0f))
            ELSE
            SET_VALUE(LW(0), 1)
            SET_FLOAT_VALUE(LW(3), LW(4))
        END_IF
        CALL_CHILD(PTR(lift_event_00))
        SET_VALUE(GF(10), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(lift_main_01)
    SET_VALUE(LF(10), 0)
    LOOP(0)
        WAIT_WHILE(GF(10))
        IF_INT_EQUAL(LF(10), 0)
            SET_VALUE(LW(0), 0)
            SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
            ELSE
            SET_VALUE(LW(0), 0)
            SET_FLOAT_VALUE(LW(3), LW(4))
        END_IF
        CALL_CHILD(PTR(lift_event_01))
        WAIT_MS(4000)
        IF_INT_EQUAL(LF(10), 0)
            SET_VALUE(LW(0), 1)
            SET_FLOAT_VALUE(LW(3), FLOAT(128.0f))
            ELSE
            SET_VALUE(LW(0), 1)
            SET_FLOAT_VALUE(LW(3), LW(4))
        END_IF
        CALL_CHILD(PTR(lift_event_01))
        SET_VALUE(GF(10), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(lift_event_init)
    CALL(evt_hit_bind_mapobj, STRING("a04_m_hako_01"), STRING("s04_m_ita_b"))
    CALL(evt_hit_bind_mapobj, STRING("a04_m_hako_02"), STRING("s04_m_hako_b"))
    SET_VALUE(GF(10), 1)
    SPAWN_EVENT(PTR(lift_main_00))
    SPAWN_EVENT(PTR(lift_main_01))
    RETURN
EVT_END()

EVT_BEGIN(lift_event)
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {120.0f, 10.0f, 0.0f}},
    {"mod_02", {-90.0f, 10.0f, -110.0f}},
    {"mod_03", {-150.0f, 10.0f, 50.0f}},
    {0},
};

static EVT_BEGIN(kuribo_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_LOOKING_FOR_A_GAL)
        CALL(evt_npc_set_position, STRING("me"), -183, 10, 135)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kuribo_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_LOOKING_FOR_A_GAL)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_21_001"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_party_stop, 0)
                CALL(evt_npc_set_ry, STRING("me"), 90)
                CALL(evt_party_set_dir_npc, 0, STRING("クリチェロ"))
                WAIT_MS(300)
                CALL(evt_msg_print, 0, STRING("irai_21_002"), 0, STRING("me"))
                SET_VALUE(GF(0), 1)
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_mov_pos2, -120, 126, 50)
                    WAIT_FRAMES(1)
                    CALL(evt_mario_set_dir, 270, 300, 0)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
                    CALL(evt_party_move_pos2, 0, -144, 86, 40)
                    WAIT_FRAMES(1)
                    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
                    SET_VALUE(GF(0), 0)
                END_INLINE_EVENT
                CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
                CALL(evt_npc_move_position, STRING("me"), -175, 86, 0, 40, 0)
                WAIT_WHILE(GF(0))
                WAIT_MS(500)
                CALL(evt_msg_print, 0, STRING("irai_21_003"), 0, STRING("me"))
                CALL(evt_msg_print, 0, STRING("irai_21_004"), 0, STRING("party"))
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
                WAIT_MS(600)
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                ADD(LW(0), 5)
                CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 30, 0)
                BEGIN_INLINE_EVENT
                    WAIT_MS(250)
                    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, 5)
                    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(250)
                    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
                    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 200, 1, 5)
                    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
                END_INLINE_EVENT
                CALL(evt_msg_print, 0, STRING("irai_21_005"), 0, STRING("me"))
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                ADD(LW(0), 10)
                ADD(LW(1), 15)
                ADD(LW(2), 5)
                CALL(evt_item_entry, STRING("ff"), ITEM_COUPLES_CAKE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, GSWF(2991), 0)
                WAIT_MS(800)
                CALL(evt_msg_print, 0, STRING("irai_21_006"), 0, STRING("party"))
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
                WAIT_MS(600)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 15)
                SUBTRACT(LW(2), 20)
                CALL(evt_party_move_pos2, 0, LW(0), LW(2), 60)
                CALL(evt_party_set_dir, 0, 270, 300)
                WAIT_MS(300)
                CALL(evt_msg_print, 0, STRING("irai_21_007"), 0, STRING("party"))
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_set_pose, STRING("M_N_5B"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_I_G"))
                    WAIT_MS(600)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
                    WAIT_MS(600)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(100)
                    CALL(evt_item_get_position, STRING("ff"), LW(2), LW(3), LW(4))
                    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(3), 10, 100)
                    LOOP(0)
                        CALL(evt_sub_intpl_msec_get_value)
                        CALL(evt_item_set_position, STRING("ff"), LW(2), LW(0), LW(4))
                        WAIT_FRAMES(1)
                        IF_INT_EQUAL(LW(1), 0)
                            BREAK_LOOP
                        END_IF
                    END_LOOP
                END_INLINE_EVENT
                WAIT_MS(800)
                CALL(evt_msg_print, 0, STRING("irai_21_008"), 0, STRING("me"))
                CALL(evt_npc_move_position, STRING("me"), -140, -76, 0, 60, 0)
                WAIT_MS(1000)
                BEGIN_INLINE_EVENT
                    WAIT_MS(200)
                    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
                    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 30)
                END_INLINE_EVENT
                CALL(evt_msg_print, 0, STRING("irai_21_009"), 0, STRING("me"))
                BEGIN_INLINE_EVENT
                    WAIT_MS(400)
                    CALL(evt_mario_set_dir, 90, 300, 0)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(400)
                    CALL(evt_party_set_dir, 0, 90, 300)
                END_INLINE_EVENT
                CALL(evt_npc_move_position, STRING("me"), 140, -76, 0, 150, 0)
                CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
                CALL(evt_msg_print, 0, STRING("irai_21_010"), 0, STRING("party"))
                CALL(evt_item_get_position, STRING("ff"), LW(0), LW(1), LW(2))
                SUBTRACT(LW(2), 10)
                CALL(evt_party_move_pos2, 0, LW(0), LW(2), 40)
                CALL(evt_item_set_position, STRING("ff"), 0, -1000, 0)
                WAIT_MS(300)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 20)
                SUBTRACT(LW(2), 20)
                CALL(evt_party_move_pos2, 0, LW(0), LW(2), 40)
                WAIT_FRAMES(1)
                CALL(evt_party_set_dir, 0, 90, 300)
                CALL(evt_msg_print, 0, STRING("irai_21_011"), 0, STRING("party"))
                CALL(evt_mario_set_dir, 270, 300, 0)
                WAIT_MS(300)
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    SET_VALUE(LW(1), 10)
                    ELSE
                    SET_VALUE(LW(1), 20)
                END_IF
                IF_INT_LESS(LW(0), LW(1))
                    CALL(evt_msg_continue)
                    CALL(evt_item_get_item, STRING("ff"))
                    CALL(evt_item_get_item_end_wait, STRING("ff"))
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_21_011_1"))
                    CALL(evt_party_set_dir, 0, 270, 300)
                    WAIT_MS(300)
                    CALL(evt_item_set_position, STRING("ff"), -165, 10, 106)
                    CALL(evt_item_flag_off, STRING("ff"), 2)
                    WAIT_MS(300)
                    CALL(evt_party_set_dir, 0, 90, 300)
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_21_013"), 0, STRING("party"))
                CALL(evt_party_run, 0)
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_LOOKING_FOR_A_GAL), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                ELSE
                LOOP(0)
                    CALL(evt_mario_party_use_check, LW(1))
                    IF_INT_EQUAL(LW(1), 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_party_stop, 0)
                SET_VALUE(GF(0), 1)
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_mov_pos2, -150, 100, 60)
                    CALL(evt_mario_set_dir_npc, STRING("クリチェロ"))
                    SET_VALUE(GF(0), 0)
                END_INLINE_EVENT
                CALL(evt_party_move_pos2, 0, -130, 130, 60)
                CALL(evt_party_set_dir_npc, 0, STRING("クリチェロ"))
                WAIT_WHILE(GF(0))
                CALL(evt_npc_set_ry, STRING("me"), 90)
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("me"))
                SWITCH(LW(0))
                    CASE_EQUAL(2)
                    SET_VALUE(LW(0), STRING("irai_21_014"))
                    SET_VALUE(LW(1), STRING("irai_21_015"))
                    CASE_EQUAL(5)
                    SET_VALUE(LW(0), STRING("irai_21_016"))
                    SET_VALUE(LW(1), STRING("irai_21_017"))
                    CASE_EQUAL(4)
                    SET_VALUE(LW(0), STRING("irai_21_018"))
                    SET_VALUE(LW(1), STRING("irai_21_019"))
                    CASE_EQUAL(6)
                    SET_VALUE(LW(0), STRING("irai_21_020"))
                    SET_VALUE(LW(1), STRING("irai_21_021"))
                    CASE_EQUAL(3)
                    SET_VALUE(LW(0), STRING("irai_21_022"))
                    SET_VALUE(LW(1), STRING("irai_21_023"))
                    CASE_EQUAL(7)
                    SET_VALUE(LW(0), STRING("irai_21_024"))
                    SET_VALUE(LW(1), STRING("irai_21_025"))
                END_SWITCH
                CALL(evt_msg_print, 0, LW(0), 0, 0)
                BEGIN_INLINE_EVENT
                    WAIT_MS(200)
                    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
                    CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(7))
                    IF_INT_GREATER_OR_EQUAL(LW(2), LW(5))
                        CALL(evt_mario_set_dir, 270, 200, 0)
                        ELSE
                        CALL(evt_mario_set_dir, 90, 200, 0)
                    END_IF
                END_INLINE_EVENT
                CALL(evt_msg_print, 0, LW(1), 0, STRING("party"))
                CALL(evt_party_run, 0)
            END_IF
        END_IF
        RETURN
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"クリチェロ"*/"\x83\x4E\x83\x8A\x83\x60\x83\x46\x83\x8D", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        kuribo_init, // initEvent
        NULL, // regularEvent
        kuribo_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(dou_04_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU6"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_npc_setup, PTR(npcEnt))
    SPAWN_EVENT(PTR(box_event))
    CALL(evt_mobj_switch_red, 0, STRING("mobj00"), 25, 32, 125, PTR(lift_event), 0)
    SPAWN_EVENT(PTR(lift_event_init))
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("moku"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_item_entry, STRING("kagitsume"), ITEM_GROTTO_KEY, 122, 110, -118, ITEM_TYPE_GROUND, GSWF(2977), 0)
    CALL(evt_item_entry, STRING("starpice"), ITEM_STAR_PIECE, -230, 200, 110, ITEM_TYPE_GROUND, GSWF_STARPIECE_79, 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -230, 290, -105, 0, GSWF_SHINE_30, 0)
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_LOOKING_FOR_A_GAL), 1)
        CALL(evt_item_entry, STRING("iri_21"), ITEM_COUPLES_CAKE, -165, 10, 106, ITEM_TYPE_STILL, GSWF(2991), 0)
    END_IF
    RETURN
EVT_END()

USER_FUNC(evt_box_pukapuka) {
    EvtWord* args = event->args;
    f32 y;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
    }
    y = 1.5f * sinfd(event->userdata[0]) - 1.5f + (0.7f * sinfd(event->userdata[1]) - 0.7f);
    event->userdata[0] = reviseAngle(event->userdata[0] + 2);
    event->userdata[1] = reviseAngle(event->userdata[1] + 1);
    y -= 25.0f;
    evtSetFloat(event, args[0], y);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mapgrp_translate2) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    const char* hitName = (const char*)evtGetValue(event, args[4]);
    f32* prev = &event->lwDataF[6];
    HitEntry* hit;

    mapObjTranslate(name, x, y, z);
    x = 10.0f * (x / 10.0f);
    y = 10.0f * (y / 10.0f);
    z = 10.0f * (z / 10.0f);
    hit = marioGetPtr()->hitobjStandOn;
    if (hit != NULL && strcmp(hitName, hitGetName(hit)) == 0) {
        mario->floorMove.x = x - prev[0];
        mario->floorMove.y = y - prev[1];
        mario->floorMove.z = z - prev[2];
    }
    prev[0] = 10.0f * (x / 10.0f);
    prev[1] = 10.0f * (y / 10.0f);
    prev[2] = 10.0f * (z / 10.0f);
    return EVT_RETURN_DONE;
}

