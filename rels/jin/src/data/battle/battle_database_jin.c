// Creepy Steeple (jin) battle database: stages, the Doopliss, fake-Mario and Atomic Boo boss setups, and Boo, Swooper, Buzzy Beetle and Spike Top groups per room.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "jin.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_faker_mario;

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

/**
 * Scenery objects of the Creepy Steeple battle stages (the ceiling frame and the lamp, desk, fridge and bath
 * of the Doopliss room, plus the back and front wall pieces); each stage in jin_btlstage_data uses a slice
 * of this list.
 */
BattleStageObjectData jin_btlobject_data[] = {
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"if_wire", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"if_body", 5, STAGE_LAYER_IRON_FRAME, {0.0f, 130.0f, 0.0f}, 0, 0, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"ranpu", 2, STAGE_LAYER_A1, {-85.0f, 0.0f, -36.0f}, 0, 60, {0}},
    {"tukue", 2, STAGE_LAYER_A1, {-27.0f, 0.0f, -46.0f}, 20, 40, {0}},
    {"reizouko", 2, STAGE_LAYER_A1, {-124.0f, 0.0f, -60.0f}, 20, 40, {0}},
    {"furo", 2, STAGE_LAYER_A1, {92.0f, 0.0f, -43.0f}, 30, 30, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"C", 2, STAGE_LAYER_C, {0}, 0, 90, {0}},
};

/**
 * The 6 battle stages of Creepy Steeple: each pairs a stage model (stg_04_3, stg_04_2, stg_04_4, stg_04_5,
 * stg_04_6) with a slice of the scenery objects above.
 */
BattleStageData jin_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_04_3", // specificStageDataDir
        2, // numObjects
        jin_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_3", // specificStageDataDir
        4, // numObjects
        &jin_btlobject_data[2], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_2", // specificStageDataDir
        2, // numObjects
        &jin_btlobject_data[6], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_4", // specificStageDataDir
        6, // numObjects
        &jin_btlobject_data[8], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_5", // specificStageDataDir
        2, // numObjects
        &jin_btlobject_data[14], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_6", // specificStageDataDir
        2, // numObjects
        &jin_btlobject_data[16], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_jin_jin_04_ramper[] = {
    {
        &unit_boss_rampell, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_jin_jin_01_faker_mario[] = {
    {
        &unit_faker_mario, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_jin_jin_00_atmic_teresa[] = {
    {
        &unit_atmic_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
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
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05
 * and BTLNO_JIN_00_06 and BTLNO_JIN_00_09, in Creepy Steeple (jin_00): Boo.
 */
BattleUnitSetup btlparty_jin_jin_00_01_off_1[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05 and
 * BTLNO_JIN_00_06 and BTLNO_JIN_00_09: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_00_01_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05
 * and BTLNO_JIN_00_06 and BTLNO_JIN_00_09, in Creepy Steeple (jin_00): Boo x2.
 */
BattleUnitSetup btlparty_jin_jin_00_01_off_2[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05 and
 * BTLNO_JIN_00_06 and BTLNO_JIN_00_09: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_00_01_off_2 = {
    2, // numEnemies
    btlparty_jin_jin_00_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05
 * and BTLNO_JIN_00_06 and BTLNO_JIN_00_09, in Creepy Steeple (jin_00): Boo x3.
 */
BattleUnitSetup btlparty_jin_jin_00_01_off_3[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05 and
 * BTLNO_JIN_00_06 and BTLNO_JIN_00_09: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_00_01_off_3 = {
    3, // numEnemies
    btlparty_jin_jin_00_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 4 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05
 * and BTLNO_JIN_00_06 and BTLNO_JIN_00_09, in Creepy Steeple (jin_00): Boo x4.
 */
BattleUnitSetup btlparty_jin_jin_00_01_off_4[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 4 of BTLNO_JIN_00_02 and BTLNO_JIN_00_03 and BTLNO_JIN_00_04 and BTLNO_JIN_00_05 and
 * BTLNO_JIN_00_06 and BTLNO_JIN_00_09: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_00_01_off_4 = {
    4, // numEnemies
    btlparty_jin_jin_00_01_off_4, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03, in Creepy Steeple (jin_01): Swooper
 * x2.
 */
BattleUnitSetup btlparty_jin_jin_01_01_off_1[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03, in Creepy Steeple (jin_01): Swooper
 * x3.
 */
BattleUnitSetup btlparty_jin_jin_01_01_off_2[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_01_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_01_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03, in Creepy Steeple (jin_01): Swooper
 * x2, Boo.
 */
BattleUnitSetup btlparty_jin_jin_01_01_off_3[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JIN_01_03 and BTLNO_JIN_02_03: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_01_off_3 = {
    3, // numEnemies
    btlparty_jin_jin_01_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01, in Creepy Steeple (jin_01): Swooper
 * x2.
 */
BattleUnitSetup btlparty_jin_jin_01_03_off_1[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_03_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01, in Creepy Steeple (jin_01): Swooper
 * x4.
 */
BattleUnitSetup btlparty_jin_jin_01_03_off_2[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_03_off_2 = {
    4, // numEnemies
    btlparty_jin_jin_01_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01, in Creepy Steeple (jin_01): Swooper,
 * Boo.
 */
BattleUnitSetup btlparty_jin_jin_01_03_off_3[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JIN_01_05 and BTLNO_JIN_02_01: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_01_03_off_3 = {
    2, // numEnemies
    btlparty_jin_jin_01_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

// These Swoopers start flying (unitWork[0] = 1), unlike the ceiling groups above.
BattleUnitSetup btlparty_jin_jin_03_01_off_1[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Group setup of formation 1 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_03_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 that no battle selects: Swooper x2. */
BattleUnitSetup btlparty_jin_jin_03_02_off_1[] = {
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
    {
        &unit_basabasa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa, // itemDropTable
    },
};

/**
 * Group setup of formation 1 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_03_02_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_03_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 that no battle selects: Boo x2. */
BattleUnitSetup btlparty_jin_jin_07_01_off_1[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Group setup of formation 1 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_07_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_07_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 that no battle selects: Boo x3. */
BattleUnitSetup btlparty_jin_jin_07_01_off_2[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Group setup of formation 2 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_07_01_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_07_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 that no battle selects: Boo x2. */
BattleUnitSetup btlparty_jin_jin_07_02_off_1[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Group setup of formation 1 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_07_02_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_07_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 that no battle selects: Boo x3. */
BattleUnitSetup btlparty_jin_jin_07_02_off_2[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Group setup of formation 2 that no battle selects: the lineup above with the default heart and flower
 * drops. Item drop weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_07_02_off_2 = {
    3, // numEnemies
    btlparty_jin_jin_07_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_09_01, in Creepy Steeple (jin_09): Buzzy Beetle. */
BattleUnitSetup btlparty_jin_jin_09_01_off_1[] = {
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_09_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_09_01_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_09_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_01, in Creepy Steeple (jin_10): Buzzy Beetle x2. */
BattleUnitSetup btlparty_jin_jin_10_01_off_1[] = {
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_01_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_10_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_02, in Creepy Steeple (jin_10): Spike Top. */
BattleUnitSetup btlparty_jin_jin_10_02_off_1[] = {
    {
        &unit_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_02_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_10_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_03, in Creepy Steeple (jin_10): Buzzy Beetle. */
BattleUnitSetup btlparty_jin_jin_10_03_off_1[] = {
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_03_off_1 = {
    1, // numEnemies
    btlparty_jin_jin_10_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_04, in Creepy Steeple (jin_10): Buzzy Beetle x4. */
BattleUnitSetup btlparty_jin_jin_10_04_off_1[] = {
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_04_off_1 = {
    4, // numEnemies
    btlparty_jin_jin_10_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_05, in Creepy Steeple (jin_10): Spike Top x2. */
BattleUnitSetup btlparty_jin_jin_10_05_off_1[] = {
    {
        &unit_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
    {
        &unit_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_05: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_05_off_1 = {
    2, // numEnemies
    btlparty_jin_jin_10_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JIN_10_06, in Creepy Steeple (jin_10): Buzzy Beetle x2, Spike Top. */
BattleUnitSetup btlparty_jin_jin_10_06_off_1[] = {
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_met, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_met, // itemDropTable
    },
    {
        &unit_togemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togemet, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JIN_10_06: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_jin_jin_10_06_off_1 = {
    3, // numEnemies
    btlparty_jin_jin_10_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

// The five test selectors reuse the single-Boo party with different stage entries.
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

/** BTLNO_JIN_01_01 always fights Doopliss on stage stg_04_4. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_01_off[] = {
    {50, &btlgrp_jin_jin_04_ramper, &jin_btlstage_data[3]},
    {0},
};

/** BTLNO_JIN_01_02 always fights Doopliss disguised as Mario on stage stg_04_4. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_02_off[] = {
    {50, &btlgrp_jin_jin_01_faker_mario, &jin_btlstage_data[3]},
    {0},
};

/** BTLNO_JIN_00_01 always fights Atomic Boo on stage stg_04_2. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_01_off[] = {
    {50, &btlgrp_jin_jin_00_atmic_teresa, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_02 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_02_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_03 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_03_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_04 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_04_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_05 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_05_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_06 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_06_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_00_09 picks one of its formations by weight (10:20:10:10) on stage stg_04_2: Boo; Boo x2; Boo
 * x3; Boo x4.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_00_09_off[] = {
    {10, &btlgrp_jin_jin_00_01_off_1, &jin_btlstage_data[2]},
    {20, &btlgrp_jin_jin_00_01_off_2, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_3, &jin_btlstage_data[2]},
    {10, &btlgrp_jin_jin_00_01_off_4, &jin_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JIN_01_03 picks one of its formations by weight (20:10:10) on stage stg_04_3: Swooper x2; Swooper
 * x3; Swooper x2, Boo.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_03_off[] = {
    {20, &btlgrp_jin_jin_01_01_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_2, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_3, &jin_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JIN_01_05 picks one of its formations by weight (10:10:20) on stage stg_04_3: Swooper x2; Swooper
 * x4; Swooper, Boo.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_01_05_off[] = {
    {10, &btlgrp_jin_jin_01_03_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_03_off_2, &jin_btlstage_data[1]},
    {20, &btlgrp_jin_jin_01_03_off_3, &jin_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JIN_02_01 picks one of its formations by weight (50:10:20) on stage stg_04_3: Swooper x2; Swooper
 * x4; Swooper, Boo.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_02_01_off[] = {
    {50, &btlgrp_jin_jin_01_03_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_03_off_2, &jin_btlstage_data[1]},
    {20, &btlgrp_jin_jin_01_03_off_3, &jin_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JIN_02_03 picks one of its formations by weight (50:10:10) on stage stg_04_3: Swooper x2; Swooper
 * x3; Swooper x2, Boo.
 */
NpcBattleSelect btlgrpsel_jin_btlno_jin_02_03_off[] = {
    {50, &btlgrp_jin_jin_01_01_off_1, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_2, &jin_btlstage_data[1]},
    {10, &btlgrp_jin_jin_01_01_off_3, &jin_btlstage_data[1]},
    {0},
};

/** BTLNO_JIN_09_01 always fights Buzzy Beetle on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_09_01_off[] = {
    {50, &btlgrp_jin_jin_09_01_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_01 always fights Buzzy Beetle x2 on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_01_off[] = {
    {50, &btlgrp_jin_jin_10_01_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_02 always fights Spike Top on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_02_off[] = {
    {50, &btlgrp_jin_jin_10_02_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_03 always fights Buzzy Beetle on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_03_off[] = {
    {50, &btlgrp_jin_jin_10_03_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_04 always fights Buzzy Beetle x4 on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_04_off[] = {
    {50, &btlgrp_jin_jin_10_04_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_05 always fights Spike Top x2 on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_05_off[] = {
    {50, &btlgrp_jin_jin_10_05_off_1, &jin_btlstage_data[5]},
    {0},
};

/** BTLNO_JIN_10_06 always fights Buzzy Beetle x2, Spike Top on stage stg_04_6. */
NpcBattleSelect btlgrpsel_jin_btlno_jin_10_06_off[] = {
    {50, &btlgrp_jin_jin_10_06_off_1, &jin_btlstage_data[5]},
    {0},
};

// Table indices are battleInfoId values; entry 7 is the Atomic Boo encounter.
NpcSetupBattleInfo btlsetup_jin_tbl[] = {
    {
        "マップstg_04_2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_04_3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_04_4", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_04_5", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test4_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_04_6", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_test5_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_04ランペルその１", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        "JIN_04ランペルその２", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_02_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        6, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG4_FAKEMARIO1", // bgmName
    },
    {
        "JIN_00アトミックテレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{0}, {0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_CHUBOSS_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_02_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_03_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_04_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_05_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_06_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_00テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_00_09_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_01バサバサ", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_01バサバサ", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_01_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_02バサバサ", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_02_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_02バサバサ", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_02_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {0}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_09メット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_09_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10メット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10トゲメット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_02_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10メット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_03_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10メット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_04_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10トゲメット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_05_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "JIN_10メット", // name
        "jin_04", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jin_btlno_jin_10_06_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
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

