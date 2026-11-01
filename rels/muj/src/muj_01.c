// muj_01 (無人島, Keelhaul Key): the castaways' camp: the shop and the inn, the sailors and the
// dancers, and the crew's homesickness, split and persuasion (kyoshu_event, nakamaware_event, settoku_event).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_urouro.h"
#include "evt/evt_window.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_pouch.h"
#include "muj.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "driver/envdrv.h"

#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/icondrv.h"
#include "stdio.h"
#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "dolphin/mtx.h"
#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "memory.h"
#include "mario/mario_pouch.h"
#include "evt/evt_door.h"
#include "evt/evt_bero.h"
#include "evt/evt_eff.h"
#include "evt/evt_kinopio.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "driver/seqdrv.h"

static s32* itemsel_table_ram;

static USER_FUNC(anim_tevcallback);
static USER_FUNC(anim_tevcallback_mobj);
static USER_FUNC(delete_itemsel_table);
USER_FUNC(evt_mario_get_reqdir);
static USER_FUNC(make_itemsel_table);
USER_FUNC(muj_get_door_check);
USER_FUNC(sekai_yami_omise);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
static USER_FUNC(yami_view);

EvtWord custom_door_dark2[34];
EvtWord custom_door_light2[30];
static EvtWord garawaru_init[39];
static EvtWord garawaru_talk[270];
static EvtWord marco_init[31];
EvtWord marco_kyorokyoro[28];
#if VERSION >= VERSION_US
static EvtWord marco_talk[848];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord marco_talk[788];
#else
static EvtWord marco_talk[831];
#endif
#endif
static EvtWord megane_init[14];
static EvtWord megane_talk[54];
static EvtWord odoodo_init[14];
static EvtWord odoodo_talk[54];
static EvtWord suifu_a_init[14];
static EvtWord suifu_a_talk[48];
static EvtWord suifu_bomb_init[14];
static EvtWord suifu_bomb_talk[48];
static EvtWord suifu_d_init[13];
EvtWord suifu_d_regl[24];
static EvtWord suifu_d_talk[56];
EvtWord borosuifu_a_init[13];
EvtWord borosuifu_a_talk[56];
EvtWord borosuifu_b_init[22];
EvtWord borosuifu_b_regl[30];
EvtWord borosuifu_b_talk[56];
static EvtWord mony_init[14];
static EvtWord mony_talk[8];
static EvtWord peton_init[14];
static EvtWord peton_talk[8];
static EvtWord musume_talk[20];
static EvtWord musume_a_entry[30];
static EvtWord musume_b_entry[30];
static EvtWord musume_c_entry[30];
static NpcSetupInfo npcEnt[17];
static EvtWord bero_in_event[5];
static BeroEntry bero_entry_data[3];
#if VERSION <= VERSION_DEMO_US
extern EvtWord muj_00_door_open[];
#else
EvtWord muj_00_door_open[247];
#endif
EvtWord muj_00_door_close[22];
static const char* mise_npc[2];
EvtWord mise_data_open[32];
EvtWord mise_data_close[36];
static DoorParam mise_data;
static const char* yado_npc[2];
EvtWord mise_open_event[22];
EvtWord mise_close_event[26];
static DoorParam yado_data;
EvtWord evt_door_entry_custom00[39];
EvtWord evt_door_entry_custom01[39];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
static KinopioData kino_dt;
EvtWord kyoshu_event[196];
EvtWord maruco_buruburu[17];
EvtWord garawaru_buruburu[17];
EvtWord suifu_a_buruburu[17];
EvtWord suifu_bom_buruburu[17];
EvtWord nakamaware_event[791];
EvtWord settoku_event[54];
EvtWord settoku_event_main[95];
static EvtWord delete_npc_regl_evt[24];
static EvtWord sekai_yami_shake[11];
static EvtWord sekai_yami_evt[283];
static EvtWord minnnanokoe_evt[402];
static EvtWord epilogue_npc_entry[60];
static EvtWord epilogue_evt_sub[117];
static EvtWord epilogue_evt[88];
EvtWord yakan_anime[17];
EvtWord muj_01_init_evt[225];

/**
 * Fades the white map blend layer 1 from opaque to clear over 300 ms;
 * run when the player walks into a building (shop, inn) so the outdoor lighting fades away.
 */
EVT_BEGIN(custom_door_dark2)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 1, 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_set_blend, 1, 255, 255, 255, 0)
    RETURN
EVT_END()

/**
 * Fades the white map blend layer 1 back in over 300 ms and then switches it off;
 * the opposite of custom_door_dark2, run when leaving a building.
 */
EVT_BEGIN(custom_door_light2)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_blend, 1, 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_map_blend_off, 1)
    RETURN
EVT_END()

/**
 * Init event of the rough sailor (NPC ガラの悪い水夫) in the camp: placed at (-155, 0, 170) from
 * STG5_11_DESERTED_ISLAND to STG5_34_KICKED_OFF and from STG5_59_ROGUEPORT_DOCK on,
 * except at STG5_18_01_BOBBERY_JOINS, when he stands at (-110, 0, 225).
 */
static EVT_BEGIN(garawaru_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_11_DESERTED_ISLAND, STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
        CASE_EQUAL(STG5_18_01_BOBBERY_JOINS)
        CALL(evt_npc_set_position, STRING("me"), -110, 0, 225)
        CASE_BETWEEN(STG5_18_02_FLAVIOS_FAVOR_2, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of the rude sailor: a different line for each story stage (stg5_muj_22_1, stg5_muj_32_02 and others). */
static EVT_BEGIN(garawaru_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_12_FLAVIO_PA_PATCH)
        // Early on, the rough sailor muses about the mysterious eastern end of the island.
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_1"), 0, STRING("me"))
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_18_BOBBERY_RESCUE_2)
        // While Bobbery is being rescued, he comments on the admiral's plight.
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_02"), 0, STRING("me"))
        CASE_EQUAL(STG5_18_01_BOBBERY_JOINS)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            // With Bobbery in the party, he greets the captain and spots Bobbery.
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_05"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_04"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_18_02_FLAVIOS_FAVOR_2, STG5_18_03_MARCOS_JEWEL)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            // He grows suspicious of the skull rock.
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_02"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_02_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_13"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_132_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_58_FAIR_WINDS, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            // After the departure, he reacts to the Piantas' commotion.
            CALL(evt_msg_print, 0, STRING("stg5_muj_149"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1492_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_62_AN_ILL_DON_PIANTA, STG5_64_LOST_WEDDING_RING)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_150"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1502_01"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(STG5_64_01_WEDDING_RING_DELIVERY)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_151"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1512_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_65_TRAIN_TICKET, STG7_20_LORD_CRUMP_DEFEATED)
        SET_VALUE(LF(0), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG7_02_MAYOR_BOB)
            IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG7_02_MAYOR_BOB)
                IF_INT_EQUAL(GSWF_NOK_GENERAL_WHITE_HINT_HEARD, 1)
                    SET_VALUE(GSWF_MUJ_01_WHITE_HINT_HEARD_UNUSED, 1)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 3)
                CALL(evt_msg_print, 0, STRING("stg5_muj_152"), 0, STRING("me"))
            ELSE
                CALL(evt_msg_print, 0, STRING("stg5_muj_1522_01"), 0, STRING("me"))
            END_IF
        ELSE
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 3)
                CALL(evt_msg_print, 0, STRING("stg7_muj_01"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg7_muj_012_01"), 0, 0)
            END_IF
        END_IF
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            // Late in the game, he remarks that the camp is quiet but dull.
            CALL(evt_msg_print, 0, STRING("stg5_muj_153"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1532_01"), 0, STRING("me"))
        END_IF
        CASE_DEFAULT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_153_01"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_153_01_san"), 0, STRING("me"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of Flavio (NPC マルコ) in the camp: placed at (-64, 0, 170) facing right for STG5_11 to 18_01
 * and STG5_19_01 to 34.
 */
static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_11_DESERTED_ISLAND, STG5_18_01_BOBBERY_JOINS)
        CALL(evt_npc_set_position, STRING("me"), -64, 0, 170)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_BETWEEN(STG5_19_01_ENTERING_THE_GROTTO, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_position, STRING("me"), -64, 0, 170)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Turns Flavio (マルコ) from LW(0) to LW(1) degrees about y over 150 ms; marco_talk calls it in pairs
 * (0 to 180 and back) so he glances around while talking.
 */
EVT_BEGIN(marco_kyorokyoro)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(0), LW(1), 150)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, STRING("マルコ"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Talk event of Flavio: during STG5_13 to 17 he starts settoku_event_main
 * if the timid sailor has not yet been persuaded, otherwise he talks according to the story stage.
 */
static EVT_BEGIN(marco_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_17_WINE_OBTAINED)
        IF_INT_EQUAL(GSWF_MUJ_01_BOBBERY_MISSING_SCENE_SEEN, 0)
            // While Bobbery is being found, the timid sailor must first be persuaded.
            CALL_CHILD(PTR(settoku_event_main))
        END_IF
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_12_FLAVIO_PA_PATCH)
        // Flavio asks what Mario has found.
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_0"), 0, STRING("me"))
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_13_BOBBERY_THE_BRIDGE)
        IF_INT_EQUAL(GSWF_MUJ_01_FLAVIO_RESCUE_ORDER_HEARD, 0)
            // Flavio brushes off talk of the admiral and tells Mario to hurry.
            CALL(evt_msg_print, 0, STRING("stg5_muj_31"), 0, STRING("me"))
            SET_VALUE(GSWF_MUJ_01_FLAVIO_RESCUE_ORDER_HEARD, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_32"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(STG5_15_RUNAWAY_SAILORS)
        // Mario asks Flavio for Chuckola Cola to help the admiral.
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_01"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02"), 0, STRING("me"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_cam3d_evt_set, LW(0), 97, 459, LW(0), 45, 163, 500, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_01"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, STG5_16_FLAVIOS_WINE)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        CASE_EQUAL(STG5_16_FLAVIOS_WINE)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("マルコ"), LW(10), LW(11), LW(12))
        CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_NO_HIT)
        IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
            ADD(LW(10), -30)
            CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(120.0f))
            CALL(evt_mario_set_dir, 90, 200, 0)
            CALL(evt_party_move_behind_mario, 0)
        ELSE
            ADD(LW(10), 30)
            CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(120.0f))
            CALL(evt_mario_set_dir, 270, 200, 0)
            CALL(evt_party_move_behind_mario, 0)
        END_IF
        CALL(evt_npc_flag_onoff, 0, STRING("マルコ"), NPC_FLAG_NO_HIT)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_02"), 0, STRING("me"))
        CALL(make_itemsel_table, LW(11))
        IF_INT_EQUAL(LW(11), 0)
            CALL(delete_itemsel_table)
            RETURN
        END_IF
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, LW(10), LW(0))
#else
        CALL(evt_win_item_select, 0, LW(10), LW(0), 0)
#endif
        CALL(delete_itemsel_table)
        IF_INT_EQUAL(LW(0), -1)
            RETURN
        END_IF
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        ADD(LW(4), 20)
        ADD(LW(5), 10)
        CALL(evt_mario_get_reqdir, LW(6))
        IF_FLOAT_EQUAL(LW(6), FLOAT(0.0f))
            ADD(LW(3), -20)
        ELSE
            ADD(LW(3), 20)
        END_IF
        CALL(evt_item_entry, STRING("watasu"), LW(0), LW(3), LW(4), LW(5), ITEM_TYPE_STILL, -1, 0)
        CALL(evt_item_flag_on, STRING("watasu"), ITEM_FLAG_NO_TOUCH_GET)
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_1"))
        WAIT_MS(500)
        CALL(evt_item_delete, STRING("watasu"))
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
        END_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_2"))
        WAIT_MS(500)

        // The reset is LF(1), while the food-range tests below set and inspect LF(0).
        SET_VALUE(LF(1), 0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 165)
            IF_INT_LESS(LW(0), 177)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(0), 148)
            IF_INT_LESS(LW(0), 161)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(0), 179)
            IF_INT_LESS(LW(0), 236)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        CALL(evt_pouch_remove_item, LW(0), LW(1))
        IF_INT_EQUAL(LF(0), 0)
            // Flavio inspects the item he is offered.
            CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_03"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_NOT_EQUAL(LW(0), ITEM_COCONUT)
            CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_04"), 0, STRING("me"))
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_05"), 0, STRING("me"))
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_2"))
        WAIT_MS(500)

        // Flavio hands over the Chuckola Cola.
        CALL(evt_item_entry, STRING("itemwine"), ITEM_CHUCKOLA_COLA, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
        CALL(evt_item_flag_on, STRING("itemwine"), ITEM_FLAG_NO_TOUCH_GET)
        CALL(evt_item_get_item, STRING("itemwine"))
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_03"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, STG5_17_WINE_OBTAINED)
        CASE_EQUAL(STG5_17_WINE_OBTAINED)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_03"), 0, STRING("me"))
        CASE_EQUAL(STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32"), 0, STRING("me"))
        CASE_EQUAL(STG5_18_01_BOBBERY_JOINS)
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_cam3d_evt_set, -60, 95, 491, -60, 35, 146, 500, EASING_SINE_IN_OUT)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_party_stop, 0)
#else
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_mov_pos2, -34, 162, FLOAT(120.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
#endif
        CALL(evt_mario_get_party, LW(0))
        SET_VALUE(LF(10), 0)
        IF_INT_NOT_EQUAL(LW(0), 3)
            SET_VALUE(LF(10), 1)
            CALL(evt_mario_hello_exparty_pos, 15, -25, 0, 210)
        END_IF
#if VERSION <= VERSION_DEMO_US
        SET_VALUE(GF(0), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, 0, 170, FLOAT(120.0f))
#else
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        SET_VALUE(GF(0), 1)
        BEGIN_INLINE_EVENT
            IF_INT_EQUAL(LW(0), 3)
                CALL(evt_party_move_pos2, 0, 0, 170, FLOAT(120.0f))
            ELSE
                CALL(evt_party_move_pos2, 0, 20, 170, FLOAT(120.0f))
            END_IF
#endif
            CALL(evt_party_set_dir, 0, 270, -1)
            SET_VALUE(GF(0), 0)
        END_INLINE_EVENT

        // Bobbery rejoins and Flavio welcomes him back.
        CALL(evt_msg_print, 0, STRING("stg5_muj_71"), 0, STRING("me"))
        WAIT_WHILE(GF(0))
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_msg_print_party, STRING("stg5_muj_72"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_72_bom"), 0, STRING("extparty"))
        END_IF
        CALL(evt_msg_print, 0, STRING("stg5_muj_79"), 0, STRING("me"))
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_80"), 0, STRING("party"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_80"), 0, STRING("extparty"))
        END_IF
        CALL(evt_set_dir_to_target, STRING("ガラの悪い水夫"), STRING("me"))

        // The rough sailor heckles Flavio.
        CALL(evt_msg_print, 0, STRING("stg5_muj_81"), 0, STRING("ガラの悪い水夫"))
        CALL(evt_npc_get_position, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SAILER_FART1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg5_muj_81_01"), 0, STRING("ガラの悪い水夫"))
        CALL(evt_msg_print, 0, STRING("stg5_muj_81_02"), 0, STRING("ガラの悪い水夫"))
        CALL(evt_msg_print, 0, STRING("stg5_muj_82"), 0, STRING("me"))
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_83"), 0, STRING("party"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_83"), 0, STRING("extparty"))
        END_IF
#if VERSION > VERSION_DEMO_US
        CALL(evt_party_run, 1)
#endif
        // Flavio stammers an excuse and paces nervously.
        CALL(evt_msg_print, 0, STRING("stg5_muj_84"), 0, STRING("me"))
        LOOP(3)
            SET_VALUE(LW(0), 0)
            SET_VALUE(LW(1), 180)
            CALL_CHILD(PTR(marco_kyorokyoro))
            WAIT_MS(160)
            SET_VALUE(LW(0), 180)
            SET_VALUE(LW(1), 0)
            CALL_CHILD(PTR(marco_kyorokyoro))
            WAIT_MS(160)
        END_LOOP
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("stg5_muj_85"), 0, STRING("me"))
        CALL(evt_cam3d_evt_set, -60, 144, 753, -60, 40, 162, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        BEGIN_INLINE_EVENT
            // The sailors cheer at the thought of Flavio going away.
            CALL(evt_msg_print, 0, STRING("stg5_muj_86"), 0, STRING("ガラの悪い水夫"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("水夫A"), STRING("me"))
            WAIT_MS(500)
            CALL(evt_npc_get_position, STRING("水夫A"), LW(0), LW(1), LW(2))
            ADD(LW(0), 20)
            ADD(LW(1), 40)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg5_muj_87"), 0, 0)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("水夫ボム兵"), STRING("me"))
            WAIT_MS(1000)
            CALL(evt_msg_print, 0, STRING("stg5_muj_88"), 0, STRING("水夫ボム兵"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("オドオド水夫"), STRING("me"))
            WAIT_MS(1500)
            CALL(evt_npc_get_position, STRING("オドオド水夫"), LW(0), LW(1), LW(2))
            ADD(LW(0), -30)
            ADD(LW(1), 40)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_togelr, 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_88_01"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(3000)
#if VERSION >= VERSION_US
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_89"), 0, STRING("party"))
        ELSE
            CALL(evt_party_stop, 1)
            CALL(evt_msg_print, 0, STRING("stg5_muj_89"), 0, STRING("extparty"))
            CALL(evt_party_run, 1)
        END_IF
#else
        CALL(evt_msg_print, 0, STRING("stg5_muj_89"), 0, STRING("me"))
#endif
        LOOP(3)
            SET_VALUE(LW(0), 0)
            SET_VALUE(LW(1), 180)
            CALL_CHILD(PTR(marco_kyorokyoro))
            WAIT_MS(160)
            SET_VALUE(LW(0), 180)
            SET_VALUE(LW(1), 0)
            CALL_CHILD(PTR(marco_kyorokyoro))
            WAIT_MS(160)
        END_LOOP
        WAIT_MS(1000)

        // Flavio calls it unfair but joins the party for a short time.
        CALL(evt_msg_print, 0, STRING("stg5_muj_90"), 0, STRING("me"))
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg5_sys_00"), 0, 0)
        IF_INT_EQUAL(LF(10), 1)
            CALL(evt_mario_goodbye_party, 1)
        END_IF
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CALL(evt_party_stop, 1)
        CALL(evt_mario_set_party_pos, 1, 9, LW(0), LW(1), LW(2))
        WAIT_MS(1000)
        CALL(evt_party_run, 1)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        SET_VALUE(GSW_STORY_PROGRESS, STG5_18_02_FLAVIOS_FAVOR_2)
        CALL(evt_party_run, 0)
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_10"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the bespectacled sailor: placed in the camp between STG5_13_BOBBERY_THE_BRIDGE
 * and STG5_30_SHIPWRECKED_SAILORS.
 */
static EVT_BEGIN(megane_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_30_SHIPWRECKED_SAILORS)
        CALL(evt_npc_set_position, STRING("me"), 115, 0, 247)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of the bespectacled sailor (めがね水夫): stg5_muj_32_01 (wondering what happened to Admiral Bobbery)
 * until Bobbery is rescued; at STG5_18_01_BOBBERY_JOINS a line that depends on whether Bobbery (party 3)
 * is
 * the partner out; later stage lines.
 */
static EVT_BEGIN(megane_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_01"), 0, STRING("me"))
        CASE_EQUAL(STG5_18_01_BOBBERY_JOINS)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_02"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_03"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_18_02_FLAVIOS_FAVOR_2, STG5_18_03_MARCOS_JEWEL)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_01"), 0, STRING("me"))
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_12"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the timid sailor (Odoodo): placed in the camp between STG5_13_BOBBERY_THE_BRIDGE
 * and STG5_34_KICKED_OFF.
 */
static EVT_BEGIN(odoodo_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_position, STRING("me"), 95, 0, 82)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of the timid sailor (オドオド水夫): stg5_muj_32_00 until Bobbery is rescued;
 * at STG5_18_01_BOBBERY_JOINS a joyful line when Bobbery (party 3) is the partner out, else a worried one;
 * later stage lines.
 */
static EVT_BEGIN(odoodo_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_00"), 0, STRING("me"))
        CASE_EQUAL(STG5_18_01_BOBBERY_JOINS)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_00"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(STG5_18_02_FLAVIOS_FAVOR_2, STG5_18_03_MARCOS_JEWEL)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_00"), 0, STRING("me"))
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_11"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of sailor A: placed in the camp between STG5_11_DESERTED_ISLAND
 * and STG5_43_X_NAUTS_RETREAT.
 */
static EVT_BEGIN(suifu_a_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_11_DESERTED_ISLAND, STG5_43_X_NAUTS_RETREAT)
        CALL(evt_npc_set_position, STRING("me"), -193, 0, 77)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of sailor A: a different line for each story stage (stg5_muj_22_2, stg5_muj_32_03 and others). */
static EVT_BEGIN(suifu_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_12_FLAVIO_PA_PATCH)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_2"), 0, STRING("me"))
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_03"), 0, STRING("me"))
        CASE_BETWEEN(STG5_18_01_BOBBERY_JOINS, STG5_18_03_MARCOS_JEWEL)
        CALL(evt_msg_print, 0, STRING("stg5_muj_71_06"), 0, STRING("me"))
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_14"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_16"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the bomb sailor: placed in the camp between STG5_11_DESERTED_ISLAND
 * and STG5_43_X_NAUTS_RETREAT.
 */
static EVT_BEGIN(suifu_bomb_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_11_DESERTED_ISLAND, STG5_43_X_NAUTS_RETREAT)
        CALL(evt_npc_set_position, STRING("me"), -33, 0, 74)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of the bomb sailor: a different line for each story stage (stg5_muj_22_3, stg5_muj_32_04 and others). */
static EVT_BEGIN(suifu_bomb_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_12_FLAVIO_PA_PATCH)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_3"), 0, STRING("me"))
        CASE_BETWEEN(STG5_13_BOBBERY_THE_BRIDGE, STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_04"), 0, STRING("me"))
        CASE_BETWEEN(STG5_18_01_BOBBERY_JOINS, STG5_18_03_MARCOS_JEWEL)
        CALL(evt_msg_print, 0, STRING("stg5_muj_71_07"), 0, STRING("me"))
        CASE_BETWEEN(STG5_19_GEMS_AND_JINGLES, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_15"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_17"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of sailor D: from STG5_59_ROGUEPORT_DOCK on he is set up to wander (suifu_d_regl). */
static EVT_BEGIN(suifu_d_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), 25, 0, 150)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Regular event of sailor D: once the story reaches STG5_59_ROGUEPORT_DOCK he wanders around his spawn
 * point (urouro) within a radius of 100 by 30 units.
 */
EVT_BEGIN(suifu_d_regl)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
        CALL(urouro_main_func, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of sailor D: lines stg5_muj_159 to 163 for the stretches of the story from Fair Winds to the end of
 * the Hideout arc.
 */
static EVT_BEGIN(suifu_d_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_58_FAIR_WINDS, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_msg_print, 0, STRING("stg5_muj_159"), 0, STRING("me"))
        CASE_BETWEEN(STG5_62_AN_ILL_DON_PIANTA, STG5_64_LOST_WEDDING_RING)
        CALL(evt_msg_print, 0, STRING("stg5_muj_160"), 0, STRING("me"))
        CASE_EQUAL(STG5_64_01_WEDDING_RING_DELIVERY)
        CALL(evt_msg_print, 0, STRING("stg5_muj_161"), 0, STRING("me"))
        CASE_BETWEEN(STG5_65_TRAIN_TICKET, STG7_26_THE_HIDEOUT_EXPLODES)
        CALL(evt_msg_print, 0, STRING("stg5_muj_162"), 0, STRING("me"))
        CASE_BETWEEN(STG7_50_A_WAITING_PROF_FRANKLY, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg5_muj_163"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_163_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the shabby sailor A: after STG5_59_ROGUEPORT_DOCK he is set up to wander. */
EVT_BEGIN(borosuifu_a_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), 95, 0, 82)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of the shabby sailor A: a different line (stg5_muj_154 to 157) for each stretch of the story from
 * Fair Winds to the end of the Hideout arc.
 */
EVT_BEGIN(borosuifu_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_58_FAIR_WINDS, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_msg_print, 0, STRING("stg5_muj_154"), 0, STRING("me"))
        CASE_BETWEEN(STG5_62_AN_ILL_DON_PIANTA, STG5_64_LOST_WEDDING_RING)
        CALL(evt_msg_print, 0, STRING("stg5_muj_155"), 0, STRING("me"))
        CASE_EQUAL(STG5_64_01_WEDDING_RING_DELIVERY)
        CALL(evt_msg_print, 0, STRING("stg5_muj_156"), 0, STRING("me"))
        CASE_BETWEEN(STG5_65_TRAIN_TICKET, STG7_26_THE_HIDEOUT_EXPLODES)
        CALL(evt_msg_print, 0, STRING("stg5_muj_157"), 0, STRING("me"))
        CASE_BETWEEN(STG7_50_A_WAITING_PROF_FRANKLY, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg5_muj_158"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_158_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the shabby sailor B: turns on NPC_STATUS_FACE_MOVE_DIR and,
 * after STG5_59_ROGUEPORT_DOCK, sets him up to wander.
 */
EVT_BEGIN(borosuifu_b_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), -350, 0, 0)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Regular event of the shabby sailor B: wanders around his spawn point once the story is past
 * STG5_59_ROGUEPORT_DOCK.
 */
EVT_BEGIN(borosuifu_b_regl)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG5_59_ROGUEPORT_DOCK)
        LOOP(0)
            CALL(evt_npc_move_position, STRING("me"), 350, 0, 0, FLOAT(40.0f), 0)
            WAIT_MS(1000)
            CALL(evt_npc_move_position, STRING("me"), -350, 0, 0, FLOAT(40.0f), 0)
            WAIT_MS(1000)
        END_LOOP
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of the shabby sailor B: a different line (stg5_muj_164 to 167) for each stretch of the story,
 * as for sailor A.
 */
EVT_BEGIN(borosuifu_b_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_58_FAIR_WINDS, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_msg_print, 0, STRING("stg5_muj_164"), 0, STRING("me"))
        CASE_BETWEEN(STG5_62_AN_ILL_DON_PIANTA, STG5_64_LOST_WEDDING_RING)
        CALL(evt_msg_print, 0, STRING("stg5_muj_165"), 0, STRING("me"))
        CASE_EQUAL(STG5_64_01_WEDDING_RING_DELIVERY)
        CALL(evt_msg_print, 0, STRING("stg5_muj_166"), 0, STRING("me"))
        CASE_BETWEEN(STG5_65_TRAIN_TICKET, STG7_26_THE_HIDEOUT_EXPLODES)
        CALL(evt_msg_print, 0, STRING("stg5_muj_167"), 0, STRING("me"))
        CASE_BETWEEN(STG7_50_A_WAITING_PROF_FRANKLY, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg5_muj_168"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_168_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of Mony in the camp: placed between STG5_59_ROGUEPORT_DOCK and STG5_61_FRANKLYS_HOUSE_STG_5. */
static EVT_BEGIN(mony_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_59_ROGUEPORT_DOCK, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_npc_set_position, STRING("me"), 105, 0, 274)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of Mony in the camp: says stg5_muj_147. */
static EVT_BEGIN(mony_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_147"), 0, STRING("me"))
    RETURN
EVT_END()

/** Init event of Peton in the camp: placed between STG5_59_ROGUEPORT_DOCK and STG5_61_FRANKLYS_HOUSE_STG_5. */
static EVT_BEGIN(peton_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_59_ROGUEPORT_DOCK, STG5_61_FRANKLYS_HOUSE_STG_5)
        CALL(evt_npc_set_position, STRING("me"), 45, 0, 274)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of Peton in the camp: says stg5_muj_148. */
static EVT_BEGIN(peton_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_148"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk event shared by the three dancers (musume): each says her line in turn (stg5_muj_168_01 to 03). */
static EVT_BEGIN(musume_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_01"), 0, STRING("３人娘Ａ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_02"), 0, STRING("３人娘Ｂ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_03"), 0, STRING("３人娘Ｃ"))
    RETURN
EVT_END()

/**
 * Adds the first dancer (3人娘Ａ, model c_dance_r, the red one) with walk and talk animations and musume_talk;
 * called from the ending map init.
 */
static EVT_BEGIN(musume_a_entry)
    CALL(evt_npc_entry, STRING("３人娘Ａ"), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ａ"), STRING("踊り子"))
    CALL(evt_npc_set_position, STRING("３人娘Ａ"), 200, -10, 360)
    CALL(evt_npc_set_ry, STRING("３人娘Ａ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ａ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/** Adds the second dancer (3人娘Ｂ, model c_dance_p, the pink one) with walk and talk animations and musume_talk. */
static EVT_BEGIN(musume_b_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｂ"), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｂ"), STRING("踊り子ピンク"))
    CALL(evt_npc_set_position, STRING("３人娘Ｂ"), 235, -7, 310)
    CALL(evt_npc_set_ry, STRING("３人娘Ｂ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ｂ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ｂ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/**
 * Adds the third dancer (3人娘Ｃ, model c_dance_k, the yellow-green one) with walk and talk animations
 * and musume_talk.
 */
static EVT_BEGIN(musume_c_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｃ"), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｃ"), STRING("踊り子黄緑"))
    CALL(evt_npc_set_position, STRING("３人娘Ｃ"), 280, -10, 300)
    CALL(evt_npc_set_ry, STRING("３人娘Ｃ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ｃ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ｃ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/**
 * NPCs of the camp: the rough sailor, Flavio, the bespectacled sailor, the nervous sailor, the shop and inn
 * clerks, sailors A, bomb and D, the two ragged sailors, Mony, Peeton and the three Elmos ghosts.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "ガラの悪い水夫", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        garawaru_init, // initEvent
        NULL, // regularEvent
        garawaru_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "マルコ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        marco_init, // initEvent
        NULL, // regularEvent
        marco_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "めがね水夫", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        megane_init, // initEvent
        NULL, // regularEvent
        megane_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "オドオド水夫", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        odoodo_init, // initEvent
        NULL, // regularEvent
        odoodo_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "店員", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "宿店員", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "水夫A", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_a_init, // initEvent
        NULL, // regularEvent
        suifu_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "水夫ボム兵", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_bomb_init, // initEvent
        NULL, // regularEvent
        suifu_bomb_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "水夫D", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_d_init, // initEvent
        suifu_d_regl, // regularEvent
        suifu_d_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ボロ水夫A", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        borosuifu_a_init, // initEvent
        NULL, // regularEvent
        borosuifu_a_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ボロ水夫B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        borosuifu_b_init, // initEvent
        borosuifu_b_regl, // regularEvent
        borosuifu_b_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "モニー", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        mony_init, // initEvent
        NULL, // regularEvent
        mony_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ピートン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        peton_init, // initEvent
        NULL, // regularEvent
        peton_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "エルモスA", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "エルモスB", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "エルモスC", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/** Enter event of both camp exits (w_bero and e_bero1 in bero_entry_data): only clears LW(0). */
static EVT_BEGIN(bero_in_event)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Doors and exits of muj_01: w_bero to muj_00; e_bero1 to muj_02. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Custom door-open event for the Keelhaul Key houses:
 * like the standard door-open event but does nothing when evt_mario_party_use_check returns 4 or
 * when Mario is more than 30 units from the door;
 * otherwise takes control, shows the interior and runs the door's open event.
 */
EVT_BEGIN(muj_00_door_open)
    CALL(evt_mario_party_use_check, LW(4))
    IF_INT_EQUAL(LW(4), 4)
        RETURN
    END_IF
    CALL(muj_get_door_check, LW(0), FLOAT(30.0f), LW(1))
    IF_INT_EQUAL(LW(1), 0)
        RETURN
    END_IF
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_door_param, LW(0), DOOR_PARAM_OPEN_CLOSE_EVTS, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUTSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_CLIP)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_door_param, LW(0), DOOR_PARAM_IN_POS_FROM_MARIO, LW(1), LW(2), LW(3))
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        ADD(LW(5), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(1), LW(5), LW(3), 0, 150, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUTSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_dark))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    CALL(evt_door_param, LW(0), DOOR_PARAM_IN_POS_FROM_MARIO, LW(1), LW(2), LW(3))
    CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_party_move_behind_mario, 1)
#endif
    CALL(evt_cam3d_evt_xyz_off, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Custom door-close event: the matching close for muj_00_door_open,
 * run when Mario leaves through the door (it does nothing when evt_mario_party_use_check returns 4 or when he is
 * too far from the door).
 */
EVT_BEGIN(muj_00_door_close)
    CALL(evt_mario_party_use_check, LW(4))
    IF_INT_EQUAL(LW(4), 4)
        RETURN
    END_IF
    CALL(muj_get_door_check, LW(0), FLOAT(30.0f), LW(1))
    IF_INT_EQUAL(LW(1), 0)
        RETURN
    END_IF
    CALL_CHILD(PTR(evt_door_close))
    RETURN
EVT_END()

/** NPCs inside the camp shop: the shop clerk. */
static const char* mise_npc[] = {
    "店員",
    NULL,
};

/**
 * Open event of the shop building: shows the interior,
 * fades the outside blend away and lowers the music and ambience to a muffled 50 percent with a low-pass filter.
 */
EVT_BEGIN(mise_data_open)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_mise_kan"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_mise_hasi04"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_mise_hasi05"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    SPAWN_EVENT(PTR(custom_door_dark2))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/**
 * Close event of the shop building: restores the exterior blend flags,
 * fades the light back in and restores the music and ambience volume and filter.
 */
EVT_BEGIN(mise_data_close)
    BEGIN_INLINE_EVENT
        CALL_CHILD(PTR(custom_door_light2))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_mise_kan"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_mise_hasi04"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_mise_hasi05"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of the camp shop: the outer door hit area A_mise_doa, the inside room S_mise and the scripts that run
 * as the door opens and closes.
 */
static DoorParam mise_data = {
    0, // flags
    3, // type
    0, NULL, // se..mapobj
    "A_mise_in_doa", // hitIn
    "A_mise_doa", // hitOut
    NULL, NULL, // outside..outsideHit
    "S_mise", // inside
    NULL, NULL, NULL, NULL, // insideHit..evtWall
    mise_npc, // npcs
    NULL, NULL, // mobjs..items
    mise_data_open, // evtOpen
    mise_data_close, // evtClose
};

/** NPCs inside the camp inn: the innkeeper. */
static const char* yado_npc[] = {
    "宿店員",
    NULL,
};

/** Open event of the inn: darkens the outside, shows the interior (S_yado) and muffles the music and ambience. */
EVT_BEGIN(mise_open_event)
    SPAWN_EVENT(PTR(door_dark))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yado"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    SPAWN_EVENT(PTR(custom_door_dark2))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Close event of the inn: lights the outside again, hides the interior and restores music and ambience. */
EVT_BEGIN(mise_close_event)
    SPAWN_EVENT(PTR(door_light))
    SPAWN_EVENT(PTR(custom_door_light2))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_yado"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of the camp inn: the outer door hit area A_yado_doa, the inside room and the scripts that run as the
 * door opens and closes.
 */
static DoorParam yado_data = {
    0, // flags
    3, // type
    0, NULL, // se..mapobj
    "A_yado_in_doa", // hitIn
    "A_yado_doa", // hitOut
    NULL, NULL, // outside..outsideHit
    "S_yado_in", // inside
    NULL, NULL, NULL, NULL, // insideHit..evtWall
    yado_npc, // npcs
    NULL, NULL, // mobjs..items
    mise_open_event, // evtOpen
    mise_close_event, // evtClose
};

/** Registers muj_00_door_open and muj_00_door_close as the door cases for the first custom door of the map. */
EVT_BEGIN(evt_door_entry_custom00)
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 17)
        CALL(evt_run_case_evt, CASE_FRONT, 1, LW(2), 0, PTR(muj_00_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 17)
        CALL(evt_run_case_evt, CASE_FRONT, 1, LW(3), 0, PTR(muj_00_door_open), 0)
    END_IF
    RETURN
EVT_END()

/** Registers muj_00_door_open and muj_00_door_close as the door cases for the second custom door of the map. */
EVT_BEGIN(evt_door_entry_custom01)
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(2), 0)
        SET_VALUE(LW(15), 19)
        CALL(evt_run_case_evt, CASE_FRONT, 1, LW(2), 0, PTR(muj_00_door_close), 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LW(15), 19)
        CALL(evt_run_case_evt, CASE_FRONT, 1, LW(3), 0, PTR(muj_00_door_open), 0)
    END_IF
    RETURN
EVT_END()

/** Shelf slots of the camp shop: the display model and hit area of each of the six items on show. */
static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

/**
 * Items the camp shop sells with their prices in coins: Ice Storm, Fire Flower, Sleepy Sheep, Fright Mask,
 * Honey Syrup and Super Shroom.
 */
static ShopItem goods_list[] = {
    {ITEM_ICE_STORM, 15},
    {ITEM_FIRE_FLOWER, 5},
    {ITEM_SLEEPY_SHEEP, 10},
    {ITEM_FRIGHT_MASK, 2},
    {ITEM_HONEY_SYRUP, 3},
    {ITEM_SUPER_SHROOM, 12},
};

/** Prices the camp shop pays when Mario sells it items; the stocked goods come first. */
static ShopDb trade_list[] = {
    {ITEM_ICE_STORM, 10, 0},
    {ITEM_FIRE_FLOWER, 2, 0},
    {ITEM_SLEEPY_SHEEP, 5, 0},
    {ITEM_FRIGHT_MASK, 1, 0},
    {ITEM_HONEY_SYRUP, 1, 0},
    {ITEM_SUPER_SHROOM, 6, 0},
    {ITEM_TASTY_TONIC, 5, 0},
    {ITEM_HOT_DOG, 13, 0},
    {ITEM_ICICLE_POP, 20, 0},
    {ITEM_ZESS_FRAPPE, 35, 0},
    {ITEM_SNOW_BUNNY, 15, 0},
    {0},
};

/**
 * The shop clerk's name and the 35 shop messages (shop_00 to shop_34) that drive the buying and selling
 * dialogue.
 */
static ShopperData35 shopper_data = {
    "店員", // name
    NULL, NULL, // fook_evt..buy_evt
    {"shop_00", "shop_01", "shop_02", "shop_03", "shop_04", "shop_05", "shop_06", "shop_07", "shop_08", "shop_09", "shop_10", "shop_11", "shop_12", "shop_13", "shop_14", "shop_15", "shop_16", "shop_17", "shop_18", "shop_19", "shop_20", "shop_21", "shop_22", "shop_23", "shop_24", "shop_25", "shop_26", "shop_27", "shop_28", "shop_29", "shop_30", "shop_31", "shop_32", "shop_33", "shop_34"}, // msgs
};

/**
 * Inn data of the camp: the price of a night, the bed, the bedside and camera positions, the innkeeper and
 * the 14 inn messages (yado_00 to yado_13).
 */
static KinopioData kino_dt = {
    0, // breakfastItem
    {0}, // breakfastPos
    10, // ryoukin
#if VERSION <= VERSION_DEMO_US
    {399.0f, 16.0f, -185.0f},
#else
    {403.0f, 17.0f, -182.0f}, // bed
#endif
    {355.0f, 0.0f, -155.0f}, // bedside
    {213.0f, 122.0f, -97.0f, 341.0f, 45.0f, -178.0f}, // cam
    -30.0f, // bedDeg
    {352.0f, 0.0f, -135.0f}, // npcTalk
    "宿店員", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

/**
 * The sailors' homesickness (kyoshu): Mario and his partner overhear the crew (stg5_muj_15 to 22);
 * Flavio asks whether Mario will help, then the story advances to STG5_12_FLAVIO_PA_PATCH
 * and control returns.
 */
EVT_BEGIN(kyoshu_event)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_position, STRING("水夫A"), -193, 0, 77)
    CALL(evt_npc_set_ry, STRING("水夫A"), 90)
    CALL(evt_npc_set_position, STRING("水夫ボム兵"), -33, 0, 74)
    CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 270)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -155, 0, 170)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_set_position, STRING("マルコ"), -64, 0, 170)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    CALL(evt_mario_set_pos, -110, 0, 230)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -167, 0, 268)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_cam3d_evt_set, -110, 119, 615, -110, 40, 172, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_muj_15"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_16"), 0, STRING("マルコ"))
    CALL(evt_msg_select, 0, STRING("stg5_muj_16_yesno"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg5_muj_17"))
    ELSE
        CALL(evt_msg_print_add, 0, STRING("stg5_muj_18"))
        BEGIN_INLINE_EVENT
            LOOP(3)
                CALL(evt_mario_set_dir, 270, 150, 0)
                WAIT_MS(160)
                CALL(evt_mario_set_dir, 90, 150, 0)
                WAIT_MS(160)
            END_LOOP
            CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
            CALL(evt_mario_set_pose, STRING("M_I_N"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_msg_print, 0, STRING("stg5_muj_19"), 0, STRING("ガラの悪い水夫"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("stg5_muj_20"), 0, STRING("水夫A"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            CALL(evt_msg_print, 0, STRING("stg5_muj_21"), 0, STRING("水夫ボム兵"))
        END_INLINE_EVENT
        WAIT_MS(2500)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22"), 0, STRING("マルコ"))
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_12_FLAVIO_PA_PATCH)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    RETURN
EVT_END()

/** Makes Flavio shiver (muj_npc_buruburu, 1 unit every 10 ms for 10 seconds). */
EVT_BEGIN(maruco_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("マルコ"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("マルコ"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

/** Makes the shabby sailor Garawaru shiver (muj_npc_buruburu for 10 seconds). */
EVT_BEGIN(garawaru_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("ガラの悪い水夫"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

/** Makes sailor A shiver (muj_npc_buruburu for 10 seconds). */
EVT_BEGIN(suifu_a_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("水夫A"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("水夫A"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

/** Makes the bomb-shaped sailor shiver (muj_npc_buruburu for 10 seconds). */
EVT_BEGIN(suifu_bom_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("水夫ボム兵"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("水夫ボム兵"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

/**
 * The crew splits (nakamaware): Mario walks into the camp at STG5_11_DESERTED_ISLAND,
 * the sailors argue about heading home (stg5_muj_05 to 14), and three Embers (ghost flames) appear and attack;
 * the fight is battle info 6. Afterwards kyoshu_event plays.
 * The GSWF_MUJ01_SKIP_TO_SHOW_OF_HANDS flag jumps straight to kyoshu_event.
 */
EVT_BEGIN(nakamaware_event)
    // If the quarrel was already seen, go straight to the show of hands.
    IF_INT_EQUAL(GSWF_MUJ_01_CREW_SPLIT_SKIP, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_cam3d_evt_set, -85, 134, 714, -85, 38, 172, 500, EASING_SINE_IN_OUT)
        SPAWN_EVENT(PTR(kyoshu_event))
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("水夫ボム兵"), -33, 0, 74)
    CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 270)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -122, 0, 170)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_set_position, STRING("マルコ"), -64, 0, 170)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスA"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスB"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスC"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -390)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_set_position, STRING("水夫A"), -220, 0, 0)
    CALL(evt_npc_move_position, STRING("水夫A"), -330, 0, 0, FLOAT(120.0f), 0)

    // Sailor A begs the captain to stop Flavio and the rough sailor, who are about to fight.
    CALL(evt_msg_print, 0, STRING("stg5_muj_05"), 0, STRING("水夫A"))
    CALL(evt_mario_mov_pos2, -385, 0, FLOAT(120.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("水夫A"), 90)
        CALL(evt_npc_move_position, STRING("水夫A"), -193, 77, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER3"))
    CALL(evt_cam3d_evt_set, -95, 128, 605, -95, 50, 162, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos2, -247, 26, FLOAT(120.0f))
    CALL(evt_mario_mov_pos2, -170, 174, FLOAT(120.0f))
    WAIT_MS(1000)

    // Flavio and the rough sailor trade insults about who was the coward.
    CALL(evt_msg_print, 0, STRING("stg5_muj_06"), 0, STRING("マルコ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_07"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_npc_get_position, STRING("マルコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), -5)
    CALL(evt_npc_move_position, STRING("マルコ"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_08"), 0, STRING("マルコ"))
    CALL(evt_npc_get_position, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2))
    ADD(LW(0), 5)
    CALL(evt_npc_move_position, STRING("ガラの悪い水夫"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_09"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_npc_set_autotalkpose, STRING("マルコ"), STRING("K_1"), STRING("K_2"))
    CALL(evt_npc_set_anim, STRING("マルコ"), STRING("K_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_10"), 0, STRING("マルコ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_11"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_npc_set_autotalkpose, STRING("マルコ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_anim, STRING("マルコ"), STRING("S_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 90)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
        CALL(evt_npc_move_position, STRING("ガラの悪い水夫"), 128, 166, 0, FLOAT(100.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    CALL(evt_npc_move_position, STRING("マルコ"), 98, 166, 0, FLOAT(100.0f), 0)
    WAIT_MS(1000)

    // Flavio shrieks that the ghosts are here and the sailors cower.
    CALL(evt_msg_print, 0, STRING("stg5_muj_12"), 0, STRING("マルコ"))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MARCO_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_move_position, STRING("ガラの悪い水夫"), -200, 166, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
        SPAWN_EVENT_GET_ID(PTR(garawaru_buruburu), GW(0))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("マルコ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MARCO_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マルコ"), -185, 170, 0, FLOAT(240.0f), 0)
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    SPAWN_EVENT_GET_ID(PTR(maruco_buruburu), GW(1))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -170, 128, 605, -170, 50, 162, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_13"), 0, STRING("ガラの悪い水夫"))
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST1"))
    CALL(evt_cam3d_evt_set, 181, 134, 692, 181, 38, 150, 500, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_npc_set_position, STRING("水夫A"), -224, 0, 48)
    CALL(evt_npc_set_ry, STRING("水夫A"), 90)
    CALL(evt_npc_set_position, STRING("水夫ボム兵"), -197, 0, 83)
    CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 90)
    SPAWN_EVENT_GET_ID(PTR(suifu_a_buruburu), GW(2))
    SPAWN_EVENT_GET_ID(PTR(suifu_bom_buruburu), GW(3))
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_party_stop, 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 5)
        CALL(evt_party_set_pos, 0, -180, -3, 265)
    ELSE
        CALL(evt_party_set_pos, 0, -180, -3, 300)
    END_IF
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(evt_npc_set_position, STRING("エルモスB"), 460, 50, 45)
        CALL(evt_npc_glide_position, STRING("エルモスB"), 135, 50, 45, 0, FLOAT(120.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING("エルモスB"), -46, 10, 151, 0, FLOAT(120.0f), 0, 11, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_npc_set_position, STRING("エルモスC"), 430, 50, 45)
        CALL(evt_npc_glide_position, STRING("エルモスC"), 105, 50, 45, 0, FLOAT(120.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING("エルモスC"), 5, 10, 169, 0, FLOAT(120.0f), 0, 11, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_position, STRING("エルモスA"), 445, 20, 45)
        CALL(evt_npc_get_position, STRING("エルモスA"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING("エルモスA"), 120, 20, 45, 0, FLOAT(120.0f), 0, 11, 0)
        CALL(evt_npc_get_position, STRING("エルモスA"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING("エルモスA"), -28, 10, 229, 0, FLOAT(120.0f), 0, 11, 0)
    END_INLINE_EVENT
    WAIT_MS(3200)
    CALL(evt_cam3d_evt_set, -100, 136, 714, -100, 40, 172, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // Flavio pushes Mario to the front.
    CALL(evt_msg_print, 0, STRING("stg5_muj_14"), 0, STRING("マルコ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        WAIT_MS(75)
        CALL(evt_npc_glide_position, STRING("エルモスB"), -146, 10, 151, 0, FLOAT(120.0f), 0, 11, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_npc_glide_position, STRING("エルモスC"), -95, 10, 169, 0, FLOAT(120.0f), 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("エルモスA"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_glide_position, STRING("エルモスA"), -128, 10, 229, 0, FLOAT(120.0f), 0, 11, 0)
    CALL(evt_npc_release_filednpc, 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    WAIT_MS(100)
    CALL(evt_npc_set_battle_info, STRING("エルモスA"), 6)

    // Mario fights the Elmos ghosts.
    CALL(evt_npc_battle_start, STRING("エルモスA"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    DELETE_EVENT(GW(0))
    DELETE_EVENT(GW(1))
    DELETE_EVENT(GW(2))
    DELETE_EVENT(GW(3))
    CALL(evt_npc_flag_onoff, 0, STRING("マルコ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("エルモスA"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("エルモスB"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("エルモスC"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_AUTO_MOVE_ANIM)
    SPAWN_EVENT(PTR(kyoshu_event))
    RETURN
EVT_END()

/** Moves the sailors and Flavio to their places in the camp for the persuasion scene. */
EVT_BEGIN(settoku_event)
    CALL(evt_npc_set_position, STRING("水夫A"), -203, 0, 61)
    CALL(evt_npc_set_ry, STRING("水夫A"), 90)
    CALL(evt_npc_set_position, STRING("水夫ボム兵"), -220, 0, 160)
    CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 90)
    CALL(evt_npc_set_position, STRING("マルコ"), -152, 0, 150)
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    CALL(evt_npc_set_position, STRING("めがね水夫"), -45, 0, 150)
    CALL(evt_npc_set_position, STRING("オドオド水夫"), -10, 0, 150)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -182, 0, 210)
    RETURN
EVT_END()

/**
 * Flavio persuades the timid sailor (settoku): everyone turns to face Flavio, he speaks (stg5_muj_29),
 * the timid sailor answers (stg5_muj_30) and GSWF_MUJ01_TIMID_SAILOR_PERSUADED is set.
 */
EVT_BEGIN(settoku_event_main)
    CALL(evt_set_dir_to_target, STRING("水夫A"), STRING("マルコ"))
    CALL(evt_set_dir_to_target, STRING("水夫ボム兵"), STRING("マルコ"))
    CALL(evt_set_dir_to_target, STRING("ガラの悪い水夫"), STRING("マルコ"))
    CALL(evt_set_dir_to_target, STRING("めがね水夫"), STRING("マルコ"))
    CALL(evt_set_dir_to_target, STRING("オドオド水夫"), STRING("マルコ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_29"), 0, STRING("マルコ"))
    CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("オドオド水夫"), -26, 180, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_move_position, STRING("オドオド水夫"), -105, 180, 0, FLOAT(120.0f), 0)
    CALL(evt_set_dir_to_target, STRING("オドオド水夫"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_30"), 0, STRING("オドオド水夫"))
    CALL(evt_npc_flag_onoff, 0, STRING("オドオド水夫"), NPC_FLAG_NO_HIT)
    SET_VALUE(GSWF_MUJ_01_BOBBERY_MISSING_SCENE_SEEN, 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg5_muj_31"), 0, STRING("マルコ"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
    RETURN
EVT_END()

/**
 * Stops the regular events of only two sailors, shabby sailor B (ボロ水夫B) and sailor D (水夫D), so they stay put;
 * the map init runs it before sekai_yami_evt or minnnanokoe_evt (STORY_STG8_15_GRODUS).
 */
static EVT_BEGIN(delete_npc_regl_evt)
    CALL(evt_npc_change_interrupt, STRING("ボロ水夫B"), 1, 0)
    CALL(evt_npc_get_reglid, STRING("ボロ水夫B"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_change_interrupt, STRING("水夫D"), 1, 0)
    CALL(evt_npc_get_reglid, STRING("水夫D"), LW(0))
    DELETE_EVENT(LW(0))
    RETURN
EVT_END()

/** Shakes the 3D camera sideways by a small amount every 500 ms, forever; used in the dark ending scene. */
static EVT_BEGIN(sekai_yami_shake)
    LOOP(0)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_LOOP
    RETURN
EVT_END()

/**
 * Script helper: copies Mario's held items (pouch->heldItems, up to 20) into a map-memory table ended by -1,
 * stores the count in args[0] and the table address in LW(10), for an item selection menu.
 */
static USER_FUNC(make_itemsel_table) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32* p;
    s32 count;
    s32 i;

    itemsel_table_ram = _mapAlloc(0x54);
    p = itemsel_table_ram;
    count = 0;
    for (i = 0; i < 20; i++) {
        s32 item = pouch->heldItems[i];

        if (item > 0) {
            *p++ = item;
            count++;
        }
    }
    *p = -1;
    evtSetValue(event, args[0], count);
    event->lwData[10] = (EvtWord)itemsel_table_ram;
    return EVT_RETURN_DONE;
}

/** Script helper: frees the item selection table made by make_itemsel_table. */
static USER_FUNC(delete_itemsel_table) {
    _mapFree(itemsel_table_ram);
    return EVT_RETURN_DONE;
}

/** Script helper: writes the direction Mario is currently requested to face (displayDirectionTarget) into args[0]. */
USER_FUNC(evt_mario_get_reqdir) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], marioGetPtr()->displayDirectionTarget);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: writes 1 into args[2] if Mario is within args[1] units (horizontal) of the door's outside hit
 * box, otherwise 0. args[0] is the door parameter block.
 */
USER_FUNC(muj_get_door_check) {
    EvtWord* args = event->args;
    DoorParam* door = (DoorParam*)evtGetValue(event, args[0]);
    f32 range = evtGetFloat(event, args[1]);
    Vec hitPos;
    Vec marioPos;

    hitObjGetPos(door->hitOut, &hitPos);
    marioPos = marioGetPtr()->position;
    marioPos.y = hitPos.y = 0.0f;
    if (PSVECDistance(&hitPos, &marioPos) > range) {
        evtSetValue(event, args[2], 0);
    } else {
        evtSetValue(event, args[2], 1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sets the "dark world" view matrix,
 * a camera looking down from height 100 at the offset z in args[0];
 * used by the dark ending scene (after STORY_STG8_15_GRODUS) where the camp goes dark.
 */
static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: turns on a TEV callback slot (args[1], low 4 bits) in the material of the NPC named args[0],
 * so the NPC is drawn through the map's colour callback. Does nothing if the NPC does not exist.
 */
static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/** Script helper: the same TEV callback switch for a map object (args[0]) instead of an NPC. */
static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtrNoAssert(name);

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(mobj->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/** Script helper for the dark-world scene: tints the six shop icons shp0 to shp5 dark grey (0x40, 0x40, 0x40). */
USER_FUNC(sekai_yami_omise) {
    char name[40];
    s32 i;
    IconEntry* icon;

    for (i = 0; i < 6; i++) {
        sprintf(name, "shp%d", i);
        icon = iconNameToPtr(name);
        icon->color = (GXColor){0x40, 0x40, 0x40, 0xFF};
    }
    return EVT_RETURN_DONE;
}

/** Script helper: creates a Star Stone effect (type 4) named args[0] at (args[1], args[2], args[3]). */
static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(4, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

/** Script helper: moves the named Star Stone effect to (args[1], args[2], args[3]). */
static USER_FUNC(starstone_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

/** Script helper: rotates the named Star Stone effect around Y by args[1] degrees. */
static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

/**
 * The world-goes-dark cutaway at STORY_STG8_15_GRODUS (entered from "sekai_yami2"):
 * control is taken, the camp is drawn dark with the camera shaking and a quake sound,
 * four sailors run back and forth in panic, someone screams for help (stg8_dark_06), the screen flashes,
 * and the game returns to pik_01 at "sekai_yami2".
 */
/**
 * The world-goes-dark cutaway at STG8_15_GRODUS (entered from "sekai_yami2"):
 * control is taken, the camp is drawn dark with the camera shaking and a quake sound,
 * four sailors run back and forth in panic, someone screams for help (stg8_dark_06), the screen flashes,
 * and the game returns to pik_01 at "sekai_yami2".
 */
static EVT_BEGIN(sekai_yami_evt)
    // The darkening of the world: Mario loses control and the island starts to shake.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ボロ水夫A"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), 0, 0, 170)
    CALL(evt_npc_status_onoff, 1, STRING("ガラの悪い水夫"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_status_onoff, 1, STRING("水夫D"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_status_onoff, 1, STRING("オドオド水夫"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_status_onoff, 1, STRING("店員"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ガラの悪い水夫"), 1)
    CALL(anim_tevcallback, STRING("水夫D"), 1)
    CALL(anim_tevcallback, STRING("オドオド水夫"), 1)
    CALL(anim_tevcallback, STRING("店員"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(sekai_yami_omise)
    CALL(evt_cam3d_evt_set, 0, 120, 795, 0, 70, 348, 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT(PTR(sekai_yami_shake))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(0))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_move_position, STRING("ガラの悪い水夫"), 155, 170, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_move_position, STRING("ガラの悪い水夫"), -150, 170, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_move_position, STRING("水夫D"), -70, 76, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_move_position, STRING("水夫D"), 120, 76, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_move_position, STRING("オドオド水夫"), -60, 82, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_move_position, STRING("オドオド水夫"), -90, 82, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_position, STRING("店員"), 4, 0, -69)
        LOOP(0)
            CALL(evt_npc_move_position, STRING("店員"), -124, -69, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_move_position, STRING("店員"), 4, -69, 0, FLOAT(120.0f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)

    // Someone screams as the darkness spreads.
    CALL(evt_msg_print, 0, STRING("stg8_dark_06"), 0, 0)
    BEGIN_INLINE_EVENT
#if VERSION <= VERSION_DEMO_US
        WAIT_MS(1000)
        LOOP(2)
#else
#if VERSION >= VERSION_US
        WAIT_MS(100)
        LOOP(1)
#else
        WAIT_MS(500)
        LOOP(2)
#endif
#endif

            // The screen flashes white several times.
            CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
            CALL(evt_fade_entry, FADE_OUT_BLACK, 0, 255, 255, 255)
            WAIT_MS(100)
            CALL(evt_fade_entry, FADE_IN_BLACK, 0, 255, 255, 255)
#if VERSION <= VERSION_DEMO_US
            WAIT_MS(100)
            END_LOOP
    END_INLINE_EVENT
    WAIT_MS(3000)
#else
#if VERSION >= VERSION_US
            WAIT_MS(400)
#else
            WAIT_MS(100)
#endif
        END_LOOP
    END_INLINE_EVENT
#if VERSION >= VERSION_US
    WAIT_MS(1200)
#else
    WAIT_MS(1000)
#endif
#endif
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)

    // Fade out and continue the scene at pik_01.
    CALL(evt_bero_mapchange, STRING("pik_01"), STRING("sekai_yami2"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    // Everyone's voices: Mario loses control and the crew gathers.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -50, 0, 200)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("水夫D"), 50, 0, 200)
    CALL(evt_npc_set_ry, STRING("水夫D"), 270)
    CALL(evt_npc_set_position, STRING("ボロ水夫A"), -10, 0, 100)
    CALL(evt_npc_set_ry, STRING("ボロ水夫A"), 270)
    CALL(evt_npc_set_position, STRING("ボロ水夫B"), 25, 0, 75)
    CALL(evt_npc_set_ry, STRING("ボロ水夫B"), 270)
    CALL(evt_npc_set_position, STRING("店員"), 0, -1000, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ガラの悪い水夫"), 1)
    CALL(anim_tevcallback, STRING("水夫D"), 1)
    CALL(anim_tevcallback, STRING("ボロ水夫A"), 1)
    CALL(anim_tevcallback, STRING("ボロ水夫B"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(sekai_yami_omise)
    CALL(evt_cam3d_evt_set, 0, 95, 548, 0, 55, 200, 0, EASING_SINE_IN_OUT)
    CALL(starstone_entry, STRING("starstone"), 0, 50, 0)
    SET_VALUE(GW(0), 50)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), 0, GW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("starstone"), LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5000, 5500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5500, 5000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)

    // The sailors cheer the fighter on: don't give up, thump them.
    CALL(evt_msg_print, 0, STRING("stg8_las_148_38"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_39"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_40"), 0, STRING("ボロ水夫B"))
    CALL(evt_cam3d_evt_set, 0, 300, 1180, 0, 100, 200, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_npc_jump_position_nohit, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2), 300, 10)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_41"), 0, STRING("ガラの悪い水夫"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_42"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_43"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_44"), 0, STRING("ボロ水夫B"))
    CALL(evt_npc_jump_position_nohit, STRING("ガラの悪い水夫"), LW(0), LW(1), LW(2), 300, 10)

    // The cheering grows louder.
    CALL(evt_msg_print, 0, STRING("stg8_las_148_45"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_46"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_47"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_48"), 0, STRING("ボロ水夫B"))
    CALL(evt_cam3d_evt_set, 0, 95, 548, 0, 55, 200, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_49"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_50"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_51"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_52"), 0, STRING("ボロ水夫B"))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)

    // Fade out and continue the scene at pik_01.
    CALL(evt_bero_mapchange, STRING("pik_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

/**
 * Adds the epilogue NPCs Goombella (クリスチーヌ) and Bobbery (サンダース, placed off screen),
 * and stops sailor D's regular event and moves him beside them.
 */
static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 0, 0, 200)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_entry, STRING("サンダース"), STRING("c_bomt_n"))
    CALL(evt_npc_set_tribe, STRING("サンダース"), STRING("サンダース"))
    CALL(evt_npc_set_position, STRING("サンダース"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("サンダース"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("サンダース"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_reglid, STRING("水夫D"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_change_interrupt, STRING("水夫D"), 1, 0)
    CALL(evt_npc_set_position, STRING("水夫D"), 24, 0, 70)
    RETURN
EVT_END()

/**
 * Animation loop of the epilogue: Goombella and the rough sailor (ガラの悪い水夫) take turns talking,
 * a speech bubble pops over Goombella, the sailor turns around, Bobbery walks in from the left,
 * and then all three keep taking turns talking.
 */
static EVT_BEGIN(epilogue_evt_sub)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("S_1"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クリスチーヌ"), 0, 0, 0, 0, 0, 0, 0, 36)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -40, 143, 791, -40, 39, 200, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ガラの悪い水夫"))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("サンダース"), -235, 0, 200)
    CALL(evt_npc_move_position, STRING("サンダース"), -80, 200, 0, 40, 0)
    LOOP(0)
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("S_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("サンダース"), STRING("T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("サンダース"), STRING("S_1"))
    END_LOOP
    RETURN
EVT_END()

/**
 * The camp's epilogue scene (STG8_END_AFTER_THE_BATTLE):
 * with the Keelhaul Key music, the epilogue narration says that Bobbery now goes out to sea with Cortez every
 * day (ep_42); then the ending moves to Ms. Mowz's scene in gor_01 if she joined the party, else to pik_04.
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -40, 0, 160)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -20, 143, 791, -20, 39, 200, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_42"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 900, FADE_IN_BLACK, 900)
    CALL(evt_mario_chk_join_party, LW(0), 7)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_bero_mapchange, STRING("gor_01"), STRING(""))
    ELSE
        CALL(evt_bero_mapchange, STRING("pik_04"), STRING(""))
    END_IF
    RETURN
EVT_END()

/** Plays the kettle (yakan) animation of the map and keeps replaying it when it finishes. */
EVT_BEGIN(yakan_anime)
    LOOP(0)
        CALL(evt_map_playanim, STRING("yakan"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_checkanim, STRING("yakan"), LW(0), LW(1))
        WAIT_MS(LW(1))
    END_LOOP
    RETURN
EVT_END()

/**
 * Map init for muj_01 (the camp): picks music (ending-scene variants after STORY_STG8_15_GRODUS),
 * registers the exits, Flavio's shop and the inn, the save block and Star Pieces,
 * then spawns the scene for the story stage (nakamaware_event, settoku_event or the ending epilogue).
 */
EVT_BEGIN(muj_01_init_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL(evt_snd_bgmoff, PSND_FADEOUT)
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ3"))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
    CALL(evt_npc_sound_data_set, STRING("マルコ"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_00_START)
        IF_INT_EQUAL(LF(0), 0)
            CALL_CHILD(musume_a_entry)
            CALL_CHILD(musume_b_entry)
            CALL_CHILD(musume_c_entry)
        END_IF
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("fuku_hata"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(yakan_anime)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 100, 60, -25, 0, 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 475, 0, 100, ITEM_TYPE_STILL, GSWF_STARPIECE_72, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, -200, -16, 485, ITEM_TYPE_STILL, GSWF_STARPIECE_73, 0)
    SET_VALUE(LW(0), PTR(&mise_data))
    SPAWN_EVENT(evt_door_entry_custom00)
    SET_VALUE(LW(0), PTR(&yado_data))
    SPAWN_EVENT(evt_door_entry_custom01)
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_11_DESERTED_ISLAND)
        SPAWN_EVENT(nakamaware_event)
        CASE_GREATER_OR_EQUAL(STG5_13_BOBBERY_THE_BRIDGE)
        IF_INT_EQUAL(GSWF_MUJ_01_BOBBERY_MISSING_SCENE_SEEN, 0)
            SPAWN_EVENT(settoku_event)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(1))
        IF_STR_EQUAL(LW(1), STRING("sekai_yami2"))
            CALL_CHILD(delete_npc_regl_evt)
            SPAWN_EVENT(sekai_yami_evt)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("minnnanokoe"))
            CALL_CHILD(delete_npc_regl_evt)
            SPAWN_EVENT(minnnanokoe_evt)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_mise"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

