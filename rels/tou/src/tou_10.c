// tou_10 (闘技場, Glitz Pit): the minor-league locker room; exits to tou_03, tou_04, tou_05; Yoshi egg and fighter scenes.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
#include "nameent.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_hit.h"
#include "evt/evt_pouch.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "driver/swdrv.h"
#include "driver/msgdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "tou.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

static USER_FUNC(getYoshiColor);
USER_FUNC(name_check);
USER_FUNC(setYoshiColor);
USER_FUNC(tou_10_mario_keyoff_check);
USER_FUNC(tou_10_mario_free_wait);

EvtWord room_out[8];
static EvtWord n_door_open[12];
static EvtWord n_door_close[12];
static BeroEntry bero_data[2];
EvtWord init_kino[13];
EvtWord talk_kino[24];
EvtWord init_gard[2];
EvtWord talk_gard[69];
static EvtWord init_michel[42];
static EvtWord talk_michel[94];
EvtWord init_octo[17];
EvtWord talk_octo[141];
static EvtWord init_brots[49];
static EvtWord talk_brots[38];
EvtWord init_gangan[17];
EvtWord talk_gangan[141];
EvtWord init_poku[43];
EvtWord talk_poku[55];
EvtWord init_iron[45];
EvtWord talk_iron[30];
EvtWord init_iron2[45];
EvtWord talk_iron2[30];
static NpcSetupInfo npcEnt[11];

#if VERSION >= VERSION_US
EvtWord evt_saisyo_after[733];
EvtWord evt_saisyo_after_re[367];
EvtWord evt_tou_chibi_yoshi[424];
static EvtWord evt_1st_leagu[235];
EvtWord evt_1st_leagu_re[249];
static EvtWord evt_door[25];
EvtWord evt_rec[274];
static EvtWord evt_gardman[202];
#else
EvtWord evt_saisyo_after[691];
EvtWord evt_saisyo_after_re[325];
EvtWord evt_tou_chibi_yoshi[424];
static EvtWord evt_1st_leagu[214];
EvtWord evt_1st_leagu_re[228];
static EvtWord evt_door[25];
EvtWord evt_rec[274];
#if VERSION == VERSION_DEMO_JP
static EvtWord evt_gardman[237];
#else
static EvtWord evt_gardman[198];
#endif
#endif

EvtWord evt_match_make_sub[138];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_match_make[];
#else
EvtWord evt_match_make[116];
#endif

static EvtWord bed_entry[10];
EvtWord evt_okorareru[301];
EvtWord evt_syorui[92];
EvtWord first_tamago[52];
static EvtWord evt_block[5];
EvtWord tou_10_init_evt[254];

// Descriptive name (not in the symbol maps); placeholder unk_tou_00006e5c, JP tou .text+0x6D98. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/** Script helper: args[0] receives 1 if Mario's key input or control is currently disabled, else 0. */
USER_FUNC(tou_10_mario_keyoff_check) {
    EvtWord* args = event->args;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
#endif

#if VERSION >= VERSION_US
/** Script helper: returns DONE once Mario has left his talking motion, otherwise blocks. */
s32 tou_10_mario_talk_end_wait(void) {
    return marioGetPtr()->currMotionId != MARIO_MOTION_TALK ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_tou_00006ec8, JP tou .text+0x6E04. See docs/NAME_PROVENANCE.md.

// Accumulate 241 eligible checks; blocked controls or other motions pause rather than reset the count.
#if VERSION >= VERSION_JP
/**
 * Script helper: waits for 241 eligible checks with Mario idle, walking or dashing and neither input nor control
 * locked out. Ineligible checks pause the count.
 */
USER_FUNC(tou_10_mario_free_wait) {
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    if (!marioKeyOffChk() && !marioCtrlOffChk() &&
        (mario->currMotionId == MARIO_MOTION_STAY || mario->currMotionId == MARIO_MOTION_WALK || mario->currMotionId == MARIO_MOTION_DASH)) {
        event->userdata[0]++;
        if (event->userdata[0] > 240) {
            event->userdata[0] = 240;
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_BLOCK;
}
#endif

/** Script helper: args[0] receives Yoshi's color (0-6) from the pouch. */
static USER_FUNC(getYoshiColor) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], pouchGetPartyColor(PARTY_YOSHI));
    return EVT_RETURN_DONE;
}

/**
 * Script helper: picks Yoshi's color from the real time that has passed since the egg was received (stored in
 * GSW_TOU_EGG_RECEIVED_TIME), modulo 20 minutes, and stores it in the pouch.
 */
USER_FUNC(setYoshiColor) {
    OSTime start;
    s32 i;
    s32 minutes;

    // the time the Yoshi egg was received
    for (i = 0; i < 8; i++) {
        ((u8*)&start)[i] = swByteGet(i + 508);
    }
    minutes = (s32)OSTicksToSeconds(gp->playTime - start) / 60;

    // The elapsed whole-minute count repeats every 20 minutes across the seven color intervals.
    minutes %= 20;
    if (minutes <= 5) {
        pouchSetPartyColor(PARTY_YOSHI, 0);
    } else if (minutes <= 8) {
        pouchSetPartyColor(PARTY_YOSHI, 1);
    } else if (minutes <= 11) {
        pouchSetPartyColor(PARTY_YOSHI, 2);
    } else if (minutes <= 14) {
        pouchSetPartyColor(PARTY_YOSHI, 3);
    } else if (minutes <= 17) {
        pouchSetPartyColor(PARTY_YOSHI, 4);
    } else if (minutes <= 18) {
        pouchSetPartyColor(PARTY_YOSHI, 5);
    } else {
        pouchSetPartyColor(PARTY_YOSHI, 6);
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(room_out)
    SET_VALUE(GSWF_TOU_10_ENTERED_UNUSED, 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback for the locker room door (door_1), opening: LW(0) is the swing angle;
 * turns door_1 about the Y axis by 0.44 times the angle.
 */
static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback for the locker room door (door_1), closing:
 * turns door_1 about the Y axis by -0.44 times the angle.
 */
static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

/** The one door of the minor-league locker room, out to the main corridor tou_04. */
static BeroEntry bero_data[] = {
    {
        "n_bero", // hitName
        1, // type
        4, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        room_out, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        "s_bero_7", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {0},
};

/** NPC init for Jolene (キノシコワ): placed at (0, 0, -160) until the first match. */
EVT_BEGIN(init_kino)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_05_MATCH)
        CALL(evt_npc_set_position, STRING("me"), 0, 0, -160)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of Jolene before the first match:
 * she tells Mario to log on to the terminal instead (stg3_tou_66),
 * or once GF(0) is set says she doesn't date fighters (stg3_tou_73).
 */
EVT_BEGIN(talk_kino)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_05_MATCH)
        IF_INT_EQUAL(GF(0), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_66"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_73"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/** NPC init for the room guard (empty). */
EVT_BEGIN(init_gard)
    RETURN
EVT_END()

/**
 * Talk script of the guard (ガードマン) who comes to fetch Mario for a match:
 * he walks out through the door, Mario follows him, and the map changes to tou_03, the arena.
 */
EVT_BEGIN(talk_gard)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -130, 0, FLOAT(80.0f), 1)
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("ガードマン"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 0, -200, 60)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

/**
 * NPC init for Michel (マイケル), a Koopa fighter: before STG3_34_RETURN_TO_ROGUEPORT he stands at (120, 0,
 * -30) facing 270, unless the KP Koopas have retired (GSWF_GLITZ_PIT_KP_KOOPAS_RETIRED, set on reaching rank 12);
 * from that story stage on he is always there.
 */
static EVT_BEGIN(init_michel)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSWF_GLITZ_PIT_KP_KOOPAS_RETIRED, 0)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
            CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_michel)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(20)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_04"), 0, 0)
            CASE_EQUAL(19)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_05"), 0, 0)
            CASE_EQUAL(18)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_06"), 0, 0)
            CASE_EQUAL(17)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_07"), 0, 0)
            CASE_EQUAL(16)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_08"), 0, 0)
            CASE_EQUAL(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_09"), 0, 0)
            CASE_EQUAL(14)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_10"), 0, 0)
            CASE_EQUAL(13)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_01_11"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_060"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_061"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for Octor (オクトール): placed at (80, 0, 85) facing 270. */
EVT_BEGIN(init_octo)
    CALL(evt_npc_set_position, STRING("オクトール"), 80, 0, 85)
    CALL(evt_npc_status_onoff, 1, STRING("オクトール"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("オクトール"), 270)
    RETURN
EVT_END()

EVT_BEGIN(talk_octo)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(20)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_04"), 0, 0)
            CASE_EQUAL(19)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_05"), 0, 0)
            CASE_EQUAL(18)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_06"), 0, 0)
            CASE_EQUAL(17)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_07"), 0, 0)
            CASE_EQUAL(16)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_08"), 0, 0)
            CASE_EQUAL(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_09"), 0, 0)
            CASE_EQUAL(14)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_10"), 0, 0)
            CASE_EQUAL(13)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_11"), 0, 0)
            CASE_EQUAL(12)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_12"), 0, 0)
            CASE_EQUAL(11)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_13"), 0, 0)
            CASE_DEFAULT
            SWITCH(GSW_STORY_PROGRESS)
                CASE_BETWEEN(STG3_20_02_BACK_TO_THE_MINORS, STG3_20_04_CONFISCATED)
                IF_INT_EQUAL(GSWF_TOU_04_CONFISCATED_EXIT_SEEN, 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_15"), 0, 0)
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_14"), 0, 0)
                END_IF
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_14"), 0, 0)
            END_SWITCH
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_065"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_066"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * NPC init for Brots (ブロッツ): he stands at (-160, 0, -50) facing 90 once Mario's ranking is 19
 * or worse (unless GSWF_TOU_RANK_18_KING_K_TALK_DONE has played), and always from STG3_34_RETURN_TO_ROGUEPORT on.
 */
static EVT_BEGIN(init_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 19)
        ELSE
            IF_INT_EQUAL(GSWF_TOU_RANK_18_KING_K_TALK_DONE, 0)
                CALL(evt_npc_set_position, STRING("me"), -160, 0, -50)
                CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
        END_IF
    ELSE
        CALL(evt_npc_set_position, STRING("me"), -160, 0, -50)
        CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER_OR_EQUAL(19)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_04_04"), 0, 0)
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_075"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_076"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for Gangan (ガンガン): placed at (-75, 0, 90) facing 90. */
EVT_BEGIN(init_gangan)
    CALL(evt_npc_set_position, STRING("me"), -75, 0, 90)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

/** Talk script of Gangan: story-dependent lines like talk_octo. */
EVT_BEGIN(talk_gangan)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(20)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_04"), 0, 0)
            CASE_EQUAL(19)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_05"), 0, 0)
            CASE_EQUAL(18)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_06"), 0, 0)
            CASE_EQUAL(17)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_07"), 0, 0)
            CASE_EQUAL(16)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_08"), 0, 0)
            CASE_EQUAL(15)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_09"), 0, 0)
            CASE_EQUAL(14)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_10"), 0, 0)
            CASE_EQUAL(13)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_11"), 0, 0)
            CASE_EQUAL(12)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_12"), 0, 0)
            CASE_EQUAL(11)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_13"), 0, 0)
            CASE_DEFAULT
            SWITCH(GSW_STORY_PROGRESS)
                CASE_BETWEEN(STG3_20_02_BACK_TO_THE_MINORS, STG3_20_04_CONFISCATED)
                IF_INT_EQUAL(GSWF_TOU_04_CONFISCATED_EXIT_SEEN, 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_15"), 0, 0)
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_14"), 0, 0)
                END_IF
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_14"), 0, 0)
            END_SWITCH
        END_SWITCH
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_070"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_071"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * NPC init for Poku (ポグ): before STG3_34_RETURN_TO_ROGUEPORT he is placed at (120, 0, -30) once the Armored
 * Harriers match opening has played (GSWF_TOU_ARMORED_HARRIERS_OPENING_SEEN), and at STG3_34_RETURN_TO_ROGUEPORT itself
 * unless Mario has left by blimp after the chapter (GSWF_TOU_LEFT_AFTER_CHAPTER); otherwise his position is left alone.
 * He faces 270.
 */
EVT_BEGIN(init_poku)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSWF_TOU_ARMORED_HARRIERS_OPENING_SEEN, 1)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        END_IF
        CASE_EQUAL(STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSWF_TOU_LEFT_AFTER_CHAPTER, 0)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        END_IF
    END_SWITCH
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

/**
 * Talk script of Poku: at rank 12 he calls himself a rookie (stg3_npc_10_05_12),
 * at rank 11 he recommends the Hot Dogs outside (05_13);
 * otherwise he says he takes notes on Mario's fights (05_14), or,
 * from STG3_20_02_BACK_TO_THE_MINORS to STG3_20_04_CONFISCATED until guard 1 has stepped aside,
 * asks if Mario is back in the minors (05_15).
 */
EVT_BEGIN(talk_poku)
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(12)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_12"), 0, 0)
        CASE_EQUAL(11)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_13"), 0, 0)
        CASE_DEFAULT
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_20_02_BACK_TO_THE_MINORS, STG3_20_04_CONFISCATED)
            IF_INT_EQUAL(GSWF_TOU_04_CONFISCATED_EXIT_SEEN, 0)
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_15"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_14"), 0, 0)
            END_IF
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_14"), 0, 0)
        END_SWITCH
    END_SWITCH
    RETURN
EVT_END()

/** NPC init for the first Iron Cleft: placed at (75, 0, -100) facing 270, scaled 1.5 in X and Y. */
EVT_BEGIN(init_iron)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 11)
            CALL(evt_npc_set_position, STRING("me"), 75, 0, -100)
        END_IF
    ELSE
        IF_INT_EQUAL(GSWF_TOU_LEFT_AFTER_CHAPTER, 0)
            CALL(evt_npc_set_position, STRING("me"), 75, 0, -100)
        END_IF
    END_IF
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    RETURN
EVT_END()

/** Talk script of the first Iron Cleft (stg3_npc_10_06_07, rematch lines stg3_npc_re_080/081). */
EVT_BEGIN(talk_iron)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_06_07"), 0, 0)
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_080"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_081"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** NPC init for the second Iron Cleft: placed at (100, 0, -80) facing 270, scaled 1.5 in X and Y. */
EVT_BEGIN(init_iron2)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 11)
            CALL(evt_npc_set_position, STRING("me"), 100, 0, -80)
        END_IF
    ELSE
        IF_INT_EQUAL(GSWF_TOU_LEFT_AFTER_CHAPTER, 0)
            CALL(evt_npc_set_position, STRING("me"), 100, 0, -80)
        END_IF
    END_IF
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    RETURN
EVT_END()

/** Talk script of the second Iron Cleft (stg3_npc_10_06_08, rematch lines). */
EVT_BEGIN(talk_iron2)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_06_08"), 0, 0)
    ELSE
        IF_INT_EQUAL(GSWF_TOU_REMATCH_DEMOTED_TO_MINORS, 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_080"), 0, 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_081"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * NPCs of the minor-league locker room: Jolene, the guard who fetches Mario, Grubba, Yoshi,
 * the fighters King K, Master Crash, Bandy Andy, Cleftor and Poku, and the two Iron Clefts.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "キノシコワ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kino, // initEvent
        NULL, // regularEvent
        talk_kino, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ガードマン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gard, // initEvent
        NULL, // regularEvent
        talk_gard, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ガンス", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ヨッシー", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "マイケル", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_michel, // initEvent
        NULL, // regularEvent
        talk_michel, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブロッツ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_brots, // initEvent
        NULL, // regularEvent
        talk_brots, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ガンガン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gangan, // initEvent
        NULL, // regularEvent
        talk_gangan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ポグ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_poku, // initEvent
        NULL, // regularEvent
        talk_poku, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "アイアン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_iron, // initEvent
        NULL, // regularEvent
        talk_iron, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "アイアン２", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_iron2, // initEvent
        NULL, // regularEvent
        talk_iron2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Scene after Mario's first match, against the Goomba Bros (opponent 19),
 * before STG3_34_RETURN_TO_ROGUEPORT: Grubba (ガンス) tells him to play to the crowd
 * if he won (rank now better than 20, stg3_tou_89) or scolds him for losing (93_1), and leaves.
 * Jolene (キノシコワ) pays the fight money, 3 coins after a win (stg3_tou_90 to 93) or 1 after a loss (93_2 to 93_4),
 * explains reserving the next match, and leaves.
 * The minor-league fighters (マイケル, オクトール, ブロッツ, ガンガン) then introduce themselves (stg3_tou_93_5 to 93_16)
 * and the story advances to STG3_07_MINORS_9TH_PL.
 */
EVT_BEGIN(evt_saisyo_after)
    // Mario, the partner, Grubba and Jolene are placed in the room after the first match.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("ガンス"), -45, 0, -95)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -15, 0, -90)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)

    // Grubba comments on the fight and walks out.
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_LESS(LW(0), 20)
        CALL(evt_msg_print, 0, STRING("stg3_tou_89"), 0, STRING("ガンス"))
    ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_1"), 0, STRING("ガンス"))
    END_IF
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("ガンス"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ガンス"), 0, -130, 0, FLOAT(80.0f), 1)
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガンス"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("ガンス"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // Jolene steps up to pay the fight money.
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -55, -95, 0, FLOAT(80.0f), 1)
    IF_INT_EQUAL(GSWF_TOU_GOOMBA_BROS_FIRST_WIN, 1)

        // After a win: 3 coins and how to reserve the next match.
        CALL(evt_msg_print, 0, STRING("stg3_tou_90"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_91"), 0, 0, 3)
#if VERSION >= VERSION_US
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_91"), 3)
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(evt_tou_ranking_msg)
        CALL(evt_pouch_add_coin, 3)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_92"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_93"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    ELSE

        // After a loss: 1 coin and her parting words.
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_2"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_93_3"), 0, 0, 1)
#if VERSION >= VERSION_US
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_93_3"), 1)
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(evt_tou_ranking_msg)
        CALL(evt_pouch_add_coin, 1)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_3_00"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_4"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    END_IF

    // Jolene leaves through the door.
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // Master Crash is put back on his spot, then King K welcomes Mario and introduces himself.
    CALL(evt_npc_get_reglid, STRING("オクトール"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_set_position, STRING("オクトール"), 80, 0, 85)
    CALL(evt_npc_set_ry, STRING("オクトール"), 270)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_5"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -20, 93, 290, -20, 57, -8, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("マイケル"), 40, -80, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_6"), 0, STRING("マイケル"))
    WAIT_MS(300)

    // He introduces Master Crash, who offers his advice.
    CALL(evt_cam3d_evt_set, 75, 107, 423, 75, 55, 128, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_7"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_8"), 0, STRING("オクトール"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_9"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_10"), 0, STRING("オクトール"))
    WAIT_MS(300)

    // Next comes Bandy Andy.
    CALL(evt_cam3d_evt_set, -160, 95, 315, -160, 37, -14, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_11"), 0, STRING("マイケル"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_12"), 0, STRING("ブロッツ"))
    WAIT_MS(300)

    // Last is Cleftor, who wants nothing to do with Mario.
    CALL(evt_cam3d_evt_set, -60, 107, 423, -60, 55, 128, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_13"), 0, STRING("マイケル"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_reverse_ry, STRING("ガンガン"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_14"), 0, STRING("ガンガン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_15"), 0, STRING("マイケル"))
    WAIT_MS(300)

    // King K sums up the crew, goes back to his spot and the story advances.
    CALL(evt_cam3d_evt_set, -35, 96, 315, -35, 57, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_16"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("マイケル"), 120, -30, 0, FLOAT(60.0f), 1)
    CALL(evt_set_dir_to_target, STRING("マイケル"), STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, STG3_07_MINORS_9TH_PL)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_restart_regular_event, STRING("オクトール"))
    RETURN
EVT_END()

EVT_BEGIN(evt_saisyo_after_re)
    // Mario, the partner and Jolene are placed in the room after the comeback's first match.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -55, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)

    // Jolene's verdict and the fight money depend on whether Mario won.
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_LESS(LW(0), 20)

        // A win: the crowd needed more excitement, and 3 coins.
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_028"), 0, STRING("キノシコワ"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_029"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_re_030"), 0, 0, 3)
#if VERSION >= VERSION_US
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_re_030"), 3)
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(evt_tou_ranking_msg)
        CALL(evt_pouch_add_coin, 3)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_031"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    ELSE

        // A loss: retire or reclaim his glory, and 1 coin.
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_032"), 0, STRING("キノシコワ"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_033"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_re_034"), 0, 0, 1)
#if VERSION >= VERSION_US
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_re_034"), 1)
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(evt_tou_ranking_msg)
        CALL(evt_pouch_add_coin, 1)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_035"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    END_IF

    // Jolene bows and leaves through the door, and control returns.
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Scene where the hatched Yoshi joins (called from tou.c):
 * his color is chosen and the matching baby Yoshi NPC is created.
 * The partner notices the egg is gone (stg3_tou_278),
 * Yoshi appears and thanks Mario for hatching him safely (279, 280),
 * wants to help win the Crystal Star (282) and asks for a name (283).
 * The name entry window opens (name_check replaces a blank name), Yoshi approves the name (284),
 * it is stored with evt_pouch_set_yoshiname and Yoshi joins as partner 4 (all_party_lecture).
 */
EVT_BEGIN(evt_tou_chibi_yoshi)
    // Yoshi's colour comes from the time since the egg was received and the matching baby Yoshi is created off-screen.
    CALL(setYoshiColor)
    CALL(getYoshiColor, LW(0))
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
    CALL(evt_npc_check, STRING("ヨッシー"), LW(10))
    IF_INT_NOT_EQUAL(LW(10), 0)
        CALL(evt_npc_delete, STRING("ヨッシー"))
    END_IF
    CALL(evt_npc_entry, STRING("ヨッシー"), LW(0))
    CALL(evt_npc_set_position, STRING("ヨッシー"), 0, -1000, 0)
    CALL(evt_npc_set_tribe, STRING("ヨッシー"), STRING("ヨッシー"))
    CALL(evt_npc_flag_onoff, 1, STRING("ヨッシー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)

    // The partner notices the egg is gone and looks around; Yoshi appears and says "Yo!".
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_msg_print_party, STRING("stg3_tou_278"))
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(800)
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(800)
    CALL(evt_npc_set_position, STRING("ヨッシー"), 100, 0, -100)
    CALL(evt_set_dir_to_target, STRING("ヨッシー"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_279"), 0, STRING("ヨッシー"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)

    // Everyone turns as Yoshi walks up and thanks Mario for hatching him.
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_cam3d_evt_set, 60, 91, 266, 60, 57, -11, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 50)
        CALL(evt_npc_move_position, STRING("ヨッシー"), LW(0), LW(2), 0, FLOAT(70.0f), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_280"), 0, STRING("ヨッシー"))
    WAIT_MS(300)

    // Mario is startled, the partner reacts, and Yoshi offers to help win the Crystal Star
    // and asks for a name.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("stg3_tou_281"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_282"), 0, STRING("ヨッシー"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(600)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_283"), 0, STRING("ヨッシー"))
    WAIT_MS(300)

    // The name entry window unrolls; a blank name becomes "Yoshi".
    CALL(evt_paper_entry, STRING("OFF_d_roll"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_roll"))
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 1)
    WAIT_FRAMES(1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_win_nameent_on, NAMEENT_YOSHI)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_NAME_ENTRY1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_wait)
    CALL(evt_win_nameent_name, LW(0))
    CALL(name_check, LW(0))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF3_NAME_ENTRY2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_off)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_roll"))
    CALL(evt_cam_letter_box_disable, 0)

    // Yoshi approves the name, which is saved, and he joins the party.
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_284"), 0, STRING("ヨッシー"), LW(0))
    WAIT_MS(300)
    CALL(evt_mario_clear_party, 1)
    CALL(evt_cam3d_evt_set, -35, 70, 180, -35, 50, -10, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_pouch_set_yoshiname, LW(0))
    SET_VALUE(LW(8), STRING("ヨッシー"))
    SET_VALUE(LW(9), 4)
    CALL_CHILD(all_party_lecture)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_1st_leagu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -45, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    IF_INT_EQUAL(GSWF_GLITZ_PIT_BEAT_LAST_OPPONENT, 1)
        SET_VALUE(LW(10), 7)
    ELSE
        SET_VALUE(LW(10), 1)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg3_tou_179_00"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
    CALL(evt_sub_get_language, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
#endif
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_179_01"), 0, STRING("キノシコワ"), LW(10))
#if VERSION >= VERSION_US
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_179_01"), LW(10))
        CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("キノシコワ"))
    END_SWITCH
#endif
    CALL_CHILD(evt_tou_ranking_msg)
    CALL(evt_pouch_add_coin, LW(10))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_286"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_mario_mov_pos2, 0, -130, FLOAT(60.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 0, -200, FLOAT(60.0f))
    END_INLINE_EVENT
    SET_VALUE(GSWF_TOU_PROMOTION_TO_MAJORS_PENDING, 1)
    CALL(evt_bero_mapchange, STRING("tou_05"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_1st_leagu_re)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -45, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, EASING_SINE_IN_OUT)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    IF_INT_EQUAL(GSWF_GLITZ_PIT_BEAT_LAST_OPPONENT, 1)
        SET_VALUE(LW(10), 6)
    ELSE
        SET_VALUE(LW(10), 1)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg3_tou_179_00"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION >= VERSION_US
    CALL(evt_sub_get_language, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
#endif
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_179_01"), 0, 0, LW(10))
#if VERSION >= VERSION_US
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_179_01"), LW(10))
        CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
    END_SWITCH
#endif
    CALL_CHILD(evt_tou_ranking_msg)
    CALL(evt_pouch_add_coin, LW(10))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_085"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSWF_TOU_REMATCH_PROMOTED_TO_MAJORS, 1)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Locker room door script: the staff member (キノシコワ) stops Mario at the door with stg3_tou_67
 * if no match is reserved (GSWF_GLITZ_PIT_MATCH_RESERVED clear), or stg3_tou_73_01 if one is.
 */
static EVT_BEGIN(evt_door)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_67"), 0, STRING("キノシコワ"))
    ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_73_01"), 0, STRING("キノシコワ"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Scene on Mario's first arrival, before STG3_05_MATCH:
 * Jolene (キノシコワ) opens the door and brings Mario and the partner in,
 * tells him this is his locker room and that he starts in the minor league (stg3_tou_63, 64),
 * and shows him the match terminal, where Grubba picks his opponents (65).
 * The door is registered to evt_door so Mario cannot leave yet.
 */
EVT_BEGIN(evt_rec)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, -180)
    CALL(evt_party_set_pos, 0, 0, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_cam3d_evt_set, -150, 133, 533, -150, 37, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("n_bero"), 0, PTR(evt_door), 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_off, 3000, EASING_LINEAR)
    WAIT_MS(3000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 1, -120, 0, FLOAT(80.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 0, -90, FLOAT(120.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, -15, -70, FLOAT(120.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_set_dir_npc, 0, STRING("キノシコワ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 0, -85, FLOAT(120.0f))
        CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("キノシコワ"), 40, -100, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_63"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 20, 104, 238, 20, 60, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg3_tou_64"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -20, 104, 238, -20, 60, -8, 1000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 270, 300, 0)
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -40, -100, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("キノシコワ"), NPC_FLAG_NO_HIT)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg3_tou_65"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Spawned once the first match is reserved: the early demos first perform 241 one-frame waits, pausing each iteration
 * while Mario's input is disabled, then wait for an idle, walking or dashing motion.
 * JP and later wait for 241 eligible idle, walking or dashing checks with input and control enabled.
 * If Mario stands by the door (x within 35, z at or below -60) he is bounced back to (40, 0, -120);
 * the guard (ガードマン) opens the door, walks in and closes it, calls Mario to his match (stg3_tou_74),
 * and Jolene says Security is here to escort him (75). Control then returns to Mario.
 */
static EVT_BEGIN(evt_gardman)
    // Lock input, wait for landing and, in US and later, also wait for talking to end.
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(0), 0)
    LOOP(0)
        ADD(LW(0), 1)
        WAIT_FRAMES(1)
        CALL(evt_mario_wait_movable)
        IF_INT_GREATER(LW(0), 240)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        CALL(evt_mario_get_motion, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(LW(0), 2)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
    WAIT_FRAMES(1)
#else
    CALL(tou_10_mario_free_wait)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
#if VERSION >= VERSION_US
    WAIT_FRAMES(10)
    CALL(tou_10_mario_talk_end_wait)
    WAIT_FRAMES(10)
#else
    WAIT_FRAMES(1)
#endif
#endif

    // If Mario stands in the doorway, he is startled and hops back out of the way.
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(2), -60)
            RETURN
        END_IF
        IF_INT_LESS(LW(0), -35)
            RETURN
        END_IF
        IF_INT_GREATER(LW(0), 35)
            RETURN
        END_IF
        CALL(evt_mario_normalize)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_jump_pos, 40, 0, -120, 400, 1, -1000)
        CALL(evt_npc_flag_onoff, 0, STRING("ガードマン"), NPC_FLAG_NO_HIT)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
        CALL(evt_mario_set_dir_npc, STRING("ガードマン"))
    END_INLINE_EVENT

    // The guard opens the door, walks in and the door closes behind him.
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 0, STRING("ガードマン"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_set_dir_to_target, STRING("ガードマン"), STRING("mario"))
    CALL(evt_npc_set_position, STRING("ガードマン"), 0, 0, -170)
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -90, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // The guard calls Mario to his match and Jolene says Security will escort him.
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_74"), 0, STRING("ガードマン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_75"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("ガードマン"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 0, STRING("ガードマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_match_make_sub)
    CALL(evt_cam3d_evt_set, -75, 91, 188, -75, 57, -1, 1000, EASING_SINE_IN_OUT)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -130, -90, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, 100)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, -90, -96, 500)
    CALL(evt_mario_mov_pos, -87, -99, 500)
    CALL_CHILD(evt_tou_monitor_on)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("gamen"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg3_tou_69"), 0, STRING("ガンス"))
    WAIT_MS(300)
    SET_VALUE(GSW_GLITZ_PIT_CONDITION, 43)
    SET_VALUE(GSW_GLITZ_PIT_CONDITION_PARAM, 1)
    SET_VALUE(GSW_GLITZ_PIT_CONDITION_PARAM2, 1)
    CALL_CHILD(evt_tou_monitor_off)
    CALL(evt_cam3d_evt_set, -40, 91, 188, -40, 57, -1, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    LOOP(0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_70"), 0, STRING("キノシコワ"))
        CALL(evt_msg_select, 0, STRING("stg3_tou_70_yn"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("stg3_tou_72"))
            BREAK_LOOP
        END_IF
        CALL(evt_msg_print_add, 0, STRING("stg3_tou_71"))
    END_LOOP
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GF(0), 1)
    CALL(evt_party_run, 0)
    SPAWN_EVENT(evt_gardman)
    SET_VALUE(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
    SET_VALUE(GSW_GLITZ_PIT_OPPONENT, 19)
    RETURN
EVT_END()

/**
 * Registration console interaction: the partner steps to the console and the prompt (stg3_tou_103) offers
 * match registration. Confirming an already reserved match prints its opponent and rule (stg3_tou_104_01);
 * otherwise the first-match booking helper brings Mario to the console.
 */
EVT_BEGIN(evt_match_make)
#if VERSION >= VERSION_JP
    CALL(tou_10_mario_keyoff_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
#endif
    CALL(evt_hit_get_position, STRING("M_make"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_TOUROKU_COM1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -90, -110, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg3_tou_103"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg3_tou_103_yn"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
            CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
            CALL(evt_tou_get_rule_msg2, GSW_GLITZ_PIT_CONDITION, LW(2))
            CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add_insert, 0, STRING("stg3_tou_104_01"), LW(0), LW(2))
        ELSE
            CALL(evt_msg_continue)
            CALL_CHILD(evt_match_make_sub)
        END_IF
        CASE_EQUAL(1)
        CALL(evt_msg_continue)
        CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
        CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
        IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_68"), 0, STRING("キノシコワ"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_73_02"), 0, STRING("キノシコワ"))
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_continue)
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Bed entry point: registers evt_tou_bed to run when Mario presses A in front of the bed named in LW(0). */
static EVT_BEGIN(bed_entry)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_tou_bed), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_okorareru)
    // Jolene waits in the room until Mario comes back out through the broken wall.
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, 0, -85)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -270)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // The music fades and she scolds Mario: major leaguers may not come in here, let alone bash walls.
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_16"), 0, STRING("キノシコワ"))
    CALL(evt_cam3d_evt_set, 0, 100, 287, 0, 55, -10, 1000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -200, 90, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(100)
    CALL(evt_party_move_pos2, 0, -225, 110, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 90, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_17"), 0, STRING("キノシコワ"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_cam3d_evt_off, 2000, EASING_SINE_IN_OUT)

    // She walks over and confiscates the Dubious Paper.
    CALL(evt_npc_move_position, STRING("キノシコワ"), -155, -55, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -161, 90, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_19"), 0, STRING("キノシコワ"))
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 15)
    ADD(LW(2), -1)
    CALL(evt_item_entry, STRING("fmoney"), ITEM_DUBIOUS_PAPER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_pouch_remove_item, ITEM_DUBIOUS_PAPER, LW(0))

    // She leaves through the door.
    CALL(evt_npc_move_position, STRING("キノシコワ"), -155, -55, 0, FLOAT(70.0f), 1)
    CALL(evt_cam3d_evt_set, 0, 100, 287, 0, 55, -10, 2000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -120, 0, FLOAT(70.0f), 1)
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
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(n_door_open)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // The partner grumbles about the lost document but guesses the Crystal Star is in the arena.
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_20"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -183, 85, 386, -183, 25, -10, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_21"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG3_TOU1"), 2000)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    RETURN
EVT_END()

EVT_BEGIN(evt_syorui)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_item_get_item_end_wait, STRING("syorui"))
    WAIT_MS(300)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -405, 75, FLOAT(80.0f))
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -365, 75, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_L_2"))
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_14"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -385, 80, 337, -385, 27, -9, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_15"))
    WAIT_MS(300)
    SPAWN_EVENT(evt_okorareru)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(first_tamago)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -30, -90, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_move_pos2, 0, 30, -90, FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_256"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(800)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_block)
    SET_VALUE(GSW_STORY_PROGRESS, STG3_20_03_THE_YELLOW_BLOCK)
    RETURN
EVT_END()

EVT_BEGIN(tou_10_init_evt)
    // Music for the story stage, and the story jump to the Yoshi egg stage (GSWF_TOU_10_JUMP_STORY_TO_EGG).
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG3_00_WELCOME_TO_GLITZVILLE, STG3_12_THE_MINI_YOSHI_EGG)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
#else
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
#endif
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_GREATER_OR_EQUAL(STG3_33_PEACH_THE_SECRET_WEAPON)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
#else
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
#endif
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
    END_SWITCH
    IF_INT_EQUAL(GSWF_TOU_10_JUMP_STORY_TO_EGG, 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_12_THE_MINI_YOSHI_EGG)
            SET_VALUE(GSW_STORY_PROGRESS, STG3_12_THE_MINI_YOSHI_EGG)
        END_IF
    END_IF

    // The door, lockers, the fighters, Master Crash's talk hook, and the match terminal and monitor.
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(evt_tou_locker_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("fuku_M"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(init_octo)
    CALL(evt_npc_change_interrupt, STRING("オクトール"), 6, PTR(talk_octo))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("isu"), MAP_OBJ_FLAG_CLIP)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_06_1ST_MATCH)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("M_make"), 0, PTR(evt_match_make), 0)
    ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("M_make"), 0, PTR(evt_tou_match_make_default), 0)
    END_IF
    CALL_CHILD(evt_tou_monitor_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_03_THE_YELLOW_BLOCK)
        CALL(evt_mobj_lv_blk, STRING("blk"), -280, 0, 100, PTR(evt_block), GSWF_TOU_10_BIG_BIG_BLOCK_BROKEN, STRING("MOBJ_Lv1BigBigBlock"))
        CALL(evt_mobj_flag_onoff, 1, STRING("blk"), MOBJ_FLAG_NO_CULL)
    END_IF

    // First visit: Jolene's welcome scene, with the door locked.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_05_MATCH)
        SPAWN_EVENT(evt_rec)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(bero_case_switch_off)
        RETURN
    END_SWITCH

    // After a match: Goombella, Koops and Flurrie rejoin if the cake poisoning was never cured,
    // then the matching after-match scene plays.
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
        IF_INT_EQUAL(GSWF_TOU_POISON_CAKE_EATEN, 1)
            IF_INT_EQUAL(GSWF_TOU_POISON_CAKE_RECOVERED, 0)
                CALL(evt_pouch_party_join, 1)
                CALL(evt_pouch_party_join, 2)
                CALL(evt_pouch_party_join, 5)
                CALL(evt_party_run, 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 1)
        SET_VALUE(GSWF_GLITZ_PIT_MATCH_JUST_ENDED, 0)
        SWITCH(GSW_GLITZ_PIT_OPPONENT)
            CASE_EQUAL(19)
            IF_INT_EQUAL(GSWF_TOU_GOOMBA_BROS_AFTER_MATCH_SEEN, 0)
                SET_VALUE(GSWF_TOU_GOOMBA_BROS_AFTER_MATCH_SEEN, 1)
                IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
                    SPAWN_EVENT(evt_saisyo_after)
                ELSE
                    SPAWN_EVENT(evt_saisyo_after_re)
                END_IF
            ELSE
                SPAWN_EVENT(evt_tou_match_after_default)
            END_IF
            CASE_EQUAL(10)
            CALL(evt_tou_get_ranking, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 10)
                IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
                    SPAWN_EVENT(evt_1st_leagu)
                ELSE
                    SPAWN_EVENT(evt_1st_leagu_re)
                END_IF
            ELSE
                SPAWN_EVENT(evt_tou_match_after_default)
            END_IF
            CASE_DEFAULT
            SPAWN_EVENT(evt_tou_match_after_default)
        END_SWITCH
    END_IF

    // The bed, the Dubious Paper until Jolene confiscates it, and the one-time partner pep talk after two losses to the Armored Harriers.
    CALL(evt_hitobj_name_callback, STRING("a_b_H"), PTR(bed_entry))
    CALL(evt_sub_animgroup_async, STRING("p_slit"))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_20_04_CONFISCATED)
        CALL(evt_item_entry, STRING("syorui"), ITEM_DUBIOUS_PAPER, -475, 90, 75, ITEM_TYPE_STILL, -1, PTR(evt_syorui))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_12_THE_MINI_YOSHI_EGG)
        IF_INT_EQUAL(GSWF_TOU_10_HARRIERS_PEP_TALK_CHECKED, 0)
            IF_INT_GREATER_OR_EQUAL(GSW_TOU_ARMORED_HARRIERS_LOSS_COUNT, 2)
                SPAWN_EVENT(first_tamago)
            END_IF
        END_IF
        SET_VALUE(GSWF_TOU_10_HARRIERS_PEP_TALK_CHECKED, 1)
    END_IF
    RETURN
EVT_END()

// Only the single full-width-space placeholder selects the default localized name.
USER_FUNC(name_check) {
    EvtWord* args = event->args;

    // a full-width space
    if (strcmp((const char*)evtGetValue(event, args[0]), "　") == 0) {
        evtSetValue(event, args[0], (EvtWord)msgSearch("name_party3"));
    }
    return EVT_RETURN_DONE;
}
