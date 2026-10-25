#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_nok_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_nok_tbl[];
extern EvtWord nok_00_init_evt[];
extern EvtWord nok_01_init_evt[];
USER_FUNC(evt_nokonoko_urouro_check_func);
