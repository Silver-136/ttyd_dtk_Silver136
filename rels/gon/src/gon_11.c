// gon_11 (ゴンババ, Hooktail Castle): Hooktail's (Gonbaba's) lair: cutscenes, the Hooktail boss fight (story STG1_16_HOOKTAIL, STG1_17_FATHER_SON_REUNITED) and the Diamond Star.
// Also a chest that gives an Attack FX item.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_story.h"
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
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

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
#include "driver/seqdrv.h"

typedef struct CloudWork {
    s32 used;
    s32 state;
    char animName[0x20];
    char imgName[0x20];
    char offscreenName[0x20];
    char groupNames[8][0x20];
    char hitName[0x20];
    s32 limit;
    s32 counter; // Advances by 2 during contact and the loss-of-contact grace period.
    EventEntry* evt;
    void* endEvt;
    s32 timer;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    f32 lastFrame;
} CloudWork;
static CloudWork* cloud;
#if VERSION <= VERSION_DEMO_US
static s8 cloud_once_flag;
#else
static u8 cloud_once_flag;
#endif
static void _mapsetofs_all(CloudWork* wp);
static void _mapclsofs_all(CloudWork* wp);
static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _img_alloc_capture(const char* name, s32 shadowId, const char* offName, s32 allocMode, s32 x, s32 y, s32 w, s32 h);

static USER_FUNC(evt_cloud_main);
USER_FUNC(evt_iri_16_cloud_ent);
USER_FUNC(evt_iri_16_cloud_init);
static USER_FUNC(gonbaba_wait_landon);

EvtWord evt_sound[8];
static EvtWord door01_close[22];
static EvtWord door01_open[22];
static BeroEntry bero_entry_data[2];
static EvtWord gonbaba_standup_shake[23];
static EvtWord gonbaba_fog_change[87];
static EvtWord gonbaba_down_shake[83];
#include "version.h"
#if VERSION <= VERSION_DEMO_US
extern EvtWord gonbaba_event[];
#else
EvtWord gonbaba_event[1410];
#endif
static const char* capturemap[2];
static Vec v_list[8];
static EvtWord evt_cloud_main_evt[4];
EvtWord iri_16_box_evt[33];
EvtWord iri_16_cloud_evt[36];
EvtWord iri_16_touch_evt[2];
static EvtWord iri_16_init[57];
#if VERSION <= VERSION_DEMO_US
extern EvtWord gon_11_init_evt[];
#else
EvtWord gon_11_init_evt[101];
#endif

/** Exit event of the lair door: fades out the cutscene music on channel 1 and returns 0 to the door code. */
EVT_BEGIN(evt_sound)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the w_bero exit to gon_10:
 * rotates map objects "door03" and "door04" about Y by 0.389 and -0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero exit to gon_10:
 * rotates map objects "door03" and "door04" about Y by -0.389 and 0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exits (beros) of this room: w_bero to gon_10. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        13, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        evt_sound, // exitEvent
        "gon_10", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {0},
};

/** Script helper: blocks until Hooktail (ゴンババ) has touched the ground. */
static USER_FUNC(gonbaba_wait_landon) {
    // ゴンババ
    return npcNameToPtr("\203S\203\223\203o\203o")->groundHit ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Waits 3.7 seconds into the Hooktail entrance cutscene,
 * then plays SFX_STG1_GNB_STEP1 at her position and shakes the camera when she stands up.
 */
static EVT_BEGIN(gonbaba_standup_shake)
    WAIT_MS(3700)
    CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_STEP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 800)
    RETURN
EVT_END()

/**
 * Fog fade during Hooktail's entrance: over 8 seconds it moves the map fog's two distance parameters outward by
 * 400 and 1600 units from their current values.
 */
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
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 400, LW(10))
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
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 1600, LW(10))
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

/**
 * Shake for Hooktail collapsing: plays SFX_STG1_GNB_DOWN1
 * and DOWN2 at her position (the second fading out over 10 seconds) while the camera shakes strongly and
 * then settles to a small rumble.
 */
static EVT_BEGIN(gonbaba_down_shake)
    CALL(evt_npc_get_position, STRING("ゴンババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_STG1_GNB_DOWN1"), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(8))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 10000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_snd_sfx_vol, LW(8), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_snd_sfxoff, LW(8))
    END_INLINE_EVENT
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 50, 5, 50)
    LOOP(0)
        CALL(evt_sub_intpl_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 50)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
    RETURN
EVT_END()

/**
 * The Hooktail boss scene, run when Mario enters the lair:
 * Mario walks in, Hooktail wakes and speaks (stg1_gon_19 to 20_2) while the fog thickens and the floor shakes,
 * then the boss battle starts.
 * After the battle (or at once when the intro is skipped) Hooktail collapses,
 * Koops' father (ノコタロウ父) appears and talks with the partner and Mario (stg1_gon_33_01 to 33_17),
 * and the story moves to STG1_16_HOOKTAIL and then STG1_17_FATHER_SON_REUNITED.
 */
EVT_BEGIN(gonbaba_event)
    // Hooktail Castle's last room: fog fills it and Mario loses control.
    CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
    CALL(evt_npc_pera_onoff, STRING("ゴンババ"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), NPC_FLAG_TALK_POSE)
    CALL(evt_map_set_fog, 2, 400, 1000, 0, 10, 40)
    CALL(evt_map_fog_onoff, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    // If the intro was already seen, skip straight to the aftermath.
    IF_INT_EQUAL(GSWF_GON_11_HOOKTAIL_INTRO_SKIP, 1)
        GOTO(100)
    END_IF
    // Mario and his partner enter the room.
    CALL(evt_cam3d_evt_set, -480, 150, 459, -480, 67, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, -560, 10, 0)
    CALL(evt_party_set_pos, 0, -600, 10, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    // The door slams shut behind them and the camera shakes.
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(door01_close))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0146484375f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 100)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    // Hooktail roars and stands up, the fog changes and the battle music begins.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_GONBABA_FLY2"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG1_GNB_ROAR2"), -900, 30, 490, 255, 255, 4300, 16, 0)
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_U_1"))
        CALL(evt_npc_wait_anim, STRING("ゴンババ"), FLOAT(1.0f))
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_S_1"))
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(gonbaba_standup_shake))
    SPAWN_EVENT(PTR(gonbaba_fog_change))
    CALL(evt_cam3d_evt_set, -900, 31, 490, -205, 175, 20, 4500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(1800)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("door10"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kesu"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    WAIT_MS(4000)
    // The camera trembles as Hooktail towers over them.
    LOOP(30)
        CALL(evt_cam3d_evt_set, -890, 33, 483, -205, 175, 20, 15, EASING_SINE_IN_OUT)
        WAIT_MS(15)
        CALL(evt_cam3d_evt_set, -900, 31, 490, -205, 175, 20, 15, EASING_SINE_IN_OUT)
        WAIT_MS(15)
    END_LOOP
    WAIT_MS(4500)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_19_2"), 0, STRING("ゴンババ"))
    CALL(evt_cam3d_evt_set, -545, -14, 417, -225, 227, 12, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("door10"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kesu"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1000)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_20"), 0, STRING("ゴンババ"))
    CALL(evt_cam3d_evt_set, 77, 455, 380, -147, 293, 183, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(LW(0), -246)
    SET_VALUE(LW(1), 146)
    SET_VALUE(LW(2), 43)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg1_gon_20_2"), 0, STRING("ゴンババ"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -325, 30, 650, -350, 125, -33, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(2500)
    // The battle with Hooktail starts.
    CALL(evt_npc_set_battle_info, STRING("ゴンババ"), 20)
    CALL(evt_npc_battle_start, STRING("ゴンババ"))
    CALL(evt_npc_wait_battle_end)
    // After the battle: lights and fog are reset and Hooktail is lying on the floor, groaning.
    LABEL(100)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_map_fog_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("baba"), 1, 1)
    CALL(evt_cam3d_evt_set, 0, 30, 842, 0, 135, 150, 0, EASING_SINE_IN_OUT)
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
        CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(gonbaba_wait_landon)
        CALL(evt_npc_set_anim, STRING("ゴンババ"), STRING("GNB_H_2"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 25, 423, 0, 55, 150, 5000, EASING_SINE_IN_OUT)
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
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 220, 160, -160)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_03"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("eff1"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_cam3d_evt_set, 255, 220, 648, 255, 170, 150, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    DELETE_EVENT(LW(10))
    // Muffled sounds come from inside Hooktail as someone fights his way out.
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
    // Mr. Koopa spins out of Hooktail's mouth and lands in the room.
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_Z_2"))
        CALL(evt_npc_flag_onoff, 0, STRING("ノコタロウ父"), NPC_FLAG_AUTO_MOVE_ANIM)
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
    CALL(evt_cam3d_evt_set, 255, 105, 648, 255, 55, 150, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ父"), NPC_FLAG_AUTO_MOVE_ANIM)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_04"), 0, STRING("ノコタロウ父"))
    // Koops is swapped in as the partner if someone else was along.
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 2)
        CALL(evt_party_stop, 0)
        SET_VALUE(GSWF_GON_11_KOOPS_SWAPPED_IN, 1)
    END_IF
    CALL(evt_mario_set_pos, 25, 10, 200)
    CALL(evt_party_set_pos, 0, -25, 10, 200)
    IF_INT_EQUAL(GSWF_GON_11_KOOPS_SWAPPED_IN, 1)
        CALL(evt_npc_set_position, STRING("クリスチーヌ"), -10, 10, 160)
        CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
        CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), NPC_STATUS_FACE_MOVE_DIR)
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
    CALL(evt_cam3d_evt_set, 0, 75, 450, 0, 50, 200, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_NOKO_SAIKAI1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_07"), 0, STRING("party"))
    // Koops runs to his father while they chat about the ten years apart.
    CALL(evt_cam3d_evt_set, 240, 75, 450, 240, 50, 200, 1000, EASING_SINE_IN_OUT)
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
    IF_INT_EQUAL(GSWF_GON_11_KOOPS_SWAPPED_IN, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_move_position, STRING("クリスチーヌ"), 150, 200, 0, FLOAT(80.0f), 1)
        END_INLINE_EVENT
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 180, 220, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_09"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_10"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 500, 380, 856, 250, 130, 190, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_11"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_12"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 220, 75, 450, 220, 50, 200, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_13"), 0, STRING("party"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ノコタロウ父"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 270, 75, 450, 270, 50, 200, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_33_14"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 245, 75, 450, 245, 50, 200, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    // The father fishes the Crystal Star out of his pocket: he kept it from Hooktail's belly.
    CALL(evt_npc_set_anim, STRING("ノコタロウ父"), STRING("NKP_A_1"))
    CALL(evt_item_entry, STRING("stone"), ITEM_DIAMOND_STAR, 245, 45, 220, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
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
    // The star bounces on the floor.
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
    CALL(evt_cam3d_evt_set, 190, 75, 450, 190, 50, 200, 500, EASING_SINE_IN_OUT)
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
    // Mario and the partner walk over; the music fades.
    CALL(evt_cam3d_evt_set, 245, 75, 450, 245, 50, 245, 2000, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 245, 215, FLOAT(30.0f))
    END_INLINE_EVENT
    WAIT_MS(2000)
    // Hooktail is defeated, father and son are reunited: the story advances and the Diamond Star is awarded.
    SET_VALUE(GSW_STORY_PROGRESS, STG1_16_HOOKTAIL)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
#else
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
#endif
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
#endif
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(1), STRING("stone"))
    CALL_CHILD(PTR(evt_sub_starstone))
    RETURN
EVT_END()

/**
 * Map group that the dark cloud of Trouble Center request 16 (evt_iri_16_cloud_ent) draws into its offscreen
 * buffer; the cloud's paper animation is a capture of it. An empty string ends the list.
 */
static const char* capturemap[] = {
    "gon_11_S",
    "\0",
};

/** Corners of a box, as offsets from its centre, that a script helper projects to the screen. */
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

/**
 * Event script: runs the cloud-blowing mechanism (evt_cloud_main) of las_14,
 * the room where Flurrie's gust uncovers the way.
 */
static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

/**
 * Calls evt_img_alloc_capture directly from C by building a throwaway event with the eight arguments;
 * used so the cloud effect can capture the screen area it needs.
 */
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

/**
 * Projects the eight corners of a box (v_list) around the named hit object to screen coordinates
 * and returns their bounding rectangle (x, y, width, height) in image space, clipped to the 608 x 480 screen.
 * Used to size the capture image for the dark cloud.
 */
// Projects eight offsets and clips right/bottom extents; this helper always returns TRUE.
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

/**
 * Sends every map group of the cloud effect (up to 8) to its offscreen buffer,
 * so the scenery under the cloud can be drawn into it.
 */
static void _mapsetofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpSetOffScreen(wp->groupNames[i], wp->offscreenName);
    }
}

/** Undoes _mapsetofs_all: returns the cloud effect's map groups to normal drawing. */
static void _mapclsofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpClearOffScreen(wp->groupNames[i]);
    }
}

/**
 * Running script of one dark cloud entry: waits for Flurrie's breath to touch the cloud's hit area,
 * then builds a paper-cloud image of the scenery under it and counts how long the breath keeps hitting it;
 * when the count reaches the limit the cloud plays its blow-away animation,
 * runs the end event (iri_16_cloud_evt) and the entry is released. If the breath stops first the cloud resets.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(evt_cloud_main) {
    CloudWork* wp = (CloudWork*)event->lwData[0];
    s32 inBattle = gp->inBattle != 0;
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
#else
/**
 * Running script of one dark cloud entry: waits for Flurrie's breath to touch the cloud's hit area,
 * then builds a paper-cloud image of the scenery under it and counts how long the breath keeps hitting it;
 * when the count reaches the limit the cloud plays its blow-away animation,
 * runs the end event (iri_16_cloud_evt) and the entry is released. If the breath stops first the cloud resets.
 */
static s32 evt_cloud_main(EventEntry *event, BOOL isFirstCall) {
#endif
    HitEntry *hit0;
    HitEntry *hit1;
    HitEntry *hit2;
    HitEntry *hit3;
#if VERSION > VERSION_DEMO_US
    MarioWork *mario;
    CloudWork *wp;
    PartyEntry *ptmp;
#endif
    HitEntry *hit;
    OffscreenEntry *off;
    ImageEntry *img;
#if VERSION > VERSION_DEMO_US
    BOOL inBattle;
    wp = (CloudWork*)event->lwData[0];
    inBattle = gp->inBattle != 0;
    mario = marioGetPtr();
    ptmp = partyGetPtr(0);
#endif
    if (isFirstCall) {
        return 0;
    }
#if VERSION <= VERSION_DEMO_US
    if (party == NULL) {
        return EVT_RETURN_BLOCK;
    }
    if (party->currentMemberId != PARTY_MEMBER_FLURRIE) {
        return EVT_RETURN_BLOCK;
    }
#else
    if (ptmp == NULL) {
        return 0;
    }
#endif
    hit0 = cloudGetHitBreathout(0);
    hit1 = cloudGetHitBreathout(1);
    hit2 = cloudGetHitBreathout(2);
    hit3 = cloudGetHitBreathout(3);
    switch (wp->state) {
        case 0:
#if VERSION <= VERSION_DEMO_US
            if (cloud_once_flag == 1) {
#else
            if (((s32) (cloud_once_flag & 0xFFFFFFFFFFFFFFFFu)) == 1) // FAKE: no-op 64-bit mask; needed for 100% (plain cloud_once_flag == 1 gives 97.9-98.1)
          {
#endif
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if ((((hit0 == hit) || (hit1 == hit)) || (hit2 == hit)) || (hit3 == hit)) {
                wp->state++;
            }
            wp->counter = 0;
            break;
        case 1:
            if ((mario->displayDirectionTarget > 90.0f) && (mario->displayDirectionTarget <= 270.0f)) {
                strcpy(wp->animName, "P_oof_cloud_2");
            } else {
                strcpy(wp->animName, "P_oof_cloud_1");
            }
            if (!animGroupBaseAsync(wp->animName, inBattle, NULL)) {
                break;
            }
            cloud_once_flag = 1;
            animPaperPoseEntry(wp->animName, inBattle);
            offscreenEntry(wp->offscreenName);
            off = offscreenNameToPtr(wp->offscreenName);
#if VERSION <= VERSION_DEMO_US
            off->texFmt = off->copyFmt = 6;
#else
            off->texFmt = (off->copyFmt = 4);
#endif
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
            img->flags |= IMAGE_FLAG_HIDE;
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
            img->flags &= ~IMAGE_FLAG_HIDE;
            wp->state = 3;
        case 3:
            img = imgNameToPtr(wp->imgName, inBattle);
            img->animName = "A_1";
            *(&img->animTime) = animTimeGetTime(inBattle);
            img->flags &= ~IMAGE_FLAG_HIDE;
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

/**
 * Script helper: registers one blowable cloud: args[0] is the list of map groups it hides,
 * args[1] the hit object name, args[2] the script to run when it has been blown away.
 * Starts evt_cloud_main for it.
 */
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

/**
 * Opens the chest that appears after the cloud is gone
 * and gives Mario the Attack FX B key item (the Trouble Center request 16 reward item).
 */
EVT_BEGIN(iri_16_box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("mb"), ITEM_ATTACK_FX_B_KEY_ITEM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("mb"))
    CALL(evt_item_get_item_end_wait, STRING("mb"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * End event of the request-16 cloud: places the chest (box), switches off the cloud's hit area,
 * plays the success jingle and sets GSWF_TROUBLE_WORK_1.
 */
EVT_BEGIN(iri_16_cloud_evt)
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(iri_16_box_evt), GSWF_TROUBLE_WORK(2))
    CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
    RETURN
EVT_END()

/** Case event on the request-16 cloud (front-A); does nothing. */
EVT_BEGIN(iri_16_touch_evt)
    RETURN
EVT_END()

/**
 * Map-init part of Trouble Center request 16: while the request is active it sets up the cloud (A_kura) with its
 * case event and the capture map, or, once the cloud is cleared (GSWF_TROUBLE_WORK_1), the reward chest (box);
 * when the request is not active it disables the hit objects A_taka and A_kura.
 */
static EVT_BEGIN(iri_16_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ELUSIVE_BADGE)
        CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
            CALL(evt_iri_16_cloud_init)
            CALL(evt_iri_16_cloud_ent, PTR(&capturemap), STRING("A_kura"), PTR(iri_16_cloud_evt))
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kura"), 0, PTR(iri_16_touch_evt), 0)
        ELSE
            CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
            CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(iri_16_box_evt), GSWF_TROUBLE_WORK(2))
        END_IF
    ELSE
        CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Map init for gon_11 (Hooktail's lair): sets up the door back to gon_10, the music and ambience,
 * and the Hooktail and Koops' father NPCs.
 * Before Hooktail is beaten (STG1_16_HOOKTAIL) it enlarges Hooktail and starts gonbaba_event;
 * afterwards both NPCs are moved off the map.
 * Also sets up the Trouble Center request 16 cloud and chest (iri_16_init).
 */
EVT_BEGIN(gon_11_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_16_HOOKTAIL)
        CASE_DEFAULT
        CALL_CHILD(PTR(evt_bero_info_run))
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_16_HOOKTAIL)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    END_SWITCH
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON4"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ父"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), NPC_FLAG_NO_HIT)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_16_HOOKTAIL)
        CALL(evt_npc_set_position, STRING("ゴンババ"), 0, -1000, 0)
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
    ELSE
        CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), NPC_FLAG_NO_VIEW_CORRECTION)
        CALL(evt_npc_set_scale, STRING("ゴンババ"), FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(1.2998047f))
        SPAWN_EVENT(PTR(gonbaba_event))
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("ゴンババ"), NPC_FLAG_NO_CULL)
    CALL(evt_light_charalight_all_onoff, 1)
    CALL(evt_light_npc_lighting_onoff, 1, STRING("ゴンババ"))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(iri_16_init)
#else
    CALL_CHILD(PTR(iri_16_init))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_16_HOOKTAIL)
        CALL(evt_npc_delete, STRING("ノコタロウ父"))
        CALL(evt_npc_delete, STRING("ゴンババ"))
    END_IF
#endif
    RETURN
EVT_END()
