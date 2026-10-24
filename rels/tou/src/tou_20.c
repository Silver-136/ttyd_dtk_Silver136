#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"


USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_bg_auto_scroll_onoff);
USER_FUNC(evt_bg_set_scrl_offset);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff64);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_dispflg_onoff);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_sfx_pos);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_spline_get_value);
USER_FUNC(evt_sub_spline_init);

f32 fd_tou_20_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tou_20_neg_one_382;
f32 fd_tou_20___local_PI_383;
f32 fd_tou_20___local_3PIO2_385;
f32 fd_tou_20_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tou_20___local_PIO2_384;
f32 fd_tou_20_one_381;
f32 fd_tou_20_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tou_20_neg_one_365;
f32 fd_tou_20___local_PI_366;
f32 fd_tou_20___local_3PIO2_368;
f32 fd_tou_20_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tou_20___local_PIO2_367;
f32 fd_tou_20_one_364;
static EvtWord evt_tou_balloon[85];
EvtWord evt_anim[66];
static f32 spline_dt[9];
EvtWord evt_anim2[147];
EvtWord tou_20_init_evt[223];

f32 fd_tou_20_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tou_20_neg_one_382 = -1.0f;

f32 fd_tou_20___local_PI_383 = 3.1415927f;

f32 fd_tou_20___local_3PIO2_385 = 4.712389f;

f32 fd_tou_20_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tou_20___local_PIO2_384 = 1.5707964f;

f32 fd_tou_20_one_381 = 1.0f;

f32 fd_tou_20_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tou_20_neg_one_365 = -1.0f;

f32 fd_tou_20___local_PI_366 = 3.1415927f;

f32 fd_tou_20___local_3PIO2_368 = 4.712389f;

f32 fd_tou_20_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tou_20___local_PIO2_367 = 1.5707964f;

f32 fd_tou_20_one_364 = 1.0f;

static EVT_BEGIN(evt_tou_balloon)
    CALL(evt_mapobj_get_position, STRING("tou_enkei1"), LW(13), LW(14), LW(15))
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

EVT_BEGIN(evt_anim)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tou_enkei1"), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(0)
            CALL(evt_sub_get_sincos, LW(0), LW(1), LW(2))
            ADD(LW(0), 1)
            MODULO(LW(0), 360)
            MULTIPLY_FLOAT(LW(1), FLOAT(30.0f))
            SET_FLOAT_VALUE(GW(3), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("Hikousen"), 0, GW(3), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -1017, -239, -331, -21, -22, 53, 10000, 11)
    WAIT_MS(10000)
    WAIT_MS(4000)
    CALL(evt_snd_sfxoff, GW(10))
    CALL(evt_snd_sfxoff, GW(11))
    CALL(evt_bero_mapchange, STRING("sys_01"), STRING("stg_3"))
    RETURN
EVT_END()

static f32 spline_dt[] = {
    -808.0f, 971.0f, 4897.0f, -801.0f, 895.0f, 4359.0f, -744.0f, 880.0f,
    3982.0f,
};

EVT_BEGIN(evt_anim2)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Hikousen"), 1)
    CALL(evt_mapobj_trans, 1, STRING("Hikousen"), FLOAT(-808.0f), FLOAT(971.0f), FLOAT(4897.0f))
    WAIT_MS(2000)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Hikousen"), 1)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(3), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        WAIT_MS(20000)
        CALL(evt_snd_sfxoff, GW(10))
        CALL(evt_snd_sfxoff, GW(11))
        CALL(evt_bero_mapchange, STRING("tou_00"), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(0)
            CALL(evt_sub_get_sincos, LW(0), LW(1), LW(2))
            ADD(LW(0), 1)
            MODULO(LW(0), 360)
            IF_INT_EQUAL(GF(0), 0)
                MULTIPLY_FLOAT(LW(1), FLOAT(50.0f))
                ELSE
                MULTIPLY_FLOAT(LW(1), FLOAT(40.0f))
            END_IF
            SET_FLOAT_VALUE(GW(3), LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_spline_init, 1, 11, PTR(spline_dt), 3, 0, 10000)
    SET_VALUE(LW(3), 1)
    LOOP(0)
        IF_INT_EQUAL(LW(3), 1)
            CALL(evt_sub_spline_get_value, GW(0), GW(1), GW(2), LW(3))
            ELSE
        END_IF
        SET_FLOAT_VALUE(LW(4), GW(1))
        ADD_FLOAT(LW(4), GW(3))
        CALL(evt_mapobj_trans, 1, STRING("Hikousen"), GW(0), LW(4), GW(2))
        CALL(evt_mapobj_rotate, 1, STRING("Hikousen"), -10, 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("Hikousen"), 0, -100, 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(tou_20_init_evt)
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("naka"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 36)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 30)
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_mario_adjust_dir)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_party_set_dir, 0, 90, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("lightcoloer"), 1, 0)
    CALL(evt_map_playanim, STRING("jet"), 1, 0)
    CALL(evt_map_playanim, STRING("omori"), 1, 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_rotate, 1, STRING("puro_2"), LW(0), 0, 0)
            ADD(LW(0), 20)
            MODULO(LW(0), 360)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_rotate, 1, STRING("puro_3"), LW(0), 0, 0)
            ADD(LW(0), 20)
            MODULO(LW(0), 360)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("start"))
        CALL(evt_cam3d_evt_set, -3020, 129, -6, -212, 722, 807, 0, 11)
        SPAWN_EVENT(PTR(evt_anim))
        ELSE
        CALL(evt_cam3d_evt_set, -700, 866, 4657, -700, 38, -60, 0, 11)
        SPAWN_EVENT(PTR(evt_anim2))
    END_IF
    CALL(evt_bg_auto_scroll_onoff, 1)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        LOOP(0)
            ADD_FLOAT(LW(0), FLOAT(0.001953125f))
            CALL(evt_bg_set_scrl_offset, LW(0), LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(11), 0)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(evt_mapobj_get_position, STRING("Hikousen"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_DRIVE1"), LW(0), LW(1), LW(2), GW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIR_SHIP_DRIVE1_2"), LW(0), LW(1), LW(2), GW(11))
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("Hikousen"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, GW(10), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, GW(11), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

