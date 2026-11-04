// Battle database of Twilight Trail (gra.rel): stage data, enemy groups and the encounter tables of its maps.

#include "version.h"
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

/**
 * Scenery objects of the Twilight Trail battle stages (trees, rocks, grass and the back and front wall
 * pieces); each stage in gra_btlstage_data uses a slice of this list.
 */
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
#if VERSION <= VERSION_DEMO_JP
    {"kusamura_usiro", 2, 1, {0.0f, 0.0f, -70.0f}, 10, 50, {0}},
#else
    {"kusamura_usiro", 2, 1, {0.0f, 0.0f, -65.0f}, 10, 50, {0}},
#endif
    {"iwa", 2, 1, {0.0f, 0.0f, -71.0f}, 20, 40, {0}},
    {"ki_1", 2, 0, {100.0f, 0.0f, -51.0f}, 0, 60, {0}},
    {"ki_2", 2, 0, {-123.0f, 0.0f, -48.0f}, 10, 50, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"murasaki", 2, 3, {0}, 0, 60, {0}},
    {"ao", 2, 3, {0}, 0, 60, {0}},
};

/**
 * The 2 battle stages of Twilight Trail: each pairs a stage model (stg_04_0, stg_04_1) with a slice of the
 * scenery objects above.
 */
BattleStageData gra_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_04_0", // specificStageDataDir
        10, // numObjects
        gra_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_04_1", // specificStageDataDir
        11, // numObjects
        &gra_btlobject_data[10], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, // reserved_1B0..disableBgA1Destroy
        1, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

/**
 * Enemy lineup of formation 1 of BTLNO_GRA_00_01 and BTLNO_GRA_00_02, in Twilight Trail (gra_00): Doopliss
 * disguised as Mario.
 */
BattleUnitSetup btlparty_gra_gra_00_01_off_1[] = {
    {
        &unit_faker_mario_gra, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_faker_mario, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_00_01 and BTLNO_GRA_00_02: the lineup above with the default heart and flower
 * drops. Item drop weights: 0 held, 0 random, 100 none.
 */
BattleGroupSetup btlgrp_gra_gra_00_01_off_1 = {
    1, // numEnemies
    btlparty_gra_gra_00_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_01_01, in Twilight Trail (gra_01): Hyper Goomba x3. */
BattleUnitSetup btlparty_gra_gra_01_01_off_1[] = {
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_01_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_GRA_01_01, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Goomba x3.
 */
BattleUnitSetup btlparty_gra_gra_01_01_on_1[] = {
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_01_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_01_01, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Goomba, Hyper Paragoomba, Crazee Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_01_01_on_2[] = {
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_01_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_01_on_2 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_01_01, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Goomba, Hyper Spiky Goomba, Crazee Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_01_01_on_3[] = {
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_01_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_01_on_3 = {
    3, // numEnemies
    btlparty_gra_gra_01_01_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_01_03, in Twilight Trail (gra_01): Hyper Paragoomba x3. */
BattleUnitSetup btlparty_gra_gra_01_03_off_1[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_03_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_GRA_01_03, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Paragoomba x3.
 */
BattleUnitSetup btlparty_gra_gra_01_03_on_1[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_03_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_03_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_01_03, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Paragoomba x4.
 */
BattleUnitSetup btlparty_gra_gra_01_03_on_2[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_01_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_03_on_2 = {
    4, // numEnemies
    btlparty_gra_gra_01_03_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_01_03, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Paragoomba x2, Crazee Dayzee x2.
 */
BattleUnitSetup btlparty_gra_gra_01_03_on_3[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_01_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_03_on_3 = {
    4, // numEnemies
    btlparty_gra_gra_01_03_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_01_04, in Twilight Trail (gra_01): Hyper Spiky Goomba x3. */
BattleUnitSetup btlparty_gra_gra_01_04_off_1[] = {
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_04_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_GRA_01_04, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Spiky Goomba x3.
 */
BattleUnitSetup btlparty_gra_gra_01_04_on_1[] = {
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_01_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_04_on_1 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_on_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_01_04, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Spiky Goomba x2, Hyper Paragoomba x2.
 */
BattleUnitSetup btlparty_gra_gra_01_04_on_2[] = {
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_01_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_04_on_2 = {
    4, // numEnemies
    btlparty_gra_gra_01_04_on_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_01_04, in Twilight Trail (gra_01) while Mario is Doopliss's
 * shadow: Hyper Spiky Goomba, Hyper Goomba, Crazee Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_01_04_on_3[] = {
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_kuriboo, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_01_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_01_04_on_3 = {
    3, // numEnemies
    btlparty_gra_gra_01_04_on_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_03_01, in Twilight Trail (gra_03): Hyper Paragoomba x2. */
BattleUnitSetup btlparty_gra_gra_03_01_off_1[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_03_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_01_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_03_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_03_01, in Twilight Trail (gra_03): Hyper Paragoomba x2, Crazee
 * Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_03_01_off_2[] = {
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_03_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_01_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_03_04, in Twilight Trail (gra_03): Crazee Dayzee x2. */
BattleUnitSetup btlparty_gra_gra_03_04_off_1[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_03_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_04_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_03_04_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_03_04, in Twilight Trail (gra_03): Crazee Dayzee x2, Amazy
 * Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_03_04_off_2[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_03_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_04_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_04_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_03_04, in Twilight Trail (gra_03): Crazee Dayzee, Hyper Spiky
 * Goomba, Hyper Paragoomba.
 */
BattleUnitSetup btlparty_gra_gra_03_04_off_3[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_03_04: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_04_off_3 = {
    3, // numEnemies
    btlparty_gra_gra_03_04_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_03_05, in Twilight Trail (gra_03): Crazee Dayzee x4. */
BattleUnitSetup btlparty_gra_gra_03_05_off_1[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_03_05: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_05_off_1 = {
    4, // numEnemies
    btlparty_gra_gra_03_05_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_03_05, in Twilight Trail (gra_03): Crazee Dayzee x2, Amazy
 * Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_03_05_off_2[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_03_05: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_05_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_03_05_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_03_05, in Twilight Trail (gra_03): Crazee Dayzee x2, Hyper
 * Paragoomba, Hyper Spiky Goomba.
 */
BattleUnitSetup btlparty_gra_gra_03_05_off_3[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_patakuri, // itemDropTable
    },
    {
        &unit_hyper_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_togekuri, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_03_05: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_03_05_off_3 = {
    4, // numEnemies
    btlparty_gra_gra_03_05_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_GRA_04_02, in Twilight Trail (gra_04): Crazee Dayzee x2, Hyper Cleft.
 */
BattleUnitSetup btlparty_gra_gra_04_02_off_1[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_04_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_04_02_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_04_02_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_GRA_04_02, in Twilight Trail (gra_04): Crazee Dayzee, Hyper Cleft. */
BattleUnitSetup btlparty_gra_gra_04_02_off_2[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_04_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_04_02_off_2 = {
    2, // numEnemies
    btlparty_gra_gra_04_02_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_04_02, in Twilight Trail (gra_04): Crazee Dayzee x2, Amazy
 * Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_04_02_off_3[] = {
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_04_02: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_04_02_off_3 = {
    3, // numEnemies
    btlparty_gra_gra_04_02_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_05_01, in Twilight Trail (gra_05): Hyper Cleft x2. */
BattleUnitSetup btlparty_gra_gra_05_01_off_1[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_05_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_05_01_off_1 = {
    2, // numEnemies
    btlparty_gra_gra_05_01_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_05_01, in Twilight Trail (gra_05): Hyper Cleft x2, Amazy Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_05_01_off_2[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_05_01: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_05_01_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_05_01_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_GRA_05_03, in Twilight Trail (gra_05): Hyper Cleft x3. */
BattleUnitSetup btlparty_gra_gra_05_03_off_1[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_GRA_05_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_05_03_off_1 = {
    3, // numEnemies
    btlparty_gra_gra_05_03_off_1, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_GRA_05_03, in Twilight Trail (gra_05): Hyper Cleft x2, Amazy Dayzee.
 */
BattleUnitSetup btlparty_gra_gra_05_03_off_2[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_GRA_05_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_05_03_off_2 = {
    3, // numEnemies
    btlparty_gra_gra_05_03_off_2, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_GRA_05_03, in Twilight Trail (gra_05): Hyper Cleft x2, Crazee Dayzee
 * x2.
 */
BattleUnitSetup btlparty_gra_gra_05_03_off_3[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_GRA_05_03: the lineup above with the default heart and flower drops. Item drop
 * weights: 25 held, 25 random, 50 none.
 */
BattleGroupSetup btlgrp_gra_gra_05_03_off_3 = {
    4, // numEnemies
    btlparty_gra_gra_05_03_off_3, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_test1_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, gra_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_gra_btlno_gra_test2_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, &gra_btlstage_data[1]},
    {0},
};

/** BTLNO_GRA_00_01 always fights Doopliss disguised as Mario on stage stg_04_0. */
NpcBattleSelect btlgrpsel_gra_btlno_gra_00_01_off[] = {
    {50, &btlgrp_gra_gra_00_01_off_1, gra_btlstage_data},
    {0},
};

/** BTLNO_GRA_00_02 always fights Doopliss disguised as Mario on stage stg_04_0. */
NpcBattleSelect btlgrpsel_gra_btlno_gra_00_02_off[] = {
    {50, &btlgrp_gra_gra_00_01_off_1, gra_btlstage_data},
    {0},
};

/**
 * BTLNO_GRA_01_01 while Mario is Doopliss's shadow picks one of its formations by weight (10:10:10) on stage
 * stg_04_0: Hyper Goomba x3; Hyper Goomba, Hyper Paragoomba, Crazee Dayzee; Hyper Goomba, Hyper Spiky
 * Goomba, Crazee Dayzee.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_on[] = {
    {10, &btlgrp_gra_gra_01_01_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_01_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_01_on_3, gra_btlstage_data},
    {0},
};

/** BTLNO_GRA_01_01 always fights Hyper Goomba x3 on stage stg_04_0. */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_01_off[] = {
    {50, &btlgrp_gra_gra_01_01_off_1, gra_btlstage_data},
    {0},
};

/**
 * BTLNO_GRA_01_03 while Mario is Doopliss's shadow picks one of its formations by weight (10:10:10) on stage
 * stg_04_0: Hyper Paragoomba x3; Hyper Paragoomba x4; Hyper Paragoomba x2, Crazee Dayzee x2.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_on[] = {
    {10, &btlgrp_gra_gra_01_03_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_03_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_03_on_3, gra_btlstage_data},
    {0},
};

/** BTLNO_GRA_01_03 always fights Hyper Paragoomba x3 on stage stg_04_0. */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_03_off[] = {
    {50, &btlgrp_gra_gra_01_03_off_1, gra_btlstage_data},
    {0},
};

/**
 * BTLNO_GRA_01_04 while Mario is Doopliss's shadow picks one of its formations by weight (10:10:10) on stage
 * stg_04_0: Hyper Spiky Goomba x3; Hyper Spiky Goomba x2, Hyper Paragoomba x2; Hyper Spiky Goomba, Hyper
 * Goomba, Crazee Dayzee.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_on[] = {
    {10, &btlgrp_gra_gra_01_04_on_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_04_on_2, gra_btlstage_data},
    {10, &btlgrp_gra_gra_01_04_on_3, gra_btlstage_data},
    {0},
};

/** BTLNO_GRA_01_04 always fights Hyper Spiky Goomba x3 on stage stg_04_0. */
NpcBattleSelect btlgrpsel_gra_btlno_gra_01_04_off[] = {
    {50, &btlgrp_gra_gra_01_04_off_1, gra_btlstage_data},
    {0},
};

/**
 * BTLNO_GRA_03_01 picks one of its formations by weight (10:10) on stage stg_04_0: Hyper Paragoomba x2;
 * Hyper Paragoomba x2, Crazee Dayzee.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_01_off[] = {
    {10, &btlgrp_gra_gra_03_01_off_1, gra_btlstage_data},
    {10, &btlgrp_gra_gra_03_01_off_2, gra_btlstage_data},
    {0},
};

/**
 * BTLNO_GRA_03_04 picks one of its formations by weight (50:10:40) on stage stg_04_1: Crazee Dayzee x2;
 * Crazee Dayzee x2, Amazy Dayzee; Crazee Dayzee, Hyper Spiky Goomba, Hyper Paragoomba.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_04_off[] = {
    {50, &btlgrp_gra_gra_03_04_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_03_04_off_2, &gra_btlstage_data[1]},
    {40, &btlgrp_gra_gra_03_04_off_3, &gra_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_GRA_03_05 picks one of its formations by weight (30:10:30) on stage stg_04_1: Crazee Dayzee x4;
 * Crazee Dayzee x2, Amazy Dayzee; Crazee Dayzee x2, Hyper Paragoomba, Hyper Spiky Goomba.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_03_05_off[] = {
    {30, &btlgrp_gra_gra_03_05_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_03_05_off_2, &gra_btlstage_data[1]},
    {30, &btlgrp_gra_gra_03_05_off_3, &gra_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_GRA_04_02 picks one of its formations by weight (60:30:10) on stage stg_04_1: Crazee Dayzee x2,
 * Hyper Cleft; Crazee Dayzee, Hyper Cleft; Crazee Dayzee x2, Amazy Dayzee.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_04_02_off[] = {
    {60, &btlgrp_gra_gra_04_02_off_1, &gra_btlstage_data[1]},
    {30, &btlgrp_gra_gra_04_02_off_2, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_04_02_off_3, &gra_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_GRA_05_01 picks one of its formations by weight (100:10) on stage stg_04_1: Hyper Cleft x2; Hyper
 * Cleft x2, Amazy Dayzee.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_05_01_off[] = {
    {100, &btlgrp_gra_gra_05_01_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_05_01_off_2, &gra_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_GRA_05_03 picks one of its formations by weight (50:10:40) on stage stg_04_1: Hyper Cleft x3; Hyper
 * Cleft x2, Amazy Dayzee; Hyper Cleft x2, Crazee Dayzee x2.
 */
NpcBattleSelect btlgrpsel_gra_btlno_gra_05_03_off[] = {
    {50, &btlgrp_gra_gra_05_03_off_1, &gra_btlstage_data[1]},
    {10, &btlgrp_gra_gra_05_03_off_2, &gra_btlstage_data[1]},
    {40, &btlgrp_gra_gra_05_03_off_3, &gra_btlstage_data[1]},
    {0},
};

NpcSetupBattleInfo btlsetup_gra_tbl[] = {
    {
        "マップstg_04_0", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_04_1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA00ランペルその１", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_00_01_off, // select
        NPC_BATTLE_STORY, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        "GRA00ランペルその２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_00_02_off, // select
        NPC_BATTLE_STORY, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG4_RUNPELL1", // bgmName
    },
    {
        "GRA01ハイパークリボー", // name
        NULL, 0, // mapName..fixedItem
        GSWF_JIN_TRADING_PLACES_ACTIVE, // swFlag
        btlgrpsel_gra_btlno_gra_01_01_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA01ハイパーパタクリ", // name
        NULL, 0, // mapName..fixedItem
        GSWF_JIN_TRADING_PLACES_ACTIVE, // swFlag
        btlgrpsel_gra_btlno_gra_01_03_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_03_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA01ハイパートゲクリ", // name
        NULL, 0, // mapName..fixedItem
        GSWF_JIN_TRADING_PLACES_ACTIVE, // swFlag
        btlgrpsel_gra_btlno_gra_01_04_on, // altSelect
        btlgrpsel_gra_btlno_gra_01_04_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA03ハイパークリボー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA03パンジーさん", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_04_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA03パンジーさん", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_03_05_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA04パンジーさん", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_04_02_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA05ハイパーシンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_05_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "GRA05ハイパーシンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_gra_btlno_gra_05_03_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
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

