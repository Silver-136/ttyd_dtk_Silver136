// rsh_02 (列車, Excess Express): passenger car with compartments 3-5: gourmet Toad, ghost (yuurei), salaryman and Toron
// NPCs, overnight beds and the day-1/day-2 end scenes; doors to rsh_01 and rsh_03.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "driver/msgdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mot/mot_roll.h"
#include "mot/mot_slit.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "mario/mariost.h"
#include "driver/npcdrv.h"
#include "pmario_sound.h"
#include "evt/evt_bero.h"
#include "rsh.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"


USER_FUNC(bgm_wait_stop);
USER_FUNC(toron_check);
void yuurei_callback(s32 type, void* param);
void powan_callback(s32 type, void* param);

static BeroEntry bero_data_a[3];
static BeroEntry bero_data_b[3];
static BeroEntry bero_data_c[3];
EvtWord evt_powan_msg[36];
EvtWord evt_nabe[247];
static EvtWord init_gurume[38];
static EvtWord talk_gurume[176];
static EvtWord talk_powan[2];
EvtWord init_yuurei[250];
static EvtWord evt_shake[39];
EvtWord evt_shake2[6];
static s32 item_tbl2[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord talk_yuurei[];
#else
EvtWord talk_yuurei[451];
#endif
static EvtWord init_toron[41];
static EvtWord talk_toron[8];
static EvtWord talk_sarary[8];
static NpcSetupInfo npc_data[6];
static const char* door_3[7];
static const char* door_4[7];
static const char* door_5[7];
EvtWord evt_1day_end[68];
EvtWord evt_2day_end[68];
EvtWord evt_normal_yado[119];
EvtWord evt_bed[22];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_toron_catch[];
#else
EvtWord evt_toron_catch[281];
#endif
static s32 item_tbl[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_toron_catch2[];
#else
EvtWord evt_toron_catch2[359];
#endif
#if VERSION >= VERSION_US
EvtWord evt_toron_appear[437];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord evt_toron_appear[408];
#else
EvtWord evt_toron_appear[414];
#endif
#endif
#if VERSION == VERSION_EU
static EvtWord evt_daiji_kami[96];
#else
static EvtWord evt_daiji_kami[88];
#endif
static EvtWord evt_yuka_simi[45];
static EvtWord evt_access[14];
static EvtWord evt_access2[44];
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_access_entry[21];
#else
static EvtWord evt_access_entry[62];
#endif
EvtWord rsh_02_init_evt[247];
#if VERSION >= VERSION_US
EvtWord rsh_02_a_init_evt[687];
#else
EvtWord rsh_02_a_init_evt[685];
#endif
EvtWord rsh_02_b_init_evt[168];
static const char* window_tbl[4];
EvtWord rsh_02_c_init_evt[142];
#if VERSION <= VERSION_DEMO_US
extern EvtWord rsh_02_init_evt_nidome[];
#else
EvtWord rsh_02_init_evt_nidome[208];
#endif
EvtWord rsh_02_a_init_evt_nidome[337];
#if VERSION <= VERSION_DEMO_US
extern EvtWord rsh_02_b_init_evt_nidome[];
#else
EvtWord rsh_02_b_init_evt_nidome[213];
#endif
EvtWord rsh_02_c_init_evt_nidome[136];
EvtWord evt_nidome_bed[126];

// Descriptive name (not in the symbol maps); placeholder unk_rsh_00002384, JP rsh .text+0x22C0. See docs/NAME_PROVENANCE.md.
// Wait until neither roll-cancel protection nor continued slit movement prevents normalization.
#if VERSION > VERSION_DEMO_US
USER_FUNC(mario_normalize_wait) {
    if (!marioRollCancelDisable() && !marioSlitContinueChk()) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
#endif

/** Doors and exits of rsh_02 variant a: w_bero to rsh_03_a; e_bero to rsh_01_a. */
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
        "rsh_03_a", // nextMapName
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
        "rsh_01_a", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

/** Doors and exits of rsh_02 variant b: w_bero to rsh_03_b; e_bero to rsh_01_b. */
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
        "rsh_03_b", // nextMapName
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
        "rsh_01_b", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

/** Doors and exits of rsh_02 variant c: w_bero to rsh_03_c; e_bero to rsh_01_c. */
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
        "rsh_03_c", // nextMapName
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
        "rsh_01_c", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

/**
 * Reaction during Pennington's speech in evt_nabe:
 * Detective Pennington plays A_1, Mario gives a question-mark balloon, and the gourmet Toad turns to Mario.
 * Started from powan_callback.
 */
EVT_BEGIN(evt_powan_msg)
    CALL(evt_npc_set_anim, STRING("ポワン探偵"), STRING("A_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("hatena"), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_set_dir_to_target, STRING("グルメキノピオ"), STRING("mario"))
    RETURN
EVT_END()

/**
 * Trigger on the pot shelf (tana_3).
 * Before the story reaches STG6_04_GRILLING_HEFF_T (or after the pot was found) it just prints
 * stg6_rsh_264; during that stage Mario finds the Galley Pot, gets the item,
 * the gourmet Toad confesses and Pennington appears and walks off,
 * and the story moves to STG6_05_GALLEY_KETTLE_DISCOVERED.
 */
EVT_BEGIN(evt_nabe)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_04_GRILLING_HEFF_T)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_05_GALLEY_KETTLE_DISCOVERED)
        SET_VALUE(LF(0), 0)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_264"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_56"), 0, 0)
    CALL(evt_item_entry, STRING("nabe"), ITEM_GALLEY_POT, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("nabe"))
    CALL(evt_item_get_item_end_wait, STRING("nabe"))
    CALL(evt_mario_set_dir_npc, STRING("グルメキノピオ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_57"), 0, STRING("グルメキノピオ"))
    CALL(evt_fade_entry, FADE_OUT_BLACK, 1000, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_mario_set_pos, 205, 0, -175)
    CALL(evt_party_set_pos, 0, 180, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 265, 0, -175)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("グルメキノピオ"))
    CALL(evt_set_dir_to_target, STRING("グルメキノピオ"), STRING("ポワン探偵"))
    CALL(evt_cam3d_evt_zoom_in, 25, 0, 20, EASING_SINE_IN_OUT)
    CALL(evt_fade_entry, FADE_IN_BLACK, 1000, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_58"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_59"), 0, STRING("グルメキノピオ"))
    WAIT_MS(500)
    SET_VALUE(LW(10), 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_60_00"), PTR(powan_callback), STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 250, -150, 0, FLOAT(40.0f), 1)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 251, -90, 0, FLOAT(40.0f), 1)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_60_01"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 252, 0, 0, FLOAT(40.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_05_GALLEY_KETTLE_DISCOVERED)
    CALL(evt_del_case_evt, 0, GW(10))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init event of the gourmet Toad: parked below the floor during the Riverside stages
 * and from the end of day 2 to the Smorg miasma; turns jumping off when he is on the floor.
 */
static EVT_BEGIN(init_gurume)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_42_DRAWBRIDGE_DOWN)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(STG6_44_END_OF_DAY_2, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of the gourmet Toad (gurume), by story stage:
 * before the stew case, the stew case (sets STG6_04_GRILLING_HEFF_T), hints about the ghost's cabin,
 * and day 1, day 2 and later lines.
 */
static EVT_BEGIN(talk_gurume)
    SWITCH(GSW_STORY_PROGRESS)
#if VERSION >= VERSION_US
        CASE_LESS(STG6_02_A_CASE_OF_LOST_STEW)
#else
        CASE_LESS(STG6_01_THE_CONDUCTOR)
#endif
        CALL(evt_msg_print, 0, STRING("stg6_rsh_12"), 0, 0)
        CASE_LESS(STG6_03_STAINED_CARPET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_54_00"), 0, 0)
        CASE_LESS(STG6_05_GALLEY_KETTLE_DISCOVERED)
        SET_VALUE(GSW_STORY_PROGRESS, STG6_04_GRILLING_HEFF_T)
        IF_INT_EQUAL(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_54_01"), 0, 0)
            SET_VALUE(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_55"), 0, 0)
            SET_VALUE(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 0)
        END_IF
        CASE_LESS(STG6_13_TALK_TO_THE_CONDUCTOR)
        IF_INT_EQUAL(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_63"), 0, 0)
            SET_VALUE(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_64"), 0, 0)
            SET_VALUE(LSWF_RSH_02_GOURMET_TALK_TOGGLE, 0)
        END_IF
        CASE_LESS(STG6_14_GHOST_T)
        IF_INT_EQUAL(GSWF_RSH_GOURMET_NOISE_TALK_DONE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_117"), 0, 0)
            SET_VALUE(GSWF_RSH_GOURMET_NOISE_TALK_DONE, 1)
        ELSE
            IF_INT_EQUAL(GSWF_RSH_NOISY_ROOM_CHECKED, 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_117_01"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_117"), 0, 0)
            END_IF
        END_IF
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_117_02"), 0, 0)
        CASE_LESS(STG6_21_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_04"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        IF_INT_EQUAL(LSWF_RSH_02_GOURMET_DAY2_TALKED, 0)
            SET_VALUE(LSWF_RSH_02_GOURMET_DAY2_TALKED, 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_161"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_162"), 0, 0)
        END_IF
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_222"), 0, 0)
        CASE_LESS(STG6_57_A_MAIL_FROM_PEACH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_258"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of Pennington in rsh_02: does nothing. */
static EVT_BEGIN(talk_powan)
    RETURN
EVT_END()

EVT_BEGIN(init_yuurei)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_13_TALK_TO_THE_CONDUCTOR)
        RETURN
        CASE_BETWEEN(STG6_13_TALK_TO_THE_CONDUCTOR, STG6_20_END_OF_DAY_1)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(STG6_22_DOCUMENT, STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(STG6_27_RIVERSIDE_STATION, STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(STG6_44_END_OF_DAY_2, STG6_48_SMORG_MIASMA)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(STG6_49_POSHLEY_HEIGHTS)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
    END_SWITCH
    CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ゆうれい"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, 0)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(GSWF_RSH_GHOST_APPEARED, 0)
            SET_VALUE(LW(0), 0)
            LOOP(0)
                // Once Mario has been inside cabin 4 with Vivian for a while, the ghost fades in.
                IF_INT_EQUAL(GSWF_RSH_02_IN_ROOM_4, 1)
                    CALL(evt_mario_party_use_check, LW(1))
                    IF_INT_EQUAL(LW(1), 6)
                        ADD(LW(0), 1)
                        IF_INT_GREATER(LW(0), 240)
                            BREAK_LOOP
                        END_IF
                    ELSE
                        SET_VALUE(LW(0), 0)
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_npc_set_position, STRING("ゆうれい"), 30, 0, -175)
            CALL(evt_set_dir_to_target, STRING("ゆうれい"), STRING("mario"))
            CALL(evt_snd_bgmoff, PSND_SILENT)
            CALL(evt_npc_get_position, STRING("ゆうれい"), LW(3), LW(4), LW(5))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_GHOST_APPEAR1"), LW(3), LW(4), LW(5), 0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, PTR(talk_yuurei))
            SET_VALUE(GSWF_RSH_GHOST_APPEARED, 1)
        ELSE
            CALL(evt_npc_set_position, STRING("ゆうれい"), 30, 0, -175)
            CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, PTR(talk_yuurei))
        END_IF
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 64, 1000)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 64, 180, 1000)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF

            // She then pulses her transparency forever, pausing while Mario talks to her.
            IF_INT_EQUAL(GSWF_RSH_GHOST_FADE_HOLD, 1)
                SET_VALUE(GSWF_RSH_GHOST_FADE_HELD, 1)
                LOOP(0)
                    IF_INT_EQUAL(GSWF_RSH_GHOST_FADE_HOLD, 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                SET_VALUE(GSWF_RSH_GHOST_FADE_HELD, 0)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Camera shake during the ghost's first speech:
 * plays the surprise jingle, then repeatedly zooms the 3D camera in and out.
 * Started by yuurei_callback and stopped with evt_shake2.
 */
static EVT_BEGIN(evt_shake)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG5_GHOST1"))
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_cam3d_evt_zoom_in, 10, 0, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(2)
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

/**
 * Stops the camera shake: resets the 3D camera event (evt_cam3d_evt_off).
 * Second half of the shake in the ghost's first speech.
 */
EVT_BEGIN(evt_shake2)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    RETURN
EVT_END()

/** Items the player may give the ghost: the Ragged Diary only, ended by -1. */
static s32 item_tbl2[] = {
    ITEM_RAGGED_DIARY, -1,
};

/**
 * Talk script of the ghost.
 * Plays the ghost music and, by story stage, runs the ghost's quest:
 * introduces herself and asks for a favour (STG6_14_GHOST_T,
 * then STG6_15_GHOST_REQUEST after Mario agrees), asks for her Ragged Diary, takes it from the item menu,
 * gives Mario the Blanket in return (STG6_18_BLANKET_EXCHANGE),
 * and afterwards repeats a hint or a closing line per stage.
 */
EVT_BEGIN(talk_yuurei)
    CALL(evt_snd_bgmoff, PSND_PAUSE)

    // The ghost's music plays and her quest begins.
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG5_GHOST1"))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_15_GHOST_REQUEST)
        SET_VALUE(LW(13), 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_14_GHOST_T)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_14_GHOST_T)

            // She introduces herself and asks Mario a favour.
            CALL(evt_msg_print, 0, STRING("stg6_rsh_118"), PTR(yuurei_callback), 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_119"), PTR(yuurei_callback), 0)
        END_IF
        CALL(evt_msg_select, 0, STRING("stg6_rsh_119_yn"))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(13), 1)
            SET_VALUE(LW(14), 0)
            SET_VALUE(LW(15), 0)
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_120"))
            SET_VALUE(GSW_STORY_PROGRESS, STG6_15_GHOST_REQUEST)
        ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_121"))
        END_IF
        CASE_LESS(STG6_17_GHOST_DIARY)
        // She asks for her Ragged Diary.
        CALL(evt_msg_print, 0, STRING("stg6_rsh_122"), 0, 0)
        CASE_LESS(STG6_18_BLANKET_EXCHANGE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_135"), 0, 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(item_tbl2), LW(1))
#else
        CALL(evt_win_item_select, 0, PTR(item_tbl2), LW(1), 0)
#endif
        IF_INT_NOT_EQUAL(LW(1), -1)
            // She takes the diary and gives Mario a Blanket in return.
            CALL(evt_msg_print, 0, STRING("stg6_rsh_136"), 0, 0)
            CALL(evt_npc_set_anim, STRING("ゆうれい"), STRING("A_1"))
            WAIT_MS(1000)
            CALL(evt_npc_set_anim, STRING("ゆうれい"), STRING("S_1"))
            CALL(evt_item_entry, STRING("mofu"), ITEM_BLANKET, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_item_flag_on, STRING("mofu"), ITEM_FLAG_BGM_SILENT)
            CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
            CALL(evt_item_get_item, STRING("mofu"))
            CALL(evt_item_get_item_end_wait, STRING("mofu"))
            CALL(evt_snd_bgmoff, PSND_PAUSE)
            CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG5_GHOST1"))
            CALL(evt_msg_print, 0, STRING("stg6_rsh_138"), 0, 0)

            // Wait for the background alpha loop to acknowledge its hold before changing the ghost directly.
            SET_VALUE(GSWF_RSH_GHOST_FADE_HOLD, 1)
            LOOP(0)
                IF_INT_EQUAL(GSWF_RSH_GHOST_FADE_HELD, 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("shouten"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_npc_get_position, STRING("ゆうれい"), LW(7), LW(8), LW(9))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_GHOST_PASSAWAY1"), LW(7), LW(8), LW(9), LW(6))
            BEGIN_INLINE_EVENT
                LOOP(10)
                    CALL(evt_npc_get_position, STRING("ゆうれい"), LW(0), LW(1), LW(2))
                    CALL(evt_eff64, 0, STRING("stardust2_n64"), 5, LW(0), LW(1), LW(2), 30, 30, 5, 30, 0, 0, 0, 0)
                    WAIT_MS(500)
                END_LOOP
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 180, 60, 5000)
                LOOP(0)
                    CALL(evt_sub_intpl_msec_get_value)
                    CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
                    WAIT_FRAMES(1)
                    IF_INT_EQUAL(LW(1), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(8), 80, 5000)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_npc_set_position, STRING("ゆうれい"), LW(7), LW(0), LW(9))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(evt_snd_sfxoff, LW(6))
            WAIT_MS(1000)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("shouten"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 60, 180, 1000)
                LOOP(0)
                    CALL(evt_sub_intpl_msec_get_value)
                    CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
                    WAIT_FRAMES(1)
                    IF_INT_EQUAL(LW(1), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(evt_npc_set_position, STRING("ゆうれい"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_GHOST_RETURN1"), LW(0), LW(1), LW(2), LW(10))
                SET_VALUE(LW(0), 0)
                LOOP(20)
                    ADD(LW(0), -204)
                    CALL(evt_snd_sfx_pit, LW(10), LW(0))
                    WAIT_MS(50)
                END_LOOP
                CALL(evt_snd_sfxoff, LW(10))
            END_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, EASING_SINE_OUT, 80, LW(8), 1000)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_npc_set_position, STRING("ゆうれい"), LW(7), LW(0), LW(9))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            SET_VALUE(GSWF_RSH_GHOST_FADE_HOLD, 0)

            // Afterwards she repeats a hint or a closing line for each stage.
            CALL(evt_msg_print, 0, STRING("stg6_rsh_139"), 0, 0)
            CALL(evt_pouch_remove_item, ITEM_RAGGED_DIARY, LW(0))
            SET_VALUE(GSW_STORY_PROGRESS, STG6_18_BLANKET_EXCHANGE)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_140"), 0, 0)
        END_IF
        CASE_LESS(STG6_19_GIVE_BLANKET)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_141"), 0, 0)
        CASE_LESS(STG6_20_END_OF_DAY_1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_05"), 0, 0)
        CASE_LESS(STG6_24_A_VITAL_CLUE)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_178_00"), 0, 0)
        CASE_LESS(STG6_27_RIVERSIDE_STATION)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_178"), 0, 0)
        CASE_LESS(STG6_43_CONDUCTOR_APPEARS)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_210"), 0, 0)
        CASE_LESS(STG6_44_END_OF_DAY_2)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_223"), 0, 0)
        CASE_LESS(STG6_57_A_MAIL_FROM_PEACH)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_255"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10"), 0, 0)
    END_SWITCH
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    RETURN
EVT_END()

// Report contact with this NPC only while player input/control and the system level permit it.
USER_FUNC(toron_check) {
    NpcEntry* hit = fbatHitCheckAll();
    // トロン
    NpcEntry* npc = npcNameToPtr("トロン");
    BOOL busy = FALSE;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        busy = TRUE;
    }
    if (marioStGetSystemLevel() != 0) {
        busy = TRUE;
    }
    if (npc == hit && !busy) {
        evtSetValue(event, LW(10), 1);
    } else {
        evtSetValue(event, LW(10), 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Init event of Zip Toad (Toron) in rsh_02: at STG6_25_ZIP_TOAD_APPEARS puts him at (-240, 0, -180)
 * and starts the chase (evt_toron_catch); once caught he sits in the D_1 pose.
 */
static EVT_BEGIN(init_toron)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG6_25_ZIP_TOAD_APPEARS)
        CALL(evt_npc_set_position, STRING("me"), -240, 0, -180)
        SPAWN_EVENT(PTR(evt_toron_catch))
        CASE_EQUAL(STG6_26_ZIP_TOAD_CAUGHT)
        CALL(evt_npc_set_anim, STRING("トロン"), STRING("D_1"))
        CALL(evt_npc_set_autotalkpose, STRING("トロン"), STRING("D_1"), STRING("T_5"))
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    END_IF
    RETURN
EVT_END()

/** Talk script of Zip Toad in rsh_02: prints stg6_rsh_186. */
static EVT_BEGIN(talk_toron)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_186"), 0, 0)
    RETURN
EVT_END()

/** Talk script of the salaryman in rsh_02: prints stg6_rsh_192. */
static EVT_BEGIN(talk_sarary)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_192"), 0, 0)
    RETURN
EVT_END()

/** NPCs of the passenger car: the gourmet Toad, the ghost and Toron. */
static NpcSetupInfo npc_data[] = {
    {
        "グルメキノピオ", // name
        NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gurume, // initEvent
        NULL, // regularEvent
        talk_gurume, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ポワン探偵", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, // reactionFlags..regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ゆうれい", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_yuurei, // initEvent
        NULL, // regularEvent
        talk_yuurei, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "トロン", // name
        NPC_FLAG_ENEMY | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_toron, // initEvent
        NULL, // regularEvent
        talk_toron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "サラリーマン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, // reactionFlags..regularEvent
        talk_sarary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
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

/**
 * Bed scene at the end of day 1 (story STG6_19_GIVE_BLANKET):
 * asks to sleep, then fades out with a spotlight on Mario, sets STG6_20_END_OF_DAY_1,
 * restores HP/FP of Mario and the party and warps to rsh_02_a for the next morning.
 */
EVT_BEGIN(evt_1day_end)
    CALL(evt_msg_print, 0, STRING("stg6_yado_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_02_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG6_20_END_OF_DAY_1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 3000, FADE_IN_BLACK, 300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    SET_VALUE(GSWF_RSH_GHOST_APPEARED, 0)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("e_bero"))
    RETURN
EVT_END()

// When armed, the first type-3 notification starts shaking and the second stops that event.
void yuurei_callback(s32 type, void* param) {
    EventEntry* ev = (*(MsgWinData**)param)->event;

    if (ev->lwData[13] != 0 && type == 3) {
        ev->lwData[14]++;
        if (ev->lwData[14] == 1) {
            ev->lwData[15] = (EvtWord)evtEntry(evt_shake, 0, 0);
        } else if (ev->lwData[14] == 2 && (EventEntry*)ev->lwData[15] != NULL) {
            evtDelete((EventEntry*)ev->lwData[15]);
            evtEntry(evt_shake2, 0, 0);
        }
    }
}

/**
 * Message-window callback for Pennington's speech in evt_nabe:
 * on the second page turn it starts evt_powan_msg (counts pages in LW(10)).
 */
// Start the reaction on the second type-3 notification (the counter starts at zero).
void powan_callback(s32 type, void* param) {
    MsgWinData* work = *(MsgWinData**)param;

    if (type == 3) {
        EventEntry* ev = work->event;
        s32 count = evtGetValue(ev, LW(10));

        switch (count) {
            case 1:
                evtEntry(evt_powan_msg, 0, 0);
                break;
        }
        evtSetValue(ev, LW(10), count + 1);
    }
}

USER_FUNC(bgm_wait_stop) {
    psndSetFlag(PSND_FLAG_BGM_WAIT_0);
    return EVT_RETURN_DONE;
}

/**
 * Bed scene at the end of day 2 (story STG6_43_CONDUCTOR_APPEARS):
 * same as evt_1day_end but sets STG6_44_END_OF_DAY_2 before warping to rsh_02_a.
 */
EVT_BEGIN(evt_2day_end)
    CALL(evt_msg_print, 0, STRING("stg6_yado_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_02_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG6_44_END_OF_DAY_2)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 3000, FADE_IN_BLACK, 300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    SET_VALUE(GSWF_RSH_GHOST_APPEARED, 0)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("e_bero"))
    RETURN
EVT_END()

/**
 * Bed outside the story nights: asks "rest?" (stg6_yado_00), plays the sleep jingle, fades out with a spotlight,
 * fully restores HP/FP of Mario and the party, repositions the partners at the foot of the bed,
 * fades back in and prints the wake-up message (stg6_yado_01).
 */
EVT_BEGIN(evt_normal_yado)
    CALL(evt_msg_print, 0, STRING("stg6_yado_00"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_00_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_fade_entry, 4, 3000, 0, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    CALL(evt_fade_end_wait)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(evt_mario_set_pos, -240, 0, -210)
    CALL(evt_party_set_pos, 0, -260, 0, -225)
    CALL(evt_party_set_pos, 1, -250, 0, -240)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_set_dir, 1, 90, 0)
    WAIT_MS(3000)
    CALL(evt_fade_entry, FADE_IN_BLACK, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_print, 0, STRING("stg6_yado_01"), 0, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    RETURN
EVT_END()

/**
 * Trigger on the bed in cabin 3-5: locks input and runs evt_1day_end at STG6_19_GIVE_BLANKET,
 * evt_2day_end at STG6_43_CONDUCTOR_APPEARS, and evt_normal_yado otherwise.
 */
EVT_BEGIN(evt_bed)
    CALL(evt_mario_key_onoff, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG6_19_GIVE_BLANKET)
        CALL_CHILD(PTR(evt_1day_end))
        CASE_EQUAL(STG6_43_CONDUCTOR_APPEARS)
        CALL_CHILD(PTR(evt_2day_end))
        CASE_DEFAULT
        CALL_CHILD(PTR(evt_normal_yado))
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Chase of the Zip Toad (Toron) at STG6_25_ZIP_TOAD_APPEARS:
 * Toron blurs and jumps between three perches in the car while the script waits for Mario to touch him;
 * when he does, the catch plays (stars, damage cry, Toron stumbles to a stop and says stg6_rsh_182),
 * then the story advances to STG6_26_ZIP_TOAD_CAUGHT and the map changes to rsh_02_b.
 */
EVT_BEGIN(evt_toron_catch)
    // GF(0) requests the jumping loop to stop; GF(1) acknowledges its cleanup.
    SET_VALUE(LW(10), 0)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_blur_onoff, 1, STRING("トロン"))
        LOOP(0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_JUMP1"), -140, 16, -225, 0)
            CALL(evt_npc_set_ry, STRING("トロン"), 90)
            CALL(evt_npc_jump_position_nohit, STRING("トロン"), -140, 16, -225, 500, 40)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_MS(200)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_JUMP1"), -200, 30, -265, 0)
            CALL(evt_npc_set_ry, STRING("トロン"), 270)
            CALL(evt_npc_jump_position_nohit, STRING("トロン"), -200, 30, -265, 500, 40)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_MS(200)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_JUMP1"), -300, 75, -189, 0)
            CALL(evt_npc_set_ry, STRING("トロン"), 270)
            CALL(evt_npc_jump_position_nohit, STRING("トロン"), -300, 75, -189, 500, 40)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_MS(200)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_npc_blur_onoff, 0, STRING("トロン"))
        SET_VALUE(GF(1), 1)
    END_INLINE_EVENT
    LOOP(0)
        CALL(toron_check)
        IF_INT_EQUAL(LW(10), 1)
#if VERSION > VERSION_DEMO_US
            CALL(evt_mario_key_onoff, 0)
            BEGIN_INLINE_EVENT
                CALL(mario_normalize_wait)
#endif
                CALL(evt_mario_normalize)
                CALL(evt_party_yoshi_ride, 0, 0)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_mario_key_onoff, 0)
#else
            END_INLINE_EVENT
#endif
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(evt_eff, 0, STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -1, 0, 2, 0, 0, 0, 0)
    SET_VALUE(GF(0), 1)
    LOOP(0)
        IF_INT_EQUAL(GF(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_DAMAGED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("トロン"), STRING("C_1"))
    CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), -220)
    SET_VALUE(LW(4), -170)
    ADD(LW(3), LW(0))
    ADD(LW(4), LW(2))
    DIVIDE(LW(3), 2)
    DIVIDE(LW(4), 2)
    CALL(evt_npc_jump_position_nohit, STRING("トロン"), LW(3), 0, LW(4), 500, 30)
    CALL(evt_npc_jump_position_nohit, STRING("トロン"), -220, 0, -170, 500, 30)
    CALL(evt_npc_set_autotalkpose, STRING("トロン"), STRING("C_1"), STRING("T_4"))

    // Zip Toad is caught and says his piece.
    CALL(evt_msg_print, 0, STRING("stg6_rsh_182"), 0, STRING("トロン"))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_BLACK, 2000, FADE_IN_BLACK, 2000)

    // The story advances and the map changes to rsh_02_b.
    SET_VALUE(GSW_STORY_PROGRESS, STG6_26_ZIP_TOAD_CAUGHT)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mario_key_onoff, 1)
#endif
    RETURN
EVT_END()

/** Items the player may hand over in the thief scene: the Briefcase only, ended by -1. */
static s32 item_tbl[] = {
    ITEM_BRIEFCASE, -1,
};

/**
 * Cutscene in rsh_02_b right after the catch: Mario, Pennington,
 * the salaryman and Toron are placed around the caught thief and they talk (stg6_rsh_183 to 191),
 * the train chimes and arrives, and the map changes to hom_00 (Riverside Station) with an iris fade.
 */
EVT_BEGIN(evt_toron_catch2)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_anim, STRING("トロン"), STRING("D_1"))
    CALL(evt_npc_set_autotalkpose, STRING("トロン"), STRING("D_1"), STRING("T_5"))
    CALL(evt_mario_set_pos, -170, 0, -130)
    CALL(evt_party_set_pos, 0, -130, 0, -140)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -190, 0, -170)
    CALL(evt_npc_set_position, STRING("トロン"), -225, 0, -160)
    CALL(evt_npc_set_position, STRING("サラリーマン"), -260, 0, -135)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -215, 140, 200, -215, 40, -180, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_npc_set_anim, STRING("トロン"), STRING("D_1"))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)

    // Pennington, the salaryman and Zip Toad talk over the caught thief.
    CALL(evt_msg_print, 0, STRING("stg6_rsh_183"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_184"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_185"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_186"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_mov_pos2, -210, -150, FLOAT(60.0f))
    WAIT_MS(1000)

    // The stolen items are laid out: the Briefcase, the Gold Ring and the Shell Earrings.
    CALL(evt_item_entry, STRING("a"), ITEM_BRIEFCASE, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("a"))
    CALL(evt_item_get_item_end_wait, STRING("a"))
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("b"), ITEM_GOLD_RING, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("b"))
    CALL(evt_item_get_item_end_wait, STRING("b"))
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("c"), ITEM_SHELL_EARRINGS, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("c"))
    CALL(evt_item_get_item_end_wait, STRING("c"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_188"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
    LOOP(0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(item_tbl), LW(1))
#else
        CALL(evt_win_item_select, 0, PTR(item_tbl), LW(1), 0)
#endif
        IF_INT_NOT_EQUAL(LW(1), -1)
            BREAK_LOOP
        END_IF

        // The salaryman gets his briefcase back.
        CALL(evt_msg_print, 0, STRING("stg6_rsh_190"), 0, STRING("サラリーマン"))
    END_LOOP
    CALL(evt_pouch_remove_item, ITEM_BRIEFCASE, LW(0))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_mov_pos2, -225, -130, FLOAT(60.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    ADD(LW(1), 20)
    CALL(evt_item_entry, STRING("kaban"), ITEM_BRIEFCASE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_189_00"), 0, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_item_delete, STRING("kaban"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_189_01"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))

    // Pennington sums up the case.
    CALL(evt_msg_print, 0, STRING("stg6_rsh_191"), 0, STRING("ポワン探偵"))
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), NPC_FLAG_ENEMY)
    CALL(evt_pouch_remove_item, ITEM_VITAL_PAPER, LW(0))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_26_ZIP_TOAD_CAUGHT)
    WAIT_MS(750)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_194"), 0, 0)
    WAIT_MS(500)
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)

    // The train arrives and the map changes to hom_00.
    CALL(evt_bero_mapchange, STRING("hom_00"), STRING("hom_touchaku"))
    RETURN
EVT_END()

/**
 * Spawned by the map init during STG6_24: loops until the story has reached STG6_24_02_ANOTHER_CLUE
 * and Mario stands at x -110 or less (not below -330 in JP and later), z -70 or less, hidden with Vivian's
 * ability (partner 6) for over 300 frames; then Toron (トロン) rises into view with an appear sound at one of two
 * spots (sinking away again if Mario comes out within 60 frames). Once Mario leaves the shadow, Toron jumps out
 * and taunts him (stg6_rsh_181), the story moves to STG6_25_ZIP_TOAD_APPEARS, the chase music starts and
 * evt_toron_catch is spawned.
 */
EVT_BEGIN(evt_toron_appear)
    LABEL(100)
    CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
    CALL(evt_npc_set_rotate, STRING("トロン"), 0, 0, 0)
    CALL(evt_npc_set_ry, STRING("トロン"), 270)
    CALL(evt_npc_pera_onoff, STRING("トロン"), 1)
    WAIT_FRAMES(1)

    // Wait until Mario has found the clue and hides in the shadows with Vivian.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_24_02_ANOTHER_CLUE)
        GOTO(100)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), -110)
#if VERSION > VERSION_DEMO_US
        GOTO(100)
    END_IF
    IF_INT_LESS(LW(0), -330)
#endif
        GOTO(100)
    END_IF
    IF_INT_GREATER(LW(2), -70)
        GOTO(100)
    END_IF
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 6)
            ADD(LW(0), 1)
            IF_INT_GREATER(LW(0), 300)
                BREAK_LOOP
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_LESS(LW(0), 300)
        GOTO(100)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), NPC_FLAG_NO_HIT)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -200)
        CALL(evt_npc_set_position, STRING("トロン"), -130, -50, -278)
        CALL(evt_npc_set_ry, STRING("トロン"), 270)
        CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_APPEAR1"), LW(0), LW(1), LW(2), 0)
        SET_FLOAT_VALUE(LW(3), FLOAT(0.8330078f))
        SET_VALUE(LW(4), 1)
        SET_VALUE(LF(0), 0)
        LOOP(60)
            ADD_FLOAT(LW(1), LW(3))
            CALL(evt_npc_set_position, STRING("トロン"), LW(0), LW(1), LW(2))
            CALL(evt_mario_party_use_check, LW(5))
            IF_INT_NOT_EQUAL(LW(5), 6)
                BEGIN_INLINE_EVENT
                    LOOP(100)
                        SUBTRACT_FLOAT(LW(1), LW(3))
                        CALL(evt_npc_set_position, STRING("トロン"), LW(0), LW(1), LW(2))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
                END_INLINE_EVENT
                SET_VALUE(LF(0), 1)
                BREAK_LOOP
            END_IF
            ADD(LW(4), 1)
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(0), 1)
            GOTO(100)
        END_IF
    ELSE
        CALL(evt_npc_pera_onoff, STRING("トロン"), 0)
        CALL(evt_npc_set_rotate, STRING("トロン"), 180, 180, 90)
        CALL(evt_npc_set_position, STRING("トロン"), -300, 2, -220)
#if VERSION >= VERSION_US
        CALL(evt_npc_set_ry, STRING("トロン"), 90)
#endif
        CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
        ADD(LW(1), 2)
#endif
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRON_APPEAR1"), LW(0), LW(1), LW(2), 0)
        SET_FLOAT_VALUE(LW(3), FLOAT(0.5f))
        SET_VALUE(LW(4), 1)
        SET_VALUE(LF(0), 0)
        LOOP(60)
            ADD_FLOAT(LW(0), LW(3))
            CALL(evt_npc_set_position, STRING("トロン"), LW(0), LW(1), LW(2))
            CALL(evt_mario_party_use_check, LW(5))
            IF_INT_NOT_EQUAL(LW(5), 6)
                BEGIN_INLINE_EVENT
                    LOOP(100)
                        SUBTRACT_FLOAT(LW(0), LW(3))
                        CALL(evt_npc_set_position, STRING("トロン"), LW(0), LW(1), LW(2))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
                END_INLINE_EVENT
                SET_VALUE(LF(0), 1)
                BREAK_LOOP
            END_IF
            ADD(LW(4), 1)
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(0), 1)
            GOTO(100)
        END_IF
    END_IF
    LABEL(2)
    CALL(evt_mario_party_use_check, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(0), 6)
        GOTO(2)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_pera_onoff, STRING("トロン"), 1)
    CALL(evt_npc_set_rotate, STRING("トロン"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    IF_INT_GREATER(LW(10), -200)
        SET_VALUE(LW(0), -260)
    END_IF
    SET_VALUE(LW(1), 2)
#endif
    CALL(evt_npc_jump_position_nohit, STRING("トロン"), LW(0), LW(1), LW(2), 300, 20)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("トロン"), STRING("B_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("トロン"), STRING("B_2"))
    CALL(evt_npc_set_autotalkpose, STRING("トロン"), STRING("B_2"), STRING("T_2"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -200)
        LOOP(10)
            CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
            ADD(LW(1), 1)
            CALL(evt_npc_set_position, STRING("トロン"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_IF
    CALL(evt_snd_bgmoff, PSND_FADEOUT)

    // Toron jumps out and taunts Mario.
    // Toron jumps out and taunts Mario.
    CALL(evt_msg_print, 0, STRING("stg6_rsh_181"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_npc_pera_onoff, STRING("トロン"), 1)
    CALL(evt_npc_set_rotate, STRING("トロン"), 0, 0, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), NPC_FLAG_NO_HIT)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_25_ZIP_TOAD_APPEARS)
    CALL(evt_del_case_evt, 0, GW(11))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_EGG1"))
    SPAWN_EVENT(PTR(evt_toron_catch))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Trigger on the paper spot (a_kami) between STG6_24_A_VITAL_CLUE and the next clue:
 * Mario finds the paper (stg6_rsh_179), the partner comments (stg6_rsh_180)
 * and the story advances to STG6_24_02_ANOTHER_CLUE.
 */
static EVT_BEGIN(evt_daiji_kami)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_DOUBTFUL_POINT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_179"), 0, 0)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(1), LW(2), LW(3))
    IF_INT_LESS(LW(0), LW(1))
        CALL(evt_mario_set_dir, 90, 300, 0)
    ELSE
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_IF
#if VERSION == VERSION_EU
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg6_rsh_180"))
    CALL(evt_party_run, 0)
#else
    CALL(evt_msg_print_party, STRING("stg6_rsh_180"))
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_24_02_ANOTHER_CLUE)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Trigger on the stained carpet spot (a_yuka_simi) on day 1:
 * Mario comments (stg6_rsh_51), the partner too the first time (stg6_rsh_52),
 * and the story advances to STG6_03_STAINED_CARPET.
 */
static EVT_BEGIN(evt_yuka_simi)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_DOUBTFUL_POINT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_51"), 0, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_03_STAINED_CARPET)
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING("stg6_rsh_52"))
        CALL(evt_party_run, 0)
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG6_03_STAINED_CARPET)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Trigger script for the shelves: prints the message in LW(1). */
static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access2)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_RSH_02_DRIED_SHROOM_DRAWER, 0)
        SET_VALUE(GSWF_RSH_02_DRIED_SHROOM_DRAWER, 1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_266"), 0, 0)
        CALL(evt_item_entry, STRING("a"), ITEM_DRIED_SHROOM, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_item_get_item, STRING("a"))
        CALL(evt_item_get_item_end_wait, STRING("a"))
    ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_267"), 0, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Registers the shelf triggers (tana_3, tana_4, tana_5) with evt_access or evt_access2,
 * depending on the version and the story stage.
 */
static EVT_BEGIN(evt_access_entry)
#if VERSION > VERSION_DEMO_US
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_56_ROGUEPORT_STATION)
#endif
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_4"), 0, PTR(evt_access), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_5"), 0, PTR(evt_access2), 0)
#if VERSION > VERSION_DEMO_US
        ELSE
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_3"), 0, PTR(evt_access), 0)
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_4"), 0, PTR(evt_access), 0)
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_5"), 0, PTR(evt_access), 0)
        SET_VALUE(GSWF_RSH_02_DRIED_SHROOM_DRAWER, 1)
    END_IF
#endif
    RETURN
EVT_END()

/**
 * Shared map init for the three variants of rsh_02 on the first trip:
 * registers the three cabin doors (3 to 5), the NPCs, the pot shelf trigger (tana_3),
 * the bed (from STG6_01_THE_CONDUCTOR on), the shelf signs, and the music for the story stage.
 */
EVT_BEGIN(rsh_02_init_evt)
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
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("tana_3"), 0, PTR(evt_nabe), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_01_THE_CONDUCTOR)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG6_25_ZIP_TOAD_APPEARS)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("bed"), 0, PTR(evt_bed), GW(11))
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_access_entry)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF_STARPIECE_82)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -310, 0, -130, 0, GSWF_SHINE_34, 0)
#else
    CALL_CHILD(PTR(evt_access_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF_STARPIECE_82)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -317, 0, -127, 0, GSWF_SHINE_34, 0)
#endif
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -365, 60, -175, 0, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_00_START)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(STG6_02_A_CASE_OF_LOST_STEW, STG6_04_GRILLING_HEFF_T)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_22_DOCUMENT, STG6_24_02_ANOTHER_CLUE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(STG6_25_ZIP_TOAD_APPEARS, STG6_25_ZIP_TOAD_APPEARS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_EGG1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1_3"))
        CASE_EQUAL(STG6_26_ZIP_TOAD_CAUGHT)
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
        CASE_BETWEEN(STG6_49_00_TRAIN_ARRIVAL, STG6_55_04_BOWSER_RAWK_HAWK)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_02_b"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_02_c"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of rsh_02_a (first visit): runs the shared init and then, depending on the story,
 * plays the arrival scenes after sleeping (day 2, day 3 and the Poshley Heights morning messages,
 * with a partner line on day 3), the first-time intro dialogue at STG6_00_START,
 * the stained-floor trigger for the carpet clue, the Vital Paper clue, Toron's appearance,
 * and the Smorg window effect.
 */
EVT_BEGIN(rsh_02_a_init_evt)
    // Decide whether a scripted entry is pending, then run the shared car init.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_20_END_OF_DAY_1)
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_44_END_OF_DAY_2)
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), GW_FLAG_NO_ENTRANCE)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_00_START)
            SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 1)
        END_IF
    END_IF
    CALL_CHILD(PTR(rsh_02_init_evt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_01_THE_CONDUCTOR, STG6_20_END_OF_DAY_1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("s_yuka_simi"), MAP_OBJ_FLAG_HIDDEN)
        IF_INT_GREATER(GSW_STORY_PROGRESS, STG6_01_THE_CONDUCTOR)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG6_05_GALLEY_KETTLE_DISCOVERED)
                CALL(evt_run_case_evt, CASE_STAND_ON_A, 1, STRING("a_yuka_simi"), 0, PTR(evt_yuka_simi), GW(10))
            END_IF
        END_IF
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("s_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_yuka_simi"), MAP_OBJ_FLAG_HIDDEN)
    END_SWITCH
    IF_INT_EQUAL(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    ELSE
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -240, 0, -175)
        CALL(evt_party_set_pos, 0, -200, 0, -175)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(STG6_00_START)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_00_START)
            CALL(evt_cam3d_evt_set, -195, 102, 113, -195, 50, -183, 0, EASING_SINE_IN_OUT)
            CALL(evt_mario_set_pos, -175, 0, -225)
            CALL(evt_party_set_pos, 0, -215, 0, -225)
            CALL(evt_mario_set_dir, 270, 0, 0)
            CALL(evt_party_set_dir, 0, 90, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                CALL(evt_seq_wait, SEQ_GAME)
                CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 5000)
                WAIT_MS(3000)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_13"), 0, 0)
                WAIT_MS(2000)

                // The first-time intro dialogue at the start of the chapter.
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_01"))
                WAIT_MS(300)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_2"))
                WAIT_MS(600)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_party_set_dir, 0, 270, 300)
                WAIT_MS(300)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1500)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_cam3d_evt_set, -220, 127, 285, -220, 45, -183, 500, EASING_SINE_IN_OUT)
                WAIT_MS(500)
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_02"))
                WAIT_MS(300)
                CALL(evt_party_move_pos2, 0, -225, -140, FLOAT(80.0f))
                WAIT_MS(300)
                CALL(evt_party_set_dir, 0, 90, 300)
                WAIT_MS(300)
                CALL(evt_cam3d_evt_set, -220, 109, 211, -220, 40, -183, 500, EASING_SINE_IN_OUT)
                CALL(evt_mario_mov_pos2, -190, -130, FLOAT(80.0f))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                ADD(LW(0), 10)
                ADD(LW(2), 5)

                // The Vital Paper clue.
                CALL(evt_item_entry, STRING("letter"), ITEM_VITAL_PAPER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_pro_03"), 0, 0)
                WAIT_MS(300)
                CALL(evt_item_delete, STRING("letter"))
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_5B"))
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1500)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_04"))
                WAIT_MS(300)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_2"))
                WAIT_MS(600)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
                CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT

            // The morning messages after sleeping (day 2, day 3 and Poshley Heights).
            CASE_LESS(STG6_21_DAY_2)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_21_DAY_2)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                WAIT_MS(2000)
                CALL(evt_msg_print, 0, STRING("stg6_yado_03"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT
            CASE_LESS(STG6_45_DAY_3_STARTS)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_45_DAY_3_STARTS)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_msg_print, 0, STRING("stg6_yado_04"), 0, 0)
                CALL(evt_mario_get_party, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(1)
                    // On day 3 the partner speaks.
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_0"), 0, STRING("party"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_1"), 0, STRING("party"))
                    CASE_EQUAL(4)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_2"), 0, STRING("party"))
                    CASE_EQUAL(5)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_3"), 0, STRING("party"))
                    CASE_EQUAL(6)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_4"), 0, STRING("party"))
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_5"), 0, STRING("party"))
                    CASE_EQUAL(7)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_230_6"), 0, STRING("party"))
                END_SWITCH
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT
            CASE_LESS(STG6_49_POSHLEY_HEIGHTS)
            SET_VALUE(GSW_STORY_PROGRESS, STG6_49_POSHLEY_HEIGHTS)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_259"), 0, 0)
                WAIT_MS(300)
                CALL(evt_msg_print_party, STRING("stg6_rsh_259_01"))
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT
        END_SWITCH
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_24_A_VITAL_CLUE, STG6_24_02_ANOTHER_CLUE)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG6_24_02_ANOTHER_CLUE)
            CALL(evt_run_case_evt, CASE_STAND_ON_A, 1, STRING("a_kami"), 0, PTR(evt_daiji_kami), 0)
        END_IF
        SPAWN_EVENT(PTR(evt_toron_appear))
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
    END_SWITCH
#if VERSION >= VERSION_US
    SPAWN_EVENT(evt_sub_mail_event)
#endif
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    RETURN
EVT_END()

/**
 * Init script of rsh_02_b (first visit): runs the shared init;
 * after the Zip Toad is caught it spawns evt_toron_catch2 for the post-catch scene.
 */
EVT_BEGIN(rsh_02_b_init_evt)
    CALL_CHILD(PTR(rsh_02_init_evt))
    IF_INT_EQUAL(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    ELSE
        SET_VALUE(GSWF_RSH_EVENT_ENTRY_PENDING, 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_set_pos, -170, 0, -130)
        CALL(evt_cam_road_reset)
        SPAWN_EVENT(PTR(evt_toron_catch2))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_touchaku"))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -205, 0, -185)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_party_set_breed_pose, 0, 1)
    RETURN
EVT_END()

/** Map objects of the scenery that passes by the windows of the three cabins. */
static const char* window_tbl[] = {
    "mado_3_uturu",
    "mado_4_uturu",
    "mado_5_uturu",
    NULL,
};

/** Init script of rsh_02_c (first visit): sets the doors, runs the shared init and the window scenery. */
EVT_BEGIN(rsh_02_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_02_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    CALL(evt_map_playanim, STRING("shouten"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("shouten"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_hassya"))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -205, 0, -185)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_seq_wait, SEQ_GAME)
            WAIT_MS(1000)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_216"), 0, 0)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

/**
 * Shared map init for rsh_02 on the second visit:
 * plays the outbound or return-trip scenery, registers the doors, bed (evt_nidome_bed), NPCs,
 * shelf signs and the music for the station the train has reached.
 */
EVT_BEGIN(rsh_02_init_evt_nidome)
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
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_EQUAL(GSWF_RSH_NEW_PASSENGERS_PENDING, 1)
        SET_VALUE(GSWF_RSH_NEW_PASSENGERS_PENDING, 0)
        CALL_CHILD(PTR(evt_rsh_rnd_npc_init))
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF_STARPIECE_82)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -310, 0, -130, 0, GSWF_SHINE_34, 0)
#else
    CALL_CHILD(PTR(evt_access_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF_STARPIECE_82)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -317, 0, -127, 0, GSWF_SHINE_34, 0)
#endif
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -365, 60, -175, 0, 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("pik_touchaku"))
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    ELSE
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
                IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
                    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH1"))
                    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
                END_IF
                IF_STR_EQUAL(LW(0), STRING("rsh_02_b"))
                END_IF
                IF_STR_EQUAL(LW(0), STRING("rsh_02_c"))
                    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH2"))
                    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("bed"), 0, PTR(evt_nidome_bed), 0)
    RETURN
EVT_END()

/**
 * Init script of rsh_02_a on the second visit: runs rsh_02_init_evt_nidome, shows the clean floors and,
 * when the map was entered by waking up in the cabin (nidome, nidome_start) or at a station arrival,
 * places Mario and the partners inside cabin 5 with only that cabin shown.
 * Waking up on the way sends the train on to Poshley Heights (pik_00)
 * or Rogueport (gor_04) depending on the direction of the trip; otherwise it uses the normal doors.
 */
EVT_BEGIN(rsh_02_a_init_evt_nidome)
    // Second visit: run the shared init and show the clean floors.
    CALL_CHILD(PTR(rsh_02_init_evt_nidome))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("s_yuka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_yuka_simi"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yuka_5"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LF(0), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome_start"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("nidome"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_touchaku"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_touchaku"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -215, 0, -185)
        CALL(evt_party_set_pos, 1, -230, 0, -195)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dir, 1, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yuka_5_kami_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("nidome"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                    // Waking up on the way sends the train on to Poshley Heights or Rogueport.
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_03"), 0, 0)
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_05"), 0, 0)
                END_IF
                WAIT_MS(2000)
                IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
                    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)
                    CALL(evt_bero_mapchange, STRING("pik_00"), STRING("resha"))
                ELSE
                    CALL(evt_bero_mapchange, STRING("gor_04"), STRING("resha_return"))
                END_IF
            END_INLINE_EVENT
        END_IF
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("pik_touchaku"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_04"), 0, 0)
                WAIT_MS(300)
                SET_VALUE(GSWF_RSH_TRAIN_AT_POSHLEY, 1)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_run, 0)
            END_INLINE_EVENT
        END_IF
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("gor_touchaku"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(300)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_run, 0)
            END_INLINE_EVENT
        END_IF
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)

        // Otherwise the normal doors are used.
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    RETURN
EVT_END()

/**
 * Init script of rsh_02_b on the second visit: runs rsh_02_init_evt_nidome and,
 * when entered by waking up (nidome) or arriving from Riverside (hom_touchaku),
 * places Mario and the partners inside cabin 5 with only that cabin shown; otherwise uses the normal doors.
 */
EVT_BEGIN(rsh_02_b_init_evt_nidome)
    CALL_CHILD(PTR(rsh_02_init_evt_nidome))
    SET_VALUE(LF(0), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_touchaku"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -215, 0, -185)
        CALL(evt_party_set_pos, 1, -230, 0, -195)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dir, 1, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("nidome"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
#if VERSION > VERSION_DEMO_US
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_RSH3"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH1"))
#endif
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_01"), 0, 0)
                WAIT_MS(300)
                CALL(evt_bero_mapchange, STRING("hom_00"), STRING("nidome"))
            END_INLINE_EVENT
        ELSE
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_c_02"), 0, 0)
                WAIT_MS(300)
                CALL(evt_bero_mapchange, STRING("hom_00"), STRING("n_bero_1"))
            END_INLINE_EVENT
        END_IF
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    RETURN
EVT_END()

/** Init script of rsh_02_c on the second visit: doors, the shared second-visit init and the window scenery. */
EVT_BEGIN(rsh_02_c_init_evt_nidome)
    CALL_CHILD(PTR(rsh_02_init_evt_nidome))
    CALL(evt_map_playanim, STRING("ranpu"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("shouten"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LF(0), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome_start"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -215, 0, -185)
        CALL(evt_party_set_pos, 1, -230, 0, -195)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dir, 1, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("heya_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("haikei_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("heya_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("haikei_5"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("rouka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("tobira_5"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    RETURN
EVT_END()

/**
 * Bed on the second trip. At a station (Poshley Heights or Rogueport) it is the normal inn bed.
 * While the train is running, asking to sleep fades out,
 * restores HP/FP and changes the map to Riverside Station (hom_10, hom_11 or hom_12),
 * setting GSW_TRAIN_LEG to the next leg of the trip.
 */
EVT_BEGIN(evt_nidome_bed)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_POSHLEY, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL_CHILD(PTR(evt_normal_yado))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_HOM_TRAIN_AT_ROGUEPORT, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL_CHILD(PTR(evt_normal_yado))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_yado_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_02_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_fade_set_mapchange_type, 1, 4, 3000, -1, -1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    IF_INT_EQUAL(GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE, 1)
        IF_INT_EQUAL(GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT, 0)
            SET_VALUE(GSW_HOM_TRAIN_LEG, 1)
        ELSE
            SET_VALUE(GSW_HOM_TRAIN_LEG, 3)
        END_IF
        CALL(evt_bero_mapchange, STRING("hom_10"), 0)
        RETURN
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
        CALL(evt_bero_mapchange, STRING("hom_11"), 0)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_02_c"))
        CALL(evt_bero_mapchange, STRING("hom_12"), 0)
    END_IF
    RETURN
EVT_END()

