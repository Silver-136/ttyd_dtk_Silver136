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
