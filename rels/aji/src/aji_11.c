#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_case.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"

#include "driver/mapdrv.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "aji.h"
#include "mario/mario_pouch.h"

USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(evt_win_item_select);
USER_FUNC(map_anim_flag_off);
static USER_FUNC(peach_evt_toumei);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[2];
EvtWord peach_evt_bero[50];
static BeroEntry bero_peach_data[2];
static EvtWord key_evt[5];
EvtWord evt_gajigaji_anim[13];
EvtWord evt_gajigaji_main[96];
EvtWord evt_gajigaji[50];
static s32 item_list[2];
EvtWord peach_evt_disksys[164];
EvtWord peach_evt_tana1[50];
EvtWord peach_evt_tana2[31];
static EvtWord peach_evt_stg5[105];
EvtWord aji_11_init_evt[111];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_A"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("door_w_B"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_10", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(peach_evt_bero)
    IF_INT_EQUAL(GSWF(4208), 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, 264)
            IF_INT_EQUAL(GSWF(4209), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_msg_print, 0, STRING("peach_aji_249_01"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
                ELSE
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_msg_print, 0, STRING("peach_aji_250"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
            END_IF
            SET_VALUE(LW(0), 1)
            ELSE
            SET_VALUE(LW(0), 0)
        END_IF
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    RETURN
EVT_END()

static BeroEntry bero_peach_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)peach_evt_bero, // exitEvent
        "aji_10", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(key_evt)
    SET_VALUE(GSWF(4180), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_gajigaji_anim)
    LOOP(0)
        CALL(evt_npc_set_anim, LW(10), STRING("GAZ_A_1"))
        CALL(evt_npc_wait_anim, LW(10), FLOAT(1.0f))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_gajigaji_main)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(11), LW(1))
    CALL(evt_sub_random, 600, LW(0))
    SUBTRACT(LW(0), 300)
    CALL(evt_sub_random, 180, LW(1))
    CALL(evt_npc_entry, LW(10), STRING("c_gazigazi"))
    CALL(evt_npc_set_anim, LW(10), STRING("GAZ_S_1"))
    CALL(evt_npc_set_position, LW(10), LW(0), LW(1), LW(11))
    CALL(evt_sub_random, 100, LW(0))
    ADD(LW(0), 50)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_scale, LW(10), LW(0), LW(0), LW(0))
    CALL(evt_sub_random, 100, LW(0))
    IF_INT_LESS(LW(0), 50)
        SPAWN_EVENT(PTR(evt_gajigaji_anim))
    END_IF
    LOOP(0)
        CALL(evt_sub_random, 600, LW(0))
        SUBTRACT(LW(0), 300)
        CALL(evt_sub_random, 180, LW(1))
        CALL(evt_sub_random, 40, LW(2))
        SUBTRACT(LW(2), 20)
        CALL(evt_npc_status_onoff, 1, LW(10), 2)
        CALL(evt_npc_glide_position, LW(10), LW(0), LW(1), LW(11), 0, FLOAT(60.0f), LW(2), 11, 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_gajigaji)
    SET_VALUE(LW(0), STRING("gazi1"))
    SET_VALUE(LW(1), -280)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    SET_VALUE(LW(0), STRING("gazi2"))
    SET_VALUE(LW(1), -275)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    SET_VALUE(LW(0), STRING("gazi3"))
    SET_VALUE(LW(1), -270)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    SET_VALUE(LW(0), STRING("gazi4"))
    SET_VALUE(LW(1), -265)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    SET_VALUE(LW(0), STRING("gazi5"))
    SET_VALUE(LW(1), -260)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    SET_VALUE(LW(0), STRING("gazi6"))
    SET_VALUE(LW(1), -255)
    SPAWN_EVENT(PTR(evt_gajigaji_main))
    RETURN
EVT_END()

static s32 item_list[] = {
    ITEM_DATA_DISK, -1,
};

EVT_BEGIN(peach_evt_disksys)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(4208), 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_244_01"), 0, STRING("mario"))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_win_item_select, 0, PTR(item_list), LW(0), 0)
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("monitor_02"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("monitor_03"), 1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_DISC_SET1"), 57, 0, -10, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_MONITOR_ON1"), 57, 0, -10, 0)
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_PEACH_DISCSET1"))
        CALL(evt_map_playanim, STRING("monitor_01"), 0, 0)
        CALL(evt_map_checkanim, STRING("monitor_01"), LW(0), LW(1))
        WAIT_MS(LW(1))
        WAIT_MS(2500)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_247"), 0, STRING("mario"))
        CALL(evt_map_playanim, STRING("monitor_02"), 1, 0)
        WAIT_MS(3000)
        CALL(map_anim_flag_off, STRING("monitor_02"), 4)
        CALL(evt_map_checkanim, STRING("monitor_02"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_PEACH_MONITOR_OFF1"), 57, 0, -10, 255, 255, 1500, 16, 0)
        CALL(evt_map_playanim, STRING("monitor_03"), 0, 0)
        CALL(evt_map_checkanim, STRING("monitor_03"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor_02"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor_03"), 1)
        CALL(evt_msg_print, 0, STRING("peach_aji_248"), 0, STRING("mario"))
        SET_VALUE(GSWF(4209), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("tana"), 0, PTR(peach_evt_tana2), 0)
        CALL(evt_exit_case_evt)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_tana1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_245"), 0, STRING("mario"))
    CALL(evt_item_entry, STRING("disk"), 86, 0, -1000, 0, 17, -1, 0)
    WAIT_FRAMES(1)
    CALL(evt_item_get_item, STRING("disk"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_246"), 0, STRING("mario"))
    SET_VALUE(GSWF(4208), 1)
    CALL(evt_run_case_evt, 9, 1, STRING("a_disksys"), 0, PTR(peach_evt_disksys), 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_tana2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_pouch_check_item, 86, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(evt_pouch_remove_item, 86, LW(0))
    END_IF
    CALL(evt_msg_print, 0, STRING("peach_aji_249"), 0, STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 264)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg5)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 64)
        CALL(evt_sub_system_flag_onoff, 0, 64)
        CALL(evt_peach_set_condition, 2)
    END_IF
    CALL(peach_evt_toumei)
    IF_INT_LESS(GSW_STORY_PROGRESS, 264)
        IF_INT_EQUAL(GSWF(4208), 0)
            CALL(evt_run_case_evt, 9, 1, STRING("tana"), 0, PTR(peach_evt_tana1), 0)
            CALL(evt_run_case_evt, 9, 1, STRING("a_disksys"), 0, PTR(peach_evt_disksys), 0)
            ELSE
            IF_INT_EQUAL(GSWF(4209), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("a_disksys"), 0, PTR(peach_evt_disksys), 0)
                ELSE
                CALL(evt_run_case_evt, 9, 1, STRING("tana"), 0, PTR(peach_evt_tana2), 0)
            END_IF
        END_IF
    END_IF
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    IF_INT_EQUAL(GSWF(4207), 0)
        CALL(evt_mario_key_onoff, 0)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("peach_aji_244"), 0, STRING("mario"))
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(GSWF(4207), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(aji_11_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(263, 264)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN1"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
    END_SWITCH
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI6"))
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("sikaku"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_1"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_2"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_3"), 1, 0)
    CALL(evt_map_playanim, STRING("mizutama_1"), 1, 0)
    CALL(evt_map_playanim, STRING("mizutama_2"), 1, 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 352)
        IF_INT_EQUAL(GSWF(4180), 0)
            CALL(evt_item_entry, STRING("dummy"), 31, 50, 25, 25, 16, -1, PTR(key_evt))
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor_02"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monitor_03"), 1)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 263)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 264)
            CALL(evt_mario_set_mode, 1)
            CALL(evt_mario_kill_party, 0)
            SPAWN_EVENT(PTR(peach_evt_stg5))
        END_IF
    END_IF
    SPAWN_EVENT(PTR(evt_gajigaji))
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

USER_FUNC(map_anim_flag_off) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);
    MapEntryAnimData* anim = mapSearchAnmObj(name);

    if (anim == NULL) {
        return EVT_RETURN_DONE;
    }
    anim->flags &= ~mask;
    return EVT_RETURN_DONE;
}
static USER_FUNC(peach_evt_toumei) {
    MarioWork* mario = marioGetPtr();

    mario->scale.z = mario->scale.y = mario->scale.x = 0.0f;
    return EVT_RETURN_DONE;
}
