#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"

extern EvtWord all_party_lecture[];
extern NpcTerritoryInfo btl_hei_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_hei_tbl[];
extern EvtWord hei_00_init_evt[];
extern EvtWord hei_01_init_evt[];
extern EvtWord hei_02_init_evt[];
extern EvtWord hei_03_init_evt[];
extern EvtWord hei_04_init_evt[];
extern EvtWord hei_05_init_evt[];
extern EvtWord hei_06_init_evt[];
extern EvtWord hei_07_init_evt[];
extern EvtWord hei_08_init_evt[];
extern EvtWord hei_09_init_evt[];
extern EvtWord hei_10_init_evt[];
extern EvtWord hei_11_init_evt[];
extern EvtWord hei_12_init_evt[];
extern EvtWord hei_13_init_evt[];
extern EvtWord hei_bt00_init_evt[];
extern BattleUnitKind unit_chorobon;
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_monochrome_sinemon;
extern BattleUnitKind unit_nokonoko;
extern BattleUnitKind unit_patakuri;
extern BattleUnitKind unit_sinnosuke;
extern BattleUnitKind unit_togedaruma;
extern BattleUnitKind unit_togekuri;

// Area-local saved values (HEI; swReInit clears them on every area change), named from the area's
// scripts
// plain brick mm in hei_13 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken; hei_13_init_evt clears it on every
// load (the brick returns each visit) and turns its shadow off
#define LSWF_HEI_13_BRICK_MM LSWF(3)
