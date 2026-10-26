#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_kinopio.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/mobjdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_mario.h"
#include "bom.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"

static USER_FUNC(anim_tevcallback);
static USER_FUNC(anim_tevcallback_mobj);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_win_item_select);
static USER_FUNC(yami_view);
extern s32 envSetYami[];

EvtWord evt_ie1_door_1[12];
EvtWord evt_ie1_wall[12];
const char* ie1_npc[2];
EvtWord evt_ie1_door_open[12];
EvtWord evt_ie1_door_close[14];
DoorParam ie1_door;
EvtWord evt_ie2_door_1[12];
EvtWord evt_ie2_wall[12];
const char* ie2_npc[2];
EvtWord evt_ie2_door_open[12];
EvtWord evt_ie2_door_close[14];
DoorParam ie2_door;
EvtWord evt_ie3_door_1[12];
EvtWord evt_ie3_door_2[12];
EvtWord evt_ie3_wall[12];
const char* ie3_npc[3];
EvtWord evt_ie3_door_open[12];
EvtWord evt_ie3_door_close[14];
const char* ie3_item[2];
DoorParam ie3_door;
static BeroEntry bero_entry_data[2];
static EvtWord evt_kime_pose[52];
static EvtWord evt_taihou[190];
EvtWord urotuku_white[190];
static EvtWord init_soncho[13];
static EvtWord talk_soncho[421];
static EvtWord init_murabito_e[2];
#ifdef VERSION_G8ME01
static EvtWord talk_murabito_e[281];
#else
static EvtWord talk_murabito_e[253];
#endif
static EvtWord init_murabito_f[2];
static EvtWord talk_murabito_f[44];
static EvtWord init_murabito_g[17];
static EvtWord talk_murabito_g[81];
static EvtWord init_murabito_h[2];
s32 iri_26_item_tbl[2];
static EvtWord talk_murabito_h[151];
static EvtWord init_murabito_i[2];
static EvtWord talk_murabito_i[44];
EvtWord init_tenin[8];
EvtWord talk_tenin[2];
static EvtWord init_white[83];
#ifdef VERSION_G8ME01
EvtWord mario_jump[203];
#else
EvtWord mario_jump[197];
#endif
#ifndef VERSION_G8ME01
EvtWord mario_jump_tail[6];
#endif
s32 iri_25_item_tbl[2];
static EvtWord talk_white[117];
EvtWord iri_25_white_bed[363];
EvtWord evt_white_bed[505];
static EvtWord kiza_talk[28];
static EvtWord kiza_entry[30];
static NpcSetupInfo npcEnt[9];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[13];
static ShopperData35 shopper_data;
static EvtWord sekai_yami_evt[459];
static EvtWord minnnanokoe_evt[183];
static KinopioData kino_dt;
EvtWord bom_02_init_evt[312];

EVT_BEGIN(evt_ie1_door_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie1_door"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
    RETURN
EVT_END()

EVT_BEGIN(evt_ie1_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.6660156f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie1_kai"), FLOAT(0.0f), FLOAT(0.0f), LW(0))
    RETURN
EVT_END()

const char* ie1_npc[] = {
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(evt_ie1_door_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_ie1_door_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam ie1_door = {
    0, // flags
    3, // type
    1, // se
    "S_ie1_door", // mapobj
    "A_ie1_door1", // hitIn
    "A_ie1_door1_u", // hitOut
    "S_ie1_soto", // outside
    NULL, // outsideHit
    "S_ie1_naka", // inside
    NULL, // insideHit
    evt_ie1_door_1, // evtIn
    NULL, // evtOut
    evt_ie1_wall, // evtWall
    ie1_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_ie1_door_open, // evtOpen
    evt_ie1_door_close, // evtClose
};

EVT_BEGIN(evt_ie2_door_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie2_door"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
    RETURN
EVT_END()

EVT_BEGIN(evt_ie2_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie2_kai"), FLOAT(0.0f), FLOAT(0.0f), LW(0))
    RETURN
EVT_END()

const char* ie2_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(evt_ie2_door_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_ie2_door_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam ie2_door = {
    0, // flags
    3, // type
    1, // se
    "S_ie2_door", // mapobj
    "A__ie2_door2", // hitIn
    "A__ie2_door2_u", // hitOut
    "S_ie2_soto", // outside
    NULL, // outsideHit
    "S_ie2_naka", // inside
    NULL, // insideHit
    evt_ie2_door_1, // evtIn
    NULL, // evtOut
    evt_ie2_wall, // evtWall
    ie2_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_ie2_door_open, // evtOpen
    evt_ie2_door_close, // evtClose
};

EVT_BEGIN(evt_ie3_door_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie3_door"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
    RETURN
EVT_END()

EVT_BEGIN(evt_ie3_door_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie3_door"), FLOAT(0.0f), FLOAT(0.0f), LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_ie3_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie3_kai"), FLOAT(0.0f), FLOAT(0.0f), LW(0))
    RETURN
EVT_END()

const char* ie3_npc[] = {
    /*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67",
    /*"ボムヘイG"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x47",
    NULL,
};

EVT_BEGIN(evt_ie3_door_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_ie3_door_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

const char* ie3_item[] = {
    "item_00",
    NULL,
};

DoorParam ie3_door = {
    0, // flags
    3, // type
    1, // se
    "S_ie3_door", // mapobj
    "A_ie3_door3_u", // hitIn
    "A_ie3_door3", // hitOut
    "S_ie3_soto", // outside
    NULL, // outsideHit
    "S_ie3_naka", // inside
    NULL, // insideHit
    evt_ie3_door_1, // evtIn
    evt_ie3_door_2, // evtOut
    evt_ie3_wall, // evtWall
    ie3_npc, // npcs
    NULL, // mobjs
    ie3_item, // items
    evt_ie3_door_open, // evtOpen
    evt_ie3_door_close, // evtClose
};

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
        "bom_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_kime_pose)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_Y_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_Y_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("Y_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_Y_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_Y_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_Y_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(evt_taihou)
    CALL(evt_cam3d_evt_set, 140, 100, 593, 140, 40, 248, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 180, 225, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 160, 250, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_60"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL_CHILD(PTR(evt_kime_pose))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_party_set_breed_pose, 0, 1)
    CALL(evt_msg_print_party, STRING("stg7_bom_60_2"))
    WAIT_MS(300)
    CALL(evt_party_run, 0)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg7_bom_61"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 50, 152, 800, 50, 57, 260, 3000, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("me"), -45, 270, 0, FLOAT(60.0f), 1)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir_npc, STRING("me"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir_npc, 0, STRING("me"))
    END_INLINE_EVENT
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_62"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("me"), -140, 200, 0, FLOAT(60.0f), 1)
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    SET_VALUE(GSW_STORY_PROGRESS, STG7_04_BACK_ONLINE)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    RETURN
EVT_END()

EVT_BEGIN(urotuku_white)
    IF_INT_EQUAL(GSWF(3888), 1)
        CALL(evt_msg_print, 0, STRING("stg7_bom_41_01"), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_cam3d_evt_set, 120, 97, 700, 120, 65, 251, 800, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg7_bom_39"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    CALL(evt_npc_set_position, STRING("ホワイト"), 230, 30, -125)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(evt_ie3_door_1))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_ie3_door_1))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("ホワイト"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ホワイト"), 200, -85, 0, FLOAT(60.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg7_bom_41"), 0, STRING("ホワイト"))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_ie3_door_1))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
        CALL(evt_npc_move_position, STRING("ホワイト"), 230, -125, 0, FLOAT(80.0f), 1)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LABEL(4)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(evt_ie3_door_1))
        IF_INT_EQUAL(LW(1), 1)
            GOTO(4)
        END_IF
        CALL(evt_npc_set_position, STRING("ホワイト"), 380, 45, -150)
        CALL(evt_npc_set_ry, STRING("ホワイト"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("ホワイト"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("N_1"))
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    CALL(evt_msg_print_add, 0, STRING("stg7_bom_40"))
    CALL(evt_msg_print_add, 0, STRING("stg7_bom_40_02"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSWF(3888), 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_soncho)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_npc_set_position, STRING("me"), 120, 0, 265)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_soncho)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_20_00"), 0, STRING("me"))
        CASE_LESS(STG7_02_MAYOR_BOB)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            IF_INT_EQUAL(GSWF(3886), 0)
                SET_VALUE(GSWF(3886), 1)
                CALL(evt_msg_print, 0, STRING("stg7_bom_20"), 0, STRING("me"))
                WAIT_MS(300)
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(800)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print, 0, STRING("stg7_bom_21"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_bom_21_01"), 0, STRING("me"))
                CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
                RETURN
            END_IF
            WAIT_MS(300)
            CALL(evt_party_stop, 0)
            WAIT_MS(300)
            CALL(evt_party_move_pos2, 0, 80, 300, FLOAT(60.0f))
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_msg_print_party, STRING("stg7_bom_22"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_23"), 0, STRING("me"))
            CALL(evt_party_run, 0)
            ELSE
            BEGIN_INLINE_EVENT
                CALL(evt_party_move_pos, 0, 50, 280, 500)
                CALL(evt_party_set_dir_npc, 0, STRING("me"))
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos, 70, 260, 500)
            CALL(evt_mario_set_dir, 90, 200, 0)
            CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            IF_INT_EQUAL(GSWF(3886), 0)
                SET_VALUE(GSWF(3886), 1)
                CALL(evt_msg_print, 0, STRING("stg7_bom_20"), 0, STRING("me"))
                WAIT_MS(300)
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(800)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print, 0, STRING("stg7_bom_21"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_bom_21_01"), 0, STRING("me"))
            END_IF
            CALL(evt_party_stop, 0)
            WAIT_MS(300)
            CALL(evt_party_move_pos2, 0, 80, 300, FLOAT(60.0f))
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_24"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1000)
            CALL(evt_cam3d_evt_set, 90, 105, 611, 90, 45, 266, 500, EASING_SINE_IN_OUT)
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("stg7_bom_25"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_26"), 0, STRING("me"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_27"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
            CALL(evt_msg_print, 0, STRING("stg7_bom_28"), 0, STRING("me"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_29"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_30"), 0, STRING("me"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_31"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_32"), 0, STRING("me"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("stg7_bom_32_02"), 0, STRING("party"))
            WAIT_MS(300)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_33"), 0, STRING("me"))
            SET_VALUE(GSW_STORY_PROGRESS, STG7_02_MAYOR_BOB)
            CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
            CALL(evt_party_run, 0)
        END_IF
        CASE_LESS(STG7_03_GOLDBOB)
        SET_VALUE(LW(0), 0)
        IF_INT_EQUAL(GSWF(3874), 1)
            ADD(LW(0), 1)
        END_IF
        IF_INT_EQUAL(GSWF(3876), 1)
            ADD(LW(0), 1)
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg7_bom_34"), 0, STRING("me"))
            CALL(evt_msg_select, 0, STRING("stg7_bom_34_yn"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("stg7_bom_38"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("stg7_bom_37"))
            END_IF
            CASE_EQUAL(1)
            IF_INT_EQUAL(GSWF(3874), 0)
                CALL(evt_msg_print, 0, STRING("stg7_bom_38_01"), 0, STRING("me"))
            END_IF
            IF_INT_EQUAL(GSWF(3876), 0)
                IF_INT_EQUAL(GSWF(3880), 0)
                    CALL(evt_msg_print, 0, STRING("stg7_bom_38_02"), 0, STRING("me"))
                    ELSE
                    CALL_CHILD(PTR(urotuku_white))
                END_IF
            END_IF
        END_SWITCH
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL_CHILD(PTR(evt_taihou))
    END_SWITCH
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

static EVT_BEGIN(init_murabito_e)
    RETURN
EVT_END()

static EVT_BEGIN(talk_murabito_e)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ERASE_THAT_GRAFFITI)
#ifdef VERSION_G8ME01
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
#endif
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_30_001"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_30_002"), 0, 0)
#ifndef VERSION_G8ME01
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
#endif
                CALL(evt_msg_print_add, 0, STRING("irai_30_003"))
#ifdef VERSION_G8ME01
            END_IF
#endif
                ELSE
#ifdef VERSION_G8ME01
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
                SET_VALUE(LF(0), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_30_002"), 0, 0)
#endif
                CALL(evt_msg_continue)
#ifdef VERSION_G8ME01
            END_IF
#endif
                CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
                SET_VALUE(GF(0), 1)
                BEGIN_INLINE_EVENT
                    CALL(evt_party_move_pos2, 0, 175, 45, 60)
                    CALL(evt_party_set_dir_npc, 0, STRING("me"))
                    WAIT_MS(300)
                    SET_VALUE(GF(0), 0)
                END_INLINE_EVENT
                CALL(evt_mario_mov_pos2, 145, 45, 60)
                CALL(evt_mario_set_dir_npc, STRING("me"))
                WAIT_WHILE(GF(0))
                CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
                CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("me"))
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(1000)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
            IF_INT_EQUAL(LF(0), 0)
#endif
                CALL(evt_msg_print, 0, STRING("irai_30_004"), 0, 0)
#ifdef VERSION_G8ME01
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_30_003_1"), 0, 0)
            END_IF
#endif
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    SET_VALUE(LW(1), 10)
                    ELSE
                    SET_VALUE(LW(1), 20)
                END_IF
                IF_INT_LESS(LW(0), LW(1))
                    CALL(evt_msg_continue)
                    CALL(evt_item_entry, STRING("sr"), ITEM_SNOW_BUNNY, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("sr"))
                    CALL(evt_item_get_item_end_wait, STRING("sr"))
                    SET_VALUE(GSWF(3890), 1)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_30_005_1"))
                    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                    CALL(evt_npc_move_position, STRING("me"), 50, 35, 0, 60, 0)
                    CALL(evt_item_entry, STRING("iri_30"), ITEM_SNOW_BUNNY, 50, 0, 35, ITEM_TYPE_STILL, GSWF(3890), 0)
                    WAIT_MS(600)
                    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 60, 0)
                    CALL(evt_npc_status_onoff, 0, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_30_006"), 0, 0)
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_ERASE_THAT_GRAFFITI), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
            END_IF
#ifndef VERSION_G8ME01
        END_IF
#endif
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_10_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_10"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
        CALL(evt_msg_print, 0, STRING("stg7_bom_10_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_11"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_11_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_murabito_f)
    RETURN
EVT_END()

static EVT_BEGIN(talk_murabito_f)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_12_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_12"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
        CALL(evt_msg_print, 0, STRING("stg7_bom_12_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_13"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_13_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_murabito_g)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        CALL(evt_npc_set_position, STRING("me"), 270, 30, -180)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        RETURN
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_murabito_g)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                CALL(evt_msg_print, 0, STRING("irai_25_00"), 0, 0)
                SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                RETURN
            END_IF
        END_IF
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(4), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(5), 0)
                CALL(evt_msg_print, 0, STRING("irai_25_01"), 0, 0)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_14_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_14"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
        CALL(evt_msg_print, 0, STRING("stg7_bom_14_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_15"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_15_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_murabito_h)
    RETURN
EVT_END()

s32 iri_26_item_tbl[] = {
    ITEM_HONEY_CANDY, -1,
};

static EVT_BEGIN(talk_murabito_h)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_I_CANT_SPEAK)
        CALL(evt_pouch_check_item, ITEM_HONEY_CANDY, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_26_01"), 0, 0)
            CALL(evt_msg_select, 0, STRING("irai_26_01_s"))
            CALL(evt_msg_print_add, 0, STRING("irai_26_02"))
            CALL(evt_msg_select, 0, STRING("irai_26_02_s"))
            CALL(evt_msg_print_add, 0, STRING("irai_26_03"))
            CALL(evt_msg_select, 0, STRING("irai_26_03_s"))
            CALL(evt_msg_print_add, 0, STRING("irai_26_04"))
            CALL(evt_win_item_select, 0, PTR(iri_26_item_tbl), LW(0), 0)
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(evt_pouch_remove_item, ITEM_HONEY_CANDY, LW(0))
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                LOOP(3)
                    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 150, 5)
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_msg_print, 0, STRING("irai_26_05"), 0, 0)
                CALL(evt_sub_get_coin, 20)
                CALL(evt_sub_save_playtime, GSW(1461))
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_I_CANT_SPEAK), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_26_00"), 0, 0)
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_16_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_16"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
        CALL(evt_msg_print, 0, STRING("stg7_bom_16_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_17"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_17_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_murabito_i)
    RETURN
EVT_END()

static EVT_BEGIN(talk_murabito_i)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
        CALL(evt_msg_print, 0, STRING("stg7_bom_18_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_18"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
        CALL(evt_msg_print, 0, STRING("stg7_bom_18_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_19"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_19_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_tenin)
    CALL(evt_npc_set_position, STRING("me"), 125, 30, -450)
    RETURN
EVT_END()

EVT_BEGIN(talk_tenin)
    RETURN
EVT_END()

static EVT_BEGIN(init_white)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(4), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(6), 0)
                CALL(evt_npc_set_position, STRING("ホワイト"), 380, 45, -150)
                CALL(evt_npc_set_ry, STRING("ホワイト"), 270)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(5), 1)
                    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_TALK_TURN)
                    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
                    ELSE
                    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("N_1"))
                    CALL(evt_npc_set_width, STRING("ホワイト"), 10)
                    CALL(evt_npc_change_interrupt, STRING("ホワイト"), 6, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(3880), 1)
        IF_INT_EQUAL(GSWF(3876), 0)
            CALL(evt_npc_set_width, STRING("ホワイト"), 10)
            CALL(evt_npc_set_position, STRING("ホワイト"), 380, 45, -150)
            CALL(evt_npc_set_ry, STRING("ホワイト"), 270)
            CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("N_1"))
            CALL(evt_npc_change_interrupt, STRING("ホワイト"), 6, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(mario_jump)
    CALL(evt_mario_cam_keep_on)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_position, STRING("ホワイト"), LW(0), LW(1), LW(2))
#else
/* JP: the script continues in mario_jump_tail (separate symbol); its last CALL is split across the two */
EvtWord mario_jump[] = {
    CALL(evt_mario_cam_keep_on)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_position, STRING(/*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67"), LW(0), LW(1), LW(2))
#endif
    ADD(LW(1), 20)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 500, 2, -1000)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        SUBTRACT(LW(0), 40)
        SUBTRACT(LW(2), 40)
#else
        SUBTRACT(LW(0), 20)
        SUBTRACT(LW(2), 20)
#endif
        CALL(evt_eff, 0, STRING("nice"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_eff, 0, STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -1, 0, 3, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_DAMAGED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_NICE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 100)
    END_INLINE_EVENT
    ADD(LW(2), -10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP4"), LW(10), LW(11), LW(12), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 500, 2, -1000)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        SUBTRACT(LW(0), 40)
        SUBTRACT(LW(2), 40)
#else
        SUBTRACT(LW(0), 20)
        SUBTRACT(LW(2), 20)
#endif
        CALL(evt_eff, 0, STRING("nice"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_eff, 0, STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -1, 0, 3, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_DAMAGED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_GOOD1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 100)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP2"), LW(10), LW(11), LW(12), 0)
    CALL(evt_mario_jump_pos, 335, 30, -130, 500, 2, -1000)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("ホワイト"), NPC_FLAG_NO_HIT)
    CALL(evt_mario_cam_keep_off)
    RETURN
EVT_END()
#else
    EVT_CMD(4, OPCODE_USER_FUNC), (EvtWord)evt_npc_flag_onoff, 0,
};
#endif

#ifndef VERSION_G8ME01  // US: this tail is part of mario_jump (US script above)
EvtWord mario_jump_tail[] = {
    STRING(/*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67"), 32, /* rest of mario_jump's evt_npc_flag_onoff call */
    CALL(evt_mario_cam_keep_off)
    RETURN
    OPCODE_END_SCRIPT,
};
#endif

s32 iri_25_item_tbl[] = {
    ITEM_PACKAGE, -1,
};

static EVT_BEGIN(talk_white)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(4), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(6), 0)
                SET_VALUE(LF(0), 0)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(5), 1)
                    SET_VALUE(LF(0), 1)
                END_IF
                IF_INT_EQUAL(LF(0), 1)
                    CALL(evt_msg_print, 0, STRING("irai_25_19"), 0, STRING("ホワイト"))
                END_IF
                CALL(evt_win_item_select, 0, PTR(iri_25_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), 109)
                    RETURN
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_25_18"), 0, STRING("ホワイト"))
                CALL(evt_pouch_remove_item, ITEM_PACKAGE, LW(0))
                SET_VALUE(GSWF_TROUBLE_WORK(6), 1)
                BEGIN_INLINE_EVENT
                    WAIT_MS(1000)
                    CALL(evt_mario_set_dir, 270, 300, 0)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(1000)
                    CALL(evt_party_set_dir_npc, 0, STRING("ホワイト"))
                END_INLINE_EVENT
                CALL(evt_npc_set_width, STRING("ホワイト"), 40)
                CALL(evt_npc_status_onoff, 1, STRING("ホワイト"), NPC_STATUS_FACE_MOVE_DIR)
                CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_NO_HIT)
                CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
                CALL(evt_npc_jump_position_nohit, STRING("ホワイト"), 245, 30, -110, 500, 80)
                CALL(evt_npc_move_position, STRING("ホワイト"), 170, -55, 0, FLOAT(80.0f), 1)
                CALL(evt_npc_set_position, STRING("ホワイト"), 0, -1000, 0)
                RETURN
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_25_white_bed)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print, 0, STRING("irai_25_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_mario_mov_pos2, 330, -120, 60)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(200)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_03"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_04"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_05"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_06"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_07"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_08"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_09"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_10"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_11"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("irai_25_12"), 0, 0)
    CALL(evt_msg_select, 0, STRING("irai_25_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(100)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    GOTO(101)
    LABEL(100)
    CALL(evt_msg_print_add, 0, STRING("irai_25_13"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
    LABEL(101)
    CALL(evt_party_run, 0)
    CALL(evt_msg_print, 0, STRING("irai_25_14"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("N_2"))
    WAIT_MS(600)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_ROUND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("O_1"))
    WAIT_MS(1900)
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("O_2"))
    WAIT_MS(1200)
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("irai_25_15"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("irai_25_16"), 0, STRING("ホワイト"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("irai_25_17"), 0, STRING("ホワイト"))
    CALL_CHILD(PTR(talk_white))
    CALL(evt_npc_change_interrupt, STRING("ホワイト"), 6, PTR(talk_white))
    CALL(evt_npc_set_width, STRING("ホワイト"), 30)
    SET_VALUE(GSWF_TROUBLE_WORK(5), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_white_bed)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print, 0, STRING("stg7_bom_43"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_43_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_NO_HIT)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("ホワイト"), LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dir, LW(3), LW(5), LW(0), LW(2), LW(6))
    IF_INT_GREATER_OR_EQUAL(LW(6), 80)
        IF_INT_LESS_OR_EQUAL(LW(6), 100)
            CALL(evt_mario_mov_pos2, 376, -111, 60)
        END_IF
    END_IF
    CALL(evt_mario_mov_pos2, 330, -120, 60)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("ホワイト"), NPC_FLAG_NO_HIT)
    CALL(evt_mario_get_dispdir, LW(0))
    IF_INT_LESS(LW(0), 180)
        CALL(evt_mario_set_dir, 90, 200, 0)
        WAIT_MS(200)
    END_IF
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_44"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_44_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_45"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_45_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_46"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_46_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_47"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_47_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_48"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_48_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_49"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_49_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_50"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_50_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_51"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_51_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_52"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_52_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    CALL(evt_msg_print, 0, STRING("stg7_bom_53"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg7_bom_53_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_msg_continue)
    CALL_CHILD(PTR(mario_jump))
    GOTO(1)
    LABEL(0)
    CALL(evt_msg_print_add, 0, STRING("stg7_bom_54"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
    LABEL(1)
    CALL(evt_msg_print, 0, STRING("stg7_bom_55"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("N_2"))
    WAIT_MS(600)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WHITE_ROUND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("O_1"))
    WAIT_MS(1900)
    CALL(evt_npc_set_anim, STRING("ホワイト"), STRING("O_2"))
    WAIT_MS(1200)
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg7_bom_56"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg7_bom_57"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg7_bom_58"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("stg7_bom_59"), 0, STRING("ホワイト"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 270, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_party_set_dir_npc, 0, STRING("ホワイト"))
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("ホワイト"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ホワイト"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_jump_position_nohit, STRING("ホワイト"), 245, 30, -110, 500, 80)
    CALL(evt_npc_move_position, STRING("ホワイト"), 170, -55, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_set_position, STRING("ホワイト"), 0, -1000, 0)
    SET_VALUE(GSWF(3876), 1)
    SET_VALUE(LW(0), 0)
    IF_INT_EQUAL(GSWF(3874), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(GSWF(3876), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SET_VALUE(GSW_STORY_PROGRESS, STG7_03_GOLDBOB)
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(kiza_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_02_MAYOR_BOB)
        CALL(evt_msg_print, 0, STRING("stg7_bom_19_01"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
        CALL(evt_msg_print, 0, STRING("stg7_bom_19_02"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_bom_19_03"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kiza_entry)
    CALL(evt_npc_entry, STRING("キザ野郎"), STRING("c_kiza"))
    CALL(evt_npc_set_tribe, STRING("キザ野郎"), STRING("キザ野郎"))
    CALL(evt_npc_set_position, STRING("キザ野郎"), -190, 5, -130)
    CALL(evt_npc_set_ry, STRING("キザ野郎"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("キザ野郎"), 6, PTR(kiza_talk))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"村長"*/"\x91\xBA\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_soncho, // initEvent
        NULL, // regularEvent
        talk_soncho, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイE"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x45", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_e, // initEvent
        NULL, // regularEvent
        talk_murabito_e, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイF"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x46", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_f, // initEvent
        NULL, // regularEvent
        talk_murabito_f, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイG"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x47", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_g, // initEvent
        NULL, // regularEvent
        talk_murabito_g, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイH"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x48", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_h, // initEvent
        NULL, // regularEvent
        talk_murabito_h, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイI"*/"\x83\x7B\x83\x80\x83\x77\x83\x43\x49", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_i, // initEvent
        NULL, // regularEvent
        talk_murabito_i, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_tenin, // initEvent
        NULL, // regularEvent
        talk_tenin, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67", // name
        NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_white, // initEvent
        NULL, // regularEvent
        talk_white, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_SHOOTING_STAR, 30},
    {ITEM_ICE_STORM, 6},
    {ITEM_RUIN_POWDER, 15},
    {ITEM_STOPWATCH, 12},
    {ITEM_MAPLE_SYRUP, 20},
    {ITEM_SUPER_SHROOM, 15},
};

static ShopDb trade_list[] = {
    {ITEM_SHOOTING_STAR, 15, 0},
    {ITEM_ICE_STORM, 3, 0},
    {ITEM_RUIN_POWDER, 7, 0},
    {ITEM_STOPWATCH, 6, 0},
    {ITEM_MAPLE_SYRUP, 10, 0},
    {ITEM_SUPER_SHROOM, 7, 0},
    {ITEM_FIRE_FLOWER, 8, 0},
    {ITEM_HOT_SAUCE, 14, 0},
    {ITEM_SPICY_PASTA, 60, 0},
    {ITEM_SPICY_SOUP, 20, 0},
    {ITEM_INKY_SAUCE, 15, 0},
    {ITEM_FIRE_POP, 24, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"shop_00", "shop_01", "shop_02", "shop_03", "shop_04", "shop_05", "shop_06", "shop_07", "shop_08", "shop_09", "shop_10", "shop_11", "shop_12", "shop_13", "shop_14", "shop_15", "shop_16", "shop_17", "shop_18", "shop_19", "shop_20", "shop_21", "shop_22", "shop_23", "shop_24", "shop_25", "shop_26", "shop_27", "shop_28", "shop_29", "shop_30", "shop_31", "shop_32", "shop_33", "shop_34"}, // msgs
};

void envSetYamiView(Mtx mtx);

static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村長"), -30, 0, 175)
    CALL(evt_npc_set_ry, STRING("村長"), 90)
    CALL(evt_npc_set_position, STRING("ボムヘイE"), -160, 0, -40)
    CALL(evt_npc_set_position, STRING("ボムヘイF"), -165, 0, 295)
    CALL(evt_npc_set_position, STRING("ボムヘイG"), 117, 0, 251)
    CALL(evt_npc_status_onoff, 1, STRING("ボムヘイE"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_status_onoff, 1, STRING("ボムヘイF"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_status_onoff, 1, STRING("ボムヘイG"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_bom02"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_bom02"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(anim_tevcallback, STRING("ボムヘイE"), 1)
    CALL(anim_tevcallback, STRING("ボムヘイF"), 1)
    CALL(anim_tevcallback, STRING("ボムヘイG"), 1)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(evt_cam3d_evt_set, -30, 88, 723, -30, 55, 24, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(15))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイE"), 10, -40, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイE"), 190, -40, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイE"), 10, -40, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイE"), -160, -40, 0, 120, 0)
        END_LOOP
        WAIT_MS(2000)
        CALL(evt_npc_reverse_ry, STRING("ボムヘイE"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("ボムヘイE"))
        CALL(evt_npc_reverse_ry, STRING("ボムヘイE"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), -62, 295, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), 44, 295, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), 131, 295, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), 44, 295, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), -62, 295, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイF"), -165, 295, 0, 120, 0)
        END_LOOP
        WAIT_MS(2000)
        CALL(evt_npc_reverse_ry, STRING("ボムヘイF"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("ボムヘイF"))
        CALL(evt_npc_reverse_ry, STRING("ボムヘイF"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), 26, 251, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), -83, 251, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), -180, 251, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), -83, 251, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), 26, 251, 0, 120, 0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_move_position, STRING("ボムヘイG"), 117, 251, 0, 120, 0)
        END_LOOP
        WAIT_MS(2000)
        CALL(evt_npc_reverse_ry, STRING("ボムヘイG"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("ボムヘイG"))
        CALL(evt_npc_reverse_ry, STRING("ボムヘイG"))
    END_INLINE_EVENT
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0068359375f))
    LOOP(5)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), LW(0), 1000)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.75f))
    END_LOOP
    SET_VALUE(GF(0), 1)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(2000)
    CALL(evt_npc_reverse_ry, STRING("村長"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("村長"))
    CALL(evt_npc_reverse_ry, STRING("村長"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("村長"))
    CALL(evt_cam3d_evt_set, -30, 55, 473, -30, 35, 24, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_dark_08"), 0, STRING("村長"))
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("las_26"), STRING("sekai_yami2"))
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村長"), 50, 0, 200)
    CALL(evt_npc_set_ry, STRING("村長"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("村長"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("ボムヘイE"), -250, 0, 150)
    CALL(evt_npc_set_ry, STRING("ボムヘイE"), 90)
    CALL(evt_npc_status_onoff, 1, STRING("ボムヘイE"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_bom02"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_bom02"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(anim_tevcallback, STRING("ボムヘイE"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_cam3d_evt_set, 50, 218, 1025, 50, 70, 188, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("村長"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 30, 92, 484, 30, 40, 188, 2000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("ボムヘイE"), 0, 150, 2000, 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_63"), 0, STRING("ボムヘイE"))
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_64"), 0, STRING("村長"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("ボムヘイE"), -450, -50, 0, 90, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("村長"), -400, 0, 0, 90, 0)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("bom_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

static KinopioData kino_dt = {
    198, // breakfastItem
    {-425.0f, 50.0f, -200.0f}, // breakfastPos
    15, // ryoukin
    {-285.0f, 46.0f, -223.0f}, // bed
    {-273.0f, 30.0f, -177.0f}, // bedside
    {-445.0f, 258.0f, 88.0f, -341.0f, 92.0f, -191.0f}, // cam
    -25.0f, // bedDeg
    {-320.0f, 30.0f, -115.0f}, // npcTalk
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

EVT_BEGIN(bom_02_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL(evt_snd_bgmoff, PSND_FADEOUT)
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_BOM1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_BOM3"))
    END_IF
    CALL_CHILD(PTR(evt_bom_snowfall))
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_00_START, STG7_05_THE_CANNON)
        CALL_CHILD(PTR(kiza_entry))
    END_SWITCH
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    SET_VALUE(LW(0), PTR(&ie1_door))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&ie2_door))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&ie3_door))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), STRING("group3713047r"))
    SPAWN_EVENT(PTR(evt_bom_01_karakara))
    SET_VALUE(LW(0), STRING("group3713048r"))
    SPAWN_EVENT(PTR(evt_bom_01_karakara))
    SET_VALUE(LW(0), STRING("group3713049r"))
    SPAWN_EVENT(PTR(evt_bom_01_karakara))
    CALL(evt_mobj_powerupblk, STRING("pblk01"), 425, 0, -45, 0, GSWF_SHINE_40, 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -30, 0, 115, STRING("S_wakuaaa"), 125, GSWF_STARPIECE_94)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 210, 30, -168, ITEM_TYPE_STILL, GSWF_STARPIECE_93, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_INN_COUPON, -220, 5, -115, ITEM_TYPE_STILL, GSWF(3892), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -300, 65, 10, 0, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
    CALL(evt_eff, 0, STRING("nice"), 0, 0, -1000, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, 0, STRING("nice"), 1, 0, -1000, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_map_playanim, STRING("yuge"), MAP_ANIM_PLAY_LOOP, 0)
    IF_INT_EQUAL(GSWF(3880), 1)
        IF_INT_EQUAL(GSWF(3876), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_w_bed"), 0, PTR(evt_white_bed), 0)
        END_IF
    END_IF
    CALL(evt_map_replace_mapobj, STRING("hi1"), 1)
    CALL(evt_map_replace_mapobj, STRING("hi2"), 1)
    CALL(evt_map_replace_mapobj, STRING("hi3"), 1)
    CALL(evt_map_replace_mapobj, STRING("hi4"), 1)
    CALL(evt_map_replace_mapobj, STRING("hi5"), 1)
    CALL(evt_map_replace_mapobj, STRING("hi6"), 1)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(4), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(6), 0)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(5), 0)
                    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_w_bed"), 0, PTR(iri_25_white_bed), 0)
                END_IF
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_ERASE_THAT_GRAFFITI), 1)
            CALL(evt_item_entry, STRING("iri_30"), ITEM_SNOW_BUNNY, 50, 0, 35, ITEM_TYPE_STILL, GSWF(3890), 0)
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie1_naka"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie2_naka"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie3_naka"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);

    animPoseSetMaterialFlagOn(mobjNameToPtr(name)->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

