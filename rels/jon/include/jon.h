#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_jon_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_jon_tbl[];
extern BattleUnitKind unit_badge_borodo;
extern BattleUnitKind unit_basabasa_chururu;
extern BattleUnitKind unit_basabasa_green;
extern BattleUnitKind unit_bllizard;
extern BattleUnitKind unit_bomhei;
extern BattleUnitKind unit_borodo;
extern BattleUnitKind unit_boss_zonbaba;
extern BattleUnitKind unit_bubble;
extern BattleUnitKind unit_burst_wanwan;
extern BattleUnitKind unit_chorobon;
extern BattleUnitKind unit_churantalar;
extern BattleUnitKind unit_dark_keeper;
extern BattleUnitKind unit_dokugassun;
extern BattleUnitKind unit_flower_chorobon;
extern BattleUnitKind unit_giant_bomb;
extern BattleUnitKind unit_hannya;
extern BattleUnitKind unit_heavy_bom;
extern BattleUnitKind unit_hennya;
extern BattleUnitKind unit_hinnya;
extern BattleUnitKind unit_honenoko;
extern BattleUnitKind unit_hyper_sinemon;
extern BattleUnitKind unit_ice_pakkun;
extern BattleUnitKind unit_karon;
extern BattleUnitKind unit_mahorn;
extern BattleUnitKind unit_mahorn_custom;
extern BattleUnitKind unit_monochrome_kurokumorn;
extern BattleUnitKind unit_monochrome_sinemon;
extern BattleUnitKind unit_pakkun_flower;
extern BattleUnitKind unit_patamet;
extern BattleUnitKind unit_patatogemet;
extern BattleUnitKind unit_phantom;
extern BattleUnitKind unit_piders;
extern BattleUnitKind unit_purple_teresa;
extern BattleUnitKind unit_sambo;
extern BattleUnitKind unit_sambo_mummy;
extern BattleUnitKind unit_sinemon;
extern BattleUnitKind unit_super_mahorn;
extern BattleUnitKind unit_teresa;
extern BattleUnitKind unit_togenoko_ace;
extern BattleUnitKind unit_twinkling_pansy;
extern BattleUnitKind unit_ura_noko;
extern BattleUnitKind unit_wanwan;
extern BattleUnitKind unit_yami_kuriboo;
extern BattleUnitKind unit_yami_noko;
extern BattleUnitKind unit_yami_pata;
extern BattleUnitKind unit_yami_patakuri;
extern BattleUnitKind unit_yami_togekuri;
extern BattleUnitKind unit_yamitogedaruma;

extern u8 enemy_100[];
extern EvtWord iri_12_makkino_okiru[];
extern EvtWord jon_init_evt[];
extern NpcSetupInfo npcEnt_wanwan[];
USER_FUNC(enemy_info);
USER_FUNC(evt_raster);
USER_FUNC(iri_12_makkino_set_y);
USER_FUNC(iri_12_paper_init);
USER_FUNC(setup_npc_ex_para);
USER_FUNC(shikakeFlag);
USER_FUNC(wait_landon);
USER_FUNC(wanwan_set_home_pos);
USER_FUNC(yattukeFlag);

// Area-local saved values (JON; swReInit clears them on every area change), named from the area's
// scripts
// passed to evt_mobj_signboard for board (setup_return); mobj_signboard never sets or reads its flag
#define LSWF_JON_SIGNBOARD_UNUSED LSWF(0)
// jon_init_evt copies GSW_PIT_FLOOR into it and reads it straight back into LW(0) to update GSW_JON_DEEPEST_LEVEL;
// nothing else uses it
#define LSW_JON_PIT_FLOOR LSW(0)
