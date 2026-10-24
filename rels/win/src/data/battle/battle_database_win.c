#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "win.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"


extern BattleUnitKind unit_boss_vivian;

f32 fd_battle_database_win_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_win_neg_one_382;
f32 fd_battle_database_win___local_PI_383;
f32 fd_battle_database_win___local_3PIO2_385;
f32 fd_battle_database_win_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_win___local_PIO2_384;
f32 fd_battle_database_win_one_381;
f32 fd_battle_database_win_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_win_neg_one_365;
f32 fd_battle_database_win___local_PI_366;
f32 fd_battle_database_win___local_3PIO2_368;
f32 fd_battle_database_win_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_win___local_PIO2_367;
f32 fd_battle_database_win_one_364;
BattleStageObjectData win_btlobject_data[5];
BattleStageData win_btlstage_data[1];
BattleUnitSetup btlparty_win_win_00_01_off_1[2];
BattleGroupSetup btlgrp_win_win_00_01_off_1;
BattleUnitSetup btlparty_win_win_00_01_on_1[2];
BattleGroupSetup btlgrp_win_win_00_01_on_1;
BattleUnitSetup btlparty_win_win_00_01_on_2[3];
BattleGroupSetup btlgrp_win_win_00_01_on_2;
BattleUnitSetup btlparty_win_win_00_01_on_3[4];
BattleGroupSetup btlgrp_win_win_00_01_on_3;
BattleUnitSetup btlparty_win_win_00_02_off_1[2];
BattleGroupSetup btlgrp_win_win_00_02_off_1;
BattleUnitSetup btlparty_win_win_00_02_on_1[2];
BattleGroupSetup btlgrp_win_win_00_02_on_1;
BattleUnitSetup btlparty_win_win_00_02_on_2[3];
BattleGroupSetup btlgrp_win_win_00_02_on_2;
BattleUnitSetup btlparty_win_win_00_03_off_1[3];
BattleGroupSetup btlgrp_win_win_00_03_off_1;
BattleUnitSetup btlparty_win_win_00_03_on_1[2];
BattleGroupSetup btlgrp_win_win_00_03_on_1;
BattleUnitSetup btlparty_win_win_00_03_on_2[2];
BattleGroupSetup btlgrp_win_win_00_03_on_2;
BattleUnitSetup btlparty_win_win_00_04_off_1[1];
BattleGroupSetup btlgrp_win_win_00_04_off_1;
BattleUnitSetup btlparty_win_win_02_01_off_1[2];
BattleGroupSetup btlgrp_win_win_02_01_off_1;
BattleUnitSetup btlparty_win_win_02_01_on_1[2];
BattleGroupSetup btlgrp_win_win_02_01_on_1;
BattleUnitSetup btlparty_win_win_02_01_on_2[3];
BattleGroupSetup btlgrp_win_win_02_01_on_2;
BattleUnitSetup btlparty_win_win_02_01_on_3[3];
BattleGroupSetup btlgrp_win_win_02_01_on_3;
BattleUnitSetup btlparty_win_win_02_02_off_1[2];
BattleGroupSetup btlgrp_win_win_02_02_off_1;
BattleUnitSetup btlparty_win_win_02_02_on_1[2];
BattleGroupSetup btlgrp_win_win_02_02_on_1;
BattleUnitSetup btlparty_win_win_02_02_on_2[3];
BattleGroupSetup btlgrp_win_win_02_02_on_2;
BattleUnitSetup btlparty_win_win_02_02_on_3[3];
BattleGroupSetup btlgrp_win_win_02_02_on_3;
BattleUnitSetup btlparty_win_win_03_01_off_1[4];
BattleGroupSetup btlgrp_win_win_03_01_off_1;
BattleUnitSetup btlparty_win_win_03_01_on_1[2];
BattleGroupSetup btlgrp_win_win_03_01_on_1;
BattleUnitSetup btlparty_win_win_03_01_on_2[3];
BattleGroupSetup btlgrp_win_win_03_01_on_2;
BattleUnitSetup btlparty_win_win_03_01_on_3[3];
BattleGroupSetup btlgrp_win_win_03_01_on_3;
BattleUnitSetup btlparty_win_win_03_01_on_4[4];
BattleGroupSetup btlgrp_win_win_03_01_on_4;
NpcBattleSelect btlgrpsel_win_btlno_win_test1_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_00_01_on[4];
NpcBattleSelect btlgrpsel_win_btlno_win_00_01_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_00_02_on[3];
NpcBattleSelect btlgrpsel_win_btlno_win_00_02_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_00_03_on[3];
NpcBattleSelect btlgrpsel_win_btlno_win_00_03_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_00_04_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_02_01_on[4];
NpcBattleSelect btlgrpsel_win_btlno_win_02_01_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_02_02_on[4];
NpcBattleSelect btlgrpsel_win_btlno_win_02_02_off[2];
NpcBattleSelect btlgrpsel_win_btlno_win_03_01_on[5];
NpcBattleSelect btlgrpsel_win_btlno_win_03_01_off[2];
NpcSetupBattleInfo btlsetup_win_tbl[9];
NpcTerritoryInfo btl_win_setup_no_tbl[9];

f32 fd_battle_database_win_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_win_neg_one_382 = -1.0f;

f32 fd_battle_database_win___local_PI_383 = 3.1415927f;

f32 fd_battle_database_win___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_win_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_win___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_win_one_381 = 1.0f;

f32 fd_battle_database_win_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_win_neg_one_365 = -1.0f;

f32 fd_battle_database_win___local_PI_366 = 3.1415927f;

f32 fd_battle_database_win___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_win_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_win___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_win_one_364 = 1.0f;

BattleStageObjectData win_btlobject_data[] = {
    {"A1", 2, 0, {92.0f, 0.0f, -46.0f}, 0, 60, {0}},
    {"S_hana", 2, 1, {2.0f, 0.0f, -46.0f}, 0, 60, {0}},
    {"S_kusa", 2, 1, {2.0f, 0.0f, 64.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData win_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_02_0", // mSpecificStageDataDir
        5, // mNumObjects
        win_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x210006F, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_win_win_00_01_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_01_off_1 = {
    2, // numEnemies
    btlparty_win_win_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_01_on_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_01_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_01_on_2[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_01_on_2 = {
    3, // numEnemies
    btlparty_win_win_00_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_01_on_3[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_01_on_3 = {
    4, // numEnemies
    btlparty_win_win_00_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_02_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_02_off_1 = {
    2, // numEnemies
    btlparty_win_win_00_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_02_on_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_02_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_02_on_2[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_02_on_2 = {
    3, // numEnemies
    btlparty_win_win_00_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_03_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_03_off_1 = {
    3, // numEnemies
    btlparty_win_win_00_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_03_on_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_03_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_03_on_2[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_03_on_2 = {
    2, // numEnemies
    btlparty_win_win_00_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_00_04_off_1[] = {
    {
        &unit_boss_vivian, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {55.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_vivian1, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_00_04_off_1 = {
    1, // numEnemies
    btlparty_win_win_00_04_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_01_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_01_off_1 = {
    2, // numEnemies
    btlparty_win_win_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_01_on_1 = {
    2, // numEnemies
    btlparty_win_win_02_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_2[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_01_on_2 = {
    3, // numEnemies
    btlparty_win_win_02_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_3[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_01_on_3 = {
    3, // numEnemies
    btlparty_win_win_02_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_02_off_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_02_off_1 = {
    2, // numEnemies
    btlparty_win_win_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_02_on_1 = {
    2, // numEnemies
    btlparty_win_win_02_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_2[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_02_on_2 = {
    3, // numEnemies
    btlparty_win_win_02_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_3[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_02_02_on_3 = {
    3, // numEnemies
    btlparty_win_win_02_02_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_03_01_off_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_03_01_off_1 = {
    4, // numEnemies
    btlparty_win_win_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_03_01_on_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_03_01_on_1 = {
    2, // numEnemies
    btlparty_win_win_03_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_03_01_on_2[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_03_01_on_2 = {
    3, // numEnemies
    btlparty_win_win_03_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_03_01_on_3[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_03_01_on_3 = {
    3, // numEnemies
    btlparty_win_win_03_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_win_win_03_01_on_4[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_win_win_03_01_on_4 = {
    4, // numEnemies
    btlparty_win_win_03_01_on_4, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_win_btlno_win_test1_off[] = {
    {50, &btlgrp_win_win_00_01_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_01_on[] = {
    {20, &btlgrp_win_win_00_01_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_01_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_00_01_on_3, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_01_off[] = {
    {10, &btlgrp_win_win_00_01_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_02_on[] = {
    {20, &btlgrp_win_win_00_02_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_02_on_2, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_02_off[] = {
    {10, &btlgrp_win_win_00_02_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_03_on[] = {
    {20, &btlgrp_win_win_00_03_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_03_on_2, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_03_off[] = {
    {10, &btlgrp_win_win_00_03_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_00_04_off[] = {
    {50, &btlgrp_win_win_00_04_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_02_01_on[] = {
    {20, &btlgrp_win_win_02_01_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_02_01_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_02_01_on_3, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_02_01_off[] = {
    {10, &btlgrp_win_win_02_01_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_02_02_on[] = {
    {20, &btlgrp_win_win_02_02_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_02_02_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_02_02_on_3, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_02_02_off[] = {
    {10, &btlgrp_win_win_02_02_off_1, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_03_01_on[] = {
    {10, &btlgrp_win_win_03_01_on_1, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_3, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_4, win_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_win_btlno_win_03_01_off[] = {
    {10, &btlgrp_win_win_03_01_off_1, win_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_win_tbl[] = {
    {
        /*"マップstg_02_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x32\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_win_btlno_win_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00ドクガッスン１"*/"\x77\x69\x6E\x5F\x30\x30\x83\x68\x83\x4E\x83\x4B\x83\x62\x83\x58\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2682), // swFlag
        btlgrpsel_win_btlno_win_00_01_on, // altSelect
        btlgrpsel_win_btlno_win_00_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00モノクロシンエモン"*/"\x77\x69\x6E\x5F\x30\x30\x83\x82\x83\x6D\x83\x4E\x83\x8D\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2682), // swFlag
        btlgrpsel_win_btlno_win_00_02_on, // altSelect
        btlgrpsel_win_btlno_win_00_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00ドクガッスン３"*/"\x77\x69\x6E\x5F\x30\x30\x83\x68\x83\x4E\x83\x4B\x83\x62\x83\x58\x83\x93\x82\x52", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2682), // swFlag
        btlgrpsel_win_btlno_win_00_03_on, // altSelect
        btlgrpsel_win_btlno_win_00_03_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win00ビビアン３姉妹"*/"\x77\x69\x6E\x30\x30\x83\x72\x83\x72\x83\x41\x83\x93\x82\x52\x8E\x6F\x96\x85", // name
        "win_00", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_win_btlno_win_00_04_off, // select
        0x20000010, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_3WOMEN1", // bgmName
    },
    {
        /*"win_02シンエモン１"*/"\x77\x69\x6E\x5F\x30\x32\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2680), // swFlag
        btlgrpsel_win_btlno_win_02_01_on, // altSelect
        btlgrpsel_win_btlno_win_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_02パックンフラワー"*/"\x77\x69\x6E\x5F\x30\x32\x83\x70\x83\x62\x83\x4E\x83\x93\x83\x74\x83\x89\x83\x8F\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2680), // swFlag
        btlgrpsel_win_btlno_win_02_02_on, // altSelect
        btlgrpsel_win_btlno_win_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_03パックン１"*/"\x77\x69\x6E\x5F\x30\x33\x83\x70\x83\x62\x83\x4E\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF(2681), // swFlag
        btlgrpsel_win_btlno_win_03_01_on, // altSelect
        btlgrpsel_win_btlno_win_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_win_setup_no_tbl[] = {
    {"BTLNO_WIN_TEST1", 0},
    {"BTLNO_WIN_00_01", 1},
    {"BTLNO_WIN_00_02", 2},
    {"BTLNO_WIN_00_03", 3},
    {"BTLNO_WIN_00_04", 4},
    {"BTLNO_WIN_02_01", 5},
    {"BTLNO_WIN_02_02", 6},
    {"BTLNO_WIN_03_01", 7},
    {0},
};

