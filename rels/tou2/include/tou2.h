#pragma once

#include "dolphin/types.h"
#include "evt/evt_cmd.h"
#include "driver/npcdrv.h"

extern NpcTerritoryInfo btl_tou_setup_no_tbl[];
USER_FUNC(evt_tou_get_fighter_tribe);
extern EvtWord tou_03_init_evt[];
extern BattleUnitKind unit_basabasa;
extern BattleUnitKind unit_bomhei;
extern BattleUnitKind unit_boomerang_bros;
extern BattleUnitKind unit_borodo;
extern BattleUnitKind unit_borodo_king;
extern BattleUnitKind unit_boss_champion;
extern BattleUnitKind unit_boss_koopa_tou;
extern BattleUnitKind unit_boss_macho_gance;
extern BattleUnitKind unit_burst_wanwan;
extern BattleUnitKind unit_chorobon;
extern BattleUnitKind unit_crimson_togemet;
extern BattleUnitKind unit_dark_keeper;
extern BattleUnitKind unit_fire_bros;
extern BattleUnitKind unit_flower_chorobon;
extern BattleUnitKind unit_green_chorobon;
extern BattleUnitKind unit_hammer_bros;
extern BattleUnitKind unit_hannya;
extern BattleUnitKind unit_hinnya;
extern BattleUnitKind unit_honenoko;
extern BattleUnitKind unit_hyper_sinnosuke;
extern BattleUnitKind unit_hyper_togezo;
extern BattleUnitKind unit_iron_sinemon;
extern BattleUnitKind unit_iron_sinemon2;
extern BattleUnitKind unit_kamec_green;
extern BattleUnitKind unit_kamec_red;
extern BattleUnitKind unit_kamec_white;
extern BattleUnitKind unit_kurikuri;
extern BattleUnitKind unit_monochrome_kurokumorn;
extern BattleUnitKind unit_monochrome_pakkun;
extern BattleUnitKind unit_nokonoko_fighter;
extern BattleUnitKind unit_patapata_fighter;
extern BattleUnitKind unit_piders;
extern BattleUnitKind unit_sambo;
extern BattleUnitKind unit_togedaruma;
extern BattleUnitKind unit_togenoko_ace;
extern BattleUnitKind unit_togezo;
extern BattleUnitKind unit_ura_noko;
extern BattleUnitKind unit_ura_pata;
USER_FUNC(evt_tou_get_fighter_num);
USER_FUNC(tou2_rankingReset);
