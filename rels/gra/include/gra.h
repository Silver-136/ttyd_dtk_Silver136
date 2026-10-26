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

// Area-local saved values (GRA; swReInit clears them on every area change), named from the area's
// scripts
// plain brick blk01 in gra_00 (evt_mobj_brick MOBJ_BRICK_PLAIN); mobj_brickblk sets it when broken, evt_mobj_brick
// leaves the brick out while set
#define LSWF_GRA_00_BRICK_BLK01 LSWF(1)
// plain brick blk02 in gra_01 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken
#define LSWF_GRA_01_BRICK_BLK02 LSWF(2)
// plain brick blk03 in gra_01 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken
#define LSWF_GRA_01_BRICK_BLK03 LSWF(3)
// plain brick blk04 in gra_01 (evt_mobj_brick MOBJ_BRICK_PLAIN), set when broken
#define LSWF_GRA_01_BRICK_BLK04 LSWF(4)
