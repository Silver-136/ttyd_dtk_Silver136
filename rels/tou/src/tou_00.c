#include "eff/n64/eff_kemuri12_n64.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"

#include "driver/camdrv.h"
#include "dolphin/os.h"
#include "driver/npcdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "evt/evt_npc.h"

USER_FUNC(evt_snd_bgm_freq);
USER_FUNC(kemuri_move);
static USER_FUNC(kpa_hikousen_yure);
USER_FUNC(time_shift);
USER_FUNC(evt_snd_bgmon_f_d);
static USER_FUNC(npc_set_pos);

static EvtWord evt_tou_balloon[85];
static EvtWord evt_light[66];
static EvtWord evt_wing[125];
EvtWord evt_to_tou[293];
EvtWord evt_pos_keep[39];
EvtWord evt_to_gor[247];
const char* kemuri_name[5];
EvtWord tou_00_koopa_evt[678];
static EvtWord hkoopa_init[7];
static NpcSetupInfo npcEnt[2];
EvtWord evt_skip[43];
EvtWord tou_00_init_evt[76];

USER_FUNC(time_shift) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);

    cam->startTimeA += OSMillisecondsToTicks((OSTime)-2000);
    event->savedTime += OSMillisecondsToTicks((OSTime)-3500);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_tou_balloon)
    CALL(evt_mapobj_get_position, STRING("tou_enkei"), LW(13), LW(14), LW(15))
    SUBTRACT(LW(14), 20)
    SUBTRACT(LW(15), 50)
    LOOP(0)
        CALL(evt_sub_random, 2, LW(0))
        SET_VALUE(LW(1), 50)
        CALL(evt_sub_random, 100, LW(10))
        SUBTRACT(LW(1), LW(10))
        SET_VALUE(LW(2), 20)
        CALL(evt_sub_random, 40, LW(10))
        SUBTRACT(LW(2), LW(10))
        SET_VALUE(LW(3), 30)
        CALL(evt_sub_random, 40, LW(10))
        SUBTRACT(LW(3), LW(10))
        ADD(LW(1), LW(13))
        ADD(LW(2), LW(14))
        ADD(LW(3), LW(15))
        CALL(evt_eff64, 0, STRING("balloon_heiho_n64"), LW(0), LW(1), LW(2), LW(3), FLOAT(0.009765625f), FLOAT(0.39941406f), 600, 0, 0, 0, 0, 0)
        CALL(evt_sub_random, 500, LW(0))
        ADD(LW(0), 300)
        WAIT_MS(LW(0))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_light)
    SET_VALUE(LW(9), LW(0))
    LOOP(0)
        CALL(evt_sub_intpl_init, 11, -20, 20, 60)
        LOOP(61)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_rotate, 1, LW(9), 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_random, 120, LW(0))
        ADD(LW(0), 1)
        WAIT_FRAMES(LW(0))
        CALL(evt_sub_intpl_init, 11, 20, -20, 60)
        LOOP(61)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_rotate, 1, LW(9), 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_random, 180, LW(0))
        ADD(LW(0), 30)
        WAIT_FRAMES(LW(0))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_wing)
    LOOP(0)
        CALL(evt_sub_random, 100, LW(0))
        IF_INT_LESS(LW(0), 20)
            LOOP(3)
                CALL(evt_sub_intpl_init, 12, -10, 10, 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_value)
                    CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                    CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_sub_intpl_init, 12, 10, -10, 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_value)
                    CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                    CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
            ELSE
            CALL(evt_sub_intpl_init, 12, -10, 10, 30)
            LOOP(31)
                CALL(evt_sub_intpl_get_value)
                CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init, 12, 10, -10, 30)
            LOOP(31)
                CALL(evt_sub_intpl_get_value)
                CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_to_tou)
    CALL(evt_cam3d_evt_set, -2000, 241, 534, -2000, 145, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmon_f_d, 768, STRING("BGM_STG3_URN1"), 18000, 18000)
    CALL(evt_snd_bgmoff, 26624)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora_2"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hako_1"), 1)
    CALL(evt_mapobj_trans, 1, STRING("gondora_1"), 0, 80, 0)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_10"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_9"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_8"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_7"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_4"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_2"), 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_AIR_SHIP_DRIVE3"), 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, 0, 241, 534, 0, 145, -8, 15000, 11)
    CALL(evt_sub_intpl_msec_init, 11, -25000, 0, 18000)
    CALL(time_shift)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, STRING("hikousen"), LW(0), 20, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 30)
            SUBTRACT(LW(2), 3)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            SUBTRACT(LW(2), 2)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_2"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_4"), 1)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_7"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_8"), 1)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_10"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_9"), 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), GW(11))
    CALL(evt_sub_intpl_msec_init, 11, 80, 0, 3000)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("gondora_1"), 0, LW(0), 0)
    CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfx_pos, GW(11), LW(7), LW(8), LW(9))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_snd_sfxoff, GW(11))
    DELETE_EVENT(GW(15))
    CALL(evt_bero_mapchange, STRING("tou_01"), 0)
    RETURN
EVT_END()
#else
EVT_BEGIN(evt_to_tou)
    CALL(evt_cam3d_evt_set, -2000, 241, 534, -2000, 145, -8, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmon_f_d, 768, STRING("BGM_STG3_URN1"), 18000, 18000)
    CALL(evt_snd_bgmoff, 26624)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora_2"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hako_1"), 1)
    CALL(evt_mapobj_trans, 1, STRING("gondora_1"), 0, 80, 0)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_10"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_9"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_8"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_7"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_4"), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_2"), 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_AIR_SHIP_DRIVE3"), 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, 0, 241, 534, 0, 145, -8, 15000, 11)
    CALL(evt_sub_intpl_msec_init, 11, -25000, 0, 18000)
    CALL(time_shift)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, STRING("hikousen"), LW(0), 20, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 30)
            SUBTRACT(LW(2), 3)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            SUBTRACT(LW(2), 2)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_2"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_4"), 1)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_7"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_8"), 1)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_10"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("bou_9"), 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), GW(11))
    CALL(evt_sub_intpl_msec_init, 11, 80, 0, 3000)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("gondora_1"), 0, LW(0), 0)
    CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfx_pos, GW(11), LW(7), LW(8), LW(9))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_snd_sfxoff, GW(11))
    DELETE_EVENT(GW(15))
    CALL(evt_bero_mapchange, STRING("tou_01"), 0)
    RETURN
EVT_END()
#endif

EVT_BEGIN(evt_pos_keep)
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(0), LW(1), LW(2))
        CALL(evt_mapobj_get_position, STRING("gondora_2"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(1), 30)
        SUBTRACT(LW(2), 3)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        SUBTRACT(LW(2), 2)
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_to_gor)
    CALL(evt_cam3d_evt_set, 0, 241, 534, 0, 145, -8, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora_1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hako_2"), 1)
    CALL(evt_mapobj_trans, 1, STRING("hikousen"), -30, 20, 0)
    CALL(evt_mapobj_rotate, 1, STRING("hikousen"), 0, 180, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_seq_wait, 2)
    SPAWN_EVENT_GET_ID(PTR(evt_pos_keep), LW(9))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_20"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_19"), 1)
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_17"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_18"), 1)
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_12"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("bou_14"), 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("gondora_2"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), GW(11))
    CALL(evt_sub_intpl_msec_init, 11, 0, 80, 3000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("gondora_2"), 0, LW(0), 0)
    CALL(evt_mapobj_get_position, STRING("gondora_2"), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfx_pos, GW(11), LW(7), LW(8), LW(9))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_snd_sfxoff, GW(11))
    DELETE_EVENT(LW(9))
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gondora_2"), 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_AIR_SHIP_DRIVE4"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, -30, -2000, 20000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("hikousen"), LW(0), 20, 0)
        CALL(evt_mapobj_rotate, 1, STRING("hikousen"), 0, 180, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -2000, 241, 534, -2000, 145, -8, 20000, 11)
    WAIT_MS(8000)
    DELETE_EVENT(GW(15))
    SET_VALUE(GSWF(1244), 1)
    CALL(evt_bero_mapchange, STRING("gor_04"), STRING("hikousen_return"))
    RETURN
EVT_END()

static USER_FUNC(npc_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    NpcEntry* npc = npcNameToPtr(name);

    npc->prevPosition = (Vec){x, y, z};
    npc->position = npc->prevPosition;
    return EVT_RETURN_DONE;
}

const char* kemuri_name[] = {
    "kosyo01",
    "kosyo02",
    "kosyo03",
    "kosyo04",
    "kosyo05",
};

static USER_FUNC(kpa_hikousen_yure) {
    f32 y = evtGetFloat(event, event->args[0]);

    // 飛クッパ
    npcNameToPtr("\x94\xF2\x83\x4E\x83\x62\x83\x70")->moveOffset.y = y;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(tou_00_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_npc_set_position, STRING("飛クッパ"), -2300, -17, 2)
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 90)
    CALL(evt_cam3d_evt_set, -2000, 186, 730, -2000, 55, -8, 0, 11)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_bg_auto_scroll_onoff, 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.001953125f))
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        LOOP(0)
            ADD_FLOAT(LW(0), GW(0))
            CALL(evt_bg_set_scrl_offset, LW(0), LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(1), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
            MULTIPLY_FLOAT(LW(2), FLOAT(5.0f))
            SET_FLOAT_VALUE(LW(3), LW(2))
            SUBTRACT_FLOAT(LW(3), LW(1))
            CALL(kpa_hikousen_yure, LW(3))
            WAIT_FRAMES(1)
            ADD_FLOAT(LW(0), FLOAT(2.0f))
            SET_FLOAT_VALUE(LW(1), LW(2))
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_seq_wait, 2)
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_W_1"))
    CALL(evt_snd_sfxon, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), GW(2))
    CALL(evt_sub_intpl_msec_init, 0, -2300, -2000, 7500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), LW(3), LW(4))
    CALL(evt_snd_sfx_pos, GW(2), LW(0), LW(3), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(3750)
    SET_VALUE(GF(1), 1)
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_BREAK1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 32000)
        LOOP(100)
            SUBTRACT(LW(0), 320)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
        CALL(evt_snd_bgmoff_f, 512, 2000)
    END_INLINE_EVENT
    SUBTRACT_FLOAT(LW(0), FLOAT(10.0f))
    ADD_FLOAT(LW(1), FLOAT(20.0f))
    ADD_FLOAT(LW(2), FLOAT(80.0f))
    CALL(evt_eff64, STRING("kosyo1"), STRING("kemuri12_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 18, 0, 0, 0, 0, 0, 0)
    ADD_FLOAT(LW(0), FLOAT(30.0f))
    CALL(evt_eff64, STRING("kosyo2"), STRING("kemuri12_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 18, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), GW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1000.0f))
        CALL(kemuri_move, STRING("kosyo1"), LW(0))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), GW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1000.0f))
        CALL(kemuri_move, STRING("kosyo2"), LW(0))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    SET_VALUE(GW(1), LW(3))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
        CALL(evt_sub_intpl_msec_init, 4, 180, 360, 400)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), FLOAT(20.0f))
        ADD_FLOAT(LW(5), LW(3))
        SET_VALUE(GW(1), LW(5))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    CALL(evt_snd_sfx_pit, GW(2), -1365)
    CALL(evt_sub_intpl_msec_init, 0, LW(2), LW(2), 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), GW(1), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    MULTIPLY_FLOAT(GW(0), FLOAT(0.5f))
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_S_1"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("飛クッパ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("飛クッパ"))
    CALL(evt_npc_reverse_ry, STRING("飛クッパ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("飛クッパ"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
        SET_FLOAT_VALUE(LW(5), FLOAT(0.0f))
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_sub_get_sincos, LW(5), LW(6), LW(7))
            SET_VALUE(LF(0), 0)
            IF_INT_GREATER_OR_EQUAL(LW(5), FLOAT(180.0f))
                IF_INT_LESS(LW(5), FLOAT(360.0f))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                MULTIPLY_FLOAT(LW(6), FLOAT(1.0f))
                ELSE
                MULTIPLY_FLOAT(LW(6), FLOAT(5.0f))
            END_IF
            ADD_FLOAT(LW(6), LW(3))
            CALL(evt_npc_set_position, STRING("飛クッパ"), LW(2), LW(6), LW(4))
            SET_VALUE(LF(0), 0)
            IF_INT_GREATER_OR_EQUAL(LW(5), FLOAT(90.0f))
                IF_INT_LESS(LW(5), FLOAT(270.0f))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                ADD_FLOAT(LW(5), FLOAT(2.0f))
                ELSE
                ADD_FLOAT(LW(5), FLOAT(6.0f))
            END_IF
            IF_INT_GREATER_OR_EQUAL(LW(5), FLOAT(360.0f))
                SUBTRACT_FLOAT(LW(5), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("kpa_stg4_008"), 0, STRING("飛クッパ"))
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(0), LW(1), LW(2))
    ADD(LW(2), 80)
    SET_INT_READ_ADDR(PTR(&kemuri_name))
    LOOP(5)
        READ_INT(LW(5))
        CALL(evt_sub_random, 40, LW(3))
        SUBTRACT(LW(3), 20)
        ADD(LW(3), LW(0))
        CALL(evt_sub_random, 40, LW(4))
        ADD(LW(4), LW(1))
        CALL(evt_eff64, LW(5), STRING("kemuri12_n64"), 0, LW(3), LW(4), LW(2), FLOAT(1.0f), 18, 0, 0, 0, 0, 0, 0)
        BEGIN_INLINE_EVENT
            SET_FLOAT_VALUE(LW(0), GW(0))
            MULTIPLY_FLOAT(LW(0), FLOAT(-1000.0f))
            CALL(kemuri_move, LW(5), LW(0))
        END_INLINE_EVENT
        WAIT_MS(100)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 4, -1365, -5461, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_snd_sfx_pit, GW(2), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, GW(2))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_1"))
    WAIT_MS(1500)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_VALUE(GF(0), 1)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_2"))
    WAIT_MS(200)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("飛クッパ"), 512)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("kpa_stg4_009"), 0, STRING("飛クッパ"))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_3"))
    WAIT_MS(500)
    CALL(evt_npc_set_rotate_offset, STRING("飛クッパ"), 0, 30, 0)
    CALL(evt_sub_intpl_msec_init, 1, 0, -180, 400)
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_rotate, STRING("飛クッパ"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_VEXATIOUS2_2"), LW(2), LW(3), LW(4), 0)
    CALL(evt_snd_sfxon, STRING("SFX_KUPPA_AIRSHIP_FALL1"), GW(1))
    CALL(evt_sub_intpl_msec_init, 1, LW(3), -200, 800)
    LABEL(6)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(6)
    END_IF
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("muj_20"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(hkoopa_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

USER_FUNC(kemuri_move) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 dy = evtGetFloat(event, args[1]);
    EffectEntry* eff = effNameToPtr(name);

    if (eff == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }
    ((EffKemuri12N64Work*)eff->userdata)->position.x += dy;
    return EVT_RETURN_BLOCK;
}

static NpcSetupInfo npcEnt[] = {
    {
        /*"飛クッパ"*/"\x94\xF2\x83\x4E\x83\x62\x83\x70", // name
        0x200, // flags
        0, // reactionFlags
        hkoopa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_skip)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1100)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), 768)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, GW(11))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("modoru"))
        SET_VALUE(GSWF(1244), 1)
        CALL(evt_bero_mapchange, STRING("gor_04"), STRING("hikousen_return"))
        ELSE
        CALL(evt_bero_mapchange, STRING("tou_01"), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(tou_00_init_evt)
    SET_VALUE(GW(11), 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 215)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), 1)
        CALL(evt_mario_set_mode, 2)
        SPAWN_EVENT(PTR(tou_00_koopa_evt))
        RETURN
    END_IF
    CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU8"))
    CALL(evt_map_playanim, STRING("jet"), 1, 0)
    CALL(evt_map_playanim, STRING("lightcoloer"), 1, 0)
    SET_VALUE(LW(0), STRING("coloer_light_1"))
    SPAWN_EVENT(PTR(evt_light))
    SET_VALUE(LW(0), STRING("coloer_light_2"))
    SPAWN_EVENT(PTR(evt_light))
    SET_VALUE(LW(0), STRING("coloer_light_3"))
    SPAWN_EVENT(PTR(evt_light))
    SPAWN_EVENT(PTR(evt_wing))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("modoru"))
        SPAWN_EVENT(PTR(evt_to_gor))
        ELSE
        SPAWN_EVENT(PTR(evt_to_tou))
    END_IF
    SPAWN_EVENT(PTR(evt_tou_balloon))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 127)
        SPAWN_EVENT_GET_ID(PTR(evt_skip), GW(15))
    END_IF
    RETURN
EVT_END()

