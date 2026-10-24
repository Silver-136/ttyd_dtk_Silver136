#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_memcard.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"

#pragma force_active on

USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_set_mode);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_kamek_kemuri2);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_set_scale);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfx_pos);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
#ifdef VERSION_G8ME01
#endif

f32 fd_kpa_00_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_kpa_00_neg_one_382;
f32 fd_kpa_00___local_PI_383;
f32 fd_kpa_00___local_3PIO2_385;
f32 fd_kpa_00_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_kpa_00___local_PIO2_384;
f32 fd_kpa_00_one_381;
f32 fd_kpa_00_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_kpa_00_neg_one_365;
f32 fd_kpa_00___local_PI_366;
f32 fd_kpa_00___local_3PIO2_368;
f32 fd_kpa_00_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_kpa_00___local_PIO2_367;
f32 fd_kpa_00_one_364;
EvtWord kpa_00_first_evt[61];
EvtWord kpa_00_fly_evt[380];
static NpcSetupInfo npcEnt[3];
EvtWord kpa_00_init_evt[24];

f32 fd_kpa_00_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_kpa_00_neg_one_382 = -1.0f;

f32 fd_kpa_00___local_PI_383 = 3.1415927f;

f32 fd_kpa_00___local_3PIO2_385 = 4.712389f;

f32 fd_kpa_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_kpa_00___local_PIO2_384 = 1.5707964f;

f32 fd_kpa_00_one_381 = 1.0f;

f32 fd_kpa_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_kpa_00_neg_one_365 = -1.0f;

f32 fd_kpa_00___local_PI_366 = 3.1415927f;

f32 fd_kpa_00___local_3PIO2_368 = 4.712389f;

f32 fd_kpa_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_kpa_00___local_PIO2_367 = 1.5707964f;

f32 fd_kpa_00_one_364 = 1.0f;

EVT_BEGIN(kpa_00_first_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_set_pos, -1000, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 292, 534, 0, 203, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STGK_CASTLE1"))
        CALL(evt_snd_bgm_scope, 0, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 134, 534, 0, 45, -8, 5000, 11)
    WAIT_MS(4500)
    WAIT_MS(500)
    WAIT_MS(1000)
    CALL(evt_bero_mapchange, STRING("kpa_01"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(kpa_00_fly_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 262144)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("飛クッパ"), 0, 30, -15)
    CALL(evt_npc_set_scale, STRING("飛クッパ"), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 90)
    CALL(evt_npc_set_position, STRING("ババ"), 10, 20, 0)
    CALL(evt_npc_set_scale, STRING("ババ"), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_cam3d_evt_set, 0, 134, 534, 0, 45, -8, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(500)
    SET_FLOAT_VALUE(GW(0), FLOAT(-1.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.25f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 180, 0, 3000)
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
        CALL(evt_sub_intpl_msec_init, 11, 90, 0, 3000)
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
        CALL(evt_sub_intpl_msec_init, 11, 100, 600, 2000)
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
        CALL(evt_sub_intpl_msec_init, 11, 100, 600, 2000)
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
        CALL(evt_sub_intpl_msec_init, 11, 10, 190, 3000)
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
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), 0, 0, 0, LW(3))
    SET_VALUE(LW(4), -1000)
    CALL(evt_sub_intpl_msec_init, 11, 0, 210, 3000)
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
    SET_VALUE(GSW(0), 61)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_snd_envoff_f, 512, 1000)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 3)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, 59, 0, 58, 0)
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("nokotarou"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        0x20, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"飛クッパ"*/"\x94\xF2\x83\x4E\x83\x62\x83\x70", // name
        0x20, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(kpa_00_init_evt)
    CALL(evt_mario_set_mode, 2)
    CALL(evt_snd_envon, 272, STRING("ENV_KOOPA_KPA1"))
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW(0))
        CASE_EQUAL(59)
        SPAWN_EVENT(kpa_00_first_evt)
        CASE_EQUAL(60)
        SPAWN_EVENT(kpa_00_fly_evt)
    END_SWITCH
    RETURN
EVT_END_PAD()

