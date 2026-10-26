#pragma once

#include "dolphin/types.h"
#include "evt/evt_cmd.h"
#include "driver/npcdrv.h"
#include "data_tables.h"

struct ZakoGun;

extern EvtWord all_party_lecture[];
extern EvtWord anm_entotsu[];
extern EvtWord anm_kaji[];
extern EvtWord anm_neji[];
extern EvtWord anm_wakka[];
extern NpcTerritoryInfo btl_muj_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_muj_tbl[];
extern EvtWord korutesu_apper[];
extern EvtWord korutesu_vanish[];
extern EvtWord muj_00_init_evt[];
extern EvtWord muj_01_init_evt[];
extern EvtWord muj_02_init_evt[];
extern EvtWord muj_03_init_evt[];
extern EvtWord muj_04_init_evt[];
extern EvtWord muj_05_init_evt[];
extern EvtWord muj_10_init_evt[];
extern EvtWord muj_11_init_evt[];
extern EvtWord muj_12_init_evt[];
extern EvtWord muj_20_init_evt[];
extern EvtWord muj_21_init_evt[];
USER_FUNC(muj_npc_guruguru);
extern EvtWord muj_ship_puka[];
extern BattleUnitKind unit_boss_gundan_zako_group1;
extern BattleUnitKind unit_flower_chorobon;
extern BattleUnitKind unit_green_chorobon;
extern BattleUnitKind unit_hermos;
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_poison_pakkun;

extern EvtWord korutesu_appear_event[];
extern TreeAccess tree_tbl_treeb_evt[];
USER_FUNC(muj_mario_pos_osreport_func);
USER_FUNC(muj_npc_buruburu);
USER_FUNC(muj_npc_fuwafuwa);
USER_FUNC(muj_ship_puka_func);
s32 zakogun_get_kanbu_id(void);
BattleUnit* zakogun_get_ptr(void);
void zakogun_ext_main3(void);
void zakogun_ext_main2(void);
void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i);
void zakogun_set_pose(struct ZakoGun* zako, s32 base, s32 num, s32 div);

// Area-local saved values (MUJ; swReInit clears them on every area change), named from the area's
// scripts
// koburon_regp (muj_00): 1 while Whacka has popped up because Mario is within 100 units, 0 while it is hidden;
// koburon_init clears it
#define LSWF_MUJ_00_WHACKA_OUT LSWF(1)
// set by koburon_dead each time Whacka is hit (with GSW_MUJ_WHACKA_HIT_COUNT); no code reads it
#define LSWF_MUJ_00_WHACKA_HIT_UNUSED LSWF(2)
// US only: plain brick bblk00 in muj_03 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken; JP uses GSWF_MUJ_03_BRICK
#define LSWF_MUJ_03_BRICK_BBLK00 LSWF(3)
