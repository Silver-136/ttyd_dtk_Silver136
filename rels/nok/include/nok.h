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

// Area-local saved values (NOK; swReInit clears them on every area change), named from the area's
// scripts
// set by kizayarou_talk (nok_00) after the long exchange with Goombella (stg1_nok_10-13); while set he gives the
// stg1_nok_14 talk instead
#define LSWF_NOK_00_KIZAYAROU_TALKED LSWF(1)
// cleared by nok_00_koopa_evt before kpa_stg2_013; the callback nok_stg2_013_cb sets it once the text passes a point,
// starting a waiting inline event
#define LSWF_NOK_00_KOOPA_MSG_CUE LSWF(2)
// Turtley Leaf in one of nok_01's bushes (item_kusa_4/5/6, chosen by LSW(1)); nok_01_init_evt clears it on each load
// (sets it in E3 mode), so the leaf returns every visit
#define LSWF_NOK_01_TURTLEY_LEAF LSWF(4)
// nok_01_init_evt picks it at random (0-2) on each load: which bush hides the Turtley Leaf (0 = item_kusa_4, 1 =
// item_kusa_5, 2 = item_kusa_6)
#define LSW_NOK_01_LEAF_BUSH LSW(1)
