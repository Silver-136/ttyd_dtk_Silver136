#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_kinopio.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "memory.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_damage.h"
#include "evt/evt_memcard.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "driver/hitdrv.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"

typedef struct PosterWork { u32 flags; s32 group; s32 shape; } PosterWork;
PosterWork* poster;
void draw_poster(void* userdata, s32 group);
void animPoseSetDispCallBack(s32 poseId, void (*callback)(void*, s32), void* userdata);

USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_snd_bgm_freq);
USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(evt_win_item_select);
USER_FUNC(kpa_chg_pose);
USER_FUNC(pposter_init);
USER_FUNC(pposter_nok_on);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
void nok_stg2_013_cb(s32 type, void** work);

EvtWord nokonoko_A_init[27];
EvtWord nokonoko_A_regl[2];
EvtWord nokonoko_A_talk[55];
EvtWord nokonoko_B_init[12];
EvtWord nokonoko_B_regl[19];
EvtWord nokonoko_B_talk[44];
EvtWord nokonoko_B_damage[18];
EvtWord nokonoko_C_regl[19];
EvtWord nokonoko_C_talk[44];
EvtWord nokonoko_D_talk[52];
s32 iri_15_item_tbl1[2];
s32 iri_15_item_tbl2[2];
EvtWord nokonoko_F_talk[318];
static EvtWord init_white[24];
static EvtWord talk_white[36];
static EvtWord kizayarou_regl[19];
static EvtWord kizayarou_talk[92];
static EvtWord kizayarou_init[38];
static NpcSetupInfo npcEnt[10];
static BeroEntry bero_entry_data[3];
static const char* yado_npc[2];
const char* yado_map[2];
static HouseParam yado_data;
static const char* mise_npc[2];
static HouseParam mise_data;
static const char* ie_npc[2];
static const char* ie_map[2];
static HouseParam ie_data;
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[13];
static ShopperData35 shopper_data;
static KinopioData kino_dt;
EvtWord first_time_camera[195];
static HitDamageReturn hitDmgRetPoints[10];
EvtWord nok_00_koopa_npc_entry[35];
EvtWord nok_00_koopa_evt[1250];
EvtWord nok_00_init_evt[205];

void nok_stg2_013_cb(s32 type, void** work) {
    s32* msg = *work;

    switch (type) {
        case 5:
            if (msg[4] > 0x31) {
                evtSetValue(NULL, LSWF(2), 1);
            }
            break;
    }
}

USER_FUNC(pposter_nok_on) {
    poster->flags |= 1;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(nokonoko_A_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_01A_PETALBURG_1ST_TIME)
        CALL(evt_npc_set_position, STRING("me"), -350, 0, 65)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_04_BOWSER_ACTION_STAGE)
        CALL(evt_npc_set_position, STRING("me"), -260, 0, 90)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_A_regl)
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_A_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_02"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_02_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_03"), 0, 0)
        CASE_EQUAL(STG7_02_MAYOR_BOB)
        SET_VALUE(GSWF(3877), 1)
        CALL(evt_msg_print, 0, STRING("stg7_nok_01"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_03_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_nok_01_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_B_init)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY)
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_B_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_B_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_04"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_04_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_05"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_05_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_05_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_B_damage)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KNP_O_1"))
    CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("イャーン　さわらないで！！<key>"), 0, 0)
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KNP_S_1"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_C_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_C_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_06"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_06_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_07"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_07_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_07_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_D_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_08"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_08_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_09"), 0, 0)
        CALL(evt_msg_select, 0, STRING("stg1_nok_09_yesno"))
        CALL(evt_msg_print_add, 0, STRING("stg1_nok_09_01"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_09_01_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_09_01_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

s32 iri_15_item_tbl1[] = {
    ITEM_HOT_DOG, -1,
};

s32 iri_15_item_tbl2[] = {
    ITEM_MOUSSE_CAKE, -1,
};

EVT_BEGIN(nokonoko_F_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_THE_FOOD_I_WANT)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_15_001"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_15_002"), 0, 0)
            SET_VALUE(LF(0), 0)
            CALL(evt_pouch_get_haveitemcnt, LW(0))
            IF_INT_GREATER(LW(0), 0)
                CALL(evt_msg_continue)
                CALL(evt_win_item_select, 0, 1, LW(0), 0)
                ELSE
                SET_VALUE(LW(0), -1)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                IF_INT_EQUAL(LW(0), 159)
                    CALL(evt_pouch_remove_item, ITEM_HOT_DOG, LW(0))
                    IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                        CALL(evt_msg_print, 0, STRING("irai_15_003"), 0, 0)
                        SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                        ELSE
                        CALL(evt_msg_print, 0, STRING("irai_15_004"), 0, 0)
                        SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
                    END_IF
                    ELSE
                    IF_INT_EQUAL(LF(0), 0)
                        CALL(evt_msg_print, 0, STRING("irai_15_008"), 0, 0)
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("irai_15_008"))
                    END_IF
                END_IF
                ELSE
                IF_INT_EQUAL(LW(0), 206)
                    CALL(evt_pouch_remove_item, ITEM_MOUSSE_CAKE, LW(0))
                    CALL(evt_msg_print, 0, STRING("irai_15_005"), 0, 0)
                    CALL(evt_item_entry, STRING("df"), ITEM_DRIED_BOUQUET, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("df"))
                    CALL(evt_item_get_item_end_wait, STRING("df"))
                    CALL(evt_msg_print, 0, STRING("irai_15_007"), 0, 0)
                    CALL(evt_sub_save_playtime, GSW(1445))
                    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_THE_FOOD_I_WANT), 1)
                    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    ELSE
                    IF_INT_EQUAL(LF(0), 0)
                        CALL(evt_msg_print, 0, STRING("irai_15_009"), 0, 0)
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("irai_15_009"))
                    END_IF
                END_IF
            END_IF
        END_IF
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_THE_FOOD_I_WANT), 1)
        CALL(evt_msg_print, 0, STRING("irai_15_010"), 0, 0)
        CALL(evt_msg_select, 0, STRING("irai_15_010_sel"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("irai_15_012"))
            SET_VALUE(LF(0), 0)
            CALL(evt_pouch_get_haveitemcnt, LW(0))
            IF_INT_GREATER(LW(0), 0)
                CALL(evt_msg_continue)
                CALL(evt_win_item_select, 0, 1, LW(0), 0)
                ELSE
                SET_VALUE(LW(0), -1)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(0), 159)
                CALL(evt_pouch_remove_item, ITEM_HOT_DOG, LW(0))
                CALL(evt_msg_print, 0, STRING("irai_15_013"), 0, 0)
                CALL(evt_item_entry, STRING("df"), ITEM_DRIED_BOUQUET, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("df"))
                CALL(evt_item_get_item_end_wait, STRING("df"))
                CALL(evt_msg_print, 0, STRING("irai_15_015"), 0, 0)
                ELSE
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_15_016"), 0, 0)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_15_016"))
                END_IF
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("irai_15_016"))
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_17"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_17_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_18"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_18_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_18_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_white)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_E3)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG2_25_04_BOWSER_ACTION_STAGE)
        CASE_LESS(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), 375, 50, -105)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_white)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_19_01"), 0, 0)
        CASE_LESS(STG1_16_HOOKTAIL)
        CALL(evt_msg_print, 0, STRING("stg1_nok_19_02"), 0, 0)
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_19_03"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_19_04"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kizayarou_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(kizayarou_talk)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        IF_INT_EQUAL(LSWF(1), 0)
            CALL(evt_msg_print, 0, STRING("stg1_nok_10"), 0, 0)
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg1_nok_11"), 0, STRING("party"))
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_msg_print, 0, STRING("stg1_nok_12"), 0, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg1_nok_13"), 0, STRING("party"))
            CALL(evt_party_cont_onoff, 0, 1)
            SET_VALUE(LSWF(1), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg1_nok_14"), 0, 0)
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg1_nok_15"), 0, STRING("party"))
            CALL(evt_party_cont_onoff, 0, 1)
        END_IF
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_16"), 0, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(kizayarou_init)
    SET_VALUE(LW(0), STRING("キザ野郎"))
    CALL(evt_npc_entry, LW(0), STRING("c_kiza"))
    CALL(evt_npc_set_tribe, LW(0), STRING("キザ野郎"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(kizayarou_talk))
    CALL(evt_npc_change_interrupt, LW(0), 1, PTR(kizayarou_regl))
    CALL(evt_npc_set_position, LW(0), 350, 0, 175)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ノコノコA"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x41", // name
        0, 0, // flags..reactionFlags
        nokonoko_A_init, // initEvent
        nokonoko_A_regl, // regularEvent
        nokonoko_A_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコB"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x42", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        nokonoko_B_regl, // regularEvent
        nokonoko_B_talk, // talkEvent
        nokonoko_B_damage, // deadEvent
        NULL, NULL, NULL, NULL, 0, // findEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコC"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x43", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        nokonoko_C_regl, // regularEvent
        nokonoko_C_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコD"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x44", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        nokonoko_D_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコF"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x46", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        nokonoko_F_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ホワイト"*/"\x83\x7A\x83\x8F\x83\x43\x83\x67", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_white, // initEvent
        NULL, // regularEvent
        talk_white, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

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
        "hei_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "nok_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static const char* yado_npc[] = {
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5",
    NULL,
};

const char* yado_map[] = {
    "S_yad_mae",
    NULL,
};

static HouseParam yado_data = {
    1, // type
    9, // hitInKind
    "A_yad_doa01", // hitIn
    9, // hitOutKind
    "A_yad_in_doa", // hitOut
    {"S_yado_open", "S_yado_doa_open", "S_yado_close", "S_yado_doa_close"}, // anim
    "S_yad_mae", // outside
    "S_yad_doa01", // door
    {"S_yado_in", "A_yado_in"}, // inside
    {"S_yado_in", "A_yado_in"}, // initInside
    yado_npc, // npcs
    yado_map, // noclip
    NULL, NULL, // mobjs..items
};

static const char* mise_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

static HouseParam mise_data = {
    1, // type
    9, // hitInKind
    "A_mise_doa", // hitIn
    9, // hitOutKind
    "A_mise_in_doa", // hitOut
    {"S_mise_open", "S_mise_doa_open", "S_mise_close", "S_mise_doa_close"}, // anim
    "S_mise_mae", // outside
    "S_mise_doa", // door
    {"S_mise_in", "A_mise_in"}, // inside
    {"S_mise_in", "A_mise_in"}, // initInside
    mise_npc, // npcs
    NULL, NULL, NULL, // noclip..items
};

static const char* ie_npc[] = {
    /*"ノコノコD"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x44",
    NULL,
};

static const char* ie_map[] = {
    "S_ie_mae",
    NULL,
};

static HouseParam ie_data = {
    1, // type
    9, // hitInKind
    "A_ie_doa", // hitIn
    9, // hitOutKind
    "A_ie_in_doa", // hitOut
    {"S_ie_open", "S_ie_doa_open", "S_ie_close", "S_ie_doa_close"}, // anim
    "S_ie_mae", // outside
    "S_ie_doa", // door
    {"S_ie_in", "A_ie_in"}, // inside
    {"S_ie_in", "A_ie_in"}, // initInside
    ie_npc, // npcs
    ie_map, // noclip
    NULL, NULL, // mobjs..items
};

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_FIRE_FLOWER, 8},
    {ITEM_POW_BLOCK, 5},
    {ITEM_COURAGE_SHELL, 5},
    {ITEM_MR_SOFTENER, 8},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_MUSHROOM, 4},
};

static ShopDb trade_list[] = {
    {ITEM_FIRE_FLOWER, 4, 0},
    {ITEM_POW_BLOCK, 2, 0},
    {ITEM_COURAGE_SHELL, 2, 0},
    {ITEM_MR_SOFTENER, 4, 0},
    {ITEM_HONEY_SYRUP, 2, 0},
    {ITEM_MUSHROOM, 2, 0},
    {ITEM_SLEEPY_SHEEP, 10, 0},
    {ITEM_KOOPASTA, 60, 0},
    {ITEM_KOOPA_TEA, 4, 0},
    {ITEM_KOOPA_BUN, 10, 0},
    {ITEM_DRIED_BOUQUET, 2, 0},
    {ITEM_GOLDEN_LEAF, 5, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"shop_00", "shop_01", "shop_02", "shop_03", "shop_04", "shop_05", "shop_06", "shop_07", "shop_08", "shop_09", "shop_10", "shop_11", "shop_12", "shop_13", "shop_14", "shop_15", "shop_16", "shop_17", "shop_18", "shop_19", "shop_20", "shop_21", "shop_22", "shop_23", "shop_24", "shop_25", "shop_26", "shop_27", "shop_28", "shop_29", "shop_30", "shop_31", "shop_32", "shop_33", "shop_34"}, // msgs
};

static KinopioData kino_dt = {
    0, // breakfastItem
    {0}, // breakfastPos
    5, // ryoukin
    {-269.0f, 41.0f, -256.0f}, // bed
    {-268.0f, 25.0f, -220.0f}, // bedside
    {-495.0f, 227.0f, -56.0f, -327.0f, 90.0f, -225.0f}, // cam
    -30.0f, // bedDeg
    {-330.0f, 25.0f, -180.0f}, // npcTalk
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

EVT_BEGIN(first_time_camera)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_set_pos, -545, 0, 0)
    CALL(evt_party_set_pos, 0, -555, 0, 0)
    CALL(evt_npc_set_position, STRING("ノコノコA"), -250, 0, 65)
    CALL(evt_cam3d_evt_set, 730, 617, 915, 245, 93, 144, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_telop_entry, 2)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, FLOAT(-450.0f), FLOAT(400.0f), FLOAT(840.0f), FLOAT(-300.0f), FLOAT(165.0f), FLOAT(255.0f), 6000, EASING_SINE_IN)
        WAIT_MS(8500)
        CALL(evt_cam3d_evt_set, FLOAT(-380.0f), FLOAT(90.0f), FLOAT(352.0f), FLOAT(-380.0f), FLOAT(35.0f), FLOAT(47.0f), 0, EASING_SINE_IN)
    END_INLINE_EVENT
    WAIT_MS(6000)
    CALL(evt_mario_mov_pos2, -400, 65, FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("ノコノコA"), -350, 65, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_nok_01"), 0, STRING("ノコノコA"))
    CALL(evt_mario_get_motion, LW(0))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_talk_motion)
    WAIT_MS(500)
    CALL(evt_mario_set_motion, LW(0))
    CALL(evt_msg_print, 0, STRING("stg1_nok_01_01"), 0, STRING("ノコノコA"))
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 1)
    END_IF
    CALL(evt_party_move_pos, 0, -414, 79, 500)
    CALL(evt_party_stop, 0)
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("stg1_nok_01_02"), 0, STRING("party"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_01_03"), 0, STRING("ノコノコA"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_01_04"), 0, STRING("party"))
    WAIT_MS(100)
    CALL(evt_cam3d_evt_off, 500, EASING_QUADRATIC_OUT)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG1_NOK1"), 2000)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_01A_PETALBURG_1ST_TIME)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_00", {0.0f, -1000.0f, 0.0f}},
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {0.0f, -1000.0f, 0.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {"mod_08", {0.0f, -1000.0f, 0.0f}},
    {"A_hasi04", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(nok_00_koopa_npc_entry)
    CALL(evt_npc_entry, STRING("poster"), STRING("c_p_peach"))
    CALL(evt_npc_pera_onoff, STRING("poster"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("poster"), NPC_FLAG_NO_HIT)
    CALL(evt_mapobj_get_position, STRING("S_ie_mad02"), LW(0), LW(1), LW(2))
    SUBTRACT_FLOAT(LW(1), FLOAT(25.0f))
    ADD_FLOAT(LW(2), FLOAT(8.0f))
    CALL(evt_npc_set_position, STRING("poster"), LW(0), LW(1), LW(2))
    CALL(pposter_init)
    RETURN
EVT_END()

EVT_BEGIN(nok_00_koopa_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_NOK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_NOK1"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_set_pos, -500, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_set_position, STRING("ババ"), -550, 50, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコA"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("ノコノコA"), 90)
    CALL(evt_cam3d_evt_set, -315, 146, 529, -315, 50, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_kamek_move_position, STRING("ババ"), -405, 57, 10, 0, 50, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -342, 80, 60)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ノコノコA"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコA"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("ノコノコA"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_004"), 0, STRING("ノコノコA"))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_NOK1"))
    CALL(evt_npc_get_position, STRING("ノコノコA"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), 1)
    LOOP(10)
        CALL(evt_npc_set_position, STRING("ノコノコA"), LW(3), LW(1), LW(2))
        SUBTRACT(LW(3), 2)
        WAIT_MS(50)
        CALL(evt_npc_set_position, STRING("ノコノコA"), LW(3), LW(1), LW(2))
        ADD(LW(3), 2)
        WAIT_MS(50)
    END_LOOP
    CALL(evt_npc_set_position, STRING("ノコノコA"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_autotalkpose, STRING("ノコノコA"), STRING("NOK_S_2"), STRING("NOK_T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_004_1"), 0, STRING("ノコノコA"))
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコA"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("ノコノコA"), STRING("NOK_R_2"))
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("ノコノコA"), 90)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_NOKO_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_blur_onoff, 1, STRING("ノコノコA"))
    CALL(evt_npc_move_position, STRING("ノコノコA"), -70, 50, 0, 180, 0)
    CALL(evt_npc_blur_onoff, 0, STRING("ノコノコA"))
    CALL(evt_mapobj_get_position, STRING("S_mise_doa"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), -250)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_ie_doa"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), -200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコA"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコB"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコC"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコD"), NPC_FLAG_NO_DRAW)
    CALL(evt_cam3d_evt_set, -335, 133, 430, -335, 55, -13, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_007"), 0, STRING("mario"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_HIT)
    CALL(evt_cam3d_evt_set, -280, 133, 430, -280, 55, -13, 2000, EASING_SINE_IN_OUT)
    CALL(evt_npc_kamek_move_position, STRING("ババ"), -275, 135, 10, 0, 80, 0)
    SET_VALUE(GF(0), 1)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_008"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 290, 144, 479, 290, 57, -13, 2000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 350, 166, 162, 350, 115, -134, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_KUPPA_FIND_PEACH1"), 0)
    WAIT_MS(400)
    CALL(evt_mario_set_pos, 200, 0, 40)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_009"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 300, 192, 363, 300, 105, -130, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, 130, 0, 40)
    CALL(evt_mario_mov_pos2, 190, -37, 120)
    CALL(evt_mario_jump_pos, 230, 25, -85, 600, 0, -1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_LAUGH2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_3"), STRING("KPA_T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_010"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 350, 166, 162, 350, 115, -134, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS_FAR, 355, 100, -150)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_011"), 0, 0)
    CALL(evt_cam3d_evt_set, 300, 192, 363, 300, 105, -130, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_012"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 190, 176, 363, 190, 90, -130, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_position, STRING("ババ"), 30, 70, -35)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_kamek_move_position, STRING("ババ"), 150, -35, 20, 0, 60, 0)
    SET_VALUE(GF(0), 1)
    WAIT_FRAMES(1)
    SET_VALUE(LSWF(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(LSWF(2), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_4"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("kpa_stg2_013"), PTR(nok_stg2_013_cb), STRING("ババ"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_3"), STRING("KPA_T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_014"), 0, STRING("mario"))
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコA"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_position, STRING("ノコノコA"), 420, 0, 0)
    CALL(evt_npc_set_ry, STRING("ノコノコA"), 270)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_015"), 0, STRING("ノコノコA"))
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    SET_VALUE(GW(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ノコノコA"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 1)
        LOOP(0)
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_set_position, STRING("ノコノコA"), LW(3), LW(1), LW(2))
            SUBTRACT(LW(3), 2)
            WAIT_MS(50)
            CALL(evt_npc_set_position, STRING("ノコノコA"), LW(3), LW(1), LW(2))
            ADD(LW(3), 2)
            WAIT_MS(50)
        END_LOOP
        CALL(evt_npc_set_position, STRING("ノコノコA"), LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 324, 189, 551, 324, 85, -44, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 65)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 403, 104, 331, 403, 43, -17, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_msg_print, 0, STRING("kpa_stg2_016"), 0, STRING("ノコノコA"))
    SET_VALUE(GW(0), 1)
    WAIT_MS(100)
    CALL(evt_cam3d_evt_set, 306, 168, 344, 306, 85, -130, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコA"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("ノコノコA"), STRING("NOK_R_4"))
    CALL(evt_npc_move_position, STRING("ノコノコA"), 408, -46, 500, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_NOKO_JUMP1"), 408, 0, -46, 0)
    CALL(evt_npc_jump_position_nohit, STRING("ノコノコA"), 374, 50, -77, 500, 60)
    CALL(evt_npc_move_position, STRING("ノコノコA"), 374, -125, 500, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_NOKO_JUMP1"), 374, 50, -125, 0)
    CALL(evt_npc_jump_position_nohit, STRING("ノコノコA"), 374, 64, -153, 500, 20)
    BEGIN_INLINE_EVENT
        WAIT_MS(750)
        CALL(evt_npc_set_anim, STRING("ノコノコA"), STRING("NOK_A_1"))
        CALL(evt_npc_set_anim, STRING("poster"), STRING("A_1"))
        WAIT_MS(750)
        CALL(evt_npc_set_anim, STRING("ノコノコA"), STRING("NOK_S_3"))
        CALL(evt_npc_set_anim, STRING("poster"), STRING("Z_2"))
        CALL(evt_npc_flag_onoff, 1, STRING("poster"), NPC_FLAG_NO_DRAW)
        CALL(pposter_nok_on)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコA"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("ノコノコA"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_NOKO_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d_ex, STRING("SFX_KUPPA_NOKO_POSTER1"), LW(0), LW(1), LW(2), 255, 255, 750, 16, 0)
    CALL(evt_npc_jump_position_nohit, STRING("ノコノコA"), LW(0), LW(1), LW(2), 1500, 50)
    CALL(kpa_chg_pose, STRING("KPA_O_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 65)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(kpa_chg_pose, STRING("KPA_S_5"))
    CALL(evt_npc_set_anim, STRING("ノコノコA"), STRING("NOK_R_3"))
    CALL(evt_npc_move_position, STRING("ノコノコA"), 316, -154, 500, 0, 0)
    CALL(evt_npc_move_position, STRING("ノコノコA"), 280, -199, 500, 0, 0)
    CALL(evt_npc_set_ry, STRING("ノコノコA"), 90)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_in_kabe"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mapobj_get_position, STRING("S_ie_doa"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(30)
            ADD_FLOAT(LW(0), FLOAT(3.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_ie_doa"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, STRING("S_ie_doa"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 255, 255, 400, 16, 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(90.0f))
        LOOP(30)
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_ie_doa"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ノコノコA"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("ノコノコA"), 258, -179, 500, 0, 0)
    CALL(evt_npc_move_position, STRING("ノコノコA"), 335, -231, 500, 0, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 208, 177, 333, 208, 97, -134, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_5"), STRING("KPA_T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_017"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_018"), 0, STRING("ババ"))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SATIATED2_2"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 32000)
        LOOP(40)
            SUBTRACT(LW(0), 750)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        SET_VALUE(GF(1), 1)
    END_INLINE_EVENT
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_2"), STRING("KPA_T_4"))
    CALL(evt_msg_print, 0, STRING("kpa_stg2_019"), 0, STRING("mario"))
    LABEL(99)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(GF(1), 0)
        GOTO(99)
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, STG2_25_05_BOWSER_PETALBURG)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 6)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, 59, 0, 58, 0)
    CALL(evt_bero_mapchange, STRING("mri_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(nok_00_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_04_BOWSER_ACTION_STAGE)
        CALL(evt_mario_set_mode, 2)
        CALL_CHILD(PTR(nok_00_koopa_npc_entry))
        SPAWN_EVENT(PTR(nok_00_koopa_evt))
        ELSE
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_NOK1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_NOK1"))
        IF_INT_LESS(GSW_STORY_PROGRESS, STG1_01A_PETALBURG_1ST_TIME)
            SPAWN_EVENT(PTR(bero_case_entry))
            SPAWN_EVENT(PTR(first_time_camera))
            ELSE
            CALL_CHILD(PTR(evt_bero_info_run))
        END_IF
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_00_PETAL_MEADOWS)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
            CALL_CHILD(PTR(kizayarou_init))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1624), 0)
        SET_VALUE(GSWF(1624), 1)
    END_IF
    CALL(evt_map_playanim, STRING("S_kawa"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&yado_data))
    CALL_CHILD(PTR(evt_door_setup))
    SET_VALUE(LW(0), PTR(&mise_data))
    CALL_CHILD(PTR(evt_door_setup))
    SET_VALUE(LW(0), PTR(&ie_data))
    CALL_CHILD(PTR(evt_door_setup))
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG2_25_04_BOWSER_ACTION_STAGE)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 155, 60, -60, 0, 0)
    END_IF
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    LOOP(5)
        CALL(evt_sub_random, 10, LW(0))
        CALL(evt_eff, 0, STRING("butterfly"), LW(0), -395, 20, 250, 0, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    LOOP(5)
        CALL(evt_sub_random, 10, LW(0))
        CALL(evt_eff, 0, STRING("butterfly"), LW(0), -80, 20, 270, 0, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    LOOP(2)
        CALL(evt_sub_random, 10, LW(0))
        CALL(evt_eff, 0, STRING("butterfly"), LW(0), -40, 20, 470, 0, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    LOOP(2)
        CALL(evt_sub_random, 10, LW(0))
        CALL(evt_eff, 0, STRING("butterfly"), LW(0), 375, 20, 250, 0, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -70, 0, 350, STRING("S_ground03"), 125, GSWF_STARPIECE_34)
    RETURN
EVT_END()

USER_FUNC(pposter_init) {
    NpcEntry* npc;

    poster = _mapAlloc(sizeof(PosterWork));
    memset(poster, 0, sizeof(PosterWork));
    poster->shape = animPoseGetShapeIdx(npcNameToPtr("poster")->poseId, "pPlaneShape2");
    // ノコノコA
    npc = npcNameToPtr("\x83\x6D\x83\x52\x83\x6D\x83\x52\x41");
    poster->group = animPoseGetGroupIdx(npc->poseId, "locator7");
    animPoseSetDispCallBack(npc->poseId, draw_poster, NULL);
    return EVT_RETURN_DONE;
}

void draw_poster(void* userdata, s32 group) {
    NpcEntry* npc = npcNameToPtr("poster");

    if (group != poster->group || !(poster->flags & 1)) {
        return;
    }
    animPoseDrawShape(npc->poseId, poster->shape);
}

