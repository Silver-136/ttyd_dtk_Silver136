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

BattleStageObjectData aji_btlobject_data[] = {
    {"dentou_01", 2, 0, {144.0f, 0.0f, -54.0f}, 0, 60, {0}},
    {"dentou_02", 2, 0, {64.0f, 0.0f, -47.0f}, 10, 50, {0}},
    {"dentou_03", 2, 0, {-64.0f, 0.0f, -47.0f}, 20, 40, {0}},
    {"dentou_04", 2, 0, {-144.0f, 0.0f, -54.0f}, 30, 30, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"haikei_C_a", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"haikei_C_b", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"haikei_C_c", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"A1", 2, 0, {0.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"A1", 2, 0, {0.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"c_ten", 2, 3, {0.0f, 0.0f, 16.0f}, 0, 60, {0}},
    {"c_kabe", 2, 3, {0.0f, 0.0f, 16.0f}, 0, 60, {0}},
    {"c_o", 2, 3, {0.0f, 0.0f, 7.0f}, 0, 60, {0}},
    {"space", 2, 3, {0.0f, 0.0f, 7.0f}, 0, 60, {0}},
    {"star_01", 2, 3, {0.0f, 0.0f, 39.0f}, 0, 60, {0}},
    {"star_02", 2, 3, {0.0f, 0.0f, 25.0f}, 0, 60, {0}},
    {"star_03", 2, 3, {0.0f, 0.0f, 31.0f}, 0, 60, {0}},
};

BattleStageData aji_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_5", // mSpecificStageDataDir
        5, // mNumObjects
        aji_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_1", // mSpecificStageDataDir
        3, // mNumObjects
        &aji_btlobject_data[5], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_1", // mSpecificStageDataDir
        3, // mNumObjects
        &aji_btlobject_data[8], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_1", // mSpecificStageDataDir
        3, // mNumObjects
        &aji_btlobject_data[11], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_2", // mSpecificStageDataDir
        3, // mNumObjects
        &aji_btlobject_data[17], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_3", // mSpecificStageDataDir
        3, // mNumObjects
        &aji_btlobject_data[14], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_6", // mSpecificStageDataDir
        7, // mNumObjects
        &aji_btlobject_data[20], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_aji_space_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_aji_aji_mbmkII[] = {
    {
        &unit_boss_magnum_battender_mkII, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_mbmkII = {
    1, // numEnemies
    btlparty_aji_aji_mbmkII, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_zako[] = {
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
};

BattleGroupSetup btlgrp_aji_aji_zako = {
    2, // numEnemies
    btlparty_aji_aji_zako, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_01_off_1[] = {
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_02_off_1[] = {
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_03_off_1[] = {
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_04_off_1[] = {
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_05_off_1[] = {
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_06_off_1[] = {
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_elite, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_07_off_1[] = {
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_01_08_off_1[] = {
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_04_01_off_1[] = {
    {
        &unit_barriern_custom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_custom, // itemDropTable
    },
    {
        &unit_barriern_custom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_06_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_08_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_barriern, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_09_01_off_1[] = {
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_10_01_off_1[] = {
    {
        &unit_barriern_custom, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_13_01_off_1[] = {
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako_magician, // itemDropTable
    },
    {
        &unit_gundan_zako_magician, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_13_02_off_1[] = {
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
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
    5, // field_1C
};

BattleUnitSetup btlparty_aji_aji_15_01_off_1[] = {
    {
        &unit_gundan_zako_elite, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
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
    5, // field_1C
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_test1_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_test2_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_test3_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_test4_off[] = {
    {50, &btlgrp_aji_aji_zako, aji_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_test5_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_mbmkii_off[] = {
    {50, &btlgrp_aji_aji_mbmkII, &aji_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_00_00_off[] = {
    {50, &btlgrp_aji_aji_zako, &aji_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_01_off[] = {
    {20, &btlgrp_aji_aji_01_01_off_1, &aji_btlstage_data[6]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_02_off[] = {
    {20, &btlgrp_aji_aji_01_02_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_03_off[] = {
    {20, &btlgrp_aji_aji_01_03_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_04_off[] = {
    {20, &btlgrp_aji_aji_01_04_off_1, &aji_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_05_off[] = {
    {20, &btlgrp_aji_aji_01_05_off_1, &aji_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_06_off[] = {
    {20, &btlgrp_aji_aji_01_06_off_1, &aji_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_07_off[] = {
    {20, &btlgrp_aji_aji_01_07_off_1, &aji_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_01_08_off[] = {
    {20, &btlgrp_aji_aji_01_08_off_1, &aji_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_04_01_off[] = {
    {20, &btlgrp_aji_aji_04_01_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_06_01_off[] = {
    {20, &btlgrp_aji_aji_06_01_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_08_01_off[] = {
    {20, &btlgrp_aji_aji_08_01_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_09_01_off[] = {
    {20, &btlgrp_aji_aji_09_01_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_10_01_off[] = {
    {20, &btlgrp_aji_aji_10_01_off_1, aji_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_13_01_off[] = {
    {20, &btlgrp_aji_aji_13_01_off_1, &aji_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_13_02_off[] = {
    {20, &btlgrp_aji_aji_13_02_off_1, &aji_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_aji_btlno_aji_15_01_off[] = {
    {20, &btlgrp_aji_aji_15_01_off_1, &aji_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_aji_tbl[] = {
    {
        /*"マップstg_07_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_07_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_07_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_07_5"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test4_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_07_6"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x36", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_test5_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マグナムバッテンダー幕痛"*/"\x83\x7D\x83\x4F\x83\x69\x83\x80\x83\x6F\x83\x62\x83\x65\x83\x93\x83\x5F\x81\x5B\x96\x8B\x92\xC9", // name
        "aji_14", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_mbmkii_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_3RDLEADER1", // bgmName
    },
    {
        /*"軍団ザコエリート"*/"\x8C\x52\x92\x63\x83\x55\x83\x52\x83\x47\x83\x8A\x81\x5B\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_00_00_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコエリート"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52\x83\x47\x83\x8A\x81\x5B\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコ"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコエリート"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52\x83\x47\x83\x8A\x81\x5B\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコ魔法使い"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52\x96\x82\x96\x40\x8E\x67\x82\xA2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01バリアーンＺ"*/"\x61\x6A\x69\x30\x31\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x82\x79", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコエリート"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52\x83\x47\x83\x8A\x81\x5B\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_06_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコ"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_07_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji01軍団ザコ"*/"\x61\x6A\x69\x30\x31\x8C\x52\x92\x63\x83\x55\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_01_08_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji04バリアーンカスタム"*/"\x61\x6A\x69\x30\x34\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x83\x4A\x83\x58\x83\x5E\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_04_01_off, // select
        0x10, // battleFlags
        0, // field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji06軍団ザコ魔法使い"*/"\x61\x6A\x69\x30\x36\x8C\x52\x92\x63\x83\x55\x83\x52\x96\x82\x96\x40\x8E\x67\x82\xA2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_06_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji08軍団ザコ魔法使い"*/"\x61\x6A\x69\x30\x38\x8C\x52\x92\x63\x83\x55\x83\x52\x96\x82\x96\x40\x8E\x67\x82\xA2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_08_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji09軍団ザコ"*/"\x61\x6A\x69\x30\x39\x8C\x52\x92\x63\x83\x55\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji10バリアーンカスタム"*/"\x61\x6A\x69\x31\x30\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x83\x4A\x83\x58\x83\x5E\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_10_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji13軍団ザコ魔法使い"*/"\x61\x6A\x69\x31\x33\x8C\x52\x92\x63\x83\x55\x83\x52\x96\x82\x96\x40\x8E\x67\x82\xA2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_13_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji13バリアーンＺ"*/"\x61\x6A\x69\x31\x33\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x82\x79", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_13_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"aji15軍団ザコエリート"*/"\x61\x6A\x69\x31\x35\x8C\x52\x92\x63\x83\x55\x83\x52\x83\x47\x83\x8A\x81\x5B\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_aji_btlno_aji_15_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
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
    CALL(_space_rotate, STRING("star_01"), LW(1), 7500)
    CALL(_space_rotate, STRING("star_02"), LW(2), 6750)
    CALL(_space_rotate, STRING("star_03"), LW(3), 6000)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

static USER_FUNC(_space_rotate) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 outVar = args[1];
    s32 step = evtGetValue(event, outVar);
    s32 total = evtGetValue(event, args[2]);
    BattleStageObject* obj = BattleSearchObjectPtr(name);

    obj->mRotation.z = 360.0f * step / total;
    step++;
    if (step >= total) {
        step = 0;
    }
    evtSetValue(event, outVar, step);
    return EVT_RETURN_DONE;
}
