#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_jin_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_jin_tbl[];
extern EvtWord evt_usu_kagemario_party_kill[];
extern EvtWord jin_00_init_evt[];
extern EvtWord jin_01_init_evt[];
extern EvtWord jin_02_init_evt[];
extern EvtWord jin_03_init_evt[];
extern EvtWord jin_04_init_evt[];
extern EvtWord jin_05_init_evt[];
extern EvtWord jin_06_init_evt[];
extern EvtWord jin_07_init_evt[];
extern EvtWord jin_08_init_evt[];
extern EvtWord jin_09_init_evt[];
extern EvtWord jin_10_init_evt[];
extern EvtWord jin_11_init_evt[];
extern BattleUnitKind unit_atmic_teresa;
extern BattleUnitKind unit_basabasa;
extern BattleUnitKind unit_boss_rampell;
extern BattleUnitKind unit_met;
extern BattleUnitKind unit_teresa;
extern BattleUnitKind unit_togemet;
USER_FUNC(evt_jin_vivian_pose_set);

// Area-local saved values (JIN; swReInit clears them on every area change), named from the area's
// scripts
// set by oumu_juyo_talk (jin_08) when the parrot's secret is first heard (with GSWF_JIN_08_PARROT_SECRET_HEARD);
// jin_07_juyo_oumu_vivian then plays the partner lines stg4_jin_46/47 once and clears it
#define LSWF_JIN_PARROT_SCENE_PENDING LSWF(1)
// Golden Leaf from jin_01's golden tree (item_ki01, evt_item_entry); jin_01_init_evt clears it on each load, so the
// leaf returns every visit
#define LSWF_JIN_01_GOLDEN_LEAF_TREE LSWF(2)
// which jin_03 staircase is in place: 0 = middle (start), 1 = front, other (255 after 0 - 1) = back; the jin_02
// switch (evt_kaidan_temae) adds 1 unless it is 1, the jin_01 switch (evt_kaidan_oku) subtracts 1 while it is 0 or 1;
// jin_03_init_evt removes the other two
#define LSW_JIN_STAIRCASE_POS LSW(1)
// oumu_talk (jin_08, before STG4_15_03): 0, 1, 2 pick the parrot's lines stg4_jin_40/41/42; counts up to 2 and stays
// there
#define LSW_JIN_08_PARROT_TALK_COUNT LSW(2)
