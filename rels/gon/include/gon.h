#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"

extern NpcTerritoryInfo btl_gon_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_gon_tbl[];
extern EvtWord gon_00_init_evt[];
extern EvtWord gon_01_init_evt[];
extern EvtWord gon_02_init_evt[];
extern EvtWord gon_03_init_evt[];
extern EvtWord gon_04_init_evt[];
extern EvtWord gon_05_init_evt[];
extern EvtWord gon_06_init_evt[];
extern EvtWord gon_07_init_evt[];
extern EvtWord gon_08_init_evt[];
extern EvtWord gon_09_init_evt[];
extern EvtWord gon_10_init_evt[];
extern EvtWord gon_11_init_evt[];
extern EvtWord gon_12_init_evt[];
extern EvtWord gon_13_init_evt[];
extern EvtWord gon_bt00_init_evt[];
extern EvtWord slit_majin_lecture[];
extern EvtWord slit_majin_lecture_init[];
extern BattleUnitKind unit_boss_gonbaba;
extern BattleUnitKind unit_honenoko;
extern BattleUnitKind unit_nokonoko;
extern BattleUnitKind unit_patakuri;
extern BattleUnitKind unit_patapata;
extern BattleUnitKind unit_togekuri;

extern s32 evt_cloud_main_evt[];

// Area-local saved values (GON; swReInit clears them on every area change), named from the area's
// scripts
// gon_09 floating block blk_0, toggled by switch sw_0 (main_0); init_0: 0 = block 200 units above its placed
// position, 1 = at it
#define LSWF_GON_09_FLOAT_BLOCK_MOVED LSWF(1)
// gon_08 floating block blk_0, toggled by switch sw_0 (main_0); init_0: 0 = at its placed position, 1 = 600 units
// above it
#define LSWF_GON_08_FLOAT_BLOCK_MOVED LSWF(3)
// gon_05 floating block blk_0, toggled by switch sw_0 (main_0); init_0: 0 = 525 units above its placed position, 1 =
// at it
#define LSWF_GON_05_FLOAT_BLOCK_0_MOVED LSWF(4)
// gon_05 floating block blk_1, toggled by switch sw_1 (main_1); init_1: 0 = at its placed position, 1 = 489 units
// above it
#define LSWF_GON_05_FLOAT_BLOCK_1_MOVED LSWF(5)
// gon_04 floating block mobj_fltbl01, toggled by switch mobj_fltsw01 (evt_gon_04flowblock_updown1); its init: 0 = 225
// units above its placed position, 1 = at it
#define LSWF_GON_04_FLOAT_BLOCK_1_MOVED LSWF(6)
// gon_04 floating block mobj_fltbl02, toggled by switch mobj_fltsw02 (evt_gon_04flowblock_updown2); its init: 0 = at
// its placed position, 1 = 200 units above it
#define LSWF_GON_04_FLOAT_BLOCK_2_MOVED LSWF(7)
