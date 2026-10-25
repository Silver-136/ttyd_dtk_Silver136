#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "mri_puni.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"

USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);

EvtWord toge_7_init[17];
EvtWord toge_7_talk[44];
EvtWord toge_8_init[17];
EvtWord toge_8_talk[44];
EvtWord toge_9_init[17];
EvtWord toge_9_talk[44];
EvtWord toge_10_init[17];
EvtWord toge_10_regl[19];
EvtWord toge_10_talk[44];
EvtWord toge_11_init[17];
EvtWord toge_11_regl[19];
EvtWord toge_11_talk[44];
static NpcSetupInfo npcEnt[6];
static EvtWord patolight_sound_off[15];
static BeroEntry bero_entry_data[3];
static EvtWord puni_win_offscreen[104];
static EvtWord puni_win[219];
static EvtWord puni_lose[86];
static EvtWord puni_winlose_check[44];
EvtWord toge_enter_sub[51];
static EvtWord toge_enter[50];
static EvtWord puni_vs_toge_battlewait[36];
static EvtWord puni_vs_toge_init[86];
static EvtWord puni_vs_toge[136];
static KusaAccess ueki_access_data[4];
EvtWord mri_11_init_evt[192];

EVT_BEGIN(toge_7_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -220, 0, 110)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_7_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_040"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_041"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_040_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_041_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_8_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -75, 0, -100)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_8_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_042"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_043"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_042_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_043_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_9_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 70, 0, 80)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_9_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_044"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_045"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_044_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_045_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_10_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 180, 0, 60)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_10_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(toge_10_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_046"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_047"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_046_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_047_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_11_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -70, 0, -35)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_11_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(toge_11_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_048"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_049"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_048_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_049_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"トゲ族７"*/"\x83\x67\x83\x51\x91\xB0\x82\x56", // name
        0x600, // flags
        0, // reactionFlags
        toge_7_init, // initEvent
        NULL, // regularEvent
        toge_7_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族８"*/"\x83\x67\x83\x51\x91\xB0\x82\x57", // name
        0x600, // flags
        0, // reactionFlags
        toge_8_init, // initEvent
        NULL, // regularEvent
        toge_8_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族９"*/"\x83\x67\x83\x51\x91\xB0\x82\x58", // name
        0x600, // flags
        0, // reactionFlags
        toge_9_init, // initEvent
        NULL, // regularEvent
        toge_9_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族１０"*/"\x83\x67\x83\x51\x91\xB0\x82\x50\x82\x4F", // name
        0x600, // flags
        0, // reactionFlags
        toge_10_init, // initEvent
        toge_10_regl, // regularEvent
        toge_10_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族１１"*/"\x83\x67\x83\x51\x91\xB0\x82\x50\x82\x50", // name
        0x600, // flags
        0, // reactionFlags
        toge_11_init, // initEvent
        toge_11_regl, // regularEvent
        toge_11_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(patolight_sound_off)
    IF_INT_EQUAL(GW(1), 1)
        SET_VALUE(GW(1), 0)
        CALL(evt_snd_sfxoff, GW(0))
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        patolight_sound_off, // exitEvent
        "mri_12", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        patolight_sound_off, // exitEvent
        "mri_06", // nextMapName
        "w_bero02", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(puni_win_offscreen)
    CALL(evt_paper_entry, STRING("OFF_d_bom"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_hati"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_patolight1"), STRING("offscreen"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, 0, 0, 0, 0)
    WAIT_MS(1000)
    SET_VALUE(GW(1), 0)
    CALL(evt_snd_sfxoff, GW(0))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF2_TORIDE_BREAK2"), -400, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hati"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_patolight1"), 1)
    CALL(evt_hitobj_onoff, STRING("A_hati"), 1, 0)
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_hati"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_patolight1"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

static EVT_BEGIN(puni_win)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_winddisable_onoff, 1)
    LOOP(0)
        CALL(evt_puni_landon_check, 0, FLOAT(0.0f), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mri_puni_win_init)
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 500, 11)
    WAIT_MS(500)
    CALL(evt_puni_flag_onoff, 1, 0, 131072)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), 512)
    CALL(evt_puni_get_flag, 1, LW(0))
    AND(LW(0), 16384)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_puni_flag_onoff, 1, 1, 16384)
        CALL(evt_npc_set_anim, STRING("ガイドsp"), STRING("S_1"))
        CALL(evt_npc_get_position, STRING("ガイドsp"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("ガイドsp"), LW(0), LW(1), LW(2), 300, 20)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01_04"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, 262144)
    CALL(evt_cam3d_evt_set, 275, 140, 516, -275, 50, 0, 1000, 11)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mri_puni_win_attack, -350, 0, 0, -450, 0, 0)
    WAIT_MS(5000)
    LOOP(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TORIDE_BREAK1"), -400, 0, 0, 0)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 500)
        WAIT_MS(500)
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mri_puni_win_end, -450, 0, 0, -350, 0, 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(puni_win_offscreen))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG2_MRI2"), 3000)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSWF(2857), 1)
    CALL(evt_party_run, 0)
    CALL(evt_puni_winddisable_onoff, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(puni_lose)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 500, 11)
    WAIT_MS(500)
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01_05"), 0, 0)
    SET_VALUE(GW(1), 0)
    CALL(evt_snd_sfxoff, GW(0))
    CALL(evt_puni_force_se_off)
    SET_VALUE(GSWF(2856), 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 500, -1, -1)
    CALL(evt_bero_mapchange, STRING("mri_06"), STRING("w_bero02"))
    RETURN
EVT_END()

static EVT_BEGIN(puni_winlose_check)
    LOOP(0)
        CALL(evt_puni_zenmetsu_check, 0, LW(10))
        CALL(evt_puni_zenmetsu_check, 1, LW(11))
        IF_INT_EQUAL(LW(10), 1)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(LW(11), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LW(10), 1)
        SPAWN_EVENT(PTR(puni_lose))
        RETURN
    END_IF
    IF_INT_EQUAL(LW(11), 1)
        SPAWN_EVENT(PTR(puni_win))
        CALL(evt_hitobj_onoff, STRING("A_gra_l"), 1, 1)
        RETURN
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(toge_enter_sub)
    SET_VALUE(LW(13), LW(10))
    ADD(LW(13), 1)
    SET_VALUE(LW(14), LW(10))
    ADD(LW(14), 4)
    CALL(evt_mri_toge_make_retsu, -450, 0, 0, LW(10), LW(13), LW(14))
    CALL(evt_puni_set_position, LW(10), -450, 0, 0)
    CALL(evt_puni_move_position2, LW(10), -330, 0, FLOAT(100.0f))
    CALL(evt_puni_move_position2, LW(10), -300, LW(12), FLOAT(100.0f))
    CALL(evt_puni_move_position2, LW(10), -200, LW(12), FLOAT(100.0f))
    CALL(evt_mri_toge_retsu_stop, LW(10), LW(13), LW(14))
    RETURN
EVT_END()

static EVT_BEGIN(toge_enter)
    SET_VALUE(LW(10), 110)
    SET_VALUE(LW(11), 10)
    LOOP(0)
        WAIT_MS(500)
        SET_VALUE(LW(12), 14)
        MULTIPLY(LW(12), LW(11))
        SPAWN_EVENT(PTR(toge_enter_sub))
        ADD(LW(10), 5)
        WAIT_MS(500)
        SET_VALUE(LW(12), -14)
        MULTIPLY(LW(12), LW(11))
        SUBTRACT(LW(12), -14)
        SPAWN_EVENT(PTR(toge_enter_sub))
        ADD(LW(10), 5)
        ADD(LW(11), -1)
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    RETURN
EVT_END()

static EVT_BEGIN(puni_vs_toge_battlewait)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), -100)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_puni_attack_on)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01_03"), 0, 0)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(puni_winlose_check))
    RETURN
EVT_END()

static EVT_BEGIN(puni_vs_toge_init)
    CALL(evt_puni_wait_systemmode, 2)
    CALL(evt_mri_toge_enter_init)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_all_movestop, 1)
    SET_VALUE(GW(1), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SIREN1"), -400, 0, 0, GW(0))
    CALL(evt_map_playanim, STRING("light_02"), 1, 0)
    CALL(evt_cam3d_evt_set, 275, 140, 516, -275, 50, 0, 1000, 11)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    WAIT_MS(1000)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL_CHILD(PTR(toge_enter))
    CALL(evt_puni_set_mode, 1, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNITOGE1"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_puni_all_movestop, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(puni_vs_toge_battlewait))
    RETURN
EVT_END()

static EVT_BEGIN(puni_vs_toge)
    CALL(evt_puni_wait_systemmode, 2)
    CALL(evt_mri_toge_enter_init)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_all_movestop, 1)
    SET_VALUE(GW(1), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SIREN1"), -400, 0, 0, GW(0))
    CALL(evt_map_playanim, STRING("light_02"), 1, 0)
    CALL(evt_cam3d_evt_set, 275, 140, 516, -275, 50, 0, 1000, 11)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    WAIT_MS(1000)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL_CHILD(PTR(toge_enter))
    CALL(evt_puni_flag_onoff, 1, 0, 262144)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), 512)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNITOGE1"))
    CALL(evt_npc_set_ry_lr, STRING("ガイドsp"), 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01_01"), 0, STRING("ガイドsp"))
    CALL(evt_cam3d_evt_set_rel, 0, 60, 345, 0, 40, 0, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01_02"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, 262144)
    CALL(evt_puni_set_mode, 1, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNITOGE1"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSWF(2855), 1)
    CALL(evt_puni_all_movestop, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(puni_vs_toge_battlewait))
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki03", "S_ueki_03", NULL},
    {1, "A_ueki04", "S_ueki_04", NULL},
    {1, "A_ueki05", "S_ueki_05", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_11_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 11, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
    IF_INT_EQUAL(GSWF(2857), 0)
        SET_VALUE(LW(6), 1)
        SET_VALUE(LW(7), -200)
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 30)
        ELSE
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), 0)
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
    END_IF
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI8"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_sibu_01"), 1, 0)
    SET_VALUE(LW(0), 110)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -150)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    IF_INT_EQUAL(GSWF(2855), 0)
        SPAWN_EVENT(PTR(puni_vs_toge))
        ELSE
        IF_INT_EQUAL(GSWF(2857), 0)
            SPAWN_EVENT(PTR(puni_vs_toge_init))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2857), 0)
        CALL(evt_hitobj_onoff, STRING("A_gra_l"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(2857), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hati"), 1)
        CALL(evt_hitobj_onoff, STRING("A_hati"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_patolight1"), 1)
    END_IF
    CALL(evt_hitobj_onoff, STRING("A_sunomae"), 1, 0)
    RETURN
EVT_END()

