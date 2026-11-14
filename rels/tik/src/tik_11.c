// tik_11 (埋没した都市, Rogueport Sewers): the power-down shop (powerdownya), which trades Mario's stats or partners' ranks.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_win.h"
#include "evt/evt_window.h"
#include "evt/evt_pouch.h"
#include "manager/winmgr.h"
#include "evt/evt_cmd.h"
#include "driver/bgdrv.h"
#include "pmario_sound.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "mario/mario.h"
#include "party.h"

#include "driver/npcdrv.h"
#include "eff/eff_stardust.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"

static s32 xlw[16];

static USER_FUNC(bgScrollControll);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(effect_stardust);
static USER_FUNC(enzan_cam_turn_keisan_f);
static USER_FUNC(enzan_lw_load);
static USER_FUNC(enzan_lw_save);
static USER_FUNC(setmertn);
static USER_FUNC(wariai_enzan_1f);
static USER_FUNC(enzan_set_f_i);
static USER_FUNC(enzan_set_i_f);
static USER_FUNC(enzan_cam_turn_keisan);
static USER_FUNC(wariai_enzan_0f);
static USER_FUNC(wariai_enzan_0);
static USER_FUNC(wariai_enzan_1);

#if VERSION >= VERSION_JP
EvtWord powerdownya_face_left[16];
#endif

#if VERSION >= VERSION_US
static EvtWord powerdownya_mario_level_rtn[639];
static EvtWord powerdownya_party_lank_rtn[963];
static EvtWord powerdownya_table_talk[105];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord powerdownya_mario_level_rtn[613];
static EvtWord powerdownya_party_lank_rtn[896];
#else
static EvtWord powerdownya_mario_level_rtn[616];
static EvtWord powerdownya_party_lank_rtn[937];
#endif
static EvtWord powerdownya_table_talk[84];
#endif

static EvtWord powerdownya_direct_talk[8];
static EvtWord powerdownya_init[10];
static NpcSetupInfo npcEnt[2];
static BeroEntry bero_entry_data[2];
EvtWord tik_11_init_evt[31];

/**
 * Script helper.
 * Sets the running event's owner NPC to the NPC named by args[0], so "me" in the shop scripts refers to it.
 */
static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Spawns a star-dust particle burst: args are the type, X, Y, Z, X and Y velocities, particle count and lifetime.
 */
static USER_FUNC(effect_stardust) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 vx = evtGetFloat(event, args[4]);
    f32 vy = evtGetFloat(event, args[5]);
    s32 count = evtGetValue(event, args[6]);
    s32 lifetime = evtGetValue(event, args[7]);

    effStardustEntry(type, count, lifetime, x, y, z, vx, vy);
    return EVT_RETURN_DONE;
}

/**
 * Script helper for ratio ("wariai") math on integers.
 * With a = args[0] - 1 and b = args[1], stores (a * args[2] + args[3] * (b - a)) / b into args[4]:
 * a blend from args[3] toward args[2] as args[0] steps up from 1 to b + 1.
 */
static USER_FUNC(wariai_enzan_1) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/** The same blend as wariai_enzan_1, but stores the result into args[4] as a float. */
static USER_FUNC(wariai_enzan_1f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/**
 * Like wariai_enzan_1, but the divisor is args[1] - 1, so the blend reaches args[2] when args[0] equals args[1].
 * Result stored as an integer.
 */
static USER_FUNC(wariai_enzan_0) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/** The same as wariai_enzan_0, with the result stored as a float. */
static USER_FUNC(wariai_enzan_0f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Rotates the point whose X and Z are held in the variables args[1]
 * and args[2] around the centre (args[3], args[4]) by args[0] degrees (modulo 360),
 * writing the new X and Z back.
 */
static USER_FUNC(enzan_cam_turn_keisan) {
    EvtWord* args = event->args;
    f32 angle = evtGetValue(event, args[0]) % 360;
    s32 xVar = args[1];
    s32 zVar = args[2];
    Vec pos;
    Vec center;
    Mtx mtx;

    pos.x = evtGetValue(event, xVar);
    pos.z = evtGetValue(event, zVar);
    pos.y = 0.0f;
    center.x = evtGetValue(event, args[3]);
    center.z = evtGetValue(event, args[4]);
    center.y = 0.0f;
    PSVECSubtract(&pos, &center, &pos);
    PSMTXRotRad(mtx, 'Y', 0.017453292f * angle);
    PSMTXMultVec(mtx, &pos, &pos);
    PSVECAdd(&pos, &center, &pos);
    evtSetValue(event, xVar, pos.x);
    evtSetValue(event, zVar, pos.z);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that sets the 3D camera for a turn around the shop counter.
 * args[0] is a percentage 0..100: the camera slides from (-9, 212, 886) toward (-9, 152, 542)
 * while orbiting the point (-9, 56, 0) by up to two turns (720 degrees), then is snapped to that position.
 */
// Move through two full camera revolutions as the interpolation parameter runs from zero to one hundred.
static USER_FUNC(enzan_cam_turn_keisan_f) {
    f32 t = evtGetFloat(event, event->args[0]) / 100.0f;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec base;
    Vec pos;
    Vec pivot;
    Mtx mtx;
    f32 x = -9.0f;

    base = (Vec){x, -60.0f * t + 212.0f, -344.0f * t + 886.0f};
    pos = base;
    pivot = (Vec){-9.0f, 56.0f, 0.0f};
    PSVECSubtract(&pos, &pivot, &pos);
    PSMTXRotRad(mtx, 'Y', 0.017453292f * (720.0f * t));
    PSMTXMultVec(mtx, &pos, &pos);
    PSVECAdd(&pos, &pivot, &pos);
    cam->presetPos = cam->cameraPos;
    cam->presetAt = cam->cameraAt;
    cam->storedPos = pos;
    cam->storedAt = cam->cameraAt;
    cam->startTimeA = gp->renderTime;
    cam->durationA = 0;
    cam->stateA = 3;
    cam->curveA = 0;
    return EVT_RETURN_DONE;
}

/** Script helper. Copies the float args[1] into the variable args[0] as an integer. */
static USER_FUNC(enzan_set_i_f) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetValue(event, var, evtGetFloat(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Script helper. Copies the integer args[1] into the variable args[0] as a float. */
static USER_FUNC(enzan_set_f_i) {
    EvtWord* args = event->args;
    s32 var = args[0];

    evtSetFloat(event, var, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Script helper. Saves the script's 16 LW registers into the module array xlw. */
// Save all sixteen local words in the shared backup before the animation reuses them.
static USER_FUNC(enzan_lw_save) {
    s32 i;

    for (i = 0; i < 16; i++) {
        xlw[i] = event->lwData[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper. Restores the 16 LW registers from the array saved by enzan_lw_save. */
static USER_FUNC(enzan_lw_load) {
    s32 i;

    for (i = 0; i < 16; i++) {
        event->lwData[i] = xlw[i];
    }
    return EVT_RETURN_DONE;
}

/** Script helper. With args[0] = 0 turns the background scroll offset off, otherwise on. */
static USER_FUNC(bgScrollControll) {
    if (evtGetValue(event, event->args[0]) == 0) {
        bgTransOffsetOff();
    } else {
        bgTransOffsetOn();
    }
    return EVT_RETURN_DONE;
}

/** Script helper. Blocks until the music on channel 1 has started, or returns immediately if the channel is empty. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

#if VERSION >= VERSION_JP
/**
 * Loop that keeps Mario and his partner facing the shopkeeper (the point X = -9) every frame,
 * until the script is stopped. JP and later.
 */
EVT_BEGIN(powerdownya_face_left)
    LOOP(0)
        CALL(evt_party_set_dir_pos, 0, -9, 0)
        CALL(evt_mario_set_dir_pos, -9, 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Chet Rippo's stat trade for Mario. Refuses if Mario has fewer than 39 coins (powerdown_04).
 * Otherwise shows the coin window and two choices (evt_win_other_select 9 and 10: the stat to boost and the stat
 * to decrease) and asks whether that trade sounds good for 39 coins (powerdown_06, powerdown_09);
 * on yes it takes the 39 coins, chants (powerdown_11, powerdown_12) over the light and house effects,
 * sets the new HP, FP and BP (unitwin_setmariolevel), and says it removed all badges because BP ran low
 * (powerdown_10) when that happened, else that it is finished (powerdown_13).
 */
static EVT_BEGIN(powerdownya_mario_level_rtn)
    // Chet Rippo turns Mario away if he cannot pay 39 coins.
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), 39)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, 0, STRING("powerdown_04"), 0, STRING("me"))
#else
        CALL(evt_msg_print_add, 0, STRING("powerdown_04"))
#endif
        RETURN
    END_IF

    // Mario picks the stat to raise and the stat to lower, and confirms the trade.
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
    LOOP(0)
        CALL(evt_win_other_select, SELECT_TYPE_STAT_SWAP_UP)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print, 0, STRING("powerdown_03_01"), 0, STRING("me"))
            RETURN
        END_IF
        WAIT_MS(240)
        SET_VALUE(GW(1), LW(1))
        CALL(evt_win_other_select, SELECT_TYPE_STAT_SWAP_DOWN)
        IF_INT_EQUAL(LW(0), 0)
            WAIT_MS(240)
            CONTINUE_LOOP
        END_IF
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, STRING("powerdown_06"), 0, STRING("me"), LW(2), LW(3), 39)
            CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("powerdown_06"), 39)
                CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"), LW(2), LW(3))
        END_SWITCH
#else
        CALL(evt_msg_print_insert, 0, STRING("powerdown_06"), 0, STRING("me"), LW(2), LW(3), 39)
#endif
        CALL(evt_msg_select, 0, STRING("powerdown_09"))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_msg_continue)
    END_LOOP

    // Mario pays 39 coins, the music stops and Chet Rippo starts his chant.
    CALL(evt_pouch_get_coin, LW(0))
    SUBTRACT(LW(0), 39)
    CALL(evt_pouch_set_coin, LW(0))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    CALL(enzan_lw_save)
#if VERSION >= VERSION_JP
    SPAWN_EVENT_GET_ID(PTR(powerdownya_face_left), GW(1))
#endif
    CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
    CALL(evt_msg_print_add, 0, STRING("powerdown_11"))
    CALL(bgScrollControll, 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -20, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(5))
        CALL(evt_mapobj_trans, 1, STRING("down_pira"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(5), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)

    // The power-down music plays, the walls of the house fold open and the camera pulls back over the light.
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_POWERDOWN1"))
    CALL(bgm_start_wait)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -9, 49, 0, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("down_pira"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_FRAMES(5)
    CALL(evt_msg_print, 0, STRING("powerdown_12"), 0, STRING("me"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("down_pira"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("ie_naka"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 90, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("ie_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("ie_o"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("ie_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 1600)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -9, 212, 886, -9, 56, 0, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1600)

    // For seven seconds the camera turns around Mario while crystal waves pulse and he glows.
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 100, 7000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(enzan_cam_turn_keisan_f, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(6), 7000)
        DIVIDE(LW(6), 1000)
        ADD(LW(6), 1)
        LOOP(LW(6))
            CALL(evt_eff64, 0, STRING("crystal_n64"), 3, -9, 49, 0, FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1000)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        SET_FLOAT_VALUE(LW(7), LW(4))
        ADD_FLOAT(LW(7), FLOAT(18.0f))
        CALL(evt_eff64, STRING("cccx"), STRING("pokopi_pcharge_n64"), 2, LW(3), LW(7), LW(5), 1, -1, 0, 0, 0, 0, 0, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_I_Z"))
    END_INLINE_EVENT
    WAIT_MS(7000)

    // The light fades, the house closes up again and the camera returns.
    CALL(evt_eff_softdelete, STRING("aaax"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("down_pira"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL(evt_eff64, STRING("dddx"), STRING("release_n64"), 65283, -9, 49, 0, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("cccx"))
    CALL(evt_mario_set_pose, STRING("M_I_1"))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    SET_FLOAT_VALUE(LW(7), LW(4))
    ADD_FLOAT(LW(7), FLOAT(18.0f))
    CALL(effect_stardust, 0, LW(3), LW(7), LW(5), FLOAT(50.0f), FLOAT(50.0f), 20, 120)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("down_pira"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("ie_naka"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("ie_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("ie_o"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("ie_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(bgScrollControll, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1400)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -20, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(5))
        CALL(evt_mapobj_trans, 1, STRING("down_pira"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(5), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // Mario poses, the music comes back and the new HP, FP and BP are set; Chet Rippo says if badges came off.
    CALL(evt_mario_set_pose, STRING("M_V_2"))
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
#if VERSION >= VERSION_JP
    DELETE_EVENT(GW(1))
#endif
    CALL(enzan_lw_load)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(unitwin_setmariolevel, LW(5), LW(6), LW(7), LW(8))
    IF_INT_NOT_EQUAL(LW(8), 0)
        CALL(evt_msg_print, 0, STRING("powerdown_10"), 0, STRING("me"))
    ELSE
        CALL(evt_msg_print, 0, STRING("powerdown_13"), 0, STRING("me"))
    END_IF
    RETURN
EVT_END()

/**
 * Chet Rippo's rank trade between two partners.
 * Refuses if Mario has fewer than 39 coins (powerdown_04) or, in JP and later, if no partner can be ranked down
 * (evt_win_party_lankdown_chk, powerdown_07).
 * Otherwise Mario picks the partner to boost and the partner to drop (evt_win_other_select 7 and 8) and confirms
 * the trade for 39 coins (powerdown_08, powerdown_09);
 * on yes it takes the coins, brings out the partner losing a rank,
 * chants (powerdown_11, powerdown_12), swaps in the boosted partner, raises that partner's rank by one,
 * lowers the other's by one and says it is
 * finished (powerdown_13).
 */
static EVT_BEGIN(powerdownya_party_lank_rtn)
    // Chet Rippo turns Mario away if he cannot pay 39 coins (or, from JP on, if no partner can lose a rank).
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), 39)
        CALL(evt_msg_print_add, 0, STRING("powerdown_04"))
        RETURN
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_win_party_lankdown_chk, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("powerdown_07"))
        RETURN
    END_IF
#endif

    // Mario picks the partner to rank up and the partner to rank down, and confirms the trade.
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
    LOOP(0)
        CALL(evt_win_other_select, SELECT_TYPE_PARTY_SWAP_RANK_UP)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_win_coin_off, LW(12))
            CALL(evt_msg_print, 0, STRING("powerdown_03_01"), 0, STRING("me"))
            RETURN
        END_IF
        WAIT_MS(240)
        SET_VALUE(GW(1), LW(1))
        CALL(evt_win_other_select, SELECT_TYPE_PARTY_SWAP_RANK_DOWN)
        IF_INT_EQUAL(LW(0), 0)
            WAIT_MS(240)
            CONTINUE_LOOP
        END_IF
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, STRING("powerdown_08"), 0, STRING("me"), LW(3), LW(4), 39)
            CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("powerdown_08"), 39)
                CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"), LW(3), LW(4))
        END_SWITCH
#else
        CALL(evt_msg_print_insert, 0, STRING("powerdown_08"), 0, STRING("me"), LW(3), LW(4), 39)
#endif
        CALL(evt_msg_select, 0, STRING("powerdown_09"))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_msg_continue)
    END_LOOP

    // Mario pays 39 coins, the partner losing a rank is brought out and Chet Rippo starts his chant.
    CALL(evt_pouch_get_coin, LW(0))
    SUBTRACT(LW(0), 39)
    CALL(evt_pouch_set_coin, LW(0))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    CALL(enzan_lw_save)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(1), LW(0))
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, LW(1))
    END_IF
    CALL(evt_msg_print_add, 0, STRING("powerdown_11"))
    CALL(bgScrollControll, 0)
    CALL(evt_mario_goodbye_party, 0)
    CALL(evt_mario_hello_party, 0, LW(2))
#if VERSION >= VERSION_US
    CALL(evt_party_stop, 0)
#endif

    // The house walls fold open and the camera pulls back over the light (power-down music from JP on).
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -20, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(5))
        CALL(evt_mapobj_trans, 1, STRING("down_pira"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(5), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
#if VERSION >= VERSION_JP
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_POWERDOWN1"))
    CALL(bgm_start_wait)
#endif
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, -9, 49, 0, FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("down_pira"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION >= VERSION_JP
    SPAWN_EVENT_GET_ID(PTR(powerdownya_face_left), GW(1))
#endif
    CALL(evt_msg_print, 0, STRING("powerdown_12"), 0, STRING("me"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 255, 0, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("down_pira"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("ie_naka"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 90, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("ie_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("ie_o"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("ie_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 1600)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -9, 212, 886, -9, 56, 0, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1600)

    // For seven seconds the camera turns: the partner losing a rank spins away, then the other one glows.
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 100, 7000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(enzan_cam_turn_keisan_f, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(6), 7000)
        DIVIDE(LW(6), 1000)
        ADD(LW(6), 1)
        LOOP(LW(6))
            CALL(evt_eff64, 0, STRING("crystal_n64"), 3, -9, 49, 0, FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1000)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(PARTY_GOOMBELLA)
                CALL(evt_party_set_pose, 0, STRING("PKR_I_1"))
            CASE_EQUAL(PARTY_KOOPS)
                CALL(evt_party_set_pose, 0, STRING("PNK_I_1"))
            CASE_EQUAL(PARTY_BOBBERY)
                CALL(evt_party_set_pose, 0, STRING("I_1"))
            CASE_EQUAL(PARTY_YOSHI)
                CALL(evt_party_set_pose, 0, STRING("PYS_I_1"))
            CASE_EQUAL(PARTY_FLURRIE)
                CALL(evt_party_set_pose, 0, STRING("PWD_I_1"))
            CASE_EQUAL(PARTY_VIVIAN)
                CALL(evt_party_set_pose, 0, STRING("PTR_I_1"))
            CASE_EQUAL(PARTY_MS_MOWZ)
                CALL(evt_party_set_pose, 0, STRING("PCH_I_1"))
        END_SWITCH
        WAIT_MS(500)
#if VERSION >= VERSION_JP
        DELETE_EVENT(GW(1))
#endif
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
        CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(effect_stardust, 4, LW(3), LW(4), LW(5), FLOAT(50.0f), FLOAT(50.0f), 14, 60)
        CALL(evt_party_get_dispdir, 0, LW(9))
        SET_FLOAT_VALUE(LW(10), LW(9))
        ADD_FLOAT(LW(10), 1440)
        CALL(evt_party_flg_onoff, 0, 33554432, 1)
        SET_VALUE(LW(6), 60)
        LOOP(LW(6))
            CALL(wariai_enzan_1f, LW(6), 60, 0, 40, LW(7))
            CALL(wariai_enzan_1f, LW(6), 60, LW(9), LW(10), LW(8))
            ADD_FLOAT(LW(7), LW(4))
            CALL(evt_party_set_pos, 0, LW(3), LW(7), LW(5))
            CALL(evt_party_set_dispdir, 0, LW(8))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
        CALL(evt_party_flg_onoff, 0, 33554432, 0)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, LW(1))
        WAIT_MS(500)
#if VERSION >= VERSION_JP
        CALL(evt_party_stop, 0)
        SPAWN_EVENT_GET_ID(PTR(powerdownya_face_left), GW(1))
#endif
        CALL(evt_mario_get_party, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(PARTY_GOOMBELLA)
                CALL(evt_party_set_pose, 0, STRING("PKR_C_1"))
            CASE_EQUAL(PARTY_KOOPS)
                CALL(evt_party_set_pose, 0, STRING("PNK_C_1"))
            CASE_EQUAL(PARTY_BOBBERY)
                CALL(evt_party_set_pose, 0, STRING("C_1"))
            CASE_EQUAL(PARTY_YOSHI)
                CALL(evt_party_set_pose, 0, STRING("PYS_C_1"))
            CASE_EQUAL(PARTY_FLURRIE)
                CALL(evt_party_set_pose, 0, STRING("PWD_C_1"))
            CASE_EQUAL(PARTY_VIVIAN)
                CALL(evt_party_set_pose, 0, STRING("PTR_C_1"))
            CASE_EQUAL(PARTY_MS_MOWZ)
                CALL(evt_party_set_pose, 0, STRING("PCH_C_1"))
        END_SWITCH
        CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
        SET_FLOAT_VALUE(LW(7), LW(4))
        ADD_FLOAT(LW(7), FLOAT(18.0f))
        CALL(evt_eff64, STRING("cccx"), STRING("pokopi_pcharge_n64"), 2, LW(3), LW(7), LW(5), 1, -1, 0, 0, 0, 0, 0, 0)
        WAIT_MS(2000)
        CALL(evt_party_set_breed_pose, 0, 1)
    END_INLINE_EVENT

    // The light fades, the house closes up again and the camera returns, and the partner cheers.
    WAIT_MS(7000)
    CALL(evt_eff_softdelete, STRING("aaax"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("down_pira"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL(evt_eff64, STRING("dddx"), STRING("release_n64"), 65283, -9, 49, 0, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_eff_softdelete, STRING("cccx"))
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(PARTY_GOOMBELLA)
            CALL(evt_party_set_pose, 0, STRING("PKR_I_1"))
        CASE_EQUAL(PARTY_KOOPS)
            CALL(evt_party_set_pose, 0, STRING("PNK_I_1"))
        CASE_EQUAL(PARTY_BOBBERY)
            CALL(evt_party_set_pose, 0, STRING("I_1"))
        CASE_EQUAL(PARTY_YOSHI)
            CALL(evt_party_set_pose, 0, STRING("PYS_I_1"))
        CASE_EQUAL(PARTY_FLURRIE)
            CALL(evt_party_set_pose, 0, STRING("PWD_I_1"))
        CASE_EQUAL(PARTY_VIVIAN)
            CALL(evt_party_set_pose, 0, STRING("PTR_I_1"))
        CASE_EQUAL(PARTY_MS_MOWZ)
            CALL(evt_party_set_pose, 0, STRING("PCH_I_1"))
    END_SWITCH
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    SET_FLOAT_VALUE(LW(7), LW(4))
    ADD_FLOAT(LW(7), FLOAT(18.0f))
    CALL(effect_stardust, 0, LW(3), LW(7), LW(5), FLOAT(50.0f), FLOAT(50.0f), 20, 120)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 1, STRING("S"), 255, 255, 255, LW(0))
            CALL(evt_mapobj_color, 1, STRING("down_pira"), 255, 255, 255, 255)
            CALL(evt_mapobj_color, 1, STRING("ie_naka"), 255, 255, 255, 255)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 1600)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("ie_l"), 0, 0, LW(0))
            MULTIPLY(LW(0), -1)
            CALL(evt_mapobj_rotate, 1, STRING("ie_o"), LW(0), 0, 0)
            CALL(evt_mapobj_rotate, 1, STRING("ie_r"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(bgScrollControll, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 1600, EASING_SINE_IN_OUT)
    WAIT_MS(1400)
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -20, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(5))
        CALL(evt_mapobj_trans, 1, STRING("down_pira"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(5), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(PARTY_GOOMBELLA)
            CALL(evt_party_set_pose, 0, STRING("PKR_Y_1"))
        CASE_EQUAL(PARTY_KOOPS)
            CALL(evt_party_set_pose, 0, STRING("PNK_Y_1"))
        CASE_EQUAL(PARTY_BOBBERY)
            CALL(evt_party_set_pose, 0, STRING("Y_1"))
        CASE_EQUAL(PARTY_YOSHI)
            CALL(evt_party_set_pose, 0, STRING("PYS_Y_1"))
        CASE_EQUAL(PARTY_FLURRIE)
            CALL(evt_party_set_pose, 0, STRING("PWD_Y_1"))
        CASE_EQUAL(PARTY_VIVIAN)
            CALL(evt_party_set_pose, 0, STRING("PTR_Y_1"))
        CASE_EQUAL(PARTY_MS_MOWZ)
            CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    END_SWITCH
    WAIT_MS(1500)
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
#if VERSION <= VERSION_DEMO_JP
    CALL(enzan_lw_load)
#else
    DELETE_EVENT(GW(1))
    CALL(enzan_lw_load)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
#endif
    CALL(unitwin_getpartylank, LW(1), LW(0))
    ADD(LW(0), 1)
    CALL(unitwin_setpartylank, LW(1), LW(0))
    CALL(unitwin_getpartylank, LW(2), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(unitwin_setpartylank, LW(2), LW(0))
    CALL(evt_msg_print, 0, STRING("powerdown_13"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(powerdownya_table_talk)
    CALL(setmertn, STRING("パワーダウン屋"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    IF_INT_EQUAL(GSWF_TIK_11_CHET_RIPPO_FIRST_TALK_DONE, 0)
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, STRING("powerdown_00"), 0, STRING("me"), 39)
            CASE_DEFAULT
                CALL(evt_msg_fmt_int, 0, LW(14), STRING("powerdown_00"), 39)
                CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"))
        END_SWITCH
#else
        CALL(evt_msg_print_insert, 0, STRING("powerdown_00"), 0, STRING("me"), 39)
#endif
        SET_VALUE(GSWF_TIK_11_CHET_RIPPO_FIRST_TALK_DONE, 1)
        SET_VALUE(GSWF_TIK_11_CHET_RIPPO_GREETED_VISIT, 1)
    ELSE
        IF_INT_EQUAL(GSWF_TIK_11_CHET_RIPPO_GREETED_VISIT, 0)
            CALL(evt_msg_print, 0, STRING("powerdown_00_01"), 0, STRING("me"))
            SET_VALUE(GSWF_TIK_11_CHET_RIPPO_GREETED_VISIT, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("powerdown_01"), 0, STRING("me"))
        END_IF
    END_IF
    CALL(evt_msg_select, 0, STRING("powerdown_02"))
    IF_INT_EQUAL(LW(0), 2)
        CALL(evt_msg_print_add, 0, STRING("powerdown_03"))
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(powerdownya_mario_level_rtn)
    ELSE
        CALL_CHILD(powerdownya_party_lank_rtn)
    END_IF
    LABEL(0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Talk script for Chet Rippo when spoken to directly instead of at the counter; he says powerdown_14. */
static EVT_BEGIN(powerdownya_direct_talk)
    CALL(evt_msg_print, 0, STRING("powerdown_14"), 0, STRING("me"))
    RETURN
EVT_END()

/** Init script for Chet Rippo (パワーダウン屋). Registers the counter talk on the A_power_down hit area. */
static EVT_BEGIN(powerdownya_init)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_power_down"), 0, PTR(powerdownya_table_talk), 0)
    RETURN
EVT_END()

/** NPCs of this room: Chet Rippo, whose counter talk is registered on the A_power_down area. */
static NpcSetupInfo npcEnt[] = {
    {
        "パワーダウン屋", // name (Chet Rippo)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        powerdownya_init, // initEvent
        NULL, // regularEvent
        powerdownya_direct_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Doors of this room: the west exit back to tik_01. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Map init script of tik_11, Chet Rippo's shop.
 * Starts the sewer music and ambience, sets up the exit back to tik_01,
 * clears the greeting flag so he greets Mario on each entry,
 * places the shopkeeper and starts the smoke animation.
 */
EVT_BEGIN(tik_11_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    SET_VALUE(GSWF_TIK_11_CHET_RIPPO_GREETED_VISIT, 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("anm_kem"), MAP_ANIM_PLAY_LOOP, 0)
    RETURN
EVT_END()
