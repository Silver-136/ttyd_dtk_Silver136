#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord box_syutugen[];
extern NpcTerritoryInfo btl_las_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_las_tbl[];
extern EvtWord hikari_evt[];
extern EvtWord las_00_init_evt[];
extern EvtWord las_01_init_evt[];
extern EvtWord las_02_init_evt[];
extern EvtWord las_03_init_evt[];
extern EvtWord las_04_init_evt[];
extern EvtWord las_05_init_evt[];
extern EvtWord las_06_init_evt[];
extern EvtWord las_07_init_evt[];
extern EvtWord las_08_init_evt[];
extern EvtWord las_09_init_evt[];
extern EvtWord las_10_init_evt[];
extern EvtWord las_11_init_evt[];
extern EvtWord las_12_init_evt[];
extern EvtWord las_13_init_evt[];
extern EvtWord las_14_init_evt[];
extern EvtWord las_15_init_evt[];
extern EvtWord las_16_init_evt[];
extern EvtWord las_17_init_evt[];
extern EvtWord las_18_init_evt[];
extern EvtWord las_19_init_evt[];
extern EvtWord las_20_init_evt[];
extern EvtWord las_21_init_evt[];
extern EvtWord las_22_init_evt[];
extern EvtWord las_23_init_evt[];
extern EvtWord las_24_init_evt[];
extern EvtWord las_25_init_evt[];
extern EvtWord las_26_init_evt[];
extern EvtWord las_27_init_evt[];
extern EvtWord las_28_init_evt[];
extern EvtWord las_29_init_evt[];
extern EvtWord las_30_init_evt[];
extern EvtWord move_dai[];
extern EvtWord shuryolight_head_normal[];
extern EvtWord shuryolight_head_off[];
extern EvtWord shuryolight_head_tenmetu[];
extern EvtWord shuryolight_pow_normal[];
extern EvtWord shuryolight_pow_tenmetu[];
extern EvtWord shuryolight_tue_normal[];
extern BattleUnitKind unit_basabasa_chururu;
extern BattleUnitKind unit_boss_black_peach;
extern BattleUnitKind unit_boss_bunbaba;
extern BattleUnitKind unit_honenoko;
extern BattleUnitKind unit_karon;
extern BattleUnitKind unit_phantom;
extern BattleUnitKind unit_super_killer;
extern BattleUnitKind unit_super_mahorn;
extern BattleUnitKind unit_wanwan;

extern s32 evt_cloud_main_evt[];
extern EvtWord firebar_coin[];
extern EvtWord hikari_off_evt[];
extern EvtWord hikari_on_evt[];
extern u8 kurokaron_move_flag;
extern u8 kurokaron_move_index;
extern f32 kurokaron_move_list[6][2];
extern u8 kurokaron_prev_goal;
extern u16 kurokaron_run_cnt;
extern u8 kurokaron_same_goal;
extern u8 kurokaron_stop_cnt;
extern s32 kurokaron_vivian_prev_sts;
extern s32 move_check_list[];
USER_FUNC(mobj_color_off);
USER_FUNC(mobj_set_color);
USER_FUNC(mobj_set_ry);

// Area-local saved values (LAS; swReInit clears them on every area change), named from the area's
// scripts
// star S_hosi_02 lit in las_20, toggled by switch hosi_02_sw (hosi_onoff); hosi_sw_check wants it lit; hosi_init
// clears it
#define LSWF_LAS_20_STAR_02_LIT LSWF(1)
// star S_hosi_03 lit, toggled by hosi_03_sw; hosi_sw_check wants it unlit; hosi_init clears it
#define LSWF_LAS_20_STAR_03_LIT LSWF(2)
// star S_hosi_04 lit, toggled by hosi_04_sw; hosi_sw_check wants it lit; hosi_init clears it
#define LSWF_LAS_20_STAR_04_LIT LSWF(3)
// star S_hosi_05 lit, toggled by hosi_05_sw; hosi_sw_check wants it lit; hosi_init clears it
#define LSWF_LAS_20_STAR_05_LIT LSWF(4)
// star S_hosi_06 lit, toggled by hosi_06_sw; hosi_sw_check wants it unlit; hosi_init clears it
#define LSWF_LAS_20_STAR_06_LIT LSWF(5)
// star S_hosi_07 lit, toggled by hosi_07_sw; hosi_sw_check wants it lit; hosi_init clears it
#define LSWF_LAS_20_STAR_07_LIT LSWF(6)
// star S_hosi_08 lit, toggled by hosi_08_sw; hosi_sw_check wants it unlit; hosi_init clears it
#define LSWF_LAS_20_STAR_08_LIT LSWF(7)
// floating block 21blk1 in las_21: 1 = down at -580, 0 = up at -130; set by las_21_init_evt, toggled by
// floatblk21_1_evt (switch 21switch1)
#define LSWF_LAS_21_BLK1_LOWERED LSWF(8)
// floating block 21blk2 in las_21: 1 = down at -580, 0 = up at -130; set by las_21_init_evt, toggled by
// floatblk21_2_evt (switch 21switch2)
#define LSWF_LAS_21_BLK2_LOWERED LSWF(9)
// set and cleared by last_evt_2 and last_evt_3 (las_29) for the light flashes; map_color_tev (C) draws the map white
// while set, black otherwise (dossier names the C use hikari_on_evt)
#define LSWF_LAS_29_MAP_LIT_WHITE LSWF(10)
// times switch1 was pressed in las_13 (switch1_evt, up to 255); switch_check wants 2; las_13_init_evt resets it until
// GSWF_LAS_13_SWITCH_PUZZLE_SOLVED
#define LSW_LAS_13_SWITCH_1_COUNT LSW(1)
// times switch2 was pressed while switch1 has not been pressed (switch2_evt, up to 255); switch_check wants 3;
// las_13_init_evt resets it
#define LSW_LAS_13_SWITCH_2_COUNT LSW(2)
