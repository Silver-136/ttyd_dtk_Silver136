// rsh_04 (列車, Excess Express): passenger car with compartments 3-5: Narikin family, conductor, salaryman and Powan
// detective NPCs, and a lost-documents event; doors to rsh_03, rsh_05 and rsh_07.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
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
#include "evt/evt_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "party.h"

#include "mario/mario.h"
#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_env.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "rsh.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

static USER_FUNC(mario_flag);

static BeroEntry bero_data_a[3];
static BeroEntry bero_data_b[3];
static BeroEntry bero_data_c[3];
static const char* door_3[7];
static const char* door_4[7];
static const char* door_5[7];
#if VERSION >= VERSION_US
EvtWord evt_rsh_konarikin_irai_party_talk[89];
#else
EvtWord evt_rsh_konarikin_irai_party_talk[85];
#endif
#if VERSION >= VERSION_US
EvtWord evt_allow_kamotsu[77];
#else
EvtWord evt_allow_kamotsu[73];
#endif
EvtWord evt_mario_chuumoku[10];
EvtWord evt_party_chuumoku[11];
#if VERSION >= VERSION_US
EvtWord evt_shorui_funshitu[362];
#else
EvtWord evt_shorui_funshitu[352];
#endif
static s32 item_tbl[2];
#if VERSION == VERSION_DEMO_JP
EvtWord evt_daijisouna_kami[206];
#else
EvtWord evt_daijisouna_kami[208];
#endif
EvtWord evt_syasyou_auto_talk[27];
static EvtWord init_syashou[56];
static s32 item_tbl2[2];
#if VERSION >= VERSION_US
static EvtWord talk_syashou[413];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord talk_syashou[354];
#else
static EvtWord talk_syashou[367];
#endif
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
#if VERSION >= VERSION_US
static EvtWord talk_konari[265];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord talk_konari[217];
#else
static EvtWord talk_konari[219];
#endif
#endif
EvtWord talk_powan_repeat[43];
static EvtWord init_powan[64];
static EvtWord talk_powan[204];
static NpcSetupInfo npc_data[7];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_tana_5[];
#else
EvtWord evt_tana_5[50];
#endif
/* shelf access table read by READ_INT4: hit, msg, event, second msg; a NULL hit ends it */
typedef struct ShelfAccess {
    const char* hit;
    const char* msg;
    EvtWord* evt;
#if VERSION > VERSION_DEMO_US
    const char* msg2;
#endif
} ShelfAccess;
typedef struct ShelfAccessTbl3 {
    ShelfAccess rows[3];
    const char* end[1];
} ShelfAccessTbl3;
static ShelfAccessTbl3 access_tbl;
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_access[21];
static EvtWord evt_access_entry[24];
#else
static EvtWord evt_access[32];
static EvtWord evt_access_entry[25];
#endif
static EvtWord evt_door_msg[102];
EvtWord evt_narikin_room[87];
EvtWord evt_powan_room[83];
#if VERSION >= VERSION_US
EvtWord rsh_04_init_evt[214];
#else
EvtWord rsh_04_init_evt[201];
#endif
EvtWord rsh_04_a_init_evt[96];
#if VERSION <= VERSION_DEMO_US
extern EvtWord rsh_04_b_init_evt[];
#else
EvtWord rsh_04_b_init_evt[44];
#endif
static const char* window_tbl[4];
#if VERSION <= VERSION_DEMO_US
extern EvtWord rsh_04_c_init_evt[];
#else
EvtWord rsh_04_c_init_evt[67];
#endif
static BeroEntry bero_data_a_return[3];
static BeroEntry bero_data_b_return[3];
static BeroEntry bero_data_c_return[3];
#if VERSION <= VERSION_DEMO_US
extern EvtWord rsh_04_init_evt_nidome[];
#else
EvtWord rsh_04_init_evt_nidome[149];
#endif
EvtWord rsh_04_a_init_evt_nidome[28];
EvtWord rsh_04_b_init_evt_nidome[28];
EvtWord rsh_04_c_init_evt_nidome[38];

/** Doors and exits of rsh_04 variant a: w_bero to rsh_05_a; e_bero to rsh_03_a. */
static BeroEntry bero_data_a[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Doors and exits of rsh_04 variant b: w_bero to rsh_05_a; e_bero to rsh_03_b. */
static BeroEntry bero_data_b[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Doors and exits of rsh_04 variant c: w_bero to rsh_05_a; e_bero to rsh_03_c. */
static BeroEntry bero_data_c[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Map objects of cabin 3 for evt_rsh_door_entry: the room, backdrop, door, door hit areas and open and close
 * animations.
 */
static const char* door_3[] = {
    "heya_3",
    "haikei_3",
    "tobira_3",
    "a_tobira_3",
    "3_door_open",
    "a_tobira_3_heya",
    "3_door_close",
};

/**
 * Map objects of cabin 4 for evt_rsh_door_entry: the room, backdrop, door, door hit areas and open and close
 * animations.
 */
static const char* door_4[] = {
    "heya_4",
    "haikei_4",
    "tobira_4",
    "a_tobira_4",
    "4_door_open",
    "a_tobira_4_heya",
    "4_door_close",
};

/**
 * Map objects of cabin 5 for evt_rsh_door_entry: the room, backdrop, door, door hit areas and open and close
 * animations.
 */
static const char* door_5[] = {
    "heya_5",
    "haikei_5",
    "tobira_5",
    "a_tobira_5",
    "5_door_open",
    "a_tobira_5_heya",
    "5_door_close",
};

// Both hints are required. US advances the story only if needed; JP writes this story step unconditionally.
EVT_BEGIN(evt_rsh_konarikin_irai_party_talk)
    IF_INT_EQUAL(GSWF_RSH_CONDUCTOR_BUB_HINT_HEARD, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_RSH_ZIP_TOAD_BUB_HINT_HEARD, 0)
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
#if VERSION >= VERSION_US
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
#endif
    SET_VALUE(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
#if VERSION >= VERSION_US
    END_IF
#endif
    RETURN
EVT_END()

/**
 * Scene where the conductor lets Mario into the baggage car (kamotsu):
 * Mario walks up, raises a hand, the conductor speaks (stg6_rsh_126, 127) and walks away,
 * the story advances to STG6_16_THE_BAGGAGE_CAR and the baggage car door (bero case) is switched on.
 */
EVT_BEGIN(evt_allow_kamotsu)
#if VERSION >= VERSION_US
    CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_mario_mov_pos, -360, -100, 500)
    CALL(evt_mario_set_dir_npc, STRING("車掌"))
#if VERSION >= VERSION_US
    CALL(evt_set_dir_to_target, STRING("車掌"), STRING("mario"))
#endif
    CALL(evt_msg_print, 0, STRING("stg6_rsh_126"), 0, STRING("車掌"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(600)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_127"), 0, STRING("車掌"))
#if VERSION < VERSION_US
    CALL(evt_npc_flag_onoff, 1, STRING("車掌"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_npc_move_position, STRING("車掌"), -395, -145, 1000, 0, 1)
    CALL(evt_npc_flag_onoff, 0, STRING("車掌"), NPC_FLAG_NO_HIT)
    CALL(evt_set_dir_to_target, STRING("車掌"), STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_16_THE_BAGGAGE_CAR)
    SET_VALUE(LW(0), 0)
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

/**
 * Helper loop for the lost documents scene: keeps turning Mario to face Detective Pennington every frame
 * (chuumoku means "pay attention").
 */
EVT_BEGIN(evt_mario_chuumoku)
    LOOP(0)
        CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Helper loop for the lost documents scene: keeps turning the partner to face Detective Pennington every frame. */
EVT_BEGIN(evt_party_chuumoku)
    LOOP(0)
        CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Cutscene of the lost documents (shorui funshitu) in cabin 3 at STG6_21_DAY_2:
 * Pennington questions the salaryman while the camera frames them,
 * the salaryman paces and mumbles (stg6_rsh_169_xx),
 * the screen blurs as the mystery deepens and the story advances to STG6_22_DOCUMENT.
 */
// Each timed speech signals GF(0); the main movement waits for it before starting the next speech.
/**
 * Cutscene of the lost documents (shorui funshitu) in cabin 3 at STG6_21_DAY_2:
 * Pennington questions the salaryman while the camera frames them,
 * the salaryman paces and mumbles (stg6_rsh_169_xx),
 * the screen blurs as the mystery deepens and the story advances to STG6_22_DOCUMENT.
 */
EVT_BEGIN(evt_shorui_funshitu)
    // The lost documents scene: Pennington's theme plays and he questions the salaryman.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
    CALL(evt_cam3d_evt_set, 220, 130, 250, 220, 55, -180, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_party_move_pos, 0, 255, -150, 500)
            CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        END_IF
    END_INLINE_EVENT
#if VERSION >= VERSION_US
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_mario_mov_pos, 260, -135, 500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_167"), 0, 0)
#else
#if VERSION >= VERSION_US
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_167"), 0, STRING("me"))
#endif
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
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_00"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
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
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_01"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
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
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_169_02"), 0, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
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
    CALL(evt_cam3d_evt_set, 225, 71, 17, 225, 35, -180, 200, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 500)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_03"), 0, 0)
    CALL(evt_cam3d_evt_set, 225, 93, 115, 225, 40, -180, 500, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_169_04"), 0, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    DELETE_EVENT(LW(10))
    DELETE_EVENT(LW(11))
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_flag_onoff, 0, STRING("サラリーマン"), NPC_FLAG_NO_TALK_BALLOON)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_22_DOCUMENT)
    RETURN
EVT_END()

/** Items the player may hand over: the Vital Paper only, ended by -1. */
static s32 item_tbl[] = {
    ITEM_VITAL_PAPER, -1,
};

/**
 * Cutscene when Mario brings the important-looking paper (Vital Paper) to the cabin:
 * the camera frames the NPCs, Mario chooses the paper from the item menu,
 * the salaryman recognises it and says thanks (stg6_rsh_172 to 177),
 * and the story advances to STG6_24_A_VITAL_CLUE.
 */
// Cancelling leaves the clue unresolved; accepting displays a temporary paper without consuming the pouch item.
/**
 * Cutscene when Mario brings the important-looking paper (Vital Paper) to the cabin:
 * the camera frames the NPCs, Mario chooses the paper from the item menu,
 * the salaryman recognises it and says thanks (stg6_rsh_172 to 177),
 * and the story advances to STG6_24_A_VITAL_CLUE.
 */
EVT_BEGIN(evt_daijisouna_kami)
    CALL(evt_cam3d_evt_set, 220, 107, 165, 220, 45, -180, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
#if VERSION >= VERSION_US
            CALL(evt_party_move_pos, 0, 275, -160, 500)
#else
            CALL(evt_party_move_pos, 0, 245, -160, 500)
#endif
            CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
    CALL(evt_mario_mov_pos, 255, -150, 500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_set_dir_to_target, STRING("サラリーマン"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_172"), 0, STRING("me"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_win_item_select, PTR(item_tbl), LW(1))
#else
    CALL(evt_win_item_select, 0, PTR(item_tbl), LW(1), 0)
#endif
    IF_INT_EQUAL(LW(1), -1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_173_2"), 0, STRING("me"))
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        RETURN
    END_IF
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    ADD(LW(1), 15)
    CALL(evt_item_entry, STRING("dummy"), ITEM_VITAL_PAPER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_173"), 0, STRING("me"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("a"), STRING("サラリーマン"), 0, 0, 0, 0, 0, 0, 0, 0)
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
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_24_A_VITAL_CLUE)
    RETURN
EVT_END()

EVT_BEGIN(evt_syasyou_auto_talk)
    IF_INT_EQUAL(GSWF_RSH_CONDUCTOR_DIARY_TALK_DONE, 1)
        RETURN
    END_IF
    CALL(evt_mario_wait_movable)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_dir_npc, STRING("車掌"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_129"), 0, STRING("車掌"))
    SET_VALUE(GSWF_RSH_CONDUCTOR_DIARY_TALK_DONE, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_syashou)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_16_THE_BAGGAGE_CAR, STG6_17_GHOST_DIARY)
        CALL(evt_npc_set_position, STRING("車掌"), -395, 0, -145)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("車掌"), 0, -1000, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
            CALL(evt_npc_set_position, STRING("車掌"), -400, 0, -100)
        ELSE
            CALL(evt_npc_set_position, STRING("車掌"), 0, -1000, 0)
        END_IF
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Items the player may hand to the conductor: the Blanket only, ended by -1. */
static s32 item_tbl2[] = {
    ITEM_BLANKET, -1,
};

/**
 * Talk script of the conductor (syashou) in rsh_04.
 * By story stage: his first greeting (sets STG6_01_THE_CONDUCTOR), ticket and autograph hints,
 * the night talk where Mario can agree to help (STG6_13_TALK_TO_THE_CONDUCTOR),
 * the Blanket exchange (takes the Blanket, gives a Mushroom, STG6_19_GIVE_BLANKET), and day 2,
 * day 3 and return-trip lines.
 */
/**
 * Talk script of the conductor (syashou) in rsh_04.
 * By story stage: his first greeting (sets STG6_01_THE_CONDUCTOR), ticket and autograph hints,
 * the night talk where Mario can agree to help (STG6_13_TALK_TO_THE_CONDUCTOR),
 * the Blanket exchange (takes the Blanket, gives a Mushroom, STG6_19_GIVE_BLANKET), and day 2,
 * day 3 and return-trip lines.
 */
static EVT_BEGIN(talk_syashou)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_12_GIVE_AUTOGRAPH, STG6_18_BLANKET_EXCHANGE)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_01_THE_CONDUCTOR)
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
        SET_VALUE(GSW_STORY_PROGRESS, STG6_01_THE_CONDUCTOR)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
        IF_INT_EQUAL(LSWF_RSH_04_CONDUCTOR_TALK_TOGGLE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_07"), 0, 0)
            SET_VALUE(LSWF_RSH_04_CONDUCTOR_TALK_TOGGLE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_08"), 0, 0)
            SET_VALUE(LSWF_RSH_04_CONDUCTOR_TALK_TOGGLE, 0)
        END_IF
        CASE_LESS(STG6_09_BUBS_REQUEST)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_84_01"), 0, 0)
        CASE_LESS(STG6_12_GIVE_AUTOGRAPH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_105"), 0, 0)
        SET_VALUE(GSWF_RSH_CONDUCTOR_BUB_HINT_HEARD, 1)
        CALL_CHILD(PTR(evt_rsh_konarikin_irai_party_talk))
        IF_INT_EQUAL(GSWF_RSH_BUB_HINT_REPEATED, 1)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
                SET_VALUE(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
            END_IF
        END_IF
        CASE_LESS(STG6_13_TALK_TO_THE_CONDUCTOR)
        IF_INT_EQUAL(GSWF_RSH_CONDUCTOR_BLANKET_TALK_DONE, 0)
            SET_VALUE(GSWF_RSH_CONDUCTOR_BLANKET_TALK_DONE, 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_111"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_114"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_114_yn"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_112"))
            SET_VALUE(GSW_STORY_PROGRESS, STG6_13_TALK_TO_THE_CONDUCTOR)
        ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_113"))
        END_IF
        CASE_LESS(STG6_15_GHOST_REQUEST)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_115"), 0, 0)
        CASE_LESS(STG6_16_THE_BAGGAGE_CAR)
        CALL_CHILD(PTR(evt_allow_kamotsu))
        CASE_LESS(STG6_17_GHOST_DIARY)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_129_01"), 0, 0)
        CASE_LESS(STG6_18_BLANKET_EXCHANGE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_129"), 0, 0)
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_142"), 0, 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(item_tbl2), LW(1))
        IF_INT_NOT_EQUAL(LW(1), -1)
#else
        CALL(evt_win_item_select, 0, PTR(item_tbl2), LW(1), 0)
        IF_INT_NOT_EQUAL(LW(1), -1)
#if VERSION >= VERSION_US
            CALL(evt_mario_adjust_dir)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_mario_get_dispdir, LW(3))
            IF_INT_LESS(LW(3), 90)
                ADD(LW(0), -18)
            ELSE
                ADD(LW(0), 18)
            END_IF
            ADD(LW(1), 18)
            CALL(evt_item_entry, STRING("mofu"), ITEM_BLANKET, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_mario_set_pose, STRING("M_C_3"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_item_delete, STRING("mofu"))
#endif
#endif
            CALL(evt_msg_print, 0, STRING("stg6_rsh_143"), 0, 0)
            CALL(evt_item_entry, STRING("orei"), ITEM_MUSHROOM, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_item_get_item, STRING("orei"))
            CALL(evt_item_get_item_end_wait, STRING("orei"))
            CALL(evt_msg_print, 0, STRING("stg6_rsh_145"), 0, 0)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_19_GIVE_BLANKET)
            CALL(evt_pouch_remove_item, ITEM_BLANKET, LW(0))
        END_IF
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_145"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        IF_INT_EQUAL(LSWF_RSH_04_CONDUCTOR_DAY2_TOGGLE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_158"), 0, 0)
            SET_VALUE(LSWF_RSH_04_CONDUCTOR_DAY2_TOGGLE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_159"), 0, 0)
            SET_VALUE(LSWF_RSH_04_CONDUCTOR_DAY2_TOGGLE, 0)
        END_IF
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_213"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_220"), 0, 0)
        CASE_LESS(STG6_57_A_MAIL_FROM_PEACH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_254"), 0, 0)
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
            IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
                RETURN
            END_IF
            CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09"), 0, 0)
        ELSE
            IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_13"), 0, 0)
            ELSE
#if VERSION <= VERSION_DEMO_US
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_14"), 0, 0)
#else
                IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_14"), 0, 0)
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_09_1"), 0, 0)
                END_IF
#endif
            END_IF
        END_IF
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_12_GIVE_AUTOGRAPH, STG6_19_GIVE_BLANKET)
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Overheard background line of the salaryman (stg6_rsh_165_01) shown in a repeating balloon
 * while Mario stands in cabin 3 on day 2 without talking to anybody; started by evt_powan_room.
 */
EVT_BEGIN(talk_sarary_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_get_position, STRING("サラリーマン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("サラリーマン"), LW(3))
    ADD(LW(0), -5)
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_01"), 0, STRING("サラリーマン"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()

/**
 * Init event of the salaryman (サラリーマン) in rsh_04: off the map from STG6_01 to STG6_04, at
 * (190, 0, -150) from STG6_21_DAY_2 to STG6_26, off the map during day 3 (STG6_45 to 48) and
 * from STG6_49_POSHLEY_HEIGHTS on; otherwise he keeps his setup position.
 * When parked below y 0 his jumping
 * flag is turned off.
 */
static EVT_BEGIN(init_sarary)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_04_GRILLING_HEFF_T)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_21_DAY_2, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_npc_set_position, STRING("me"), 190, 0, -150)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Talk script of the salaryman in rsh_04: lines for the story stage (stg6_rsh_18, 79, 125, 166, 215, 229). */
static EVT_BEGIN(talk_sarary)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_05_GALLEY_KETTLE_DISCOVERED)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_18"), 0, 0)
        CASE_LESS(STG6_12_GIVE_AUTOGRAPH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_79"), 0, 0)
        CASE_LESS(STG6_19_GIVE_BLANKET)
        IF_INT_EQUAL(GSWF_RSH_PENNINGTON_GHOST_TALK_DONE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_79_01"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_125"), 0, 0)
        END_IF
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_08"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_166"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_215"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_229"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Shared conversation of the Narikin parents and their child when Mario talks to either parent:
 * Mama, Papa and Bub speak (stg6_rsh_86 to 88_01), the partner steps aside,
 * and Bub asks a yes/no question (stg6_rsh_94_yn) that moves the story to STG6_09_BUBS_REQUEST (accepted)
 * or to STG6_07_SYLVIA or STG6_08_BUB.
 */
EVT_BEGIN(talk_papa_mama)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_cam3d_evt_set, -220, 105, 186, -220, 40, -183, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンパパ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンママ"), NPC_FLAG_NO_HIT)
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
        SET_VALUE(GSW_STORY_PROGRESS, STG6_09_BUBS_REQUEST)
    ELSE
        CALL(evt_msg_print_add, 0, STRING("stg6_rsh_93"))
        SET_VALUE(GSW_STORY_PROGRESS, STG6_07_SYLVIA)
        SET_VALUE(GSW_STORY_PROGRESS, STG6_08_BUB)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("ナリキンパパ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("ナリキンママ"), NPC_FLAG_NO_HIT)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    RETURN
EVT_END()

/**
 * Overheard background line of Papa Narikin (stg6_rsh_85_01) shown in a repeating balloon
 * while Mario stands in cabin 5 without talking to anybody; started by evt_narikin_room.
 * Repeats the parents' conversation (talk_papa_mama) when Mario talks to him at the right stage.
 */
EVT_BEGIN(talk_papa_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンパパ"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_get_position, STRING("ナリキンパパ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ナリキンパパ"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 15)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_01"), 0, STRING("ナリキンパパ"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()

/**
 * Init event of Papa Narikin (ナリキンパパ): parks him off the map from STG6_27_RIVERSIDE_STATION to
 * STG6_42_DRAWBRIDGE_DOWN, during day 3 (STG6_45 to 48) and from STG6_49_POSHLEY_HEIGHTS on,
 * turning his jumping flag off; otherwise he keeps his setup position.
 */
static EVT_BEGIN(init_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Talk script of Papa Narikin: by story stage; once the family has been introduced he runs talk_papa_mama. */
static EVT_BEGIN(talk_papa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_15"), 0, 0)
        CASE_LESS(STG6_07_SYLVIA)
        CALL_CHILD(PTR(talk_papa_mama))
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_85"), 0, 0)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_09"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_163"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_226"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Overheard background line of Mama Narikin (stg6_rsh_85_02),
 * shown alongside talk_papa_repeat while Mario stands in cabin 5;
 * repeats the parents' conversation (talk_papa_mama) when Mario talks to her at the right stage.
 */
EVT_BEGIN(talk_mama_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ナリキンママ"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_get_position, STRING("ナリキンママ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ナリキンママ"), LW(3))
    ADD(LW(1), LW(3))
    ADD(LW(1), 10)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_85_02"), 0, STRING("ナリキンママ"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()

/**
 * Init event of Mama Narikin (ナリキンママ): parks her off the map during day 3 (STG6_45 to 48) and from
 * STG6_49_POSHLEY_HEIGHTS on, turning her jumping flag off; otherwise she keeps her setup position.
 */
static EVT_BEGIN(init_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Talk script of Mama Narikin: by story stage; once the family has been introduced she runs talk_papa_mama. */
static EVT_BEGIN(talk_mama)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_16"), 0, 0)
        CASE_LESS(STG6_07_SYLVIA)
        CALL_CHILD(PTR(talk_papa_mama))
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_89"), 0, 0)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_10"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_164"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_214"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_227"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the Narikin child (コナリキン): parks it off the map from STG6_27_RIVERSIDE_STATION to
 * STG6_42_DRAWBRIDGE_DOWN, during day 3 (STG6_45 to 48) and from STG6_49_POSHLEY_HEIGHTS on,
 * turning its jumping flag off; otherwise it keeps its setup position.
 */
static EVT_BEGIN(init_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Items the player may hand over for the autograph: the Autograph only, ended by -1. */
s32 item_tbl3[] = {
    ITEM_AUTOGRAPH, -1,
};

static EVT_BEGIN(talk_konari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_17"), 0, 0)
        CASE_LESS(STG6_07_SYLVIA)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_90"), 0, 0)
        CASE_LESS(STG6_09_BUBS_REQUEST)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_08_BUB)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_08_BUB)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_91"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_94"), 0, 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_94_yn"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_92"))
            SET_VALUE(GSW_STORY_PROGRESS, STG6_09_BUBS_REQUEST)
        ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_93"))
        END_IF
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
        CASE_LESS(STG6_12_GIVE_AUTOGRAPH)
        // With the Autograph, Mario can hand it over.
        CALL(evt_pouch_check_item, ITEM_AUTOGRAPH, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            IF_INT_EQUAL(GSWF_RSH_BUB_HINT_GIVEN, 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_95"), 0, 0)
                SET_VALUE(GSWF_RSH_BUB_HINT_GIVEN, 1)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_95_01"), 0, 0)
                SET_VALUE(GSWF_RSH_BUB_HINT_REPEATED, 1)
                IF_INT_EQUAL(GSWF_RSH_CONDUCTOR_BUB_HINT_HEARD, 1)
                    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
                        SET_VALUE(GSW_STORY_PROGRESS, STG6_10_THE_CONDUCTOR_ZIP_TOAD)
                    END_IF
                END_IF
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_96"), 0, 0)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_win_item_select, PTR(item_tbl3), LW(1))
#else
            CALL(evt_win_item_select, 0, PTR(item_tbl3), LW(1), 0)
#endif
            IF_INT_EQUAL(LW(1), -1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_101"), 0, 0)
            ELSE
                IF_INT_NOT_EQUAL(LW(1), ITEM_AUTOGRAPH)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_100"), 0, 0)
                ELSE
#if VERSION >= VERSION_US
                    CALL(evt_mario_adjust_dir)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    CALL(evt_mario_get_dispdir, LW(3))
                    IF_INT_LESS(LW(3), 90)
                        ADD(LW(0), -18)
                    ELSE
                        ADD(LW(0), 18)
                    END_IF
                    ADD(LW(1), 18)
                    CALL(evt_item_entry, STRING("sign"), ITEM_AUTOGRAPH, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_mario_set_pose, STRING("M_C_3"))
                    WAIT_MS(1000)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_item_delete, STRING("sign"))
#endif
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_97"), 0, 0)
                    // He pays Mario with a Shine Sprite.
                    CALL(evt_item_entry, STRING("orei"), ITEM_SHINE_SPRITE, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("orei"))
                    CALL(evt_item_get_item_end_wait, STRING("orei"))
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_99"), 0, 0)
                    CALL(evt_pouch_remove_item, ITEM_AUTOGRAPH, LW(0))
                    SET_VALUE(GSW_STORY_PROGRESS, STG6_12_GIVE_AUTOGRAPH)
                    SET_VALUE(GSWF_SHINE_35, 1)
                END_IF
            END_IF
        END_IF
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_102"), 0, 0)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_11"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_165"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_228"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Overheard background line of Detective Pennington (stg6_rsh_165_02) shown in a repeating balloon beside the
 * salaryman's while Mario stands in cabin 3; started by evt_powan_room.
 */
EVT_BEGIN(talk_powan_repeat)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_NO_TALK_BALLOON)
    CALL(evt_npc_get_position, STRING("ポワン探偵"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_height, STRING("ポワン探偵"), LW(3))
    ADD(LW(0), 15)
    ADD(LW(1), LW(3))
    SUBTRACT(LW(1), 4)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_165_02"), 0, STRING("ポワン探偵"))
    LOOP(0)
        CALL(evt_msg_repeat)
    END_LOOP
    RETURN
EVT_END()

/**
 * Init event of Pennington (ポワン探偵) in rsh_04: off the map from STG6_01 to STG6_04, at
 * (230, 0, -180) from STG6_21_DAY_2 to STG6_26, off the map from STG6_27 to STG6_42,
 * during day 3 (STG6_45 to 48) and from STG6_49_POSHLEY_HEIGHTS on; otherwise he keeps his setup
 * position. When parked below y 0 his jumping flag is turned off.
 */
static EVT_BEGIN(init_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_04_GRILLING_HEFF_T)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_21_DAY_2, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_npc_set_position, STRING("me"), 230, 0, -180)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_45_DAY_3_STARTS, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of Pennington in rsh_04: his case lines by story stage;
 * at STG6_05_GALLEY_KETTLE_DISCOVERED he takes Mario on as his assistant (stg6_rsh_65 to 68,
 * STG6_06_PENNINGTONS_ASSISTANT).
 */
static EVT_BEGIN(talk_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_05_GALLEY_KETTLE_DISCOVERED)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_14"), 0, 0)
        CASE_LESS(STG6_06_PENNINGTONS_ASSISTANT)
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
        CALL(evt_cam3d_evt_set, 265, 93, 137, 265, 36, -183, 500, EASING_SINE_IN_OUT)
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
        SET_VALUE(GSW_STORY_PROGRESS, STG6_06_PENNINGTONS_ASSISTANT)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        CASE_LESS(STG6_09_BUBS_REQUEST)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_69"), 0, 0)
        CASE_LESS(STG6_12_GIVE_AUTOGRAPH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_103"), 0, 0)
        SET_VALUE(GSWF_RSH_PENNINGTON_BUB_TALK_UNUSED, 1)
        CASE_LESS(STG6_13_TALK_TO_THE_CONDUCTOR)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_104"), 0, 0)
        CASE_LESS(STG6_15_GHOST_REQUEST)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_116"), 0, 0)
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_124"), 0, 0)
        SET_VALUE(GSWF_RSH_PENNINGTON_GHOST_TALK_DONE, 1)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147"), 0, 0)
        CASE_LESS(STG6_22_DOCUMENT)
        CALL_CHILD(PTR(evt_shorui_funshitu))
        CASE_LESS(STG6_23_VITAL_PAPER)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_170"), 0, 0)
        CASE_LESS(STG6_24_A_VITAL_CLUE)
        CALL_CHILD(PTR(evt_daijisouna_kami))
        CASE_LESS(STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_177_01"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_225"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** NPCs of the car: the conductor, the salaryman, Goldbob and his wife, Little Narikin and Pennington. */
static NpcSetupInfo npc_data[] = {
    {
        "車掌", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
        "サラリーマン", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
        "ナリキンパパ", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
        "ナリキンママ", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
        "コナリキン", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
        "ポワン探偵", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
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
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_STARPIECE_85, 1)
#else
    IF_INT_EQUAL(GSWF_STARPIECE_85, 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
#endif
            CALL(evt_msg_print, 0, LW(1), 0, 0)
#if VERSION > VERSION_DEMO_US
            ELSE
            CALL(evt_msg_print, 0, LW(3), 0, 0)
        END_IF
#endif
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_item_entry, STRING("_item00"), ITEM_STAR_PIECE, -260, 0, -240, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("_item00"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_item_get_item, STRING("_item00"))
    SET_VALUE(GSWF_STARPIECE_85, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Luggage shelves of the car: each shelf hit area with the message it shows and the script a shelf runs. */
static ShelfAccessTbl3 access_tbl = {
    {
#if VERSION <= VERSION_DEMO_US
        {"tana_3", "stg6_rsh_272", NULL},
        {"tana_4", "stg6_rsh_273", NULL},
        {"tana_5", "stg6_rsh_274", evt_tana_5},
#else
        {"tana_3", "stg6_rsh_272", NULL, "stg6_rsh_267"},
        {"tana_4", "stg6_rsh_273", NULL, "stg6_rsh_267"},
        {"tana_5", "stg6_rsh_274", evt_tana_5, "stg6_rsh_267"},
#endif
    },
    {NULL},
};

/** Trigger script for the shelves: prints the message in LW(1). */
static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(LW(2), 0)
#if VERSION > VERSION_DEMO_US
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
#endif
            CALL(evt_msg_print, 0, LW(1), 0, 0)
#if VERSION > VERSION_DEMO_US
            ELSE
            CALL(evt_msg_print, 0, LW(3), 0, 0)
        END_IF
#endif
        ELSE
        CALL_CHILD(LW(2))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Registers the shelf triggers from access_tbl; each runs evt_access (or evt_tana_5 for tana_5). */
static EVT_BEGIN(evt_access_entry)
    SET_INT_READ_ADDR(PTR(&access_tbl))
    LOOP(0)
#if VERSION <= VERSION_DEMO_US
        READ_INT3(LW(0), LW(1), LW(2))
#else
        READ_INT4(LW(0), LW(1), LW(2), LW(3))
#endif
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_access), 0)
    END_LOOP
    RETURN
EVT_END()

/** Trigger on the west door (w_bero) on day 3: prints the message about the locked door. */
static EVT_BEGIN(evt_door_msg)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        LOOP(3)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -50, 200)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_trans, 1, STRING("w_tobira"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -50, 0, 200)
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
    IF_INT_EQUAL(GSWF_RSH_04_STUCK_DOOR_SEEN, 0)
        CALL(evt_cam3d_evt_set, 150, 135, 261, -200, 67, -13, 800, EASING_SINE_IN_OUT)
        WAIT_MS(800)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg6_rsh_236_02"), 0, 0)
    IF_INT_EQUAL(GSWF_RSH_04_STUCK_DOOR_SEEN, 0)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        SET_VALUE(GSWF_RSH_04_STUCK_DOOR_SEEN, 1)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

// GW(14)/GW(15) hold the two repeat-message event IDs for the active room.
EVT_BEGIN(evt_narikin_room)
    SET_TYPE(0)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        IF_INT_EQUAL(GSWF_RSH_04_IN_ROOM_5, 1)
            IF_INT_EQUAL(LF(0), 0)
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    SET_VALUE(LF(0), 1)
                    SWITCH(GSW_STORY_PROGRESS)
                        CASE_BETWEEN(STG6_06_PENNINGTONS_ASSISTANT, STG6_18_BLANKET_EXCHANGE)
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
                    CASE_BETWEEN(STG6_06_PENNINGTONS_ASSISTANT, STG6_18_BLANKET_EXCHANGE)
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
        IF_INT_EQUAL(GSWF_RSH_04_IN_ROOM_3, 1)
            IF_INT_EQUAL(LF(0), 0)
                CALL(mario_flag, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    SET_VALUE(LF(0), 1)
                    SWITCH(GSW_STORY_PROGRESS)
                        CASE_EQUAL(STG6_21_DAY_2)
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
                    CASE_EQUAL(STG6_21_DAY_2)
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

/**
 * Shared map init for the three variants of rsh_04 on the first trip:
 * scenery, the three cabin doors, the NPCs, the baggage car door (open only while the story needs it),
 * the shelf signs, the cabin watchers (evt_narikin_room, evt_powan_room), and the music for the story stage.
 */
EVT_BEGIN(rsh_04_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CASE_GREATER_OR_EQUAL(STG6_49_00_TRAIN_ARRIVAL)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("yure"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
    END_SWITCH
    CALL_CHILD(PTR(evt_rsh_door_init))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_4))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    SET_VALUE(LW(0), PTR(&door_5))
    CALL_CHILD(PTR(evt_rsh_door_entry))
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_16_THE_BAGGAGE_CAR)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_18_BLANKET_EXCHANGE)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_45_DAY_3_STARTS)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_on))
#if VERSION >= VERSION_US
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_49_POSHLEY_HEIGHTS)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_55_PEACH_GRODUS_X_NAUT)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(bero_case_switch_off))
        END_IF
#endif
    END_IF
    CALL_CHILD(PTR(evt_access_entry))
    SPAWN_EVENT(PTR(evt_narikin_room))
    SPAWN_EVENT(PTR(evt_powan_room))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_02_A_CASE_OF_LOST_STEW, STG6_04_GRILLING_HEFF_T)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_22_DOCUMENT, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_HOM1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_BETWEEN(STG6_44_END_OF_DAY_2, STG6_45_DAY_3_STARTS)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_46_ENGINEER, STG6_47_ESCAPING_THE_SMORGS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_49_POSHLEY_HEIGHTS, STG6_55_04_BOWSER_RAWK_HAWK)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_04_b"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_04_c"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of rsh_04_a (first visit): doors, shared init, the Smorg window effect,
 * the conductor's automatic talk at STG6_17_GHOST_DIARY and the locked west door message on day 3.
 */
EVT_BEGIN(rsh_04_a_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_a))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt))
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_45_DAY_3_STARTS)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("w_bero"), 0, PTR(evt_door_msg), 0)
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

/** Init script of rsh_04_b (first visit): doors (a scripted entry skips the normal door run), shared init. */
EVT_BEGIN(rsh_04_b_init_evt)
    IF_INT_EQUAL(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    ELSE
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, 220, 0, -20)
        CALL(evt_party_set_pos, 0, 200, 0, -20)
#if VERSION > VERSION_DEMO_US
        DEBUG_PUT_MSG(STRING("パーティドア移動フラグクリア"))
        CALL(evt_party_trigflg_onoff, 0, PARTY_TRIG_FOLLOW_CLOSE | PARTY_TRIG_HALF_PUSH | PARTY_TRIG_BEHIND_DIR_SET | PARTY_TRIG_DOOR_MOVE, 0)
#endif
    END_IF
    CALL_CHILD(PTR(rsh_04_init_evt))
    RETURN
EVT_END()

/** Map objects of the scenery that passes by the windows. */
static const char* window_tbl[] = {
    "mado_uturi_1",
    "mado_uturi_2",
    "mado_uturi_3",
    NULL,
};

/**
 * Init script of rsh_04_c (first visit): doors (a scripted entry skips the normal door run),
 * shared init and window scenery.
 */
EVT_BEGIN(rsh_04_c_init_evt)
    IF_INT_EQUAL(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    ELSE
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -200, 0, -30)
        CALL(evt_party_set_pos, 0, -230, 0, -30)
#if VERSION > VERSION_DEMO_US
        DEBUG_PUT_MSG(STRING("パーティドア移動フラグクリア"))
        CALL(evt_party_trigflg_onoff, 0, PARTY_TRIG_FOLLOW_CLOSE | PARTY_TRIG_HALF_PUSH | PARTY_TRIG_BEHIND_DIR_SET | PARTY_TRIG_DOOR_MOVE, 0)
#endif
    END_IF
    CALL_CHILD(PTR(rsh_04_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_17_GHOST_DIARY)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("w_bero"))
            SPAWN_EVENT(PTR(evt_syasyou_auto_talk))
        END_IF
    END_IF
    RETURN
EVT_END()

/** Doors and exits of rsh_04 variant a (return trip): w_bero to rsh_07_a; e_bero to rsh_03_a. */
static BeroEntry bero_data_a_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Doors and exits of rsh_04 variant b (return trip): w_bero to rsh_07_b; e_bero to rsh_03_b. */
static BeroEntry bero_data_b_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Doors and exits of rsh_04 variant c (return trip): w_bero to rsh_07_c; e_bero to rsh_03_c. */
static BeroEntry bero_data_c_return[] = {
    {
        "w_bero", // hitName
        1, // type
        8, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Shared map init for rsh_04 on the second visit:
 * scenery for the outbound or return trip, the cabin doors, NPCs, random passengers (evt_rsh_04_rnd_npc_entry),
 * shelf signs and the music for the station the train has reached.
 */
EVT_BEGIN(rsh_04_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 0)
        IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 0)
            IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 0)
                CALL(evt_map_playanim, STRING("yure"), MAP_ANIM_PLAY_LOOP, 0)
                IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                    CALL(evt_map_playanim, STRING("hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
                ELSE
                    CALL(evt_map_playanim, STRING("rt_hai_sora"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_yama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_iwayama"), MAP_ANIM_PLAY_LOOP, 0)
                    CALL(evt_map_playanim, STRING("rt_hai_saboten"), MAP_ANIM_PLAY_LOOP, 0)
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
    IF_INT_EQUAL(GSWF_RSH_TRAIN_AT_POSHLEY, 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    ELSE
        IF_INT_EQUAL(GSWF_RSH_TRAIN_AT_ROGUEPORT, 1)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
#if VERSION <= VERSION_DEMO_US
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR5"))
#else
            CALL(evt_snd_envoff, PSND_FADEOUT)
#endif
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        ELSE
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_04_a"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF
            // Revisit variant B retains the current moving-train audio.
            IF_STR_EQUAL(LW(0), STRING("rsh_04_b"))
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_04_c"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_rsh_04_rnd_npc_entry))
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_a_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_a_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 1)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_b_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_b_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 1)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_04_c_init_evt_nidome)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
        SET_VALUE(LW(0), PTR(bero_data_c))
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_c_return))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_04_init_evt_nidome))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    SET_VALUE(GSWF_RSH_REVISIT_MAP_A, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_B, 0)
    SET_VALUE(GSWF_RSH_REVISIT_MAP_C, 1)
    RETURN
EVT_END()


// Suspend room-repeat dialogue while field-battle mode 2 or the ghost-diary window is active.
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
