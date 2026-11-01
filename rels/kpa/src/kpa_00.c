// kpa_00 (クッパ編, Bowser's interludes): a Bowser's Castle scene with Kammy Koopa; continues in kpa_01 and ends in
// Petalburg (nok_01).

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_memcard.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"


EvtWord kpa_00_first_evt[61];
EvtWord kpa_00_fly_evt[380];
static NpcSetupInfo npcEnt[3];
EvtWord kpa_00_init_evt[24];

/**
 * Opening cutscene of the Bowser interludes: the camera pans across Bowser's Castle to a BGM_STGK_CASTLE1 start,
 * then the game moves on to kpa_01.
 * Runs at the start of the first interlude (story progress below STG1_19_01_BOWSER_BOWSER_CASTLE).
 */
EVT_BEGIN(kpa_00_first_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_set_pos, -1000, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 292, 534, 0, 203, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STGK_CASTLE1"))
        CALL(evt_snd_bgm_scope, 0, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 134, 534, 0, 45, -8, 5000, EASING_SINE_IN_OUT)
    WAIT_MS(4500)
    WAIT_MS(500)
    WAIT_MS(1000)
    CALL(evt_bero_mapchange, STRING("kpa_01"), STRING(""))
    RETURN
EVT_END()

/**
 * Closing cutscene of the first interlude: the flying Bowser NPC (飛クッパ) bobs up and away and grows from 0.1 to
 * 0.6 scale, with the airship engine sound (SFX_KUPPA_AIRSHIP_MOVE1) following its position,
 * while Kammy Koopa flies alongside with her broom sound and a smoke trail.
 * The story advances to STG1_19_02_BOWSER_CASTLE_INT, the game is saved,
 * and play returns to Petalburg (nok_01).
 */
EVT_BEGIN(kpa_00_fly_evt)
    // Cutscene start: controls are off and Mario is moved out of sight; Bowser's airship and Kammy are placed
    // small and far away.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_SHADOW)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("飛クッパ"), 0, 30, -15)
    CALL(evt_npc_set_scale, STRING("飛クッパ"), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 90)
    CALL(evt_npc_set_position, STRING("ババ"), 10, 20, 0)
    CALL(evt_npc_set_scale, STRING("ババ"), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_cam3d_evt_set, 0, 134, 534, 0, 45, -8, 0, EASING_SINE_IN_OUT)

    // The camera holds on the sky while the airship and Kammy's broom swing in sine-wave arcs and grow as they
    // approach.
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    SET_FLOAT_VALUE(GW(0), FLOAT(-1.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.25f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 3000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(0), LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 90, 0, 3000)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(2), LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 100, 600, 2000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("飛クッパ"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 100, 600, 2000)
        LABEL(5)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("ババ"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(5)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)

        // Kammy puffs her magic smoke and her flying sound loops until she reaches her mark.
        CALL(evt_npc_kamek_kemuri2, STRING("ババ"), 2000, 1)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        SET_VALUE(LW(4), -1000)
        LOOP(0)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            ADD(LW(4), 30)
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(4), LW(3))
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 10, 190, 3000)
        LABEL(4)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(LW(2), GW(2))
        MULTIPLY_FLOAT(LW(2), FLOAT(60.0f))
        ADD_FLOAT(LW(2), FLOAT(20.0f))
        CALL(evt_npc_set_position, STRING("ババ"), LW(0), LW(2), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(4)
        END_IF
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT

    // The airship flies across the screen with its engine sound following it.
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), 0, 0, 0, LW(3))
    SET_VALUE(LW(4), -1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 210, 3000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    SET_FLOAT_VALUE(LW(2), GW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(40.0f))
    ADD_FLOAT(LW(2), FLOAT(70.0f))
    ADD(LW(4), 3)
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), LW(2), -15)
    CALL(evt_snd_sfx_pos, LW(3), LW(0), LW(2), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(3))
    WAIT_MS(800)

    // The story advances to Bowser's castle interior; the music fades, the screen closes to a spot, and the
    // game is saved.
    SET_VALUE(GSW_STORY_PROGRESS, STG1_19_02_BOWSER_CASTLE_INT)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 3)
#if VERSION >= VERSION_US
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_BOWSER_A1, 0, FADE_STORY_BOWSER_A3, 0)

    // The scene ends in Petalburg (nok_01).
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("nokotarou"))
    RETURN
EVT_END()

/**
 * NPCs of the airship scene: Kammy Koopa (ババ) and Bowser's flying airship (飛クッパ), both created by
 * npc_init_evt without battle or talk.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "ババ", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "飛クッパ", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Map init for kpa_00 (Bowser's Castle exterior).
 * Sets up the two NPCs, Kammy and the flying Bowser,
 * and starts the opening cutscene or the flying cutscene depending on the story progress.
 */
EVT_BEGIN(kpa_00_init_evt)
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_KOOPA_KPA1"))
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG1_19_PEACH_TEC)
        SPAWN_EVENT(kpa_00_first_evt)
        CASE_EQUAL(STG1_19_01_BOWSER_BOWSER_CASTLE)
        SPAWN_EVENT(kpa_00_fly_evt)
    END_SWITCH
    RETURN
EVT_END_PAD()

