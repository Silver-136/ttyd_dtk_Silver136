#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "rels/aaa/include/aaa.h"

#include "driver/camdrv.h"
#include "win/win_main.h"
#include "dolphin/gx.h"
#include "manager/filemgr.h"
#include "memory.h"
#include "system.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "evt/evt_door.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"

static void draw(CameraId camera, void* param);
typedef struct AaaWork { FileEntry* tpl; s32 alpha; } AaaWork;
static AaaWork* wp;

USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(mapdraw);

static NpcSetupInfo npcEnt[3];
static EvtWord evt_door[15];
static EvtWord evt_door2[12];
static EvtWord evt_wall[22];
static const char* ie_npc[2];
static EvtWord door_1_open[12];
static EvtWord door_1_close[14];
static DoorParam door_1;
static EvtWord evt_prologue[713];
#ifdef VERSION_G8ME01
EvtWord evt_prologue2[196];
#else
EvtWord evt_prologue2[193];
#endif
#ifdef VERSION_G8ME01
static EvtWord epilogue_evt[328];
#else
static EvtWord epilogue_evt[318];
#endif
static EvtWord epilogue_npc_entry[68];
#ifdef VERSION_G8ME01
static EvtWord epilogue_evt2[500];
#else
static EvtWord epilogue_evt2[495];
#endif
static TreeAccess ki_data[2];
static KusaAccess kusa_data[11];
static BeroEntry bero_entry_data[2];
EvtWord aaa_00_init_evt[81];

static NpcSetupInfo npcEnt[] = {
    {
        /*"パレッタ"*/"\x83\x70\x83\x8C\x83\x62\x83\x5E", // name
        0x40000600, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57", // name
        0x40000600, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5546875f))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door2)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_door"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_kaiten2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_kaiten1"), LW(0), 0, 0)
    RETURN
EVT_END()

static const char* ie_npc[] = {
    /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57",
    NULL,
};

static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envon, 160, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_1 = {
    0, // flags
    3, // type
    4, // se
    "S_ie_door", // mapobj
    "A_ie_door", // hitIn
    "A_ie_door_u", // hitOut
    "S_ie_soto", // outside
    "A_ie_soto", // outsideHit
    "S_naka", // inside
    "A_naka1", // insideHit
    evt_door, // evtIn
    NULL, // evtOut
    evt_wall, // evtWall
    ie_npc, // npcs
    NULL, NULL, // mobjs..items
    door_1_open, // evtOpen
    door_1_close, // evtClose
};

static EVT_BEGIN(evt_prologue)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MARIO_HOUSE1"))
    END_INLINE_EVENT
    CALL(evt_snd_envon, 272, STRING("ENV_OPN_AAA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_cam_letter_box_camid, 6)
    CALL(evt_npc_status_onoff, 1, STRING("ルイージ"), 2)
    CALL(evt_npc_status_onoff, 1, STRING("パレッタ"), 2)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -150, 30, -330)
    CALL(evt_party_set_pos, 0, -150, 30, -330)
    CALL(evt_cam3d_evt_set, 145, 122, 884, 145, 108, -180, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_mario_set_pos, -150, 30, -330)
    CALL(evt_party_set_pos, 0, -150, 30, -330)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 1500, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_trans, 1, STRING("sai_ki2"), LW(0), 0, 0)
            MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
            CALL(evt_mapobj_trans, 1, STRING("sai_ki1"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 145, 122, 717, 145, 108, -180, 5000, 11)
    WAIT_MS(4000)
    CALL(evt_npc_set_position, STRING("パレッタ"), 500, 500, 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        LOOP(14)
            CALL(evt_npc_get_position, STRING("パレッタ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_OPN_PARETTA_MOVE1"), LW(0), LW(1), -100, 0)
            WAIT_MS(200)
        END_LOOP
        LOOP(0)
            CALL(evt_npc_get_position, STRING("パレッタ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_OPN_PARETTA_MOVE1"), LW(0), LW(1), -100, 0)
            WAIT_MS(400)
            IF_INT_EQUAL(GF(1), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        LOOP(17)
            CALL(evt_npc_get_position, STRING("パレッタ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_OPN_PARETTA_MOVE1"), LW(0), LW(1), -100, 0)
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_glide_position, STRING("パレッタ"), 210, 25, -30, 0, FLOAT(80.0f), FLOAT(-30.0f), 11, 0)
    CALL(evt_npc_reverse_ry, STRING("パレッタ"))
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_posuto"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_POST_SHAKE1"), LW(0), LW(1), LW(2), 0)
    LOOP(3)
        CALL(evt_mapobj_trans, 1, STRING("S_posuto"), 2, 0, 0)
        WAIT_FRAMES(2)
        CALL(evt_mapobj_trans, 1, STRING("S_posuto"), 0, 0, 0)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(evt_npc_reverse_ry, STRING("パレッタ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("pro_00"), 0, STRING("パレッタ"))
    SET_VALUE(GF(1), 1)
    CALL(evt_npc_glide_position, STRING("パレッタ"), 0, 300, -100, 0, FLOAT(80.0f), FLOAT(-30.0f), 11, 0)
    CALL(evt_npc_set_position, STRING("ルイージ"), 20, 30, -150)
    CALL(evt_npc_set_ry, STRING("ルイージ"), 90)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_naka"), 1)
    CALL(evt_mapobj_get_position, STRING("S_ie_door"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_door))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_move_position, STRING("ルイージ"), 63, -110, 0, FLOAT(80.0f), 1)
    CALL(evt_mapobj_get_position, STRING("S_ie_door"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(evt_door))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), 131088)
    CALL(evt_npc_move_position, STRING("ルイージ"), 195, 0, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_move_position, STRING("ルイージ"), 225, -75, 0, FLOAT(80.0f), 1)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_posuto"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_POST_SHAKE1"), LW(0), LW(1), LW(2), 0)
    LOOP(3)
        CALL(evt_mapobj_trans, 1, STRING("S_posuto"), 2, 0, 0)
        WAIT_FRAMES(2)
        CALL(evt_mapobj_trans, 1, STRING("S_posuto"), 0, 0, 0)
        WAIT_FRAMES(2)
    END_LOOP
    WAIT_MS(340)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), 1024)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_11"))
    CALL(evt_npc_move_position, STRING("ルイージ"), 195, 0, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_move_position, STRING("ルイージ"), 63, -110, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), 131088)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    CALL(evt_mapobj_get_position, STRING("S_ie_door"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_door))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_6"))
        CALL(evt_npc_move_position, STRING("ルイージ"), 25, -155, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -345, 154, 96, -32, 75, -217, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_aaa_00"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_door"), 1)
    CALL(evt_bg_set_color, 0, 0, 0, 255)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mapobj_get_position, STRING("S_ie_door"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(evt_door))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    CALL(evt_npc_set_autotalkpose, STRING("ルイージ"), STRING("L_5"), STRING("L_7"))
    CALL(evt_msg_print, 0, STRING("pro_01"), 0, STRING("ルイージ"))
    CALL(evt_cam3d_evt_set, -376, 154, 65, -63, 75, -247, 2000, 11)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_6"))
    CALL(evt_npc_move_position, STRING("ルイージ"), 23, -156, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pos, -155, 30, -320)
        CALL(evt_mario_mov_pos2, -125, -290, FLOAT(80.0f))
        CALL(evt_mario_mov_pos2, -85, -310, FLOAT(80.0f))
        CALL(evt_mario_adjust_dir)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_6"))
    CALL(evt_npc_move_position, STRING("ルイージ"), -52, -275, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_5"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("pro_02"), 0, STRING("ルイージ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_8"))
    CALL(evt_npc_set_autotalkpose, STRING("ルイージ"), STRING("L_8"), STRING("L_9"))
    CALL(evt_msg_print, 0, STRING("pro_03"), 0, STRING("ルイージ"))
    WAIT_MS(300)
    CALL(evt_cam_letter_box_camid, 10)
    CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
    CALL(evt_bero_mapchange, STRING("gor_10"), 0)
    RETURN
EVT_END()

static void draw(CameraId camera, void* param) {
    CameraEntry saved;
    f32 viewport[6];
    f32 projection[7];
    CameraEntry* cam;

    GXGetViewportv(viewport);
    GXGetProjectionv(projection);
    cam = camGetPtr(CAMERA_2D);
    GXSetProjection(camGetPtr(CAMERA_2D)->projection, cam->projType);
    saved = *camGetCurPtr();
    cam = camGetPtr(CAMERA_2D);
    *camGetCurPtr() = *cam;
    winTexInit(*(TEXPalette**)wp->tpl->data);
    winTexSet(3, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, wp->alpha});
    *camGetCurPtr() = saved;
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projection);
}

EVT_BEGIN(evt_prologue2)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_aaa_00"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_naka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_door"), 1)
    CALL(evt_bg_set_color, 0, 0, 0, 255)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -85, 30, -310)
    CALL(evt_mario_set_dir, 90, 0, 0)
#ifndef VERSION_G8ME01
    CALL(evt_mario_adjust_dir)
#endif
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), 1024)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("L_8"))
    CALL(evt_npc_set_autotalkpose, STRING("ルイージ"), STRING("L_8"), STRING("L_9"))
    CALL(evt_npc_set_position, STRING("ルイージ"), -52, 30, -275)
    CALL(evt_npc_set_ry, STRING("ルイージ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), 32768)
    CALL(evt_cam3d_evt_set, -376, 154, 65, -63, 75, -247, 0, 11)
    CALL(evt_seq_wait, 2)
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_dir, 90, 0, 0)
#endif
    CALL(evt_fade_end_wait)
    CALL(evt_cam_letter_box_camid, 8)
    CALL(evt_msg_print, 0, STRING("pro_05"), 0, STRING("ルイージ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("I_1"))
    CALL(evt_npc_get_position, STRING("ルイージ"), LW(0), LW(1), LW(2))
    ADD(LW(0), -25)
    ADD(LW(1), 15)
    ADD(LW(2), -3)
    CALL(evt_item_entry, STRING("map"), 96, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_item_set_scale, STRING("map"), FLOAT(0.5498047f))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("map"))
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("S_1"))
    CALL(evt_mario_set_pose, STRING("M_L_3"))
    WAIT_MS(1500)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(mapdraw)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_envoff, 512)
    CALL(evt_fade_set_mapchange_type, 1, 49, 0, 50, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 2)
    CALL(evt_bero_mapchange, STRING("muj_20"), STRING("prologue"))
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_envon, 272, STRING("ENV_END_AAA1"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -126, 40, -224)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_npc_set_position, STRING("ルイージ"), -44, 40, -163)
    CALL(evt_npc_set_ry, STRING("ルイージ"), 270)
    CALL(evt_seq_wait, 2)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    CALL(evt_mario_set_pose, STRING("M_S_5"))
    CALL(evt_npc_set_anim, STRING("ルイージ"), STRING("S_2"))
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("ep_31"), 0, 0)
    WAIT_MS(500)
    CALL(evt_fade_entry, 10, 1, 0, 0, 0)
    WAIT_FRAMES(1)
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -360, 154, 80, -46, 81, -232, 0, 11)
    CALL(evt_fade_in, 1000)
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("ルイージ"), STRING("S_2"), STRING("T_2"))
    CALL(evt_msg_print, 0, STRING("ep_32"), 0, STRING("ルイージ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_11"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_33"), 0, STRING("ルイージ"))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_balloon_bikkuri)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION3"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_msg_print, 0, STRING("ep_34"), 0, STRING("ルイージ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_dir, 90, 0, 0)
#endif
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, -128, 30, -196, 400, 1, 20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_LANDING3"), LW(0), LW(1), LW(2), 0)
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_dir, 90, 0, 0)
#endif
    CALL(evt_cam3d_evt_set, -307, 117, 9, -58, 58, -244, 1200, 11)
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), 32)
    CALL(evt_npc_jump_position_nohit, STRING("ルイージ"), -68, 30, -149, 400, 20)
    CALL(evt_npc_move_position, STRING("ルイージ"), -107, -166, 800, 0, 0)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_35"), 0, 0)
    CALL(evt_snd_bgmoff_f, 512, 3000)
    CALL(evt_snd_envoff_f, 512, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("gor_02"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("ピーチ姫"), STRING("c_peach"))
    CALL(evt_npc_set_tribe, STRING("ピーチ姫"), STRING("ピーチ姫（普通）"))
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), 1073743392)
    CALL(evt_map_blend_set_flag, 1, STRING("ピーチ姫"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("ピーチ姫"), 4096)
    CALL(evt_npc_entry, STRING("キノじい"), STRING("c_kino_ji"))
    CALL(evt_npc_set_tribe, STRING("キノじい"), STRING("キノじい"))
    CALL(evt_npc_set_position, STRING("キノじい"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("キノじい"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("キノじい"), 1073743392)
    CALL(evt_map_blend_set_flag, 1, STRING("キノじい"), 2048)
    CALL(evt_map_blend_set_flag, 1, STRING("キノじい"), 4096)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt2)
    CALL(evt_snd_envon, 272, STRING("ENV_END_AAA1"))
    CALL(evt_snd_envoff, 18432)
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -128, 30, -196)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_npc_set_position, STRING("ルイージ"), -107, 30, -166)
    CALL(evt_npc_set_ry, STRING("ルイージ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), 32768)
    CALL(evt_seq_wait, 2)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_open_quick))
    CALL(evt_cam3d_evt_set, -307, 117, 9, -58, 58, -244, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_fade_end_wait)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), 32768)
    CALL(evt_msg_print, 0, STRING("ep_47"), 0, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MARIO_HOUSE2"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, -375, 154, 66, -62, 81, -248, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_dir, 90, 0, 0)
#endif
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_48"), 0, STRING("ルイージ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mapobj_get_position, STRING("S_ie_door"), LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_EPILOGUE_KINOJII_KNOCK1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_EPILOGUE_KINOJII_KNOCK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ルイージ"), 0, 0, 0, 0, 0, 0, 0, 36)
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING("ルイージ"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ルイージ"))
    CALL(evt_msg_print, 0, STRING("ep_49"), 0, STRING("ルイージ"))
    CALL(evt_npc_set_position, STRING("キノじい"), 60, 30, -110)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 78, 30, -89)
    CALL(evt_cam3d_evt_set, -345, 154, 96, -31, 81, -217, 600, 11)
    WAIT_MS(900)
    CALL(evt_msg_print, 0, STRING("ep_50"), 0, STRING("キノじい"))
    SET_VALUE(LW(0), PTR(&door_1))
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_sub_intpl_msec_init, 5, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_door))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_move_position, STRING("キノじい"), 10, -205, 0, 60, 0)
    CALL(evt_npc_move_position, STRING("ピーチ姫"), 10, -160, 0, 60, 0)
    CALL(evt_msg_print, 0, STRING("ep_51"), 0, STRING("ピーチ姫"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_52"), 0, STRING("キノじい"))
    CALL(evt_cam3d_evt_set, -259, 137, 44, -15, 76, -200, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_5"), STRING("P_T_5"))
    CALL(evt_msg_print, 0, STRING("ep_53"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_7"))
    CALL(evt_cam3d_evt_set, -406, 179, 126, -47, 90, -232, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_7"), STRING("P_T_13"))
    CALL(evt_msg_print, 0, STRING("ep_54"), 0, STRING("ピーチ姫"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(550)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_reverse_ry, STRING("ルイージ"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("ルイージ"))
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(200)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ピーチ姫"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_set_ry, STRING("キノじい"), 90)
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("キノじい"))
        CALL(evt_npc_move_position, STRING("キノじい"), 20, -145, 0, 80, 0)
        CALL(evt_npc_move_position, STRING("キノじい"), 78, -89, 0, 80, 0)
        CALL(evt_npc_set_position, STRING("キノじい"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ピーチ姫"), 20, -145, 0, 60, 0)
    CALL(evt_npc_move_position, STRING("ピーチ姫"), 78, -89, 0, 60, 0)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 0, -1000, 0)
    WAIT_WHILE(GF(0))
    CALL(evt_snd_bgmon, 512, STRING("BGM_ENDING4"))
    CALL(evt_cam3d_evt_set, -260, 90, -62, -68, 42, -253, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    CALL(evt_fade_entry, 4, 3000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(3000)
    SET_VALUE(GSW_STORY_PROGRESS, 403)
    CALL(evt_fade_set_mapchange_type, 1, 10, 1, 9, 300)
    CALL(evt_bero_mapchange, STRING("end_00"), STRING(""))
    RETURN
EVT_END()

static TreeAccess ki_data[] = {
    {6, "A_ki_1", "S_ki_1", "S_ha_1", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static KusaAccess kusa_data[] = {
    {0, "A_kusa_01", "S_kusa_01", NULL},
    {0, "A_kusa_02", "S_kusa_02", NULL},
    {0, "A_kusa_03", "S_kusa_03", NULL},
    {0, "A_kusa_04", "S_kusa_04", NULL},
    {0, "A_kusa_05", "S_kusa_05", NULL},
    {0, "A_kusa_06", "S_kusa_06", NULL},
    {0, "A_kusa_07", "S_kusa_07", NULL},
    {0, "A_kusa_08", "S_kusa_08", NULL},
    {0, "A_kusa_09", "S_kusa_09", NULL},
    {0, "A_kusa_10", "S_kusa_10", NULL},
    {4, NULL, NULL, NULL},
};

static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(aaa_00_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sai_ki1"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sai_ki2"), 2)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("prologue"))
        SPAWN_EVENT(PTR(evt_prologue))
        WAIT_FRAMES(1)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("prologue2"))
        SPAWN_EVENT(PTR(evt_prologue2))
        WAIT_FRAMES(1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("epilogue2"))
            CALL_CHILD(PTR(epilogue_npc_entry))
            SPAWN_EVENT(PTR(epilogue_evt2))
            ELSE
            SPAWN_EVENT(PTR(epilogue_evt))
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(ki_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(kusa_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    RETURN
EVT_END()

USER_FUNC(mapdraw) {
    if (isFirstCall) {
        wp = _mapAlloc(sizeof(AaaWork));
        memset(wp, 0, sizeof(AaaWork));
        wp->tpl = fileAllocf(4, "%s/mariost.tpl", getMarioStDvdRoot());
        wp->alpha = 0;
    }
    wp->alpha += 2;
    if (wp->alpha > 255) {
        wp->alpha = 255;
        evtSetValue(event, GF(0), 1);
    }
    dispEntry(CAMERA_3D, 1000.0f, 7, draw, wp);
    return EVT_RETURN_BLOCK;
}

void mapdelete(void) {
    if (wp != NULL && wp->tpl != NULL) {
        fileFree(wp->tpl);
    }
}

