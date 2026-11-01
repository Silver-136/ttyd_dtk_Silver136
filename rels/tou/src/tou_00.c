// tou_00 (闘技場, Glitzville): the blimp landing outside the arena; exits to tou_01 and back to gor_04.

#include "version.h"
#include "eff/n64/eff_kemuri12_n64.h"
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
#include "driver/mapdrv.h"
#include "system.h"
#include "pmario_sound.h"

#include "driver/camdrv.h"
#include "dolphin/os.h"
#include "driver/npcdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"
#include <dolphin/pad.h>

USER_FUNC(kemuri_move);
static USER_FUNC(kpa_hikousen_yure);
USER_FUNC(time_shift);
static USER_FUNC(npc_set_pos);

static EvtWord evt_tou_balloon[85];
static EvtWord evt_light[66];
static EvtWord evt_wing[125];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_to_tou[];
#else
EvtWord evt_to_tou[293];
#endif
EvtWord evt_pos_keep[39];
EvtWord evt_to_gor[247];
const char* kemuri_name[5];
EvtWord tou_00_koopa_evt[678];
static EvtWord hkoopa_init[7];
static NpcSetupInfo npcEnt[2];
EvtWord evt_skip[43];
EvtWord tou_00_init_evt[76];

/**
 * Script helper: moves the 3D camera's start time back 2 s and the event's saved time back 3.5 s,
 * so the blimp fly-in scripted to a fixed duration lines up with the camera move.
 */
USER_FUNC(time_shift) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);

    cam->startTimeA += OSMillisecondsToTicks((OSTime)-2000);
    event->savedTime += OSMillisecondsToTicks((OSTime)-3500);
    return EVT_RETURN_DONE;
}

/**
 * Decoration loop for the Glitz Pit exterior: every 0.3 to 0.8 seconds a random "balloon_heiho_n64" balloon
 * effect floats out from a point near the tou_enkei arena object.
 */
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

/**
 * Searchlight sway loop: rocks the map object whose name is in LW(0) between -20
 * and +20 degrees about the Z axis over 60 frames each way, with a random pause between swings.
 */
static EVT_BEGIN(evt_light)
    SET_VALUE(LW(9), LW(0))
    LOOP(0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -20, 20, 60)
        LOOP(61)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_rotate, 1, LW(9), 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_random, 120, LW(0))
        ADD(LW(0), 1)
        WAIT_FRAMES(LW(0))
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 20, -20, 60)
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

/**
 * Wing flap loop for the arena's decoration: the map objects hane_1 and hane_2 swing back
 * and forth about Z by 10 degrees.
 * 20% of the time they flutter three quick strokes (10 frames each way),
 * otherwise they take one slow stroke (30 frames each way).
 */
// Inclusive random 0..100 selects the fast flap for values 0..19.
static EVT_BEGIN(evt_wing)
    LOOP(0)
        CALL(evt_sub_random, 100, LW(0))
        IF_INT_LESS(LW(0), 20)
            LOOP(3)
                CALL(evt_sub_intpl_init, EASING_SINE_OUT, -10, 10, 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_value)
                    CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                    CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_sub_intpl_init, EASING_SINE_OUT, 10, -10, 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_value)
                    CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                    CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        ELSE
            CALL(evt_sub_intpl_init, EASING_SINE_OUT, -10, 10, 30)
            LOOP(31)
                CALL(evt_sub_intpl_get_value)
                CALL(evt_mapobj_rotate, 1, STRING("hane_1"), 0, 0, LW(0))
                CALL(evt_mapobj_rotate, 1, STRING("hane_2"), 0, 0, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_intpl_init, EASING_SINE_OUT, 10, -10, 30)
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

/**
 * Arrival cutscene when Mario comes from Rogueport by blimp:
 * Glitz Pit music starts, controls are locked, the camera holds on the blimp over the arena and
 * then pulls to tou_01 (Glitzville's entrance square).
 */
EVT_BEGIN(evt_to_tou)
    CALL(evt_cam3d_evt_set, -2000, 241, 534, -2000, 145, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG3_URN1"), 18000)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
#else
    CALL(evt_snd_bgmon_f_d, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG3_URN1"), 18000, 18000)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
#endif
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("gondora_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hako_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("gondora_1"), 0, 80, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_10"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_9"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_8"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_7"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_AIR_SHIP_DRIVE3"), 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 0, 241, 534, 0, 145, -8, 15000, EASING_SINE_IN_OUT)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -25000, 0, 18000)
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
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_4"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_7"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_8"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(800)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_10"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 0, STRING("bou_9"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("gondora_1"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), GW(11))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 80, 0, 3000)
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

/** Loop that glues Mario and his partner to the blimp gondola (gondora_1/gondora_2 map objects) while it moves. */
// The second lookup supplies the position used below; the first lookup is retained.
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
    CALL(evt_cam3d_evt_set, 0, 241, 534, 0, 145, -8, 0, EASING_LINEAR)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("gondora_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hako_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("hikousen"), -30, 20, 0)
    CALL(evt_mapobj_rotate, 1, STRING("hikousen"), 0, 180, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT_GET_ID(PTR(evt_pos_keep), LW(9))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_20"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_19"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_17"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_18"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(1000)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_12"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("bou_14"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("gondora_2"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_GONDOLA_MOVE1"), LW(10), LW(11), LW(12), GW(11))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 80, 3000)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("gondora_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_AIR_SHIP_DRIVE4"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -30, -2000, 20000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("hikousen"), LW(0), 20, 0)
        CALL(evt_mapobj_rotate, 1, STRING("hikousen"), 0, 180, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -2000, 241, 534, -2000, 145, -8, 20000, EASING_SINE_IN_OUT)
    WAIT_MS(8000)
    DELETE_EVENT(GW(15))
    SET_VALUE(GSWF_TOU_BLIMP_TO_ROGUEPORT_UNUSED, 1)
    CALL(evt_bero_mapchange, STRING("gor_04"), STRING("hikousen_return"))
    RETURN
EVT_END()

/** Script helper: args[0] = NPC name, args[1..3] = x, y, z; places the NPC and its previous position there. */
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

/** Names of the five smoke puffs that pour out of Bowser's failing airship in tou_00_koopa_evt. */
const char* kemuri_name[] = {
    "kosyo01",
    "kosyo02",
    "kosyo03",
    "kosyo04",
    "kosyo05",
};

/** Script helper: args[0] = height; sets the y offset of Bowser's flying fortress NPC (the 飛クッパ NPC) to make it bob. */
static USER_FUNC(kpa_hikousen_yure) {
    f32 y = evtGetFloat(event, event->args[0]);

    // 飛クッパ
    npcNameToPtr("\x94\xF2\x83\x4E\x83\x62\x83\x70")->moveOffset.y = y;
    return EVT_RETURN_DONE;
}

/**
 * Chapter 4 interlude (after the Peach/X-Naut report):
 * Bowser's flying fortress passes over Glitzville with the sky scrolling.
 * Bowser talks (kpa_stg4_008, kpa_stg4_009) and the script then changes map to muj_20.
 */
EVT_BEGIN(tou_00_koopa_evt)
    // Mario is hidden, the airship is placed off to the left and the sky starts scrolling past.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_npc_set_position, STRING("飛クッパ"), -2300, -17, 2)
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 90)
    CALL(evt_cam3d_evt_set, -2000, 186, 730, -2000, 55, -8, 0, EASING_SINE_IN_OUT)
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
    // The airship bobs gently while cruising.
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
    // The airship flies in from the left over Glitzville.
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_W_1"))
    CALL(evt_snd_sfxon, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), GW(2))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -2300, -2000, 7500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), LW(3), LW(4))
    CALL(evt_snd_sfx_pos, GW(2), LW(0), LW(3), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    // Something breaks: the music winds down and two smoke puffs stream out behind the ship.
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
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
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
    // The ship lurches and slows down while the engine sound drops in pitch.
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    SET_VALUE(GW(1), LW(3))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 180, 360, 400)
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
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_sfx_pit, GW(9), -1365)
#else
    CALL(evt_snd_sfx_pit, GW(2), -1365)
#endif
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(2), LW(2), 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), GW(1), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    // Bowser looks around, then the ship starts shaking unsteadily.
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
    // Bowser wonders about the warning light while more smoke pours out and the engine dies.
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
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, -1365, -5461, 1500)
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
    // The sky stops scrolling and Bowser panics.
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_1"))
    WAIT_MS(1500)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_VALUE(GF(0), 1)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_2"))
    WAIT_MS(200)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("飛クッパ"), NPC_FLAG_TALK_POSE)
    // Bowser screams, the ship tips over and falls out of the sky.
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("kpa_stg4_009"), 0, STRING("飛クッパ"))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_3"))
    WAIT_MS(500)
    CALL(evt_npc_set_rotate_offset, STRING("飛クッパ"), 0, 30, 0)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, -180, 400)
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
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(3), -200, 800)
    LABEL(6)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(6)
    END_IF
    // Cut to the next scene in muj_20.
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("muj_20"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(hkoopa_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

/** Script helper: args[0] = smoke effect name, args[1] = distance; shifts that smoke puff effect along x each call. */
// Follow the named effect until it disappears; the first call waits without moving it.
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

/** NPCs of this map: Bowser in his airship, who flies over Glitzville in the chapter 4 interlude. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"飛クッパ"*/"\x94\xF2\x83\x4E\x83\x62\x83\x70", // name
        NPC_FLAG_TALK_POSE, // flags
        0, // reactionFlags
        hkoopa_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Lets the player skip the blimp scene: after 1.1 s,
 * A or B ends the scene and changes map (tou_01 normally, or back to gor_04 on the return trip).
 */
EVT_BEGIN(evt_skip)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1100)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), PAD_BUTTON_A | PAD_BUTTON_B)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, GW(11))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("modoru"))
        SET_VALUE(GSWF_TOU_BLIMP_TO_ROGUEPORT_UNUSED, 1)
        CALL(evt_bero_mapchange, STRING("gor_04"), STRING("hikousen_return"))
    ELSE
        CALL(evt_bero_mapchange, STRING("tou_01"), 0)
    END_IF
    RETURN
EVT_END()

/**
 * Map init for tou_00, the blimp landing outside the arena:
 * sets up the NPCs and, normally, the blimp lights, wing and ambient sound.
 * Plays evt_to_gor when entered as "modoru", else evt_to_tou.
 * In Chapter 4 (after the Peach X-Naut PhD report) it runs Bowser's fortress scene instead.
 */
EVT_BEGIN(tou_00_init_evt)
    SET_VALUE(GW(11), 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mario_set_mode, 2)
        SPAWN_EVENT(PTR(tou_00_koopa_evt))
        RETURN
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU8"))
    CALL(evt_map_playanim, STRING("jet"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("lightcoloer"), MAP_ANIM_PLAY_LOOP, 0)
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
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_00_WELCOME_TO_GLITZVILLE)
        SPAWN_EVENT_GET_ID(PTR(evt_skip), GW(15))
    END_IF
    RETURN
EVT_END()

