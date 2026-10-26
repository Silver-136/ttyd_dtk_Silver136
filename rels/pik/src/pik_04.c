#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_damage.h"
#include "evt/evt_kinopio.h"
#include "pik.h"
#include "evt/evt_npc.h"
#include "driver/hitdrv.h"
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
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(unitwincoin_disp_onoff);
USER_FUNC(unitwincoin_wait);

static BeroEntry bero_data[3];
static HitDamageReturn dmgReturnPt[9];
static const char* name_tbl[6];
static EvtWord evt_door[12];
static EvtWord evt_door2[12];
static EvtWord evt_wall[29];
const char* ie_3_npc[3];
static EvtWord door_1_open[12];
static EvtWord door_1_close[14];
static DoorParam door_1;
EvtWord evt_door_h[22];
EvtWord evt_door2_h[19];
EvtWord evt_wall_h[12];
const char* hotel_npc[3];
static EvtWord door_2_open[18];
static EvtWord door_2_close[14];
static DoorParam door_2;
EvtWord imginit[46];
EvtWord imgstart[107];
EvtWord imgend[129];
EvtWord evt_up[402];
EvtWord evt_down[404];
EvtWord init_miyage[8];
EvtWord talk_miyage[2];
static EvtWord talk_musume[20];
EvtWord init_musume1[13];
EvtWord talk_musume1[4];
EvtWord init_musume2[13];
EvtWord talk_musume2[4];
EvtWord init_musume3[13];
EvtWord talk_musume3[4];
EvtWord init_takun1[2];
EvtWord talk_takun1[28];
EvtWord init_takun2[2];
EvtWord talk_takun2[28];
EvtWord init_takun3[7];
EvtWord talk_takun3[28];
static EvtWord init_kiza[13];
static EvtWord talk_kiza[8];
static NpcSetupInfo npc_data[9];
EvtWord evt_roten[113];
EvtWord evt_kinopio_to_bed[29];
EvtWord evt_kinopio_from_bed[27];
static KinopioData kino_dt;
static EvtWord epilogue_npc_entry[169];
static EvtWord epilogue_evt_sub[91];
static EvtWord epilogue_evt[63];
EvtWord pik_04_init_evt[166];

static BeroEntry bero_data[] = {
    {
        "nw_bero", // hitName
        0, 0, // type..sfxId
        7, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_00", // nextMapName
        "se_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "se_bero", // hitName
        0, 0, // type..sfxId
        3, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_01", // nextMapName
        "nw_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn dmgReturnPt[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {0.0f, -1000.0f, 0.0f}},
    {"mod_3", {0.0f, -1000.0f, 0.0f}},
    {"mod_4", {0.0f, -1000.0f, 0.0f}},
    {"mod_5", {0.0f, -1000.0f, 0.0f}},
    {"mod_6", {0.0f, -1000.0f, 0.0f}},
    {"mod_7", {0.0f, -1000.0f, 0.0f}},
    {"mod_8", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static const char* name_tbl[] = {
    "yajirusi_1",
    "yajirusi_2",
    "yajirusi_3",
    "yajirusi_4",
    "yajirusi_5",
    "yajirusi_6",
};

static EVT_BEGIN(evt_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_3_kabe_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ie_3_kabe_2"), 0, LW(0), 0)
    CALL(evt_mapobj_rotate, 1, STRING("ie_3_kabe_3"), 0, 0, LW(0))
    RETURN
EVT_END()

const char* ie_3_npc[] = {
    /*"ターくん１"*/"\x83\x5E\x81\x5B\x82\xAD\x82\xF1\x82\x50",
    /*"ターくん２"*/"\x83\x5E\x81\x5B\x82\xAD\x82\xF1\x82\x51",
    NULL,
};

static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_1 = {
    0, // flags
    1, // type
    1, // se
    "door_1", // mapobj
    "a_door_1_2", // hitIn
    "a_door_1_1", // hitOut
    "ie_3_soto", // outside
    "a_ie_3_soto", // outsideHit
    "ie_3_naka", // inside
    "a_ie_3_naka", // insideHit
    evt_door, // evtIn
    NULL, // evtOut
    evt_wall, // evtWall
    (const char**)&ie_3_npc, // npcs
    NULL, NULL, // mobjs..items
    door_1_open, // evtOpen
    door_1_close, // evtClose
};

EVT_BEGIN(evt_door_h)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("door_h_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_h_b"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door2_h)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("door_h_a"), LW(0), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("door_h_b"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_h)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.75f))
    CALL(evt_mapobj_rotate, 1, STRING("hotel_kabe_1"), 0, LW(0), 0)
    RETURN
EVT_END()

const char* hotel_npc[] = {
    /*"店員女"*/"\x93\x58\x88\xF5\x8F\x97",
    /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59",
    NULL,
};

static EVT_BEGIN(door_2_open)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ERE"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(door_2_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_2 = {
    0, // flags
    3, // type
    1, // se
    "door_h", // mapobj
    "a_door_h_2", // hitIn
    "a_door_h_1", // hitOut
    "hotel_soto", // outside
    "a_hotel_soto", // outsideHit
    "hotel_naka", // inside
    "a_hotel_naka", // insideHit
    evt_door_h, // evtIn
    NULL, // evtOut
    evt_wall_h, // evtWall
    (const char**)hotel_npc, // npcs
    NULL, NULL, // mobjs..items
    door_2_open, // evtOpen
    door_2_close, // evtClose
};

EVT_BEGIN(imginit)
    CALL(evt_paper_entry, STRING("OFF_hei_07"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_hei_07"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 0, 0, 0, 608, 480)
    CALL(evt_img_alloc_capture, STRING("capture_img"), 2, STRING("offscreen"), 0, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    RETURN
EVT_END()

EVT_BEGIN(imgstart)
    CALL(evt_mapobj_set_offscreen, 1, STRING("hotel_naka"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("door_h"), STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("ERE"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hotel_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_h"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ERE"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_set_offscreen, STRING("店員女"), STRING("offscreen"))
    CALL(evt_npc_set_offscreen, STRING("キザ野郎"), STRING("offscreen"))
    CALL(evt_npc_flag_onoff, 1, STRING("店員女"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_NO_DRAW)
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        CALL(evt_item_set_alpha, STRING("breakfast"), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("hotel_naka"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("door_h"))
    CALL(evt_npc_set_offscreen, STRING("店員女"), 0)
    CALL(evt_npc_set_offscreen, STRING("キザ野郎"), 0)
    RETURN
EVT_END()

EVT_BEGIN(imgend)
    CALL(evt_mapobj_set_offscreen, 1, STRING("hotel_naka"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("door_h"), STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("ERE"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hotel_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_h"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ERE"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_set_offscreen, STRING("店員女"), STRING("offscreen"))
    CALL(evt_npc_set_offscreen, STRING("キザ野郎"), STRING("offscreen"))
    CALL(evt_npc_flag_onoff, 1, STRING("店員女"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_NO_DRAW)
    CALL(evt_img_onoff, STRING("capture_img"), 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_img_set_color, STRING("capture_img"), 255, 255, 255, LW(0))
        CALL(evt_item_set_alpha, STRING("breakfast"), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("hotel_naka"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("door_h"))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hotel_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door_h"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_set_offscreen, STRING("店員女"), 0)
    CALL(evt_npc_set_offscreen, STRING("キザ野郎"), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("店員女"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 0, STRING("キザ野郎"), NPC_FLAG_NO_DRAW)
    RETURN
EVT_END()

EVT_BEGIN(evt_up)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ERE"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 40, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("door_2_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_hitobj_onoff, STRING("a_door_2"), 1, 0)
    CALL(evt_cam3d_evt_set, FLOAT(-78.0f), FLOAT(133.0f), FLOAT(-450.0f), FLOAT(307.0f), FLOAT(56.5f), FLOAT(-65.0f), 800, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 435, 45, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, 410, 67, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_set_pos, 0, 410, LW(1), 67)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_stop, 0)
    SPAWN_EVENT(PTR(imgstart))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(10))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 40, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("door_2_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_hitobj_onoff, STRING("a_door_2"), 1, 1)
    CALL(evt_cam3d_evt_set, FLOAT(-78.0f), FLOAT(1463.0f), FLOAT(-450.0f), FLOAT(307.0f), FLOAT(1386.5f), FLOAT(-65.0f), 10000, 11)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_hit_bind_mapobj, STRING("a_hako"), STRING("hako"))
    CALL(evt_snd_sfxon, STRING("SFX_STG6_PIK_ELEVATOR_MOVE1"), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 13250, 10000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("hako"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_hako"))
        ADD_FLOAT(LW(0), FLOAT(4.0f))
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
        CALL(evt_party_set_pos, 0, LW(2), LW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(imgend))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 40, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("door_3_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_cam3d_evt_off, 800, 11)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 383, -10, FLOAT(80.0f))
    CALL(evt_party_move_pos2, 0, 360, 10, FLOAT(80.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 40, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("door_3_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_down)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 4000, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_hitobj_onoff, STRING("a_door_3"), 1, 0)
    CALL(evt_cam3d_evt_set, FLOAT(-78.0f), FLOAT(1463.0f), FLOAT(-450.0f), FLOAT(307.0f), FLOAT(1386.5f), FLOAT(-65.0f), 800, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 435, 45, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, 410, 67, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_set_pos, 0, 410, LW(1), 67)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_stop, 0)
    SPAWN_EVENT(PTR(imgstart))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 4000, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_3_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_hitobj_onoff, STRING("a_door_3"), 1, 1)
    CALL(evt_cam3d_evt_set, FLOAT(-78.0f), FLOAT(133.0f), FLOAT(-450.0f), FLOAT(307.0f), FLOAT(56.5f), FLOAT(-65.0f), 10000, 11)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_hit_bind_mapobj, STRING("a_hako"), STRING("hako"))
    CALL(evt_snd_sfxon, STRING("SFX_STG6_PIK_ELEVATOR_MOVE1"), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 13250, 0, 10000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("hako"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_hako"))
        ADD_FLOAT(LW(0), FLOAT(4.0f))
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
        CALL(evt_party_set_pos, 0, LW(2), LW(0), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SPAWN_EVENT(PTR(imgend))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 4000, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_cam3d_evt_off, 800, 11)
    CALL(evt_mario_mov_pos2, 383, -10, FLOAT(80.0f))
    CALL(evt_party_move_pos2, 0, 360, 10, FLOAT(80.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PIK_ELEVATOR_DOOR1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 4000, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_b"), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, STRING("door_2_a"), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(init_miyage)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_position, STRING("me"), -363, 10, 142)
#else
    CALL(evt_npc_set_position, STRING("me"), -363, 15, 142)
#endif
    RETURN
EVT_END()

EVT_BEGIN(talk_miyage)
    RETURN
EVT_END()

static EVT_BEGIN(talk_musume)
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_10"), 0, STRING("３人娘１"))
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_11"), 0, STRING("３人娘２"))
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_12"), 0, STRING("３人娘３"))
    RETURN
EVT_END()

EVT_BEGIN(init_musume1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER(347)
        CALL(evt_npc_set_position, STRING("３人娘１"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume1)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_musume2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER(347)
        CALL(evt_npc_set_position, STRING("３人娘２"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume2)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_musume3)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER(347)
        CALL(evt_npc_set_position, STRING("３人娘３"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_musume3)
    CALL_CHILD(PTR(talk_musume))
    RETURN
EVT_END()

EVT_BEGIN(init_takun1)
    RETURN
EVT_END()

EVT_BEGIN(talk_takun1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_13"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_14"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_14_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_takun2)
    RETURN
EVT_END()

EVT_BEGIN(talk_takun2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_15"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_16"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_16_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_takun3)
    CALL(evt_npc_flag_onoff, 1, STRING("ターくん３"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY)
    RETURN
EVT_END()

EVT_BEGIN(talk_takun3)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_17"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_18"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_18_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_kiza)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(347)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_kiza)
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_19"), 0, STRING("me"))
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"土産屋"*/"\x93\x79\x8E\x59\x89\xAE", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_miyage, // initEvent
        NULL, // regularEvent
        talk_miyage, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘１"*/"\x82\x52\x90\x6C\x96\xBA\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_musume1, // initEvent
        NULL, // regularEvent
        talk_musume1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘２"*/"\x82\x52\x90\x6C\x96\xBA\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_musume2, // initEvent
        NULL, // regularEvent
        talk_musume2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"３人娘３"*/"\x82\x52\x90\x6C\x96\xBA\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_musume3, // initEvent
        NULL, // regularEvent
        talk_musume3, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ターくん１"*/"\x83\x5E\x81\x5B\x82\xAD\x82\xF1\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_takun1, // initEvent
        NULL, // regularEvent
        talk_takun1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ターくん２"*/"\x83\x5E\x81\x5B\x82\xAD\x82\xF1\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_takun2, // initEvent
        NULL, // regularEvent
        talk_takun2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員女"*/"\x93\x58\x88\xF5\x8F\x97", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        init_kiza, // initEvent
        NULL, // regularEvent
        talk_kiza, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_roten)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_unitwin_alloc, 0)
    CALL(evt_msg_print, 0, STRING("stg7_pik_shop_00"), 0, STRING("土産屋"))
    CALL(unitwincoin_disp_onoff, 1)
    CALL(evt_msg_select, 0, STRING("stg7_pik_shop_01"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_pouch_get_haveitemcnt, LW(0))
        CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(1), 10)
            ELSE
            SET_VALUE(LW(1), 20)
        END_IF
        IF_INT_LESS(LW(0), LW(1))
            CALL(evt_pouch_get_coin, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 50)
                CALL(evt_pouch_add_coin, -50)
                CALL(unitwincoin_wait)
                WAIT_MS(200)
                CALL(unitwincoin_disp_onoff, 0)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_shop_02"))
                CALL(evt_item_entry, STRING("rp"), ITEM_FRESH_PASTA, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("rp"))
                CALL(evt_item_get_item_end_wait, STRING("rp"))
                ELSE
                CALL(unitwincoin_disp_onoff, 0)
                CALL(evt_msg_print_add, 0, STRING("stg7_pik_shop_03"))
            END_IF
            ELSE
            CALL(unitwincoin_disp_onoff, 0)
            CALL(evt_msg_print_add, 0, STRING("stg7_pik_shop_04"))
        END_IF
        ELSE
        CALL(unitwincoin_disp_onoff, 0)
        CALL(evt_msg_print_add, 0, STRING("stg7_pik_shop_05"))
    END_IF
    CALL(evt_unitwin_free)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kinopio_to_bed)
    CALL(evt_mario_mov_pos2, 340, -25, FLOAT(70.0f))
    CALL(evt_mario_mov_pos2, 370, 5, FLOAT(70.0f))
    CALL_CHILD(PTR(evt_up))
    CALL(evt_mario_mov_pos2, 285, -65, FLOAT(70.0f))
    CALL(evt_mario_mov_pos2, 245, -20, FLOAT(70.0f))
    CALL(evt_mario_mov_pos2, 270, 60, FLOAT(70.0f))
    RETURN
EVT_END()

EVT_BEGIN(evt_kinopio_from_bed)
    CALL(evt_party_run, 0)
    CALL(evt_mario_mov_pos2, 220, -15, FLOAT(70.0f))
    CALL(evt_mario_mov_pos2, 285, -80, FLOAT(70.0f))
    CALL(evt_mario_mov_pos2, 360, -10, FLOAT(70.0f))
    CALL_CHILD(PTR(evt_down))
    CALL(evt_mario_mov_pos2, 300, -20, FLOAT(70.0f))
    RETURN
EVT_END()

static KinopioData kino_dt = {
    221, // breakfastItem
    {360.0f, 33.0f, -235.0f}, // breakfastPos
    30, // ryoukin
    {290.0f, 1345.0f, 80.0f}, // bed
    {270.0f, 1330.0f, 60.0f}, // bedside
    {185.0f, 1442.0f, -285.0f, 295.0f, 1386.0f, -24.0f}, // cam
    -60.0f, // bedDeg
    {260.0f, 5.0f, 30.0f}, // npcTalk
    /*"店員女"*/"\x93\x58\x88\xF5\x8F\x97", // npcName
    {"stg7_pik_clear_20", "stg7_pik_clear_21", "stg7_pik_clear_22", "stg7_pik_clear_23", "stg7_pik_clear_24", "stg7_pik_clear_25", "stg7_pik_clear_26", "stg7_pik_clear_27", "stg7_pik_clear_28", "stg7_pik_clear_29", "stg7_pik_clear_30", "stg7_pik_clear_31", "stg7_pik_clear_32", "stg7_pik_clear_33"}, // msg
    evt_kinopio_to_bed, // toBedEvt
    evt_kinopio_from_bed, // fromBedEvt
};

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("シュリョー"), STRING("c_shuryo"))
    CALL(evt_npc_set_tribe, STRING("シュリョー"), STRING("第三勢力総統"))
    CALL(evt_npc_set_position, STRING("シュリョー"), 10, 0, 250)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_4"))
    CALL(evt_npc_set_width, STRING("シュリョー"), 40)
    CALL(evt_npc_entry, STRING("カンブー"), STRING("c_kanbu_ibt"))
    CALL(evt_npc_set_tribe, STRING("カンブー"), STRING("第三勢力幹部"))
    CALL(evt_npc_set_position, STRING("カンブー"), 90, 0, 250)
    CALL(evt_npc_set_ry, STRING("カンブー"), 270)
    CALL(evt_npc_entry, STRING("軍団Ａ"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("軍団Ａ"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("軍団Ａ"), 35, 5, 1240)
    CALL(evt_npc_set_ry, STRING("軍団Ａ"), 270)
    CALL(evt_npc_entry, STRING("軍団Ｂ"), STRING("c_zako_m"))
    CALL(evt_npc_set_tribe, STRING("軍団Ｂ"), STRING("第三勢力研究員"))
    CALL(evt_npc_set_position, STRING("軍団Ｂ"), 7, 5, 151)
    CALL(evt_npc_set_ry, STRING("軍団Ｂ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("軍団Ｂ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("軍団Ｃ"), STRING("c_zako_e"))
    CALL(evt_npc_set_tribe, STRING("軍団Ｃ"), STRING("第三勢力エリート"))
    CALL(evt_npc_set_position, STRING("軍団Ｃ"), -48, 0, 193)
    CALL(evt_npc_set_ry, STRING("軍団Ｃ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("軍団Ｃ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_entry, STRING("軍団Ｄ"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("軍団Ｄ"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("軍団Ｄ"), 142, 5, 233)
    CALL(evt_npc_set_ry, STRING("軍団Ｄ"), 270)
    CALL(evt_npc_entry, STRING("軍団Ｅ"), STRING("c_zako_e"))
    CALL(evt_npc_set_tribe, STRING("軍団Ｅ"), STRING("第三勢力エリート"))
    CALL(evt_npc_set_position, STRING("軍団Ｅ"), 138, 5, 316)
    CALL(evt_npc_set_ry, STRING("軍団Ｅ"), 270)
    CALL(evt_npc_entry, STRING("ポワン"), STRING("c_powan"))
    CALL(evt_npc_set_tribe, STRING("ポワン"), STRING("ポワン探偵"))
    CALL(evt_npc_set_position, STRING("ポワン"), -29, 0, 455)
    CALL(evt_npc_set_ry, STRING("ポワン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン"), NPC_FLAG_INSTANT_TURN)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt_sub)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    LOOP(3)
        CALL(evt_npc_jump_position_nohit, STRING("シュリョー"), LW(0), LW(1), LW(2), 150, 10)
        WAIT_MS(50)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("カンブー"), STRING("T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カンブー"), STRING("S_1"))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_EPILOGUE_3RD_BATTEN1"), 0)
    CALL(evt_npc_set_anim, STRING("軍団Ａ"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("軍団Ｂ"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("軍団Ｃ"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("軍団Ｄ"), STRING("P_1"))
    CALL(evt_npc_set_anim, STRING("軍団Ｅ"), STRING("P_1"))
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
        LOOP(3)
            CALL(evt_npc_jump_position_nohit, STRING("シュリョー"), LW(0), LW(1), LW(2), 150, 10)
            WAIT_MS(50)
        END_LOOP
        CALL(evt_npc_set_anim, STRING("カンブー"), STRING("T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("カンブー"), STRING("S_1"))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, 50, 150, 842, 50, 50, 250, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("ep_44"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("tik_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(pik_04_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_snd_bgm_scope, 0, 1)
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_PIK1_2"))
    CALL(evt_map_playanim, STRING("anm_taki"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_mizu_1"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_mizu_2"), 1, 0)
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(6)
        READ_INT(LW(9))
        SPAWN_EVENT(PTR(evt_pik_yajirushi))
    END_LOOP
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_npc_setup, PTR(npc_data))
    CALL_CHILD(PTR(init_takun3))
    CALL(evt_npc_change_interrupt, STRING("ターくん３"), 6, PTR(talk_takun3))
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_2))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_door_2"), 0, PTR(evt_up), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_door_3"), 0, PTR(evt_down), 0)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("nw_bero"), 4)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("se_bero"), 4)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_roten"), 0, PTR(evt_roten), 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -250, 5, -425, ITEM_TYPE_STILL, GSWF(5657), 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_INN_COUPON, -305, 0, 105, ITEM_TYPE_STILL, GSWF(3279), 0)
    CALL_CHILD(PTR(imginit))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("front_"), 0, PTR(kinopio_house_talk_evt), 0)
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_bero"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("group92"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

