#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "battle/battle_stage_object.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "moo.h"
#include "battle/battle_stage.h"
#include "driver/npcdrv.h"

static USER_FUNC(_space_rotate);
extern BattleUnitKind unit_barriern_z;

f32 fd_battle_database_moo_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_moo_neg_one_382;
f32 fd_battle_database_moo___local_PI_383;
f32 fd_battle_database_moo___local_3PIO2_385;
f32 fd_battle_database_moo_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_moo___local_PIO2_384;
f32 fd_battle_database_moo_one_381;
f32 fd_battle_database_moo_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_moo_neg_one_365;
f32 fd_battle_database_moo___local_PI_366;
f32 fd_battle_database_moo___local_3PIO2_368;
f32 fd_battle_database_moo_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_moo___local_PIO2_367;
f32 fd_battle_database_moo_one_364;
BattleStageObjectData moo_btlobject_data[5];
BattleStageData moo_btlstage_data[1];
BattleUnitSetup btlparty_moo_moo_00_01_off_1[3];
BattleGroupSetup btlgrp_moo_moo_00_01_off_1;
BattleUnitSetup btlparty_moo_moo_01_01_off_1[2];
BattleGroupSetup btlgrp_moo_moo_01_01_off_1;
BattleUnitSetup btlparty_moo_moo_01_02_off_1[2];
BattleGroupSetup btlgrp_moo_moo_01_02_off_1;
BattleUnitSetup btlparty_moo_moo_01_02_off_2[3];
BattleGroupSetup btlgrp_moo_moo_01_02_off_2;
BattleUnitSetup btlparty_moo_moo_01_03_off_1[3];
BattleGroupSetup btlgrp_moo_moo_01_03_off_1;
BattleUnitSetup btlparty_moo_moo_01_04_off_1[2];
BattleGroupSetup btlgrp_moo_moo_01_04_off_1;
BattleUnitSetup btlparty_moo_moo_01_04_off_2[3];
BattleGroupSetup btlgrp_moo_moo_01_04_off_2;
BattleUnitSetup btlparty_moo_moo_01_04_off_3[4];
BattleGroupSetup btlgrp_moo_moo_01_04_off_3;
BattleUnitSetup btlparty_moo_moo_01_05_off_1[4];
BattleGroupSetup btlgrp_moo_moo_01_05_off_1;
BattleUnitSetup btlparty_moo_moo_02_01_off_1[2];
BattleGroupSetup btlgrp_moo_moo_02_01_off_1;
BattleUnitSetup btlparty_moo_moo_02_02_off_1[2];
BattleGroupSetup btlgrp_moo_moo_02_02_off_1;
BattleUnitSetup btlparty_moo_moo_02_03_off_1[2];
BattleGroupSetup btlgrp_moo_moo_02_03_off_1;
BattleUnitSetup btlparty_moo_moo_02_04_off_1[3];
BattleGroupSetup btlgrp_moo_moo_02_04_off_1;
BattleUnitSetup btlparty_moo_moo_02_05_off_1[2];
BattleGroupSetup btlgrp_moo_moo_02_05_off_1;
BattleUnitSetup btlparty_moo_moo_05_01_off_1[2];
BattleGroupSetup btlgrp_moo_moo_05_01_off_1;
BattleUnitSetup btlparty_moo_moo_05_01_off_2[3];
BattleGroupSetup btlgrp_moo_moo_05_01_off_2;
BattleUnitSetup btlparty_moo_moo_05_01_off_3[4];
BattleGroupSetup btlgrp_moo_moo_05_01_off_3;
BattleUnitSetup btlparty_moo_moo_05_02_off_1[2];
BattleGroupSetup btlgrp_moo_moo_05_02_off_1;
BattleUnitSetup btlparty_moo_moo_05_03_off_1[2];
BattleGroupSetup btlgrp_moo_moo_05_03_off_1;
BattleUnitSetup btlparty_moo_moo_05_03_off_2[3];
BattleGroupSetup btlgrp_moo_moo_05_03_off_2;
BattleUnitSetup btlparty_moo_moo_05_04_off_1[3];
BattleGroupSetup btlgrp_moo_moo_05_04_off_1;
BattleUnitSetup btlparty_moo_moo_05_05_off_1[2];
BattleGroupSetup btlgrp_moo_moo_05_05_off_1;
BattleUnitSetup btlparty_moo_moo_05_05_off_2[3];
BattleGroupSetup btlgrp_moo_moo_05_05_off_2;
BattleUnitSetup btlparty_moo_moo_05_05_off_3[4];
BattleGroupSetup btlgrp_moo_moo_05_05_off_3;
NpcBattleSelect btlgrpsel_moo_btlno_moo_test1_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_01_01_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_01_02_off[3];
NpcBattleSelect btlgrpsel_moo_btlno_moo_01_03_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_01_04_off[4];
NpcBattleSelect btlgrpsel_moo_btlno_moo_01_05_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_02_01_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_02_02_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_02_03_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_02_04_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_02_05_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_05_01_off[4];
NpcBattleSelect btlgrpsel_moo_btlno_moo_05_02_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_05_03_off[3];
NpcBattleSelect btlgrpsel_moo_btlno_moo_05_04_off[2];
NpcBattleSelect btlgrpsel_moo_btlno_moo_05_05_off[4];
NpcSetupBattleInfo btlsetup_moo_tbl[17];
NpcTerritoryInfo btl_moo_setup_no_tbl[17];
EvtWord bgset_moo_space_event[35];

f32 fd_battle_database_moo_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_moo_neg_one_382 = -1.0f;

f32 fd_battle_database_moo___local_PI_383 = 3.1415927f;

f32 fd_battle_database_moo___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_moo_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_moo___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_moo_one_381 = 1.0f;

f32 fd_battle_database_moo_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_moo_neg_one_365 = -1.0f;

f32 fd_battle_database_moo___local_PI_366 = 3.1415927f;

f32 fd_battle_database_moo___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_moo_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_moo___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_moo_one_364 = 1.0f;

BattleStageObjectData moo_btlobject_data[] = {
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"space", 2, 3, {0}, 0, 60, {0}},
    {"star_01", 2, 3, {0}, 0, 60, {0}},
    {"star_02", 2, 3, {0}, 0, 60, {0}},
    {"star_03", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData moo_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_4", // mSpecificStageDataDir
        5, // mNumObjects
        moo_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_moo_space_event, // bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_moo_moo_00_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_00_01_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_01_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_02_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_02_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_02_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_01_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_03_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_03_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_04_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_04_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_01_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_04_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_04_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_01_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_04_off_3[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_04_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_01_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_01_05_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_01_05_off_1 = {
    4, // numEnemies
    btlparty_moo_moo_01_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_02_01_off_1[] = {
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

BattleGroupSetup btlgrp_moo_moo_02_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_02_02_off_1[] = {
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
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_02_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_02_03_off_1[] = {
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

BattleGroupSetup btlgrp_moo_moo_02_03_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_heart_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_02_04_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_02_04_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_02_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_02_05_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_barriern_z, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern_z, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_02_05_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_02_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_01_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_01_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_01_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_01_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_01_off_3[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_01_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_05_01_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_02_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_02_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_03_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_03_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_03_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_03_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_04_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_04_off_1 = {
    3, // numEnemies
    btlparty_moo_moo_05_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_05_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_05_off_1 = {
    2, // numEnemies
    btlparty_moo_moo_05_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_05_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_05_off_2 = {
    3, // numEnemies
    btlparty_moo_moo_05_05_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_moo_moo_05_05_off_3[] = {
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_moo_moo_05_05_off_3 = {
    4, // numEnemies
    btlparty_moo_moo_05_05_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_test1_off[] = {
    {50, &btlgrp_moo_moo_00_01_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_01_01_off[] = {
    {10, &btlgrp_moo_moo_01_01_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_01_02_off[] = {
    {10, &btlgrp_moo_moo_01_02_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_01_02_off_2, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_01_03_off[] = {
    {10, &btlgrp_moo_moo_01_03_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_01_04_off[] = {
    {10, &btlgrp_moo_moo_01_04_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_01_04_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_01_04_off_3, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_01_05_off[] = {
    {5, &btlgrp_moo_moo_01_05_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_02_01_off[] = {
    {20, &btlgrp_moo_moo_02_01_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_02_02_off[] = {
    {20, &btlgrp_moo_moo_02_02_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_02_03_off[] = {
    {20, &btlgrp_moo_moo_02_03_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_02_04_off[] = {
    {20, &btlgrp_moo_moo_02_04_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_02_05_off[] = {
    {20, &btlgrp_moo_moo_02_05_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_05_01_off[] = {
    {10, &btlgrp_moo_moo_05_01_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_01_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_05_01_off_3, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_05_02_off[] = {
    {20, &btlgrp_moo_moo_05_02_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_05_03_off[] = {
    {10, &btlgrp_moo_moo_05_03_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_03_off_2, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_05_04_off[] = {
    {20, &btlgrp_moo_moo_05_04_off_1, moo_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_moo_btlno_moo_05_05_off[] = {
    {10, &btlgrp_moo_moo_05_05_off_1, moo_btlstage_data},
    {10, &btlgrp_moo_moo_05_05_off_2, moo_btlstage_data},
    {5, &btlgrp_moo_moo_05_05_off_3, moo_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_moo_tbl[] = {
    {
        /*"マップstg_07_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo01シンエモン"*/"\x6D\x6F\x6F\x30\x31\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo01シンエモン"*/"\x6D\x6F\x6F\x30\x31\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo01シンエモン"*/"\x6D\x6F\x6F\x30\x31\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo01シンエモン"*/"\x6D\x6F\x6F\x30\x31\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo01シンエモン"*/"\x6D\x6F\x6F\x30\x31\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_01_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo02バリアーンZ"*/"\x6D\x6F\x6F\x30\x32\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x5A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo02バリアーンZ"*/"\x6D\x6F\x6F\x30\x32\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x5A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo02バリアーンZ"*/"\x6D\x6F\x6F\x30\x32\x83\x6F\x83\x8A\x83\x41\x81\x5B\x83\x93\x5A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo02シンエモン"*/"\x6D\x6F\x6F\x30\x32\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo02シンエモン"*/"\x6D\x6F\x6F\x30\x32\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_02_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo05シンエモン"*/"\x6D\x6F\x6F\x30\x35\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo05シンエモン"*/"\x6D\x6F\x6F\x30\x35\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo05シンエモン"*/"\x6D\x6F\x6F\x30\x35\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo05シンエモン"*/"\x6D\x6F\x6F\x30\x35\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"moo05シンエモン"*/"\x6D\x6F\x6F\x30\x35\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_moo_btlno_moo_05_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_moo_setup_no_tbl[] = {
    {"BTLNO_MOO_TEST1", 0},
    {"BTLNO_MOO_01_01", 1},
    {"BTLNO_MOO_01_02", 2},
    {"BTLNO_MOO_01_03", 3},
    {"BTLNO_MOO_01_04", 4},
    {"BTLNO_MOO_01_05", 5},
    {"BTLNO_MOO_02_01", 6},
    {"BTLNO_MOO_02_02", 7},
    {"BTLNO_MOO_02_03", 8},
    {"BTLNO_MOO_02_04", 9},
    {"BTLNO_MOO_02_05", 10},
    {"BTLNO_MOO_05_01", 11},
    {"BTLNO_MOO_05_02", 12},
    {"BTLNO_MOO_05_03", 13},
    {"BTLNO_MOO_05_04", 14},
    {"BTLNO_MOO_05_05", 15},
    {0},
};

EVT_BEGIN(bgset_moo_space_event)
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
