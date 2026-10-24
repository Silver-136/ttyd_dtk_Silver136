#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "gon.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"


extern BattleUnitKind unit_red_honenoko;

f32 fd_battle_database_gon_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_gon_neg_one_382;
f32 fd_battle_database_gon___local_PI_383;
f32 fd_battle_database_gon___local_3PIO2_385;
f32 fd_battle_database_gon_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_gon___local_PIO2_384;
f32 fd_battle_database_gon_one_381;
f32 fd_battle_database_gon_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_gon_neg_one_365;
f32 fd_battle_database_gon___local_PI_366;
f32 fd_battle_database_gon___local_3PIO2_368;
f32 fd_battle_database_gon_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_gon___local_PIO2_367;
f32 fd_battle_database_gon_one_364;
BattleStageObjectData gon_btlobject_data[10];
BattleStageData gon_btlstage_data[5];
BattleUnitSetup btlparty_gon_gon_02_02_off_1[3];
BattleGroupSetup btlgrp_gon_gon_02_02_off_1;
BattleUnitSetup btlparty_gon_gon_02_03_off_1[2];
BattleGroupSetup btlgrp_gon_gon_02_03_off_1;
BattleUnitSetup btlparty_gon_gon_02_04_off_1[3];
BattleGroupSetup btlgrp_gon_gon_02_04_off_1;
BattleUnitSetup btlparty_gon_gon_03_01_off_1[5];
BattleGroupSetup btlgrp_gon_gon_03_01_off_1;
BattleUnitSetup btlparty_gon_gon_04_01_off_1[2];
BattleGroupSetup btlgrp_gon_gon_04_01_off_1;
BattleUnitSetup btlparty_gon_gon_04_02_off_1[4];
BattleGroupSetup btlgrp_gon_gon_04_02_off_1;
BattleUnitSetup btlparty_gon_gon_04_03_off_1[3];
BattleGroupSetup btlgrp_gon_gon_04_03_off_1;
BattleUnitSetup btlparty_gon_gon_06_03_off_1[3];
BattleGroupSetup btlgrp_gon_gon_06_03_off_1;
BattleUnitSetup btlparty_gon_gon_06_04_off_1[3];
BattleGroupSetup btlgrp_gon_gon_06_04_off_1;
BattleUnitSetup btlparty_gon_gon_08_01_off_1[2];
BattleGroupSetup btlgrp_gon_gon_08_01_off_1;
BattleUnitSetup btlparty_gon_gon_08_03_off_1[2];
BattleGroupSetup btlgrp_gon_gon_08_03_off_1;
BattleUnitSetup btlparty_gon_gon_08_05_off_1[2];
BattleGroupSetup btlgrp_gon_gon_08_05_off_1;
BattleUnitSetup btlparty_gon_gon_09_01_off_1[3];
BattleGroupSetup btlgrp_gon_gon_09_01_off_1;
BattleUnitSetup btlparty_gon_gon_09_02_off_1[3];
BattleGroupSetup btlgrp_gon_gon_09_02_off_1;
BattleUnitSetup btlparty_gon_gon_09_04_off_1[4];
BattleGroupSetup btlgrp_gon_gon_09_04_off_1;
BattleUnitSetup btlparty_gon_gon_10_01_off_1[4];
BattleGroupSetup btlgrp_gon_gon_10_01_off_1;
BattleUnitSetup btlparty_gon_gon_10_02_off_1[3];
BattleGroupSetup btlgrp_gon_gon_10_02_off_1;
BattleUnitSetup btlparty_gon_gon_10_03_off_1[4];
BattleGroupSetup btlgrp_gon_gon_10_03_off_1;
BattleUnitSetup btlparty_gon_gon_11_01_off_1[1];
BattleGroupSetup btlgrp_gon_gon_11_01_off_1;
NpcBattleSelect btlgrpsel_gon_btlno_gon_test1_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_test2_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_test3_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_02_02_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_02_03_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_02_04_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_03_01_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_04_02_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_04_03_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_06_03_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_06_04_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_08_01_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_08_03_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_08_05_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_09_01_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_09_02_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_09_04_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_10_01_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_10_02_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_10_03_off[2];
NpcBattleSelect btlgrpsel_gon_btlno_gon_11_01_off[2];
NpcSetupBattleInfo btlsetup_gon_tbl[22];
NpcTerritoryInfo btl_gon_setup_no_tbl[22];

f32 fd_battle_database_gon_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_gon_neg_one_382 = -1.0f;

f32 fd_battle_database_gon___local_PI_383 = 3.1415927f;

f32 fd_battle_database_gon___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_gon_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_gon___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_gon_one_381 = 1.0f;

f32 fd_battle_database_gon_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_gon_neg_one_365 = -1.0f;

f32 fd_battle_database_gon___local_PI_366 = 3.1415927f;

f32 fd_battle_database_gon___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_gon_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_gon___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_gon_one_364 = 1.0f;

BattleStageObjectData gon_btlobject_data[] = {
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"haikei_B_a", 2, 2, {0}, 0, 60, {0}},
    {"haikei_C_a", 2, 3, {0}, 0, 60, {0}},
    {"haikei_B_b", 2, 2, {0}, 0, 60, {0}},
    {"haikei_C_b", 2, 3, {0}, 0, 60, {0}},
    {"haikei_B_c", 2, 2, {0}, 0, 60, {0}},
    {"haikei_C_c", 2, 3, {0}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData gon_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_5", // mSpecificStageDataDir
        2, // mNumObjects
        gon_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_6", // mSpecificStageDataDir
        2, // mNumObjects
        &gon_btlobject_data[8], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_3", // mSpecificStageDataDir
        2, // mNumObjects
        &gon_btlobject_data[2], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_3", // mSpecificStageDataDir
        2, // mNumObjects
        &gon_btlobject_data[4], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_01_3", // mSpecificStageDataDir
        2, // mNumObjects
        &gon_btlobject_data[6], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_gon_gon_02_02_off_1[] = {
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_02_02_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_02_03_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_02_03_off_1 = {
    2, // numEnemies
    btlparty_gon_gon_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_02_04_off_1[] = {
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_02_04_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_03_01_off_1[] = {
    {
        &unit_red_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_red_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_03_01_off_1 = {
    5, // numEnemies
    btlparty_gon_gon_03_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_04_01_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_04_01_off_1 = {
    2, // numEnemies
    btlparty_gon_gon_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_04_02_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_04_02_off_1 = {
    4, // numEnemies
    btlparty_gon_gon_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_04_03_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_04_03_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_04_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_06_03_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_06_03_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_06_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_06_04_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_06_04_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_06_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_08_01_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_08_01_off_1 = {
    2, // numEnemies
    btlparty_gon_gon_08_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_08_03_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_08_03_off_1 = {
    2, // numEnemies
    btlparty_gon_gon_08_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_08_05_off_1[] = {
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_08_05_off_1 = {
    2, // numEnemies
    btlparty_gon_gon_08_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_09_01_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_09_01_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_09_02_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_09_02_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_09_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_09_04_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_09_04_off_1 = {
    4, // numEnemies
    btlparty_gon_gon_09_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_10_01_off_1[] = {
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_10_01_off_1 = {
    4, // numEnemies
    btlparty_gon_gon_10_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_10_02_off_1[] = {
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_10_02_off_1 = {
    3, // numEnemies
    btlparty_gon_gon_10_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_10_03_off_1[] = {
    {
        &unit_nokonoko, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_nokonoko, // itemDropTable
    },
    {
        &unit_togekuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togekuri, // itemDropTable
    },
    {
        &unit_patakuri, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patakuri, // itemDropTable
    },
    {
        &unit_patapata, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patapata, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_10_03_off_1 = {
    4, // numEnemies
    btlparty_gon_gon_10_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gon_gon_11_01_off_1[] = {
    {
        &unit_boss_gonbaba, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {365.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gonbaba, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gon_gon_11_01_off_1 = {
    1, // numEnemies
    btlparty_gon_gon_11_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_test1_off[] = {
    {50, &btlgrp_gon_gon_02_02_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_test2_off[] = {
    {50, &btlgrp_gon_gon_02_02_off_1, gon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_test3_off[] = {
    {50, &btlgrp_gon_gon_02_02_off_1, &gon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_02_02_off[] = {
    {50, &btlgrp_gon_gon_02_02_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_02_03_off[] = {
    {10, &btlgrp_gon_gon_02_03_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_02_04_off[] = {
    {10, &btlgrp_gon_gon_02_04_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_03_01_off[] = {
    {10, &btlgrp_gon_gon_03_01_off_1, &gon_btlstage_data[4]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_04_02_off[] = {
    {10, &btlgrp_gon_gon_04_02_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_04_03_off[] = {
    {10, &btlgrp_gon_gon_04_03_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_06_03_off[] = {
    {10, &btlgrp_gon_gon_06_03_off_1, &gon_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_06_04_off[] = {
    {10, &btlgrp_gon_gon_06_04_off_1, &gon_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_08_01_off[] = {
    {10, &btlgrp_gon_gon_08_01_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_08_03_off[] = {
    {10, &btlgrp_gon_gon_08_03_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_08_05_off[] = {
    {10, &btlgrp_gon_gon_08_05_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_09_01_off[] = {
    {10, &btlgrp_gon_gon_09_01_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_09_02_off[] = {
    {10, &btlgrp_gon_gon_09_02_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_09_04_off[] = {
    {10, &btlgrp_gon_gon_09_04_off_1, &gon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_10_01_off[] = {
    {10, &btlgrp_gon_gon_10_01_off_1, gon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_10_02_off[] = {
    {10, &btlgrp_gon_gon_10_02_off_1, gon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_10_03_off[] = {
    {10, &btlgrp_gon_gon_10_03_off_1, gon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gon_btlno_gon_11_01_off[] = {
    {20, &btlgrp_gon_gon_11_01_off_1, &gon_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_gon_tbl[] = {
    {
        /*"マップstg_01_03"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x30\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_01_05"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x30\x35", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_01_06"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x31\x5F\x30\x36", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_test3_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon02パタパタ1"*/"\x67\x6F\x6E\x30\x32\x83\x70\x83\x5E\x83\x70\x83\x5E\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon02ノコノコ2"*/"\x67\x6F\x6E\x30\x32\x83\x6D\x83\x52\x83\x6D\x83\x52\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_02_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon02パタパタ2"*/"\x67\x6F\x6E\x30\x32\x83\x70\x83\x5E\x83\x70\x83\x5E\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_02_04_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {0}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon03赤ホネノコ"*/"\x67\x6F\x6E\x30\x33\x90\xD4\x83\x7A\x83\x6C\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_03_01_off, // select
        0x20000010, // battleFlags
        3, // field_1C
        {{0}, {0}, {0}, {0}, {1, 1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}, // field_20
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        /*"gon04ホネノコ2"*/"\x67\x6F\x6E\x30\x34\x83\x7A\x83\x6C\x83\x6D\x83\x52\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_04_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon04ホネノコ3"*/"\x67\x6F\x6E\x30\x34\x83\x7A\x83\x6C\x83\x6D\x83\x52\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_04_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon06トゲクリボー"*/"\x67\x6F\x6E\x30\x36\x83\x67\x83\x51\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_06_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon06ホネノコ2"*/"\x67\x6F\x6E\x30\x36\x83\x7A\x83\x6C\x83\x6D\x83\x52\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_06_04_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon08ホネノコ"*/"\x67\x6F\x6E\x30\x38\x83\x7A\x83\x6C\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_08_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon08ホネノコ2"*/"\x67\x6F\x6E\x30\x38\x83\x7A\x83\x6C\x83\x6D\x83\x52\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_08_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon08パタクリ"*/"\x67\x6F\x6E\x30\x38\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_08_05_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon09ノコノコ1"*/"\x67\x6F\x6E\x30\x39\x83\x6D\x83\x52\x83\x6D\x83\x52\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_09_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon09ホネノコ1"*/"\x67\x6F\x6E\x30\x39\x83\x7A\x83\x6C\x83\x6D\x83\x52\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_09_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon09ホネノコ2"*/"\x67\x6F\x6E\x30\x39\x83\x7A\x83\x6C\x83\x6D\x83\x52\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_09_04_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {10, 10}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon10トゲクリボー"*/"\x67\x6F\x6E\x31\x30\x83\x67\x83\x51\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_10_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon10パタパタ"*/"\x67\x6F\x6E\x31\x30\x83\x70\x83\x5E\x83\x70\x83\x5E", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_10_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon10ノコノコ"*/"\x67\x6F\x6E\x31\x30\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_10_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {10, 10}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"gon11ゴンババ"*/"\x67\x6F\x6E\x31\x31\x83\x53\x83\x93\x83\x6F\x83\x6F", // name
        "gon_11", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gon_btlno_gon_11_01_off, // select
        0x20100010, // battleFlags
        3, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {0}, {0}, {0}}, // field_20
        "BGM_BOSS_STG1_GONBABA1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_gon_setup_no_tbl[] = {
    {"BTLNO_GON_TEST1", 0},
    {"BTLNO_GON_TEST2", 1},
    {"BTLNO_GON_TEST3", 2},
    {"BTLNO_GON_02_02", 3},
    {"BTLNO_GON_02_03", 4},
    {"BTLNO_GON_02_04", 5},
    {"BTLNO_GON_03_01", 6},
    {"BTLNO_GON_04_02", 7},
    {"BTLNO_GON_04_03", 8},
    {"BTLNO_GON_06_03", 9},
    {"BTLNO_GON_06_04", 10},
    {"BTLNO_GON_08_01", 11},
    {"BTLNO_GON_08_03", 12},
    {"BTLNO_GON_08_05", 13},
    {"BTLNO_GON_09_01", 14},
    {"BTLNO_GON_09_02", 15},
    {"BTLNO_GON_09_04", 16},
    {"BTLNO_GON_10_01", 17},
    {"BTLNO_GON_10_02", 18},
    {"BTLNO_GON_10_03", 19},
    {"BTLNO_GON_11_01", 20},
    {0},
};

