#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "countdown.h"
#include "mri_puni.h"

#include "driver/fadedrv.h"
#include "mario/mariost.h"
#include "driver/swdrv.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_memcard.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "mri.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"

void* puniGetPunigp(void);
void puniswGetindex(s32 flag, s32 index, s32* a, s32* b);
void puniFadeOff(s32 index);
static u8* bp;

USER_FUNC(akari_charge_entry);
USER_FUNC(bossrobot);
USER_FUNC(elder_talk_sub_fadeoff);
USER_FUNC(elder_talk_sub_punicheck);
USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_zoom_in);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_eff_softdelete);
USER_FUNC(evt_env_blur_on);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_fade_in);
USER_FUNC(evt_fade_out);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_free_capture);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_delete_check);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_off);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_get_item_end_wait);
USER_FUNC(evt_item_get_position);
USER_FUNC(evt_item_move_3d_position);
USER_FUNC(evt_item_set_position);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_blend);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_clear_party);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_mov_spd);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_jump_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_party_use_check);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_mode);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_sleep_off);
USER_FUNC(evt_mario_sleep_on);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_msg_continue);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_change_interrupt);
USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_get_rotate);
USER_FUNC(evt_npc_glide_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_release_fieldnpc);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_rotate);
USER_FUNC(evt_npc_set_rotate_offset);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_set_ry_lr);
USER_FUNC(evt_npc_set_scale);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_flg_onoff);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_sleep_off);
USER_FUNC(evt_party_sleep_on);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_puni_all_movestop);
USER_FUNC(evt_puni_flag_onoff);
USER_FUNC(evt_puni_get_flag);
USER_FUNC(evt_puni_get_lsw_index);
USER_FUNC(evt_puni_get_position);
USER_FUNC(evt_puni_get_trap_index);
USER_FUNC(evt_puni_init_puni);
USER_FUNC(evt_puni_jump_position);
USER_FUNC(evt_puni_korokoro);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_move_position2);
USER_FUNC(evt_puni_pera_onoff);
USER_FUNC(evt_puni_puninum_onoff);
USER_FUNC(evt_puni_set_anim);
USER_FUNC(evt_puni_set_dir);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_lsw_index);
USER_FUNC(evt_puni_set_mode);
USER_FUNC(evt_puni_set_position);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_puni_wait_systemmode);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_bgmon_f);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_coin);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_win_item_select);
USER_FUNC(kpa_chg_pose);
static USER_FUNC(evtflash);
void bossrobot_batu(CameraId camera, void* param);
USER_FUNC(evt_countdown_stop);
#ifdef VERSION_G8ME01
#endif
void mri_stg3_012_cb(s32 type, void* param);

static EvtWord guide_init[29];
static EvtWord guide_talk[20];
static EvtWord rival_init[30];
static EvtWord rival_talk[8];
static EvtWord elder_init[39];
s32 puni_return_data[204];
EvtWord puni_return[99];
EvtWord elder_talk_sub[376];
s32 irai_item_tbl[2];
static s32 iri_17_item_tbl[2];
static EvtWord elder_talk[177];
EvtWord puni_1_init[24];
EvtWord puni_1_talk[43];
EvtWord puni_2_init[24];
EvtWord puni_2_talk[20];
EvtWord puni_init[9];
static EvtWord baba_init[9];
static EvtWord baba_talk[8];
static EvtWord sister_init[31];
EvtWord sister_jump[48];
static EvtWord sister_talk[230];
static NpcSetupInfo npcEnt[9];
EvtWord bero_00_doa_close[10];
EvtWord bero_00_doa_open[10];
static EvtWord bero_01_doa_close[10];
static EvtWord bero_01_doa_open[10];
EvtWord out_mri_event[5];
static BeroEntry bero_entry_data[4];
EvtWord bigtree_firstbattle[734];
EvtWord meet_rival[2247];
EvtWord rival_nakama[1137];
EvtWord elder_kaisetsu[206];
EvtWord puni_taiki[119];
s32 puni_position[102];
EvtWord boss_battle_init[215];
EvtWord dai3kanbu_suikomare[81];
EvtWord boss_robot_pera[34];
EvtWord boss_robot_pera_after[19];
EvtWord boss_battle[1224];
EvtWord starstone_item[19];
EvtWord boss_robot_electro[43];
EvtWord boss_robot_kemuri[49];
EvtWord boss_battle_win_init[49];
EvtWord boss_battle_win[1493];
static EvtWord door_switch_init[17];
EvtWord ueki_puni_nigeru[100];
EvtWord evt_ueki_01[18];
static EvtWord evt_ueki_02[24];
EvtWord evt_ueki_03[49];
static EvtWord evt_ueki_08[8];
static KusaAccess ueki_access_data[5];
EvtWord puni_mario_wait[49];
EvtWord mri_01_koopa_first_evt[400];
EvtWord mri_01_koopa_second_evt[682];
EvtWord mri_01_init_evt[464];

static USER_FUNC(evtflash) {
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0:
            fadeEntry(FADE_OUT_BLACK, 20, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 50;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3:
            fadeEntry(FADE_IN_BLACK, 80, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(elder_talk_sub_punicheck) {
    EvtWord* args = event->args;
    s32* punigp = puniGetPunigp();
    s32 i;
    s32 traps = 0;
    s32 others = 0;
    s32 result;
    s32 c[5];
    s32 trap;
    s32 puni;

    for (i = 1; i < 103; i++) {
        puniGetPunidat();
        if (i == 1 && swGet(0xB14)) {
            continue;
        }
        puniswGetindex(LSW(0), i - 1, &puni, c);
        puniswTrapGetindex(i, &trap);
        if (trap != 0) {
            traps++;
        } else if (puni != 0 && puni != punigp[0x328 / 4]) {
            others++;
        }
    }
    result = 1;
    if (others != 0) {
        result = 0;
    } else if (traps != 0) {
        result = 2;
    }
    evtSetValue(event, args[0], result);
    return EVT_RETURN_DONE;
}

USER_FUNC(elder_talk_sub_fadeoff) {
    puniFadeOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(guide_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(169, 171)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -20, 0, 40)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(guide_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_002"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_002_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(rival_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(90, 93)
        CALL(evt_npc_set_position, STRING("me"), -5, 0, 70)
        CASE_EQUAL(94)
        CALL(evt_npc_set_position, STRING("me"), -20, 0, 38)
        CASE_BETWEEN(169, 171)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(rival_talk)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(elder_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(94, 108)
        CALL(evt_npc_set_position, STRING("me"), -78, 0, 38)
        CASE_BETWEEN(169, 171)
        CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -78, 0, 38)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

s32 puni_return_data[] = {
    200, 0, 200, 10, 200, 20, 200, 30,
    200, 40, 200, 50, 200, 60, 200, 70,
    200, 80, 200, 90, 210, 90, 210, 80,
    210, 70, 210, 60, 210, 50, 210, 40,
    210, 30, 210, 20, 210, 10, 210, 0,
    220, 0, 220, 10, 220, 20, 220, 30,
    220, 40, 220, 50, 220, 60, 220, 70,
    220, 80, 220, 90, 230, 90, 230, 80,
    230, 70, 230, 60, 230, 50, 230, 40,
    230, 30, 230, 20, 230, 10, 230, 0,
    240, 0, 240, 10, 240, 20, 240, 30,
    240, 40, 240, 50, 240, 60, 240, 70,
    240, 80, 240, 90, 250, 90, 250, 80,
    250, 70, 250, 60, 250, 50, 250, 40,
    250, 30, 250, 20, 250, 10, 250, 0,
    260, 0, 260, 10, 260, 20, 260, 30,
    260, 40, 260, 50, 260, 60, 260, 70,
    260, 80, 260, 90, 270, 90, 270, 80,
    270, 70, 270, 60, 270, 50, 270, 40,
    270, 30, 270, 20, 270, 10, 270, 0,
    280, 0, 280, 10, 280, 20, 280, 30,
    280, 40, 280, 50, 280, 60, 280, 70,
    280, 80, 280, 90, 290, 90, 290, 80,
    290, 70, 290, 60, 290, 50, 290, 40,
    290, 30, 290, 20, 290, 10, 290, 0,
    300, 0, 300, 10,
};

EVT_BEGIN(puni_return)
    SET_VALUE(LW(10), 1)
    SET_INT_READ_ADDR(PTR(puni_return_data))
    LOOP(0)
        SET_VALUE(LW(11), LW(10))
        ADD(LW(11), -1)
        CALL(evt_puni_get_lsw_index, LSW(0), LW(11), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_puni_get_flag, LW(10), LW(4))
        CALL(evt_puni_get_trap_index, LW(10), LW(5))
        IF_INT_EQUAL(LW(10), 1)
            IF_INT_EQUAL(GSWF(2836), 1)
                DEBUG_PUT_MSG(STRING("牢屋の前に待機しています"))
                GOTO(100)
            END_IF
        END_IF
        IF_INT_NOT_EQUAL(LW(5), 0)
            DEBUG_PUT_MSG(STRING("トラップにかかっています"))
            GOTO(100)
        END_IF
        IF_INT_EQUAL(LW(0), 0)
            DEBUG_PUT_MSG(STRING("ここにいます＆仲間になっていません"))
            GOTO(100)
        END_IF
        IF_BITS_SET(LW(4), 1024)
            DEBUG_PUT_MSG(STRING("同じエリアにいます"))
            GOTO(100)
        END_IF
        DEBUG_PUT_MSG(STRING("はぐれから合流しました"))
        READ_INT2(LW(12), LW(13))
        CALL(evt_puni_set_lsw_index, LSW(0), LW(11), 0, 0, 0, 0)
        CALL(evt_puni_init_puni, LW(10), 1, LW(12), 0, LW(13), 0)
        LABEL(100)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(elder_talk_sub)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_22"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg2_mri_e15_22_sel"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_add, 0, STRING("stg2_mri_e15_28"))
        CASE_EQUAL(1)
        CALL(elder_talk_sub_punicheck, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("stg2_mri_e15_23"))
            CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 500, 11)
            WAIT_MS(1000)
            CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("S_1"), STRING("T_2"))
            WAIT_FRAMES(1)
            SET_VALUE(GF(0), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_CALL_PUNI1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_cam3d_evt_set, -75, 85, 351, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_set, -75, 85, 351, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_set, -75, 85, 351, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 100, 11)
                WAIT_MS(100)
                SET_VALUE(GF(0), 0)
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(evt_env_blur_on, 1, 600)
            END_INLINE_EVENT
            CALL(evt_msg_print, 0, STRING("stg2_mri_e15_24"), 0, STRING("me"))
            WAIT_WHILE(GF(0))
            WAIT_MS(500)
            CALL(evt_cam3d_evt_off, 500, 11)
            CALL(evt_mario_set_dir, 270, 200, 0)
            WAIT_MS(500)
            CALL_CHILD(PTR(puni_return))
            WAIT_MS(2000)
            CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("S_1"), STRING("T_1"))
            WAIT_FRAMES(1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_e15_25"), 0, STRING("me"))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("stg2_mri_e15_26"))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING("stg2_mri_e15_27"))
        END_SWITCH
        CASE_EQUAL(2)
        CALL(evt_msg_continue)
        CALL(evt_puni_all_movestop, 1)
        CALL(evt_party_stop, 0)
        CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 500, 11)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e15_29"), 0, STRING("me"))
        CALL(elder_talk_sub_fadeoff, 0)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e15_30"), 0, STRING("me"))
        SET_VALUE(GF(3), 0)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SLEEP1"), LW(0), LW(1), LW(2), 0)
                WAIT_FRAMES(140)
                IF_INT_EQUAL(GF(3), 1)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_mario_sleep_on)
        CALL(evt_party_sleep_on, 0)
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(elder_talk_sub_fadeoff, 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e15_31"), 0, STRING("me"))
        SET_VALUE(GF(3), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_sleep_off)
        CALL(evt_party_sleep_off, 0)
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(1000)
        CALL(evt_party_run, 0)
        CALL(evt_cam3d_evt_off, 500, 11)
        CALL(evt_puni_all_movestop, 0)
    END_SWITCH
    RETURN
EVT_END()

s32 irai_item_tbl[] = {
    151, -1,
};

static s32 iri_17_item_tbl[] = {
    ITEM_ROUTING_SLIP, -1,
};

static EVT_BEGIN(elder_talk)
    IF_INT_EQUAL(GSW(1420), 10)
        CALL(evt_pouch_check_item, 151, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_msg_print, 0, STRING("irai_10_01"), 0, STRING("me"))
            CALL(evt_win_item_select, 0, PTR(irai_item_tbl), LW(0), 0)
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(evt_msg_print, 0, STRING("irai_10_02"), 0, STRING("me"))
                CALL(evt_sub_get_coin, 60)
                CALL(evt_pouch_remove_item, 151, LW(0))
                SET_VALUE(GSW(1420), 0)
                SET_VALUE(GSWF(5384), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_10_00"), 0, STRING("me"))
        END_IF
        RETURN
    END_IF
    IF_INT_EQUAL(GSW(1420), 17)
        IF_INT_EQUAL(GSWF(5405), 0)
            ELSE
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_17_00"), 0, 0)
                CALL(evt_win_item_select, 0, PTR(iri_17_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_pouch_remove_item, 98, LW(0))
                    CALL(evt_msg_print, 0, STRING("irai_17_01"), 0, 0)
                    CALL(evt_item_entry, STRING("kairanban"), 98, 0, -1000, 0, 17, -1, 0)
                    CALL(evt_item_get_item, STRING("kairanban"))
                    CALL(evt_item_get_item_end_wait, STRING("kairanban"))
                    SET_VALUE(GSWF(5406), 1)
                END_IF
                RETURN
                ELSE
                IF_INT_EQUAL(GSWF(5407), 0)
                    CALL(evt_msg_print, 0, STRING("irai_17_02"), 0, 0)
                    RETURN
                    ELSE
                    CALL(evt_msg_print, 0, STRING("irai_17_03"), 0, 0)
                    RETURN
                END_IF
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(95, 118)
        IF_INT_EQUAL(GSWF(2829), 0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_e15_28"), 0, STRING("me"))
            ELSE
            CALL_CHILD(PTR(elder_talk_sub))
        END_IF
        CASE_BETWEEN(118, 402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_001"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_001_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_1_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(169, 171)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), -280, 0, 10)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(354, 354)
        IF_INT_EQUAL(GSWF(3879), 0)
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_000"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_000_1"), 0, STRING("me"))
            SET_VALUE(GSWF(3889), 1)
        END_IF
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_000"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_000_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_2_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(169, 171)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 340, 0, 100)
            CALL(evt_npc_set_ry, STRING("me"), 270)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_017"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_017_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(puni_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

static EVT_BEGIN(baba_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

static EVT_BEGIN(baba_talk)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_006"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(sister_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(169, 171)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
        CASE_GREATER_OR_EQUAL(118)
        IF_INT_EQUAL(GSWF(2884), 1)
            CALL(evt_npc_set_position, STRING("me"), 100, 0, -10)
        END_IF
    END_SWITCH
    SET_VALUE(GF(10), 0)
    SET_VALUE(GF(11), 0)
    RETURN
EVT_END()

EVT_BEGIN(sister_jump)
    SET_VALUE(GF(10), 1)
    SET_VALUE(GF(11), 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    LOOP(0)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
        END_INLINE_EVENT
        CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 18)
        IF_INT_EQUAL(GF(11), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

static EVT_BEGIN(sister_talk)
    SET_VALUE(GF(11), 1)
    WAIT_WHILE(GF(10))
    IF_INT_EQUAL(GSWF(2827), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_015"), 0, STRING("me"))
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_003"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_016_1"), 0, STRING("me"))
    END_SWITCH
    CALL(evt_msg_select, 0, STRING("stg2_mri_re_004"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg2_mri_re_005"))
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg2_mri_re_006"), 0, 0)
        CALL(evt_msg_select, 0, STRING("stg2_mri_re_007"))
        SET_VALUE(LW(1), LW(0))
        CALL(evt_msg_select, 0, STRING("stg2_mri_re_008"))
        SET_VALUE(LW(2), LW(0))
        CALL(evt_msg_select, 0, STRING("stg2_mri_re_009"))
        SET_VALUE(LW(3), LW(0))
        CALL(evt_msg_select, 0, STRING("stg2_mri_re_010"))
        SET_VALUE(LW(4), LW(0))
        CALL(evt_msg_continue)
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            IF_INT_EQUAL(LW(2), 1)
                IF_INT_EQUAL(LW(3), 1)
                    IF_INT_EQUAL(LW(4), 0)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_011"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            BEGIN_INLINE_EVENT
                CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
            END_INLINE_EVENT
            CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 18)
            BEGIN_INLINE_EVENT
                CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
            END_INLINE_EVENT
            CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 18)
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_012"), 0, STRING("me"))
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_014"), 0, STRING("me"))
            CALL(evt_item_entry, STRING("puniko_item"), 168, 0, -1000, 0, 16, -1, 0)
            CALL(evt_item_get_item, STRING("puniko_item"))
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_015"), 0, STRING("me"))
            SET_VALUE(GSWF(2827), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg2_mri_re_013"), 0, STRING("me"))
        END_IF
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg2_mri_re_016"))
        SPAWN_EVENT(PTR(sister_jump))
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガイド"*/"\x83\x4B\x83\x43\x83\x68", // name
        0x80600, // flags
        0, // reactionFlags
        guide_init, // initEvent
        NULL, // regularEvent
        guide_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ライバル"*/"\x83\x89\x83\x43\x83\x6F\x83\x8B", // name
        0x80600, // flags
        0, // reactionFlags
        rival_init, // initEvent
        NULL, // regularEvent
        rival_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56", // name
        0x80600, // flags
        0, // reactionFlags
        elder_init, // initEvent
        NULL, // regularEvent
        elder_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族１"*/"\x83\x76\x83\x6A\x91\xB0\x82\x50", // name
        0x80600, // flags
        0, // reactionFlags
        puni_1_init, // initEvent
        NULL, // regularEvent
        puni_1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族２"*/"\x83\x76\x83\x6A\x91\xB0\x82\x51", // name
        0x80600, // flags
        0, // reactionFlags
        puni_2_init, // initEvent
        NULL, // regularEvent
        puni_2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族"*/"\x83\x76\x83\x6A\x91\xB0", // name
        0x80600, // flags
        0, // reactionFlags
        puni_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"プニ族B"*/"\x83\x76\x83\x6A\x91\xB0\x42", // name
        0x80600, // flags
        0, // reactionFlags
        puni_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガイド妹"*/"\x83\x4B\x83\x43\x83\x68\x96\x85", // name
        0x80600, // flags
        0, // reactionFlags
        sister_init, // initEvent
        NULL, // regularEvent
        sister_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(bero_00_doa_close)
    SET_VALUE(LW(9), 1)
    SET_VALUE(LW(10), STRING("S_doa_l_a"))
    SPAWN_EVENT(PTR(evt_mri_door_close))
    RETURN
EVT_END()

EVT_BEGIN(bero_00_doa_open)
    SET_VALUE(LW(9), 1)
    SET_VALUE(LW(10), STRING("S_doa_l_a"))
    SPAWN_EVENT(PTR(evt_mri_door_open))
    RETURN
EVT_END()

static EVT_BEGIN(bero_01_doa_close)
    SET_VALUE(LW(9), 1)
    SET_VALUE(LW(10), STRING("S_doa_r_a"))
    SPAWN_EVENT(PTR(evt_mri_door_close))
    RETURN
EVT_END()

static EVT_BEGIN(bero_01_doa_open)
    SET_VALUE(LW(9), 1)
    SET_VALUE(LW(10), STRING("S_doa_r_a"))
    SPAWN_EVENT(PTR(evt_mri_door_open))
    RETURN
EVT_END()

EVT_BEGIN(out_mri_event)
    SET_VALUE(LW(0), 0)
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
        (s32*)out_mri_event, // exitEvent
        "mri_00", // nextMapName
        "ne_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        bero_00_doa_close, // enterAnimeArg
        bero_00_doa_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_05", // nextMapName
        "w_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        bero_01_doa_close, // enterAnimeArg
        bero_01_doa_open, // exitAnimeArg
    },
    {
        "dokan", // hitName
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
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(bigtree_firstbattle)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -120)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_dispflag_onoff, 1, 4)
        CALL(evt_mario_set_dir, 90, 200, 0)
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_dispflag_onoff, 0, 4)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -28, 152, 548, -28, 57, 7, 1000, 11)
    WAIT_MS(1000)
    SET_VALUE(LW(10), STRING("ガイド"))
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_mario_clear_party, 1)
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_npc_flag_onoff, 1, LW(10), 1073743392)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), -20, 73, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    WAIT_MS(500)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(400)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(450)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_00"), 0, LW(10))
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), 0, 73, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_01"), 0, LW(10))
    CALL(evt_cam3d_evt_set, 350, 115, 400, 350, 45, 7, 500, 11)
    CALL(evt_snd_bgmoff, 512)
    WAIT_MS(580)
    CALL(evt_mapobj_get_position, STRING("S_lights_r"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_UNLOCK"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_LAMP"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("doalight01"), 0, 0)
    WAIT_MS(580)
    SET_VALUE(LW(9), 1)
    SET_VALUE(LW(10), STRING("S_doa_r_a"))
    SPAWN_EVENT(PTR(evt_mri_door_open_pure))
    WAIT_MS(1580)
    CALL(evt_npc_set_position, STRING("ガイド"), -20, 0, 50)
    CALL(evt_mario_set_pos, -60, 0, 30)
    CALL(evt_party_set_pos, 0, -100, 0, 20)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        SET_VALUE(LW(9), 1)
        SET_VALUE(LW(10), STRING("S_doa_r_a"))
        SPAWN_EVENT(PTR(evt_mri_door_close_pure))
    END_INLINE_EVENT
    SET_VALUE(LW(10), STRING("第三勢力部下Ａ"))
    SET_VALUE(LW(11), STRING("第三勢力部下Ｂ"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, LW(10), 1073743392)
        CALL(evt_npc_set_position, LW(10), 445, 0, 30)
        CALL(evt_npc_set_ry, LW(10), 270)
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), 307, 20, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, LW(11), 1073743392)
    CALL(evt_npc_set_position, LW(11), 445, 0, 30)
    CALL(evt_npc_set_ry, LW(11), 270)
    CALL(evt_npc_move_position, LW(11), 337, 50, 0, FLOAT(120.0f), 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER2"))
    CALL(evt_snd_bgm_scope, 0, -1)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(10), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(11), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, LW(10), 200, 20, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, LW(11), 200, 50, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, LW(11), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -20, 130, 450, -20, 50, 7, 0, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_set_position, LW(10), 170, 0, 20)
        CALL(evt_npc_move_position, LW(10), 40, 20, 0, FLOAT(240.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_set_position, LW(11), 200, 0, 50)
        CALL(evt_npc_move_position, LW(11), 70, 50, 0, FLOAT(240.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("ガイド"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイド"), -60, 50, 0, FLOAT(180.0f), 0)
        DELETE_EVENT(LW(9))
    END_INLINE_EVENT
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ガイド"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_02"), 0, LW(10))
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(11), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 52, 85, 303, 50, 35, 7, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_03"), 0, LW(11))
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_04"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_05"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_06"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_07"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_08"), 0, LW(10))
    CALL(evt_npc_set_ry, LW(10), 270)
    CALL(evt_cam3d_evt_set, -20, 135, 500, -20, 50, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -40, 135, 500, -40, 50, 7, 700, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, LW(10), -28, 20, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, LW(11), -9, 50, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(250)
    CALL(evt_npc_release_fieldnpc, 1)
    CALL(evt_npc_set_battle_info, LW(10), 4)
    CALL(evt_npc_battle_start, LW(10))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 89)
    SET_VALUE(LW(10), STRING("第三勢力部下Ａ"))
    SET_VALUE(LW(11), STRING("第三勢力部下Ｂ"))
    CALL(evt_npc_set_position, STRING("ガイド"), -80, 0, 50)
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 1073743392)
    CALL(evt_mario_set_pos, -60, 0, 30)
    CALL(evt_party_set_pos, 0, -100, 0, 20)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_npc_set_position, LW(10), -18, 0, 20)
    CALL(evt_npc_set_ry, LW(10), 270)
    CALL(evt_npc_flag_onoff, 1, LW(10), 1073743392)
    CALL(evt_npc_set_position, LW(11), 1, 0, 50)
    CALL(evt_npc_set_ry, LW(11), 270)
    CALL(evt_npc_flag_onoff, 1, LW(11), 1073743392)
    CALL(evt_map_playanim, STRING("doalight01"), 0, 0)
    CALL(evt_cam3d_evt_set, -20, 125, 450, -20, 45, 7, 0, 11)
    SPAWN_EVENT(PTR(meet_rival))
    RETURN
EVT_END()

EVT_BEGIN(meet_rival)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(15), 5)
    LOOP(9)
        CALL(evt_puni_init_puni, LW(15), 30, 0, -1000, 0, 0)
        CALL(evt_puni_flag_onoff, LW(15), 0, 8388608)
        ADD(LW(15), 1)
    END_LOOP
    SET_VALUE(LW(10), STRING("第三勢力部下Ａ"))
    SET_VALUE(LW(11), STRING("第三勢力部下Ｂ"))
    CALL(evt_npc_set_position, STRING("ガイド"), -80, 0, 50)
    CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 1073743392)
    CALL(evt_mario_set_pos, -60, 0, 30)
    CALL(evt_party_set_pos, 0, -100, 0, 20)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_npc_set_position, LW(10), -18, 0, 20)
    CALL(evt_npc_set_ry, LW(10), 270)
    CALL(evt_npc_flag_onoff, 1, LW(10), 1073743392)
    CALL(evt_npc_set_position, LW(11), 1, 0, 50)
    CALL(evt_npc_set_ry, LW(11), 270)
    CALL(evt_npc_flag_onoff, 1, LW(11), 1073743392)
    CALL(evt_map_playanim, STRING("doalight01"), 0, 0)
    CALL(evt_cam3d_evt_set, -20, 125, 450, -20, 45, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_npc_move_position, LW(10), 2, 20, 0, FLOAT(180.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_09"), 0, LW(10))
    CALL(evt_npc_move_position, LW(11), 21, 50, 0, FLOAT(180.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e11_10"), 0, LW(11))
    WAIT_MS(500)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mapobj_trans, 1, STRING("S_doa_r_a"), 5, 0, -60)
        CALL(evt_cam3d_evt_set, 350, 115, 400, 350, 45, 7, 0, 11)
        WAIT_MS(1000)
        SET_VALUE(LW(9), 1)
        SET_VALUE(LW(10), STRING("S_doa_r_a"))
        CALL_CHILD(PTR(evt_mri_door_close_pure))
        WAIT_MS(600)
        CALL(evt_mapobj_get_position, STRING("S_lights_r"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_UNLOCK"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_LAMP"), LW(0), LW(1), LW(2), 0)
        CALL(evt_map_playanim, STRING("doalight02"), 0, 0)
        WAIT_MS(1000)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, LW(10), 90)
        CALL(evt_npc_move_position, LW(10), 307, 20, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_move_position, LW(10), 445, 30, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, LW(11), 90)
        CALL(evt_npc_move_position, LW(11), 337, 50, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_move_position, LW(11), 445, 30, 0, FLOAT(240.0f), 0)
        CALL(evt_npc_set_position, LW(11), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    SET_VALUE(LW(10), STRING("ガイド"))
    SET_VALUE(LW(11), STRING("ライバル"))
    CALL(evt_cam3d_evt_set, -20, 141, 500, -20, 55, 7, 0, 11)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), -60, 50, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_00"), 0, LW(10))
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon_f, 257, STRING("BGM_EVT_STG2_PUNI1"), 2000)
    CALL(evt_puni_pera_onoff, 5, 1)
    CALL(evt_puni_set_dir, 5, 270)
    CALL(evt_puni_set_position, 5, 40, 0, -66)
    CALL(evt_puni_move_position2, 5, -10, -66, FLOAT(150.0f))
    CALL(evt_puni_set_dir, 5, 90)
    WAIT_MS(200)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(10), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(800)
    CALL(evt_puni_move_position2, 5, 14, -66, FLOAT(150.0f))
    CALL(evt_puni_pera_onoff, 5, 0)
    WAIT_MS(1500)
    CALL(evt_cam3d_evt_set, 150, 90, 275, 40, 40, 0, 1500, 11)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), 24, -38, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_01"), 0, LW(10))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), 0, 25, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_set_ry, LW(10), 270)
    END_INLINE_EVENT
    CALL(evt_puni_set_dir, 5, 270)
    CALL(evt_puni_set_anim, 5, 1)
    CALL(evt_puni_move_position2, 5, -15, 20, FLOAT(120.0f))
    CALL(evt_puni_set_anim, 5, 0)
    CALL(evt_cam3d_evt_set, -20, 150, 550, -20, 55, 9, 0, 11)
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), -10, 25, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        WAIT_MS(1000)
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), -14, 56, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
    END_INLINE_EVENT
    WAIT_MS(100)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_set_anim, 5, 5)
        CALL(evt_puni_korokoro, 5, 25, 0)
    END_INLINE_EVENT
    CALL(evt_puni_move_position2, 5, -40, 20, FLOAT(120.0f))
    CALL(evt_puni_set_anim, 5, 0)
    WAIT_MS(1000)
    CALL(evt_puni_get_position, 5, LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_02"), 0, 0)
    CALL(evt_puni_set_position, 6, 60, 0, -66)
    CALL(evt_puni_pera_onoff, 6, 1)
    CALL(evt_puni_set_dir, 6, 270)
    CALL(evt_puni_move_position2, 6, 30, -66, FLOAT(180.0f))
    CALL(evt_puni_pera_onoff, 6, 0)
    WAIT_MS(500)
    CALL(evt_puni_get_position, 6, LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_03"), 0, 0)
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(10), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_04"), 0, LW(10))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_puni_pera_onoff, 7, 1)
        CALL(evt_puni_set_dir, 7, 90)
        CALL(evt_puni_set_position, 7, 55, 30, -67)
        CALL(evt_puni_get_position, 7, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 7, 90, -67, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 7, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(450)
        CALL(evt_puni_pera_onoff, 8, 1)
        CALL(evt_puni_set_dir, 8, 270)
        CALL(evt_puni_set_position, 8, 46, 38, -63)
        CALL(evt_puni_get_position, 8, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 8, 36, -63, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 8, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_puni_pera_onoff, 9, 1)
        CALL(evt_puni_set_dir, 9, 90)
        CALL(evt_puni_set_position, 9, -26, 36, -107)
        CALL(evt_puni_get_position, 9, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 9, 4, -107, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 9, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_puni_pera_onoff, 10, 1)
        CALL(evt_puni_set_dir, 10, 270)
        CALL(evt_puni_set_position, 10, -19, 10, -83)
        CALL(evt_puni_get_position, 10, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 10, -49, -83, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 10, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_pera_onoff, 11, 1)
        CALL(evt_puni_set_dir, 11, 90)
        CALL(evt_puni_set_position, 11, -111, 47, -78)
        CALL(evt_puni_get_position, 11, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 11, -81, -78, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 11, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_puni_pera_onoff, 12, 1)
        CALL(evt_puni_set_dir, 12, 270)
        CALL(evt_puni_set_position, 12, -115, 60, -81)
        CALL(evt_puni_get_position, 12, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 12, -125, -81, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 12, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_puni_pera_onoff, 13, 1)
        CALL(evt_puni_set_dir, 13, 270)
        CALL(evt_puni_set_position, 13, -117, 26, -66)
        CALL(evt_puni_get_position, 13, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_HIDE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_puni_move_position2, 13, -147, -66, FLOAT(120.0f))
        CALL(evt_puni_pera_onoff, 13, 0)
    END_INLINE_EVENT
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 7, LW(0), LW(1), LW(2))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_05"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 12, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_10"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 11, LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_09"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 9, LW(0), LW(1), LW(2))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_07"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 10, LW(0), LW(1), LW(2))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_08"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 13, LW(0), LW(1), LW(2))
        ADD(LW(0), 10)
        ADD(LW(1), 10)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_11"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_get_position, 8, LW(0), LW(1), LW(2))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e12_06"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_12"), 0, LW(10))
    BEGIN_INLINE_EVENT
        WAIT_MS(350)
        CALL(evt_puni_move_position2, 6, 60, -66, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_puni_move_position2, 7, 55, -67, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(450)
        CALL(evt_puni_move_position2, 8, 55, -63, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_puni_move_position2, 9, -26, -107, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_puni_move_position2, 10, -19, -83, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_move_position2, 11, -111, -78, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_puni_move_position2, 12, -115, -81, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_puni_move_position2, 13, -117, -66, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_npc_set_ry, LW(10), 270)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_pera_onoff, 6, 1)
        CALL(evt_puni_set_dir, 6, 90)
        CALL(evt_puni_set_position, 6, -57, 0, -68)
        CALL(evt_puni_set_anim, 6, 2)
        CALL(evt_puni_move_position2, 6, -40, 93, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 6, 0)
        CALL(evt_puni_pera_onoff, 6, 0)
        CALL(evt_puni_set_dir, 6, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_pera_onoff, 7, 1)
        CALL(evt_puni_set_dir, 7, 90)
        CALL(evt_puni_set_position, 7, -57, 0, -68)
        CALL(evt_puni_set_anim, 7, 2)
        CALL(evt_puni_move_position2, 7, 29, 10, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 7, 0)
        CALL(evt_puni_pera_onoff, 7, 0)
        CALL(evt_puni_set_dir, 7, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_puni_pera_onoff, 8, 1)
        CALL(evt_puni_set_dir, 8, 90)
        CALL(evt_puni_set_position, 8, 12, 0, -65)
        CALL(evt_puni_set_anim, 8, 2)
        CALL(evt_puni_move_position2, 8, 50, 18, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 8, 0)
        CALL(evt_puni_pera_onoff, 8, 0)
        CALL(evt_puni_set_dir, 8, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(evt_puni_pera_onoff, 9, 1)
        CALL(evt_puni_set_dir, 9, 90)
        CALL(evt_puni_set_position, 9, 30, 0, -65)
        CALL(evt_puni_set_anim, 9, 2)
        CALL(evt_puni_move_position2, 9, 52, 66, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 9, 0)
        CALL(evt_puni_pera_onoff, 9, 0)
        CALL(evt_puni_set_dir, 9, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_pera_onoff, 10, 1)
        CALL(evt_puni_set_dir, 10, 90)
        CALL(evt_puni_set_position, 10, -57, 0, -68)
        CALL(evt_puni_set_anim, 10, 2)
        CALL(evt_puni_move_position2, 10, -55, 55, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 10, 0)
        CALL(evt_puni_pera_onoff, 10, 0)
        CALL(evt_puni_set_dir, 10, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(750)
        CALL(evt_puni_pera_onoff, 11, 1)
        CALL(evt_puni_set_dir, 11, 90)
        CALL(evt_puni_set_position, 11, 12, 0, -65)
        CALL(evt_puni_set_anim, 11, 2)
        CALL(evt_puni_move_position2, 11, 38, 46, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 11, 0)
        CALL(evt_puni_pera_onoff, 11, 0)
        CALL(evt_puni_set_dir, 11, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_pera_onoff, 12, 1)
        CALL(evt_puni_set_dir, 12, 270)
        CALL(evt_puni_set_position, 12, 30, 0, -65)
        CALL(evt_puni_set_anim, 12, 2)
        CALL(evt_puni_move_position2, 12, 24, 92, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 12, 0)
        CALL(evt_puni_pera_onoff, 12, 0)
        CALL(evt_puni_set_dir, 12, 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_pera_onoff, 13, 1)
        CALL(evt_puni_set_dir, 13, 90)
        CALL(evt_puni_set_position, 13, 30, 0, -65)
        CALL(evt_puni_set_anim, 13, 2)
        CALL(evt_puni_move_position2, 13, 71, 39, FLOAT(120.0f))
        CALL(evt_puni_set_anim, 13, 0)
        CALL(evt_puni_pera_onoff, 13, 0)
        CALL(evt_puni_set_dir, 13, 270)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_13"), 0, LW(10))
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_13_01"), 0, 0)
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_npc_set_position, LW(11), 47, 0, -82)
    CALL(evt_cam3d_evt_set, 20, 90, 303, 20, 40, 7, 500, 11)
    CALL(evt_npc_set_ry, LW(11), 270)
    CALL(evt_npc_flag_onoff, 1, LW(11), 1073743392)
    SET_VALUE(LW(0), LW(11))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(11), 20, -21, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_14"), 0, LW(11))
    CALL(evt_cam3d_evt_set, 3, 71, 226, 3, 49, 104, 1000, 11)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), 0, 3, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    WAIT_WHILE(GF(0))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_15"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_16"), 0, LW(11))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_flag_onoff, 0, LW(10), 1024)
        SET_VALUE(LW(0), LW(10))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(10), -20, -10, 0, FLOAT(180.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_flag_onoff, 1, LW(10), 1024)
    END_INLINE_EVENT
    SET_VALUE(LW(0), LW(11))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(11), -15, 50, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_cam3d_evt_set, -28, 152, 548, -28, 57, 7, 0, 11)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_17"), 0, LW(10))
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), 15, 50, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_ry, LW(11), 90)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_17_02"), 0, LW(10))
    CALL(evt_cam3d_evt_set, 0, 80, 309, 0, 28, 9, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_18"), 0, LW(11))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 18)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, -20, 80, 309, -20, 28, 9, 300, 11)
        SET_VALUE(LW(0), LW(11))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, LW(11), -35, 50, 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
    END_INLINE_EVENT
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), -10, 50, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    WAIT_MS(200)
    SET_VALUE(LW(0), LW(10))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, LW(10), -5, 50, 0, FLOAT(180.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_19"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_20"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_21"), 0, LW(10))
    CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(11), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(11), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(11), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(11), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_22"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_23"), 0, LW(10))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_24"), 0, LW(11))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_24_00"), 0, LW(10))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -35, 95, 352, -35, 35, 7, 500, 11)
        WAIT_MS(200)
        CALL(evt_npc_set_ry, LW(11), 270)
        WAIT_MS(800)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_24_01"), 0, LW(11))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_25"), 0, LW(10))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    SET_VALUE(GSWF(2876), 1)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    CALL(evt_puni_init_puni, 1, 1, LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_flag_onoff, 0, LW(11), 1073741856)
    CALL(evt_puni_set_mode, 4, 14)
    CALL(evt_puni_set_mode, 5, 14)
    CALL(evt_puni_set_mode, 6, 14)
    CALL(evt_puni_set_mode, 7, 14)
    CALL(evt_puni_set_mode, 8, 14)
    CALL(evt_puni_set_mode, 9, 14)
    CALL(evt_puni_set_mode, 10, 14)
    CALL(evt_puni_set_mode, 11, 14)
    CALL(evt_puni_set_mode, 12, 14)
    CALL(evt_puni_set_mode, 13, 14)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 90)
    CALL(evt_mario_key_onoff, 1)
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 5, -57, -68, FLOAT(120.0f))
        CALL(evt_puni_set_position, 5, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 6, -57, -68, FLOAT(120.0f))
        CALL(evt_puni_set_position, 6, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 7, 57, -68, FLOAT(120.0f))
        CALL(evt_puni_set_position, 7, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 8, 12, -65, FLOAT(120.0f))
        CALL(evt_puni_set_position, 8, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 9, 30, -65, FLOAT(120.0f))
        CALL(evt_puni_set_position, 9, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 10, -57, -68, FLOAT(120.0f))
        CALL(evt_puni_set_position, 10, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 11, 12, -65, FLOAT(120.0f))
        CALL(evt_puni_set_position, 11, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 12, 30, -65, FLOAT(120.0f))
        CALL(evt_puni_set_position, 12, 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_move_position2, 13, 30, -65, FLOAT(120.0f))
        CALL(evt_puni_set_position, 13, 0, -1000, 0)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(rival_nakama)
    CALL(evt_puni_init_puni, 5, 14, -89, 0, 0, 0)
    CALL(evt_puni_flag_onoff, 5, 0, 8388608)
    CALL(evt_puni_set_dir, 5, 90)
    CALL(evt_puni_init_puni, 6, 14, -63, 0, -10, 0)
    CALL(evt_puni_flag_onoff, 6, 0, 8388608)
    CALL(evt_puni_set_dir, 6, 90)
    CALL(evt_puni_init_puni, 7, 14, -116, 0, 54, 0)
    CALL(evt_puni_flag_onoff, 7, 0, 8388608)
    CALL(evt_puni_set_dir, 7, 90)
    CALL(evt_puni_init_puni, 8, 14, -75, 0, 86, 0)
    CALL(evt_puni_flag_onoff, 8, 0, 8388608)
    CALL(evt_puni_set_dir, 8, 90)
    CALL(evt_puni_init_puni, 9, 14, -105, 0, 84, 0)
    CALL(evt_puni_flag_onoff, 9, 0, 8388608)
    CALL(evt_puni_set_dir, 9, 90)
    CALL(evt_puni_init_puni, 10, 14, -120, 0, 10, 0)
    CALL(evt_puni_flag_onoff, 10, 0, 8388608)
    CALL(evt_puni_set_dir, 10, 90)
    CALL(evt_puni_init_puni, 11, 14, -134, 0, 42, 0)
    CALL(evt_puni_flag_onoff, 11, 0, 8388608)
    CALL(evt_puni_set_dir, 11, 90)
    CALL(evt_puni_init_puni, 12, 14, -151, 0, 72, 0)
    CALL(evt_puni_flag_onoff, 12, 0, 8388608)
    CALL(evt_puni_set_dir, 12, 90)
    CALL(evt_puni_init_puni, 13, 14, -136, 0, -19, 0)
    CALL(evt_puni_flag_onoff, 13, 0, 8388608)
    CALL(evt_puni_set_dir, 13, 90)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), 60)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL_CHILD(PTR(mri_guide_goryu))
    CALL(evt_puni_get_position, 1, LW(0), LW(1), LW(2))
    CALL(evt_puni_set_position, 1, 0, -1000, 0)
    CALL(evt_puni_set_mode, 1, 30)
    CALL(evt_npc_set_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 1073743392)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ライバル"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -5, 95, 352, -5, 35, 7, 500, 11)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), 10, 38, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_00"), 0, STRING("ライバル"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_01"), 0, STRING("ガイド"))
    CALL(evt_npc_get_position, STRING("ライバル"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ライバル"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ライバル"), LW(0), LW(1), LW(2), 200, 18)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ライバル"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ライバル"), LW(0), LW(1), LW(2), 200, 18)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_02"), 0, STRING("ライバル"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -45, 95, 352, -45, 35, 7, 200, 11)
        WAIT_MS(500)
        CALL(evt_cam3d_evt_set, FLOAT(-45.0f), FLOAT(93.19922f), FLOAT(342.0f), FLOAT(-45.0f), FLOAT(35.0f), FLOAT(7.0f), 1000, 9)
        WAIT_MS(900)
        CALL(evt_cam3d_evt_set, -45, 95, 352, -45, 35, 7, 200, 11)
        WAIT_MS(200)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNI1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_BIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 2, FLOAT(1.0f), FLOAT(3.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_03"), 0, STRING("プニ族長老"))
    WAIT_WHILE(GF(0))
    CALL(evt_sub_intpl_msec_init, 5, FLOAT(3.0f), FLOAT(1.0f), 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_03_01"), 0, STRING("プニ族長老"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_04"), 0, STRING("ライバル"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_05"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_set, 40, 95, 352, 40, 35, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_06"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -45, 95, 352, -45, 35, 7, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_07"), 0, STRING("プニ族長老"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_08"), 0, STRING("ライバル"))
    SET_VALUE(LW(10), 5)
    LOOP(9)
        BEGIN_INLINE_EVENT
            CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
            CALL(evt_puni_jump_position, LW(10), LW(0), LW(1), LW(2), 200, 18)
        END_INLINE_EVENT
        ADD(LW(10), 1)
    END_LOOP
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, -39, 152, 548, -39, 57, 7, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_09"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_set, 60, 115, 400, 60, 45, 7, 500, 11)
    SET_VALUE(LW(10), 5)
    LOOP(9)
        CALL(evt_puni_set_mode, LW(10), 1)
        CALL(evt_puni_flag_onoff, LW(10), 1, 8388608)
        ADD(LW(10), 1)
    END_LOOP
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("FF_GET_KEYITEM1"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_GET_PARTY1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_11"), 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_puni_all_movestop, 1)
        SET_VALUE(LW(10), 5)
        LOOP(9)
            CALL(evt_puni_set_dir, LW(10), 270)
            ADD(LW(10), 1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_npc_set_ry, STRING("ライバル"), 90)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_12"), 0, STRING("ガイド"))
    CALL(evt_cam3d_evt_set, -50, 115, 400, -50, 45, 7, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        ADD(LW(10), -20)
        ADD(LW(12), -20)
        SET_VALUE(LW(0), STRING("ガイド"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ガイド"), LW(10), LW(12), 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_set_ry, STRING("ガイド"), 90)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ライバル"), LW(10), LW(11), LW(12))
        ADD(LW(10), -20)
        ADD(LW(12), -20)
        SET_VALUE(LW(0), STRING("ライバル"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("ライバル"), LW(10), LW(12), 0, FLOAT(120.0f), 0)
        DELETE_EVENT(LW(9))
    END_INLINE_EVENT
    CALL(evt_party_flg_onoff, 0, 16777216, 1)
    CALL(evt_party_move_pos2, 0, -20, 37, FLOAT(120.0f))
    CALL(evt_party_flg_onoff, 0, 16777216, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 25, 37, FLOAT(120.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print_party, STRING("stg2_mri_e15_13"))
    CALL(evt_cam3d_evt_set, -75, 69, 253, -75, 25, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_14"), 0, STRING("プニ族長老"))
    CALL(evt_npc_flag_onoff, 1, STRING("ライバル"), 1073741856)
    CALL(evt_cam3d_evt_set, -25, 115, 400, -25, 45, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_15"), 0, STRING("プニ族長老"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(1000)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("プニ族長老"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1500)
    CALL(evt_cam3d_evt_set, -75, 69, 253, -75, 25, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_16"), 0, STRING("プニ族長老"))
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_PUNIDAMA1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 5)
    ADD(LW(2), -2)
    CALL(evt_item_entry, STRING("item00"), 64, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    ADD(LW(1), 8)
    CALL(evt_item_move_3d_position, STRING("item00"), LW(0), LW(1), LW(2), 500, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -40, 115, 400, -40, 45, 7, 0, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -30, 10, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(evt_mario_mov_pos2, -48, 37, FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_16_01"), 0, STRING("プニ族長老"))
    WAIT_MS(100)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    WAIT_MS(700)
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 10, 25, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 270, -1)
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(0), STRING("ガイド"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("ガイド"), -19, 56, 0, FLOAT(120.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_18"), 0, STRING("ガイド"))
    SET_VALUE(LW(10), 5)
    LOOP(9)
        BEGIN_INLINE_EVENT
            CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
            CALL(evt_puni_jump_position, LW(10), LW(0), LW(1), LW(2), 200, 18)
        END_INLINE_EVENT
        ADD(LW(10), 1)
    END_LOOP
    CALL(evt_puni_all_movestop, 0)
    WAIT_MS(700)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI3"))
    SET_VALUE(GSWF(2879), 1)
    SET_VALUE(GSW(651), 9)
    CALL(evt_npc_get_position, STRING("ライバル"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ライバル"), 0, -1000, 0)
    CALL(evt_puni_init_puni, 4, 1, LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    CALL(evt_puni_init_puni, 1, 1, LW(0), LW(1), LW(2), 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 95)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(elder_kaisetsu))
    RETURN
EVT_END()

EVT_BEGIN(elder_kaisetsu)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 25)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("プニ族長老"), 90)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e15_19"), 0, STRING("プニ族長老"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_mov_spd, LW(10))
    SET_FLOAT_VALUE(LW(11), LW(10))
    MULTIPLY_FLOAT(LW(11), FLOAT(0.2998047f))
    ADD_FLOAT(LW(0), LW(11))
    IF_INT_LESS_OR_EQUAL(LW(2), -15)
        SET_VALUE(LW(2), -10)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(2), 110)
        SET_VALUE(LW(2), 105)
    END_IF
    CALL(evt_mario_mov_pos2, LW(0), LW(2), LW(10))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_D_8"))
    WAIT_MS(1500)
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    WAIT_WHILE(GF(0))
    CALL(evt_cam3d_evt_set, -25, 152, 548, -25, 57, 7, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 50, -15, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_20"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_set, -75, 77, 303, -75, 25, 7, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e15_21"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(2829), 1)
    RETURN
EVT_END()

void bossrobot_batu(CameraId camera, void* param) {
    u8* alpha = param;
    NpcEntry* robot = npcNameToPtr("\203{\203X\203\215\203{\203b\203g");
    NpcEntry* batu = npcNameToPtr("\203{\203X\203\215\203{\203b\203g\203o\203c");
    Mtx* mtxs = *animPoseGetAnimPosePtr(robot->poseId)->worldMatrices;
    s32 idx = animPoseGetGroupIdx(robot->poseId, "locator23");
    Mtx rot;
    Mtx mtx;

    mtxGetRotationElement(mtxs[idx], rot, 'x', 'z');
    PSMTXTrans(mtx, mtxs[idx][0][3], mtxs[idx][1][3], mtxs[idx][2][3]);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXTrans(rot, 0.0f, 0.0f, 0.2f);
    PSMTXConcat(mtx, rot, mtx);
    animPosePeraOff(batu->poseId);
    animPoseSetMaterialFlagOn(batu->poseId, 0x40);
    animPoseSetMaterialEvtColor(batu->poseId, (GXColor){0xFF, 0xFF, 0xFF, *alpha});
    switch (dispGetCurWork()->renderMode) {
        case 0:
            animPoseDrawMtx(batu->poseId, mtx, 1, 0.0f, 2.0f);
            break;
        case 1:
            animPoseDrawMtx(batu->poseId, mtx, 2, 0.0f, 2.0f);
            break;
        case 2:
            animPoseDrawMtx(batu->poseId, mtx, 3, 0.0f, 2.0f);
            break;
    }
}

USER_FUNC(bossrobot) {
    u8 alpha = evtGetValue(event, event->args[0]);
    NpcEntry* robot = npcNameToPtr("\203{\203X\203\215\203{\203b\203g");
    NpcEntry* batu = npcNameToPtr("\203{\203X\203\215\203{\203b\203g\203o\203c");
    NpcEntry* maru = npcNameToPtr("\203{\203X\203\215\203{\203b\203g\203}\203\213");
    AnimationPose* pose;
    Mtx* mtxs;
    s32 idx;
    s32 i;
    Vec pos;

    if (isFirstCall) {
        bp = _mapAlloc(1);
        animPoseWorldMatrixEvalOn(robot->poseId);
        pose = animPoseGetAnimPosePtr(robot->poseId);
        mtxs = *pose->worldMatrices;
        for (i = 0; i < pose->numWorldMatrices; i++) {
            PSMTXTrans(mtxs[i], 0.0f, -1000.0f, 0.0f);
        }
    }
    mtxs = *animPoseGetAnimPosePtr(robot->poseId)->worldMatrices;
    idx = animPoseGetGroupIdx(robot->poseId, "locator17");
    maru->position = (Vec){mtxs[idx][0][3], mtxs[idx][1][3], mtxs[idx][2][3]};
    pos = (Vec){0.0f, 0.0f, 0.0f};
    *bp = alpha;
    dispEntry(CAMERA_3D, dispCalcZ(&pos), 0, bossrobot_batu, bp);
    dispEntry(CAMERA_3D, dispCalcZ(&pos), 1, bossrobot_batu, bp);
    dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, bossrobot_batu, bp);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_countdown_stop) {
    countDownStop();
    return EVT_RETURN_DONE;
}

USER_FUNC(akari_charge_entry) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    s32 a = evtGetValue(event, args[3]);
    s32 b = evtGetValue(event, args[4]);
    s32 c = evtGetValue(event, args[5]);
    f32 scale = evtGetFloat(event, args[6]);
    EffectEntry* eff = effAkariChargeN64Entry(0, evtGetValue(event, args[7]), x, y, z, scale);

    ((EffAkariChargeN64Work*)eff->userdata)->elem[0].r = a;
    ((EffAkariChargeN64Work*)eff->userdata)->elem[0].g = b;
    ((EffAkariChargeN64Work*)eff->userdata)->elem[0].b = c;
    return EVT_RETURN_DONE;
}

void mri_stg3_012_cb(s32 type, void* param) {
    u16* work = *(u16**)param;

    switch (type) {
        case 3:
            if (work[4] == 1) {
                evtSetValue(NULL, LSWF(2), TRUE);
            }
            break;
    }
}

EVT_BEGIN(puni_taiki)
    CALL(evt_puni_init_puni, 5, 14, -55, 0, 20, 0)
    CALL(evt_puni_flag_onoff, 5, 0, 9437184)
    CALL(evt_puni_init_puni, 6, 14, -40, 0, 93, 0)
    CALL(evt_puni_flag_onoff, 6, 0, 9437184)
    CALL(evt_puni_init_puni, 7, 14, 19, 0, 52, 0)
    CALL(evt_puni_flag_onoff, 7, 0, 9437184)
    CALL(evt_puni_init_puni, 8, 14, 50, 0, 18, 0)
    CALL(evt_puni_flag_onoff, 8, 0, 9437184)
    CALL(evt_puni_init_puni, 9, 14, 52, 0, 66, 0)
    CALL(evt_puni_flag_onoff, 9, 0, 9437184)
    CALL(evt_puni_init_puni, 10, 14, -35, 0, 50, 0)
    CALL(evt_puni_flag_onoff, 10, 0, 9437184)
    CALL(evt_puni_init_puni, 11, 14, 38, 0, 46, 0)
    CALL(evt_puni_flag_onoff, 11, 0, 9437184)
    CALL(evt_puni_init_puni, 12, 14, 24, 0, 92, 0)
    CALL(evt_puni_flag_onoff, 12, 0, 9437184)
    CALL(evt_puni_init_puni, 13, 14, 71, 0, 39, 0)
    CALL(evt_puni_flag_onoff, 13, 0, 9437184)
    RETURN
EVT_END()

s32 puni_position[] = {
    19267575, 23724122, 25100278, 19857429, 20774962, 24182781, 21037044, 23789617,
    25100278, 17956936, 18415731, 18153523, 19136593, 24051726, 22151142, 20840431,
    19136500, 20709482, 19922987, 22413292, 22020198, 21561381, 22282199, 22741005,
    21757919, 19922909, 22085652, 18808832, 20250713, 23068712, 19464196, 25886725,
    21495889, 22151242, 25493548, 17891316, 21102680, 26542152, 24510518, 23658525,
    23461956, 22872086, 16580641, 18087936, 18284634, 24444960, 21102560, 20971646,
    20512836, 21889021, 19726295, 20381734, 19922956, 18350119, 22478850, 21364752,
    20316139, 20250631, 23593070, 17760277, 24772618, 23199744, 21954561, 19922919,
    19333162, 22347896, 20447262, 25296922, 18481222, 16252980, 17367094, 21430264,
    16515142, 24903759, 17432610, 20905942, 22806613, 20512761, 18874402, 19333221,
    22478830, 19202105, 18677779, 26411035, 21364796, 20905947, 23789547, 26607666,
    20119627, 24313947, 22282335, 17498216, 24248390, 22085672, 25428031, 19726457,
    19267607, 16973911, 23199730, 22675515, 17432589, 20774915,
};

EVT_BEGIN(boss_battle_init)
    CALL(evt_npc_entry, STRING("第三勢力幹部"), STRING("c_kanbu_ibt"))
    CALL(evt_npc_set_tribe, STRING("第三勢力幹部"), STRING("第三勢力幹部"))
    CALL(evt_npc_set_position, STRING("第三勢力幹部"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力幹部"), 524288)
    CALL(evt_npc_entry, STRING("ボスロボット"), STRING("c_mb_robo_1a"))
    CALL(evt_npc_set_tribe, STRING("ボスロボット"), STRING("ボスロボット"))
    CALL(evt_npc_set_position, STRING("ボスロボット"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ボスロボット"), 1074266144)
    CALL(evt_npc_set_ry, STRING("ボスロボット"), 270)
    CALL(evt_npc_entry, STRING("ボスロボットバツ"), STRING("c_mb_robo_c"))
    CALL(evt_npc_set_tribe, STRING("ボスロボットバツ"), STRING("ボスロボットバツ"))
    CALL(evt_npc_set_position, STRING("ボスロボットバツ"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ボスロボットバツ"), 786432)
    CALL(evt_npc_set_ry, STRING("ボスロボットバツ"), 270)
    CALL(evt_npc_entry, STRING("ボスロボットマル"), STRING("c_mb_robo_1a"))
    CALL(evt_npc_set_tribe, STRING("ボスロボットマル"), STRING("ボスロボット"))
    CALL(evt_npc_set_position, STRING("ボスロボットマル"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ボスロボットマル"), 786432)
    CALL(evt_npc_set_anim, STRING("ボスロボットマル"), STRING("L_1"))
    CALL(evt_npc_set_ry, STRING("ボスロボットマル"), 270)
    CALL(evt_npc_entry, STRING("部下１"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("部下１"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("部下１"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("部下１"), 1073742880)
    CALL(evt_npc_entry, STRING("部下２"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("部下２"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("部下２"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("部下２"), 1073742880)
    CALL(evt_npc_entry, STRING("部下３"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("部下３"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("部下３"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("部下３"), 1073742880)
    CALL(evt_npc_entry, STRING("部下４"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("部下４"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("部下４"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("部下４"), 1073742880)
    CALL(evt_npc_entry, STRING("部下５"), STRING("c_zako_n"))
    CALL(evt_npc_set_tribe, STRING("部下５"), STRING("第三勢力部下"))
    CALL(evt_npc_set_position, STRING("部下５"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("部下５"), 1073742880)
    CALL(evt_npc_entry, STRING("ダミー"), STRING("c_zako_n"))
    CALL(evt_npc_set_position, STRING("ダミー"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), 1073741984)
    SET_VALUE(GW(10), 0)
    BEGIN_INLINE_EVENT_GET_ID(GW(11))
        SET_TYPE(0)
        CALL(bossrobot, GW(10))
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(dai3kanbu_suikomare)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 100, 0, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_npc_set_scale, STRING("第三勢力幹部"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(evt_npc_get_rotate, STRING("第三勢力幹部"), LW(0), LW(1), LW(2))
        LOOP(0)
            ADD_FLOAT(LW(1), FLOAT(40.0f))
            CALL(evt_npc_set_rotate, STRING("第三勢力幹部"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_glide_position, STRING("第三勢力幹部"), 150, 60, 30, 300, 0, 0, 0, 0)
    DELETE_EVENT(LW(0))
    CALL(evt_npc_set_position, STRING("第三勢力幹部"), 0, -1000, 0)
    CALL(evt_npc_set_scale, STRING("第三勢力幹部"), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    RETURN
EVT_END()

EVT_BEGIN(boss_robot_pera)
    CALL(evt_paper_entry, STRING("OFF_d_xten"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_xten"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_onoff, STRING("img"), 1)
    RETURN
EVT_END()

EVT_BEGIN(boss_robot_pera_after)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_xten"))
    RETURN
EVT_END()

EVT_BEGIN(boss_battle)
    CALL(evt_npc_set_position, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), -320, 0, 30)
    CALL(evt_npc_set_ry, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), 1073741856)
    CALL(evt_npc_set_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), -250, 0, 30)
    CALL(evt_npc_set_ry, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 1073743392)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_puni_puninum_onoff, 0)
    CALL(evt_cam3d_evt_set, -285, 152, 548, -285, 57, 7, 500, 11)
    WAIT_MS(1000)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_position, LW(10), 0, -100, 0)
        CALL(evt_puni_set_anim, LW(10), 0)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_00"), 0, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"))
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_01"), 0, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_cam3d_evt_set, FLOAT(-285.0f), FLOAT(150.19922f), FLOAT(538.2998f), FLOAT(-285.0f), FLOAT(57.0f), FLOAT(7.0f), 1000, 9)
        WAIT_MS(900)
        CALL(evt_cam3d_evt_set, -285, 152, 548, -285, 57, 7, 500, 11)
        WAIT_MS(200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_BIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 2, FLOAT(1.0f), FLOAT(3.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
        ADD(LW(0), 10)
        CALL(evt_npc_move_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(2), 200, 0, 0)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    SET_VALUE(GF(2), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_SMALL1"), LW(1), LW(2), LW(3), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.7998047f))
        CALL(evt_npc_set_scale, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(0), LW(0))
        WAIT_WHILE(GF(0))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_SMALL1"), LW(1), LW(2), LW(3), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.6699219f))
        CALL(evt_npc_set_scale, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(0), LW(0))
        WAIT_WHILE(GF(1))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_SMALL1"), LW(1), LW(2), LW(3), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.5292969f))
        CALL(evt_npc_set_scale, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(0), LW(0))
        WAIT_WHILE(GF(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_SMALL1"), LW(1), LW(2), LW(3), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.39941406f))
        CALL(evt_npc_set_scale, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(0), LW(0))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_02"), 0, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 5, FLOAT(3.0f), FLOAT(1.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    CALL(evt_msg_continue)
    SET_VALUE(GF(1), 0)
    CALL(evt_msg_continue)
    SET_VALUE(GF(2), 0)
    CALL(evt_msg_continue)
    SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
    CALL(evt_npc_set_scale, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(0), LW(0))
    CALL(evt_cam3d_evt_set, -240, 86, 302, -240, 35, 7, 0, 11)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_npc_move_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(2), 200, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_03"), 0, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"))
    CALL(evt_cam3d_evt_set, -275, 86, 302, -275, 35, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_04"), 0, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"))
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 50)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_WAIST1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), STRING("D_1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_04_00"), 0, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), STRING("D_1"), STRING("D_1"))
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_04_01"), 0, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), STRING("S_2"))
    CALL(evt_npc_set_autotalkpose, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), STRING("S_1"), STRING("T_1"))
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_04_02"), 0, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("stg2_mri_e24_05"))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_npc_set_ry, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 90)
    CALL(evt_mario_set_pos, -100, 0, 30)
    CALL(evt_party_set_pos, 0, -70, 0, 30)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -180, 152, 548, -180, 57, 7, 1000, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -120, 30, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -150, 30, FLOAT(120.0f))
        CALL(evt_mario_dispflag_onoff, 1, 2)
        CALL(evt_mario_set_pose, STRING("M_I_Y"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print_party, STRING("stg2_mri_e24_05_01"))
    CALL(evt_cam3d_evt_set, -230, 88, 253, -230, 45, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_06"), 0, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"))
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_6"))
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_TIMER_SET1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_7"))
    CALL(evt_sub_countdown_flag_onoff, 1, 8)
    CALL(evt_countdown_stop)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_sub_countdown_flag_onoff, 1, 4)
            WAIT_MS(200)
            CALL(evt_sub_countdown_flag_onoff, 0, 4)
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_sub_countdown_end)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_AWAY1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_8"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -180, 152, 548, -180, 57, 7, 0, 11)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_07"), 0, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"))
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_6"))
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_TIMER_SET1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_7"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 150, 186, 549, 150, 90, 7, 500, 11)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_AWAY1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), STRING("I_8"))
    WAIT_MS(1000)
    DELETE_EVENT(LW(10))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pos, -120, 0, 30)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_set_pos, 0, -150, 0, 30)
    SET_VALUE(LW(10), STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"))
    CALL(evt_npc_set_anim, LW(10), STRING("Z_1"))
    CALL(evt_npc_set_position, LW(10), 150, 240, 30)
    CALL(evt_npc_flag_onoff, 1, LW(10), 512)
    CALL(evt_npc_flag_onoff, 0, LW(10), 524288)
    CALL(evt_npc_set_width, LW(10), 100)
    CALL(evt_npc_set_autotalkpose, LW(10), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOSS_APPEAR1"), LW(0), LW(1), LW(2), LW(9))
    WAIT_MS(1000)
    CALL(evt_npc_sound_data_set, LW(10), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_npc_glide_position, LW(10), 150, 100, 30, 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_npc_set_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), -100, 0, 30)
    CALL(evt_npc_set_ry, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 90)
    CALL(evt_npc_move_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), 150, 30, 0, FLOAT(180.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING(/*"第三勢力幹部"*/"\x91\xE6\x8E\x4F\x90\xA8\x97\xCD\x8A\xB2\x95\x94"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_NPC_KANBU_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL_CHILD(dai3kanbu_suikomare)
    WAIT_MS(200)
    CALL(evt_npc_set_anim, LW(10), STRING("R_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, LW(10), STRING("S_1"))
    CALL(evt_npc_sound_data_reset, LW(10))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MAGNUM_APP1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_APPEAR2"), LW(9))
    CALL_CHILD(boss_robot_pera)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, LW(10), STRING("S_2"))
    CALL(evt_npc_set_autotalkpose, LW(10), STRING("S_2"), STRING("T_2"))
    CALL_CHILD(boss_robot_pera_after)
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 150, 185, 400, 150, 115, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e24_08"), 0, STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"))
    CALL(evt_cam3d_evt_set, 145, 186, 549, 145, 90, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_LIGHT1"), 0)
    CALL(evtflash)
    BEGIN_INLINE_EVENT
        WAIT_MS(750)
        CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_SET1"), 0)
        WAIT_MS(933)
        CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_SET1"), 0)
        WAIT_MS(366)
        CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_SET1"), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_SET2"), 0)
        WAIT_MS(2416)
        CALL(evt_snd_sfxon, STRING("SFX_STG2_BOSS_LANDING1"), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, LW(10), STRING("C_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(3830)
        CALL(evt_sub_intpl_msec_init, 0, 0, 255, 666)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(GW(10), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4400)
        CALL(evt_cam3d_evt_set, 145, 167, 549, 150, 71, 7, 500, 11)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4600)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.029296875f), 500)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_npc_glide_position, LW(10), 150, 150, 30, 1500, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(evt_npc_set_position, LW(10), 150, 0, 30)
    CALL(evt_npc_set_anim, LW(10), STRING("S_3"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -20, 167, 549, -20, 71, 7, 1500, 11)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"), 1024)
    BEGIN_INLINE_EVENT_GET_ID(LW(12))
        CALL(evt_npc_move_position, LW(10), -20, 30, 1500, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1300)
    CALL(evt_snd_bgmoff, 512)
    SET_VALUE(GSW_STORY_PROGRESS, 111)
    CALL(evt_npc_release_fieldnpc, 1)
    CALL(evt_npc_set_battle_info, STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"), 20)
    CALL(evt_npc_battle_start, STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI3"))
    DELETE_EVENT(LW(12))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 20, 0, 10)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -10, 0, 5)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_ry, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), 90)
    CALL(evt_npc_set_position, STRING(/*"プニ族長老"*/"\x83\x76\x83\x6A\x91\xB0\x92\xB7\x98\x56"), -100, 0, 30)
    CALL(evt_npc_set_position, STRING(/*"ボスロボット"*/"\x83\x7B\x83\x58\x83\x8D\x83\x7B\x83\x62\x83\x67"), 90, 0, 30)
    CALL(evt_cam3d_evt_set, 70, 156, 549, 70, 60, 7, 0, 11)
    SET_VALUE(LW(0), 0)
    SPAWN_EVENT(boss_battle_win)
    RETURN
EVT_END()

EVT_BEGIN(starstone_item)
    LOOP(0)
        CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, LW(9), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(boss_robot_electro)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD_EXPLSION1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(500)
        END_LOOP
    END_SIBLING_EVENT
    ADD(LW(1), 90)
    ADD(LW(2), 40)
    LOOP(0)
        CALL(akari_charge_entry, LW(0), LW(1), LW(2), 50, 50, 0, FLOAT(1.5f), 30)
        WAIT_MS(400)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(boss_robot_kemuri)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD_EXPLSION3"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(1000)
        END_LOOP
    END_SIBLING_EVENT
    ADD(LW(1), 75)
    ADD(LW(2), 70)
    LOOP(0)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.75f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(700)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(boss_battle_win_init)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 20, 0, 10)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -10, 0, 5)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 90)
    CALL(evt_npc_set_position, STRING("プニ族長老"), -100, 0, 30)
    CALL(evt_npc_set_position, STRING("ボスロボット"), 90, 0, 30)
    RETURN
EVT_END()

EVT_BEGIN(boss_battle_win)
    CALL(evt_cam3d_evt_set, 70, 156, 549, 70, 60, 7, 0, 11)
    CALL(evt_snd_bgmoff, 512)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_party_stop, 0)
        CALL(evt_snd_envoff, 512)
        CALL(evt_puni_wait_systemmode, 2)
        CALL(evt_puni_puninum_onoff, 0)
        SET_VALUE(GW(10), 255)
    END_IF
    SET_VALUE(LW(10), 5)
    SET_VALUE(LW(11), -15)
    SET_VALUE(LW(14), 0)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        SET_VALUE(LW(13), -10)
        ADD(LW(13), LW(14))
        IF_INT_EQUAL(LW(14), 0)
            SET_VALUE(LW(14), 1)
            ELSE
            SET_VALUE(LW(14), 0)
        END_IF
        CALL(evt_puni_set_position, LW(10), LW(11), 0, LW(13))
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_set_dir, LW(10), 90)
        ADD(LW(10), 1)
        ADD(LW(11), -15)
        IF_INT_EQUAL(LW(10), 28)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(11), -30)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        SET_VALUE(LW(13), -20)
        ADD(LW(13), LW(14))
        IF_INT_EQUAL(LW(14), 0)
            SET_VALUE(LW(14), 1)
            ELSE
            SET_VALUE(LW(14), 0)
        END_IF
        CALL(evt_puni_set_position, LW(10), LW(11), 0, LW(13))
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_set_dir, LW(10), 90)
        ADD(LW(10), 1)
        ADD(LW(11), -15)
        IF_INT_EQUAL(LW(10), 51)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(11), -40)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        SET_VALUE(LW(13), -30)
        ADD(LW(13), LW(14))
        IF_INT_EQUAL(LW(14), 0)
            SET_VALUE(LW(14), 1)
            ELSE
            SET_VALUE(LW(14), 0)
        END_IF
        CALL(evt_puni_set_position, LW(10), LW(11), 0, LW(13))
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_set_dir, LW(10), 90)
        ADD(LW(10), 1)
        ADD(LW(11), -15)
        IF_INT_EQUAL(LW(10), 71)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(11), -55)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        SET_VALUE(LW(13), -40)
        ADD(LW(13), LW(14))
        IF_INT_EQUAL(LW(14), 0)
            SET_VALUE(LW(14), 1)
            ELSE
            SET_VALUE(LW(14), 0)
        END_IF
        CALL(evt_puni_set_position, LW(10), LW(11), 0, LW(13))
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_set_dir, LW(10), 90)
        ADD(LW(10), 1)
        ADD(LW(11), -15)
        IF_INT_EQUAL(LW(10), 91)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(11), -150)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        SET_VALUE(LW(13), -50)
        ADD(LW(13), LW(14))
        IF_INT_EQUAL(LW(14), 0)
            SET_VALUE(LW(14), 1)
            ELSE
            SET_VALUE(LW(14), 0)
        END_IF
        CALL(evt_puni_set_position, LW(10), LW(11), 0, LW(13))
        CALL(evt_puni_set_anim, LW(10), 0)
        CALL(evt_puni_set_dir, LW(10), 90)
        ADD(LW(10), 1)
        ADD(LW(11), -15)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    IF_INT_EQUAL(GSWF(2878), 1)
        CALL(evt_puni_set_mode, 3, 30)
        CALL(evt_puni_set_position, 3, -120, 0, 10)
        CALL(evt_puni_set_anim, 3, 0)
        CALL(evt_puni_set_dir, 3, 90)
    END_IF
    CALL(evt_puni_flag_onoff, 1, 0, 262144)
    CALL(evt_puni_set_mode, 1, 30)
    CALL(evt_puni_set_anim, 1, 0)
    CALL(evt_npc_set_position, STRING("ガイドsp"), -90, 0, 10)
    CALL(evt_npc_set_ry, STRING("ガイドsp"), 90)
    CALL(evt_npc_set_anim, STRING("ガイドsp"), STRING("S_1"))
    CALL(evt_puni_flag_onoff, 2, 0, 262144)
    CALL(evt_puni_set_mode, 2, 30)
    CALL(evt_puni_set_anim, 2, 0)
    CALL(evt_npc_set_position, STRING("ガイド妹sp"), -80, 0, 50)
    CALL(evt_npc_set_ry, STRING("ガイド妹sp"), 90)
    CALL(evt_npc_set_anim, STRING("ガイド妹sp"), STRING("S_1"))
    CALL(evt_puni_flag_onoff, 4, 0, 262144)
    CALL(evt_puni_set_mode, 4, 30)
    CALL(evt_puni_set_anim, 4, 0)
    CALL(evt_npc_set_position, STRING("ライバルsp"), -20, 0, 50)
    CALL(evt_npc_set_ry, STRING("ライバルsp"), 90)
    CALL(evt_npc_set_anim, STRING("ライバルsp"), STRING("S_1"))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SET_VALUE(LW(10), STRING("ボスロボット"))
    SPAWN_EVENT_GET_ID(PTR(boss_robot_electro), LW(15))
    CALL(evt_seq_wait, 2)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 100)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)
    DELETE_EVENT(LW(15))
    CALL(evt_npc_get_position, STRING("ボスロボット"), LW(0), LW(1), LW(2))
    ADD(LW(1), 90)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD_EXPLSION2"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_eff64, STRING("eff_hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    SET_VALUE(LW(10), STRING("ボスロボット"))
    SPAWN_EVENT_GET_ID(PTR(boss_robot_kemuri), LW(15))
    WAIT_MS(500)
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_eff_softdelete, STRING("eff_hikari"))
    SET_VALUE(GF(0), 1)
    WAIT_MS(1300)
    BEGIN_INLINE_EVENT
        WAIT_MS(1700)
        DELETE_EVENT(LW(15))
    END_INLINE_EVENT
    DELETE_EVENT(GW(11))
    CALL(evt_npc_set_position, STRING("ボスロボット"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ボスロボットマル"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ボスロボットバツ"), 0, -1000, 0)
    SET_VALUE(LW(10), STRING("第三勢力幹部"))
    CALL(evt_npc_set_position, LW(10), 100, 40, 20)
    CALL(evt_npc_set_ry, LW(10), 270)
    CALL(evt_npc_flag_onoff, 1, LW(10), 1073743392)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_party_set_dir, 0, 270, 200)
        CALL(evt_npc_set_ry, STRING("ガイドsp"), 270)
        CALL(evt_npc_set_ry, STRING("ガイド妹sp"), 270)
        CALL(evt_npc_set_ry, STRING("ライバルsp"), 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_rotate_offset, STRING("第三勢力幹部"), 0, 25, 0)
        CALL(evt_sub_intpl_msec_init, 0, 1800, 855, 700)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_rotate, STRING("第三勢力幹部"), 0, 0, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -200, 153, 549, -200, 57, 7, 1000, 11)
    SET_VALUE(LW(10), STRING("第三勢力幹部"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ROUND1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(550)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(10), -200, 0, 20, 700, 70)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_STICK1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("第三勢力幹部"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(14), -50)
        SET_VALUE(LW(10), LW(14))
        SUBTRACT(LW(10), LW(0))
        DIVIDE(LW(10), 10)
        SET_VALUE(LW(11), LW(10))
        MULTIPLY(LW(11), 5)
        ADD(LW(11), LW(0))
        SET_VALUE(LW(12), LW(10))
        MULTIPLY(LW(12), 7)
        ADD(LW(12), LW(0))
        SET_VALUE(LW(13), LW(10))
        MULTIPLY(LW(13), 9)
        ADD(LW(13), LW(0))
        CALL(evt_npc_set_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        CALL(evt_item_entry, STRING("item00"), 115, 0, -1000, 0, 16, -1, 0)
        SET_VALUE(LW(8), STRING("ダミー"))
        SET_VALUE(LW(9), STRING("item00"))
        SPAWN_EVENT_GET_ID(PTR(starstone_item), LW(15))
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(11), 0, LW(2), 625, FLOAT(30.0f))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(11), 0, LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(12), 0, LW(2), 250, FLOAT(15.0f))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(12), 0, LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(13), 0, LW(2), 250, FLOAT(7.5f))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(13), 0, LW(2), 0)
        CALL(evt_npc_jump_position_nohit, STRING("ダミー"), LW(14), 0, LW(2), 125, FLOAT(3.25f))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_STARSTONE_JUMP1"), LW(14), 0, LW(2), 0)
        DELETE_EVENT(LW(15))
        CALL(evt_npc_set_position, STRING("ダミー"), 0, -1000, 0)
        CALL(evt_item_flag_off, STRING("item00"), 2)
        CALL(evt_item_set_position, STRING("item00"), LW(14), 0, LW(2))
    END_INLINE_EVENT
    WAIT_MS(800)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_HAPPY1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_00"), 0, LW(10))
    WAIT_MS(500)
    CALL(evt_npc_set_rotate, LW(10), 0, 0, 0)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_cam3d_evt_set, -200, 88, 253, -200, 45, 7, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(833)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_01"), 0, LW(10))
    SET_VALUE(GF(0), 1)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(600)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, LW(10), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(1000)
    SET_VALUE(LW(10), STRING("部下１"))
    SET_VALUE(LW(11), STRING("部下２"))
    SET_VALUE(LW(12), STRING("部下３"))
    SET_VALUE(LW(13), STRING("部下４"))
    SET_VALUE(LW(14), STRING("部下５"))
    CALL(evt_npc_set_position, LW(10), 200, 0, 80)
    CALL(evt_npc_set_position, LW(11), 200, 0, 65)
    CALL(evt_npc_set_position, LW(12), 200, 0, 50)
    CALL(evt_npc_set_position, LW(13), 200, 0, 35)
    CALL(evt_npc_set_position, LW(14), 200, 0, 20)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 200, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
        CALL(evt_npc_set_ry, STRING("ガイドsp"), 90)
        CALL(evt_npc_set_ry, STRING("ガイド妹sp"), 90)
        CALL(evt_npc_set_ry, STRING("ライバルsp"), 90)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_02"), 0, LW(10))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, LW(11), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(11), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_get_position, LW(12), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, LW(12), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(12), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_get_position, LW(13), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, LW(13), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(13), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_npc_get_position, LW(14), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, LW(14), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(14), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_get_position, LW(11), LW(0), LW(1), LW(2))
        ADD(LW(0), -80)
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_03"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_get_position, LW(12), LW(0), LW(1), LW(2))
        ADD(LW(0), -80)
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_04"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1200)
        CALL(evt_npc_get_position, LW(13), LW(0), LW(1), LW(2))
        ADD(LW(0), -80)
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_05"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_npc_get_position, LW(14), LW(0), LW(1), LW(2))
        ADD(LW(0), -80)
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_00_06"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_move_position, LW(10), 30, LW(2), 0, FLOAT(180.0f), 0)
    CALL(evt_npc_flag_onoff, 0, LW(10), 1024)
    CALL(evt_npc_set_anim, LW(10), STRING("D_4"))
    CALL(evt_npc_jump_position_nohit, LW(10), 0, 0, LW(2), 200, 15)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD1_MISS1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, LW(10), 1024)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, LW(10), -400, LW(2), 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_npc_set_ry, STRING("ガイドsp"), 270)
    CALL(evt_npc_set_ry, STRING("ガイド妹sp"), 270)
    CALL(evt_npc_set_ry, STRING("ライバルsp"), 270)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -50, 115, 400, -50, 45, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_npc_set_ry, STRING("ガイドsp"), 90)
    CALL(evt_npc_set_ry, STRING("ガイド妹sp"), 90)
    CALL(evt_npc_set_ry, STRING("ライバルsp"), 90)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_HAPPY1"))
    CALL(evt_msg_print_party, STRING("stg2_mri_e25_00"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e25_01"), 0, STRING("ガイドsp"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e25_02"), 0, STRING("プニ族長老"))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry_lr, STRING("ライバルsp"), 0)
    CALL(evt_mario_mov_pos, -50, 60, 500)
    SET_VALUE(GSW_STORY_PROGRESS, 112)
    CALL(evt_item_get_position, STRING("item00"), LW(0), LW(1), LW(2))
    CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
    SET_VALUE(LW(10), STRING("item01"))
    CALL(evt_item_entry, LW(10), 115, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, LW(10), 2)
    SET_VALUE(LW(0), 2)
    SET_VALUE(LW(1), LW(10))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL_CHILD(PTR(evt_sub_starstone))
    RETURN
EVT_END()

static EVT_BEGIN(door_switch_init)
    IF_INT_EQUAL(GSWF(2841), 0)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_map_playanim, STRING("doalight01"), 2, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ueki_puni_nigeru)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族"), 128)
    CALL(evt_npc_set_position, STRING("プニ族"), LW(0), 0, LW(1))
    CALL(evt_msg_print, 0, LW(2), 0, STRING("プニ族"))
    CALL(evt_npc_flag_onoff, 0, STRING("プニ族"), 128)
    ADD(LW(0), 50)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族"), LW(0), 0, LW(1), 300, 30)
    ADD(LW(0), 200)
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("プニ族"), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_MOVE2_1"), LW(10), LW(11), LW(12), 0)
            WAIT_FRAMES(5)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("プニ族"), LW(0), LW(1), 0, 150, 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_position, STRING("プニ族"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("プニ族"), 32)
    RETURN
EVT_END()

EVT_BEGIN(evt_ueki_01)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 170)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("kpa_stg3_005"), 0, STRING("ライバル"))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_ueki_02)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 170)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("kpa_stg3_003"), 0, STRING("ガイド妹"))
        CALL(evt_msg_print, 0, STRING("kpa_stg3_004"), 0, STRING("ガイド"))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_ueki_03)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 170)
        IF_INT_EQUAL(GSWF(2883), 0)
            CALL(evt_mario_key_onoff, 0)
            SET_VALUE(LW(0), -245)
            SET_VALUE(LW(1), -15)
            SET_VALUE(LW(2), STRING("kpa_stg3_006_2"))
            CALL_CHILD(PTR(ueki_puni_nigeru))
            SET_VALUE(GSWF(2883), 1)
            CALL(evt_mario_key_onoff, 1)
        END_IF
        ELSE
        CALL(evt_item_delete_check, STRING("item_ueki_03"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_item_entry, STRING("item_ueki_03"), 121, -240, 0, -20, 4, GSWF(2828), 0)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_ueki_08)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 170)
        SPAWN_EVENT(PTR(mri_01_koopa_second_evt))
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", evt_ueki_01},
    {1, "A_ueki02", "S_ueki02", evt_ueki_02},
    {1, "A_ueki03", "S_ueki03", evt_ueki_03},
    {1, "A_ueki08", "S_ueki08", evt_ueki_08},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(puni_mario_wait)
    CALL(evt_puni_wait_systemmode, 2)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_sub_random, 60, LW(0))
        ADD(LW(0), -280)
        CALL(evt_sub_random, 40, LW(2))
        ADD(LW(2), 0)
        CALL(evt_puni_get_trap_index, LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            CALL(evt_puni_init_puni, LW(10), 1, LW(0), 0, LW(2), 1)
        END_IF
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_01_koopa_first_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -415, 0, 30)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_set_position, STRING("ババ"), -415, 0, 30)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 32)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_npc_set_position, STRING("プニ族"), -126, 0, 11)
    CALL(evt_npc_set_position, STRING("プニ族B"), -95, 0, 11)
    CALL(evt_npc_set_position, STRING("プニ族長老"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("プニ族"), 90)
    CALL(evt_npc_set_ry, STRING("プニ族B"), 270)
    CALL(evt_cam3d_evt_set, -126, 134, 399, -126, 65, 8, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_fade_end_wait)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    SET_VALUE(LW(0), STRING("dokan"))
    CALL_CHILD(PTR(bero_case_switch_off))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("ババ"), -355, 30, 0, FLOAT(60.0f), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -280, 30, FLOAT(60.0f))
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("プニ族"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("プニ族B"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("プニ族"), 270)
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("プニ族"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("プニ族"), LW(0), LW(1), LW(2), 200, 10)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_000"), 0, STRING("プニ族"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族B"), LW(10), LW(11), LW(12))
        ADD(LW(10), 150)
        SET_VALUE(LW(0), STRING("プニ族B"))
        SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
        CALL(evt_npc_move_position, STRING("プニ族B"), LW(10), LW(12), 0, FLOAT(90.0f), 0)
        DELETE_EVENT(LW(9))
        CALL(evt_npc_flag_onoff, 1, STRING("プニ族B"), 160)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("プニ族"), LW(10), LW(11), LW(12))
    ADD(LW(10), 150)
    SET_VALUE(LW(0), STRING("プニ族"))
    SPAWN_EVENT_GET_ID(PTR(mri_puni_walk_se), LW(9))
    CALL(evt_npc_move_position, STRING("プニ族"), LW(10), LW(12), 0, FLOAT(90.0f), 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族"), 160)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_000_01"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, -165, 135, 365, -310, 65, 7, 1000, 11)
    WAIT_MS(500)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    WAIT_MS(500)
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_000_02"), 0, STRING("ババ"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_000_03"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(250)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_001"), 0, STRING("mario"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_002"), 0, STRING("ババ"))
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), 32)
    CALL(evt_mario_set_dir, 90, 300, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 170)
    CALL(evt_npc_set_position, STRING("ライバル"), -198, -5, 53)
    CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ライバル"), 32)
    CALL(evt_npc_set_position, STRING("ガイド"), -202, 5, -59)
    CALL(evt_npc_set_position, STRING("ガイド妹"), -196, -3, -69)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド妹"), 32)
    CALL(evt_npc_set_position, STRING("プニ族長老"), 261, 0, 62)
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("プニ族長老"), 32)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(mri_01_koopa_second_evt)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_zoom_in, 15, 800, 0, 11)
    WAIT_MS(800)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("プニ族長老"), LW(3), LW(4), LW(5))
    IF_INT_LESS_OR_EQUAL(LW(0), LW(3))
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
        SET_VALUE(LF(0), 0)
        ELSE
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_npc_set_ry, STRING("プニ族長老"), 90)
        SET_VALUE(LF(0), 1)
    END_IF
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LF(0), 0)
            SET_VALUE(LW(0), 178)
            ELSE
            SET_VALUE(LW(0), 338)
        END_IF
        CALL(evt_mario_jump_pos, LW(0), 0, LW(2), 200, 1, -1000)
        WAIT_MS(50)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
        CALL(kpa_chg_pose, STRING("KPA_O_1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_BIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 2, FLOAT(1.0f), FLOAT(3.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("kpa_stg3_008"), 0, STRING("プニ族長老"))
    CALL(evt_fade_out, 700)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNI1"))
    CALL(evt_npc_set_scale, STRING("プニ族長老"), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(evt_mario_set_pos, 0, 0, 25)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_set_position, STRING("ババ"), -54, 0, 40)
    CALL(evt_npc_set_position, STRING("プニ族長老"), 90, 0, 25)
    CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
    CALL(evt_npc_set_position, STRING("ガイド"), 131, 0, 27)
    CALL(evt_npc_set_ry, STRING("ガイド"), 270)
    CALL(evt_npc_set_position, STRING("ガイド妹"), 145, 0, 30)
    CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
    CALL(evt_npc_set_position, STRING("ライバル"), 121, 0, 13)
    CALL(evt_npc_set_ry, STRING("ライバル"), 270)
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_cam3d_evt_set, 45, 151, 500, 45, 67, 27, 0, 11)
    WAIT_MS(500)
    CALL(evt_fade_in, 700)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_010"), 0, STRING("プニ族長老"))
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 32)
    CALL(evt_npc_move_position, STRING("ババ"), 20, 40, 0, FLOAT(120.0f), 0)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("A_1B"), STRING("T_4"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_011"), 0, STRING("ババ"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("プニ族長老"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHOUROU_BIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 2, FLOAT(1.0f), FLOAT(3.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_jump_position_nohit, STRING("ババ"), -73, 0, 40, 200, 20)
        WAIT_MS(50)
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("P_1"))
    END_INLINE_EVENT
    SET_VALUE(LSWF(2), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(LSWF(2), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_msec_init, 5, FLOAT(3.0f), FLOAT(1.0f), 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_scale, STRING("プニ族長老"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -20, 151, 500, -20, 67, 27, 200, 11)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_012"), PTR(mri_stg3_012_cb), STRING("プニ族長老"))
    CALL(evt_npc_set_scale, STRING("プニ族長老"), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("K_1"), STRING("T_7"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_013"), 0, STRING("ババ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("S_1"))
    CALL(evt_cam3d_evt_set, 17, 131, 388, 17, 67, 27, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_THINK1_2"), LW(0), LW(1), LW(2), 0)
    CALL(kpa_chg_pose, STRING("KPA_N_1"))
    WAIT_MS(500)
    CALL(evt_msg_select, 0, STRING("kpa_stg3_014"))
    CALL(evt_cam3d_evt_set, -20, 151, 500, -20, 67, 27, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_7"), STRING("KPA_S_7"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_016"), 0, STRING("mario"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_017"), 0, STRING("ババ"))
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_018"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_cam3d_evt_set, 45, 151, 500, 45, 67, 27, 300, 11)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_018_1"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_019"), 0, STRING("プニ族長老"))
    CALL(kpa_chg_pose, STRING("KPA_O_1"))
    CALL(evt_cam3d_evt_set, 20, 125, 353, 20, 67, 27, 500, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SURPRISED3"))
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_5"), STRING("KPA_T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_020"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 45, 151, 500, 45, 67, 27, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_021"), 0, STRING("プニ族長老"))
    CALL(evt_cam3d_evt_set, -20, 151, 500, -20, 67, 27, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg3_022"), 0, STRING("ババ"))
    CALL(kpa_chg_pose, STRING("KPA_S_4"))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 270, 250, 0)
    WAIT_MS(250)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg3_023"), 0, STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 171)
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff_f, 512, 1000)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 9)
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_memcard_save_wrapper)
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, 59, 0, 58, 0)
    CALL(evt_bero_mapchange, STRING("tou_02"), 0)
    RETURN
EVT_END()

EVT_BEGIN(mri_01_init_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 169)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 170)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_puni_mem_init)
        CALL(evt_puni_set_yuka, 1, 0, -310, -1000, -1000, 0, 0)
        CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), 0)
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        CALL_CHILD(PTR(mri_puni_init))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(LF(0), 0)
        SPAWN_EVENT(PTR(puni_disp_info))
    END_IF
    SPAWN_EVENT(PTR(mri_countdown))
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_sibu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    SET_VALUE(LW(0), 110)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -45)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_BETWEEN(169, 170)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG2_PUNI1"))
        CALL(evt_snd_bgmon, 512, STRING("ENV_STG2_MRI3"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI3"))
    END_SWITCH
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 330, 60, -25, PTR(mri_puni_save), 0)
        CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 8, 260, 60, -25, 0, 0)
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 88)
        SPAWN_EVENT(PTR(bigtree_firstbattle))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 89)
        SPAWN_EVENT(PTR(meet_rival))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 94)
        SPAWN_EVENT(PTR(rival_nakama))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 110)
        CALL_CHILD(PTR(boss_battle_init))
        SPAWN_EVENT(PTR(boss_battle))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 111)
        CALL_CHILD(PTR(boss_battle_init))
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(boss_battle_win_init))
        SPAWN_EVENT(PTR(boss_battle_win))
    END_IF
    SPAWN_EVENT(PTR(door_switch_init))
    IF_INT_EQUAL(GSW(665), 2)
        IF_INT_EQUAL(GSW(666), 1)
            SPAWN_EVENT(PTR(puni_mario_wait))
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 95)
        IF_INT_EQUAL(GSWF(2829), 0)
            SPAWN_EVENT(PTR(elder_kaisetsu))
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 169)
        CALL(evt_mario_set_mode, 2)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_npc_entry, STRING("ババ"), STRING("c_kamek_be"))
        CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（地上）"))
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 1073743360)
        CALL(evt_npc_status_onoff, 1, STRING("ババ"), 2)
        CALL(evt_npc_set_position, STRING("ババ"), 0, -1000, 0)
        CALL(evt_npc_set_ry, STRING("ババ"), 90)
        CALL(evt_npc_change_interrupt, STRING("ババ"), 6, PTR(baba_talk))
        SPAWN_EVENT(PTR(mri_01_koopa_first_evt))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 170)
        CALL(evt_mario_set_mode, 2)
        CALL(evt_npc_entry, STRING("ババ"), STRING("c_kamek_be"))
        CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（地上）"))
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 1073743360)
        CALL(evt_npc_status_onoff, 1, STRING("ババ"), 2)
        CALL(evt_npc_set_position, STRING("ババ"), 0, -1000, 0)
        CALL(evt_npc_set_ry, STRING("ババ"), 90)
        CALL(evt_npc_change_interrupt, STRING("ババ"), 6, PTR(baba_talk))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_npc_set_position, STRING("ライバル"), -198, -5, 53)
        CALL(evt_npc_set_ry, STRING("ライバル"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("ライバル"), 32)
        CALL(evt_npc_set_position, STRING("ガイド"), -202, 5, -59)
        CALL(evt_npc_set_position, STRING("ガイド妹"), -196, -3, -69)
        CALL(evt_npc_set_ry, STRING("ガイド"), 270)
        CALL(evt_npc_set_ry, STRING("ガイド妹"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), 32)
        CALL(evt_npc_flag_onoff, 1, STRING("ガイド妹"), 32)
        CALL(evt_npc_set_position, STRING("プニ族長老"), 261, 0, 62)
        CALL(evt_npc_set_ry, STRING("プニ族長老"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("プニ族長老"), 32)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("player"))
            CALL(evt_mario_set_pos, -280, 0, 30)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_npc_set_position, STRING("ババ"), -355, 0, 30)
            CALL(evt_npc_set_ry, STRING("ババ"), 90)
            ELSE
            CALL(evt_mario_set_pos, 226, 0, 48)
            CALL(evt_mario_set_dir, 90, 0, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                SPAWN_EVENT(PTR(mri_01_koopa_second_evt))
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

