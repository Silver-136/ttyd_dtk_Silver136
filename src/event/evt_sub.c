#include "version.h"
#include "eff/eff_itemget.h"
#include "icon_ids.h"
#include "evt/evt_cmd.h"
#include "countdown.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_starstone.h"
#include "driver/itemdrv.h"
#include "driver/npcdrv.h"
#include "driver/swdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "driver/icondrv.h"
#include "memory.h"
#include "mario/mario_pouch.h"
#include "evt/evt_mario.h"
#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include <dolphin/pad.h>

void marioSetCamId(s32);
s32 effStageClearEndChk();
void statusWinForceOpen();
void statusWinForceClose();
BOOL statusWinCheckUpdate();

#include "system.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include <stdio.h>
#include <dolphin/os.h>
#ifdef VERSION_G8ME01
void evtMsgFmtInt(char* buf, s32 value);
void statusWinForceUpdate(void);
#endif

int rand(void);
double sin(double);
double cos(double);

#define KEYFN(name, fn)                                       \
    USER_FUNC(name) {                                         \
        EvtWord* args = event->args;                              \
        s32 chan = evtGetValue(event, args[0]);               \
        evtSetValue(event, args[1], fn(chan));                \
        return EVT_RETURN_DONE;                               \
    }

s32 stop;

typedef struct SplineWork {
    s32 n;         // 0x0
    f32* ts;       // 0x4
    Vec* pts;      // 0x8
    Vec* tbl;      // 0xC
    s32 cur;       // 0x10
    s32 steps;     // 0x14
    s32 mode;      // 0x18
    s32 msec;      // 0x1C
    OSTime start;  // 0x20
    s32 manual;    // 0x28
} SplineWork;

void spline_getvalue(f32* out, f32 t, s32 n, Vec* pts, f32* ts, Vec* tbl);
void spline_maketable(s32 n, Vec* pts, f32* ts, Vec* tbl);
#define INTPL_F(mode, cur, steps, a, b) ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(mode, a, b, cur, steps)



#pragma force_active on
static const char coin_fmt[] = "%f%f%f";
#pragma force_active reset

static const GXColor coin_black = {0x00, 0x00, 0x00, 0xFF};

USER_FUNC(bgm_start_wait);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_set_npc_rel);
USER_FUNC(evt_cam3d_evt_set_rel);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_delete);
USER_FUNC(evt_eff_softdelete);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_fade_reset);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_item_delete);
USER_FUNC(evt_item_get_position);
USER_FUNC(evt_item_set_position);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_balloon_bikkuri);
USER_FUNC(evt_mario_balloon_off);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_init_camid);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_wait_rideon);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_camid);
USER_FUNC(evt_party_init_camid);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_party_yoshi_ride);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_get_starstone);
USER_FUNC(evt_pouch_receive_mail);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfx_all_stop);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_check_intersect);
#ifdef VERSION_G8ME01
USER_FUNC(evt_sub_scale_by_fps);
#endif
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);
USER_FUNC(evt_sub_rumble_onoff);
USER_FUNC(evt_sub_save_playtime);
USER_FUNC(evt_win_one_message);
USER_FUNC(get_stop);
USER_FUNC(irai_mail_check);
USER_FUNC(mail_evt_gor_04_keyoff_check);
USER_FUNC(set_stop);
USER_FUNC(stageclear_wait);
USER_FUNC(stone_bg);
USER_FUNC(stone_bg_open);
USER_FUNC(stone_ry);
USER_FUNC(evt_sub_get_coin);
void coingetDisp(WinMgrEntry* entry);
USER_FUNC(evt_sub_countdown_set_restart_time);
USER_FUNC(evt_sub_countdown_flag_onoff);
USER_FUNC(evt_sub_countdown_end);
USER_FUNC(evt_sub_countdown_get_status);
USER_FUNC(evt_sub_countdown_start);
USER_FUNC(evt_sub_animgroup_async);
USER_FUNC(evt_sub_get_language);
USER_FUNC(evt_sub_load_progresstime);
USER_FUNC(evt_sub_save_playtime);
USER_FUNC(evt_sub_status_gauge_check_update);
USER_FUNC(evt_sub_status_gauge_force_close);
USER_FUNC(evt_sub_status_gauge_force_open);
USER_FUNC(evt_sub_get_areaname);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_key_get_button);
USER_FUNC(evt_key_get_dir);
USER_FUNC(evt_sub_system_flag_onoff);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_sub_get_dir);
USER_FUNC(evt_sub_get_dist);
USER_FUNC(evt_sub_area_check);
USER_FUNC(evt_sub_get_stopwatch);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_spline_free);
USER_FUNC(evt_sub_spline_get_value_manual);
USER_FUNC(evt_sub_spline_get_value);
USER_FUNC(evt_sub_spline_init);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_intpl_get_float);
USER_FUNC(evt_sub_intpl_init_float);
USER_FUNC(evt_sub_intpl_get_value_para);

extern EvtWord evt_sub_stopwatch[7];
extern EvtWord evt_sub_tree_access_entry[28];
extern EvtWord evt_sub_kusa_access_entry[26];
extern EvtWord evt_sub_starstone[VERSION_PICK(1043, 1084)];
extern EvtWord evt_sub_mail_vibration[26];
extern EvtWord evt_sub_mail_event[VERSION_PICK(117, 123)];
#ifndef VERSION_G8ME01
s32 evt_sub_mail_event_end[1];
#endif

s32 tree_data[] = {
    -230000000, -230000000, -230000000, -230000547, -229998519, -229998338, -230001199, -229996749,
    -229996348, -230001643, -229995557, -229995012, -230001570, -229995811, -229995310, -230000702,
    -229998212, -229998062, -229999267, -230002117, -230002542, -229997718, -230006381, -230007413,
    -229996507, -230009855, -230011338, -229996086, -230011391, -230012979, -229996824, -230010228,
    -230011414, -229998419, -230007130, -230007534, -230000317, -230003241, -230002722, -230001969,
    -229999705, -229998364, -230002820, -229997666, -229995842, -230002613, -229997527, -229995637,
    -230001717, -229998526, -229996827, -230000517, -230000056, -229998691, -229999402, -230001513,
    -230000504, -229998756, -230002289, -230001546, -229998665, -230002273, -230001701, -229998872,
    -230001871, -230001450, -229999248, -230001247, -230000966, -229999667, -230000568, -230000425,
    -230000000, -230000000, -230000000,
};

EVT_BEGIN(evt_sub_stopwatch)
    LOOP(0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static s32 kusa_data[] = {
    -230000000, -230001918, -230003070, -230002273, -230000794, -230000000, -230000794, -230002273,
    -230003070, -230002501, -230001249, -230000000,
};

WinMgrDesc coinget_window_desc[] = {
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, coingetDisp},
};

#ifdef VERSION_G8ME01
EVT_BEGIN(tree_evt)
    SET_TYPE(0)
    SET_VALUE(LF(0), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(4), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1_LEAVES1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_get_position, LW(3), LW(10), LW(11), LW(12))
        SET_VALUE(LW(14), LW(11))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(15), LW(12))
        SUBTRACT(LW(14), 25)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 0, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(1)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(2)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(3)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(4)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(5)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(6)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
        END_SWITCH
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 0)
        SPAWN_EVENT(LW(5))
    END_IF
    SET_INT_READ_ADDR(PTR(tree_data))
    LOOP(25)
        READ_INT3(LW(10), LW(11), LW(12))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_rotate, 1, LW(2), FLOAT(0.0f), FLOAT(0.0f), LW(10))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), 0)
            CALL(evt_mapobj_trans, 1, LW(3), LW(11), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        IF_INT_NOT_EQUAL(LW(4), 0)
            CALL(evt_mapobj_trans, 1, LW(4), LW(12), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(tree_evt)
    SET_TYPE(0)
    SET_VALUE(LF(0), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(4), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1_LEAVES1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_get_position, LW(3), LW(10), LW(11), LW(12))
        SET_VALUE(LW(14), LW(11))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(15), LW(12))
        SUBTRACT(LW(14), 25)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 0, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(1)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(2)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(3)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(4)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(5)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(6)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
        END_SWITCH
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 0)
        SPAWN_EVENT(LW(5))
    END_IF
    SET_INT_READ_ADDR(PTR(tree_data))
    LOOP(25)
        READ_INT3(LW(10), LW(11), LW(12))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_rotate, 1, LW(2), FLOAT(0.0f), FLOAT(0.0f), LW(10))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), 0)
            CALL(evt_mapobj_trans, 1, LW(3), LW(11), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        IF_INT_NOT_EQUAL(LW(4), 0)
            CALL(evt_mapobj_trans, 1, LW(4), LW(12), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

EVT_BEGIN(evt_sub_tree_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT3(LW(3), LW(4), LW(5))
        IF_INT_EQUAL(LW(0), 7)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 13, 1, LW(1), 0, PTR(tree_evt), 0)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(kusa_evt)
    SET_TYPE(0)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(10), LW(11), LW(12), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SPAWN_EVENT(LW(3))
    END_IF
    SET_INT_READ_ADDR(PTR(kusa_data))
    LOOP(12)
        READ_INT(LW(10))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_trans, 1, LW(2), LW(10), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(kusa_evt)
    SET_TYPE(0)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(10), LW(11), LW(12), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SPAWN_EVENT(LW(3))
    END_IF
    SET_INT_READ_ADDR(PTR(kusa_data))
    LOOP(12)
        READ_INT(LW(10))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_trans, 1, LW(2), LW(10), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

EVT_BEGIN(evt_sub_kusa_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT(LW(3))
        IF_INT_EQUAL(LW(0), 4)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 9, 1, LW(1), 0, PTR(kusa_evt), 0)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_sub_starstone)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_pouch_get_starstone, 1)
        CALL(evt_pouch_add_item, 114, LW(2))
        CASE_EQUAL(2)
        CALL(evt_pouch_get_starstone, 2)
        CALL(evt_pouch_add_item, 115, LW(2))
        CASE_EQUAL(3)
        CALL(evt_pouch_get_starstone, 3)
        CALL(evt_pouch_add_item, 116, LW(2))
        CASE_EQUAL(4)
        CALL(evt_pouch_get_starstone, 4)
        CALL(evt_pouch_add_item, 117, LW(2))
        CASE_EQUAL(5)
        CALL(evt_pouch_get_starstone, 5)
        CALL(evt_pouch_add_item, 118, LW(2))
        CASE_EQUAL(6)
        CALL(evt_pouch_get_starstone, 6)
        CALL(evt_pouch_add_item, 119, LW(2))
        CASE_EQUAL(7)
        CALL(evt_pouch_get_starstone, 7)
        CALL(evt_pouch_add_item, 120, LW(2))
        CASE_EQUAL(-1)
    END_SWITCH
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LF(1), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SET_VALUE(LF(2), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        SET_VALUE(LF(3), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 4)
        SET_VALUE(LF(4), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 5)
        SET_VALUE(LF(5), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 6)
        SET_VALUE(LF(6), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 7)
        SET_VALUE(LF(7), 1)
    END_IF
    IF_INT_EQUAL(LW(0), -1)
        SET_VALUE(LF(8), 1)
    END_IF
    CALL(set_stop, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(9700)
        CALL(set_stop, 1)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_EVT_STARSTONE_WHITEOUT1"), 0)
    CALL(evt_fade_entry, 10, 300, 255, 255, 255)
    CALL(evt_fade_end_wait)
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
        ELSE
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    END_IF
    CALL(evt_sub_get_sincos, 0, LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(40.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(40.0f))
    ADD_FLOAT(LW(5), LW(2))
    ADD_FLOAT(LW(6), LW(4))
    CALL(evt_item_set_position, LW(1), LW(5), LW(3), LW(6))
    CALL(evt_mario_adjust_dir)
    WAIT_MS(500)
    IF_INT_EQUAL(LF(8), 0)
        CALL(evt_mario_set_pose, STRING("M_N_3"))
    END_IF
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_GET_STARSTONE1"))
    CALL(bgm_start_wait)
    CALL(evt_fade_entry, 9, 300, 255, 255, 255)
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_cam3d_evt_set_npc_rel, STRING("にせマリオ"), 0, 50, 397, 0, 70, 25, 2000, 11)
        ELSE
        CALL(evt_cam3d_evt_set_rel, 0, 50, 397, 0, 70, 25, 2000, 11)
    END_IF
    CALL(evt_snd_sfxon_3d, STRING("SFX_SE2_EVT_STARSTONE_GET1"), LW(5), LW(3), LW(6), 0)
    SET_VALUE(LW(7), LW(1))
    SET_FLOAT_VALUE(LW(8), FLOAT(35.0f))
    SET_FLOAT_VALUE(LW(9), LW(3))
    CALL(evt_sub_intpl_msec_init, 4, 0, 3600, 2833)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        IF_INT_EQUAL(LF(8), 1)
            CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
            ELSE
            CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        END_IF
        CALL(evt_sub_get_sincos, LW(0), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), LW(8))
        MULTIPLY_FLOAT(LW(6), LW(8))
        ADD_FLOAT(LW(5), LW(2))
        ADD_FLOAT(LW(6), LW(4))
        CALL(evt_item_set_position, LW(7), LW(5), LW(9), LW(6))
        CALL(stone_ry, LW(7), LW(0))
        SET_FLOAT_VALUE(LW(2), FLOAT(0.20507813f))
        SET_FLOAT_VALUE(LW(3), FLOAT(0.28222656f))
        CALL(evt_sub_scale_by_fps, LW(2), LW(3))
        SUBTRACT_FLOAT(LW(8), LW(2))
        ADD_FLOAT(LW(9), LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
        ELSE
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    END_IF
    ADD(LW(3), 48)
    CALL(evt_item_set_position, LW(7), LW(2), LW(3), LW(4))
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY1"), LW(0), LW(1), LW(2), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_I_1"))
        ELSE
        CALL(evt_mario_set_pose, STRING("M_P_2"))
    END_IF
    CALL(evt_item_get_position, LW(7), LW(0), LW(1), LW(2))
    ADD(LW(1), 16)
    CALL(evt_eff64, STRING("sub_hikari"), STRING("radiation_n64"), 12, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    ADD(LW(2), -10)
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stone_bg, LW(7))
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_33_18"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_02"), 0, 0)
    END_IF
    LOOP(0)
        CALL(get_stop, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STAGE_CLEAR1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_eff_softdelete, STRING("sub_hikari"))
    CALL(stone_bg_open, LW(7))
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LF(1), 1)
            SET_VALUE(LW(0), STRING("gon_11_S"))
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(3), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(4), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        IF_INT_EQUAL(LF(5), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(6), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(7), 1)
            SET_VALUE(LW(0), STRING("S_aji_14"))
        END_IF
        IF_INT_EQUAL(LF(8), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        WAIT_FRAMES(10)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 1)
    END_INLINE_EVENT
    CALL(evt_fade_entry, 34, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_eff, STRING("sub_clear"), STRING("stageclear"), 10, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stageclear_wait)
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY2"), LW(0), LW(1), LW(2), 0)
    END_IF
    WAIT_MS(2000)
    CALL(evt_fade_entry, 30, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_item_delete, LW(7))
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_34"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_04"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_03"), 0, 0)
    END_IF
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        SET_VALUE(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        SET_VALUE(LW(0), 4)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        SET_VALUE(LW(0), 7)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        SET_VALUE(LW(0), 10)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        SET_VALUE(LW(0), 13)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        SET_VALUE(LW(0), 16)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        SET_VALUE(LW(0), 19)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        SET_VALUE(LW(0), 22)
    END_IF
    SET_VALUE(LF(10), 0)
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
    CALL(evt_mario_init_camid)
    CALL(evt_party_init_camid, 0)
    CALL(evt_party_init_camid, 1)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("usu_00"), 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("muj_12"), 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_fade_set_mapchange_type, 0, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("pik_03"), STRING("next"))
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("pik_01"), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_eff_delete, STRING("sub_bg"))
        CALL(evt_eff_delete, STRING("sub_clear"))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_jin04"), 1)
        CALL(evt_fade_reset, 1)
        CALL(evt_npc_set_camid, STRING("にせマリオ"), 4)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        CALL(evt_fade_entry, 16, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#else
EVT_BEGIN(evt_sub_starstone)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_pouch_get_starstone, 1)
        CALL(evt_pouch_add_item, 114, LW(2))
        CASE_EQUAL(2)
        CALL(evt_pouch_get_starstone, 2)
        CALL(evt_pouch_add_item, 115, LW(2))
        CASE_EQUAL(3)
        CALL(evt_pouch_get_starstone, 3)
        CALL(evt_pouch_add_item, 116, LW(2))
        CASE_EQUAL(4)
        CALL(evt_pouch_get_starstone, 4)
        CALL(evt_pouch_add_item, 117, LW(2))
        CASE_EQUAL(5)
        CALL(evt_pouch_get_starstone, 5)
        CALL(evt_pouch_add_item, 118, LW(2))
        CASE_EQUAL(6)
        CALL(evt_pouch_get_starstone, 6)
        CALL(evt_pouch_add_item, 119, LW(2))
        CASE_EQUAL(7)
        CALL(evt_pouch_get_starstone, 7)
        CALL(evt_pouch_add_item, 120, LW(2))
        CASE_EQUAL(-1)
    END_SWITCH
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LF(1), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SET_VALUE(LF(2), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        SET_VALUE(LF(3), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 4)
        SET_VALUE(LF(4), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 5)
        SET_VALUE(LF(5), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 6)
        SET_VALUE(LF(6), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 7)
        SET_VALUE(LF(7), 1)
    END_IF
    IF_INT_EQUAL(LW(0), -1)
        SET_VALUE(LF(8), 1)
    END_IF
    CALL(set_stop, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(9700)
        CALL(set_stop, 1)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_EVT_STARSTONE_WHITEOUT1"), 0)
    CALL(evt_fade_entry, 10, 300, 255, 255, 255)
    CALL(evt_fade_end_wait)
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
        ELSE
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    END_IF
    CALL(evt_sub_get_sincos, 0, LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(40.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(40.0f))
    ADD_FLOAT(LW(5), LW(2))
    ADD_FLOAT(LW(6), LW(4))
    CALL(evt_item_set_position, LW(1), LW(5), LW(3), LW(6))
    CALL(evt_mario_adjust_dir)
    WAIT_MS(500)
    IF_INT_EQUAL(LF(8), 0)
        CALL(evt_mario_set_pose, STRING("M_N_3"))
    END_IF
    CALL(evt_fade_entry, 9, 300, 255, 255, 255)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_GET_STARSTONE1"))
    CALL(bgm_start_wait)
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_cam3d_evt_set_npc_rel, STRING("にせマリオ"), 0, 50, 397, 0, 70, 25, 2000, 11)
        ELSE
        CALL(evt_cam3d_evt_set_rel, 0, 50, 397, 0, 70, 25, 2000, 11)
    END_IF
    CALL(evt_snd_sfxon_3d, STRING("SFX_SE2_EVT_STARSTONE_GET1"), LW(5), LW(3), LW(6), 0)
    SET_VALUE(LW(7), LW(1))
    SET_FLOAT_VALUE(LW(8), FLOAT(35.0f))
    SET_FLOAT_VALUE(LW(9), LW(3))
    CALL(evt_sub_intpl_init, 4, 0, 3600, 170)
    LOOP(171)
        CALL(evt_sub_intpl_get_value)
        IF_INT_EQUAL(LF(8), 1)
            CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
            ELSE
            CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        END_IF
        CALL(evt_sub_get_sincos, LW(0), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), LW(8))
        MULTIPLY_FLOAT(LW(6), LW(8))
        ADD_FLOAT(LW(5), LW(2))
        ADD_FLOAT(LW(6), LW(4))
        CALL(evt_item_set_position, LW(7), LW(5), LW(9), LW(6))
        CALL(stone_ry, LW(7), LW(0))
        SUBTRACT_FLOAT(LW(8), FLOAT(0.20507813f))
        ADD_FLOAT(LW(9), FLOAT(0.28222656f))
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY1"), LW(0), LW(1), LW(2), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_I_1"))
        ELSE
        CALL(evt_mario_set_pose, STRING("M_P_2"))
    END_IF
    CALL(evt_item_get_position, LW(7), LW(0), LW(1), LW(2))
    ADD(LW(1), 16)
    CALL(evt_eff64, STRING("sub_hikari"), STRING("radiation_n64"), 12, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    ADD(LW(2), -10)
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stone_bg, LW(7))
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_33_18"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_02"), 0, 0)
    END_IF
    LOOP(0)
        CALL(get_stop, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STAGE_CLEAR1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_eff_softdelete, STRING("sub_hikari"))
    CALL(stone_bg_open, LW(7))
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LF(1), 1)
            SET_VALUE(LW(0), STRING("gon_11_S"))
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(3), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(4), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        IF_INT_EQUAL(LF(5), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(6), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(7), 1)
            SET_VALUE(LW(0), STRING("S_aji_14"))
        END_IF
        IF_INT_EQUAL(LF(8), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        WAIT_FRAMES(10)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), 1)
    END_INLINE_EVENT
    CALL(evt_fade_entry, 34, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_eff, STRING("sub_clear"), STRING("stageclear"), 10, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stageclear_wait)
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY2"), LW(0), LW(1), LW(2), 0)
    END_IF
    WAIT_MS(2000)
    CALL(evt_fade_entry, 30, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_item_delete, LW(7))
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_34"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_04"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_03"), 0, 0)
    END_IF
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        SET_VALUE(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        SET_VALUE(LW(0), 4)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        SET_VALUE(LW(0), 7)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        SET_VALUE(LW(0), 10)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        SET_VALUE(LW(0), 13)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        SET_VALUE(LW(0), 16)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        SET_VALUE(LW(0), 19)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        SET_VALUE(LW(0), 22)
    END_IF
    SET_VALUE(LF(10), 0)
    CALL_CHILD(PTR(evt_memcard_save))
    CALL(evt_mario_init_camid)
    CALL(evt_party_init_camid, 0)
    CALL(evt_party_init_camid, 1)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("usu_00"), 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("muj_12"), 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_fade_set_mapchange_type, 0, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("pik_03"), STRING("next"))
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
        CALL(evt_bero_mapchange, STRING("pik_01"), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_snd_bgmoff_f, 512, 2000)
        CALL(evt_snd_envoff_f, 512, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, 17, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_eff_delete, STRING("sub_bg"))
        CALL(evt_eff_delete, STRING("sub_clear"))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_jin04"), 1)
        CALL(evt_fade_reset, 1)
        CALL(evt_npc_set_camid, STRING("にせマリオ"), 4)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        CALL(evt_fade_entry, 16, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif

EVT_BEGIN(evt_sub_mail_vibration)
    LOOP(10)
        LOOP(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(50)
            CALL(evt_sub_rumble_onoff, 1, 0)
            WAIT_MS(50)
        END_LOOP
        CALL(evt_sub_rumble_onoff, 2, 0)
        WAIT_MS(2000)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(mail_evt)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_mario_wait_rideon)
    CALL(evt_mario_balloon_bikkuri)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION3"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_win_one_message, 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    IF_INT_EQUAL(GSWF(37), 0)
        SET_VALUE(GSWF(37), 1)
        CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
        ELSE
        IF_INT_EQUAL(GSWF(38), 0)
            CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()
#else
EVT_BEGIN(mail_evt)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_mario_wait_rideon)
    CALL(evt_mario_balloon_bikkuri)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION3"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_win_one_message, 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    IF_INT_EQUAL(GSWF(37), 0)
        SET_VALUE(GSWF(37), 1)
        CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
        ELSE
        IF_INT_EQUAL(GSWF(38), 0)
            CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()
#endif

EVT_BEGIN(mail_evt_nok_01)
    IF_INT_EQUAL(GSWF(7), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 190, -2000, 190, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 27)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(7), 1)
    CALL(evt_pouch_receive_mail, 16)
    CALL(evt_pouch_receive_mail, 26)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_03)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -100, -2000, -100, 2000, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, 125)
                IF_INT_EQUAL(GSWF(8), 0)
                    SET_VALUE(GSWF(8), 1)
                    CALL(evt_pouch_receive_mail, 17)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(PTR(mail_evt))
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_02)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -280, -50, -230, 220, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, 175)
                IF_INT_EQUAL(GSWF(9), 0)
                    SET_VALUE(GSWF(9), 1)
                    CALL(evt_pouch_receive_mail, 18)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(PTR(mail_evt))
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_01)
    IF_INT_EQUAL(GSWF(10), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 350, -2000, 350, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 228)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(10), 1)
    CALL(evt_pouch_receive_mail, 19)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_04)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 30, -2000, 30, 2000, LW(5))
        CALL(mail_evt_gor_04_keyoff_check, LW(3))
        IF_INT_GREATER(LW(3), 0)
            SET_VALUE(LW(5), 0)
        END_IF
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, 280)
                IF_INT_EQUAL(GSWF(11), 0)
                    SET_VALUE(GSWF(11), 1)
                    CALL(evt_pouch_receive_mail, 20)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(PTR(mail_evt))
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(mail_evt_rsh_03_a)
    IF_INT_EQUAL(GSWF(12), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -100, -2000, -100, 2000, LW(5))
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 328)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(12), 1)
    CALL(evt_pouch_receive_mail, 21)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()
#else
EVT_BEGIN(mail_evt_rsh_03_a)
    IF_INT_EQUAL(GSWF(12), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 400, -2000, 400, 2000, LW(5))
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 328)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(12), 1)
    CALL(evt_pouch_receive_mail, 21)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()
#endif

EVT_BEGIN(mail_evt_hom_00)
    IF_INT_EQUAL(GSWF(13), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -600, -2000, -600, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 325)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(13), 1)
    CALL(evt_pouch_receive_mail, 22)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_02_2)
    IF_INT_EQUAL(GSWF(14), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -1000, -4226, 1000, 7374, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 348)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(14), 1)
    CALL(evt_pouch_receive_mail, 23)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_bom_01)
    IF_INT_EQUAL(GSWF(15), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -230, -2000, -230, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 354)
            IF_INT_EQUAL(GSWF(3874), 1)
                IF_INT_EQUAL(LW(5), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(15), 1)
    CALL(evt_pouch_receive_mail, 24)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_tik_00)
    IF_INT_EQUAL(GSWF(16), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 470, -2000, 470, 2000, LW(5))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -500, -2000, -500, 2000, LW(6))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 378)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(6), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(16), 1)
    CALL(evt_pouch_receive_mail, 25)
    CALL(evt_pouch_receive_mail, 33)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_win_01)
    IF_INT_EQUAL(GSWF(17), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 390, -2000, 390, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 87)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(17), 1)
    CALL(evt_pouch_receive_mail, 27)
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_tou_01)
    IF_INT_EQUAL(GSWF(18), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -50, -2000, -50, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 137)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(18), 1)
    CALL(evt_pouch_receive_mail, 28)
    SET_VALUE(GSWF(250), 1)
    CALL(evt_sub_save_playtime, GSW(5))
    CALL_CHILD(PTR(mail_evt))
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_usu_01)
    IF_INT_EQUAL(GSWF(19), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 110, -2000, 110, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 187)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(19), 1)
    CALL(evt_pouch_receive_mail, 29)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_01_2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -2000, 200, 2000, 200, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, 270)
                IF_INT_EQUAL(GSWF(20), 0)
                    SET_VALUE(GSWF(20), 1)
                    CALL(evt_pouch_receive_mail, 30)
                    SET_VALUE(GSWF(251), 1)
                    CALL(evt_sub_save_playtime, GSW(14))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(mail_evt)
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_rsh_03_a_2)
    IF_INT_EQUAL(GSWF(21), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 400, -2000, 400, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 303)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(21), 1)
    CALL(evt_pouch_receive_mail, 31)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_pik_00)
    IF_INT_EQUAL(GSWF(22), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -60, -2000, -60, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 354)
            IF_INT_EQUAL(GSWF(3874), 1)
                IF_INT_EQUAL(LW(5), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(22), 1)
    CALL(evt_pouch_receive_mail, 32)
    SET_VALUE(GSWF(252), 1)
    CALL(evt_sub_save_playtime, GSW(23))
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_sub_mail_event)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        RETURN
    END_IF
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nok_01"))
        CALL_CHILD(PTR(mail_evt_nok_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_03"))
        CALL_CHILD(PTR(mail_evt_gor_03))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_02"))
        SPAWN_EVENT(PTR(mail_evt_gor_02))
        CALL_CHILD(PTR(mail_evt_gor_02_2))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_01"))
        SPAWN_EVENT(PTR(mail_evt_gor_01_2))
        CALL_CHILD(PTR(mail_evt_gor_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL_CHILD(PTR(mail_evt_gor_04))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
        CALL_CHILD(PTR(mail_evt_rsh_03_a))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_03_a"))
        CALL_CHILD(PTR(mail_evt_rsh_03_a_2))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL_CHILD(PTR(mail_evt_hom_00))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("bom_01"))
        CALL_CHILD(PTR(mail_evt_bom_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tik_00"))
        CALL_CHILD(PTR(mail_evt_tik_00))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("win_01"))
        CALL_CHILD(PTR(mail_evt_win_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tou_01"))
        CALL_CHILD(PTR(mail_evt_tou_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("usu_01"))
        CALL_CHILD(PTR(mail_evt_usu_01))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL_CHILD(PTR(mail_evt_pik_00))
        RETURN
    END_IF
    RETURN
EVT_END()
#else
EvtWord evt_sub_mail_event[] = {
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        RETURN
    END_IF
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nok_01"))
        CALL_CHILD(mail_evt_nok_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_03"))
        CALL_CHILD(mail_evt_gor_03)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_02"))
        SPAWN_EVENT(mail_evt_gor_02)
        CALL_CHILD(mail_evt_gor_02_2)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_01"))
        SPAWN_EVENT(mail_evt_gor_01_2)
        CALL_CHILD(mail_evt_gor_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
        CALL_CHILD(mail_evt_gor_04)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_03_a"))
        CALL_CHILD(mail_evt_rsh_03_a_2)
        CALL_CHILD(mail_evt_rsh_03_a)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL_CHILD(mail_evt_hom_00)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("bom_01"))
        CALL_CHILD(mail_evt_bom_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tik_00"))
        CALL_CHILD(mail_evt_tik_00)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("win_01"))
        CALL_CHILD(mail_evt_win_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tou_01"))
        CALL_CHILD(mail_evt_tou_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("usu_01"))
        CALL_CHILD(mail_evt_usu_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL_CHILD(mail_evt_pik_00)
        RETURN
    END_IF
    RETURN
};
#endif

#pragma force_active on
#ifndef VERSION_G8ME01
__declspec(section ".data") s32 evt_sub_mail_event_end[] = {
    1,
};
#endif
#pragma force_active reset

USER_FUNC(evt_sub_intpl_init) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 start = evtGetValue(event, args[1]);
    s32 end = evtGetValue(event, args[2]);
    s32 steps = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = start;
    event->lwData[13] = end;
    event->lwData[14] = 0;
    event->lwData[15] = steps;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_get_value) {
    event->lwData[0] = intplGetValue(event->lwData[11], event->lwData[14], event->lwData[15], event->lwData[12], event->lwData[13]);
    if (event->lwData[14] >= event->lwData[15]) {
        event->lwData[1] = 0;
    } else {
        event->lwData[1] = 1;
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_get_value_para) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], intplGetValue(event->lwData[11], event->lwData[14], event->lwData[15], event->lwData[12], event->lwData[13]));
    if (event->lwData[14] >= event->lwData[15]) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_init_float) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    f32 start = evtGetFloat(event, args[1]);
    f32 end = evtGetFloat(event, args[2]);
    s32 steps = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = FLOAT(start);
    event->lwData[13] = FLOAT(end);
    event->lwData[14] = 0;
    event->lwData[15] = steps;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_get_float) {
    EvtWord* args = event->args;
    s32 out = args[0];
    s32 done = args[1];
    f32 start = evtGetFloat(event, LW(12));
    f32 end = evtGetFloat(event, LW(13));
    f32 v = intplGetValue(event->lwData[11], event->lwData[14], event->lwData[15], start, end);
    if (out != EVT_NULLPTR) {
        evtSetFloat(event, out, v);
    }
    if (done != EVT_NULLPTR) {
        if (event->lwData[14] >= event->lwData[15]) {
            evtSetValue(event, done, 0);
        } else {
            evtSetValue(event, done, 1);
        }
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_msec_init) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 start = evtGetValue(event, args[1]);
    s32 end = evtGetValue(event, args[2]);
    s32 msec = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = start;
    event->lwData[13] = end;
    event->lwData[15] = msec;
    event->savedTime = event->lifetime;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_msec_get_value) {
    s32 ms = (u64)(s32)((s32)event->lifetime - (s32)event->savedTime) / (OS_TIMER_CLOCK / 1000);
    if (ms < event->lwData[15]) {
        event->lwData[0] = intplGetValue(event->lwData[11], ms, event->lwData[15], event->lwData[12], event->lwData[13]);
        event->lwData[1] = 1;
    } else {
        event->lwData[0] = event->lwData[13];
        event->lwData[1] = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_intpl_msec_get_value_para) {
    EvtWord* args = event->args;
    s32 ms = (u64)(s32)((s32)event->lifetime - (s32)event->savedTime) / (OS_TIMER_CLOCK / 1000);
    if (ms < event->lwData[15]) {
        evtSetFloat(event, args[0], intplGetValue(event->lwData[11], ms, event->lwData[15], event->lwData[12], event->lwData[13]));
        evtSetValue(event, args[1], 1);
    } else {
        evtSetFloat(event, args[0], event->lwData[13]);
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_spline_init) {
    EvtWord* args = event->args;
    s32 manual = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    Vec* pts = (Vec*)evtGetValue(event, args[2]);
    s32 n = evtGetValue(event, args[3]);
    s32 steps = evtGetValue(event, args[4]);
    s32 msec = evtGetValue(event, args[5]);
    SplineWork* w = _mapAlloc(sizeof(SplineWork));
    event->lwData[15] = (EvtWord)w;
    w->n = n;
    w->ts = _mapAlloc(n * 4);
    w->pts = pts;
    w->tbl = _mapAlloc(n * 12);
    spline_maketable(n, w->pts, w->ts, w->tbl);
    w->manual = manual;
    w->mode = mode;
    w->cur = 0;
    w->steps = steps;
    w->msec = msec;
    w->start = event->lifetime;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_spline_get_value) {
    EvtWord* args = event->args;
    SplineWork* w = (SplineWork*)event->lwData[15];
    f32 el;
    f32 v[3];
    f32 t;
    if (!w->manual) {
        t = INTPL_F(w->mode, w->cur, w->steps, 0.0f, 1.0f);
    } else {
        el = (u64)(event->lifetime - w->start) / (OS_TIMER_CLOCK / 1000);
        t = INTPL_F(w->mode, (s32)el, w->msec, 0.0f, 1.0f);
    }
    spline_getvalue(v, t, w->n, w->pts, w->ts, w->tbl);
    evtSetValue(event, args[0], FLOAT(v[0]));
    evtSetValue(event, args[1], FLOAT(v[1]));
    evtSetValue(event, args[2], FLOAT(v[2]));
    w->cur++;
    if (!w->manual) {
        if (w->cur <= w->steps) {
            evtSetValue(event, args[3], 1);
            return EVT_RETURN_DONE;
        }
    } else if (el <= (f32)w->msec) {
        evtSetValue(event, args[3], 1);
        return EVT_RETURN_DONE;
    }
    _mapFree(w->ts);
    _mapFree(w->tbl);
    _mapFree(w);
    evtSetValue(event, args[3], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_spline_get_value_manual) {
    EvtWord* args = event->args;
    SplineWork* w = (SplineWork*)event->lwData[15];
    f32 t = evtGetValue(event, args[0]);
    f32 v[3];
    t = t;
    t = t / (w->manual ? (f32)w->msec : (f32)w->steps);
    spline_getvalue(v, t, w->n, w->pts, w->ts, w->tbl);
    evtSetValue(event, args[1], FLOAT(v[0]));
    evtSetValue(event, args[2], FLOAT(v[1]));
    evtSetValue(event, args[3], FLOAT(v[2]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_spline_free) {
    SplineWork* w = (SplineWork*)event->lwData[15];
    _mapFree(w->ts);
    _mapFree(w->tbl);
    _mapFree(w);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_sincos) {
    EvtWord* args = event->args;
    f32 rad = (3.141592f * evtGetFloat(event, args[0])) / 180.0f;
    f32 s = (f32)sin(rad);
    evtSetValue(event, args[1], FLOAT(s));
    s = (f32)cos(rad);
    evtSetValue(event, args[2], FLOAT(s));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_rumble_onoff) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 chan = evtGetValue(event, args[1]);
    switch (mode) {
    case 0:
        padRumbleOn(chan);
        break;
    case 1:
        padRumbleOff(chan);
        break;
    case 2:
        padRumbleHardOff(chan);
        break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_random) {
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], rand() % (n + 1));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_stopwatch) {
    EvtWord* args = event->args;
    s64* sw = (s64*)evtGetPtrID(evtGetValue(event, args[0]));
    s32 ms = *sw / (OS_TIMER_CLOCK / 1000);
    if (ms > 600000) {
        ms = 600000;
    }
    evtSetValue(event, args[1], ms);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_area_check) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    f32 a1 = evtGetValue(event, args[1]);
    f32 a2 = evtGetValue(event, args[2]);
    f32 a3 = evtGetValue(event, args[3]);
    f32 a4 = evtGetValue(event, args[4]);
    f32 a5 = evtGetValue(event, args[5]);
    s32 result;
    switch (mode) {
    case 0: {
        f32 dist = distABf(a1, a2, a4, a5);
        if (a3 < dist) {
            result = 0;
        } else {
            result = 1;
        }
        break;
    }
    case 1:
        result = 1;
        if (a1 - a3 > a4) {
            result = 0;
        }
        if (a1 + a3 < a4) {
            result = 0;
        }
        if (a2 - a3 > a5) {
            result = 0;
        }
        if (a2 + a3 < a5) {
            result = 0;
        }
        break;
    default:
        result = 0;
        break;
    }
    evtSetValue(event, args[6], result);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_dist) {
    EvtWord* args = event->args;
    f32 x1 = evtGetValue(event, args[0]);
    f32 y1 = evtGetValue(event, args[1]);
    f32 x2 = evtGetValue(event, args[2]);
    f32 y2 = evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], distABf(x1, y1, x2, y2));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetValue(event, args[0]);
    f32 y1 = evtGetValue(event, args[1]);
    f32 x2 = evtGetValue(event, args[2]);
    f32 y2 = evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], angleABf(x1, y1, x2, y2));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_system_flag) {
    evtSetValue(event, event->args[0], gp->flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_system_flag_onoff) {
    EvtWord* args = event->args;
    s32 onoff = evtGetValue(event, args[0]);
    s32 flag = args[1];
    if (onoff) {
        gp->flags |= flag;
    } else {
        gp->flags &= ~flag;
    }
    return EVT_RETURN_DONE;
}

KEYFN(evt_key_get_dir, keyGetDir)

KEYFN(evt_key_get_button, keyGetButton)

KEYFN(evt_key_get_buttontrg, keyGetButtonTrg)

USER_FUNC(evt_sub_get_mapname) {
    evtSetValue(event, event->args[0], (EvtWord)gp->currentMapName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_areaname) {
    evtSetValue(event, event->args[0], (EvtWord)gp->currentAreaName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_status_gauge_force_open) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_status_gauge_force_close) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_status_gauge_check_update) {
    EvtWord* args = event->args;
    if (statusWinCheckUpdate()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
USER_FUNC(N_evt_sub_status_gauge_force_update) {
    statusWinForceUpdate();
    return EVT_RETURN_DONE;
}
#endif

USER_FUNC(evt_sub_save_playtime) {
    s32 base = event->args[0]; // a GSW(n) script value: base + EVTDAT_GSW_BASE is n
    u8 buf[8];
    *(s64*)buf = gp->retraceLocalTime;
    swByteSet(base + EVTDAT_GSW_BASE + 0, buf[0]);
    swByteSet(base + EVTDAT_GSW_BASE + 1, buf[1]);
    swByteSet(base + EVTDAT_GSW_BASE + 2, buf[2]);
    swByteSet(base + EVTDAT_GSW_BASE + 3, buf[3]);
    swByteSet(base + EVTDAT_GSW_BASE + 4, buf[4]);
    swByteSet(base + EVTDAT_GSW_BASE + 5, buf[5]);
    swByteSet(base + EVTDAT_GSW_BASE + 6, buf[6]);
    swByteSet(base + EVTDAT_GSW_BASE + 7, buf[7]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_load_progresstime) {
    EvtWord* args = event->args;
    s32 base = args[0];
    u8 buf[8];
    s64 ms;
    buf[0] = swByteGet(base + EVTDAT_GSW_BASE + 0);
    buf[1] = swByteGet(base + EVTDAT_GSW_BASE + 1);
    buf[2] = swByteGet(base + EVTDAT_GSW_BASE + 2);
    buf[3] = swByteGet(base + EVTDAT_GSW_BASE + 3);
    buf[4] = swByteGet(base + EVTDAT_GSW_BASE + 4);
    buf[5] = swByteGet(base + EVTDAT_GSW_BASE + 5);
    buf[6] = swByteGet(base + EVTDAT_GSW_BASE + 6);
    buf[7] = swByteGet(base + EVTDAT_GSW_BASE + 7);
    ms = (gp->retraceLocalTime - *(s64*)buf) / (OS_TIMER_CLOCK / 1000);
    if (ms > 86400000) {
        ms = 86400000;
    }
    evtSetValue(event, args[1], ms);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_language) {
    evtSetValue(event, event->args[0], gp->language);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_animgroup_async) {
    const char* group = (const char*)evtGetValue(event, event->args[0]);
    s32 r = animGroupBaseAsync(group, gp->inBattle != 0, NULL);
    if (r == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (r != 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(evt_sub_countdown_start) {
    EvtWord* args = event->args;
    s32 limit = evtGetValue(event, args[0]);
    s32 restart = evtGetValue(event, args[1]);
    countDownStart(limit, restart);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_countdown_get_status) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], countDownGetStatus());
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_countdown_end) {
    countDownEnd();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_countdown_flag_onoff) {
    EvtWord* args = event->args;
    s32 onoff = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    if (onoff) {
        countDownFlagOn(flag);
    } else {
        countDownFlagOff(flag);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_countdown_set_restart_time) {
    countDownSetRestartTime(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

void coingetDisp(WinMgrEntry* entry) {
    s32* work = entry->param;
    char buf[0x400];
    if (work[1]) {
#ifdef VERSION_G8ME01
        u16 lines;
        s32 w;

        if ((u32)gp->language == 0) {
            sprintf(buf, msgSearch("msg_window_coin_get"), work[0]);
        } else {
            strcpy(buf, msgSearch("msg_window_coin_get"));
            evtMsgFmtInt(buf, work[0]);
        }
        w = FontGetMessageWidthLine(buf, &lines) + 20;
        if (w > entry->width) {
            entry->width = w;
            entry->x = -entry->width / 2;
        }
        entry->height = entry->description->height + lines * 24;
#else
        sprintf(buf, msgSearch("msg_window_coin_get"), work[0]);
#endif
        FontDrawStart();
        FontDrawColor(coin_black);
        FontDrawMessage(-(s32)FontGetMessageWidth(buf) / 2, entry->y - 8, buf);
    }
}

static inline f32 coin_sin(f32 x) { f32 a = 0.00761f; f32 b = 0.16605f; f32 c = 1.0f; f32 y; f32 x2; if (x <= 3.1415927f) { if (x <= 1.5707964f) { x2 = x * x; a = a * x2 - b; a = a * x2 + c; a = a * x; } else { y = 1.5707964f - (x - 1.5707964f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y; } } else if (x < 4.712389f) { y = x - 3.1415927f; x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y; a = -a; } else { y = 1.5707964f - (x - 4.712389f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = a * y; a = -a; } return a; }
static inline f32 coin_cos(f32 x) { f32 a = 0.03705f; f32 b = 0.4967f; f32 c = 1.0f; f32 y; f32 x2; if (x <= 3.1415927f) { if (x <= 1.5707964f) { x2 = x * x; a = a * x2 - b; a = a * x2 + c; } else { y = 1.5707964f - (x - 1.5707964f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = -a; } } else if (x < 4.712389f) { y = x - 3.1415927f; x2 = y * y; a = a * x2 - b; a = a * x2 + c; a = -a; } else { y = 1.5707964f - (x - 4.712389f); x2 = y * y; a = a * x2 - b; a = a * x2 + c; } return a; }
void marioChgGetItemMotion(void);
void marioChgStayMotion(void);
#pragma opt_propagation off
#pragma opt_lifetimes off
USER_FUNC(evt_sub_get_coin) {
    s32 coin = evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    Vec a = mario->position;
    Vec b = a;
    f32 rad = reviseAngle((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);
    f32 s;
    f32 c;
    s32 state;
    EffectEntry* eff;
    s32* work;
    rad = 6.2831855f * rad / 360.0f;
    s = 10.0f * coin_sin(rad);
    c = 10.0f * coin_cos(rad);
    a.x += s;
    a.z -= c;
    a.y += 62.0f;
    b.x += s;
    b.y += 52.0f;
    b.z -= c;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdatap[1] = effItemGetEntry(7, a.x, a.y, a.z);
        event->userdata[2] = 0;
        event->userdata3 = 0;
        event->wInterpWork = _mapAlloc(8);
    }
    state = event->userdata[0];
    eff = event->userdatap[1];
    work = event->wInterpWork;
    switch (state) {
    case 0:
        iconEntry("iraiCoin", ICON_18B);
        iconSetPos("iraiCoin", b.x, b.y, b.z);
        iconFlagOn("iraiCoin", 0x102);
        ((ItemGetWork*)eff->userdata)->camera = 5;
        marioChgGetItemMotion();
        event->userdata3 = winMgrEntry(coinget_window_desc);
        winMgrOpen(event->userdata3);
        winMgrSetParam(event->userdata3, work);
        work[0] = coin;
        work[1] = 0;
        psndBGMOff(0x3800);
        psndBGMOn(0x201, "BGM_FF_GET_ITEM1");
        event->userdata[2] = 200;
        event->savedTime = gp->renderTime;
        event->userdata[0] = 1;
        return EVT_RETURN_BLOCK;
    case 1:
        if ((s32)(((u32)gp->renderTime - (u32)event->savedTime) / (OS_TIMER_CLOCK / 1000)) > event->userdata[2]) {
            iconFlagOff("iraiCoin", 2);
            work[1] = 1;
            event->userdata[2] = 500;
            event->savedTime = gp->renderTime;
            event->userdata[0] = 2;
        }
        return EVT_RETURN_BLOCK;
    case 2:
        if ((s32)(((u32)gp->renderTime - (u32)event->savedTime) / (OS_TIMER_CLOCK / 1000)) > event->userdata[2]) {
            event->userdata[0] = 10;
        }
        return EVT_RETURN_BLOCK;
    case 10:
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            event->userdata[0] = 100;
        }
        return EVT_RETURN_BLOCK;
    case 100:
        ((s16 (*)(s32))pouchAddCoin)(coin);
        effSoftDelete(eff);
        iconDelete("iraiCoin");
        marioChgStayMotion();
        psndBGMOn(0xa0, NULL);
        psndBGMOff(0x201);
        winMgrCloseAutoDelete(event->userdata3);
        work[1] = 0;
        event->userdata[0] = 101;
        return EVT_RETURN_DONE;
    case 101:
        if (!winMgrAction(event->userdata3)) {
            _mapFree(work);
            return EVT_RETURN_DONE;
        }
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

USER_FUNC(stone_ry) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    s32 ang = evtGetValue(event, args[1]);
    EffectEntry* item = itemNameToPtr(name)->effect;
    ((EffStarStoneWork*)item->userdata)->flags &= ~4;
    ((EffStarStoneWork*)item->userdata)->rotY = ang % 360;
    return EVT_RETURN_DONE;
}

USER_FUNC(stone_bg) {
    char* name;
    CameraEntry* cam;
    EffectEntry* eff;
    u8* item;
    name = (char*)evtGetValue(event, event->args[0]);
    cam = camGetPtr(CAMERA_2D);
    cam->letterBoxCamId = CAMERA_3D_EFFECTA;
    eff = effNameToPtr("sub_bg");
    ((ItemGetWork*)eff->userdata)->camera = 5;
    item = (u8*)itemNameToPtr(name);
    ((EffStarStoneWork*)((ItemEntry*)item)->effect->userdata)->cam = 5;
    if (npcNameToPtr_NoAssert("\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49") != NULL && evtGetValue(event, GSW_STORY_PROGRESS) < 211) {
        npcNameToPtr_NoAssert("\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49")->cameraId = 5;
    } else {
        marioSetCamId(CAMERA_3D_EFFECTA);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(stone_bg_open) {
    EffectEntry* e = effNameToPtr("sub_bg");
    ((ItemGetWork*)e->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(stageclear_wait) {
    effNameToPtr("sub_clear");
    return effStageClearEndChk() ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(set_stop) {
    stop = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_stop) {
    evtSetValue(event, event->args[0], stop);
    return EVT_RETURN_DONE;
}

USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

#ifdef VERSION_G8ME01
USER_FUNC(evt_sub_scale_by_fps) {
    EvtWord* args = event->args;
    s32 a0 = args[0];
    f32 x = evtGetFloat(event, a0);
    s32 a1 = args[1];
    f32 y = evtGetFloat(event, a1);
    x *= 60.0f / gp->framerate;
    y *= 60.0f / gp->framerate;
    evtSetFloat(event, a0, x);
    evtSetFloat(event, a1, y);
    return EVT_RETURN_DONE;
}
#endif

USER_FUNC(evt_sub_check_intersect) {
    EvtWord* args = event->args;
    f32 a, b, c, d, e, f, g, h;
    f32 t1, t2;
    a = evtGetFloat(event, args[0]);
    b = evtGetFloat(event, args[1]);
    c = evtGetFloat(event, args[2]);
    d = evtGetFloat(event, args[3]);
    e = evtGetFloat(event, args[4]);
    f = evtGetFloat(event, args[5]);
    g = evtGetFloat(event, args[6]);
    h = evtGetFloat(event, args[7]);
    if (a >= c) {
        if ((a < e && a < g) || (c > e && c > g)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    } else {
        if ((c < e && c < g) || (a > e && a > g)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    }
    if (b >= d) {
        if ((b < f && b < h) || (d > f && d > h)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    } else {
        if ((d < f && d < h) || (b > f && b > h)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    }
    t1 = (a - c) * (f - b) + (b - d) * (a - e);
    t2 = (a - c) * (h - b) + (b - d) * (a - g);
    if (t1 * t2 > 0.0f) {
        evtSetValue(event, args[8], 0);
        return EVT_RETURN_DONE;
    }
    t1 = (e - g) * (b - f) + (f - h) * (e - a);
    t2 = (e - g) * (d - f) + (f - h) * (e - c);
    if (t1 * t2 > 0.0f) {
        evtSetValue(event, args[8], 0);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[8], 1);
    return EVT_RETURN_DONE;
}

#define IRAI_CHECK(flag, done, base, mail)                                    \
    if (swGet(flag) && !swGet(done)) {                                        \
        for (i = 0; i < 8; i++) {                                             \
            u.b[i] = swByteGet(base + i);                                     \
        }                                                                     \
        if (OSTicksToSeconds(gp->retraceLocalTime - u.t) > 7200) {           \
            ret = 1;                                                          \
            if (arg) {                                                        \
                swSet(done);                                                  \
                pouchReceiveMail(mail);                                       \
            }                                                                 \
        }                                                                     \
    }
USER_FUNC(irai_mail_check) {
    EvtWord* args = event->args;
    s32 arg = evtGetValue(event, args[0]);
    s32 ret = 0;
    s32 i;
    union {
        u8 b[8];
        OSTime t;
    } u;
    IRAI_CHECK(0x1505, 0x17, 0x58d, 0x25)
    IRAI_CHECK(0x150a, 0x18, 0x595, 0x26)
    IRAI_CHECK(0x150b, 0x19, 0x59d, 0x27)
    IRAI_CHECK(0x150d, 0x1a, 0x5a5, 0x28)
    IRAI_CHECK(0x1512, 0x1b, 0x5ad, 0x29)
    IRAI_CHECK(0x1518, 0x1c, 0x5b5, 0x2a)
    evtSetValue(event, args[1], ret);
    return EVT_RETURN_DONE;
}

USER_FUNC(mail_evt_gor_04_keyoff_check) {
    EvtWord* args = event->args;
    s32 a = marioCtrlOffChk();
    s32 b = marioKeyOffChk();
    evtSetValue(event, args[0], b + a);
    return EVT_RETURN_DONE;
}
