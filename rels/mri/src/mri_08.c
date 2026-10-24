#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "evt/evt_npc.h"
#include "data_tables.h"


void togeMakeRetsu(Vec* pos, s32 leader, s32 start, s32 end);

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_event_from_road);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_set_rel);
USER_FUNC(evt_cam3d_get_shift);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_free_capture);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_set_offscreen);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mri_puni_win_attack);
USER_FUNC(evt_mri_puni_win_end);
USER_FUNC(evt_mri_puni_win_init);
USER_FUNC(evt_mri_toge_enter_init);
USER_FUNC(evt_mri_toge_make_retsu);
USER_FUNC(evt_mri_toge_retsu_stop);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry_lr);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_offscreen_delete);
USER_FUNC(evt_offscreen_entry);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_puni_all_movestop);
USER_FUNC(evt_puni_attack_on);
USER_FUNC(evt_puni_flag_onoff);
USER_FUNC(evt_puni_force_se_off);
USER_FUNC(evt_puni_get_flag);
USER_FUNC(evt_puni_get_position);
USER_FUNC(evt_puni_landon_check);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_move_position2);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_mode);
USER_FUNC(evt_puni_set_position);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_puni_wait_systemmode);
USER_FUNC(evt_puni_winddisable_onoff);
USER_FUNC(evt_puni_zenmetsu_check);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
USER_FUNC(toge_enter_func00);

f32 fd_mri_08_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_mri_08_neg_one_382;
f32 fd_mri_08___local_PI_383;
f32 fd_mri_08___local_3PIO2_385;
f32 fd_mri_08_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_mri_08___local_PIO2_384;
f32 fd_mri_08_one_381;
f32 fd_mri_08_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_mri_08_neg_one_365;
f32 fd_mri_08___local_PI_366;
f32 fd_mri_08___local_3PIO2_368;
f32 fd_mri_08_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_mri_08___local_PIO2_367;
f32 fd_mri_08_one_364;
EvtWord toge_1_init[17];
EvtWord toge_1_talk[44];
EvtWord toge_2_init[17];
EvtWord toge_2_talk[44];
EvtWord toge_3_init[17];
EvtWord toge_3_talk[44];
EvtWord toge_4_init[17];
EvtWord toge_4_talk[44];
EvtWord toge_5_init[17];
EvtWord toge_5_regl[19];
EvtWord toge_5_talk[44];
EvtWord toge_6_init[17];
EvtWord toge_6_talk[44];
static NpcSetupInfo npcEnt[7];
static EvtWord patolight_sound_off[15];
static BeroEntry bero_entry_data[3];
static EvtWord puni_win_offscreen[104];
static EvtWord puni_win[219];
static EvtWord puni_lose[86];
static EvtWord puni_winlose_check[44];
static EvtWord toge_enter[106];
static EvtWord puni_vs_toge_battlewait[36];
static EvtWord puni_vs_toge_init[86];
static EvtWord puni_vs_toge[140];
static KusaAccess ueki_access_data[6];
EvtWord mri_08_init_evt[190];

f32 fd_mri_08_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_08_neg_one_382 = -1.0f;

f32 fd_mri_08___local_PI_383 = 3.1415927f;

f32 fd_mri_08___local_3PIO2_385 = 4.712389f;

f32 fd_mri_08_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_08___local_PIO2_384 = 1.5707964f;

f32 fd_mri_08_one_381 = 1.0f;

f32 fd_mri_08_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_08_neg_one_365 = -1.0f;

f32 fd_mri_08___local_PI_366 = 3.1415927f;

f32 fd_mri_08___local_3PIO2_368 = 4.712389f;

f32 fd_mri_08_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_08___local_PIO2_367 = 1.5707964f;

f32 fd_mri_08_one_364 = 1.0f;

EVT_BEGIN(toge_1_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -220, 0, 110)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_1_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_028"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_029"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_028_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_029_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_2_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -75, 0, -100)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_2_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_030"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_031"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_030_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_031_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_3_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 70, 0, 80)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_3_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_032"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_033"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_032_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_033_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_4_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 180, 0, 60)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_4_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_034"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_035"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_034_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_035_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_5_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -70, 0, -35)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_5_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(toge_5_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_036"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_037"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_036_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_037_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_6_init)
    SWITCH(GSW(0))
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 300, 0, -60)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(toge_6_talk)
    SWITCH(GSW(0))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_038"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_039"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_038_1"), 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_039_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"トゲ族１"*/"\x83\x67\x83\x51\x91\xB0\x82\x50", // name
        0x600, // flags
        0, // reactionFlags
        toge_1_init, // initEvent
        NULL, // regularEvent
        toge_1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族２"*/"\x83\x67\x83\x51\x91\xB0\x82\x51", // name
        0x600, // flags
        0, // reactionFlags
        toge_2_init, // initEvent
        NULL, // regularEvent
        toge_2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族３"*/"\x83\x67\x83\x51\x91\xB0\x82\x52", // name
        0x600, // flags
        0, // reactionFlags
        toge_3_init, // initEvent
        NULL, // regularEvent
        toge_3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族４"*/"\x83\x67\x83\x51\x91\xB0\x82\x53", // name
        0x600, // flags
        0, // reactionFlags
        toge_4_init, // initEvent
        NULL, // regularEvent
        toge_4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族５"*/"\x83\x67\x83\x51\x91\xB0\x82\x54", // name
        0x600, // flags
        0, // reactionFlags
        toge_5_init, // initEvent
        toge_5_regl, // regularEvent
        toge_5_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲ族６"*/"\x83\x67\x83\x51\x91\xB0\x82\x55", // name
        0x600, // flags
        0, // reactionFlags
        toge_6_init, // initEvent
        NULL, // regularEvent
        toge_6_talk, // talkEvent
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
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)patolight_sound_off, // exitEvent
        "mri_09", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)patolight_sound_off, // exitEvent
        "mri_06", // nextMapName
        "e_bero02", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

USER_FUNC(toge_enter_func00) {
    Vec pos = {450.0f, 0.0f, 0.0f};

    togeMakeRetsu(&pos, 0x6E, 0x6F, 0x70);
    togeMakeRetsu(&pos, 0x71, 0x72, 0x73);
    togeMakeRetsu(&pos, 0x74, 0x75, 0x77);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(puni_win_offscreen)
    CALL(evt_paper_entry, STRING("OFF_d_bom"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_hati"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_patolight"), STRING("offscreen"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, 0, 0, 0, 0)
    WAIT_MS(1000)
    SET_VALUE(GW(1), 0)
    CALL(evt_snd_sfxoff, GW(0))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF2_TORIDE_BREAK2"), 400, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hati"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_patolight"), 1)
    CALL(evt_hitobj_onoff, STRING("A_hati"), 1, 0)
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_hati"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_patolight"))
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
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_01"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, 262144)
    CALL(evt_cam3d_evt_set, -275, 140, 516, 275, 50, 0, 1000, 11)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mri_puni_win_attack, 350, 0, 0, 450, 0, 0)
    WAIT_MS(5000)
    LOOP(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_TORIDE_BREAK1"), 400, 0, 0, 0)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 500)
        WAIT_MS(500)
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mri_puni_win_end, 450, 0, 0, 350, 0, 0)
    END_INLINE_EVENT
    CALL_CHILD(PTR(puni_win_offscreen))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG2_MRI1"), 3000)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSWF(2849), 1)
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
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_02"), 0, 0)
    SET_VALUE(GW(1), 0)
    CALL(evt_snd_sfxoff, GW(0))
    CALL(evt_puni_force_se_off)
    SET_VALUE(GSWF(2848), 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 500, -1, -1)
    CALL(evt_bero_mapchange, STRING("mri_06"), STRING("e_bero02"))
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
        CALL(evt_hitobj_onoff, STRING("A_gra_r"), 1, 1)
        RETURN
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(toge_enter)
    BEGIN_INLINE_EVENT
        CALL(evt_mri_toge_make_retsu, 450, 0, 0, 110, 111, 112)
        SET_VALUE(LW(10), 110)
        CALL(evt_puni_set_position, LW(10), 450, 0, 0)
        CALL(evt_puni_move_position2, LW(10), 309, 0, FLOAT(100.0f))
        CALL(evt_puni_move_position2, LW(10), 275, -40, FLOAT(100.0f))
        CALL(evt_mri_toge_retsu_stop, 110, 111, 112)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        SET_VALUE(LW(10), 113)
        CALL(evt_mri_toge_make_retsu, 450, 0, 0, 113, 114, 115)
        CALL(evt_puni_set_position, LW(10), 450, 0, 0)
        CALL(evt_puni_move_position2, LW(10), 309, 0, FLOAT(100.0f))
        CALL(evt_puni_move_position2, LW(10), 275, 40, FLOAT(100.0f))
        CALL(evt_mri_toge_retsu_stop, 113, 114, 115)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mri_toge_make_retsu, 450, 0, 0, 116, 117, 119)
    SET_VALUE(LW(10), 116)
    CALL(evt_puni_set_position, LW(10), 450, 0, 0)
    CALL(evt_puni_move_position2, LW(10), 300, 0, FLOAT(100.0f))
    CALL(evt_mri_toge_retsu_stop, 116, 117, 119)
    RETURN
EVT_END()

static EVT_BEGIN(puni_vs_toge_battlewait)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 175)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_puni_attack_on)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e17_00_02"), 0, 0)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(puni_winlose_check))
    RETURN
EVT_END()

static EVT_BEGIN(puni_vs_toge_init)
    CALL(evt_puni_wait_systemmode, 2)
    CALL(evt_mri_toge_enter_init)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_all_movestop, 1)
    SET_VALUE(GW(1), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SIREN1"), 400, 0, 0, GW(0))
    CALL(evt_map_playanim, STRING("light_02"), 1, 0)
    CALL(evt_cam3d_evt_set, -275, 140, 516, 275, 50, 0, 1000, 11)
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
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_puni_all_movestop, 1)
    SET_VALUE(GW(1), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SIREN1"), 400, 0, 0, GW(0))
    CALL(evt_map_playanim, STRING("light_02"), 1, 0)
    CALL(evt_cam3d_evt_set, -275, 140, 516, 275, 50, 0, 1000, 11)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    WAIT_MS(1000)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL_CHILD(PTR(toge_enter))
    WAIT_FRAMES(1)
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_puni_flag_onoff, 1, 0, 262144)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), 512)
    CALL(evt_npc_set_ry_lr, STRING("ガイドsp"), 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNITOGE1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_00"), 0, STRING("ガイドsp"))
    CALL(evt_cam3d_evt_set_rel, 0, 60, 345, 0, 40, 0, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e17_00_01"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, 262144)
    CALL(evt_puni_set_mode, 1, 1)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSWF(2847), 1)
    CALL(evt_puni_all_movestop, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(puni_vs_toge_battlewait))
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_08_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 8, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
    IF_INT_EQUAL(GSWF(2849), 0)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), 180)
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
    SWITCH(GSW(0))
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI7"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_sibu_01"), 1, 0)
    SET_VALUE(LW(0), -107)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -150)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    SET_VALUE(GW(1), 0)
    IF_INT_EQUAL(GSWF(2847), 0)
        SPAWN_EVENT(PTR(puni_vs_toge))
        ELSE
        IF_INT_EQUAL(GSWF(2849), 0)
            SPAWN_EVENT(PTR(puni_vs_toge_init))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2849), 0)
        CALL(evt_hitobj_onoff, STRING("A_gra_r"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(2849), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hati"), 1)
        CALL(evt_hitobj_onoff, STRING("A_hati"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_patolight"), 1)
    END_IF
    RETURN
EVT_END()

