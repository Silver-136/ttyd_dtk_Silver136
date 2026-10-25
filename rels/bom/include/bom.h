#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord bom_00_init_evt[];
extern EvtWord bom_01_init_evt[];
extern EvtWord bom_02_init_evt[];
extern EvtWord bom_03_init_evt[];
extern EvtWord bom_04_init_evt[];
extern NpcTerritoryInfo btl_bom_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_bom_tbl[];
extern EvtWord evt_bom_01_karakara[];
extern EvtWord evt_bom_snowfall[];
extern BattleUnitKind unit_bllizard;
extern BattleUnitKind unit_ice_pakkun;
extern BattleUnitKind unit_kuriboo;

extern EvtWord evt_bress[];
USER_FUNC(bress);
