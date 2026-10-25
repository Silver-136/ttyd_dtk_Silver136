#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/mapdrv.h"
#include "evt/evt_img.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_light.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"

#include "driver/npcdrv.h"
#include "driver/hitdrv.h"
#include "driver/camdrv.h"
#include "dolphin/mtx.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "driver/animdrv.h"
#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "string.h"
#include "memory.h"
#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "gon.h"

typedef struct CloudWork {
    s32 used;
    s32 state;
    char animName[0x20];
    char imgName[0x20];
    char offscreenName[0x20];
    char groupNames[8][0x20];
    char hitName[0x20];
    s32 limit;
    s32 counter;
    EventEntry* evt;
    void* endEvt;
    s32 timer;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    f32 lastFrame;
} CloudWork;
void mapGrpSetOffScreen(const char* name, const char* offscreen);
static CloudWork* cloud;
static u8 cloud_once_flag;
static void _mapsetofs_all(CloudWork* wp);
static void _mapclsofs_all(CloudWork* wp);
static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _img_alloc_capture(const char* name, s32 shadowId, const char* offName, s32 allocMode, s32 x, s32 y, s32 w, s32 h);

static USER_FUNC(evt_cloud_main);
USER_FUNC(evt_iri_16_cloud_ent);
USER_FUNC(evt_iri_16_cloud_init);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfxon_3d_ex);
static USER_FUNC(gonbaba_wait_landon);

EvtWord evt_sound[8];
static EvtWord door01_close[22];
static EvtWord door01_open[22];
static BeroEntry bero_entry_data[2];
static EvtWord gonbaba_standup_shake[23];
static EvtWord gonbaba_fog_change[87];
static EvtWord gonbaba_down_shake[83];
EvtWord gonbaba_event[1410];
static const char* capturemap[2];
static Vec v_list[8];
static EvtWord evt_cloud_main_evt[4];
EvtWord iri_16_box_evt[33];
EvtWord iri_16_cloud_evt[36];
EvtWord iri_16_touch_evt[2];
static EvtWord iri_16_init[57];
EvtWord gon_11_init_evt[101];

EVT_BEGIN(evt_sound)
    CALL(evt_snd_bgmoff, 513)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        13, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)evt_sound, // exitEvent
        "gon_10", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {0},
};

static USER_FUNC(gonbaba_wait_landon) {
    // ゴンババ
    return npcNameToPtr("\203S\203\223\203o\203o")->groundHit ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static EVT_BEGIN(gonbaba_standup_shake)
    WAIT_MS(3700)
    CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_STEP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 800)
    RETURN
EVT_END()

static EVT_BEGIN(gonbaba_fog_change)
    CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    SET_VALUE(GW(0), LW(5))
    SET_VALUE(GW(1), LW(6))
    CONVERT_MS_TO_FRAME(LW(10), 8000)
    ADD(LW(10), 1)
    BEGIN_INLINE_EVENT
        LOOP(LW(10))
            CALL(evt_map_set_fog, LW(4), GW(0), GW(1), LW(7), LW(8), LW(9))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_init, 11, 0, 400, LW(10))
        LOOP(LW(10))
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(GW(0), LW(0))
            ADD(GW(0), LW(5))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_init, 11, 0, 1600, LW(10))
    LOOP(LW(10))
        CALL(evt_sub_intpl_get_value)
        SET_VALUE(GW(1), LW(0))
        ADD(GW(1), LW(6))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(gonbaba_down_shake)
    CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_STG1_GNB_DOWN1"), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(8))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 90, 0, 10000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_snd_sfx_vol, LW(8), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_snd_sfxoff, LW(8))
    END_INLINE_EVENT
    CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_sub_intpl_init, 11, 50, 5, 50)
    LOOP(0)
        CALL(evt_sub_intpl_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 50)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
    RETURN
EVT_END()

EVT_BEGIN(gonbaba_event)
    CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
    CALL(evt_npc_pera_onoff, STRING("ゴンババ"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), 512)
    CALL(evt_map_set_fog, 2, 400, 1000, 0, 10, 40)
    CALL(evt_map_fog_onoff, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    IF_INT_EQUAL(GSWF(1498), 1)
        GOTO(100)
    END_IF
    CALL(evt_cam3d_evt_set, -480, 150, 459, -480, 67, -13, 0, 11)
    CALL(evt_mario_set_pos, -560, 10, 0)
    CALL(evt_party_set_pos, 0, -600, 10, 0)
    CALL(evt_seq_wait, 2)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(door01_close))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_cam_shake, 4, FLOAT(0.0146484375f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 100)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -490, 0, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -450, 0, FLOAT(120.0f))
    SPAWN_EVENT(PTR(bero_case_entry))
    WAIT_MS(1300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_19"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_GONBABA_FLY2"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG1_GNB_ROAR2"), -900, 30, 490, 255, 255, 4300, 16, 0)
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_U_1"))
        CALL(evt_npc_wait_anim, STRING("ゴンババ"), FLOAT(1.0f))
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_S_1"))
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(gonbaba_standup_shake))
    SPAWN_EVENT(PTR(gonbaba_fog_change))
    CALL(evt_cam3d_evt_set, -900, 31, 490, -205, 175, 20, 4500, 11)
    BEGIN_INLINE_EVENT
        WAIT_MS(1800)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door10"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kesu"), 1)
    END_INLINE_EVENT
    WAIT_MS(4000)
    LOOP(30)
        CALL(evt_cam3d_evt_set, -890, 33, 483, -205, 175, 20, 15, 11)
        WAIT_MS(15)
        CALL(evt_cam3d_evt_set, -900, 31, 490, -205, 175, 20, 15, 11)
        WAIT_MS(15)
    END_LOOP
    WAIT_MS(4500)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_19_2"), 0, STRING("ゴンババ"))
    CALL(evt_cam3d_evt_set, -545, -14, 417, -225, 227, 12, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door10"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kesu"), 1)
    WAIT_MS(1000)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_20"), 0, STRING("ゴンババ"))
    CALL(evt_cam3d_evt_set, 77, 455, 380, -147, 293, 183, 0, 11)
    WAIT_MS(500)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_20_2"), 0, STRING("ゴンババ"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -325, 30, 650, -350, 125, -33, 3000, 11)
    WAIT_MS(2500)
    CALL(evt_npc_set_battle_info, STRING("ゴンババ"), 20)
    CALL(evt_npc_battle_start, STRING("ゴンババ"))
    CALL(evt_npc_wait_battle_end)
    LABEL(100)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_map_fog_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("baba"), 1, 1)
    CALL(evt_cam3d_evt_set, 0, 30, 842, 0, 135, 150, 0, 11)
    CALL(evt_light_mario_lighting_onoff, 0)
    CALL(evt_light_party_lighting_onoff, 0)
    CALL(evt_light_npc_lighting_onoff, 0, STRING("ノコタロウ父"))
    CALL(evt_light_npc_lighting_onoff, 0, STRING("クリスチーヌ"))
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_position, STRING("ゴンババ"), 0, 70, -100)
    CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_1"))
    CALL(evt_mario_set_pos, 25, 10, 150)
    CALL(evt_party_set_pos, 0, -25, 10, 150)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    SPAWN_EVENT(PTR(gonbaba_down_shake))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), 131088)
        CALL(gonbaba_wait_landon)
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_2"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 25, 423, 0, 55, 150, 5000, 11)
    WAIT_MS(5000)
    CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_3"))
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_3"))
            CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(500)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(2000)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(166)
            WAIT_MS(2333)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_msg_print_party, STRING("stg1_gon_33_01"))
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(800)
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("stg1_gon_33_02"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_toge, 2, 220, 160, -160)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_03"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("eff1"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_cam3d_evt_set, 255, 220, 648, 255, 170, 150, 1000, 11)
    WAIT_MS(1000)
    DELETE_EVENT(LW(10))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT1_1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT1_2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT1_2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT1_2"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1100)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT1_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_4"))
    WAIT_MS(2750)
    SET_VALUE(GW(10), 30)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_Z_2"))
        CALL(evt_npc_flag_onoff, 0, STRING("ノコタロウ父"), 1024)
        CALL(evt_npc_pera_onoff, STRING("ノコタロウ父"), 0)
        SET_VALUE(LW(0), 0)
        LOOP(0)
            ADD(LW(0), GW(10))
            CALL(evt_npc_set_ry, STRING("ノコタロウ父"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 90)
    CALL(evt_npc_set_position, STRING("ノコタロウ父"), 280, 60, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT2"), 280, 60, 0, LW(10))
        CALL(evt_npc_jump_position_nohit, STRING("ノコタロウ父"), 275, 10, 200, 2000, 500)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_LANDING1"), 275, 10, 200, 0)
        CALL(evt_npc_get_position, STRING("ノコタロウ父"), LW(0), LW(1), LW(2))
        ADD(LW(1), 7)
        ADD(LW(2), 10)
        SET_VALUE(LW(3), LW(0))
        SUBTRACT(LW(3), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(3), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(3), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_3"))
    CALL(evt_cam3d_evt_set, 255, 105, 648, 255, 55, 150, 500, 11)
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("ノコタロウ父"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_NOKOPAPA_OUT3"), LW(0), LW(1), LW(2), 0)
    LOOP(0)
        SUBTRACT(GW(10), 1)
        IF_INT_EQUAL(GW(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(2)
    END_LOOP
    DELETE_EVENT(LW(10))
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_S_1"))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ノコタロウ父"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ノコタロウ父"), LW(0), LW(1), LW(2), 300, 50)
    CALL(evt_npc_pera_onoff, STRING("ノコタロウ父"), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ父"), 1024)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_04"), 0, STRING("ノコタロウ父"))
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 2)
        CALL(evt_party_stop, 0)
        SET_VALUE(GSWF(1500), 1)
    END_IF
    CALL(evt_mario_set_pos, 25, 10, 200)
    CALL(evt_party_set_pos, 0, -25, 10, 200)
    IF_INT_EQUAL(GSWF(1500), 1)
        CALL(evt_npc_set_position, STRING("クリスチーヌ"), -10, 10, 160)
        CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), 32768)
        CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
        CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), 2)
    END_IF
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_05"), 0, STRING("party"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 270)
    END_INLINE_EVENT
    WAIT_MS(500)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_06"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 0, 75, 450, 0, 50, 200, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_NOKO_SAIKAI1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_07"), 0, STRING("party"))
    CALL(evt_cam3d_evt_set, 240, 75, 450, 240, 50, 200, 1000, 11)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 215, 200, FLOAT(80.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_08"), 0, STRING("ノコタロウ父"))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    IF_INT_EQUAL(GSWF(1500), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_move_position, STRING("クリスチーヌ"), 150, 200, 0, FLOAT(80.0f), 1)
        END_INLINE_EVENT
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 180, 220, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_09"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_10"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 500, 380, 856, 250, 130, 190, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_11"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_12"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 220, 75, 450, 220, 50, 200, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_13"), 0, STRING("party"))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ノコタロウ父"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 270, 75, 450, 270, 50, 200, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_14"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 245, 75, 450, 245, 50, 200, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_A_1"))
    CALL(evt_item_entry, STRING("stone"), 114, 245, 45, 220, 17, -1, 0)
    WAIT_MS(500)
    CALL(evt_item_get_position, STRING("stone"), LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(3), FLOAT(-1.0f))
    SET_FLOAT_VALUE(LW(4), FLOAT(-0.39941406f))
    SET_VALUE(LF(0), 0)
    CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_A_2"))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_S_1"))
    END_INLINE_EVENT
    LOOP(0)
        ADD_FLOAT(LW(1), LW(3))
        ADD_FLOAT(LW(3), LW(4))
        CALL(evt_item_set_position, STRING("stone"), LW(0), LW(1), LW(2))
        IF_FLOAT_LESS(LW(1), FLOAT(10.0f))
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(0), LW(1), LW(2), 0)
                MULTIPLY_FLOAT(LW(3), FLOAT(-0.69921875f))
                SET_FLOAT_VALUE(LW(1), FLOAT(10.0f))
                SET_VALUE(LF(0), 1)
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(0), LW(1), LW(2), 0)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_item_set_position, STRING("stone"), LW(0), FLOAT(10.0f), LW(2))
    CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_15"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_16"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 190, 75, 450, 190, 50, 200, 500, 11)
    WAIT_MS(600)
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_17"), 0, STRING("ノコタロウ父"))
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 245, 75, 450, 245, 50, 245, 2000, 11)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 245, 215, FLOAT(30.0f))
    END_INLINE_EVENT
    WAIT_MS(2000)
    SET_VALUE(GSW_STORY_PROGRESS, 55)
    SET_VALUE(GSW_STORY_PROGRESS, 56)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_envoff_f, 512, 2000)
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(1), STRING("stone"))
    CALL_CHILD(PTR(evt_sub_starstone))
    RETURN
EVT_END()

static const char* capturemap[] = {
    "gon_11_S",
    "\0",
};

static Vec v_list[] = {
    {-50.0f, 100.0f, 50.0f},
    {50.0f, 100.0f, 50.0f},
    {50.0f, 0.0f, 50.0f},
    {-50.0f, 0.0f, 50.0f},
    {-50.0f, 100.0f, -50.0f},
    {50.0f, 100.0f, -50.0f},
    {50.0f, 0.0f, -50.0f},
    {-50.0f, 0.0f, -50.0f},
};

static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

static void _img_alloc_capture(const char* name, s32 shadowId, const char* offName, s32 allocMode, s32 x, s32 y, s32 w, s32 h) {
    EventEntry event;
    EvtWord args[10];

    event.args = args;
    args[0] = (EvtWord)name;
    args[1] = shadowId;
    args[2] = (EvtWord)offName;
    args[3] = allocMode;
    args[4] = x;
    args[5] = y;
    args[6] = w;
    args[7] = h;
    evt_img_alloc_capture(&event, TRUE);
}

static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h) {
    Vec pos;
    Vec screen;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;

    hitObjGetPos(name, &pos);
    PSVECAdd(&pos, &v_list[0], &screen);
    getScreenPoint(&screen, &screen);
    {
        s32 sx = screen.x;
        s32 sy = screen.y;

        minX = sx;
        maxX = sx;
        minY = sy;
        maxY = sy;
    }
    {
        s32 i;

        for (i = 1; i < 8; i++) {
            PSVECAdd(&pos, &v_list[i], &screen);
            getScreenPoint(&screen, &screen);
            if (screen.x < minX) {
                minX = screen.x;
            }
            if (maxX < screen.x) {
                maxX = screen.x;
            }
            if (screen.y < minY) {
                minY = screen.y;
            }
            if (maxY < screen.y) {
                maxY = screen.y;
            }
        }
    }
    *x = minX + 304;
    *y = minY + 240;
    *w = maxX - minX;
    *h = maxY - minY;
    if (*x + *w > 608) {
        *w = 608 - *x;
    }
    if (*y + *h > 480) {
        *h = 480 - *y;
    }
    if (*x < 0) {
        *x = 0;
    }
    if (*y < 0) {
        *y = 0;
    }
    return TRUE;
}

static void _mapsetofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpSetOffScreen(wp->groupNames[i], wp->offscreenName);
    }
}

static void _mapclsofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpClearOffScreen(wp->groupNames[i]);
    }
}

static s32 evt_cloud_main(EventEntry *event, BOOL isFirstCall) {
    HitEntry *hit0;
    HitEntry *hit1;
    HitEntry *hit2;
    HitEntry *hit3;
    MarioWork *mario;
    CloudWork *wp;
    PartyEntry *party;
    PartyEntry *ptmp;
    HitEntry *hit;
    OffscreenEntry *off;
    ImageEntry *img;
    BOOL inBattle;
    wp = (CloudWork*)event->lwData[0];
    inBattle = gp->inBattle != 0;
    mario = marioGetPtr();
    ptmp = partyGetPtr(0);
    if (isFirstCall) {
        return 0;
    }
    if (ptmp == NULL) {
        return 0;
    }
    hit0 = (HitEntry*)cloudGetHitBreathout(0);
    hit1 = (HitEntry*)cloudGetHitBreathout(1);
    hit2 = (HitEntry*)cloudGetHitBreathout(2);
    hit3 = (HitEntry*)cloudGetHitBreathout(3);
    switch (wp->state) {
        case 0:
            if (((s32) (cloud_once_flag & 0xFFFFFFFFFFFFFFFFu)) == 1) // FAKE: no-op 64-bit mask; needed for 100% (plain cloud_once_flag == 1 gives 97.9-98.1)
          {
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if ((((hit0 == hit) || (hit1 == hit)) || (hit2 == hit)) || (hit3 == hit)) {
                wp->state++;
            }
            wp->counter = 0;
            break;

        case 1:
            if ((mario->displayDirectionTarget > 90.0f) && ((mario->displayDirectionTarget <= 270.0f) ^ 0)) {
                strcpy(wp->animName, "P_oof_cloud_2");
            } else {
                strcpy(wp->animName, "P_oof_cloud_1");
            }
            if (!animGroupBaseAsync(wp->animName, inBattle, (void*)0)) {
                break;
            }
            cloud_once_flag = 1;
            animPaperPoseEntry(wp->animName, inBattle);
            offscreenEntry(wp->offscreenName);
            off = offscreenNameToPtr(wp->offscreenName);
            off->texFmt = (off->copyFmt = 4);
            _mapsetofs_all(wp);
            wp->state++;
            break;

        case 2:
            imgEntry(wp->imgName, inBattle);
            img = imgNameToPtr(wp->imgName, inBattle);
            img->poseId = animPaperPoseGetId(wp->animName, inBattle);
            *(&img->animTime) = animTimeGetTime(inBattle);
            imgClearVirtualPoint(img);
            imgSetShadow(img, 2);
            img->flags |= 2;
            wp->state = 222;
            break;

        case 222:
            if (!_get_boundingbox(wp->hitName, &wp->x, &wp->y, &wp->w, &wp->h)) {
                break;
            }
            _img_alloc_capture(wp->imgName, 0, wp->offscreenName, 1, wp->x, wp->y, wp->w, wp->h);
            wp->state = 223;
            break;

        case 223:
            img = imgNameToPtr(wp->imgName, inBattle);
            img->flags &= ~2;
            wp->state = 3;

        case 3:
            img = imgNameToPtr(wp->imgName, inBattle);
            img->animName = "A_1";
            *(&img->animTime) = animTimeGetTime(inBattle);
            img->flags &= ~2;
            wp->timer = 30;
            wp->lastFrame = 0.0f;
            wp->state++;
            break;

        case 4: {
                img = imgNameToPtr(wp->imgName, inBattle);
                if ((wp->lastFrame == 0.0f) || ((wp->lastFrame + 1.0f) < img->loopTimes)) {
                    psndSFXOn_3D("SFX_OFF_CLAUD_WIND2", &mario->position);
                    wp->lastFrame = img->loopTimes;
                }
                if (mario->currMotionId != MARIO_MOTION_CLOUD) {
                    wp->state = 10;
                    break;
                }
                hit = hitNameToPtr(wp->hitName);
                if ((((hit0 != hit) && (hit1 != hit)) && (hit2 != hit)) && (hit3 != hit)) {
                    if (wp->timer == 0) {
                        wp->state = 10;
                    } else {
                        wp->timer--;
                        wp->counter += 2;
                    }
                } else {
                    wp->timer = 30;
                    wp->counter += 2;
                    if (wp->counter > wp->limit) {
                        wp->state = 20;
                    }
                }
                break;
            }

        case 10:
            wp->state = 99;
            break;

        case 20:
            psndSFXOn_3D("SFX_OFF_CLAUD_WIND3", &mario->position);
            if (wp->endEvt != NULL) {
                evtEntry(wp->endEvt, 0, 0);
            }
            img = imgNameToPtr(wp->imgName, inBattle);
            img->animName = "A_2";
            *(&img->animTime) = animTimeGetTime(inBattle);
            wp->timer = 60;
            wp->state++;
            break;

        case 21:
            if (wp->timer != 0) {
                wp->timer--;
                break;
            }
            wp->timer = 30;
            wp->counter += 2;
            wp->state = 100;
            break;

        case 99:

        case 100:
            img = imgNameToPtr(wp->imgName, inBattle);
            _mapclsofs_all(wp);
            animPaperPoseRelease(img->poseId);
            imgFreeCapture(img, 0);
            imgSetShadow(img, 0);
            imgRelease(img, inBattle);
            offscreenDelete(wp->offscreenName);
            cloud_once_flag = 0;
            if (wp->state == 99) {
                wp->state = 0;
                break;
            }
            return 2;

    }

    return 0;
}

USER_FUNC(evt_iri_16_cloud_init) {
    s32 i;

    cloud = _mapAlloc(sizeof(CloudWork) * 16);
    for (i = 0; i < 16; i++) {
        cloud[i].used = 0;
        sprintf(cloud[i].imgName, "i_name_%1d", i);
        sprintf(cloud[i].offscreenName, "o_name_%1d", i);
    }
    cloud_once_flag = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_iri_16_cloud_ent) {
    EvtWord* args = event->args;
    char** groups;
    char* hitName;
    void* endEvt;
    CloudWork* wp;
    s32 i;
    s32 j;

    groups = (char**)evtGetValue(event, args[0]);
    hitName = (char*)evtGetValue(event, args[1]);
    endEvt = (void*)evtGetValue(event, args[2]);

    for (i = 0; i < 16; i++) {
        if (cloud[i].used == 0) {
            break;
        }
    }
    if (i >= 16) {
        return EVT_RETURN_DONE;
    }
    wp = &cloud[i];
    wp->used = 1;
    strcpy(wp->animName, "P_oof_cloud_1");
    for (j = 0; j < 8; j++) {
        if ((s8)groups[j][0] == 0) {
            break;
        }
        strcpy(wp->groupNames[j], groups[j]);
    }
    strcpy(wp->hitName, hitName);
    wp->endEvt = endEvt;
    wp->limit = 180;
    wp->counter = 0;
    wp->state = 0;
    wp->timer = 0;
    wp->evt = evtEntry(evt_cloud_main_evt, 0, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(iri_16_box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("mb"), 110, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("mb"))
    CALL(evt_item_get_item_end_wait, STRING("mb"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(iri_16_cloud_evt)
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, 0, 0, PTR(iri_16_box_evt), GSWF(5407))
    CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    SET_VALUE(GSWF(5406), 1)
    RETURN
EVT_END()

EVT_BEGIN(iri_16_touch_evt)
    RETURN
EVT_END()

static EVT_BEGIN(iri_16_init)
    IF_INT_EQUAL(GSW(1420), 16)
        CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
        IF_INT_EQUAL(GSWF(5406), 0)
            CALL(evt_iri_16_cloud_init)
            CALL(evt_iri_16_cloud_ent, PTR(&capturemap), STRING("A_kura"), PTR(iri_16_cloud_evt))
            CALL(evt_run_case_evt, 9, 1, STRING("A_kura"), 0, PTR(iri_16_touch_evt), 0)
            ELSE
            CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
            CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, 0, 0, PTR(iri_16_box_evt), GSWF(5407))
        END_IF
        ELSE
        CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(gon_11_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(55)
        CASE_DEFAULT
        CALL_CHILD(PTR(evt_bero_info_run))
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(55)
        CALL(evt_snd_bgmoff, 512)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG1_GON1"))
    END_SWITCH
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_GON4"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ父"), 1536)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), 32)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 55)
        CALL(evt_npc_set_position, STRING("ゴンババ"), 0, -1000, 0)
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
        ELSE
        CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), 33554432)
        CALL(evt_npc_set_scale, STRING("ゴンババ"), FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(1.2998047f))
        SPAWN_EVENT(PTR(gonbaba_event))
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), 64)
    CALL(evt_light_charalight_all_onoff, 1)
    CALL(evt_light_npc_lighting_onoff, 1, STRING("ゴンババ"))
    CALL_CHILD(PTR(iri_16_init))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 55)
        CALL(evt_npc_delete, STRING("ノコタロウ父"))
        CALL(evt_npc_delete, STRING("ゴンババ"))
    END_IF
    RETURN
EVT_END()
