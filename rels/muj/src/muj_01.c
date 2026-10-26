#include "battle/battle_unit.h"
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
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_reqdir);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_win_item_select);
static USER_FUNC(make_itemsel_table);
USER_FUNC(muj_get_door_check);
USER_FUNC(sekai_yami_omise);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
static USER_FUNC(yami_view);
extern s32 envSetYami[];

void envSetYamiView(Mtx mtx);
EvtWord custom_door_dark2[34];
EvtWord custom_door_light2[30];
static EvtWord garawaru_init[39];
static EvtWord garawaru_talk[270];
static EvtWord marco_init[31];
EvtWord marco_kyorokyoro[28];
#ifdef VERSION_G8ME01
static EvtWord marco_talk[848];
#else
static EvtWord marco_talk[831];
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
EvtWord muj_00_door_open[247];
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

EVT_BEGIN(custom_door_dark2)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, 300)
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

EVT_BEGIN(custom_door_light2)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 255, 300)
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

static EVT_BEGIN(garawaru_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(232, 238)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
        CASE_EQUAL(239)
        CALL(evt_npc_set_position, STRING("me"), -110, 0, 225)
        CASE_BETWEEN(240, 254)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
        CASE_GREATER_OR_EQUAL(269)
        CALL(evt_npc_set_position, STRING("me"), -155, 0, 170)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(garawaru_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(233)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_1"), 0, STRING("me"))
        CASE_BETWEEN(234, 238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_02"), 0, STRING("me"))
        CASE_EQUAL(239)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_05"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_04"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(240, 241)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_02"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_02_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(242, 254)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_13"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_90_132_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(268, 272)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_149"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1492_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(273, 275)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_150"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1502_01"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(276)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_151"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_1512_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(279, 373)
        SET_VALUE(LF(0), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 354)
            IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 354)
                IF_INT_EQUAL(GSWF(3877), 1)
                    SET_VALUE(GSWF(3878), 1)
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
        CASE_LESS(401)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 3)
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

static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(232, 239)
        CALL(evt_npc_set_position, STRING("me"), -64, 0, 170)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_BETWEEN(243, 254)
        CALL(evt_npc_set_position, STRING("me"), -64, 0, 170)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

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

static EVT_BEGIN(marco_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(234, 237)
        IF_INT_EQUAL(GSWF(3129), 0)
            CALL_CHILD(PTR(settoku_event_main))
        END_IF
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(233)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_0"), 0, STRING("me"))
        CASE_BETWEEN(234, 234)
        IF_INT_EQUAL(GSWF(3127), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_31"), 0, STRING("me"))
            SET_VALUE(GSWF(3127), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_32"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(235)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_01"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02"), 0, STRING("me"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_cam3d_evt_set, LW(0), 97, 459, LW(0), 45, 163, 500, 11)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_01"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, 236)
        CALL(evt_cam3d_evt_off, 500, 11)
        WAIT_MS(500)
        CASE_EQUAL(236)
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
        CALL(evt_win_item_select, 0, LW(10), LW(0), 0)
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
        CALL(evt_item_flag_on, STRING("watasu"), 2)
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
            CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_03"), 0, STRING("me"))
            RETURN
        END_IF
        IF_INT_NOT_EQUAL(LW(0), 166)
            CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_04"), 0, STRING("me"))
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_02_05"), 0, STRING("me"))
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("me"), STRING("I_2"))
        WAIT_MS(500)
        CALL(evt_item_entry, STRING("itemwine"), ITEM_CHUCKOLA_COLA, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
        CALL(evt_item_flag_on, STRING("itemwine"), 2)
        CALL(evt_item_get_item, STRING("itemwine"))
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_03"), 0, STRING("me"))
        SET_VALUE(GSW_STORY_PROGRESS, 237)
        CASE_EQUAL(237)
        CALL(evt_msg_print, 0, STRING("stg5_muj_31_03"), 0, STRING("me"))
        CASE_EQUAL(238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32"), 0, STRING("me"))
        CASE_EQUAL(239)
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_cam3d_evt_set, -60, 95, 491, -60, 35, 146, 500, 11)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_mov_pos2, -34, 162, FLOAT(120.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_get_party, LW(0))
        SET_VALUE(LF(10), 0)
        IF_INT_NOT_EQUAL(LW(0), 3)
            SET_VALUE(LF(10), 1)
            CALL(evt_mario_hello_exparty_pos, 15, -25, 0, 210)
        END_IF
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        SET_VALUE(GF(0), 1)
        BEGIN_INLINE_EVENT
            IF_INT_EQUAL(LW(0), 3)
                CALL(evt_party_move_pos2, 0, 0, 170, FLOAT(120.0f))
                ELSE
                CALL(evt_party_move_pos2, 0, 20, 170, FLOAT(120.0f))
            END_IF
            CALL(evt_party_set_dir, 0, 270, -1)
            SET_VALUE(GF(0), 0)
        END_INLINE_EVENT
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
        CALL(evt_party_run, 1)
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
        CALL(evt_cam3d_evt_set, -60, 144, 753, -60, 40, 162, 500, 11)
        WAIT_MS(500)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_print, 0, STRING("stg5_muj_86"), 0, STRING("ガラの悪い水夫"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, STRING("水夫A"), STRING("me"))
            WAIT_MS(500)
            CALL(evt_npc_get_position, STRING("水夫A"), LW(0), LW(1), LW(2))
            ADD(LW(0), 20)
            ADD(LW(1), 40)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
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
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_togelr, 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_88_01"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(3000)
#ifdef VERSION_G8ME01
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
        CALL(evt_msg_print, 0, STRING("stg5_muj_90"), 0, STRING("me"))
        CALL(evt_msg_toge, 1, 0, 0, 0)
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
        CALL(evt_cam3d_evt_off, 500, 11)
        SET_VALUE(GSW_STORY_PROGRESS, 240)
        CALL(evt_party_run, 0)
        CASE_BETWEEN(242, 254)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_10"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(megane_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(234, 250)
        CALL(evt_npc_set_position, STRING("me"), 115, 0, 247)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(megane_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(234, 238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_01"), 0, STRING("me"))
        CASE_EQUAL(239)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_02"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_03"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(240, 241)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_01"), 0, STRING("me"))
        CASE_BETWEEN(242, 254)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_12"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(odoodo_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(234, 254)
        CALL(evt_npc_set_position, STRING("me"), 95, 0, 82)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(odoodo_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(234, 238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_00"), 0, STRING("me"))
        CASE_EQUAL(239)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_00"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_71_01"), 0, STRING("me"))
        END_IF
        CASE_BETWEEN(240, 241)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_00"), 0, STRING("me"))
        CASE_BETWEEN(242, 254)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_11"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_a_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(232, 261)
        CALL(evt_npc_set_position, STRING("me"), -193, 0, 77)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(233)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_2"), 0, STRING("me"))
        CASE_BETWEEN(234, 238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_03"), 0, STRING("me"))
        CASE_BETWEEN(239, 241)
        CALL(evt_msg_print, 0, STRING("stg5_muj_71_06"), 0, STRING("me"))
        CASE_BETWEEN(242, 254)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_14"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(255)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_16"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_bomb_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(232, 261)
        CALL(evt_npc_set_position, STRING("me"), -33, 0, 74)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_bomb_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(233)
        CALL(evt_msg_print, 0, STRING("stg5_muj_22_3"), 0, STRING("me"))
        CASE_BETWEEN(234, 238)
        CALL(evt_msg_print, 0, STRING("stg5_muj_32_04"), 0, STRING("me"))
        CASE_BETWEEN(239, 241)
        CALL(evt_msg_print, 0, STRING("stg5_muj_71_07"), 0, STRING("me"))
        CASE_BETWEEN(242, 254)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_15"), 0, STRING("me"))
        CASE_GREATER_OR_EQUAL(255)
        CALL(evt_msg_print, 0, STRING("stg5_muj_90_17"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_d_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(269)
        CALL(evt_npc_set_position, STRING("me"), 25, 0, 150)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(suifu_d_regl)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(269)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
        CALL(urouro_main_func, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_d_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(268, 272)
        CALL(evt_msg_print, 0, STRING("stg5_muj_159"), 0, STRING("me"))
        CASE_BETWEEN(273, 275)
        CALL(evt_msg_print, 0, STRING("stg5_muj_160"), 0, STRING("me"))
        CASE_EQUAL(276)
        CALL(evt_msg_print, 0, STRING("stg5_muj_161"), 0, STRING("me"))
        CASE_BETWEEN(279, 377)
        CALL(evt_msg_print, 0, STRING("stg5_muj_162"), 0, STRING("me"))
        CASE_BETWEEN(378, 402)
        CALL(evt_msg_print, 0, STRING("stg5_muj_163"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_163_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borosuifu_a_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(269)
        CALL(evt_npc_set_position, STRING("me"), 95, 0, 82)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borosuifu_a_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(268, 272)
        CALL(evt_msg_print, 0, STRING("stg5_muj_154"), 0, STRING("me"))
        CASE_BETWEEN(273, 275)
        CALL(evt_msg_print, 0, STRING("stg5_muj_155"), 0, STRING("me"))
        CASE_EQUAL(276)
        CALL(evt_msg_print, 0, STRING("stg5_muj_156"), 0, STRING("me"))
        CASE_BETWEEN(279, 377)
        CALL(evt_msg_print, 0, STRING("stg5_muj_157"), 0, STRING("me"))
        CASE_BETWEEN(378, 402)
        CALL(evt_msg_print, 0, STRING("stg5_muj_158"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_158_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borosuifu_b_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(269)
        CALL(evt_npc_set_position, STRING("me"), -350, 0, 0)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borosuifu_b_regl)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(269)
        LOOP(0)
            CALL(evt_npc_move_position, STRING("me"), 350, 0, 0, FLOAT(40.0f), 0)
            WAIT_MS(1000)
            CALL(evt_npc_move_position, STRING("me"), -350, 0, 0, FLOAT(40.0f), 0)
            WAIT_MS(1000)
        END_LOOP
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(borosuifu_b_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(268, 272)
        CALL(evt_msg_print, 0, STRING("stg5_muj_164"), 0, STRING("me"))
        CASE_BETWEEN(273, 275)
        CALL(evt_msg_print, 0, STRING("stg5_muj_165"), 0, STRING("me"))
        CASE_EQUAL(276)
        CALL(evt_msg_print, 0, STRING("stg5_muj_166"), 0, STRING("me"))
        CASE_BETWEEN(279, 377)
        CALL(evt_msg_print, 0, STRING("stg5_muj_167"), 0, STRING("me"))
        CASE_BETWEEN(378, 402)
        CALL(evt_msg_print, 0, STRING("stg5_muj_168"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg5_muj_168_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mony_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(269, 272)
        CALL(evt_npc_set_position, STRING("me"), 105, 0, 274)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mony_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_147"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(peton_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(269, 272)
        CALL(evt_npc_set_position, STRING("me"), 45, 0, 274)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(peton_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_148"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(musume_talk)
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_01"), 0, STRING("３人娘Ａ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_02"), 0, STRING("３人娘Ｂ"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_168_03"), 0, STRING("３人娘Ｃ"))
    RETURN
EVT_END()

static EVT_BEGIN(musume_a_entry)
    CALL(evt_npc_entry, STRING("３人娘Ａ"), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ａ"), STRING("踊り子"))
    CALL(evt_npc_set_position, STRING("３人娘Ａ"), 200, -10, 360)
    CALL(evt_npc_set_ry, STRING("３人娘Ａ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ａ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

static EVT_BEGIN(musume_b_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｂ"), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｂ"), STRING("踊り子ピンク"))
    CALL(evt_npc_set_position, STRING("３人娘Ｂ"), 235, -7, 310)
    CALL(evt_npc_set_ry, STRING("３人娘Ｂ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ｂ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ｂ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

static EVT_BEGIN(musume_c_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｃ"), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｃ"), STRING("踊り子黄緑"))
    CALL(evt_npc_set_position, STRING("３人娘Ｃ"), 280, -10, 300)
    CALL(evt_npc_set_ry, STRING("３人娘Ｃ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("３人娘Ｃ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, STRING("３人娘Ｃ"), 6, PTR(musume_talk))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76", // name
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
        /*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52", // name
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
        /*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76", // name
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
        /*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76", // name
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
        /*"店員"*/"\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"水夫A"*/"\x90\x85\x95\x76\x41", // name
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
        /*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA", // name
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
        /*"水夫D"*/"\x90\x85\x95\x76\x44", // name
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
        /*"ボロ水夫A"*/"\x83\x7B\x83\x8D\x90\x85\x95\x76\x41", // name
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
        /*"ボロ水夫B"*/"\x83\x7B\x83\x8D\x90\x85\x95\x76\x42", // name
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
        /*"モニー"*/"\x83\x82\x83\x6A\x81\x5B", // name
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
        /*"ピートン"*/"\x83\x73\x81\x5B\x83\x67\x83\x93", // name
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
        /*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43", // name
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

static EVT_BEGIN(bero_in_event)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
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
        20000, // directionId
        100000, // centerPosition
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
    CALL(evt_door_param, LW(0), 7, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 0)
        SPAWN_EVENT(LW(1))
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_save_mapflag, LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_CLIP)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_door_param, LW(0), 10, LW(1), LW(2), LW(3))
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        ADD(LW(5), 2)
        CALL(evt_cam3d_evt_set_xyz, LW(1), LW(5), LW(3), 0, 150, 11)
    END_INLINE_EVENT
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 1)
    END_IF
    CALL(door_dark_flag, LW(0), 1)
    IF_BITS_CLEAR(LW(15), 2)
        SPAWN_EVENT(PTR(door_dark))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    CALL(evt_door_param, LW(0), 10, LW(1), LW(2), LW(3))
    CALL(evt_mario_party_door_move, LW(1), LW(3), FLOAT(135.0f))
    CALL(evt_cam3d_evt_xyz_off, 0, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

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

static const char* mise_npc[] = {
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(mise_data_open)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mise_kan"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mise_hasi04"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mise_hasi05"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    SPAWN_EVENT(PTR(custom_door_dark2))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(mise_data_close)
    BEGIN_INLINE_EVENT
        CALL_CHILD(PTR(custom_door_light2))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mise_kan"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mise_hasi04"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_mise_hasi05"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

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

static const char* yado_npc[] = {
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(mise_open_event)
    SPAWN_EVENT(PTR(door_dark))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yado"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    SPAWN_EVENT(PTR(custom_door_dark2))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(mise_close_event)
    SPAWN_EVENT(PTR(door_light))
    SPAWN_EVENT(PTR(custom_door_light2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_yado"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

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

EVT_BEGIN(evt_door_entry_custom00)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
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

EVT_BEGIN(evt_door_entry_custom01)
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
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

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_ICE_STORM, 15},
    {ITEM_FIRE_FLOWER, 5},
    {ITEM_SLEEPY_SHEEP, 10},
    {ITEM_FRIGHT_MASK, 2},
    {ITEM_HONEY_SYRUP, 3},
    {ITEM_SUPER_SHROOM, 12},
};

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

static ShopperData35 shopper_data = {
    /*"店員"*/"\x93\x58\x88\xF5", // name
    NULL, NULL, // fook_evt..buy_evt
    {"shop_00", "shop_01", "shop_02", "shop_03", "shop_04", "shop_05", "shop_06", "shop_07", "shop_08", "shop_09", "shop_10", "shop_11", "shop_12", "shop_13", "shop_14", "shop_15", "shop_16", "shop_17", "shop_18", "shop_19", "shop_20", "shop_21", "shop_22", "shop_23", "shop_24", "shop_25", "shop_26", "shop_27", "shop_28", "shop_29", "shop_30", "shop_31", "shop_32", "shop_33", "shop_34"}, // msgs
};

static KinopioData kino_dt = {
    0, // breakfastItem
    {0}, // breakfastPos
    10, // ryoukin
    {403.0f, 17.0f, -182.0f}, // bed
    {355.0f, 0.0f, -155.0f}, // bedside
    {213.0f, 122.0f, -97.0f, 341.0f, 45.0f, -178.0f}, // cam
    -30.0f, // bedDeg
    {352.0f, 0.0f, -135.0f}, // npcTalk
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

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
    CALL(evt_cam3d_evt_set, -110, 119, 615, -110, 40, 172, 0, 11)
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
            CALL(evt_mario_dispflag_onoff, 1, 2)
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
        CALL(evt_mario_dispflag_onoff, 0, 2)
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 233)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    RETURN
EVT_END()

EVT_BEGIN(maruco_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("マルコ"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("マルコ"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(garawaru_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("ガラの悪い水夫"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(suifu_a_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("水夫A"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("水夫A"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(suifu_bom_buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING("水夫ボム兵"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_buruburu, STRING("水夫ボム兵"), 0, FLOAT(1.0f), 10, 10000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(nakamaware_event)
    IF_INT_EQUAL(GSWF(3126), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_cam3d_evt_set, -85, 134, 714, -85, 38, 172, 500, 11)
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
    CALL(evt_msg_print, 0, STRING("stg5_muj_05"), 0, STRING("水夫A"))
    CALL(evt_mario_mov_pos2, -385, 0, FLOAT(120.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("水夫A"), 90)
        CALL(evt_npc_move_position, STRING("水夫A"), -193, 77, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER3"))
    CALL(evt_cam3d_evt_set, -95, 128, 605, -95, 50, 162, 500, 11)
    CALL(evt_mario_mov_pos2, -247, 26, FLOAT(120.0f))
    CALL(evt_mario_mov_pos2, -170, 174, FLOAT(120.0f))
    WAIT_MS(1000)
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
    CALL(evt_msg_print, 0, STRING("stg5_muj_12"), 0, STRING("マルコ"))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_dispflag_onoff, 1, 2)
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
    CALL(evt_cam3d_evt_set, -170, 128, 605, -170, 50, 162, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_13"), 0, STRING("ガラの悪い水夫"))
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG5_GHOST1"))
    CALL(evt_cam3d_evt_set, 181, 134, 692, 181, 38, 150, 500, 11)
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
    CALL(evt_cam3d_evt_set, -100, 136, 714, -100, 40, 172, 1000, 11)
    WAIT_MS(1000)
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
    CALL(evt_mario_dispflag_onoff, 0, 2)
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
    CALL(evt_npc_release_fieldnpc, 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    WAIT_MS(100)
    CALL(evt_npc_set_battle_info, STRING("エルモスA"), 6)
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
    SET_VALUE(GSWF(3129), 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 235)
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

static EVT_BEGIN(delete_npc_regl_evt)
    CALL(evt_npc_change_interrupt, STRING("ボロ水夫B"), 1, 0)
    CALL(evt_npc_get_reglid, STRING("ボロ水夫B"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_change_interrupt, STRING("水夫D"), 1, 0)
    CALL(evt_npc_get_reglid, STRING("水夫D"), LW(0))
    DELETE_EVENT(LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_shake)
    LOOP(0)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_LOOP
    RETURN
EVT_END()

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

static USER_FUNC(delete_itemsel_table) {
    _mapFree(itemsel_table_ram);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_reqdir) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], marioGetPtr()->displayDirectionTarget);
    return EVT_RETURN_DONE;
}

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

static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtrNoAssert(name);

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(mobj->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

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

static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(4, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

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

static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ボロ水夫A"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), 0, 0, 170)
    CALL(evt_npc_status_onoff, 1, STRING("ガラの悪い水夫"), 2)
    CALL(evt_npc_status_onoff, 1, STRING("水夫D"), 2)
    CALL(evt_npc_status_onoff, 1, STRING("オドオド水夫"), 2)
    CALL(evt_npc_status_onoff, 1, STRING("店員"), 2)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ガラの悪い水夫"), 1)
    CALL(anim_tevcallback, STRING("水夫D"), 1)
    CALL(anim_tevcallback, STRING("オドオド水夫"), 1)
    CALL(anim_tevcallback, STRING("店員"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(sekai_yami_omise)
    CALL(evt_cam3d_evt_set, 0, 120, 795, 0, 70, 348, 0, 11)
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
    CALL(evt_msg_print, 0, STRING("stg8_dark_06"), 0, 0)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        WAIT_MS(100)
        LOOP(1)
#else
        WAIT_MS(500)
        LOOP(2)
#endif
            CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
            CALL(evt_fade_entry, 10, 0, 255, 255, 255)
            WAIT_MS(100)
            CALL(evt_fade_entry, 9, 0, 255, 255, 255)
#ifdef VERSION_G8ME01
            WAIT_MS(400)
#else
            WAIT_MS(100)
#endif
        END_LOOP
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    WAIT_MS(1200)
#else
    WAIT_MS(1000)
#endif
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("pik_01"), STRING("sekai_yami2"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
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
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ガラの悪い水夫"), 1)
    CALL(anim_tevcallback, STRING("水夫D"), 1)
    CALL(anim_tevcallback, STRING("ボロ水夫A"), 1)
    CALL(anim_tevcallback, STRING("ボロ水夫B"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(sekai_yami_omise)
    CALL(evt_cam3d_evt_set, 0, 95, 548, 0, 55, 200, 0, 11)
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
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 5000, 5500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 5500, 5000, 500)
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
    CALL(evt_msg_print, 0, STRING("stg8_las_148_38"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_39"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_40"), 0, STRING("ボロ水夫B"))
    CALL(evt_cam3d_evt_set, 0, 300, 1180, 0, 100, 200, 0, 11)
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
    CALL(evt_msg_print, 0, STRING("stg8_las_148_45"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_46"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_47"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_48"), 0, STRING("ボロ水夫B"))
    CALL(evt_cam3d_evt_set, 0, 95, 548, 0, 55, 200, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_49"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_50"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_51"), 0, STRING("ボロ水夫A"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_52"), 0, STRING("ボロ水夫B"))
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("pik_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

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

static EVT_BEGIN(epilogue_evt_sub)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ガラの悪い水夫"), STRING("S_1"))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("クリスチーヌ"), 0, 0, 0, 0, 0, 0, 0, 36)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -40, 143, 791, -40, 39, 200, 300, 11)
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

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -40, 0, 160)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -20, 143, 791, -20, 39, 200, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_INSTANT_TURN)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_42"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_mario_chk_join_party, LW(0), 7)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_bero_mapchange, STRING("gor_01"), STRING(""))
        ELSE
        CALL(evt_bero_mapchange, STRING("pik_04"), STRING(""))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(yakan_anime)
    LOOP(0)
        CALL(evt_map_playanim, STRING("yakan"), 1, 0)
        CALL(evt_map_checkanim, STRING("yakan"), LW(0), LW(1))
        WAIT_MS(LW(1))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(muj_01_init_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL(evt_snd_bgmoff, PSND_FADEOUT)
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ3"))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_tribe, STRING(/*"モニー"*/"\x83\x82\x83\x6A\x81\x5B"), STRING(/*"モニー（指輪）"*/"\x83\x82\x83\x6A\x81\x5B\x81\x69\x8E\x77\x97\xD6\x81\x6A"))
    CALL(evt_npc_set_tribe, STRING(/*"ピートン"*/"\x83\x73\x81\x5B\x83\x67\x83\x93"), STRING(/*"ピートン（指輪）"*/"\x83\x73\x81\x5B\x83\x67\x83\x93\x81\x69\x8E\x77\x97\xD6\x81\x6A"))
    CALL(evt_npc_sound_data_set, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 382)
        IF_INT_EQUAL(LF(0), 0)
            CALL_CHILD(musume_a_entry)
            CALL_CHILD(musume_b_entry)
            CALL_CHILD(musume_c_entry)
        END_IF
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("fuku_hata"), 1, 0)
    SPAWN_EVENT(yakan_anime)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 100, 60, -25, 0, 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 475, 0, 100, ITEM_TYPE_STILL, GSWF(5639), 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, -200, -16, 485, ITEM_TYPE_STILL, GSWF(5640), 0)
    SET_VALUE(LW(0), PTR(&mise_data))
    SPAWN_EVENT(evt_door_entry_custom00)
    SET_VALUE(LW(0), PTR(&yado_data))
    SPAWN_EVENT(evt_door_entry_custom01)
    CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(232)
        SPAWN_EVENT(nakamaware_event)
        CASE_GREATER_OR_EQUAL(234)
        IF_INT_EQUAL(GSWF(3129), 0)
            SPAWN_EVENT(settoku_event)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
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
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_mise"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

