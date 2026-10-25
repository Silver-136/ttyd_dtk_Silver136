#include "battle/battle_unit.h"
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

s32 marioRollCancelDisable(void);
s32 U_marioSlitContinueChk(void);
NpcEntry* fbatHitCheckAll(void);

USER_FUNC(bgm_wait_stop);
USER_FUNC(evt_cam_shift_reset);
USER_FUNC(evt_fade_set_spot_pos);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_item_end_wait);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_party_use_check);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_npc_blur_onoff);
USER_FUNC(evt_party_move_behind_mario);
USER_FUNC(evt_party_set_breed_pose);
USER_FUNC(evt_party_yoshi_ride);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_win_item_select);
USER_FUNC(toron_check);
USER_FUNC(mario_normalize_wait);
#ifdef VERSION_G8ME01
#endif
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
EvtWord talk_yuurei[451];
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
EvtWord evt_toron_catch[281];
static s32 item_tbl[2];
EvtWord evt_toron_catch2[359];
#ifdef VERSION_G8ME01
EvtWord evt_toron_appear[437];
#else
EvtWord evt_toron_appear[414];
#endif
static EvtWord evt_daiji_kami[88];
static EvtWord evt_yuka_simi[45];
static EvtWord evt_access[14];
static EvtWord evt_access2[44];
static EvtWord evt_access_entry[62];
EvtWord rsh_02_init_evt[247];
#ifdef VERSION_G8ME01
EvtWord rsh_02_a_init_evt[687];
#else
EvtWord rsh_02_a_init_evt[685];
#endif
EvtWord rsh_02_b_init_evt[168];
static const char* window_tbl[4];
EvtWord rsh_02_c_init_evt[142];
EvtWord rsh_02_init_evt_nidome[208];
EvtWord rsh_02_a_init_evt_nidome[337];
EvtWord rsh_02_b_init_evt_nidome[213];
EvtWord rsh_02_c_init_evt_nidome[136];
EvtWord evt_nidome_bed[126];

USER_FUNC(mario_normalize_wait) {
    if (!marioRollCancelDisable() && !U_marioSlitContinueChk()) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

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
        20000, // directionId
        100000, // centerPosition
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
        20000, // directionId
        100000, // centerPosition
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
        20000, // directionId
        100000, // centerPosition
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

EVT_BEGIN(evt_powan_msg)
    CALL(evt_npc_set_anim, STRING("ポワン探偵"), STRING("A_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("hatena"), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_set_dir_to_target, STRING("グルメキノピオ"), STRING("mario"))
    RETURN
EVT_END()

EVT_BEGIN(evt_nabe)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 286)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 287)
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
    CALL(evt_item_entry, STRING("nabe"), 74, 0, 0, 0, 17, -1, 0)
    CALL(evt_item_get_item, STRING("nabe"))
    CALL(evt_item_get_item_end_wait, STRING("nabe"))
    CALL(evt_mario_set_dir_npc, STRING("グルメキノピオ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_57"), 0, STRING("グルメキノピオ"))
    CALL(evt_fade_entry, 10, 1000, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_mario_set_pos, 205, 0, -175)
    CALL(evt_party_set_pos, 0, 180, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 265, 0, -175)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 131088)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("グルメキノピオ"))
    CALL(evt_set_dir_to_target, STRING("グルメキノピオ"), STRING("ポワン探偵"))
    CALL(evt_cam3d_evt_zoom_in, 25, 0, 20, 11)
    CALL(evt_fade_entry, 9, 1000, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_58"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_59"), 0, STRING("グルメキノピオ"))
    WAIT_MS(500)
    SET_VALUE(LW(10), 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_60_00"), PTR(powan_callback), STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), 2)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 250, -150, 0, FLOAT(40.0f), 1)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 251, -90, 0, FLOAT(40.0f), 1)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_60_01"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), 131088)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 252, 0, 0, FLOAT(40.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ポワン探偵"), 131088)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(LSWF(3), 0)
    SET_VALUE(GSW_STORY_PROGRESS, 287)
    CALL(evt_del_case_evt, 0, GW(10))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_gurume)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(310, 325)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_BETWEEN(327, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_gurume)
    SWITCH(GSW_STORY_PROGRESS)
#ifdef VERSION_G8ME01
        CASE_LESS(284)
#else
        CASE_LESS(283)
#endif
        CALL(evt_msg_print, 0, STRING("stg6_rsh_12"), 0, 0)
        CASE_LESS(285)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_54_00"), 0, 0)
        CASE_LESS(287)
        SET_VALUE(GSW_STORY_PROGRESS, 286)
        IF_INT_EQUAL(LSWF(3), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_54_01"), 0, 0)
            SET_VALUE(LSWF(3), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_55"), 0, 0)
            SET_VALUE(LSWF(3), 0)
        END_IF
        CASE_LESS(295)
        IF_INT_EQUAL(LSWF(3), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_63"), 0, 0)
            SET_VALUE(LSWF(3), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_64"), 0, 0)
            SET_VALUE(LSWF(3), 0)
        END_IF
        CASE_LESS(296)
        IF_INT_EQUAL(GSWF(3434), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_117"), 0, 0)
            SET_VALUE(GSWF(3434), 1)
            ELSE
            IF_INT_EQUAL(GSWF(3435), 1)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_117_01"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg6_rsh_117"), 0, 0)
            END_IF
        END_IF
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_117_02"), 0, 0)
        CASE_LESS(303)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_04"), 0, 0)
        CASE_LESS(310)
        IF_INT_EQUAL(LSWF(4), 0)
            SET_VALUE(LSWF(4), 1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_161"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_162"), 0, 0)
        END_IF
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_222"), 0, 0)
        CASE_LESS(346)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_258"), 0, 0)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_powan)
    RETURN
EVT_END()

EVT_BEGIN(init_yuurei)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(295)
        RETURN
        CASE_BETWEEN(295, 302)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(304, 309)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(310, 326)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
        CASE_BETWEEN(327, 332)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_GREATER_OR_EQUAL(334)
        CALL(evt_npc_set_position, STRING("me"), 30, 0, -170)
    END_SWITCH
    CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ゆうれい"), 262176)
    CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, 0)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(GSWF(3445), 0)
            SET_VALUE(LW(0), 0)
            LOOP(0)
                IF_INT_EQUAL(GSWF(3459), 1)
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
            CALL(evt_snd_bgmoff, 2048)
            CALL(evt_npc_get_position, STRING("ゆうれい"), LW(3), LW(4), LW(5))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_GHOST_APPEAR1"), LW(3), LW(4), LW(5), 0)
            CALL(evt_sub_intpl_msec_init, 11, 0, 180, 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, PTR(talk_yuurei))
            SET_VALUE(GSWF(3445), 1)
            ELSE
            CALL(evt_npc_set_position, STRING("ゆうれい"), 30, 0, -175)
            CALL(evt_npc_change_interrupt, STRING("ゆうれい"), 6, PTR(talk_yuurei))
        END_IF
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, 11, 180, 64, 1000)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, 11, 64, 180, 1000)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
            IF_INT_EQUAL(GSWF(3448), 1)
                SET_VALUE(GSWF(3449), 1)
                LOOP(0)
                    IF_INT_EQUAL(GSWF(3448), 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                SET_VALUE(GSWF(3449), 0)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(evt_shake)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SURPRISED1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_STG5_GHOST1"))
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_cam3d_evt_zoom_in, 10, 0, 0, 11)
        WAIT_FRAMES(2)
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_shake2)
    CALL(evt_cam3d_evt_off, 0, 11)
    RETURN
EVT_END()

static s32 item_tbl2[] = {
    ITEM_RAGGED_DIARY, -1,
};

EVT_BEGIN(talk_yuurei)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_STG5_GHOST1"))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(297)
        SET_VALUE(LW(13), 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, 296)
            SET_VALUE(GSW_STORY_PROGRESS, 296)
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
            SET_VALUE(GSW_STORY_PROGRESS, 297)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg6_rsh_121"))
        END_IF
        CASE_LESS(299)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_122"), 0, 0)
        CASE_LESS(300)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_135"), 0, 0)
        CALL(evt_win_item_select, 0, PTR(item_tbl2), LW(1), 0)
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_136"), 0, 0)
            CALL(evt_npc_set_anim, STRING("ゆうれい"), STRING("A_1"))
            WAIT_MS(1000)
            CALL(evt_npc_set_anim, STRING("ゆうれい"), STRING("S_1"))
            CALL(evt_item_entry, STRING("mofu"), 79, 0, 0, 0, 17, -1, 0)
            CALL(evt_item_flag_on, STRING("mofu"), 8)
            CALL(evt_snd_bgmoff, 513)
            CALL(evt_item_get_item, STRING("mofu"))
            CALL(evt_item_get_item_end_wait, STRING("mofu"))
            CALL(evt_snd_bgmoff, 1024)
            CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_STG5_GHOST1"))
            CALL(evt_msg_print, 0, STRING("stg6_rsh_138"), 0, 0)
            SET_VALUE(GSWF(3448), 1)
            LOOP(0)
                IF_INT_EQUAL(GSWF(3449), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("shouten"), 1)
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
                CALL(evt_sub_intpl_msec_init, 0, 180, 60, 5000)
                LOOP(0)
                    CALL(evt_sub_intpl_msec_get_value)
                    CALL(evt_npc_set_color, STRING("ゆうれい"), 255, 255, 255, LW(0))
                    WAIT_FRAMES(1)
                    IF_INT_EQUAL(LW(1), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, 0, LW(8), 80, 5000)
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
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("shouten"), 1)
            CALL(evt_snd_bgmoff, 513)
            CALL(evt_snd_bgmon, 288, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_sub_intpl_msec_init, 0, 60, 180, 1000)
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
            CALL(evt_sub_intpl_msec_init, 12, 80, LW(8), 1000)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_npc_set_position, STRING("ゆうれい"), LW(7), LW(0), LW(9))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            SET_VALUE(GSWF(3448), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_139"), 0, 0)
            CALL(evt_pouch_remove_item, 78, LW(0))
            SET_VALUE(GSW_STORY_PROGRESS, 300)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg6_rsh_140"), 0, 0)
        END_IF
        CASE_LESS(301)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_141"), 0, 0)
        CASE_LESS(302)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_147_05"), 0, 0)
        CASE_LESS(306)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_178_00"), 0, 0)
        CASE_LESS(310)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_178"), 0, 0)
        CASE_LESS(326)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_210"), 0, 0)
        CASE_LESS(327)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_223"), 0, 0)
        CASE_LESS(346)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_255"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_rsh_c_10"), 0, 0)
    END_SWITCH
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    RETURN
EVT_END()

USER_FUNC(toron_check) {
    NpcEntry* hit = fbatHitCheckAll();
    // トロン
    NpcEntry* npc = npcNameToPtr("\x83\x67\x83\x8D\x83\x93");
    BOOL busy = FALSE;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        busy = TRUE;
    }
    if ((u32)marioStGetSystemLevel() != 0) {
        busy = TRUE;
    }
    if (npc == hit && !busy) {
        evtSetValue(event, LW(10), 1);
    } else {
        evtSetValue(event, LW(10), 0);
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(init_toron)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(308)
        CALL(evt_npc_set_position, STRING("me"), -240, 0, -180)
        SPAWN_EVENT(PTR(evt_toron_catch))
        CASE_EQUAL(309)
        CALL(evt_npc_set_anim, STRING("トロン"), STRING("D_1"))
        CALL(evt_npc_set_autotalkpose, STRING("トロン"), STRING("D_1"), STRING("T_5"))
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), 131072)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_toron)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_186"), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(talk_sarary)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_192"), 0, 0)
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"グルメキノピオ"*/"\x83\x4F\x83\x8B\x83\x81\x83\x4C\x83\x6D\x83\x73\x83\x49", // name
        0x40020610, // flags
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
        /*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", // name
        0x40000600, // flags
        0, NULL, NULL, // reactionFlags..regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ゆうれい"*/"\x82\xE4\x82\xA4\x82\xEA\x82\xA2", // name
        0x40000600, // flags
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
        /*"トロン"*/"\x83\x67\x83\x8D\x83\x93", // name
        0x40000608, // flags
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
        /*"サラリーマン"*/"\x83\x54\x83\x89\x83\x8A\x81\x5B\x83\x7D\x83\x93", // name
        0x40000600, // flags
        0, NULL, NULL, // reactionFlags..regularEvent
        talk_sarary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
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

EVT_BEGIN(evt_1day_end)
    CALL(evt_msg_print, 0, STRING("stg6_yado_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_02_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, 302)
    CALL(evt_snd_bgmon, 512, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, 1024)
    SET_VALUE(GSWF(3444), 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 3000, 9, 300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    SET_VALUE(GSWF(3445), 0)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("e_bero"))
    RETURN
EVT_END()

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
    psndSetFlag(0x1000);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_2day_end)
    CALL(evt_msg_print, 0, STRING("stg6_yado_02"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_02_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, 327)
    CALL(evt_snd_bgmon, 512, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, 1024)
    SET_VALUE(GSWF(3444), 1)
    CALL(evt_fade_set_mapchange_type, 0, 4, 3000, 9, 300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    SET_VALUE(GSWF(3445), 0)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("e_bero"))
    RETURN
EVT_END()

EVT_BEGIN(evt_normal_yado)
    CALL(evt_msg_print, 0, STRING("stg6_yado_00"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_yado_00_yn"))
    CALL(evt_msg_continue)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, 1024)
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
    CALL(evt_fade_entry, 9, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_print, 0, STRING("stg6_yado_01"), 0, 0)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_bed)
    CALL(evt_mario_key_onoff, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(301)
        CALL_CHILD(PTR(evt_1day_end))
        CASE_EQUAL(326)
        CALL_CHILD(PTR(evt_2day_end))
        CASE_DEFAULT
        CALL_CHILD(PTR(evt_normal_yado))
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_toron_catch)
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
            CALL(evt_mario_key_onoff, 0)
            BEGIN_INLINE_EVENT
                CALL(mario_normalize_wait)
                CALL(evt_mario_normalize)
                CALL(evt_party_yoshi_ride, 0, 0)
            END_INLINE_EVENT
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
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), 32)
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), 1024)
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
    CALL(evt_msg_print, 0, STRING("stg6_rsh_182"), 0, STRING("トロン"))
    CALL(evt_snd_bgmoff, 512)
    SET_VALUE(GSWF(3444), 1)
    CALL(evt_fade_set_mapchange_type, 0, 10, 2000, 9, 2000)
    SET_VALUE(GSW_STORY_PROGRESS, 309)
    CALL(evt_bero_mapchange, STRING("rsh_02_b"), STRING("w_bero"))
    RETURN
EVT_END()

static s32 item_tbl[] = {
    ITEM_BRIEFCASE, -1,
};

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
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 131088)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), 131088)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("サラリーマン"), 32768)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), 32768)
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), 32768)
    CALL(evt_cam3d_evt_set, -215, 140, 200, -215, 40, -180, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_npc_set_anim, STRING("トロン"), STRING("D_1"))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_183"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_184"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_185"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_186"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_mario_mov_pos2, -210, -150, FLOAT(60.0f))
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("a"), 81, 0, 0, 0, 17, -1, 0)
    CALL(evt_item_get_item, STRING("a"))
    CALL(evt_item_get_item_end_wait, STRING("a"))
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("b"), 75, 0, 0, 0, 17, -1, 0)
    CALL(evt_item_get_item, STRING("b"))
    CALL(evt_item_get_item_end_wait, STRING("b"))
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("c"), 76, 0, 0, 0, 17, -1, 0)
    CALL(evt_item_get_item, STRING("c"))
    CALL(evt_item_get_item_end_wait, STRING("c"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_188"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("サラリーマン"))
    LOOP(0)
        CALL(evt_win_item_select, 0, PTR(item_tbl), LW(1), 0)
        IF_INT_NOT_EQUAL(LW(1), -1)
            BREAK_LOOP
        END_IF
        CALL(evt_msg_print, 0, STRING("stg6_rsh_190"), 0, STRING("サラリーマン"))
    END_LOOP
    CALL(evt_pouch_remove_item, 81, LW(0))
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(evt_mario_mov_pos2, -225, -130, FLOAT(60.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    ADD(LW(1), 20)
    CALL(evt_item_entry, STRING("kaban"), 81, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_189_00"), 0, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_item_delete, STRING("kaban"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_189_01"), 0, STRING("サラリーマン"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_191"), 0, STRING("ポワン探偵"))
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), 8)
    CALL(evt_pouch_remove_item, 80, LW(0))
    SET_VALUE(GSW_STORY_PROGRESS, 309)
    WAIT_MS(750)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_194"), 0, 0)
    WAIT_MS(500)
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("hom_00"), STRING("hom_touchaku"))
    RETURN
EVT_END()

EVT_BEGIN(evt_toron_appear)
    LABEL(100)
    CALL(evt_npc_set_position, STRING("トロン"), 0, -1000, 0)
    CALL(evt_npc_set_rotate, STRING("トロン"), 0, 0, 0)
    CALL(evt_npc_set_ry, STRING("トロン"), 270)
    CALL(evt_npc_pera_onoff, STRING("トロン"), 1)
    WAIT_FRAMES(1)
    IF_INT_LESS(GSW_STORY_PROGRESS, 307)
        GOTO(100)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), -110)
        GOTO(100)
    END_IF
    IF_INT_LESS(LW(0), -330)
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
    CALL(evt_npc_flag_onoff, 1, STRING("トロン"), 32)
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
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_ry, STRING("トロン"), 90)
#endif
        CALL(evt_npc_get_position, STRING("トロン"), LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
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
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_181"), 0, STRING("トロン"))
    WAIT_MS(500)
    CALL(evt_npc_pera_onoff, STRING("トロン"), 1)
    CALL(evt_npc_set_rotate, STRING("トロン"), 0, 0, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("トロン"), 32)
    SET_VALUE(GSW_STORY_PROGRESS, 308)
    CALL(evt_del_case_evt, 0, GW(11))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG3_EGG1"))
    SPAWN_EVENT(PTR(evt_toron_catch))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

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
    CALL(evt_msg_print_party, STRING("stg6_rsh_180"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 307)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(evt_yuka_simi)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_DOUBTFUL_POINT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_51"), 0, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 285)
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING("stg6_rsh_52"))
        CALL(evt_party_run, 0)
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, 285)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, LW(1), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access2)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(3462), 0)
        SET_VALUE(GSWF(3462), 1)
        CALL(evt_msg_print, 0, STRING("stg6_rsh_266"), 0, 0)
        CALL(evt_item_entry, STRING("a"), 152, 0, -1000, 0, 17, -1, 0)
        CALL(evt_item_get_item, STRING("a"))
        CALL(evt_item_get_item_end_wait, STRING("a"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg6_rsh_267"), 0, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_access_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, 345)
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, 9, 1, STRING("tana_4"), 0, PTR(evt_access), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("tana_5"), 0, PTR(evt_access2), 0)
        ELSE
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, 9, 1, STRING("tana_3"), 0, PTR(evt_access), 0)
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, 9, 1, STRING("tana_4"), 0, PTR(evt_access), 0)
        SET_VALUE(LW(1), STRING("stg6_rsh_267"))
        CALL(evt_run_case_evt, 9, 1, STRING("tana_5"), 0, PTR(evt_access), 0)
        SET_VALUE(GSWF(3462), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_02_init_evt)
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
    CALL(evt_run_case_evt, 9, 1, STRING("tana_3"), 0, PTR(evt_nabe), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 283)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 308)
            CALL(evt_run_case_evt, 9, 1, STRING("bed"), 0, PTR(evt_bed), GW(11))
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_access_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF(5649))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -317, 0, -127, 0, GSWF(5559), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -365, 60, -175, 0, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(282)
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        CASE_BETWEEN(284, 286)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(304, 307)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_POWAN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_BETWEEN(308, 308)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG3_EGG1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        CASE_EQUAL(309)
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
        CASE_BETWEEN(333, 344)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_02_b"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH3"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("rsh_02_c"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(rsh_02_a_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 302)
        SET_VALUE(GSWF(3444), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 327)
        SET_VALUE(GSWF(3444), 1)
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), 2)
        IF_INT_LESS(GSW_STORY_PROGRESS, 282)
            SET_VALUE(GSWF(3444), 1)
        END_IF
    END_IF
    CALL_CHILD(PTR(rsh_02_init_evt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(283, 302)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_yuka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_yuka_simi"), 1)
        IF_INT_GREATER(GSW_STORY_PROGRESS, 283)
            IF_INT_LESS(GSW_STORY_PROGRESS, 287)
                CALL(evt_run_case_evt, 5, 1, STRING("a_yuka_simi"), 0, PTR(evt_yuka_simi), GW(10))
            END_IF
        END_IF
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_yuka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_yuka_simi"), 1)
    END_SWITCH
    IF_INT_EQUAL(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        SET_VALUE(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_a))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mario_set_pos, -240, 0, -175)
        CALL(evt_party_set_pos, 0, -200, 0, -175)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(282)
            SET_VALUE(GSW_STORY_PROGRESS, 282)
            CALL(evt_cam3d_evt_set, -195, 102, 113, -195, 50, -183, 0, 11)
            CALL(evt_mario_set_pos, -175, 0, -225)
            CALL(evt_party_set_pos, 0, -215, 0, -225)
            CALL(evt_mario_set_dir, 270, 0, 0)
            CALL(evt_party_set_dir, 0, 90, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                CALL(evt_seq_wait, 2)
                CALL(evt_snd_bgmoff_f, 512, 5000)
                WAIT_MS(3000)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_13"), 0, 0)
                WAIT_MS(2000)
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_01"))
                WAIT_MS(300)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_2"))
                WAIT_MS(600)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_party_set_dir, 0, 270, 300)
                WAIT_MS(300)
                CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1500)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5_kami_2"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5"), 1)
                CALL(evt_cam3d_evt_set, -220, 127, 285, -220, 45, -183, 500, 11)
                WAIT_MS(500)
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_02"))
                WAIT_MS(300)
                CALL(evt_party_move_pos2, 0, -225, -140, FLOAT(80.0f))
                WAIT_MS(300)
                CALL(evt_party_set_dir, 0, 90, 300)
                WAIT_MS(300)
                CALL(evt_cam3d_evt_set, -220, 109, 211, -220, 40, -183, 500, 11)
                CALL(evt_mario_mov_pos2, -190, -130, FLOAT(80.0f))
                CALL(evt_party_stop, 0)
                WAIT_MS(300)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5"), 1)
                CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                ADD(LW(0), 10)
                ADD(LW(2), 5)
                CALL(evt_item_entry, STRING("letter"), 80, LW(0), LW(1), LW(2), 17, -1, 0)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("stg6_rsh_pro_03"), 0, 0)
                WAIT_MS(300)
                CALL(evt_item_delete, STRING("letter"))
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_5B"))
                CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1500)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print_party, STRING("stg6_rsh_pro_04"))
                WAIT_MS(300)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_N_2"))
                WAIT_MS(600)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
                CALL(evt_cam3d_evt_off, 500, 11)
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT
            CASE_LESS(303)
            SET_VALUE(GSW_STORY_PROGRESS, 303)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                WAIT_MS(2000)
                CALL(evt_msg_print, 0, STRING("stg6_yado_03"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
            END_INLINE_EVENT
            CASE_LESS(328)
            SET_VALUE(GSW_STORY_PROGRESS, 328)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_msg_print, 0, STRING("stg6_yado_04"), 0, 0)
                CALL(evt_mario_get_party, LW(0))
                SWITCH(LW(0))
                    CASE_EQUAL(1)
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
            CASE_LESS(334)
            SET_VALUE(GSW_STORY_PROGRESS, 334)
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
        CASE_BETWEEN(306, 307)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5_kami"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5"), 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, 307)
            CALL(evt_run_case_evt, 5, 1, STRING("a_kami"), 0, PTR(evt_daiji_kami), 0)
        END_IF
        SPAWN_EVENT(PTR(evt_toron_appear))
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5"), 1)
    END_SWITCH
#ifdef VERSION_G8ME01
    SPAWN_EVENT(evt_sub_mail_event)
#endif
    CALL_CHILD(PTR(evt_rsh_moamoa_on_window))
    RETURN
EVT_END()

EVT_BEGIN(rsh_02_b_init_evt)
    CALL_CHILD(PTR(rsh_02_init_evt))
    IF_INT_EQUAL(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        SET_VALUE(GSWF(3444), 0)
        SET_VALUE(LW(0), PTR(bero_data_b))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
    END_IF
    CALL(evt_party_set_breed_pose, 0, 1)
    RETURN
EVT_END()

static const char* window_tbl[] = {
    "mado_3_uturu",
    "mado_4_uturu",
    "mado_5_uturu",
    NULL,
};

EVT_BEGIN(rsh_02_c_init_evt)
    SET_VALUE(LW(0), PTR(bero_data_c))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(rsh_02_init_evt))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    CALL(evt_map_playanim, STRING("shouten"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("shouten"), 1)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("hom_hassya"))
        CALL(evt_mario_set_pos, -245, 0, -185)
        CALL(evt_party_set_pos, 0, -205, 0, -185)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_seq_wait, 2)
            WAIT_MS(1000)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_msg_print, 0, STRING("stg6_rsh_216"), 0, 0)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(rsh_02_init_evt_nidome)
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
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_EQUAL(GSWF(3456), 1)
        SET_VALUE(GSWF(3456), 0)
        CALL_CHILD(PTR(evt_rsh_rnd_npc_init))
    END_IF
    CALL_CHILD(PTR(evt_access_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, -125, STRING("heya_4"), 125, GSWF(5649))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -317, 0, -127, 0, GSWF(5559), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -365, 60, -175, 0, 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("pik_touchaku"))
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        ELSE
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
                IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
                    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH1"))
                    CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
                END_IF
                IF_STR_EQUAL(LW(0), STRING("rsh_02_b"))
                END_IF
                IF_STR_EQUAL(LW(0), STRING("rsh_02_c"))
                    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH2"))
                    CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(evt_run_case_evt, 9, 1, STRING("bed"), 0, PTR(evt_nidome_bed), 0)
    RETURN
EVT_END()

EVT_BEGIN(rsh_02_a_init_evt_nidome)
    CALL_CHILD(PTR(rsh_02_init_evt_nidome))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_yuka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_yuka_simi"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yuka_5"), 1)
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yuka_5_kami_2"), 1)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("nidome"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                WAIT_MS(2000)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TRAIN_CHIME1"), LW(0), LW(1), LW(2), 0)
                IF_INT_EQUAL(GSWF(3578), 0)
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_03"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg6_rsh_c_05"), 0, 0)
                END_IF
                WAIT_MS(2000)
                IF_INT_EQUAL(GSWF(3578), 0)
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
                SET_VALUE(GSWF(3460), 1)
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
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    RETURN
EVT_END()

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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("nidome"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_stop, 0)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_RSH3"))
                CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1"))
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

EVT_BEGIN(rsh_02_c_init_evt_nidome)
    CALL_CHILD(PTR(rsh_02_init_evt_nidome))
    CALL(evt_map_playanim, STRING("ranpu"), 1, 0)
    SET_VALUE(LW(0), PTR(&window_tbl))
    CALL_CHILD(PTR(evt_rsh_window))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("shouten"), 1)
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_3"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("heya_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("haikei_4"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("heya_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("haikei_5"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("rouka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tobira_5"), 1)
        ELSE
        SET_VALUE(LW(0), PTR(bero_data_c))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_nidome_bed)
    IF_INT_EQUAL(GSWF(3580), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL_CHILD(PTR(evt_normal_yado))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3581), 1)
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
    CALL(evt_snd_bgmon, 512, STRING("BGM_FF_GO_SLEEPING1"))
    CALL(evt_snd_envoff, 1024)
    CALL(evt_fade_set_mapchange_type, 1, 4, 3000, -1, -1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(bgm_wait_stop)
    IF_INT_EQUAL(GSWF(3579), 1)
        IF_INT_EQUAL(GSWF(3578), 0)
            SET_VALUE(GSW(870), 1)
            ELSE
            SET_VALUE(GSW(870), 3)
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

