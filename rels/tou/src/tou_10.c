#include "battle/battle_unit.h"
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

#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "driver/swdrv.h"
#include "driver/msgdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "tou.h"
#include "evt/evt_npc.h"

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_party_join);
USER_FUNC(evt_tou_get_fighter_info);
USER_FUNC(evt_tou_get_ranking);
USER_FUNC(evt_tou_get_rule_msg2);
USER_FUNC(evt_win_nameent_name);
USER_FUNC(evt_win_nameent_off);
USER_FUNC(evt_win_nameent_on);
USER_FUNC(evt_win_nameent_wait);
static USER_FUNC(getYoshiColor);
USER_FUNC(name_check);
USER_FUNC(setYoshiColor);
USER_FUNC(tou_10_mario_keyoff_check);
USER_FUNC(tou_10_mario_free_wait);
#ifdef VERSION_G8ME01
#endif

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
#ifdef VERSION_G8ME01
EvtWord evt_saisyo_after[733];
#else
EvtWord evt_saisyo_after[691];
#endif
#ifdef VERSION_G8ME01
EvtWord evt_saisyo_after_re[367];
#else
EvtWord evt_saisyo_after_re[325];
#endif
EvtWord evt_tou_chibi_yoshi[424];
#ifdef VERSION_G8ME01
static EvtWord evt_1st_leagu[235];
#else
static EvtWord evt_1st_leagu[214];
#endif
#ifdef VERSION_G8ME01
EvtWord evt_1st_leagu_re[249];
#else
EvtWord evt_1st_leagu_re[228];
#endif
static EvtWord evt_door[25];
EvtWord evt_rec[274];
#ifdef VERSION_G8ME01
static EvtWord evt_gardman[202];
#else
static EvtWord evt_gardman[198];
#endif
EvtWord evt_match_make_sub[138];
EvtWord evt_match_make[116];
static EvtWord bed_entry[10];
EvtWord evt_okorareru[301];
EvtWord evt_syorui[92];
EvtWord first_tamago[52];
static EvtWord evt_block[5];
EvtWord tou_10_init_evt[254];

USER_FUNC(tou_10_mario_keyoff_check) {
    EvtWord* args = event->args;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
s32 tou_10_mario_talk_end_wait(void) {
    return marioGetPtr()->currMotionId != MARIO_MOTION_TALK ? 2 : 0;
}
#endif
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

static USER_FUNC(getYoshiColor) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], pouchGetPartyColor(PARTNER_YOSHI));
    return EVT_RETURN_DONE;
}

USER_FUNC(setYoshiColor) {
    OSTime start;
    s32 i;
    s32 minutes;

    // the time the Yoshi egg was received
    for (i = 0; i < 8; i++) {
        ((u8*)&start)[i] = swByteGet(i + 508);
    }
    minutes = (s32)OSTicksToSeconds(gp->retraceLocalTime - start) / 60;
    minutes %= 20;
    if (minutes <= 5) {
        pouchSetPartyColor(PARTNER_YOSHI, 0);
    } else if (minutes <= 8) {
        pouchSetPartyColor(PARTNER_YOSHI, 1);
    } else if (minutes <= 11) {
        pouchSetPartyColor(PARTNER_YOSHI, 2);
    } else if (minutes <= 14) {
        pouchSetPartyColor(PARTNER_YOSHI, 3);
    } else if (minutes <= 17) {
        pouchSetPartyColor(PARTNER_YOSHI, 4);
    } else if (minutes <= 18) {
        pouchSetPartyColor(PARTNER_YOSHI, 5);
    } else {
        pouchSetPartyColor(PARTNER_YOSHI, 6);
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(room_out)
    SET_VALUE(GSWF(2390), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "n_bero", // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
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

EVT_BEGIN(init_kino)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(132)
        CALL(evt_npc_set_position, STRING("me"), 0, 0, -160)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_kino)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(132)
        IF_INT_EQUAL(GF(0), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_66"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_73"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_gard)
    RETURN
EVT_END()

EVT_BEGIN(talk_gard)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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

static EVT_BEGIN(init_michel)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(172)
        IF_INT_EQUAL(GSWF(2412), 0)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
            CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_michel)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
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
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_060"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_061"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_octo)
    CALL(evt_npc_set_position, STRING("オクトール"), 80, 0, 85)
    CALL(evt_npc_status_onoff, 1, STRING("オクトール"), 2)
    CALL(evt_npc_set_ry, STRING("オクトール"), 270)
    RETURN
EVT_END()

EVT_BEGIN(talk_octo)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
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
                CASE_BETWEEN(141, 143)
                IF_INT_EQUAL(GSWF(2517), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_15"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_14"), 0, 0)
                END_IF
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_02_14"), 0, 0)
            END_SWITCH
        END_SWITCH
        ELSE
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_065"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_066"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(init_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 19)
            ELSE
            IF_INT_EQUAL(GSWF(2406), 0)
                CALL(evt_npc_set_position, STRING("me"), -160, 0, -50)
                CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
        END_IF
        ELSE
        CALL(evt_npc_set_position, STRING("me"), -160, 0, -50)
        CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER_OR_EQUAL(19)
            CALL(evt_msg_print, 0, STRING("stg3_npc_10_04_04"), 0, 0)
        END_SWITCH
        ELSE
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_075"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_076"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_gangan)
    CALL(evt_npc_set_position, STRING("me"), -75, 0, 90)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

EVT_BEGIN(talk_gangan)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
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
                CASE_BETWEEN(141, 143)
                IF_INT_EQUAL(GSWF(2517), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_15"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_14"), 0, 0)
                END_IF
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg3_npc_10_03_14"), 0, 0)
            END_SWITCH
        END_SWITCH
        ELSE
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_070"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_071"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_poku)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(172)
        IF_INT_EQUAL(GSWF(2397), 1)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        END_IF
        CASE_EQUAL(172)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING("me"), 120, 0, -30)
        END_IF
    END_SWITCH
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    RETURN
EVT_END()

EVT_BEGIN(talk_poku)
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(12)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_12"), 0, 0)
        CASE_EQUAL(11)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_05_13"), 0, 0)
        CASE_DEFAULT
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(141, 143)
            IF_INT_EQUAL(GSWF(2517), 0)
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

EVT_BEGIN(init_iron)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 11)
            CALL(evt_npc_set_position, STRING("me"), 75, 0, -100)
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING("me"), 75, 0, -100)
        END_IF
    END_IF
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    RETURN
EVT_END()

EVT_BEGIN(talk_iron)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_06_07"), 0, 0)
        ELSE
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_080"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_081"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_iron2)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS(LW(0), 11)
            CALL(evt_npc_set_position, STRING("me"), 100, 0, -80)
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING("me"), 100, 0, -80)
        END_IF
    END_IF
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_set_scale, STRING("me"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    RETURN
EVT_END()

EVT_BEGIN(talk_iron2)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_msg_print, 0, STRING("stg3_npc_10_06_08"), 0, 0)
        ELSE
        IF_INT_EQUAL(GSWF(2505), 0)
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_080"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_npc_re_081"), 0, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"キノシコワ"*/"\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F", // name
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
        /*"ガードマン"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93", // name
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
        /*"ガンス"*/"\x83\x4B\x83\x93\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ヨッシー"*/"\x83\x88\x83\x62\x83\x56\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マイケル"*/"\x83\x7D\x83\x43\x83\x50\x83\x8B", // name
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
        /*"ブロッツ"*/"\x83\x75\x83\x8D\x83\x62\x83\x63", // name
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
        /*"ガンガン"*/"\x83\x4B\x83\x93\x83\x4B\x83\x93", // name
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
        /*"ポグ"*/"\x83\x7C\x83\x4F", // name
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
        /*"アイアン"*/"\x83\x41\x83\x43\x83\x41\x83\x93", // name
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
        /*"アイアン２"*/"\x83\x41\x83\x43\x83\x41\x83\x93\x82\x51", // name
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

EVT_BEGIN(evt_saisyo_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("ガンス"), -45, 0, -95)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -15, 0, -90)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, 11)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_LESS(LW(0), 20)
        CALL(evt_msg_print, 0, STRING("stg3_tou_89"), 0, STRING("ガンス"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_1"), 0, STRING("ガンス"))
    END_IF
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("ガンス"), 2)
    CALL(evt_npc_move_position, STRING("ガンス"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -55, -95, 0, FLOAT(80.0f), 1)
    IF_INT_EQUAL(GSWF(2396), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_90"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_91"), 0, 0, 3)
#ifdef VERSION_G8ME01
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_91"), 3)
            CALL(evt_msg_print, 1, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(PTR(evt_tou_ranking_msg))
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
        CALL(evt_msg_print, 0, STRING("stg3_tou_93_2"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_93_3"), 0, 0, 1)
#ifdef VERSION_G8ME01
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_93_3"), 1)
            CALL(evt_msg_print, 1, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(PTR(evt_tou_ranking_msg))
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
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_get_reglid, STRING("オクトール"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_set_position, STRING("オクトール"), 80, 0, 85)
    CALL(evt_npc_set_ry, STRING("オクトール"), 270)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_5"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -20, 93, 290, -20, 57, -8, 1000, 11)
    CALL(evt_npc_move_position, STRING("マイケル"), 40, -80, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_6"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 75, 107, 423, 75, 55, 128, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_7"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_8"), 0, STRING("オクトール"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_9"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_10"), 0, STRING("オクトール"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 95, 315, -160, 37, -14, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_11"), 0, STRING("マイケル"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_12"), 0, STRING("ブロッツ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -60, 107, 423, -60, 55, 128, 1000, 11)
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
    CALL(evt_cam3d_evt_set, -35, 96, 315, -35, 57, -8, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_93_16"), 0, STRING("マイケル"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_move_position, STRING("マイケル"), 120, -30, 0, FLOAT(60.0f), 1)
    CALL(evt_set_dir_to_target, STRING("マイケル"), STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 134)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_restart_regular_event, STRING("オクトール"))
    RETURN
EVT_END()

EVT_BEGIN(evt_saisyo_after_re)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -55, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, 11)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_LESS(LW(0), 20)
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_028"), 0, STRING("キノシコワ"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_029"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_re_030"), 0, 0, 3)
#ifdef VERSION_G8ME01
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_re_030"), 3)
            CALL(evt_msg_print, 1, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(PTR(evt_tou_ranking_msg))
        CALL(evt_pouch_add_coin, 3)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_031"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_032"), 0, STRING("キノシコワ"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_033"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("fmoney"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(15))
        SWITCH(LW(15))
            CASE_EQUAL(0)
#endif
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_re_034"), 0, 0, 1)
#ifdef VERSION_G8ME01
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_re_034"), 1)
            CALL(evt_msg_print, 1, LW(14), 0, 0)
        END_SWITCH
#endif
        CALL_CHILD(PTR(evt_tou_ranking_msg))
        CALL(evt_pouch_add_coin, 1)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_035"), 0, STRING("キノシコワ"))
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
        WAIT_MS(600)
        CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    END_IF
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_tou_chibi_yoshi)
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
    CALL(evt_eff_fukidashi, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_cam3d_evt_set, 60, 91, 266, 60, 57, -11, 1000, 11)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 50)
        CALL(evt_npc_move_position, STRING("ヨッシー"), LW(0), LW(2), 0, FLOAT(70.0f), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, 11)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_280"), 0, STRING("ヨッシー"))
    WAIT_MS(300)
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
    CALL(evt_win_nameent_on, 1)
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
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_284"), 0, STRING("ヨッシー"), LW(0))
    WAIT_MS(300)
    CALL(evt_mario_clear_party, 1)
    CALL(evt_cam3d_evt_set, -35, 70, 180, -35, 50, -10, 800, 11)
    WAIT_MS(800)
    CALL(evt_pouch_set_yoshiname, LW(0))
    SET_VALUE(LW(8), STRING("ヨッシー"))
    SET_VALUE(LW(9), 4)
    CALL_CHILD(PTR(all_party_lecture))
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_1st_leagu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -45, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, 11)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    IF_INT_EQUAL(GSWF(2442), 1)
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
    CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
#endif
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_179_01"), 0, STRING("キノシコワ"), LW(10))
#ifdef VERSION_G8ME01
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_179_01"), LW(10))
        CALL(evt_msg_print, 1, LW(14), 0, STRING("キノシコワ"))
    END_SWITCH
#endif
    CALL_CHILD(PTR(evt_tou_ranking_msg))
    CALL(evt_pouch_add_coin, LW(10))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_286"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SET_VALUE(GSWF(2393), 1)
    CALL(evt_bero_mapchange, STRING("tou_05"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_1st_leagu_re)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -90, 0, -95)
    CALL(evt_party_set_pos, 0, -120, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -45, 0, -95)
    CALL(evt_cam3d_evt_set, -75, 91, 266, -75, 57, -11, 0, 11)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    IF_INT_EQUAL(GSWF(2442), 1)
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
    CALL(evt_item_entry, STRING("fmoney"), ITEM_COIN, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(15))
    SWITCH(LW(15))
        CASE_EQUAL(0)
#endif
    CALL(evt_msg_print_insert, 0, STRING("stg3_tou_179_01"), 0, 0, LW(10))
#ifdef VERSION_G8ME01
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("stg3_tou_179_01"), LW(10))
        CALL(evt_msg_print, 1, LW(14), 0, 0)
    END_SWITCH
#endif
    CALL_CHILD(PTR(evt_tou_ranking_msg))
    CALL(evt_pouch_add_coin, LW(10))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_085"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSWF(2504), 1)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

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

EVT_BEGIN(evt_rec)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, -180)
    CALL(evt_party_set_pos, 0, 0, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_cam3d_evt_set, -150, 133, 533, -150, 37, -8, 0, 11)
    CALL(evt_run_case_evt, 9, 1, STRING("n_bero"), 0, PTR(evt_door), 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_off, 3000, 0)
    WAIT_MS(3000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_63"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 20, 104, 238, 20, 60, -8, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg3_tou_64"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -20, 104, 238, -20, 60, -8, 1000, 11)
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
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_gardman)
    CALL(tou_10_mario_free_wait)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
#ifdef VERSION_G8ME01
    WAIT_FRAMES(10)
    CALL(tou_10_mario_talk_end_wait)
    WAIT_FRAMES(10)
#else
    WAIT_FRAMES(1)
#endif
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
        CALL(evt_mario_dispflag_onoff, 1, 2)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_jump_pos, 40, 0, -120, 400, 1, -1000)
        CALL(evt_npc_flag_onoff, 0, STRING("ガードマン"), NPC_FLAG_NO_HIT)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_dispflag_onoff, 0, 2)
        CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
        CALL(evt_mario_set_dir_npc, STRING("ガードマン"))
    END_INLINE_EVENT
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 0, STRING("ガードマン"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_set_dir_to_target, STRING("ガードマン"), STRING("mario"))
    CALL(evt_npc_set_position, STRING("ガードマン"), 0, 0, -170)
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -90, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_74"), 0, STRING("ガードマン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_75"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_status_onoff, 1, STRING("ガードマン"), 2)
    CALL(evt_npc_flag_onoff, 0, STRING("ガードマン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_match_make_sub)
    CALL(evt_cam3d_evt_set, -75, 91, 188, -75, 57, -1, 1000, 11)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -130, -90, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, 100)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, -90, -96, 500)
    CALL(evt_mario_mov_pos, -87, -99, 500)
    CALL_CHILD(PTR(evt_tou_monitor_on))
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("gamen"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg3_tou_69"), 0, STRING("ガンス"))
    WAIT_MS(300)
    SET_VALUE(GSW_GLITZ_PIT_CONDITION, 43)
    SET_VALUE(GSW(503), 1)
    SET_VALUE(GSW(504), 1)
    CALL_CHILD(PTR(evt_tou_monitor_off))
    CALL(evt_cam3d_evt_set, -40, 91, 188, -40, 57, -1, 500, 11)
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
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GF(0), 1)
    CALL(evt_party_run, 0)
    SPAWN_EVENT(PTR(evt_gardman))
    SET_VALUE(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
    SET_VALUE(GSW_GLITZ_PIT_OPPONENT, 19)
    RETURN
EVT_END()

EVT_BEGIN(evt_match_make)
    CALL(tou_10_mario_keyoff_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
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
            CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add_insert, 0, STRING("stg3_tou_104_01"), LW(0), LW(2))
            ELSE
            CALL(evt_msg_continue)
            CALL_CHILD(PTR(evt_match_make_sub))
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

static EVT_BEGIN(bed_entry)
    CALL(evt_run_case_evt, 9, 1, LW(0), 0, PTR(evt_tou_bed), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_okorareru)
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, 0, -85)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -270)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_16"), 0, STRING("キノシコワ"))
    CALL(evt_cam3d_evt_set, 0, 100, 287, 0, 55, -10, 1000, 11)
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
    CALL(evt_cam3d_evt_off, 2000, 11)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -155, -55, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -161, 90, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_19"), 0, STRING("キノシコワ"))
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 15)
    ADD(LW(2), -1)
    CALL(evt_item_entry, STRING("fmoney"), ITEM_DUBIOUS_PAPER, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("fmoney"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_pouch_remove_item, ITEM_DUBIOUS_PAPER, LW(0))
    CALL(evt_npc_move_position, STRING("キノシコワ"), -155, -55, 0, FLOAT(70.0f), 1)
    CALL(evt_cam3d_evt_set, 0, 100, 287, 0, 55, -10, 2000, 11)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -120, 0, FLOAT(70.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
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
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_20"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -183, 85, 386, -183, 25, -10, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_21"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 143)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG3_TOU1"), 2000)
    CALL(evt_snd_bgmoff, 26624)
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
    CALL(evt_cam3d_evt_set, -385, 80, 337, -385, 27, -9, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_15"))
    WAIT_MS(300)
    SPAWN_EVENT(PTR(evt_okorareru))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(first_tamago)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
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
    SET_VALUE(GSW_STORY_PROGRESS, 142)
    RETURN
EVT_END()

EVT_BEGIN(tou_10_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 137)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_GREATER_OR_EQUAL(165)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU6"))
    END_SWITCH
    IF_INT_EQUAL(GSWF(2391), 1)
        IF_INT_LESS(GSW_STORY_PROGRESS, 137)
            SET_VALUE(GSW_STORY_PROGRESS, 137)
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_tou_locker_init))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuku_M"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(init_octo))
    CALL(evt_npc_change_interrupt, STRING("オクトール"), 6, PTR(talk_octo))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("isu"), MAP_OBJ_FLAG_CLIP)
    IF_INT_LESS(GSW_STORY_PROGRESS, 133)
        CALL(evt_run_case_evt, 9, 1, STRING("M_make"), 0, PTR(evt_match_make), 0)
        ELSE
        CALL(evt_run_case_evt, 9, 1, STRING("M_make"), 0, PTR(evt_tou_match_make_default), 0)
    END_IF
    CALL_CHILD(PTR(evt_tou_monitor_init))
    IF_INT_LESS(GSW_STORY_PROGRESS, 142)
        CALL(evt_mobj_lv_blk, STRING("blk"), -280, 0, 100, PTR(evt_block), GSWF(2440), STRING("MOBJ_Lv1BigBigBlock"))
        CALL(evt_mobj_flag_onoff, 1, STRING("blk"), MOBJ_FLAG_NO_CULL)
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(132)
        SPAWN_EVENT(PTR(evt_rec))
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        RETURN
    END_SWITCH
    IF_INT_EQUAL(GSWF(2389), 1)
        IF_INT_EQUAL(GSWF(2450), 1)
            IF_INT_EQUAL(GSWF(2451), 0)
                CALL(evt_pouch_party_join, 1)
                CALL(evt_pouch_party_join, 2)
                CALL(evt_pouch_party_join, 5)
                CALL(evt_party_run, 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2389), 1)
        SET_VALUE(GSWF(2389), 0)
        SWITCH(GSW_GLITZ_PIT_OPPONENT)
            CASE_EQUAL(19)
            IF_INT_EQUAL(GSWF(2392), 0)
                SET_VALUE(GSWF(2392), 1)
                IF_INT_LESS(GSW_STORY_PROGRESS, 172)
                    SPAWN_EVENT(PTR(evt_saisyo_after))
                    ELSE
                    SPAWN_EVENT(PTR(evt_saisyo_after_re))
                END_IF
                ELSE
                SPAWN_EVENT(PTR(evt_tou_match_after_default))
            END_IF
            CASE_EQUAL(10)
            CALL(evt_tou_get_ranking, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 10)
                IF_INT_LESS(GSW_STORY_PROGRESS, 172)
                    SPAWN_EVENT(PTR(evt_1st_leagu))
                    ELSE
                    SPAWN_EVENT(PTR(evt_1st_leagu_re))
                END_IF
                ELSE
                SPAWN_EVENT(PTR(evt_tou_match_after_default))
            END_IF
            CASE_DEFAULT
            SPAWN_EVENT(PTR(evt_tou_match_after_default))
        END_SWITCH
    END_IF
    CALL(evt_hitobj_name_callback, STRING("a_b_H"), PTR(bed_entry))
    CALL(evt_sub_animgroup_async, STRING("p_slit"))
    IF_INT_LESS(GSW_STORY_PROGRESS, 143)
        CALL(evt_item_entry, STRING("syorui"), ITEM_DUBIOUS_PAPER, -475, 90, 75, 16, -1, PTR(evt_syorui))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 137)
        IF_INT_EQUAL(GSWF(2446), 0)
            IF_INT_GREATER_OR_EQUAL(GSW(505), 2)
                SPAWN_EVENT(PTR(first_tamago))
            END_IF
        END_IF
        SET_VALUE(GSWF(2446), 1)
    END_IF
    RETURN
EVT_END()

USER_FUNC(name_check) {
    EvtWord* args = event->args;

    // a full-width space
    if (strcmp((const char*)evtGetValue(event, args[0]), "\x81\x40") == 0) {
        evtSetValue(event, args[0], (EvtWord)msgSearch("name_party3"));
    }
    return EVT_RETURN_DONE;
}

