// Battle database of Boggly Woods (win.rel): stage data, enemy groups and the encounter tables of its maps.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "win.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_boss_vivian;

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

/**
 * Stage objects of the Boggly Woods battle stage stg_02_0: the A1 backdrop piece, flower (S_hana) and grass
 * (S_kusa) props, and the B and C background sets.
 */
BattleStageObjectData win_btlobject_data[] = {
    {"A1", 2, 0, {92.0f, 0.0f, -46.0f}, 0, 60, {0}},
    {"S_hana", 2, 1, {2.0f, 0.0f, -46.0f}, 0, 60, {0}},
    {"S_kusa", 2, 1, {2.0f, 0.0f, 64.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

/**
 * The one battle stage used by every win.rel fight: bti_01 shared data with the stg_02_0 scenery and the
 * objects above.
 */
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

/** Enemy lineup for the first win_00 encounter, default set (flag clear), variant 1: Dark Puff x2. */
BattleUnitSetup btlparty_win_win_00_01_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the first win_00 encounter, default set (flag clear), variant 1: Dark Puff x2, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_01_off_1 = {
    2, // numEnemies
    btlparty_win_win_00_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Dark Puff x2.
 */
BattleUnitSetup btlparty_win_win_00_01_on_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Dark Puff x2, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_01_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Dark Puff x3.
 */
BattleUnitSetup btlparty_win_win_00_01_on_2[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Dark Puff x3, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_01_on_2 = {
    3, // numEnemies
    btlparty_win_win_00_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 3: Dark Puff x4.
 */
BattleUnitSetup btlparty_win_win_00_01_on_3[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the first win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 3: Dark Puff x4, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_01_on_3 = {
    4, // numEnemies
    btlparty_win_win_00_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup for the second win_00 encounter, default set (flag clear), variant 1: Cleft, Dark Puff. */
BattleUnitSetup btlparty_win_win_00_02_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the second win_00 encounter, default set (flag clear), variant 1: Cleft, Dark Puff, with
 * its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_02_off_1 = {
    2, // numEnemies
    btlparty_win_win_00_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the second win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Cleft, Dark Puff.
 */
BattleUnitSetup btlparty_win_win_00_02_on_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the second win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Cleft, Dark Puff, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_02_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_02_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the second win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Cleft, Dark Puff x2.
 */
BattleUnitSetup btlparty_win_win_00_02_on_2[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the second win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Cleft, Dark Puff x2, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_02_on_2 = {
    3, // numEnemies
    btlparty_win_win_00_02_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup for the third win_00 encounter, default set (flag clear), variant 1: Dark Puff x2, Cleft. */
BattleUnitSetup btlparty_win_win_00_03_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the third win_00 encounter, default set (flag clear), variant 1: Dark Puff x2, Cleft,
 * with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_03_off_1 = {
    3, // numEnemies
    btlparty_win_win_00_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the third win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Dark Puff x2.
 */
BattleUnitSetup btlparty_win_win_00_03_on_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Battle group for the third win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 1: Dark Puff x2, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_03_on_1 = {
    2, // numEnemies
    btlparty_win_win_00_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the third win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Dark Puff, Cleft.
 */
BattleUnitSetup btlparty_win_win_00_03_on_2[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

/**
 * Battle group for the third win_00 encounter, alternate set (used once the Flurrie-joined story flag is
 * set), variant 2: Dark Puff, Cleft, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_03_on_2 = {
    2, // numEnemies
    btlparty_win_win_00_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup for the Vivian boss fight in win_00, default set (flag clear), variant 1: Vivian. */
BattleUnitSetup btlparty_win_win_00_04_off_1[] = {
    {
        &unit_boss_vivian, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {55.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_vivian1, // itemDropTable
    },
};

/**
 * Battle group for the Vivian boss fight in win_00, default set (flag clear), variant 1: Vivian, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_00_04_off_1 = {
    1, // numEnemies
    btlparty_win_win_00_04_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup for the first win_02 encounter, default set (flag clear), variant 1: Cleft, Pale Piranha. */
BattleUnitSetup btlparty_win_win_02_01_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

/**
 * Battle group for the first win_02 encounter, default set (flag clear), variant 1: Cleft, Pale Piranha,
 * with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_02_01_off_1 = {
    2, // numEnemies
    btlparty_win_win_02_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_2[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_01_on_3[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

/** Enemy lineup for the second win_02 encounter, default set (flag clear), variant 1: Pale Piranha, Cleft. */
BattleUnitSetup btlparty_win_win_02_02_off_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

/**
 * Battle group for the second win_02 encounter, default set (flag clear), variant 1: Pale Piranha, Cleft,
 * with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_02_02_off_1 = {
    2, // numEnemies
    btlparty_win_win_02_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_2[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

BattleUnitSetup btlparty_win_win_02_02_on_3[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
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
    5, // reserved_1C
};

/** Enemy lineup for the win_03 encounter, default set (flag clear), variant 1: Pale Piranha x4. */
BattleUnitSetup btlparty_win_win_03_01_off_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

/**
 * Battle group for the win_03 encounter, default set (flag clear), variant 1: Pale Piranha x4, with its
 * item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_03_01_off_1 = {
    4, // numEnemies
    btlparty_win_win_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 1: Pale Piranha x2.
 */
BattleUnitSetup btlparty_win_win_03_01_on_1[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

/**
 * Battle group for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 1: Pale Piranha x2, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_03_01_on_1 = {
    2, // numEnemies
    btlparty_win_win_03_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 2: Pale Piranha x3.
 */
BattleUnitSetup btlparty_win_win_03_01_on_2[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

/**
 * Battle group for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 2: Pale Piranha x3, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_03_01_on_2 = {
    3, // numEnemies
    btlparty_win_win_03_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 3: Pale Piranha, Dark Puff, Cleft.
 */
BattleUnitSetup btlparty_win_win_03_01_on_3[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_sinemon, // itemDropTable
    },
};

/**
 * Battle group for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 3: Pale Piranha, Dark Puff, Cleft, with its item drop odds and heart and flower drop
 * tables.
 */
BattleGroupSetup btlgrp_win_win_03_01_on_3 = {
    3, // numEnemies
    btlparty_win_win_03_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 4: Pale Piranha x4.
 */
BattleUnitSetup btlparty_win_win_03_01_on_4[] = {
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

/**
 * Battle group for the win_03 encounter, alternate set (used once the Flurrie-request-heard story flag is
 * set), variant 4: Pale Piranha x4, with its item drop odds and heart and flower drop tables.
 */
BattleGroupSetup btlgrp_win_win_03_01_on_4 = {
    4, // numEnemies
    btlparty_win_win_03_01_on_4, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Debug battle choice BTLNO_WIN_TEST1: the win_00 Dark Puff pair (btlgrp_win_win_00_01_off_1) on the Boggly
 * Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_test1_off[] = {
    {50, &btlgrp_win_win_00_01_off_1, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_01 as the alternate set (used once the Flurrie-joined story flag is set): 3
 * groups with weights 20, 20, 10, all on the Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_01_on[] = {
    {20, &btlgrp_win_win_00_01_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_01_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_00_01_on_3, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_01 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_01_off[] = {
    {10, &btlgrp_win_win_00_01_off_1, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_02 as the alternate set (used once the Flurrie-joined story flag is set): 2
 * groups with weights 20, 20, all on the Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_02_on[] = {
    {20, &btlgrp_win_win_00_02_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_02_on_2, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_02 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_02_off[] = {
    {10, &btlgrp_win_win_00_02_off_1, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_03 as the alternate set (used once the Flurrie-joined story flag is set): 2
 * groups with weights 20, 20, all on the Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_03_on[] = {
    {20, &btlgrp_win_win_00_03_on_1, win_btlstage_data},
    {20, &btlgrp_win_win_00_03_on_2, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_03 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_00_03_off[] = {
    {10, &btlgrp_win_win_00_03_off_1, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_00_04 as the default set (flag clear): 1 group with weight 50, all on the
 * Boggly Woods stage.
 */
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

/**
 * Group choice for BTLNO_WIN_02_01 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
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

/**
 * Group choice for BTLNO_WIN_02_02 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_02_02_off[] = {
    {10, &btlgrp_win_win_02_02_off_1, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_03_01 as the alternate set (used once the Flurrie-request-heard story flag is
 * set): 4 groups with weights 10, 10, 10, 10, all on the Boggly Woods stage.
 */
NpcBattleSelect btlgrpsel_win_btlno_win_03_01_on[] = {
    {10, &btlgrp_win_win_03_01_on_1, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_2, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_3, win_btlstage_data},
    {10, &btlgrp_win_win_03_01_on_4, win_btlstage_data},
    {0},
};

/**
 * Group choice for BTLNO_WIN_03_01 as the default set (flag clear): 1 group with weight 10, all on the
 * Boggly Woods stage.
 */
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
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00ドクガッスン１"*/"\x77\x69\x6E\x5F\x30\x30\x83\x68\x83\x4E\x83\x4B\x83\x62\x83\x58\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_FLURRIE_JOINED, // swFlag
        btlgrpsel_win_btlno_win_00_01_on, // altSelect
        btlgrpsel_win_btlno_win_00_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00モノクロシンエモン"*/"\x77\x69\x6E\x5F\x30\x30\x83\x82\x83\x6D\x83\x4E\x83\x8D\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_FLURRIE_JOINED, // swFlag
        btlgrpsel_win_btlno_win_00_02_on, // altSelect
        btlgrpsel_win_btlno_win_00_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_00ドクガッスン３"*/"\x77\x69\x6E\x5F\x30\x30\x83\x68\x83\x4E\x83\x4B\x83\x62\x83\x58\x83\x93\x82\x52", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_FLURRIE_JOINED, // swFlag
        btlgrpsel_win_btlno_win_00_03_on, // altSelect
        btlgrpsel_win_btlno_win_00_03_off, // select
        0, 0, // battleFlags..randomMode
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
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_BOSS_STG3_3WOMEN1", // bgmName
    },
    {
        /*"win_02シンエモン１"*/"\x77\x69\x6E\x5F\x30\x32\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_02_ALT_BATTLES_UNUSED, // swFlag
        btlgrpsel_win_btlno_win_02_01_on, // altSelect
        btlgrpsel_win_btlno_win_02_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_02パックンフラワー"*/"\x77\x69\x6E\x5F\x30\x32\x83\x70\x83\x62\x83\x4E\x83\x93\x83\x74\x83\x89\x83\x8F\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_02_ALT_BATTLES_UNUSED, // swFlag
        btlgrpsel_win_btlno_win_02_02_on, // altSelect
        btlgrpsel_win_btlno_win_02_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {10, 10}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"win_03パックン１"*/"\x77\x69\x6E\x5F\x30\x33\x83\x70\x83\x62\x83\x4E\x83\x93\x82\x50", // name
        NULL, 0, // mapName..fixedItem
        GSWF_WIN_FLURRIE_REQUEST_ACCEPTED, // swFlag
        btlgrpsel_win_btlno_win_03_01_on, // altSelect
        btlgrpsel_win_btlno_win_03_01_off, // select
        0, 0, // battleFlags..randomMode
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

