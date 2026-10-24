#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "bom.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"


USER_FUNC(_snow_control);

f32 fd_battle_database_bom_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_bom_neg_one_382;
f32 fd_battle_database_bom___local_PI_383;
f32 fd_battle_database_bom___local_3PIO2_385;
f32 fd_battle_database_bom_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_bom___local_PIO2_384;
f32 fd_battle_database_bom_one_381;
f32 fd_battle_database_bom_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_bom_neg_one_365;
f32 fd_battle_database_bom___local_PI_366;
f32 fd_battle_database_bom___local_3PIO2_368;
f32 fd_battle_database_bom_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_bom___local_PIO2_367;
f32 fd_battle_database_bom_one_364;
BattleStageObjectData bom_btlobject_data[11];
BattleStageData bom_btlstage_data[1];
BattleUnitSetup btlparty_bom_bom_00_01_off_1[1];
BattleGroupSetup btlgrp_bom_bom_00_01_off_1;
BattleUnitSetup btlparty_bom_bom_03_01_off_1[2];
BattleGroupSetup btlgrp_bom_bom_03_01_off_1;
BattleUnitSetup btlparty_bom_bom_03_01_off_2[3];
BattleGroupSetup btlgrp_bom_bom_03_01_off_2;
BattleUnitSetup btlparty_bom_bom_03_03_off_1[2];
BattleGroupSetup btlgrp_bom_bom_03_03_off_1;
BattleUnitSetup btlparty_bom_bom_03_03_off_2[3];
BattleGroupSetup btlgrp_bom_bom_03_03_off_2;
BattleUnitSetup btlparty_bom_bom_04_01_off_1[2];
BattleGroupSetup btlgrp_bom_bom_04_01_off_1;
BattleUnitSetup btlparty_bom_bom_04_01_off_2[3];
BattleGroupSetup btlgrp_bom_bom_04_01_off_2;
BattleUnitSetup btlparty_bom_bom_04_02_off_1[2];
BattleGroupSetup btlgrp_bom_bom_04_02_off_1;
BattleUnitSetup btlparty_bom_bom_04_02_off_2[3];
BattleGroupSetup btlgrp_bom_bom_04_02_off_2;
BattleUnitSetup btlparty_bom_bom_04_03_off_1[4];
BattleGroupSetup btlgrp_bom_bom_04_03_off_1;
NpcBattleSelect btlgrpsel_bom_btlno_bom_test1_off[2];
NpcBattleSelect btlgrpsel_bom_btlno_bom_03_01_off[3];
NpcBattleSelect btlgrpsel_bom_btlno_bom_03_02_off[3];
NpcBattleSelect btlgrpsel_bom_btlno_bom_03_03_off[3];
NpcBattleSelect btlgrpsel_bom_btlno_bom_04_01_off[3];
NpcBattleSelect btlgrpsel_bom_btlno_bom_04_02_off[3];
NpcBattleSelect btlgrpsel_bom_btlno_bom_04_03_off[2];
NpcSetupBattleInfo btlsetup_bom_tbl[8];
NpcTerritoryInfo btl_bom_setup_no_tbl[8];
EvtWord bgset_bom_snow_event[4];

f32 fd_battle_database_bom_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_bom_neg_one_382 = -1.0f;

f32 fd_battle_database_bom___local_PI_383 = 3.1415927f;

f32 fd_battle_database_bom___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_bom_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_bom___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_bom_one_381 = 1.0f;

f32 fd_battle_database_bom_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_bom_neg_one_365 = -1.0f;

f32 fd_battle_database_bom___local_PI_366 = 3.1415927f;

f32 fd_battle_database_bom___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_bom_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_bom___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_bom_one_364 = 1.0f;

BattleStageObjectData bom_btlobject_data[] = {
    {"yuki_front", 2, 1, {0.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"yuki_back", 2, 1, {0.0f, 0.0f, -40.0f}, 10, 50, {0}},
    {"kusa_01", 2, 1, {-167.0f, 0.0f, -21.0f}, 20, 40, {0}},
    {"kusa_02", 2, 1, {162.0f, 0.0f, -11.0f}, 30, 30, {0}},
    {"kabe_01", 2, 1, {194.0f, 0.0f, 66.0f}, 0, 60, {0}},
    {"kabe_02", 2, 1, {111.0f, 0.0f, -25.0f}, 10, 50, {0}},
    {"kabe_03", 2, 0, {-71.0f, 0.0f, -60.0f}, 0, 60, {0}},
    {"ki_01", 2, 0, {185.0f, 0.0f, -55.0f}, 10, 50, {0}},
    {"ki_02", 2, 0, {76.0f, 0.0f, -50.0f}, 20, 40, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData bom_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_07_0", // mSpecificStageDataDir
        11, // mNumObjects
        bom_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgBEvt
        bgset_bom_snow_event, // bgCEvt
        0, // reserved_1B0
        1, // disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

BattleUnitSetup btlparty_bom_bom_00_01_off_1[] = {
    {
        &unit_kuriboo, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_default_param_list, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_00_01_off_1 = {
    1, // numEnemies
    btlparty_bom_bom_00_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_03_01_off_1[] = {
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_03_01_off_1 = {
    2, // numEnemies
    btlparty_bom_bom_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_03_01_off_2[] = {
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_03_01_off_2 = {
    3, // numEnemies
    btlparty_bom_bom_03_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_03_03_off_1[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_03_03_off_1 = {
    2, // numEnemies
    btlparty_bom_bom_03_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_03_03_off_2[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_03_03_off_2 = {
    3, // numEnemies
    btlparty_bom_bom_03_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_04_01_off_1[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_04_01_off_1 = {
    2, // numEnemies
    btlparty_bom_bom_04_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_04_01_off_2[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_04_01_off_2 = {
    3, // numEnemies
    btlparty_bom_bom_04_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_04_02_off_1[] = {
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_04_02_off_1 = {
    2, // numEnemies
    btlparty_bom_bom_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_04_02_off_2[] = {
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_04_02_off_2 = {
    3, // numEnemies
    btlparty_bom_bom_04_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_bom_bom_04_03_off_1[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_bom_bom_04_03_off_1 = {
    4, // numEnemies
    btlparty_bom_bom_04_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_test1_off[] = {
    {50, &btlgrp_bom_bom_00_01_off_1, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_03_01_off[] = {
    {20, &btlgrp_bom_bom_03_01_off_1, bom_btlstage_data},
    {10, &btlgrp_bom_bom_03_01_off_2, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_03_02_off[] = {
    {10, &btlgrp_bom_bom_03_01_off_1, bom_btlstage_data},
    {20, &btlgrp_bom_bom_03_01_off_2, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_03_03_off[] = {
    {10, &btlgrp_bom_bom_03_03_off_1, bom_btlstage_data},
    {10, &btlgrp_bom_bom_03_03_off_2, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_04_01_off[] = {
    {10, &btlgrp_bom_bom_04_01_off_1, bom_btlstage_data},
    {10, &btlgrp_bom_bom_04_01_off_2, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_04_02_off[] = {
    {10, &btlgrp_bom_bom_04_02_off_1, bom_btlstage_data},
    {10, &btlgrp_bom_bom_04_02_off_2, bom_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_bom_btlno_bom_04_03_off[] = {
    {10, &btlgrp_bom_bom_04_03_off_1, bom_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_bom_tbl[] = {
    {
        /*"マップstg_07_0"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x37\x5F\x30", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom03ブリザード"*/"\x62\x6F\x6D\x30\x33\x83\x75\x83\x8A\x83\x55\x81\x5B\x83\x68", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom03ブリザード"*/"\x62\x6F\x6D\x30\x33\x83\x75\x83\x8A\x83\x55\x81\x5B\x83\x68", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_03_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom03アイスパックン"*/"\x62\x6F\x6D\x30\x33\x83\x41\x83\x43\x83\x58\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_03_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom04アイスパックン"*/"\x62\x6F\x6D\x30\x34\x83\x41\x83\x43\x83\x58\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_04_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom04ブリザード"*/"\x62\x6F\x6D\x30\x34\x83\x75\x83\x8A\x83\x55\x81\x5B\x83\x68", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_04_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"bom04アイスパックン"*/"\x62\x6F\x6D\x30\x34\x83\x41\x83\x43\x83\x58\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_bom_btlno_bom_04_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_bom_setup_no_tbl[] = {
    {"BTLNO_BOM_TEST1", 0},
    {"BTLNO_BOM_03_01", 1},
    {"BTLNO_BOM_03_02", 2},
    {"BTLNO_BOM_03_03", 3},
    {"BTLNO_BOM_04_01", 4},
    {"BTLNO_BOM_04_02", 5},
    {"BTLNO_BOM_04_03", 6},
    {0},
};

EVT_BEGIN(bgset_bom_snow_event)
    CALL(_snow_control)
    RETURN
EVT_END()

struct EffEntry* effSnowfallN64Entry(s32 type, s32 count);

USER_FUNC(_snow_control) {
    if (isFirstCall) {
        effSnowfallN64Entry(0, 64);
    }
    return EVT_RETURN_BLOCK;
}
