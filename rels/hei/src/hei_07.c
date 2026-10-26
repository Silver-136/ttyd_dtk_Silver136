#include "data_tables.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/msgdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/casedrv.h"

#include "driver/mapdrv.h"
#include "pmario_sound.h"
#include "string.h"
#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "evt/evt_bero.h"

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(mario_set_dir);
static void callback(s32 type, void** work);

static BeroEntry bero_entry_data[3];
static EvtWord npc_jump[53];
EvtWord dokan_shake[8];
EvtWord dokan_shake2[20];
EvtWord sekizou_move[136];
EvtWord animation_wait[46];
static EvtWord shake_evt[52];
EvtWord seikai_anime[46];
EvtWord seikai_anime_stop[19];
EvtWord fuseikai_anime[21];
EvtWord fuseikai_anime_stop[19];
static EvtWord evt_down_snd[116];
static EvtWord evt_up_snd_[12];
static EvtWord evt_up_snd__[76];
EvtWord offscreen_to_quiz[101];
static EvtWord offscreen_to_normal[100];
QuizEntry quiz_data[8];
EvtWord a_sekizou_1[1018];
EvtWord hei_07_init_evt[114];

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
        NULL, // exitEvent
        "hei_06", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "hei_10", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(npc_jump)
    CALL(evt_npc_get_position, LW(10), LW(5), LW(6), LW(7))
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, LW(6), 0, 30)
    LOOP(31)
        CALL(evt_sub_intpl_get_value)
        CALL(evt_npc_set_position, LW(10), LW(5), LW(0), LW(7))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_position, LW(10), LW(5), LW(6), LW(7))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_SINNOSUKE_LANDING2"), LW(5), LW(6), LW(7), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_SINNOSUKE_LANDING3"), LW(5), LW(6), LW(7), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 200)
    RETURN
EVT_END()

EVT_BEGIN(dokan_shake)
    CALL(evt_cam_shake, 4, FLOAT(0.001953125f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

EVT_BEGIN(dokan_shake2)
    CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 500)
    CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 500)
    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0f), 500)
    RETURN
EVT_END()

EVT_BEGIN(sekizou_move)
    SPAWN_EVENT_GET_ID(PTR(dokan_shake), LW(10))
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAKE1"), GW(15))
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("DAIZA1"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, GW(15), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_mapobj_get_position, STRING("DAIZA1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_QUAKE2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_cam_shake, 4, FLOAT(0.0390625f), FLOAT(0.0f), 100)
        CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 100)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 100)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("DAIZA1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_DSN_ROUND1"), LW(0), LW(1), LW(2), LW(10))
        WAIT_MS(4000)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_sekizou"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_sekizou"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_sfxoff, GW(15))
    DELETE_EVENT(LW(15))
    CALL(evt_hitobj_onoff, STRING("a_sekizou_1_a"), 0, 0)
    CALL(evt_hitobj_onoff, STRING("a_sekizou_1_b"), 0, 0)
    CALL(evt_hitobj_onoff, STRING("a_sekizou_2"), 0, 1)
    DELETE_EVENT(LW(10))
    SPAWN_EVENT(PTR(dokan_shake2))
    SET_VALUE(LW(0), STRING("dokan"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(animation_wait)
    CALL(evt_map_playanim, LW(0), 0, 0)
    CALL(evt_map_checkanim, LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        MULTIPLY_FLOAT(LW(2), FLOAT(0.7998047f))
        ADD(LW(2), 1)
        WAIT_MS(LW(2))
        IF_STR_EQUAL(LW(0), STRING("anm_kaiten1"))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_QUAKE2"), 110, 50, 0, 0)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("anm_kaiten3"))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_QUAKE2"), 110, 50, 0, 0)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(shake_evt)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 5, 15)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(seikai_anime)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 150)
    CALL(evt_eff, STRING(""), STRING("confetti"), 3, LW(0), LW(1), LW(2), 160, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dennshoku"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dennshoku_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_seikai"), 1, 0)
    WAIT_MS(1800)
    RETURN
EVT_END()

EVT_BEGIN(seikai_anime_stop)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dennshoku"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dennshoku_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_seikai"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(fuseikai_anime)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dennshoku"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dennshoku_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_fuseikai"), 1, 0)
    WAIT_MS(1500)
    RETURN
EVT_END()

EVT_BEGIN(fuseikai_anime_stop)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dennshoku"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dennshoku_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_fuseikai"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_down_snd)
    WAIT_MS(100)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(666)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(333)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(250)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(83)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(183)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(166)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(83)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(150)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    WAIT_MS(133)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    WAIT_MS(33)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    WAIT_MS(50)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(16)
    WAIT_MS(33)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE1"), 0)
    WAIT_MS(83)
    WAIT_MS(50)
    WAIT_MS(16)
    WAIT_MS(33)
    WAIT_MS(33)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    RETURN
EVT_END()

static EVT_BEGIN(evt_up_snd_)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE_MOVE1"), 0)
    WAIT_MS(366)
    CALL(evt_snd_sfxon, STRING("SFX_OFF1_PUZZLE2"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_up_snd__)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(50)
    WAIT_MS(83)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(16)
    WAIT_MS(50)
    WAIT_MS(16)
    WAIT_MS(50)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(16)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(83)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(50)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(16)
    WAIT_MS(16)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(400)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(183)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(83)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(250)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(333)
    SPAWN_EVENT(PTR(evt_up_snd_))
    WAIT_MS(666)
    SPAWN_EVENT(PTR(evt_up_snd_))
    RETURN
EVT_END()

EVT_BEGIN(offscreen_to_quiz)
    CALL(evt_paper_entry, STRING("OFF_d_pazu"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_pazu"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hei_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hei_07_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dennshoku"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dennshoku_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pos, -85, 20, 0)
    CALL(mario_set_dir, 90, 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_party_set_pos, 0, -100, 20, -10)
    END_IF
    SPAWN_EVENT(PTR(evt_down_snd))
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(offscreen_to_normal)
    WAIT_MS(1000)
    CALL(evt_map_playanim, STRING("anm_kuti_1"), 2, 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("hei_07"), STRING("offscreen"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 0, 0, 0, 608, 480)
    SPAWN_EVENT(PTR(evt_up_snd__))
    WAIT_MS(500)
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("hei_07"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hei_07"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hei_07_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, LW(0), 0, LW(2))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_party_cont_onoff, 0, 1)
    END_IF
    RETURN
EVT_END()

QuizEntry quiz_data[] = {
    {"anm_1mon", "stg1_hei_23", "stg1_hei_q_01", 0},
    {"anm_2mon", "stg1_hei_24", "stg1_hei_q_02", 2},
    {"anm_3mon", "stg1_hei_25", "stg1_hei_q_03", 3},
    {"anm_4mon", "stg1_hei_26", "stg1_hei_q_04", 1},
    {"anm_5mon", "stg1_hei_27", "stg1_hei_q_05", 2},
    {"anm_6mon", "stg1_hei_28", "stg1_hei_q_06", 0},
    {"anm_7mon", "stg1_hei_29", "stg1_hei_q_07", 1},
    {"anm_8mon", "stg1_hei_30", "stg1_hei_q_08", 0},
};

EVT_BEGIN(a_sekizou_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(GW(0), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(2000)
            CALL(evt_eff_fukidashi, 0, STRING("fukidashi"), 0, 1, 0, 0, 0, 0, 0, 0, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_QUESTION_MARK1"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAKE1"), GW(10))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(1500)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_N_7"))
        END_INLINE_EVENT
        SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(10))
        WAIT_MS(2000)
        CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 3000, EASING_SINE_IN_OUT)
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            CALL(mario_set_dir, 45, 100)
        END_INLINE_EVENT
        LABEL(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            WAIT_FRAMES(1)
            GOTO(0)
        END_IF
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_snd_sfxoff, GW(10))
        ELSE
        CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 1000, EASING_SINE_IN_OUT)
        BEGIN_INLINE_EVENT
            WAIT_MS(200)
            CALL(mario_set_dir, 45, 100)
        END_INLINE_EVENT
        WAIT_MS(1000)
    END_IF
    WAIT_MS(500)
    SET_VALUE(LW(0), STRING("anm_dossun_1"))
    CALL_CHILD(PTR(animation_wait))
    WAIT_MS(500)
    SET_VALUE(LW(15), 0)
    CALL(evt_msg_toge, 2, 95, 65, 0)
    IF_INT_EQUAL(GW(0), 0)
        CALL(evt_msg_print, 0, STRING("stg1_hei_20"), PTR(callback), 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_hei_20_01"), PTR(callback), 0)
    END_IF
    SET_VALUE(GW(0), 1)
    CALL(evt_msg_select, 0, STRING("stg1_hei_yn_01"))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(0), STRING("anm_kaiten1"))
        CALL_CHILD(PTR(animation_wait))
        CALL(evt_msg_print_add, 0, STRING("stg1_hei_21"))
        SET_VALUE(LW(0), STRING("anm_dossun_2"))
        CALL_CHILD(PTR(animation_wait))
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
        CALL(evt_snd_bgmoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(1))
        RETURN
    END_IF
    SET_VALUE(LW(0), STRING("anm_kaiten1"))
    CALL_CHILD(PTR(animation_wait))
    CALL(evt_msg_print_add, 0, STRING("stg1_hei_22"))
    CALL(evt_map_playanim, STRING("anm_denshoku"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_mamekyuu"), 1, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_QUIZ1"))
    CALL_CHILD(PTR(offscreen_to_quiz))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 83, 591, 0, 100, 91, 1500, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        SET_VALUE(LF(1), 0)
        SET_VALUE(LW(14), 0)
        SET_VALUE(LW(15), 0)
        SET_INT_READ_ADDR(PTR(&quiz_data))
        SET_VALUE(LW(0), LW(11))
        LABEL(10)
        READ_INT4(LW(5), LW(6), LW(7), LW(8))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SUBTRACT(LW(0), 1)
            GOTO(10)
        END_IF
        CALL(evt_map_playanim, LW(5), 1, 0)
        WAIT_MS(400)
        IF_INT_NOT_EQUAL(LW(11), 0)
            CALL(evt_snd_sfxon, STRING("SFX_QUIZ_START1"), 0)
        END_IF
        CALL(evt_msg_toge, 2, 95, 65, 0)
        CALL(evt_msg_print, 0, LW(6), PTR(callback), 0)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_QUIZ2"))
        BEGIN_INLINE_EVENT
            IF_INT_EQUAL(LW(11), 0)
                SET_VALUE(LW(0), STRING("anm_kaiten1"))
                CALL_CHILD(PTR(animation_wait))
                WAIT_MS(400)
            END_IF
        END_INLINE_EVENT
        LABEL(100)
        CALL(evt_msg_select, 0, LW(7))
        IF_INT_EQUAL(LW(0), LW(8))
            SET_VALUE(LF(1), 1)
        END_IF
        CALL(evt_sub_random, 2, LW(0))
        IF_INT_EQUAL(LW(11), 0)
            SET_VALUE(LW(0), 0)
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LW(0), 0)
            SET_VALUE(LF(0), 0)
        END_IF
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg1_hei_31"))
            CALL(evt_msg_select, 0, STRING("stg1_hei_yn_01_01"))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_msg_print_add, 0, STRING("stg1_hei_32"))
                SET_VALUE(LF(1), 0)
                GOTO(100)
            END_IF
        END_IF
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        CALL(evt_msg_continue)
        CALL(evt_msg_toge, 2, 95, 65, 0)
        IF_INT_EQUAL(LF(1), 1)
            IF_INT_EQUAL(LW(12), 4)
                CALL(evt_snd_sfxon, STRING("SFX_GOOD_SOUND2"), 0)
                ELSE
                CALL(evt_snd_sfxon, STRING("SFX_GOOD_SOUND1"), 0)
            END_IF
            CALL_CHILD(PTR(seikai_anime))
            ADD(LW(12), 1)
            SWITCH(LW(12))
                CASE_LESS(4)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_FRAMES(1)
                SET_VALUE(LW(0), STRING("anm_kaiten1"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_msg_print, 0, STRING("stg1_hei_33"), PTR(callback), 0)
                CASE_EQUAL(4)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_FRAMES(1)
                SET_VALUE(LW(0), STRING("anm_kaiten1"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_msg_print, 0, STRING("stg1_hei_34"), PTR(callback), 0)
                SET_VALUE(LF(0), 1)
                CASE_EQUAL(5)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_FRAMES(1)
                SET_VALUE(LW(0), STRING("anm_kaiten3"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_msg_print, 0, STRING("stg1_hei_35"), PTR(callback), 0)
                CALL_CHILD(PTR(offscreen_to_normal))
                CALL(evt_snd_bgmoff, PSND_FADEOUT)
                WAIT_MS(1000)
                CALL(evt_msg_toge, 2, 95, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_35_01"), PTR(callback), 0)
                CALL_CHILD(PTR(sekizou_move))
                SET_VALUE(LW(0), STRING("anm_kaiten1"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_msg_toge, 2, 150, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_41"), PTR(callback), 0)
                SET_VALUE(LW(0), STRING("anm_dossun_2"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
                CALL(evt_snd_bgmoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(1))
                SET_VALUE(GSW_STORY_PROGRESS, STG1_06_QUIZ)
                CALL(evt_exit_case_evt)
                RETURN
            END_SWITCH
            CALL_CHILD(PTR(seikai_anime_stop))
            ELSE
            IF_INT_EQUAL(LW(13), 2)
                CALL(evt_snd_sfxon, STRING("SFX_BAD_SOUND2"), 0)
                ELSE
                CALL(evt_snd_sfxon, STRING("SFX_BAD_SOUND1"), 0)
            END_IF
            CALL_CHILD(PTR(fuseikai_anime))
            ADD(LW(13), 1)
            SWITCH(LW(13))
                CASE_EQUAL(1)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_MS(300)
                SET_VALUE(LW(15), 1)
                CALL(evt_msg_print, 0, STRING("stg1_hei_36"), PTR(callback), 0)
                CASE_EQUAL(2)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_MS(300)
                SET_VALUE(LW(15), 2)
                CALL(evt_msg_print, 0, STRING("stg1_hei_37"), PTR(callback), 0)
                SET_VALUE(LF(0), 1)
                CASE_EQUAL(3)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_MS(300)
                SET_VALUE(LW(15), 1)
                CALL(evt_msg_print, 0, STRING("stg1_hei_38"), PTR(callback), 0)
                BEGIN_INLINE_EVENT
                    WAIT_MS(700)
                    SET_VALUE(LW(0), STRING("anm_kaiten1"))
                    CALL_CHILD(PTR(animation_wait))
                END_INLINE_EVENT
                CALL_CHILD(PTR(offscreen_to_normal))
                CALL(evt_snd_bgmoff, PSND_FADEOUT)
                WAIT_MS(1000)
                SET_VALUE(LW(14), 0)
                SET_VALUE(LW(15), 0)
                CALL(evt_msg_toge, 2, 95, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_38_01"), PTR(callback), 0)
                CALL(evt_npc_set_position, STRING("シンエモン1"), 70, 250, -53)
                SET_VALUE(LW(10), STRING("シンエモン1"))
                SPAWN_EVENT(PTR(npc_jump))
                WAIT_MS(200)
                CALL(evt_npc_set_position, STRING("シンエモン2"), 47, 250, 2)
                SET_VALUE(LW(10), STRING("シンエモン2"))
                SPAWN_EVENT(PTR(npc_jump))
                WAIT_MS(200)
                CALL(evt_npc_set_position, STRING("シンエモン3"), 79, 250, 53)
                SET_VALUE(LW(10), STRING("シンエモン3"))
                SPAWN_EVENT(PTR(npc_jump))
                WAIT_MS(200)
                CALL(evt_npc_set_position, STRING("シンエモン4"), 122, 250, 73)
                SET_VALUE(LW(10), STRING("シンエモン4"))
                SPAWN_EVENT(PTR(npc_jump))
                WAIT_MS(200)
                WAIT_MS(500)
                SET_VALUE(LW(14), 0)
                SET_VALUE(LW(15), 0)
                CALL(evt_msg_toge, 2, 95, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_39"), PTR(callback), 0)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                BEGIN_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING("シンエモン1"), LW(0), LW(2), 1000, 0, 0)
                END_INLINE_EVENT
                WAIT_MS(100)
                BEGIN_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING("シンエモン2"), LW(0), LW(2), 1000, 0, 0)
                END_INLINE_EVENT
                WAIT_MS(100)
                BEGIN_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING("シンエモン3"), LW(0), LW(2), 1000, 0, 0)
                END_INLINE_EVENT
                WAIT_MS(100)
                BEGIN_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING("シンエモン4"), LW(0), LW(2), 1000, 0, 0)
                END_INLINE_EVENT
                WAIT_MS(100)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
                CALL(evt_mario_set_pose, STRING("M_N_7"))
                WAIT_MS(500)
                CALL(evt_npc_set_link, STRING("シンエモン1"), STRING("シンエモン2"))
                CALL(evt_npc_set_link, STRING("シンエモン2"), STRING("シンエモン3"))
                CALL(evt_npc_set_link, STRING("シンエモン3"), STRING("シンエモン4"))
                CALL(evt_npc_set_battle_info, STRING("シンエモン1"), 14)
                CALL(evt_npc_battle_start, STRING("シンエモン1"))
                CALL(evt_npc_wait_battle_end)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
                CALL(evt_npc_set_position, STRING("シンエモン1"), 0, -1000, 0)
                CALL(evt_npc_set_position, STRING("シンエモン2"), 0, -1000, 0)
                CALL(evt_npc_set_position, STRING("シンエモン3"), 0, -1000, 0)
                CALL(evt_npc_set_position, STRING("シンエモン4"), 0, -1000, 0)
                WAIT_MS(1000)
                CALL(evt_cam3d_evt_set, -189, 72, 207, 219, 69, -80, 0, EASING_SINE_IN_OUT)
                WAIT_MS(300)
                SET_VALUE(LW(14), 0)
                SET_VALUE(LW(15), 0)
                CALL(evt_msg_toge, 2, 95, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_40"), PTR(callback), 0)
                CALL_CHILD(PTR(sekizou_move))
                SET_VALUE(LW(0), STRING("anm_kaiten1"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_msg_toge, 2, 150, 65, 0)
                CALL(evt_msg_print, 0, STRING("stg1_hei_41"), PTR(callback), 0)
                SET_VALUE(LW(0), STRING("anm_dossun_2"))
                CALL_CHILD(PTR(animation_wait))
                CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
                CALL(evt_snd_bgmoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(1))
                SET_VALUE(GSW_STORY_PROGRESS, STG1_06_QUIZ)
                CALL(evt_exit_case_evt)
                RETURN
            END_SWITCH
            CALL_CHILD(PTR(fuseikai_anime_stop))
        END_IF
        CALL(evt_cam3d_evt_set, 0, 83, 591, 0, 100, 91, 800, EASING_SINE_IN_OUT)
        WAIT_MS(800)
        ADD(LW(11), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(hei_07_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), STRING("dokan"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("dokan"))
        CALL(evt_snd_bgmoff, PSND_QUICK | PSND_SILENT | PSND_SILENT_LEVEL(1))
        ELSE
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(1))
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI4"))
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_map_playanim, STRING("anm_hokori"), 1, 0)
    CALL(evt_mapobj_trans, 1, STRING("hei_07_2"), 0, -1000, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hei_07_2"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_06_QUIZ)
        CALL(evt_hitobj_onoff, STRING("a_sekizou_2"), 0, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_sekizou_1_a"), 0, PTR(a_sekizou_1), 0)
        SET_VALUE(GW(0), 0)
        ELSE
        CALL(evt_hitobj_onoff, STRING("a_sekizou_2"), 0, 1)
        CALL(evt_hitobj_onoff, STRING("a_sekizou_1_a"), 0, 0)
        CALL(evt_hitobj_onoff, STRING("a_sekizou_1_b"), 0, 0)
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_map_playanim, STRING("anm_sekizou"), 2, 0)
    END_IF
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -120, 60, -75, 0, 0)
    RETURN
EVT_END()


static void callback(s32 type, void** work) {
    EventEntry* evt = ((MsgWinData*)*work)->event;
    BOOL valid;
    f32 frame;

    if (strcmp(*(const char**)*work, "stg1_hei_23") == 0 && type == 3) {
        evt->lwData[14]++;
        if (evt->lwData[14] == 3) {
            psndSFXOn("SFX_QUIZ_START1");
        }
    }
    switch (evt->lwData[15]) {
        case 0:
            switch (type) {
                case 5:
                    mapCheckAnimation("anm_kuti_1", &valid, &frame);
                    if (valid) {
                        mapPlayAnimationLv("anm_kuti_1", 0, 0);
                    }
                    break;
            }
            break;
        case 1:
            switch (type) {
                case 5:
                    mapCheckAnimation("anm_egao", &valid, &frame);
                    if (valid) {
                        mapPlayAnimationLv("anm_egao", 0, 0);
                        evt->lwData[14]++;
                        if (evt->lwData[14] > 1) {
                            evt->lwData[15] = 0;
                        }
                    }
                    break;
            }
            break;
        case 2:
            switch (type) {
                case 5:
                    mapCheckAnimation("anm_kuti_1", &valid, &frame);
                    if (valid) {
                        mapPlayAnimationLv("anm_kuti_1", 0, 0);
                    }
                    break;
                case 3:
                    evt->lwData[14]++;
                    if (evt->lwData[14] >= 2) {
                        mapPlayAnimationLv("anm_egao", 0, 0);
                        evt->lwData[14] = 0;
                        evt->lwData[15] = 1;
                    }
                    break;
            }
            break;
    }
}

USER_FUNC(mario_set_dir) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        event->userdataf[1] = mario->moveDirection;
        event->userdataf[2] = evtGetFloat(event, args[0]) - event->userdataf[1];
        event->userdata3 = evtGetValue(event, args[1]);
        mario->multiTimer = 0;
        if (event->userdataf[2] < -180.0f) {
            event->userdataf[2] = 360.0f + event->userdataf[2];
        }
        if (event->userdataf[2] > 180.0f) {
            event->userdataf[2] -= 360.0f;
        }
    }
    if ((s32)event->userdata3 > 0) {
        mario->multiTimer++;
        mario->moveDirection = reviseAngle(event->userdataf[1] + event->userdataf[2] * (s32)mario->multiTimer / (s32)event->userdata3);
        return (s32)mario->multiTimer >= (s32)event->userdata3;
    }
    mario->moveDirection = reviseAngle(mario->moveDirection + event->userdataf[2]);
    return EVT_RETURN_DONE;
}

