#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_dou_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_dou_tbl[];
extern EvtWord dou_00_init_evt[];
extern EvtWord dou_01_init_evt[];
extern EvtWord dou_02_init_evt[];
extern EvtWord dou_03_init_evt[];
extern EvtWord dou_04_init_evt[];
extern EvtWord dou_05_init_evt[];
extern EvtWord dou_06_init_evt[];
extern EvtWord dou_07_init_evt[];
extern EvtWord dou_08_init_evt[];
extern EvtWord dou_09_init_evt[];
extern EvtWord dou_10_init_evt[];
extern EvtWord dou_11_init_evt[];
extern EvtWord dou_12_init_evt[];
extern EvtWord dou_13_init_evt[];
extern EvtWord dou_normal_puka[];
extern EvtWord dou_puka_event[];
extern EvtWord ship_majin_lecture[];
extern EvtWord ship_majin_lecture_init[];
extern BattleUnitKind unit_bubble;
extern BattleUnitKind unit_heavy_bom;
extern BattleUnitKind unit_hermos;
extern BattleUnitKind unit_killer;
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_patamet;
USER_FUNC(dou_mario_pos_osreport_func);
USER_FUNC(evt_dou_normal_puka);
