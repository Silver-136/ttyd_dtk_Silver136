// yuu_01 (遊技場, Pianta Parlor): minigame 2, Boat Mode (fune); entered from gor_03.

#include "version.h"
#include "evt/evt_yuuranking.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "party.h"
#include "mario/mario_motion.h"
#include "cam_shift.h"
#include "evt/evt_map.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_sub.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mot/mot_ship.h"
#include "evt/evt_yuugijou.h"
#include "driver/mapdrv.h"
#include "rels/yuu/include/yuu.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "icon_ids.h"
#include "evt/evt_movefloor.h"
#include "yuu.h"
#include "driver/seqdrv.h"
#include <dolphin/pad.h>

typedef struct FuneWork {
    u32 flags;
    s32 phase;
    s32 coins;
    s32 result;
    s32 index;
    s32 state;
    f32 dist;
    Vec dir;
    s32 timer;
    f32 y;
    f32 x;
    s32 monteCount;
    EventEntry* viewEvent;
} FuneWork;
static void* yuunpc_wp;
static Monte* monte_p;
static void monte_disp(CameraId camera, void* param);

/**
 * Returns TRUE when two points are within 20 units in 3D and within 20 in height,
 * i.e. the boat is touching a floating coin.
 */
static inline BOOL monte_check_inl(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Returns a random float in [0, d) with two decimal digits of resolution,
 * used to scatter coins inside a montetable rectangle.
 */
static inline f32 randRange(f32 d) { return irand(100.0f * d) / 100.0f; }

static USER_FUNC(fall_rtn);
static USER_FUNC(re_fall_rtn);
USER_FUNC(yuu_01_cam_shift_reset);
static USER_FUNC(yuuminigame2_get_name);
static USER_FUNC(yuuminigame2_opening_chk);
static USER_FUNC(yuuminigame2_opening_on_off);
static USER_FUNC(yuuminigame2_set_phase);
static BOOL get_monte_check(Vec* a, Vec* b);

static const char* s_nametbl[21];
static const char* a2_nametbl[21];
static const char* a_nametbl[21];
static s32 turn_tbl[22];
static s32 hoko_tbl[22];
static MonteTable montetable[6];
static EvtWord minigame_view_monte[4];
EvtWord yuu_01_kaijo_init[12];
EvtWord yuu_01_kaijo_init2[12];
static EvtWord minigame02_main[37];
static EvtWord minigame02_start_cam[40];
static EvtWord minigame02_start_cam_cancel[17];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord minigame02_start[];
extern EvtWord minigame02_end[];
#else
EvtWord minigame02_start[101];
EvtWord minigame02_end[172];
#endif

EvtWord yuu_01_init_evt[VERSION_PICK(1076, 1166)];

/**
 * Map object names of the Boat Mode hazards by index: 1-4 the big whirlpools, 5-12 the small whirlpools
 * and 13-20 the barrels (index 0 is unused). a2_nametbl and a_nametbl below list their hit objects.
 */
static const char* s_nametbl[] = {
    NULL, "S_dai_uzu1", "S_dai_uzu2", "S_dai_uzu3", "S_dai_uzu4", "S_uzu1", "S_uzu2", "S_uzu3", "S_uzu4", "S_uzu5",
    "S_uzu6", "S_uzu7", "S_uzu8", "S_taru01", "S_taru02", "S_taru03", "S_taru04", "S_taru05", "S_taru06",
    "S_taru07", "S_taru08",
};

/** Unused: a second list of Boat Mode hazard collision names (map polygons for the whirlpools). */
static const char* a2_nametbl[] = {
    NULL,
    "polySurface439_q00000",
    "polySurface439_q00001",
    "polySurface439_q00002",
    "polySurface439_q00003",
    "polySurface439_q00008",
    "polySurface439_q00010",
    "polySurface439_q00012",
    "polySurface439_q00014",
    "polySurface439_q00016",
    "polySurface439_q00018",
    "polySurface439_q00020",
    "polySurface439_q00022",
    "A_taru01",
    "A_taru02",
    "A_taru03",
    "A_taru04",
    "A_taru05",
    "A_taru06",
    "A_taru07",
    "A_taru08",
};

/** Hit object names of the Boat Mode hazards, indexed like s_nametbl; yuuminigame2_get_name returns them. */
static const char* a_nametbl[] = {
    NULL, "A_dai_uzu1", "A_dai_uzu2", "A_dai_uzu3", "A_dai_uzu4", "A_uzu1", "A_uzu2", "A_uzu3", "A_uzu4", "A_uzu5",
    "A_uzu6", "A_uzu7", "A_uzu8", "A_taru01", "A_taru02", "A_taru03", "A_taru04", "A_taru05", "A_taru06",
    "A_taru07", "A_taru08",
};

/**
 * Degrees per frame the boat turns while caught by each Boat Mode hazard (same indices as s_nametbl):
 * 5 for the big whirlpools and 10 for the small ones, alternating direction; the barrels do not turn it.
 */
static s32 turn_tbl[] = {
    0, 5, -5, 5, -5, 10, -10, 10,
    -10, 10, -10, 10, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};

/**
 * Direction (degrees, give or take 45 at random) each small whirlpool (indices 5-12) flings the boat;
 * yuuminigame2_gamerule passes it to marioSetSwirlPower.
 */
static s32 hoko_tbl[] = {
    0, 0, 0, 0, 0, 45, 135, 45,
    135, 45, 45, 135, 180, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};

/** Where fall_rtn scatters the Boat Mode coins: five spots, one coin each (x range, then z range). */
static MonteTable montetable[] = {
    {0.0f, 0.0f, 50.0f, 340.0f, 1},
    {250.0f, 250.0f, 50.0f, 340.0f, 1},
    {450.0f, 730.0f, -330.0f, 50.0f, 1},
    {-155.0f, -155.0f, -330.0f, -60.0f, 1},
    {150.0f, -155.0f, -330.0f, -60.0f, 1},
    {0},
};

/** The Boat Mode game's coin event (data->viewEvent): runs fall_rtn, which scatters and collects the coins. */
static EVT_BEGIN(minigame_view_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

/** Puts Mario on the Boat Mode starting spot (x -800, z 200) facing right. */
EVT_BEGIN(yuu_01_kaijo_init)
    CALL(evt_mario_set_pos, -800, 0, 200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

/**
 * Puts Mario back on the start spot after the opening camera or between rounds (x -800 in retail,
 * -670 in both demos, z 200).
 */
EVT_BEGIN(yuu_01_kaijo_init2)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_mario_set_pos, -670, 0, 200)
#else
    CALL(evt_mario_set_pos, -800, 0, 200)
#endif
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

/**
 * Set-up for minigame 2 (Boat Mode): freezes Mario's controls, allocates the minigame work area,
 * dismisses the partners, starts the crowd and boat-water sounds and sets phase 1.
 */
static EVT_BEGIN(minigame02_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(yuu_01_kaijo_init)
    CALL(evt_yuuminigame_alloc, LW(0), 1)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_BOAT_WATER1"), GW(11))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame2_set_phase, LW(0), 1)
    RETURN
EVT_END()

/**
 * Opening cutscene of Boat Mode: pans the 3D camera along the river,
 * then shows the "menu_title_minigame2" title card.
 */
static EVT_BEGIN(minigame02_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-560.0f), FLOAT(565.0f), FLOAT(1185.0f), FLOAT(-560.0f), FLOAT(75.0f), FLOAT(155.0f), 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, SEQ_GAME)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_cam3d_evt_set, FLOAT(550.0f), FLOAT(565.0f), FLOAT(1185.0f), FLOAT(550.0f), FLOAT(75.0f), FLOAT(155.0f), 3000, EASING_LINEAR)
#else
    CALL(evt_cam3d_evt_set, FLOAT(550.0f), FLOAT(565.0f), FLOAT(1185.0f), FLOAT(550.0f), FLOAT(75.0f), FLOAT(155.0f), 4000, EASING_LINEAR)
#endif
    WAIT_MS(500)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame2"))
#if VERSION <= VERSION_DEMO_JP
    WAIT_MS(2000)
#else
    WAIT_MS(3000)
#endif
    CALL(evt_yuutitle_free)
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * Cleans up when the player skips the opening camera:
 * frees the title card, turns off the event camera and puts Mario at the start spot.
 */
static EVT_BEGIN(minigame02_start_cam_cancel)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL_CHILD(yuu_01_kaijo_init2)
    RETURN
EVT_END()

/**
 * Phase 1 of Boat Mode: plays the opening camera (skippable), starts the minigame music,
 * shows the countdown effect, gives Mario control and starts the 120-second countdown timer.
 */
EVT_BEGIN(minigame02_start)
    CALL(yuuminigame2_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame02_start_cam), LW(8))
        LOOP(0)
            IS_EVENT_RUNNING(LW(8), LW(9))
            IF_INT_EQUAL(LW(9), 0)
                CALL(evt_seq_wait, SEQ_GAME)
                BREAK_LOOP
            ELSE
                CALL(evt_key_get_buttontrg, 0, LW(9))
                IF_BITS_SET(LW(9), PAD_BUTTON_A | PAD_BUTTON_B)
                    DELETE_EVENT(LW(8))
                    CALL(evt_seq_wait, SEQ_GAME)
                    CALL_CHILD(minigame02_start_cam_cancel)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame2_opening_on_off, LW(0), 0)
        CALL_CHILD(yuu_01_kaijo_init2)
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_hitobj_onoff, STRING("taisaku"), 0, 0)
    CALL(evt_mario_mov_pos2, -660, 200, 120)
    CALL(evt_hitobj_onoff, STRING("taisaku"), 0, 1)
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_MINIGAME1"))
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)
#if VERSION >= VERSION_JP
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_yuucounter_alloc, 0, 70, 1, 120)
#endif
    CALL(evt_mario_key_onoff, 1)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_yuucounter_alloc, 0, 70, 1, 120)
#endif
    RETURN
EVT_END()

EVT_BEGIN(minigame02_end)
#if VERSION >= VERSION_JP
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
#endif
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
    IF_INT_LESS(LW(0), 0)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        WAIT_MS(4000)
    ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(4000)
    END_IF
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 3, 0, LW(0), LW(2), LW(1), 120)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 3, 1, LW(0), LW(2), LW(1), 120)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
    ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_01_00"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF_GOR_RETURNING_FROM_MINIGAME, 1)
        CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
        CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
        RETURN
    END_IF
    MULTIPLY(LW(1), -1)
    CALL(yuugijou_add_montemedal, 0, LW(1))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(yuugijou_montemedal_disp_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_sfxoff, GW(10))
    CALL(evt_snd_sfxoff, GW(11))
    CALL(evt_fade_out, 1000)
    CALL(re_fall_rtn)
#if VERSION <= VERSION_DEMO_JP
    CALL_CHILD(yuu_01_kaijo_init2)
#else
    CALL_CHILD(yuu_01_kaijo_init2)
    CALL(yuu_01_cam_shift_reset)
#endif
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_BOAT_WATER1"), GW(11))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

/**
 * Map init for yuu_01 (Boat Mode room): starts the water, cloud and neon animations,
 * spawns the whirlpool and barrel moving floors, starts the minigame set-up and the NPC script,
 * and (US and later) hides all but the current game-language sign graphics.
 */
EVT_BEGIN(yuu_01_init_evt)
    // The room's lights, clouds and water start animating.
    CALL(evt_map_playanim, STRING("den1"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("den2"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("kumo"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("wakka"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("nagare"), MAP_ANIM_PLAY_LOOP, 0)

    // The four big whirlpools (S_dai_uzu1-4) have version-dependent paths and travel times.
    CALL(yuuminigame2_get_name, 1, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -140)
    SET_VALUE(LW(3), 140)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(4), 3500)
    SET_VALUE(LW(5), 200)
#else
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 100)
#endif
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 1, LW(15))
    CALL(yuuminigame2_get_name, 2, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 20)
    SET_VALUE(LW(4), 800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), -30)
    SET_VALUE(LW(9), 50)
    SET_VALUE(LW(10), 3200)
#else
    SET_VALUE(LW(2), -220)
    SET_VALUE(LW(3), 50)
    SET_VALUE(LW(4), 3500)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
#endif
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 2, LW(15))
    CALL(yuuminigame2_get_name, 3, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), -20)
#else
    SET_VALUE(LW(2), 20)
    SET_VALUE(LW(3), -60)
#endif
    SET_VALUE(LW(4), 800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), 0)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(8), 40)
    SET_VALUE(LW(9), -40)
    SET_VALUE(LW(10), 3200)
#else
    SET_VALUE(LW(8), 115)
    SET_VALUE(LW(9), -40)
    SET_VALUE(LW(10), 1600)
#endif
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 3, LW(15))

    // Big whirlpool 4 stays in place in the early demos; later versions slide it a short way.
    CALL(yuuminigame2_get_name, 4, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 5)
#else
    SET_VALUE(LW(2), -50)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 800)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
#endif
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 4, LW(15))

    // The eight small whirlpools (S_uzu1-8), which spin the boat around, slide back and forth.
    CALL(yuuminigame2_get_name, 5, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
    SET_VALUE(LW(3), 75)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(4), 2000)
#else
    SET_VALUE(LW(4), 1500)
#endif
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 5, LW(15))
    CALL(yuuminigame2_get_name, 6, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), -75)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(4), 2000)
#else
    SET_VALUE(LW(4), 1500)
#endif
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 6, LW(15))
    CALL(yuuminigame2_get_name, 7, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 3000)
#else
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 1500)
#endif
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 7, LW(15))
    CALL(yuuminigame2_get_name, 8, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 500)
#else
    SET_VALUE(LW(3), 40)
    SET_VALUE(LW(4), 1500)
    SET_VALUE(LW(5), 100)
#endif
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 8, LW(15))

    // Small whirlpools 5 to 8.
    CALL(yuuminigame2_get_name, 9, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 120)
    SET_VALUE(LW(3), -40)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 9, LW(15))
    CALL(yuuminigame2_get_name, 10, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 60)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 10, LW(15))
    CALL(yuuminigame2_get_name, 11, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 40)
    SET_VALUE(LW(3), -50)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 11, LW(15))
    CALL(yuuminigame2_get_name, 12, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 40)
    SET_VALUE(LW(4), 2800)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 2)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 12, LW(15))

    // The eight barrels (S_taru01-08) bob and float back and forth across the course.
    CALL(yuuminigame2_get_name, 13, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -20)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 3600)
    SET_VALUE(LW(5), 400)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 13, LW(15))
    CALL(yuuminigame2_get_name, 14, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -75)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 800)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 14, LW(15))
    CALL(yuuminigame2_get_name, 15, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -70)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 15, LW(15))
    CALL(yuuminigame2_get_name, 16, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 75)
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 16, LW(15))

    // Barrels 5 to 7 hold still in the early demos; later versions move them too.
    CALL(yuuminigame2_get_name, 17, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 5)
#else
    SET_VALUE(LW(2), -40)
    SET_VALUE(LW(3), 100)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 300)
    SET_VALUE(LW(6), 2)
#endif
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 17, LW(15))
    CALL(yuuminigame2_get_name, 18, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), 0)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 5)
#else
    SET_VALUE(LW(3), 240)
    SET_VALUE(LW(4), 4000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
#endif
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 18, LW(15))
    CALL(yuuminigame2_get_name, 19, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 5)
#else
    SET_VALUE(LW(2), -60)
    SET_VALUE(LW(3), 0)
    SET_VALUE(LW(4), 1000)
    SET_VALUE(LW(5), 100)
    SET_VALUE(LW(6), 2)
#endif
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 19, LW(15))
    CALL(yuuminigame2_get_name, 20, LW(0), LW(1), LW(11))
    SET_VALUE(LW(0), LW(0))
    SET_VALUE(LW(1), LW(1))
    SET_VALUE(LW(2), -25)
    SET_VALUE(LW(3), 75)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(11))
    SET_VALUE(LW(12), 1)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 20, LW(15))

    // The game is set up and the audience appears.
    SPAWN_EVENT(minigame02_main)
    CALL_CHILD(evt_yuunpc_minigame2)
#if VERSION >= VERSION_US
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    END_SWITCH
#endif
    RETURN
EVT_END()

/**
 * Per-frame rules of Boat Mode: detects Mario's boat touching a big whirlpool (index 1 to 4,
 * which drags him down and back and costs a point) or a small whirlpool (5 to 12, which spins the boat),
 * applies the movement, and ends the round (TRUE) when Mario reaches the far end (x <= -700, z <= 0)
 * or the counter runs out, while no whirlpool is holding Mario.
 */
BOOL yuuminigame2_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, FuneWork* data) {
    MarioWork* mario = marioGetPtr();
    Vec box[3];
    Mtx mtx;
    Vec pos;
    Vec tmp;
    Vec ofs;
    f32 angle;
    s32 i;
    s32 j;

    if (data->index == 0x15) {
        if ((mario->position.x <= -700.0f && mario->position.z <= 0.0f) || yuucounter_get_data() < 0) {
            data->coins = yuucounter_get_data();
            marioKeyOff();
            partyKeyOff();
            yuucounter_flag_on(2);
            return TRUE;
        }
    }
    box[2] = mario->position;
    box[1] = mario->position;
    box[0] = mario->position;
    box[0].x -= 12.5f;
    box[2].x += 12.5f;
    while (data->index < 1 || data->index >= 5) {
        for (i = 1; i < 5; i++) {
            mapObjGetPos(s_nametbl[i], &pos);
            pos.y = mario->position.y;
            for (j = 0; j < 3; j++) {
                if (PSVECSquareDistance(&box[j], &pos) <= 3306.25f) {
                    tmp = mario->position;
                    if (tmp.x == pos.x && tmp.z == pos.z) {
                        tmp.x += 1.0f;
                    }
                    data->index = i;
                    marioKeyOff();
                    partyKeyOff();
                    mario->flags |= MARIO_FLAG_FORCE_WALL;
                    mario->flags |= MARIO_FLAG_FLOORS_DISABLED;
                    data->state = 0;
                    PSVECSubtract(&tmp, &pos, &data->dir);
                    data->dir.y = 0.0f;
                    data->dist = PSVECMag(&data->dir);
                    PSVECNormalize(&data->dir, &data->dir);
#if VERSION <= VERSION_DEMO_JP
                    data->timer = 240;
#else
                    data->timer = 120;
#endif
                    data->y = mario->position.y;
                    mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_GAME_BOAT_UZUSIO1), &mario->position);
                    data->result--;
                    break;
                }
            }
            if (j < 3) {
                break;
            }
        }
        if (i < 5) {
            break;
        }
        for (i = 5; i < 13; i++) {
            mapObjGetPos(s_nametbl[i], &pos);
            pos.y = mario->position.y;
            for (j = 0; j < 3; j++) {
                if (PSVECSquareDistance(&box[j], &pos) <= 225.0f) {
                    if (!marioChkSwirlPower()) {
                        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_EVT_GAME_BOAT_UZUSIO2), &mario->position);
                        data->result--;
                    }
                    data->index = i;
                    marioSetSwirlPower(1000, 8.0f, irand(91) + hoko_tbl[data->index] - 45);
                    mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                    break;
                }
            }
            if (j < 3) {
                break;
            }
        }
        if (i < 13) {
            break;
        }
        break;
    }
    if (data->index != 0x15) {
    angle = mario->displayDirection + turn_tbl[data->index];
    if (data->index >= 5 && data->index < 13) {
        if (!marioChkSwirlPower()) {
            data->index = 0x15;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            if (mario->displayDirection >= -90.0f && mario->displayDirection < 90.0f) {
                angle = 0.0f;
            } else {
                angle = 180.0f;
            }
        }
    } else if (data->index >= 1 && data->index < 5) {
        switch (data->state) {
            case 0:
                data->timer--;
                PSMTXRotRad(mtx, 'Y', 0.017453292f * turn_tbl[data->index]);
                PSMTXMultVec(mtx, &data->dir, &data->dir);
                data->dir.y = 0.0f;
                PSVECNormalize(&data->dir, &data->dir);
#if VERSION <= VERSION_DEMO_JP
                PSVECScale(&data->dir, &ofs, data->timer >= 60 ? data->dist * ((data->timer - 60) / 180.0f) : 0.0f);
#else
                PSVECScale(&data->dir, &ofs, data->timer >= 30 ? data->dist * ((data->timer - 30) / 90.0f) : 0.0f);
#endif
                mapObjGetPos(s_nametbl[data->index], &pos);
                PSVECAdd(&ofs, &pos, &ofs);
                mario->position.x = ofs.x;
                mario->position.z = ofs.z;
#if VERSION <= VERSION_DEMO_JP
                mario->position.y = data->y - 30.0f * ((240 - data->timer) / 240.0f);
#else
                mario->position.y = data->y - 30.0f * ((120 - data->timer) / 120.0f);
#endif
                if (data->timer <= 0) {
                    data->state++;
                    angle = 0.0f;
#if VERSION <= VERSION_DEMO_JP
                    data->timer = 60;
#else
                    data->timer = 30;
#endif
                    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                    data->x = mario->position.x;
                }
                break;
            case 1:
                data->timer--;
                angle = 0.0f;
#if VERSION <= VERSION_DEMO_JP
                mario->position.x = data->x + -100.0f * ((60 - data->timer) / 60.0f);
#else
                mario->position.x = data->x + -100.0f * ((30 - data->timer) / 30.0f);
#endif
                mario->position.y = data->y - 30.0f;
                if (data->timer <= 0) {
                    data->state++;
#if VERSION <= VERSION_DEMO_JP
                    data->timer = 60;
#else
                    data->timer = 30;
#endif
                }
                break;
            case 2:
                data->timer--;
                angle = 0.0f;
#if VERSION <= VERSION_DEMO_JP
                mario->position.y = data->y - 30.0f * (data->timer / 60.0f);
#else
                mario->position.y = data->y - 30.0f * (data->timer / 30.0f);
#endif
                if (data->timer <= 0) {
                    data->index = 0x15;
                    marioKeyOn();
                    partyKeyOn();
                    mario->flags &= ~MARIO_FLAG_FORCE_WALL;
                    mario->flags &= ~MARIO_FLAG_FLOORS_DISABLED;
                }
                break;
        }
    }
    mario->displayDirectionTarget = reviseAngle(angle);
    mario->displayDirection = mario->displayDirectionTarget;
    }
    return FALSE;
}

/** Draw callback for minigame 2: draws the collected-coin counter (monteCountDisp). */
void yuuminigame_disp_2fune(CameraId camera, void* param) {
    monteCountDisp();
}

/**
 * Per-frame state machine for Boat Mode: phase 1 starts minigame02_start, phases 2 and 3 run the rules check,
 * phase 4 launches minigame02_end with the remaining time, a result value and the coin count.
 */
void yuuminigame_main_2fune(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    FuneWork* data = wp->data;

    switch (data->phase) {
        case 0:
            break;
        case 5:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame02_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            data->index = 0x15;
            data->phase = 3;
            data->result = 0;
        case 3:
            if (yuuminigame2_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 4:
            wp->subEvent = evtEntry(minigame02_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
#if VERSION <= VERSION_DEMO_JP
            if (data->result == 0) {
                data->result += 2;
            } else if (data->result < 0) {
#else
            if (data->result + 5 <= 0) {
#endif
                data->result = 1;
#if VERSION >= VERSION_JP
            } else {
                data->result += 0x105;
#endif
            }
            wp->subEvent->lwData[1] = data->result;
            wp->subEvent->lwData[2] = data->monteCount;
            data->phase++;
            break;
    }
}

/**
 * Init callback for minigame 2: starts the coin view event,
 * marks the opening cutscene as pending and switches Mario to the boat motion.
 */
void yuuminigame_init_2fune(EventEntry* event, BOOL isFirstCall) {
    FuneWork* data = ((MiniGameWork*)event->lwData[0])->data;

    data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
    marioChgShipMotion();
}

/**
 * args[0] = minigame work pointer, args[1] = output.
 * Writes 1 if the opening cutscene is still pending (flags bit 0), else 0.
 */
static USER_FUNC(yuuminigame2_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((FuneWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer, args[1] = on/off.
 * Sets or clears the "opening cutscene pending" bit (flags bit 0).
 */
static USER_FUNC(yuuminigame2_opening_on_off) {
    EvtWord* args = event->args;
    FuneWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = index into the whirlpool/barrel tables;
 * returns the map object name in args[1], the hit object name in args[2] and the turn angle in args[3].
 */
static USER_FUNC(yuuminigame2_get_name) {
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (EvtWord)s_nametbl[i]);
    evtSetValue(event, args[2], (EvtWord)a_nametbl[i]);
    evtSetValue(event, args[3], turn_tbl[i]);
    return EVT_RETURN_DONE;
}

/** args[0] = minigame work pointer, args[1] = new phase for the Boat Mode state machine. */
static USER_FUNC(yuuminigame2_set_phase) {
    EvtWord* args = event->args;
    FuneWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Resets the coins for the next round: stops the view event,
 * frees the Monte array and starts a fresh minigame_view_monte event.
 */
static USER_FUNC(re_fall_rtn) {
    FuneWork* data;

    if (monte_p != NULL) {
        data = monte_p->wp->data;
        if (data->viewEvent != NULL) {
            evtDeleteID(data->viewEvent->eventId);
            data->viewEvent = NULL;
            _mapFree(monte_p);
            data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
            data->viewEvent->lwData[0] = (EvtWord)monte_p->wp;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Frame-by-frame coin spawner and collector for Boat Mode.
 * On first call it allocates the Monte array and scatters coins at random spots in the montetable rectangles (at
 * least 20 units apart within each rectangle), then each frame it checks the boat against every coin,
 * counting and hiding collected ones with a 3D sound. Blocks forever as a view event.
 */
static USER_FUNC(fall_rtn) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    BOOL exists = FALSE;
    FuneWork* data = wp->data;
    s32 total;
    s32 i;
    s32 t;
    s32 k;
    SfxNameWord sfx;
    s32 j;
    f32 lo;
    f32 hi;
    Vec v;
    Monte* m;
    MonteSub* sub;

    if (isFirstCall) {
        if (monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; montetable[i].num; i++) {
            total += montetable[i].num;
        }
        monte_p = _mapAlloc(total * sizeof(Monte));
        memset(monte_p, 0, total * sizeof(Monte));
        monte_p->num = total;
        monte_p->wp = wp;
        data->monteCount = 0;
        t = 0;
        k = 0;
        for (i = 0; i < monte_p->num; i++) {
            sub = &monte_p[i].sub;
            sub->pos.y = 0.0f;
            do {
                if (montetable[t].x0 < montetable[t].x1) {
                    lo = montetable[t].x0;
                    hi = montetable[t].x1;
                } else {
                    lo = montetable[t].x1;
                    hi = montetable[t].x0;
                }
                hi -= lo;
                sub->pos.x = lo + randRange(hi);
                if (montetable[t].y0 < montetable[t].y1) {
                    lo = montetable[t].y0;
                    hi = montetable[t].y1;
                } else {
                    lo = montetable[t].y1;
                    hi = montetable[t].y0;
                }
                hi -= lo;
                sub->pos.z = lo + randRange(hi);
                for (j = 0; j < k; j++) {
                    PSVECSubtract(&sub->pos, &(sub - (j + 1))->pos, &v);
                    if (PSVECMag(&v) < 20.0f) {
                        break;
                    }
                }
            } while (j != k);
            if (++k >= montetable[t].num) {
                k = 0;
                t++;
            }
            sub->scale = 1.0f;
        }
    }
    for (i = 0; i < monte_p->num; i++) {
        sfx = (SfxNameWord)"SFX_EVT_GAME_MONTE_COUNT1";
        m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (monte_check_inl(&m->sub.pos, &marioGetPtr()->position) == TRUE) {
                    data->monteCount++;
                    psndSFXOn_3D((const char*)sfx, &marioGetPtr()->position);
                    m->hidden = 1;
                    m->state = 1;
                }
                break;
            case 1:
                break;
        }
    }
    if (!exists) {
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, monte_disp, monte_p);
    }
    return EVT_RETURN_BLOCK;
}

/** Draw callback that renders every coin icon of a Monte array that has not been collected yet. */
static void monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                iconDispGx(&monte[i].sub.pos, 0, ICON_ITEM_PIANTA_TOKEN, 1.0f);
            }
        }
    }
}

/**
 * Returns TRUE when two points are within 20 units in 3D and within 20 in height;
 * unused here, the inline copy monte_check_inl does the work.
 */
static BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

// Descriptive name (not in the symbol maps); placeholder unk_yuu_00005268, JP yuu .text+0x519C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/** Resets the camera shift offset (camShiftReset) so the camera returns to its normal framing after the minigame. */
USER_FUNC(yuu_01_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}
#endif
