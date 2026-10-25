#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_tik_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_tik_tbl[];
extern EvtWord plane_majin_lecture[];
extern EvtWord plane_majin_lecture_init[];
extern EvtWord tik_00_init_evt[];
extern EvtWord tik_01_init_evt[];
extern EvtWord tik_02_init_evt[];
extern EvtWord tik_03_init_evt[];
extern EvtWord tik_04_init_evt[];
extern EvtWord tik_05_init_evt[];
extern EvtWord tik_06_init_evt[];
extern EvtWord tik_07_init_evt[];
extern EvtWord tik_08_init_evt[];
extern EvtWord tik_09_init_evt[];
extern EvtWord tik_10_init_evt[];
extern EvtWord tik_11_init_evt[];
extern EvtWord tik_12_init_evt[];
extern EvtWord tik_13_init_evt[];
extern EvtWord tik_14_init_evt[];
extern EvtWord tik_15_init_evt[];
extern EvtWord tik_16_init_evt[];
extern EvtWord tik_17_init_evt[];
extern EvtWord tik_18_init_evt[];
extern EvtWord tik_19_init_evt[];
extern EvtWord tik_20_init_evt[];
extern EvtWord tik_21_init_evt[];
extern EvtWord tik_villager_urouro[];
extern BattleUnitKind unit_hammer_bros;
extern BattleUnitKind unit_hannya;
extern BattleUnitKind unit_hennya;
extern BattleUnitKind unit_hinnya;
extern BattleUnitKind unit_kamec;
extern BattleUnitKind unit_kuriboo;
extern BattleUnitKind unit_lecture_frankli_kuriboo;
extern BattleUnitKind unit_nokonoko;
extern BattleUnitKind unit_patakuri;
extern BattleUnitKind unit_patapata;
extern BattleUnitKind unit_togekuri;
extern BattleUnitKind unit_togenoko;

extern EvtWord evt_uranai_cap[];
USER_FUNC(evt_tik_villager_urouro_check_func);
