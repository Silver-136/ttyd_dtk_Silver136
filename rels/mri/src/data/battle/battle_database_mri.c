// The Great Tree (mri): battle database: the enemy groups (BattleSetup) and stages of the tree's rooms
// and of the Magnus von Grapple boss fight.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "mri.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"
#include "evt/evt_map.h"
#include "driver/mapdrv.h"

extern BattleUnitKind unit_barriern;

BattleStageObjectData mri_btlobject_data[43];
BattleStageData mri_btlstage_data[4];
BattleUnitSetup btlparty_mri_mri_01_01[2];
BattleGroupSetup btlgrp_mri_mri_01_01;
BattleUnitSetup btlparty_mri_mri_04_01[1];
BattleGroupSetup btlgrp_mri_mri_04_01;
BattleUnitSetup btlparty_mri_mri_05_01[2];
BattleGroupSetup btlgrp_mri_mri_05_01;
BattleUnitSetup btlparty_mri_mri_06_2F_02[3];
BattleGroupSetup btlgrp_mri_mri_06_2F_02;
BattleUnitSetup btlparty_mri_mri_06_2F_03[4];
BattleGroupSetup btlgrp_mri_mri_06_2F_03;
BattleUnitSetup btlparty_mri_mri_12_01[2];
BattleGroupSetup btlgrp_mri_mri_12_01;
BattleUnitSetup btlparty_mri_mri_12_02[4];
BattleGroupSetup btlgrp_mri_mri_12_02;
BattleUnitSetup btlparty_mri_mri_15_01[2];
BattleGroupSetup btlgrp_mri_mri_15_01;
BattleUnitSetup btlparty_mri_mri_17_01[2];
BattleGroupSetup btlgrp_mri_mri_17_01;
BattleUnitSetup btlparty_mri_mri_18_02[3];
BattleGroupSetup btlgrp_mri_mri_18_02;
BattleUnitSetup btlparty_mri_mri_18_03[3];
BattleGroupSetup btlgrp_mri_mri_18_03;
BattleUnitSetup btlparty_mri_mri_19_01[2];
BattleGroupSetup btlgrp_mri_mri_19_01;
BattleUnitSetup btlparty_mri_mri_19_02[2];
BattleGroupSetup btlgrp_mri_mri_19_02;
BattleUnitSetup btlparty_mri_mri_19_04[4];
BattleGroupSetup btlgrp_mri_mri_19_04;
BattleUnitSetup btlparty_mri_mri_20_02[2];
BattleGroupSetup btlgrp_mri_mri_20_02;
BattleUnitSetup btlparty_mri_mri_20_03[3];
BattleGroupSetup btlgrp_mri_mri_20_03;
BattleUnitSetup btlparty_mri_mri_mb[1];
BattleGroupSetup btlgrp_mri_mri_mb;
NpcBattleSelect btlgrpsel_mri_btlno_mri_test1_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_test2_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_test3_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_test4_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_01_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_04_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_05_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_06_2f_02_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_06_2f_03_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_12_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_12_02_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_15_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_17_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_18_02_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_18_03_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_19_01_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_19_02_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_19_04_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_20_02_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_20_03_off[2];
NpcBattleSelect btlgrpsel_mri_btlno_mri_mb_off[2];
NpcSetupBattleInfo btlsetup_mri_tbl[22];
NpcTerritoryInfo btl_mri_setup_no_tbl[22];
EvtWord stg_02_1_init_evt[8];

/** Scenery pieces of the Great Tree battle stages; mri_btlstage_data entries point into this array. */
BattleStageObjectData mri_btlobject_data[] = {
    {"S_isk_01", 2, 0, {19.0f, 0.0f, -55.0f}, 0, 60, {0}},
    {"S_ueki_01", 2, 1, {-188.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_ueki_02", 2, 1, {-147.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"S_ueki_03", 2, 1, {-56.0f, 0.0f, -36.0f}, 0, 60, {0}},
    {"S_ueki_04", 2, 1, {38.0f, 0.0f, -28.0f}, 0, 60, {0}},
    {"S_ueki_05", 2, 1, {157.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"S_ueki_06", 2, 1, {198.0f, 0.0f, 57.0f}, 0, 60, {0}},
    {"S_ueki_07", 2, 1, {-204.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"S_kusa_01", 2, 1, {-1.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"S_isk_02", 2, 0, {25.0f, 0.0f, -55.0f}, 0, 60, {0}},
    {"S_ueki_01", 2, 1, {-188.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_ueki_02", 2, 1, {-147.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"S_ueki_03", 2, 1, {-56.0f, 0.0f, -36.0f}, 0, 60, {0}},
    {"S_ueki_04", 2, 1, {38.0f, 0.0f, -28.0f}, 0, 60, {0}},
    {"S_ueki_05", 2, 1, {157.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"S_ueki_06", 2, 1, {198.0f, 0.0f, 57.0f}, 0, 60, {0}},
    {"S_ueki_07", 2, 1, {-204.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"S_kusa_01", 2, 1, {-1.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"S_isk_03", 2, 0, {12.0f, 0.0f, -55.0f}, 0, 60, {0}},
    {"S_ueki_01", 2, 1, {-188.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_ueki_02", 2, 1, {-147.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"S_ueki_03", 2, 1, {-56.0f, 0.0f, -36.0f}, 0, 60, {0}},
    {"S_ueki_04", 2, 1, {38.0f, 0.0f, -28.0f}, 0, 60, {0}},
    {"S_ueki_05", 2, 1, {157.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"S_ueki_06", 2, 1, {198.0f, 0.0f, 57.0f}, 0, 60, {0}},
    {"S_ueki_07", 2, 1, {-204.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"S_kusa_01", 2, 1, {-1.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
    {"S_ueki_01", 2, 1, {-188.0f, 0.0f, 48.0f}, 0, 60, {0}},
    {"S_ueki_02", 2, 1, {-147.0f, 0.0f, -52.0f}, 0, 60, {0}},
    {"S_ueki_03", 2, 1, {-56.0f, 0.0f, -46.0f}, 0, 60, {0}},
    {"S_ueki_04", 2, 1, {38.0f, 0.0f, -57.0f}, 0, 60, {0}},
    {"S_ueki_05", 2, 1, {157.0f, 0.0f, -50.0f}, 0, 60, {0}},
    {"S_ueki_06", 2, 1, {198.0f, 0.0f, 57.0f}, 0, 60, {0}},
    {"S_ueki_07", 2, 1, {-204.0f, 0.0f, -64.0f}, 0, 60, {0}},
    {"S_kusa_01", 2, 1, {-1.0f, 0.0f, 76.0f}, 0, 60, {0}},
    {"B", 2, 2, {0}, 0, 60, {0}},
    {"C", 2, 3, {0}, 0, 60, {0}},
};

/** Battle stages of the Great Tree: backdrop folders, scenery pieces and background scripts. */
BattleStageData mri_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_02_1", // specificStageDataDir
        11, // numObjects
        mri_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_02_1_init_evt, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        0, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_02_1", // specificStageDataDir
        11, // numObjects
        &mri_btlobject_data[11], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_02_1_init_evt, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        0, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_02_1", // specificStageDataDir
        11, // numObjects
        &mri_btlobject_data[22], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_02_1_init_evt, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        0, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_02_1", // specificStageDataDir
        10, // numObjects
        &mri_btlobject_data[33], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        stg_02_1_init_evt, // initEvt
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // destroyBgA1Evt..reserved_1B0
        1, // disableBgA1Destroy
        0, // disableBgA2Destroy
        1, // disableBgBDestroy
    },
};

BattleUnitSetup btlparty_mri_mri_01_01[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_01_01 = {
    2, // numEnemies
    btlparty_mri_mri_01_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_04_01[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_04_01 = {
    1, // numEnemies
    btlparty_mri_mri_04_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_05_01[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_05_01 = {
    2, // numEnemies
    btlparty_mri_mri_05_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_06_2F_02[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_06_2F_02 = {
    3, // numEnemies
    btlparty_mri_mri_06_2F_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_06_2F_03[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_06_2F_03 = {
    4, // numEnemies
    btlparty_mri_mri_06_2F_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_12_01[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_12_01 = {
    2, // numEnemies
    btlparty_mri_mri_12_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_12_02[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_12_02 = {
    4, // numEnemies
    btlparty_mri_mri_12_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_15_01[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_15_01 = {
    2, // numEnemies
    btlparty_mri_mri_15_01, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_17_01[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_17_01 = {
    2, // numEnemies
    btlparty_mri_mri_17_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_18_02[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_18_02 = {
    3, // numEnemies
    btlparty_mri_mri_18_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_18_03[] = {
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {50.0f, 40.0f, -10.0f},
#else
        {0.0f, 40.0f, -20.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {90.0f, 40.0f, 0.0f},
#else
        {75.0f, 40.0f, 0.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {130.0f, 40.0f, 10.0f},
#else
        {150.0f, 40.0f, 20.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_18_03 = {
    3, // numEnemies
    btlparty_mri_mri_18_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_19_01[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_19_01 = {
    2, // numEnemies
    btlparty_mri_mri_19_01, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_19_02[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_19_02 = {
    2, // numEnemies
    btlparty_mri_mri_19_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_19_04[] = {
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_monochrome_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome__pakkun, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_19_04 = {
    4, // numEnemies
    btlparty_mri_mri_19_04, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_20_02[] = {
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_20_02 = {
    2, // numEnemies
    btlparty_mri_mri_20_02, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_20_03[] = {
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
#if VERSION <= VERSION_DEMO_JP
        {50.0f, 40.0f, -10.0f},
#else
        {10.0f, 40.0f, -20.0f}, // position
#endif
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_barriern, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_barriern, // itemDropTable
    },
    {
        &unit_gundan_zako, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_gundan_zako, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_20_03 = {
    3, // numEnemies
    btlparty_mri_mri_20_03, // enemyData
    25, // heldItemWeight
    25, // randomItemWeight
    50, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

BattleUnitSetup btlparty_mri_mri_mb[] = {
    {
        &unit_boss_magnum_battender, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_magnum_battender, // itemDropTable
    },
};

BattleGroupSetup btlgrp_mri_mri_mb = {
    1, // numEnemies
    btlparty_mri_mri_mb, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_test1_off[] = {
    {50, &btlgrp_mri_mri_01_01, mri_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_test2_off[] = {
    {50, &btlgrp_mri_mri_01_01, &mri_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_test3_off[] = {
    {50, &btlgrp_mri_mri_01_01, &mri_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_test4_off[] = {
    {50, &btlgrp_mri_mri_01_01, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_01_01_off[] = {
    {10, &btlgrp_mri_mri_01_01, mri_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_04_01_off[] = {
    {10, &btlgrp_mri_mri_04_01, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_05_01_off[] = {
    {10, &btlgrp_mri_mri_05_01, &mri_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_06_2f_02_off[] = {
    {10, &btlgrp_mri_mri_06_2F_02, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_06_2f_03_off[] = {
    {10, &btlgrp_mri_mri_06_2F_03, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_12_01_off[] = {
    {10, &btlgrp_mri_mri_12_01, &mri_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_12_02_off[] = {
    {10, &btlgrp_mri_mri_12_02, &mri_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_15_01_off[] = {
    {10, &btlgrp_mri_mri_15_01, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_17_01_off[] = {
    {10, &btlgrp_mri_mri_17_01, &mri_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_18_02_off[] = {
    {10, &btlgrp_mri_mri_18_02, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_18_03_off[] = {
    {10, &btlgrp_mri_mri_18_03, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_19_01_off[] = {
    {10, &btlgrp_mri_mri_19_01, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_19_02_off[] = {
    {10, &btlgrp_mri_mri_19_02, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_19_04_off[] = {
    {10, &btlgrp_mri_mri_19_04, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_20_02_off[] = {
    {10, &btlgrp_mri_mri_20_02, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_20_03_off[] = {
    {10, &btlgrp_mri_mri_20_03, &mri_btlstage_data[3]},
    {0},
};

NpcBattleSelect btlgrpsel_mri_btlno_mri_mb_off[] = {
    {10, &btlgrp_mri_mri_mb, mri_btlstage_data},
    {0},
};

/**
 * Encounter settings of every Great Tree map: which battle choice its enemies use, the battle music and the
 * random audience rolls.
 */
NpcSetupBattleInfo btlsetup_mri_tbl[] = {
    {
        "マップstg_02_1_1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_test1_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_02_1_2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_test2_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_02_1_3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_test3_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マップstg_02_1_3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_test4_off, // select
        0, 0, // battleFlags..randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri01ぐんだんザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_01_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "mri04ぐんだんザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_04_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "mri05ぐんだんザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_05_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri06パイダース2F_２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_06_2f_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri06パイダース2F_３", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_06_2f_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri12パックンフラワー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_12_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri12パックンフラワー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_12_02_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri15ぐんだんザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_15_01_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        0, // randomMode
        {{4, 4}, {10, 10}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_KOBOSS_BATTLE1", // bgmName
    },
    {
        "mri17パイダース", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_17_01_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {4, 4}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri18軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_18_02_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {10, 10}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri18バリアーン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_18_03_off, // select
        0, 0, // battleFlags..randomMode
        {{4, 4}, {10, 10}, {0}, {0}, {4, 4}, {4, 4}, {0}, {10, 10}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri19パイダース", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_19_01_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri19パックン１", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_19_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri19パックン３", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_19_04_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri20軍団ザコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_20_02_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "mri20バリアーン２", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_20_03_off, // select
        0, 0, // battleFlags..randomMode
        {{5, 5}, {10, 10}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {5, 5}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マグナムバッテンダー", // name
        "mri_01", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_mri_btlno_mri_mb_off, // select
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_STORY), // battleFlags
        2, // randomMode
        {{0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {10, 10}, {0}, {0}, {0}, {0}, {0}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG3_3RDLEADER1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_mri_setup_no_tbl[] = {
    {"BTLNO_MRI_TEST1", 0},
    {"BTLNO_MRI_TEST2", 1},
    {"BTLNO_MRI_TEST3", 2},
    {"BTLNO_MRI_TEST4", 3},
    {"BTLNO_MRI_01_01", 4},
    {"BTLNO_MRI_04_01", 5},
    {"BTLNO_MRI_05_01", 6},
    {"BTLNO_MRI_06_2F_02", 7},
    {"BTLNO_MRI_06_2F_03", 8},
    {"BTLNO_MRI_12_01", 9},
    {"BTLNO_MRI_12_02", 10},
    {"BTLNO_MRI_15_01", 11},
    {"BTLNO_MRI_17_01", 12},
    {"BTLNO_MRI_18_02", 13},
    {"BTLNO_MRI_18_03", 14},
    {"BTLNO_MRI_19_01", 15},
    {"BTLNO_MRI_19_02", 16},
    {"BTLNO_MRI_19_04", 17},
    {"BTLNO_MRI_20_02", 18},
    {"BTLNO_MRI_20_03", 19},
    {"BTLNO_MRI_MB", 20},
    {0},
};

/**
 * Stage init script for the battle stage stg_02_1 (the Great Tree's Puni village):
 * turns on the planter model group (S_ueki) so it is visible in the battle backdrop.
 */
EVT_BEGIN(stg_02_1_init_evt)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ueki"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

