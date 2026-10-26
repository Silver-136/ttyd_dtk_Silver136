#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord all_juyoitem_lecture_rel[];
extern NpcTerritoryInfo btl_mri_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_mri_tbl[];
extern EvtWord evt_mri_door_close[];
extern EvtWord evt_mri_door_close_pure[];
extern EvtWord evt_mri_door_naname_close[];
extern EvtWord evt_mri_door_naname_close_pure[];
extern EvtWord evt_mri_door_naname_open[];
extern EvtWord evt_mri_door_naname_open_pure[];
extern EvtWord evt_mri_door_open[];
extern EvtWord evt_mri_door_open_pure[];
f32 getMri02Yuka(void);
extern EvtWord mri_00_init_evt[];
extern EvtWord mri_01_init_evt[];
extern EvtWord mri_02_init_evt[];
extern EvtWord mri_03_init_evt[];
extern EvtWord mri_04_init_evt[];
extern EvtWord mri_05_init_evt[];
extern EvtWord mri_06_init_evt[];
extern EvtWord mri_07_init_evt[];
extern EvtWord mri_08_init_evt[];
extern EvtWord mri_09_init_evt[];
extern EvtWord mri_10_init_evt[];
extern EvtWord mri_11_init_evt[];
extern EvtWord mri_12_init_evt[];
extern EvtWord mri_13_init_evt[];
extern EvtWord mri_14_init_evt[];
extern EvtWord mri_15_init_evt[];
extern EvtWord mri_16_init_evt[];
extern EvtWord mri_17_init_evt[];
extern EvtWord mri_18_init_evt[];
extern EvtWord mri_19_init_evt[];
extern EvtWord mri_20_init_evt[];
extern EvtWord mri_countdown[];
extern EvtWord mri_guide_goryu[];
extern EvtWord mri_guide_hagure_check[];
extern EvtWord mri_hikari_color[];
extern EvtWord mri_puni_init[];
extern EvtWord mri_puni_init_none[];
extern EvtWord mri_puni_jump_se[];
extern EvtWord mri_puni_save[];
extern EvtWord mri_puni_walk_se[];
extern EvtWord mri_punipuni_itemremove[];
extern EvtWord mri_punipuni_itemselect[];
extern EvtWord mri_punipuni_itemset[];
extern EvtWord mri_punipuni_setup[];
extern EvtWord mri_waterfall_effect[];
extern EvtWord puni_disp_info[];
extern EvtWord puni_piders_check[];
extern BattleUnitKind unit_boss_magnum_battender;
extern BattleUnitKind unit_gundan_zako;
extern BattleUnitKind unit_monochrome_pakkun;
extern BattleUnitKind unit_piders;

extern s32 JailAwayTable_New[];
extern s32 evt_cloud_main_evt[];
extern EvtWord mri_puni_init_dvd[];
extern EvtWord mri_puni_main[];
extern EvtWord mri_puni_position_init[];
extern EvtWord mri_puninum_fadein[];
extern EvtWord mri_puninum_fadeout[];
extern u8 puniMapdata_MRI_06[];
extern u8 puniMapdata_MRI_09[];
extern u8 puniMapdata_MRI_09_01[];
extern u8 puniMapdata_MRI_16[];
USER_FUNC(evt_mri_door_naname_func);
USER_FUNC(evt_mri_get16bitdata);
USER_FUNC(evt_mri_puni_win_attack);
USER_FUNC(evt_mri_puni_win_end);
USER_FUNC(evt_mri_puni_win_init);
USER_FUNC(evt_mri_toge_enter_init);
USER_FUNC(evt_mri_toge_make_retsu);
USER_FUNC(evt_mri_toge_retsu_stop);
USER_FUNC(hakari_color);
USER_FUNC(mri_npc_buruburu);
void togeMakeRetsu(Vec* pos, s16 leader, s16 start, s16 end);

// Area-local saved values (MRI; swReInit clears them on every area change), named from the area's
// scripts
// hidden brick mobj_hidden1 in mri_06 holding a coin (evt_mobj_brick MOBJ_BRICK_HIDDEN_ITEM), set when the coin comes
// out
#define LSWF_MRI_06_HIDDEN_COIN_BRICK LSWF(1)
// cleared by mri_01_koopa_second_evt before kpa_stg3_012; the callback mri_stg3_012_cb sets it at the second page to
// start a waiting inline event (dossier names the C use elder_kaisetsu)
#define LSWF_MRI_01_KOOPA_MSG_CUE LSWF(2)
// set by sekai_yami_evt and minnnanokoe_evt (mri_00) when the room goes dark (envSetYami); puni_puni_disp
// (mri_puni.c, _swGet(3)) then draws each Puni with the darkness TEV
#define LSWF_MRI_00_DARKNESS_ON LSWF(3)
// start of 102 seven-byte Puni records LSW(0..713), one per Puni index - 1: byte 0 = floor where the Puni strayed (0
// = none), bytes 1-6 = x, y, z as 16-bit low/high; written by evt_puni_all_save, the area-exit code and
// evt_puni_set_lsw_index, read by puniswGetindex and evt_puni_get_lsw_index (mri, mri_01, mri_03)
#define LSW_MRI_PUNI_RECORDS LSW(0)
// puniinfo_talk (mri_07, the Puni information broker): 0-2 picks the next of three lines per story stage and cycles
// back to 0; follows the Puni records
#define LSW_MRI_07_PUNI_INFO_TALK_INDEX LSW(714)
