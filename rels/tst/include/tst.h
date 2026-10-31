// Declarations for the developer test REL (tst): __assert2, map init events, unit kinds, drop tables, battle setups.
#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "battle/battle_unit.h"
#include "driver/npcdrv.h"

// system.c. Retail builds strip every assert; tst.rel keeps them, with the original source file name and line.
BOOL __assert2(const char* filename, s32 line, const char* assertion, const char* message, ...);
#define TST_ASSERTMSG(file, line, cond, msg) \
    if (!(cond))                             \
    __assert2(file, line, #cond, msg)

// Map init events registered by tst.c's _prolog (relSetEvtAddr), all in evt_init.c.
extern EvtWord tst_bt00_init_evt[];
extern EvtWord tst_00_init_evt[];
extern EvtWord tst_01_init_evt[];
extern EvtWord tst_02_init_evt[];
extern EvtWord tst_moj_00_init_evt[];
extern EvtWord tst_moj_01_init_evt[];
extern EvtWord tst_moj_02_init_evt[];
extern EvtWord tst_moj_03_init_evt[];
extern EvtWord tst_off_01_init_evt[];
extern EvtWord tst_npc_init_evt[];
extern EvtWord tst_nakayama_init_evt[];
extern EvtWord tst_item_00_init_evt[];
extern EvtWord btl_tst_00_init_evt[];

// Unit kinds the tst battle setups refer to.
extern BattleUnitKind unit_atmic_teresa;
extern BattleUnitKind unit_barriern;
extern BattleUnitKind unit_barriern_z;
extern BattleUnitKind unit_basabasa;
extern BattleUnitKind unit_basabasa_chururu;
extern BattleUnitKind unit_black_heiho;
extern BattleUnitKind unit_bllizard;
extern BattleUnitKind unit_bomhei;
extern BattleUnitKind unit_boomerang_bros;
extern BattleUnitKind unit_borodo;
extern BattleUnitKind unit_bubble;
extern BattleUnitKind unit_chorobon;
extern BattleUnitKind unit_chorobon_gundan;
extern BattleUnitKind unit_command_heiho;
extern BattleUnitKind unit_dancing_heiho;
extern BattleUnitKind unit_dokugassun;
extern BattleUnitKind unit_fire_heiho;
extern BattleUnitKind unit_flower_chorobon;
extern BattleUnitKind unit_gold_chorobon;
extern BattleUnitKind unit_green_chorobon;
extern BattleUnitKind unit_gundan_zako;
extern BattleUnitKind unit_gundan_zako_magician;
extern BattleUnitKind unit_hammer_bros;
extern BattleUnitKind unit_hannya;
extern BattleUnitKind unit_hatty;
extern BattleUnitKind unit_heavy_bom;
extern BattleUnitKind unit_heiho;
extern BattleUnitKind unit_hermos;
extern BattleUnitKind unit_honenoko;
extern BattleUnitKind unit_hyper_kuriboo;
extern BattleUnitKind unit_hyper_patakuri;
extern BattleUnitKind unit_hyper_sinemon;
extern BattleUnitKind unit_hyper_sinnosuke;
extern BattleUnitKind unit_hyper_togekuri;
extern BattleUnitKind unit_ice_pakkun;
extern BattleUnitKind unit_iron_sinemon;
extern BattleUnitKind unit_jyugem;
extern BattleUnitKind unit_kamec;
extern BattleUnitKind unit_karon;
extern BattleUnitKind unit_killer;
extern BattleUnitKind unit_killer_cannon;
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_kurokumorn;
extern BattleUnitKind unit_mahorn;
extern BattleUnitKind unit_mecha_kame;
extern BattleUnitKind unit_mecha_kuri;
extern BattleUnitKind unit_met;
extern BattleUnitKind unit_minarai_kamec;
extern BattleUnitKind unit_nokonoko;
extern BattleUnitKind unit_okorl;
extern BattleUnitKind unit_pakkun_flower;
extern BattleUnitKind unit_pansy;
extern BattleUnitKind unit_patakuri;
extern BattleUnitKind unit_patapata;
extern BattleUnitKind unit_patatogemet;
extern BattleUnitKind unit_poison_pakkun;
extern BattleUnitKind unit_red_honenoko;
extern BattleUnitKind unit_sambo;
extern BattleUnitKind unit_sambo_mummy;
extern BattleUnitKind unit_sinemon;
extern BattleUnitKind unit_sinnosuke;
extern BattleUnitKind unit_super_killer;
extern BattleUnitKind unit_super_killer_cannon;
extern BattleUnitKind unit_super_mahorn;
extern BattleUnitKind unit_teresa;
extern BattleUnitKind unit_test;
extern BattleUnitKind unit_togedaruma;
extern BattleUnitKind unit_togekuri;
extern BattleUnitKind unit_togemet;
extern BattleUnitKind unit_togenoko;
extern BattleUnitKind unit_togezo;
extern BattleUnitKind unit_tuyonarl;
extern BattleUnitKind unit_twinkling_pansy;
extern BattleUnitKind unit_ufo;
extern BattleUnitKind unit_ura_noko;
extern BattleUnitKind unit_ura_pata;
extern BattleUnitKind unit_wanawana;
extern BattleUnitKind unit_wanwan;
extern BattleUnitKind unit_yami_kuriboo;
extern BattleUnitKind unit_yami_noko;
extern BattleUnitKind unit_yami_pata;
extern BattleUnitKind unit_yami_patakuri;
extern BattleUnitKind unit_yami_togekuri;
extern BattleUnitKind unit_yowarl;
extern BattleUnitKind unit_boss_gonbaba;
extern BattleUnitKind unit_barriern_satellite;
extern BattleUnitKind unit_fire_bros;
extern BattleUnitKind unit_iron_sinemon2;
extern BattleUnitKind unit_piders;
extern BattleUnitKind unit_powan;
extern BattleUnitKind unit_kohatty;
extern BattleUnitKind unit_monochrome_sinemon;
extern BattleUnitKind unit_hinnya;
extern BattleUnitKind unit_hennya;
extern BattleUnitKind unit_barriern_z_satellite;
extern BattleUnitKind unit_purple_teresa;
extern BattleUnitKind unit_gundan_zako_elite;
extern BattleUnitKind unit_monban;
extern BattleUnitKind unit_patamet;
extern BattleUnitKind unit_badge_borodo;
extern BattleUnitKind unit_barriern_custom;
extern BattleUnitKind unit_barriern_custom_satellite;
extern BattleUnitKind unit_basabasa_green;
extern BattleUnitKind unit_churantalar;
extern BattleUnitKind unit_dark_keeper;
extern BattleUnitKind unit_giant_bomb;
extern BattleUnitKind unit_hyper_togezo;
extern BattleUnitKind unit_kamec_green;
extern BattleUnitKind unit_kamec_red;
extern BattleUnitKind unit_kamec_white;
extern BattleUnitKind unit_mahorn_custom;
extern BattleUnitKind unit_monochrome_kurokumorn;
extern BattleUnitKind unit_monochrome_pakkun;
extern BattleUnitKind unit_yoshi;
extern BattleUnitKind unit_phantom;
extern BattleUnitKind unit_togenoko_ace;
extern BattleUnitKind unit_yamitogedaruma;
extern BattleUnitKind unit_hyper_jyugem;
extern BattleUnitKind unit_crimson_met;
extern BattleUnitKind unit_crimson_togemet;
extern BattleUnitKind unit_crimson_patamet;
extern BattleUnitKind unit_crimson_patatogemet;
extern BattleUnitKind unit_nokonoko_fighter;
extern BattleUnitKind unit_patapata_fighter;
extern BattleUnitKind unit_borodo_king;
extern BattleUnitKind unit_nancy;
extern BattleUnitKind unit_master;
extern BattleUnitKind unit_reporter;
extern BattleUnitKind unit_hotdog_master;
extern BattleUnitKind unit_marco;

// Test drop tables in the main DOL (battle_database_common.c) used by the tst battle setups.
extern ItemDropData battle_item_sinabita_kinoko[];
extern ItemDropData battle_item_kirakira_otoshi[];
extern ItemDropData battle_item_koorino_ibuki[];
extern ItemDropData battle_item_fire_flower[];
extern ItemDropData battle_item_yurayura_jishin[];
extern ItemDropData battle_item_suitooru[];
extern ItemDropData battle_item_dekadeka_drink[];
extern ItemDropData battle_item_pow_block[];
extern ItemDropData battle_item_funyafunya_kun[];
extern ItemDropData battle_item_kinoko[];

// Battle setups in battle_bgset_test.c, registered by tst.c's _prolog (relSetBtlAddr).
extern NpcSetupBattleInfo btlsetup_tst_tbl[];
extern NpcTerritoryInfo btl_tst_setup_no_tbl[];

// tst.c: map user function that turns the Koopa mobjs on (mobjKoopaOn), called from evt_init.c's events.
s32 tst_mobj_koopa_on(struct EventEntry* event, BOOL isFirstCall);
