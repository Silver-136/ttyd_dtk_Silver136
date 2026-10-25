#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"

#include "eff/eff_stardust.h"
#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "mario/mario_pouch.h"

static USER_FUNC(effect_stardust);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_majinai_set);
USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(evt_win_item_select);
USER_FUNC(majinaisi_ryokin_kaisu);
static USER_FUNC(setmertn);
USER_FUNC(unitwincoin_disp_onoff);
USER_FUNC(unitwincoin_wait);

s32 majinaisi_ryokin_kaisu_tbl[6];
static BeroEntry bero_entry_data[2];
EvtWord suisyou_kirakira[30];
EvtWord majinaisi_table_talk[877];
s32 iri_14_item_tbl[2];
EvtWord majinaisi_direct_talk[108];
EvtWord majinaisi_init[10];
static NpcSetupInfo npcEnt[2];
EvtWord tik_12_init_evt[58];

s32 majinaisi_ryokin_kaisu_tbl[] = {
    50, 20, 20, 10, 5, 5,
};

static BeroEntry bero_entry_data[] = {
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
        "tik_01", // nextMapName
        "w_dero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(suisyou_kirakira)
    CALL(evt_mapobj_get_position, STRING("uranai"), LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    ADD(LW(2), 20)
    CALL(evt_eff64, STRING("aaax"), STRING("lensflare_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(majinaisi_table_talk)
    CALL(setmertn, STRING("まじない師"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_npc_set_ry, STRING("me"), 90)
    CALL(evt_msg_print, 0, STRING("majinaisi_01"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("majinaisi_02"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("majinaisi_03"))
        CALL(evt_party_run, 0)
        CALL(evt_party_run, 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("majinaisi_04"))
    CALL(evt_unitwin_alloc, 0)
    CALL(unitwincoin_disp_onoff, 1)
    CALL(evt_msg_select, 0, STRING("majinaisi_05"))
    IF_INT_EQUAL(LW(0), 3)
        CALL(unitwincoin_disp_onoff, 0)
        CALL(evt_unitwin_free)
        CALL(evt_msg_print_add, 0, STRING("majinaisi_03"))
        CALL(evt_party_run, 0)
        CALL(evt_party_run, 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(majinaisi_ryokin_kaisu, LW(0), LW(1), LW(2))
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        CALL(evt_msg_print_add, 0, STRING("majinaisi_06"))
        CALL(unitwincoin_disp_onoff, 0)
        CALL(evt_unitwin_free)
        CALL(evt_party_run, 0)
        CALL(evt_party_run, 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    MULTIPLY(LW(1), -1)
    CALL(evt_pouch_add_coin, LW(1))
    CALL(unitwincoin_wait)
    WAIT_MS(200)
    CALL(unitwincoin_disp_onoff, 0)
    CALL(evt_unitwin_free)
    CALL(evt_msg_print_add, 0, STRING("majinaisi_07"))
    LABEL(100)
    CALL(evt_npc_set_anim, STRING("me"), STRING("T_2"))
    CALL(evt_pouch_majinai_set, LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 75, 0, 60)
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 1, 105, 0, 60)
        CALL(evt_party_set_dir, 1, 270, 300)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 25, 0, 60)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("majinai_BOX"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    SUBTRACT(LW(3), LW(0))
    SUBTRACT(LW(5), LW(2))
    CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), LW(3), 0, LW(5))
    CALL(evt_npc_set_anim, STRING("me"), STRING("D_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_LIGHT1"), LW(0), LW(1), LW(2), GW(1))
    SPAWN_EVENT_GET_ID(PTR(suisyou_kirakira), GW(0))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("anm_sen"), 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("senn"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_checkanim, STRING("anm_sen"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("anm_men"), 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("group4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_checkanim, STRING("anm_men"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_npc_set_anim, STRING("me"), STRING("H_1"))
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 60, 1000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), LW(3), LW(0), LW(5))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_2"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 5)
        ADD(LW(1), 95)
        ADD(LW(2), 100)
        WAIT_MS(500)
        LOOP(8)
            CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE4_2"), LW(0), LW(1), LW(2), 0)
            CALL(evt_eff64, 0, STRING("crystal_n64"), 5, LW(0), LW(1), LW(2), FLOAT(1.0f), -1, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1200)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(13), LW(3))
    SET_VALUE(LW(14), LW(0))
    SET_VALUE(LW(15), LW(5))
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(-0.099609375f))
    SET_FLOAT_VALUE(LW(3), FLOAT(-1.8994141f))
    SET_FLOAT_VALUE(LW(4), FLOAT(-0.029296875f))
    CALL(evt_mario_get_pos, 0, LW(5), LW(6), LW(7))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE4"), LW(5), LW(6), LW(7), 0)
    LOOP(0)
        SET_VALUE(LF(0), 0)
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(-1800)
            SET_VALUE(LF(0), 1)
            CASE_LESS_OR_EQUAL(-360)
            ADD_FLOAT(LW(0), LW(2))
            ADD_FLOAT(LW(1), LW(3))
            IF_INT_GREATER(LW(3), LW(2))
                ADD_FLOAT(LW(3), LW(4))
                ELSE
                SET_FLOAT_VALUE(LW(3), LW(2))
            END_IF
            CASE_DEFAULT
            ADD_FLOAT(LW(0), LW(2))
            ADD_FLOAT(LW(2), LW(4))
        END_SWITCH
        IF_INT_EQUAL(LF(0), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_rotate, 1, STRING("majinai_BOX"), LW(1), LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(0)
        ADD_FLOAT(LW(0), FLOAT(360.0f))
        IF_INT_GREATER(LW(0), -360)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        ADD_FLOAT(LW(1), FLOAT(360.0f))
        IF_INT_GREATER(LW(1), -360)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_FLOAT_VALUE(LW(5), FLOAT(-360.0f))
    SET_FLOAT_VALUE(LW(6), FLOAT(-360.0f))
    SUBTRACT_FLOAT(LW(5), LW(0))
    SUBTRACT_FLOAT(LW(6), LW(1))
    DIVIDE_FLOAT(LW(6), LW(5))
    SET_FLOAT_VALUE(LW(3), LW(2))
    MULTIPLY_FLOAT(LW(3), LW(6))
    LOOP(0)
        ADD_FLOAT(LW(0), LW(2))
        ADD_FLOAT(LW(1), LW(3))
        SET_FLOAT_VALUE(LW(3), LW(2))
        MULTIPLY_FLOAT(LW(3), LW(6))
        SUBTRACT_FLOAT(LW(2), LW(4))
        CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_rotate, 1, STRING("majinai_BOX"), LW(1), LW(0), 0)
        IF_INT_LESS_OR_EQUAL(LW(0), -360)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("me"), STRING("H_2"))
    SET_VALUE(LW(3), LW(13))
    SET_VALUE(LW(4), LW(14))
    SET_VALUE(LW(5), LW(15))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE5"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, LW(4), 0, 1000)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), LW(3), LW(0), LW(5))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_mario_dispflag_onoff, 0, 1)
    DELETE_EVENT(GW(0))
    CALL(evt_eff_softdelete, STRING("aaax"))
    CALL(evt_snd_sfxoff, GW(1))
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group4"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("senn"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
        CALL(evt_mario_get_exparty, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_map_blend_set_flag, 1, STRING("extparty"), 2048)
        END_IF
        CALL(evt_map_blend_set_flag, 1, STRING("まじない師"), 2048)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 500)
        LABEL(4)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(4)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 500, 1, 60)
        CALL(evt_mario_set_pose, STRING("M_I_1"))
        ADD_FLOAT(LW(1), FLOAT(18.0f))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE7"), LW(0), LW(1), LW(2), 0)
        CALL(effect_stardust, 0, LW(0), LW(1), LW(2), FLOAT(50.0f), FLOAT(50.0f), 20, 60)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_INCANT_CUBE3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("anm_ten"), 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("senn"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group4"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("BOX_tenkai"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("BOX_tenkai"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_map_checkanim, STRING("anm_ten"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_msg_print, 0, STRING("majinaisi_08"), 0, STRING("me"))
    CALL(evt_mario_set_motion, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("majinai_BOX"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_blend_off, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("majinai_BOX"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_map_blend_set_flag, 0, STRING("mario"), 2048)
    CALL(evt_map_blend_set_flag, 0, STRING("party"), 2048)
    CALL(evt_mario_get_exparty, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_map_blend_set_flag, 0, STRING("extparty"), 2048)
    END_IF
    CALL(evt_map_blend_set_flag, 0, STRING("まじない師"), 2048)
    CALL(evt_mapobj_trans, 1, STRING("majinai_BOX"), 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 iri_14_item_tbl[] = {
    ITEM_CAKE_MIX, -1,
};

EVT_BEGIN(majinaisi_direct_talk)
    IF_INT_EQUAL(GSW(1420), 14)
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_14_00"), 0, 0)
            ELSE
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_14_01"), 0, 0)
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(1000)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print, 0, STRING("irai_14_02"), 0, 0)
                SET_VALUE(GSWF(5406), 1)
                ELSE
                CALL(evt_pouch_check_item, 172, LW(0))
                IF_INT_GREATER(LW(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_14_04"), 0, 0)
                    CALL(evt_win_item_select, 0, PTR(iri_14_item_tbl), LW(0), 0)
                    IF_INT_NOT_EQUAL(LW(0), -1)
                        CALL(evt_pouch_remove_item, 172, LW(0))
                        CALL(evt_msg_print, 0, STRING("irai_14_05"), 0, 0)
                        CALL(evt_sub_get_coin, 30)
                        SET_VALUE(GSW(1420), 0)
                        SET_VALUE(GSWF(5388), 1)
                        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    END_IF
                    ELSE
                    CALL(evt_msg_print, 0, STRING("irai_14_03"), 0, 0)
                END_IF
            END_IF
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("majinaisi_00"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(majinaisi_init)
    CALL(evt_run_case_evt, 9, 1, STRING("a_majinai"), 0, PTR(majinaisi_table_talk), 0)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"まじない師"*/"\x82\xDC\x82\xB6\x82\xC8\x82\xA2\x8E\x74", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        majinaisi_init, // initEvent
        NULL, // regularEvent
        majinaisi_direct_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(tik_12_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e_yuka_s"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group2"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ie_kakoi"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("mad"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("majinai_BOX"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("anm_kem"), 1, 0)
    RETURN
EVT_END()

static USER_FUNC(effect_stardust) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 vx = evtGetFloat(event, args[4]);
    f32 vy = evtGetFloat(event, args[5]);
    s32 count = evtGetValue(event, args[6]);
    s32 camera = evtGetValue(event, args[7]);

    effStardustEntry(type, count, camera, x, y, z, vx, vy);
    return EVT_RETURN_DONE;
}
static USER_FUNC(setmertn) {
    event->ownerNPC = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}
USER_FUNC(majinaisi_ryokin_kaisu) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], majinaisi_ryokin_kaisu_tbl[idx * 2]);
    evtSetValue(event, args[2], majinaisi_ryokin_kaisu_tbl[idx * 2 + 1]);
    return EVT_RETURN_DONE;
}
