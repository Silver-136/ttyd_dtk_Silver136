// tou_08 (闘技場, Glitz Pit): the major-league locker room; exits to tou_04 and tou_05; champion belt, the poisoned cake scene.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include "driver/itemdrv.h"
#include "driver/icondrv.h"
#include "mario/mario_pouch.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "tou.h"
#include "evt/evt_npc.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"
#include <dolphin/pad.h>

static USER_FUNC(get_yoshi_color);
USER_FUNC(icon_off);
USER_FUNC(tou_08_belt_camid);

static EvtWord evt_toile[42];
static EvtWord n_door_open[12];
static EvtWord n_door_close[12];
static EvtWord toile_in[13];
static EvtWord toile_out[35];
static BeroEntry bero_data[3];
EvtWord init_togenoko[12];
EvtWord talk_togenoko[136];
EvtWord init_togemet[12];
EvtWord talk_togemet[136];
EvtWord init_hammerbros[12];
EvtWord talk_hammerbros[136];
static EvtWord init_uranoko[12];
static EvtWord talk_uranoko[136];
static NpcSetupInfo npcEnt[7];
static EvtWord evt_sensyu[70];
EvtWord evt_1st_sub[107];
static EvtWord evt_1st_leagu[747];
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_toile_door[];
#else
EvtWord evt_toile_door[236];
#endif
EvtWord evt_toile_door2[136];
#if VERSION >= VERSION_US
EvtWord evt_champion_win[290];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord evt_champion_win[269];
#else
EvtWord evt_champion_win[271];
#endif
#endif
static EvtWord bed_entry[10];
EvtWord evt_tou_08_cake[53];
static EvtWord epilogue_npc_entry[84];
static EvtWord epilogue_evt_sub[322];
static EvtWord epilogue_evt[83];
EvtWord tou_08_init_evt[289];

/**
 * Run when Mario arrives from the toilet pipe before STG3_21_1ST_MATCH_WITH_RAWK_HAWK:
 * he walks to (-265, 125), the partner remarks (stg3_tou_422),
 * GSWF_TOU_TITLE_MATCH_RESERVED is cleared and the story advances to STG3_21_1ST_MATCH_WITH_RAWK_HAWK.
 */
static EVT_BEGIN(evt_toile)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_bero_exec_wait, 65536)
        CALL(evt_mario_mov_pos2, -265, 125, FLOAT(80.0f))
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING("stg3_tou_422"))
        SET_VALUE(GSWF_GLITZ_PIT_TITLE_MATCH_BOOKED, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Door animation callback for the room door (door_1), opening: LW(0) is the swing angle;
 * turns door_1 about the Y axis by 0.44 times the angle.
 */
static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback for the room door (door_1), closing:
 * turns door_1 about the Y axis by -0.44 times the angle.
 */
static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(toile_in)
    SET_VALUE(GSWF_TOU_09_TOILET_WARP_PENDING, 0)
    SET_VALUE(GSWF_TOU_08_TOILET_WARP_PENDING, 0)
    SPAWN_EVENT(PTR(evt_toile))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(toile_out)
    CALL(evt_map_playanim, STRING("anm_mizu"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_map_playanim, STRING("anm_futa"), MAP_ANIM_PLAY_ONCE, 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_TOILET1"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GSWF_TOU_08_TOILET_WARP_PENDING, 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exits of this room: the door back to the main corridor tou_04 and the restroom pipe down to tou_04. */
static BeroEntry bero_data[] = {
    {
        "n_bero", // hitName
        1, // type
        4, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        "s_bero_6", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        100, // length
        toile_in, // enterEvent
        6, // caseType
        toile_out, // exitEvent
        "tou_04", // nextMapName
        "toire2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** NPC init for the Koopatrol (togenoko) fighter in the locker room: placed at (90, 0, -50) facing 270. */
EVT_BEGIN(init_togenoko)
    CALL(evt_npc_set_position, STRING("me"), 90, 0, -50)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

/**
 * Talk script of the Koopatrol fighter:
 * story-dependent locker-room lines (the first Rawk Hawk match, the rematch stages).
 */
EVT_BEGIN(talk_togenoko)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
            IF_INT_EQUAL(GSWF_TOU_RAWK_HAWK_OPENING_SEEN, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_13_01"), 0, 0)
                RETURN
            END_IF
        END_IF
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_04"), 0, 0)
            CASE_EQUAL(9)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_05"), 0, 0)
            CASE_EQUAL(8)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_06"), 0, 0)
            CASE_EQUAL(7)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_07"), 0, 0)
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_08"), 0, 0)
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_09"), 0, 0)
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_10"), 0, 0)
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_11"), 0, 0)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_12"), 0, 0)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_01_13"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            IF_INT_EQUAL(GSWF_TOU_CHAMPION_TITLE_REGAINED, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_040"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_041_1"), 0, 0)
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_041"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for the Spike Top fighter (togemet): placed at (120, 0, 80) facing 270. */
EVT_BEGIN(init_togemet)
    CALL(evt_npc_set_position, STRING("トゲメット"), 120, 0, 80)
    CALL(evt_npc_set_ry, STRING("トゲメット"), 270)
    RETURN
EVT_END()

/** Talk script of the Spike Top fighter: story-dependent locker-room lines. */
EVT_BEGIN(talk_togemet)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
            IF_INT_EQUAL(GSWF_TOU_RAWK_HAWK_OPENING_SEEN, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_13_01"), 0, 0)
                RETURN
            END_IF
        END_IF
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_04"), 0, 0)
            CASE_EQUAL(9)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_05"), 0, 0)
            CASE_EQUAL(8)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_06"), 0, 0)
            CASE_EQUAL(7)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_07"), 0, 0)
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_08"), 0, 0)
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_09"), 0, 0)
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_10"), 0, 0)
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_11"), 0, 0)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_12"), 0, 0)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_02_13"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            IF_INT_EQUAL(GSWF_TOU_CHAMPION_TITLE_REGAINED, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_045"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_046_1"), 0, 0)
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_046"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for the Hammer Bro fighter: placed at (-115, 0, 115) facing 90. */
EVT_BEGIN(init_hammerbros)
    CALL(evt_npc_set_position, STRING("ハンマーブロス"), -115, 0, 115)
    CALL(evt_npc_set_ry, STRING("ハンマーブロス"), 90)
    RETURN
EVT_END()

/** Talk script of the Hammer Bro fighter: story-dependent locker-room lines. */
EVT_BEGIN(talk_hammerbros)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
            IF_INT_EQUAL(GSWF_TOU_RAWK_HAWK_OPENING_SEEN, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_13_01"), 0, 0)
                RETURN
            END_IF
        END_IF
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_04"), 0, 0)
            CASE_EQUAL(9)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_05"), 0, 0)
            CASE_EQUAL(8)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_06"), 0, 0)
            CASE_EQUAL(7)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_07"), 0, 0)
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_08"), 0, 0)
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_09"), 0, 0)
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_10"), 0, 0)
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_11"), 0, 0)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_12"), 0, 0)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_03_13"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            IF_INT_EQUAL(GSWF_TOU_CHAMPION_TITLE_REGAINED, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_050"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_051_1"), 0, 0)
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_051"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for the Shady Koopa (ウラノコ): placed at (-160, 0, -50) facing 90. */
static EVT_BEGIN(init_uranoko)
    CALL(evt_npc_set_position, STRING("me"), -160, 0, -50)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

static EVT_BEGIN(talk_uranoko)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
            IF_INT_EQUAL(GSWF_TOU_RAWK_HAWK_OPENING_SEEN, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_13_01"), 0, 0)
                RETURN
            END_IF
        END_IF
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(10)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_04"), 0, 0)
            CASE_EQUAL(9)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_05"), 0, 0)
            CASE_EQUAL(8)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_06"), 0, 0)
            CASE_EQUAL(7)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_07"), 0, 0)
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_08"), 0, 0)
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_09"), 0, 0)
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_10"), 0, 0)
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_11"), 0, 0)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_11_02"), 0, 0)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg3_npc_08_04_12"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            IF_INT_EQUAL(GSWF_TOU_CHAMPION_TITLE_REGAINED, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_055"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg3_npc_re_056_1"), 0, 0)
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_056"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * NPCs of the major-league locker room: Grubba, Jolene, a guard, the Koopatrol and Shady Koopa fighters
 * and Rawk Hawk.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "ガンス", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "キノシコワ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ガードマン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "トゲノコエース", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_togenoko, // initEvent
        NULL, // regularEvent
        talk_togenoko, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ウラノコ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_uranoko, // initEvent
        NULL, // regularEvent
        talk_uranoko, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "チャンピョン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Second stop of Grubba's tour, run from the map init before STG3_03_GRUBBA:
 * Grubba (ガンス) stands next to Mario and introduces the major-league locker room (stg3_tou_50),
 * then the map changes to tou_05 with a page-flip transition.
 */
static EVT_BEGIN(evt_sensyu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, -75)
    CALL(evt_party_set_pos, 0, -30, 0, -75)
    CALL(evt_npc_set_position, STRING("ガンス"), 40, 0, -75)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_cam3d_evt_set, 0, 145, 534, 0, 50, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_50"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_fade_set_mapchange_type, 0, FADE_PAGE_FLIP_Z1, 0, FADE_PAGE_FLIP_A1, 0)
    CALL(evt_bero_mapchange, STRING("tou_05"), 0)
    RETURN
EVT_END()

/**
 * Exit helper for the promotion scene (evt_1st_leagu):
 * the NPC named in LW(0) (Jolene or the champion) turns intangible,
 * walks to the door at (0, -130) with Mario turning to watch,
 * opens it (180 degrees for Jolene, 260 for anyone else) with the door sounds, walks out and is parked off-map,
 * and the door closes.
 */
EVT_BEGIN(evt_1st_sub)
    SET_VALUE(LW(8), LW(0))
    CALL(evt_npc_flag_onoff, 1, LW(8), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, LW(8), 0, -130, 0, FLOAT(80.0f), 1)
    CALL(evt_mario_set_dir_npc, LW(8))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    IF_STR_EQUAL(LW(8), STRING("キノシコワ"))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    ELSE
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 260, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, LW(8), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, LW(8), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    IF_STR_EQUAL(LW(8), STRING("キノシコワ"))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    ELSE
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 260, 0, 500)
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Scene on Mario's first league promotion (the majors, 10th place):
 * Jolene (キノシコワ) opens the door and introduces him (stg3_tou_294),
 * then the champion (チャンピョン, wearing the belt via tou_champion_belt) comes in,
 * greets Mario and the partner (stg3_tou_294_02 to 08),
 * after which Mario's mail arrives with the vibration jingle.
 * Mario reads it (stg3_tou_294_12), mail 6 is received and opened,
 * the Glitz Pit music resumes and the story advances to STG3_20_MAJORS_10TH_PLACE.
 */
static EVT_BEGIN(evt_1st_leagu)
    // Jolene opens the door and leads Mario and the partner into the major-league locker room.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, 0, -180)
    CALL(evt_mario_set_pos, 0, 0, -190)
    CALL(evt_party_set_pos, 0, 0, 0, -200)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING("チャンピョン"))
    END_INLINE_EVENT
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -1, -90, 0, FLOAT(80.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -3, -80, FLOAT(120.0f))
        CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 0, -100, FLOAT(120.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 40, -100, FLOAT(120.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("キノシコワ"))
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("キノシコワ"), -40, -90, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(n_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -20, 84, 174, -20, 37, -93, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    SET_VALUE(LW(0), STRING("キノシコワ"))
    CALL_CHILD(PTR(evt_1st_sub))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -138, 150, 533, -138, 50, -8, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 138, 150, 533, 138, 50, -8, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(3500)
    CALL(evt_cam3d_evt_set, 20, 104, 238, 20, 60, -8, 1000, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_01"))
    // The music fades and Rawk Hawk swaggers in through the door.
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("チャンピョン"), 0, 0, -180)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -260, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -60, -85, FLOAT(60.0f))
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -95, -85, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("チャンピョン"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("チャンピョン"), 0, -85, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -260, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_02"), 0, STRING("チャンピョン"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -30, 104, 238, -30, 60, -8, 500, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("チャンピョン"), STRING("mario"))
    WAIT_MS(800)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, 0, STRING("チャンピョン"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_02_01"), 0, STRING("チャンピョン"))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
    // Mario eyes the belt and the partner realises its Crystal Star is a fake.
    CALL(evt_cam3d_evt_set, -80, 104, 238, -80, 60, -8, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_03"))
    CALL(evt_cam3d_evt_set, -10, 104, 238, -10, 60, -8, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_move_pos2, 0, -20, -75, FLOAT(60.0f))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_04"), 0, STRING("チャンピョン"))
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_05"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_06"), 0, STRING("チャンピョン"))
    CALL(evt_party_set_dir_npc, 0, STRING("チャンピョン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_06_01"), 0, STRING("チャンピョン"))
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_07"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_08"), 0, STRING("チャンピョン"))
    WAIT_MS(300)
    SET_VALUE(LW(0), STRING("チャンピョン"))
    CALL_CHILD(PTR(evt_1st_sub))
    CALL(evt_cam3d_evt_set, -45, 120, 484, -45, 45, -9, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_09"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_N_1"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("stg3_tou_294_10"))
    WAIT_MS(300)
    WAIT_MS(1000)
    // Mail arrives: the jingle plays, the partner points it out and Mario waits with a "!" until A is pressed.
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1500)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION2"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_294_11"))
    LOOP(0)
        IS_EVENT_RUNNING(LW(11), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), PAD_BUTTON_A)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    // Mario reads X's mail about the Crystal Star, the partner finds it fishy, mail 6 is filed
    // and the story advances.
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG3_TOU1"), 2000)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    CALL(evt_snd_envon_f, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"), 2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_294_12"), 0, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("stg3_tou_294_13"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_pouch_receive_mail, 6)
    CALL(evt_pouch_open_mail, 6)
    SET_VALUE(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
    RETURN
EVT_END()

/**
 * Restroom door (t_door_2) used from the locker room:
 * Mario gets "OCCUPIED!" (stg3_tou_t_00) while a non-title match is reserved,
 * before STG3_21_1ST_MATCH_WITH_RAWK_HAWK, from STG3_34_RETURN_TO_ROGUEPORT on,
 * and in JP and later once the champion's opening scene has played.
 * Otherwise Mario and the partner walk to the door, it opens with a wood-door sound,
 * they step through into the restroom and it shuts.
 */
// This entrance is available only for the first title-match escape; the return path is separate.
EVT_BEGIN(evt_toile_door)
    IF_INT_EQUAL(GSWF_GLITZ_PIT_TITLE_MATCH_BOOKED, 0)
        IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_t_00"), 0, 0)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_21_1ST_MATCH_WITH_RAWK_HAWK)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_t_00"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
#else
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_t_00"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_TOU_RAWK_HAWK_OPENING_SEEN, 1)
#endif
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_t_00"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("t_door_2"), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("t_door_3"), 8388608)
    CALL(evt_mario_adjust_dir)
    WAIT_FRAMES(1)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -200, 110, FLOAT(80.0f))
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -200, 130, FLOAT(80.0f))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 80, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos, 0, -270, 110, 700)
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, -270, 130, 700)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 80, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_hitobj_attr_onoff, 0, 0, STRING("t_door_2"), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 0, STRING("t_door_3"), 8388608)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/** Restroom door from inside: opens door_2 with a wood-door sound, walks the party out and shuts it. */
EVT_BEGIN(evt_toile_door2)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("t_door_2"), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("t_door_3"), 8388608)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 80, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos, 0, -200, 110, 700)
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, -200, 130, 700)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 80, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_hitobj_attr_onoff, 0, 0, STRING("t_door_2"), 8388608)
    CALL(evt_hitobj_attr_onoff, 0, 0, STRING("t_door_3"), 8388608)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_champion_win)
    CALL(evt_mario_key_onoff, 0)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("ガンス"), -45, 0, -95)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -15, 0, -90)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_npc_flag_onoff, 1, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 0, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("キノシコワ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg3_tou_445"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("ガンス"), -60, -95, 0, FLOAT(80.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("ガンス"), -45, -95, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_npc_entry, STRING("ベルト"), STRING("c_bred_h"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 56)
    CALL(evt_npc_set_position, STRING("ベルト"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_anim, STRING("ベルト"), STRING("C_1"))
    ADD(LW(1), 4)
    ADD(LW(2), 15)
    CALL(evt_eff, STRING("belt"), STRING("starstone"), 9, LW(0), LW(1), LW(2), FLOAT(0.75f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_item_entry, STRING("belt"), ITEM_CHAMPS_BELT, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(icon_off)
    CALL(evt_item_get_item, STRING("belt"))
    CALL(evt_item_get_item_end_wait, STRING("belt"))
    CALL(evt_npc_delete, STRING("ベルト"))
    CALL(evt_eff_delete, STRING("belt"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_447"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_448"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#else
#if VERSION >= VERSION_US
    CALL(evt_npc_check, STRING("ハンマーブロス"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_npc_get_reglid, STRING("ハンマーブロス"), LW(0))
            DELETE_EVENT(LW(0))
        END_INLINE_EVENT
    END_IF
#endif
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("ガンス"), -45, 0, -95)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -15, 0, -90)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_npc_flag_onoff, 1, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 0, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("キノシコワ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg3_tou_445"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("ガンス"), -60, -95, 0, FLOAT(80.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("ガンス"), -45, -95, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_npc_entry, STRING("ベルト"), STRING("c_bred_h"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 56)
    CALL(evt_npc_set_position, STRING("ベルト"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_anim, STRING("ベルト"), STRING("C_1"))
    ADD(LW(1), 4)
    ADD(LW(2), 15)
    CALL(evt_eff, STRING("belt"), STRING("starstone"), 9, LW(0), LW(1), LW(2), FLOAT(0.75f), 0, 0, 0, 0, 0, 0, 0)
    CALL(tou_08_belt_camid)
    CALL(evt_item_entry, STRING("belt"), ITEM_CHAMPS_BELT, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(icon_off)
    CALL(evt_item_get_item, STRING("belt"))
    CALL(evt_item_get_item_end_wait, STRING("belt"))
    CALL(evt_npc_delete, STRING("ベルト"))
    CALL(evt_eff_delete, STRING("belt"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_447"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_448"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), -5, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_mario_mov_pos2, 0, -130, FLOAT(60.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -5, -200, FLOAT(60.0f))
    END_INLINE_EVENT
    SET_VALUE(GSWF_TOU_JOLENE_ESCORT_TO_CHAMP_ROOM, 1)
    CALL(evt_bero_mapchange, STRING("tou_04"), 0)
    RETURN
EVT_END()

// Descriptive name (not in the symbol maps); placeholder unk_tou_00006e08, JP tou .text+0x6D44. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
USER_FUNC(tou_08_belt_camid) {
    EffectEntry* eff = effNameToPtr("belt");
    // ベルト
    NpcEntry* npc = npcNameToPtr("ベルト");

    ((EffStarStoneWork*)eff->userdata)->cam = CAMERA_3D_EFFECTB;
    npc->cameraId = CAMERA_3D_EFFECTB;
    return EVT_RETURN_DONE;
}
#endif

/** Script helper: hides the belt item's icon. */
USER_FUNC(icon_off) {
    iconFlagOn(itemNameToPtr("belt")->name, ICON_FLAG_HIDDEN);
    return EVT_RETURN_DONE;
}

/** Bed entry point: registers evt_tou_bed to run when Mario presses A in front of the bed named in LW(0). */
static EVT_BEGIN(bed_entry)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_tou_bed), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_tou_08_cake)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_get_position, STRING("ウラノコ"), LW(0), LW(1), LW(2))
    CALL(evt_cam3d_evt_set, LW(0), 90, 283, LW(0), 45, -9, 1000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir_npc, STRING("ウラノコ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir_npc, 0, STRING("ウラノコ"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_2"), 0, STRING("ウラノコ"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF_TOU_CAKE_ON_TABLE, 0)
    RETURN
EVT_END()

/**
 * Epilogue setup: spawns Goombella (クリスチーヌ) at (30, 0, -50) facing 270
 * and Mario's Yoshi (チビヨッシー) at (-30, 0, -50) facing 90,
 * using the baby Yoshi model that matches the Yoshi's colour (get_yoshi_color),
 * with no wall collision or hitbox for the Yoshi.
 */
static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 30, 0, -50)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(get_yoshi_color, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("c_babyyoshi"))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("c_babyyoshi2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("c_babyyoshi3"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("c_babyyoshi4"))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("c_babyyoshi5"))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("c_babyyoshi6"))
        CASE_EQUAL(6)
        SET_VALUE(LW(0), STRING("c_babyyoshi7"))
    END_SWITCH
    CALL(evt_npc_entry, STRING("チビヨッシー"), LW(0))
    CALL(evt_npc_set_tribe, STRING("チビヨッシー"), STRING("ヨッシー"))
    CALL(evt_npc_set_position, STRING("チビヨッシー"), -30, 0, -50)
    CALL(evt_npc_set_ry, STRING("チビヨッシー"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("チビヨッシー"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Background action of the epilogue scene: Goombella (クリスチーヌ)
 * and Yoshi (チビヨッシー) take turns in their talking animation;
 * after 3 seconds the door opens and a guard (ガードマン) walks in and faces Yoshi.
 * Once Yoshi has talked once more he walks over to the guard and both leave through the door, which closes,
 * and Goombella then keeps turning back and forth.
 */
static EVT_BEGIN(epilogue_evt_sub)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_S_1"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_open_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -180, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
            CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_npc_set_position, STRING("ガードマン"), 0, 0, -170)
        CALL(evt_npc_move_position, STRING("ガードマン"), 0, -90, 0, FLOAT(80.0f), 0)
        CALL(evt_set_dir_to_target, STRING("ガードマン"), STRING("チビヨッシー"))
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
            CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_S_1"))
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("チビヨッシー"), STRING("PYS_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("ガードマン"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 15)
    ADD(LW(2), 20)
    CALL(evt_npc_move_position, STRING("チビヨッシー"), LW(0), LW(2), 0, 60, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン"), 0, -130, 0, FLOAT(80.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("チビヨッシー"), -15, -110, 0, FLOAT(80.0f), 0)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン"), 0, -200, 0, FLOAT(80.0f), 0)
        CALL(evt_npc_set_position, STRING("ガードマン"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("チビヨッシー"), 0, -200, 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("チビヨッシー"), 0, -1000, 0)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        WAIT_MS(3000)
        CALL(evt_npc_reverse_ry, STRING("クリスチーヌ"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("クリスチーヌ"))
        CALL(evt_npc_reverse_ry, STRING("クリスチーヌ"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("クリスチーヌ"))
    END_LOOP
    RETURN
EVT_END()

/**
 * Epilogue scene in the major-league locker room:
 * Glitz Pit epilogue music, Mario hidden, the camera framing Goombella and Yoshi,
 * who talk while epilogue_evt_sub runs; the narration ep_40 shows Yoshi's name,
 * then the screen fades out and the map changes to usu_00.
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_URN1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("cake_tukue"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING("チビヨッシー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 100, 337, 0, 45, -9, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_flag_onoff, 0, STRING("チビヨッシー"), NPC_FLAG_INSTANT_TURN)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_pouch_get_yoshiname, LW(0))
    CALL(evt_msg_print_insert, 0, STRING("ep_40"), 0, 0, LW(0))
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 900, FADE_IN_BLACK, 900)
    CALL(evt_bero_mapchange, STRING("usu_00"), STRING(""))
    RETURN
EVT_END()

#if VERSION >= VERSION_US
// Pause/restart these regular events only when Hammer Bros. exists; the post-match child always starts.
EVT_BEGIN(evt_match_after_stop_npcs)
    SPAWN_EVENT_GET_ID(PTR(evt_tou_match_after_default), LW(10))
    CALL(evt_npc_check, STRING("ハンマーブロス"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_npc_get_reglid, STRING("ハンマーブロス"), LW(0))
        DELETE_EVENT(LW(0))
        CALL(evt_npc_check, STRING("トゲメット"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_get_reglid, STRING("トゲメット"), LW(0))
            DELETE_EVENT(LW(0))
        END_IF
        LOOP(0)
            IS_EVENT_RUNNING(LW(10), LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_restart_regular_event, STRING("ハンマーブロス"))
        CALL(evt_npc_check, STRING("トゲメット"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_restart_regular_event, STRING("トゲメット"))
        END_IF
    END_IF
    RETURN
EVT_END()
#endif

EVT_BEGIN(tou_08_init_evt)
#if VERSION <= VERSION_DEMO_US
    // The story is moved up to match Mario's rank, then the debug story-skip flags are applied.
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_12_JOLENES_SECRET)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_12_JOLENES_SECRET)
        END_IF
        CASE_LESS_OR_EQUAL(7)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
        END_IF
        CASE_LESS_OR_EQUAL(8)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_00_MAJORS_8TH_PLACE)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_00_MAJORS_8TH_PLACE)
        END_IF
        CASE_LESS_OR_EQUAL(10)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_CONFISCATED, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_CONFISCATED, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
    END_IF
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_MAJORS_4TH, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_MAJORS_4TH, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_05_MAJORS_4TH_PL)
    END_IF
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_MAJORS_1ST, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_MAJORS_1ST, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_13_MAJORS_1ST_PL)
    END_IF
    // Doors, the Ice Storm badge, lockers, the fighters, the match terminal and monitor,
    // the restroom doors and the bed.
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_item_entry, STRING("item_01"), ITEM_ICE_STORM, -120, 12, -135, ITEM_TYPE_STILL, GSWF_TOU_08_ICE_STORM, 0)
    CALL_CHILD(evt_tou_locker_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("fuku_M"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(init_hammerbros)
    CALL(evt_npc_change_interrupt, STRING("ハンマーブロス"), 6, PTR(talk_hammerbros))
    CALL_CHILD(init_togemet)
    CALL(evt_npc_change_interrupt, STRING("トゲメット"), 6, PTR(talk_togemet))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("M_make"), 0, PTR(evt_tou_match_make_default), 0)
    CALL_CHILD(evt_tou_monitor_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("isu"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("t_door_2"), 0, PTR(evt_toile_door), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("t_door_3"), 0, PTR(evt_toile_door2), 0)
    CALL(evt_hitobj_name_callback, STRING("a_b_H"), PTR(bed_entry))
    CALL(evt_sub_animgroup_async, STRING("p_slit"))
    // After a match: the title win scene at rank 0, otherwise the default after-match scene.
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
        SET_VALUE(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 0)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
            SPAWN_EVENT(evt_champion_win)
        ELSE
SPAWN_EVENT(evt_tou_match_after_default)
        END_IF
        RETURN
    END_IF
    // Music: Grubba's theme and his tour on the first visit, else the music for the story stage.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_03_GRUBBA)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_GANCE1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        SPAWN_EVENT(evt_sensyu)
        CASE_DEFAULT
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_00_WELCOME_TO_GLITZVILLE, STG3_12_THE_MINI_YOSHI_EGG)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_GREATER_OR_EQUAL(STG3_33_PEACH_THE_SECRET_WEAPON)
            IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            END_IF
            CASE_DEFAULT
            IF_INT_NOT_EQUAL(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 1)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            ELSE
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            END_IF
        END_SWITCH
    END_SWITCH
    // The promotion scene, Shady Koopa's word about the cake, and the epilogue.
    IF_INT_EQUAL(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 1)
        SET_VALUE(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 0)
        SPAWN_EVENT(evt_1st_leagu)
    END_IF
    IF_INT_EQUAL(GSWF_TOU_CAKE_ON_TABLE, 1)
        SPAWN_EVENT(evt_tou_08_cake)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
#else
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_12_JOLENES_SECRET)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_12_JOLENES_SECRET)
        END_IF
        CASE_LESS_OR_EQUAL(7)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
        END_IF
        CASE_LESS_OR_EQUAL(8)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_00_MAJORS_8TH_PLACE)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_00_MAJORS_8TH_PLACE)
        END_IF
        CASE_LESS_OR_EQUAL(10)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
        END_IF
    END_SWITCH
    // These pending story jumps are consumed after the rank-based minimum progress updates.
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_CONFISCATED, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_CONFISCATED, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
    END_IF
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_MAJORS_4TH, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_MAJORS_4TH, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_05_MAJORS_4TH_PL)
    END_IF
    IF_INT_EQUAL(GSWF_TOU_08_JUMP_STORY_MAJORS_1ST, 1)
        SET_VALUE(GSWF_TOU_08_JUMP_STORY_MAJORS_1ST, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_20_13_MAJORS_1ST_PL)
    END_IF
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_item_entry, STRING("item_01"), ITEM_ICE_STORM, -240, 0, -90, ITEM_TYPE_STILL, GSWF_TOU_08_ICE_STORM, 0)
    CALL_CHILD(PTR(evt_tou_locker_init))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("fuku_M"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(init_hammerbros))
    CALL(evt_npc_change_interrupt, STRING("ハンマーブロス"), 6, PTR(talk_hammerbros))
    CALL_CHILD(PTR(init_togemet))
    CALL(evt_npc_change_interrupt, STRING("トゲメット"), 6, PTR(talk_togemet))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("M_make"), 0, PTR(evt_tou_match_make_default), 0)
    CALL_CHILD(PTR(evt_tou_monitor_init))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("isu"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("t_door_2"), 0, PTR(evt_toile_door), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("t_door_3"), 0, PTR(evt_toile_door2), 0)
    CALL(evt_hitobj_name_callback, STRING("a_b_H"), PTR(bed_entry))
    CALL(evt_sub_animgroup_async, STRING("p_slit"))
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
        SET_VALUE(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 0)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
            SPAWN_EVENT(PTR(evt_champion_win))
        ELSE
#if VERSION >= VERSION_US
            SPAWN_EVENT(PTR(evt_match_after_stop_npcs))
#else
            SPAWN_EVENT(PTR(evt_tou_match_after_default))
#endif
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_03_GRUBBA)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_GANCE1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        SPAWN_EVENT(PTR(evt_sensyu))
        CASE_DEFAULT
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_00_WELCOME_TO_GLITZVILLE, STG3_12_THE_MINI_YOSHI_EGG)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_GREATER_OR_EQUAL(STG3_33_PEACH_THE_SECRET_WEAPON)
            IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            END_IF
            CASE_DEFAULT
            IF_INT_NOT_EQUAL(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 1)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            ELSE
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            END_IF
        END_SWITCH
    END_SWITCH
    IF_INT_EQUAL(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 1)
        SET_VALUE(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 0)
        SPAWN_EVENT(PTR(evt_1st_leagu))
    END_IF
    IF_INT_EQUAL(GSWF_TOU_CAKE_ON_TABLE, 1)
        SPAWN_EVENT(PTR(evt_tou_08_cake))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
#endif
    RETURN
EVT_END()

static USER_FUNC(get_yoshi_color) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], pouchGetPartyColor(PARTNER_YOSHI));
    return EVT_RETURN_DONE;
}
