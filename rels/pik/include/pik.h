#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"

extern NpcTerritoryInfo btl_pik_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_pik_tbl[];
extern EvtWord evt_pik_yajirushi[];
extern EvtWord pik_00_init_evt[];
extern EvtWord pik_01_init_evt[];
extern EvtWord pik_02_init_evt[];
extern EvtWord pik_03_init_evt[];
extern EvtWord pik_04_init_evt[];
extern BattleUnitKind unit_purple_teresa;

// Area-local saved values (PIK; swReInit clears them on every area change), named from the area's
// scripts
// talk_powan (pik_02, STG6_53..STG6_54): alternates stg6_pik_22 (0) and stg6_pik_23 (1)
#define LSWF_PIK_02_PENNINGTON_TALK_TOGGLE LSWF(0)
// miyageya_talk (pik_01, STG7_20..STG7_25): the first talk plays the scream kpa_stg7_004 and sets it; later talks
// print kpa_stg7_004_1
#define LSWF_PIK_01_SOUVENIR_NPC_SCREAMED LSWF(1)
