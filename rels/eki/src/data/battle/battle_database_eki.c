#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "eki.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

BattleStageObjectData eki_btlobject_data[24];
BattleStageData eki_btlstage_data[4];
BattleUnitSetup btlparty_eki_eki_06_01_off_1[3];
BattleGroupSetup btlgrp_eki_eki_06_01_off_1;
BattleUnitSetup btlparty_eki_eki_02_01_off_1[2];
BattleGroupSetup btlgrp_eki_eki_02_01_off_1;
BattleUnitSetup btlparty_eki_eki_02_02_off_1[3];
BattleGroupSetup btlgrp_eki_eki_02_02_off_1;
BattleUnitSetup btlparty_eki_eki_02_03_off_1[2];
BattleGroupSetup btlgrp_eki_eki_02_03_off_1;
BattleUnitSetup btlparty_eki_eki_03_01_off_1[2];
BattleGroupSetup btlgrp_eki_eki_03_01_off_1;
BattleUnitSetup btlparty_eki_eki_03_02_off_1[3];
BattleGroupSetup btlgrp_eki_eki_03_02_off_1;
BattleUnitSetup btlparty_eki_eki_03_03_off_1[3];
BattleGroupSetup btlgrp_eki_eki_03_03_off_1;
BattleUnitSetup btlparty_eki_eki_03_05_off_1[2];
BattleGroupSetup btlgrp_eki_eki_03_05_off_1;
BattleUnitSetup btlparty_eki_eki_03_07_off_1[4];
BattleGroupSetup btlgrp_eki_eki_03_07_off_1;
BattleUnitSetup btlparty_eki_eki_04_01_off_1[3];
BattleGroupSetup btlgrp_eki_eki_04_01_off_1;
BattleUnitSetup btlparty_eki_eki_04_02_off_1[3];
BattleGroupSetup btlgrp_eki_eki_04_02_off_1;
BattleUnitSetup btlparty_eki_eki_04_06_off_1[2];
BattleGroupSetup btlgrp_eki_eki_04_06_off_1;
BattleUnitSetup btlparty_eki_eki_04_03_off_1[2];
BattleGroupSetup btlgrp_eki_eki_04_03_off_1;
BattleUnitSetup btlparty_eki_eki_04_04_off_1[4];
BattleGroupSetup btlgrp_eki_eki_04_04_off_1;
BattleUnitSetup btlparty_eki_eki_04_05_off_1[2];
BattleGroupSetup btlgrp_eki_eki_04_05_off_1;
NpcBattleSelect btlgrpsel_eki_btlno_eki_test1_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_test2_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_test3_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_06_01_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_06_02_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_02_01_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_02_02_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_02_03_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_03_01_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_03_02_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_03_03_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_03_06_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_03_08_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_01_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_02_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_06_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_03_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_04_off[2];
NpcBattleSelect btlgrpsel_eki_btlno_eki_04_05_off[2];
NpcSetupBattleInfo btlsetup_eki_tbl[20];
NpcTerritoryInfo btl_eki_setup_no_tbl[20];

BattleStageObjectData eki_btlobject_data[] = {
    {"lamp_01", 2, 0, {-116.0f, 0.0f, -59.0f}, 0, 60, {0}},
    {"lamp_02", 2, 0, {116.0f, 0.0f, -59.0f}, 10, 50, {0}},
    {"lamp_03", 2, 0, {0.0f, 0.0f, -59.0f}, 20, 40, {0}},
    {"gomibako_01", 2, 1, {-178.0f, 0.0f, 49.0f}, 0, 60, {0}},
    {"gomibako_02", 2, 1, {178.0f, 0.0f, 49.0f}, 10, 50, {0}},
    {"saku", 2, 1, {0.0f, 0.0f, 69.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"gomibako_01", 2, 1, {-178.0f, 0.0f, 49.0f}, 0, 60, {0}},
    {"gomibako_02", 2, 1, {178.0f, 0.0f, 49.0f}, 10, 50, {0}},
    {"saku", 2, 1, {0.0f, 0.0f, 69.0f}, 20, 40, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"A1", 2, 0, {0.0f, 0.0f, 3.0f}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"B", 2, 2, {0.0f, 0.0f, 5.0f}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"saku_01", 2, 1, {-114.0f, 0.0f, -23.0f}, 0, 60, {0}},
    {"saku_02", 2, 1, {130.0f, 0.0f, -25.0f}, 10, 50, {0}},
    {"sikake_01", 2, 0, {-118.0f, 0.0f, -41.0f}, 0, 60, {0}},
    {"sikake_02", 2, 0, {-42.0f, 0.0f, -59.0f}, 10, 50, {0}},
    {"sikake_03", 2, 0, {132.0f, 0.0f, -38.0f}, 20, 40, {0}},
    {"sikake_04", 2, 0, {36.0f, 0.0f, -48.0f}, 30, 30, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData eki_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_1", // mSpecificStageDataDir
        8, // mNumObjects
        eki_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_1", // mSpecificStageDataDir
        4, // mNumObjects
        &eki_btlobject_data[8], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_2", // mSpecificStageDataDir
        4, // mNumObjects
        &eki_btlobject_data[12], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_3", // mSpecificStageDataDir
        8, // mNumObjects
        &eki_btlobject_data[16], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_eki_eki_06_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_06_01_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_06_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_02_01_off_1[] = {
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_02_01_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_02_02_off_1[] = {
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_02_02_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_02_03_off_1[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_02_03_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_03_01_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_03_01_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_03_02_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_03_02_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_03_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_03_03_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_03_03_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_03_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_03_05_off_1[] = {
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_03_05_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_03_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_03_07_off_1[] = {
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_03_07_off_1 = {
    4, // numEnemies
    btlparty_eki_eki_03_07_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_01_off_1[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_01_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_02_off_1[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_02_off_1 = {
    3, // numEnemies
    btlparty_eki_eki_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_06_off_1[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_06_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_04_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_03_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_03_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_04_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_04_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_04_off_1 = {
    4, // numEnemies
    btlparty_eki_eki_04_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_eki_eki_04_05_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_eki_eki_04_05_off_1 = {
    2, // numEnemies
    btlparty_eki_eki_04_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_test1_off[] = {
    {50, &btlgrp_eki_eki_06_01_off_1, eki_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_test2_off[] = {
    {50, &btlgrp_eki_eki_06_01_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_test3_off[] = {
    {50, &btlgrp_eki_eki_06_01_off_1, &eki_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_06_01_off[] = {
    {50, &btlgrp_eki_eki_06_01_off_1, eki_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_06_02_off[] = {
    {50, &btlgrp_eki_eki_06_01_off_1, eki_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_02_01_off[] = {
    {20, &btlgrp_eki_eki_02_01_off_1, &eki_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_02_02_off[] = {
    {20, &btlgrp_eki_eki_02_02_off_1, &eki_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_02_03_off[] = {
    {20, &btlgrp_eki_eki_02_03_off_1, &eki_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_03_01_off[] = {
    {20, &btlgrp_eki_eki_03_01_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_03_02_off[] = {
    {20, &btlgrp_eki_eki_03_02_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_03_03_off[] = {
    {20, &btlgrp_eki_eki_03_03_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_03_06_off[] = {
    {20, &btlgrp_eki_eki_03_05_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_03_08_off[] = {
    {20, &btlgrp_eki_eki_03_07_off_1, &eki_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_01_off[] = {
    {20, &btlgrp_eki_eki_04_01_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_02_off[] = {
    {20, &btlgrp_eki_eki_04_02_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_06_off[] = {
    {20, &btlgrp_eki_eki_04_06_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_03_off[] = {
    {20, &btlgrp_eki_eki_04_03_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_04_off[] = {
    {20, &btlgrp_eki_eki_04_04_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_eki_btlno_eki_04_05_off[] = {
    {20, &btlgrp_eki_eki_04_05_off_1, &eki_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_eki_tbl[] = {
    {
        /*"マップstg_06_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x36\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_06_2"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x36\x5F\x32", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_06_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x36\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_06 クリボー３"*/"\x65\x6B\x69\x5F\x30\x36\x20\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_06_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_06 クリボー３"*/"\x65\x6B\x69\x5F\x30\x36\x20\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_06_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_02クロクモーン"*/"\x65\x6B\x69\x5F\x30\x32\x83\x4E\x83\x8D\x83\x4E\x83\x82\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_02_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_02クロクモーン"*/"\x65\x6B\x69\x5F\x30\x32\x83\x4E\x83\x8D\x83\x4E\x83\x82\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_02_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_02パタトゲメット"*/"\x65\x6B\x69\x5F\x30\x32\x83\x70\x83\x5E\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_02_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_03サンボ"*/"\x65\x6B\x69\x5F\x30\x33\x83\x54\x83\x93\x83\x7B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_03_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_03サンボ"*/"\x65\x6B\x69\x5F\x30\x33\x83\x54\x83\x93\x83\x7B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_03_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_03サンボ"*/"\x65\x6B\x69\x5F\x30\x33\x83\x54\x83\x93\x83\x7B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_03_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_03クロクモーン"*/"\x65\x6B\x69\x5F\x30\x33\x83\x4E\x83\x8D\x83\x4E\x83\x82\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_03_06_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_03クロクモーン"*/"\x65\x6B\x69\x5F\x30\x33\x83\x4E\x83\x8D\x83\x4E\x83\x82\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_03_08_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04パタトゲメット"*/"\x65\x6B\x69\x5F\x30\x34\x83\x70\x83\x5E\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_01_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04パタトゲメット"*/"\x65\x6B\x69\x5F\x30\x34\x83\x70\x83\x5E\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_02_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04パタトゲメット"*/"\x65\x6B\x69\x5F\x30\x34\x83\x70\x83\x5E\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_06_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04サンボマミー"*/"\x65\x6B\x69\x5F\x30\x34\x83\x54\x83\x93\x83\x7B\x83\x7D\x83\x7E\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_03_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04サンボマミー"*/"\x65\x6B\x69\x5F\x30\x34\x83\x54\x83\x93\x83\x7B\x83\x7D\x83\x7E\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_04_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"eki_04サンボマミー"*/"\x65\x6B\x69\x5F\x30\x34\x83\x54\x83\x93\x83\x7B\x83\x7D\x83\x7E\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_eki_btlno_eki_04_05_off, // select
        0, 0, // battleFlags..randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_eki_setup_no_tbl[] = {
    {"BTLNO_EKI_TEST1", 0},
    {"BTLNO_EKI_TEST2", 1},
    {"BTLNO_EKI_TEST3", 2},
    {"BTLNO_EKI_06_01", 3},
    {"BTLNO_EKI_06_02", 4},
    {"BTLNO_EKI_02_01", 5},
    {"BTLNO_EKI_02_02", 6},
    {"BTLNO_EKI_02_03", 7},
    {"BTLNO_EKI_03_01", 8},
    {"BTLNO_EKI_03_02", 9},
    {"BTLNO_EKI_03_03", 10},
    {"BTLNO_EKI_03_06", 11},
    {"BTLNO_EKI_03_08", 12},
    {"BTLNO_EKI_04_01", 13},
    {"BTLNO_EKI_04_02", 14},
    {"BTLNO_EKI_04_06", 15},
    {"BTLNO_EKI_04_03", 16},
    {"BTLNO_EKI_04_04", 17},
    {"BTLNO_EKI_04_05", 18},
    {0},
};

