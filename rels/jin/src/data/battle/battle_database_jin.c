#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "jin.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"


extern BattleUnitKind unit_faker_mario;

f32 fd_battle_database_jin_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_jin_neg_one_382;
f32 fd_battle_database_jin___local_PI_383;
f32 fd_battle_database_jin___local_3PIO2_385;
f32 fd_battle_database_jin_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_jin___local_PIO2_384;
f32 fd_battle_database_jin_one_381;
f32 fd_battle_database_jin_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_jin_neg_one_365;
f32 fd_battle_database_jin___local_PI_366;
f32 fd_battle_database_jin___local_3PIO2_368;
f32 fd_battle_database_jin_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_jin___local_PIO2_367;
f32 fd_battle_database_jin_one_364;
BattleStageObjectData jin_btlobject_data[18];
BattleStageData jin_btlstage_data[6];
BattleUnitSetup btlparty_jin_jin_04_ramper[1];
BattleGroupSetup btlgrp_jin_jin_04_ramper;
BattleUnitSetup btlparty_jin_jin_01_faker_mario[1];
BattleGroupSetup btlgrp_jin_jin_01_faker_mario;
BattleUnitSetup btlparty_jin_jin_00_atmic_teresa[1];
BattleGroupSetup btlgrp_jin_jin_00_atmic_teresa;
BattleUnitSetup btlparty_jin_jin_00_01_off_1[1];
BattleGroupSetup btlgrp_jin_jin_00_01_off_1;
BattleUnitSetup btlparty_jin_jin_00_01_off_2[2];
BattleGroupSetup btlgrp_jin_jin_00_01_off_2;
BattleUnitSetup btlparty_jin_jin_00_01_off_3[3];
BattleGroupSetup btlgrp_jin_jin_00_01_off_3;
BattleUnitSetup btlparty_jin_jin_00_01_off_4[4];
BattleGroupSetup btlgrp_jin_jin_00_01_off_4;
BattleUnitSetup btlparty_jin_jin_01_01_off_1[2];
BattleGroupSetup btlgrp_jin_jin_01_01_off_1;
BattleUnitSetup btlparty_jin_jin_01_01_off_2[3];
BattleGroupSetup btlgrp_jin_jin_01_01_off_2;
BattleUnitSetup btlparty_jin_jin_01_01_off_3[3];
BattleGroupSetup btlgrp_jin_jin_01_01_off_3;
BattleUnitSetup btlparty_jin_jin_01_03_off_1[2];
BattleGroupSetup btlgrp_jin_jin_01_03_off_1;
BattleUnitSetup btlparty_jin_jin_01_03_off_2[4];
BattleGroupSetup btlgrp_jin_jin_01_03_off_2;
BattleUnitSetup btlparty_jin_jin_01_03_off_3[2];
BattleGroupSetup btlgrp_jin_jin_01_03_off_3;
BattleUnitSetup btlparty_jin_jin_03_01_off_1[2];
BattleGroupSetup btlgrp_jin_jin_03_01_off_1;
BattleUnitSetup btlparty_jin_jin_03_02_off_1[2];
BattleGroupSetup btlgrp_jin_jin_03_02_off_1;
BattleUnitSetup btlparty_jin_jin_07_01_off_1[2];
BattleGroupSetup btlgrp_jin_jin_07_01_off_1;
BattleUnitSetup btlparty_jin_jin_07_01_off_2[3];
BattleGroupSetup btlgrp_jin_jin_07_01_off_2;
BattleUnitSetup btlparty_jin_jin_07_02_off_1[2];
BattleGroupSetup btlgrp_jin_jin_07_02_off_1;
BattleUnitSetup btlparty_jin_jin_07_02_off_2[3];
BattleGroupSetup btlgrp_jin_jin_07_02_off_2;
BattleUnitSetup btlparty_jin_jin_09_01_off_1[1];
BattleGroupSetup btlgrp_jin_jin_09_01_off_1;
BattleUnitSetup btlparty_jin_jin_10_01_off_1[2];
BattleGroupSetup btlgrp_jin_jin_10_01_off_1;
BattleUnitSetup btlparty_jin_jin_10_02_off_1[1];
BattleGroupSetup btlgrp_jin_jin_10_02_off_1;
BattleUnitSetup btlparty_jin_jin_10_03_off_1[1];
BattleGroupSetup btlgrp_jin_jin_10_03_off_1;
BattleUnitSetup btlparty_jin_jin_10_04_off_1[4];
BattleGroupSetup btlgrp_jin_jin_10_04_off_1;
BattleUnitSetup btlparty_jin_jin_10_05_off_1[2];
BattleGroupSetup btlgrp_jin_jin_10_05_off_1;
BattleUnitSetup btlparty_jin_jin_10_06_off_1[3];
BattleGroupSetup btlgrp_jin_jin_10_06_off_1;
NpcBattleSelect btlgrpsel_jin_btlno_jin_test1_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_test2_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_test3_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_test4_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_test5_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_01_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_02_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_01_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_02_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_03_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_04_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_05_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_06_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_09_off[5];
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_03_off[4];
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_05_off[4];
NpcBattleSelect btlgrpsel_jin_btlno_jin_02_01_off[4];
NpcBattleSelect btlgrpsel_jin_btlno_jin_02_03_off[4];
NpcBattleSelect btlgrpsel_jin_btlno_jin_09_01_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_01_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_02_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_03_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_04_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_05_off[2];
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_06_off[2];
NpcSetupBattleInfo btlsetup_jin_tbl[26];
NpcTerritoryInfo btl_jin_setup_no_tbl[26];

f32 fd_battle_database_jin_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_jin_neg_one_382 = -1.0f;

f32 fd_battle_database_jin___local_PI_383 = 3.1415927f;

f32 fd_battle_database_jin___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_jin_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_jin___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_jin_one_381 = 1.0f;

f32 fd_battle_database_jin_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_jin_neg_one_365 = -1.0f;

f32 fd_battle_database_jin___local_PI_366 = 3.1415927f;

f32 fd_battle_database_jin___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_jin_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_jin___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_jin_one_364 = 1.0f;

BattleStageObjectData jin_btlobject_data[] = {
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"if_wire", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, 6, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"ranpu", 2, 0, {-85.0f, 0.0f, -36.0f}, 0, 60, {0}},
    {"tukue", 2, 0, {-27.0f, 0.0f, -46.0f}, 20, 40, {0}},
    {"reizouko", 2, 0, {-124.0f, 0.0f, -60.0f}, 20, 40, {0}},
    {"furo", 2, 0, {92.0f, 0.0f, -43.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 90, {0}},
};

BattleStageData jin_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_3", // mSpecificStageDataDir
        2, // mNumObjects
        jin_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_3", // mSpecificStageDataDir
        4, // mNumObjects
        &jin_btlobject_data[2], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_2", // mSpecificStageDataDir
        2, // mNumObjects
        &jin_btlobject_data[6], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_4", // mSpecificStageDataDir
        6, // mNumObjects
        &jin_btlobject_data[8], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_5", // mSpecificStageDataDir
        2, // mNumObjects
        &jin_btlobject_data[14], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_6", // mSpecificStageDataDir
        2, // mNumObjects
        &jin_btlobject_data[16], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_jin_jin_04_ramper[] = {
    {
        &unit_boss_rampell, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_rampell, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_04_ramper = {
    1, // numEnemies
    btlparty_jin_jin_04_ramper, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_faker_mario[] = {
    {
        &unit_faker_mario, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_faker_mario, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_faker_mario = {
    1, // numEnemies
    btlparty_jin_jin_01_faker_mario, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_00_atmic_teresa[] = {
    {
        &unit_atmic_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 20.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_atmic_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_00_atmic_teresa = {
    1, // numEnemies
    btlparty_jin_jin_00_atmic_teresa, // enemyData
    0, // heldItemWeight
    10, // randomItemWeight
    0, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_00_01_off_1[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_00_01_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_00_01_off_2[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_00_01_off_2 = {
    2, // numEnemies
    btlparty_jin_jin_00_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_00_01_off_3[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_00_01_off_3 = {
    3, // numEnemies
    btlparty_jin_jin_00_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_00_01_off_4[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_00_01_off_4 = {
    4, // numEnemies
    btlparty_jin_jin_00_01_off_4, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_01_off_1[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_01_off_2[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_01_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_01_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_01_off_3[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_01_off_3 = {
    3, // numEnemies
    btlparty_jin_jin_01_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_03_off_1[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_03_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_03_off_2[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_03_off_2 = {
    4, // numEnemies
    btlparty_jin_jin_01_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_01_03_off_3[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_01_03_off_3 = {
    2, // numEnemies
    btlparty_jin_jin_01_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_03_01_off_1[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_03_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_03_02_off_1[] = {
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_03_02_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_03_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_07_01_off_1[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_07_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_07_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_07_01_off_2[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_07_01_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_07_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_07_02_off_1[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_07_02_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_07_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_07_02_off_2[] = {
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_07_02_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_07_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_09_01_off_1[] = {
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_09_01_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_01_off_1[] = {
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_10_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_02_off_1[] = {
    {
        &unit_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_02_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_10_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_03_off_1[] = {
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_03_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_10_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_04_off_1[] = {
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_04_off_1 = {
    4, // numEnemies
    btlparty_jin_jin_10_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_05_off_1[] = {
    {
        &unit_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
    {
        &unit_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_05_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_10_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jin_jin_10_06_off_1[] = {
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_togemet, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jin_jin_10_06_off_1 = {
    3, // numEnemies
    btlparty_jin_jin_10_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_test1_off[] = {
    {50, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_test2_off[] = {
    {50, &btlgrp_jin_jin_00_01_off_1, jin_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_test3_off[] = {
    {50, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_test4_off[] = {
    {50, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_test5_off[] = {
    {50, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_01_01_off[] = {
    {50, &btlgrp_jin_jin_04_ramper, &jin_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_01_02_off[] = {
    {50, &btlgrp_jin_jin_01_faker_mario, &jin_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_01_off[] = {
    {50, &btlgrp_jin_jin_00_atmic_teresa, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_02_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_03_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_04_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_05_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_06_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_00_09_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_01_03_off[] = {
    {20, &btlgrp_jin_jin_01_01_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_2, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_3, &jin_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_01_05_off[] = {
    {10, &btlgrp_jin_jin_01_03_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_03_off_2, &jin_btlstage_data[1]},
    {20, &btlgrp_jin_jin_01_03_off_3, &jin_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_02_01_off[] = {
    {50, &btlgrp_jin_jin_01_03_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_03_off_2, &jin_btlstage_data[1]},
    {20, &btlgrp_jin_jin_01_03_off_3, &jin_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_02_03_off[] = {
    {50, &btlgrp_jin_jin_01_01_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_2, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_3, &jin_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_09_01_off[] = {
    {50, &btlgrp_jin_jin_09_01_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_01_off[] = {
    {50, &btlgrp_jin_jin_10_01_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_02_off[] = {
    {50, &btlgrp_jin_jin_10_02_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_03_off[] = {
    {50, &btlgrp_jin_jin_10_03_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_04_off[] = {
    {50, &btlgrp_jin_jin_10_04_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_05_off[] = {
    {50, &btlgrp_jin_jin_10_05_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcBattleSelect btlgrpsel_jin_btlno_jin_10_06_off[] = {
    {50, &btlgrp_jin_jin_10_06_off_1, &jin_btlstage_data[5]},
    {0},
};

NpcSetupBattleInfo btlsetup_jin_tbl[] = {
    {
        /*"マップstg_04_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_04_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_04_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_04_5"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test4_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_04_6"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x36", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test5_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_04ランペルその１"*/"\x4A\x49\x4E\x5F\x30\x34\x83\x89\x83\x93\x83\x79\x83\x8B\x82\xBB\x82\xCC\x82\x50", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        /*"JIN_04ランペルその２"*/"\x4A\x49\x4E\x5F\x30\x34\x83\x89\x83\x93\x83\x79\x83\x8B\x82\xBB\x82\xCC\x82\x51", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_02_off, // select
        0x20000010, // battleFlags
        6, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG4_FAKEMARIO1", // bgmName
    },
    {
        /*"JIN_00アトミックテレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x41\x83\x67\x83\x7E\x83\x62\x83\x4E\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_01_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{0}, {0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_02_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_03_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_04_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_05_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_06_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_00テレサ"*/"\x4A\x49\x4E\x5F\x30\x30\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_09_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_01バサバサ"*/"\x4A\x49\x4E\x5F\x30\x31\x83\x6F\x83\x54\x83\x6F\x83\x54", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_01バサバサ"*/"\x4A\x49\x4E\x5F\x30\x31\x83\x6F\x83\x54\x83\x6F\x83\x54", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_02バサバサ"*/"\x4A\x49\x4E\x5F\x30\x32\x83\x6F\x83\x54\x83\x6F\x83\x54", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_02バサバサ"*/"\x4A\x49\x4E\x5F\x30\x32\x83\x6F\x83\x54\x83\x6F\x83\x54", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_02_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_09メット"*/"\x4A\x49\x4E\x5F\x30\x39\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10メット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_01_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10トゲメット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_02_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10メット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_03_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10メット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_04_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10トゲメット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_05_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"JIN_10メット"*/"\x4A\x49\x4E\x5F\x31\x30\x83\x81\x83\x62\x83\x67", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_06_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_jin_setup_no_tbl[] = {
    {"BTLNO_JIN_TEST1", 0},
    {"BTLNO_JIN_TEST2", 1},
    {"BTLNO_JIN_TEST3", 2},
    {"BTLNO_JIN_TEST4", 3},
    {"BTLNO_JIN_TEST5", 4},
    {"BTLNO_JIN_01_01", 5},
    {"BTLNO_JIN_01_02", 6},
    {"BTLNO_JIN_00_01", 7},
    {"BTLNO_JIN_00_02", 8},
    {"BTLNO_JIN_00_03", 9},
    {"BTLNO_JIN_00_04", 10},
    {"BTLNO_JIN_00_05", 11},
    {"BTLNO_JIN_00_06", 12},
    {"BTLNO_JIN_00_09", 13},
    {"BTLNO_JIN_01_03", 14},
    {"BTLNO_JIN_01_05", 15},
    {"BTLNO_JIN_02_01", 16},
    {"BTLNO_JIN_02_03", 17},
    {"BTLNO_JIN_09_01", 18},
    {"BTLNO_JIN_10_01", 19},
    {"BTLNO_JIN_10_02", 20},
    {"BTLNO_JIN_10_03", 21},
    {"BTLNO_JIN_10_04", 22},
    {"BTLNO_JIN_10_05", 23},
    {"BTLNO_JIN_10_06", 24},
    {0},
};

