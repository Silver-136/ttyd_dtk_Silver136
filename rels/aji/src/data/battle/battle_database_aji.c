// X-Naut Fortress (aji): battle database: the enemy groups (BattleSetup) and stages of the fortress and
// of the Magnus von Grapple 2.0 boss fight.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "aji.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

static USER_FUNC(_space_rotate);
extern BattleUnitKind unit_barriern;
extern BattleUnitKind unit_barriern_custom;
extern BattleUnitKind unit_barriern_z;

BattleStageObjectData aji_btlobject_data[27];
BattleStageData aji_btlstage_data[7];
BattleUnitSetup btlparty_aji_aji_mbmkII[1];
BattleGroupSetup btlgrp_aji_aji_mbmkII;
BattleUnitSetup btlparty_aji_aji_zako[2];
BattleGroupSetup btlgrp_aji_aji_zako;
BattleUnitSetup btlparty_aji_aji_01_01_off_1[2];
BattleGroupSetup btlgrp_aji_aji_01_01_off_1;
BattleUnitSetup btlparty_aji_aji_01_02_off_1[3];
BattleGroupSetup btlgrp_aji_aji_01_02_off_1;
BattleUnitSetup btlparty_aji_aji_01_03_off_1[4];
BattleGroupSetup btlgrp_aji_aji_01_03_off_1;
BattleUnitSetup btlparty_aji_aji_01_04_off_1[2];
BattleGroupSetup btlgrp_aji_aji_01_04_off_1;
BattleUnitSetup btlparty_aji_aji_01_05_off_1[2];
BattleGroupSetup btlgrp_aji_aji_01_05_off_1;
BattleUnitSetup btlparty_aji_aji_01_06_off_1[4];
BattleGroupSetup btlgrp_aji_aji_01_06_off_1;
BattleUnitSetup btlparty_aji_aji_01_07_off_1[3];
BattleGroupSetup btlgrp_aji_aji_01_07_off_1;
BattleUnitSetup btlparty_aji_aji_01_08_off_1[4];
BattleGroupSetup btlgrp_aji_aji_01_08_off_1;
BattleUnitSetup btlparty_aji_aji_04_01_off_1[2];
BattleGroupSetup btlgrp_aji_aji_04_01_off_1;
BattleUnitSetup btlparty_aji_aji_06_01_off_1[2];
BattleGroupSetup btlgrp_aji_aji_06_01_off_1;
BattleUnitSetup btlparty_aji_aji_08_01_off_1[3];
BattleGroupSetup btlgrp_aji_aji_08_01_off_1;
BattleUnitSetup btlparty_aji_aji_09_01_off_1[5];
BattleGroupSetup btlgrp_aji_aji_09_01_off_1;
BattleUnitSetup btlparty_aji_aji_10_01_off_1[1];
BattleGroupSetup btlgrp_aji_aji_10_01_off_1;
BattleUnitSetup btlparty_aji_aji_13_01_off_1[4];
BattleGroupSetup btlgrp_aji_aji_13_01_off_1;
BattleUnitSetup btlparty_aji_aji_13_02_off_1[2];
BattleGroupSetup btlgrp_aji_aji_13_02_off_1;
BattleUnitSetup btlparty_aji_aji_15_01_off_1[1];
BattleGroupSetup btlgrp_aji_aji_15_01_off_1;
NpcBattleSelect btlgrpsel_aji_btlno_aji_test1_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_test2_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_test3_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_test4_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_test5_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_mbmkii_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_00_00_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_02_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_03_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_04_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_05_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_06_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_07_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_08_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_04_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_06_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_08_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_09_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_10_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_13_01_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_13_02_off[2];
NpcBattleSelect btlgrpsel_aji_btlno_aji_15_01_off[2];
NpcSetupBattleInfo btlsetup_aji_tbl[24];
NpcTerritoryInfo btl_aji_setup_no_tbl[24];
EvtWord bgset_aji_space_event[35];

/**
 * Scenery objects of the fortress battle arenas (electric pylons, backdrops and floor pieces), shared by the
 * stages below.
 */
BattleStageObjectData aji_btlobject_data[] = {
    {"dentou_01", 2, STAGE_LAYER_A1, {144.0f, 0.0f, -54.0f}, 0, 60, {0}},
    {"dentou_02", 2, STAGE_LAYER_A1, {64.0f, 0.0f, -47.0f}, 10, 50, {0}},
    {"dentou_03", 2, STAGE_LAYER_A1, {-64.0f, 0.0f, -47.0f}, 20, 40, {0}},
    {"dentou_04", 2, STAGE_LAYER_A1, {-144.0f, 0.0f, -54.0f}, 30, 30, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"haikei_C_a", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"A2", 2, STAGE_LAYER_A2, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"haikei_C_b", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"A2", 2, STAGE_LAYER_A2, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"haikei_C_c", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"A2", 2, STAGE_LAYER_A2, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"A1", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"A1", 2, STAGE_LAYER_A1, {0.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"c_ten", 2, STAGE_LAYER_C, {0.0f, 0.0f, 16.0f}, 0, 60, {0}},
    {"c_kabe", 2, STAGE_LAYER_C, {0.0f, 0.0f, 16.0f}, 0, 60, {0}},
    {"c_o", 2, STAGE_LAYER_C, {0.0f, 0.0f, 7.0f}, 0, 60, {0}},
    {"space", 2, STAGE_LAYER_C, {0.0f, 0.0f, 7.0f}, 0, 60, {0}},
    {"star_01", 2, STAGE_LAYER_C, {0.0f, 0.0f, 39.0f}, 0, 60, {0}},
    {"star_02", 2, STAGE_LAYER_C, {0.0f, 0.0f, 25.0f}, 0, 60, {0}},
    {"star_03", 2, STAGE_LAYER_C, {0.0f, 0.0f, 31.0f}, 0, 60, {0}},
};

/** The seven battle arenas of the X-Naut Fortress, each with its model folders and the objects it uses. */
BattleStageData aji_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_07_5", // specificStageDataDir
        5, // numObjects
        aji_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_1", // specificStageDataDir
        3, // numObjects
        &aji_btlobject_data[5], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_1", // specificStageDataDir
        3, // numObjects
        &aji_btlobject_data[8], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_1", // specificStageDataDir
        3, // numObjects
        &aji_btlobject_data[11], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_2", // specificStageDataDir
        3, // numObjects
        &aji_btlobject_data[17], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_3", // specificStageDataDir
        3, // numObjects
        &aji_btlobject_data[14], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_07_6", // specificStageDataDir
        7, // numObjects
        &aji_btlobject_data[20], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_aji_space_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

/** Enemy of the boss fight in the boss room: Magnus von Grapple 2.0. */
BattleUnitSetup btlparty_aji_aji_mbmkII[] = {
    {
        &unit_boss_magnum_battender_mkII, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

/** Fight group for the Magnus von Grapple 2.0 boss fight, with its drop weights. */
BattleGroupSetup btlgrp_aji_aji_mbmkII = {
    1, // numEnemies
    btlparty_aji_aji_mbmkII, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemies of the generic fortress group, used by the debug battle entries: 2 Elite X-Naut. */
BattleUnitSetup btlparty_aji_aji_zako[] = {
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

/**
 * Fight group for the generic fortress group: its enemy party with the item, heart and flower drop weights.
 */
BattleGroupSetup btlgrp_aji_aji_zako = {
    2, // numEnemies
    btlparty_aji_aji_zako, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_01_off_1[] = {
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_01_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_02_off_1[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_02_off_1 = {
    3, // numEnemies
    btlparty_aji_aji_01_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_03_off_1[] = {
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_03_off_1 = {
    4, // numEnemies
    btlparty_aji_aji_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_04_off_1[] = {
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_04_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_01_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_05_off_1[] = {
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_05_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_01_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_06_off_1[] = {
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_06_off_1 = {
    4, // numEnemies
    btlparty_aji_aji_01_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_07_off_1[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {50.0f, 0.0f, -10.0f},
#else
        {30.0f, 0.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {90.0f, 40.0f, 0.0f},
#else
        {70.0f, 40.0f, 0.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {130.0f, 40.0f, 10.0f},
#else
        {150.0f, 40.0f, 20.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_07_off_1 = {
    3, // numEnemies
    btlparty_aji_aji_01_07_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_01_08_off_1[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_01_08_off_1 = {
    4, // numEnemies
    btlparty_aji_aji_01_08_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_04_01_off_1[] = {
    {
        &unit_barriern_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {70.0f, 40.0f, 0.0f},
#else
        {50.0f, 40.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_custom, // itemDropTable
    },
    {
        &unit_barriern_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {110.0f, 40.0f, 10.0f},
#else
        {130.0f, 40.0f, 10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_custom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_04_01_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_04_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_06_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_06_01_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_06_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_08_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {50.0f, 0.0f, -10.0f},
#else
        {30.0f, 0.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {90.0f, 40.0f, 0.0f},
#else
        {70.0f, 40.0f, 0.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {130.0f, 40.0f, 10.0f},
#else
        {150.0f, 40.0f, 20.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_08_01_off_1 = {
    3, // numEnemies
    btlparty_aji_aji_08_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_09_01_off_1[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_09_01_off_1 = {
    5, // numEnemies
    btlparty_aji_aji_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_10_01_off_1[] = {
    {
        &unit_barriern_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_custom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_10_01_off_1 = {
    1, // numEnemies
    btlparty_aji_aji_10_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_13_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_13_01_off_1 = {
    4, // numEnemies
    btlparty_aji_aji_13_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_13_02_off_1[] = {
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {70.0f, 40.0f, 0.0f},
#else
        {50.0f, 40.0f, -10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {110.0f, 40.0f, 10.0f},
#else
        {130.0f, 40.0f, 10.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_13_02_off_1 = {
    2, // numEnemies
    btlparty_aji_aji_13_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_aji_aji_15_01_off_1[] = {
    {
        &unit_gundan_zako_elite, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_15_01_off_1 = {
    1, // numEnemies
    btlparty_aji_aji_15_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Debug battle entry 1: fights the generic fortress group. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_test1_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[1]},
    {0},
};

/** Debug battle entry 2: fights the generic fortress group. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_test2_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[4]},
    {0},
};

/** Debug battle entry 3: fights the generic fortress group. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_test3_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[5]},
    {0},
};

/** Debug battle entry 4: fights the generic fortress group. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_test4_off[] = {
    {50, &btlgrp_aji_aji_zako, aji_btlstage_data},
    {0},
};

/** Debug battle entry 5: fights the generic fortress group. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_test5_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[6]},
    {0},
};

/** Selects the boss fight against Magnus von Grapple 2.0 and its arena. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_mbmkii_off[] = {
    {50, &btlgrp_aji_aji_mbmkII, &aji_btlstage_data[5]},
    {0},
};

/** Selects the generic fortress group for the elite X-Naut encounters. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_00_00_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[6]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_01_off[] = {
    {20, &btlgrp_aji_aji_01_01_off_1, &aji_btlstage_data[6]},
    {0},
};

/** Selects the group and arena for fight 2 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_02_off[] = {
    {20, &btlgrp_aji_aji_01_02_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 3 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_03_off[] = {
    {20, &btlgrp_aji_aji_01_03_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 4 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_04_off[] = {
    {20, &btlgrp_aji_aji_01_04_off_1, &aji_btlstage_data[2]},
    {0},
};

/** Selects the group and arena for fight 5 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_05_off[] = {
    {20, &btlgrp_aji_aji_01_05_off_1, &aji_btlstage_data[2]},
    {0},
};

/** Selects the group and arena for fight 6 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_06_off[] = {
    {20, &btlgrp_aji_aji_01_06_off_1, &aji_btlstage_data[2]},
    {0},
};

/** Selects the group and arena for fight 7 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_07_off[] = {
    {20, &btlgrp_aji_aji_01_07_off_1, &aji_btlstage_data[3]},
    {0},
};

/** Selects the group and arena for fight 8 in aji_01. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_01_08_off[] = {
    {20, &btlgrp_aji_aji_01_08_off_1, &aji_btlstage_data[3]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_04. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_04_01_off[] = {
    {20, &btlgrp_aji_aji_04_01_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_06. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_06_01_off[] = {
    {20, &btlgrp_aji_aji_06_01_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_08. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_08_01_off[] = {
    {20, &btlgrp_aji_aji_08_01_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_09. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_09_01_off[] = {
    {20, &btlgrp_aji_aji_09_01_off_1, &aji_btlstage_data[1]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_10. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_10_01_off[] = {
    {20, &btlgrp_aji_aji_10_01_off_1, aji_btlstage_data},
    {0},
};

/** Selects the group and arena for fight 1 in aji_13. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_13_01_off[] = {
    {20, &btlgrp_aji_aji_13_01_off_1, &aji_btlstage_data[4]},
    {0},
};

/** Selects the group and arena for fight 2 in aji_13. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_13_02_off[] = {
    {20, &btlgrp_aji_aji_13_02_off_1, &aji_btlstage_data[4]},
    {0},
};

/** Selects the group and arena for fight 1 in aji_15. */
NpcBattleSelect btlgrpsel_aji_btlno_aji_15_01_off[] = {
    {20, &btlgrp_aji_aji_15_01_off_1, &aji_btlstage_data[1]},
    {0},
};

/** The battle setups of the fortress: one entry per fight with its name, group selector and battle music. */
NpcSetupBattleInfo btlsetup_aji_tbl[] = {
    {
        "マップstg_07_1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_07_2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_07_3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_07_5", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test4_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_07_6", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test5_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マグナムバッテンダー幕痛", // name
        "aji_14", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_mbmkii_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        6, // randomMode
        {{0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG3_3RDLEADER1", // bgmName
    },
    {
        "軍団ザコエリート", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_00_00_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコエリート", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコエリート", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコ魔法使い", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01バリアーンＺ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコエリート", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_06_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_07_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji01軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_08_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji04バリアーンカスタム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_04_01_off, // select
        NPC_BATTLE_NO_ESCAPE, // battleFlags
        0, // randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji06軍団ザコ魔法使い", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_06_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji08軍団ザコ魔法使い", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_08_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji09軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_09_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji10バリアーンカスタム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_10_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji13軍団ザコ魔法使い", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_13_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji13バリアーンＺ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_13_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "aji15軍団ザコエリート", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_15_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_aji_setup_no_tbl[] = {
    {"BTLNO_AJI_TEST1", 0},
    {"BTLNO_AJI_TEST2", 1},
    {"BTLNO_AJI_TEST3", 2},
    {"BTLNO_AJI_TEST4", 3},
    {"BTLNO_AJI_TEST5", 4},
    {"BTLNO_AJI_MBMKII", 5},
    {"BTLNO_AJI_00_00", 6},
    {"BTLNO_AJI_01_01", 7},
    {"BTLNO_AJI_01_02", 8},
    {"BTLNO_AJI_01_03", 9},
    {"BTLNO_AJI_01_04", 10},
    {"BTLNO_AJI_01_05", 11},
    {"BTLNO_AJI_01_06", 12},
    {"BTLNO_AJI_01_07", 13},
    {"BTLNO_AJI_01_08", 14},
    {"BTLNO_AJI_04_01", 15},
    {"BTLNO_AJI_06_01", 16},
    {"BTLNO_AJI_08_01", 17},
    {"BTLNO_AJI_09_01", 18},
    {"BTLNO_AJI_10_01", 19},
    {"BTLNO_AJI_13_01", 20},
    {"BTLNO_AJI_13_02", 21},
    {"BTLNO_AJI_15_01", 22},
    {0},
};

EVT_BEGIN(bgset_aji_space_event)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 0)
    LABEL(0)

    // Each star keeps an independent frame counter and revolution period.
    CALL(_space_rotate, STRING("star_01"), LW(1), 7500)
    CALL(_space_rotate, STRING("star_02"), LW(2), 6750)
    CALL(_space_rotate, STRING("star_03"), LW(3), 6000)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Script function: advances the step counter in the variable args[1] by one (wrapping at args[2])
 * and turns the battle stage object named args[0] to 360 degrees times step over total around Z. Used for the
 * rotating star backdrops.
 */
static USER_FUNC(_space_rotate) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 outVar = args[1];
    s32 step = evtGetValue(event, outVar);
    s32 total = evtGetValue(event, args[2]);
    BattleStageObject* obj = BattleSearchObjectPtr(name);

    // Draw the current phase before advancing and wrapping the counter.
    obj->rotation.z = 360.0f * step / total;
    step++;
    if (step >= total) {
        step = 0;
    }
    evtSetValue(event, outVar, step);
    return EVT_RETURN_DONE;
}
