#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"

extern EvtWord all_juyoitem_lecture_rel[];
extern NpcTerritoryInfo btl_eki_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_eki_tbl[];
extern EvtWord eki_00_init_evt[];
extern EvtWord eki_01_init_evt[];
extern EvtWord eki_02_init_evt[];
extern EvtWord eki_03_init_evt[];
extern EvtWord eki_04_init_evt[];
extern EvtWord eki_05_init_evt[];
extern EvtWord eki_06_init_evt[];
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_kurokumorn;
extern BattleUnitKind unit_patatogemet;
extern BattleUnitKind unit_sambo_mummy;

extern EvtWord evt_sw_complete[];

// Area-local saved values (EKI; swReInit clears them on every area change), named from the area's
// scripts
// JP: plain brick blk01 in eki_03 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken; US: passed to evt_mobj_blk,
// whose mobj_blk never sets or reads it
#define LSWF_EKI_03_BRICK_BLK01 LSWF(1)
