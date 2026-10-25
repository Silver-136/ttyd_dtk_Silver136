#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "win/win_main.h"

#include "mario/mario.h"
#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_env.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "rsh.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_item_end_wait);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mario_wait_movable);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_win_item_select);
static USER_FUNC(mario_flag);
USER_FUNC(evt_party_trigflg_onoff);
#ifdef VERSION_G8ME01
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_get_dispdir);
#endif

static BeroEntry bero_data_a[3];
static BeroEntry bero_data_b[3];
static BeroEntry bero_data_c[3];
static const char* door_3[7];
static const char* door_4[7];
static const char* door_5[7];
#ifdef VERSION_G8ME01
EvtWord evt_rsh_konarikin_irai_party_talk[89];
#else
EvtWord evt_rsh_konarikin_irai_party_talk[85];
#endif
#ifdef VERSION_G8ME01
EvtWord evt_allow_kamotsu[77];
#else
EvtWord evt_allow_kamotsu[73];
#endif
EvtWord evt_mario_chuumoku[10];
EvtWord evt_party_chuumoku[11];
#ifdef VERSION_G8ME01
EvtWord evt_shorui_funshitu[362];
#else
EvtWord evt_shorui_funshitu[352];
#endif
static s32 item_tbl[2];
EvtWord evt_daijisouna_kami[208];
EvtWord evt_syasyou_auto_talk[27];
static EvtWord init_syashou[56];
static s32 item_tbl2[2];
#ifdef VERSION_G8ME01
static EvtWord talk_syashou[413];
#else
static EvtWord talk_syashou[367];
#endif
EvtWord talk_sarary_repeat[43];
static EvtWord init_sarary[55];
static EvtWord talk_sarary[72];
EvtWord talk_papa_mama[167];
EvtWord talk_papa_repeat[40];
static EvtWord init_papa[46];
static EvtWord talk_papa[49];
EvtWord talk_mama_repeat[40];
static EvtWord init_mama[37];
static EvtWord talk_mama[57];
static EvtWord init_konari[46];
s32 item_tbl3[2];
#ifdef VERSION_G8ME01
static EvtWord talk_konari[265];
#else
static EvtWord talk_konari[219];
#endif
EvtWord talk_powan_repeat[43];
static EvtWord init_powan[64];
static EvtWord talk_powan[204];
static NpcSetupInfo npc_data[7];
EvtWord evt_tana_5[50];
/* shelf access table read by READ_INT4: hit, msg, event, second msg; a NULL hit ends it */
typedef struct ShelfAccess { const char* hit; const char* msg; EvtWord* evt; const char* msg2; } ShelfAccess;
typedef struct ShelfAccessTbl3 { ShelfAccess rows[3]; const char* end[1]; } ShelfAccessTbl3;
static ShelfAccessTbl3 access_tbl;
static EvtWord evt_access[32];
static EvtWord evt_access_entry[25];
static EvtWord evt_door_msg[102];
EvtWord evt_narikin_room[87];
EvtWord evt_powan_room[83];
#ifdef VERSION_G8ME01
EvtWord rsh_04_init_evt[214];
#else
EvtWord rsh_04_init_evt[201];
#endif
EvtWord rsh_04_a_init_evt[96];
EvtWord rsh_04_b_init_evt[44];
static const char* window_tbl[4];
EvtWord rsh_04_c_init_evt[67];
static BeroEntry bero_data_a_return[3];
static BeroEntry bero_data_b_return[3];
static BeroEntry bero_data_c_return[3];
EvtWord rsh_04_init_evt_nidome[149];
EvtWord rsh_04_a_init_evt_nidome[28];
EvtWord rsh_04_b_init_evt_nidome[28];
EvtWord rsh_04_c_init_evt_nidome[38];

static BeroEntry bero_data_a[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_05_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_a", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_05_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_05_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

static const char* door_3[] = {
    "heya_3",
    "haikei_3",
    "tobira_3",
    "a_tobira_3",
    "3_door_open",
    "a_tobira_3_heya",
    "3_door_close",
};

static const char* door_4[] = {
    "heya_4",
    "haikei_4",
    "tobira_4",
    "a_tobira_4",
    "4_door_open",
    "a_tobira_4_heya",
    "4_door_close",
};

static const char* door_5[] = {
    "heya_5",
    "haikei_5",
    "tobira_5",
    "a_tobira_5",
    "5_door_open",
    "a_tobira_5_heya",
    "5_door_close",
};

EVT_BEGIN(evt_rsh_konarikin_irai_party_talk)
    IF_INT_EQUAL(GSWF(3436), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3437), 0)
        RETURN
    END_IF
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_107_6"), 0, STRING("party"))
    END_SWITCH
    CALL(evt_party_run, 0)
#ifdef VERSION_G8ME01
    IF_INT_LESS(GSW_STORY_PROGRESS, 292)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, 292)
#ifdef VERSION_G8ME01
    END_IF
#endif
    RETURN
EVT_END()

EVT_BEGIN(evt_allow_kamotsu)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("車掌"), 32)
#endif
    CALL(evt_mario_mov_pos, -360, -100, 500)
    CALL(evt_mario_set_dir_npc, STRING("車掌"))
#ifdef VERSION_G8ME01
    CALL(evt_set_dir_to_target, STRING("車掌"), STRING("mario"))
#endif
    CALL(evt_msg_print, 0, STRING("stg6_rsh_126"), 0, STRING("車掌"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(600)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_127"), 0, STRING("車掌"))
#ifndef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("車掌"), 32)
#endif
    CALL(evt_npc_move_position, STRING("車掌"), -395, -145, 1000, 0, 1)
    CALL(evt_npc_flag_onoff, 0, STRING("車掌"), 32)
    CALL(evt_set_dir_to_target, STRING("車掌"), STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 298)
    SET_VALUE(LW(0), 0)
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(evt_mario_chuumoku)
    LOOP(0)
        CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_party_chuumoku)
    LOOP(0)
        CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_shorui_funshitu)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
    CALL(evt_cam3d_evt_set, 220, 130, 250, 220, 55, -180, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_party_move_pos, 0, 255, -150, 500)
            CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 32)
    CALL(evt_mario_mov_pos, 260, -135, 500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), 32)
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), 2)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_167"), 0, STRING("me"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168_01"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168_02"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    SPAWN_EVENT_GET_ID(PTR(evt_mario_chuumoku), LW(10))
    SPAWN_EVENT_GET_ID(PTR(evt_party_chuumoku), LW(11))
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 32)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_00"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 220, -125, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 210, -115, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 180, -110, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_01"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 270, -100, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_02"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 290, -140, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 280, -170, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 250, -200, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_move_position, STRING("me"), 230, -180, 0, FLOAT(30.0f), 0)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 225, 71, 17, 225, 35, -180, 200, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 500)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_03"), 0, 0)
    CALL(evt_cam3d_evt_set, 225, 93, 115, 225, 40, -180, 500, 11)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_04"), 0, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), 32)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), -2147483648)
    CALL(evt_npc_flag_onoff, 0, STRING("サラリーマン"), -2147483648)
    SET_VALUE(GSW_STORY_PROGRESS, 304)
    RETURN
EVT_END()
#else
EVT_BEGIN(evt_shorui_funshitu)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
    CALL(evt_cam3d_evt_set, 220, 130, 250, 220, 55, -180, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_party_move_pos, 0, 255, -150, 500)
            CALL(evt_party_set_dir_npc, 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"))
        END_IF
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, 260, -135, 500)
    CALL(evt_mario_set_dir_npc, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"))
    CALL(evt_set_dir_to_target, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_status_onoff, 1, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 2)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_167"), 0, STRING("me"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168"), 0, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168_01"), 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_168_02"), 0, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"))
    WAIT_MS(500)
    SPAWN_EVENT_GET_ID(PTR(evt_mario_chuumoku), LW(10))
    SPAWN_EVENT_GET_ID(PTR(evt_party_chuumoku), LW(11))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 32)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_00"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 220, -125, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 210, -115, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 180, -110, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_01"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 270, -100, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 512)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_02"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), 512)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("me"), 290, -140, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 280, -170, 0, FLOAT(30.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 250, -200, 0, FLOAT(30.0f), 0)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_move_position, STRING("me"), 230, -180, 0, FLOAT(30.0f), 0)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 225, 71, 17, 225, 35, -180, 200, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 500)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_03"), 0, 0)
    CALL(evt_cam3d_evt_set, 225, 93, 115, 225, 40, -180, 500, 11)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_04"), 0, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), 32)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), -2147483648)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"), -2147483648)
    SET_VALUE(GSW_STORY_PROGRESS, 304)
    RETURN
EVT_END()
#endif

static s32 item_tbl[] = {
    ITEM_VITAL_PAPER, -1,
};

EVT_BEGIN(evt_daijisouna_kami)
    CALL(evt_cam3d_evt_set, 220, 107, 165, 220, 45, -180, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
#ifdef VERSION_G8ME01
            CALL(evt_party_move_pos, 0, 275, -160, 500)
#else
            CALL(evt_party_move_pos, 0, 245, -160, 500)
#endif
            CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 32)
    CALL(evt_mario_mov_pos, 255, -150, 500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), 32)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_172"), 0, STRING("me"))
    CALL(evt_win_item_select, 0, PTR(item_tbl), LW(1), 0)
    IF_INT_EQUAL(LW(1), -1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_173_2"), 0, STRING("me"))
        CALL(evt_cam3d_evt_off, 500, 11)
        RETURN
    END_IF
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    ADD(LW(1), 15)
    CALL(evt_item_entry, STRING("dummy"), 80, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_173"), 0, STRING("me"))
    CALL(evt_eff_fukidashi, 1, STRING("a"), STRING("サラリーマン"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1200)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("サラリーマン"))
        CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_174"), 0, STRING("サラリーマン"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_item_delete, STRING("dummy"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_175"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_176"), 0, STRING("me"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_177_00"), 0, STRING("me"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 306)
    RETURN
EVT_END()

EVT_BEGIN(evt_syasyou_auto_talk)
    IF_INT_EQUAL(GSWF(3443), 1)
        RETURN
    END_IF
    CALL(evt_mario_wait_movable)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_dir_npc, STRING("車掌"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_129"), 0, STRING("車掌"))
    SET_VALUE(GSWF(3443), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_syashou)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(298, 299)
        CALL(evt_npc_set_position, STRING("車掌"), -395, 0, -145)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("車掌"), 0, -1000, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(3578), 0)
            CALL(evt_npc_set_position, STRING("車掌"), -400, 0, -100)
            ELSE
            CALL(evt_npc_set_position, STRING("車掌"), 0, -1000, 0)
        END_IF
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static s32 item_tbl2[] = {
    ITEM_BLANKET, -1,
};

static EVT_BEGIN(talk_syashou)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(294, 300)
        CALL(evt_snd_bgmoff, 18432)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(283)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_06"), 0, 0)
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg6_rsh_06_01"), 0, 0)
        CALL(evt_npc_set_position, STRING("サラリーマン"), 0, -1000, 0)
        CALL(evt_npc_set_position, STRING("ポワン探偵"), 0, -1000, 0)
        SET_VALUE(GSW_STORY_PROGRESS, 283)
        CASE_LESS(288)
        IF_INT_EQUAL(LSWF(9), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_07"), 0, 0)
            SET_VALUE(LSWF(9), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_08"), 0, 0)
            SET_VALUE(LSWF(9), 0)
        END_IF
        CASE_LESS(291)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_84_01"), 0, 0)
        CASE_LESS(294)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_105"), 0, 0)
        SET_VALUE(GSWF(3436), 1)
        CALL_CHILD(PTR(evt_rsh_konarikin_irai_party_talk))
        IF_INT_EQUAL(GSWF(3452), 1)
            IF_INT_LESS(GSW_STORY_PROGRESS, 292)
                SET_VALUE(GSW_STORY_PROGRESS, 292)
            END_IF
        END_IF
        CASE_LESS(295)
        IF_INT_EQUAL(GSWF(3438), 0)
            SET_VALUE(GSWF(3438), 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_111"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_114"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_114_yn"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_112"))
            SET_VALUE(GSW_STORY_PROGRESS, 295)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_113"))
        END_IF
        CASE_LESS(297)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_115"), 0, 0)
        CASE_LESS(298)
        CALL_CHILD(PTR(evt_allow_kamotsu))
        CASE_LESS(299)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_129_01"), 0, 0)
        CASE_LESS(300)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_129"), 0, 0)
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_142"), 0, 0)
        CALL(evt_win_item_select, 0, PTR(item_tbl2), LW(1), 0)
        IF_INT_NOT_EQUAL(LW(1), -1)
#ifdef VERSION_G8ME01
            CALL(evt_mario_adjust_dir)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_mario_get_dispdir, LW(3))
            IF_INT_LESS(LW(3), 90)
                ADD(LW(0), -18)
                ELSE
                ADD(LW(0), 18)
            END_IF
            ADD(LW(1), 18)
            CALL(evt_item_entry, STRING("mofu"), 79, LW(0), LW(1), LW(2), 17, -1, 0)
            CALL(evt_mario_set_pose, STRING("M_C_3"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_item_delete, STRING("mofu"))
#endif
            CALL(evt_msg_print, 0, STRING("stg6_rsh_143"), 0, 0)
            CALL(evt_item_entry, STRING("orei"), 148, 0, 0, 0, 17, -1, 0)
            CALL(evt_item_get_item, STRING("orei"))
            CALL(evt_item_get_item_end_wait, STRING("orei"))
            CALL(evt_msg_print, 0, STRING("stg6_rsh_145"), 0, 0)
            SET_VALUE(GSW_STORY_PROGRESS, 301)
            CALL(evt_pouch_remove_item, 79, LW(0))
        END_IF
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_145"), 0, 0)
        CASE_LESS(310)
        IF_INT_EQUAL(LSWF(10), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_158"), 0, 0)
            SET_VALUE(LSWF(10), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_159"), 0, 0)
            SET_VALUE(LSWF(10), 0)
        END_IF
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_213"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_220"), 0, 0)
        CASE_LESS(346)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_254"), 0, 0)
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
            IF_INT_EQUAL(GSWF(3580), 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
                RETURN
            END_IF
            CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09"), 0, 0)
            ELSE
            IF_INT_EQUAL(GSWF(3578), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_13"), 0, 0)
                ELSE
                IF_INT_EQUAL(GSWF(3581), 0)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_14"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
                END_IF
            END_IF
        END_IF
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(294, 301)
        CALL(evt_snd_bgmon, 160, 0)
    END_SWITCH
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(talk_sarary_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), -2147483648)
    CALL(evt_npc_get_position, STRING("サラリーマン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("サラリーマン"), LW(3))
    ADD(LW(0), -5)
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_01"), 0, STRING("サラリーマン"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(talk_sarary_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"), -2147483648)
    CALL(evt_npc_get_position, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"), LW(3))
    ADD(LW(0), -5)
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_01"), 0, STRING(/*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#endif

static EVT_BEGIN(init_sarary)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(283, 286)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(303, 309)
        CALL(evt_npc_set_position, STRING("me"), 190, 0, -150)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_sarary)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(287)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_18"), 0, 0)
        CASE_LESS(294)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_79"), 0, 0)
        CASE_LESS(301)
        IF_INT_EQUAL(GSWF(3440), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_79_01"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_125"), 0, 0)
        END_IF
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_08"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_166"), 0, 0)
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_215"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_229"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_papa_mama)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_cam3d_evt_set, -220, 105, 186, -220, 40, -183, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_86"), 0, STRING("ナリキンママ"))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("ナリキンパパ"), STRING("ナリキンママ"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_87"), 0, STRING("ナリキンパパ"))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("ナリキンママ"), STRING("ナリキンパパ"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_88"), 0, STRING("ナリキンママ"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_88_01"), 0, STRING("コナリキン"))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンパパ"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンママ"), 32)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -200, -120, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
        CALL(evt_party_stop, 0)
        WAIT_MS(200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -225, -135, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_91"), 0, STRING("コナリキン"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_94_yn"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg6_rsh_92"))
        SET_VALUE(GSW_STORY_PROGRESS, 291)
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg6_rsh_93"))
        SET_VALUE(GSW_STORY_PROGRESS, 289)
        SET_VALUE(GSW_STORY_PROGRESS, 290)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("ナリキンパパ"), 32)
    CALL(evt_npc_flag_onoff, 0, STRING("ナリキンママ"), 32)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(talk_papa_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンパパ"), -2147483648)
    CALL(evt_npc_get_position, STRING("ナリキンパパ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ナリキンパパ"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 15)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_01"), 0, STRING("ナリキンパパ"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(talk_papa_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70"), -2147483648)
    CALL(evt_npc_get_position, STRING(/*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING(/*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 15)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_01"), 0, STRING(/*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#endif

static EVT_BEGIN(init_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(288)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_15"), 0, 0)
        CASE_LESS(289)
        CALL_CHILD(PTR(talk_papa_mama))
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_85"), 0, 0)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_09"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_163"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_226"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(talk_mama_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンママ"), -2147483648)
    CALL(evt_npc_get_position, STRING("ナリキンママ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ナリキンママ"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_02"), 0, STRING("ナリキンママ"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(talk_mama_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D"), -2147483648)
    CALL(evt_npc_get_position, STRING(/*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING(/*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_02"), 0, STRING(/*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#endif

static EVT_BEGIN(init_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(288)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_16"), 0, 0)
        CASE_LESS(289)
        CALL_CHILD(PTR(talk_papa_mama))
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_89"), 0, 0)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_10"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_164"), 0, 0)
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_214"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_227"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

s32 item_tbl3[] = {
    ITEM_AUTOGRAPH, -1,
};

static EVT_BEGIN(talk_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(288)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_17"), 0, 0)
        CASE_LESS(289)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_90"), 0, 0)
        CASE_LESS(291)
        CALL(evt_snd_bgmoff, 18432)
        IF_INT_LESS(GSW_STORY_PROGRESS, 290)
            SET_VALUE(GSW_STORY_PROGRESS, 290)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_91"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_94"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_94_yn"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_92"))
            SET_VALUE(GSW_STORY_PROGRESS, 291)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_93"))
        END_IF
        CALL(evt_snd_bgmon, 160, 0)
        CASE_LESS(294)
        CALL(evt_pouch_check_item, 77, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            IF_INT_EQUAL(GSWF(3425), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_95"), 0, 0)
                SET_VALUE(GSWF(3425), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_95_01"), 0, 0)
                SET_VALUE(GSWF(3452), 1)
                IF_INT_EQUAL(GSWF(3436), 1)
                    IF_INT_LESS(GSW_STORY_PROGRESS, 292)
                        SET_VALUE(GSW_STORY_PROGRESS, 292)
                    END_IF
                END_IF
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_96"), 0, 0)
            CALL(evt_win_item_select, 0, PTR(item_tbl3), LW(1), 0)
            IF_INT_EQUAL(LW(1), -1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_101"), 0, 0)
                ELSE
                IF_INT_NOT_EQUAL(LW(1), 77)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_100"), 0, 0)
                    ELSE
#ifdef VERSION_G8ME01
                    CALL(evt_mario_adjust_dir)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_mario_get_dispdir, LW(3))
                    IF_INT_LESS(LW(3), 90)
                        ADD(LW(0), -18)
                        ELSE
                        ADD(LW(0), 18)
                    END_IF
                    ADD(LW(1), 18)
                    CALL(evt_item_entry, STRING("sign"), 77, LW(0), LW(1), LW(2), 17, -1, 0)
                    CALL(evt_mario_set_pose, STRING("M_C_3"))
                    WAIT_MS(1000)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_item_delete, STRING("sign"))
#endif
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_97"), 0, 0)
                    CALL(evt_item_entry, STRING("orei"), 87, 0, 0, 0, 17, -1, 0)
                    CALL(evt_item_get_item, STRING("orei"))
                    CALL(evt_item_get_item_end_wait, STRING("orei"))
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_99"), 0, 0)
                    CALL(evt_pouch_remove_item, 77, LW(0))
                    SET_VALUE(GSW_STORY_PROGRESS, 294)
                    SET_VALUE(GSWF(5560), 1)
                END_IF
            END_IF
        END_IF
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_102"), 0, 0)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_11"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_165"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_228"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(talk_powan_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), -2147483648)
    CALL(evt_npc_get_position, STRING("ポワン探偵"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ポワン探偵"), LW(3))
    ADD(LW(0), 15)
    ADD(LW(1), LW(3))
    SUBTRACT(LW(1), 4)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_02"), 0, STRING("ポワン探偵"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#else
EVT_BEGIN(talk_powan_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), -2147483648)
    CALL(evt_npc_get_position, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"), LW(3))
    ADD(LW(0), 15)
    ADD(LW(1), LW(3))
    SUBTRACT(LW(1), 4)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_02"), 0, STRING(/*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()
#endif

static EVT_BEGIN(init_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(283, 286)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(303, 309)
        CALL(evt_npc_set_position, STRING("me"), 230, 0, -180)
        CASE_BETWEEN(310, 325)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(328, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(287)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_14"), 0, 0)
        CASE_LESS(288)
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        END_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_65"), 0, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_I_G"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg6_rsh_66"), 0, 0)
        CALL(evt_cam3d_evt_set, 265, 93, 137, 265, 36, -183, 500, 11)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_67"), 0, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        WAIT_MS(1500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_C"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg6_rsh_68"), 0, 0)
        SET_VALUE(GSW_STORY_PROGRESS, 288)
        CALL(evt_cam3d_evt_off, 500, 11)
        CASE_LESS(291)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_69"), 0, 0)
        CASE_LESS(294)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_103"), 0, 0)
        SET_VALUE(GSWF(3426), 1)
        CASE_LESS(295)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_104"), 0, 0)
        CASE_LESS(297)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_116"), 0, 0)
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_124"), 0, 0)
        SET_VALUE(GSWF(3440), 1)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147"), 0, 0)
        CASE_LESS(304)
        CALL_CHILD(PTR(evt_shorui_funshitu))
        CASE_LESS(305)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_170"), 0, 0)
        CASE_LESS(306)
        CALL_CHILD(PTR(evt_daijisouna_kami))
        CASE_LESS(309)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_177_01"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_225"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"車掌"*/"\x8E\xD4\x8F\xB6", // name
        0x40020610, // flags
        0, // reactionFlags
        init_syashou, // initEvent
        NULL, // regularEvent
        talk_syashou, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93", // name
        0x40020610, // flags
        0, // reactionFlags
        init_sarary, // initEvent
        NULL, // regularEvent
        talk_sarary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70", // name
        0x40020610, // flags
        0, // reactionFlags
        init_papa, // initEvent
        NULL, // regularEvent
        talk_papa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D", // name
        0x40020610, // flags
        0, // reactionFlags
        init_mama, // initEvent
        NULL, // regularEvent
        talk_mama, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93", // name
        0x40020610, // flags
        0, // reactionFlags
        init_konari, // initEvent
        NULL, // regularEvent
        talk_konari, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", // name
        0x40020610, // flags
        0, // reactionFlags
        init_powan, // initEvent
        NULL, // regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_tana_5)
    IF_INT_EQUAL(GSWF(5652), 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, 345)
            CALL(evt_msg_print, 0, LW(1), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, LW(3), 0, 0)
        END_IF
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_entry, STRING("_item00"), 125, -260, 0, -240, 16, -1, 0)
    CALL(evt_item_flag_on, STRING("_item00"), 2)
    CALL(evt_item_get_item, STRING("_item00"))
    SET_VALUE(GSWF(5652), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static ShelfAccessTbl3 access_tbl = {
    {
        {"tana_3", "stg6_rsh_272", NULL, "stg6_rsh_267"},
        {"tana_4", "stg6_rsh_273", NULL, "stg6_rsh_267"},
        {"tana_5", "stg6_rsh_274", evt_tana_5, "stg6_rsh_267"},
    },
    {NULL},
};

static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, 345)
            CALL(evt_msg_print, 0, LW(1), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, LW(3), 0, 0)
        END_IF
        ELSE
        CALL_CHILD(LW(2))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
        READ_INT4(LW(0), LW(1), LW(2), LW(3))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 9, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_msg)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        LOOP(3)
            CALL(evt_sub_intpl_msec_init, 11, 0, -50, 200)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_trans, 1, STRING("w_tobira"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_sub_intpl_msec_init, 11, -50, 0, 200)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_trans, 1, STRING("w_tobira"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    IF_INT_EQUAL(GSWF(3433), 0)
        CALL(evt_cam3d_evt_set, 150, 135, 261, -200, 67, -13, 800, 11)
        WAIT_MS(800)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg6_rsh_236_02"), 0, 0)
    IF_INT_EQUAL(GSWF(3433), 0)
        CALL(evt_cam3d_evt_off, 500, 11)
        SET_VALUE(GSWF(3433), 1)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_narikin_room)
    SET_TYPE(0)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        IF_INT_EQUAL(GSWF(3446), 1)
            IF_INT_EQUAL(LF(0), 0)
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    SET_VALUE(LF(0), 1)
                    SWITCH(GSW_STORY_PROGRESS)
                        CASE_BETWEEN(288, 300)
                        SPAWN_EVENT_GET_ID(PTR(talk_papa_repeat), GW(14))
                        SPAWN_EVENT_GET_ID(PTR(talk_mama_repeat), GW(15))
                    END_SWITCH
                END_IF
                ELSE
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    SET_VALUE(LF(0), 0)
                    DELETE_EVENT(GW(14))
                    DELETE_EVENT(GW(15))
                    CALL(evt_msg_close, STRING("stg6_rsh_85_01"))
                    CALL(evt_msg_close, STRING("stg6_rsh_85_02"))
                END_IF
            END_IF
            ELSE
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(LF(0), 0)
                SWITCH(GSW_STORY_PROGRESS)
                    CASE_BETWEEN(288, 300)
                    DELETE_EVENT(GW(14))
                    DELETE_EVENT(GW(15))
                    CALL(evt_msg_close, STRING("stg6_rsh_85_01"))
                    CALL(evt_msg_close, STRING("stg6_rsh_85_02"))
                END_SWITCH
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_powan_room)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        IF_INT_EQUAL(GSWF(3447), 1)
            IF_INT_EQUAL(LF(0), 0)
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    SET_VALUE(LF(0), 1)
                    SWITCH(GSW_STORY_PROGRESS)
                        CASE_EQUAL(303)
                        SPAWN_EVENT_GET_ID(PTR(talk_sarary_repeat), GW(14))
                        SPAWN_EVENT_GET_ID(PTR(talk_powan_repeat), GW(15))
                    END_SWITCH
                END_IF
                ELSE
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 1)
                    SET_VALUE(LF(0), 0)
                    DELETE_EVENT(GW(14))
                    DELETE_EVENT(GW(15))
                    CALL(evt_msg_close, STRING("stg6_rsh_165_01"))
                    CALL(evt_msg_close, STRING("stg6_rsh_165_02"))
                END_IF
            END_IF
            ELSE
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(LF(0), 0)
                SWITCH(GSW_STORY_PROGRESS)
                    CASE_EQUAL(303)
                    DELETE_EVENT(GW(14))
                    DELETE_EVENT(GW(15))
                    CALL(evt_msg_close, STRING("stg6_rsh_165_01"))
                    CALL(evt_msg_close, STRING("stg6_rsh_165_02"))
                END_SWITCH
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CASE_GREATER_OR_EQUAL(333)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
        CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
    END_SWITCH
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_5))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_LESS(GSW_STORY_PROGRESS, 298)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 300)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 328)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_on))
#ifdef VERSION_G8ME01
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 334)
        IF_INT_LESS(GSW_STORY_PROGRESS, 340)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(bero_case_switch_off))
        END_IF
#endif
    END_IF
    CALL_CHILD(PTR(evt_access_entry))
    SPAWN_EVENT(PTR(evt_narikin_room))
    SPAWN_EVENT(PTR(evt_powan_room))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(284, 286)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(304, 309)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(310, 325)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_BETWEEN(327, 328)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(329, 331)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(334, 344)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_04_b"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_04_c"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_a_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_a))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt))
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 328)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("w_bero"), 0, PTR(evt_door_msg), 0)
        CALL(evt_npc_entry, STRING("moa1"), STRING("c_n_moa_a"))
        CALL(evt_npc_entry, STRING("moa2"), STRING("c_n_moa_a"))
        CALL(evt_npc_entry, STRING("moa3"), STRING("c_n_moa_a"))
        CALL(evt_npc_set_position, STRING("moa1"), -440, 10, -100)
        CALL(evt_npc_set_position, STRING("moa2"), -441, 30, -125)
        CALL(evt_npc_set_position, STRING("moa3"), -442, 40, -75)
        CALL(evt_npc_pera_onoff, STRING("moa1"), 0)
        CALL(evt_npc_pera_onoff, STRING("moa2"), 0)
        CALL(evt_npc_pera_onoff, STRING("moa3"), 0)
        CALL(evt_npc_set_ry, STRING("moa1"), 270)
        CALL(evt_npc_set_ry, STRING("moa2"), 270)
        CALL(evt_npc_set_ry, STRING("moa3"), 270)
        CALL(evt_npc_set_anim, STRING("moa1"), STRING("S_1"))
        CALL(evt_npc_set_anim, STRING("moa2"), STRING("S_1"))
        CALL(evt_npc_set_anim, STRING("moa3"), STRING("S_1"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_b_init_evt)
    IF_INT_EQUAL(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        SET_VALUE(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, 220, 0, -20)
        CALL(evt_party_set_pos, 0, 200, 0, -20)
        DEBUG_PUT_MSG(STRING("パーティドア移動フラグクリア"))
        CALL(evt_party_trigflg_onoff, 0, 3538944, 0)
    END_IF
    CALL_CHILD(PTR(rsh_04_init_evt))
    RETURN
EVT_END()

static const char* window_tbl[] = {
    "mado_uturi_1",
    "mado_uturi_2",
    "mado_uturi_3",
    NULL,
};

EVT_BEGIN(rsh_04_c_init_evt)
    IF_INT_EQUAL(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        SET_VALUE(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -200, 0, -30)
        CALL(evt_party_set_pos, 0, -230, 0, -30)
        DEBUG_PUT_MSG(STRING("パーティドア移動フラグクリア"))
        CALL(evt_party_trigflg_onoff, 0, 3538944, 0)
    END_IF
    CALL_CHILD(PTR(rsh_04_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 299)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("w_bero"))
            SPAWN_EVENT(PTR(evt_syasyou_auto_talk))
        END_IF
    END_IF
    RETURN
EVT_END()

static BeroEntry bero_data_a_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_07_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_a", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_b_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_07_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_data_c_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_07_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "w_door_close", // enterAnimeArg
        "w_door_open", // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_03_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(rsh_04_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3579), 0)
        IF_INT_EQUAL(GSWF(3580), 0)
            IF_INT_EQUAL(GSWF(3581), 0)
                CALL(evt_map_playanim, STRING("yure"), 1, 0)
                IF_INT_EQUAL(GSWF(3578), 0)
                    CALL(evt_map_playanim, STRING("hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("hai_saboten"), 1, 0)
                    ELSE
                    CALL(evt_map_playanim, STRING("rt_hai_sora"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_yama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_iwayama"), 1, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_saboten"), 1, 0)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_5))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL_CHILD(PTR(evt_access_entry))
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_EQUAL(GSWF(3460), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        ELSE
        IF_INT_EQUAL(GSWF(3461), 1)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envoff, 512)
            CALL(evt_snd_bgmoff, 26624)
            ELSE
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_04_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_04_c"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_04_rnd_npc_entry))
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_a_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_a_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    SET_VALUE(GSWF(3453), 1)
    SET_VALUE(GSWF(3454), 0)
    SET_VALUE(GSWF(3455), 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_b_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_b_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    SET_VALUE(GSWF(3453), 0)
    SET_VALUE(GSWF(3454), 1)
    SET_VALUE(GSWF(3455), 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_c_init_evt_nidome)
    IF_INT_EQUAL(GSWF(3578), 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_c_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    SET_VALUE(GSWF(3453), 0)
    SET_VALUE(GSWF(3454), 0)
    SET_VALUE(GSWF(3455), 1)
    RETURN
EVT_END()


static USER_FUNC(mario_flag) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    if (fbatGetPointer()->mode == 2 || winGhostDiaryChk()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
