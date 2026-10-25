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
