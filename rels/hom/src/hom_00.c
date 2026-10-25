#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "driver/envdrv.h"

#include "memory.h"
#include "mario/mariost.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "party/party_motion.h"
#include "hom.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

static smartEntry* ind_lp;
void envGlareFilter(s32 on, s16 x0, s16 y0, s16 x1, s16 y1);

USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_cam3d_event_from_road);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_set_xyz);
USER_FUNC(evt_cam3d_get_shift);
USER_FUNC(evt_cam_ctrl_onoff);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_item_end_wait);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_reverse_ry);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_move_pos);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_dir_npc);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_win_item_select);
static USER_FUNC(test_glare);
USER_FUNC(tokei);
static void sun_tev(MapTevCallbackInfo* info);

static EvtWord n_2_door_open[22];
static EvtWord n_2_door_close[22];
static BeroEntry bero_data[3];
BeroEntry bero_data_nidome[2];
EvtWord evt_carry_up_bridge[92];
EvtWord init_powan_toron_konari_papa_gurume[23];
static EvtWord talk_powan[8];
static EvtWord talk_toron[8];
static EvtWord talk_gurume[8];
static EvtWord talk_papa[8];
static EvtWord talk_konari[8];
EvtWord init_ekiin1[2];
EvtWord talk_ekiin1[32];
EvtWord init_ekiin2[13];
EvtWord talk_ekiin2[28];
static NpcSetupInfo npc_data[9];
EvtWord evt_bridge_down[144];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
static EvtWord evt_lock[10];
EvtWord evt_train_door[13];
static EvtWord evt_train[262];
EvtWord evt_train_nidome[307];
EvtWord evt_hom_touchaku[460];
EvtWord evt_hom_touchaku_nidome[56];
#ifdef VERSION_G8ME01
EvtWord hom_00_init_evt[259];
#else
EvtWord hom_00_init_evt[247];
#endif

static EVT_BEGIN(n_2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("tobira_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "n_bero_1", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        45, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)evt_train_door, // exitEvent
        "rsh_01_b", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "n_1_door_close", // enterAnimeArg
        "n_1_door_open", // exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_00", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_2_door_close, // enterAnimeArg
        n_2_door_open, // exitAnimeArg
    },
    {0},
};

BeroEntry bero_data_nidome[] = {
    {
        "n_bero_1", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        45, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)evt_train_nidome, // exitEvent
        "rsh_01_b", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "n_1_door_close", // enterAnimeArg
        "n_1_door_open", // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_carry_up_bridge)
    IF_INT_EQUAL(GSWF(3574), 0)
        CALL(evt_msg_print, 0, STRING("stg6_hom_04"), 0, STRING("me"))
        CALL(evt_cam3d_evt_set, -65, 33, 575, 213, 64, 102, 3000, 11)
        WAIT_MS(6000)
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_MS(200)
    END_IF
    IF_INT_EQUAL(GSWF(3574), 0)
        SET_VALUE(GSWF(3574), 1)
        CALL(evt_msg_print, 0, STRING("stg6_hom_05"), 0, STRING("me"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_hom_10"), 0, STRING("me"))
    END_IF
    CALL(evt_msg_select, 0, STRING("stg6_hom_10_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg6_hom_06"))
        CALL(evt_item_entry, STRING("a"), 24, 0, 0, 0, 17, -1, 0)
        CALL(evt_item_get_item, STRING("a"))
        CALL(evt_item_get_item_end_wait, STRING("a"))
        CALL(evt_msg_print, 0, STRING("stg6_hom_08"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, 311)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg6_hom_09"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_powan_toron_konari_papa_gurume)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(325)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 310)
        CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_powan)
    CALL(evt_msg_print, 0, STRING("stg6_hom_00"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(talk_toron)
    CALL(evt_msg_print, 0, STRING("stg6_hom_01"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(talk_gurume)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_211"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(talk_papa)
    CALL(evt_msg_print, 0, STRING("stg6_hom_02"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(talk_konari)
    CALL(evt_msg_print, 0, STRING("stg6_hom_03"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(init_ekiin1)
    RETURN
EVT_END()

EVT_BEGIN(talk_ekiin1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(311)
        CALL_CHILD(PTR(evt_carry_up_bridge))
        CASE_LESS(325)
        CALL(evt_msg_print, 0, STRING("stg6_hom_11"), 0, 0)
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_hom_12"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_hom_13"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_ekiin2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(325)
        CALL(evt_npc_set_position, STRING("me"), -865, 80, 100)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_ekiin2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(325)
        CALL(evt_msg_print, 0, STRING("stg6_eki_03"), 0, 0)
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_eki_04"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_hom_16"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", // name
        0x40000600, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トロン"*/"\x83\x67\x83\x8D\x83\x93", // name
        0x40000600, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_toron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"グルメキノピオ"*/"\x83\x4F\x83\x8B\x83\x81\x83\x4C\x83\x6D\x83\x73\x83\x49", // name
        0x40000600, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_gurume, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70", // name
        0x40000600, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_papa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93", // name
        0x40000600, // flags
        0, // reactionFlags
        init_powan_toron_konari_papa_gurume, // initEvent
        NULL, // regularEvent
        talk_konari, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"駅員1"*/"\x89\x77\x88\xF5\x31", // name
        0x40000600, // flags
        0, // reactionFlags
        init_ekiin1, // initEvent
        NULL, // regularEvent
        talk_ekiin1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"駅員2"*/"\x89\x77\x88\xF5\x32", // name
        0x40000600, // flags
        0, // reactionFlags
        init_ekiin2, // initEvent
        NULL, // regularEvent
        talk_ekiin2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B", // name
        0x40000600, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_bridge_down)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set_xyz, -350, 15, 0, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set_xyz, 100, 15, 0, 0, 5000, 0)
    WAIT_MS(5300)
    CALL(evt_eff_fukidashi, 1, STRING("a"), STRING("駅員1"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("駅員1"), 90)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -65, 33, 575, 213, 64, 102, 2000, 11)
    WAIT_MS(2000)
    CALL(evt_map_playanim, STRING("hasi"), 0, 0)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG6_BRIDGE_MOVE1"), LW(10))
        CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon, STRING("SFX_STG6_BRIDGE_MOVE2"), 0)
    END_INLINE_EVENT
    CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
    MULTIPLY(LW(1), 2)
    DIVIDE(LW(1), 3)
    WAIT_MS(LW(1))
    CALL(evt_cam3d_evt_set, 275, 260, 1400, 275, 210, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_map_checkanim, STRING("hasi"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0009765625f), 200)
    WAIT_MS(2000)
    CALL(evt_bero_mapchange, STRING("eki_00"), STRING("s_bero"))
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_STEEPLE_KEY_0x18, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, 24, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_lock)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, 312)
    RETURN
EVT_END()

EVT_BEGIN(evt_train_door)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 325)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_train)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_4"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_5"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_6"), 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(10), LW(11), LW(12), LW(13), LW(14), LW(15))
    CALL(evt_cam3d_evt_set, LW(10), LW(11), LW(12), LW(13), LW(14), LW(15), 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -820, 30, 500)
        CALL(evt_mario_set_dir_npc, STRING("駅員2"))
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -810, 70, 300)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -810, 20, 500)
    CALL(evt_party_set_dir_npc, 0, STRING("駅員2"))
    CALL(evt_npc_status_onoff, 1, STRING("駅員2"), 2)
    CALL(evt_npc_move_position, STRING("駅員2"), -800, 80, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_close"), 0, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_close"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_eki_05"), 0, STRING("駅員2"))
    WAIT_MS(400)
    CALL(evt_cam3d_evt_set, 318, 80, 347, -134, 65, 35, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_CROSSING_OPEN1"), -170, 30, 0, LW(10))
    CALL(evt_map_playanim, STRING("shadanki"), 0, 0)
    CALL(evt_map_checkanim, STRING("shadanki"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_sfxoff, LW(10))
    SET_VALUE(GF(15), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("eki_03_S"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group614"), 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    SPAWN_EVENT(PTR(hom_10_evt_resha_start))
    WAIT_MS(4500)
    CALL(evt_cam3d_evt_set, 50, 125, 500, 50, 75, 0, 1500, 12)
    WAIT_MS(1500)
    WAIT_MS(4500)
    CALL(evt_npc_set_ry, STRING("駅員1"), 90)
    CALL(evt_cam3d_evt_set, -150, 25, 250, 273, 75, -10, 1500, 13)
    WAIT_MS(1500)
    WAIT_MS(4000)
    SET_VALUE(GSWF(3582), 1)
    SET_VALUE(GSWF(3577), 1)
    SET_VALUE(GSW_STORY_PROGRESS, 326)
    WAIT_FRAMES(1)
    CALL(evt_bero_mapchange, STRING("rsh_02_c"), STRING("hom_hassya"))
    RETURN
EVT_END()

EVT_BEGIN(evt_train_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_6"), 1)
        ELSE
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("n_bero_1"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_open"), 0, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_open"), LW(0), LW(1))
    WAIT_MS(LW(1))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -820, 12, 500)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -800, 40, 300)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 0, -800, 20, 500)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_party_stop, 1)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 1, -810, 40, 300)
    CALL(evt_party_stop, 1)
    WAIT_MS(200)
    CALL(evt_party_move_pos, 1, -810, 30, 500)
    CALL(evt_party_set_dir, 1, 270, 200)
    CALL(evt_npc_status_onoff, 1, STRING("駅員2"), 2)
    CALL(evt_npc_move_position, STRING("駅員2"), -800, 80, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("駅員2"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("n_1_door_close"), 0, 0)
    CALL(evt_map_checkanim, STRING("n_1_door_close"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_eki_05"), 0, STRING("駅員2"))
    WAIT_MS(400)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_CROSSING_OPEN1"), -170, 30, 0, LW(10))
    CALL(evt_map_playanim, STRING("shadanki"), 0, 0)
    CALL(evt_map_checkanim, STRING("shadanki"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_sfxoff, LW(10))
    SET_VALUE(GF(15), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("eki_03_S"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group614"), 1)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    IF_INT_EQUAL(GSWF(3578), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(6000)
            CALL(evt_npc_set_ry, STRING("駅員2"), 90)
        END_INLINE_EVENT
        SPAWN_EVENT(PTR(hom_10_evt_resha_start))
        ELSE
        BEGIN_INLINE_EVENT
            WAIT_MS(6000)
            CALL(evt_npc_set_ry, STRING("駅員2"), 270)
        END_INLINE_EVENT
        SPAWN_EVENT(PTR(hom_10_evt_resha_start2))
    END_IF
    WAIT_MS(10000)
    SET_VALUE(GSWF(3582), 1)
    SET_VALUE(GSWF(3577), 1)
    WAIT_FRAMES(1)
    SET_VALUE(GSWF(3579), 0)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(GSW(870), 1)
        ELSE
        SET_VALUE(GSW(870), 3)
    END_IF
    CALL(evt_bero_mapchange, STRING("rsh_02_c"), STRING("nidome_start"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_hom_touchaku)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 275, 260, 1400, 275, 210, 0, 0, 11)
    CALL(evt_npc_set_position, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(/*"グルメキノピオ"*/"\x83\x4F\x83\x8B\x83\x81\x83\x4C\x83\x6D\x83\x73\x83\x49"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(/*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(/*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), 90)
    CALL(evt_npc_set_ry, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 90)
    CALL(evt_npc_set_position, STRING(/*"駅員2"*/"\x89\x77\x88\xF5\x32"), -750, 80, 100)
    CALL(evt_seq_wait, 2)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_telop_entry, 11)
    END_INLINE_EVENT
    CALL_CHILD(hom_10_evt_resha_stop)
    WAIT_MS(2500)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_4"), 1)
    CALL(evt_cam3d_evt_set, 458, 97, 366, 78, 110, 189, 3000, 11)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_4"), 1)
    CALL(evt_cam3d_evt_set, -810, 232, 628, -810, 137, 87, 0, 11)
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 32768)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), 32768)
    CALL(evt_npc_set_position, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), -815, 80, 15)
    CALL(evt_npc_set_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), -800, 80, 20)
    CALL(evt_set_dir_to_target, STRING(/*"駅員2"*/"\x89\x77\x88\xF5\x32"), STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, -40, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("tobira"), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 32768)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), 32768)
    CALL(evt_npc_status_onoff, 1, STRING(/*"駅員2"*/"\x89\x77\x88\xF5\x32"), 2)
    CALL(evt_npc_move_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), -800, 100, 0, FLOAT(60.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), -780, 100, 0, FLOAT(60.0f), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), -815, 100, 0, FLOAT(60.0f), 1)
    CALL(evt_set_dir_to_target, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, -40, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("tobira"), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    LOOP(3)
        CALL(evt_npc_reverse_ry, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"))
        WAIT_MS(500)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg6_hom_01_01"), 0, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"))
    CALL(evt_npc_set_anim, STRING(/*"駅員2"*/"\x89\x77\x88\xF5\x32"), STRING("G_1"))
    CALL(evt_npc_set_anim, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), STRING("O_1"))
    CALL(evt_npc_get_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_LANPEL_CHANGE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_set_position, STRING(/*"トロン"*/"\x83\x67\x83\x8D\x83\x93"), 0, -1000, 0)
        CALL(evt_npc_set_position, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    ADD(LW(2), 20)
    LOOP(3)
        CALL(evt_eff, 0, STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -780, 194, 481, -780, 125, 87, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_hom_01_02"), 0, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -780, 259, 776, -780, 137, 87, 500, 11)
    CALL(evt_npc_sound_data_set, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, STRING("SFX_STG6_LANPEL_JUMP1"), EVT_NULLPTR)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), 32)
    CALL(evt_npc_status_onoff, 1, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), 2)
    CALL(evt_npc_set_ry, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), 90)
    LOOP(0)
        CALL(evt_npc_get_position, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), LW(0), LW(1), LW(2))
        ADD(LW(0), 60)
        CALL(evt_npc_jump_position_nohit, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), LW(0), LW(1), LW(2), 400, 40)
        IF_INT_GREATER(LW(0), -500)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_bgm_scope, 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 310)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("hom_touchaku"))
    RETURN
EVT_END()

EVT_BEGIN(evt_hom_touchaku_nidome)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 406, 317, 596, -389, 137, -44, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL_CHILD(PTR(hom_10_evt_resha_stop))
    WAIT_MS(2500)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    SET_VALUE(GSWF(3579), 1)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("hom_touchaku"))
    RETURN
EVT_END()

EVT_BEGIN(hom_00_init_evt)
    SET_VALUE(GF(15), 0)
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hasi_L"), 2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hasi_R"), 2)
#endif
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_HOM1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG6_EKI1"))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -175, 140, -170, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 15, -125, 140, -170, 0, 0)
    IF_INT_EQUAL(GSWF(3578), 0)
        CALL_CHILD(PTR(hom_10_evt_resha_right))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("train_6"), 1)
        ELSE
        CALL_CHILD(PTR(hom_10_evt_resha_left))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 339)
        CALL(evt_bero_get_entername, LW(0))
        CALL(evt_sub_get_system_flag, LW(1))
        IF_STR_NOT_EQUAL(LW(0), STRING("n_bero_1"))
            IF_STR_NOT_EQUAL(LW(0), STRING("n_bero_2"))
                IF_BITS_CLEAR(LW(1), 1)
                    CALL(evt_map_playanim, STRING("shadanki"), 2, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(evt_map_set_tevcallback, 2, PTR(sun_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sun"), 33554432)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sun"), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sun"), 268435456)
    BEGIN_INLINE_EVENT
        CALL(test_glare)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(tokei)
            CALL(evt_mapobj_rotate, 1, STRING("short"), 0, LW(0), 0)
            CALL(evt_mapobj_rotate, 0, STRING("long"), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    IF_INT_EQUAL(GSWF(3576), 1)
        SPAWN_EVENT(PTR(evt_bridge_down))
        RETURN
    END_IF
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, 310)
        SPAWN_EVENT(PTR(evt_hom_touchaku))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome"))
        SPAWN_EVENT(PTR(evt_hom_touchaku_nidome))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 340)
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("n_bero_1"), PTR(bero_data_nidome))
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 312)
        CALL(evt_mobj_lock, STRING("lock"), 24, -260, 90, -190, 0, PTR(evt_chk), PTR(evt_lock), GSWF(3575))
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 325)
        CALL(evt_map_playanim, STRING("hasi"), 2, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 325)
        CALL(evt_hitobj_attr_onoff, 0, 1, STRING("n_bero_1"), 8388608)
        CALL(evt_run_case_evt, 8, 1, STRING("n_bero_1"), 0, PTR(evt_train), 0)
        CALL(evt_map_playanim, STRING("n_1_door_open"), 2, 0)
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("train_2"), 2)
    RETURN
EVT_END()

static void sun_tev(MapTevCallbackInfo* info) {
    static const s32 texmtx_tbl[] = {
        GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4,
        GX_TEXMTX5, GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9,
    };
    s32 stage = info->tevStage;
    s32 texMap = info->texCoord;
    s32 texCoord = info->texMap;
    s32 indStage = info->indStage;
    GXTexObj obj;
    f32 mtx[2][3];
    u8* buf;
    s32 i;
    s32 j;

    // a scrolling sine-wave warp texture for the sun's heat haze
    if (ind_lp == NULL) {
        ind_lp = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), 1);
    }
    buf = ind_lp->address;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            f32 v = sin(31.41592f * j / 64 - gp->retraceCount / 8.0f);
            s32 off;

            off = i / 4;
            off = (off << 5) + ((j / 4) << 9);
            off += ((i % 4) + ((j % 4) << 2)) * 2;
            buf[off] = 3.0f * v + 128.0f;
            buf[off + 1] = 0x80;
        }
    }
    DCFlushRange(buf, 0x2000);
    GXInitTexObj(&obj, buf, 64, 64, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, texMap);
    mtx[0][0] = 0.5f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.5f;
    mtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, mtx, 1);
    GXSetIndTexOrder(indStage, GX_TEXCOORD0, texMap);
    GXSetIndTexCoordScale(indStage, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, indStage, GX_TRUE, GX_FALSE, GX_ITM_0);
    info->tevStage = stage;
    info->texMap = texCoord;
    info->indStage = indStage + 1;
    info->texCoord = texMap + 1;
}

static USER_FUNC(test_glare) {
    MapObject* sun = mapGetMapObj("sun");
    u16* rect;

    if (isFirstCall) {
        sun->flags |= 0x20000000;
        return EVT_RETURN_BLOCK;
    }
    rect = sun->screenRect;
    if (rect[0] == 0x3FF || rect[1] == 0x3FF || rect[2] == 0x3FF || rect[3] == 0x3FF) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else if (rect[0] == 0 && rect[1] == 0 && rect[2] == 0 && rect[3] == 0) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else if (evtGetValue(event, GF(15))) {
        envGlareFilter(0, 0, 0, 0, 0);
    } else {
        envGlareFilter(1, rect[1] - 50, rect[0] - 50, rect[3] + 50, rect[2] + 50);
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(tokei) {
    s32 min;
    OSCalendarTime time;
    u32 sec;

    OSTicksToCalendarTime(OSGetTime(), &time);
    min = time.min * 60;
    sec = time.hour * 3600 + min + time.sec;
    event->lwData[0] = FLOAT(-(360.0f * (sec / 3600.0f) / 12.0f));
    event->lwData[1] = FLOAT(-(360.0f * (sec / 60.0f) / 60.0f));
    return EVT_RETURN_DONE;
}
