#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"

extern EvtWord aji_00_init_evt[];
extern EvtWord aji_01_init_evt[];
extern EvtWord aji_02_init_evt[];
extern EvtWord aji_03_init_evt[];
extern EvtWord aji_04_init_evt[];
extern EvtWord aji_05_init_evt[];
extern EvtWord aji_06_init_evt[];
extern EvtWord aji_07_init_evt[];
extern EvtWord aji_08_init_evt[];
extern EvtWord aji_09_init_evt[];
extern EvtWord aji_10_init_evt[];
extern EvtWord aji_11_init_evt[];
extern EvtWord aji_12_init_evt[];
extern EvtWord aji_13_init_evt[];
extern EvtWord aji_14_init_evt[];
extern EvtWord aji_15_init_evt[];
extern EvtWord aji_16_init_evt[];
extern EvtWord aji_17_init_evt[];
extern EvtWord aji_18_init_evt[];
extern EvtWord aji_19_init_evt[];
extern NpcTerritoryInfo btl_aji_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_aji_tbl[];
extern EvtWord evt_emergency[];
extern EvtWord shuryolight_head_normal[];
extern EvtWord shuryolight_pow_normal[];
extern EvtWord shuryolight_tue_normal[];
extern BattleUnitKind unit_boss_magnum_battender_mkII;
extern BattleUnitKind unit_gundan_zako;
extern BattleUnitKind unit_gundan_zako_elite;
extern BattleUnitKind unit_gundan_zako_magician;

extern s32 kusuri_id[];
extern s32 kusuri_list[];
extern s32 kusuri_tyogo_list[];

// Area-local saved values (AJI; swReInit clears them on every area change), named from the area's
// scripts
// set when Peach picks up the Red Potion (peach_evt_kusuri_aka) or takes it back out of a slot
// (peach_evt_pick_kusuri); cleared when she sets it in a slot (peach_evt_kaihei1-4); peach_evt_kusuri_select lists it
// while set
#define LSWF_AJI_06_RED_POTION_HELD LSWF(1)
#define LSWF_AJI_06_BLUE_POTION_HELD LSWF(2) /* as LSWF(1) for the Blue Potion (peach_evt_kusuri_ao) */
#define LSWF_AJI_06_ORANGE_POTION_HELD LSWF(3) /* as LSWF(1) for the Orange Potion (peach_evt_kusuri_ki) */
#define LSWF_AJI_06_GREEN_POTION_HELD LSWF(4) /* as LSWF(1) for the Green Potion (peach_evt_kusuri_mid) */
// set by peach_evt_check_all_kusuri_set when Peach confirms the placement (start event peach_evt_tyogo_entry on
// A_sui1); peach_evt_pick_kusuri removes that event and clears it when a potion is taken back out
#define LSWF_AJI_06_MIX_START_OFFERED LSWF(5)
// set by peach_evt_tyogo while the machine runs, cleared at its end; while set peach_evt_kaihei1-4 dispense instead
// of setting or taking potions
#define LSWF_AJI_06_MIXING_RUNNING LSWF(6)
// set when a dispense button is pressed inside the window (peach_evt_kaihei1-4, peach_evt_stop_renji for the heater);
// peach_evt_tyogo_sub waits on it; peach_evt_kusuri_dasu colors the mix while set and clears it
#define LSWF_AJI_06_DISPENSE_PRESSED LSWF(7)
// set by peach_evt_tyogo_sub while the beaker stands at a stop and while the heater runs; peach_evt_kaihei1-4 and
// peach_evt_stop_renji only count a press while set
#define LSWF_AJI_06_DISPENSE_WINDOW_OPEN LSWF(8)
// set by peach_evt_tyogo when all nine checks succeeded (GW(13) == 9); peach_evt_kusuri_nomu: 0 = random grow/shrink,
// 1 = invisibility and STG5_45_PEACH_POTION_PUZZLE
#define LSWF_AJI_06_MIX_CORRECT LSWF(9)
// set by peach_evt_tyogo when Peach accepts the result (drink event on A_bika1); peach_evt_check_all_kusuri_set
// returns early while set; peach_evt_kusuri_nomu clears it
#define LSWF_AJI_06_DRINK_PENDING LSWF(10)
// set while a memo (peach_evt_memo1-4) or the locked-door line peach_aji_226 (bero_peach_evt) is shown;
// peach_evt_wait_memo waits until it is clear; peach_evt_stg5 clears it
#define LSWF_AJI_06_PEACH_MSG_OPEN LSWF(11)
// potion set in Concoctionator slot 1 (peach_evt_kaihei1, case A_sui2, S_mizu1): 0 = empty, 1 = Red, 2 = Blue, 3 =
// Orange, 4 = Green Potion; peach_evt_check_all_kusuri_set offers the start once slots 1-4 sum to 10;
// peach_evt_seikai_check reads it
#define LSW_AJI_06_SLOT_1_POTION LSW(1)
// potion set in slot 2 (peach_evt_kaihei2, A_sui3, S_mizu2); values as LSW(1)
#define LSW_AJI_06_SLOT_2_POTION LSW(2)
// potion set in slot 3 (peach_evt_kaihei3, A_sui4, S_mizu3); values as LSW(1)
#define LSW_AJI_06_SLOT_3_POTION LSW(3)
// potion set in slot 4 (peach_evt_kaihei4, A_sui5, S_mizu4); values as LSW(1)
#define LSW_AJI_06_SLOT_4_POTION LSW(4)
// set at random by peach_evt_kusuri_nomu when a wrong mix is drunk: 0 = Peach grew, 1 = Peach shrank;
// peach_evt_sugata_modoru reverses it after 15 s
#define LSW_AJI_06_WRONG_POTION_EFFECT LSW(5)
