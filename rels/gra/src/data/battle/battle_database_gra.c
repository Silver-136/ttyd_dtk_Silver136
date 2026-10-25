#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "gra.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

BattleStageObjectData gra_btlobject_data[22];
BattleStageData gra_btlstage_data[2];
BattleUnitSetup btlparty_gra_gra_00_01_off_1[1];
BattleGroupSetup btlgrp_gra_gra_00_01_off_1;
BattleUnitSetup btlparty_gra_gra_01_01_off_1[3];
BattleGroupSetup btlgrp_gra_gra_01_01_off_1;
BattleUnitSetup btlparty_gra_gra_01_01_on_1[3];
BattleGroupSetup btlgrp_gra_gra_01_01_on_1;
BattleUnitSetup btlparty_gra_gra_01_01_on_2[3];
BattleGroupSetup btlgrp_gra_gra_01_01_on_2;
BattleUnitSetup btlparty_gra_gra_01_01_on_3[3];
BattleGroupSetup btlgrp_gra_gra_01_01_on_3;
BattleUnitSetup btlparty_gra_gra_01_03_off_1[3];
BattleGroupSetup btlgrp_gra_gra_01_03_off_1;
BattleUnitSetup btlparty_gra_gra_01_03_on_1[3];
BattleGroupSetup btlgrp_gra_gra_01_03_on_1;
BattleUnitSetup btlparty_gra_gra_01_03_on_2[4];
BattleGroupSetup btlgrp_gra_gra_01_03_on_2;
BattleUnitSetup btlparty_gra_gra_01_03_on_3[4];
BattleGroupSetup btlgrp_gra_gra_01_03_on_3;
BattleUnitSetup btlparty_gra_gra_01_04_off_1[3];
BattleGroupSetup btlgrp_gra_gra_01_04_off_1;
BattleUnitSetup btlparty_gra_gra_01_04_on_1[3];
BattleGroupSetup btlgrp_gra_gra_01_04_on_1;
BattleUnitSetup btlparty_gra_gra_01_04_on_2[4];
BattleGroupSetup btlgrp_gra_gra_01_04_on_2;
BattleUnitSetup btlparty_gra_gra_01_04_on_3[3];
BattleGroupSetup btlgrp_gra_gra_01_04_on_3;
BattleUnitSetup btlparty_gra_gra_03_01_off_1[2];
BattleGroupSetup btlgrp_gra_gra_03_01_off_1;
BattleUnitSetup btlparty_gra_gra_03_01_off_2[3];
BattleGroupSetup btlgrp_gra_gra_03_01_off_2;
BattleUnitSetup btlparty_gra_gra_03_04_off_1[2];
BattleGroupSetup btlgrp_gra_gra_03_04_off_1;
BattleUnitSetup btlparty_gra_gra_03_04_off_2[3];
BattleGroupSetup btlgrp_gra_gra_03_04_off_2;
BattleUnitSetup btlparty_gra_gra_03_04_off_3[3];
BattleGroupSetup btlgrp_gra_gra_03_04_off_3;
BattleUnitSetup btlparty_gra_gra_03_05_off_1[4];
BattleGroupSetup btlgrp_gra_gra_03_05_off_1;
BattleUnitSetup btlparty_gra_gra_03_05_off_2[3];
BattleGroupSetup btlgrp_gra_gra_03_05_off_2;
BattleUnitSetup btlparty_gra_gra_03_05_off_3[4];
BattleGroupSetup btlgrp_gra_gra_03_05_off_3;
BattleUnitSetup btlparty_gra_gra_04_02_off_1[3];
BattleGroupSetup btlgrp_gra_gra_04_02_off_1;
BattleUnitSetup btlparty_gra_gra_04_02_off_2[2];
BattleGroupSetup btlgrp_gra_gra_04_02_off_2;
BattleUnitSetup btlparty_gra_gra_04_02_off_3[3];
BattleGroupSetup btlgrp_gra_gra_04_02_off_3;
BattleUnitSetup btlparty_gra_gra_05_01_off_1[2];
BattleGroupSetup btlgrp_gra_gra_05_01_off_1;
BattleUnitSetup btlparty_gra_gra_05_01_off_2[3];
BattleGroupSetup btlgrp_gra_gra_05_01_off_2;
BattleUnitSetup btlparty_gra_gra_05_03_off_1[3];
BattleGroupSetup btlgrp_gra_gra_05_03_off_1;
BattleUnitSetup btlparty_gra_gra_05_03_off_2[3];
BattleGroupSetup btlgrp_gra_gra_05_03_off_2;
BattleUnitSetup btlparty_gra_gra_05_03_off_3[4];
BattleGroupSetup btlgrp_gra_gra_05_03_off_3;
NpcBattleSelect btlgrpsel_gra_btlno_gra_test1_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_test2_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_00_01_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_00_02_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_on[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_on[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_on[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_off[2];
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_01_off[3];
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_04_off[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_05_off[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_04_02_off[4];
NpcBattleSelect btlgrpsel_gra_btlno_gra_05_01_off[3];
NpcBattleSelect btlgrpsel_gra_btlno_gra_05_03_off[4];
NpcSetupBattleInfo btlsetup_gra_tbl[14];
NpcTerritoryInfo btl_gra_setup_no_tbl[14];

BattleStageObjectData gra_btlobject_data[] = {
    {"ki_1", 2, 0, {-100.0f, 0.0f, -30.0f}, 0, 60, {0}},
    {"isi_A", 2, 0, {150.0f, 0.0f, -30.0f}, 10, 50, {0}},
    {"isi_B", 2, 0, {50.0f, 0.0f, -34.0f}, 20, 40, {0}},
    {"isigaki", 2, 1, {2.0f, 0.0f, -23.0f}, 0, 60, {0}},
    {"kusa_1", 2, 1, {-184.0f, 0.0f, 37.0f}, 10, 50, {0}},
    {"kusa_2", 2, 1, {180.0f, 0.0f, 60.0f}, 20, 40, {0}},
    {"kusa_3", 2, 1, {70.0f, 0.0f, 65.0f}, 30, 30, {0}},
    {"kusa_4", 2, 1, {-87.0f, 0.0f, 56.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"kusa_1", 2, 1, {147.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"kusa_2", 2, 1, {35.0f, 0.0f, 80.0f}, 10, 50, {0}},
    {"kusa_3", 2, 1, {-77.0f, 0.0f, 77.0f}, 20, 40, {0}},
    {"kusa_4", 2, 1, {-155.0f, 0.0f, 71.0f}, 30, 30, {0}},
    {"kusamura_mae", 2, 1, {0.0f, 0.0f, 72.0f}, 0, 60, {0}},
    {"kusamura_usiro", 2, 1, {0.0f, 0.0f, -65.0f}, 10, 50, {0}},
    {"iwa", 2, 1, {0.0f, 0.0f, -71.0f}, 20, 40, {0}},
    {"ki_1", 2, 0, {100.0f, 0.0f, -51.0f}, 0, 60, {0}},
    {"ki_2", 2, 0, {-123.0f, 0.0f, -48.0f}, 10, 50, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"murasaki", 2, 3, {0}, 0, 60, {0}},
    {"ao", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData gra_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_0", // mSpecificStageDataDir
        10, // mNumObjects
        gra_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_04_1", // mSpecificStageDataDir
        11, // mNumObjects
        &gra_btlobject_data[10], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, // reserved_1B0..disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

BattleUnitSetup btlparty_gra_gra_00_01_off_1[] = {
    {
        &unit_faker_mario_gra, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_faker_mario, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_00_01_off_1 = {
    1, // numEnemies
    btlparty_gra_gra_00_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_01_off_1[] = {
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_01_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_01_on_1[] = {
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_01_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_01_on_2[] = {
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_01_on_2 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_01_on_3[] = {
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_01_on_3 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_03_off_1[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_03_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_03_on_1[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_03_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_03_on_2[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_03_on_2 = {
    4, // numEnemies
    btlparty_gra_gra_01_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_03_on_3[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_03_on_3 = {
    4, // numEnemies
    btlparty_gra_gra_01_03_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_04_off_1[] = {
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_04_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_04_on_1[] = {
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_04_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_04_on_2[] = {
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_04_on_2 = {
    4, // numEnemies
    btlparty_gra_gra_01_04_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_01_04_on_3[] = {
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_01_04_on_3 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_01_off_1[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_01_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_01_off_2[] = {
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_01_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_04_off_1[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_04_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_03_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_04_off_2[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_04_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_04_off_3[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_04_off_3 = {
    3, // numEnemies
    btlparty_gra_gra_03_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_05_off_1[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_05_off_1 = {
    4, // numEnemies
    btlparty_gra_gra_03_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_05_off_2[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_05_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_05_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_03_05_off_3[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_03_05_off_3 = {
    4, // numEnemies
    btlparty_gra_gra_03_05_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_04_02_off_1[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_04_02_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_04_02_off_2[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_04_02_off_2 = {
    2, // numEnemies
    btlparty_gra_gra_04_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_04_02_off_3[] = {
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_04_02_off_3 = {
    3, // numEnemies
    btlparty_gra_gra_04_02_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_05_01_off_1[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_05_01_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_05_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_05_01_off_2[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_05_01_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_05_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_05_03_off_1[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_05_03_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_05_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_05_03_off_2[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_05_03_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_05_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_gra_gra_05_03_off_3[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_gra_gra_05_03_off_3 = {
    4, // numEnemies
    btlparty_gra_gra_05_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_test1_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_test2_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, &gra_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_00_01_off[] = {
    {50, &btlgrp_gra_gra_00_01_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_00_02_off[] = {
    {50, &btlgrp_gra_gra_00_01_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_on[] = {
    {10, &btlgrp_gra_gra_01_01_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_01_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_01_on_3, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_on[] = {
    {10, &btlgrp_gra_gra_01_03_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_03_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_03_on_3, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_off[] = {
    {50, &btlgrp_gra_gra_01_03_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_on[] = {
    {10, &btlgrp_gra_gra_01_04_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_04_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_04_on_3, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_off[] = {
    {50, &btlgrp_gra_gra_01_04_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_03_01_off[] = {
    {10, &btlgrp_gra_gra_03_01_off_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_03_01_off_2, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_03_04_off[] = {
    {50, &btlgrp_gra_gra_03_04_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_03_04_off_2, &gra_btlstage_data[1]},
    {40, &btlgrp_gra_gra_03_04_off_3, &gra_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_03_05_off[] = {
    {30, &btlgrp_gra_gra_03_05_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_03_05_off_2, &gra_btlstage_data[1]},
    {30, &btlgrp_gra_gra_03_05_off_3, &gra_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_04_02_off[] = {
    {60, &btlgrp_gra_gra_04_02_off_1, &gra_btlstage_data[1]},
    {30, &btlgrp_gra_gra_04_02_off_2, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_04_02_off_3, &gra_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_05_01_off[] = {
    {100, &btlgrp_gra_gra_05_01_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_05_01_off_2, &gra_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_05_03_off[] = {
    {50, &btlgrp_gra_gra_05_03_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_05_03_off_2, &gra_btlstage_data[1]},
    {40, &btlgrp_gra_gra_05_03_off_3, &gra_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_gra_tbl[] = {
    {
        /*"マップstg_04_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マップstg_04_1"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x34\x5F\x31", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_test2_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA00ランペルその１"*/"\x47\x52\x41\x30\x30\x83\x89\x83\x93\x83\x79\x83\x8B\x82\xBB\x82\xCC\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_00_01_off, // select
        0x20000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        /*"GRA00ランペルその２"*/"\x47\x52\x41\x30\x30\x83\x89\x83\x93\x83\x79\x83\x8B\x82\xBB\x82\xCC\x82\x51", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_00_02_off, // select
        0x20000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        /*"GRA01ハイパークリボー"*/"\x47\x52\x41\x30\x31\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2081), // swFlag
        btlgrpsel_gra_btlno_gra_01_01_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA01ハイパーパタクリ"*/"\x47\x52\x41\x30\x31\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2081), // swFlag
        btlgrpsel_gra_btlno_gra_01_03_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_03_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA01ハイパートゲクリ"*/"\x47\x52\x41\x30\x31\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x67\x83\x51\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2081), // swFlag
        btlgrpsel_gra_btlno_gra_01_04_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_04_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA03ハイパークリボー"*/"\x47\x52\x41\x30\x33\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA03パンジーさん"*/"\x47\x52\x41\x30\x33\x83\x70\x83\x93\x83\x57\x81\x5B\x82\xB3\x82\xF1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_04_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA03パンジーさん"*/"\x47\x52\x41\x30\x33\x83\x70\x83\x93\x83\x57\x81\x5B\x82\xB3\x82\xF1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_05_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA04パンジーさん"*/"\x47\x52\x41\x30\x34\x83\x70\x83\x93\x83\x57\x81\x5B\x82\xB3\x82\xF1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_04_02_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA05ハイパーシンエモン"*/"\x47\x52\x41\x30\x35\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_05_01_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"GRA05ハイパーシンエモン"*/"\x47\x52\x41\x30\x35\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_05_03_off, // select
        0, 0, // battleFlags..field_1C
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_gra_setup_no_tbl[] = {
    {"BTLNO_GRA_TEST1", 0},
    {"BTLNO_GRA_TEST2", 1},
    {"BTLNO_GRA_00_01", 2},
    {"BTLNO_GRA_00_02", 3},
    {"BTLNO_GRA_01_01", 4},
    {"BTLNO_GRA_01_03", 5},
    {"BTLNO_GRA_01_04", 6},
    {"BTLNO_GRA_03_01", 7},
    {"BTLNO_GRA_03_04", 8},
    {"BTLNO_GRA_03_05", 9},
    {"BTLNO_GRA_04_02", 10},
    {"BTLNO_GRA_05_01", 11},
    {"BTLNO_GRA_05_03", 12},
    {0},
};

