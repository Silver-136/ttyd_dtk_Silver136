#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_gra_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_gra_tbl[];
extern EvtWord evt_usu_kagemario_party_kill[];
extern EvtWord gra_00_init_evt[];
extern EvtWord gra_01_init_evt[];
extern EvtWord gra_02_init_evt[];
extern EvtWord gra_03_init_evt[];
extern EvtWord gra_04_init_evt[];
extern EvtWord gra_05_init_evt[];
extern EvtWord gra_06_init_evt[];
extern BattleUnitKind unit_faker_mario_gra;
extern BattleUnitKind unit_hyper_kuriboo;
extern BattleUnitKind unit_hyper_patakuri;
extern BattleUnitKind unit_hyper_sinemon;
extern BattleUnitKind unit_hyper_togekuri;
extern BattleUnitKind unit_pansy;
extern BattleUnitKind unit_twinkling_pansy;
USER_FUNC(evt_gra_vivian_pose_set);
