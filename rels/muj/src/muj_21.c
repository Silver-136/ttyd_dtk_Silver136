// muj_21 (無人島, Keelhaul Key): an ending scene on the ship at dawn with Peach and Mario
// (BGM_ENDING5, ep_27 to ep_30) after leaving Rogueport (from gor_00), then on to the sea scene gor_12.

#include "version.h"
#include "battle/battle_unit.h"
#include "eff/eff_funemizu.h"
#include "evt/evt_cmd.h"
#include "eff/n64/eff_water_damage_n64.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_light.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "pmario_sound.h"

#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "muj.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

static USER_FUNC(fune_eff);

EvtWord peach_init[12];
static NpcSetupInfo npcEnt[2];
EvtWord fune_yure[42];
static EvtWord epilogue_evt[344];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord muj_21_init_evt[];
#else
EvtWord muj_21_init_evt[58];
#endif

/** Init event of Princess Peach in the ending scene: places her at (62, 28, 44) on the deck facing right. */
EVT_BEGIN(peach_init)
    CALL(evt_npc_set_position, STRING("me"), 62, 28, 44)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

/** NPCs of the ending ship scene: Peach. */
static NpcSetupInfo npcEnt[] = {
    {
        "ピーチ姫", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        peach_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Rocks the ship S_fune_m (and its hit box A_fune_m) by a sine wave on its roll,
 * advancing the phase 2 degrees per frame, forever.
 */
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

/**
 * The ending on the ship as it sails away from Rogueport ("Sail Away"):
 * BGM_ENDING5 plays while Princess Peach talks with Mario on the deck about the people of Rogueport
 * and thanks him (ep_27 to ep_29).
 * Then the dawn animation (ake) and sunlight play, the music changes to BGM_ENDING3,
 * the sea and sun glints fade in, and Peach points out Rogueport shining in the sunrise (ep_30)
 * before the map fades to gor_12.
 */
static EVT_BEGIN(epilogue_evt)
    // The ending voyage: the music starts and Mario and Peach stand on the ship's deck.
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_ENDING5"), 800)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ2"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 40, 28, 64)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_flag_onoff, 1, 1024)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_SHIP_MOVE1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_vol, LW(3), 63)
    CALL(evt_cam3d_evt_set, 44, 173, 585, 44, 79, 50, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_7"))
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_7"), STRING("P_T_13"))

    // Peach tells Mario how lovely the people of Rogueport are, despite her unhappy memories of being kidnapped there.
    CALL(evt_msg_print, 0, STRING("ep_27"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ピーチ姫"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_28"), 0, STRING("ピーチ姫"))
    CALL(evt_cam3d_evt_set, 44, 137, 380, 44, 79, 50, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // Peach remembers how frightened she was when taken captive.
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ピーチ姫"), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_5"))
    CALL(evt_map_playanim, STRING("ake"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)

        // The sun comes up and the music changes.
        CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
        WAIT_MS(1500)
        CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_ENDING3"), 2000)
    END_INLINE_EVENT
    WAIT_MS(1500)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_color, 1, STRING("S_nami_kira"), 255, 255, 255, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_nami_kira"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1000)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_sun_hika"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_color, 1, STRING("S_sun_hika"), 255, 255, 255, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_sun_hika"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1000)
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

    // Peach points out Rogueport shining and the map fades to the sea scene gor_12.
    CALL(evt_msg_print, 0, STRING("ep_30"), 0, STRING("ピーチ姫"))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 1000, FADE_IN_BLACK, 1000)
    CALL(evt_bero_mapchange, STRING("gor_12"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init for muj_21 (the ending ship at dawn):
 * sets up Peach, the wave and sunlight animations, the wake effect and the ship's spinning parts,
 * removes the partner and starts the epilogue conversation (ep_27 to ep_30) that ends by sailing to gor_12.
 */
EVT_BEGIN(muj_21_init_evt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hikari"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_nami_kira"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sun_hika"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(fune_eff)
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(10), FLOAT(1.0f))
    SPAWN_EVENT(PTR(anm_kaji))
    SPAWN_EVENT(PTR(anm_wakka))
    SPAWN_EVENT(PTR(anm_entotsu))
    SPAWN_EVENT(PTR(anm_neji))
    SPAWN_EVENT(PTR(fune_yure))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_f_wa"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mario_kill_party, 0)
#if VERSION <= VERSION_DEMO_JP
    SPAWN_EVENT(epilogue_evt)
#else
    SPAWN_EVENT(PTR(epilogue_evt))
    CALL(evt_light_charalight_all_onoff, 1)
#endif
    RETURN
EVT_END()


/**
 * Script helper, one per frame: keeps two wake effects (funemizu) beside the ship S_fune_m
 * so water churns behind it. Always blocks.
 */
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
    ((EffFunemizuWork*)eff1->userdata)->size = 1.0f;
    ((EffFunemizuWork*)eff1->userdata)->yaw = 270.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.x = pos.x - 49.5f;
    ((EffFunemizuWork*)eff2->userdata)->position.y = pos.y - 51.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.z = 70.7f + pos.z;
    ((EffFunemizuWork*)eff2->userdata)->size = 1.0f;
    ((EffFunemizuWork*)eff2->userdata)->yaw = 305.0f;
    // Splash timing follows the global retrace clock, not time since this event started.
    if (gp->retraceCount % 15 == 0) {
        effWaterDamageN64Entry(5, 30, 106.1f + pos.x, pos.y - 50.0f, 106.1f + pos.z, 2.0f);
    }
    return EVT_RETURN_BLOCK;
}

