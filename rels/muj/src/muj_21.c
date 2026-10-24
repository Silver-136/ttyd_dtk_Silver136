#include "battle/battle_unit.h"
#include "eff/eff_funemizu.h"
#include "evt/evt_cmd.h"

#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "muj.h"
#include "evt/evt_npc.h"

USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_light_charalight_all_onoff);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_flag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_wait_pera);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfx_vol);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
static USER_FUNC(fune_eff);

f32 fd_muj_21_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_muj_21_neg_one_382;
f32 fd_muj_21___local_PI_383;
f32 fd_muj_21___local_3PIO2_385;
f32 fd_muj_21_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_muj_21___local_PIO2_384;
f32 fd_muj_21_one_381;
f32 fd_muj_21_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_muj_21_neg_one_365;
f32 fd_muj_21___local_PI_366;
f32 fd_muj_21___local_3PIO2_368;
f32 fd_muj_21_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_muj_21___local_PIO2_367;
f32 fd_muj_21_one_364;
EvtWord peach_init[12];
static NpcSetupInfo npcEnt[2];
EvtWord fune_yure[42];
static EvtWord epilogue_evt[344];
EvtWord muj_21_init_evt[58];

f32 fd_muj_21_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_muj_21_neg_one_382 = -1.0f;

f32 fd_muj_21___local_PI_383 = 3.1415927f;

f32 fd_muj_21___local_3PIO2_385 = 4.712389f;

f32 fd_muj_21_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_muj_21___local_PIO2_384 = 1.5707964f;

f32 fd_muj_21_one_381 = 1.0f;

f32 fd_muj_21_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_muj_21_neg_one_365 = -1.0f;

f32 fd_muj_21___local_PI_366 = 3.1415927f;

f32 fd_muj_21___local_3PIO2_368 = 4.712389f;

f32 fd_muj_21_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_muj_21___local_PIO2_367 = 1.5707964f;

f32 fd_muj_21_one_364 = 1.0f;

EVT_BEGIN(peach_init)
    CALL(evt_npc_set_position, STRING("me"), 62, 28, 44)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50", // name
        0x80600, // flags
        0, // reactionFlags
        peach_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(fune_yure)
    CALL(evt_hit_bind_mapobj, STRING("A_fune_m"), STRING("S_fune_m"))
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_sub_get_sincos, LW(0), LW(1), LW(2))
        MULTIPLY_FLOAT(LW(1), FLOAT(0.7998047f))
        CALL(evt_mapobj_rotate, 1, STRING("S_fune_m"), 0, 0, LW(1))
        CALL(evt_hit_bind_update, STRING("A_fune_m"))
        WAIT_FRAMES(1)
        ADD_FLOAT(LW(0), FLOAT(2.0f))
        IF_INT_GREATER_OR_EQUAL(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_ENDING5"), 800)
    CALL(evt_snd_envon, 272, STRING("ENV_END_MUJ2"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 40, 28, 64)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_flag_onoff, 1, 1024)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), 163856)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_SHIP_MOVE1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_vol, LW(3), 63)
    CALL(evt_cam3d_evt_set, 44, 173, 585, 44, 79, 50, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_7"))
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), 32768)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_7"), STRING("P_T_13"))
    CALL(evt_msg_print, 0, STRING("ep_27"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ピーチ姫"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_28"), 0, STRING("ピーチ姫"))
    CALL(evt_cam3d_evt_set, 44, 137, 380, 44, 79, 50, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_29"), 0, STRING("ピーチ姫"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ピーチ姫"))
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ピーチ姫"), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_5"))
    CALL(evt_map_playanim, STRING("ake"), 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_snd_bgmoff_f, 512, 3000)
        WAIT_MS(1500)
        CALL(evt_snd_bgmon_f, 768, STRING("BGM_ENDING3"), 2000)
    END_INLINE_EVENT
    WAIT_MS(1500)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_color, 1, STRING("S_nami_kira"), 255, 255, 255, 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_nami_kira"), 1)
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S_nami_kira"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1300)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sun_hika"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_color, 1, STRING("S_sun_hika"), 255, 255, 255, 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sun_hika"), 1)
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S_sun_hika"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_map_checkanim, STRING("ake"), LW(0), LW(1))
    SUBTRACT(LW(1), 8000)
    WAIT_MS(LW(1))
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_5"), STRING("P_T_5"))
    CALL(evt_msg_print, 0, STRING("ep_30"), 0, STRING("ピーチ姫"))
    CALL(evt_fade_set_mapchange_type, 1, 10, 1000, 9, 1000)
    CALL(evt_bero_mapchange, STRING("gor_12"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(muj_21_init_evt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("hikari"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_nami_kira"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sun_hika"), 1)
    BEGIN_INLINE_EVENT
        CALL(fune_eff)
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(10), FLOAT(1.0f))
    SPAWN_EVENT(PTR(anm_kaji))
    SPAWN_EVENT(PTR(anm_wakka))
    SPAWN_EVENT(PTR(anm_entotsu))
    SPAWN_EVENT(PTR(anm_neji))
    SPAWN_EVENT(PTR(fune_yure))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_f_wa"), 2)
    CALL(evt_mario_kill_party, 0)
    SPAWN_EVENT(PTR(epilogue_evt))
    CALL(evt_light_charalight_all_onoff, 1)
    RETURN
EVT_END()

EffectEntry* effFunemizuEntry(s32 type, f32 x, f32 y, f32 z, f32 size, f32 yaw);
EffectEntry* effWaterDamageN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale);
void mapObjGetPos(const char* name, Vec* pos);

static USER_FUNC(fune_eff) {
    EffectEntry* eff1;
    EffectEntry* eff2;
    Vec pos;

    if (isFirstCall) {
        event->userdatap[0] = effFunemizuEntry(0, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        event->userdatap[1] = effFunemizuEntry(2, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        mapObjGetPos("S_fune_m", &pos);
        event->userdataf[2] = pos.x;
        event->userdata3f = pos.z;
    }
    eff1 = event->userdatap[0];
    eff2 = event->userdatap[1];
    mapObjGetPos("S_fune_m", &pos);
    event->userdataf[2] = pos.x;
    event->userdata3f = pos.z;
    ((EffFunemizuWork*)eff1->userdata)->scale = 3.0f;
    ((EffFunemizuWork*)eff1->userdata)->position.x = pos.x - 81.3f;
    ((EffFunemizuWork*)eff1->userdata)->position.y = pos.y - 51.0f;
    ((EffFunemizuWork*)eff1->userdata)->position.z = pos.z - 38.9f;
    ((EffFunemizuWork*)eff1->userdata)->f10 = 1.0f;
    ((EffFunemizuWork*)eff1->userdata)->f14 = 270.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.x = pos.x - 49.5f;
    ((EffFunemizuWork*)eff2->userdata)->position.y = pos.y - 51.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.z = 70.7f + pos.z;
    ((EffFunemizuWork*)eff2->userdata)->f10 = 1.0f;
    ((EffFunemizuWork*)eff2->userdata)->f14 = 305.0f;
    if (gp->retraceCount % 15 == 0) {
        effWaterDamageN64Entry(5, 30, 106.1f + pos.x, pos.y - 50.0f, 106.1f + pos.z, 2.0f);
    }
    return EVT_RETURN_BLOCK;
}

