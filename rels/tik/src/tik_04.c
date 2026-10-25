#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_mario_get_name_hitobj_ride);

static BeroEntry bero_entry_data[5];
static EvtWord puni_walk_se[20];
EvtWord puni_soguu_1[102];
EvtWord puni_soguu_2[146];
EvtWord plane_tobashi_check[87];
EvtWord tik04_wall_blow_evt[5];
EvtWord kaidan_pera[122];
EvtWord tik_04_switch_evt[41];
static EvtWord mobj_box00[34];
EvtWord tik_04_init_evt[220];

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
        "tik_05", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_03", // nextMapName
        "s_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero_5", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_19", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(puni_walk_se)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_MOVE2_1"), LW(0), LW(1), LW(2), 0)
        WAIT_FRAMES(5)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(puni_soguu_1)
    CALL(evt_npc_set_position, STRING("ガイド"), -155, 25, -80)
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -275)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    WAIT_MS(200)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("ガイド"), STRING("R_1"))
        CALL(evt_npc_set_ry, STRING("ガイド"), 90)
        SPAWN_EVENT_GET_ID(PTR(puni_walk_se), LW(10))
        CALL(evt_npc_move_position, STRING("ガイド"), -150, -220, 0, FLOAT(180.0f), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 13)
    RETURN
EVT_END()

EVT_BEGIN(puni_soguu_2)
    CALL(evt_npc_set_position, STRING("ガイド"), -155, 25, -80)
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -275)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    WAIT_MS(200)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("ガイド"), STRING("R_1"))
        CALL(evt_npc_set_ry, STRING("ガイド"), 90)
        SPAWN_EVENT_GET_ID(PTR(puni_walk_se), LW(10))
        CALL(evt_npc_move_position, STRING("ガイド"), -150, -220, 0, FLOAT(180.0f), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 35)
    IF_INT_LESS_OR_EQUAL(LW(2), -110)
        ADD(LW(2), 10)
        ELSE
        ADD(LW(2), -10)
    END_IF
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 270, 500)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_1_011"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(200)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 66)
    RETURN
EVT_END()

EVT_BEGIN(plane_tobashi_check)
    LOOP(0)
        CALL(evt_mario_get_motion, LW(0))
        IF_INT_EQUAL(LW(0), 24)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DEBUG_PUT_MSG(STRING("飛行機でとんだ"))
    CALL(evt_mario_wait_landon)
    DEBUG_PUT_MSG(STRING("飛行機で着地した"))
    CALL(evt_mario_get_name_hitobj_ride, LW(0))
    DEBUG_PUT_REG(LW(0))
    SET_VALUE(LF(0), 0)
    IF_STR_EQUAL(LW(0), STRING("a_dai_1"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("MOBJ_block"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(GSWF(1339), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 1)
    WAIT_MS(750)
    IF_INT_EQUAL(GSWF(1370), 0)
        CALL(evt_msg_print, 0, STRING("mac_0_094_01"), 0, STRING("extparty"))
        SET_VALUE(GSWF(1370), 1)
        ELSE
        CALL(evt_msg_print, 0, STRING("mac_0_094_01_1"), 0, STRING("extparty"))
    END_IF
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(plane_tobashi_check))
    RETURN
EVT_END()

EVT_BEGIN(tik04_wall_blow_evt)
    SET_VALUE(GSWF(1336), 1)
    RETURN
EVT_END()

EVT_BEGIN(kaidan_pera)
    CALL(evt_paper_entry, STRING("OFF_d_meku"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_meku"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_dai_nise"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("win_1"), STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("ana"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(evt_mapobj_get_position, STRING("OFF_d_meku"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF0_STEP_APPEAR1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_dai_nise"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dai_6"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_dai_nise"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("win_1"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("ana"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    RETURN
EVT_END()

EVT_BEGIN(tik_04_switch_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 450, 255, 628, 450, 150, 37, 500, 11)
    WAIT_MS(1500)
    CALL_CHILD(PTR(kaidan_pera))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_hitobj_onoff, STRING("a_dai_nise"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("a_dai_6"), 1, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 14)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 293, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_04_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 12)
        SPAWN_EVENT(PTR(puni_soguu_1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 65)
        SPAWN_EVENT(PTR(puni_soguu_2))
    END_IF
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hikou"))
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 15)
        IF_INT_EQUAL(GSWF(1339), 0)
            SPAWN_EVENT(PTR(plane_tobashi_check))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1336), 0)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("win_1"), STRING("a_win_1"), PTR(tik04_wall_blow_evt))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("win_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_win_1"), 1, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_dai_nise"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_dai_nise"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dai_toreru"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dai_6"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_dai_6"), 1, 0)
    IF_INT_EQUAL(GSWF(1335), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_dai_nise"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_dai_nise"), 1, 1)
        CALL(evt_mobj_switch_blue, 0, STRING("mobj_switch00"), 275, 0, 100, PTR(tik_04_switch_evt), GSWF(1335))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("dai_6"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_dai_6"), 1, 1)
    END_IF
    CALL(evt_item_entry, STRING("blackkey"), 33, 175, 125, -100, 16, GSWF(1337), 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 150, -450, 0, 0, 0, 0)
    CALL(evt_mobj_lv_blk, STRING("block"), -425, 0, 0, 0, GSWF(1338), STRING("MOBJ_Lv1BigBigBlock"))
    IF_INT_EQUAL(GSWF(1338), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("block"), MOBJ_FLAG_NO_CULL)
    END_IF
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 310, 0, -25, 0, 0, PTR(mobj_box00), GSWF(1357))
    CALL(evt_mobj_brick, STRING("mobj_box01"), -150, 60, 130, 301, 12, 0, GSWF(1358))
    CALL(evt_item_entry, STRING("item_02"), 125, -460, 25, -100, 16, GSWF(5591), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 261)
        CALL(evt_npc_check_delete, STRING("カメック"))
        CALL(evt_npc_check_delete, STRING("トゲノコ"))
        ELSE
        CALL(evt_npc_check_delete, STRING("ヒンニャー１"))
        CALL(evt_npc_check_delete, STRING("ヒンニャー２"))
    END_IF
    RETURN
EVT_END()

