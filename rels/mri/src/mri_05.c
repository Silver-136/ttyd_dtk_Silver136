#include "battle/battle_unit.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "eff/eff_puniballoon.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "mri_puni.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_cam.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"

#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "evt/evt_npc.h"
#include "data_tables.h"

EffectEntry* marioeff;

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_pcharge_pos);
USER_FUNC(mario_shabon_eff);
USER_FUNC(mario_shabon_effdel);
USER_FUNC(mario_shabon_effpos);
USER_FUNC(mario_shabon_init);

EvtWord puni_4_init[17];
EvtWord puni_4_talk[20];
EvtWord puni_5_init[17];
EvtWord puni_5_talk[20];
static NpcSetupInfo npcEnt[3];
static EvtWord bero_s_doa_close[10];
static EvtWord bero_s_doa_open[10];
static BeroEntry bero_entry_data[6];
static EvtWord door_switch_init[17];
static EvtWord door_switch[59];
static EvtWord evt_ueki_04[20];
static KusaAccess ueki_access_data[6];
static EvtWord punipuni_ball00_init[41];
static EvtWord punipuni_ball00[51];
EvtWord punipuni_ball01_init[41];
EvtWord punipuni_ball01[51];
EvtWord mario_shabon_move_sound[14];
EvtWord mario_shabon_break_sound[9];
EvtWord mario_shabon_check[251];
EvtWord evt_shabon_s[312];
EvtWord mri_05_init_evt[178];

USER_FUNC(mario_shabon_init) {
    MarioWork* mario = marioGetPtr();

    mario->hitobjStandOn = NULL;
    mario->airTimer = 1;
    marioChgMot(MARIO_MOTION_JUMP_3);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(puni_4_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -130, -309, 90)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_4_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_019_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_019_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_5_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 130, -309, 90)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_5_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_020"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_020_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"プニ族４"*/"\x83\x76\x83\x6A\x91\xB0\x82\x53", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        puni_4_init, // initEvent
        NULL, // regularEvent
        puni_4_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族５"*/"\x83\x76\x83\x6A\x91\xB0\x82\x54", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        puni_5_init, // initEvent
        NULL, // regularEvent
        puni_5_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(bero_s_doa_close)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_close))
    RETURN
EVT_END()

static EVT_BEGIN(bero_s_doa_open)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_open))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_01", // nextMapName
        "e_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        bero_s_doa_close, // enterAnimeArg
        bero_s_doa_open, // exitAnimeArg
    },
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_02", // nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan4", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_19", // nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(door_switch_init)
    IF_INT_EQUAL(GSWF(2841), 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_map_playanim, STRING("doalight"), 2, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(door_switch)
    CALL(evt_mapobj_get_position, STRING("S_lights"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_UNLOCK"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_LAMP"), LW(0), LW(1), LW(2), 0)
    IF_INT_EQUAL(GSWF(2841), 0)
        SET_VALUE(GSWF(2841), 1)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_map_playanim, STRING("doalight"), 0, 0)
        ELSE
        SET_VALUE(GSWF(2841), 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_map_playanim, STRING("doalight"), 0, 0)
        CALL(evt_map_pauseanim, 0, STRING("doalight"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_ueki_04)
    CALL(evt_item_delete_check, STRING("item_ueki_04"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_04"), ITEM_THUNDER_RAGE, 225, -309, -40, 4, GSWF(2844), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", evt_ueki_04},
    {1, "A_ueki05", "S_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF(2842), 1)
        CALL(evt_item_set_position, LW(10), -150, -109, -37)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari_01"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_target_set_position, LW(11), -150, -139, 20)
    CALL(evt_target_set_soko, LW(11), STRING("A_dai_01_soko"))
    CALL(evt_target_flag_onoff, LW(11), 0, 8)
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(2842), 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF(2842), 1)
        CALL(evt_item_set_position, LW(10), -150, -109, -37)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sui_hikari_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        ELSE
        SET_VALUE(GSWF(2842), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(punipuni_ball01_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF(2843), 1)
        CALL(evt_item_set_position, LW(10), 150, -109, -37)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari_02"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_target_set_position, LW(11), 150, -139, 20)
    CALL(evt_target_set_soko, LW(11), STRING("A_dai_02_soko"))
    CALL(evt_target_flag_onoff, LW(11), 0, 8)
    RETURN
EVT_END()

USER_FUNC(mario_shabon_eff) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);

    marioeff = effPuniBalloonEntry(type, x, y, evtGetFloat(event, args[3]), 2.5f);
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_shabon_effpos) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    ((PuniBalloonWork*)marioeff->userdata)->position.x = x;
    ((PuniBalloonWork*)marioeff->userdata)->position.y = y;
    ((PuniBalloonWork*)marioeff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_shabon_effdel) {
    effDelete(marioeff);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(punipuni_ball01)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(2843), 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF(2843), 1)
        CALL(evt_item_set_position, LW(10), 150, -109, -37)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sui_hikari_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        ELSE
        SET_VALUE(GSWF(2843), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mario_shabon_move_sound)
    LOOP(0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SOAPBALL_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mario_shabon_break_sound)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SOAPBALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(mario_shabon_check)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    LOOP(0)
        CALL(evt_mario_get_name_hitobj_ride, LW(0))
        IF_STR_EQUAL(LW(0), STRING("A_syabon_yuka"))
            SET_VALUE(LW(1), 1)
            ELSE
            SET_VALUE(LW(1), 0)
        END_IF
        CALL(evt_mario_get_state, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            SET_VALUE(LW(2), 0)
            ELSE
            ADD(LW(2), 1)
        END_IF
        IF_INT_EQUAL(LW(1), 1)
            IF_INT_EQUAL(LW(2), 60)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_party_stop, 0)
    CALL(mario_shabon_init)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), LW(4))
    ADD(LW(4), -30)
    CALL(mario_shabon_eff, 0, LW(3), LW(4), LW(5))
    SPAWN_EVENT_GET_ID(PTR(mario_shabon_move_sound), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(4), LW(6), 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        ADD(LW(0), 20)
        SET_VALUE(LW(2), LW(5))
        ADD(LW(2), 2)
        CALL(mario_shabon_effpos, LW(3), LW(0), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    SET_VALUE(LW(6), LW(4))
    ADD(LW(6), 40)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(4), LW(6), 1500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mario_set_pos, LW(3), LW(0), LW(5))
        ADD(LW(0), 20)
        SET_VALUE(LW(2), LW(5))
        ADD(LW(2), 2)
        CALL(mario_shabon_effpos, LW(3), LW(0), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(mario_shabon_effdel)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    ADD(LW(2), 2)
    CALL(mario_shabon_eff, 1, LW(0), LW(1), LW(2))
    DELETE_EVENT(LW(9))
    SPAWN_EVENT(PTR(mario_shabon_break_sound))
    CALL(evt_mario_set_pose, STRING("M_F_1"))
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(6), LW(4), 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mario_set_pos, LW(3), LW(0), LW(5))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_set_motion, 0)
    WAIT_MS(500)
    CALL(evt_party_run, 0)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    SPAWN_EVENT(PTR(mario_shabon_check))
    RETURN
EVT_END()

USER_FUNC(evt_pcharge_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = 15.0f + evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.x = x;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.y = y;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_shabon_s)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_random, 700, LW(0))
        WAIT_MS(LW(0))
        LOOP(0)
            CALL(evt_sub_random, 20, LW(3))
            CALL(evt_sub_random, 60, LW(2))
            ADD(LW(3), -80)
            ADD(LW(2), 55)
            CALL(evt_eff, STRING("shabon1eff"), STRING("puni_balloon"), 0, LW(3), -319, LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            CALL(evt_sub_random, 280, LW(4))
            ADD(LW(4), -69)
            CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -319, LW(4), 6600)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_pcharge_pos, STRING("shabon1eff"), LW(3), LW(0), LW(2))
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_eff_delete, STRING("shabon1eff"))
            IF_INT_LESS(LW(4), 40)
#ifdef VERSION_G8ME01
                CALL(evt_eff, STRING("_shabon1eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#else
                CALL(evt_eff, STRING("shabon1eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#endif
            END_IF
            CALL(evt_sub_random, 700, LW(0))
            ADD(LW(0), 2000)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_random, 1500, LW(0))
        WAIT_MS(LW(0))
        LOOP(0)
            CALL(evt_sub_random, 100, LW(3))
            CALL(evt_sub_random, 20, LW(2))
            ADD(LW(3), -50)
            ADD(LW(2), 100)
            CALL(evt_eff, STRING("shabon2eff"), STRING("puni_balloon"), 0, LW(3), -319, LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            CALL(evt_sub_random, 280, LW(4))
            ADD(LW(4), -69)
            CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -319, LW(4), 7500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_pcharge_pos, STRING("shabon2eff"), LW(3), LW(0), LW(2))
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_eff_delete, STRING("shabon2eff"))
            IF_INT_LESS(LW(4), 40)
#ifdef VERSION_G8ME01
                CALL(evt_eff, STRING("_shabon2eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#else
                CALL(evt_eff, STRING("shabon2eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#endif
            END_IF
            CALL(evt_sub_random, 950, LW(0))
            ADD(LW(0), 2000)
            WAIT_MS(LW(0))
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_random, 1000, LW(0))
    WAIT_MS(LW(0))
    LOOP(0)
        CALL(evt_sub_random, 20, LW(3))
        CALL(evt_sub_random, 160, LW(2))
        ADD(LW(3), 60)
        SUBTRACT(LW(2), 100)
        CALL(evt_eff, STRING("shabon3eff"), STRING("puni_balloon"), 0, LW(3), -319, LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_sub_random, 280, LW(4))
        ADD(LW(4), -69)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -319, LW(4), 7200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_pcharge_pos, STRING("shabon3eff"), LW(3), LW(0), LW(2))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_eff_delete, STRING("shabon3eff"))
        IF_INT_LESS(LW(4), 40)
#ifdef VERSION_G8ME01
            CALL(evt_eff, STRING("_shabon3eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#else
            CALL(evt_eff, STRING("shabon3eff"), STRING("puni_balloon"), 1, LW(3), LW(0), LW(2), FLOAT(0.32910156f), 0, 0, 0, 0, 0, 0, 0)
#endif
        END_IF
        CALL(evt_sub_random, 500, LW(0))
        ADD(LW(0), 2000)
        WAIT_MS(LW(0))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_05_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 5, -139, -309, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, -139, 0, -137, -141, -305, -320)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    IF_INT_GREATER(GSW_STORY_PROGRESS, 113)
        CALL(evt_npc_check_delete, STRING("軍団ザコ"))
    END_IF
    CALL(evt_npc_change_fbat_mode, 1)
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI6"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("syabon_01"), 1, 0)
    CALL(evt_map_playanim, STRING("syabon_02"), 1, 0)
    CALL(evt_map_playanim, STRING("sui_hikari"), 1, 0)
    SPAWN_EVENT(PTR(evt_shabon_s))
    SET_VALUE(LW(10), STRING("punipuni00"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai_01_mae"), 0, PTR(punipuni_ball00), 0)
    SET_VALUE(LW(10), STRING("punipuni01"))
    SET_VALUE(LW(11), 1)
    SPAWN_EVENT(PTR(punipuni_ball01_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai_mae"), 0, PTR(punipuni_ball01), 0)
    SPAWN_EVENT(PTR(door_switch_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui"), 0, PTR(door_switch), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 250, -250, 100, 0, GSWF(5542), 0)
    SPAWN_EVENT(PTR(mario_shabon_check))
    RETURN
EVT_END()

