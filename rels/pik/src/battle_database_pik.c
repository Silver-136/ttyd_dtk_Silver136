#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "pik.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"


f32 fd_battle_database_pik_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_battle_database_pik_neg_one_382;
f32 fd_battle_database_pik___local_PI_383;
f32 fd_battle_database_pik___local_3PIO2_385;
f32 fd_battle_database_pik_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_battle_database_pik___local_PIO2_384;
f32 fd_battle_database_pik_one_381;
f32 fd_battle_database_pik_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_battle_database_pik_neg_one_365;
f32 fd_battle_database_pik___local_PI_366;
f32 fd_battle_database_pik___local_3PIO2_368;
f32 fd_battle_database_pik_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_battle_database_pik___local_PIO2_367;
f32 fd_battle_database_pik_one_364;
BattleStageObjectData pik_btlobject_data[2];
BattleStageData pik_btlstage_data[1];
BattleUnitSetup btlparty_pik_pik_06_01_off_1[2];
BattleGroupSetup btlgrp_pik_pik_06_01_off_1;
BattleUnitSetup btlparty_pik_pik_06_02_off_1[3];
BattleGroupSetup btlgrp_pik_pik_06_02_off_1;
BattleUnitSetup btlparty_pik_pik_06_03_off_1[1];
BattleGroupSetup btlgrp_pik_pik_06_03_off_1;
BattleUnitSetup btlparty_pik_pik_06_04_off_1[2];
BattleGroupSetup btlgrp_pik_pik_06_04_off_1;
BattleUnitSetup btlparty_pik_pik_06_05_off_1[2];
BattleGroupSetup btlgrp_pik_pik_06_05_off_1;
BattleUnitSetup btlparty_pik_pik_06_06_off_1[4];
BattleGroupSetup btlgrp_pik_pik_06_06_off_1;
BattleUnitSetup btlparty_pik_pik_06_07_off_1[1];
BattleGroupSetup btlgrp_pik_pik_06_07_off_1;
BattleUnitSetup btlparty_pik_pik_06_08_off_1[3];
BattleGroupSetup btlgrp_pik_pik_06_08_off_1;
BattleUnitSetup btlparty_pik_pik_06_09_off_1[4];
BattleGroupSetup btlgrp_pik_pik_06_09_off_1;
NpcBattleSelect btlgrpsel_pik_btlno_pik_test1_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_01_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_02_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_03_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_04_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_05_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_06_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_07_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_08_off[2];
NpcBattleSelect btlgrpsel_pik_btlno_pik_03_09_off[2];
NpcSetupBattleInfo btlsetup_pik_tbl[11];
NpcTerritoryInfo btl_pik_setup_no_tbl[11];

f32 fd_battle_database_pik_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_pik_neg_one_382 = -1.0f;

f32 fd_battle_database_pik___local_PI_383 = 3.1415927f;

f32 fd_battle_database_pik___local_3PIO2_385 = 4.712389f;

f32 fd_battle_database_pik_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_pik___local_PIO2_384 = 1.5707964f;

f32 fd_battle_database_pik_one_381 = 1.0f;

f32 fd_battle_database_pik_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_battle_database_pik_neg_one_365 = -1.0f;

f32 fd_battle_database_pik___local_PI_366 = 3.1415927f;

f32 fd_battle_database_pik___local_3PIO2_368 = 4.712389f;

f32 fd_battle_database_pik_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_battle_database_pik___local_PIO2_367 = 1.5707964f;

f32 fd_battle_database_pik_one_364 = 1.0f;

BattleStageObjectData pik_btlobject_data[] = {
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"A2", 2, 1, {0.0f, 0.0f, -74.0f}, 0, 60, {0}},
};

BattleStageData pik_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_06_4", // mSpecificStageDataDir
        2, // mNumObjects
        pik_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_pik_pik_06_01_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_01_off_1 = {
    2, // numEnemies
    btlparty_pik_pik_06_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_02_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_02_off_1 = {
    3, // numEnemies
    btlparty_pik_pik_06_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_03_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_03_off_1 = {
    1, // numEnemies
    btlparty_pik_pik_06_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_04_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_04_off_1 = {
    2, // numEnemies
    btlparty_pik_pik_06_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_05_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_05_off_1 = {
    2, // numEnemies
    btlparty_pik_pik_06_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_06_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_06_off_1 = {
    4, // numEnemies
    btlparty_pik_pik_06_06_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_07_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_07_off_1 = {
    1, // numEnemies
    btlparty_pik_pik_06_07_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_08_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_08_off_1 = {
    3, // numEnemies
    btlparty_pik_pik_06_08_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_pik_pik_06_09_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_pik_pik_06_09_off_1 = {
    4, // numEnemies
    btlparty_pik_pik_06_09_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_test1_off[] = {
    {50, &btlgrp_pik_pik_06_01_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_01_off[] = {
    {20, &btlgrp_pik_pik_06_01_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_02_off[] = {
    {20, &btlgrp_pik_pik_06_02_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_03_off[] = {
    {20, &btlgrp_pik_pik_06_03_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_04_off[] = {
    {20, &btlgrp_pik_pik_06_04_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_05_off[] = {
    {20, &btlgrp_pik_pik_06_05_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_06_off[] = {
    {20, &btlgrp_pik_pik_06_06_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_07_off[] = {
    {20, &btlgrp_pik_pik_06_07_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_08_off[] = {
    {20, &btlgrp_pik_pik_06_08_off_1, pik_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_pik_btlno_pik_03_09_off[] = {
    {20, &btlgrp_pik_pik_06_09_off_1, pik_btlstage_data},
    {0},
};

NpcSetupBattleInfo btlsetup_pik_tbl[] = {
    {
        /*"マップstg_06_4"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x36\x5F\x34", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_test1_off, // select
        0, 0, // battleFlags..field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_01_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_02_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_03_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_04_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_05_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_06_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_07_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_08_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"pik_03パープルテレサ"*/"\x70\x69\x6B\x5F\x30\x33\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_pik_btlno_pik_03_09_off, // select
        0, 0, // battleFlags..field_1C
        {{3, 3}, {3, 3}, {10, 10}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_pik_setup_no_tbl[] = {
    {"BTLNO_PIK_TEST1", 0},
    {"BTLNO_PIK_03_01", 1},
    {"BTLNO_PIK_03_02", 2},
    {"BTLNO_PIK_03_03", 3},
    {"BTLNO_PIK_03_04", 4},
    {"BTLNO_PIK_03_05", 5},
    {"BTLNO_PIK_03_06", 6},
    {"BTLNO_PIK_03_07", 7},
    {"BTLNO_PIK_03_08", 8},
    {"BTLNO_PIK_03_09", 9},
    {0},
};

